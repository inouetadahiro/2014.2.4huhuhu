#include <stdio.h>              //for NULL,printf
#include "gui.h"
#include "win_tbl.h"
#include "req_id.h"
#include "notify_id.h"
#include "ipc_command.h"
#include "gui_err.h"
#include "x_dbg.h"

/////////////Function Declarations//////////
static void     OpeningOnLoad (USHORT prev_win_id);

/////////////Window Object//////////
WINOBJ_T        gWinOpening = { OpeningOnLoad, NULL };

///////////////////OnLoad////////////////////
static void
OpeningOnLoad (USHORT prev_win_id)
{
    //Ç»Ç…Ç©èàóù
	
	usleep(1000000);

    LoadWindow(WINID_MENU);
}

