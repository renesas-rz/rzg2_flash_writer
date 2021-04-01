/*
 * Copyright (c) 2021, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __SCIF_REGS_H__
#define __SCIF_REGS_H__

/* SCIF ch0 */
#define SCIF0_SMR		(RZG2L_SCIF0_BASE + 0x0000U)
#define SCIF0_BRR		(RZG2L_SCIF0_BASE + 0x0002U)
#define SCIF0_MDDR		(RZG2L_SCIF0_BASE + 0x0002U)
#define SCIF0_SCR		(RZG2L_SCIF0_BASE + 0x0004U)
#define SCIF0_FTDR		(RZG2L_SCIF0_BASE + 0x0006U)
#define SCIF0_FSR		(RZG2L_SCIF0_BASE + 0x0008U)
#define SCIF0_FRDR		(RZG2L_SCIF0_BASE + 0x000AU)
#define SCIF0_FCR		(RZG2L_SCIF0_BASE + 0x000CU)
#define SCIF0_FDR		(RZG2L_SCIF0_BASE + 0x000EU)
#define SCIF0_SPTR		(RZG2L_SCIF0_BASE + 0x0010U)
#define SCIF0_LSR		(RZG2L_SCIF0_BASE + 0x0012U)
#define SCIF0_SEMR		(RZG2L_SCIF0_BASE + 0x0014U)

#endif	/* __SCIF_REGS_H__ */
