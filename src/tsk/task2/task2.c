#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "datatypes.h"
#include "ipc.h"
#include "notify_id.h"
#include "tsk_id.h"
#include "req_id.h"
#include "x_dbg.h"
#include "ipc_command.h"


//---------------------------------------------------
// macro

//#define DEBUG

#ifdef DEBUG

#define Response(x,y,z)				(Response(x,y,z),FakeResponse(x,y,z))	//
#define Notify(x,y,z)				(Notify(x,y,z),FakeNotify(x,y,z))	//

#endif

static void sendTask2Notify1(void)
{
	unsigned char buf[2048];
	int ret;

	/* buf set */
	
	ret = Notify(TASK2_NOTIFY1, buf, 32);
    if(ret != IPC_SUCCESS)
	{
		X_PRINTF(DBG_CRIT,"Notify failed, ret=%d\n", ret);
		exit(-1);
	}
}

static
int ProcessRequest(HANDLE hReq, USHORT ReqId, void * IpcBuf, USHORT ReqSize)
{
    int ret, id, i;
	unsigned char buf[2048];

	switch(ReqId)
	{
	case TASK1_REQUEST1:
	    /* buf set */
		ret = Response (hReq, buf, 32);
	    break;
	default:
	    X_PRINTF(DBG_CRIT,"Unknown Request ID %d\n", ReqId);
	    exit(-1);
	    break;
	}
	if(ret != IPC_SUCCESS)
	{
		X_PRINTF(DBG_CRIT,"Response failed, ret=%d\n", ret);
		exit(1);
	}
	return 0;
}

USHORT gCurrentTaskID = TSKID_TASK2;

int
main ()
{
    UINT            ret;
    int             quit = 0;

    ret = InitializeIpcFramework(TSKID_TASK2);
	if(ret != IPC_SUCCESS)
	{
		X_PRINTF(DBG_CRIT,"InitializeIpcFramework failed, ret=%d\n", ret);
		exit(1);
	}
	
	while (!quit)
	{
        HANDLE          hReq;
        USHORT          ReqID;
        UINT            ReqSize;
        char            IpcBuf[MAX_MESSAGE_SIZE + 1];
        int             TimeOut = 100;
		
        ret = WaitForRequest (&hReq, &ReqID, IpcBuf, &ReqSize, TimeOut);
        switch (ret)
        {
        case IPC_SUCCESS:
#ifdef DEBUG
			FakeRcvRequest(hReq, ReqID, (void*) IpcBuf, ReqSize);	/* console出力だけ */
#endif
			/*	リクエスト解析へ	*/
			quit = ProcessRequest(hReq, ReqID, (void*) IpcBuf, ReqSize);
            break;
        case IPC_ERR_TIMEOUT:
            break;//continue;
        default:
            X_PRINTF(DBG_CRIT,"WaitForRequest failed, ret=%d\n", ret);
            exit(-1);
            break;
        }
	}

    return 0;
}
