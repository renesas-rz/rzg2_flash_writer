/*
 * Copyright (c) 2020, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*! *****************************************************************************
    @file    TSIP_Wrapper_Boot_API.h
    @brief   Boot API
    
    @par    History
    @subsection sub_history 
      - Initial version\n
 ********************************************************************************/

#ifndef  TSIP_WRAPPER_BOOT_API_H
#define  TSIP_WRAPPER_BOOT_API_H

/*------------------------------------------------------------------------------*/
/*                        Include Files                                         */
/*------------------------------------------------------------------------------*/

#include "common/TSIP_Common_Define.h"

unsigned long R_TSIP_VerifyBootData(unsigned char *InData_BootData,
                                    unsigned long InData_BootData_ByteSize,
                                    unsigned char *InData_BootData_HeapArea,
                                    unsigned char *OutData_BootData);

#endif  /* TSIP_WRAPPER_BOOT_API_H */

