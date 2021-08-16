/*
 * Copyright (c) 2015-2019, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include "boot_init_dram_regdef.h"
#include "boot_init_dram_config.h"

extern uint32_t boardcnf_get_brd_type(void);
extern const struct _boardcnf boardcnfs[];

/*******************************************************************************
 *	Set DRAM ODT , VREFca , Derating  condtition
 ******************************************************************************/
/* #define _def_LPDDR4_ODT	0x36	// MR11 */
/* #define _def_LPDDR4_ODT	0x66	// MR11 */
/* #define _def_LPDDR4_VREFCA	0x11	// MR12 */
/* #define JS2_DERATE		1	// 1: Temperature Derating */
#define DDR_FAST_INIT

/*******************************************************************************
 *	EXTAL CLOCK DEFINITION
 *	PLEASE DEFINE HOW TO JUDGE BOARD CLK
 ******************************************************************************/
/*
 * RENESAS SALVATOR/KRIEK BOARD EXAMPLE
 * judge by md14/md13
 *
 * 16.66MHz CLK,DIV= 50,3  (md14,md13==0,0)
 * 20.00MHz CLK,DIV= 60,3  (md14,md13==0,1)
 * 25.00MHz CLK,DIV= 75,3  (md14,md13==1,0)
 * 33.33MHz CLK,DIV=100,3  (md14,md13==1,1)
 */
void boardcnf_get_brd_clk(uint32_t brd, uint32_t *clk, uint32_t *div)
{
	uint32_t md;

	md = (mmio_read_32(RST_MODEMR) >> 13) & 0x3;
	switch (md)
	{
		case 0x0:
			*clk = 50;
			*div = 3;
		break;
		case 0x1:
			*clk = 60;
			*div = 3;
		break;
		case 0x2:
			*clk = 75;
			*div = 3;
		break;
		case 0x3:
			*clk = 100;
			*div = 3;
		break;
	}
	(void)brd;
}

/*******************************************************************************
 *	DDR MBPS TARGET
 *	PLEASE DEFINE HOW TO JUDGE DDR BPS
 ******************************************************************************/
/*
 * DDRxxxx (judge_ by md19,17) : 0
 * DDR3200 (md19,17==0,0) : 3200
 * DDR2800 (md19,17==0,1) : 2800
 * DDR2400 (md19,17==1,0) : 2400
 * DDR1600 (md19,17==1,1) : 1600
 */
void boardcnf_get_ddr_mbps(uint32_t brd, uint32_t *mbps, uint32_t *div)
{
	uint32_t md;

	md = (mmio_read_32(RST_MODEMR) >> 17) & 0x05;
	md = (md | (md >> 1)) & 0x03;
	switch (md)
	{
		case 0x0:
			*mbps = 3200;
			*div = 1;
		break;
		case 0x1:
			*mbps = 2800;
			*div = 1;
		break;
		case 0x2:
			*mbps = 2400;
			*div = 1;
		break;
		case 0x3:
			*mbps = 1600;
			*div = 1;
		break;
	}
	(void)brd;
}

/*******************************************************************************
 *	REFRESH TARGET DEFINITION
 *	PLEASE DEFINE _def_REFPERIOD (in ns)
 ******************************************************************************/
/* WARN: do not modify in this IPL version */
/* #define _def_REFPERIOD  1890 */

/*******************************************************************************
 *	PREDEFINED TERM CODE by chip id
 ******************************************************************************/
#define M3_SAMPLE_TT_A84        0xB866CC10, 0x3B250421
#define M3_SAMPLE_TT_A85        0xB866CC10, 0x3AA50421
#define M3_SAMPLE_TT_A86        0xB866CC10, 0x3AA48421
#define M3_SAMPLE_FF_B45        0xB866CC10, 0x3AB00C21
#define M3_SAMPLE_FF_B49        0xB866CC10, 0x39B10C21
#define M3_SAMPLE_FF_B56        0xB866CC10, 0x3AAF8C21
#define M3_SAMPLE_SS_E24        0xB866CC10, 0x3BA39421
#define M3_SAMPLE_SS_E28        0xB866CC10, 0x3C231421
#define M3_SAMPLE_SS_E32        0xB866CC10, 0x3C241421
static const uint32_t TermcodeBySample[20][3] = {
	{	M3_SAMPLE_TT_A84,		0x000158D5	},
	{	M3_SAMPLE_TT_A85,		0x00015955	},
	{	M3_SAMPLE_TT_A86,		0x00015955	},
	{	M3_SAMPLE_FF_B45,		0x00015690	},
	{	M3_SAMPLE_FF_B49,		0x00015753	},
	{	M3_SAMPLE_FF_B56,		0x00015793	},
	{	M3_SAMPLE_SS_E24,		0x00015996	},
	{	M3_SAMPLE_SS_E28,		0x000159D7	},
	{	M3_SAMPLE_SS_E32,		0x00015997	},
	{	0xFFFFFFFF,	0xFFFFFFFF,	0x0001554F	}
};
