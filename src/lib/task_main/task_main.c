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

extern USHORT gCurrentTaskID;

extern int onCreate ();
extern int onLoop ();
extern int onDestroy ();

int
main ()
{
    UINT            ret;
    int             quit = 0;

    ret = InitializeIpcFramework(gCurrentTaskID);
    if(ret != IPC_SUCCESS) {
        X_PRINTF(DBG_CRIT,"InitializeIpcFramework failed, ret=%d\n", ret);
        exit(1);
    }
    onTaskCreate();

    while (!quit) {
        HANDLE          hReq;
        USHORT          ReqID;
        UINT            ReqSize;
        char            IpcBuf[MAX_MESSAGE_SIZE + 1];
        int             TimeOut = 100;

        ret = WaitForRequest (&hReq, &ReqID, IpcBuf, &ReqSize, TimeOut);
        switch (ret) {
        case IPC_SUCCESS:
			/* to request analysis */
			quit = ProcessRequest(hReq, ReqID, (void*) IpcBuf, ReqSize);
            break;
        case IPC_ERR_TIMEOUT:
            break;//continue;
        default:
            X_PRINTF(DBG_CRIT,"WaitForRequest failed, ret=%d\n", ret);
            exit(-1);
            break;
        }
        onLoop();

    }
    onDestroy();

    return 0;
}
