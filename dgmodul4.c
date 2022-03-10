/*
 * Copyright (c) 2015-2021, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "common.h"
#include "dgtable.h"
#include "dgmodul4.h"
#include "rpcqspidrv.h"
#include "spiflash1drv.h"
#include "ramckmdl.h"
#include "devdrv.h"
#include "dgmodul1.h"
#include "bit.h"
#include <string.h>

uint32_t	gSpiFlashSvArea;
uint32_t	gUserPrgStartAdd;
uint32_t	gUserPrgSize;

uint32_t	gManufacturerId;
uint32_t	gDeviceId;
uint32_t	gQspi_sa_size;
uint32_t	gQspi_end_addess;

extern char gKeyBuf[64];

static char dgLS_Load_Offset(uint32_t *maxADD ,uint32_t *minADD, uint32_t prgStatAdd);
static char dgLS_Load_And_Write_LowMemory(uint32_t spiStartAdd, uint32_t *spiEndAdd, uint32_t prgStatAdd);

void InitRPC_Mode(void)
{
	InitRPC_QspiFlash(RPC_CLK_40M);
}

//////////////////////////////////////////
// CheckQspiFlashId
//////////////////////////////////////////
static uint32_t CheckQspiFlashId(void)
{
	char		str[64];
	uint32_t	readDevId, ret = 0;
	uint8_t		manuId;
	uint16_t	deviceId;

	ReadQspiFlashID(&readDevId);

	manuId   = readDevId & 0x000000ff;
	deviceId = (readDevId & 0x0000ff00) | ((readDevId >> 16) & 0xff);

	gManufacturerId	= manuId;
	gDeviceId		= deviceId;
	switch(manuId)
	{
		case CYPRESS_MANUFACTURER_ID:
			PutStr(" Cypress : ", 0);
			switch(deviceId)
			{
				case DEVICE_ID_S25FS512S:
						PutStr("S25FS512S", 1);
						gQspi_sa_size    = SA_256KB;
						gQspi_end_addess = TOTAL_SIZE_64MB - 0x8000 - 1;
				break;
				case DEVICE_ID_S25FS128S:
						PutStr("S25FS128S", 1);
						gQspi_sa_size    = SA_256KB;
						gQspi_end_addess = TOTAL_SIZE_16MB - 0x8000 - 1;
				break;
				default:
					ret = 1;
				break;
			}
		break;
		case WINBOND_MANUFACTURER_ID:
			PutStr(" Winbond : ", 0);
			switch(deviceId)
			{
				case DEVICE_ID_W25Q64JV:
						PutStr("W25Q64JV", 1);
						gQspi_sa_size    = SA_64KB;
						gQspi_end_addess = TOTAL_SIZE_8MB - 0x8000 - 1;
				break;
				case DEVICE_ID_W25Q64JW:
						PutStr("W25Q64JW", 1);
						gQspi_sa_size    = SA_64KB;
						gQspi_end_addess = TOTAL_SIZE_8MB - 0x8000 - 1;
				break;
				case DEVICE_ID_W25Q128JV:
						PutStr("W25Q128JV", 1);
						gQspi_sa_size    = SA_64KB;
						gQspi_end_addess = TOTAL_SIZE_16MB - 0x8000 - 1;
				break;

				case DEVICE_ID_W25Q128JW:
						PutStr("W25Q128JW", 1);
						gQspi_sa_size    = SA_64KB;
						gQspi_end_addess = TOTAL_SIZE_16MB - 0x8000 - 1;
				break;
				case DEVICE_ID_W25Q256:
						PutStr("W25Q256", 1);
						gQspi_sa_size    = SA_64KB;
						gQspi_end_addess = TOTAL_SIZE_32MB - 0x8000 - 1;
				break;
				case DEVICE_ID_W25M512JV:
						PutStr("W25M512JV", 1);
						gQspi_sa_size    = SA_64KB;
						gQspi_end_addess = TOTAL_SIZE_32MB - 0x8000 - 1;
				break;
				case DEVICE_ID_W25M512JW:
						PutStr("W25M512JW", 1);
						gQspi_sa_size    = SA_64KB;

						gQspi_end_addess = TOTAL_SIZE_32MB - 0x8000 - 1;
				break;
				case DEVICE_ID_W25Q512JV:
						PutStr("W25Q512JV", 1);
						gQspi_sa_size    = SA_64KB;
						gQspi_end_addess = TOTAL_SIZE_64MB - 0x8000 - 1;
				break;
				case DEVICE_ID_W25Q512JV_DTR:
						PutStr("W25Q512JV-DTR", 1);
						gQspi_sa_size    = SA_64KB;
						gQspi_end_addess = TOTAL_SIZE_64MB - 0x8000 - 1;
				break;
				default:
					ret = -1;
				break;
			}
		break;
		case MACRONIX_MANUFACTURER_ID:
			PutStr(" Macronix : ", 0);
			switch(deviceId)
			{
				case DEVICE_ID_MX25L12805:
						PutStr("MX25L12805", 1);
						gQspi_sa_size    = SA_64KB;
						gQspi_end_addess = TOTAL_SIZE_16MB - 0x8000 - 1;
				break;
				case DEVICE_ID_MX25L25645G:
						PutStr("MX25L25645G", 1);
						gQspi_sa_size    = SA_64KB;
						gQspi_end_addess = TOTAL_SIZE_32MB - 0x8000 - 1;
				break;
				case DEVICE_ID_MX25L51245G:
						PutStr("MX25L51245G", 1);
						gQspi_sa_size    = SA_64KB;
						gQspi_end_addess = TOTAL_SIZE_64MB - 0x8000 - 1;
				break;
				case DEVICE_ID_MX66U25635F:
						PutStr("MX66U25635F", 1);
						gQspi_sa_size    = SA_64KB;
						gQspi_end_addess = TOTAL_SIZE_32MB - 0x8000 - 1;
				break;
				case DEVICE_ID_MX66U51235F:
						PutStr("MX66U51235F", 1);
						gQspi_sa_size    = SA_64KB;
						gQspi_end_addess = TOTAL_SIZE_64MB - 0x8000 - 1;
				break;
				case DEVICE_ID_MX66UM1G45G:
						PutStr("MX66UM1G45G", 1);
						gQspi_sa_size    = SA_64KB;
						gQspi_end_addess = TOTAL_SIZE_128MB - 0x8000 - 1;
				break;
				case DEVICE_ID_MX66UW1G45G:
						PutStr("MX66UW1G45G", 1);
						gQspi_sa_size    = SA_64KB;
						gQspi_end_addess = TOTAL_SIZE_128MB - 0x8000 - 1;
				break;
				default:
					ret = -1;
				break;
			}
		break;
		case MICRON_MANUFACTURER_ID:
			PutStr(" Micron : ", 0);
			switch(deviceId)
			{
				case DEVICE_ID_MT25QL128:
						PutStr("MT25QL128", 1);
						gQspi_sa_size    = SA_64KB;
						gQspi_end_addess = TOTAL_SIZE_16MB - 0x8000 - 1;
				break;
				case DEVICE_ID_MT25QU128:
						PutStr("MT25QU128", 1);
						gQspi_sa_size    = SA_64KB;
						gQspi_end_addess = TOTAL_SIZE_16MB - 0x8000 - 1;
				break;
				case DEVICE_ID_MT25QL256:
						PutStr("MT25QL256", 1);
						gQspi_sa_size    = SA_64KB;
						gQspi_end_addess = TOTAL_SIZE_32MB - 0x8000 - 1;
				break;
				case DEVICE_ID_MT25QU256:
						PutStr("MT25QU256", 1);
						gQspi_sa_size    = SA_64KB;
						gQspi_end_addess = TOTAL_SIZE_32MB - 0x8000 - 1;
				break;
				case DEVICE_ID_MT25QL512:
						PutStr("MT25QL512", 1);
						gQspi_sa_size    = SA_64KB;
						gQspi_end_addess = TOTAL_SIZE_64MB - 0x8000 - 1;
				break;
				case DEVICE_ID_MT25QU512:
						PutStr("MT25QU512", 1);
						gQspi_sa_size    = SA_64KB;
						gQspi_end_addess = TOTAL_SIZE_64MB - 0x8000 - 1;
				break;
				case DEVICE_ID_MT25QL01G:
						PutStr("MT25QL01G", 1);
						gQspi_sa_size    = SA_64KB;
						gQspi_end_addess = TOTAL_SIZE_128MB - 0x8000 - 1;
				break;
				case DEVICE_ID_MT25QU01G:
						PutStr("MT25QU01G", 1);
						gQspi_sa_size    = SA_64KB;
						gQspi_end_addess = TOTAL_SIZE_128MB - 0x8000 - 1;
				break;
				case DEVICE_ID_MT25QL02G:
						PutStr("MT25QL02G", 1);
						gQspi_sa_size    = SA_64KB;
						gQspi_end_addess = TOTAL_SIZE_256MB - 0x8000 - 1;
				break;
				case DEVICE_ID_MT25QU02G:
						PutStr("MT25QU02G", 1);
						gQspi_sa_size    = SA_64KB;
						gQspi_end_addess = TOTAL_SIZE_256MB - 0x8000 - 1;
				break;
				default:
					ret = -1;
				break;
			}
		break;
		case DIALOG_MANUFACTURER_ID:
			PutStr(" Dialog : ", 0);
			switch(deviceId)
			{
				case DEVICE_ID_AT25QL128A:
						PutStr("AT25QL128A", 1);
						gQspi_sa_size    = SA_64KB;
						gQspi_end_addess = TOTAL_SIZE_16MB - 0x8000 - 1;
				break;
				default:
					ret = -1;
				break;
			}
		break;
		default:
			ret = 1;
		break;
	}
	if (ret)
	{
		Data2HexAscii(readDevId, str, 4);
		PutStr(" FlashID = 0x", 0);
		PutStr(str, 1);
	}
	return ret;
}

//////////////////////////////////////////
// SetData
//////////////////////////////////////////
static void SetData(uint32_t *setAdd)
{
	char		buf[16], key[16], chCnt,chPtr;
	uint32_t	loop;
	uint32_t 	wrData;

	loop = 1;

	while(loop)
	{
		PutStr(" Set Data : ",0);
		GetStr(key, &chCnt);
		chPtr = 0;
		if (!GetStrBlk(key, buf, &chPtr, 0))
		{
			if (chPtr == 1)
			{
				/* Case Return */
			}
			else if (chPtr > (char)((SIZE_32BIT<<1)+1))
			{
				/* Case Data Size Over */
				PutStr("Syntax Error", 1);
			}
			else
			{
				if (HexAscii2Data((unsigned char*)buf, &wrData))
				{
					PutStr("Syntax Error", 1);
				}
				else
				{
					*setAdd = wrData;
					loop = 0;
				}
			}
		}
		else
		{
			PutStr("Syntax Error", 1);
		}
	}
}

