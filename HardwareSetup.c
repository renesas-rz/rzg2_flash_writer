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
 * File Name    : HardwareSetup.c
 * Description  : 
 ******************************************************************************/


#include <stdint.h>
#include <stdbool.h>

#include "rdk_common.h"
#include "rdk_cmn_cpg.h"
#include "rdk_cmn_pmc.h"
#include "rdk_pfc.h"

#include "HardwareSetup.h"

#define UART_BAUDRATE	115200


void InitPFC(void){
	/* set select function for URT */
	PFC_SetPinFunc(PFC_SELECT_URT0);
	/* set select function for EMM */
	PFC_SetPinFunc(PFC_SELECT_EMM);
}

void InitCPG(void)
{
	int t;
	uint32_t clock_sts = (CPG_CLKSTATUS_DIVA | CPG_CLKSTATUS_DIVD | CPG_CLKSTATUS_DIVE);
	t = 0;
	
	//Check complete status
	while (1000000 > t++)	//1sec time out
	{
		if (0 == (CPG_ReadReg(CPG_CLKSTATUS) & clock_sts)) {
			break;
		}
		CMN_DelayInUS(10);
	}

	//Set dividor and selector for CA53 clock and UART0, eMM
	CPG_WriteReg(CPG_CA53_DDIV, (CPG_CA53_DDIV_WEN_DIVA | CPG_CA53_DDIV_DIVA_SET_MIN));			//CA53 clock:996MHz

	CPG_WriteReg(CPG_SYS_DDIV, 
		(CPG_SYS_DDIV_WEN_DIVD) | (CPG_SYS_DDIV_DIVD_SET_MIN << CPG_SYS_DDIV_DIVD_SET_SHIFT) |	//CPG_MPCLK:200MHz
		(CPG_SYS_DDIV_WEN_DIVE) | (CPG_SYS_DDIV_DIVE_SET_MIN << CPG_SYS_DDIV_DIVE_SET_SHIFT)	//CPG_SPCLK:100MHz
	);
	
	CPG_WriteReg(CPG_CLK48_DSEL, 
		(CPG_CLK48_DSEL_WEN_SELD | CPG_CLK48_DSEL_SELD) |										//Set Selector D clcok
		(CPG_CLK48_DSEL_WEN_SELE | CPG_CLK48_DSEL_SELE)											//Set Selector E clcok
	);

	CPG_WriteReg(CPG_SDIEMM_SSEL, (CPG_SDIEMM_SSEL_WEN_SELSDI | CPG_SDIEMM_SSEL_SELSDI));		//SDI/EMM core clock: 200MHz
	CPG_WriteReg(CPG_URT_RCLK_SSEL, (CPG_URT_RCLK_SSEL_WEN_SELW0 | 0x0));						//Set UART0 clock : 48MHz

	//Check complete status
	t = 0;
	while (1000000 > t++)	//1sec time out
	{
		if (0 == (CPG_ReadReg(CPG_CLKSTATUS) & clock_sts)) {
			break;
		}
		CMN_DelayInUS(10);
	}

	/* enable supply clock to URT(URT_CLK[0]/URT_PCLK) and eMMC(EMM_CLK_HS/EMM_IMCLK2/EMM_IMCLK/EMM_ACLK) */
	CPG_SetClockCtrl(15, 0x0030, 0x0030);	//UART
	CPG_SetClockCtrl(3, 0x0f00, 0x0f00);	//eMMC

	/* asset RESET to URT0 and eMMC */
	CPG_SetResetCtrl(6, 0x0400, 0x0000);	//UART
	CPG_SetResetCtrl(3, 0x0004, 0x0000);	//eMMC

	CPG_WaitResetMon(0, (CPG_RST_MON_URT|CPG_RST_MON_EMM), (CPG_RST_MON_URT|CPG_RST_MON_EMM));

	/* deasset RESET to URT0 and eMMC */
	CPG_SetResetCtrl(6, 0x0400, 0x0400);	//UART
	CPG_SetResetCtrl(3, 0x0004, 0x0004);	//eMMC

	CPG_WaitResetMon(0, (CPG_RST_MON_URT|CPG_RST_MON_EMM), 0);

	return;
}

void SYC_enable(void)
{
    uint32_t tmp;

    /* enable SYC_CNT_CLK */
    CPG_SetClockCtrl(8, (1<<12), (1<<12));

    /* enable ICB_SYC_CNT_CLK */
    CPG_SetClockCtrl(16, (1<<15), (1<<15));

    do{
        CPG_GetClockCtrl(8, &tmp);
    } while((tmp & (1<<12)) == 0);

    /* SYC_RST_N(TYPE-B) OFF */
    CPG_SetResetCtrl(5, (1<<9), (1<<9));
}

void PowerOnRAMB(void)
{
	
	uint32_t value;
	
	/** A */
	/** A-1) Set power on time to register */
	/**		 Use the initial value		   */

	/** A-2) Start sequence to power on PD_RFX */
	PMC_WriteReg(PMC_SPLY_ENA,(PMC_SPLY_ENA_PD_ON|PMC_SPLY_ENA_PD_RFX));
	/** A-3) Wait for Power on process */
	while (0 == (PMC_INT_STS_PD_DONE & PMC_ReadReg(PMC_INT_STS)))
		;

	/** A-4) Clear to interrupt status */
	PMC_WriteReg(PMC_INT_CLR, PMC_INT_CLR_PD_DONE);
	/** B) release RESET and start supplies clock */
	/** B-1) release RESET for PD_RFX */
	CPG_WriteReg(CPG_PD_RST,
			 (CPG_PD_RST_WEN_RFX_RSTB | CPG_PD_RST_RFX_RSTB));

	/** B-2) start supplies clock for target IPs */


	/** CPG_CLK_ON23 : CLK0, 1 */
	CPG_SetClockCtrl(23, 0x0003, 0x0003);

	/** B-3) turn off isolation for PD_RFX */
	/** B-4) check turned off isolation */
	PMC_WriteReg(PMC_PD_RFX_ISOEN, PMC_PD_ISO_DONE);
	while (0 == (PMC_PD_ISO_DONE & PMC_ReadReg(PMC_PD_RFX_ISOEN)))
		;

	/** B-5) release RESET for target IPs */
	/** CPG_RST12 : UNIT0, 1, 2, 3 */
		CPG_SetResetCtrl(12,0x0001, 0x0001);
	/** B-6) 50nsec wait */
		CMN_DelayInUS(1);

	/** B-9) release RESET for PD_RFX ICB */
	/** CPG_RST7  : UNIT4 */
	CPG_SetResetCtrl(7, 0x0010, 0x0010);

	/** B-10) 50nsec wait */
	CMN_DelayInUS(1);
	
	/** B-11) start supply clock for PD_RFX ICB */
	/** CPG_CLK_ON16 : CLK9 */
	CPG_SetClockCtrl(16, 0x0200, 0x0200);

	/** CPG_CLK_ON17 : CLK4 */
	CPG_SetClockCtrl(17, 0x0010, 0x0010);

	/** C) connecting bus */
	/** C-1) setting connecting bus */
	PMC_WriteReg(PMC_IDLE_REQ, (PMC_IDLE_RAW_IDLE_PD_RFX));

	/** C-2) check connecting bus */
	while (0 == (PMC_IDLE_STS_PD_RFX_ACT_DONE & PMC_ReadReg(PMC_IDLE_STS)))
		;
}
