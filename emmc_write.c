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
 * @file  emmc_write.c
 * @brief write api
 *
 */

/* ************************ HEADER (INCLUDE) SECTION *********************** */
#include "emmc_config.h"
#include "emmc_hal.h"
#include "emmc_std.h"
#include "emmc_registers.h"
#include "emmc_def.h"
#include "common.h"

/* ***************** MACROS, CONSTANTS, COMPILATION FLAGS ****************** */
#define MIN(a,b)        (((a) < (b)) ? (a) : (b))

#define EMMC_RW_SECTOR_COUNT_MAX        0x0000ffffUL

/* ********************** STRUCTURES, TYPE DEFINITIONS ********************* */

/* ********************** DECLARATION OF EXTERNAL DATA ********************* */

/* ************************** FUNCTION PROTOTYPES ************************** */

static EMMC_ERROR_CODE emmc_multiple_block_write (uint32_t *buff_address_virtual, uint32_t sector_number, uint32_t count, HAL_MEMCARD_DATA_TRANSFER_MODE transfer_mode);


/* ********************************* CODE ********************************** */

/** @brief function of write sector
 *
 * This function always use block write.
 * Single block write is not used.
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
EMMC_ERROR_CODE emmc_write_sector (
    uint32_t *buff_address_virtual,
    uint32_t sector_number,
    uint32_t count,
    uint32_t feature_flags
    )
{
    uint32_t trans_count;
    uint32_t remain;
    EMMC_ERROR_CODE result;
    HAL_MEMCARD_DATA_TRANSFER_MODE transfer_mode;

    /* parameter check */
    if (count == 0)
    {
        emmc_write_error_info(EMMC_FUNCNO_WRITE_SECTOR, EMMC_ERR_PARAM);
        return EMMC_ERR_PARAM;
    }       
        
    /* state check */
    if (mmc_drv_obj.mount != TRUE)
    {
        emmc_write_error_info(EMMC_FUNCNO_WRITE_SECTOR, EMMC_ERR_STATE);
        return EMMC_ERR_STATE;
    }

    /* DMA? */
    if ((feature_flags & LOADIMAGE_FLAGS_DMA_ENABLE) != 0)
    {
        transfer_mode = HAL_MEMCARD_DMA;
    }
    else
    {
        transfer_mode = HAL_MEMCARD_NOT_DMA;
    }

    remain = count;
    while (remain != 0)
    {
        trans_count = MIN(remain, EMMC_RW_SECTOR_COUNT_MAX);
        result = emmc_multiple_block_write(buff_address_virtual, sector_number, trans_count, transfer_mode);
        if (result != EMMC_SUCCESS)
        {
            return result;
        }

        buff_address_virtual += (EMMC_BLOCK_LENGTH_DW*trans_count);
        sector_number += trans_count;
        remain -= trans_count;
    }
    
    return EMMC_SUCCESS;
}

/** @brief multiple block write
 *
 * Multiple block write with pre-defined block count.
 *
 * - Pre-conditions:<BR>
 * MMC card is mounted.
 *
 * - Post-conditions:<BR>
 * .
 *
 * @param[in,out] buff_address_virtual  virtual address of write data buffer.
 * @param[in] sector_number data address for MMC device (sector number).
 * @param[in] count number of sector. (0x1 - 0xffff)
 * @param[in] transfer_mode Mode of data transfer, DMA or not DMA.
 */
static EMMC_ERROR_CODE emmc_multiple_block_write
    (
    uint32_t *buff_address_virtual,
    uint32_t sector_number,
    uint32_t count,
    HAL_MEMCARD_DATA_TRANSFER_MODE transfer_mode
    )
{
    EMMC_ERROR_CODE result;
//    st_error_info   backup;
#ifdef EMMC_DEBUG
	int32_t lchCnt;
	char buf[16];
#endif /* EMMC_DEBUG */

    /* parameter check */
    if ((count > EMMC_RW_SECTOR_COUNT_MAX)
        || (count == 0)
        || ((transfer_mode != HAL_MEMCARD_DMA) && (transfer_mode != HAL_MEMCARD_NOT_DMA))
        )
    {
        emmc_write_error_info(EMMC_FUNCNO_WRITE_SECTOR, EMMC_ERR_PARAM);
        return EMMC_ERR_PARAM;
    }

#ifdef REWRITE_TOOL
	/* CMD16 */
    emmc_make_nontrans_cmd(CMD16_SET_BLOCKLEN, EMMC_BLOCK_LENGTH);
    result = emmc_exec_cmd(EMMC_R1_ERROR_MASK, mmc_drv_obj.response);
    if (result != EMMC_SUCCESS)
    {
        return result;
    }

	/* Transfer Data Length */
	SETR_32( SD_SIZE, EMMC_BLOCK_LENGTH );
#endif /* REWRITE_TOOL */

	/* CMD23 */
    emmc_make_nontrans_cmd(CMD23_SET_BLOCK_COUNT, ( 0x80000000U | count) );
    result = emmc_exec_cmd(EMMC_R1_ERROR_MASK, mmc_drv_obj.response);
    if (result != EMMC_SUCCESS)
    {
        return result;
    }
    SETR_32( SD_SECCNT, count );
    SETR_32( SD_STOP, 0x00000100U );
	SETR_32( CC_EXT_MODE, (CC_EXT_MODE_CLEAR | CC_EXT_MODE_DMASDRW_ENABLE) );     /* SD_BUF Read/Write DMA Transfer enable */

#ifdef EMMC_DEBUG
	PutStr("",1);
	PutStr("SD_STOP   = 0x",0);
	Hex2Ascii(GETR_32( SD_STOP ),buf,&lchCnt);
	PutStr(buf,1);
	PutStr("SD_SECCNT = 0x",0);
	Hex2Ascii(GETR_32( SD_SECCNT ),buf,&lchCnt);
	PutStr(buf,1);
#endif /* EMMC_DEBUG */

	/* CMD25 */
    emmc_make_trans_cmd(CMD25_WRITE_MULTIPLE_BLOCK, sector_number, buff_address_virtual, count<<EMMC_SECTOR_SIZE_SHIFT, HAL_MEMCARD_WRITE, transfer_mode);
    result = emmc_exec_cmd(EMMC_R1_ERROR_MASK, mmc_drv_obj.response);
    if (result != EMMC_SUCCESS)
    {
        return result;                  /* CMD18 error code */
    }

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
        emmc_write_error_info(EMMC_FUNCNO_WRITE_SECTOR, EMMC_ERR_CARD_BUSY);
        return EMMC_ERR_CARD_BUSY;
    }

    /* state check */
    if (mmc_drv_obj.current_state != EMMC_R1_STATE_TRAN)
    {
        emmc_write_error_info(EMMC_FUNCNO_WRITE_SECTOR, EMMC_ERR_CARD_STATE);
        return EMMC_ERR_CARD_STATE;
    }

    return EMMC_SUCCESS;
}

/* ******************************** END ************************************ */

