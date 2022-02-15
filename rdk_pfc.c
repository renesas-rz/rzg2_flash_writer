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
 * File Name    : rdk_pfc.c
 * Description  : operation functions for PFC
 ******************************************************************************/

#include <stdint.h>

#include "rdk_common.h"
#include "rdk_pfc.h"

#define PFC_PORT_NUM                        (22)
#define PFC_PORT_REGISTER_SIZE              (0x00040)
#define PFC_PORT_WE_SHIFT                   (16)
#define PFC_PORT_PFSEL_PIN_NUM_IN_REG       (4)
#define PFC_PORT_PFSEL_REG_NUM              (4)

typedef struct {
    uint8_t                 pin_num;
	union {
        uint32_t            word;
		struct {
            uint32_t        gpio_do:1;
            uint32_t        gpio_oe:1;
            uint32_t        gpio_ie:1;
            uint32_t        :1;
            uint32_t        pfsel:4;
            uint32_t        di_mon:1;
            uint32_t        pupd:1;
            uint32_t        drv:1;
            uint32_t        sr:1;
            uint32_t        di_msk:1;
            uint32_t        en_msk:1;
        } flag;
    } exist_reg;
} st_pfc_internal_info_t;

static const uint8_t g_PFC_flag_bit_position[10] = {
    0, /* PFC_PORT_GPIO_DO */
    1, /* PFC_PORT_GPIO_OE */
    2, /* PFC_PORT_GPIO_IE */
    4, /* PFC_PORT_PFSEL */
    8, /* PFC_PORT_DI_MON */
    9, /* PFC_PORT_PUPD */
    10, /* PFC_PORT_DRV */
    11, /* PFC_PORT_SR */
    12, /* PFC_PORT_DI_MSK */
    13  /* PFC_PORT_EN_MSK */
};

static const st_pfc_internal_info_t g_PFC_Port_Info[PFC_PORT_NUM] = {
{
	/** PORT00 **/
        .pin_num = 14,
        .exist_reg.word = 0x3FF7,
    },
    {
	/** PORT01 **/
        .pin_num = 16,
        .exist_reg.word = 0x3FF7,
    },
    {
	/** PORT02 **/
        .pin_num = 8,
        .exist_reg.word = 0x3F37,
    },
    {
	/** PORT03 **/
        .pin_num = 16,
        .exist_reg.word = 0x3FF7,
    },
    {
	/** PORT04 **/
        .pin_num = 8,
        .exist_reg.word = 0x3F37,
    },
    {
	/** PORT05 **/
        .pin_num = 4,
        .exist_reg.word = 0x3F17,
    },
    {
	/** PORT06 **/
        .pin_num = 12,
        .exist_reg.word = 0x3F77,
    },
    {
	/** PORT07 **/
        .pin_num = 6,
        .exist_reg.word = 0x3F37,
    },
    {
	/** PORT08 **/
        .pin_num = 8,
        .exist_reg.word = 0x3F37,
    },
    {
	/** PORT09 **/
        .pin_num = 8,
        .exist_reg.word = 0x3F37,
    },
    {
	/** PORT10 **/
        .pin_num = 9,
        .exist_reg.word = 0x3F77,
    },
    {
	/** PORT11 **/
        .pin_num = 9,
        .exist_reg.word = 0x3F77,
    },
    {
	/** PORT12 **/
        .pin_num = 4,
        .exist_reg.word = 0x3F17,
    },
    {
	/** PORT13 **/
        .pin_num = 12,
        .exist_reg.word = 0x3F77,
    },
    {
	/** PORT14 **/
        .pin_num = 8,
        .exist_reg.word = 0x3F37,
    },
    {
	/** PORT15 **/
        .pin_num = 16,
        .exist_reg.word = 0x3FF7,
    },
    {
	/** PORT16 **/
        .pin_num = 14,
        .exist_reg.word = 0x3FF7,
    },
    {
	/** PORT17 **/
        .pin_num = 1,
        .exist_reg.word = 0x3F17,
    },
    {
	/** (PORT18) **/
        .pin_num = 0,
        .exist_reg.word = 0,
    },
    {
	/** (PORT19) **/
        .pin_num = 0,
        .exist_reg.word = 0,
    },
    {
	/** PORT20 **/
        .pin_num = 3,
        .exist_reg.word = 0x3517,
    },
    {
	/** PORT21 **/
        .pin_num = 1,
        .exist_reg.word = 0x0D07,
    },
};


