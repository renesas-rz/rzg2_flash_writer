/*
 * Copyright (c) 2015-2017, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/**********************
* COMMAND_UNITE       *
***********************/
#define		COMMAND_UNIT		30
#define		NORMAL_END		0
#define		ERROR_END		1

/**********************
* MESSAGE LINE       *
***********************/
#define		START_MESS_MON_LINE	2
#define		ALL_HELP_MESS_LINE	30

/**********************
* TABLE PARAMETER     *
***********************/
#define		TBL_END				0
#define		MESSAGE_END			0
#define		DISABLE				0x00
#define		ENABLE				0x01

#ifdef AArch32
#define			ARM_CPSR		0
#define			ARM_R0			1
#define			ARM_R1			2
#define			ARM_R2			3
#define			ARM_R3			4
#define			ARM_R4			5
#define			ARM_R5			6
#define			ARM_R6			7
#define			ARM_R7			8
#define			ARM_R8			9
#define			ARM_R9			10
#define			ARM_R10			11
#define			ARM_R11			12
#define			ARM_R12			13
//#define			ARM_R13(SP)		13
//#define			ARM_R14(LR)		14
//#define			ARM_R15(PC)		15
#define			ARM_R14(LR)		14
#define			ARM_R15(PC)		15
#define			ARM_R13(SP)		16
#endif

/****************************************************************/
/*                                                              */
/*   Command Table Struct                                       */
/*                                                              */
/****************************************************************/
typedef struct protect_add {
	long		startAdd;
	long		endAdd;
} protect_add;

typedef struct com_menu {
	char		*comStr;
	void		(*comProg)();	 /* Program Pointer	*/
	protect_add *protectAdd;
} com_menu;

