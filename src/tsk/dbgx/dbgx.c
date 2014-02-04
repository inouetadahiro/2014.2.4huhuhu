#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "datatypes.h"
#include "ipc.h"
#include "notify_id.h"
#include "tsk_id.h"
#include "req_id.h"
#include "x_dbg.h"
#include "ipc_command.h"
#include "gpio.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>

#define IOCTL_EX

#define DGB()

#include "ctype.h"

#define HEADER        '$'

#define MSG_NUM     7

#define MAX_WORD_LEN    11

#define PARAM_NUM    2048

static const char * MSG_HEADER[] = {
    "Request",
    "Inl",
    "Outl",
    "Kill",
};

int param[PARAM_NUM+2];

static void
readStory(char * str) {
    int i;
    char c;
    char *pc = str;
    char wd[11];
    char cmd;
    int * pi;
    int cnt;
    int id, size;
    unsigned char buf[PARAM_NUM];

    if (*pc++ != '$')
        return;
    for (i = 0; i < MAX_WORD_LEN; i++) {
        c = *pc++;
        if (isalnum(c)) {
            wd[i] = c;
        } else {
            if (i > 0) {
                wd[i] = '\0';
                break;
            } else
                return;
        }
    }
    if (i == MAX_WORD_LEN
            && wd[MAX_WORD_LEN-1] != '\0')
        return;

    for (i = 0; i < MSG_NUM; i++) {
        if (strcmp(wd, MSG_HEADER[i]))
            continue;
        else {
            cmd = i;
            break;
        }
    }
    if (i == MSG_NUM)
        return;
    pi = &param[0];
    cnt = 0;
    while (1) {
        for (i = 0; i < MAX_WORD_LEN; i++) {
            c = *pc++;
            if (isdigit(c)) {
                wd[i] = c;
            } else {
                if (i > 0) {
                    wd[i] = '\0';
                    *pi++ = atoi(wd);
                    cnt++;
                }
                if (c == '\0' || c == LF) {
                    goto next;
                }
                break;
            }
        }
    }
next:

    switch (cmd) {
    case 0:
        id = param[0];
        size = param[1];
        for (i = 0; i < cnt - 2; i++) {
            buf[i] = (unsigned char) param[i+2];
        }
        FakeRequest(id, buf, size);
        break;
    case 1:
//       Ioperm();
//       printf("$Inl Result = %4x\n", Inl (param[0]));
       break;
    case 2:
//       Ioperm();
        printf("$Outl Rcv\n");
//       Ioperm();
//       Outl (param[0], param[1]);
       break;
    case 3:
        printf("$Kill Rcv.\n");
        exit(0);
        break;
    default:
        break;
    }
}

char msgBuf[PARAM_NUM*4];

static int
waitMsg(void) {
    int index = 0;
    char c;

    while (1) {
        c = getchar();
        if (index) {
            msgBuf[index++] = c;
            if (index >= (PARAM_NUM*4)-1)
                index = 0;
            else if (c == LF) {
                msgBuf[index++] = '\0';
                readStory(msgBuf);
                sleep(20);
                index = 0;
            } else if (c == HEADER) {
                index = 0;
                msgBuf[index++] = c;
            } else if (c == '\b') {
                index -= 2;
            }
        } else {
            if (c == HEADER) {
                msgBuf[index++] = c;
            }
        }
    }
    return 0;
}

USHORT gCurrentTaskID = TSKID_DBGX;

int
main () {
    UINT            ret;

    ret = InitializeIpcFramework(gCurrentTaskID);
    if(ret != IPC_SUCCESS) {
        X_PRINTF(DBG_CRIT,"InitializeIpcFramework failed, ret=%d\n", ret);
        exit(1);
    }

    waitMsg();

    return 0;
}
