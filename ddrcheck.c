/*
 * Copyright (c) 2015-2021, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#pragma GCC optimize ("Og")

#include "common.h"
#include "dgtable.h"
#include "ramckmdl.h"
#include "devdrv.h"
#include "ddrcheck.h"

uintptr_t	gErrDdrAdd;
uint32_t	gErrDdrData,gTrueDdrData;
uintptr_t	gSubErrAdd;
uintptr_t	gSubErrData;
uintptr_t	gSubTrueData;

extern char gKeyBuf[64];

static uint32_t PutDdrErrInfo(void)
{
	char str[64];

	PutStr(" error address  : H'",0);	Data2HexAscii_64(gErrDdrAdd,str,CPU_BYTE_SIZE);	PutStr(str,1);
	PutStr(" error data     : H'",0);	Data2HexAscii(gErrDdrData,str,4);	PutStr(str,1);
	PutStr(" true  data     : H'",0);	Data2HexAscii(gTrueDdrData,str,4);	PutStr(str,1);
}

static uint32_t CkExtendDdrRamCheck(void* ramAddr)
{
	volatile uint32_t *read_adr;
	uint32_t data;
	uint32_t loop, i;
	char str[64];

	read_adr = (uint32_t *)ramAddr;

	PutStr("Data=0x5A5A5A5A",0);

	/* Write */
	data = 0x5A5A5A5A;
	for (loop = 0; loop < 0x100000; loop++)
	{
		read_adr[loop] = data;
	}
	/* Verify */
	data = 0x5A5A5A5A;
	for (loop = 0; loop < 0x100000; loop++)
	{
		if (read_adr[loop] != data)
		{
			gErrDdrAdd   = (uintptr_t)&read_adr[loop];
			gErrDdrData  = read_adr[loop];
			gTrueDdrData = data;
			return(ERROR_END);
		}
	}
	DelStr(15);
	PutStr("Data=0xA5A5A5A5",0);

	/* Write */
	data = 0xA5A5A5A5;
	for (loop = 0; loop < 0x100000; loop++)
	{
		read_adr[loop] = data;
	}
	/* Verify */
	data = 0xA5A5A5A5;
	for (loop = 0; loop < 0x100000; loop++)
	{
		if (read_adr[loop] != data)
		{
			gErrDdrAdd   = (uintptr_t)&read_adr[loop];
			gErrDdrData  = read_adr[loop];
			gTrueDdrData = data;
			return(ERROR_END);
		}
	}
	DelStr(15);
	PutStr("Data=0x12345678",0);
 
	/* Write */
	data = 0x12345678;
	for (loop = 0; loop < 0x100000; loop++)
	{
		read_adr[loop] = data;
		data += 0x11111111;
	}
	/* Verify */
	data = 0x12345678;
	for (loop = 0; loop < 0x100000; loop++)
	{
		if (read_adr[loop] != data)
		{
			gErrDdrAdd   = (uintptr_t)&read_adr[loop];
			gErrDdrData  = read_adr[loop];
			gTrueDdrData = data;
			return(ERROR_END);
		}
		data += 0x11111111;
	}
	DelStr(15);
	return(NORMAL_END);
}

static int32_t CheckAndFillData8Bit( uint8_t *startAddr, uint8_t *endAddr, uint8_t writeData, uint8_t checkData )
{
	volatile uint8_t *pData;
	pData = (uint8_t *)startAddr;
	while(1)
	{
		if (*pData!=checkData) break;  *(pData++) = writeData;
		if (*pData!=checkData) break;  *(pData++) = writeData;
		if (*pData!=checkData) break;  *(pData++) = writeData;
		if (*pData!=checkData) break;  *(pData++) = writeData;
		if (*pData!=checkData) break;  *(pData++) = writeData;
		if (*pData!=checkData) break;  *(pData++) = writeData;
		if (*pData!=checkData) break;  *(pData++) = writeData;
		if (*pData!=checkData) break;  *(pData++) = writeData;
		if (*pData!=checkData) break;  *(pData++) = writeData;
		if (*pData!=checkData) break;  *(pData++) = writeData;
		if (*pData!=checkData) break;  *(pData++) = writeData;
		if (*pData!=checkData) break;  *(pData++) = writeData;
		if (*pData!=checkData) break;  *(pData++) = writeData;
		if (*pData!=checkData) break;  *(pData++) = writeData;
		if (*pData!=checkData) break;  *(pData++) = writeData;
		if (*pData!=checkData) break;  *(pData++) = writeData;
		if (pData >= endAddr)  return NORMAL_END;
	}
	gSubErrAdd   = (uint64_t)pData;
	gSubErrData  = (uint64_t)*pData;
	gSubTrueData = (uint64_t)checkData;
	return ERROR_END;
}

