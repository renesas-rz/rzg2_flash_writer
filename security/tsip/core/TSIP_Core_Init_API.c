/*
 * Copyright (c) 2020, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*! *****************************************************************************
    @file    TSIP_Core_Init_API.c
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

#include "common/TSIP_Common_Define.h"
#include "core/TSIP_Core_API.h"
#include "core/TSIP_Core_Proc_API.h"
#include "core/TSIP_Core_Local_API.h"
#include "stub/TSIP_Driver.h"
#include "stub/TSIP_Stub_API.h"

#ifdef _TSIP_SB

/*------------------------------------------------------------------------------*/
/*                        Prototype Definition                                  */
/*------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------*/
/*                        Function Definition                                   */
/*------------------------------------------------------------------------------*/
static uint32_t TSIP_Self_Judge(void);

/*! *****************************************************************************
    @fn       static uint32_t TSIP_Self_Judge(void)
    @brief    TSIP self checking
    
    @param     void
    @retval    R_PASS                       success
    @retval    R_SEQUENCE_FAIL              cannot perform in current state
    @retval    R_INITIALIZATION_FAIL        initialization failed
    @retval    R_INITIALIZATION_ERROR       initialization error
    @retval    R_RESOURCE_CONFLICT_FAIL     resource for this process already used in other process

    @par    Details
    It performs TSIP self checking.
    
    @par    Processing flow
    -# software reset of TSIP
        - TSIP_Core_Prc_softreset()
    -# TSIP self checking
        - TSIP_Core_Prc_p01(),TSIP_Core_Prc_p02()
 ********************************************************************************/
static uint32_t TSIP_Self_Judge(void)
{
    uint32_t RetCode;
    uint32_t Count;

    (void)TSIP_Core_Prc_softreset();

    RetCode = TSIP_Core_Prc_p01();
    if(RetCode == R_PASS) {
        for(Count = 0U; Count < 5U; Count++) {
            RetCode = TSIP_Core_Prc_p02();
            if(RetCode != R_PROC_RETRY) {
                break;
            }
        }
    }

    if(RetCode == R_PROC_RETRY) {
        RetCode = R_INITIALIZATION_FAIL;
    }
    return RetCode;

}
/*! *****************************************************************************
    @fn       uint32_t TSIP_Init_Core(uint8_t *S_RAMData, uint8_t *S_INSTData)
    @brief    Initialization for Secure Boot(No OS)
    
    @param     [in]        *S_RAMData          Pointer of S_RAM data(it needs 432Byte area)
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
    -# parameter checking
    -# Initialize TSIP driver
        - TSIPDrv_Initialize()
    -# Initialize TSIP_Core layer
        - TSIP_Core_Init()
    -# TSIP self checking
        - TSIP_Self_Judge()
    -# set the initialization flag in TSIP Core layer
        - TSIP_Core_Set_Init()
    -# set the transition state of TSIP_Core layer
        - TSIP_Core_Set_State()
 ********************************************************************************/
uint32_t TSIP_Init_Core(uint8_t *S_RAMData, uint8_t *S_INSTData)
{
    uint32_t RetCode;
    uint8_t  Ret;

    if((S_RAMData != NULL) && (S_INSTData != NULL)) {
        Ret = TSIPDrv_Initialize();
        if(Ret == DRIVER_OK) {
            RetCode = TSIP_Core_Init(S_RAMData, S_INSTData);
            /* TSIP_Core_Init retun only R_PASS */
            if(RetCode == R_PASS) {
                RetCode = TSIP_Self_Judge();
                switch(RetCode) {
                    case R_PASS:
                        TSIP_Core_Set_Init(TSIP_TRUE);
                        TSIP_Core_Set_State(TSIP_CORE_STATE01);
                        break;
                    case R_INITIALIZATION_ERROR:
                        TSIP_Core_Set_Init(TSIP_TRUE);
                        TSIP_Core_Set_State(TSIP_CORE_STATE_STOP);
                        break;
                     default:
                        TSIPDrv_Finalize();
                        break;
                }
            }
        }
        else {
            RetCode = R_INITIALIZATION_FAIL;
        }
    }
    else {
        RetCode = R_PARAMETER_FAIL;
    }

    return RetCode;
}

#endif /* _TSIP_SB */
