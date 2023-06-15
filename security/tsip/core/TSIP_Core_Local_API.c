/*
 * Copyright (c) 2020, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*! *****************************************************************************
    @file    TSIP_Core_Local_API.c
    @brief   TSIP Core Layer Local API
    
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
#include "core/TSIP_Core_Local_API.h"
#include "stub/TSIP_Stub_API.h"

/*!
    @name  TSIP use RAM Area in Systm Memory
 */
/* @{ */
uint32_t *S_RAM  = NULL;
uint32_t *S_INST = NULL;
uint32_t *S_HEAP = NULL;
/* @} */


/*!
    @name State Flag in TSIP Core Layer
 */

static uint8_t TSIP_Core_Init_Flag  = TSIP_FALSE;

#ifdef _TSIP_SB
static uint32_t TSIP_Core_State_Flag = TSIP_CORE_STATE00;
#else
static uint32_t TSIP_Core_State_Flag = TSIP_CORE_STATE09;
#endif
/* @} */



/*!
    @name State Require Table for API in TSIP Core Layer
 */
/* @{ */
const uint16_t RequireState_TBL[TSIP_STATE_TAG_MAX][TSIP_CORE_STATE_MAX_ARRAY] = {
    {TSIP_CORE_STATE00, TSIP_CORE_STATE_NONE, TSIP_CORE_STATE_NONE, TSIP_CORE_STATE_NONE}, /* TSIP_INIT                            */
    {TSIP_CORE_STATE01, TSIP_CORE_STATE03,    TSIP_CORE_STATE_NONE, TSIP_CORE_STATE_NONE}, /* TSIP_INJECT_KEY                      */
    {TSIP_CORE_STATE01, TSIP_CORE_STATE05,    TSIP_CORE_STATE_NONE, TSIP_CORE_STATE_NONE}, /* TSIP_INSTALL_UIMAGE(No Used)         */
    {TSIP_CORE_STATE04, TSIP_CORE_STATE_NONE, TSIP_CORE_STATE_NONE, TSIP_CORE_STATE_NONE}, /* TSIP_INSTALL_DEVICETREE(No Used)     */
    {TSIP_CORE_STATE01, TSIP_CORE_STATE07,    TSIP_CORE_STATE_NONE, TSIP_CORE_STATE_NONE}, /* TSIP_SB_UIMAGE(No Used)              */
    {TSIP_CORE_STATE08, TSIP_CORE_STATE_NONE, TSIP_CORE_STATE_NONE, TSIP_CORE_STATE_NONE}, /* TSIP_SB_DEVICETREE(No Used)          */
    {TSIP_CORE_STATE09, TSIP_CORE_STATE_NONE, TSIP_CORE_STATE_NONE, TSIP_CORE_STATE_NONE}, /* TSIP_LIB_INIT                        */
    {TSIP_CORE_STATE10, TSIP_CORE_STATE_NONE, TSIP_CORE_STATE_NONE, TSIP_CORE_STATE_NONE}, /* TSIP_LIB_APIS                        */
    {TSIP_CORE_STATE11, TSIP_CORE_STATE_NONE, TSIP_CORE_STATE_NONE, TSIP_CORE_STATE_NONE}, /* TSIP_LIB_SECURE_UPDATE_APIS          */
    {TSIP_CORE_STATE03, TSIP_CORE_STATE_NONE, TSIP_CORE_STATE_NONE, TSIP_CORE_STATE_NONE}, /* TSIP_INJECT_KEY_SKIP                 */
    {TSIP_CORE_STATE05, TSIP_CORE_STATE_NONE, TSIP_CORE_STATE_NONE, TSIP_CORE_STATE_NONE}, /* TSIP_INSTALL_UIMAGE_SKIP(No Used)    */
    {TSIP_CORE_STATE01, TSIP_CORE_STATE20,    TSIP_CORE_STATE21,    TSIP_CORE_STATE_NONE}, /* TSIP_INJECT_ADDON_KEY                */
    {TSIP_CORE_STATE20, TSIP_CORE_STATE21,    TSIP_CORE_STATE_NONE, TSIP_CORE_STATE_NONE}, /* TSIP_INJECT_ADDON_KEY_SKIP           */
    {TSIP_CORE_STATE20, TSIP_CORE_STATE_NONE, TSIP_CORE_STATE_NONE, TSIP_CORE_STATE_NONE}, /* TSIP_INJECT_ADDON_KEY_FIXEDSTATE     */
    {TSIP_CORE_STATE01, TSIP_CORE_STATE_NONE, TSIP_CORE_STATE_NONE, TSIP_CORE_STATE_NONE}, /* TSIP_KEYRING_VERIFY(RZG2 New)        */
    {TSIP_CORE_STATE01, TSIP_CORE_STATE24,    TSIP_CORE_STATE_NONE, TSIP_CORE_STATE_NONE}, /* TSIP_REENC_BOOTDATA(RZG2 New)        */
    {TSIP_CORE_STATE24, TSIP_CORE_STATE_NONE, TSIP_CORE_STATE_NONE, TSIP_CORE_STATE_NONE}, /* TSIP_REENC_BOOTDATA_SKIP(RZG2 New)   */
    {TSIP_CORE_STATE01, TSIP_CORE_STATE25,    TSIP_CORE_STATE26,    TSIP_CORE_STATE_NONE}, /* TSIP_VERIFY_BOOTDATA(RZG2 New)       */
    {TSIP_CORE_STATE25, TSIP_CORE_STATE_NONE, TSIP_CORE_STATE_NONE, TSIP_CORE_STATE_NONE}, /* TSIP_VERIFY_BOOTDATA_SKIP(RZG2 New)  */
};
/* @} */

