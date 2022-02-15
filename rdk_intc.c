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
 * File Name    : rdk_intc.c
 * Version      : 0.9
 * Description  : operation functions for interrputs
 ******************************************************************************/

#include <stdint.h>

#include "rdk_cmn_gic.h"
#include "rdk_common.h"

/*******************************************************************************
 * Function Name: CPG_ReadReg
 * Description  : Read the given GIC register
 *
 * Arguments    : Offset is the register offset to be read
 * Return Value : error code -
 *
 ******************************************************************************/
uint32_t GIC_ReadReg(uint32_t offset)
{
    return (CMN_REG_Read32(GIC_BASE_ADDRESS + offset));
}
/*******************************************************************************
 End of function CPG_ReadReg
 ******************************************************************************/
/*******************************************************************************
 * Function Name: GIC_WriteReg
 * Description  : Write the given GIC register
 *                Data is the 32-bit value to write to the register
 * 
 * Arguments    : Offset is the register offset to be written
 * Return Value : error code -
 *
 ******************************************************************************/
void GIC_WriteReg(uint32_t offset, uint32_t value)
{
    CMN_REG_Write32((GIC_BASE_ADDRESS + offset), value);
}
/*******************************************************************************
 End of function GIC_WriteReg
 ******************************************************************************/
/*****************************************************************************/
/**
*
* CfgInitialize a specific interrupt controller instance/driver. The
* initialization entails:
*
* - Initialize fields of the XScuGic structure
* - Initial vector table with stub function calls
* - All interrupt sources are disabled*
* @return
*		 None
* @note		None.
*
******************************************************************************/
static void GIC_CfgInitialize(uint32_t cpu_id)
{

	uint32_t int_id;
    uint32_t RegValue;
    uint32_t local_cpu_id = ((uint32_t)0x1 << cpu_id);
    local_cpu_id |= local_cpu_id << 8U;
	local_cpu_id |= local_cpu_id << 16U;

    GIC_DistWriteReg(GIC_DIST_EN_OFFSET, 0U);
	
	for (int_id = 32U; int_id < GIC_MAX_NUM_INTR_INPUTS;
	     int_id = int_id + 4U) {
		/* Remove interrupt target register */
		GIC_DistWriteReg(GIC_SPI_TARGET_OFFSET_CALC(int_id), 0);
       
        /* Disable all the interrupts */
		GIC_DistWriteReg(
		    GIC_EN_DIS_OFFSET_CALC(GIC_DISABLE_OFFSET, int_id),
                0xFFFFFFFFU);
    }

    /*
	 * The trigger mode in the int_config register
	 * Only write to the SPI interrupts, so start at 32
	 */
	for (int_id = 32U; int_id < GIC_MAX_NUM_INTR_INPUTS;
	     int_id = int_id + 16U) {
		/*
		 * Each INT_ID uses two bits, or 16 INT_ID per register
		 * Set them all to be level sensitive, active HIGH.
		 */
		GIC_DistWriteReg(GIC_INT_CFG_OFFSET_CALC(int_id), 0U);
	}

#define DEFAULT_PRIORITY    0xa0a0a0a0U
	for (int_id = 0U; int_id < GIC_MAX_NUM_INTR_INPUTS;
	     int_id = int_id + 4U) {
		/*
		 * The priority using int the priority_level register
		 * The priority_level and spi_target registers use one byte per
		 * INT_ID.
		 * Write a default value that can be changed elsewhere.
		 */
		GIC_DistWriteReg(GIC_PRIORITY_OFFSET_CALC(int_id),
				 DEFAULT_PRIORITY);
	}

	for (int_id = 32U; int_id < GIC_MAX_NUM_INTR_INPUTS;
	     int_id = int_id + 4U) {
		/*
		 * The CPU interface in the spi_target register
		 * Only write to the SPI interrupts, so start at 32
		 */
		GIC_DistWriteReg(GIC_SPI_TARGET_OFFSET_CALC(int_id),
				 local_cpu_id);
	}

	for (int_id = 0U; int_id < GIC_MAX_NUM_INTR_INPUTS;
	     int_id = int_id + 32U) {
		/*
		 * Enable the SPI using the enable_set register. Leave all
		 * disabled for now.
		 */
		/* clear pending status */
		GIC_DistWriteReg(
		    GIC_EN_DIS_OFFSET_CALC(GIC_PENDING_CLR_OFFSET, int_id),
		        0xFFFFFFFFU);

		/* clear active status */
		GIC_DistWriteReg(
		    GIC_EN_DIS_OFFSET_CALC(GIC_ACTIVE_CLR_OFFSET, int_id),
		        0xFFFFFFFFU);
	}

    /*
	 * Program the priority mask of the CPU using the Priority mask register
	 */
    GIC_CPUWriteReg( GIC_CPU_PRIOR_OFFSET, GIC_CPU_PROPERTY_MSK_STEP16);

    /* Boot lader doesn't need interrupts via GIC so disable GICD/GICC */
#if 0
	GIC_CPUWriteReg(GIC_CONTROL_OFFSET, 0x03U);
    
	GIC_DistWriteReg(GIC_DIST_EN_OFFSET, GIC_EN_INT_MASK);
	/*
	 * If the CPU operates only in the secure domain, setup the
	 * control_s register.
	 * EnableGrp0[0] = 1
	 * EnableGrp1[1] = 1
	 * AckCtl[2] = 0
	 * FIQEn[3] = 0 (using IRQ)
	 */
#endif
}
/*******************************************************************************
 End of function GIC_CfgInitialize
 ******************************************************************************/
/*******************************************************************************
 * Function Name: Init_GIC
 * Description  : Interrupt controller driver initialization
 *
 * Arguments    : cpu_id - interrupt target CPU No
 * Return Value : error code -
 *
 ******************************************************************************/
void Init_GIC(uint32_t cpu_id)
{
	
    GIC_DistWriteReg(GIC_DIST_EN_OFFSET, 0U);
    GIC_CPUWriteReg(GIC_BIN_PT_OFFSET, 0x2);
    GIC_CPUWriteReg(GIC_CONTROL_OFFSET, 0x0);
    GIC_CPUWriteReg(GIC_EOI_OFFSET, 0x0);

    GIC_CfgInitialize(cpu_id);

    return;
}
/*******************************************************************************
 End of function INTC_Initalize
 ******************************************************************************/
