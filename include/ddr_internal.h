/*
 * Copyright (c) 2021, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __DDR_INTERNAL_H__
#define __DDR_INTERNAL_H__

#include <mmio.h>
#include <ddr_mc_if.h>
#include <ddr_phy_regs.h>

#define	ERROR(...)
void panic(void);

#define MC_PHYSET_NUM       (4)
#define SWIZZLE_MC_NUM      (9)
#define SIZZLE_PHY_NUM      (16)

static inline uint32_t read_mc_reg(uint32_t offset)
{
	return mmio_read_32(DDR_MC_BASE + offset);
}

static inline uint32_t read_mc_field(uint32_t address_offset, uint32_t field_width, uint32_t field_offset)
{
	if (32 < field_width) {
		ERROR("DDR error: field width %u of register %u is too large.", field_width, address_offset);
		panic();
	}

	if (31 < field_offset) {
		ERROR("DDR error: field offset %u of register %u is too large.", field_offset, address_offset);
		panic();
	}

	uint32_t value_mask = ((1 << field_width) - 1);

	return (read_mc_reg(address_offset) >> field_offset) & value_mask;
}

static inline void write_mc_reg(uint32_t offset, uint32_t val)
{
	mmio_write_32(DDR_MC_BASE + offset, val);
}

static inline void rmw_mc_reg(uint32_t offset, uint32_t mask, uint32_t val)
{
	write_mc_reg(offset, (read_mc_reg(offset) & mask) | val);
}

static inline void rmw_mc_set_field(uint32_t address_offset, uint32_t field_width, uint32_t field_offset, uint32_t val)
{
	if (32 < field_width) {
		ERROR("DDR error: field width %u of register %u is too large.", field_width, address_offset);
		panic();
	}

	if (31 < field_offset) {
		ERROR("DDR error: field offset %u of register %u is too large.", field_offset, address_offset);
		panic();
	}

	uint32_t value_mask = ((1 << field_width) - 1);
	uint32_t register_mask = (value_mask << field_offset) ^ 0xFFFFFFFF;

	rmw_mc_reg(address_offset, register_mask, (val & value_mask) << field_offset);
}

static inline uint32_t read_phy_reg(uint32_t offset)
{
	return mmio_read_32(DDR_PHY_BASE + offset);
}

static inline void write_phy_reg(uint32_t offset, uint32_t val)
{
	mmio_write_32(DDR_PHY_BASE + offset, val);
}

static inline void rmw_phy_reg(uint32_t offset, uint32_t mask, uint32_t val)
{
	write_phy_reg(offset, (read_phy_reg(offset) & mask) | val);
}

void ddr_ctrl_reten_en_n(uint8_t val);


#endif	/* __DDR_INTERNAL_H__ */
