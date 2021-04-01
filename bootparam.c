/*
 * Copyright (c) 2021, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

const unsigned int __attribute__ ((section (".boot_param"))) boot_param = 0x00010000;
const unsigned int __attribute__ ((section (".sig_55aa")))   sig_55aa = 0xAA55FFFF;
