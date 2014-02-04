#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>          //for shmem
#include <sys/ipc.h>            //for shmem
#include <sys/shm.h>            //for shmem
#include <sys/stat.h>
#include <fcntl.h>              //for shmem
#include <errno.h>              //for errno
#include "gui.h"
#include "gui_local.h"
#include "x_dbg.h"
#include "ipc.h"

/////////////////// Externs //////////////////////////////
extern GUI_CONTEXT_T gCtxt;

/////////////////// Globals //////////////////////////////
static UINT     gSeqNum = 1;

/////////////////// Request Functions/////////////////////
static UINT
SendShortRequest (GUI_REQUEST_T * pRequest, void *pReqData, UINT ReqSize)
{
    int             ret;
    REQUEST_MSG_T  *pMsg;
    GUI_REQUEST_T   Cleared = { 0, 0, 0, {0, 0} };

    //Sending IPC Message
    pMsg = (REQUEST_MSG_T *) calloc (ReqSize + REQMSG_HDR_SIZE, 1);
    if (NULL == pMsg) {
        X_PRINTF (DBG_ERR,"ShortRequest(): calloc failed\n", "");
        (*pRequest) = Cleared;
        return GUI_ERR_NO_MEMORY;
    }

    pMsg->Type      = MSG_TYPE_SHORT_REQUEST;
    pMsg->SeqNum    = pRequest->SeqNum;
    pMsg->RequestID = pRequest->RequestID;
    pMsg->Size      = ReqSize;
    if (ReqSize != 0) {
        memcpy (pMsg->pData, pReqData, ReqSize);
    }

    ret = SendIpcMessage ((char *) pMsg, (ReqSize + REQMSG_HDR_SIZE),
                          TSKID_FROM_REQID (pRequest->RequestID));
    if (ret != IPC_SUCCESS) {
        X_PRINTF (DBG_ERR,"ShortRequest(): SendIpcMessage failed %d\n", ret);
        (*pRequest) = Cleared;
        free (pMsg);
        return GUI_ERR_IPC_SEND;
    }

    free (pMsg);

    return ret;
}

static UINT
SendLongRequest (GUI_REQUEST_T * pRequest, void *pReqData, UINT ReqSize)
{
    int             shm_id;
    void           *shm_ptr;
    int             ret;
    REQUEST_MSG_T  *pMsg;
    GUI_REQUEST_T   Cleared = { 0, 0, 0, {0, 0} };

    //Copy message to shared memory
    shm_id = shmget ((key_t) pRequest->SeqNum, ReqSize, S_IRWXU | IPC_CREAT);
    if (shm_id == -1) {
        X_PRINTF (DBG_ERR,"SendLongRequest: shmget failed errno=%d\n",errno);
        (*pRequest) = Cleared;
        return GUI_ERR_SHM_ALLOCATE;
    }

    shm_ptr = shmat (shm_id, NULL, 0);
    if (((int) shm_ptr) == -1) {
        X_PRINTF (DBG_ERR,"SendLongRequest: shmat failed errno=%d\n",errno);
        (*pRequest) = Cleared;
        return GUI_ERR_SHM_ATTACH;
    }

    memcpy (shm_ptr, pReqData, ReqSize);

    ret = shmdt (shm_ptr);
    if (ret == -1) {
        X_PRINTF (DBG_ERR,"SendLongRequest: shmdt failed errno=%d\n",errno);
        (*pRequest) = Cleared;
        return GUI_ERR_SHM_DETACH;
    }
    //Sending IPC Message
    pMsg = (REQUEST_MSG_T *) calloc (REQMSG_HDR_SIZE, 1);
    if (NULL == pMsg) {
        X_PRINTF (DBG_ERR,"SendLongRequest(): calloc failed\n", "");
        (*pRequest) = Cleared;
        return GUI_ERR_NO_MEMORY;
    }

    pMsg->Type      = MSG_TYPE_LONG_REQUEST;
    pMsg->SeqNum    = pRequest->SeqNum;
    pMsg->RequestID = pRequest->RequestID;
    pMsg->Size      = ReqSize;

    ret = SendIpcMessage ((char *) pMsg, (REQMSG_HDR_SIZE),
                          TSKID_FROM_REQID (pRequest->RequestID));
    if (ret != IPC_SUCCESS) {
        X_PRINTF (DBG_ERR,"SendLongRequest: SendIpcMessage failed %d\n", ret);
        (*pRequest) = Cleared;
        free (pMsg);
        return GUI_ERR_IPC_SEND;
    }

    free (pMsg);

    return ret;
}

