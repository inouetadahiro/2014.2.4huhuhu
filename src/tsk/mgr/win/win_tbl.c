#include "gui.h"
#include "win_tbl.h"

//We can add new windows as below

extern WINOBJ_T gWinOpening;
extern WINOBJ_T gWinMenu;
extern WINOBJ_T gWinError;

WINTBL_T        gWindowTbl[] = {
    {WINID_OPENING,      &gWinOpening},     //opening.c
    {WINID_MENU,         &gWinMenu},        //menu.c
    {WINID_ERROR,        &gWinError}        //winerror.c
};

