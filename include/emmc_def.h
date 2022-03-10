/*
 * Copyright (c) 2015-2016, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/**
 * @file  emmc_def.h
 * @brief eMMC boot is expecting this header file
 *
 */

#ifndef __EMMC_DEF_H__
#define __EMMC_DEF_H__

#ifndef __EMMC_GLOBAL_DEFINE__
#define EXTERN      extern
#else
#define EXTERN
#endif

#include <stdint.h>		//for uint32_t

/* ************************ HEADER (INCLUDE) SECTION *********************** */

/* ***************** MACROS, CONSTANTS, COMPILATION FLAGS ****************** */

/* ********************** STRUCTURES, TYPE DEFINITIONS ********************* */

/* ********************** DECLARATION OF EXTERNAL DATA ********************* */
/* Temporary code to boot Secureboot IPL on Non-secureboot LSI */
#ifndef REWRITE_TOOL
EXTERN	st_mmc_base __attribute__ ((__section__(".gdata"))) mmc_drv_obj;
#else /* REWRITE_TOOL */
EXTERN	st_mmc_base mmc_drv_obj;
#endif /* REWRITE_TOOL */
/* ************************** FUNCTION PROTOTYPES ************************** */

/** @brief for assembler program 
 */
EXTERN uint32_t _rom_emmc_finalize (void);

/** @brief eMMC driver API
 */
EXTERN EMMC_ERROR_CODE emmc_init(uint8_t low_clock_mode_enable);
EXTERN EMMC_ERROR_CODE emmc_terminate(void);
EXTERN EMMC_ERROR_CODE emmc_memcard_power(uint8_t mode);
EXTERN EMMC_ERROR_CODE emmc_mount(void);
EXTERN EMMC_ERROR_CODE emmc_set_request_mmc_clock(uint32_t *freq);
EXTERN EMMC_ERROR_CODE emmc_send_idle_cmd (uint32_t arg);
EXTERN EMMC_ERROR_CODE emmc_select_partition(EMMC_PARTITION_ID id);
EXTERN EMMC_ERROR_CODE emmc_read_sector(uint32_t *buff_address_virtual, uint32_t sector_number, uint32_t count, uint32_t feature_flags);
EXTERN EMMC_ERROR_CODE emmc_write_sector(uint32_t *buff_address_virtual, uint32_t sector_number, uint32_t count, uint32_t feature_flags);
EXTERN EMMC_ERROR_CODE emmc_erase_sector(uint32_t start_address,uint32_t end_address );
EXTERN uint32_t emmc_bit_field (uint8_t *data, uint32_t top, uint32_t bottom);


/** @brief interrupt service
 */
EXTERN uint32_t emmc_interrupt(void);

/** @brief DMA
 */


/** @brief send command API
 */
EXTERN EMMC_ERROR_CODE emmc_exec_cmd (uint32_t error_mask, uint32_t *response);
EXTERN void emmc_make_nontrans_cmd (HAL_MEMCARD_COMMAND cmd, uint32_t arg);
EXTERN void emmc_make_trans_cmd (HAL_MEMCARD_COMMAND cmd, uint32_t arg, uint32_t *buff_address_virtual,
            uint32_t len, HAL_MEMCARD_OPERATION dir, HAL_MEMCARD_DATA_TRANSFER_MODE transfer_mode);
EXTERN EMMC_ERROR_CODE emmc_set_ext_csd(uint32_t arg);

/** @brief for error information
 */
EXTERN void emmc_write_error_info(uint16_t func_no, EMMC_ERROR_CODE error_code);
EXTERN void emmc_write_error_info_func_no (uint16_t func_no);

/* ********************************* CODE ********************************** */

#undef EXTERN

#endif /* #define __EMMC_DEF_H__ */
/* ******************************** END ************************************ */

