/**********************************************************************************************************************
 * DISCLAIMER
 * This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products. No
 * other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
 * applicable laws, including copyright laws.
 * THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
 * THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED. TO THE MAXIMUM
 * EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES
 * SHALL BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO
 * THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
 * Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability of
 * this software. By using this software, you agree to the additional terms and conditions found by accessing the
 * following link:
 * http://www.renesas.com/disclaimer
 *
 * Copyright (C) 2020 Renesas Electronics Corporation. All rights reserved.
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * File Name    : tzc_400.c
 * Version      : 1.0
 * Description  : TZC-400 driver.
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * History : DD.MM.YYYY Version  Description
 *         : 01.09.2020 1.00     First Release
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Includes   <System Includes> , "Project Includes"
 *********************************************************************************************************************/
#if 0
#include "iodefine.h"
#include "gpio_iodefine.h"
#endif
#include "tzc_400_iodefine.h"

/**********************************************************************************************************************
 Macro definitions
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Local Typedef definitions
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Exported global variables
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Private (static) variables and functions
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: init_tzc_400_spimulti
 * Description  : Initialize TAC-400 for using SRAM and SPI.
 * Arguments    : none.
 * Return Value : none.
 *********************************************************************************************************************/
void init_tzc_400_spimulti(void)
{
    /*** Initialize SPI Multi ***/
    TZC_RGN_ATTR_SPI   = TZC_RGN_ATTR_EN_ALLFLT;

    TZC_RGN_ID_ACC_SPI = TZC_RGN_ID_ACC_EN_RDWR_NSAID0_1;

    TZC_GATE_KEEP_SPI  = TZC_GATE_KEEP_OPEN_ALLFLT;

    TZC_SPECUL_CTL_SPI = TZC_SPECUL_DIS_PREFETCH;

    /*** Initialize DDR ***/
    TZC_RGN_ATTR_TZCDDR   = TZC_RGN_ATTR_EN_ALLFLT;

    TZC_RGN_ID_ACC_TZCDDR = TZC_RGN_ID_ACC_EN_RDWR_NSAID0_1;

    TZC_GATE_KEEP_TZCDDR  = TZC_GATE_KEEP_OPEN_ALLFLT;

    TZC_SPECUL_CTL_TZCDDR = TZC_SPECUL_DIS_PREFETCH;;
}
/**********************************************************************************************************************
 * End of function init_tzc_400_spimulti
 *********************************************************************************************************************/

