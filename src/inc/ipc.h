#ifndef __IPC_H__
#define __IPC_H__

#include "tsk_id.h"

#define INVALID_MSGQ_ID         (-1)

#define NUM_MAX_MESSAGES        (32)
#define MAX_MESSAGE_SIZE        (512)
#define LONG_MAX_MESSAGE_SIZE   (8192+1)

#define MAX_NOTIFY_DATA_SIZE    (MAX_MESSAGE_SIZE-NTFYMSG_HDR_SIZE)
#define REQMSG_HDR_SIZE         (sizeof(REQUEST_MSG_T)-1)
#define RESPMSG_HDR_SIZE        (sizeof(RESPONSE_MSG_T)-1)
#define NTFYMSG_HDR_SIZE        (sizeof(NOTIFY_MSG_T)-1)

#define IPC_SUCCESS             (0)
#define IPC_ERR_INIT_FAILURE    (1)
#define IPC_ERR_TIMEOUT         (2)
#define IPC_ERR_MSG_RECV        (3)
#define IPC_ERR_NOT_INITIALIZED (4)
#define IPC_ERR_MSG_SEND        (5)
#define IPC_ERR_INVALID_TASKID  (6)
#define IPC_ERR_PARAMETER       (7)
#define IPC_ERR_NO_MEMORY       (8)
#define IPC_ERR_MSG_FORMAT      (9)
#define IPC_ERR_SHM_ALLOCATE    (10)
#define IPC_ERR_SHM_ATTACH      (11)
#define IPC_ERR_RCV_BUFF_SMALL  (12)
#define IPC_ERR_SHM_REMOVE      (13)
#define IPC_ERR_SHM_DETACH      (14)

#define MGR_QUEUE_NAME          "/mgr"
#define DBGX_QUEUE_NAME         "/dbgx"
#define TASK1_QUEUE_NAME        "/task1"
#define TASK2_QUEUE_NAME        "/task2"
#define TASK3_QUEUE_NAME        "/task3"
#define TASK4_QUEUE_NAME        "/task4"
#define TASK5_QUEUE_NAME        "/task5"
#define TASK6_QUEUE_NAME        "/task6"
#define TASK7_QUEUE_NAME        "/task7"
#define TASK8_QUEUE_NAME        "/task8"
#define WATCH_QUEUE_NAME        "/watch"

#define MSG_TYPE_SHORT_NOTIFY   (1)
#define MSG_TYPE_LONG_NOTIFY    (2)
#define MSG_TYPE_SHORT_REQUEST  (3)
#define MSG_TYPE_LONG_REQUEST   (4)
#define MSG_TYPE_SHORT_RESPONSE (5)
#define MSG_TYPE_LONG_RESPONSE  (6)

typedef unsigned int HANDLE;

#pragma pack(push, 1)

typedef struct
{
    USHORT          Type;
} IPC_MSG_T;

typedef struct
{
    USHORT          Type;
    USHORT          NotifyID;
    UINT            Size;
    char            pData[1];
} NOTIFY_MSG_T;

typedef struct
{
    USHORT          Type;
    UINT            SeqNum;
    USHORT          RequestID;
    UINT            Size;
    char            pData[1];
} REQUEST_MSG_T;
#pragma pack(pop)

typedef REQUEST_MSG_T RESPONSE_MSG_T;

////////Upper level Routines///////
UINT            InitializeIpcFramework (TASKID_T task_id);
UINT            Notify (USHORT Notify, void *pNotifyData, UINT Size);
UINT            FakeRequest (USHORT Request, void *pRequestData, UINT Size);
UINT            FakeNotify (USHORT Notify, void *pNotifyData, UINT Size);
UINT            FakeResponse (HANDLE hReq, void *pRespData, UINT Size);
UINT            FakeRcvRequest (HANDLE hReq, USHORT ReqID, void *IpcBuf, UINT Size);
UINT            WaitForRequest (HANDLE * phReq, USHORT * pRequestID, void *pReqData,
                                UINT * pReqSize, ULONG TimeOut);
UINT            Response (HANDLE hReq, void *pRespData, UINT RespSize);

////////Low level Routines///////
UINT            SendIpcMessage (char *pBuf, USHORT BufLen, TASKID_T task_id);
UINT            RecvIpcMessage (char *pBuf, USHORT BufLen, ULONG TimeOut,
                                USHORT * ReadLen);
//------------------------------------------------------------------------

#define LF           '\n'
#define NULL_CHAR    ((char) 0)

typedef struct
{
    char * start;
    int index;
    int size;
    int line;
} TStrings;

TStrings * CreateStrings(int size);
int ReleaseStrings(TStrings * S);
int AddString(TStrings * S, char * str);

//------------------------------------------------------------------------

#endif //__IPC_H__
