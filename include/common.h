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

#ifndef COMMON_H
#define COMMON_H

#include <stdint-gcc.h>

typedef uint64_t    uintptr_t;
#define CPU_BYTE_SIZE			SIZE_64BIT

#define		DIS_RTN			0		/* Disable Return		*/
#define		ENB_RTN			1		/* Enable Return		*/
#define		OK			0x1

#ifndef NULL
#define		NULL			0x0
#endif

#define		INT_CODE		0x25		/* "%"					*/
#define		BS_CODE			0x08		/* "BS"					*/
#define		CR_CODE			0x0d		/* "CR"					*/
#define		SP_CODE			0x20		/* "LF"					*/
#define		LF_CODE			0x0a		/* "LF"					*/

#define		SIZE_8BIT		1		// Old name : BYTE_SIZE
#define		SIZE_16BIT		2		// Old name : WORD_SIZE
#define		SIZE_32BIT		4		// Old name : LONG_SIZE
#define		SIZE_64BIT		8		// New

#define		COMMAND_BUFFER_SIZE	1024


/********************************
	Module Proto Type	*
*********************************/
int32_t	PutStr(const char *str,char rtn);
int32_t	GetStr(char *str,char *chCnt);
uint32_t Hex2Ascii(int32_t hexdata,char *str,int32_t *chcnt);
uint32_t Hex2DecAscii(int32_t hexdata,char *str,int32_t *chcnt);
void DelStr(int32_t delCnt);
void ChgLtl2Lrg(char *str);
char HexAscii2Data(unsigned char *buf,uint32_t *data);
char HexAscii2Data_64(unsigned char *buf,uintptr_t *data);
char Data2HexAscii(uint32_t data,char *buf,char size);
char Data2HexAscii_64(uintptr_t data,char *buf,char size);
char GetStrBlk(char *inStr,char *outStr, char *chPtr,char method);
void SoftDelay(uint32_t roop);
void WaitKeyIn_Y(void);
char WaitKeyIn_YorN(void);

#endif
