#ifndef __GUI_ERR_H__
#define __GUI_ERR_H__

#include "x_dbg.h"

/////////////////Error Macros /////////////////
#define FUNCTION_ENTRY()                                               \
{                                                                      \
     X_PRINTF( DBG_DEBUG, "Function Entry--> %s\n", __FUNCTION__);   \
}

#define FUNCTION_EXIT()                                                \
{                                                                      \
     X_PRINTF( DBG_DEBUG, "Function Exit<-- %s\n", __FUNCTION__);    \
}

extern UINT gErr;
extern char gErrStr[];

#define GUICHECK(x...)                                           \
{                                                                \
     gErr = (x);                                                 \
     if (GUI_SUCCESS != gErr) {                                  \
          fprintf( stderr, "%s <line %d>:%s FAILED err=%d\n",    \
                   __FILE__, __LINE__,#x,gErr );                 \
     }                                                           \
}

#define GUI_ERR(x)                                               \
{                                                                \
     if (GUI_SUCCESS != x) {                                     \
          fprintf( stderr, "%s <line %d>: ERROR=%d\n",           \
                   __FILE__, __LINE__,x);                        \
          snprintf( gErrStr,64,"%s <line %d>: ERROR=%d",         \
                   __FILE__, __LINE__,x);                        \
          FUNCTION_EXIT();                                       \
          LoadWindow(WINID_ERROR);                               \
          return;                                                \
     }                                                           \
}

#define GUI_ASSERT(x)                                            \
{                                                                \
     if (FALSE == x) {                                           \
          fprintf( stderr, "%s <line %d>: ASSERT failed!\n",     \
                   __FILE__, __LINE__);                          \
          snprintf( gErrStr,64,"%s <line %d>: ASSERT failed!",   \
                   __FILE__, __LINE__);                          \
          FUNCTION_EXIT();                                       \
          LoadWindow(WINID_ERROR);                               \
          return;                                                \
     }                                                           \
}

#define GUI_SYSNG(x)                                             \
{                                                                \
     if (sys_NG == x) {                                          \
          fprintf( stderr, "%s <line %d>: FATAL sys_NG!\n",      \
                   __FILE__, __LINE__);                          \
          snprintf( gErrStr,64, "%s <line %d>: FATAL sys_NG!",   \
                   __FILE__, __LINE__);                          \
          FUNCTION_EXIT();                                       \
          LoadWindow(WINID_ERROR);                               \
          return;                                                \
     }                                                           \
}


#endif //__GUI_ERR_H__
