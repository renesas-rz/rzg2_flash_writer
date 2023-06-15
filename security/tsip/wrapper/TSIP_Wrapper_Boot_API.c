/*
 * Copyright (c) 2020, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*! *****************************************************************************
    @file    TSIP_Wrapper_Boot_API.c
    @brief   Boot API

    @par    History
    @subsection sub_history
      - Initial version\n
 ********************************************************************************/

/*------------------------------------------------------------------------------*/
/*                        Include Files                                         */
/*------------------------------------------------------------------------------*/

#include <stddef.h>
#include <stdint.h>

#include "wrapper/TSIP_Wrapper_Boot_API.h"
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
    @fn       unsigned long R_TSIP_VerifyBootData(unsigned char *InData_BootData,
                                                  unsigned long InData_BootData_ByteSize,
                                                  unsigned char *InData_BootData_HeapArea,
                                                  unsigned char *OutData_BootData)
    @brief    Verificatoin and decryption of BootData

    @param     [in]        *InData_BootData            Pointer of encrypted Boot data by TSIP key
    @param     [in]        InData_BootData_ByteSize    Size of encrypted Boot data by TSIP key (multiple of 16Byte)
    @param     [in]        *InData_BootData_HeapArea   Pointer of heap(Size: InData_BootData_ByteSize – 272)
    @param     [out]       *OutData_BootData           Pointer of decrypted Boot data

    @retval    R_PASS                   success
    @retval    R_PARAMETER_FAIL         invalid input parameter
    @retval    R_SEQUENCE_FAIL          cannot perform in current state
    @retval    R_VERIFICATION_FAIL      verification failed
    @retval    R_RESOURCE_CONFLICT_FAIL resource for this process already used in other process

    @par    Details
    it verifies Boot data and decrypts by TSIP key \n

    @par    Processing flow
    -# check initialization state in TSIP_Wrapper layer
        - Is_TSIP_Wrapper_Init()
    -# output Boot data verified and decrypted by TSIP key
        - TSIP_VerifyBootData()
 ********************************************************************************/
unsigned long R_TSIP_VerifyBootData(
    unsigned char *InData_BootData,
    unsigned long InData_BootData_ByteSize,
    unsigned char *InData_BootData_HeapArea,
    unsigned char *OutData_BootData)
{
    uint8_t BoolFg;
    unsigned long RetCode;

    BoolFg = Is_TSIP_Wrapper_Init();
    if(BoolFg == TSIP_TRUE) {
        RetCode = (unsigned long)TSIP_VerifyBootData((uint8_t *)InData_BootData, (uint32_t)InData_BootData_ByteSize,
                                                (uint8_t *)InData_BootData_HeapArea, (uint8_t *)OutData_BootData);
    }
    else {
        RetCode = R_SEQUENCE_FAIL;
    }

    return RetCode;
}
