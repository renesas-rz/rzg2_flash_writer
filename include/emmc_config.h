/*******************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only
* intended for use with Renesas products. No other uses are authorized. This
* software is owned by Renesas Electronics Corporation and is protected under
* all applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT
* LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
* AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.
* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS
* ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE
* FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR
* ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE
* BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software
* and to discontinue the availability of this software. By using this software,
* you agree to the additional terms and conditions found by accessing the
* following link:
* http://www.renesas.com/disclaimer
* Copyright (C) 2021 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/ 

#ifndef __EMMC_CONFIG_H__
#define __EMMC_CONFIG_H__

/* ************************ HEADER (INCLUDE) SECTION *********************** */

/* ***************** MACROS, CONSTANTS, COMPILATION FLAGS ****************** */

#define SYS_RAM_BASE_ADD			0xE6300000UL					/* SYSTEM RAM BASE ADD						*/
#define EMMC_MULTI_BOOT_SADD			0x00004000UL					/* Multi Boot start add	(512byte order)				*/
#define EMMC_MULTI_BOOT_SIZE			0x0000A000UL					/* Multi Boot size		(512byte order)			*/
#define EMMC_MULTI_BOOT_DEST_ADD		(SYS_RAM_BASE_ADD + EMMC_MULTI_BOOT_SADD)	/* Multi Boot dest add						*/
#define EMMC_UBOOT_TRANS_DATA_SADD		0x0001EFF0UL					/* UBOOT trans data add						*/
#define EMMC_UBOOT_TRANS_DATA_DEST_ADD	(SYS_RAM_BASE_ADD + EMMC_UBOOT_TRANS_DATA_SADD)		/* UBOOT trans data dest add (512byte order)			*/

#define EMMC_UBOOT_TRANS_DEST_ADD_INDEX	((EMMC_UBOOT_TRANS_DATA_SADD & 0x1FF)>>2)		/* UBOOT trans dest add index					*/
#define EMMC_UBOOT_TRANS_SIZE_INDEX		(EMMC_UBOOT_TRANS_DEST_ADD_INDEX+1)		/* UBOOT trans size index					*/

#define EMMC_RCA                1UL                       /* RCA  */
#define EMMC_RW_DATA_TIMEOUT    0x40UL                    /* 345ms (freq = 400KHz, timeout Counter = 0x04(SDCLK * 2^17)  */
#define EMMC_RETRY_COUNT        0                         /* how many times to try after fail. Don't change. */
#define EMMC_CMD_MAX            60UL                      /* Don't change. */

#define LOADIMAGE_FLAGS_DMA_ENABLE              0x00000001UL


/* ********************** STRUCTURES, TYPE DEFINITIONS ********************* */
//#define EMMC_VOLTAGE_1_8		/* 			Voltage=1.8V driver spec 1/2		*/
								/* invalid: Voltage=3.3V driver spec 3/4	*/
//#define EMMC_DEBUG			/* eMMC drv debug log output					*/
/* ********************** DECLARATION OF EXTERNAL DATA ********************* */

/* ************************** FUNCTION PROTOTYPES ************************** */

/* ********************************* CODE ********************************** */

#endif  /* #ifndef __EMMC_CONFIG_H__ */
/* ******************************** END ************************************ */

