#include <mqueue.h>
#include <time.h>
#include <errno.h>
#include <sys/time.h>           //gettimeofday
#include <stdio.h>              //NULL,perror
#include <string.h>             //memcpy
#include <stdlib.h>             //malloc
#include <sys/ipc.h>            //for shmem
#include <sys/shm.h>            //for shmem
#include <sys/stat.h>        //for S_IRWXU symbol
#include <fcntl.h>
#include <mqueue.h>
#include "datatypes.h"
#include "x_dbg.h"
#include "ipc.h"

mqd_t           gMyQ     = INVALID_MSGQ_ID;
static mqd_t    gTskQ[TSK_NUM];

//------------------------------------------------------------------------
TStrings * CreateStrings(int size)
{
    TStrings  * res;
    res = (TStrings *) malloc(sizeof(TStrings));
    res->start = (char *) malloc(size);
    res->start[0] = NULL_CHAR;
    res->index = 0;
    res->size = size;
    res->line = 1;
    return res;
}

int ReleaseStrings(TStrings * S)
{
    free(S->start);
    free(S);
    return 0;
}

int AddString(TStrings * S, char * str)
{
    int res;
    char c;

    res = 0;
    while (1) {   
        if (S->index >= S->size) {
            return res;
        }
        c = *str++;
        S->start[S->index++] = c;
        switch (c) {
        case LF:
            S->line++;
            break;
        case NULL_CHAR:
            S->index--;
            return res;
        }
        res++;
    }
    return 0;
}

//------------------------------------------------------------------------

static const char * QUEUE_NAME[] = {
    MGR_QUEUE_NAME,
    DBGX_QUEUE_NAME,
    TASK1_QUEUE_NAME,
    TASK2_QUEUE_NAME,
    TASK3_QUEUE_NAME,
    TASK4_QUEUE_NAME,
    TASK5_QUEUE_NAME,
    TASK6_QUEUE_NAME,
    TASK7_QUEUE_NAME,
    TASK8_QUEUE_NAME,
    WATCH_QUEUE_NAME,
};

UINT
InitializeIpcFramework (TASKID_T task_id)
{
    struct mq_attr  attrib;
    int i;

    if (task_id > TSK_NUM) {
            X_PRINTF (DBG_CRIT,"Unsupported task_id=%d\n",task_id);
        return IPC_ERR_INVALID_TASKID;
    }

    attrib.mq_flags   = 0;
    attrib.mq_maxmsg  = NUM_MAX_MESSAGES;
    attrib.mq_msgsize = MAX_MESSAGE_SIZE;
    attrib.mq_curmsgs = 0;

    for (i = 0; i < TSK_NUM; i++) {
        gTskQ[i] = mq_open (QUEUE_NAME[i], O_RDWR | O_CREAT, S_IRWXU, &attrib);
        if (gTskQ[i] == -1) {
            X_PRINTF (DBG_CRIT,"Msgq open failed errno=%d\n",errno);
            return IPC_ERR_INIT_FAILURE;
        }
    }

    gMyQ = gTskQ[task_id - 1];
    return IPC_SUCCESS;
}

UINT
RecvIpcMessage (char *pBuf, USHORT BufLen, ULONG TimeOut, USHORT * RecvLen)
{
    int             len;
    struct timeval  now;
    struct timespec abs_timeout;

    if (gMyQ == INVALID_MSGQ_ID) {
        return IPC_ERR_NOT_INITIALIZED;
    }

    gettimeofday (&now, NULL);
    abs_timeout.tv_sec  = now.tv_sec + (TimeOut / 1000);
    abs_timeout.tv_nsec = now.tv_usec * 1000 + (TimeOut % 1000) * 1000 * 1000;
    if (abs_timeout.tv_nsec >= 1000000000) {
        abs_timeout.tv_sec++;
        abs_timeout.tv_nsec -= 1000000000;
    }

    len = mq_timedreceive (gMyQ, pBuf, BufLen, NULL, &abs_timeout);
    if (len == -1) {
        if (errno == ETIMEDOUT) {
            return IPC_ERR_TIMEOUT;
        } else {
            X_PRINTF (DBG_ERR,"RecvIpcMessage failed errno=%d\n",errno);
            return IPC_ERR_MSG_RECV;
        }
    }

    (*RecvLen) = len;

    {
        int i;

        X_PRINTF(DBG_DEBUG,"Recv Message:","");
        for(i=0;i<len;i++){
            printf("0x%X ",pBuf[i]);
        }
        printf("\n");
    }

    return IPC_SUCCESS;
}

