/*
 * Copyright (c) 2015-2021, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stddef.h>
#include "types.h"
#include "common.h"
#include "devdrv.h"

__attribute__((aligned(32))) uint8_t	gCOMMAND_Area[COMMAND_BUFFER_SIZE];
extern char gKeyBuf[64];

/************************************************************************/
/*NAME		: PutMes						*/
/************************************************************************/
int32_t PutMess(const char *const mess[])
{
	int32_t i = 0;
	while(mess[i])
	{
		PutStr(mess[i], ENB_RTN);
		i++;
	}
	return(0);
}

/************************************************************************/
/*NAME		: PutStr						*/
/************************************************************************/
int32_t	PutStr(const char *str, char rtn)
{
	while(*str)
	{
		PutChar(*str);
		str++;
	}
	if (rtn == 1)
	{
		PutChar(CR_CODE);
		PutChar(LF_CODE);
	}
	return(0);

}

/************************************************************************/
/*NAME		: GetStr						*/
/************************************************************************/
int32_t	GetStr(char *str, char *chCnt)
{
	char *intstr;
	int32_t i;

	intstr = str;

	while(1)
	{
		i = 1;
		while(i == 1)
		{
			i = GetChar(str);
		}
		if (*str == CR_CODE)
		{
			break;
		}
		switch(*str)
		{
			case BS_CODE:
				if (*chCnt == 0)
				{
					break;
				}
				else
				{
					PutChar(BS_CODE);
					PutChar(SP_CODE);
					PutChar(BS_CODE);
					*chCnt = *chCnt - 1;
					str--;
				}
			break;
			case LF_CODE:
			break;
			default:
				PutChar(*str);
				str++;
				*chCnt = *chCnt + 1;
			break;
		}
	}
	*str = 0;
	PutChar(LF_CODE);
	PutChar(CR_CODE);
	return(0);

}

/************************************************************************/
/*NAME		: Hex2Ascii						*/
/************************************************************************/
uint32_t Hex2Ascii(int32_t hexdata, char *str, int32_t *chcnt)
{
	long    i;
	char    ch;

	for (i = 7; i >= 0; i-- )
	{
		ch = (char)(hexdata & 0x0F);
		if (ch > 9)
		{
			ch += 7;
		}
		ch += 0x30;
		hexdata >>= 4;
		*(str + i) = ch;
	}
	*(str + 8) = '\0';
	*chcnt = 8;
	return 0U;
}


uint32_t Hex2DecAscii(int32_t hexdata, char *str, int32_t *chcnt)
{
	char Count;
	char countstart;
	uint32_t Compdata;

	unsigned char dataB;
	uint32_t dataL;
	uint32_t i;

	Count = 0;
	countstart = 0x0;
	dataB = 0;
	dataL = 10;
	while(dataL > 0)
	{
		Compdata = 1;
		i = 1;
		while(i < dataL)
		{
			Compdata *= 10;
			i++;
		}
		while(hexdata >= Compdata)
		{
			hexdata -= Compdata;
			dataB++;
			countstart = 1;
		}
		if (countstart == 1)
		{
			*(str++) = dataB + '0';
			Count++;
		}
		dataB = 0;
		dataL--;
	}
	if (Count == 0)
	{
		*str = '0';
		str++;
		Count++;
	}
	*str = '\0';
	*chcnt = Count;
	return(0);

}

/************************************************************************/
void DelStr(int32_t delCnt)
{
	int32_t i, j;
	for (i = 0; i <delCnt; i++)
	{
		PutChar(BS_CODE);
		for (j = 0 ;j < 100; j++);
	}
	for (i = 0; i <delCnt; i++)
	{
		PutChar(' ');
		for (j = 0; j < 100; j++);
	}
	for (i = 0; i <delCnt; i++)
	{
		PutChar(BS_CODE);
		for (j = 0; j < 100; j++);
	}
}

