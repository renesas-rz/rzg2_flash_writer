/*
 * Copyright (c) 2015-2016, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/**
 * @file  emmc_config.h
 * @brief Configuration file
 *
 */

#ifndef __EMMC_CONFIG_H__
#define __EMMC_CONFIG_H__

/* ************************ HEADER (INCLUDE) SECTION *********************** */

/* ***************** MACROS, CONSTANTS, COMPILATION FLAGS ****************** */

/** @brief eMMC address map
 */
#define SYS_RAM_BASE_ADD				0xE6300000UL									/* SYSTEM RAM BASE ADD							*/
#define EMMC_MULTI_BOOT_SADD			0x00004000UL									/* Multi Boot start add	(512byte order)			*/
#define EMMC_MULTI_BOOT_SIZE			0x0000A000UL									/* Multi Boot size		(512byte order)			*/
#define EMMC_MULTI_BOOT_DEST_ADD		(SYS_RAM_BASE_ADD + EMMC_MULTI_BOOT_SADD)		/* Multi Boot dest add							*/
#define EMMC_UBOOT_TRANS_DATA_SADD		0x0001EFF0UL									/* UBOOT trans data add							*/
#define EMMC_UBOOT_TRANS_DATA_DEST_ADD	(SYS_RAM_BASE_ADD + EMMC_UBOOT_TRANS_DATA_SADD)	/* UBOOT trans data dest add (512byte order)	*/

#define EMMC_UBOOT_TRANS_DEST_ADD_INDEX	((EMMC_UBOOT_TRANS_DATA_SADD & 0x1FF)>>2)		/* UBOOT trans dest add index					*/
#define EMMC_UBOOT_TRANS_SIZE_INDEX		(EMMC_UBOOT_TRANS_DEST_ADD_INDEX+1)				/* UBOOT trans size index						*/

/** @brief MMC driver config
 */
#define EMMC_RCA                1UL                       /* RCA  */
#define EMMC_RW_DATA_TIMEOUT    0x40UL                    /* 345ms (freq = 400KHz, timeout Counter = 0x04(SDCLK * 2^17)  */
#define EMMC_RETRY_COUNT        0                         /* how many times to try after fail. Don't change. */
#define EMMC_CMD_MAX            60UL                      /* Don't change. */

/** @brief etc
 */
#define LOADIMAGE_FLAGS_DMA_ENABLE              0x00000001UL


/* ********************** STRUCTURES, TYPE DEFINITIONS ********************* */
//#define EMMC_VOLTAGE_1_8		/* 			Voltage=1.8V driver spec 1/2		*/
								/* invalid: Voltage=3.3V driver spec 3/4		*/
//#define EMMC_DEBUG			/* eMMC drv debug log output					*/
								/* 1st Cut: No conditions						*/
								/* 2nd Cut: After the multi-boot was successful	*/
//#define EMMC_TOOL_DUMP		/* eMMC Tool dump log output					*/
/* ********************** DECLARATION OF EXTERNAL DATA ********************* */

/* ************************** FUNCTION PROTOTYPES ************************** */

/* ********************************* CODE ********************************** */

#endif  /* #ifndef __EMMC_CONFIG_H__ */
/* ******************************** END ************************************ */

