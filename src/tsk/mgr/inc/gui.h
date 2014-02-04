#ifndef __GUI_H__
#define __GUI_H__
#include "datatypes.h"

#define GUI_MAX_IMAGES          (32)    //Maximum number of images in a window
#define GUI_MAX_TIMERS          (16)    //Maximum number of timers in a window
#define GUI_MAX_TOUCH_RESERVES  (16)    //Maximum number of touch reserves in a window
#define GUI_MAX_KEY_RESERVES    (16)    //Maximum number of key reserves in a window
#define GUI_MAX_NOTIFIES        (16)    //Maximum number of notifications in a window
#define GUI_MAX_REQUESTS        (16)    //Maximum number of ongoing requests in a window
#define GUI_MAX_WIDTH           (400)   //Horizantal Resolution
#define GUI_MAX_HEIGHT          (240)   //Vertical   Resolution

#define GUI_KEY_LF              (1)
#define GUI_KEY_UP              (14)
#define GUI_KEY_DOWN            (15)
#define GUI_KEY_HOME            (19)
#define GUI_KEY_RT              (2)
#define GUI_KEY_ENTER           (13)


#define INVALID_TIMERID         (0)     //Zero is not a valid Timer-ID
#define INVALID_NOTIFYID        (0)     //Zero is not a valid Notify-ID
#define INVALID_REQUESTID       (0)     //Zero is not a valid Request-ID

/////Error Codes//////
#define GUI_SUCCESS               (0)
#define GUI_ERR_AREAS_FULL        (1)
#define GUI_ERR_IMAGES_FULL       (2)
#define GUI_ERR_PARAMETER         (3)
#define GUI_ERR_TIMERID_INVALID	  (4)
#define GUI_ERR_TIMERS_FULL       (5)
#define GUI_ERR_AREA_INVALID      (6)
#define GUI_ERR_WINID_INVALID     (7)
#define GUI_ERR_MOVE_FULL         (8)
#define GUI_ERR_SCALE_FULL        (9)
#define GUI_ERR_NOTIFYID_INVALID  (10)
#define GUI_ERR_NOTIFIES_FULL     (11)
#define GUI_ERR_REQUESTID_INVALID (12)
#define GUI_ERR_REQUESTS_FULL     (13)
#define GUI_ERR_NO_MEMORY         (14)
#define GUI_ERR_IPC_SEND          (15)
#define GUI_ERR_RESP_TIMEOUT      (16)
#define GUI_ERR_SHM_ALLOCATE      (17)
#define GUI_ERR_SHM_ATTACH        (18)
#define GUI_ERR_SHM_REMOVE        (19)
#define GUI_ERR_SHM_DETACH        (20)
#define GUI_ERR_NO_IMAGE_DECODER  (21)

#define DEFINE_AREA(a,x1,y1,w1,h1)       \
{       a.x = x1;                        \
        a.y = y1;                        \
        a.w = w1;                        \
        a.h = h1;                        \
}

/////Data Structures//////
typedef struct
{
    USHORT          x;
    USHORT          y;
    USHORT          w;
    USHORT          h;
} GUI_AREA_T;

typedef struct
{
    USHORT          x;
    USHORT          y;
} GUI_POS_T;

typedef struct
{
    USHORT          w;
    USHORT          h;
} GUI_SIZE_T;

typedef struct
{
    USHORT          type;
    USHORT          time;
} GUI_STATE_T;

typedef void    (*TIMER_CALLBACK_T) (USHORT);
typedef void    (*TOUCH_CALLBACK_T) (void);
typedef void    (*KEY_CALLBACK_T) (void);
typedef void    (*NOTIFY_CALLBACK_T) (void *pData, UINT size);
typedef void    (*RESPONSE_CALLBACK_T) (void *pRespData, UINT RespSize, UINT GuiErr);

/////APIs provided to application//////
void            LoadWindow (USHORT win_id);

UINT            RegisterTimerCallback (USHORT TimerID, ULONG TimeOut,
                                       TIMER_CALLBACK_T timer_callback);
UINT            DeregisterTimer (USHORT TimerID);
UINT            RegisterTouchCallback (GUI_AREA_T area,
                                       TOUCH_CALLBACK_T touch_callback);
UINT            RegisterKeyCallback (GUI_STATE_T key,
                                       KEY_CALLBACK_T key_callback);

UINT            RegisterNotifyCallback (USHORT NotifyID,
                                        NOTIFY_CALLBACK_T notify_callback);
UINT            Request (USHORT RequestID, void *pReqData, UINT ReqSize,
                         ULONG TimeOut, RESPONSE_CALLBACK_T request_callback);

#endif //__GUI_H__
