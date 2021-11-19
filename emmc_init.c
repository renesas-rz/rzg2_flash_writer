/*
 * Copyright (c) 2015-2018, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/** 
 * @file  emmc_init.c
 * @brief eMMC boot initialize program.
 *
 */

/* ************************ HEADER (INCLUDE) SECTION *********************** */
#include "emmc_config.h"
#include "emmc_hal.h"
#include "emmc_std.h"
#include "emmc_registers.h"
#include "emmc_def.h"
#include "common.h"
#include "bit.h"
#include "cpg_regs.h"

/* ***************** MACROS, CONSTANTS, COMPILATION FLAGS ****************** */

/* ********************** STRUCTURES, TYPE DEFINITIONS ********************* */

/* ********************** DECLARATION OF EXTERNAL DATA ********************* */

/* ************************** FUNCTION PROTOTYPES ************************** */
static void emmc_drv_init(void);
static EMMC_ERROR_CODE emmc_dev_init(void);
static EMMC_ERROR_CODE emmc_dev_finalize(void);
static void emmc_memset(uint8_t *buff, uint8_t data, uint32_t cnt);
static EMMC_ERROR_CODE emmc_reset_controller(void);
static void emmc_driver_config (void);
__inline static void emmc_set_retry_count(uint32_t retry);
__inline static void emmc_set_data_timeout(uint32_t data_timeout);

#ifdef EMMC_VOLTAGE_1_8
static void SetMmcPfcVoltage(uint32_t voltage);
static void SetMmcVoltage(uint32_t voltage);
static void InitMmcPinFunction(void);
#endif /* EMMC_VOLTAGE_1_8 */

/* ********************************* CODE ********************************** */

/** @brief eMMC initialize.
 * 
 * - Pre-conditions:<BR>
 * .
 * - Post-conditions:<BR>
 * .
 * 
 * @param[in] low_clock_mode_enable TRUE = low clock mode(max26MHz), FALSE= Normal clock mode(Max52MHz)
 * @retval EMMC_SUCCESS successful.
 * @retval EMMC_ERR error from interrupt API.
 */
EMMC_ERROR_CODE emmc_init(uint8_t low_clock_mode_enable)
{
	EMMC_ERROR_CODE retult;

	/* initialize H/W */
    retult = emmc_reset_controller();
	if( EMMC_SUCCESS != retult )
	{
		return retult;
	}

    /* Configuration */
    emmc_driver_config();

    /* clock mode */
    mmc_drv_obj.low_clock_mode_enable = low_clock_mode_enable;

    return EMMC_SUCCESS;
}

/** @brief terminate emmc driver
 *
 * EMMC H/W and S/W resource is released.
 * 
 * - Pre-conditions:<BR>
 * .
 * - Post-conditions:<BR>
 * .
 * 
 * @return None.
 */
EMMC_ERROR_CODE emmc_terminate (void)
{
    EMMC_ERROR_CODE result;

   /* H/W finalize */
    result = emmc_dev_finalize();
    if (result != EMMC_SUCCESS)
    {
        /* nothing to do. */
    }

    /* driver finalize */
    emmc_memset((uint8_t *)(&mmc_drv_obj), 0, sizeof(st_mmc_base));  /* clear global variable */

    return result;
}

/** @brief Function executes full reset to MMC host controller without taking power out from the memory card.
 * 
 * - Pre-conditions:<BR>
 * .
 * - Post-conditions:<BR>
 * Reset MMC host controller without taking power out from the memory card.
 * Memory card preserves its state.
 * 
 * @return None
 */
static EMMC_ERROR_CODE emmc_reset_controller(void)
{
	EMMC_ERROR_CODE retult;
	
    /* initialize mmc driver */
    emmc_drv_init();

    /* initialize H/W */
    retult = emmc_dev_init();
	if( EMMC_SUCCESS != retult )
	{
		return retult;
	}

    mmc_drv_obj.initialize = TRUE;

	return retult;

}

/** @brief Configuration eMMC driver
 * 
 * - Pre-conditions:<BR>
 * initialized eMMC driver.
 * - Post-conditions:<BR>
 * .
 * 
 * @return None
 */
static void emmc_driver_config (void)
{
    /* retry */
    emmc_set_retry_count(EMMC_RETRY_COUNT);

    /* Read/Write data timeout */
    emmc_set_data_timeout(EMMC_RW_DATA_TIMEOUT);
}


/** @brief  Registers configuration
 *
 * This function gives the configuration arguments to the driver.
 * It must be possible to call this function multiple times.
 * 
 * - Pre-conditions:<BR>
 * initialized eMMC driver.
 * 
 * - Post-conditions:<BR>
 * Driver is using configuration values.
 * 
 * @param[in] retry how many times to try after fail, for instance sending command.
 * @return None
 */
__inline static void emmc_set_retry_count (
    uint32_t  retry
    )
{
    mmc_drv_obj.retries_after_fail = retry;
}

/** @brief Sets data timeout
 *
 * Sets the data timeout value for read and write operations.
 * 
 * - Pre-conditions:<BR>
 * initialized eMMC driver.
 * 
 * - Post-conditions:<BR>
 * After this function is called, the timeout value is set according to argument. 
 * 
 * @param[in] time_out The desired timeout value in milliseconds.
 * @return None
 */
__inline static void emmc_set_data_timeout (
    uint32_t data_timeout
    )
{
    mmc_drv_obj.data_timeout = data_timeout;
}

/** @brief eMMC driver initialize. (software)
 * 
 * - Pre-conditions:<BR>
 * .
 * - Post-conditions:<BR>
 * .
 * 
 * @return None.
 */
