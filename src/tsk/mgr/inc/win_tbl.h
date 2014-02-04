#ifndef __WIN_TBL_H__
#define __WIN_TBL_H__
#include "gui.h"

#define WINID_INVALID  (0)      //Not to be used

//Add other window ids here
#define WINID_OPENING  (1)
#define WINID_MENU     (2)
#define WINID_ERROR    (3)
#define WINID_MAXIMUM  (3)      //Pls update everytime

typedef struct
{
    void    (*OnLoad) (USHORT prev_win_id);//per-window OnLoad routine
    void    *pWinData;//per-window data allocated and used by framework
} WINOBJ_T;

typedef struct
{
    USHORT   win_id;     //Window ID
    WINOBJ_T *win_obj;    //Window Object
} WINTBL_T;

#endif //__WIN_TBL_H__
