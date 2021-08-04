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

/**
 * @file  emmc_erase.c
 * @brief erase api
 *
 */

/* ************************ HEADER (INCLUDE) SECTION *********************** */
#include "emmc_config.h"
#include "emmc_hal.h"
#include "emmc_std.h"
#include "emmc_registers.h"
#include "emmc_def.h"
#include "types.h"

/* ***************** MACROS, CONSTANTS, COMPILATION FLAGS ****************** */

/* ********************** STRUCTURES, TYPE DEFINITIONS ********************* */

/* ********************** DECLARATION OF EXTERNAL DATA ********************* */

/* ************************** FUNCTION PROTOTYPES ************************** */

/* ********************************* CODE ********************************** */

/** @brief function of erase sector
 *
 *
 * - Pre-conditions:<BR>
 * MMC card is mounted.
 *
 * - Post-conditions:<BR>
 * .
 *
 * @param[in,out] buff_address_virtual  virtual address of write data buffer.
 * @param[in] sector_number data address for MMC device (sector number).
 * @param[in] count number of sector.
 * @param[in] transfer_mode Mode of data transfer, DMA or not DMA.
 */
EMMC_ERROR_CODE emmc_erase_sector(
    uint32_t start_address,
    uint32_t end_address
	)
{
    EMMC_ERROR_CODE result;

    /* parameter check */
    if (start_address > end_address)
    {
        emmc_write_error_info(EMMC_FUNCNO_ERASE_SECTOR, EMMC_ERR_PARAM);
        return EMMC_ERR_PARAM;
    }

    /* state check */
    if (mmc_drv_obj.mount != TRUE)
    {
        emmc_write_error_info(EMMC_FUNCNO_ERASE_SECTOR, EMMC_ERR_STATE);
        return EMMC_ERR_STATE;
    }
	/* EXT_CSD[175] ERASE_GROUP_DEF check */
	
	
    /* CMD35 */
    emmc_make_nontrans_cmd(CMD35_ERASE_GROUP_START, start_address);
    result = emmc_exec_cmd(EMMC_R1_ERROR_MASK, mmc_drv_obj.response);
    if (result != EMMC_SUCCESS)
    {
        return result;
    }

    /* CMD36 */
    emmc_make_nontrans_cmd(CMD36_ERASE_GROUP_END, end_address);
    result = emmc_exec_cmd(EMMC_R1_ERROR_MASK, mmc_drv_obj.response);
    if (result != EMMC_SUCCESS)
    {
        return result;
    }
	
    /* CMD38 */
    emmc_make_nontrans_cmd(CMD38_ERASE, 0);
    result = emmc_exec_cmd(EMMC_R1_ERROR_MASK, mmc_drv_obj.response);
    if (result != EMMC_SUCCESS)
    {
        return result;
    }

	//Figure A.11 - CIM_ERASE_GROUP   CMD13
    /* CMD13 */
    emmc_make_nontrans_cmd(CMD13_SEND_STATUS, EMMC_RCA<<16);
    result = emmc_exec_cmd(EMMC_R1_ERROR_MASK, mmc_drv_obj.response);
    if (result != EMMC_SUCCESS)
    {
        return result;
    }

	/* ready status check */
    if ( (mmc_drv_obj.r1_card_status & EMMC_R1_READY) == 0) 
    {
        emmc_write_error_info(EMMC_FUNCNO_ERASE_SECTOR, EMMC_ERR_CARD_BUSY);
        return EMMC_ERR_CARD_BUSY;
    }

    return EMMC_SUCCESS;
}

/* ******************************** END ************************************ */

