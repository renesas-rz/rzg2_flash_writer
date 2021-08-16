/*
 * Copyright (c) 2021, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#define BOARDNUM 6

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
