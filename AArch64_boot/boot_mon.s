/*
 * Copyright (c) 2015-2021, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*
  W0-W30 : 32bit Register (W30=Link Register)
  X0-X30 : 64bit Register (X30=Link Register)
  WZR    : 32bit Zero Register
  XZR    : 64bit Zero Register
  WSP    : 32bit Stack Pointer
  SP     : 64bit Stack Pointer
*/

    .section BOOT_MON, "ax"
	.include		"boot_mon.h"
	.align	4

	.global Register_init

	.extern InitScif

// Initialize registers
Register_init:
	LDR		X0, =0
	LDR		X1, =0
	LDR		X2, =0
	LDR		X3, =0
	LDR		X4, =0
	LDR		X5, =0
	LDR		X6, =0
	LDR		X7, =0
	LDR		X8, =0
	LDR		X9, =0
	LDR		X10, =0
	LDR		X11, =0
	LDR		X12, =0
	LDR		X13, =0
	LDR		X14, =0
	LDR		X15, =0
	LDR		X16, =0
	LDR		X17, =0
	LDR		X18, =0
	LDR		X19, =0
	LDR		X20, =0
	LDR		X21, =0
	LDR		X22, =0
	LDR		X23, =0
	LDR		X24, =0
	LDR		X25, =0
	LDR		X26, =0
	LDR		X27, =0
	LDR		X28, =0
	LDR		X29, =0
	LDR		X30, =0

Set_EnableRAM:
	LDR	W0, =0x11040000		// TZC-400(ACPU)

	LDR	W1, [X0, #0x0110]	// REGION_ATTRIBUTES_0
//	AND	W1, W1, #0xFFFFFFFF
	MOV	W2, #0x3FFFFFFF
	AND	W1, W1, W2
	STR	W1, [X0, #0x0110]
	LDR	W1, [X0, #0x0114]	// REGION_ID_ACCESS_0
	ORR	W1, W1, #0x000F000F
	STR	W1, [X0, #0x0114]
	LDR	W1, [X0, #0x0008]	// GATE_KEEPER
	ORR	W1, W1, #0x0000000F
	STR	W1, [X0, #0x0008]
	LDR	W1, [X0, #0x000C]	// SPECULATION_CTRL
	ORR	W1, W1, #0x00000003
	STR	W1, [X0, #0x000C]

	LDR	W0, =0x11050000		// TZC-400(MCPU)

	LDR	W1, [X0, #0x0110]	// REGION_ATTRIBUTES_0
//	AND	W1, W1, #0xFFFFFFFF
	MOV	W2, #0x3FFFFFFF
	AND	W1, W1, W2
	STR	W1, [X0, #0x0110]
	LDR	W1, [X0, #0x0114]	// REGION_ID_ACCESS_0
	ORR	W1, W1, #0x000F000F
	STR	W1, [X0, #0x0114]
	LDR	W1, [X0, #0x0008]	// GATE_KEEPER
	ORR	W1, W1, #0x0000000F
	STR	W1, [X0, #0x0008]
	LDR	W1, [X0, #0x000C]	// SPECULATION_CTRL
	ORR	W1, W1, #0x00000003
	STR	W1, [X0, #0x000C]

	LDR	W0, =0x011060000	// TZC-400(SPI Multi)

	LDR	W1, [X0, #0x0110]	// REGION_ATTRIBUTES_0
//	AND	W1, W1, #0xFFFFFFFF
	MOV	W2, #0x3FFFFFFF
	AND	W1, W1, W2
	STR	W1, [X0, #0x0110]
	LDR	W1, [X0, #0x0114]	// REGION_ID_ACCESS_0
	ORR	W1, W1, #0x000F000F
	STR	W1, [X0, #0x0114]
	LDR	W1, [X0, #0x0008]	// GATE_KEEPER
	ORR	W1, W1, #0x0000000F
	STR	W1, [X0, #0x0008]
	LDR	W1, [X0, #0x000C]	// SPECULATION_CTRL
	ORR	W1, W1, #0x00000003
	STR	W1, [X0, #0x000C]

// Loader
    LDR x0, =__STACKS_END__
    MSR SP_EL0,x0
    MSR SP_EL1,x0
    MSR SP_EL2,x0
    MOV sp,x0
    MSR ELR_EL1,x0
    MSR ELR_EL2,x0
    MSR ELR_EL3,x0
    MSR SPSR_EL1,x0
    MSR SPSR_EL2,x0
    MSR SPSR_EL3,x0


// Board Initialize
Init_set_WDT:
	LDR		X0, =WDT_WDTCNT_PON
	MOV		W1, #0x00000001
	STR		W1, [X0]


// Enable cache
//	mov     x1, #(SCTLR_I_BIT | SCTLR_A_BIT | SCTLR_SA_BIT)
	mrs     x0, sctlr_el3
	orr     x0, x0, #(0x1 << 12)
	orr     x0, x0, #(0x1 <<  1)
	orr     x0, x0, #(0x1 <<  3)
	msr     sctlr_el3, x0
	isb


	/* clear bss section */
	mov	    W0, #0x0
	ldr	X1, =__BSS_START__
	ldr	X2, =__BSS_SIZE__
bss_loop:
	subs	X2, X2, #4
	bcc	    bss_end
	str	    W0, [X1, X2]
	b	    bss_loop
bss_end:

//.ifdef Area0Boot
//	/* copy data section */
//	ldr	X0, =__DATA_COPY_START__
//	ldr	X1, =__DATA_START__
//	ldr	X2, =__DATA_SIZE__
//data_loop:
//	subs	X2, X2, #4
//	bcc	    data_end
//	ldr	    W3, [X0, X2]
//	str	    W3, [X1, X2]
//	b	    data_loop
//.endif

data_end:

//	BL InitPORT
//	BL InitGPIO
//	BL InitLBSC
	BL InitScif
//	BL InitDram
//
//	cmp	x0, #0
//	beq	2f	/* InitDram success */
//	mov	x19, x0
//
//	ldr	x0, =dram_err_msg
//	mov	x1, #0
//	bl	PutStr
//
//	mov	x0, x19	/* return value of InitDram */
//	ldr	x1, =str_buf
//	ldr	x2, =cnt
//	bl	Hex2Ascii
//	mov	x0, x1
//	mov	x1, #1
//	bl	PutStr
//1:
//	wfi		/* InitDram fail */
//	b	1b

2:
	BL		Main


	.section .rodata
dram_err_msg:
	.asciz "InitDram error=0x"

	.section .bss
	.align	4
cnt:
	.space	4
str_buf:
	.space	16

	.end

