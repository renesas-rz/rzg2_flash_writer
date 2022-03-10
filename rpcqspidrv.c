/*
 * Copyright (c) 2015-2022, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "common.h"
#include "rpcqspidrv.h"
#include "rzfive_def.h"
#include "rzfive_cpg_regs.h"
#include "rpc_regs.h"
#include "bit.h"
#include "cpudrv.h"

void InitRPC_QspiFlash4FastReadExtMode(void)
{
	*((volatile uint32_t*)RPC_PHYCNT)    = 0x00030260;
	*((volatile uint32_t*)RPC_PHYCNT)    = 0x80030260;
	*((volatile uint32_t*)RPC_CMNCR)      = 0x01FFF300;
	*((volatile uint32_t*)RPC_DRCR)       = 0x001F0100;
		//bit20-16 RBURST[4:0] = 11111 : 32 continuous data unit
		//bit8     RBE         =     1 : Burst read
	*((volatile uint32_t*)RPC_DRCMR)      = 0x000C0000;
		//bit23-16 CMD[7:0] = 0x0C : 4FAST_READ 0Ch Command 4-byte address command

	*((volatile uint32_t*)RPC_DREAR)      = 0x00000001;
		//bit23-16 EAV[7:0]   = 0    : ADR[32:26] output set0
		//bit2-0   EAC[2:0]   = 001  : ADR[25:0 ] Enable

	*((volatile uint32_t*)RPC_DRENR)      = 0x0000CF00;
		//bit31-30 CDB[1:0]   =   00 : 1bit width command (QSPI0_MOSI)
		//bit25-24 ADB[1:0]   =   00 : 1bit width address (QSPI0_MOSI)
		//bit17-16 DRDB[1:0]  =   00 : 1bit width transfer data (QSPI0_IO0)
		//bit15    DME        =    1 : dummy cycle enable
		//bit14    CDE        =    1 : Command enable
		//bit11-8  ADE[3:0]   = 1111 : ADR[23:0] output (24 Bit Address)
	*((volatile uint32_t*)RPC_DRDMCR)     = 0x00000007;
		//bit2-0 DMCYC[2:0]   = 111 : 8 cycle dummy wait
	*((volatile uint32_t*)RPC_DRDRENR)    = 0x00000000;
		//bit8 ADDRE  = 0 : Address SDR transfer
		//bit0 DRDRE  = 0 : DATA SDR transfer
}

void InitRPC_QspiFlash(uint32_t rpcclk)
{
	PowerOnRPC();

	SetRPC_ClockMode(RPC_CLK_80M);
	ResetRPC();
	SetRPC_SSL_Delay();

	*((volatile uint32_t*)RPC_OFFSET1)= 0x31511144;		/* DDRTMG[1:0](Bit[29:28]) == B'11 */
}

//4SE DCh 4-byte address
void SectorErase4QspiFlash(uint32_t sector_addr)
{
	char str[64];

	*((volatile uint32_t*)RPC_PHYCNT)    = 0x80030260;
	*((volatile uint32_t*)RPC_CMNCR)      = 0x81FFF300;
		//bit31  MD       =  1 : Manual mode
		//bit1-0 BSZ[1:0] = 00 : QSPI Flash x 1
	*((volatile uint32_t*)RPC_SMCMR)      = 0x00DC0000;
		//bit23-16 CMD[7:0] = 0xDC : Sector Erase 4-byte address command
	*((volatile uint32_t*)RPC_SMADR)      = sector_addr;
	*((volatile uint32_t*)RPC_SMDRENR)    = 0x00000000;
		//bit8 ADDRE  = 0 : Address SDR transfer
		//bit0 SPIDRE = 0 : DATA SDR transfer
	*((volatile uint32_t*)RPC_SMENR)      = 0x00004F00;
		//bit31-30 CDB[1:0]   =   00 : 1bit width command (QSPI0_MOSI)
		//bit25-24 ADB[1:0]   =   00 : 1bit width address (QSPI0_MOSI)
		//bit17-16 SPIDB[1:0] =   00 : 1bit width transfer data (QSPI0_MISO)
		//bit15    DME        =    0 : No dummy cycle
		//bit14    CDE        =    1 : Command enable
		//bit11-8  ADE[3:0]   = 1111 : ADR[31:0] output (32 Bit Address)
		//bit3-0   SPIDE[3:0] = 0000 : No transfer
	*((volatile uint32_t*)RPC_SMCR)       = 0x00000001;
		//bit2     SPIRE      = 0 : Data read disable
		//bit1     SPIWE      = 0 : Data write disable
		//bit0     SPIE       = 1 : SPI transfer start

	WaitRpcTxEnd();
}