UINT
SendIpcMessage (char *pBuf, USHORT BufLen, TASKID_T task_id)
{
    int             ret;
    mqd_t           MsgQ;

    if (task_id == TSKID_INVALID || task_id > TSK_NUM) {
        X_PRINTF (DBG_CRIT,"Unsupported task_id=%d\n",task_id);
        return IPC_ERR_INVALID_TASKID;
    }
    MsgQ = gTskQ[task_id - 1];


    {
        int i;

        X_PRINTF(DBG_DEBUG,"Sent Message:","");
        for(i=0;i<BufLen;i++){
            printf("0x%X ",pBuf[i]);
        }
        printf("\n");
    }


    ret = mq_send (MsgQ, pBuf, BufLen, 0);
    if (ret == -1) {
        X_PRINTF (DBG_ERR,"mq_send failed errno=%d\n",errno);
        return IPC_ERR_MSG_SEND;
    }

    return IPC_SUCCESS;
}

////////////Upper level Routines//////////////////// 

UINT
FakeRequest (USHORT Request, void *pRequestData, UINT Size)
{
    REQUEST_MSG_T   *pMsg;
    int             ret;

    if ((Size != 0) && (pRequestData == NULL)) {
        return IPC_ERR_PARAMETER;
    }

    if ((Size > (MAX_MESSAGE_SIZE-REQMSG_HDR_SIZE))) {
        return IPC_ERR_PARAMETER;
    }

    pMsg = (REQUEST_MSG_T *) calloc (Size + REQMSG_HDR_SIZE, 1);
    if (NULL == pMsg) {
        return IPC_ERR_NO_MEMORY;
    }

    pMsg->Type     = MSG_TYPE_SHORT_REQUEST;
    pMsg->RequestID = Request;
    pMsg->Size     = Size;
    memcpy (pMsg->pData, pRequestData, Size);

    ret = SendIpcMessage ((char *) pMsg, (Size + REQMSG_HDR_SIZE), TSKID_MGR);
    if (ret != IPC_SUCCESS) {
        X_PRINTF (DBG_ERR,"SendIpcMessage failed %d\n", ret);
    }

    free (pMsg);

    return ret;
}

UINT
FakeNotify (USHORT Notify, void *pNotifyData, UINT Size)
{
    int i;

    printf("$Notify %d %d", Notify, Size);
    for (i = 0; i < Size; i++)
        printf(" %d", ((unsigned char *) pNotifyData)[i]);
    printf("\n");

    return IPC_SUCCESS;
}

UINT
FakeResponse (HANDLE hReq, void *pRespData, UINT Size)
{
    int i;

    printf("$Response %d %d", hReq, Size);
    for (i = 0; i < Size; i++)
        printf(" %d", ((unsigned char *) pRespData)[i]);
    printf("\n");

    return IPC_SUCCESS;
}

UINT
FakeRcvRequest (HANDLE hReq, USHORT ReqID, void *IpcBuf, UINT Size)
{
    int i;

    printf("$RcvRequest %d %d %d", hReq, ReqID, Size);
    for (i = 0; i < Size; i++)
        printf(" %d", ((unsigned char *) IpcBuf)[i]);
    printf("\n");

    return IPC_SUCCESS;
}

