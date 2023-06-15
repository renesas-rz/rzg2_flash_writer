/*
 * Copyright (c) 2020, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*! *****************************************************************************
    @file    TSIP_Core_Proc_API.c
    @brief   TSIP Core Layer Procedure API
    
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
#include "proc/TSIP_Procedure.h"
#include "stub/TSIP_Stub_API.h"

static unsigned char OutPut_S_INST[1296U];

/*! @var TSIP_Core_Share_InData_IV
    @brief IV for Keyring injection
*/
static const uint8_t TSIP_Core_Share_InData_IV[] = {
    0x85, 0xc1, 0x67, 0x34, 0x83, 0xd5, 0xd2, 0x91,  0xf0, 0xd0, 0x71, 0x3e,  0x3e, 0xa4, 0x34, 0xa3,
};

/*! *****************************************************************************
    @fn       uint32_t TSIP_Core_Prc_softreset(void)
    @brief    software reset
    
    @param    void
    
    @retval    R_PASS                success
    
    @par    Details
    It performs software reset.

    @par    Processing flow
    -# perform software reset
        - TSIPPrc_p00()
 ********************************************************************************/
uint32_t TSIP_Core_Prc_softreset(void)
{
    TSIPPrc_p00();
    return R_PASS;
}

/*! *****************************************************************************
    @fn       uint32_t TSIP_Core_Prc_p01(void)
    @brief    TSIP self-check1
    
    @param    void
    
    @retval    R_PASS                           success
    @retval    R_INITIALIZATION_ERROR           TSIP initialization error
    @retval    R_RESOURCE_CONFLICT_FAIL         resource for this process already used in other process
    
    @par    Details
    It performs TSIP self-check1

    @par    Processing flow
    -# TSIP self-check1
        - TSIPPrc_p01()
 ********************************************************************************/
uint32_t TSIP_Core_Prc_p01(void)
{
    uint32_t RetCode;
    uint8_t  Ret;

    Ret = TSIPPrc_p01();
    switch(Ret) {
        case SELFTEST_FAIL:
            RetCode = R_INITIALIZATION_ERROR;
            break;
        case RETURN_OK:
            RetCode = R_PASS;
            break;
        default:
           RetCode = R_RESOURCE_CONFLICT_FAIL;
            break;
    }
    
    return RetCode;
}

/*! *****************************************************************************
    @fn       uint32_t TSIP_Core_Prc_p02(void)
    @brief    TSIP self-check2
    
    @retval    R_PASS                           success
    @retval    R_PROC_RETRY                     retry
    @retval    R_RESOURCE_CONFLICT_FAIL         resource for this process already used in other process

    @par    Details
    It performs TSIP self-check2

    @par    Processing flow
    -# TSIP self-check2
        - TSIPPrc_p02()
 ********************************************************************************/
uint32_t TSIP_Core_Prc_p02(void)
{
    uint32_t RetCode;
    uint8_t  Ret;

    Ret = TSIPPrc_p02();
    switch(Ret) {
        case PROC_RETRY:
            RetCode = R_PROC_RETRY;
            break;
        case RETURN_OK:
            RetCode = R_PASS;
            break;
        default:
            RetCode = R_RESOURCE_CONFLICT_FAIL;
            break;
    }
    
    return RetCode;
}

/*! *****************************************************************************
    @fn       uint32_t TSIP_Core_Prc_p03(void)
    @brief    change status for injection
    
    @param    void
    
    @retval    R_PASS                       success
    @retval    R_RESOURCE_CONFLICT_FAIL     resource for this process already used in other process
    
    @par    Details
    It performs to change status for injection

    @par    Processing flow
    -# change status for injection
        - TSIPPrc_p03()
 ********************************************************************************/
uint32_t TSIP_Core_Prc_p03(void)
{
    uint32_t RetCode;
    uint8_t Ret;

    Ret = TSIPPrc_p03();
    if(Ret == RETURN_OK) {
        RetCode = R_PASS;
    }
    else {
        RetCode = R_RESOURCE_CONFLICT_FAIL;
    }
    
    return RetCode;
}