//4P4E 21h 4-byte address
void ParameterSectorErase4QspiFlash(uint32_t sector_addr)
{
	*((volatile uint32_t*)RPC_PHYCNT)    = 0x80030260;
	*((volatile uint32_t*)RPC_CMNCR)      = 0x81FFF300;
		//bit31  MD       =  1 : Manual mode
		//bit1-0 BSZ[1:0] = 00 : QSPI Flash x 1
	*((volatile uint32_t*)RPC_SMCMR)      = 0x00210000;
		//bit23-16 CMD[7:0] = 0x21 : Parameter 4-kB Sector Erasecommand
	*((volatile uint32_t*)RPC_SMADR)      = sector_addr;
	*((volatile uint32_t*)RPC_SMDRENR)    = 0x00000000;
		//bit8 ADDRE  = 0 : Address SDR transfer
		//bit0 SPIDRE = 0 : DATA SDR transfer
	*((volatile uint32_t*)RPC_SMENR)      = 0x00004F00;
		//bit31-30 CDB[1:0]   =   00 : 1bit width command (QSPI0_MOSI)
		//bit25-24 ADB[1:0]   =   00 : 1bit width address (QSPI0_MOSI)
		//bit17-16 SPIDB[1:0] =   00 : 1bit width transfer data (QSPI0_MISO)
		//bit15    DME        =    0 : No dummy cycle
		//bit14    CDE        =    1 : Command enable
		//bit11-8  ADE[3:0]   = 1111 : ADR[31:0] output (32 Bit Address)
		//bit3-0   SPIDE[3:0] = 0000 : No transfer
	*((volatile uint32_t*)RPC_SMCR)       = 0x00000001;
		//bit2     SPIRE      = 0 : Data read disable
		//bit1     SPIWE      = 0 : Data write disable
		//bit0     SPIE       = 1 : SPI transfer start

	WaitRpcTxEnd();
}

