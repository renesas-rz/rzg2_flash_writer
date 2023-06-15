/*
 * Copyright (c) 2020, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*! *****************************************************************************
    @file    TSIP_Core_KeyRingVerify_API.c
    @brief   keyring verification
    
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
/*! *****************************************************************************
    @fn       uint32_t TSIP_KeyRing_Verify(void)
    @brief    Keyring verification in Boot(No OS)
    
    @retval    R_PASS                          success
    @retval    R_KEYRING_FORMAT_FAIL           keyring format failed
    @retval    R_VERIFICATION_FAIL             verification failed
    @retval    R_SEQUENCE_FAIL                 cannot perform in current state
    @retval    R_RESOURCE_CONFLICT_FAIL        resource for this process already used in other process
    
    @par    Details
    It verifies Keyring in Boot(No OS) \n

    @par    Processing flow
    -# check initialization state in TSIP_Core layer
        - Is_TSIP_Core_Init()
    -# check transition state in TSIP Core layer
        - TSIP_Core_Check_State()
    -# verify Keyring
        - TSIP_Core_Prc_p12()
    -# set transition state in TSIP_Core layer
        - TSIP_Core_Set_State()
 ********************************************************************************/
uint32_t TSIP_KeyRing_Verify(void)
{
    uint8_t  BoolFg;
    uint32_t RetCode;

    BoolFg = Is_TSIP_Core_Init();
    RetCode = TSIP_Core_Check_State(TSIP_KEYRING_VERIFY);
    if((BoolFg == TSIP_TRUE) && (RetCode == R_PASS)) {
        RetCode = TSIP_Core_Prc_p12();
    }
    else {
        RetCode = R_SEQUENCE_FAIL;
    }

    if(RetCode == R_PASS) {
        TSIP_Core_Set_State(TSIP_CORE_STATE22);
    }
    else if((RetCode == R_VERIFICATION_FAIL) || (RetCode == R_KEYRING_FORMAT_FAIL)) {
        TSIP_Core_Set_State(TSIP_CORE_STATE_STOP);
    }
    else {
        /* do nothing */
    }

    return RetCode;
}

#endif  /* _TSIP_SB */