//////////////////////////////////////////
// CheckDataChange
//////////////////////////////////////////
static int32_t CheckDataChange(uintptr_t checkAdd)
{
	uint32_t	data;
	uint32_t	change;
	char		str[64];
	char		keyBuf[32], chCnt;

	change = CHANGE_OFF;

	data = *(uint32_t *)checkAdd;

	Data2HexAscii(data,str,4);
	PutStr(str,1);

	while(1)
	{
		PutStr(" Change ?(Y/N)", 0);
		GetStr(keyBuf, &chCnt);
		if (chCnt ==1)
		{
			if ((keyBuf[0] == 'Y') || (keyBuf[0] == 'y'))
			{
				PutStr(" Please Input New Data ",1);
				SetData(&data);
				*((uint32_t*)checkAdd) = data;
				change = CHANGE_ON;
				break;
			}
			if ((keyBuf[0] == 'N') || (keyBuf[0] == 'n'))
			{
				break;
			}
		}
	}
	return(change);
}

//////////////////////////////////////////
// SetAddInputKey
//////////////////////////////////////////
static void SetAddInputKey(uint32_t *Address)
{
	char		str[64];
	char		buf[16],key[16],chCnt,chPtr;
	uint32_t	loop;
	uint32_t	wrData;

	loop = 1;
	while(loop)
	{
		PutStr("  Please Input : H'",0);
		GetStr(key,&chCnt);
		chPtr = 0;
		if (!GetStrBlk(key,buf,&chPtr,0))
		{
			if (chPtr == 1)
			{
				/* Case Return */
			}
			else if((buf[0]=='.'))
			{
				/* Case End */
				gUserPrgStartAdd = 0x40000000;
				loop = 0;
			}
			else if (chPtr > (char)((SIZE_32BIT<<1)+1))
			{
				/* Case Data Size Over */
				PutStr("Syntax Error",1);
			}
			else
			{
				if (HexAscii2Data((unsigned char*)buf,&wrData))
				{
					PutStr("Syntax Error",1);
				}
				else
				{
					if (wrData & 0x00000003)
					{
						PutStr("Memory Boundary Error",1);
					}
					else
					{
						*Address = wrData;
						loop = 0;
					}
				}
			}
		}
		else
		{
			PutStr("Syntax Error",1);
		}
	}
}


