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

#include <stddef.h>
#include "types.h"
#include "common.h"
#include "devdrv.h"

__attribute__((aligned(32))) uint8_t	gCOMMAND_Area[COMMAND_BUFFER_SIZE];

/************************************************************************/
/*NAME		: PutStr						*/
/************************************************************************/
int32_t PutStr(const char *str,char rtn)
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
int32_t GetStr(char *str,char *chCnt)
{
	char	*intstr;
	int32_t	i;

	intstr = str;
	*chCnt = 0;

	while(1)
	{
		i=1;
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
				*chCnt = *chCnt+1;
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
uint32_t Hex2Ascii(int32_t hexdata,char *str,int32_t *chcnt)
{
	long    i;
	char    ch;

	for( i = 7; i >= 0; i-- )
	{
		ch = (char)(hexdata & 0x0F);
		if ( ch > 9 )
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


uint32_t Hex2DecAscii(int32_t hexdata,char *str,int32_t *chcnt)
{
	char Count;
	char countstart;
	uint32_t Compdata;

	unsigned char dataB;
	uint32_t dataL;
	uint32_t i;

	Count		= 0;
	countstart	= 0x0;
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
	int32_t i,j;

	for(i = 0; i < delCnt; i++)
	{
		PutChar(BS_CODE);
		for(j = 0; j < 100; j++);
	}
	for(i = 0; i < delCnt; i++)
	{
		PutChar(' ');
		for(j = 0; j < 100; j++);
	}
	for(i = 0; i < delCnt; i++)
	{
		PutChar(BS_CODE);
		for(j = 0; j < 100; j++);
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

char HexAscii2Data(unsigned char *buf,uint32_t *data)
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
		else if (('A'<= *buf) && (*buf<='F'))
		{
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

char HexAscii2Data_64(unsigned char *buf,uintptr_t *data)
{
	char chCnt;
	uintptr_t tmpData;

	*data = 0;
	chCnt = 0;

	ChgLtl2Lrg(buf);

	if (*buf=='@')
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
		else if (('A'<= *buf)&&(*buf<='F'))
		{
			tmpData = (uintptr_t)(*buf - 55);
			*data <<= 4;
			*data |= tmpData;
		}
		else
		{
			return(1);
		}
		buf++; chCnt++;
		if (chCnt>(CPU_BYTE_SIZE*2))
		{
			return(1);
		}
	}
	return(0);
}

char Data2HexAscii(uint32_t data,char *buf,char size)
{
	char loopCnt,i;
	uint32_t tmpData;
	switch(size)
	{
		case SIZE_8BIT:
			data <<= (SIZE_32BIT*8-8); loopCnt=2;
		break;
		case SIZE_16BIT:
			data <<= (SIZE_32BIT*8-16); loopCnt=4;
		break;
		case SIZE_32BIT:
			data <<= (SIZE_32BIT*8-32); loopCnt=8;
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

char Data2HexAscii_64(uintptr_t data,char *buf,char size)
{
	char loopCnt, i;
	uintptr_t tmpData;
	switch(size)
	{
		case SIZE_8BIT:
			data <<= (CPU_BYTE_SIZE*8-8); loopCnt=2;
		break;
		case SIZE_16BIT:
			data <<= (CPU_BYTE_SIZE*8-16); loopCnt=4;
		break;
		case SIZE_32BIT:
			data <<= (CPU_BYTE_SIZE*8-32); loopCnt=8;
		break;
		case SIZE_64BIT:
			data <<= (CPU_BYTE_SIZE*8-64); loopCnt=16;
		break;
	}
	for (i = 0; i < loopCnt; i++, buf++)
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

char GetStrBlk(char *inStr,char *outStr,char *chPtr,char method)
{
	char serchEnd,frstSpace,endCh;
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
						endCh    = ' ';
					}
				}
			break;
			case 0:
				serchEnd = 1;
				*outStr  = 0;
				inStr++;
				endCh    = 0;
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
	uint32_t i;

	for(i = 0; i < loop; i++);
}

void WaitKeyIn_Y(void)
{
	uint32_t loop;
	char tmp;

	loop=1;
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