/*! *****************************************************************************
    @fn       uint32_t TSIP_Core_Prc_p04(const uint32_t *InData_CustomerKeyOperationCode, const uint32_t *InData_KeyRingOperationCode)
    @brief    Inject Key
    
    @param     [in]        *InData_CustomerKeyOperationCode   key data combined with CustomrKey1 and CustomerKey2 including identifier (data size:36Byte)
    @param     [in]        *InData_KeyRingOperationCode       Keyring data (data size:1840Byte)
    
    @retval    R_PASS                           success
    @retval    R_VERIFICATION_FAIL              verification failed
    @retval    R_PROVISIONING_KEY_FAIL          provisioning key failed
    @retval    R_RESOURCE_CONFLICT_FAIL         resource for this process already used in other process
    
    @par    Details
    It moves to Inject process.

    @par    Processing flow
    -# perform Inject Key process
        - TSIPPrc_p04()
 ********************************************************************************/
uint32_t TSIP_Core_Prc_p04(const uint32_t *InData_CustomerKeyOperationCode, const uint32_t *InData_KeyRingOperationCode)
{
    uint32_t  RetCode;
    uint8_t   Ret;
    uint8_t   Count;
    uint32_t  InData_SharedKeyIndex[1];
    uint32_t  InData_SessionKey[8];
    uint32_t  i;
    uint8_t   *workp;
    
    for(i = 0; i < sizeof(OutPut_S_INST); i++) {
        OutPut_S_INST[i] = 0U;
    }
    InData_SharedKeyIndex[0] = InData_CustomerKeyOperationCode[0];
    for(Count = 1; Count < 9; Count++){
        InData_SessionKey[Count - 1] = InData_CustomerKeyOperationCode[Count];
    }
    Ret = TSIPPrc_p04(InData_SharedKeyIndex, InData_SessionKey, (const uint32_t *)TSIP_Core_Share_InData_IV, InData_KeyRingOperationCode, (uint32_t *)OutPut_S_INST);  /* PRQA S 3305 */
    if(Ret == RETURN_OK) {
        RetCode = R_PASS;
        workp = (uint8_t *)S_INST;
        for(i = 0; i < sizeof(OutPut_S_INST); i++) {
             workp[i] = OutPut_S_INST[i];
        }
    }
    else if(Ret == INTERNAL_ERROR) {
        RetCode = R_PROVISIONING_KEY_FAIL;
    }
    else if(Ret == VERIFY_ERROR) {
        RetCode = R_VERIFICATION_FAIL;
    }
    else {
        RetCode = R_RESOURCE_CONFLICT_FAIL;
    }
    
    return RetCode;
}

/*! *****************************************************************************
    @fn       uint32_t TSIP_Core_Prc_p10(void)
    @brief    Inject key for basic encryption
    
    @param    void
    
    @retval    R_PASS                       success
    @retval    R_RESOURCE_CONFLICT_FAIL     resource for this process already used in other process
    
    @par    Details
    It moves to inject key process for basic encryption

    @par    Processing flow
    -# perform key install for basic encryption
        - TSIPPrc_p10()
 ********************************************************************************/
uint32_t TSIP_Core_Prc_p10(void)
{
    uint32_t RetCode;
    uint8_t  Ret;

    Ret = TSIPPrc_p10();
    if(Ret == RETURN_OK) {
        RetCode = R_PASS;
    }
    else {
        RetCode = R_RESOURCE_CONFLICT_FAIL;
    }
    
    return RetCode;
}

/*! *****************************************************************************
    @fn       uint32_t TSIP_Core_Prc_p11(uint32_t InData_Operation, const uint32_t *InData_IV, const uint32_t *InData_KeyOperationCode, uint32_t *OutData_KeyOperationCode)
    @brief    install key for basic encryption 
    
    @param     [in]        InData_Operation          Key information
    @param     [in]        *InData_IV                Initial Vector
    @param     [in]        *InData_KeyOperationCode  Input key data
    @param     [out]       *OutData_KeyOperationCode Output key data
    
    @retval    R_PASS                       success
    @retval    R_RESOURCE_CONFLICT_FAIL     resource for this process already used in other process
    @retval    R_PARAMETER_FAIL             invalid input parameter
    @retval    R_VERIFICATION_FAIL          verification failed

    @par    Details
    It installs key for basic encryption

    @par    Processing flow
    -# perform key install process for basic encryption
        - TSIPPrc_p11()
 ********************************************************************************/
