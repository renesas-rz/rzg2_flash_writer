/*
 * Copyright (c) 2015-2021, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "common.h"
#include "main.h"
#include "dgtable.h"
#include "bit.h"
#include "cpudrv.h"
#if EMMC == 1
#include "dgemmc.h"
#endif /* EMMC == 1 */
#include "scifdrv.h"
#include "devdrv.h"
#include "rpcqspidrv.h"
#include "dgmodul1.h"
#include "dgmodul4.h"
#include "tzc_400.h"
#include "mmio.h"
#include "cpg.h"
#include "pfc.h"
#include "syc.h"
#include "ddr.h"
#include "sysc.h"

extern const char *const StartMessMonitor[START_MESS_MON_LINE];
extern const com_menu MonCom[COMMAND_UNIT];
extern uint8_t	gCOMMAND_Area[COMMAND_BUFFER_SIZE];

char gKeyBuf[64];
int32_t gComNo;
extern uintptr_t gUDump[3];
extern uintptr_t gUMem[3];

extern uint32_t gDumpMode;
uint32_t gDumpStatus;

#define RZG2L_SYC_INCK_HZ           (24000000)

void Main(void)
{
	uint32_t readDevId;

	init_tzc_400_spimulti();

	/* early setup Clock and Reset */
	cpg_early_setup();

	/* initialize SYC */
	syc_init(RZG2L_SYC_INCK_HZ);

	/* setup PFC */
	pfc_setup();

	/* setup Clock and Reset */
	cpg_setup();

	udelay(100);

#if (INTERNAL_MEMORY_ONLY == 0)
	ddr_setup();
#endif
	gUDump[0] =  gUMem[0] =  0;
	gUDump[1] =  gUMem[1] =  255;

	gDumpMode	= SIZE_8BIT;
	gDumpStatus	= DISABLE;

	InitRPC_Mode();
	ReadQspiFlashID(&readDevId);	/* dummy	*/

	InitMain();
	StartMess();
	DecCom();
}

void InitMain(void)
{
#if EMMC == 1
	dg_init_emmc();
#endif /* EMMC == 1 */
}

void StartMess( void )
{
	unsigned int rev;
	char	str[4];

	PutStr("  ",1);
	PutMess(StartMessMonitor);
	PutStr(" Product Code : ", 0);
#if (RZG2L == 1)
	PutStr("RZ/G2L" ,0);
#endif
#if (RZG2LC == 1)
	PutStr("RZ/G2CL" ,0);
#endif
	PutStr(" rev ", 0);
	rev = sysc_get_device_revision();
	if (rev > 9)
	{
		str[0] = '1';
		str[1] = '0' + (rev - 10);
		str[2] = 0;
	}
	else
	{
		str[0] = '0' + rev;
		str[1] = 0;
	}
	PutStr(str, 0);
	PutStr(".0", 1);
	PutStr(">", 0);
}

void DecCom(void)
{
	char tmp[64], chCnt, chPtr;
	uint32_t rtn = 0;
	uint32_t res;
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
		chPtr = 0;
		GetStrBlk(gKeyBuf,tmp,&chPtr,0);
		if (chPtr != 1)
		{
			ChgLtl2Lrg(tmp);
			if (!CmpCom(tmp))
			{
				(MonCom[gComNo].comProg)();
			}
			else
			{
				PutStr("command not found",1);
			}
		}
		PutStr(">",0);
		chCnt = 0;
	}
}

long CmpCom(char *str)
{
	char *cmpStr, *tmpStr, err;
	gComNo = 0;
	while(TBL_END != MonCom[gComNo].comStr)
	{
		err=0;
		tmpStr = str;
		cmpStr = MonCom[gComNo].comStr;
		while(*tmpStr!=0)
		{
			if (*tmpStr == *cmpStr)
			{
				tmpStr++;
				cmpStr++;
			}
			else
			{
				err = 1;
				break;
			}
		}
		if ((!err) && (*cmpStr == 0))
		{
			return(0);
		}
		gComNo++;
	}
	return(1);
}
