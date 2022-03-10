/*
 * Copyright (c) 2022, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __SCIF_REGS_H__
#define __SCIF_REGS_H__

/* SCIF ch0 */
#define SCIF0_SMR		(RZF_SCIF0_BASE + 0x0000U)
#define SCIF0_BRR		(RZF_SCIF0_BASE + 0x0002U)
#define SCIF0_MDDR		(RZF_SCIF0_BASE + 0x0002U)
#define SCIF0_SCR		(RZF_SCIF0_BASE + 0x0004U)
#define SCIF0_FTDR		(RZF_SCIF0_BASE + 0x0006U)
#define SCIF0_FSR		(RZF_SCIF0_BASE + 0x0008U)
#define SCIF0_FRDR		(RZF_SCIF0_BASE + 0x000AU)
#define SCIF0_FCR		(RZF_SCIF0_BASE + 0x000CU)
#define SCIF0_FDR		(RZF_SCIF0_BASE + 0x000EU)
#define SCIF0_SPTR		(RZF_SCIF0_BASE + 0x0010U)
#define SCIF0_LSR		(RZF_SCIF0_BASE + 0x0012U)
#define SCIF0_SEMR		(RZF_SCIF0_BASE + 0x0014U)

#endif	/* __SCIF_REGS_H__ */
