/*
 * Copyright (c) 2015-2016, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef TYPES_H
#define TYPES_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
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

#if 0
typedef unsigned long        uint32;
typedef long                 int32;
typedef unsigned short       uint16;
typedef short                int16;
typedef unsigned char        uint8;
typedef char                 int8;
typedef unsigned int         uint;
typedef unsigned char        bool8;
typedef unsigned long long   uint64;

typedef signed char          int8_t;
typedef unsigned char        uint8_t;
typedef signed short         int16_t;
typedef unsigned short       uint16_t;
typedef signed long          int32_t;
typedef unsigned long        uint32_t;
typedef unsigned long long int	uint64_t;
#endif

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