/*------------------------------------------------------------------------------*/
/*                        Function Definition                                   */
/*------------------------------------------------------------------------------*/

/*! *****************************************************************************
    @fn       void TSIP_Core_Set_Init(uint8_t SetValue)
    @brief    set initialization flag of TSIP Core layer

    @param     [in]        SetValue         transition state

    @retval    void
    
    @par    Details
    It set initialization flag of TSIP Core layer

    @par    Processing flow
    -# set initialization flag
 ********************************************************************************/
void TSIP_Core_Set_Init(uint8_t SetValue)
{
    if(SetValue==TSIP_FALSE) {
        TSIP_Core_Init_Flag = TSIP_FALSE;
    }
    else {
        TSIP_Core_Init_Flag = TSIP_TRUE;
    }
}

/*! *****************************************************************************
    @fn       void TSIP_Core_Set_State(uint32_t State)
    @brief    set transition state of TSIP Core layer

    @param     [in]        State         transition state

    @retval    R_PASS                  success
    @retval    R_PARAMETER_FAIL        invalid input parameter
    
    @par    Details
    It set transition state of TSIP Core layer

    @par    Processing flow
    -# set transition state
 ********************************************************************************/
void TSIP_Core_Set_State(uint32_t State)
{
   TSIP_Core_State_Flag = State;
}

/*! *****************************************************************************
    @fn       uint32_t TSIP_Core_Init(unsigned char *S_RAMData, unsigned char *S_INSTData)
    @brief    Initialize in TSIP Core

    @param     [in]        S_RAMData         Pointer of S_RAM area
    @param     [in]        S_INSTData        Pointer of S_INST area

    @retval    R_PASS                  success
    
    @par    Details
    It perform initialization of TSIP Core

    @par    Processing flow
    -# save pointer of S_RAM and S_INT
 ********************************************************************************/
uint32_t TSIP_Core_Init(unsigned char *S_RAMData, unsigned char *S_INSTData)
{
    uint32_t RetCode = R_PASS;

    S_RAM  = (uint32_t *)S_RAMData;  /* PRQA S 3305 */
    S_INST = (uint32_t *)S_INSTData; /* PRQA S 3305 */

    return RetCode;
}

/*! *****************************************************************************
    @fn       uint8_t Is_TSIP_Core_Init(void)
    @brief    check initialization of TSIP Core Layer
    
    @retval    TSIP_TRUE                  initialized
    @retval    TSIP_FALSE                 not initialized
    
    @par    Details
    It check if it has been initialized in TSIP Core Layer

    @par    Processing flow
    -# return the TSIP_Core_Init_Flag
 ********************************************************************************/
uint8_t Is_TSIP_Core_Init(void)
{
    return(TSIP_Core_Init_Flag);
}


/*! *****************************************************************************
    @fn       uint32_t TSIP_Core_Check_State(uint32_t Tag)
    @brief    check transition state of TSIP Core layer

    @param     [in]        Tag         TAG of checked API

    @retval    R_PASS                  success
    @retval    R_SEQUENCE_FAIL         cannot perform in current state
    
    @par    Details
    It checks transition state that API can perform

    @par    Processing flow
    -# check if TAG API can perform in current state by using RequireState_TBL
 ********************************************************************************/
uint32_t TSIP_Core_Check_State(uint32_t Tag)
{
   uint32_t RetCode = R_SEQUENCE_FAIL;
   uint32_t Cnt;

   for(Cnt = 0;Cnt < TSIP_CORE_STATE_MAX_ARRAY;Cnt++) {
       if(TSIP_Core_State_Flag == RequireState_TBL[Tag][Cnt]) {
           RetCode = R_PASS;
           break;
       }
       if(RequireState_TBL[Tag][Cnt] == TSIP_CORE_STATE_NONE) {
           break;
       }
   }

   return RetCode;
}

/*! *****************************************************************************
    @fn       uint64_t TSIP_Core_Bswap_64(uint64_t Data)
    @brief    Endian swap for 64bit data

    @param     [in]        Data         64bit data

    @retval    endian swapped data
    
    @par    Details
    It perform endian swapping for 64bit data.

    @par    Processing flow
    -# endian swapping for 64bit data
 ********************************************************************************/
#ifndef _SB_COVERAGE
uint64_t TSIP_Core_Bswap_64(uint64_t Data)
{
    uint64_t Ret;
    Ret = ((((Data) & 0xff00000000000000ULL) >> 56) |  /* PRQA S 2984, 0580, 2984, 2790 */
           (((Data) & 0x00ff000000000000ULL) >> 40) |  /* PRQA S 2984, 0580, 2984, 2790 */
           (((Data) & 0x0000ff0000000000ULL) >> 24) |  /* PRQA S 2984, 0580, 2984 */
           (((Data) & 0x000000ff00000000ULL) >> 8)  |  /* PRQA S 2984, 0580, 2984 */
           (((Data) & 0x00000000ff000000ULL) << 8)  |  /* PRQA S 2984, 0580, 2984 */
           (((Data) & 0x0000000000ff0000ULL) << 24) |  /* PRQA S 2984, 0580, 2984 */
           (((Data) & 0x000000000000ff00ULL) << 40) |  /* PRQA S 2984, 0580, 2984, 2790 */
           (((Data) & 0x00000000000000ffULL) << 56) ); /* PRQA S 2984, 0580, 2984, 2790 */

    return Ret;
}
#endif
