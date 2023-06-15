/*
 * Copyright (c) 2020, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*! *****************************************************************************
    @file    TSIP_Wrapper_Prepare_API.c
    @brief   SecureBoot Preparation API
    
    @par    History
    @subsection sub_history 
      - Initial version\n
 ********************************************************************************/

/*------------------------------------------------------------------------------*/
/*                        Include Files                                         */
/*------------------------------------------------------------------------------*/

#include <stddef.h>
#include <stdint.h>

#include "wrapper/TSIP_Wrapper_Prepare_API.h"
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
    @fn       unsigned long R_TSIP_Inject_Key(unsigned char *InData_ProvisioningKeyOperationCode,
                                              unsigned char *InData_KeyRingOperationCode)
    @brief    Keyring Injection
    
    @param     [in]        *InData_ProvisioningKeyOperationCode   key data combined with CustomrKey1 and CustomerKey2 including identifier (data size:36Byte)
    @param     [in]        *InData_KeyRingOperationCode       Keyring data (data size:688Byte)

    @retval    R_PASS                   success
    @retval    R_PROVISIONING_KEY_FAIL  prvisioning key failed
    @retval    R_PARAMETER_FAIL         invalid input parameter
    @retval    R_SEQUENCE_FAIL          cannot perform in current state
    @retval    R_VERIFICATION_FAIL      verification failed
    @retval    R_RESOURCE_CONFLICT_FAIL resource for this process already used in other process
    
    
    @par    Details
    it decrypts the encrypted Keyring by CustomerKey1 and CustomerKey2 and re-encrypts it by TSIP key \n
    
    @par    Processing flow
    -# check initialization state in TSIP_Wrapper layer
        - Is_TSIP_Wrapper_Init()
    -# decrypt the encypted Keyring by CustomerKey and output the Keyring ecnrypted by TSIP Key
        - TSIP_Inject_Key()
 ********************************************************************************/
unsigned long R_TSIP_Inject_Key(
    unsigned char *InData_ProvisioningKeyOperationCode,
    unsigned char *InData_KeyRingOperationCode)
{
    uint8_t BoolFg;
    unsigned long RetCode;

    BoolFg = Is_TSIP_Wrapper_Init();
    if(BoolFg == TSIP_TRUE) {
        RetCode = (unsigned long)TSIP_Inject_Key((uint8_t *)InData_ProvisioningKeyOperationCode, (uint8_t *)InData_KeyRingOperationCode);
    }
    else {
        RetCode = R_SEQUENCE_FAIL;
    }

    return RetCode;
}

/*! *****************************************************************************
    @fn       unsigned long R_TSIP_ReEncBootData(TSIP_REENC_BOOT_DATA  *tsip_reenc_bootdata)
    @brief    convert boot data (ReEnc)
    
    @param     [in/out]        *tsip_reenc_bootdata        BootData structure

    @retval    R_PASS                   success
    @retval    R_PARAMETER_FAIL         invalid input parameter
    @retval    R_SEQUENCE_FAIL          cannot perform in current state
    @retval    R_VERIFICATION_FAIL      verification failed
    @retval    R_RESOURCE_CONFLICT_FAIL resource for this process already used in other process
    
    @par    Details
    it decrypts encrypted Boot data(max 16 data) and re-encrypts by TSIP Key after verification. \n
    
    @par    Processing flow
    -# check initialization state in TSIP_Wrapper layer.
        - Is_TSIP_Wrapper_Init()
    -# decrypt encrypted Boot data and output re-encrypted Boot data by TSIP key after verification
        - TSIP_ReEncBootData()
 ********************************************************************************/
unsigned long R_TSIP_ReEncBootData(TSIP_REENC_BOOT_DATA  *tsip_reenc_bootdata)
{
    uint8_t           BoolFg;
    unsigned long     RetCode;
    REENC_BOOT_DATA   reencbootdata;
    BOOTDATA          *bdata;
    WRAP_RE_BOOT_DATA *wrap_bdata;
    unsigned long     cnt;
    char              *cdata;

    BoolFg = Is_TSIP_Wrapper_Init();
    if(BoolFg == TSIP_TRUE) {
        if(tsip_reenc_bootdata == NULL) {
            RetCode = (unsigned long)TSIP_ReEncBootData(NULL);
        }
        else {
            /* copy each member of structure for common impelemantation between 32bit and 64bit */
            cdata = (char *)&reencbootdata;
            for(cnt = 0; cnt < sizeof(reencbootdata); cnt++) {
                cdata[cnt] = 0;
            }
            bdata = (BOOTDATA *)&reencbootdata;
            wrap_bdata = (WRAP_RE_BOOT_DATA *)tsip_reenc_bootdata;
            for(cnt = 0; cnt < REENC_BOOT_DATA_MAX; cnt++, bdata++, wrap_bdata++ ) {
                bdata->InData_BootData = (uint8_t *)wrap_bdata->InData_BootData;
                bdata->InData_BootData_ByteSize = (uint64_t)wrap_bdata->InData_BootData_ByteSize;
                bdata->OutData_BootData = (uint8_t *)wrap_bdata->OutData_BootData;
            }
            RetCode = (unsigned long)TSIP_ReEncBootData(&reencbootdata);
        }
    }
    else {
        RetCode = R_SEQUENCE_FAIL;
    }

    return RetCode;
}


/*! *****************************************************************************
    @fn       unsigned long R_TSIP_Inject_Addon_Key(unsigned long InData_Operation,
                                                    unsigned char *InData_IV, 
                                                    unsigned char *InData_KeyOperationCode, 
                                                    unsigned char *OutData_KeyOperationCode)
    @brief    inject addon key
    
    @param     [in]       InData_Operation              Key information
    @param     [in]       *InData_KeyOperationCode      input key data
    @param     [out]      *OutData_KeyOperationCode     output re-encrypted key data

    @retval    R_PASS                   success
    @retval    R_PARAMETER_FAIL         invalid input parameter
    @retval    R_SEQUENCE_FAIL          cannot perform in current state
    @retval    R_VERIFICATION_FAIL      verification failed
    @retval    R_RESOURCE_CONFLICT_FAIL resource for this process already used in other process

    @par    Details
     it re-encrypts addon key \n
    
    @par    Processing flow
    -# check initialization state in TSIP_Wrapper layer
        - Is_TSIP_Wrapper_Init()
    -# re-encrypt key
        - TSIP_Inject_Addon_Key()
 ********************************************************************************/
unsigned long R_TSIP_Inject_Addon_Key(unsigned long InData_Operation, unsigned char *InData_IV, unsigned char *InData_KeyOperationCode, unsigned char *OutData_KeyOperationCode)
{
    uint8_t BoolFg;
    unsigned long RetCode;

    BoolFg = Is_TSIP_Wrapper_Init();
    if(BoolFg == TSIP_TRUE) {
        RetCode = TSIP_Inject_Addon_Key((uint32_t)InData_Operation, (uint8_t *)InData_IV, (uint8_t *)InData_KeyOperationCode, (uint8_t *)OutData_KeyOperationCode);
    }
    else {
        RetCode = R_SEQUENCE_FAIL;
    }

    return RetCode;
}