//////////////////////////////////////////
// SetSizeInputKey
//////////////////////////////////////////
static void SetSizeInputKey(uint32_t *size)
{
	char		str[64];
	char		buf[16],key[16],chCnt,chPtr;
	uint32_t	loop;
	uint32_t	wrData;

	loop = 1;
	while(loop)
	{
		PutStr("  Please Input : H'",0);
		GetStr(key,&chCnt);
		chPtr = 0;
		if (!GetStrBlk(key,buf,&chPtr,0))
		{
			if (chPtr == 1)
			{
				/* Case Return */
			} else if ((buf[0]=='.'))
			{
				/* Case End */
				loop = 0;
			}
			else if (chPtr > (char)((SIZE_32BIT<<1)+1))
			{
				/* Case Data Size Over */
				PutStr("Syntax Error",1);
			}
			else
			{
				if (HexAscii2Data((unsigned char*)buf,&wrData))
				{
					PutStr("Syntax Error",1);
				}
				else
				{
					*size = wrData;
					loop  = 0;
				}
			}
		}
		else
		{
			PutStr("Syntax Error",1);
		}
	}
}

//////////////////////////////////////////
// CkSpiFlashAllF
//////////////////////////////////////////
static int32_t CkSpiFlashAllF(int32_t sAdd,int32_t cap)
{
	uintptr_t	ckAdd;
	unsigned char	rdData;

	for (ckAdd = sAdd; ckAdd < (sAdd+cap); ckAdd++)
	{
		rdData = *((volatile unsigned char*)ckAdd);
		if (rdData != 0xFF)
		{
			return(1);
		}
	}
	return(0);
}

