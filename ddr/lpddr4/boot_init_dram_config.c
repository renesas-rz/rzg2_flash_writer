/*
 * Copyright (c) 2015-2019, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#define	GPIO_INDT5	0xE605500C
#define	LPDDR4_2RANK	(0x01 << 25)

/*******************************************************************************
 *	NUMBER OF BOARD CONFIGRATION
 *	PLEASE DEFINE
 ******************************************************************************/
#define BOARDNUM 6
/*******************************************************************************
 *	PLEASE SET board number or board judge function
 ******************************************************************************/
static uint32_t boardcnf_get_brd_type(void)
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

/*******************************************************************************
 *	Set DRAM ODT , VREFca , Derating  condtition
 ******************************************************************************/
/* #define _def_LPDDR4_ODT	0x36	// MR11 */
/* #define _def_LPDDR4_ODT	0x66	// MR11 */
/* #define _def_LPDDR4_VREFCA	0x11	// MR12 */
/* #define JS2_DERATE		1	// 1: Temperature Derating */
#define DDR_FAST_INIT

/*******************************************************************************
 *	BOARD CONFIGRATION
 *	PLEASE DEFINE boardcnfs[]
 ******************************************************************************/
struct _boardcnf_ch {
	/*0x00... 4Gbit/die ( 2Gb/channel)
	 *0x01... 6Gbit/die ( 3Gb/channel)
	 *0x02... 8Gbit/die ( 4Gb/channel) or 4Gb/die (4Gb/channel)
	 *0x03...12Gbit/die ( 6Gb/channel) or 6Gb/die (6Gb/channel)
	 *0x04...16Gbit/die ( 8Gb/channel) or 8Gb/die (8Gb/channel)
//non:	 *0x05...24Gbit/die (12Gb/channel)
//non:	 *0x06...32Gbit/die (16Gb/channel)
	 *0xff...NO_MEMORY
	 */
	uint8_t ddr_density[CS_CNT];
	/* SoC caX([15][14]....[3][2][1][0]) -> MEM caY: */
	uint64_t ca_swap;
	/* SoC dqsX([3][2][1][0]) -> MEM dqsY: */
	uint16_t dqs_swap;
	/* SoC dq([7][6][5][4][3][2][1][0]) -> MEM dqY/dm:  (8 means DM) */
	uint32_t dq_swap[SLICE_CNT];
	/* SoC dm -> MEM dqY/dm:  (8 means DM) */
	uint8_t dm_swap[SLICE_CNT];
	/* write traing pattern
	 * (DM,DQ7,....DQ0) x BL16
	 */
	uint16_t wdqlvl_patt[16];
	/* delay adjustment is ps */
	int8_t cacs_adj[16];
	int8_t dm_adj_w[SLICE_CNT];
	int8_t dq_adj_w[SLICE_CNT*8];
	int8_t dm_adj_r[SLICE_CNT];
	int8_t dq_adj_r[SLICE_CNT*8];
};

struct _boardcnf {
	/* ch in use */
	uint8_t phyvalid;
	/* use dbi mode */
	uint8_t dbi_en;
	/* default CA/CS delay value */
	uint16_t cacs_dly;
	/* default CA/CS delay adjust value in ps*/
	int16_t cacs_dly_adj;
	/* default DQ/DM delay value for write*/
	uint16_t dqdm_dly_w;
	/* default DQ/DM delay value for read*/
	uint16_t dqdm_dly_r;
	struct _boardcnf_ch ch[DRAM_CH_CNT];
};
/* write traing pattern
 * (DM,DQ7,....DQ0) x BL16
 */
#define WDQLVL_PAT {\
	0x00AA,\
	0x0055,\
	0x00AA,\
	0x0155,\
	0x01CC,\
	0x0133,\
	0x00CC,\
	0x0033,\
	0x00F0,\
	0x010F,\
	0x01F0,\
	0x010F,\
	0x00F0,\
	0x00F0,\
	0x000F,\
	0x010F}

static const struct _boardcnf boardcnfs[BOARDNUM] = {
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
