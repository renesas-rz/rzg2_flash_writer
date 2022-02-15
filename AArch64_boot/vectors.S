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
/*******************************************************************************
 * File Name    : vectors.s
 * Version      : 0.9
 * Description  : vector table for the Cortex A53 processor
 ******************************************************************************/

.org 0
.text

.globl _boot
.globl _vector_table

.org 0

.section .vectors, "a"

_vector_table:

.set	VBAR, _vector_table

.org VBAR
	b	_boot

.org (VBAR + 0x80)
	b	.

.org (VBAR + 0x100)
	b	.

.org (VBAR + 0x180)
	b	.


.org (VBAR + 0x200)
	b	.

.org (VBAR + 0x280)
	b	.

.org (VBAR + 0x300)
	b	.

.org (VBAR + 0x380)
	b	.



.org (VBAR + 0x400)
	b	.

.org (VBAR + 0x480)
	b	.

.org (VBAR + 0x500)
	b	.

.org (VBAR + 0x580)
	b	.

.org (VBAR + 0x600)
	b	.

.org (VBAR + 0x680)
	b	.

.org (VBAR + 0x700)
	b	.

.org (VBAR + 0x780)
	b	.
	
.org (VBAR + 0x1000)

.end