//////////////////////////////////////////
// CkQspiFlash1ClearSectorSize
//////////////////////////////////////////
static int32_t CkQspiFlash1ClearSectorSize(uint32_t rdBufAdd, uint32_t spiFlashStartAdd, uint32_t checkSize, uint32_t orgSpiFlashStartAdd, uint32_t orgSpiEndAdd)
{
	uint32_t	flashStatus, flashEraseFlg;
	uint32_t	deltaS, deltaE;
	char 		str1Buf[10], str2Buf[10];
	char		str[64];

	deltaS = orgSpiFlashStartAdd - spiFlashStartAdd;;
	deltaE = (spiFlashStartAdd + checkSize - orgSpiEndAdd - 1) & 0xFFFFFF00U;

	PutStr("SPI Data Clear(H'FF) Check :",0);
	if (gQspi_end_addess <= TOTAL_SIZE_16MB)
	{
		FastRdQspiFlash(spiFlashStartAdd, rdBufAdd, checkSize);
	}
	else
	{
		Fast4RdQspiFlash(spiFlashStartAdd, rdBufAdd, checkSize);
	}

	flashEraseFlg = 0;

	if (CkSpiFlashAllF(rdBufAdd, checkSize))
	{
		PutStr("H'",0);
		Data2HexAscii(spiFlashStartAdd, str1Buf, 4);
		PutStr(&str1Buf[0], 0);
		PutStr("-", 0);
		Data2HexAscii(((spiFlashStartAdd + checkSize) - 1), str2Buf, 4);
		PutStr(&str2Buf[0], 0);
		PutStr(",Clear OK?(y/n)", 0);

		if (WaitKeyIn_YorN())
		{
			DelStr(34);
			PutStr(" Exit ", 1);
			return(1);
		}
		DelStr(34);
		flashEraseFlg = 1;
	}
	else
	{
		PutStr(" OK ",1);
	}
	if (flashEraseFlg)
	{
		PutStr("H'",0);
		PutStr(&str1Buf[0], 0);
		PutStr("-",0);
		PutStr(&str2Buf[0], 0);
		PutStr(" Erasing.", 0);
   		SectorEraseQspi_Flash(spiFlashStartAdd, ((spiFlashStartAdd + checkSize) - 1));

		if (deltaS)
		{
			SaveDataWithBuffeQspiFlash(rdBufAdd, spiFlashStartAdd, deltaS);	// ReStore
		}
		if (deltaE)
		{
			SaveDataWithBuffeQspiFlash((rdBufAdd + checkSize - deltaE), (spiFlashStartAdd + checkSize - deltaE), deltaE);	// ReStore
		}
	}
	return(0);
}

