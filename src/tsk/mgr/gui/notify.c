#include "gui.h"
#include "gui_local.h"

/////////////////// Externs //////////////////////////////
extern GUI_CONTEXT_T gCtxt;

/////////////////// Notify Functions//////////////////////////////

UINT
RegisterNotifyCallback (USHORT NotifyID, NOTIFY_CALLBACK_T notify_callback)
{
    int                  i;
    GUI_NOTIFY_REQUEST_T *pNotify;

    if (NULL == notify_callback) {
        return GUI_ERR_PARAMETER;
    }

    if (INVALID_NOTIFYID == NotifyID) {
        return GUI_ERR_NOTIFYID_INVALID;
    }

    for (i = 0; i < GUI_MAX_NOTIFIES; i++) {
        if (NotifyID == gCtxt.notifies[i].NotifyID) {
            return GUI_ERR_NOTIFYID_INVALID;
        }
    }

    for (i = 0; i < GUI_MAX_NOTIFIES; i++) {
        if (NULL == gCtxt.notifies[i].pNotifyCb) {
            pNotify = &(gCtxt.notifies[i]);
            break;
        }
    }

    if (i == GUI_MAX_NOTIFIES) {
        return GUI_ERR_NOTIFIES_FULL;
    }

    pNotify->NotifyID  = NotifyID;
    pNotify->pNotifyCb = notify_callback;

    return GUI_SUCCESS;
}


void
ProcessNotifies (USHORT NotifyID, void *pData, UINT size)
{
    int             i;

    for (i = 0; i < GUI_MAX_NOTIFIES; i++) {
        if (NULL != gCtxt.notifies[i].pNotifyCb) {
            if (NotifyID == gCtxt.notifies[i].NotifyID) {
                gCtxt.notifies[i].pNotifyCb (pData, size);;
                break;
            }
        }
    }
}

void
ClearNotifies ()
{
    int                  idx;
    GUI_NOTIFY_REQUEST_T Cleared = { 0, 0 };

    for (idx = 0; idx < GUI_MAX_NOTIFIES; idx++) {
        gCtxt.notifies[idx] = Cleared;;
    }
}
