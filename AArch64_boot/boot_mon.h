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
;#RWDT
.EQU	RWDT_RWTCNT	,	0xE6020000		;#RCLK watchdog timer counter
.EQU	RWDT_RWTCSRA	,	0xE6020004		;#RCLK watchdog timer control/status register A
.EQU	RWDT_RWTCSRB	,	0xE6020008		;#RCLK watchdog timer control/status register B

;#SystemWDT
.EQU	SYSWDT_WTCNT	,	0xE6030000		;#watchdog timer counter
.EQU	SYSWDT_WTCSRA	,	0xE6030004		;#watchdog timer control/status register A
.EQU	SYSWDT_WTCSRB	,	0xE6030008		;#watchdog timer control/status register B

.EQU 	PRR		,	0xFFF00044		;#Product Register