static uint32_t pfc_GetBitMask(e_pfc_port_num_t port_num)
{
    uint32_t value = 0;
    uint32_t shift = 0;
    uint32_t pin_num = g_PFC_Port_Info[port_num].pin_num;

	if (0 != (pin_num & 0x010)) {
        value |= (0x0FFFF << shift);
        shift += 16;
    }
	if (0 != (pin_num & 0x008)) {
        value |= (0x0FF << shift);
        shift += 8;
    }
	if (0 != (pin_num & 0x004)) {
        value |= (0x0F << shift);
        shift += 4;
    }
	if (0 != (pin_num & 0x002)) {
        value |= (0x03 << shift);
        shift += 2;
    }
	if (0 != (pin_num & 0x001)) {
        value |= (0x01 << shift);
        shift += 1;
    }

    return value;
}

static uint32_t pfc_Get2BitMask(e_pfc_port_num_t port_num)
{
    uint32_t value = 0;
    uint32_t shift = 0;
    uint32_t pin_num = g_PFC_Port_Info[port_num].pin_num;

	if (0 != (pin_num & 0x010)) {
        value |= (0xFFFFFFFF << shift);
        shift += 32;
    }
	if (0 != (pin_num & 0x008)) {
        value |= (0x0FFFF << shift);
        shift += 16;
    }
	if (0 != (pin_num & 0x004)) {
        value |= (0x0FF << shift);
        shift += 8;
    }
	if (0 != (pin_num & 0x002)) {
        value |= (0x0F << shift);
        shift += 4;
    }
	if (0 != (pin_num & 0x001)) {
        value |= (0x03 << shift);
        shift += 2;
    }

    return value;
}


uint32_t PFC_ReadReg(uint32_t offset)
{
    return CMN_REG_Read32(PFC_BASE_ADDRESS + offset);
}

void PFC_WriteReg(uint32_t offset, uint32_t value)
{
    CMN_REG_Write32((PFC_BASE_ADDRESS + offset), value);
}

int32_t PFC_SetPortParam(e_pfc_port_num_t port_num, e_pfc_kind_reg_t kind_reg,
    uint16_t target, u_pfc_data_t *p_set_data)
{
    uint32_t offset = PFC_P00_GPIO_DO;
    uint32_t value[4] = { 0 };
    uint32_t mask[4] = { 0 };
    int32_t  rslt = CMN_SUCCESS;
    int32_t  i;
    uint8_t  pos;

    pos = g_PFC_flag_bit_position[kind_reg];
	if (0 == (g_PFC_Port_Info[port_num].exist_reg.word & (0x01 << pos))) {
            return PFC_ERROR_NO_EXIST_REG;
    }

	if (NULL == p_set_data) {
            return PFC_ERROR_NULL_POINTER;
    }

    offset += (port_num * PFC_PORT_REGISTER_SIZE);
    offset += (pos * sizeof(uint32_t));

	switch (kind_reg) {
        case PFC_PORT_GPIO_DO:
        case PFC_PORT_GPIO_OE:
        case PFC_PORT_GPIO_IE:
        case PFC_PORT_SR:
        case PFC_PORT_DI_MSK:
        case PFC_PORT_EN_MSK:
            mask[0] = pfc_GetBitMask(port_num);
		value[0] = (p_set_data->uh_data & mask[0]) |
			   ((target & mask[0]) << PFC_PORT_WE_SHIFT);
            PFC_WriteReg(offset, value[0]);
            break;

        case PFC_PORT_PFSEL:
		for (i = 0; i < g_PFC_Port_Info[port_num].pin_num; i++) {
			if (0 != (target & (0x0001 << i))) {
                    mask[i/PFC_PORT_PFSEL_PIN_NUM_IN_REG] |=
				    (0x00070007
				     << (4 *
					 (i % PFC_PORT_PFSEL_PIN_NUM_IN_REG)));
                }
            }

		for (i = 0; i < PFC_PORT_PFSEL_REG_NUM; i++) {
			value[i] =
			    (p_set_data->func_sel.half.sel[i]) | 0xFFFF0000UL;
                value[i] &= mask[i];

			if ((0 !=
			     (g_PFC_Port_Info[port_num].exist_reg.flag.pfsel &
			      (0x01 << i))) &&
			    0 != value[i]) {
                    PFC_WriteReg(offset, value[i]);
                }
                offset += sizeof(uint32_t);
            }
            break;

        case PFC_PORT_PUPD:
        case PFC_PORT_DRV:
		for (i = 0; i < g_PFC_Port_Info[port_num].pin_num; i++) {
			if (0 != (target & (0x0001 << i))) {
                    mask[0] |= (0x00000003 << (2 * i));
                }
            }
            mask[0] &= pfc_Get2BitMask(port_num);
		if (PFC_PORT_PUPD == kind_reg) {
                value[0] = p_set_data->pupd.word.pupd[0];
		} else /** if (PFC_PORT_DRV == kind_reg) */
            {
                value[0] = p_set_data->drv_sel.word.drv_sel[0];
            }
            value[0] &= mask[0];
            value[0] |= (PFC_ReadReg(offset) & (~mask[0]));
            PFC_WriteReg(offset, value[0]);
            break;

        case PFC_PORT_DI_MON:
        default:
            rslt = PFC_ERROR_NO_EXIST_REG;
            break;
    }

    return rslt;
}

