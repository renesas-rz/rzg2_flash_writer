/*
 * Copyright (c) 2020, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*! *****************************************************************************
    @file    TSIP_Wrapper_Local_API.c
    @brief   TSIP Wrapper Local API
    
    @par    History
    @subsection sub_history 
      - Initial version\n
 ********************************************************************************/

/*------------------------------------------------------------------------------*/
/*                        Include Files                                         */
/*------------------------------------------------------------------------------*/

#include <stddef.h>
#include <stdint.h>

#include "common/TSIP_Common_Define.h"
#include "wrapper/TSIP_Wrapper_Local_API.h"


static uint8_t TSIP_Wrapper_Init_Flag = TSIP_FALSE;

/*! *****************************************************************************
    @fn       void TSIP_Wrapper_Set_Init(uint8_t BoolFg)
    @brief    Initialization flag setting
    
    @retval    void
    
    @par    Details
    It set the initialization flag in TSIP Wrapper Layer.

    @par    Procession flow
    -# set the flag to initialized
 ********************************************************************************/
void TSIP_Wrapper_Set_Init(uint8_t BoolFg)
{
    TSIP_Wrapper_Init_Flag = BoolFg;
}

/*! *****************************************************************************
    @fn       uint32_t TSIP_Wrapper_Init(void)
    @brief    Initialization of TSIP Wrapper Layer
    
    @retval    R_PASS           success
    
    @par    Details
    It performs the initialization in TSIP Wrapper Layer.

    @par    Processing flow
    -# set the initialization flag in TSIP Wrapper Layer
        - TSIP_Wrapper_Set_Init()
 ********************************************************************************/
uint32_t TSIP_Wrapper_Init(void)
{
    TSIP_Wrapper_Set_Init(TSIP_TRUE);
    return R_PASS;
}

/*! *****************************************************************************
    @fn       uint8_t Is_TSIP_Wrapper_Init(void)
    @brief    Initialization checking
    
    @retval    TSIP_TRUE           initialized
    @retval    TSIP_FALSE          not initialized
    
    @par    Details
    It check if it has been initialized in TSIP Wrapper Layer.

    @par    Processing flow
    -# return the TSIP_Wrapper_Init_Flag
 ********************************************************************************/
uint8_t Is_TSIP_Wrapper_Init(void)
{
    return(TSIP_Wrapper_Init_Flag);
}


