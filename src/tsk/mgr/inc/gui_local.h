#include <time.h>
#include <sys/time.h>
#include "gui.h"
#include "win_tbl.h"

#define TSKID_FROM_REQID(reqID)   (reqID>>8)

///// Data structures used intenally by the gui framework /////

typedef struct
{
    USHORT           TimerID;
    TIMER_CALLBACK_T pTimerCb;
    struct timeval   expires;
} GUI_TIMER_T;

typedef struct
{
    USHORT              NotifyID;
    NOTIFY_CALLBACK_T   pNotifyCb;
} GUI_NOTIFY_REQUEST_T;

typedef struct
{
    UINT                SeqNum;
    USHORT              RequestID;
    RESPONSE_CALLBACK_T pRespCb;
    struct timeval      expires;
} GUI_REQUEST_T;

typedef struct
{
    GUI_AREA_T       area;
    TOUCH_CALLBACK_T pTouchCb;
} GUI_TOUCH_T;

typedef struct
{
    GUI_STATE_T       state;
    KEY_CALLBACK_T pKeyCb;
} GUI_KEY_T;

typedef struct
{
    int                   input_fd;

    int                   last_win_idx;

    GUI_REQUEST_T         requests[GUI_MAX_REQUESTS];
    GUI_TIMER_T           timers[GUI_MAX_TIMERS];
    GUI_NOTIFY_REQUEST_T  notifies[GUI_MAX_NOTIFIES];

    GUI_TOUCH_T           touch_reserves[GUI_MAX_TOUCH_RESERVES];
    GUI_KEY_T             key_reserves[GUI_MAX_KEY_RESERVES];

    WINTBL_T              current_win;
} GUI_CONTEXT_T;

void            CloseWindows();

//timer.c routines
void            ProcessTimers();
void            ClearTimers();

//touch.c routines
void            ProcessTouchEvents( const struct input_event *data );
void            ProcessKeyEvents( const struct input_event *data );
void            ClearTouchAreas();
void            ClearKeyStates();

//notify.c routines
void            ProcessNotifies(USHORT NotifyID, void *pData, UINT size);
void            ClearNotifies();

//request.c routines
void            ProcessShortResponses (USHORT SeqNum, void *pData, UINT size);
UINT            ProcessLongResponses (USHORT SeqNum, UINT size);
void            ProcessResponseTimeouts();
void            ClearRequests();

//main.c routines
int             IsTimeOver(struct timeval *now, struct timeval *expires);
int             CompareAreas (GUI_AREA_T area1, GUI_AREA_T area2);
int             CompareStates (GUI_STATE_T state1, GUI_STATE_T state2);
