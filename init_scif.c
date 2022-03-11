/*
 * Copyright (c) 2022, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "common.h"
#include "scifdrv.h"
#include "init_scif.h"

void InitScif(void)
{
	PowerOnScif0();
	InitScif0_SCIFCLK(115200);
}
