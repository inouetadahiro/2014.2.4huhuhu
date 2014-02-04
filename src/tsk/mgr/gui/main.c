#include <stdio.h>
#include <stdlib.h>             //for exit
#include <mqueue.h>
#include <sys/select.h>         //for select
#include <unistd.h>             //for read()
#include <errno.h>              //for errno
#include <linux/input.h>       //for input_event
#include "gui.h"
#include "ipc.h"
#include "gui_local.h"
#include "x_dbg.h"
#include "win_tbl.h"

/////////////////// Defines //////////////////////////////

#define INPUT_DEV "/dev/input/event0"

/////////////////// Globals //////////////////////////////
GUI_CONTEXT_T   gCtxt;
USHORT          gCurrentTaskID = TSKID_MGR;

/////////////////// Externs //////////////////////////////
extern WINTBL_T gWindowTbl[];
extern mqd_t    gMyQ;

///////////////////Utility Functions //////////////////////////
int
IsTimeOver (struct timeval *now, struct timeval *expires)
{
    if (now->tv_sec > expires->tv_sec) {
        return TRUE;
    }

    if (now->tv_sec == expires->tv_sec) {
        if (now->tv_usec >= expires->tv_usec) {
            return TRUE;
        }
    }

    return FALSE;
}

int
CompareAreas (GUI_AREA_T area1, GUI_AREA_T area2)
{
    if ((area1.x == area2.x) && (area1.y == area2.y) && (area1.w == area2.w)
        && (area1.h == area2.h)) {
        return TRUE;
    }

    return FALSE;
}

int CompareStates (GUI_STATE_T state1, GUI_STATE_T state2)
{
    if ((state1.type == state2.type) && (state1.time == state2.time)) {

        return TRUE;
    }

    return FALSE;
}

/////////////////// Window Switch //////////////////////////////
void
LoadWindow (USHORT win_id)
{
    int             i;
    USHORT          current_win_id;

    if (win_id == WINID_INVALID) {
        return;
    }

    ClearTimers ();
    ClearRequests ();
    ClearNotifies ();
    ClearTouchAreas ();
    ClearKeyStates ();

    for (i = 0; i < WINID_MAXIMUM; i++) {
        if (gWindowTbl[i].win_id == win_id) {
            current_win_id    = gCtxt.current_win.win_id;
            gCtxt.current_win = gWindowTbl[i];
            (gCtxt.current_win.win_obj)->OnLoad (current_win_id);
            break;
        }
    }

    if (i == WINID_MAXIMUM) {
        //TODO: Add sys_NG error screen and display error
        X_PRINTF (DBG_CRIT,"LoadWindow: win_id=%d not found!\n", win_id);
        exit (-1);
    }
}
////////////////////////////////////////////////////////////////////////
static void
ResponseToConsole(void *pRespData, UINT RespSize, UINT GuiErr)
{
	int i;

    printf("$Response %d %d", GuiErr/*hReq*/, RespSize);
    for (i = 0; i < RespSize; i++)
    	printf(" %d", ((unsigned char *) pRespData)[i]);
    printf("\n");

//    return IPC_SUCCESS;
}