int32_t PFC_GetPortParam(e_pfc_port_num_t port_num, e_pfc_kind_reg_t kind_reg,
    u_pfc_data_t *p_data)
{
    uint32_t offset = PFC_P00_GPIO_DO;
    int32_t  rslt = CMN_SUCCESS;
    uint32_t value;
    int32_t  i;
    uint8_t  pos;

    pos = g_PFC_flag_bit_position[kind_reg];
	if (0 == (g_PFC_Port_Info[port_num].exist_reg.word & (0x01 << pos))) {
            return PFC_ERROR_NO_EXIST_REG;
    }

	if (NULL == p_data) {
            return PFC_ERROR_NULL_POINTER;
    }

    offset += (port_num * PFC_PORT_REGISTER_SIZE);
    offset += (pos * 4UL);

	switch (kind_reg) {
        case PFC_PORT_GPIO_DO:
        case PFC_PORT_GPIO_OE:
        case PFC_PORT_GPIO_IE:
        case PFC_PORT_DI_MON:
        case PFC_PORT_PUPD:
        case PFC_PORT_DRV:
        case PFC_PORT_SR:
        case PFC_PORT_DI_MSK:
        case PFC_PORT_EN_MSK:
            p_data->uw_data = PFC_ReadReg(offset);
            break;

        case PFC_PORT_PFSEL:
            p_data->func_sel.word.sel[0] = 0;
            p_data->func_sel.word.sel[1] = 0;
		for (i = 0; i < PFC_PORT_PFSEL_REG_NUM; i++) {
                value = 0;
			if (0 !=
			    (g_PFC_Port_Info[port_num].exist_reg.flag.pfsel &
			     (0x01 << i))) {
                    value = PFC_ReadReg(offset);
                }
                p_data->func_sel.half.sel[i] = (value & 0x0FFFF);
                offset += sizeof(uint32_t);
            }
            break;

        default:
            rslt = PFC_ERROR_NO_EXIST_REG;
            break;
    }

    return rslt;
}

int32_t PFC_SetPortParamH(e_pfc_port_num_t port_num, e_pfc_kind_reg_t kind_reg,
    uint16_t target, uint16_t set_data)
{
    int32_t res;
    u_pfc_data_t data;

    data.uh_data = set_data;

    res = PFC_SetPortParam(port_num, kind_reg, target, &data);

    return res;
}

int32_t PFC_SetCXRXD_SEL(uint16_t target, uint16_t set_data)
{
    uint32_t value;

	value = (set_data & target) | (((uint32_t)target) << PFC_PORT_WE_SHIFT);

    PFC_WriteReg( PFC_CSRXD_SEL, value);

    return PFC_SUCCESSED;
}

int32_t PFC_GetCXRXD_SEL(uint16_t *p_data)
{
	if (NULL == p_data) {
        return PFC_ERROR_NULL_POINTER;
    }

    *p_data = PFC_ReadReg(PFC_CSRXD_SEL);

    return PFC_SUCCESSED;
}

