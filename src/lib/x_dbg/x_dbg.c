/*
 * Ported to Linux from -- [dtv_dbg.c,v1.24,kobashi]
 */

#include <syslog.h>
#include <stdio.h>
#include <string.h>
#include "tsk_id.h"
#include "x_dbg.h"

extern USHORT   gCurrentTaskID;

UINT            dMgrPrintMode = DBG_INFO | DBG_DEBUG | DBG_STAT;
UINT            dWtdPrintMode = DBG_INFO | DBG_DEBUG | DBG_STAT;
UINT            dDbgxPrintMode = DBG_INFO | DBG_DEBUG | DBG_STAT;
UINT            dTask1PrintMode = DBG_INFO | DBG_DEBUG | DBG_STAT;
UINT            dTask2PrintMode = DBG_INFO | DBG_DEBUG | DBG_STAT;
UINT            dTask3PrintMode = DBG_INFO | DBG_DEBUG | DBG_STAT;
UINT            dTask4PrintMode = DBG_INFO | DBG_DEBUG | DBG_STAT;
UINT            dTask5PrintMode = DBG_INFO | DBG_DEBUG | DBG_STAT;
UINT            dTask6PrintMode = DBG_INFO | DBG_DEBUG | DBG_STAT;
UINT            dTask7PrintMode = DBG_INFO | DBG_DEBUG | DBG_STAT;
UINT            dTask8PrintMode = DBG_INFO | DBG_DEBUG | DBG_STAT;

typedef struct
{
    UINT            tsk_id;
    UINT           *mode;
} DBG_PRINT_TBL;

UINT
GetTaskPrintMode (USHORT tsk_id)
{
    UINT            mode;

    switch (tsk_id) {
    case TSKID_MGR:
        mode = dMgrPrintMode;
        break;
    case TSKID_WATCH:
        mode = dWtdPrintMode;
        break;
    case TSKID_DBGX:
        mode = dDbgxPrintMode;
        break;
    case TSKID_TASK1:
        mode = dTask1PrintMode;
        break;
    case TSKID_TASK2:
        mode = dTask2PrintMode;
        break;
    case TSKID_TASK3:
        mode = dTask3PrintMode;
        break;
    case TSKID_TASK4:
        mode = dTask4PrintMode;
        break;
    case TSKID_TASK5:
        mode = dTask5PrintMode;
        break;
    case TSKID_TASK6:
        mode = dTask6PrintMode;
        break;
    case TSKID_TASK7:
        mode = dTask7PrintMode;
        break;
    case TSKID_TASK8:
        mode = dTask8PrintMode;
        break;
    default:
        mode = 0;
        break;
    }

    return mode;
}

UCHAR*
GetTskName (USHORT tsk_id)
{
    UCHAR          *name;

    switch (tsk_id) {
    case TSKID_MGR:
        name = "MGR";
        break;
    case TSKID_WATCH:
        name = "WTD";
        break;
    case TSKID_DBGX:
        name = "DBGX";
        break;
    case TSKID_TASK1:
        name = "TSK1";
        break;
    case TSKID_TASK2:
        name = "TSK2";
        break;
    case TSKID_TASK3:
        name = "TSK3";
        break;
    case TSKID_TASK4:
        name = "TSK4";
        break;
    case TSKID_TASK5:
        name = "TSK5";
        break;
    case TSKID_TASK6:
        name = "TSK6";
        break;
    case TSKID_TASK7:
        name = "TSK7";
        break;
    case TSKID_TASK8:
        name = "TSK8";
        break;
    default:
        name = "---";
        break;
    }

    return name;
}

UCHAR*
dbgPrintHeader (UINT mode)
{
    UCHAR          *header;

    switch (mode) {
    case DBG_CRIT:
        header = DBG_CRIT_HDR;
        break;
    case DBG_EMRG:
        header = DBG_EMRG_HDR;
        break;
    case DBG_ERR:
        header = DBG_ERR_HDR;
        break;
    case DBG_WARN:
        header = DBG_WARN_HDR;
        break;
    case DBG_SMSG:
        header = DBG_SMSG_HDR;
        break;
    case DBG_SPRM:
        header = DBG_SPRM_HDR;
        break;
    case DBG_RMSG:
        header = DBG_RMSG_HDR;
        break;
    case DBG_RPRM:
        header = DBG_RPRM_HDR;
        break;
    case DBG_INFO:
        header = DBG_INFO_HDR;
        break;
    case DBG_DEBUG:
        header = DBG_DEBUG_HDR;
        break;
    case DBG_STAT:
        header = DBG_STAT_HDR;
        break;
    default:
        header = DBG_INVALID_HDR;
        break;
    }

    return header;
}


BOOL
dbgPrintCheck (UINT mode)
{

    UINT            dprint_mode;

    if ((mode == DBG_CRIT) || (mode == DBG_EMRG)
        || (mode == DBG_ERR) || (mode == DBG_WARN)) {
        return TRUE;
    }

    dprint_mode = GetTaskPrintMode (gCurrentTaskID);
    if (dprint_mode & mode) {
        return TRUE;
    }

    return FALSE;
}

UCHAR*
dbgPrintTaskName (void)
{
    return GetTskName (gCurrentTaskID);
}

void
ConsolePrintf (const char *format, ...)
{
    va_list         arg_list;

    va_start (arg_list, format);
    vprintf (format, arg_list);
    va_end (arg_list);
}

static BOOL     gSyslogInit = FALSE;

void
SyslogPrintf (const char *format, ...)
{
    va_list         arg_list;

    if (FALSE == gSyslogInit) {
        openlog (NULL, LOG_CONS | LOG_PERROR, LOG_KERN);
        gSyslogInit = TRUE;
    }

    va_start (arg_list, format);
    vsyslog (LOG_NOTICE, format, arg_list);
    va_end (arg_list);
}
