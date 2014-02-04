#include <stdio.h>
#include <sys/time.h>         //for timeval
#include "gui.h"
#include "gui_local.h"
#include "x_dbg.h"

/////////////////// Externs //////////////////////////////
extern GUI_CONTEXT_T   gCtxt;


/////////////////// Timer Routines ///////////////////////
UINT
RegisterTimerCallback (USHORT TimerID, ULONG TimeOut,
                       TIMER_CALLBACK_T timer_callback)
{
    int             i;
    struct timeval  now;
    struct timeval  interval;
    GUI_TIMER_T     *pTimer;

    if (NULL == timer_callback) {
        return GUI_ERR_PARAMETER;
    }

    if (INVALID_TIMERID == TimerID) {
        return GUI_ERR_TIMERID_INVALID;
    }

    for (i = 0; i < GUI_MAX_TIMERS; i++) {
        if (TimerID == gCtxt.timers[i].TimerID) {
            return GUI_ERR_TIMERID_INVALID;
        }
    }

    for (i = 0; i < GUI_MAX_TIMERS; i++) {
        if (NULL == gCtxt.timers[i].pTimerCb) {
            pTimer = &(gCtxt.timers[i]);
            break;
        }
    }

    if (i == GUI_MAX_TIMERS) {
        return GUI_ERR_TIMERS_FULL;
    }

    pTimer->TimerID  = TimerID;
    pTimer->pTimerCb = timer_callback;

    gettimeofday (&now, NULL);
    interval.tv_sec  = TimeOut / 1000;
    interval.tv_usec = (TimeOut % 1000) * 1000;
    timeradd(&now,&interval,&(pTimer->expires));

    return GUI_SUCCESS;
}


void
ProcessTimers ()
{
    int             i;
    struct timeval  now;
    GUI_TIMER_T     Saved;
    GUI_TIMER_T     Cleared = { 0, 0, {0, 0} };

    gettimeofday (&now, NULL);

    for (i = 0; i < GUI_MAX_TIMERS; i++) {
        if (NULL != gCtxt.timers[i].pTimerCb) {
            if (TRUE == IsTimeOver (&now, &(gCtxt.timers[i].expires))) {
                //reuse the slot if a register() is called within callback()
                Saved = gCtxt.timers[i];
                gCtxt.timers[i] = Cleared;
                Saved.pTimerCb (Saved.TimerID);
            }
        }
    }
}

UINT
DeregisterTimer (USHORT TimerID)
{
    int             i;
    GUI_TIMER_T     Cleared = { 0, 0, {0, 0} };

    if (INVALID_TIMERID == TimerID) {
        return GUI_ERR_TIMERID_INVALID;
    }

    for (i = 0; i < GUI_MAX_TIMERS; i++) {
        if (TimerID == gCtxt.timers[i].TimerID) {
            gCtxt.timers[i] = Cleared;
            break;
        }
    }

    if (i == GUI_MAX_TIMERS) {
        return GUI_ERR_TIMERID_INVALID;
    }

    return GUI_SUCCESS;
}

void
ClearTimers ()
{
    int             i;
    GUI_TIMER_T     Cleared = { 0, 0, {0, 0} };

    for (i = 0; i < GUI_MAX_TIMERS; i++) {
        gCtxt.timers[i] = Cleared;
    }
}

void
DumpTimers ()
{
    int             i;

    for (i = 0; i < GUI_MAX_TIMERS; i++) {
        X_PRINTF (DBG_DEBUG,"timer%d TimerID=%d %p\n", i, 
                gCtxt.timers[i].TimerID, gCtxt.timers[i].pTimerCb);
    }
}

