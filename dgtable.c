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


#include "common.h"
#include "dgtable.h"
#if EMMC == 1
#include "dgemmc.h"
#endif /* EMMC == 1 */

/********************************************************/
/*        ROM TABLE                                     */
/********************************************************/
const com_menu MonCom[COMMAND_UNIT] = {
/*--------------------- Basic command ------------------------------*/
#if EMMC == 1
	"EM_DCID"	, &dg_emmc_disp_cid		,  0	,
	"EM_DCSD"	, &dg_emmc_disp_csd		,  0	,
	"EM_DECSD"	, &dg_emmc_disp_ext_csd		,  0	,
	"EM_SECSD"	, &dg_emmc_set_ext_csd		,  0	,
	"EM_WB"		, &dg_emmc_write_bin		,  0	,
	"EM_E"		, &dg_emmc_erase		,  0	,
#endif /* EMMC == 1 */
	TBL_END		, 0				,  0
};
