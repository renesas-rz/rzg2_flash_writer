/*
 * Copyright (c) 2020, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*! *****************************************************************************
    @file    TSIP_Wrapper_Init_API.h
    @brief   NoOS Initialization API
    
    @par    History
    @subsection sub_history 
      - Initial version\n
 ********************************************************************************/

#ifndef  TSIP_WRAPPER_INIT_API_H
#define  TSIP_WRAPPER_INIT_API_H

/*------------------------------------------------------------------------------*/
/*                        Include Files                                         */
/*------------------------------------------------------------------------------*/
#include "common/TSIP_Common_Define.h"

unsigned long R_TSIP_Init(unsigned char *S_RAMData, unsigned char *S_INSTData);

#endif  /* TSIP_WRAPPER_INIT_API_H */

