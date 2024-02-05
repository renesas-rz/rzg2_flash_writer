/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * Startup Code for RISC-V Core
 *
 * Copyright (c) 2017 Microsemi Corporation.
 * Copyright (c) 2017 Padmarao Begari <Padmarao.Begari@microsemi.com>
 *
 * Copyright (C) 2017 Andes Technology Corporation
 * Rick Chen, Andes Technology Corporation <rick@andestech.com>
 */

#define MODE_PREFIX(__suffix) s##__suffix

#define LREG			ld
#define SREG			sd
#define REGBYTES		8
#define RELOC_TYPE		R_RISCV_64
#define SYM_INDEX		0x20
#define SYM_SIZE		0x18

#define CONFIG_SPL_STACK        0x00030000

.section .text
.globl start
start:
	/* mask all interrupts */
	csrw	MODE_PREFIX(ie), zero
/*
 * Set stackpointer in internal
 */
call_board_init_f:
	li	t0, -16
	li	t1, CONFIG_SPL_STACK
	and	sp, t1, t0		/* force 16 byte alignment */

clear_bss:
	la	t0, __BSS_START__
	la	t1, __BSS_END__
	beq	t0, t1, stack_gd_setup

clear_bss_loop:
	SREG	zero, 0(t0)
	addi	t0, t0, REGBYTES
	blt	t0, t1, clear_bss_loop

stack_gd_setup:
	mv	a0, zero
	mv	a1, zero
	jal	Main
