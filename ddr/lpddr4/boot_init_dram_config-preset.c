/*
 * Copyright (c) 2021, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <mmio.h>
#include <stdint-gcc.h>
#include "boot_init_dram_regdef.h"
#include "boot_init_dram_config.h"

#define	GPIO_INDT5	0xE605500C
#define	LPDDR4_2RANK	(0x01 << 25)

uint32_t boardcnf_get_brd_type(void) __attribute__((weak));

/*******************************************************************************
 *	PLEASE SET board number or board judge function
 ******************************************************************************/
uint32_t boardcnf_get_brd_type(void)
{
	uint32_t Prr_Product;
	uint32_t judge = 0;
	uint32_t reg;
	uint32_t boardInfo;

	reg = mmio_read_32(PRR);
	Prr_Product = reg & PRR_PRODUCT_MASK;
	if (Prr_Product == PRR_PRODUCT_G2M)
	{
		if (RZG2M_CUT_11 == (reg & PRR_CUT_MASK))
		{
			judge = 1;	/* 2rank setting	*/
		}
		else
		{
			boardInfo = mmio_read_32(GPIO_INDT5);
			if (boardInfo & LPDDR4_2RANK)
			{
				judge = 1;	/* 2rank setting	*/
			}
			else
			{
				judge = 0;	/* 1rank setting	*/
			}
		}
	}
	else if (Prr_Product == PRR_PRODUCT_G2N)
	{
		judge = 3;
	}
	else if (Prr_Product == PRR_PRODUCT_G2H)
	{
		judge = 5;	/* 1rank setting	*/
	}
	return (judge);
}


