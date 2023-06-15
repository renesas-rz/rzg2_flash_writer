/*
 * Copyright (c) 2020, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*! *****************************************************************************
    @file    TSIP_Wrapper_Prepare_API.h
    @brief   BootUp API
    
    @par    History
    @subsection sub_history 
      - Initial version\n
 ********************************************************************************/

#ifndef  TSIP_WRAPPER_PREPARE_API_H
#define  TSIP_WRAPPER_PREPARE_API_H

/*------------------------------------------------------------------------------*/
/*                        Include Files                                         */
/*------------------------------------------------------------------------------*/

#include "common/TSIP_Common_Define.h"

typedef struct str_tsip_reenc_boot_data {
    unsigned char *InData_BootData;
    unsigned long  InData_BootData_ByteSize;
    unsigned char *OutData_BootData;
} TSIP_REENC_BOOT_DATA[REENC_BOOT_DATA_MAX];

typedef struct wrap_reenc_boot_data {
    unsigned char *InData_BootData;
    unsigned long  InData_BootData_ByteSize;
    unsigned char *OutData_BootData;
} WRAP_RE_BOOT_DATA;


unsigned long R_TSIP_Inject_Key(unsigned char *InData_ProvisioningKeyOperationCode, unsigned char *InData_KeyRingOperationCode);
unsigned long R_TSIP_ReEncBootData(TSIP_REENC_BOOT_DATA  *tsip_reenc_bootdata);
unsigned long R_TSIP_Inject_Addon_Key(unsigned long InData_Operation, unsigned char *InData_IV, unsigned char *InData_KeyOperationCode, unsigned char *OutData_KeyOperationCode);

#endif  /* TSIP_WRAPPER_PREPARE_API_H */