uint32_t TSIP_Core_Prc_p11(
    uint32_t InData_Operation,
    const uint32_t *InData_IV,
    const uint32_t *InData_KeyOperationCode,
          uint32_t *OutData_KeyOperationCode)
{
    uint32_t RetCode;
    uint8_t  Ret;


    Ret = TSIPPrc_p11(&InData_Operation, InData_IV, InData_KeyOperationCode, OutData_KeyOperationCode);
    if(Ret == RETURN_OK) {
        RetCode = R_PASS;
    }
    else if(Ret == ILLEGAL_PARAMETER) {
        RetCode = R_PARAMETER_FAIL;
    }
    else if(Ret == VERIFY_ERROR) {
        RetCode = R_VERIFICATION_FAIL;
    }
    else {
        RetCode = R_RESOURCE_CONFLICT_FAIL;
    }
   
    return RetCode;
}

/*! *****************************************************************************
    @fn       uint32_t TSIP_Core_Prc_p12(void)
    @brief    verify Keyring
    
    @retval    R_PASS                       success
    @retval    R_VERIFICATION_FAIL          verification failed
    @retval    R_KEYRING_FORMAT_FAIL        Keyring format failed
    @retval    R_RESOURCE_CONFLICT_FAIL     resource for this process already used in other process
    
    @par    Deatails
    It verifies Keyring.

    @par    Processing flow
    -#  perform Keyring verification process
        - TSIPPrc_p12()
 ********************************************************************************/
uint32_t TSIP_Core_Prc_p12(void)
{
    uint32_t RetCode;
    uint8_t  Ret;

    Ret = TSIPPrc_p12();
    if(Ret == RETURN_OK) {
        RetCode = R_PASS;
    }
    else if(Ret == VERIFY_ERROR) {
        RetCode = R_VERIFICATION_FAIL;
    }
    else if(Ret == FORMAT_ERROR) {
        RetCode = R_KEYRING_FORMAT_FAIL;
    }
    else {
        RetCode = R_RESOURCE_CONFLICT_FAIL;
    }
   
    return RetCode;
}

/*! *****************************************************************************
    @fn       uint32_t TSIP_Core_Prc_p13(void)
    @brief    select write process for Boot Program
    
    @param    void
    
    @retval    R_PASS                       success
    @retval    R_RESOURCE_CONFLICT_FAIL     resource for this process already used in other process
    
    @par    Details
    It selects write process for Boot Program

    @par    Processing flow
    -# Select write process for Boot Program
        - TSIPPrc_p13()
 ********************************************************************************/
uint32_t TSIP_Core_Prc_p13(void)
{
    uint32_t RetCode;
    uint8_t  Ret;

    Ret = TSIPPrc_p13();
    if(Ret == RETURN_OK) {
        RetCode = R_PASS;
    }
    else {
        RetCode = R_RESOURCE_CONFLICT_FAIL;
    }
    
    return RetCode;
}

/*! *****************************************************************************
    @fn       uint32_t TSIP_Core_Prc_p14(uint32_t InData_Prog_Num)
    @brief    specify number of Boot Program
    
    @param     [in]        InData_Prog_Num  number of Boot Program
    
    @retval    R_PASS                       success
    @retval    R_PARAMETER_FAIL             invalid input parameter
    @retval    R_RESOURCE_CONFLICT_FAIL     resource for this process already used in other process
    
    @par    Details
    It specifies number of Boot Program for writting

    @par    Processing flow
    -# specify number of Boot Program
        - TSIPPrc_p14()
 ********************************************************************************/
uint32_t TSIP_Core_Prc_p14(uint32_t InData_Prog_Num)
{
    uint32_t RetCode;
    uint8_t  Ret;

    Ret = TSIPPrc_p14((const uint32_t *)&InData_Prog_Num);
    if(Ret == RETURN_OK) {
        RetCode = R_PASS;
    }
    else if(Ret == ILLEGAL_PARAMETER) {
        RetCode = R_PARAMETER_FAIL;
    }
    else {
        RetCode = R_RESOURCE_CONFLICT_FAIL;
    }
    
    return RetCode;
}

