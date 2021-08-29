/*
 * Copyright (c) 2021, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdint.h>

const uint32_t mc_odt_pins_tbl[4] = {
	1,
	0,
	0,
	0
};

const uint32_t mc_mr1_tbl[2] = {
	0x0706,
	0x0100
};

const uint32_t mc_mr2_tbl[2] = {
	0x0E00,
	0x0000
};

const uint32_t mc_mr5_tbl[2] = {
	0x01C0,
	0x01C0
};

const uint32_t mc_mr6_tbl[2] = {
	0x007F,
	0x000F
};

const uint32_t mc_phy_settings_tbl[][2] = {
	{DENALI_CTL_414,	0x57630BB8},
	{DENALI_CTL_415,	0x00002828},
	{DENALI_CTL_416,	0x00003C22},
	{DENALI_CTL_417,	0x00102611},
};

const uint32_t swizzle_mc_tbl[][2] = {
	{DENALI_CTL_403,	0x00000B02},
	{DENALI_CTL_404,	0x00000106},
	{DENALI_CTL_405,	0x11131C0D},
	{DENALI_CTL_406,	0x12001F15},
	{DENALI_CTL_407,	0x1D19140E},
	{DENALI_CTL_408,	0x17091B1A},
	{DENALI_CTL_409,	0x0A10160C},
	{DENALI_CTL_410,	0x0018051E},
	{DENALI_CTL_411,	0x00000000}
};

const uint32_t swizzle_phy_tbl[][2] = {
	{DDRPHY_R29,	0x00000000},
	{DDRPHY_R11,	0x14001816},
	{DDRPHY_R29,	0x00000001},
	{DDRPHY_R11,	0x010D0608},
	{DDRPHY_R29,	0x00000002},
	{DDRPHY_R11,	0x02190403},
	{DDRPHY_R29,	0x00000003},
	{DDRPHY_R11,	0x1705150B},
	{DDRPHY_R29,	0x00000004},
	{DDRPHY_R11,	0x13121110},
	{DDRPHY_R29,	0x00000005},
	{DDRPHY_R11,	0x07090F0E},
	{DDRPHY_R29,	0x00000006},
	{DDRPHY_R11,	0x1A1D0A1B},
	{DDRPHY_R29,	0x00000007},
	{DDRPHY_R11,	0x001E1C0C}
};
