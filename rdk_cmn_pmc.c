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
 * File Name    : rdk_cmn_pmc.c
 * Version      : 0.9
 * Description  : operation functions for PMC
 ******************************************************************************/

#include <stdint.h>
#include <stdbool.h>

#include "rdk_common.h"
#include "rdk_cmn_pmc.h"
#include "rdk_cmn_cpg.h"

#define PMC_TIMEOUT_UNIT_IN_US          (10)
#define PMC_SEQUENCE_POWER_ON_TIMEOUT   (500000)
#define PMC_ISOLATION_OFF_TIMEOUT       (500000)
#define PMC_CONNECTED_BUS_TIMEOUT       (500000)
#define PMC_SEPARATED_BUS_TIMEOUT       (500000)
#define PMC_ISOLATION_ON_TIMEOUT        (500000)
#define PMC_SEQUENCE_POWER_OFF_TIMEOUT  (500000)
#define PMC_RELEASE_RESET_TIMEOUT       (500000)
#define PMC_HWFFC_CHG_STS_TIMEOUT       (500000)

#define PMC_WAIT_EVENT(m_toc, m_err_code, m_condition, m_rslt)  \
    {   \
        uint32_t count = (m_toc)/PMC_TIMEOUT_UNIT_IN_US;   \
        while (true) {  \
            if ((m_condition)) {    \
                (m_rslt) = CMN_SUCCESS; \
                break;  \
            }   \
            if ((0 == (m_toc)) || (0 < count)) {    \
                CMN_DelayInUS(PMC_TIMEOUT_UNIT_IN_US);  \
                count--;    \
            } else {    \
                (m_rslt) = (m_err_code);    \
                break;  \
            }   \
        }   \
    }

typedef struct
{
    uint8_t  reg_num;
    uint16_t target;
    uint16_t value;
} st_cpg_setting_data_t;

typedef struct
{
    uint32_t mem_time;
    uint16_t pwron_time;
} st_pmc_local_t;

static st_pmc_local_t gs_pmc_priv =
{
    .mem_time = 0x176F176F,     /* PD_MEM External Power On/Off Wait: 2ms/2ms */
    .pwron_time = 0x176F,       /* PD External Power On Wait: 2ms */
};

uint32_t PMC_ReadReg(uint32_t offset)
{
    return (CMN_REG_Read32(PMC_BASE_ADDRESS + offset));
}

void PMC_WriteReg(uint32_t offset, uint32_t value)
{
    CMN_REG_Write32((PMC_BASE_ADDRESS + offset), value);
}

