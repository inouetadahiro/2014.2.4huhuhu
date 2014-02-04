//----------------------------------------------------------------------------------------------------------------------
//
//	ipc_command.h
//		概要：IPCコマンド共通構造体ヘッダ
//
//----------------------------------------------------------------------------------------------------------------------

#ifndef __IPC_COMMAND_H__
#define __IPC_COMMAND_H__

#include "datatypes.h"

/*	定数	*/
#define sys_OK		(1)
#define sys_NG		(0)

#pragma pack(1)

/* Start user define. */

//task1

typedef struct task1_a1_request_t {
	BYTE	reserve[8];
} TASK1_A1_REQUEST_T;

typedef struct task1_a1_response_t {
	BYTE	response_id;				//	応答内容
} TASK1_A1_RESPONSE_T;

typedef struct task1_a2_request_t {
	BYTE	reserve[8];
} TASK1_A2_REQUEST_T;

/*	MAIN_READ_MACHINE_INFO_RESPONSE	*/
typedef struct task1_a2_response_t {
	BYTE	response_id;				//	応答内容
	BYTE	reserve[8];
} TASK1_A2_RESPONSE_T;

/* End user define. */

#pragma pack()
#endif	/*	__IPC_COMMAND_H__	*/
