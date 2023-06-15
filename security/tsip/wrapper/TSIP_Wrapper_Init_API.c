/*
 * Copyright (c) 2020, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*! *****************************************************************************
    @file    TSIP_Wrapper_Init_API.c
    @brief   NoOS Initialization
    
    @par    History
    @subsection sub_history 
      - Initial version\n
 ********************************************************************************/

/*------------------------------------------------------------------------------*/
/*                        Include Files                                         */
/*------------------------------------------------------------------------------*/

#include <stddef.h>
#include <stdint.h>

#include "wrapper/TSIP_Wrapper_Init_API.h"
#include "wrapper/TSIP_Wrapper_Local_API.h"
#include "stub/TSIP_Stub_API.h"
#include "core/TSIP_Core_API.h"

/*------------------------------------------------------------------------------*/
/*                        Prototype Definition                                  */
/*------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------*/
/*                        Function Definition                                   */
/*------------------------------------------------------------------------------*/
/*! *****************************************************************************
    @fn       unsigned long R_TSIP_Init(unsigned char  *S_RAMData, unsigned char *S_INSTData)
    @brief    Initialization for Secure Boot(No OS)
    
    @param     [in]        *S_RAMData          Pointer of S_RAM data(it needs 480Byte area)
    @param     [in]        *S_INSTData         Pointer of S_INST data(it needs 1296Byte area)

    @retval    R_PASS                          success
    @retval    R_PARAMETER_FAIL                invalid input paramter
    @retval    R_RESOURCE_CONFLICT_FAIL        resource for this process already used in other process
    @retval    R_INITIALIZATION_FAIL           initialization failed
    @retval    R_INITIALIZATION_ERROR          initialization error
    
    @par    Details
    It performs initialization for Secure Boot(No OS). \n
    It also performs TSIP self checking.
    
    @par    Processing flow
    -# check if it is initialized in TSIP_Wrapper layer
        - TSIP_Wrapper_Init()
    -# initialize TSIP Core
        - TSIP_Init_Core()
 ********************************************************************************/
unsigned long R_TSIP_Init(unsigned char *S_RAMData, unsigned char *S_INSTData)
{
    unsigned long RetCode;

    RetCode = TSIP_Wrapper_Init();
    if(RetCode == R_PASS) {
        RetCode = TSIP_Init_Core(S_RAMData, S_INSTData);
    }

    return RetCode;
}