//////////////////////////////////////////
// mem_copy
//////////////////////////////////////////
void mem_copy(uint32_t prgStartAd, uint32_t sector_Ad, uint32_t accessSize)
{
	uintptr_t	dst, src;

	dst = prgStartAd;
	src = sector_Ad;

	memcpy((void *)dst, (const void *)src, accessSize);
}

/****************************************************************
	MODULE			: dgG2InfoSpiflash0_BP		*
	FUNCTION		: read BP spi Spiflash memory	*
	COMMAND			: XINFO_BP			*
	INPUT PARAMETER		: XINFO_BP			*
*****************************************************************/
void dgG2InfoSpiflash0_BP(void)
{
	char		str[16];
	uint32_t	bootProgramSize;
	uint32_t	spiFlashStatAdd, rdBufstatAdd;
	uintptr_t	readAdd;

	PutStr("=== SPI Boot Parameter Information  ===", 1);
	InitRPC_Mode();
	if (CheckQspiFlashId())
	{
		return;		//Error abortt
	}
	spiFlashStatAdd = QSPI_BP_STARTAD;
	rdBufstatAdd    = WORK_SPI_LOAD_AREA;
	readAdd         = rdBufstatAdd;
	SectorRdQspiFlash(spiFlashStatAdd, rdBufstatAdd);

	bootProgramSize = *(uint32_t *)readAdd;	// Read Boot Program Size
	Data2HexAscii(bootProgramSize, str, SIZE_32BIT);
	PutStr(" Boot Program Size : H'", 0);
	PutStr(str, 1);
	PutStr("=======================================", 1);
}

/****************************************************************
	MODULE			: dgG2InfoSpiflash0_BP_S	*
	FUNCTION		: set BP spi Spiflash memory	*
	COMMAND			: XINFO_BP_S			*
	INPUT PARAMETER		: XINFO_BP_S		 	*
*****************************************************************/
void dgG2InfoSpiflash0_BP_S(void)
{
	char		str[16];
	uint32_t	change, size;
	uint32_t	spiFlashStatAdd,rdBufstatAdd;
	uintptr_t	signatureAdd, bootProgramAdd;

	change = CHANGE_OFF;

	PutStr("=== SPI Boot Parameter Information  ===",1);
	InitRPC_Mode();
	if(CheckQspiFlashId())
	{
		return;		//Error abortt
	}
	spiFlashStatAdd = QSPI_BP_STARTAD;
	rdBufstatAdd    = WORK_SPI_LOAD_AREA;

	SectorRdQspiFlash(spiFlashStatAdd, rdBufstatAdd);

	PutStr(" Boot Program Size : H'", 0);
	if (CHANGE_ON == CheckDataChange(rdBufstatAdd))
	{
		change = CHANGE_ON;
	}
	signatureAdd   = rdBufstatAdd + 0x1FC;	// Signature
	bootProgramAdd = rdBufstatAdd;		// Boot Program Size
	if ((change == CHANGE_ON) || (*(uint32_t *)signatureAdd != 0xAA55FFFFU))
	{
		size = *(uint32_t *)bootProgramAdd;
		size = (size + 3) & ~3;
		*(uint32_t *)bootProgramAdd = size;

		*(uint32_t *)signatureAdd = 0xAA55FFFFU;	/* Signature	*/

		PutStr("SPI Data Clear(H'FF):H'000000-", 0);
		Data2HexAscii(gQspi_sa_size - 1, str, SIZE_32BIT);
		PutStr(str, 1);
		PutStr("  Erasing.", 0);
		SectorEraseQspi_Flash(spiFlashStatAdd, spiFlashStatAdd + gQspi_sa_size - 1);

		PutStr("SAVE SPI-FLASH.......", 0);
		SaveDataWithBuffeQspiFlash(rdBufstatAdd, spiFlashStatAdd, gQspi_sa_size);
		PutStr(" complete!", 1);
	}
}

