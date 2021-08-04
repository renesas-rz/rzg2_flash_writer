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


#include "common.h"
#include "scifdrv.h"
#include "bit.h"
#include "rdk_cmn_cpg.h"
#include "devdrv.h"
#include "rdk_common.h"
/************************************************************************/
/*									*/
/*	Debug Seirial(SCIF2)						*/
/*									*/
/************************************************************************/
#define	UART_LSR_RXRDY_MASK		(UART_16550_LINE_STATUS_DR)
#define	UART_LSR_TXRDY_MASK		(UART_16550_LINE_STATUS_TEMT | UART_16550_LINE_STATUS_THRE)
#define	UART_LSR_READY_MASK		(UART_LSR_TXRDY_MASK | UART_LSR_RXRDY_MASK)

static st_UART_REG_t	*gs_uART_cReg = 0;		/** UART Control Regster Top Pointer */


int32_t PutCharSCIF2(char outChar)
{
		
	while(!((UART_16550_LINE_STATUS_THRE | UART_16550_LINE_STATUS_TEMT) & (gs_uART_cReg->LSR)));
	gs_uART_cReg->RBR_THR =  outChar;

	return(0);
}

int32_t GetCharSCIF2(char *inChar)
{
	uint16_t sts;

	do
	{
		sts = gs_uART_cReg->LSR;			/** Read Line Status */
		
		if (UART_16550_LINE_STATUS_FE & sts)
		{
			/*Framing Error*/
		}
		if (UART_16550_LINE_STATUS_OE & sts)
		{
			/*Overrun Error*/
		}
	} while(!(sts & UART_LSR_RXRDY_MASK));

	*inChar = gs_uART_cReg->RBR_THR;		/** Read Rx data */
	return(0);
}

int32_t GetCharTimeOutSCIF2(char *inChar, uint64_t us)
{
	uint16_t sts;
    int32_t  err = 0;
    
    uint64_t start = CMN_GetSysCnt();
    uint64_t cycles = (CMN_GetFreq4SysCnt() / 1000000UL) * us;

	do
	{
	    if ((CMN_GetSysCnt() - start) > cycles) {
	        err = -1;
	        break;
	    }
		sts = gs_uART_cReg->LSR;			/** Read Line Status */
		
		if (UART_16550_LINE_STATUS_FE & sts)
		{
			/*Framing Error*/
		}
		if (UART_16550_LINE_STATUS_OE & sts)
		{
			/*Overrun Error*/
		}
	} while(!(sts & UART_LSR_RXRDY_MASK));

    if (err != -1) //not time out
    	*inChar = gs_uART_cReg->RBR_THR;		/** Read Rx data */
    
	return(err);
}

void WaitPutScif2SendEnd(void)
{
	uint16_t dataW;
	uint32_t loop;

	loop=1;
	while(loop)
	{
		dataW = gs_uART_cReg->LSR;
		if (dataW & UART_16550_LINE_STATUS_THRE)
		{
			loop = 0;
		}
	}
}

void InitScif2_SCIFCLK(void)
{
	volatile unsigned long i;
	unsigned long bit_rate;
	
	// UART Target Channel set
	gs_uART_cReg = (st_UART_REG_t *)UART_CH0_BASE;

	// UART initialize reset.
	gs_uART_cReg->FCR = UART_MODE_CONST_FCR_RST;			// Reset Rx,Rx FIFO
	gs_uART_cReg->HCR0 |= 0x0080;							// S/W Reset
	for(i=0; i < 1000; i++);								// Delay Over 6*PLCK(For fail soft)
	gs_uART_cReg->HCR0 &= ~(0x0080);						// S/W Reset release
	for(i=0; i < 1000; i++);								// Delay Over 6*PLCK(For fail soft)

	// UART set bit rate.
	gs_uART_cReg->LCR |= 0x0080;							// Select Div. Latch Register

	bit_rate = SCLK / (UART_BAUDRATE *16);

	gs_uART_cReg->DLL = (uint8_t)bit_rate;
	gs_uART_cReg->DLM = (uint8_t)(bit_rate>>8);

	gs_uART_cReg->LCR = UART_MODE_CONST_LCR;				// Select Data Buff & Set Parity,etc.

	gs_uART_cReg->IER = UART_MODE_CONST_IER_P;			// Set Int. All Disable
	gs_uART_cReg->FCR = UART_MODE_CONST_FCR_P;			// Set Int. FIFO Trigger Level,etc.
	gs_uART_cReg->MCR = UART_MODE_CONST_MCR_P;			// Set Flow Ctl. Enable/Disble,etc.

	// Reset Tx&Rx DMA_REQ
	gs_uART_cReg->HCR0 = 0x0000;
}

uint32_t SCIF_TerminalInputCheck(char* str)
{
	uint32_t result = 0;
	uint16_t sts;
	
	sts = gs_uART_cReg->LSR;			/** Read Line Status */
	
	if (UART_16550_LINE_STATUS_FE & sts)
	{
		/*Framing Error*/
	}
	if (UART_16550_LINE_STATUS_OE & sts)
	{
		/*Overrun Error*/
	}
	if(sts & UART_LSR_RXRDY_MASK){
		*str = gs_uART_cReg->RBR_THR;		/** Read Rx data */
		result = 1;
	}
	return result;
}