///////////////////Handle IPC messages received/////////////////////////
static void
ProcessIpcMessages ()
{

    NOTIFY_MSG_T   *pNtfyMsg;
    REQUEST_MSG_T  *pReqMsg;
    RESPONSE_MSG_T *pRespMsg;
    IPC_MSG_T      *pIpcMsg;
    char            IpcBuf[MAX_MESSAGE_SIZE + 1];
    USHORT          RecvLen = 0;
    int             ret;

    ret = RecvIpcMessage (IpcBuf, (MAX_MESSAGE_SIZE + 1), 1000, &RecvLen);
    if (ret != IPC_SUCCESS) {
        X_PRINTF (DBG_ERR,"Gui: RecvIpcMessage failed,ret=%d!\n", ret);
        return;
    }

    X_PRINTF (DBG_DEBUG,"Message Received, RecvLen=%d\n",RecvLen);

    pIpcMsg = (IPC_MSG_T *) IpcBuf;

    switch (pIpcMsg->Type) {

    case MSG_TYPE_SHORT_REQUEST:
        pReqMsg = (REQUEST_MSG_T *) IpcBuf;
        if (RecvLen != ((pReqMsg->Size) + REQMSG_HDR_SIZE)) {
            X_PRINTF (DBG_ERR,"Invalid Request length RecvLen=%d!\n",RecvLen);
            break;
        }
        ret = Request(pReqMsg->RequestID, pReqMsg->pData, pReqMsg->Size, 60*60*1000,ResponseToConsole);
        if (ret != IPC_SUCCESS)
        {
            X_PRINTF(DBG_ERR, "!!!Request Faild!!! ret = %d\n", ret);
        }
        break;

    case MSG_TYPE_SHORT_NOTIFY:
        pNtfyMsg = (NOTIFY_MSG_T *) IpcBuf;
        if (RecvLen != ((pNtfyMsg->Size) + NTFYMSG_HDR_SIZE)) {
            X_PRINTF (DBG_ERR,"Invalid Notify length RecvLen=%d!\n",RecvLen);
            break;
        }
        ProcessNotifies (pNtfyMsg->NotifyID, pNtfyMsg->pData, pNtfyMsg->Size);
        break;

    case MSG_TYPE_SHORT_RESPONSE:
        pRespMsg = (RESPONSE_MSG_T *) IpcBuf;
        if (RecvLen != ((pRespMsg->Size) + RESPMSG_HDR_SIZE)) {
            X_PRINTF (DBG_ERR,"Invalid Response length RecvLen=%d!\n",RecvLen);
            break;
        }
        ProcessShortResponses (pRespMsg->SeqNum, pRespMsg->pData, pRespMsg->Size);
        break;

    case MSG_TYPE_LONG_RESPONSE:
        pRespMsg = (RESPONSE_MSG_T *) IpcBuf;
        if (RecvLen != (RESPMSG_HDR_SIZE)) {
            X_PRINTF (DBG_ERR,"Invalid Response length RecvLen=%d!\n",RecvLen);
            break;
        }
        ret = ProcessLongResponses (pRespMsg->SeqNum, pRespMsg->Size);
        if (ret != GUI_SUCCESS) {
            X_PRINTF (DBG_ERR,"ProcessLongResponses failed %d!\n", ret);
        }
        break;

    default:
        X_PRINTF (DBG_ERR,"ProcessIpcMessages: Unknown message type %d\n",
                    pIpcMsg->Type);
        break;
    } //switch
}

/////////////////// MGR Task entry point//////////////////////////////
int
main (int argc, char *argv[])
{
    int             ret;
    fd_set          read_set;
    int             nfds;
    struct timeval  tv;
    struct input_event data;

    if (InitializeIpcFramework (TSKID_MGR) != IPC_SUCCESS) {
        printf("can not init!!!\n");
        return -1;
    }

    if ((gCtxt.input_fd = open(INPUT_DEV, O_RDONLY)) < 0){
        printf("can not open!!!\n");
        return -1;
    }

    LoadWindow (gWindowTbl[0].win_id);

    while (1) {

        FD_ZERO (&read_set);
        FD_SET (gCtxt.input_fd, &read_set);
        FD_SET (gMyQ, &read_set);
        nfds = gCtxt.input_fd > gMyQ ? gCtxt.input_fd : gMyQ;
        nfds++;        //nfdsは最大のディスクリプタ+1
        tv.tv_sec = 0;
        tv.tv_usec = 1000; //1millisec
        ret = select (nfds, &read_set, NULL, NULL, &tv);
        switch (ret) {
        case -1:
            X_PRINTF (DBG_ERR,"select failed errno=%d",errno);
            break;
        case 0:
            //select time out
            break;
        default:
            if (FD_ISSET (gCtxt.input_fd, &read_set)) {
                //X_PRINTF (DBG_DEBUG,"input detected\n","");

            	ret = read (gCtxt.input_fd, &data, sizeof(data));
                //X_PRINTF(DBG_DEBUG, "%d.%06d ", data.time.tv_sec, data.time.tv_usec);
                //X_PRINTF(DBG_DEBUG, "%d %d %d\n", data.type, data.code, data.value);
                switch (data.type) {
                case EV_KEY:
                    ProcessKeyEvents ( &data );
                    break;
                case EV_ABS:
                    ProcessTouchEvents ( &data );
                    break;
                }
            }
            if (FD_ISSET (gMyQ, &read_set)) {
                X_PRINTF (DBG_DEBUG,"IPC message detected\n","");
                ProcessIpcMessages ();
            }
            break;
        }

        ProcessTimers ();
        ProcessResponseTimeouts ();
    } //while (1)

    return 0;
}