/*! *****************************************************************************
    @fn       uint32_t TSIP_Core_Prc_p15(const uint32_t *InData_Prog, uint32_t InData_Prog_ByteSize, uint32_t *OutData_Prog, uint32_t *OutData_Header)
    @brief    write Boot Program
    
    @param     [in]        *InData_Prog             Pointer of encrypted Boot Program
    @param     [in]        InData_Prog_ByteSize     Size of encrypted Boot Program(multiple of 16)
    @param     [out]       *OutData_Prog            Pointer of re-encrypted Boot Program by TSIP Key
    @param     [out]       *OutData_Header          Pointer of Heder Information(48Byte)
    
    @retval    R_PASS                       success
    @retval    R_VERIFICATION_FAIL          verification failed
    @retval    R_RESOURCE_CONFLICT_FAIL     resource for this process already used in other process
    @retval    PROC_CONT                    process continue
    
    @par    Details
    It writes Boot Program

    @par    Processing flow
    -# Perform write process for Boot Program
        - TSIPPrc_p15()
 ********************************************************************************/
uint32_t TSIP_Core_Prc_p15(const uint32_t *InData_Prog, uint32_t InData_Prog_ByteSize, uint32_t *OutData_Prog, uint32_t *OutData_Header)
{
    uint32_t RetCode;
    uint8_t  Ret;

    InData_Prog_ByteSize = InData_Prog_ByteSize / 4U;
    
    Ret = TSIPPrc_p15(InData_Prog, (const uint32_t *)&InData_Prog_ByteSize, OutData_Prog, OutData_Header);
    if((Ret == RETURN_OK) || (Ret == PROC_CONT)) {
        RetCode = R_PASS;
    }
    else if(Ret == VERIFY_ERROR) {
        RetCode = R_VERIFICATION_FAIL;
    }
    else {
        RetCode = R_RESOURCE_CONFLICT_FAIL;
    }
    
    return RetCode;
}

/*! *****************************************************************************
    @fn       uint32_t TSIP_Core_Prc_p16(const uint32_t *InData_Header)
    @brief    select verification process for Boot Program
    
    @param     [in]        *InData_Header   Pointer of header information
    
    @retval    R_PASS                       success
    @retval    R_VERIFICATION_FAIL          verification failed
    @retval    R_RESOURCE_CONFLICT_FAIL     resource for this process already used in other process
    
    @par    Details
    It selects verification process of Boot Program.

    @par    Processing flow
    -# select verification process 
        - TSIPPrc_p16()
 ********************************************************************************/
uint32_t TSIP_Core_Prc_p16(const uint32_t *InData_Header)
{
    uint32_t RetCode;
    uint8_t  Ret;

    
    Ret = TSIPPrc_p16(InData_Header);
    if(Ret == RETURN_OK) {
        RetCode = R_PASS;
    }
    else if(Ret == VERIFY_ERROR) {
        RetCode = R_VERIFICATION_FAIL;
    }
    else {
        RetCode = R_RESOURCE_CONFLICT_FAIL;
    }
    
    return RetCode;
}

/*! *****************************************************************************
    @fn       uint32_t TSIP_Core_Prc_p17(const uint32_t *InData_Prog, uint32_t InData_Prog_ByteSize, uint32_t *InData_HeapArea, uint32_t *OutData_Prog)
    @brief    Verify Boot Progaram
    
    @param     [in]        *InData_Prog              Pointer of re-encrypted program by TSIP Key
    @param     [in]        InData_Prog_ByteSize      Size of re-encrypted program by TSIP Key(multiple of 16)
    @param     [in]        *InData_HeapArea          Heap area using by TSIP
    @param     [out]       *OutData_Prog             Pointer of decrypted program by TSIP Key
    
    @retval    R_PASS                       success(all data)
    @retval    R_PROC_CONT                  success
    @retval    R_VERIFICATION_FAIL          verification failed
    @retval    R_RESOURCE_CONFLICT_FAIL     resource for this process already used in other process
    
    @par    Details
    It verifies Boot Program.

    @par    Processing flow
    -# perform verification for Boot Program
        - TSIPPrc_p17()
 ********************************************************************************/
