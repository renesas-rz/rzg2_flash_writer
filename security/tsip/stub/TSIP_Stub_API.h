/*
 * Copyright (c) 2020, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*! *****************************************************************************
    @file    TSIP_Stub_API.h
    @brief   TSIP Stub Layer API
    
    @par    History
    @subsection sub_history 
      - Initial version\n
 ********************************************************************************/

#ifndef  TSIP_STUB_API_H_
#define  TSIP_STUB_API_H_

/*------------------------------------------------------------------------------*/
/*                        Include Files                                         */
/*------------------------------------------------------------------------------*/

#include <stdint.h>
#include <stddef.h>

#include "common/TSIP_Common_Define.h"

void *TSIP_Malloc(size_t Size);
void *TSIP_Calloc(size_t Nmemb, size_t Size);
void TSIP_Free(void *Ptr);
int  TSIP_uSleep(uint32_t uTime);
void *TSIP_Memcpy(void *Buf1, const void *Buf2, size_t Num);
void *TSIP_Memset(void *Buf, int Ch, size_t Num);

#endif  /* TSIP_STUB_API_H_ */