/********************************************************
	MODULE			: dgClearSpiflash0	*
	FUNCTION		: Clear Spiflash memory	*
	COMMAND			: CS			*
	INPUT PARAMETER		: CS			*
*********************************************************/
void dgClearSpiflash0(void)
{
	uint32_t	readManuId,readDevId;
	int32_t		Rtn;

	Rtn = NORMAL_END;

	PutStr("ALL ERASE SpiFlash memory ", 1);
	PutStr("Clear OK?(y/n)", 0);
	if (WaitKeyIn_YorN())
	{
		// Return1=N
		DelStr(14);
		return;
	}
	DelStr(14);
	InitRPC_Mode();

	if (CheckQspiFlashId())
	{
		return;		//Error abortt
	}
	PutStr(" ERASE QSPI-FLASH (60sec[typ])....",0);
	Rtn = BulkEraseQspiFlash();
	if (Rtn == NORMAL_END)
	{
		PutStr(" complete!", 1);
	}
	else
	{
		PutStr(" Fail!", 1);
	}
}

//////////////////////////////////////////
// XLoadSpiflash0_2
//////////////////////////////////////////
static void XLoadSpiflash0_2(uint32_t mode)
{
	char		str[64];
	uint32_t	readManuId, readDevId;

	uintptr_t	Load_workStartAdd, Load_workEndAdd;
	uint32_t	workAdd_Min, workAdd_Max;

	uint32_t	Read_workStartAdd;
	uint32_t	ClrSpiStartSecTopAdd, ClrSpiSecEndAdd;
	uint32_t	clearSize;

	uint32_t	MaskSectorSize;
	uint32_t	WriteDataStatAdd;
	uint32_t	PrgSpiStartAdd, PrgSpiEndAdd;
	uint32_t	saveSize;

	uint32_t	WrittenSize;
	uint32_t	RemainingSize;

	PutStr("===== Qspi writing of RZ/Five Board Command =============",1);
	PutStr("Load Program to Spiflash",1);
	PutStr("Writes to any of SPI address.",1);
	InitRPC_Mode();

	if(CheckQspiFlashId())
	{
		return;		//Error abortt
	}
	gUserPrgStartAdd = 0x0;
	PutStr((mode == 0U ? "Program Top Address & Qspi Save Address " : "Program size & Qspi Save Address "),1);

	gSpiFlashSvArea		= 3;
	Load_workStartAdd	= LS_WORK_DRAM_SADD;
	Load_workEndAdd		= LS_WORK_DRAM_EADD;

	PrgSpiStartAdd		= 0x0;
	gUserPrgStartAdd	= 0x0;

	if (0U == mode)
	{
		PutStr("===== Please Input Program Top Address ============",1);
		SetAddInputKey(&gUserPrgStartAdd);
	}
	else
	{
		PutStr("===== Please Input Program size ============",1);
		SetSizeInputKey(&gUserPrgSize);
	}

	PutStr(" ",1);
	PutStr("===== Please Input Qspi Save Address ===",1);
	SetAddInputKey(&PrgSpiStartAdd);

	if (gQspi_end_addess < PrgSpiStartAdd)
	{
			PutStr("Address Input Error", 1);
			Data2HexAscii(gQspi_end_addess, str, SIZE_32BIT);
			PutStr("Range of H'00000000 ~ H'", 0);
			PutStr(str, 1);
			return;
	}
	// WorkMemory CLEAR (Write H'FF)
	PutStr("Work RAM(H'", 0);
	Data2HexAscii(Load_workStartAdd, str, SIZE_32BIT);
	PutStr(str, 0);
	PutStr("-H'", 0);
	Data2HexAscii(Load_workEndAdd, str, SIZE_32BIT);
	PutStr(str, 0);
	PutStr(") Clear....",1);
	FillData32Bit((uint32_t *)Load_workStartAdd, (uint32_t *)Load_workEndAdd, 0xFFFFFFFF);

	if (0U == mode)
	{
		if (dgLS_Load_Offset(&workAdd_Max, &workAdd_Min, gUserPrgStartAdd))
		{
			return;
		}
	}
	else
	{
		char bin_data;
		uint32_t image_offset = 0U;
		PutStr("please send ! (binary)",1);

		while (image_offset < gUserPrgSize)
		{
			GetChar(&bin_data);
			*(uint8_t *)(Load_workStartAdd + image_offset) = bin_data;
			image_offset++;
		}
		workAdd_Min = Load_workStartAdd;
		workAdd_Max = Load_workStartAdd + gUserPrgSize - 1;
	}
	PrgSpiStartAdd = PrgSpiStartAdd + (workAdd_Min - Load_workStartAdd);
	PrgSpiEndAdd   = PrgSpiStartAdd + (workAdd_Max - workAdd_Min);
	saveSize       = (PrgSpiEndAdd-PrgSpiStartAdd) + 1;
	if (PrgSpiEndAdd > gQspi_end_addess)
	{
			PutStr("Program over size Error", 1);
			PutStr(" SpiFlashMemory Stat Address : H'", 0);
			Data2HexAscii(PrgSpiStartAdd, str, SIZE_32BIT);
			PutStr(str ,1);
			PutStr(" SpiFlashMemory End Address  : H'", 0);
			Data2HexAscii(PrgSpiEndAdd, str, SIZE_32BIT);
			PutStr(str, 1);
			return;
	}
	MaskSectorSize = (~gQspi_sa_size) + 1;

	WriteDataStatAdd     = workAdd_Min;
	ClrSpiStartSecTopAdd = PrgSpiStartAdd &   MaskSectorSize;
	ClrSpiSecEndAdd      = PrgSpiEndAdd   | ~(MaskSectorSize);

	clearSize = (ClrSpiSecEndAdd - ClrSpiStartSecTopAdd) + 1;

	Read_workStartAdd    = WORK_SPI_LOAD_AREA;

	if (CkQspiFlash1ClearSectorSize(Read_workStartAdd, ClrSpiStartSecTopAdd, clearSize, PrgSpiStartAdd, PrgSpiEndAdd))
	{
			return;
	}
	// SAVE QSPI-FLASH
	PutStr("SAVE SPI-FLASH.......",0);
	SaveDataWithBuffeQspiFlash(WriteDataStatAdd, PrgSpiStartAdd, saveSize);	//Manual Mode Single WriteBuffe
	PutStr("",1);
	PutStr("======= Qspi  Save Information  =================",1);
	PutStr(" SpiFlashMemory Stat Address : H'",0);
	Data2HexAscii(PrgSpiStartAdd, str, SIZE_32BIT);
	PutStr(str,1);
	PutStr(" SpiFlashMemory End Address  : H'",0);
	Data2HexAscii(PrgSpiEndAdd, str, SIZE_32BIT);
	PutStr(str,1);
	PutStr("===========================================================",1);
	PutStr("",1);
}

