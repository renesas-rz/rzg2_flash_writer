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
 * File Name    : rdk_cmn_pmc.h
 * Description  : register and API information for PMC
 ******************************************************************************/

#ifndef RDK_CMN_PMC_H
#define RDK_CMN_PMC_H

#define PMC_BASE_ADDRESS        (0x0A3600000ULL)

/** Register offset defines */
#define PMC_SPLY_ENA            (0x0000)
#define PMC_MAIN_STS            (0x0004)
#define PMC_PD_RFX_ISOEN        (0x0028)

#define PMC_IDLE_REQ            (0x0038)
#define PMC_IDLE_STS            (0x003C)
#define PMC_INT_STS             (0x0044)
#define PMC_INT_CLR             (0x0048)
#define PMC_INT_MSK             (0x004C)

#define PMC_PD_MEM_TIM          (0x0014)
#define PMC_PD_ON_TIM           (0x0018)
#define PMC_PD_MEM_ISOEN        (0x001C)

/** Bit assign */
#define PMC_SPLY_ENA_PD_ON                  (0x00000010)
#define PMC_SPLY_ENA_PD_MEM                 (0x00000001)
#define PMC_MAIN_STS_PD_MEM                 (0x00000002)
#define PMC_SPLY_ENA_PD_RFX                 (0x00000004)

#define PMC_PD_MEM_TIM_OFF_MASK             (0x0000FFFF)
#define PMC_PD_MEM_TIM_ON_MASK              (0x0000FFFF)
#define PMC_PD_MEM_TIM_OFF_SHIFT            (16)
#define PMC_PD_MEM_TIM_ON_SHIFT             (0)

#define PMC_PD_ON_TIM_MASK                  (0x0000FFFF)
#define PMC_PD_ON_TIM_SHIFT                 (0)

#define PMC_PD_ISO_EN                       (0x00000001)
#define PMC_PD_ISO_DONE                     (0x00008000)

#define PMC_IDLE_REQ_PD_MEM                 (0x00000002)
#define PMC_IDLE_REQ_WEN_PD_MEM             (0x00020000)
#define PMC_IDLE_STS_PD_MEM_IDLE_DONE       (0x00000002)
#define PMC_IDLE_STS_PD_MEM_ACT_DONE        (0x00000200)

#define PMC_IDLE_STS_PD_RFX_ACT_DONE        (0x00001000)

#define PMC_INT_STS_PD_DONE                 (0x00000001)
#define PMC_INT_STS_IDLE_DONE               (0x00000002)
#define PMC_INT_STS_L2_STBY                 (0x00000020)

#define PMC_INT_CLR_PD_DONE                 (0x00000001)
#define PMC_INT_CLR_IDLE_DONE               (0x00000002)

#define PMC_IDLE_RAW_IDLE_PD_RFX            (0x00100000)

/** typdef defined */
typedef enum
{
    PMC_ERROR_POWER_ON_TIMEOUT      = -311,
    PMC_ERROR_ISOLATION_OFF_TIMEOUT = -312,
    PMC_ERROR_CONNECTED_BUS_TIMEOUT = -313,
    PMC_ERROR_SEPARATED_BUS_TIMEOUT = -314,
    PMC_ERROR_ISOLATION_ON_TIMEOUT  = -315,
    PMC_ERROR_POWER_OFF_TIMEOUT     = -316,
    PMC_ERROR_PLL_LOCK_TIMEOUT      = -317,
    PMC_ERROR_RELEASE_RESET_TIMEOUT = -318,
    PMC_ERROR_PLL_STANDBY_TIMEOUT   = -319,
    PMC_ERROR_HWFFC_CHG_STS_TIMEOUT = -320,
    PMC_ERROR_NOT_IDLE_BUS          = -321,
    PMC_ERROR_PLL3_NOT_STANDBY_MODE = -333,
    PMC_ERROR_PLL4_NOT_STANDBY_MODE = -334,
    PMC_ERROR_PLL6_NOT_STANDBY_MODE = -336,
    PMC_ERROR_NO_EXIST_SPLY         = -341,
    PMC_ERROR_NO_EXIST_ISOLATION    = -342,
    PMC_ERROR_NO_EXIST_BUS_CONNECT  = -343
} e_pmc_error_code_t;

/** prototype defined **/
uint32_t PMC_ReadReg(uint32_t offset);
void PMC_WriteReg(uint32_t offset, uint32_t value);


int32_t PMC_PowerOn_PD_MEM_FromPOR(void);


#endif /* RDK_CMN_PMC_H */
