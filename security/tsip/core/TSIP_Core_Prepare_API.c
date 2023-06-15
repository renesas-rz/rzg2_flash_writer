/*
 * Copyright (c) 2020, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*! *****************************************************************************
    @file    TSIP_Core_Prepare_API.c
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

#include "common/TSIP_Common_Define.h"
#include "core/TSIP_Core_API.h"
#include "core/TSIP_Core_Proc_API.h"
#include "core/TSIP_Core_Local_API.h"
#include "stub/TSIP_Stub_API.h"

#ifdef _TSIP_SB

/*------------------------------------------------------------------------------*/
/*                        Prototype Definition                                  */
/*------------------------------------------------------------------------------*/
static uint32_t CheckInject_Addon_KeyOperationCode(uint8_t *Operation);
static uint32_t Get_ReEncBootData_Num(REENC_BOOT_DATA *reenc_bootdata);

/*------------------------------------------------------------------------------*/
/*                        Function Definition                                   */
/*------------------------------------------------------------------------------*/
/*! *****************************************************************************
    @fn       static uint32_t CheckInject_Addon_KeyOperationCode(uint8_t *Operation)
    @brief    check key operation data
    
    @param     [in]        *Operation                Pointer of operation data

    @retval    R_PASS                   success
    @retval    R_PARAMETER_FAIL         invalid input parameter
    
    @par    Details
     It checks key operation data \n
    
    @par    Processing flow
    -# check encryption type
    -# check use attribute
 ********************************************************************************/
static uint32_t CheckInject_Addon_KeyOperationCode(uint8_t *Operation)
{
    uint32_t RetCode;
    uint8_t  Type = Operation[KEYOPERATION_CRYPTO_TYPE_OFFSET];
    uint8_t  Attr = Operation[KEYOPERATION_USE_BITFIELD_OFFSET];

    if(Type <= KEYOPERATION_CRYPTO_TYPE_MAX) {
        if(Attr & KEYOPERATION_USE_BITFIELD_UNMASK) {
            RetCode = R_PARAMETER_FAIL;
        }
        else {
            RetCode = R_PASS;
        }
    }
    else {
         RetCode = R_PARAMETER_FAIL;
    }

    return RetCode;
}

/*! *****************************************************************************
    @fn       static uint32_t Get_ReEncBootData_Num(REENC_BOOT_DATA *reenc_bootdata)
    @brief    get number of valid Boot data
    
    @param     [in]    *reenc_bootdata      BootData structure

    @retval    0                            no valid data
    @retval    1 - 16                       number of valid data
    
    @par    Details
    It gets number of valid data from BootData structure  \n
    
    @par    Processing flow
    -# check parameter
    -# get number of valid Boot data
 ********************************************************************************/
static uint32_t Get_ReEncBootData_Num(REENC_BOOT_DATA *reenc_bootdata)
{
    uint32_t DataNum;
    uint32_t LooopCount;
    uint64_t Minimum    = 0x00000110U;
    uint64_t Maximum;  /* array[0]:0xFFFFFFB0U, array[1:15]:0xFFFFFFE0U */
    BOOTDATA *bootdata;

    if(reenc_bootdata == NULL) {
        DataNum = 0;
    }
    else {
        bootdata = (BOOTDATA *)reenc_bootdata;
        for(LooopCount = 0; LooopCount < REENC_BOOT_DATA_MAX; LooopCount++, bootdata++) {
            if(bootdata->InData_BootData == NULL) {
                if(bootdata->InData_BootData_ByteSize == 0) {
                    if(bootdata->OutData_BootData == NULL) {
                        if(LooopCount != 0) {
                            DataNum = LooopCount;
                            break;
                        }
                        else {
                            DataNum = 0;
                        }
                    }
                    else {
                        DataNum = 0;
                    }
                }
                else {
                    DataNum = 0;
                }
            }
            else {
               if(LooopCount == 0) {
                   Maximum    = 0xFFFFFFB0U;
               }
               else {
                   Maximum    = 0xFFFFFFE0U;
               }
               if(bootdata->InData_BootData_ByteSize >= Minimum && 
                  bootdata->InData_BootData_ByteSize <= Maximum && 
                  ((bootdata->InData_BootData_ByteSize%16) == 0)) {
                   if(bootdata->OutData_BootData != NULL) {
                       DataNum = LooopCount + 1;
                   }
                   else {
                       DataNum = 0;
                   }
               }
               else {
                    DataNum = 0;
               }
            }
            if(DataNum == 0) {
                break;
            }
        }
    }

    return DataNum;
}


/*! *****************************************************************************
    @fn       uint32_t TSIP_Inject_Key(uint8_t *InData_ProvisioningKeyOperationCode,
                                       uint8_t *InData_KeyRingOperationCode)
    @brief    Inject Keyring
    
    @param     [in]        *InData_ProvisioningKeyOperationCode   key data combined by CustomrKey1 and CustomerKey2 including identifier (data size:36Byte)
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
    -# check initialization state in TSIP_Core layer
        - Is_TSIP_Core_Init()
    -# check transition state in TSIP Core layer
        - TSIP_Core_Check_State()
    -# check parameter
    -# perform injectoin process of Keyring
        - in the case of state01, call TSIP_Core_Prc_p03()
        - call TSIP_Core_Prc_p04()
    -# set transition state of TSIP_Core layer
        - TSIP_Core_Set_State()
 ********************************************************************************/