UINT
Request (USHORT RequestID, void *pReqData, UINT ReqSize, ULONG TimeOut,
         RESPONSE_CALLBACK_T response_callback)
{
    int             idx;
    GUI_REQUEST_T   *pRequest;
    struct timeval  interval;
    struct timeval  now;

    if ((pReqData == NULL) && (ReqSize != 0)) {
        return GUI_ERR_PARAMETER;
    }

    if (INVALID_REQUESTID == RequestID) {
        return GUI_ERR_REQUESTID_INVALID;
    }

    for (idx = 0; idx < GUI_MAX_REQUESTS; idx++) {
        if (INVALID_REQUESTID == gCtxt.requests[idx].RequestID) {
            pRequest = &(gCtxt.requests[idx]);
            break;
        }
    }

    if (idx == GUI_MAX_REQUESTS) {
        return GUI_ERR_REQUESTS_FULL;
    }
    //Assigning Local Context
    pRequest->SeqNum    = gSeqNum++;
    pRequest->RequestID = RequestID;
    pRequest->pRespCb   = response_callback;

    gettimeofday (&now, NULL);
    interval.tv_sec  = TimeOut / 1000;
    interval.tv_usec = (TimeOut % 1000) * 1000;
    timeradd(&now,&interval,&(pRequest->expires));

    if ((ReqSize + REQMSG_HDR_SIZE) <= MAX_MESSAGE_SIZE) {
        return SendShortRequest (pRequest, pReqData, ReqSize);
    } else {
        return SendLongRequest (pRequest, pReqData, ReqSize);
    }

}

void
ClearRequests ()
{
    int             idx;
    GUI_REQUEST_T   Cleared = { 0, 0, 0, {0, 0} };

    for (idx = 0; idx < GUI_MAX_REQUESTS; idx++) {
        gCtxt.requests[idx] = Cleared;;
    }
}

/////////////////// Response Functions/////////////////////
void
ProcessResponseTimeouts ()
{
    int                 i;
    RESPONSE_CALLBACK_T pRespCb;
    struct timeval      now;
    GUI_REQUEST_T       Cleared = { 0, 0, 0, {0, 0} };

    gettimeofday (&now, NULL);

    for (i = 0; i < GUI_MAX_REQUESTS; i++) {
        if (INVALID_REQUESTID != gCtxt.requests[i].RequestID) {
            if (TRUE == IsTimeOver (&now, &(gCtxt.requests[i].expires))) {
                pRespCb = gCtxt.requests[i].pRespCb;
                gCtxt.requests[i] = Cleared;
                if (NULL != pRespCb) {
                    pRespCb (NULL, 0, GUI_ERR_RESP_TIMEOUT);
                }
            }
        }
    } //for
}

void
ProcessShortResponses (USHORT SeqNum, void *pData, UINT size)
{
    int                 i;
    RESPONSE_CALLBACK_T pRespCb;
    GUI_REQUEST_T       Cleared = { 0, 0, 0, {0, 0} };

    for (i = 0; i < GUI_MAX_REQUESTS; i++) {
        if (SeqNum == gCtxt.requests[i].SeqNum) {
            pRespCb = gCtxt.requests[i].pRespCb;
            gCtxt.requests[i] = Cleared;
            if (NULL != pRespCb) {
                pRespCb (pData, size, GUI_SUCCESS);
                break;
            } //if
        } //if
    } //for
}

UINT
ProcessLongResponses (USHORT SeqNum, UINT size)
{
    int                 i;
    int                 ret;
    int                 shm_id;
    void                *shm_ptr;
    RESPONSE_CALLBACK_T pRespCb;
    GUI_REQUEST_T       Cleared = { 0, 0, 0, {0, 0} };

    shm_id = shmget ((key_t) SeqNum, size, S_IRWXU);
    if (shm_id == -1) {
        X_PRINTF (DBG_ERR,"ProcessLongResponses: shmget failed errno=%d\n",errno);
        return GUI_ERR_SHM_ALLOCATE;
    }

    shm_ptr = shmat (shm_id, NULL, 0);
    if (((int) shm_ptr) == -1) {
        X_PRINTF (DBG_ERR,"ProcessLongResponses: shmat failed errno=%d\n",errno);
        return GUI_ERR_SHM_ATTACH;
    }

    ret = shmctl (shm_id, IPC_RMID, NULL);
    if (ret == -1) {
        X_PRINTF (DBG_ERR,"ProcessLongResponses: shmctl failed errno=%d\n",errno);
        return GUI_ERR_SHM_REMOVE;
    }

    for (i = 0; i < GUI_MAX_REQUESTS; i++) {
        if (SeqNum == gCtxt.requests[i].SeqNum) {
            pRespCb = gCtxt.requests[i].pRespCb;
            gCtxt.requests[i] = Cleared;
            if (NULL != pRespCb) {
                pRespCb (shm_ptr, size, GUI_SUCCESS);
                break;
            } //if
        } //if
    } //for

    ret = shmdt (shm_ptr);
    if (ret == -1) {
        X_PRINTF (DBG_ERR,"ProcessLongResponses: shmdt failed errno=%d\n",errno);
    }

    return GUI_SUCCESS;
}