void ChgLtl2Lrg(char *str)
{
	while(*str != 0)
	{
		if (('a' <= *str) && (*str <= 'z'))
		{
			*str -= 0x20;
		}
		str++;
	}
}
char HexAscii2Data(unsigned char *buf, uint32_t *data)
{
	char chCnt;
	uint32_t tmpData;
	*data = 0;
	chCnt = 0;

	ChgLtl2Lrg(buf);

	if (*buf == '@')
	{
		return(3);
	}
	while(*buf)
	{
		if (('0' <= *buf) && (*buf <= '9'))
		{
			tmpData = (uint32_t)(*buf - '0');
			*data <<= 4;
			*data |= tmpData;
		}
		else if (('A' <= *buf) && (*buf <= 'F')
		){
			tmpData = (uint32_t)(*buf - 55);
			*data <<= 4;
			*data |= tmpData;
		}
		else
		{
			return(1);
		}
		buf++;
		chCnt++;
		if (chCnt > (SIZE_32BIT*2))
		{
			return(1);
		}
	}
	return(0);
}

char Data2HexAscii(uint32_t data, char *buf, char size)
{
	char loopCnt, i;
	uint32_t tmpData;
	switch(size)
	{
		case SIZE_8BIT:
			data <<= (SIZE_32BIT*8-8);
			loopCnt = 2;
		break;
		case SIZE_16BIT:
			data <<= (SIZE_32BIT*8-16);
			loopCnt = 4;
		break;
		case SIZE_32BIT:
			data <<= (SIZE_32BIT*8-32);
			loopCnt = 8;
		break;
		default:
		break;
	}
	for (i = 0; i < loopCnt; i++, buf++)
	{
		tmpData = (data >> (SIZE_32BIT*8-4));
		if (tmpData < 0x0a)
		{
			/* case 1 to 9 */
			*buf = (char)(tmpData + '0');
		}
		else
		{
			/* case A to F */
			*buf = (char)(tmpData + 55);
		}
		data <<= 4;
	}
	*buf = 0;
	return(0);
}

char GetStrBlk(char *inStr, char *outStr, char *chPtr, char method)
{
	char serchEnd, frstSpace, endCh;
	inStr = inStr + *chPtr;
	serchEnd  = 0;
	frstSpace = 0;
	while(!serchEnd)
	{
		switch(*inStr)
		{
			case ' ':   /* space Code */
				if (frstSpace)
				{
					if ((method == 0) || (method == ' '))
					{
						serchEnd = 1;
						*outStr  = 0;
						endCh = ' ';
					}
				}
				break;
			case 0:
				serchEnd = 1;
				*outStr = 0;
				inStr++;
				endCh = 0;
				break;
			default:
				frstSpace = 1;
				*outStr = *inStr;
				outStr++;
				break;
		}
		*chPtr += 1;
		inStr++;
	}
	return(endCh);
}

void SoftDelay(uint32_t loop)
{
	volatile uint32_t i;
	for (i = 0; i < loop; i++);
}

void WaitKeyIn_Y(void)
{
	uint32_t loop;
	char tmp;

	loop = 1;
	while(loop)
	{
		GetChar(&tmp);
		if ((tmp == 'y') || (tmp == 'Y'))
		{
			loop = 0;
		}
	}
}

char WaitKeyIn_YorN(void)
{
	char tmp;

	while(1)
	{
		GetChar(&tmp);
		if ((tmp == 'y') || (tmp == 'Y'))
		{
			return(0);
		}
		else if ((tmp == 'n') || (tmp == 'N'))
		{
			return(1);
		}
	}
}

void *memset(void *dst, int val, unsigned long count)
{
	char *ptr = dst;

	while (count--)
	{
		*ptr++ = val;
	}
	return dst;
}

void *memcpy(void *dst, const void *src, size_t len)
{
	const char *s = src;
	char *d = dst;

	while (len--)
	{
		*d++ = *s++;
	}
	return dst;
}