uint32_t TSIP_Inject_Key(
    uint8_t *InData_ProvisioningKeyOperationCode,
    uint8_t *InData_KeyRingOperationCode)
{
    uint8_t  BoolFg;
    uint32_t RetCode;

    BoolFg = Is_TSIP_Core_Init();
    RetCode = TSIP_Core_Check_State(TSIP_INJECT_KEY);
    if((BoolFg == TSIP_TRUE) && (RetCode == R_PASS)) {
        if(((InData_ProvisioningKeyOperationCode != NULL) && ((uint32_t)InData_ProvisioningKeyOperationCode[0] <= 15U)) 
         && (InData_KeyRingOperationCode != NULL)){
            RetCode = TSIP_Core_Check_State(TSIP_INJECT_KEY_SKIP);
            if(RetCode != R_PASS) {
                RetCode = TSIP_Core_Prc_p03();
            }
            if(RetCode == R_PASS) {
                RetCode = TSIP_Core_Prc_p04((uint32_t *)InData_ProvisioningKeyOperationCode, (uint32_t *)InData_KeyRingOperationCode); /* PRQA S 3305 */
                if(RetCode != R_PASS) {
                    TSIP_Core_Set_State(TSIP_CORE_STATE03);
                }
            }
        }
        else {
            RetCode = TSIP_Core_Check_State(TSIP_INJECT_KEY_SKIP);
            if(RetCode != R_PASS) {
                RetCode = TSIP_Core_Prc_p03();
                if(RetCode == R_PASS) {
                    TSIP_Core_Set_State(TSIP_CORE_STATE03);
                    RetCode = R_PARAMETER_FAIL;
                }
            }
            else {
                RetCode = R_PARAMETER_FAIL;
            }
        }
    }
    else {
        RetCode = R_SEQUENCE_FAIL;
    }

    if(RetCode == R_PASS) {
        TSIP_Core_Set_State(TSIP_CORE_STATE02);
    }

    return RetCode;
}

/*! *****************************************************************************
    @fn       uint32_t TSIP_ReEncBootData(REENC_BOOT_DATA *reenc_bootdata)
    @brief   convert boot data (ReEnc)
    
    @param     [in/out]    *reenc_bootdata  BootData structure

    @retval    R_PASS                       success
    @retval    R_PARAMETER_FAIL             invalid input parameter
    @retval    R_SEQUENCE_FAIL              cannot perform in current state
    @retval    R_VERIFICATION_FAIL          verification failed
    @retval    R_RESOURCE_CONFLICT_FAIL     resource for this process already used in other process
    
    @par    Details
    it decrypts encrypted Boot data(max 16 data) and re-encrypts by TSIP Key after verification. \n
    
    @par    Processing flow
    -# check initialization state in TSIP_Core layer
        - Is_TSIP_Core_Init()
    -# check transition state in TSIP Core layer
        - TSIP_Core_Check_State()
    -# check parameter
    -# get number of valid Boot data
    -# decrypt encrypted Boot data and output re-encrypted Boot data
       - in the case of state01, call TSIP_Core_Prc_p13()
       - call TSIP_Core_Prc_p14()  set number of valid Boot data
       - call TSIP_Core_Prc_p15()  loop for number of valid data and set Boot data
    -# set transition state in TSIP_Core layer
        - TSIP_Core_Set_State()
 ********************************************************************************/
