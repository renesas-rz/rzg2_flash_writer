/*
 * Copyright (c) 2015-2021, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "common.h"
#include "dgtable.h"
#include "memory_cmd.h"
#include "devdrv.h"
#include "devdrv.h"
#include "bit.h"
#include "cpudrv.h"
#include "scifdrv.h"

extern const char *const AllHelpMess[ALL_HELP_MESS_LINE];
extern const com_menu MonCom[COMMAND_UNIT];
extern int32_t  gComNo;
extern char gKeyBuf[64];

extern uintptr_t gUDump[3];
extern uintptr_t gUMem[3];
extern uint32_t gDumpMode;
extern uint32_t gDumpStatus;

static void ChgDumpAsciiCode(uintptr_t chCode, char *buf, char chPtr, uint32_t width)
{
	unsigned char tmpBCode,i;
	uintptr_t tmpLCode;

	switch(width)
	{
		case SIZE_8BIT:
			tmpLCode = 0xFF & chCode;
			tmpBCode = (unsigned char)tmpLCode;
			if ((0x20 <= tmpBCode) && (tmpBCode <= 0x7E))
			{
				*((unsigned char *)(buf+chPtr)) = tmpBCode;
			}
			else
			{
				*((unsigned char *)(buf+chPtr)) = '.';
			}
		break;
		case SIZE_16BIT:
			for (i = 0; i < width; i++)
			{
				tmpLCode = chCode;
				tmpLCode = 0xFF & (tmpLCode>>(8-(i*8)));
				tmpBCode = (unsigned char)tmpLCode;
				if ((0x20 <= tmpBCode) && (tmpBCode <= 0x7E))
				{
					*((unsigned char *)(buf+i+chPtr)) = tmpBCode;
				}
				else
				{
					*((unsigned char *)(buf+i+chPtr)) = '.';
				}
			}
		break;
		case SIZE_32BIT:
			for (i = 0; i < width; i++)
			{
				tmpLCode = chCode;
				tmpLCode = 0xFF & (tmpLCode>>(24-(i*8)));
				tmpBCode = (unsigned char)tmpLCode;
				if ((0x20 <= tmpBCode) && (tmpBCode <= 0x7E))
				{
					*((unsigned char *)(buf+i+chPtr)) = tmpBCode;
				}
				else
				{
					*((unsigned char *)(buf+i+chPtr)) = '.';
				}
			}
		break;
		case SIZE_64BIT:
			for (i = 0; i < width; i++)
			{
				tmpLCode = chCode;
				tmpLCode = 0xFF & (tmpLCode>>(56-(i*8)));
				tmpBCode = (unsigned char)tmpLCode;
				if ((0x20 <= tmpBCode) && (tmpBCode <= 0x7E))
				{
					*((unsigned char *)(buf+i+chPtr)) = tmpBCode;
				}
				else
				{
					*((unsigned char *)(buf+i+chPtr)) = '.';
				}
			}
		break;
		default:
		break;
	}
}

static void ChgDumpAsciiStr(char *buf)
{
	unsigned char i;
	buf[18] = 0;
	buf[17] = 0x22;
	for (i = 16; i > 0; i--)
	{
		buf[i] = buf[i-1];
	}
	buf[0] = 0x22;
}

static void Get1ByteMem(uintptr_t readAdd, uintptr_t *readData, uint32_t width)
{
	unsigned char tmpB;
	uint16_t tmpW;
	uint32_t tmpL;
	uint64_t tmpX;
	
	switch(width)
	{
		case SIZE_8BIT:
			tmpB = *((volatile uint8_t *)readAdd);
			*readData = (uintptr_t)tmpB;
		break;
		case SIZE_16BIT:
			tmpW = *((volatile uint16_t *)readAdd);
			*readData = (uintptr_t)tmpW;
		break;
		case SIZE_32BIT:
			tmpL = *((volatile uint32_t *)readAdd);
			*readData = (uintptr_t)tmpL;
		break;
		case SIZE_64BIT:
			tmpX = *((volatile uint64_t *)readAdd);
			*readData = (uintptr_t)tmpX;
		break;
		default:
		break;
	}
}

static void Put1ByteMem(uintptr_t writeAdd, uintptr_t writeData, uint32_t width)
{
	uint8_t tmpB;
	uint16_t tmpW;
	uint32_t tmpL;

	switch(width)
	{
		case SIZE_8BIT:
			tmpB = (uint8_t)writeData;
			*((volatile uint8_t *)writeAdd) = tmpB;
		break;
		case SIZE_16BIT:
			tmpW = (uint16_t)writeData;
			*((volatile uint16_t *)writeAdd) = tmpW;
		break;
		case SIZE_32BIT:
			tmpL = (uint32_t)writeData;
			*((volatile uint32_t *)writeAdd) = tmpL;
		break;
		case SIZE_64BIT:
			*((uint64_t *)writeAdd) = writeData;
		break;
		break;
	}
}

static void DisplayDump(uintptr_t stAdd, uintptr_t dumpSize, uint32_t width)
{
	char buf[32], asciiBuf[20];
	uintptr_t readAdd, readData;
	uint32_t  blankCnt, byte_count;

	byte_count = 0x0;

	/********* Dump Data 1Line All Display *********************/
	if (!((dumpSize&0x0000000F) == 0x0000000F))
	{
		dumpSize = dumpSize | 0x0000000F;
	}
	for (readAdd = stAdd; readAdd <= (stAdd+dumpSize); readAdd += width)
	{
		/********* Dump Address Display ************************/
		if ((!(byte_count & 0x0000000F)) || (byte_count == 0))
		{
			Data2HexAscii_64(readAdd,buf,CPU_BYTE_SIZE);
			PutStr(buf, 0);
			PutStr("  ", 0);
		}
		Get1ByteMem(readAdd, &readData, width);
		Data2HexAscii_64(readData, buf, (char)width);
		ChgDumpAsciiCode(readData, asciiBuf, (char)(byte_count & 0xF), (char)width);
		/********* Dump Data Display blank ********************/
		if (!(byte_count & 0x8))
		{
			PutStr(buf, 0);
			for (blankCnt = 0; blankCnt < width; blankCnt++)
			{
				PutStr(" ",0);
			}
		}
		else
		{
			for (blankCnt = 0; blankCnt < width; blankCnt++)
			{
				PutStr(" ",0);
			}
			PutStr(buf, 0);
		}
		if (((width == SIZE_8BIT) && ((byte_count & 0xF) ==0xF)) ||
			((width == SIZE_16BIT) && ((byte_count & 0xF) ==0xE)) ||
			((width == SIZE_32BIT) && ((byte_count & 0xF) ==0xC)) ||
			((width == SIZE_64BIT) && ((byte_count & 0xF) ==0x8)))
		{
			ChgDumpAsciiStr(asciiBuf);
			PutStr("  ", 0);
			PutStr(asciiBuf, 1);
		}
		byte_count += width;
	}
}

