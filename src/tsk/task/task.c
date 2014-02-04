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

extern
int ProcessRequest(HANDLE hReq, USHORT ReqId, void * IpcBuf, USHORT ReqSize);

extern
USHORT gCurrentTaskID;

int
main ()
{
    UINT            ret;
    int             quit = 0;

    ret = InitializeIpcFramework(TSKID_TASK1);
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
            FakeRcvRequest(hReq, ReqID, (void*) IpcBuf, ReqSize);    /* consoleèoóÕÇæÇØ */
#endif
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
