/*
 * Copyright (c) 2021, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __OSTM_REGS_H__
#define __OSTM_REGS_H__

// OS Timer
#define OSTM0_BASE		(0x12801000U)
#define OSTM0CMP		(OSTM0_BASE + 0x0000U)
#define OSTM0CNT		(OSTM0_BASE + 0x0004U)
#define OSTM0TE			(OSTM0_BASE + 0x0010U)
#define OSTM0TS			(OSTM0_BASE + 0x0014U)
#define OSTM0TT			(OSTM0_BASE + 0x0018U)
#define OSTM0CTL		(OSTM0_BASE + 0x0020U)

#endif	/* __OSTM_REGS_H__ */
