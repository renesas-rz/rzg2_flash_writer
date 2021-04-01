/*
 * Copyright (c) 2015-2016, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __DG_EMMC_H__
#define __DG_EMMC_H__

extern void	dg_emmc_disp_cid(void);
extern void	dg_emmc_disp_csd(void);
extern void	dg_emmc_disp_ext_csd(void);
extern void	dg_emmc_set_ext_csd(void);

extern void dg_init_emmc(void);
extern unsigned long dg_emmc_check_init(void);
extern void	dg_emmc_write_mot(void);
extern void	dg_emmc_write_bin(void);
extern void	dg_emmc_erase(void);
extern void	InitMmcDrv(void);

#endif /* __DG_EMMC_H__ */