//Page Program (4PP:12h)  4-byte address
void WriteData4ppWithBufferQspiFlash(uint32_t addr, uint32_t source_addr)
{
	uintptr_t i=0;
	*((volatile uint32_t*)RPC_DRCR)       = 0x011F0301;
		//bit9   RCF         =  1 : Read Cache Clear

	*((volatile uint32_t*)RPC_PHYCNT)    = 0x80030274;

	for(i = 0; i < 256; i = i+0x4)
	{
		(*(volatile uint32_t*)(RPC_WRBUF+i)) = (*(volatile uint32_t*)(source_addr+i));
	}

		//bit31  CAL         =  1 : PHY calibration
		//bit2   WBUF        =  1 : Write Buffer Enable
		//bit1-0 PHYMEM[1:0] = 00 : QSPI-SDR
	*((volatile uint32_t*)RPC_CMNCR)      = 0x81FFF300;
		//bit31  MD       =  1 : Manual mode
		//bit1-0 BSZ[1:0] = 00 : QSPI Flash x 1
	*((volatile uint32_t*)RPC_SMCMR)      = 0x00120000;
		//bit23-16 CMD[7:0] = 0x12 : Page Program 4-byte address
	*((volatile uint32_t*)RPC_SMADR)      = addr;
	*((volatile uint32_t*)RPC_SMDRENR)    = 0x00000000;
		//bit8 ADDRE  = 0 : Address SDR transfer
		//bit0 SPIDRE = 0 : DATA SDR transfer
	*((volatile uint32_t*)RPC_SMENR)      = 0x00004F0F;
		//bit31-30 CDB[1:0]   =   00 : 1bit width command (QSPI0_MOSI)
		//bit25-24 ADB[1:0]   =   00 : 1bit width address (QSPI0_MOSI)
		//bit17-16 SPIDB[1:0] =   00 : 1bit width transfer data (QSPI0_MISO)
		//bit15    DME        =    0 : No dummy cycle
		//bit14    CDE        =    1 : Command enable
		//bit11-8  ADE[3:0]   = 1111 : ADR[23:0] is output
		//bit3-0   SPIDE[3:0] = 1111 : 32bit transfer
	*((volatile uint32_t*)RPC_SMCR)       = 0x00000003;
		//bit2     SPIRE      = 0 : Data read disable
		//bit1     SPIWE      = 1 : Data write enable
		//bit0     SPIE       = 1 : SPI transfer start

	WaitRpcTxEnd();

	*((volatile uint32_t*)RPC_PHYCNT)    = 0x00030273;
	*((volatile uint32_t*)RPC_DRCR)       = 0x011F0301;
		//bit9   RCF         =  1 : Read Cache Clear
}

//Page Program (PP:02h)  3-byte address
void WriteDataPpWithBufferQspiFlash(uint32_t addr, uint32_t source_addr)
{
	//uint32_t i=0;
	uintptr_t i=0;

	*((volatile uint32_t*)RPC_DRCR)       = 0x011F0301;
		//bit9   RCF         =  1 : Read Cache Clear

	*((volatile uint32_t*)RPC_PHYCNT)    = 0x80030274;

	for(i = 0; i < 256; i = i+0x4)
	{
		(*(volatile uint32_t*)(RPC_WRBUF+i)) = (*(volatile uint32_t*)(source_addr+i));
	}
		//bit31  CAL         =  1 : PHY calibration
		//bit2   WBUF        =  1 : Write Buffer Enable
		//bit1-0 PHYMEM[1:0] = 00 : QSPI-SDR
	*((volatile uint32_t*)RPC_CMNCR)      = 0x81FFF300;
		//bit31  MD       =  1 : Manual mode
		//bit1-0 BSZ[1:0] = 00 : QSPI Flash x 1
	*((volatile uint32_t*)RPC_SMCMR)      = 0x00020000;
		//bit23-16 CMD[7:0] = 0x02 : Page Program 3-byte address
	*((volatile uint32_t*)RPC_SMADR)      = addr;
	*((volatile uint32_t*)RPC_SMDRENR)    = 0x00000000;
		//bit8 ADDRE  = 0 : Address SDR transfer
		//bit0 SPIDRE = 0 : DATA SDR transfer
	*((volatile uint32_t*)RPC_SMENR)      = 0x0000470F;
		//bit31-30 CDB[1:0]   =   00 : 1bit width command (QSPI0_MOSI)
		//bit25-24 ADB[1:0]   =   00 : 1bit width address (QSPI0_MOSI)
		//bit17-16 SPIDB[1:0] =   00 : 1bit width transfer data (QSPI0_MISO)
		//bit15    DME        =    0 : No dummy cycle
		//bit14    CDE        =    1 : Command enable
		//bit11-8  ADE[3:0]   = 0111 : ADR[23:0] is output
		//bit3-0   SPIDE[3:0] = 1111 : 32bit transfer
	*((volatile uint32_t*)RPC_SMCR)       = 0x00000003;
		//bit2     SPIRE      = 0 : Data read disable
		//bit1     SPIWE      = 1 : Data write enable
		//bit0     SPIE       = 1 : SPI transfer start

	WaitRpcTxEnd();

	*((volatile uint32_t*)RPC_PHYCNT)    = 0x00030273;
	*((volatile uint32_t*)RPC_DRCR)       = 0x011F0301;
		//bit9   RCF         =  1 : Read Cache Clear
}

