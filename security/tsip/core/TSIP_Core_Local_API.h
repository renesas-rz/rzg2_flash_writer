/*
 * Copyright (c) 2020, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*! *****************************************************************************
    @file    TSIP_Core_Local_API.h
    @brief   TSIP Core Local API
    
    @par    History
    @subsection sub_history 
      - Initial version\n
 ********************************************************************************/

#ifndef  TSIP_CORE_Local_API_H_
#define  TSIP_CORE_Local_API_H_

/*------------------------------------------------------------------------------*/
/*                        Include Files                                         */
/*------------------------------------------------------------------------------*/

#include <stdint.h>
#include <stddef.h>

#include "common/TSIP_Common_Define.h"

void TSIP_Core_Set_Init(uint8_t SetValue);
void TSIP_Core_Set_State(uint32_t State);
uint32_t TSIP_Core_Init(unsigned char *S_RAMData, unsigned char *S_INSTData);
uint8_t Is_TSIP_Core_Init(void);
uint32_t TSIP_Core_Check_State(uint32_t Tag);
uint64_t TSIP_Core_Bswap_64(uint64_t Data);


#endif  /* TSIP_CORE_Local_API_H_ */

