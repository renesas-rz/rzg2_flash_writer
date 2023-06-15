/*
 * Copyright (c) 2020, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*! *****************************************************************************
    @file    TSIP_Core_API.h
    @brief   TSIP Core API
    
    @par    History
    @subsection sub_history 
      - Initial version\n
 ********************************************************************************/

#ifndef  TSIP_CORE_API_H_
#define  TSIP_CORE_API_H_

/*------------------------------------------------------------------------------*/
/*                        Include Files                                         */
/*------------------------------------------------------------------------------*/
#include <stdint.h>
#include <stddef.h>

#include "common/TSIP_Common_Define.h"

typedef struct str_bootdata {
    uint8_t *InData_BootData;
    uint64_t InData_BootData_ByteSize;
    uint8_t *OutData_BootData;
} BOOTDATA;

typedef struct str_updatebootdata {
    uint32_t InData_BootData_UpdateFlag;
    uint8_t *InData_BootData;
    uint64_t InData_BootData_ByteSize;
    uint8_t *OutData_BootData;
} UPDATEBOOTDATA;

#ifdef _TSIP_SB

typedef struct str_reenc_boot_data {
    uint8_t *InData_BootData;
    uint64_t InData_BootData_ByteSize;
    uint8_t *OutData_BootData;
} REENC_BOOT_DATA[REENC_BOOT_DATA_MAX];


/*                             */
/*  TSIP Core Initial API      */
/*                             */
uint32_t TSIP_Init_Core(uint8_t *S_RAMData, uint8_t *S_INSTData);


/*                            */
/*  TSIP Advance preparation  */
/*                            */
uint32_t TSIP_Inject_Key(uint8_t *InData_ProvisioningKeyOperationCode, uint8_t *InData_KeyRingOperationCode);
uint32_t TSIP_ReEncBootData(REENC_BOOT_DATA *reenc_bootdata);
uint32_t TSIP_Inject_Addon_Key(uint32_t InData_Operation, uint8_t *InData_IV, uint8_t *InData_KeyOperationCode, uint8_t *OutData_KeyOperationCode);


/*                  */
/* TSIP Verfy Boot Data */
/*                  */
uint32_t TSIP_VerifyBootData(uint8_t *InData_BootData, uint32_t InData_BootData_ByteSize, uint8_t *InData_BootData_HeapArea, uint8_t *OutData_BootData);

/*                     */
/* TSIP KeyRing Verify */
/*                     */
uint32_t TSIP_KeyRing_Verify(void);

#else  /* _TSIP_SB */

typedef struct str_update_boot_data {
    uint32_t InData_BootData_UpdateFlag;
    uint8_t *InData_BootData;
    uint64_t InData_BootData_ByteSize;
    uint8_t *OutData_BootData;
} UPDATE_BOOT_DATA[UPDATE_BOOT_DATA_MAX];

/*                             */
/*  TSIP Library Initial API   */
/*                             */
uint32_t TSIP_Lib_Init(uint8_t *S_RAMData, uint8_t *S_INSTData, uint8_t *TSIP_BaseAddress, uint8_t *MSTP_BaseAddress);


/*                           */
/*  TSIP Secure Update API   */
/*                           */
uint32_t TSIP_SU_Activate(void);
uint32_t TSIP_UpdateBootData(UPDATE_BOOT_DATA *update_bootdata);
uint32_t TSIP_SU_Key(uint8_t *InData_KeyRingOperationCode);


/*                                         */
/*  TSIP Basic Cryptographic Function API  */
/*                                         */

uint32_t TSIP_BCF_GenerateRandom(
    uint32_t InData_GenByteSize,
    uint32_t InData_UseCASE,
    uint8_t *OutData);


#endif /* _TSIP_SB */

#endif  /* TSIP_CORE_API_H_ */