/*********************************************************/
/* Hex Ascii str -> HexData                              */
/*********************************************************/
char DecodeHexAscStr(uintptr_t *para, char *buf)
{
	char value;
	uintptr_t tmpData;

	value = HexAscii2Data_64((unsigned char*)buf, &tmpData);
	if (value == 1)
	{
		/* Error */
		return(1);
	}
	else if (value == 0)
	{
		*para = tmpData;
	}
	return(0);	/* Normal End   */
}

/*********************************************************/
/* Hex Ascii str -> HexData (para1st, para2nd)           */
/*********************************************************/
char DecodeHexAscStr2(uintptr_t *para1st, uintptr_t *para2nd, char *buf)
{
	char value;
	uintptr_t tmpData;

	value = HexAscii2Data_64((unsigned char*)buf, &tmpData);
	if (value == 1)
	{
		/* Error */
		return(1);
	}
	else if (value == 0)
	{
		if (tmpData < *para1st)
		{
			return(2);	/* Error(Address Size Error) */
		}
		else
		{
			*para2nd = tmpData - *para1st;
		}
	}
	return(0);	/* Normal End   */
}

/***********************************************************/
/*        Decode Format01                                  */
/* COM <start Add>[ (<End Add>)] (RET)                     */
/*        Dump,                                            */
/***********************************************************/
char DecodeForm01(uintptr_t *para1st, uintptr_t *para2nd)
{
	char tmp[64], chPtr, endCh, getCnt, value;

	chPtr= getCnt = 0;
	do
	{
		endCh =GetStrBlk(gKeyBuf, tmp, &chPtr, 0);
		switch(getCnt)
		{
			/*********** Ana 1st parameter <start Add> **********/
			case 1:
				if (DecodeHexAscStr(para1st, tmp) ==1)
				{
					return(1);	/* Syntax Error */
				}
			break;
			/*********** Ana 2nd parameter <End Add>   **********/
			case 2:
				if (value = DecodeHexAscStr2(para1st, para2nd,tmp))
				{
					return(value);	/* 1:Syntax Error  2:Address Size Error */
				}
			break;
		}
		getCnt++;
	} while(endCh);
	return(0);	/* Normal End   */
}

/***********************************************************/
/*        Decode Format02                                  */
/* COM <start Add>  (RET)                                  */
/*        ME,                                              */
/***********************************************************/
char DecodeForm02(uintptr_t *para1st)
{
	char tmp[64], chPtr, endCh, getCnt, value;

	chPtr = getCnt = 0;
	do
	{
		endCh = GetStrBlk(gKeyBuf, tmp, &chPtr, 0);
		switch(getCnt)
		{
			/*********** "ME" Block parameter  ****************/
			case 0:
				if (endCh == 0)
				{
					/* non set start Add = Error */
					return(1);
				}
			break;
			/*********** Ana 1st parameter <start Add> ********/
			case 1:
				if (DecodeHexAscStr(para1st, tmp) == 1)
				{
					return(1);	/* Syntax Error */
				}
			break;
		}
		getCnt++;
	} while(endCh);
	return(0);
}

/***********************************************************/
/*        Decode Format03                                  */
/* COM <start Add> <End Add> <data>(RET)                   */
/*        Fill	                                           */
/***********************************************************/
char DecodeForm03(uintptr_t *para1st, uintptr_t *para2nd, uintptr_t *para3rd, uint32_t *setPara)
{
	char tmp[64], chPtr,endCh, getCnt, value;

	*setPara = 0;
	chPtr= getCnt = 0;
	do
	{
		endCh = GetStrBlk(gKeyBuf, tmp, &chPtr, 0);
		switch(getCnt)
		{
			/*********** "F"Command  Block parameter  ****************/
			case 0:
				if (endCh == 0)
				{
					/* non set start Add = Error */
					return(1);
				}
			break;
			/*********** Ana 1st parameter <start Add> **********/
			case 1:
				if (DecodeHexAscStr(para1st, tmp) == 1)
				{
					return(1);	/* Syntax Error */
				}
				else
				{
					*setPara = 0x01;
				}
			break;
			/*********** Ana 2nd parameter <End Add>   **********/
			 case 2:
				if (value = DecodeHexAscStr2(para1st, para2nd,tmp))
				{
					return(value);	/* 1:Syntax Error  2:Address Size Error */
				}
				else
				{
					*setPara = 0x02;
				}
			break;
			/*********** Ana 3rd parameter <data> ***************/
			case 3:
				if (DecodeHexAscStr(para3rd, tmp) == 1)
				{
					return(1);	/* Syntax Error */
				}
				else
				{
					*setPara = 0x03;
				}
			break;
			default:
			break;
		}
		getCnt++;
	} while(endCh);
	return(0);
}