static void emmc_drv_init(void)
{
    /* initialize */
    emmc_memset((uint8_t *)(&mmc_drv_obj), 0, sizeof(st_mmc_base));
    
    mmc_drv_obj.card_present = HAL_MEMCARD_CARD_IS_IN;
    mmc_drv_obj.data_timeout = EMMC_RW_DATA_TIMEOUT;
    
    mmc_drv_obj.bus_width = HAL_MEMCARD_DATA_WIDTH_1_BIT;
}

/** @brief eMMC driver initialize. (H/W)
 * 
 * - Pre-conditions:<BR>
 * .
 * - Post-conditions:<BR>
 * .
 * 
 * @return None.
 */
static EMMC_ERROR_CODE emmc_dev_init(void)
{
#ifdef EMMC_VOLTAGE_1_8
	InitMmcPinFunction();
	SetMmcVoltage(1);	/* I/O Voltage=1.8V */
	StartTMU2(10);		/* wait 100ms		*/
#endif /* EMMC_VOLTAGE_1_8 */
	
	/* MMCIF initialize */
	SETR_32(SD_INFO1, 0x00000000U);			/* all interrupt clear */
	SETR_32(SD_INFO2, SD_INFO2_CLEAR );			/* all interrupt clear */
	SETR_32(SD_INFO1_MASK, 0x00000000U);		/* all interrupt disable */
	SETR_32(SD_INFO2_MASK, SD_INFO2_CLEAR );	/* all interrupt disable */

	SETR_32(HOST_MODE, 0x00000000U);			/* SD_BUF access width = 64-bit */
	SETR_32(SD_OPTION, 0x0000C0EEU);  		/* Bus width = 8bit, timeout=MAX */
	SETR_32(SD_CLK_CTRL, 0x000000000U);		/* Automatic Control=Disable, Clock Output=Disable */

    return EMMC_SUCCESS;
}

/** @brief EMMC H/W finalize 
 *
 * EMMC Host and Card hardware resource is released.
 *
 * - Pre-conditions:<BR>
 * .
 *
 * - Post-conditions:<BR>
 * .
 * @return None.
 */
static EMMC_ERROR_CODE emmc_dev_finalize(void)
{
    EMMC_ERROR_CODE result;
	uint32_t mmc_ch = USE_MMC_CH;
	uint32_t dataL;
	uint32_t tmp_val;

    /* MMC power off
     * the power supply of eMMC device is always turning on. 
     * RST_n : Hi --> Low level.
     */
    result = emmc_memcard_power(FALSE);
    if (result != EMMC_SUCCESS)
    {
        /* nothing to do. */
    }

    /* host controller reset */
//	SETR_32(SOFT_RST, ( GETR(SOFT_RST) & (~SOFT_RST_SDRST) ) );		/* Soft reset */
//	SETR_32(SOFT_RST, ( GETR(SOFT_RST) | SOFT_RST_SDRST ) );		/* Soft reset released */
	SETR_32(SD_INFO1, 0x00000000U);			/* all interrupt clear */
	SETR_32(SD_INFO2, SD_INFO2_CLEAR );		/* all interrupt clear */
	SETR_32(SD_INFO1_MASK, 0x00000000U);	/* all interrupt disable */
	SETR_32(SD_INFO2_MASK, SD_INFO2_CLEAR );	/* all interrupt disable */
	SETR_32(SD_CLK_CTRL, 0x00000000U);		/* MMC clock stop */

    return result;
}

/** @brief Set power to memory card IF.
 * This function control Vcc and Vccq and RST_n.
 *
 * @attention
 * CPU cannot control Vcc&Vccq.
 * The power supply of eMMC device is always turning on. 
 * 
 * @param[in] mode TRUE = power on, FALSE = power off
 * 
 * @retval EMMC_SUCCESS powering succeeded
 * @retval EMMC_ERR_CARD_POWER powering failed 
 */
EMMC_ERROR_CODE emmc_memcard_power
    (
    uint8_t mode
    )
{

    if (mode == TRUE)
    {
        /* power on (Vcc&Vccq is always power on) */
        mmc_drv_obj.card_power_enable = TRUE;
    }
    else
    {
        /* power off (Vcc&Vccq is always power on) */
        mmc_drv_obj.card_power_enable = FALSE;
        mmc_drv_obj.mount = FALSE;
        mmc_drv_obj.selected = FALSE;
    }

    return EMMC_SUCCESS;
}

/** @brief memset(). no use C standard library.
 * 
 * - Pre-conditions:<BR>
 * .
 * - Post-conditions:<BR>
 * .
 * 
 * @param[in,out] buff pointer to buffer (virtual)
 * @param[in] data fill data.
 * @param[in] cnt fill size (number of bytes)
 * @return None.
 */

static void emmc_memset(
    uint8_t *buff,
    uint8_t data,
    uint32_t cnt
    )
{
    if (buff == NULL)
    {
        return;
    }

    while (cnt > 0)
    {
        *buff++ = data;
        cnt--;
    }
}

#ifdef EMMC_VOLTAGE_1_8
// voltage=0: 1.8V
// voltage=1: 3.3V
static void SetMmcPfcVoltage(uint32_t voltage)
{
}

// voltage=0: 1.8V
// voltage=1: 3.3V
static void SetMmcVoltage(uint32_t voltage)
{
}

static void InitMmcPinFunction(void)
{
}
#endif /* EMMC_VOLTAGE_1_8 */

/* ******************************** END ************************************ */

