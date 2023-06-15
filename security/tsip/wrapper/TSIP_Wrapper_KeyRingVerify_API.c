/*
 * Copyright (c) 2020, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*! *****************************************************************************
    @file    TSIP_Wrapper_KeyRingVerify_API.c
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
    @fn       unsigned long R_TSIP_KeyRing_Verify(void)
    @brief    Keyring verification in Boot(No OS)

    @retval    R_PASS                          success
    @retval    R_KEYRING_FORMAT_FAIL           keyring format failed
    @retval    R_VERIFICATION_FAIL             verification failed
    @retval    R_SEQUENCE_FAIL                 cannot perform in current state
    @retval    R_RESOURCE_CONFLICT_FAIL        resource for this process already used in other process
    
    @par    Details
    It verifies Keyring in Boot(No OS) \n
    
    @par    Processing flow
    -# check initialization state in TSIP_Wrapper layer
        - Is_TSIP_Wrapper_Init()
    -# verify Keyring
        - TSIP_KeyRing_Verify()
 ********************************************************************************/
unsigned long R_TSIP_KeyRing_Verify(void)
{
    uint8_t BoolFg;
    unsigned long RetCode;

    BoolFg = Is_TSIP_Wrapper_Init();
    if(BoolFg == TSIP_TRUE) {
        RetCode = TSIP_KeyRing_Verify();
    }
    else {
        RetCode = R_SEQUENCE_FAIL;
    }

    return RetCode;
}
