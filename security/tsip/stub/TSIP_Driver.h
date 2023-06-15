/*
 * Copyright (c) 2020, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*! *****************************************************************************
    @file    TSIP_Driver.h
    @brief   TSIP Driver API
    
    @par    History
    @subsection sub_history 
      - Initial version\n
 ********************************************************************************/

#ifndef  R__TSIP_DRIVER_H__
#define  R__TSIP_DRIVER_H__

#define  DRIVER_OK           ((uint8_t)0x00)
#define  DRIVER_ERROR        ((uint8_t)0x01)

#define  MASK_DGSTATE_REG    (0x0000FF00U)
#define  TSIP_STATE_MAIN07   (0x00000700U)
#define  TSIP_STATE_SECURE01 (0x00000100U)

#define  TSIP_BASE_ADDR      (0xE7800000U)
#define  TSIP_SIZE           (0x00000200U)

/*------------------------------------------------------------------------------*/
/*                        Include Files                                         */
/*------------------------------------------------------------------------------*/

#include <stdint.h>

extern volatile uint32_t* gpTSIP;

#ifdef _TSIP_SB
uint8_t TSIPDrv_Initialize(void);
#else
uint8_t TSIPDrv_Initialize(uint8_t *TSIP_BaseAddress, uint8_t *MSTP_BaseAddress);
#endif
void TSIPDrv_Finalize(void);

/*!
    @def    TSIPDrv_Read(regNumber)
    @brief     TSIP Drier read macro
 */
#define TSIPDrv_Read(regNumber) \
     *((gpTSIP) + regNumber)

/*! 
	@def    TSIPDrv_Write(regNumber, value)
    @brief     TSIP Driver write macro
 */
#define TSIPDrv_Write(regNumber, value) \
    *((gpTSIP) + regNumber) = value

#endif  /* R__TSIP_DRIVER_H__ */


