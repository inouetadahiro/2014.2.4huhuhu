//----------------------------------------------------------------------------------------------------------------------
//
//	notify.h
//		概要：タスク間通信 NOTIFYコマンド定義用ヘッダ
//
//----------------------------------------------------------------------------------------------------------------------

#ifndef __NOTIFY_ID_H__
#define __NOTIFY_ID_H__

//==============================================================================
//		タスク毎のID定義
//==============================================================================
#define TASK1_NOTIFY_OFFSET			(0)
#define TASK2_NOTIFY_OFFSET			(20)
#define TASK3_NOTIFY_OFFSET			(40)

//==============================================================================
//		Notifyコマンド定義
//==============================================================================

/*	TASK1 タスク	*/
#define TASK1_NOTIFY1							(TASK1_NOTIFY_OFFSET + 1)
#define TASK1_NOTIFY2							(TASK1_NOTIFY_OFFSET + 2)

/*	TASK2 タスク	*/
#define TASK2_NOTIFY1							(TASK2_NOTIFY_OFFSET + 1)
#define TASK2_NOTIFY2							(TASK2_NOTIFY_OFFSET + 2)

/*	RD/TASK3 タスク	*/
#define TASK3_NOTIFY1							(TASK3_NOTIFY_OFFSET + 1)
#define TASK3_NOTIFY2							(TASK3_NOTIFY_OFFSET + 2)

#endif // __NOTIFY_ID_H__
