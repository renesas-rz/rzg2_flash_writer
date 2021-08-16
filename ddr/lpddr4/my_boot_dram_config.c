/*
 * Copyright (c) 2021, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdint-gcc.h>
#include "boot_init_dram_regdef.h"
#include "boot_init_dram_config.h"

uint32_t boardcnf_get_brd_type(void)
{
	return 0;
}

const struct _boardcnf boardcnfs[BOARDNUM] = {
/* Only 1 entire for my board */
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
}
};