int32_t PFC_SetROP_DI_SEL(uint32_t set_data)
{
    PFC_WriteReg( PFC_ROP_DI_SEL, set_data);

    return PFC_SUCCESSED;
}

int32_t PFC_GetROP_DI_SEL(uint32_t *p_data)
{
	if (NULL == p_data) {
        return PFC_ERROR_NULL_POINTER;
    }

    *p_data = PFC_ReadReg(PFC_ROP_DI_SEL);

    return PFC_SUCCESSED;
}

int32_t PFC_SetPEXDRV(uint16_t target, u_pfc_pex_drv_t *p_set_data)
{
    uint32_t i;
    uint32_t mask = 0;
    uint32_t value;
    uint16_t exist_bit = 0x5867;

	if (NULL == p_set_data) {
        return PFC_ERROR_NULL_POINTER;
    }

	for (i = 0; i < (sizeof(uint16_t) * 8); i++) {
		if (0 != ((target & exist_bit) & (0x0001 << i))) {
            mask |= (0x00000003 << (2 * i));
        }
    }

    value = PFC_ReadReg(PFC_PEX0_DRV);
    value &= ~(mask);
    value |= (p_set_data->word.drv[0] & mask);
    PFC_WriteReg(PFC_PEX0_DRV, value);

    return PFC_SUCCESSED;
}

int32_t PFC_GetPEXDRV(uint32_t *p_data)
{
	if (NULL == p_data) {
        return PFC_ERROR_NULL_POINTER;
    }

    *p_data = PFC_ReadReg(PFC_PEX0_DRV);

    return PFC_SUCCESSED;
}

int32_t PFC_SetPEXSR(uint16_t target, uint16_t set_data)
{
    uint32_t value;

    value = set_data | ((uint32_t)target << PFC_PORT_WE_SHIFT);
    PFC_WriteReg(PFC_PEX0_SR, value);

    return PFC_SUCCESSED;
}

int32_t PFC_GetPEXSR(uint32_t *p_data)
{
	if (NULL == p_data) {
        return PFC_ERROR_NULL_POINTER;
    }

    *p_data = PFC_ReadReg(PFC_PEX0_SR);

    return PFC_SUCCESSED;
}

int32_t PFC_SetEXTINT(e_pfc_extint_reg_num_t reg_num, uint16_t target_pin,
		      uint16_t set_data)
{
    uint32_t offset = PFC_EXTINT_INV0;
    uint32_t value;

    offset += reg_num * sizeof(uint32_t);
    value = (set_data & target_pin) |
        (((uint32_t)target_pin) << PFC_PORT_WE_SHIFT);

    PFC_WriteReg(offset, value);

    return PFC_SUCCESSED;
}

int32_t PFC_GetEXTINT(e_pfc_extint_reg_num_t reg_num, uint32_t *p_data)
{
    uint32_t offset = PFC_EXTINT_INV0;

	if (NULL == p_data) {
        return PFC_ERROR_NULL_POINTER;
    }

    offset += reg_num * sizeof(uint32_t);

    *p_data = PFC_ReadReg(offset);

    return PFC_SUCCESSED;
}

/*****************************************************************************
 *****************************************************************************/

/*
 * Set change pin function procduer
 * 1. target pins are set mask to Pmm_DI_MSK
 * 2. target pins are set enable mask to Pmm_EN_MSK
 * 3. set change function to Pmm_PFSEL0-3
 * 4. target pins are set enable to Pmm_EN_MSK
 * 5. target pins are set unmask to Pmm_DI_MSK
 */
/*
 * EMM   : Port00, pin 0-7 10-11, 2
 * URT0  : Port03, pin 0-3, 2
 */
/*
 * port#, target pin map, select#
 */
typedef struct {
    uint32_t pin_pupd;
    uint32_t pin_drv;
    uint16_t pin_oe;
    uint16_t pin_ie;
    uint16_t pin_map;
    uint16_t pin_sr;
    uint8_t port;
    uint8_t select;
} st_pfc_select_port_info_t;

typedef struct {
    uint32_t cnt;
    const st_pfc_select_port_info_t *p_data;
} st_pfc_select_info_t;

