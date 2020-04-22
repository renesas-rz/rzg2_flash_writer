/*
 * Copyright (c) 2015-2019, Renesas Electronics Corporation
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *   - Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *
 *   - Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *
 *   - Neither the name of Renesas nor the names of its contributors may be
 *     used to endorse or promote products derived from this software without
 *     specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include "common.h"
#include "dgtable.h"
#include "dgmodul4.h"
#include "rpcqspidrv.h"
#include "spiflash1drv.h"
#include "reg_rzg2.h"
#include "ramckmdl.h"
#include "dmaspi.h"
#include "devdrv.h"
#include "dgmodul1.h"
#include "boardid.h"
#if USB_ENABLE == 1
#include "usb_lib.h"
#endif /* USB_ENABLE == 1 */

uint32_t	gSpiFlashSvArea;
uint32_t	gUserPrgStartAdd;
uint32_t	gUserPrgSize;

uint32_t	gManufacturerId;
uint32_t	gDeviceId;
uint32_t	gQspi_sa_size;
uint32_t	gQspi_end_addess;

uintptr_t	gErrDdrAdd;
uint32_t	gErrDdrData,gTrueDdrData;
uintptr_t	gSubErrAdd;
uintptr_t	gSubErrData;
uintptr_t	gSubTrueData;

extern char gKeyBuf[64];

