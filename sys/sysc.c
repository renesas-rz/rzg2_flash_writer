/*
 * Copyright (c) 2021, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <mmio.h>
#include "rzg2l_def.h"

#define SYSC_BASE	RZG2L_SYSC_BASE

#define	SYS_LSI_DEVID	(0xA04)
#define	DEVID_MASK	(0x0FFFFFFFul)
#define	INFO_MASK	(0xF0000000ul)

static inline uint32_t sysc_reg_read(uint32_t offset)
{
	return mmio_read_32(SYSC_BASE + offset);
}

unsigned int sysc_get_device_id(void)
{
	return sysc_reg_read(SYS_LSI_DEVID) & DEVID_MASK;
}

unsigned int sysc_get_device_info(void)
{
	return ((sysc_reg_read(SYS_LSI_DEVID) & INFO_MASK) >> 28) + 1;
}
