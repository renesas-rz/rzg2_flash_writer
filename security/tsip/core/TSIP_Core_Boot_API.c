/*
 * Copyright (c) 2020, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*! *****************************************************************************
    @file    TSIP_Core_Boot_API.c
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

#include "common/TSIP_Common_Define.h"
#include "stub/TSIP_Stub_API.h"
#include "core/TSIP_Core_API.h"
#include "core/TSIP_Core_Proc_API.h"
#include "core/TSIP_Core_Local_API.h"

/*------------------------------------------------------------------------------*/
/*                        Prototype Definition                                  */
/*------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------*/
/*                        Function Definition                                   */
/*------------------------------------------------------------------------------*/
/*! *****************************************************************************
    @fn       static uint32_t Check_VerifyBootData_Param(uint8_t *InData_BootData,
                                                         uint32_t InData_BootData_ByteSize,
                                                         uint8_t *InData_BootData_HeapArea,
                                                         uint8_t  *OutData_BootData)
    @brief    check parameter of Boot data
    
    @param     [in]        *InData_BootData            Pointer of encrypted Boot data by TSIP key
    @param     [in]        InData_BootData_ByteSize    Size of encrypted Boot data by TSIP key(multiple of 16byte)
    @param     [in]        *InData_BootData_HeapArea   Pointer of Heap
    @param     [out]       *OutData_BootData           Pointer of decrypted Boot data
    
    @retval    R_PASS                   correct
    @retval    R_PARAMETER_FAIL         incorrect
    
    @par    Details
    It checks Boot data parameter \n
    
    @par    Processing flow
    -# check if address is NULL
    -# check data size
 ********************************************************************************/
static uint32_t Check_VerifyBootData_Param(uint8_t *InData_BootData,
                             uint32_t InData_BootData_ByteSize,
                             uint8_t *InData_BootData_HeapArea,
                              uint8_t *OutData_BootData)
{
    uint32_t RetCode;
    uint32_t Minimum;
    uint32_t Maximum;

    if((InData_BootData != NULL) && (InData_BootData_HeapArea != NULL) && (OutData_BootData != NULL)) {
        RetCode = TSIP_Core_Check_State(TSIP_VERIFYBOOTDATA_SKIP);
        if(RetCode == R_PASS) {
            Minimum = 0x00000120U;
        }
        else {
            /* Top Data include Header */
            Minimum = 0x00000150U;
        }
        Maximum = 0xFFFFFFF0U;

        if((InData_BootData_ByteSize >= Minimum) && (InData_BootData_ByteSize <= Maximum) &&
           ((InData_BootData_ByteSize % 16U) == 0)) {
             RetCode = R_PASS;
        }
        else {
             RetCode = R_PARAMETER_FAIL;
        }
    }
    else {
        RetCode = R_PARAMETER_FAIL;
    }
    return RetCode;
}
/*! *****************************************************************************
    @fn       uint32_t TSIP_VerifyBootData(uint8_t *InData_BootData,
                                           uint32_t InData_BootData_ByteSize, 
                                           uint8_t *InData_BootData_HeapArea, 
                                           uint8_t  *OutData_BootData)
    @brief    verify and decrypt Boot data
    
    @param     [in]        *InData_BootData            Pointer of encrypted Boot data by TSIP key
    @param     [in]        InData_BootData_ByteSize    Size of encrypted Boot data by TSIP key(multiple of 16byte)
    @param     [in]        *InData_BootData_HeapArea   Pointer of Heap(Size: InData_BootData_ByteSize – 272)
    @param     [out]       *OutData_BootData           Pointer of decrypted Boot data
    
    @retval    R_PASS                   success
    @retval    R_PROC_CONT              continue procedure
    @retval    R_PARAMETER_FAIL         invalid input parameter
    @retval    R_SEQUENCE_FAIL          cannot perform in current state
    @retval    R_VERIFICATION_FAIL      verification failed
    @retval    R_RESOURCE_CONFLICT_FAIL resource for this process already used in other process
    
    @par    Details
    It decrypts Boot data by TSIP key \n
    
    @par    Processing flow
    -# check initialization in TSIP_Core layer
        - Is_TSIP_Core_Init()
    -# check transition state in TSIP Core layer
        - TSIP_Core_Check_State()
    -# check parameter
    -# get and output uImage data encrypted by TSIP key
        - TSIP_Core_Prc_p17()
    -# set transition state in TSIP_Core layer
        - TSIP_Core_Set_State()
 ********************************************************************************/
uint32_t TSIP_VerifyBootData(uint8_t *InData_BootData,
                             uint32_t InData_BootData_ByteSize,
                             uint8_t *InData_BootData_HeapArea,
                             uint8_t *OutData_BootData)
{
    uint8_t  BoolFg;
    uint32_t RetCode;
    uint32_t ByteSize;

    BoolFg = Is_TSIP_Core_Init();
    RetCode = TSIP_Core_Check_State(TSIP_VERIFYBOOTDATA);
    if((BoolFg == TSIP_TRUE) && (RetCode == R_PASS)) {
        RetCode = Check_VerifyBootData_Param(InData_BootData, InData_BootData_ByteSize, InData_BootData_HeapArea, OutData_BootData);
        if(RetCode == R_PASS) {
            RetCode = TSIP_Core_Check_State(TSIP_VERIFYBOOTDATA_SKIP);
            if(RetCode != R_PASS) {
                /* Top Data include Header */
                ByteSize = InData_BootData_ByteSize - 48U;
                RetCode = TSIP_Core_Prc_p16((uint32_t *)&InData_BootData[ByteSize]);  /* PRQA S 3305 */
            }
            else {
                ByteSize = InData_BootData_ByteSize;
                RetCode = R_PASS;
            }
            if(RetCode == R_PASS) {
                RetCode = TSIP_Core_Prc_p17((uint32_t *)InData_BootData, ByteSize, (uint32_t *)InData_BootData_HeapArea, (uint32_t *)OutData_BootData);  /* PRQA S 3305 */
            }
        }
        else {
            RetCode = R_PARAMETER_FAIL;
        }
    }
    else {
        RetCode = R_SEQUENCE_FAIL;
    }

    if(RetCode == R_PASS) {
        TSIP_Core_Set_State(TSIP_CORE_STATE27);
    }
    else if(RetCode == R_PROC_CONT) {
        TSIP_Core_Set_State(TSIP_CORE_STATE25);
        RetCode = R_PASS;
    }
    else if(RetCode == R_VERIFICATION_FAIL) {
        TSIP_Core_Set_State(TSIP_CORE_STATE_STOP);
    }
    else if((RetCode == R_PARAMETER_FAIL) && (TSIP_Core_Check_State(TSIP_VERIFYBOOTDATA_SKIP) != R_PASS)) {
        TSIP_Core_Set_State(TSIP_CORE_STATE26);
    }
    else {
        /* do nothing */
    }

    return RetCode;
}

