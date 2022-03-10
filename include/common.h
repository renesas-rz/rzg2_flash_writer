/*
 * Copyright (c) 2015-2021, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef COMMON_H
#define COMMON_H

#include <stdint.h>

#ifdef AArch64
//typedef uint64_t    uintptr_t;
#define CPU_BYTE_SIZE			SIZE_64BIT
#endif

#ifdef RISCV
//typedef uint64_t    uintptr_t;
#define CPU_BYTE_SIZE			SIZE_64BIT
#endif

#ifdef AArch32
//typedef uint32_t    uintptr_t;
#define CPU_BYTE_SIZE			SIZE_32BIT
#endif


#define DIS_RTN			0		/* Disable Return		*/
#define ENB_RTN			1		/* Enable Return		*/
#define OK			0x1

#ifndef NULL
#define NULL			 0x0
#endif

#define INT_CODE		0x25		/* "%"					*/
#define BS_CODE			0x08		/* "BS"					*/
#define CR_CODE			0x0d		/* "CR"					*/
#define SP_CODE			0x20		/* "LF"					*/
#define LF_CODE			0x0a		/* "LF"					*/

#define SIZE_8BIT		1		// Old name : BYTE_SIZE
#define SIZE_16BIT		2		// Old name : WORD_SIZE
#define SIZE_32BIT		4		// Old name : LONG_SIZE
#define SIZE_64BIT		8		// New

#define COMMAND_BUFFER_SIZE	1024

/****************************
	Module Proto Type		*
****************************/
int32_t PutMess(const char *const mess[]);
int32_t	PutStr(const char *str,char rtn);
int32_t	GetStr(char *str,char *chCnt);
uint32_t Hex2Ascii(int32_t hexdata,char *str,int32_t *chcnt);
uint32_t Hex2DecAscii(int32_t hexdata,char *str,int32_t *chcnt);
void DelStr(int32_t delCnt);
void ChgLtl2Lrg(char *str);
char HexAscii2Data(unsigned char *buf,uint32_t *data);
char Data2HexAscii(uint32_t data,char *buf,char size);
char GetStrBlk(char *inStr,char *outStr, char *chPtr,char method);
void SoftDelay(uint32_t roop);
void WaitKeyIn_Y(void);
char WaitKeyIn_YorN(void);
char DecodeHexAscStr(uintptr_t *para, char *buf);
char DecodeHexAscStr2(uintptr_t *para1st, uintptr_t *para2nd, char *buf);
char DecodeForm01(uintptr_t *para1st, uintptr_t *para2nd);
char DecodeForm02(uintptr_t *para1st);
char DecodeForm03(uintptr_t *para1st, uintptr_t *para2nd, uintptr_t *para3rd, uint32_t *setPara);
int32_t	GetStr_MemEd(char *str,char *chCnt);
char HexAscii2Data_64(unsigned char *buf,uintptr_t *data);
char Data2HexAscii_64(uintptr_t data,char *buf,char size);
#endif