UINT
Notify (USHORT Notify, void *pNotifyData, UINT Size)
{
    NOTIFY_MSG_T   *pMsg;
    int             ret;

    if ((Size != 0) && (pNotifyData == NULL)) {
        return IPC_ERR_PARAMETER;
    }

    if ((Size > MAX_NOTIFY_DATA_SIZE)) {
        return IPC_ERR_PARAMETER;
    }

    pMsg = (NOTIFY_MSG_T *) calloc (Size + NTFYMSG_HDR_SIZE, 1);
    if (NULL == pMsg) {
        return IPC_ERR_NO_MEMORY;
    }

    pMsg->Type     = MSG_TYPE_SHORT_NOTIFY;
    pMsg->NotifyID = Notify;
    pMsg->Size     = Size;
    memcpy (pMsg->pData, pNotifyData, Size);

    ret = SendIpcMessage ((char *) pMsg, (Size + NTFYMSG_HDR_SIZE), TSKID_MGR);
    if (ret != IPC_SUCCESS) {
        X_PRINTF (DBG_ERR,"SendIpcMessage failed %d\n", ret);
    }

    free (pMsg);

    return ret;
}

static UINT
ReceiveLongRequest (HANDLE * phReq, USHORT * pRequestID, void *pReqData,
                    UINT * pReqSize, REQUEST_MSG_T * pMsg)
{
    int             shm_id;
    void           *shm_ptr;
    int             ret;

    shm_id = shmget ((key_t) pMsg->SeqNum, pMsg->Size, IPC_CREAT);//S_IRWXU);
    if (shm_id == -1) {
        X_PRINTF (DBG_ERR,"ReceiveLongRequest: shmget failed errno=%d\n",errno);
        return IPC_ERR_SHM_ALLOCATE;
    }

    shm_ptr = shmat (shm_id, NULL, 0);
    if (((int) shm_ptr) == -1) {
        X_PRINTF (DBG_ERR,"ReceiveLongRequest: shmat failed errno=%d\n",errno);
        return IPC_ERR_SHM_ATTACH;
    }

    (*phReq)      = pMsg->SeqNum;
    (*pRequestID) = pMsg->RequestID;
    (*pReqSize)   = pMsg->Size;
    memcpy (pReqData, shm_ptr, pMsg->Size);

    ret = shmctl (shm_id, IPC_RMID, NULL);
    if (ret == -1) {
        X_PRINTF (DBG_ERR,"ReceiveLongRequest: shmctl failed errno=%d\n",errno);
        return IPC_ERR_SHM_REMOVE;
    }

    ret = shmdt (shm_ptr);
    if (ret == -1) {
        X_PRINTF (DBG_ERR,"ReceiveLongRequest: shmdt failed errno=%d\n",errno);
        return IPC_ERR_SHM_DETACH;
    }

    return IPC_SUCCESS;
}

UINT
WaitForRequest (HANDLE * phReq, USHORT * pRequestID, void *pReqData,
                UINT * pReqSize, ULONG TimeOut)
{
    char            IpcBuf[MAX_MESSAGE_SIZE + 1];
    USHORT          RecvLen = 0;
    REQUEST_MSG_T  *pMsg;
    int             ret;

    if ((phReq == NULL) || (pRequestID == NULL) || (pReqData == NULL)
        || (pReqSize == NULL)) {
        return IPC_ERR_PARAMETER;
    }

    ret = RecvIpcMessage (IpcBuf, (MAX_MESSAGE_SIZE + 1), TimeOut, &RecvLen);
    if (ret != IPC_SUCCESS) {
        return ret;
    }

    if (RecvLen == 0) {
        X_PRINTF (DBG_ERR,"Invalid RecvLen=%d!\n",RecvLen);
        return IPC_ERR_MSG_RECV;
    }

    pMsg = (REQUEST_MSG_T *) IpcBuf;

    if ((pMsg->Size) > (*pReqSize)) {
        X_PRINTF (DBG_ERR,"Rcv Buffer too small msg size=%d\n",pMsg->Size);
        return IPC_ERR_RCV_BUFF_SMALL;
    }

    switch (pMsg->Type) {

    case MSG_TYPE_SHORT_REQUEST:
        if (RecvLen != ((pMsg->Size) + REQMSG_HDR_SIZE)) {
            X_PRINTF (DBG_ERR,"Task: Invalid length RecvLen=%d!\n",RecvLen);
            ret = IPC_ERR_MSG_FORMAT;
            break;
        }
        (*phReq)      = pMsg->SeqNum;
        (*pRequestID) = pMsg->RequestID;
        (*pReqSize)   = pMsg->Size;
        if (pMsg->Size != 0) {
            memcpy (pReqData, pMsg->pData, pMsg->Size);
        }
        ret = IPC_SUCCESS;
        break;

    case MSG_TYPE_LONG_REQUEST:
        ret = ReceiveLongRequest (phReq, pRequestID, pReqData, pReqSize, pMsg);
        break;

    default:
        X_PRINTF (DBG_ERR,"Invalid Message Type=%d\n",pMsg->Type);
        ret = IPC_ERR_MSG_FORMAT;
        break;
    }

    return ret;
}