//65h Read Any Register command (RADR 65h)
void ReadAnyRegisterQspiFlash(uint32_t addr, unsigned char *readData)		// Add24bit,Data8bit
{
	*((volatile uint32_t*)RPC_PHYCNT)    = 0x00030260;
	*((volatile uint32_t*)RPC_PHYCNT)    = 0x80030260;
	*((volatile uint32_t*)RPC_CMNCR)      = 0x81FFF300;
		//bit31  MD       =  1 : Manual mode
		//bit1-0 BSZ[1:0] = 00 : QSPI Flash x 1
	*((volatile uint32_t*)RPC_SMCMR)      = 0x00650000;
		//bit23-16 CMD[7:0] = 0x65 :        Read Any Register command (RADR 65h)
	*((volatile uint32_t*)RPC_SMADR)      = addr;
	*((volatile uint32_t*)RPC_SMDMCR)     = 0x00000007;
		//bit2-0 DMCYC[2:0] = 111 : 8 cycle dummy wait
	*((volatile uint32_t*)RPC_SMDRENR)    = 0x00000000;
		//bit8 ADDRE  = 0 : Address SDR transfer
		//bit0 SPIDRE = 0 : DATA SDR transfer
	*((volatile uint32_t*)RPC_SMENR)      = 0x0000C708;
		//bit31-30 CDB[1:0]   =   00 : 1bit width command (QSPI0_MOSI)
		//bit25-24 ADB[1:0]   =   00 : 1bit width address (QSPI0_MOSI)
		//bit17-16 SPIDB[1:0] =   00 : 1bit width transfer data (QSPI0_MISO)
		//bit15    DME        =    1 : dummy cycle enable
		//bit14    CDE        =    1 : Command enable
		//bit11-8  ADE[3:0]   = 0111 : ADR[23:0] output (24 Bit Address)
		//bit3-0   SPIDE[3:0] = 1000 : 8bit transfer
	*((volatile uint32_t*)RPC_SMCR)       = 0x00000005;
		//bit2     SPIRE      = 1 : Data read enable
		//bit1     SPIWE      = 0 : Data write disable
		//bit0     SPIE       = 1 : SPI transfer start

	WaitRpcTxEnd();

	*readData   = *((volatile unsigned char*)RPC_SMRDR0);	//read data[7:0]
}

//71h Write Any Register command (WRAR 71h)
void WriteAnyRegisterQspiFlash(uint32_t addr, unsigned char writeData)			// Add24bit,Data8bit
{
	*((volatile uint32_t*)RPC_PHYCNT)    = 0x80030260;
	*((volatile uint32_t*)RPC_CMNCR)      = 0x81FFF300;
		//bit31  MD       =  1 : Manual mode
		//bit1-0 BSZ[1:0] = 00 : QSPI Flash x 1
	*((volatile uint32_t*)RPC_SMCMR)      = 0x00710000;
		//bit23-16 CMD[7:0] = 0x71 : Write Any Register Command  (WRAR)
	*((volatile uint32_t*)RPC_SMADR)      = addr;
	*((volatile uint32_t*)RPC_SMDRENR)    = 0x00000000;
		//bit8 ADDRE  = 0 : Address SDR transfer
		//bit0 SPIDRE = 0 : DATA SDR transfer
	*((volatile uint32_t*)RPC_SMENR)      = 0x00004708;
		//bit31-30 CDB[1:0]   =   00 : 1bit width command (QSPI0_MOSI)
		//bit25-24 ADB[1:0]   =   00 : 1bit width address (QSPI0_MOSI)
		//bit17-16 SPIDB[1:0] =   00 : 1bit width transfer data (QSPI0_MISO)
		//bit15    DME        =    0 : No dummy cycle
		//bit14    CDE        =    1 : Command enable
		//bit11-8  ADE[3:0]   = 0111 : ADR[24:0] is output
		//bit3-0   SPIDE[3:0] = 1000 : 8bit transfer
	*((volatile unsigned char*)RPC_SMWDR0)= writeData;
	*((volatile uint32_t*)RPC_SMCR)       = 0x00000003;
		//bit2     SPIRE      = 0 : Data read disable
		//bit1     SPIWE      = 1 : Data write disable
		//bit0     SPIE       = 1 : SPI transfer start

	WaitRpcTxEnd();

	*((volatile uint32_t*)RPC_PHYCNT)    = 0x00030273;
	*((volatile uint32_t*)RPC_DRCR)       = 0x011F0301;
		//bit9   RCF         =  1 : Read Cache Clear
}

