/*
 * Copyright (c) 2015-2020, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "common.h"
#include "dgtable.h"
#include "dgmodul1.h"
#include "devdrv.h"
#include "devdrv.h"
#include "bit.h"
#include "rzg2l_def.h"
#include "cpudrv.h"
#include "scifdrv.h"

#define	WDTCNT0	(RZG2L_WDT_BASE + 0x00)
#define	WDTSET0	(RZG2L_WDT_BASE + 0x04)

extern const char *const AllHelpMess[ALL_HELP_MESS_LINE];
extern const com_menu MonCom[COMMAND_UNIT];
extern int32_t  gComNo;
extern char gKeyBuf[64];

uint32_t gFLASH_CS1_ID;



/****************************************************************
	MODULE		: dgHelp				*
	FUNCTION	: HELP	MESSAGE				*
	COMMAND		: H					*
	INPUT PARAMETER	: H					*
*****************************************************************/
void	dgHelp(void)
{
	char tmp[64],chPtr,helpNo;

	chPtr=0;
	if(!GetStrBlk(gKeyBuf,tmp,&chPtr,0)){
		PutMess(AllHelpMess);
	}
}



int32_t	GetStr_ByteCount(char *str,uint32_t getByteCount)
{
	uint32_t	byteCount;
	int32_t i;

	for(byteCount=1;byteCount<=getByteCount;byteCount=byteCount+1){
		i=1;
		while(i==1){
			i=GetChar(str);
		}
		str++;
		i=1;
		while(i==1){
			i=GetChar(str);
		}
		str++;
	}
	*str = 0;
}



void	dgScifSpeedUp(void)
{
	dgScifSpeedUp_921600();
}

/****************************************************************
	MODULE		: dgScifSpeedUp				*
	FUNCTION	: Scif speed UP	Change 921.6kbps	*
	COMMAND		: SUP					*
	INPUT PARAMETER	: SUP					*
*****************************************************************/
void	dgScifSpeedUp_921600(void)
{
}

void	dgReset(void)
{
	*((volatile uint32_t*)WDTSET0) = 0x001FFFFF;
	*((volatile uint32_t*)WDTCNT0) = 0x01;
}
