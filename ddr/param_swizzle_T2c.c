/*
 * Copyright (c) 2020, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdint.h>

const uint32_t mc_odt_pins_tbl[4] = {
	1,
	2,
	0,
	0
};

const uint32_t mc_mr1_tbl[2] = {
	0x0706,
	0x0200
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
	0x001C
};

const uint32_t mc_phy_settings_tbl[][2] = {
	{DENALI_CTL_414,	0x576E08E8},
	{DENALI_CTL_415,	0x00003C22},
	{DENALI_CTL_416,	0x22227822},
	{DENALI_CTL_417,	0x00123312}
};

const uint32_t swizzle_mc_tbl[][2] = {
	{DENALI_CTL_403,	0x00000A02},
	{DENALI_CTL_404,	0x00000106},
	{DENALI_CTL_405,	0x171E120B},
	{DENALI_CTL_406,	0x1800000C},
	{DENALI_CTL_407,	0x00161C00},
	{DENALI_CTL_408,	0x0D1F1319},
	{DENALI_CTL_409,	0x15101B1A},
	{DENALI_CTL_410,	0x001D0509},
	{DENALI_CTL_411,	0x00000000}
};

const uint32_t swizzle_phy_tbl[][2] = {
	{DDRPHY_R29,	0x00000000},
	{DDRPHY_R11,	0x0A020419},
	{DDRPHY_R29,	0x00000001},
	{DDRPHY_R11,	0x03161701},
	{DDRPHY_R29,	0x00000002},
	{DDRPHY_R11,	0x0D000807},
	{DDRPHY_R29,	0x00000003},
	{DDRPHY_R11,	0x0F0B0518},
	{DDRPHY_R29,	0x00000004},
	{DDRPHY_R11,	0x13121110},
	{DDRPHY_R29,	0x00000005},
	{DDRPHY_R11,	0x14091506},
	{DDRPHY_R29,	0x00000006},
	{DDRPHY_R11,	0x1A1D1B0E},
	{DDRPHY_R29,	0x00000007},
	{DDRPHY_R11,	0x001E1C0C}
};
