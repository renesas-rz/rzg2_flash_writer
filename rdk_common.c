/*******************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only
* intended for use with Renesas products. No other uses are authorized. This
* software is owned by Renesas Electronics Corporation and is protected under
* all applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT
* LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
* AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.
* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS
* ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE
* FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR
* ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE
* BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software
* and to discontinue the availability of this software. By using this software,
* you agree to the additional terms and conditions found by accessing the
* following link:
* http://www.renesas.com/disclaimer
* Copyright (C) 2021 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/ 
/*******************************************************************************
 * File Name    : rdk_common.c
 * Description  : operation functions for common
 ******************************************************************************/

#include <stdint.h>
#include "rdk_common.h"

#define SYSCNT_FREQUENCY    (24000000)
#define SYSCNT_BASE_ADDRESS (0xA3F00000)

#define SYSCNT_CNTCR        (0x0000)
#define SYSCNT_CNTCR_EN     (1 << 0)
#define SYSCNT_CNTCV        (0x0008)
#define SYSCNT_CNTFID0      (0x0020)


/*******************************************************************************
 * Function Name: cmn_SYC_ReadReg
 * Description  : read access to System Counter register
 * Arguments    : uint32_t offset		: SYC register offset
 * Return Value : uint32_t				: read data
 ******************************************************************************/
static inline uint32_t cmn_SYC_ReadReg(uint32_t offset)
{
	return (CMN_REG_Read32(SYSCNT_BASE_ADDRESS + offset));
}


/*******************************************************************************
 * Function Name: cmn_SYC_WriteReg
 * Description  : read access to System Counter register
 * Arguments    : uint32_t offset		: SYC register offset
 *              : uint32_t value		: write data
 * Return Value : none
 ******************************************************************************/
static inline void cmn_SYC_WriteReg(uint32_t offset, uint32_t value)
{
	CMN_REG_Write32((SYSCNT_BASE_ADDRESS + offset), value);
}


/*******************************************************************************
 * Function Name: CMN_InitSysCnt
 * Description  : initialize system counter
 * Arguments    : none
 * Return Value : none
 ******************************************************************************/
void CMN_InitSysCnt(void)
{
    cmn_SYC_WriteReg(SYSCNT_CNTFID0, SYSCNT_FREQUENCY);
    cmn_SYC_WriteReg(SYSCNT_CNTCR, SYSCNT_CNTCR_EN);
}


/*******************************************************************************
 * Function Name: CMN_GetSysCnt
 * Description  : get system counter
 * Arguments    : none
 * Return Value : system counter
 ******************************************************************************/
uint64_t CMN_GetSysCnt(void)
{
    return cmn_SYC_ReadReg(SYSCNT_CNTCV);
}


/*******************************************************************************
 * Function Name: CMN_GetFreq4SysCnt
 * Description  : get system counter
 * Arguments    : none
 * Return Value : system counter
 ******************************************************************************/
uint32_t CMN_GetFreq4SysCnt(void)
{
    return cmn_SYC_ReadReg(SYSCNT_CNTFID0);
}


/*******************************************************************************
 * Function Name: CMN_DelayInUSec
 * Description  : delayed time in microsecond
 * Arguments    :
 *                us -
 *                    delay times in microsecond
 * Return Value : none
 ******************************************************************************/
void CMN_DelayInUSec(uint64_t us)
{
    uint64_t start = CMN_GetSysCnt();
    uint64_t cycles = (CMN_GetFreq4SysCnt() / 1000000UL) * us;

    while ((CMN_GetSysCnt() - start) < cycles)
    {
        asm volatile("nop");
    }
}
/*- End of File -*/
