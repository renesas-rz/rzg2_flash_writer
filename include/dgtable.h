/*******************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only
* intended for use with Renesas products. No other uses are authorized. This
* software is owned by Renesas Electronics Corporation and is protected under
* all applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT
* LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
* AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.
* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS
* ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE
* FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR
* ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE
* BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software
* and to discontinue the availability of this software. By using this software,
* you agree to the additional terms and conditions found by accessing the
* following link:
* http://www.renesas.com/disclaimer
* Copyright (C) 2021 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/ 

/**********************
* COMMAND_UNITE       *
***********************/
#define		COMMAND_UNIT		20
#define		NORMAL_END		0
#define		ERROR_END		1

/**********************
* MESSAGE LINE       *
***********************/
#define		START_MESS_MON_LINE	2
#define		ALL_HELP_MESS_LINE	20

/**********************
* TABLE PARAMETER     *
***********************/
#define		TBL_END			0
#define		MESSAGE_END		0
#define		DISABLE			0x00
#define		ENABLE			0x01

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

