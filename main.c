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
#include "main.h"
#include "dgtable.h"
#include "bit.h"
#include "dgemmc.h"
#include "rdk_cmn_gic.h"
#include "scifdrv.h"
#include "devdrv.h"
#include "init_scif.h"
#include "HardwareSetup.h"

#define WRITER_LSI	"RZ/V2MA"		/* LSI */
#define WRITER_VERSION	" V1.00"		/* Software Version */
#define WRITER_DATE	" September 2, 2022"	/* Release date */

extern const com_menu MonCom[COMMAND_UNIT];

char gKeyBuf[64];
int32_t gComNo;

void Main(void)
{
	/* Initialize the GIC for core0 */
	Init_GIC(0);
    
	/* Set Pin Function */
	InitPFC();
	/* Initialize for CPG */
	InitCPG();

    /* Initialize for System ram with RAMB */
	PowerOnRAMB();
    
    /* Initialize for UART */
	InitScif();
	
    /* Initialize for eMM */
	dg_init_emmc();
    
    /* Output the stating messages */
	StartMess();
    
    /* Main routine */
	DecCom();
}


void StartMess( void )
{
	uint32_t product;

	PutStr("  ",1);
	PutStr("Flash writer for ",0);
	PutStr(WRITER_LSI, 0);
	PutStr(WRITER_VERSION, 0);
	PutStr(WRITER_DATE,1);
	PutStr(">", 0);
}

void DecCom(void)
{
	char tmp[64], chCnt, chPtr;
	uint32_t rtn = 0;
	chCnt = 1;

	while (rtn == 0)
	{
		if (rtn == 0)
		{
			rtn = SCIF_TerminalInputCheck(gKeyBuf);
		}
		if (0 < rtn)
		{
			if (gKeyBuf[0] == CR_CODE)
			{
				PutChar(LF_CODE);
				PutChar(CR_CODE);
				PutStr(">", 0);
				chCnt = 0;

			}
			else if (gKeyBuf[0] != LF_CODE)
			{
				PutChar(gKeyBuf[0]);
			}
		}
	}
	while(1)
	{
		GetStr(gKeyBuf+chCnt,&chCnt);
		chPtr=0;
		GetStrBlk(gKeyBuf,tmp,&chPtr,0);
		if (chPtr!=1)
		{
			ChgLtl2Lrg(tmp);
			if(!CmpCom(tmp))
			{
				(MonCom[gComNo].comProg)();
			}
			else
			{
				PutStr("command not found",1);
			}
		}
		PutStr(">",0);
		chCnt=0;
	}
}

long CmpCom(char *str)
{
	char *cmpStr,*tmpStr,err;
	gComNo = 0;
	while(TBL_END!=MonCom[gComNo].comStr)
	{
		err=0; tmpStr = str;
		cmpStr = MonCom[gComNo].comStr;
		while(*tmpStr!=0)
		{
			if (*tmpStr == *cmpStr)
			{
				tmpStr++; cmpStr++;
			}
			else
			{
				err=1; break;
			}
		}
		if ((!err)&&(*cmpStr==0))
		{
			return(0);
		}
		gComNo++;
	}
	return(1);
}
