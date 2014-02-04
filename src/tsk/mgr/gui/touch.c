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
static int             ts_x = GUI_MAX_WIDTH / 2;
static int             ts_y = GUI_MAX_HEIGHT / 2;

static int x_min =  85;
static int x_max = 940;
static int y_min = 100;
static int y_max = 900;

/////////////////// Touch Routines ///////////////////////////
void
ClearTouchAreas ()
{
    int             i;
    GUI_TOUCH_T     Cleared = { {0, 0, 0, 0}, 0 };

    for (i = 0; i < GUI_MAX_TOUCH_RESERVES; i++) {
        gCtxt.touch_reserves[i] = Cleared;
    }
}

UINT
RegisterTouchCallback (GUI_AREA_T area, TOUCH_CALLBACK_T touch_callback)
{
    int             i;
    GUI_TOUCH_T     Cleared = { {0, 0, 0, 0}, 0 };

    if ((area.x == 0) && (area.y == 0) && (area.w == 0) && (area.h == 0)) {
        return GUI_ERR_PARAMETER;
    }

    if ((area.x > GUI_MAX_WIDTH) || (area.y > GUI_MAX_HEIGHT)) {
        return GUI_ERR_PARAMETER;
    }

    if ((area.w > GUI_MAX_WIDTH) || (area.h > GUI_MAX_HEIGHT)) {
        return GUI_ERR_PARAMETER;
    }

    //Deregistering callback
    if (touch_callback == NULL) {
        for (i = 0; i < GUI_MAX_TOUCH_RESERVES; i++) {
            if (TRUE == CompareAreas (area, gCtxt.touch_reserves[i].area)) {
                gCtxt.touch_reserves[i] = Cleared;
                return GUI_SUCCESS;
            } //if
        } //for
        return GUI_ERR_AREA_INVALID;
    }

    //Registering callback
    for (i = 0; i < GUI_MAX_TOUCH_RESERVES; i++) {
        if (NULL == gCtxt.touch_reserves[i].pTouchCb) {
            gCtxt.touch_reserves[i].pTouchCb = touch_callback;
            gCtxt.touch_reserves[i].area = area;
            return GUI_SUCCESS;
        } //if
    } //for

    return GUI_ERR_AREAS_FULL;
}


static void
ProcessTouchAreas ()
{
    int             i;
    GUI_AREA_T      area;

    for (i = 0; i < GUI_MAX_TOUCH_RESERVES; i++) {
        if (NULL != gCtxt.touch_reserves[i].pTouchCb) {
            area = gCtxt.touch_reserves[i].area;
            if ((ts_x >= area.x) &&
                (ts_x <= area.x + area.w) &&
                (ts_y >= area.y) && (ts_y <= area.y + area.h)) {
                gCtxt.touch_reserves[i].pTouchCb ();
                break;
            }
        }
    }
}

void
ProcessTouchEvents ( struct input_event data )
{
}