static int32_t FillData8Bit(uint8_t *startAddr, uint8_t *endAddr, uint8_t writeData)
{
	volatile uint8_t *pData;
	pData = (uint8_t *)startAddr;
	while(1){
		*(pData++) = writeData;
		*(pData++) = writeData;
		*(pData++) = writeData;
		*(pData++) = writeData;
		*(pData++) = writeData;
		*(pData++) = writeData;
		*(pData++) = writeData;
		*(pData++) = writeData;
		*(pData++) = writeData;
		*(pData++) = writeData;
		*(pData++) = writeData;
		*(pData++) = writeData;
		*(pData++) = writeData;
		*(pData++) = writeData;
		*(pData++) = writeData;
		*(pData++) = writeData;
		if( pData >= endAddr )  return NORMAL_END;
	}
	return NORMAL_END;
}

static int32_t CheckData8Bit( uint8_t *startAddr, uint8_t *endAddr, uint8_t checkData )
{
	volatile uint8_t *pData;
	pData = (uint8_t *)startAddr;
	while(1)
	{
		if(*(pData++)!=checkData) break;
		if(*(pData++)!=checkData) break;
		if(*(pData++)!=checkData) break;
		if(*(pData++)!=checkData) break;
		if(*(pData++)!=checkData) break;
		if(*(pData++)!=checkData) break;
		if(*(pData++)!=checkData) break;
		if(*(pData++)!=checkData) break;
		if(*(pData++)!=checkData) break;
		if(*(pData++)!=checkData) break;
		if(*(pData++)!=checkData) break;
		if(*(pData++)!=checkData) break;
		if(*(pData++)!=checkData) break;
		if(*(pData++)!=checkData) break;
		if(*(pData++)!=checkData) break;
		if(*(pData++)!=checkData) break;
		if (pData >= endAddr)  return NORMAL_END;
	}
	pData--;
	gSubErrAdd   = (uint64_t)pData;
	gSubErrData  = (uint64_t)*pData;
	gSubTrueData = (uint64_t)checkData;
	return ERROR_END;
}

static int32_t WriteIncData8Bit( uint8_t *startAddr, uint8_t *endAddr, uint8_t startData )
{
	volatile uint8_t *pData;
	pData = (uint8_t *)startAddr;
	while(1)
	{
		*(pData++) = (startData++);
		*(pData++) = (startData++);
		*(pData++) = (startData++);
		*(pData++) = (startData++);
		*(pData++) = (startData++);
		*(pData++) = (startData++);
		*(pData++) = (startData++);
		*(pData++) = (startData++);
		*(pData++) = (startData++);
		*(pData++) = (startData++);
		*(pData++) = (startData++);
		*(pData++) = (startData++);
		*(pData++) = (startData++);
		*(pData++) = (startData++);
		*(pData++) = (startData++);
		*(pData++) = (startData++);
		if (pData >= endAddr)  return NORMAL_END;
	}
	return NORMAL_END;
}

static int32_t CheckIncData8Bit( uint8_t *startAddr, uint8_t *endAddr, uint8_t startData )
{
	volatile uint8_t *pData;
	pData = (uint8_t *)startAddr;
	while(1)
	{
		if(*(pData++)!=(startData++)) break;
		if(*(pData++)!=(startData++)) break;
		if(*(pData++)!=(startData++)) break;
		if(*(pData++)!=(startData++)) break;
		if(*(pData++)!=(startData++)) break;
		if(*(pData++)!=(startData++)) break;
		if(*(pData++)!=(startData++)) break;
		if(*(pData++)!=(startData++)) break;
		if(*(pData++)!=(startData++)) break;
		if(*(pData++)!=(startData++)) break;
		if(*(pData++)!=(startData++)) break;
		if(*(pData++)!=(startData++)) break;
		if(*(pData++)!=(startData++)) break;
		if(*(pData++)!=(startData++)) break;
		if(*(pData++)!=(startData++)) break;
		if(*(pData++)!=(startData++)) break;
		if( pData >= endAddr )  return NORMAL_END;
	}
	pData--;
	startData--;
	gSubErrAdd   = (uint64_t)pData;
	gSubErrData  = (uint64_t)*pData;
	gSubTrueData = (uint64_t)startData;
	return ERROR_END;
}

