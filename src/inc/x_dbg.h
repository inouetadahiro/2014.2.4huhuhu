/*
 * Ported to Linux from -- [dtv_dbg.h,v1.4,kobashi]
 */

#ifndef __X_DBG_H__
#define __X_DBG_H__

#include "datatypes.h"
#include <stdarg.h>

#define DBG_CRIT                0x80000000      /* �N���e�B�J�����O */
#define DBG_EMRG                0x40000000      /* �G�}�[�W�F���V�[���O */
#define DBG_ERR                 0x20000000      /* �G���[���O */
#define DBG_WARN                0x10000000      /* ���[�j���O���O */
#define DBG_SMSG                0x00000001      /* ���M���b�Z�[�W���O */
#define DBG_SPRM                0x00000002      /* ���M���b�Z�[�W�p�����[�^���O */
#define DBG_RMSG                0x00000004      /* ��M���b�Z�[�W���O */
#define DBG_RPRM                0x00000008      /* ��M���b�Z�[�W�p�����[�^���O */
#define DBG_INFO                0x00000010      /* ��񃍃O   */
#define DBG_DEBUG               0x00000020      /* �f�o�b�O���O */
#define DBG_STAT                0x00000040      /* �X�e�[�^�X���O */

#define DBG_CRIT_HDR            "[CRT] : ******** "
#define DBG_EMRG_HDR            "[EMR] : **** "
#define DBG_ERR_HDR             "[ERR] : ** "
#define DBG_WARN_HDR            "[WRN] : * "
#define DBG_SMSG_HDR            "[MSG] :"
#define DBG_SPRM_HDR            "[PRM] :    "
#define DBG_RMSG_HDR            "[MSG] :"
#define DBG_RPRM_HDR            "[PRM] :    "
#define DBG_INFO_HDR            "[INF] :    "
#define DBG_DEBUG_HDR           "[DBG] : #### "
#define DBG_STAT_HDR            "[STA] :    "
#define DBG_INVALID_HDR         ""

UCHAR          *dbgPrintHeader (UINT);
UCHAR          *dbgPrintTaskName (void);
void            ConsolePrintf (const char *format, ...);
void            SyslogPrintf  (const char *format, ...);

#define DBG_HEADER(X)   dbgPrintHeader(X)
#define DBG_TSK()       dbgPrintTaskName()

#define  SYSLOG_PRINTF(format, ...)                             \
do{                                                             \
        SyslogPrintf(format, __VA_ARGS__);                      \
}while(0)

#define  CONSOLE_PRINTF(format, ...)                            \
do{                                                             \
        ConsolePrintf(format, __VA_ARGS__);                     \
}while(0)

#define X_PRINTF(mode, fmt, ...)                              \
do{                                                             \
        extern BOOL dbgPrintCheck(UINT);                        \
        if(dbgPrintCheck(mode) == TRUE){                        \
            if(mode == DBG_DEBUG){                              \
                CONSOLE_PRINTF("[%s] : %s"fmt, DBG_TSK(),       \
                          DBG_HEADER(mode), __VA_ARGS__);       \
            }else {                                             \
                SYSLOG_PRINTF("[%s] : %s"fmt, DBG_TSK(),        \
                          DBG_HEADER(mode), __VA_ARGS__);       \
            }                                                   \
        }                                                       \
}while(0)



/* To add support for messages print*/
typedef struct
{
    UINT            msg_id;
    BYTE           *msg_name_en;
    BYTE           *msg_name_jp;
} PRINT_MSG_T;
extern const PRINT_MSG_T gDtvPrintMsgTbl[];


#endif /* __RADAR_DBG_H__ */