/****************************************************************
	MODULE		: dgG2LoadSpiflash0_3			*
	FUNCTION	: load Program to Spiflash memory	*
	COMMAND		: XLS3					*
	INPUT PARAMETER	: XLS3			 		*
*****************************************************************/
void dgG2LoadSpiflash0_3(void)
{
	XLoadSpiflash0_2(1U);
}

/************************************************************************
	MODULE			: dgG2LoadSpiflash0_2			*
	FUNCTION		: load Program to Spiflash memory	*
	COMMAND			: XLS2					*
	INPUT PARAMETER		: XLS2					*
*************************************************************************/
void dgG2LoadSpiflash0_2(void)
{
	XLoadSpiflash0_2(0U);
}

//////////////////////////////////////////
// dgLS_Load_Offset
//////////////////////////////////////////
static char dgLS_Load_Offset(uint32_t *maxADD ,uint32_t *minADD, uint32_t prgStartAdd)
{
	char		str[12];
	uint32_t	data;
	uint32_t	getByteCount, byteCount;
	uint32_t	loadGetCount,adByteCount,loadGetData,loadGetSum,loadGetCR;
	uint32_t	loadGetAddress, prevLoadAddress;
	uint32_t	loop, loop_S0, s0flag,errFlg, checkData,endFlg;
	uint32_t	workAdd_Min,workAdd_Max;

	unsigned char *ptr;

	prevLoadAddress = prgStartAdd;

	ptr = (unsigned char *)LS_WORK_DRAM_SADD;
	workAdd_Min = LS_WORK_DRAM_SADD;
	workAdd_Max = LS_WORK_DRAM_SADD;
	loop	= 1;
	loop_S0	= 1;
	errFlg	= 0;
	endFlg	= 0;
	checkData = 0xFF;

	PutStr("please send ! ('.' & CR stop load)",1);
	while(loop)
	{
		loop_S0 = 1;
		s0flag  = 0;
		while(1)
		{
			GetChar(str);
			if (*str == '.' || *str == 's' || *str == 'S')
			{
				break;
			}
		}
		if (*str == '.')
		{
			while(1)
			{
				GetChar(str);
				if (*str == CR_CODE)
				{
				 	return(1);
				}
			}
		}
		else if (*str == 's' || *str == 'S')
		{
			GetChar(str);
			switch(*str)
			{
				case '0':
					s0flag = 1;
					while(loop_S0)
					{
						GetChar(str);
						if ((*str == CR_CODE) || (*str == LF_CODE))
						{
							loop_S0 = 0;
						}
					}
				break;
				case '1':
					// S1:2Byte Address
					adByteCount = 2;
				break;
				case '2':
					// S2:3Byte Address
					adByteCount = 3;
				break;
				case '3':
					// S3:4Byte Address
					adByteCount = 4;
				break;
				case '7':
				case '8':
				case '9':
					endFlg = 1;
				break;
				default:
					errFlg = 1;
				break;
			}
		}
		if (endFlg == 1 || errFlg == 1)
		{
			while(1)
			{
				GetChar(str);
				if ((*str == CR_CODE) || (*str == LF_CODE))
				{
					*maxADD = workAdd_Max - 1;
					*minADD = workAdd_Min;
					return(0);
				}
			}
		}
		if (s0flag == 0)
		{
			//Get Byte count (addressByteCount + dataByteCount + sumCheckByteCount(=1) )
			getByteCount = 1;
			GetStr_ByteCount(str,getByteCount);
			HexAscii2Data((unsigned char*)str,&data);
			loadGetCount = data;

			//Get Address
			getByteCount   = adByteCount;
			GetStr_ByteCount(str, getByteCount);
			HexAscii2Data((unsigned char*)str,&data);
			loadGetAddress = data;
			if (prevLoadAddress != loadGetAddress)
			{
				memset(ptr, 0xff, (loadGetAddress - prevLoadAddress));
				ptr         += (loadGetAddress - prevLoadAddress);
				workAdd_Max += (loadGetAddress - prevLoadAddress);
			}
			loadGetCount = loadGetCount - getByteCount;  // Get Address byte count -
			//Get Data & Data write
			getByteCount = 1;
			for (byteCount = loadGetCount; loadGetCount > 1; loadGetCount = loadGetCount - 1)
			{
				GetStr_ByteCount(str, getByteCount);
				HexAscii2Data((unsigned char*)str, &data);
				loadGetAddress++;
				*ptr = (unsigned char)data;
				ptr++;
				workAdd_Max++;
			}
			prevLoadAddress = loadGetAddress;
			//Get sum
			getByteCount = 1;
			GetStr_ByteCount(str, getByteCount);
			HexAscii2Data((unsigned char*)str,&data);
			loadGetSum   = data;
			//Get CR code
			GetChar(str);
			loadGetCR = *str;
			if ((loadGetCR == CR_CODE) || (loadGetCR == LF_CODE))
			{
				loop=1;
			}
		}
	}
}