static UINT
SendShortResponse (HANDLE hReq, void *pRespData, UINT RespSize)
{
    RESPONSE_MSG_T *pMsg;
    int             ret;

    pMsg = (RESPONSE_MSG_T *) calloc (RespSize + RESPMSG_HDR_SIZE, 1);
    if (NULL == pMsg) {
        return IPC_ERR_NO_MEMORY;
    }

    pMsg->Type   = MSG_TYPE_SHORT_RESPONSE;
    pMsg->SeqNum = hReq;
    pMsg->Size   = RespSize;
    memcpy (pMsg->pData, pRespData, RespSize);

    ret = SendIpcMessage ((char *) pMsg, (RespSize + RESPMSG_HDR_SIZE), TSKID_MGR);
    if (ret != IPC_SUCCESS) {
        X_PRINTF (DBG_ERR,"SendShortResponse: SendIpcMessage failed %d\n", ret);
    }

    free (pMsg);

    return ret;
}

static UINT
SendLongResponse (HANDLE hReq, void *pRespData, UINT RespSize)
{
    int             shm_id;
    void           *shm_ptr;
    int             ret;
    RESPONSE_MSG_T *pMsg;

    //Copy message to shared memory
    shm_id = shmget ((key_t) hReq, RespSize, S_IRWXU | IPC_CREAT);
    if (shm_id == -1) {
        X_PRINTF (DBG_ERR,"SendLongResponse: shmget failed errno=%d\n",errno);
        return IPC_ERR_SHM_ALLOCATE;
    }

    shm_ptr = shmat (shm_id, NULL, 0);
    if (((int) shm_ptr) == -1) {
        X_PRINTF (DBG_ERR,"SendLongResponse: shmat failed errno=%d\n",errno);
        return IPC_ERR_SHM_ATTACH;
    }

    memcpy (shm_ptr, pRespData, RespSize);

    ret = shmdt (shm_ptr);
    if (ret == -1) {
        X_PRINTF (DBG_ERR,"SendLongResponse: shmdt failed errno=%d\n",errno);
        return IPC_ERR_SHM_DETACH;
    }
    //Sending IPC Message
    pMsg = (RESPONSE_MSG_T *) calloc (RESPMSG_HDR_SIZE, 1);
    if (NULL == pMsg) {
        return IPC_ERR_NO_MEMORY;
    }

    pMsg->Type   = MSG_TYPE_LONG_RESPONSE;
    pMsg->SeqNum = hReq;
    pMsg->Size   = RespSize;

    ret = SendIpcMessage ((char *) pMsg, (RESPMSG_HDR_SIZE), TSKID_MGR);
    if (ret != IPC_SUCCESS) {
        X_PRINTF (DBG_ERR,"SendLongResponse: SendIpcMessage failed %d\n", ret);
    }

    free (pMsg);

    return ret;
}

UINT
Response (HANDLE hReq, void *pRespData, UINT RespSize)
{
    if ((pRespData == NULL) || (RespSize == 0)) {
        return IPC_ERR_PARAMETER;
    }

    if ((RespSize + RESPMSG_HDR_SIZE) <= MAX_MESSAGE_SIZE) {
        return SendShortResponse (hReq, pRespData, RespSize);
    } else {
        return SendLongResponse (hReq, pRespData, RespSize);
    }
}
