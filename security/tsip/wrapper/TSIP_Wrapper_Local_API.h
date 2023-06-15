/*
 * Copyright (c) 2020, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*! *****************************************************************************
    @file    TSIP_Wrapper_Local_API.h
    @brief   Wrapper Layer Local API
    
    @par    History
    @subsection sub_history 
      - Initial version\n
 ********************************************************************************/

#ifndef  TSIP_WRAPPER_LOCAL_API_H
#define  TSIP_WRAPPER_LOCAL_API_H

/*------------------------------------------------------------------------------*/
/*                        Include Files                                         */
/*------------------------------------------------------------------------------*/
#include "common/TSIP_Common_Define.h"

void     TSIP_Wrapper_Set_Init(uint8_t BoolFg);
uint32_t TSIP_Wrapper_Init(void);
uint8_t  Is_TSIP_Wrapper_Init(void);

#endif  /* TSIP_WRAPPER_LOCAL_API_H */