//FAST_READ 0Bh (CR2V[7]=0) is followed by a 3-byte address
void InitRPC_QspiFlashFastReadExtMode(void)
{
	*((volatile uint32_t*)RPC_PHYCNT)    = 0x00030260;
	*((volatile uint32_t*)RPC_PHYCNT)    = 0x80030260;
	*((volatile uint32_t*)RPC_CMNCR)      = 0x01FFF300;
	*((volatile uint32_t*)RPC_DRCR)       = 0x001F0100;
		//bit20-16 RBURST[4:0] = 11111 : 32 continuous data unit
		//bit8     RBE         =     1 : Burst read
	*((volatile uint32_t*)RPC_DRCMR)      = 0x000B0000;
		//bit23-16 CMD[7:0] = 0x0B : FAST_READ 0Bh

	*((volatile uint32_t*)RPC_DREAR)      = 0x00000000;
		//bit23-16 EAV[7:0]   = 0    : ADR[32:25] output set0
		//bit2-0   EAC[2:0]   = 000  : ADR[24:0 ] Enable

	*((volatile uint32_t*)RPC_DROPR)      = 0x00000000;
		//bit31-24 OPD3[7:0]  = H'0  : Option Data 3 (Set Mode)

	*((volatile uint32_t*)RPC_DRENR)      = 0x0000C700;	//
		//bit31-30 CDB[1:0]   =   00 : 1bit width command (QSPI0_MOSI)
		//bit25-24 ADB[1:0]   =   00 : 1bit width address (QSPI0_MOSI)
		//bit17-16 DRDB[1:0]  =   00 : 1bit width transfer data (QSPI0_IO0-3)
		//bit15    DME        =    1 : dummy cycle enable
		//bit14    CDE        =    1 : Command enable
		//bit11-8  ADE[3:0]   = 0111 : ADR[23:0] output (24 Bit Address)
	*((volatile uint32_t*)RPC_DRDMCR)     = 0x00000007;		//8 cycle dummy
		//bit17-16 DMCYC[2:0] =  10 : 4 bit width
		//bit2-0 DMCYC[2:0]   = 111 : 8 cycle dummy wait
		//bit2-0 DMCYC[2:0]   = 011 : 4 cycle dummy wait
	*((volatile uint32_t*)RPC_DRDRENR)    = 0x00000000;
		//bit8 ADDRE  = 0 : Address SDR transfer
		//bit0 DRDRE  = 0 : DATA SDR transfer
}

void SetRPC_ClockMode(uint32_t mode)
{
	uint32_t dataL=0;

	/* --- Set 66.6MHz on PLL3 for SPI Multi --- */
	dataL = *((volatile uint32_t*)CPG_PL3A_DDIV);
	dataL &= (~0x00000700U);
	dataL |=  0x01000100U;	/* DIV_PLL3_C = 1/2 */
	*((volatile uint32_t*)CPG_PL3A_DDIV) = dataL;

	*((volatile uint32_t*)CPG_PL3_SSEL) = 0x01110011U;

	SoftDelay(50000);
}

