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

#ifndef TYPES_H
#define TYPES_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint-gcc.h>
/****************************************************************************
 * File Name: types.h
 * Contents : Types Define
 ****************************************************************************/

#ifndef FALSE
#define FALSE       0
#endif

#ifndef TRUE
#define TRUE        1
#endif

#ifndef SUCCESS
#define SUCCESS     0UL
#endif

#ifndef YES
#define YES         1
#endif

#ifndef NO
#define NO          0
#endif

#ifndef NULL
#define NULL        (void *)0UL
#endif

#define PFAR        /**/
#define FFAR        /**/

typedef signed char          BYTE;
typedef signed short         WORD;
typedef signed long          DWORD;

typedef signed char B;      /* signed 8-bit integer */
typedef long W;             /* signed 32-bit integer */
typedef unsigned char UB;   /* unsigned 8-bit integer */
typedef unsigned long UW;   /* unsigned 32-bit integer */

typedef short H;            /* signed 16-bit integer */
typedef unsigned short UH;  /* unsigned 16-bit integer */

typedef int INT;
typedef unsigned int UINT;

typedef INT BOOL;

typedef void (FFAR *FP)();
typedef INT FN;
typedef INT ID;
typedef INT BOOL_ID;
typedef INT HNO;
typedef INT RNO;
typedef INT RDVNO;
typedef UINT RDVPTN;
typedef UINT ATR;
typedef UINT MODE;
typedef INT ER;
typedef INT PRI;
typedef ER ER_ID;
typedef UINT STAT;
typedef INT ER_UINT;
typedef UINT TEXPTN;
typedef UINT FLGPTN;
typedef UINT INHNO;
typedef UINT INTNO;

typedef unsigned long SIZE;
typedef W TMO;
typedef W DLYTIME;
typedef DLYTIME RELTIM;
typedef void PFAR *VP;
typedef VP VP_INT;

#ifdef __cplusplus
}
#endif

#endif