static char DecodeForm5(uintptr_t *para1st, uintptr_t *para2nd, uint32_t *setPara)
{
	char tmp[64],tmp2[64],chPtr,chPtr2,endCh,getCnt,value;
	uintptr_t tmpData;

	*setPara = 0;
	chPtr = getCnt = 0;
	do
	{
		endCh = GetStrBlk(gKeyBuf,tmp,&chPtr,0);
		switch(getCnt)
		{
			/*********** "RAMCK" Block parameter  *************/
			case 0:
				if (endCh == 0)
				{
					/* non set start Add */
					return(2);
				}
			break;
			/*********** Ana 1st parameter  *******************/
			case 1:
				value = HexAscii2Data_64((unsigned char*)tmp,para1st);
				if (value == 0)
				{
					*setPara |= 0x01;
				}
				else
				{
					return(1);
				}
			break;
			/*********** Ana 2nd parameter  *******************/
		 	case 2:
				value = HexAscii2Data_64((unsigned char*)tmp,&tmpData);
				if (value == 0)
				{
					*setPara |= 0x02;
					if (*para1st > tmpData)
					{
						return(2);
					}
					else
					{
						*setPara |= 0x02;
						*para2nd = (tmpData - *para1st) + 0x01 ;
				 	}
				}
				else if (value == 3)
				{
					/* find @ */
					chPtr2 = 1;
					GetStrBlk(tmp,tmp2,&chPtr2,0);
					if (HexAscii2Data_64((unsigned char*)tmp2,para2nd))
					{
						return(1);
					}
					*setPara |= 0x02;
				}
				else
				{
					return(1);
				}
			break;
		}
		if (endCh == ';')
		{
			if (GetStrBlk(gKeyBuf,tmp,&chPtr,0))
			{
				return(1);
			}
			endCh=0;
			*setPara |= 0x08;
		}
		getCnt++;
	} while(endCh);
	return(0);
}

static int32_t TPRAMCK( uint8_t *startAddr, uint8_t *endAddr )
{
	FillData8Bit(startAddr, endAddr, 0x00 );
	if (CheckAndFillData8Bit(startAddr, endAddr, 0x55, 0x00))
	{
		return ERROR_END;
	}
	if (CheckAndFillData8Bit(startAddr, endAddr, 0xAA, 0x55))
	{
		return ERROR_END;
	}
	if (CheckAndFillData8Bit(startAddr, endAddr, 0xFF, 0xAA))
	{
		return ERROR_END;
	}
	if (CheckData8Bit(startAddr, endAddr, 0xFF))
	{
		return ERROR_END;
	}
	WriteIncData8Bit(startAddr, endAddr, 0x00);
	if (CheckIncData8Bit(startAddr, endAddr, 0x00))
	{
		return ERROR_END;
	}
	return NORMAL_END;
}

void dgDdrTest(void)
{
	uint32_t readData;

	PutStr("=== DDR R/W CHECK ====",1);
	PutStr("=== RZ/Five (Memory controller is only channel 1) ===",1);
	readData = *((volatile uint32_t*)0x000000041000000);	//Access Check
	PutStr("Check:0x00_41000000 ... ",0);
	if (CkExtendDdrRamCheck((void*)0x000000041000000))
	{
		PutStr(" Fail!",1);
		PutDdrErrInfo();
		return;
	}
	else
	{
		PutStr(" Pass!",1);
	}
#if (DDR_C_011_D4_01_1 == 1)
	PutStr("Check:0x00_80000000 ... ",0);
	if (CkExtendDdrRamCheck((void*)0x000000080000000))
	{
		PutStr(" Fail!",1);
		PutDdrErrInfo();
		return;
	}
	else
	{
		PutStr(" Pass!",1);
	}
#endif
}

void dgRamTest(void)
{
	uint64_t ramck1st,ramck2nd;
	uint32_t setPara;

	char decRtn;
	char str[10];

	ramck1st=ramck2nd=0x0;
	decRtn = DecodeForm5(&ramck1st,&ramck2nd,&setPara);
	if (!(setPara&0x3))
	{
		PutStr("Syntax Error",1);	return;
	}
	else if (decRtn==1)
	{
		PutStr("Syntax Error",1);	return;
	}
	else if (decRtn==2)
	{
		PutStr("Address Size Error",1);	return;
	}
	else
	{
		PutStr("== RAM CHECK (Byte Access) ===",1);
		PutStr("- Marching Data Check --------",1);
		PutStr(" [ Write H'00               ]",1);
		PutStr(" [ Check H'00 -> Write H'55 ]",1);
		PutStr(" [ Check H'55 -> Write H'AA ]",1);
		PutStr(" [ Check H'AA -> Write H'FF ]",1);
		PutStr(" [ Check H'FF               ]",1);
		PutStr("- Decoder Pattern Check ------",1);
		PutStr(" [ Write H'00,H'01,H'02 ... ]",1);
		PutStr(" [ Check H'00,H'01,H'02 ... ]",1);
		PutStr("CHECK RESULT",0);
	}
	if (TPRAMCK( ((uint8_t *)ramck1st),((uint8_t *)(ramck1st+ramck2nd)) ) )
	{
		PutStr("---->NG",1);
		Data2HexAscii_64(gSubErrAdd,str,CPU_BYTE_SIZE);
		PutStr("ERROR ADDRESS:",0); PutStr(str,1);
		Data2HexAscii_64(gSubErrData,str,CPU_BYTE_SIZE);
		PutStr("ERROR DATA   :",0); PutStr(str,1);
		Data2HexAscii_64(gSubTrueData,str,CPU_BYTE_SIZE);
		PutStr("TRUE DATA    :",0); PutStr(str,1);
		return;
	}
	else
	{
		PutStr("---->OK",1);
	}
}
