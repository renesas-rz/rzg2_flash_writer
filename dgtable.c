/*
 * Copyright (c) 2015-2022, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "common.h"
#include "dgtable.h"
#include "dgmodul1.h"
#include "memory_cmd.h"
#include "ddrcheck.h"
#if SERIAL_FLASH == 1
#include "dgmodul4.h"
#endif /* SERIAL_FLASH == 1 */
#if EMMC == 1
#include "dgemmc.h"
#endif /* EMMC == 1 */

/********************************************************/
/*        ROM TABLE                                     */
/********************************************************/
const com_menu MonCom[COMMAND_UNIT] = {
/*--------------------- Basic command ------------------------------*/
	 "H"			, dgHelp			,  0	,
#if SERIAL_FLASH == 1
	"XCS"			, dgClearSpiflash0		,  0	,
	"XLS2"			, dgG2LoadSpiflash0_2		,  0	,
	"XLS3"			, dgG2LoadSpiflash0_3		,  0	,
	"XINFO_BP"		, dgG2InfoSpiflash0_BP		,  0	,
	"XINFO_BP_S"		, dgG2InfoSpiflash0_BP_S	,  0	,
#endif /* SERIAL_FLASH == 1 */
#if EMMC == 1
	"EM_DCID"		, &dg_emmc_disp_cid		,  0	,
	"EM_DCSD"		, &dg_emmc_disp_csd		,  0	,
	"EM_DECSD"		, &dg_emmc_disp_ext_csd		,  0	,
	"EM_SECSD"		, &dg_emmc_set_ext_csd		,  0	,
	"EM_W"			, &dg_emmc_write_mot		,  0	,
	"EM_WB"			, &dg_emmc_write_bin		,  0	,
	"EM_E"			, &dg_emmc_erase		,  0	,
#endif /* EMMC == 1 */
	"SUP"			, dgScifSpeedUp			,  0	,
	"SUD"			, dgScifSpeedDown		,  0	,
	"RESET"			, dgReset			,  0	,
#if INTERNAL_MEMORY_ONLY == 0
	"DDRCK"			, dgDdrTest			,  0	,
	"RAMCK"			, dgRamTest			,  0	,
#endif /* INTERNAL_MEMORY_ONLY == 0 */
	"D"			, dgDump			,  0	,
	"DM"			, dgDumpMode			,  0	,
	"M"			, dgMemEdit_byte		,  0	,
	"MW"			, dgMemEdit_word		,  0	,
	"ML"			, dgMemEdit_long		,  0	,
	"MX"			, dgMemEdit_longlong		,  0	,
	"F"			, dgFill_byte			,  0	,
	"FL"			, dgFill_long			,  0	,
	"FX"			, dgFill_longlong		,  0	,
	TBL_END			, 0				,  0
};

/********************************************************/
/*        RAM TABLE                                     */
/********************************************************/
uintptr_t gUDump[3];
uintptr_t gUMem[3];
uint32_t gDumpMode;
