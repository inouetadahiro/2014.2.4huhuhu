#include <stdio.h>
#include <unistd.h>             //for read()
#include <errno.h>             //for read()
#include <sys/select.h>        //for fd_set
#include <linux/input.h>       //for input_event
#include "gui.h"
#include "gui_local.h"
#include "x_dbg.h"

#include "gpio.h"


/////////////////// Externs //////////////////////////////////
extern GUI_CONTEXT_T   gCtxt;


/////////////////// Globals  /////////////////////////////////

/////////////////// Touch Routines ///////////////////////////
void
ClearKeyStates ()
{
    int             i;
    GUI_KEY_T     Cleared = { {0, 0}, 0 };

    for (i = 0; i < GUI_MAX_KEY_RESERVES; i++) {
        gCtxt.key_reserves[i] = Cleared;
    }
}

UINT
RegisterKeyPushCallback (GUI_STATE_T state, KEY_CALLBACK_T key_callback)
{
    int             i;
    GUI_KEY_T     Cleared = { {0, 0}, 0 };

//    if ( key‚ª”ÍˆÍŠO ) {
//        return GUI_ERR_PARAMETER;
//    }

    //Deregistering callback
    if (key_callback == NULL) {
        for (i = 0; i < GUI_MAX_KEY_RESERVES; i++) {
            if (TRUE ==CompareStates(state, gCtxt.key_reserves[i].state)) {
                gCtxt.key_reserves[i] = Cleared;
                return GUI_SUCCESS;
            } //if
        } //for
        return GUI_ERR_AREA_INVALID;
    }

    //Registering callback
    for (i = 0; i < GUI_MAX_KEY_RESERVES; i++) {
        if (NULL == gCtxt.key_reserves[i].pKeyCb) {
            gCtxt.key_reserves[i].pKeyCb = key_callback;
            gCtxt.key_reserves[i].state = state;
            return GUI_SUCCESS;
        } //if
    } //for

    return GUI_ERR_AREAS_FULL;
}

static void
ProccessKeyState(GUI_STATE_T state)
{
    int             i;

    for (i = 0; i < GUI_MAX_KEY_RESERVES; i++) {
        if (NULL != gCtxt.key_reserves[i].pKeyCb) {
            if ((state.type == gCtxt.key_reserves[i].state.type)
                    && (state.time == gCtxt.key_reserves[i].state.time)) {
                gCtxt.key_reserves[i].pKeyCb ();
                break;
            }
        }
    }
}

#define KEY_RELEASE 0
#define KEY_PUSH    1
#define KEY_REPEAT  2

void
ProcessKeyEvents ( struct input_event data )
{
    GUI_STATE_T state;
    unsigned short key_type = data.code;
    unsigned long push = data.value;
    static struct timeval begin_time;
    static long count;
    long diff;

    switch (push) {
    case KEY_RELEASE:
        state.type = key_type;
        state.time = 0;
        ProccessKeyState(state);
        break;
    case KEY_PUSH:
        state.type = key_type;
        state.time = 1;
        ProccessKeyState(state);
        begin_time = data.time;
        count = 1;
        break;
    case KEY_REPEAT:
        count++;
	state.type = key_type;
	state.time = count * 100;
	ProccessKeyState(state);
        break;
    }
}