static char CheckAccessSize(char *buf, uint32_t *sizeId)
{
	ChgLtl2Lrg(buf);
	if ((*buf == 'B') && (*(buf+1) == 0))
	{
		*sizeId = SIZE_8BIT;
	}
	else if ((*buf == 'W') && (*(buf+1) == 0))
	{
		*sizeId = SIZE_16BIT;
	}
	else if ((*buf == 'L') && (*(buf+1) == 0))
	{
		*sizeId = SIZE_32BIT;
	}
	else if ((*buf == 'X') && (*(buf+1) == 0))
	{
		*sizeId = SIZE_64BIT;
	}
	else
	{
		return(1);
	}
	return(0);
}

static void DisplayMemEd(uintptr_t mem1st, uint32_t width, uint32_t verifyFlg)
{

	uintptr_t readData, wrData;
	char mEnd, buf[32], key[32], chCnt, chPtr;

	mEnd = 0;
	while(!mEnd)
	{
		Data2HexAscii_64(mem1st, buf, CPU_BYTE_SIZE);
		PutStr(buf, 0);
		PutStr(" ", 0);
		Get1ByteMem(mem1st, &readData, width);
		Data2HexAscii_64(readData, buf, (char)width);
		PutStr(buf, 0);
		PutStr(" ? ", 0);
		GetStr_MemEd(key, &chCnt);
		chPtr = 0;
		if (!GetStrBlk(key, buf, &chPtr, 0))
		{
			if (chPtr == 1)
			{
				/* Case Return */
				mem1st += width;
			}
			else if ((buf[0] == '.'))
			{
				/* Case End */
				mEnd = 1;
			}
			else if ((buf[0] == '^'))
			{
				/* Case ^ */
				mem1st -= width;
			}
			else if (chPtr > (char)((width<<1)+1))
			{
				/* Case Data Size Over */
				PutStr("Syntax Error", 1);
			}
			else
			{
				if (HexAscii2Data_64((unsigned char*)buf, &wrData))
				{
					PutStr("Syntax Error", 1);
				}
				else
				{
					/* Case Mem Edit  */
					Put1ByteMem(mem1st, wrData, width);
					
					if (verifyFlg == VERIFY_ON)
					{
						Get1ByteMem(mem1st, &readData, width);
						if (wrData != readData)	
						{
							PutStr("Verify Error", 1);
						}
					}
					mem1st += width;
				}
			}
		}
		else
		{
			PutStr("Syntax Error",1);
		}
	}
	gUMem[0] = mem1st;
}

