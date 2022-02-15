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
#define CONFIG_ARM_ERRATA_855873 1

#define EL3 1

.global _prestart
.global _boot

.global __el3_stack
.global _vector_table

.set EL3_stack,		__el3_stack

.set TT_S1_FAULT,	0x0
.set TT_S1_TABLE,	0x3

.set vector_base,	_vector_table
.set rvbar_base,	0xA3F02020	// CA53_RVA0CRL

.set counterfreq,	24000000
.set MODE_EL1, 0x5
.set DAIF_BIT,	0x1C0

.section .boot,"ax"


/* this initializes the various processor modes */

_prestart:
_boot:
	mov      x0, #0
	mov      x1, #0
	mov      x2, #0
	mov      x3, #0
	mov      x4, #0
	mov      x5, #0
	mov      x6, #0
	mov      x7, #0
	mov      x8, #0
	mov      x9, #0
	mov      x10, #0
	mov      x11, #0
	mov      x12, #0
	mov      x13, #0
	mov      x14, #0
	mov      x15, #0
	mov      x16, #0
	mov      x17, #0
	mov      x18, #0
	mov      x19, #0
	mov      x20, #0
	mov      x21, #0
	mov      x22, #0
	mov      x23, #0
	mov      x24, #0
	mov      x25, #0
	mov      x26, #0
	mov      x27, #0
	mov      x28, #0
	mov      x29, #0
	mov      x30, #0

OKToRun:

	mrs	x0, currentEL
	cmp	x0, #0xC
	beq	InitEL3

	b 	error			// go to error if current exception level is neither EL3
InitEL3:
	/*Set vector table base address*/
	ldr	x1, =vector_base
	msr	VBAR_EL3,x1

	/* Set reset vector address */
	/* Get the cpu ID */
	mrs  x0, MPIDR_EL1
	and  x0, x0, #0xFF
	mov  w0, w0
	ldr	 w2, =rvbar_base
	/* calculate the rvbar base address for particular CPU core */
	mov	 w3, #0x8
	mul	 w0, w0, w3
	add	 w2, w2, w0
	/* store vector base address to RVBAR */
	mov  w3, w1
	str  w3, [x2]
	lsr  x3, x1, #32
	mov  w3, w3
	str  w3, [x2, #4]

	/*Define stack pointer for current exception level*/
	ldr	 x2,=EL3_stack
	mov	 sp,x2

	/* Enable Trapping of SIMD/FPU register for standalone BSP */
	mov      x0, #0
	msr      CPTR_EL3, x0
	isb

	/* program the counter frequency */
	bl SYC_enable
	bl CMN_InitSysCnt
	ldr	x0, =counterfreq
	msr	CNTFRQ_EL0, x0

	/*Enable hardware coherency between cores*/
	mrs      x0, S3_1_c15_c2_1  	//Read EL1 CPU Extended Control Register
	orr      x0, x0, #(1 << 6)  	//Set the SMPEN bit
	msr      S3_1_c15_c2_1, x0  	//Write EL1 CPU Extended Control Register
	isb

	/**********************************************
	* Set up memory attributes
	* This equates to:
	* 0 = b01000100 = Normal, Inner/Outer Non-Cacheable
	* 1 = b11111111 = Normal, Inner/Outer WB/WA/RA
	* 2 = b00000000 = Device-nGnRnE
	* 3 = b00000100 = Device-nGnRE
	* 4 = b10111011 = Normal, Inner/Outer WT/WA/RA
	**********************************************/
	ldr      x1, =0x000000BB0400FF44
	msr      MAIR_EL3, x1

	/**********************************************
	 * Set up TCR_EL3
	 * Physical Address Size PS =  001 -> 36bits 64GB
	 * Granual Size TG0 = 00 -> 4KB
	 * size offset of the memory region T0SZ = 30 -> (region size 2^(64-30) = 2^34)
	 ***************************************************/
	ldr     x1,=0x8081351E
	msr     TCR_EL3, x1
	isb

	/* Enable SError Exception for asynchronous abort */
	mrs 	x1,DAIF
	bic	x1,x1,#(0x1<<8)
	msr	DAIF,x1

	/* Configure SCTLR_EL3 */
	ldr	x1, = 0x30C50830
	orr	x1, x1, #(1 << 3)	//Enable SP alignment check
	msr	SCTLR_EL3, x1
	dsb	sy
	isb

	b 	 _startup		//jump to start

	b 	error			// present exception level and selected exception level mismatch

error: 	b	error

_startup:

	/* clear bss section */
	mov	X0, #0x0
	ldr	X1, =__bss_start
	ldr	X2, =__bss_end
bss_loop:
	cmp	X1,X2
	bge	bss_end
	str	X0, [X1], #8
	b	bss_loop
bss_end:

	bl		Main

exit:	/* should never get here */
	bl		exit

.end