/*
 * [pin_pupd] b00:pull-down / b10:pull-up / bX1:neither
 * [pin_driv] b00:X1 / b01:X2 / b10:X4 / b11:X6
 * [pin_sr  ] b0:FastSlew / b1:SlowSlew
 */

static const st_pfc_select_port_info_t gl_pfc_select_emm[] = {
    {.port = PFC_PORT00,
     .select = 2,
     .pin_map = 0x0CFFU,
     .pin_oe = 0x0000U,
     .pin_ie = 0x0000U,
     .pin_pupd = 0x00505555,
     .pin_drv = 0x00505555,
     .pin_sr = 0x0CFFU},
};

static const st_pfc_select_port_info_t gl_pfc_select_urt0[] = {
    {.port = PFC_PORT03,
     .select = 2,
     .pin_map = 0x000FU,
     .pin_oe = 0x0000U,
     .pin_ie = 0x0000U,
     .pin_pupd = 0x00000055,
     .pin_drv = 0x00000055,
     .pin_sr = 0x000FU},
};


#define PFC_SET_SELECT_TBL(m_ip_info)   \
    .cnt = (sizeof(m_ip_info)/sizeof(st_pfc_select_port_info_t)),   \
    .p_data = m_ip_info

static const st_pfc_select_info_t gl_pfc_select_tbl[] = {
    {   PFC_SET_SELECT_TBL(gl_pfc_select_emm)   },
    {   PFC_SET_SELECT_TBL(gl_pfc_select_urt0)  },
};

int32_t PFC_SetPinFunc(e_pfc_select_ip_t tgt_ip)
{
    int32_t rslt;
    uint32_t i;
    uint32_t j;
    const st_pfc_select_port_info_t *p_sel_info;
    uint32_t cnt;
    uint32_t port_num;
    uint32_t pin_cnt;
    u_pfc_pfsel_t sel_data;
    uint16_t sel_pin;
    uint32_t select_num;
    u_pfc_pupd_t   pupd_data;
    u_pfc_drvsel_t drv_data;
    uint16_t sr_data;
    uint16_t oe_data;
    uint16_t ie_data;

	do {
		if (PFC_SELECT_NUM <= tgt_ip) {
            rslt = PFC_ERROR_INVALID_ARG;
            break;
        }

        cnt = gl_pfc_select_tbl[tgt_ip].cnt;
        p_sel_info = gl_pfc_select_tbl[tgt_ip].p_data;

		for (i = 0; i < cnt; i++) {
            port_num = p_sel_info[i].port;
            sel_pin = p_sel_info[i].pin_map;
            select_num = p_sel_info[i].select;
            pin_cnt = g_PFC_Port_Info[port_num].pin_num;
            pupd_data.word.pupd[0] = p_sel_info[i].pin_pupd;
            drv_data.word.drv_sel[0] = p_sel_info[i].pin_drv;
            sr_data = p_sel_info[i].pin_sr;
            oe_data = p_sel_info[i].pin_oe;
            ie_data = p_sel_info[i].pin_ie;
            sel_data.word.sel[0] = 0;
            sel_data.word.sel[1] = 0;
			for (j = 0; j < pin_cnt; j++) {
				if (0 != (sel_pin & (0x01U << j))) {
                    sel_data.word.sel[j/8] |=
                        (select_num << (j * 4));
                }
            }

            (void)PFC_SetDI_MSK(port_num, sel_pin, sel_pin);

            (void)PFC_SetEN_MSK(port_num, sel_pin, sel_pin);

            (void)PFC_SetPFSEL(port_num, sel_pin, &sel_data);

            (void)PFC_SetGPIO_OE(port_num, sel_pin, oe_data);

            (void)PFC_SetGPIO_IE(port_num, sel_pin, ie_data);

            (void)PFC_SetEN_MSK(port_num, sel_pin, 0);

            (void)PFC_SetDI_MSK(port_num, sel_pin, 0);

            (void)PFC_SetPUPD(port_num, sel_pin, &pupd_data);

            (void)PFC_SetDRV(port_num, sel_pin, &drv_data);

            (void)PFC_SetSR(port_num, sel_pin, sr_data);
        }
        rslt = CMN_SUCCESS;
	} while (0);

    return rslt;
}