/****************************************************************
	MODULE			: dgG2LoadSpi			*
	FUNCTION		: load Program to Spi  memory	*
	COMMAND			: XLS				*
	INPUT PARAMETER		: XLS				*
*****************************************************************/
void dgG2LoadSpiflash0(void)
{
	char		str[64];
	uint32_t	OnBoardSpiSysSize;

	char		buf[16],key[16],chCnt,chPtr;
	uint32_t	readManuId,readDevId;
	uint32_t	loop;
	uint32_t	wrData;

	uintptr_t	Load_workStartAdd,Load_workEndAdd;
	uint32_t	workAdd_Min,workAdd_Max;

	uint32_t	Read_workStartAdd;
	uint32_t	ClrSpiStartSecTopAdd,ClrSpiSecEndAdd;
	uint32_t	clearSize;

	uint32_t	MaskSectorSize;
	uint32_t	WriteDataStatAdd;
	uint32_t	PrgSpiStatAdd,PrgSpiEndAdd;
	uint32_t	saveSize;

	uint32_t	InfoPrgStatAdd;
	uint32_t	rdBufstatAdd;
	uintptr_t	prgStAdd,prgSize;

	uint32_t	WrittenSize;
	uint32_t	RemainingSize;

	PutStr("===== Qspi writing of RZ/G2 Board Command ========",1);
	PutStr("Load Program to Spiflash",1);
	InitRPC_Mode();
	if (CheckQspiFlashId())
	{
		return;		//Error abortt
	}
	PutStr("------------------------------------------------------------",1);
	PutStr("Please select,Qspi Save Area. ",1);
	PutStr(" ",1);
	PutStr("== Loader Program : Program to execute on SystemRAM ========",1);
	PutStr("   1 : A-Side SPI_Address = H' 004_0000-H' 007_FFFF         ",1);
	PutStr("   2 : B-Side SPI_Address = H' 008_0000-H' 00B_FFFF         ",1);
	PutStr(" ",1);
	PutStr("== User Program : Program to execute on DRAM or SystemRAM ==",1);
	PutStr("   3 :        SPI_Address = H' 010_0000-H' 3FF_FFFF         ",1);
	PutStr("------------------------------------------------------------",1);

	loop = 1;
	while(loop)
	{
		PutStr("  Select area(1-3)>",0);
		GetStr(str,&chCnt);
		switch(str[0])
		{
			case '1':			//LoaderProgram
				gSpiFlashSvArea		= 1;
				Load_workStartAdd	= LS_WORK_DRAM_SADD;
				Load_workEndAdd		= LS_WORK_DRAM_EADD_192K;
				PrgSpiStatAdd		= QSPI_SA01_STARTAD;
				loop = 0;
			break;
			case '2':			//LoaderProgram
				gSpiFlashSvArea		= 2;
				Load_workStartAdd	= LS_WORK_DRAM_SADD;
				Load_workEndAdd		= LS_WORK_DRAM_EADD_192K;
				PrgSpiStatAdd		= QSPI_SA02_STARTAD;
				loop = 0;
			break;
			case '3':			//UserProgram
				gSpiFlashSvArea		= 3;
				Load_workStartAdd	= LS_WORK_DRAM_SADD;
				Load_workEndAdd		= LS_WORK_DRAM_EADD_64M;
				PrgSpiStatAdd		= QSPI_SA04_STARTAD;
				loop = 0;
			break;
		}
	}
	if (gSpiFlashSvArea == 1 || gSpiFlashSvArea ==2 )
	{
		gUserPrgStartAdd = SYSTEMRAM_IPL_SADD;
		PutStr("-- Loader Program --------------------------",1);
	}
	else if (gSpiFlashSvArea == 3)
	{
		PutStr("-- User Program ----------------------------",1);
		loop = 1;
		while(loop)
		{
			PutStr("Please Input User Program Start Address : ",0);
			GetStr(key,&chCnt);
			chPtr = 0;
			if (!GetStrBlk(key,buf,&chPtr,0))
			{
				if (chPtr == 1)
				{
					/* Case Return */
				}
				else if ((buf[0] == '.'))
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
							gUserPrgStartAdd = wrData;
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
	//=====================================================================================
	//	InfoPrgStatAdd = QSPI_SA03_STARTAD;
	//	InfoPrgSizeAdd = QSPI_SA03_STARTAD + 0x04;
	//=====================================================================================
	// WorkMemory CLEAR (Write H'FF)
	if (gSpiFlashSvArea == 1 || gSpiFlashSvArea == 2)
	{
		//Loader Area
		PutStr("Work RAM(H'50000000-H'5002FFFF) Clear....",1);
	}
	else if (gSpiFlashSvArea == 3)
	{
		PutStr("Work RAM(H'50000000-H'53FFFFFF) Clear....",1);
	}
	FillData32Bit((uint32_t *)Load_workStartAdd,(uint32_t *)Load_workEndAdd,0xFFFFFFFF);

	if (dgLS_Load_Offset2(&workAdd_Max ,&workAdd_Min))
	{
		return;
	}
	PrgSpiStatAdd  = PrgSpiStatAdd + (workAdd_Min - Load_workStartAdd);
	PrgSpiEndAdd   = PrgSpiStatAdd + (workAdd_Max - workAdd_Min);
	saveSize       = (PrgSpiEndAdd-PrgSpiStatAdd)+1;
	MaskSectorSize = (~gQspi_sa_size) + 1;

	WriteDataStatAdd     = workAdd_Min;
	ClrSpiStartSecTopAdd = PrgSpiStatAdd &   MaskSectorSize;
	ClrSpiSecEndAdd      = PrgSpiEndAdd  | ~(MaskSectorSize);

	clearSize = (ClrSpiSecEndAdd-ClrSpiStartSecTopAdd)+1;

	Read_workStartAdd    = WORK_SPI_LOAD_AREA;

	if (CkQspiFlash1ClearSectorSize(Read_workStartAdd,ClrSpiStartSecTopAdd,clearSize,1))
	{
			return;
	}

	// SAVE QSPI-FLASH
	PutStr("SAVE SPI-FLASH.......",0);
	SaveDataWithBuffeQspiFlash(WriteDataStatAdd, PrgSpiStatAdd, saveSize);	//Manual Mode Single WriteBuffe
	PutStr("-- Save (Program Start Address & Size ) -----",1);
	if (gSpiFlashSvArea == 1)
	{
		//A-Side IPL
		InfoPrgStatAdd	= QSPI_SA00_STARTAD;
		prgStAdd	= Read_workStartAdd + SPIBOOT_A_IPL_ADD;
		prgSize		= Read_workStartAdd + SPIBOOT_A_IPL_SIZE;
	}
	else if (gSpiFlashSvArea == 2)
	{
		//B-Side IPL
		InfoPrgStatAdd	= QSPI_SA00_STARTAD;
		prgStAdd	= Read_workStartAdd + SPIBOOT_B_IPL_ADD;
		prgSize		= Read_workStartAdd + SPIBOOT_B_IPL_SIZE;
	}
	else if (gSpiFlashSvArea == 3)
	{
		//User Program Area
		InfoPrgStatAdd	= QSPI_SA03_STARTAD;
		prgStAdd	= Read_workStartAdd + SPIBOOT_UPRG_ST_AD;
		prgSize		= Read_workStartAdd + SPIBOOT_UPRG_SIZE;
   	}
	rdBufstatAdd      = Read_workStartAdd + InfoPrgStatAdd;
	OnBoardSpiSysSize = 0x2000;

	if ((gSpiFlashSvArea == 1) || (gSpiFlashSvArea == 2))
	{
		Fast4RdQspiFlash(InfoPrgStatAdd, rdBufstatAdd, OnBoardSpiSysSize);
		PutStr("SPI Data Clear(H'FF):H'000000-03FFFF Erasing.", 0);
		ParameterSectorEraseQspiFlash(InfoPrgStatAdd, ((OnBoardSpiSysSize) - 1));
	}
	else
	{
		SectorRdQspiFlash(InfoPrgStatAdd, rdBufstatAdd);
		PutStr("SPI Data Clear(H'FF):H'0C0000-0FFFFF Erasing.", 0);
		SectorEraseQspi_Flash(InfoPrgStatAdd, ((InfoPrgStatAdd + 0x8) -1));
	}

	*((uint32_t*)prgStAdd) = gUserPrgStartAdd;
	saveSize               = (saveSize|0x3)>>2;
	*((uint32_t*)prgSize)  = saveSize;

	PutStr("SAVE SPI-FLASH.......",0);
	if ((gSpiFlashSvArea == 1) || (gSpiFlashSvArea == 2))
	{
			SaveDataWithBuffeQspiFlash(rdBufstatAdd, InfoPrgStatAdd, OnBoardSpiSysSize);	//Manual Mode Single WriteBuffe
	}
	else
	{
			SaveDataWithBuffeQspiFlash(rdBufstatAdd, InfoPrgStatAdd, gQspi_sa_size);	//Manual Mode Single WriteBuffe
	}
	PutStr(" complete!",1);

	PutStr("",1);
	PutStr("==========  Qspi Save Information  =================",1);
	PutStr(" Program Start Address :  H'",0);
	Data2HexAscii(gUserPrgStartAdd,str,4);
	PutStr(str,1);
	Data2HexAscii(saveSize,str,4);
	PutStr(" Program Size (Byte/4) :  H'",0);
	PutStr(str,1);
	PutStr("===============================================================",1);
	PutStr("",1);
}

void InitRPC_Mode(void)
{
	InitRPC_QspiFlash();
}

uint32_t CheckQspiFlashId(void)
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
				case DEVICE_ID_MX25L25635F:
						PutStr("MX25L25635F", 1);
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

int32_t CkQspiFlash1ClearSectorSize(uint32_t rdBufAdd,uint32_t spiFlashStatAdd,uint32_t checkSize,uint32_t accessSize)
{
	uint32_t	flashStatus,flashEraseFlg;
	char 		str1Buf[10],str2Buf[10];
	char		str[64];

	PutStr("SPI Data Clear(H'FF) Check :",0);
	Fast4RdQspiFlash(spiFlashStatAdd, rdBufAdd,checkSize);

	flashEraseFlg = 0;

	if (CkSpiFlashAllF(rdBufAdd,checkSize))
	{
		PutStr("H'",0);
		Data2HexAscii(spiFlashStatAdd,str1Buf,4);
		PutStr(&str1Buf[0],0);
		PutStr("-",0);
		Data2HexAscii(((spiFlashStatAdd+checkSize)-1),str2Buf,4);
		PutStr(&str2Buf[0],0);
		PutStr(",Clear OK?(y/n)",0);

		if (WaitKeyIn_YorN())
		{
		    DelStr(34);
			PutStr(" Exit ",1);
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
		// FLASH: erase
	   	if (spiFlashStatAdd < 0x40000)
	   	{
			//Parameter Data Area Erase (H'0-H'7FFF)
			PutStr("H'00000000-H'00007FFF",0);
			PutStr(" Erasing.",1);
			ParameterSectorEraseQspiFlash(0x0, 0x7FFF);
	   	}
		PutStr("H'",0);
		PutStr(&str1Buf[0],0);
		PutStr("-",0);
		PutStr(&str2Buf[0],0);
		PutStr(" Erasing.",0);
   		SectorEraseQspi_Flash(spiFlashStatAdd, ((spiFlashStatAdd + checkSize) - 1));	//SPI-FLASH-Address H'0000-H'FFFF
	}
	return(0);
}

void mem_copy(uint32_t prgStartAd, uint32_t sector_Ad, uint32_t accessSize)
{
	uintptr_t	readAdd, wrAdd;
	uint32_t	paddingOffset = 0;
	uint32_t	accessCount   = 0;

	paddingOffset = (accessSize + 0xFF ) & ~0xFF;

	//accessCount = accessSize/64;
	accessCount = paddingOffset >> 6;
	//DMA Setting
	InitDma01_Data(prgStartAd, sector_Ad, accessCount);
	StartDma01();
	WaitDma01();
	DisableDma01();
	ClearDmaCh01();
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

void XLoadSpiflash0_2(uint32_t mode)
{
	char		str[64];
	uint32_t	readManuId,readDevId;

	uintptr_t	Load_workStartAdd,Load_workEndAdd;
	uint32_t	workAdd_Min,workAdd_Max;

	uint32_t	Read_workStartAdd;
	uint32_t	ClrSpiStartSecTopAdd,ClrSpiSecEndAdd;
	uint32_t	clearSize;

	uint32_t	MaskSectorSize;
	uint32_t	WriteDataStatAdd;
	uint32_t	PrgSpiStatAdd,PrgSpiEndAdd;
	uint32_t	saveSize;

	uint32_t	WrittenSize;
	uint32_t	RemainingSize;

	PutStr("===== Qspi writing of RZ/G2 Board Command =============",1);
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
	Load_workEndAdd		= LS_WORK_DRAM_EADD_64M;

	PrgSpiStatAdd		= 0x0;
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
	SetAddInputKey(&PrgSpiStatAdd);

	if (gQspi_end_addess < PrgSpiStatAdd)
	{
			PutStr("Address Input Error", 1);
			PutStr("Range of H'000_0000 ~ H'0FF_7FFF", 1);
			return;
	}
	// WorkMemory CLEAR (Write H'FF)
	PutStr("Work RAM(H'50000000-H'53FFFFFF) Clear....",1);
	FillData32Bit((uint32_t *)Load_workStartAdd,(uint32_t *)Load_workEndAdd,0xFFFFFFFF);

	if (0U == mode)
	{
		if (dgLS_Load_Offset2(&workAdd_Max, &workAdd_Min))
		{
			return;
		}
	}
	else
	{
		char bin_data;
		uint32_t image_offset = 0U;
		PutStr("please send ! (binary)",1);

#if USB_ENABLE == 1
		if (gTerminal == USB_TERMINAL)
		{
			image_offset = ((gUserPrgSize + (DMA_TRANSFER_SIZE-1)) & ~(DMA_TRANSFER_SIZE-1));
			USB_ReadDataWithDMA((unsigned long)Load_workStartAdd, image_offset);
		}
		else
		{
			while (image_offset < gUserPrgSize)
			{
				GetChar(&bin_data);
				*(uint8_t *)(Load_workStartAdd + image_offset) = bin_data;
				image_offset++;
			}
		}
#else  /* USB_ENABLE == 1 */
		while (image_offset < gUserPrgSize)
		{
			GetChar(&bin_data);
			*(uint8_t *)(Load_workStartAdd + image_offset) = bin_data;
			image_offset++;
		}
#endif /* USB_ENABLE == 1 */

		workAdd_Min = Load_workStartAdd;
		workAdd_Max = Load_workStartAdd + gUserPrgSize - 1;
	}
	PrgSpiStatAdd = PrgSpiStatAdd + (workAdd_Min - Load_workStartAdd);
	PrgSpiEndAdd  = PrgSpiStatAdd + (workAdd_Max - workAdd_Min);
	saveSize      = (PrgSpiEndAdd-PrgSpiStatAdd) + 1;
	if (PrgSpiEndAdd > gQspi_end_addess)
	{
			PutStr("Program over size Error", 1);
			PutStr(" SpiFlashMemory Stat Address : H'", 0);
			Data2HexAscii(PrgSpiStatAdd, str, 4);
			PutStr(str ,1);
			PutStr(" SpiFlashMemory End Address  : H'", 0);
			Data2HexAscii(PrgSpiEndAdd,str, 4);
			PutStr(str, 1);
			return;
	}
	MaskSectorSize = (~gQspi_sa_size) + 1;

	WriteDataStatAdd     = workAdd_Min;
	ClrSpiStartSecTopAdd = PrgSpiStatAdd &   MaskSectorSize;
	ClrSpiSecEndAdd      = PrgSpiEndAdd  | ~(MaskSectorSize);

	clearSize = (ClrSpiSecEndAdd-ClrSpiStartSecTopAdd)+1;

	Read_workStartAdd    = WORK_SPI_LOAD_AREA;

	if (CkQspiFlash1ClearSectorSize(Read_workStartAdd, ClrSpiStartSecTopAdd, clearSize,1))
	{
			return;
	}
	// SAVE QSPI-FLASH
	PutStr("SAVE SPI-FLASH.......",0);
	SaveDataWithBuffeQspiFlash(WriteDataStatAdd, PrgSpiStatAdd, saveSize);	//Manual Mode Single WriteBuffe
	PutStr("",1);
	PutStr("======= Qspi  Save Information  =================",1);
	PutStr(" SpiFlashMemory Stat Address : H'",0);
	Data2HexAscii(PrgSpiStatAdd,str,4);
	PutStr(str,1);
	PutStr(" SpiFlashMemory End Address  : H'",0);
	Data2HexAscii(PrgSpiEndAdd,str,4);
	PutStr(str,1);
	PutStr("===========================================================",1);
	PutStr("",1);
}

void SetData(uint32_t *setAdd)
{
	char		buf[16],key[16],chCnt,chPtr;
	uint32_t	loop;
	uint32_t 	wrData;

	loop = 1;

	while(loop)
	{
		PutStr(" Set Data : ",0);
		GetStr(key,&chCnt);
		chPtr = 0;
		if (!GetStrBlk(key,buf,&chPtr,0))
		{
			if (chPtr == 1)
			{
				/* Case Return */
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
					*setAdd = wrData;
					loop = 0;
				}
			}
		}
		else
		{
			PutStr("Syntax Error",1);
		}
	}
}

int32_t CkSpiFlashAllF(int32_t sAdd,int32_t cap)
{
	uintptr_t		ckAdd;
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

void SetAddInputKey(uint32_t *Address)
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

void SetSizeInputKey(uint32_t *size)
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


/****************************************************************
	MODULE			: dgG2InfoSpiflash0_SA0		*
	FUNCTION		: read SA0 spi Spiflash memory	*
	COMMAND			: XINFO_SA0			*
	INPUT PARAMETER		: XINFO_SA0			*
*****************************************************************/
void dgG2InfoSpiflash0_SA0(void)
{
	char		str[64];
	uint32_t	bootRomPara;
	uint32_t	prgStAdd,prgSize;
	uint32_t	prgStAdd_B,prgSize_B;
	uint32_t	readManuId,readDevId;
	uint32_t	spiFlashStatAdd,workTopAdd,rdBufstatAdd;
	uintptr_t	readAdd;

	PutStr("=== SPI SA0 System Area Information  ===",1);
	InitRPC_Mode();
	if (CheckQspiFlashId())
	{
		return;		//Error abortt
	}
	//Setting  SPI_Add, workRAM_Add
	spiFlashStatAdd = QSPI_SA00_STARTAD;
	workTopAdd      = WORK_SPI_LOAD_AREA;
	rdBufstatAdd    = workTopAdd + spiFlashStatAdd;

	SectorRdQspiFlash(spiFlashStatAdd, rdBufstatAdd);

	readAdd  = workTopAdd + SPIBOOT_BTROM_PARA;		bootRomPara    = *(uint32_t *)readAdd;	//Read Boot ROM Parameters Address
	readAdd  = workTopAdd + SPIBOOT_A_IPL_ADD;		prgStAdd       = *(uint32_t *)readAdd;	//Read Program Start Address
	readAdd  = workTopAdd + SPIBOOT_A_IPL_SIZE;		prgSize        = *(uint32_t *)readAdd;	//Read Program Size
	readAdd  = workTopAdd + SPIBOOT_B_IPL_ADD;		prgStAdd_B     = *(uint32_t *)readAdd;	//Read Program Start Address
	readAdd  = workTopAdd + SPIBOOT_B_IPL_SIZE;		prgSize_B      = *(uint32_t *)readAdd;	//Read Program Size

	Data2HexAscii(bootRomPara,str,4);	PutStr(" Boot ROM Parameters                        : H'", 0);	PutStr(str, 1);
	Data2HexAscii(prgStAdd,str,4);		PutStr(" A-side IPL Address   (Loader Address)      : H'", 0);	PutStr(str, 1);
	Data2HexAscii(prgSize,str,4);		PutStr(" A-side IPL data size (Loader size)         : H'", 0);	PutStr(str, 1);
	Data2HexAscii(prgStAdd_B,str,4);	PutStr(" B-side IPL Address   (Loader Address)      : H'", 0);	PutStr(str, 1);
	Data2HexAscii(prgSize_B,str,4);		PutStr(" B-side IPL data size (Loader size)         : H'", 0);	PutStr(str, 1);
	PutStr("=========================================================", 1);
}

int32_t CheckDataChange(uintptr_t checkAdd)
{
	uint32_t	data;
	uint32_t	change;
	char		str[64];
	char		keyBuf[32],chCnt;

	change = CHANGE_OFF;

	data = *(uint32_t *)checkAdd;

	Data2HexAscii(data,str,4);	PutStr(str,1);
	while(1)
	{
		PutStr(" Change ?(Y/N)",0);
		GetStr(keyBuf,&chCnt);
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

/****************************************************************
	MODULE			: dgG2InfoSpiflash0_SA0		*
	FUNCTION		: set SA0 spi Spiflash memory	*
	COMMAND			: XINFO_SA0_S			*
	INPUT PARAMETER		: XINFO_SA0_S		 	*
*****************************************************************/
void dgG2InfoSetSpiflash0_SA0(void)
{
	char		str[64];
	uint32_t	change;
	uint32_t	spiFlashStatAdd,workTopAdd,rdBufstatAdd;
	uintptr_t	readAdd;
	uint32_t	readManuId,readDevId;
	uint32_t	OnBoardSpiSysSize;

	change = CHANGE_OFF;

	PutStr("=== SPI SA0 System Area Information  ===",1);
	InitRPC_Mode();
	if(CheckQspiFlashId())
	{
		return;		//Error abortt
	}
	//Setting  SPI_Add, workRAM_Add
	spiFlashStatAdd = QSPI_SA00_STARTAD;
	workTopAdd      = WORK_SPI_LOAD_AREA;
	rdBufstatAdd    = workTopAdd + spiFlashStatAdd;

	OnBoardSpiSysSize    = 0x2000;

	Fast4RdQspiFlash(spiFlashStatAdd, rdBufstatAdd, OnBoardSpiSysSize);
	//Boot ROM Parameter
	readAdd  = workTopAdd + SPIBOOT_BTROM_PARA;
	PutStr(" Boot ROM Parameters                        : H'", 0);
	if (CHANGE_ON == CheckDataChange(readAdd))
	{
		change = CHANGE_ON;
	}
	//A-side IPL Address
	readAdd  = workTopAdd + SPIBOOT_A_IPL_ADD;
	PutStr(" A-side IPL Address   (Loader Address)      : H'", 0);
	if (CHANGE_ON == CheckDataChange(readAdd))
	{
		change = CHANGE_ON;
	}
	//A-side IPL data size
	readAdd  = workTopAdd + SPIBOOT_A_IPL_SIZE;
	PutStr(" A-side IPL data size (Loader size)         : H'", 0);
	if (CHANGE_ON == CheckDataChange(readAdd))
	{
		change = CHANGE_ON;
	}
	//B-side IPL Address
	readAdd  = workTopAdd + SPIBOOT_B_IPL_ADD;
	PutStr(" B-side IPL Address   (Loader Address)      : H'", 0);
	if (CHANGE_ON == CheckDataChange(readAdd))
	{
		change = CHANGE_ON;
	}
	//B-side IPL data size
	readAdd  = workTopAdd + SPIBOOT_B_IPL_SIZE;
	PutStr(" B-side IPL data size (Loader size)         : H'", 0);
	if (CHANGE_ON == CheckDataChange(readAdd))
	{
		change = CHANGE_ON;
	}
	if (change == CHANGE_ON)
	{
		PutStr("SPI Data Clear(H'FF):H'000000-007FFF Erasing.",0);
		ParameterSectorEraseQspiFlash(spiFlashStatAdd, ((OnBoardSpiSysSize) - 1));
		PutStr("SAVE SPI-FLASH.......", 0);
		SaveDataWithBuffeQspiFlash(rdBufstatAdd, spiFlashStatAdd, OnBoardSpiSysSize);	//Manual Mode Single WriteBuffe
		PutStr(" complete!", 1);
	}
}

/****************************************************************
	MODULE			: dgG2InfoSpiflash0		*
	FUNCTION		: read SA3 spi Spiflash memory	*
	COMMAND			: XINFO				*
	INPUT PARAMETER		: XINFO				*
*****************************************************************/
void dgG2InfoSpiflash0(void)
{
	char		str[64];
	uint32_t	prgStAdd,prgSize;
	uint32_t	readManuId,readDevId;
	uint32_t	spiFlashStatAdd,workTopAdd,rdBufstatAdd;
	uintptr_t	readAdd;

	PutStr("=== SPI SA3 System Area Information  ===",1);
	InitRPC_Mode();
	if(CheckQspiFlashId())
	{
		return;		//Error abortt
	}
	//Setting  SPI_Add, workRAM_Add
	spiFlashStatAdd = QSPI_SA03_STARTAD;
	workTopAdd      = WORK_SPI_LOAD_AREA;
	rdBufstatAdd    = workTopAdd + spiFlashStatAdd;

	SectorRdQspiFlash(spiFlashStatAdd, rdBufstatAdd);
	//Read Program Start Address
	readAdd  = workTopAdd + SPIBOOT_UPRG_ST_AD;
	prgStAdd = *(uint32_t *)readAdd;
	//Read Program Size
	readAdd  = workTopAdd + SPIBOOT_UPRG_SIZE;
	prgSize  = *(uint32_t *)readAdd;

	Data2HexAscii(prgStAdd,str,4);
	PutStr(" Program Start Address : H'",0);
	PutStr(str,1);
	Data2HexAscii(prgSize,str,4);
	PutStr(" Program Size (Byte/4) : H'",0);
	PutStr(str,1);
	PutStr("==================================================",1);
}



/****************************************************************
	MODULE			: dgG2InfoSetSpiflash0		*
	FUNCTION		: set SA3 spi Spiflash memory	*
	COMMAND			: XINFO_S			*
	INPUT PARAMETER		: XINFO_S			*
*****************************************************************/
void dgG2InfoSetSpiflash0(void)
{
	char		str[64];
	uint32_t	change;
	uint32_t	spiFlashStatAdd,workTopAdd,rdBufstatAdd,readAdd;
	uint32_t	readManuId,readDevId;

	change = CHANGE_OFF;

	PutStr("=== SPI SA3 System Area Information  ===",1);
	InitRPC_Mode();
	if(CheckQspiFlashId())
	{
		return;		//Error abortt
	}
	//Setting  SPI_Add, workRAM_Add
	spiFlashStatAdd = QSPI_SA03_STARTAD;
	workTopAdd      = WORK_SPI_LOAD_AREA;
	rdBufstatAdd    = workTopAdd + spiFlashStatAdd;

	SectorRdQspiFlash(spiFlashStatAdd, rdBufstatAdd);
	//Address
	readAdd  = workTopAdd + SPIBOOT_UPRG_ST_AD;
	PutStr(" Program Start Address : H'",0);
	if (CHANGE_ON == CheckDataChange(readAdd))
	{
		change = CHANGE_ON;
	}
	//Size
	readAdd  = workTopAdd + SPIBOOT_UPRG_SIZE;
	PutStr(" Program Size (Byte/4) : H'",0);
	if (CHANGE_ON == CheckDataChange(readAdd))
	{
		change=CHANGE_ON;
	}
	if (change == CHANGE_ON)
	{
		PutStr("SPI Data Clear(H'FF):H'0C0000-0FFFFF Erasing.",0);
		SectorEraseQspi_Flash(spiFlashStatAdd, ((spiFlashStatAdd + 0x4) - 1));

		PutStr("SAVE SPI-FLASH.......",0);
		SaveDataWithBuffeQspiFlash(rdBufstatAdd, spiFlashStatAdd, gQspi_sa_size);	//Manual Mode Single WriteBuffe
		PutStr(" complete!",1);
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
	char		str[64];
	uint32_t	readManuId,readDevId;
	int32_t		Rtn;

	Rtn = NORMAL_END;

	PutStr("ALL ERASE SpiFlash memory ",1);
	PutStr("Clear OK?(y/n)",0);
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
		PutStr(" complete!",1);
	}
	else
	{
		PutStr(" Fail!",1);
	}
}

char dgLS_Load_Offset2(uint32_t *maxADD ,uint32_t *minADD)
{
	char		str[12];
	uint32_t	data;
	uint32_t	getByteCount,byteCount;
	uint32_t	loadGetCount,adByteCount,loadGetData,loadGetSum,loadGetCR;
	uintptr_t	loadGetAddress;
	uint32_t	loop,loop_S0,s0flag,errFlg,checkData,endFlg;
	uint32_t	workAdd_Min,workAdd_Max;
	uint32_t	WorkStartAdd,Calculation;
	uint32_t	loadOffset;

	workAdd_Min = 0xFFFFFFFF;
	workAdd_Max = 0x00000000;

	WorkStartAdd = LS_WORK_DRAM_SADD;

	if (gSpiFlashSvArea == 1 || gSpiFlashSvArea == 2)
	{
		loadOffset  = gUserPrgStartAdd - WorkStartAdd;
		Calculation = SUBTRACTION;
	}
	if (gSpiFlashSvArea == 3)
	{
		if ((0x40000000 <= gUserPrgStartAdd) && (gUserPrgStartAdd < WorkStartAdd))
		{
			//H'40000000 =< gUserPrgStartAdd < H'50000000
			loadOffset  = WorkStartAdd - gUserPrgStartAdd ;
			Calculation = ADDITION;
   		}
		else if((WorkStartAdd <= gUserPrgStartAdd) && (gUserPrgStartAdd < 0xC0000000))
		{
			//H'50000000 =< gUserPrgStartAdd < H'C0000000
			loadOffset  = gUserPrgStartAdd - WorkStartAdd ;
			Calculation = SUBTRACTION;
   		}
		else if(( SYSTEMRAM_SADD<= gUserPrgStartAdd) && (gUserPrgStartAdd <= PUBLICRAM_EADD))
		{
			//H'E6300000 =< gUserPrgStartAdd < H'E635FFFF
			loadOffset  = gUserPrgStartAdd - WorkStartAdd;
			Calculation = SUBTRACTION;
   		}
		else
		{
			PutStr("ERROR Load file.   <Download  file  DRAM(H'40000000-H'BFFFFFFF) , RAM(H'E6300000-H'E635FFFF) ONLY > ",1);
			return(1);
		}
	}

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
					*maxADD = workAdd_Max;
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
			GetStr_ByteCount(str,getByteCount);
			HexAscii2Data((unsigned char*)str,&data);
			loadGetAddress = data;

			if(Calculation == SUBTRACTION)
			{
				loadGetAddress = loadGetAddress - loadOffset;
   			}
			else
			{
				loadGetAddress = loadGetAddress + loadOffset;
			}
			loadGetCount = loadGetCount - getByteCount;  // Get Address byte count -
			//Min Address Check
			if (loadGetAddress < workAdd_Min)
			{
				workAdd_Min = loadGetAddress;
			}
			//Get Data & Data write
			getByteCount = 1;
			for (byteCount = loadGetCount; loadGetCount > 1; loadGetCount=loadGetCount-1)
			{
				GetStr_ByteCount(str,getByteCount);
				HexAscii2Data((unsigned char*)str,&data);
				loadGetData    = data;
				*((unsigned char *)loadGetAddress) = loadGetData;
				loadGetAddress = loadGetAddress +1;
			}
			//Max Address Check
			if ((loadGetAddress-1) > workAdd_Max)
			{
				workAdd_Max = (loadGetAddress-1);
			}
			//Get sum
			getByteCount = 1;
			GetStr_ByteCount(str,getByteCount);
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

static uint32_t PutDdrErrInfo(void)
{
	char str[64];

	PutStr(" error address  : H'",0);	Data2HexAscii_64(gErrDdrAdd,str,CPU_BYTE_SIZE);	PutStr(str,1);
	PutStr(" error data     : H'",0);	Data2HexAscii(gErrDdrData,str,4);	PutStr(str,1);
	PutStr(" true  data     : H'",0);	Data2HexAscii(gTrueDdrData,str,4);	PutStr(str,1);
}

static uint32_t CkExtendDdrRamCheck(void* ramAddr)
{
	volatile uint32_t *read_adr;
	uint32_t data;
	uint32_t loop, i;
	char str[64];

	read_adr = (uint32_t *)ramAddr;

	PutStr("Data=0x5A5A5A5A",0);

	/* Write */
	data = 0x5A5A5A5A;
	for (loop = 0; loop < 0x100000; loop++)
	{
		read_adr[loop] = data;
	}
	/* Verify */
	data = 0x5A5A5A5A;
	for (loop = 0; loop < 0x100000; loop++)
	{
		if (read_adr[loop] != data)
		{
			gErrDdrAdd   = (uintptr_t)&read_adr[loop];
			gErrDdrData  = read_adr[loop];
			gTrueDdrData = data;
			return(ERROR_END);
		}
	}
	DelStr(15);
	PutStr("Data=0xA5A5A5A5",0);

	/* Write */
	data = 0xA5A5A5A5;
	for (loop = 0; loop < 0x100000; loop++)
	{
		read_adr[loop] = data;
	}
	/* Verify */
	data = 0xA5A5A5A5;
	for (loop = 0; loop < 0x100000; loop++)
	{
		if (read_adr[loop] != data)
		{
			gErrDdrAdd   = (uintptr_t)&read_adr[loop];
			gErrDdrData  = read_adr[loop];
			gTrueDdrData = data;
			return(ERROR_END);
		}
	}
	DelStr(15);
	PutStr("Data=0x12345678",0);
 
	/* Write */
	data = 0x12345678;
	for (loop = 0; loop < 0x100000; loop++)
	{
		read_adr[loop] = data;
		data += 0x11111111;
	}
	/* Verify */
	data = 0x12345678;
	for (loop = 0; loop < 0x100000; loop++)
	{
		if (read_adr[loop] != data)
		{
			gErrDdrAdd   = (uintptr_t)&read_adr[loop];
			gErrDdrData  = read_adr[loop];
			gTrueDdrData = data;
			return(ERROR_END);
		}
		data += 0x11111111;
	}
	DelStr(15);
	return(NORMAL_END);
}

static int32_t CheckAndFillData8Bit( uint8_t *startAddr, uint8_t *endAddr, uint8_t writeData, uint8_t checkData )
{
	volatile uint8_t *pData;
	pData = (uint8_t *)startAddr;
	while(1)
	{
		if (*pData!=checkData) break;  *(pData++) = writeData;
		if (*pData!=checkData) break;  *(pData++) = writeData;
		if (*pData!=checkData) break;  *(pData++) = writeData;
		if (*pData!=checkData) break;  *(pData++) = writeData;
		if (*pData!=checkData) break;  *(pData++) = writeData;
		if (*pData!=checkData) break;  *(pData++) = writeData;
		if (*pData!=checkData) break;  *(pData++) = writeData;
		if (*pData!=checkData) break;  *(pData++) = writeData;
		if (*pData!=checkData) break;  *(pData++) = writeData;
		if (*pData!=checkData) break;  *(pData++) = writeData;
		if (*pData!=checkData) break;  *(pData++) = writeData;
		if (*pData!=checkData) break;  *(pData++) = writeData;
		if (*pData!=checkData) break;  *(pData++) = writeData;
		if (*pData!=checkData) break;  *(pData++) = writeData;
		if (*pData!=checkData) break;  *(pData++) = writeData;
		if (*pData!=checkData) break;  *(pData++) = writeData;
		if (pData >= endAddr)  return NORMAL_END;
	}
	gSubErrAdd   = (uint64_t)pData;
	gSubErrData  = (uint64_t)*pData;
	gSubTrueData = (uint64_t)checkData;
	return ERROR_END;
}

static int32_t FillData8Bit(uint8_t *startAddr, uint8_t *endAddr, uint8_t writeData)
{
	volatile uint8_t *pData;
	pData = (uint8_t *)startAddr;
	while(1){
		*(pData++) = writeData;
		*(pData++) = writeData;
		*(pData++) = writeData;
		*(pData++) = writeData;
		*(pData++) = writeData;
		*(pData++) = writeData;
		*(pData++) = writeData;
		*(pData++) = writeData;
		*(pData++) = writeData;
		*(pData++) = writeData;
		*(pData++) = writeData;
		*(pData++) = writeData;
		*(pData++) = writeData;
		*(pData++) = writeData;
		*(pData++) = writeData;
		*(pData++) = writeData;
		if( pData >= endAddr )  return NORMAL_END;
	}
	return NORMAL_END;
}

static int32_t CheckData8Bit( uint8_t *startAddr, uint8_t *endAddr, uint8_t checkData )
{
	volatile uint8_t *pData;
	pData = (uint8_t *)startAddr;
	while(1)
	{
		if(*(pData++)!=checkData) break;
		if(*(pData++)!=checkData) break;
		if(*(pData++)!=checkData) break;
		if(*(pData++)!=checkData) break;
		if(*(pData++)!=checkData) break;
		if(*(pData++)!=checkData) break;
		if(*(pData++)!=checkData) break;
		if(*(pData++)!=checkData) break;
		if(*(pData++)!=checkData) break;
		if(*(pData++)!=checkData) break;
		if(*(pData++)!=checkData) break;
		if(*(pData++)!=checkData) break;
		if(*(pData++)!=checkData) break;
		if(*(pData++)!=checkData) break;
		if(*(pData++)!=checkData) break;
		if(*(pData++)!=checkData) break;
		if (pData >= endAddr)  return NORMAL_END;
	}
	pData--;
	gSubErrAdd   = (uint64_t)pData;
	gSubErrData  = (uint64_t)*pData;
	gSubTrueData = (uint64_t)checkData;
	return ERROR_END;
}

static int32_t WriteIncData8Bit( uint8_t *startAddr, uint8_t *endAddr, uint8_t startData )
{
	volatile uint8_t *pData;
	pData = (uint8_t *)startAddr;
	while(1)
	{
		*(pData++) = (startData++);
		*(pData++) = (startData++);
		*(pData++) = (startData++);
		*(pData++) = (startData++);
		*(pData++) = (startData++);
		*(pData++) = (startData++);
		*(pData++) = (startData++);
		*(pData++) = (startData++);
		*(pData++) = (startData++);
		*(pData++) = (startData++);
		*(pData++) = (startData++);
		*(pData++) = (startData++);
		*(pData++) = (startData++);
		*(pData++) = (startData++);
		*(pData++) = (startData++);
		*(pData++) = (startData++);
		if (pData >= endAddr)  return NORMAL_END;
	}
	return NORMAL_END;
}

static int32_t CheckIncData8Bit( uint8_t *startAddr, uint8_t *endAddr, uint8_t startData )
{
	volatile uint8_t *pData;
	pData = (uint8_t *)startAddr;
	while(1)
	{
		if(*(pData++)!=(startData++)) break;
		if(*(pData++)!=(startData++)) break;
		if(*(pData++)!=(startData++)) break;
		if(*(pData++)!=(startData++)) break;
		if(*(pData++)!=(startData++)) break;
		if(*(pData++)!=(startData++)) break;
		if(*(pData++)!=(startData++)) break;
		if(*(pData++)!=(startData++)) break;
		if(*(pData++)!=(startData++)) break;
		if(*(pData++)!=(startData++)) break;
		if(*(pData++)!=(startData++)) break;
		if(*(pData++)!=(startData++)) break;
		if(*(pData++)!=(startData++)) break;
		if(*(pData++)!=(startData++)) break;
		if(*(pData++)!=(startData++)) break;
		if(*(pData++)!=(startData++)) break;
		if( pData >= endAddr )  return NORMAL_END;
	}
	pData--;
	startData--;
	gSubErrAdd   = (uint64_t)pData;
	gSubErrData  = (uint64_t)*pData;
	gSubTrueData = (uint64_t)startData;
	return ERROR_END;
}

static char DecodeForm5(uintptr_t *para1st, uintptr_t *para2nd, uint32_t *setPara)
{
	char tmp[64],tmp2[64],chPtr,chPtr2,endCh,getCnt,value;
	uintptr_t tmpData;

	*setPara = 0;
	chPtr = getCnt = 0;
	do
	{
		endCh = GetStrBlk(gKeyBuf,tmp,&chPtr,0);
		switch(getCnt)
		{
			/*********** "RAMCK" Block parameter  *************/
			case 0:
				if (endCh == 0)
				{
					/* non set start Add */
					return(2);
				}
			break;
			/*********** Ana 1st parameter  *******************/
			case 1:
				value = HexAscii2Data_64((unsigned char*)tmp,para1st);
				if (value == 0)
				{
					*setPara |= 0x01;
				}
				else
				{
					return(1);
				}
			break;
			/*********** Ana 2nd parameter  *******************/
		 	case 2:
				value = HexAscii2Data_64((unsigned char*)tmp,&tmpData);
				if (value == 0)
				{
					*setPara |= 0x02;
					if (*para1st > tmpData)
					{
						return(2);
					}
					else
					{
						*setPara |= 0x02;
						*para2nd = (tmpData - *para1st) + 0x01 ;
				 	}
				}
				else if (value == 3)
				{
					/* find @ */
					chPtr2 = 1;
					GetStrBlk(tmp,tmp2,&chPtr2,0);
					if (HexAscii2Data_64((unsigned char*)tmp2,para2nd))
					{
						return(1);
					}
					*setPara |= 0x02;
				}
				else
				{
					return(1);
				}
			break;
		}
		if (endCh == ';')
		{
			if (GetStrBlk(gKeyBuf,tmp,&chPtr,0))
			{
				return(1);
			}
			endCh=0;
			*setPara |= 0x08;
		}
		getCnt++;
	} while(endCh);
	return(0);
}

static int32_t TPRAMCK( uint8_t *startAddr, uint8_t *endAddr )
{
	FillData8Bit(startAddr, endAddr, 0x00 );
	if (CheckAndFillData8Bit(startAddr, endAddr, 0x55, 0x00))
	{
		return ERROR_END;
	}
	if (CheckAndFillData8Bit(startAddr, endAddr, 0xAA, 0x55))
	{
		return ERROR_END;
	}
	if (CheckAndFillData8Bit(startAddr, endAddr, 0xFF, 0xAA))
	{
		return ERROR_END;
	}
	if (CheckData8Bit(startAddr, endAddr, 0xFF))
	{
		return ERROR_END;
	}
	WriteIncData8Bit(startAddr, endAddr, 0x00);
	if (CheckIncData8Bit(startAddr, endAddr, 0x00))
	{
		return ERROR_END;
	}
	return NORMAL_END;
}

void dgDdrTest(void)
{
	uint32_t readData;

	uint32_t product;

	product = *((volatile uint32_t*)PRR) & PRR_PRODUCT_MASK;

	PutStr("=== DDR R/W CHECK ====",1);
	switch (product)
	{
		case PRR_PRODUCT_G2E:
		case PRR_PRODUCT_G2N:
			if (product == PRR_PRODUCT_G2E)
			{
				PutStr("=== RZ/G2E (Memory controller is only channel 1) ===",1);
			}
			else
			{
				PutStr("=== RZ/G2N (Memory controller is only channel 1) ===",1);
			}
			readData = *((volatile uint32_t*)0x0000000410000000);	//Access Check
			PutStr("Check:0x04_10000000 ... ",0);
			if (CkExtendDdrRamCheck((void*)0x0000000410000000))
			{
				PutStr(" Fail!",1);
				PutDdrErrInfo();
				return;
			}
			else
			{
				PutStr(" Pass!",1);
			}
			PutStr("Check:0x04_40000000 ... ",0);
			if (CkExtendDdrRamCheck((void*)0x0000000440000000))
			{
				PutStr(" Fail!",1);
				PutDdrErrInfo();
				return;
			}
			else
			{
				PutStr(" Pass!",1);
			}
		break;
		case PRR_PRODUCT_G2H:
		case PRR_PRODUCT_G2M:
			if (product == PRR_PRODUCT_G2H)
			{
				PutStr("=== Memory map RZ/G2H ====",1);

			}
			else
			{
				PutStr("=== Memory map RZ/G2M ====",1);
			}
			//CH0 Check
			readData = *((volatile uint32_t*)0x0000000410000000);	//Access Check
			PutStr("Check:0x04_10000000 ... ",0);
			if (CkExtendDdrRamCheck((void*)0x0000000410000000))
			{
				PutStr(" Fail!",1);
				PutDdrErrInfo();
				PutStr("Next channel Test OK?(y/n)",0);
				if (WaitKeyIn_YorN())
				{
					// Return1=N
			    		DelStr(26);
			    		return;
				}
				DelStr(26);
			}
			else
			{
				PutStr(" Pass!",1);
			}
			//CH2 Check
			PutStr("Check:0x06_00000100 ... ",0);
			if (CkExtendDdrRamCheck((void*)0x0000000600000100))
			{
				PutStr(" Fail!",1);
				PutDdrErrInfo();
				return;
			}
			else
			{
				PutStr(" Pass!",1);
			}
		break;
		default:
		break;
	}
}

void dgRamTest(void)
{
	uint64_t ramck1st,ramck2nd;
	uint32_t setPara;

	char decRtn;
	char str[10];

	ramck1st=ramck2nd=0x0;
	decRtn = DecodeForm5(&ramck1st,&ramck2nd,&setPara);
	if (!(setPara&0x3))
	{
		PutStr("Syntax Error",1);	return;
	}
	else if (decRtn==1)
	{
		PutStr("Syntax Error",1);	return;
	}
	else if (decRtn==2)
	{
		PutStr("Address Size Error",1);	return;
	}
	else
	{
		PutStr("== RAM CHECK (Byte Access) ===",1);
		PutStr("- Marching Data Check --------",1);
		PutStr(" [ Write H'00               ]",1);
		PutStr(" [ Check H'00 -> Write H'55 ]",1);
		PutStr(" [ Check H'55 -> Write H'AA ]",1);
		PutStr(" [ Check H'AA -> Write H'FF ]",1);
		PutStr(" [ Check H'FF               ]",1);
		PutStr("- Decoder Pattern Check ------",1);
		PutStr(" [ Write H'00,H'01,H'02 ... ]",1);
		PutStr(" [ Check H'00,H'01,H'02 ... ]",1);
		PutStr("CHECK RESULT",0);
	}
	if (TPRAMCK( ((uint8_t *)ramck1st),((uint8_t *)(ramck1st+ramck2nd)) ) )
	{
		PutStr("---->NG",1);
		Data2HexAscii_64(gSubErrAdd,str,CPU_BYTE_SIZE);
		PutStr("ERROR ADDRESS:",0); PutStr(str,1);
		Data2HexAscii_64(gSubErrData,str,CPU_BYTE_SIZE);
		PutStr("ERROR DATA   :",0); PutStr(str,1);
		Data2HexAscii_64(gSubTrueData,str,CPU_BYTE_SIZE);
		PutStr("TRUE DATA    :",0); PutStr(str,1);
		return;
	}
	else
	{
		PutStr("---->OK",1);
	}
}