/************************************************************************/
/*NAME		: GetStr_MemEd						*/
/************************************************************************/
int32_t	GetStr_MemEd(char *str, char *chCnt)
{
	char *intstr;
	int32_t i;

	intstr = str;
	*chCnt = 0;

	while(1)
	{
		i = 1;
		while(i == 1)
		{
			i = GetChar(str);
		}
		if (*str == CR_CODE)
		{
			break;
		}
		switch(*str)
		{
			case BS_CODE:
				if (*chCnt == 0)
				{
					break;
				}
				else
				{
					PutChar(BS_CODE);
					PutChar(SP_CODE);
					PutChar(BS_CODE);
					*chCnt = *chCnt - 1;
					str--;
				}
			break;
			case LF_CODE:
			break;
			default:
				PutChar(*str);
				str++;
				*chCnt = *chCnt + 1;
			break;
		}
		if (*intstr == '.')
		{
			break;
		}
		else if (*intstr == '^')
		{
			break;
		}
	}
	*str = 0;
	PutChar(LF_CODE);
	PutChar(CR_CODE);
	return(0);
}

char HexAscii2Data_64(unsigned char *buf, uintptr_t *data)
{
	char chCnt;
	uintptr_t tmpData;
	*data = 0;
	chCnt = 0;

	ChgLtl2Lrg(buf);

	if (*buf == '@')
	{
		return(3);
	}
	while(*buf)
	{
		if (('0' <= *buf) && (*buf <= '9'))
		{
			tmpData = (uintptr_t)(*buf - '0');
			*data <<= 4;
			*data |= tmpData;
		}
		else if (('A' <= *buf) && (*buf <= 'F'))
		{
			tmpData = (uintptr_t)(*buf - 55);
			*data <<= 4;
			*data |= tmpData;
		}
		else
		{
			return(1);
		}
		buf++;
		chCnt++;
		if (chCnt > (CPU_BYTE_SIZE*2))
		{
			return(1);
		}
	}
	return(0);
}

char Data2HexAscii_64(uintptr_t data, char *buf, char size)
{
	char loopCnt, i;
	uintptr_t tmpData;
	switch(size)
	{
		case SIZE_8BIT:
			data <<= (CPU_BYTE_SIZE*8-8);
			loopCnt = 2;
		break;
		case SIZE_16BIT:
			data <<= (CPU_BYTE_SIZE*8-16);
			loopCnt = 4;
		break;
		case SIZE_32BIT:
			data <<= (CPU_BYTE_SIZE*8-32);
			loopCnt = 8;
		break;
#if defined(AArch64) || defined (RISCV)
		case SIZE_64BIT:
			data <<= (CPU_BYTE_SIZE*8-64);
			loopCnt = 16;
		break;
#endif
		default:
		break;
	}
	for ( i= 0; i < loopCnt; i++ ,buf++)
	{
		tmpData = (data >> (CPU_BYTE_SIZE*8-4));
		if (tmpData < 0x0a)
		{
			/* case 1 to 9 */
			*buf = (char)(tmpData + '0');
		}
		else
		{
			/* case A to F */
			*buf = (char)(tmpData + 55);
		}
		data <<= 4;
	}
	*buf = 0;
	return(0);
}