uint32_t TSIP_Core_Prc_p17(const uint32_t *InData_Prog, uint32_t InData_Prog_ByteSize, uint32_t *InData_HeapArea, uint32_t *OutData_Prog)
{
    uint32_t RetCode;
    uint8_t  Ret;

    S_HEAP = InData_HeapArea;

    InData_Prog_ByteSize = InData_Prog_ByteSize / 4U;
    
    Ret = TSIPPrc_p17(InData_Prog, &InData_Prog_ByteSize, OutData_Prog);
    if((Ret == RETURN_OK)) {
        RetCode = R_PASS;
    }
    else if(Ret == PROC_CONT) {
        RetCode = R_PROC_CONT;
    }
    else if(Ret == VERIFY_ERROR) {
        RetCode = R_VERIFICATION_FAIL;
    }
    else {
        RetCode = R_RESOURCE_CONFLICT_FAIL;
    }
    
    return RetCode;
}


/*! *****************************************************************************
    @fn       uint32_t TSIP_Core_Prc_p40(void)
    @brief    Initialize update process
    
    @param     void        *Param   not used
    
    @retval    R_PASS                       success
    @retval    R_FALSIFICATION_ERROR        tamper detection error
    @retval    R_RESOURCE_CONFLICT_FAIL     resource for this process already used in other process
    
    @par    Details
    It initializes update process

    @par    Processing flow
    -# perform initialization of update process
        - TSIPPrc_p40()
 ********************************************************************************/
uint32_t TSIP_Core_Prc_p40(void)
{
    uint32_t RetCode;
    uint8_t  Ret;

    Ret = TSIPPrc_p40();
    if(Ret == RETURN_OK) {
        RetCode = R_PASS;
    }
    else if(Ret == FALSIFICATION_ERROR) {
        RetCode = R_FALSIFICATION_ERROR;
    }
    else {
        RetCode = R_RESOURCE_CONFLICT_FAIL;
    }
    
    return RetCode;
}

/*! *****************************************************************************
    @fn       uint32_t TSIP_Core_Prc_p45(const uint32_t *InData_Prog, uint32_t InData_Prog_ByteSize, 
                                         uint32_t *OutData_Prog)
    @brief    update Boot Program
    
    @param     [in]        *InData_ProgNo          Pointer of number of update Boot Program
    @param     [in]        *InData_Prog            Pointer of encrypted Boot Program
    @param     [in]        InData_Prog_ByteSize    Byte size of Boot Program
    @param     [out]       *OutData_Prog           Pointer of re-encrypted Boot Program by TSIP Key

    @retval    R_PASS                       success
    @retval    R_RESOURCE_CONFLICT_FAIL     resource for this process already used in other process
    @retval    R_VERIFICATION_FAIL          verification failed
    @retval    R_PARAMETER_FAIL             invalid InData_ProgNo
    @retval    R_FALSIFICATION_ERROR        tamper detection error

    @par    Details
    It perform Boot Program update

    @par    Processing flow
    -# perform Boot Program update
        - TSIPPrc_p45()
 ********************************************************************************/
uint32_t TSIP_Core_Prc_p45(
           uint32_t InData_ProgNo,
     const uint32_t *InData_Prog, 
           uint32_t InData_Prog_ByteSize, 
           uint32_t *OutData_Prog)
{
    uint32_t RetCode;
    uint8_t Ret;

    InData_Prog_ByteSize = InData_Prog_ByteSize / 4;
    Ret = TSIPPrc_p45((const uint32_t *)&InData_ProgNo, InData_Prog, (const uint32_t *)&InData_Prog_ByteSize, OutData_Prog);
    if((Ret == RETURN_OK) || (Ret == PROC_CONT)) {
        RetCode = R_PASS;
    }
    else if(Ret == VERIFY_ERROR) {
        RetCode = R_VERIFICATION_FAIL;
    }
    else if(Ret == ILLEGAL_PARAMETER) {
        RetCode = R_PARAMETER_FAIL;
    }
    else if(Ret == FALSIFICATION_ERROR) {
        RetCode = R_FALSIFICATION_ERROR;
    }
    else {
        RetCode = R_RESOURCE_CONFLICT_FAIL;
    }
   
    return RetCode;
}