void WaitRpcTxEnd(void)
{
	uint32_t dataL=0;

	while(1)
	{
		dataL = *((volatile uint32_t*)RPC_CMNSR);
		if(dataL & BIT0)
		{
			break;
		}
		// Wait for TEND = 1
	}
}

void ResetRPC(void)
{
}

void SetRPC_SSL_Delay(void)
{
	uint32_t dataL=0;

	*((volatile uint32_t*)RPC_PHYADJ2) = 0xA5390000;
	*((volatile uint32_t*)RPC_PHYADJ1) = 0x80000000;
	*((volatile uint32_t*)RPC_PHYADJ2) = 0x00008080;
	*((volatile uint32_t*)RPC_PHYADJ1) = 0x80000022;
	*((volatile uint32_t*)RPC_PHYADJ2) = 0x00008080;
	*((volatile uint32_t*)RPC_PHYADJ1) = 0x80000024;

	dataL = *((volatile uint32_t*)RPC_PHYCNT);
	*((volatile uint32_t*)RPC_PHYCNT)  = (dataL | 0x00030000);
	*((volatile uint32_t*)RPC_PHYADJ2) = 0x00000030;
	*((volatile uint32_t*)RPC_PHYADJ1) = 0x80000032;


	*((volatile uint32_t*)RPC_SSLDR) = 0x00000400;
		//bit10-8  SLNDL[2:0] =  100 : 5.5 cycles from QSPIn_SPCLK edge
}

void PowerOnRPC(void)
{
	uint32_t dataL=0;

	dataL = *((volatile uint32_t*)CPG_CLKON_SPI_MULTI);
	dataL |= 0x00030003U;									/* Set WEN[0](bit[8) w/B'1 */
	*((volatile uint32_t*)CPG_CLKON_SPI_MULTI) = dataL;
	do {
		dataL = *((volatile uint32_t*)CPG_CLKMON_SPI_MULTI);
	} while ((dataL & (BIT1 | BIT0)) == 0U);	/* wait until bit1/0=1 */

	dataL = *((volatile uint32_t*)CPG_RST_SPI);
	dataL |= 0x00010001U;
	*((volatile uint32_t*)CPG_RST_SPI) = dataL;
	do {
		dataL = *((volatile uint32_t*)CPG_RSTMON_SPI);
	} while ((dataL & BIT0) == 1U);	/* wait until bit0=1 */
}


uint32_t ReadQspiFlashID(uint32_t *readData)	//for QSPIx1ch
{
	char str[64];

	*((volatile uint32_t*)RPC_PHYCNT)    = 0x00030060;
	*((volatile uint32_t*)RPC_PHYCNT)    = 0x80030060;
	*((volatile uint32_t*)RPC_CMNCR)      = 0x81FFF300;
		//bit31  MD       =  1 : Manual mode
		//bit1-0 BSZ[1:0] = 00 : QSPI Flash x 1
	*((volatile uint32_t*)RPC_SMCMR)      = 0x009F0000;
		//bit23-16 CMD[7:0] = 0x9F : Read ID command (for Palladium QSPI model)
	*((volatile uint32_t*)RPC_SMDRENR)    = 0x00000000;
		//bit8 ADDRE  = 0 : Address SDR transfer
		//bit0 SPIDRE = 0 : DATA SDR transfer
	*((volatile uint32_t*)RPC_SMENR)      = 0x0000400F;
		//bit31-30 CDB[1:0]   =   00 : 1bit width command (QSPI0_MOSI)
		//bit25-24 ADB[1:0]   =   00 : 1bit width address (QSPI0_MOSI)
		//bit17-16 SPIDB[1:0] =   00 : 1bit width transfer data (QSPI0_MISO)
		//bit15    DME        =    0 : No dummy cycle
		//bit14    CDE        =    1 : Command enable
		//bit11-8  ADE[3:0]   = 0000 : Address output disable
		//bit3-0   SPIDE[3:0] = 1111 : 32bit transfer
	*((volatile uint32_t*)RPC_SMCR)       = 0x00000005;
		//bit2     SPIRE      = 1 : Data read enable
		//bit1     SPIWE      = 0 : Data write disable
		//bit0     SPIE       = 1 : SPI transfer start

	WaitRpcTxEnd();

	readData[0] = *((volatile uint32_t*)RPC_SMRDR0);	//read data[31:0]
	return(readData[0]);
}

