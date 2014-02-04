#include <stdio.h>              //for NULL,printf
#include "gui.h"
#include "win_tbl.h"
#include "gui_err.h"
#include "ipc_command.h"
#include "req_id.h"
#include "notify_id.h"
#include "x_dbg.h"

/////////////Function Declarations//////////
static void     MenuOnLoad (USHORT prev_win_id);

/////////////Window Object//////////
WINOBJ_T        gWinMenu = { MenuOnLoad, NULL };

///////////////Defines //////////////////
#define TIMER_ID           (1)
#define TIME_OUT           (6000)  //6 seconds
#define TIMEOUT_LONG       (60000)
///////////////Globals //////////////////

static GUI_AREA_T            full_area  = { 0, 0, GUI_MAX_WIDTH, GUI_MAX_HEIGHT };
static GUI_AREA_T            up_area    = { 0,0,GUI_MAX_WIDTH, GUI_MAX_HEIGHT/2 };
static GUI_AREA_T            down_area  = { 0,GUI_MAX_HEIGHT/2,
                                            GUI_MAX_WIDTH, GUI_MAX_HEIGHT/2 };

static GUI_STATE_T           lf_short   = { GUI_KEY_LF, 1 };
static GUI_STATE_T           up_short   = { GUI_KEY_UP, 1 };
static GUI_STATE_T           down_short = { GUI_KEY_DOWN, 1 };
static GUI_STATE_T           home_short = { GUI_KEY_HOME, 1 };
static GUI_STATE_T           rt_short   = { GUI_KEY_RT, 1 };
static GUI_STATE_T           enter_short= { GUI_KEY_ENTER, 1 };

static GUI_STATE_T           lf_500   = { GUI_KEY_LF, 500 };
static GUI_STATE_T           up_500   = { GUI_KEY_UP, 500 };
static GUI_STATE_T           down_500 = { GUI_KEY_DOWN, 500 };
static GUI_STATE_T           home_500 = { GUI_KEY_HOME, 500 };
static GUI_STATE_T           rt_500   = { GUI_KEY_RT, 500 };
static GUI_STATE_T           enter_500= { GUI_KEY_ENTER, 500 };

static GUI_STATE_T           lf_release   = { GUI_KEY_LF, 0 };
static GUI_STATE_T           up_release   = { GUI_KEY_UP, 0 };
static GUI_STATE_T           down_release = { GUI_KEY_DOWN, 0 };
static GUI_STATE_T           home_release = { GUI_KEY_HOME, 0 };
static GUI_STATE_T           rt_release   = { GUI_KEY_RT, 0 };
static GUI_STATE_T           enter_release= { GUI_KEY_ENTER, 0 };

/////////////Touch Callback//////////

static void
MenuOnDownTouch (void)
{
    FUNCTION_ENTRY();

    FUNCTION_EXIT();
}

static void
MenuOnUpTouch (void)
{
    FUNCTION_ENTRY();

    FUNCTION_EXIT();
}

/////////////Key Callback//////////

static void
MenuOnLfPush (void)
{
    FUNCTION_ENTRY();

    FUNCTION_EXIT();
}

static void
MenuOnUpPush (void)
{
    FUNCTION_ENTRY();

    FUNCTION_EXIT();
}

static void
MenuOnDownPush (void)
{
    FUNCTION_ENTRY();

    FUNCTION_EXIT();
}

static void
MenuOnHomePush (void)
{
    FUNCTION_ENTRY();

    FUNCTION_EXIT();
}

static void
MenuOnRtPush (void)
{
    FUNCTION_ENTRY();

    FUNCTION_EXIT();
}

static void
MenuOnEnterPush (void)
{
    FUNCTION_ENTRY();

    FUNCTION_EXIT();
}

static void
MenuOnLf500Push (void)
{
    TASK1_A1_REQUEST_T req;
    int i;

    FUNCTION_ENTRY();

    for (i=0; i<8; i++)
        req.reserve[i] = i;
    Request (TASK1_REQUEST1, (void *) &req, sizeof(TASK1_A1_REQUEST_T), TIMEOUT_LONG, NULL);


    FUNCTION_EXIT();
}

static void
MenuOnLfRelease (void)
{
    FUNCTION_ENTRY();

    FUNCTION_EXIT();
}

/////////////Timer Callback//////////

static void
MenuOnTimer1 (USHORT id)
{
    FUNCTION_ENTRY();

    FUNCTION_EXIT();
}

///////////////////OnLoad////////////////////
static void
MenuOnLoad (USHORT prev_win_id)
{
//    GUI_STATE_T state;

    FUNCTION_ENTRY();

//    GUICHECK(RegisterTouchCallback(full_area,MenuOnDownTouch));
    GUICHECK(RegisterTouchCallback(up_area,MenuOnUpTouch));
    GUICHECK(RegisterTouchCallback(down_area,MenuOnDownTouch));
    
    GUICHECK(RegisterKeyPushCallback(lf_short, MenuOnLfPush));
    GUICHECK(RegisterKeyPushCallback(up_short, MenuOnUpPush));
    GUICHECK(RegisterKeyPushCallback(down_short, MenuOnDownPush));
    GUICHECK(RegisterKeyPushCallback(home_short, MenuOnHomePush));
    GUICHECK(RegisterKeyPushCallback(rt_short, MenuOnRtPush));
    GUICHECK(RegisterKeyPushCallback(enter_short, MenuOnEnterPush));
    GUICHECK(RegisterKeyPushCallback(lf_500, MenuOnLf500Push));
    GUICHECK(RegisterKeyPushCallback(lf_release, MenuOnLfRelease));

    GUICHECK(RegisterTimerCallback(TIMER_ID, TIME_OUT, MenuOnTimer1));

    FUNCTION_EXIT();
}

