/*
 * Copyright (c) 2020, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*! *****************************************************************************
    @file    TSIP_Stub_API_nonos.c
    @brief   TSIP Stub Layer Local API

    @par    Copyright
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

/*! *****************************************************************************
    @fn       void *TSIP_Malloc(size_t Size)
    @brief    Malloc process(No OS)

    @param     [in]        Size          Size of memory to be allcated
    @retval    NULL

    @par    Details
    dummy function of Malloc

    @par    Processing flow
    -# return NULL
 ********************************************************************************/
void *TSIP_Malloc(size_t Size)
{
    UNUSED(Size);/* PRQA S 3112 */
    return NULL;
}

/*! *****************************************************************************
    @fn       void *TSIP_Calloc(size_t Nmemb, size_t Size)
    @brief    Calloc process(No OS)

    @param     [in]        Nmemb         number of member
    @param     [in]        Size          sizeo of member
    @retval    NULL

    @par    Details
    dummy function of Calloc

    @par    Processing flow
    -# return NULL
 ********************************************************************************/
void *TSIP_Calloc(size_t Nmemb, size_t Size)
{
    UNUSED(Nmemb);/* PRQA S 3112 */
    UNUSED(Size);/* PRQA S 3112 */
    return NULL;
}

/*! *****************************************************************************
    @fn       void TSIP_Free(void *Ptr)
    @brief    Calloc process(No OS)

    @param     [in]        Ptr          Pointer of memory to be free
    @retval    NULL

    @par    Details
    dummy function of free

    @par    Processing flow
    -# nothing
 ********************************************************************************/
void TSIP_Free(void *Ptr)
{
    UNUSED(Ptr);/* PRQA S 3112 */
    return;
}

/*! *****************************************************************************
    @fn       void TSIP_uSleep(uint32_t uTime)
    @brief    usleep process

    @param     [in]        uTime          time of sleep(uSec)
    @retval    0           success
    @retval    -1          failure

    @par    Details
    It free memory allocated by TSIP_Malloc and TSIP_Calloc

    @par    Processing flow
    -# nothing
 ********************************************************************************/
int TSIP_uSleep(uint32_t uTime)
{
    UNUSED(uTime);/* PRQA S 3112 */
    return 0;
}

/*! *****************************************************************************
    @fn       void *TSIP_Memcpy(void *Buf1, const void *Buf2, size_t Num)
    @brief    memcpy process

    @param     [out]       Buf1          Pointer to the destination array
    @param     [in]        Buf2          Pointer to the source of data
    @param     [in]        Num           Number of bytes
    @retval    Buf1 is returned.

    @par    Details
    Copy memory area

    @par    Processing flow
    -# Copy Num bytes from memory area Buf2 to memory area buf1.
 ********************************************************************************/
void *TSIP_Memcpy(void *Buf1, const void *Buf2, size_t Num)
{
    uint8_t *dst = Buf1;
    const uint8_t *src = Buf2;

    for(; Num > 0; Num--) {
        *dst++ = *src++;
    }
    return Buf1;
}

/*! *****************************************************************************
    @fn       void *TSIP_Memset(void *Buf, int Ch, size_t Num)
    @brief    memcpy process

    @param     [out]       Buf           Pointer to the block of memory to fill
    @param     [in]        Ch            Value to be set
    @param     [in]        Num           Number of bytes
    @retval    Buf is returned.

    @par    Details
    fill memory with a constant byte

    @par    Processing flow
    -# Fill the first Num bytes of the memory area pointed to by Buf
       with the constant byte Ch.
 ********************************************************************************/
void *TSIP_Memset(void *Buf, int Ch, size_t Num)
{
    uint8_t *dst = Buf;

    for(; Num > 0; Num--) {
        *dst++ = Ch;
    }
    return Buf;
}