uint32_t ReadStatusQspiFlash(uint32_t *readData)	//for QSPIx1ch
{
	*((volatile uint32_t*)RPC_PHYCNT)    = 0x00030260;
	*((volatile uint32_t*)RPC_PHYCNT)    = 0x80030260;
	*((volatile uint32_t*)RPC_CMNCR)      = 0x81FFF300;
		//bit31  MD       =  1 : Manual mode
		//bit1-0 BSZ[1:0] = 00 : QSPI Flash x 1
	*((volatile uint32_t*)RPC_SMCMR)      = 0x00050000;
		//bit23-16 CMD[7:0] = 0x05 : Status Read command (for Palladium QSPI model)
	*((volatile uint32_t*)RPC_SMDRENR)    = 0x00000000;
		//bit8 ADDRE  = 0 : Address SDR transfer
		//bit0 SPIDRE = 0 : DATA SDR transfer
	*((volatile uint32_t*)RPC_SMENR)      = 0x0000400F;
		//bit31-30 CDB[1:0]   =   00 : 1bit width command (QSPI0_MOSI)
		//bit25-24 ADB[1:0]   =   00 : 1bit width address (QSPI0_MOSI)
		//bit17-16 SPIDB[1:0] =   00 : 1bit width transfer data (QSPI0_MISO)
		//bit15    DME        =    0 : No dummy cycle
		//bit14    CDE        =    1 : Command enable
		//bit11-8  ADE[3:0]   = 0000 : Address output disable
		//bit3-0   SPIDE[3:0] = 1111 : 32bit transfer
	*((volatile uint32_t*)RPC_SMCR)       = 0x00000005;
		//bit2     SPIRE      = 1 : Data read enable
		//bit1     SPIWE      = 0 : Data write disable
		//bit0     SPIE       = 1 : SPI transfer start

	WaitRpcTxEnd();

	readData[0] = *((volatile uint32_t*)RPC_SMRDR0);	//read data[31:0]

	return(readData[0]);
}

void WriteCommandQspiFlash(uint32_t command)	//for QSPIx1ch
{
	*((volatile uint32_t*)RPC_PHYCNT)    = 0x80030260;
	*((volatile uint32_t*)RPC_CMNCR)      = 0x81FFF300;
		//bit31  MD       =  1 : Manual mode
		//bit1-0 BSZ[1:0] = 00 : QSPI Flash x 1
	*((volatile uint32_t*)RPC_SMCMR)      = command;
		//bit23-16 CMD[7:0] : command
	*((volatile uint32_t*)RPC_SMDRENR)    = 0x00000000;
		//bit8 ADDRE  = 0 : Address SDR transfer
		//bit0 SPIDRE = 0 : DATA SDR transfer
	*((volatile uint32_t*)RPC_SMENR)      = 0x00004000;
		//bit31-30 CDB[1:0]   =   00 : 1bit width command (QSPI0_MOSI)
		//bit25-24 ADB[1:0]   =   00 : 1bit width address (QSPI0_MOSI)
		//bit17-16 SPIDB[1:0] =   00 : 1bit width transfer data (QSPI0_MISO)
		//bit15    DME        =    0 : No dummy cycle
		//bit14    CDE        =    1 : Command enable
		//bit11-8  ADE[3:0]   = 0000 : Address output disable
		//bit3-0   SPIDE[3:0] = 0000 : No transfer
	*((volatile uint32_t*)RPC_SMCR)       = 0x00000001;
		//bit2     SPIRE      = 0 : Data read disable
		//bit1     SPIWE      = 0 : Data write disable
		//bit0     SPIE       = 1 : SPI transfer start

	WaitRpcTxEnd();

}

