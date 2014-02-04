//----------------------------------------------------------------------------------------------------------------------
//
//	req_id.h
//		概要：IPCリクエスト＆レスポンスコマンド定義用ヘッダ
//
//----------------------------------------------------------------------------------------------------------------------

#ifndef __REQ_ID_H__
#define __REQ_ID_H__

#include "tsk_id.h"

//==============================================================================
//		ID計算用マクロ
//==============================================================================
#define DECLARE_REQ_ID(TSK_ID, REQ_NUM)			( (((USHORT)TSK_ID) << 8) | (((USHORT)REQ_NUM) ) )
#define DECLARE_RESP_ID(TSK_ID, RESP_NUM)		( (((USHORT)TSK_ID) << 8) | (((USHORT)RESP_NUM) ) )

//==============================================================================
//		TASK1 タスク
//==============================================================================
#define TASK1_REQUEST1							DECLARE_REQ_ID	(TSKID_TASK1, 0x01)
#define TASK1_RESPONSE1							DECLARE_RESP_ID	(TSKID_TASK1, 0x01)
#define TASK1_REQUEST2							DECLARE_REQ_ID	(TSKID_TASK1, 0x02)
#define TASK1_RESPONSE2							DECLARE_RESP_ID	(TSKID_TASK1, 0x02)

//==============================================================================
//		TASK2 タスク
//==============================================================================
#define TASK2_REQUEST1							DECLARE_REQ_ID	(TSKID_TASK2, 0x01)
#define TASK2_RESPONSE1							DECLARE_RESP_ID	(TSKID_TASK2, 0x01)
#define TASK2_REQUEST2							DECLARE_REQ_ID	(TSKID_TASK2, 0x02)
#define TASK2_RESPONSE2							DECLARE_RESP_ID	(TSKID_TASK2, 0x02)

#endif //__REQ_ID_H__