static void dgMemEdit(uint32_t width)
{

	uint32_t  verifyFlg;
	uintptr_t mem1st;
	char decRtn;

//	verifyFlg = VERIFY_ON;
	verifyFlg = VERIFY_OFF;

	mem1st = gUMem[0];	/* Start Address */
	decRtn = DecodeForm02(&mem1st);

	if (decRtn == 1)
	{
		PutStr("Syntax Error", 1);
	}
	else if(decRtn == 0)
	{
		switch(width)
		{
			case SIZE_16BIT:
				if (mem1st & 0x00000001)
				{
					PutStr("Memory Boundary Error", 1);
					return;
				}
			break;
			case SIZE_32BIT:
				if (mem1st & 0x00000003)
				{
					PutStr("Memory Boundary Error", 1);
					return;
				}
			break;
			case SIZE_64BIT:
				if (mem1st & 0x00000007)
				{
					PutStr("Memory Boundary Error", 1);
					return;
				}
			break;
			default:
			break;
		}
		DisplayMemEd(mem1st, width, verifyFlg);
		gUMem[0] = gUMem[0] + width;	/* Start Address */
	}
}

/****************************************************************
	MODULE			: dgDump 			*
	FUNCTION		: Memory Dump			*
	COMMAND			: D				*
	INPUT PARAMETER		: D  {sadr {eadr}}  		*
*****************************************************************/
void	dgDump(void)
{
	uintptr_t dmp1st, dmp2nd;
	char decRtn;

	gDumpStatus = ENABLE;

	/* Init Dump Parameter */
	dmp1st = gUDump[0];	/* Start Address */
	dmp2nd = gUDump[1];	/* End   Address */
	decRtn = DecodeForm01(&dmp1st, &dmp2nd);	/* format check */

	/* dmp1st: Start Address */
	/* dmp2nd: dumpSize      */
	switch(gDumpMode)
	{
		case SIZE_16BIT:
			if (dmp1st & 0x00000001)
			{
				PutStr("Memory Boundary Error", 1);
				return;
			}
		break;
		case SIZE_32BIT:
			if (dmp1st & 0x00000003)
			{
				PutStr("Memory Boundary Error", 1);
				return;
			}
		break;
		case SIZE_64BIT:
			if (dmp1st & 0x00000007)
			{
				PutStr("Memory Boundary Error", 1);
				return;
			}
		break;
		default:
		break;
	}
	if (decRtn == 1)
	{
		PutStr("Syntax Error", 1);
	}
	if (decRtn == 2)
	{
		PutStr("Address Size Error", 1);
	}
	if (decRtn == 0)
	{
		DisplayDump(dmp1st, dmp2nd, gDumpMode);
		dmp2nd=dmp2nd | 0x0000000F;		/* Start line adjustment			*/
		gUDump[0] = dmp1st+dmp2nd+1;		/* Start Address				*/
		gUDump[1] = 255;			/* Dump Size (Byte) Return to initial value	*/
	}
}
/****************************************************************
	MODULE			: dgDumpMode			*
	FUNCTION		: set&disp dump mode		*
	COMMAND			: DM				*
	INPUT PARAMETER		: DM {B|W|L}  			*
*****************************************************************/
void	dgDumpMode(void)
{
	uint32_t setPara;
	char tmp[64], chPtr;
	char endCh;

	chPtr = 0;
	setPara = gDumpMode;

	/*DM         (Display DumpMode) */
	endCh = GetStrBlk(gKeyBuf, tmp, &chPtr, 0);
	if (endCh == 0)
	{
		// When nothing other than DM is entered
		PutStr("DMmode = ", 0);
		switch(gDumpMode)
		{
			case SIZE_8BIT:	
				PutStr("byte", 1);
			break;
			case SIZE_16BIT:
				PutStr("word", 1);
			break;
			case SIZE_32BIT:
				PutStr("long", 1);
			break;
			case SIZE_64BIT:
				PutStr("long long", 1);
			break;
			default:
			break;
		}
	}
	/*DM <size> (Set DumpMode Size)*/
	else
	{
		/*********** Ana 2nd parameter  check ****************/
		GetStrBlk(gKeyBuf, tmp, &chPtr, 0);
		if (CheckAccessSize(tmp, &setPara))
		{
			PutStr("Syntax Error", 1);
		}
		gDumpMode=setPara;
	}
}

