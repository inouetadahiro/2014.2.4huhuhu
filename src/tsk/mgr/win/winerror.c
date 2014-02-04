#include <stdio.h>              //for NULL,printf
#include "gui.h"
#include "win_tbl.h"
#include "gui_err.h"

/////////////Function Declarations//////////
static void     WinErrOnLoad (USHORT prev_win_id);

/////////////Window Object//////////
WINOBJ_T        gWinError = { WinErrOnLoad, NULL };

///////////////Globals //////////////////

UINT gErr;
char gErrStr[64];

///////////////////OnLoad////////////////////
static void
WinErrOnLoad (USHORT prev_win_id)
{
    FUNCTION_ENTRY();

    FUNCTION_EXIT();
}