/*! *****************************************************************************
    @fn       uint32_t TSIP_Core_Prc_p43(const uint32_t *InData_KeyRingOperationCode)
    @brief    Keyring decryption
    
    @param     [in]        *InData_KeyRingOperationCode     Pointer of Keyring

    @retval    R_PASS                       success
    @retval    R_RESOURCE_CONFLICT_FAIL     resource for this process already used in other process
    @retval    R_VERIFICATION_FAIL          verification failed
    @retval    R_FALSIFICATION_ERROR        tamper detection error
    
    @par    Details
    It re-encrypts Keyring by TSIP Key

    @par    Processing flow
    -# re-encrypt Keyring by TSIP Key
        - TSIPPrc_p43()
 ********************************************************************************/
uint32_t TSIP_Core_Prc_p43(const uint32_t *InData_KeyRingOperationCode)
{
    uint32_t RetCode;
    uint8_t  Ret;

    TSIP_Memset(OutPut_S_INST, 0, sizeof(OutPut_S_INST));
    Ret = TSIPPrc_p43((const uint32_t *)TSIP_Core_Share_InData_IV, InData_KeyRingOperationCode, (uint32_t *)OutPut_S_INST); /* PRQA S 3305 */
    if(Ret == RETURN_OK) {
        RetCode = R_PASS;
        TSIP_Memcpy(S_INST, OutPut_S_INST, sizeof(OutPut_S_INST));
    }
    else if(Ret == VERIFY_ERROR) {
        RetCode = R_VERIFICATION_FAIL;
    }
    else if(Ret == FALSIFICATION_ERROR) {
        RetCode = R_FALSIFICATION_ERROR;
    }
    else {
        RetCode = R_RESOURCE_CONFLICT_FAIL;
    }

     return RetCode;
}

/*! *****************************************************************************
    @fn       uint32_t TSIP_Core_Prc_p3f(uint32_t InData_GenByteSize, uint32_t *OutData)
    @brief    random number generation
    
    @param     [in]        InData_GenByteSize         Byte of generated random number
    @param     [out]       *OutData                   Pointer of random number

    @retval    R_PASS                       success
    @retval    R_RESOURCE_CONFLICT_FAIL     resource for this process already used in other process
    @retval    R_PARAMETER_FAIL             invalid input parameter
    
    @par    Details
    It generates random number \n
    
    @par    Processing flow
    -# Perform random number generation process
        - TSIPPrc_p3f()
 ********************************************************************************/
uint32_t TSIP_Core_Prc_p3f(
    uint32_t InData_GenByteSize, 
    uint32_t *OutData)
{
    uint32_t RetCode = R_PASS;
    uint32_t OutData_Text[4];
    uint32_t Count;
    uint32_t Last_Len;
    uint32_t iLoop;
    uint8_t  Ret = RETURN_OK;

    if((InData_GenByteSize != 0U) && (OutData != NULL)) {
        Count = InData_GenByteSize / 16U;
        Last_Len = InData_GenByteSize % 16U;
        for(iLoop = 0U; iLoop < Count; iLoop++) {
            Ret = TSIPPrc_p3f(OutData_Text);
            if(Ret != RETURN_OK) {
                RetCode = R_RESOURCE_CONFLICT_FAIL;
                break;
            }
            TSIP_Memcpy(&OutData[iLoop * 4U], OutData_Text, 16U);
        }

        if(Ret == RETURN_OK) {
            if(Last_Len != 0U) {
                Ret = TSIPPrc_p3f(OutData_Text);
                if(Ret == RETURN_OK) {
                    TSIP_Memcpy(&OutData[Count * 4U], OutData_Text, Last_Len);
                    RetCode = R_PASS;
                }
                else {
                    RetCode = R_RESOURCE_CONFLICT_FAIL;
                }
            }
            else {
                RetCode = R_PASS;
            }
        }
    }
    else {
        RetCode = R_PARAMETER_FAIL;
    }
    return RetCode;
}