/****************************************************************
	MODULE			: dgMemEdit_byte		*
	FUNCTION		: set memory(BYTE)		*
	COMMAND			: M				*
	INPUT PARAMETER		: M  [adr]  			*
*****************************************************************/
void	dgMemEdit_byte(void)
{
	dgMemEdit(SIZE_8BIT);
}

/****************************************************************
	MODULE			: dgMemEdit_word		*
	FUNCTION		: set memory(WORD)		*
	COMMAND			: MW				*
	INPUT PARAMETER		: MW  [adr]  			*
*****************************************************************/
void	dgMemEdit_word(void)
{
	dgMemEdit(SIZE_16BIT);
}

/****************************************************************
	MODULE			: dgMemEdit_long		*
	FUNCTION		: set memory(LONG)		*
	COMMAND			: ML				*
	INPUT PARAMETER		: ML  [adr]  			*
*****************************************************************/
void	dgMemEdit_long(void)
{
	dgMemEdit(SIZE_32BIT);
}

/****************************************************************
	MODULE			: dgMemEdit_long_long		*
	FUNCTION		: set memory(LONG LONG)		*
	COMMAND			: MX				*
	INPUT PARAMETER		: MX  [adr]  			*
*****************************************************************/
void	dgMemEdit_longlong(void)
{
	dgMemEdit(SIZE_64BIT);
}


/****************************************************************
	FUNCTION			: fill memory		*
*****************************************************************/
static void	dgFill(uint32_t width)
{
	uintptr_t fill1st, fill2nd, fill3rd, wrAdd;
	uint32_t  setPara;
	char decRtn;

	/* fill1st: Start Address */
	/* fill2nd: FillSize      */
	/* fill3rd: FillData      */
	decRtn = DecodeForm03(&fill1st, &fill2nd, &fill3rd, &setPara);
	if (!(setPara == 0x03))
	{
		if (decRtn == 1)
		{
			PutStr("Syntax Error",1);
		}
		else if (decRtn == 2)
		{
			PutStr("Address Size Error",1);
		}
		else
		{
			PutStr("Syntax Error",1);
		}
	}
	else
	{
		if (width == SIZE_32BIT)
		{
			if (fill1st & 0x00000003)
			{
				PutStr("Memory Boundary Error",1);
				return;
			}
		}
		if (width == SIZE_64BIT)
		{
			if (fill1st & 0x00000007)
			{
				PutStr("Memory Boundary Error",1);
				return;
			}
		}
		for (wrAdd = fill1st; wrAdd <= (fill1st + fill2nd); wrAdd += width)
		{
			Put1ByteMem(wrAdd, fill3rd, width);
		}
	}
}

/****************************************************************
	MODULE			: dgFill_byte			*
	FUNCTION		: fill memory (Byte)		*
	COMMAND			: F				*
	INPUT PARAMETER		: F  [sadr] [eadr] [data]  	*
*****************************************************************/
void	dgFill_byte(void)
{
	dgFill(SIZE_8BIT);
}
/****************************************************************
	MODULE			: dgFill_long			*
	FUNCTION		: fill memory (LONG)		*
	COMMAND			: FL				*
	INPUT PARAMETER		: FL  [sadr] [eadr] [data]  	*
*****************************************************************/
void	dgFill_long(void)
{
	dgFill(SIZE_32BIT);
}
/****************************************************************
	MODULE			: dgFill_longlong		*
	FUNCTION		: fill memory (LONG LONG)	*
	COMMAND			: FX				*
	INPUT PARAMETER		: FX  [sadr] [eadr] [data]  	*
*****************************************************************/
void	dgFill_longlong(void)
{
	dgFill(SIZE_64BIT);
}
