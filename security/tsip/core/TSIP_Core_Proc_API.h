/*
 * Copyright (c) 2020, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*! *****************************************************************************
    @file    TSIP_Core_Proc_API.H
    @brief   TSIP Core Layer Procedure API Headder
    
    @par    History
    @subsection sub_history 
      - Initial version\n
 ********************************************************************************/

/*------------------------------------------------------------------------------*/
/*                        Include Files                                         */
/*------------------------------------------------------------------------------*/

#ifndef TSIP_CORE_PROC_API_H_
#define TSIP_CORE_PROC_API_H_

#include <stdint.h>
#include <stddef.h>

#include "common/TSIP_Common_Define.h"
#include "core/TSIP_Core_API.h"
#include "core/TSIP_Core_Local_API.h"
#include "stub/TSIP_Stub_API.h"

uint32_t TSIP_Core_Prc_softreset(void);
uint32_t TSIP_Core_Prc_p01(void);
uint32_t TSIP_Core_Prc_p02(void);
uint32_t TSIP_Core_Prc_p03(void);
uint32_t TSIP_Core_Prc_p04(const uint32_t *InData_CustomerKeyOperationCode, const uint32_t *InData_KeyRingOperationCode);
uint32_t TSIP_Core_Prc_p10(void);
uint32_t TSIP_Core_Prc_p11(
          uint32_t InData_Operation,
    const uint32_t *InData_IV,
    const uint32_t *InData_KeyOperationCode,
          uint32_t *OutData_KeyIndex);
uint32_t TSIP_Core_Prc_p12(void);

uint32_t TSIP_Core_Prc_p13(void);
uint32_t TSIP_Core_Prc_p14(uint32_t InData_Prog_Num);
uint32_t TSIP_Core_Prc_p15(const uint32_t *InData_Prog, uint32_t InData_Prog_ByteSize, uint32_t *OutData_Prog, uint32_t *OutData_Header);
uint32_t TSIP_Core_Prc_p16(const uint32_t *InData_Header);
uint32_t TSIP_Core_Prc_p17(const uint32_t *InData_Prog, uint32_t InData_Prog_ByteSize, uint32_t *InData_HeapArea, uint32_t *OutData_Prog);

    
uint32_t TSIP_Core_Prc_p40(void);
uint32_t TSIP_Core_Prc_p43(const uint32_t *InData_KeyRingOperationCode);
uint32_t TSIP_Core_Prc_p45(      uint32_t InData_ProgNo,
                            const uint32_t *InData_Prog,
                                 uint32_t InData_Prog_ByteSize,
                                 uint32_t *OutData_Prog);

uint32_t TSIP_Core_Prc_p3f(uint32_t InData_GenByteSize, uint32_t *OutData);

#endif  /* TSIP_CORE_PROC_API_H_ */
