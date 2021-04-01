/*
 * Copyright (c) 2015-2021, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

	.section writer_stack, "aw", %nobits
	.align 5
stacks:
	.space (4*1024), 0

	.end