// SE (4KB) 20h 3-byte address
void ParameterSectorErase3QspiFlash(uint32_t sector_addr)
{
	*((volatile uint32_t*)RPC_PHYCNT)    = 0x80030260;
	*((volatile uint32_t*)RPC_CMNCR)      = 0x81FFF300;
		//bit31  MD       =  1 : Manual mode
		//bit1-0 BSZ[1:0] = 00 : QSPI Flash x 1
	*((volatile uint32_t*)RPC_SMCMR)      = 0x00200000;
		//bit23-16 CMD[7:0] = 0x20 : Sector Erase command (for Palladium QSPI model)
	*((volatile uint32_t*)RPC_SMADR)      = sector_addr;
	*((volatile uint32_t*)RPC_SMDRENR)    = 0x00000000;
		//bit8 ADDRE  = 0 : Address SDR transfer
		//bit0 SPIDRE = 0 : DATA SDR transfer
	*((volatile uint32_t*)RPC_SMENR)      = 0x00004700;
		//bit31-30 CDB[1:0]   =   00 : 1bit width command (QSPI0_MOSI)
		//bit25-24 ADB[1:0]   =   00 : 1bit width address (QSPI0_MOSI)
		//bit17-16 SPIDB[1:0] =   00 : 1bit width transfer data (QSPI0_MISO)
		//bit15    DME        =    0 : No dummy cycle
		//bit14    CDE        =    1 : Command enable
		//bit11-8  ADE[3:0]   = 0111 : ADR[23:0] output (24 Bit Address)
		//bit3-0   SPIDE[3:0] = 0000 : No transfer
	*((volatile uint32_t*)RPC_SMCR)       = 0x00000001;
		//bit2     SPIRE      = 0 : Data read disable
		//bit1     SPIWE      = 0 : Data write disable
		//bit0     SPIE       = 1 : SPI transfer start

	WaitRpcTxEnd();
}

//SE D8h  3-byte address
void SectorEraseQspiFlash(uint32_t sector_addr)	//for QSPIx1ch
{
	*((volatile uint32_t*)RPC_PHYCNT)    = 0x80030260;
	*((volatile uint32_t*)RPC_CMNCR)      = 0x81FFF300;
		//bit31  MD       =  1 : Manual mode
		//bit1-0 BSZ[1:0] = 00 : QSPI Flash x 1
	*((volatile uint32_t*)RPC_SMCMR)      = 0x00D80000;
		//bit23-16 CMD[7:0] = 0xD8 : Sector Erase command (for Palladium QSPI model)
	*((volatile uint32_t*)RPC_SMADR)      = sector_addr;
	*((volatile uint32_t*)RPC_SMDRENR)    = 0x00000000;
		//bit8 ADDRE  = 0 : Address SDR transfer
		//bit0 SPIDRE = 0 : DATA SDR transfer
	*((volatile uint32_t*)RPC_SMENR)      = 0x00004700;
		//bit31-30 CDB[1:0]   =   00 : 1bit width command (QSPI0_MOSI)
		//bit25-24 ADB[1:0]   =   00 : 1bit width address (QSPI0_MOSI)
		//bit17-16 SPIDB[1:0] =   00 : 1bit width transfer data (QSPI0_MISO)
		//bit15    DME        =    0 : No dummy cycle
		//bit14    CDE        =    1 : Command enable
		//bit11-8  ADE[3:0]   = 0111 : ADR[23:0] output (24 Bit Address)
		//bit3-0   SPIDE[3:0] = 0000 : No transfer
	*((volatile uint32_t*)RPC_SMCR)       = 0x00000001;
		//bit2     SPIRE      = 0 : Data read disable
		//bit1     SPIWE      = 0 : Data write disable
		//bit0     SPIE       = 1 : SPI transfer start

	WaitRpcTxEnd();
}