uint32_t TSIP_ReEncBootData(REENC_BOOT_DATA *reenc_boot_data)
{
    uint8_t  BoolFg;
    uint32_t RetCode;
    uint32_t DataNum;
    uint32_t LoopCount;
    BOOTDATA *bootdata;
    BOOTDATA *bootdata0;
    
    BoolFg = Is_TSIP_Core_Init();
    RetCode = TSIP_Core_Check_State(TSIP_REENC_BOOTDATA);
    if((BoolFg == TSIP_TRUE) && (RetCode == R_PASS)) {
        if(reenc_boot_data != NULL) {
            RetCode = TSIP_Core_Check_State(TSIP_REENC_BOOTDATA_SKIP);
            if(RetCode != R_PASS) {
                RetCode = TSIP_Core_Prc_p13();
                if(RetCode == R_PASS) {
                    TSIP_Core_Set_State(TSIP_CORE_STATE24);
                }
            }
            if(RetCode == R_PASS) {
                DataNum = Get_ReEncBootData_Num(reenc_boot_data);
                if(DataNum == 0) {
                    RetCode = R_PARAMETER_FAIL;
                }
            }
            if(RetCode == R_PASS) {
                RetCode = TSIP_Core_Prc_p14(DataNum);
                if(RetCode == R_PASS) {
                    bootdata0 = (BOOTDATA *)reenc_boot_data;
                    bootdata  = (BOOTDATA *)reenc_boot_data;
                    for(LoopCount = 0; LoopCount < DataNum; LoopCount++, bootdata++) {
                        RetCode = TSIP_Core_Prc_p15((const uint32_t *)bootdata->InData_BootData,  /* PRQA S 3305 */
                                                            (uint32_t)bootdata->InData_BootData_ByteSize,
                                                          (uint32_t *)bootdata->OutData_BootData, /* PRQA S 3305 */
                                                          (uint32_t *)&bootdata0->OutData_BootData[bootdata0->InData_BootData_ByteSize+16]); /* PRQA S 3305 */
                        if(RetCode != R_PASS) {
                            break;
                        }
                    }
                }
                if(RetCode != R_PASS) {
                    TSIP_Core_Set_State(TSIP_CORE_STATE24);
                }
                else {
                    TSIP_Core_Set_State(TSIP_CORE_STATE23);
                }
            }
        }
        else {
            RetCode = TSIP_Core_Check_State(TSIP_REENC_BOOTDATA_SKIP);
            if(RetCode != R_PASS) {
                RetCode = TSIP_Core_Prc_p13();
                if(RetCode == R_PASS) {
                    TSIP_Core_Set_State(TSIP_CORE_STATE24);
                    RetCode = R_PARAMETER_FAIL;
                }
            }
            else {
                RetCode = R_PARAMETER_FAIL;
            }
        }
    }
    else {
        RetCode = R_SEQUENCE_FAIL;
    }

    if(RetCode == R_PASS) {
        TSIP_Core_Set_State(TSIP_CORE_STATE23);
    }

    return RetCode;
}

/*! *****************************************************************************
    @fn       uint32_t TSIP_Inject_Addon_Key(uint32_t InData_Operation, uint8_t *InData_IV, uint8_t *InData_KeyOperationCode, uint8_t *OutData_KeyOperationCode)
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
    -# check initialization state in TSIP_Core layer
        - Is_TSIP_Core_Init()
    -# check transition state in TSIP Core layer
        - TSIP_Core_Check_State()
    -# check parameter
    -# check key operation data
        - CheckInject_Addon_KeyOperationCode()
    -# re-encrypt key data
        - if key install is not selected, call TSIP_Core_Prc_p10()
        - call TSIP_Core_Prc_p11()
    -# set transition state in TSIP_Core layer
        - TSIP_Core_Set_State()
 ********************************************************************************/
uint32_t TSIP_Inject_Addon_Key(uint32_t InData_Operation, uint8_t *InData_IV, uint8_t *InData_KeyOperationCode, uint8_t *OutData_KeyOperationCode)
{
    uint8_t  BoolFg;
    uint32_t RetCode;
    
    BoolFg = Is_TSIP_Core_Init();
    RetCode = TSIP_Core_Check_State(TSIP_INJECT_ADDON_KEY);
    if((BoolFg == TSIP_TRUE) && (RetCode == R_PASS)) {
        RetCode = CheckInject_Addon_KeyOperationCode((uint8_t *)&InData_Operation);
        if(RetCode == R_PASS) {
            if((InData_IV != NULL) && (InData_KeyOperationCode != NULL) && (OutData_KeyOperationCode !=NULL) ) {
                RetCode = TSIP_Core_Check_State(TSIP_INJECT_ADDON_KEY_SKIP);
                if(RetCode != R_PASS) {
                    RetCode = TSIP_Core_Prc_p10();
                }
                if(RetCode == R_PASS) {
                    RetCode = TSIP_Core_Prc_p11(InData_Operation, (uint32_t *)InData_IV, (uint32_t *)InData_KeyOperationCode, (uint32_t *)OutData_KeyOperationCode);  /* PRQA S 3305 */
                    if(RetCode != R_PASS) {
                        if(TSIP_Core_Check_State(TSIP_INJECT_ADDON_KEY_FIXEDSTATE) != R_PASS) {  /* if state is State20 then don't move to state */
                            TSIP_Core_Set_State(TSIP_CORE_STATE21);
                        }
                    }
                }
            }
            else {
                RetCode = R_PARAMETER_FAIL;
            }
        }
        else {
            RetCode = R_PARAMETER_FAIL;
        }

        if(RetCode == R_PARAMETER_FAIL) {
            RetCode = TSIP_Core_Check_State(TSIP_INJECT_ADDON_KEY_SKIP);
            if(RetCode != R_PASS) {
                RetCode = TSIP_Core_Prc_p10();
                if(RetCode == R_PASS) {
                    TSIP_Core_Set_State(TSIP_CORE_STATE21);
                    RetCode = R_PARAMETER_FAIL;
                }
            }
            else {
                RetCode = R_PARAMETER_FAIL;
            }
        }
    }
    else {
        RetCode = R_SEQUENCE_FAIL;
    }

    if(RetCode == R_PASS) {
        TSIP_Core_Set_State(TSIP_CORE_STATE20);
    }

    return RetCode;
}

#endif /* _TSIP_SB */