const struct _boardcnf __attribute__((weak)) boardcnfs[BOARDNUM] = {
/*
 * boardcnf[0] Jiangsu HopeRun Software Co., Ltd. HiHope RZ/G2M board 16Gbit/1rank/2ch board with G2M/SOC
 */
{
	0x03,		/* phyvalid */
	0x01,		/* dbi_en */
	0x02c0,		/* cacs_dly */
	0,		/* cacs_dly_adj */
	0x0300,		/* dqdm_dly_w */
	0x00a0,		/* dqdm_dly_r */
	{
/*ch[0]*/	{
/*ddr_density[]*/	{ 0x04, 0xff },
/*ca_swap*/		0x00345201,
/*dqs_swap*/		0x3201,
/*dq_swap[]*/		{ 0x01672543, 0x45361207, 0x45632107, 0x60715234 },
/*dm_swap[]*/		{ 0x08, 0x08, 0x08, 0x08 },
/*wdqlvl_patt[]*/	WDQLVL_PAT,
/*cacs_adj*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0 },
/*dm_adj_w*/		{ 0, 0, 0, 0 },
/*dqdm_adj_w*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0 },
/*dm_adj_r*/		{ 0, 0, 0, 0 },
/*dqdm_adj_r*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0 }
		},
/*ch[1]*/	{
/*ddr_density[]*/	{ 0x04, 0xff },
/*ca_swap*/		0x00302154,
/*dqs_swap*/		0x2310,
/*dq_swap[]*/		{ 0x01672543, 0x45361207, 0x45632107, 0x60715234 },
/*dm_swap[]*/		{ 0x08, 0x08, 0x08, 0x08 },
/*wdqlvl_patt[]*/	WDQLVL_PAT,
/*cacs_adj*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0 },
/*dm_adj_w*/		{ 0, 0, 0, 0 },
/*dqdm_adj_w*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0 },
/*dm_adj_r*/		{ 0, 0, 0, 0 },
/*dqdm_adj_r*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0 }
		}
	}
},
/*
 * boardcnf[1] Jiangsu HopeRun Software Co., Ltd. HiHope RZ/G2M board 8Gbit/2rank/2ch board with G2M/SOC
 */
{
	0x03,		/* phyvalid */
	0x01,		/* dbi_en */
	0x02c0,		/* cacs_dly */
	0,		/* cacs_dly_adj */
	0x0300,		/* dqdm_dly_w */
	0x00a0,		/* dqdm_dly_r */
	{
/*ch[0]*/	{
/*ddr_density[]*/	{ 0x02, 0x02 },
/*ca_swap*/		0x00345201,
/*dqs_swap*/		0x3201,
/*dq_swap[]*/		{ 0x01672543, 0x45361207, 0x45632107, 0x60715234 },
/*dm_swap[]*/		{ 0x08, 0x08, 0x08, 0x08 },
/*wdqlvl_patt[]*/	WDQLVL_PAT,
/*cacs_adj*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0 },
/*dm_adj_w*/		{ 0, 0, 0, 0 },
/*dqdm_adj_w*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0 },
/*dm_adj_r*/		{ 0, 0, 0, 0 },
/*dqdm_adj_r*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0 }
		},
/*ch[1]*/	{
/*ddr_density[]*/	{ 0x02, 0x02 },
/*ca_swap*/		0x00302154,
/*dqs_swap*/		0x2310,
/*dq_swap[]*/		{ 0x01672543, 0x45361207, 0x45632107, 0x60715234 },
/*dm_swap[]*/		{ 0x08, 0x08, 0x08, 0x08 },
/*wdqlvl_patt[]*/	WDQLVL_PAT,
/*cacs_adj*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0 },
/*dm_adj_w*/		{ 0, 0, 0, 0 },
/*dqdm_adj_w*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0 },
/*dm_adj_r*/		{ 0, 0, 0, 0 },
/*dqdm_adj_r*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0 }
		}
	}
},
/*
 * boardcnf[2] Jiangsu HopeRun Software Co., Ltd. HiHope RZ/G2N board 8Gbit/2rank/1ch board with G2N/SOC
 */
{
	0x01,		/* phyvalid */
	0x01,		/* dbi_en */
	0x0300,		/* cacs_dly */
	0,		/* cacs_dly_adj */
	0x0300,		/* dqdm_dly_w */
	0x00a0,		/* dqdm_dly_r */
	{
/*ch[0]*/	{
/*ddr_density[]*/	{ 0x02, 0x02 },
/*ca_swap*/		0x00345201,
/*dqs_swap*/		0x3201,
/*dq_swap[]*/		{ 0x01672543, 0x45361207, 0x45632107, 0x60715234 },
/*dm_swap[]*/		{ 0x08, 0x08, 0x08, 0x08 },
/*wdqlvl_patt[]*/	WDQLVL_PAT,
/*cacs_adj*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0 },
/*dm_adj_w*/		{ 0, 0, 0, 0 },
/*dqdm_adj_w*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0 },
/*dm_adj_r*/		{ 0, 0, 0, 0 },
/*dqdm_adj_r*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0 }
		}
	}
},
/*
 * boardcnf[3] Jiangsu HopeRun Software Co., Ltd. HiHope RZ/G2N board 16Gbit/2rank/1ch board with G2N/SOC
 */
{
	0x01,		/* phyvalid */
	0x01,		/* dbi_en */
	0x0300,		/* cacs_dly */
	0,		/* cacs_dly_adj */
	0x0300,		/* dqdm_dly_w */
	0x00a0,		/* dqdm_dly_r */
	{
/*ch[0]*/	{
/*ddr_density[]*/	{ 0x04, 0x04 },
/*ca_swap*/		0x00345201,
/*dqs_swap*/		0x3201,
/*dq_swap[]*/		{ 0x01672543, 0x45361207, 0x45632107, 0x60715234 },
/*dm_swap[]*/		{ 0x08, 0x08, 0x08, 0x08 },
/*wdqlvl_patt[]*/	WDQLVL_PAT,
/*cacs_adj*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0 },
/*dm_adj_w*/		{ 0, 0, 0, 0 },
/*dqdm_adj_w*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0 },
/*dm_adj_r*/		{ 0, 0, 0, 0 },
/*dqdm_adj_r*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0 }
		}
	}
},
/*
 * boardcnf[4] Jiangsu HopeRun Software Co., Ltd. HiHope RZ/G2H board 8Gbit/2rank/2ch board with G2M/SOC
 */
{
	0x05,		/* phyvalid */
	0x01,		/* dbi_en */
	0x0300,		/* cacs_dly */
	0,		/* cacs_dly_adj */
	0x0300,		/* dqdm_dly_w */
	0x00a0,		/* dqdm_dly_r */
	{
/*ch[0]*/	{
/*ddr_density[]*/	{ 0x02, 0x02 },
/*ca_swap*/		0x00345201,
/*dqs_swap*/		0x3201,
/*dq_swap[]*/		{ 0x01672543, 0x45367012, 0x45632107, 0x60715234 },
/*dm_swap[]*/		{ 0x08, 0x08, 0x08, 0x08 },
/*wdqlvl_patt[]*/	WDQLVL_PAT,
/*cacs_adj*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0 },
/*dm_adj_w*/		{ 0, 0, 0, 0 },
/*dqdm_adj_w*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0 },
/*dm_adj_r*/		{ 0, 0, 0, 0 },
/*dqdm_adj_r*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0 }
		},
/*ch[1]*/	{	// for DRAM_SPLIT_2CH
/*ddr_density[]*/	{ 0x02, 0x02 },
/*ca_swap*/		0x00302154,
/*dqs_swap*/		0x2310,
/*dq_swap[]*/		{ 0x01672543, 0x45361207, 0x45632107, 0x60715234 },
/*dm_swap[]*/		{ 0x08, 0x08, 0x08, 0x08 },
/*wdqlvl_patt[]*/	WDQLVL_PAT,
/*cacs_adj*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0 },
/*dm_adj_w*/		{ 0, 0, 0, 0 },
/*dqdm_adj_w*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0 },
/*dm_adj_r*/		{ 0, 0, 0, 0 },
/*dqdm_adj_r*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0 }
		},
/*ch[2]*/	{	// for DRAM_SPLIT_NON
/*ddr_density[]*/	{ 0x02, 0x02 },
/*ca_swap*/		0x00302154,
/*dqs_swap*/		0x2310,
/*dq_swap[]*/		{ 0x01672543, 0x45361207, 0x45632107, 0x60715234 },
/*dm_swap[]*/		{ 0x08, 0x08, 0x08, 0x08 },
/*wdqlvl_patt[]*/	WDQLVL_PAT,
/*cacs_adj*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0 },
/*dm_adj_w*/		{ 0, 0, 0, 0 },
/*dqdm_adj_w*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0 },
/*dm_adj_r*/		{ 0, 0, 0, 0 },
/*dqdm_adj_r*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0 }
		},
/*ch[3]*/	{ // Dummy
/*ddr_density[]*/	{ 0xff, 0xff },
/*ca_swap*/		0,
/*dqs_swap*/		0,
/*dq_swap[]*/		{ 0, 0, 0, 0 },
/*dm_swap[]*/		{ 0, 0, 0, 0 },
/*wdqlvl_patt[]*/	WDQLVL_PAT,
/*cacs_adj*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0 },
/*dm_adj_w*/		{ 0, 0, 0, 0 },
/*dqdm_adj_w*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0 },
/*dm_adj_r*/		{ 0, 0, 0, 0 },
/*dqdm_adj_r*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0 }
		}
	}
},
/*
 * boardcnf[5] Jiangsu HopeRun Software Co., Ltd. HiHope RZ/G2H board 16Gbit/1rank/2ch board with G2H/SOC
 */
{
	0x05,		/* phyvalid */
	0x01,		/* dbi_en */
	0x0300,		/* cacs_dly */
	0,		/* cacs_dly_adj */
	0x0300,		/* dqdm_dly_w */
	0x00a0,		/* dqdm_dly_r */
	{
/*ch[0]*/	{
/*ddr_density[]*/	{ 0x04, 0xff },
/*ca_swap*/		0x00345201,
/*dqs_swap*/		0x3201,
/*dq_swap[]*/		{ 0x01672543, 0x45367012, 0x45632107, 0x60715234 },
/*dm_swap[]*/		{ 0x08, 0x08, 0x08, 0x08 },
/*wdqlvl_patt[]*/	WDQLVL_PAT,
/*cacs_adj*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0 },
/*dm_adj_w*/		{ 0, 0, 0, 0 },
/*dqdm_adj_w*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0 },
/*dm_adj_r*/		{ 0, 0, 0, 0 },
/*dqdm_adj_r*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0 }
		},
/*ch[1]*/	{	// for DRAM_SPLIT_2CH
/*ddr_density[]*/	{ 0x04, 0xff },
/*ca_swap*/		0x00302154,
/*dqs_swap*/		0x2310,
/*dq_swap[]*/		{ 0x01672543, 0x45361207, 0x45632107, 0x60715234 },
/*dm_swap[]*/		{ 0x08, 0x08, 0x08, 0x08 },
/*wdqlvl_patt[]*/	WDQLVL_PAT,
/*cacs_adj*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0 },
/*dm_adj_w*/		{ 0, 0, 0, 0 },
/*dqdm_adj_w*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0 },
/*dm_adj_r*/		{ 0, 0, 0, 0 },
/*dqdm_adj_r*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0 }
		},
/*ch[2]*/	{	// for DRAM_SPLIT_NON
/*ddr_density[]*/	{ 0x04, 0xff },
/*ca_swap*/		0x00302154,
/*dqs_swap*/		0x2310,
/*dq_swap[]*/		{ 0x01672543, 0x45361207, 0x45632107, 0x60715234 },
/*dm_swap[]*/		{ 0x08, 0x08, 0x08, 0x08 },
/*wdqlvl_patt[]*/	WDQLVL_PAT,
/*cacs_adj*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0 },
/*dm_adj_w*/		{ 0, 0, 0, 0 },
/*dqdm_adj_w*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0 },
/*dm_adj_r*/		{ 0, 0, 0, 0 },
/*dqdm_adj_r*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0 }
		},
/*ch[3]*/	{ // Dummy
/*ddr_density[]*/	{ 0xff, 0xff },
/*ca_swap*/		0,
/*dqs_swap*/		0,
/*dq_swap[]*/		{ 0, 0, 0, 0 },
/*dm_swap[]*/		{ 0, 0, 0, 0 },
/*wdqlvl_patt[]*/	WDQLVL_PAT,
/*cacs_adj*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0 },
/*dm_adj_w*/		{ 0, 0, 0, 0 },
/*dqdm_adj_w*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0 },
/*dm_adj_r*/		{ 0, 0, 0, 0 },
/*dqdm_adj_r*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0 }
		}
	}
}
};
