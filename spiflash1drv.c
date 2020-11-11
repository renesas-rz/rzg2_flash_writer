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
#include "bit.h"
#include "spiflash1drv.h"
#include "rpcqspidrv.h"
#include "dgtable.h"
#include "dgmodul4.h"

#define	DEBUG_MSG	(0)

extern uint32_t	gManufacturerId;
extern uint32_t	gDeviceId;
extern uint32_t	gQspi_sa_size;
extern uint32_t	gQspi_end_addess;

//////////////////////////////////
// Qspi:Fast Read  (4FAST_READ 0Ch)
//////////////////////////////////
void Fast4RdQspiFlash(uint32_t sourceSpiAdd,uint32_t destinationAdd,uint32_t byteCount)
{
	uint32_t sourceAdd;

#if (DEBUG_MSG)
	PutStr("## Fast4RdQspiFlash", 1);
#endif
	InitRPC_QspiFlash4FastReadExtMode();

	sourceAdd = SPI_IOADDRESS_TOP + sourceSpiAdd;
	mem_copy(destinationAdd, sourceAdd, byteCount);
}

//////////////////////////////////
// Qspi:Fast Read  (FAST_READ 0Bh)
//////////////////////////////////
void FastRdQspiFlash(uint32_t sourceSpiAdd,uint32_t destinationAdd,uint32_t byteCount)
{
	uint32_t sourceAdd;

#if (DEBUG_MSG)
	PutStr("## FastRdQspiFlash", 1);
#endif
	InitRPC_QspiFlashFastReadExtMode();

	sourceAdd = SPI_IOADDRESS_TOP + sourceSpiAdd;
	mem_copy(destinationAdd, sourceAdd, byteCount);
}

//////////////////////////////////////////
// Qspi:Sector Erase (64kB)	
//////////////////////////////////////////
static void SectorEraseQspiFlashInternal(uint32_t addr)
{
	uint32_t	status;

#if (DEBUG_MSG)
	PutStr("## SectorEraseQspiFlashInternal", 1);
#endif
	WriteCommandQspiFlash(0x00060000);	//WRITE ENABLE
	if (gQspi_end_addess <= TOTAL_SIZE_16MB)
	{
#if (DEBUG_MSG)
		PutStr("## SectorEraseQspiFlash", 1);
#endif
		// Sector Erase (D8h)
		SectorEraseQspiFlash(addr);
	}
	else
	{
#if (DEBUG_MSG)
		PutStr("## SectorErase4QspiFlash", 1);
#endif
		// Sector Erase with 4-Byte Address (DCh)
		SectorErase4QspiFlash(addr);
	}
	while(1)
	{
		ReadStatusQspiFlash(&status);
		if (!(status & BIT0))
		{
			break;
		}
	}
}

//////////////////////////////////////////
// Qspi:Bulk Erase (All)
//////////////////////////////////////////
int32_t BulkEraseQspiFlash(void)
{
	uint32_t	status;

#if (DEBUG_MSG)
	PutStr("## BulkEraseQspiFlash", 1);
#endif

	WriteCommandQspiFlash(0x00060000);	//WRITE ENABLE
	WriteCommandQspiFlash(0x00600000);	//Bulk Erase (BE 60h)
	while(1)
	{
		ReadStatusQspiFlash(&status);
		if (!(status & BIT0))
		{
			break;		//BIT0  1:Device Busy  0:Ready Device is in Standby
		}
	}
	return(NORMAL_END);
}

//////////////////////////////////////////
// Page Program
//////////////////////////////////////////
void PageProgramWithBuffeQspiFlash(uint32_t addr, uint32_t source_addr)
{
	uint32_t status;
#if (DEBUG_MSG)
	char	str[64];

	PutStr("## PageProgramWithBuffeQspiFlash", 1);

	PutStr("addr : 0x", 0);
	Data2HexAscii(addr, str, 4);
	PutStr(str, 1);
	PutStr("source_addr : 0x", 0);
	Data2HexAscii(source_addr, str, 4);
	PutStr(str, 1);
#endif
	WriteCommandQspiFlash(0x00060000);	//WRITE ENABLE
	if (gQspi_end_addess <= TOTAL_SIZE_16MB)
	{
#if (DEBUG_MSG)
		PutStr("## WriteDataPpWithBuffer", 1);
#endif
		//Page Program (PP:02h)  3-byte address
		WriteDataPpWithBufferQspiFlash(addr, source_addr);
	}
	else
	{
#if (DEBUG_MSG)
		PutStr("## WriteData4ppWithBufferQspiFlash", 1);
#endif
		// Page Program with 4-Byte Address (12h)
		WriteData4ppWithBufferQspiFlash(addr, source_addr);
	}
	while(1)
	{
		ReadStatusQspiFlash(&status);
		if (!(status & BIT0))
		{
			break;
		}
	}
}

///////////////////////////////////////////////
// Qspi:Parameter 4-kB Sector Erase
///////////////////////////////////////////////
void ParameterSectorErase4kbQspiFlash(uint32_t addr)
{
	uint32_t	status;
#if (DEBUG_MSG)
	char		str[64];

	PutStr("## ParameterSectorErase4kbQspiFlash", 1);
	PutStr("addr : 0x", 0);
	Data2HexAscii(addr, str, 4);
	PutStr(str, 1);
#endif
	WriteCommandQspiFlash(0x00060000);	//WRITE ENABLE
	if (gQspi_end_addess <= TOTAL_SIZE_16MB)
	{
		// 4KB Sector Erase (20h)
		ParameterSectorErase3QspiFlash(addr);
	}
	else
	{
		// 4KB Sector Erase with 4-Byte Address (21h)
		ParameterSectorErase4QspiFlash(addr);
	}
	while(1)
	{
		ReadStatusQspiFlash(&status);
		if (!(status & BIT0))
		{
			break;
		}
	}
}

///////////////////////////////////////////////////////////////
// SetSectorErase256kbQspiFlashCypress
///////////////////////////////////////////////////////////////
static void SetSectorErase256kbQspiFlashCypress(void)
{
	unsigned char	readData;
	uint32_t		status;
	uint32_t		addr;

	char str[64];

	addr = (uint32_t)SPIREG_CR3V;
	ReadAnyRegisterQspiFlash(addr, &readData);

	if (!(readData & BIT1))
	{
		WriteCommandQspiFlash(0x00060000);
		readData |= BIT1;			// Bit1=Block Erase Size  1:256KB , 0:64KB
		WriteAnyRegisterQspiFlash(addr, readData);
		while(1)
		{
			ReadStatusQspiFlash(&status);
			if (!(status & BIT0))
			{
				break;
			}
		}
	}
}

///////////////////////////////////////////////////////////////
// SetSectorErase64kbQspiFlashCypress
///////////////////////////////////////////////////////////////
static void SetSectorErase64kbQspiFlashCypress(void)
{
	unsigned char	readData;
	uint32_t		status;
	uint32_t		addr;

	char str[64];

	addr = (uint32_t)SPIREG_CR3V;
	ReadAnyRegisterQspiFlash(addr, &readData);

	if (readData & BIT1)
	{
		WriteCommandQspiFlash(0x00060000);
		readData &= ~BIT1;			// Bit1=Block Erase Size  1:256KB , 0:64KB
		WriteAnyRegisterQspiFlash(addr, readData);
		while(1)
		{
			ReadStatusQspiFlash(&status);
			if (!(status & BIT0))
			{
				break;
			}
		}
	}
}

//////////////////////////////////////////
// SaveDataWithBuffeQspiFlash
//////////////////////////////////////////
void SaveDataWithBuffeQspiFlash(uint32_t srcAdd, uint32_t svFlashAdd, uint32_t svSize)
{
#if (DEBUG_MSG)
	char		str[64];
#endif
	uint32_t	flashAdd;
	uint32_t	writeDataAdd;

#if (DEBUG_MSG)
	PutStr("## SaveDataWithBuffeQspiFlash", 1);

	PutStr("srcAdd : 0x", 0);
	Data2HexAscii(srcAdd, str, 4);
	PutStr(str, 1);
	PutStr("svFlashAdd : 0x", 0);
	Data2HexAscii(svFlashAdd, str, 4);
	PutStr(str, 1);
	PutStr("svSize : 0x", 0);
	Data2HexAscii(svSize, str, 4);
	PutStr(str, 1);
#endif
	WriteCommandQspiFlash(0x00060000);	//WRITE ENABLE

	writeDataAdd = srcAdd;
	for (flashAdd = svFlashAdd; flashAdd<(svFlashAdd+svSize); flashAdd += 256)
	{
		// 256byte:RPC Write Buffer size
		PageProgramWithBuffeQspiFlash(flashAdd, writeDataAdd);
		writeDataAdd = writeDataAdd + 256;
	}
}

//////////////////////////////////////////
// SectorEraseQspi_Flash
//////////////////////////////////////////
void SectorEraseQspi_Flash(uint32_t EraseStatAdd, uint32_t EraseEndAdd)
{
#if (DEBUG_MSG)
	char		str[64];
#endif
	uint32_t	sectorAd;
	uint32_t	SectorStatTopAdd, SectorEndTopAdd;
	SectorStatTopAdd = EraseStatAdd & ((~gQspi_sa_size) + 1);
	SectorEndTopAdd  = EraseEndAdd  & ((~gQspi_sa_size) + 1);

#if (DEBUG_MSG)
	PutStr("## SectorEraseQspi_Flash", 1);

	PutStr("SectorStatTopAdd : 0x", 0);
	Data2HexAscii(SectorStatTopAdd, str, 4);
	PutStr(str, 1);
	PutStr("SectorEndTopAdd : 0x", 0);
	Data2HexAscii(SectorEndTopAdd, str, 4);
	PutStr(str, 1);
	PutStr("SectorSize : 0x", 0);
	Data2HexAscii(gQspi_sa_size, str, 4);
	PutStr(str, 1);
#endif

	if ((gManufacturerId == CYPRESS_MANUFACTURER_ID ) && (gDeviceId == DEVICE_ID_S25FS128S))
	{
		if (gQspi_sa_size == SA_256KB)
		{
			PutStr("## 256KB Sector" , 1);
			SetSectorErase256kbQspiFlashCypress();
		}
		else
		{
			PutStr("## 64KB Sector" , 1);
			SetSectorErase64kbQspiFlashCypress();
		}
	}
	for (sectorAd = SectorStatTopAdd; sectorAd <= SectorEndTopAdd; sectorAd = sectorAd + gQspi_sa_size)
	{
		SectorEraseQspiFlashInternal(sectorAd);
		PutStr(".",0);
	}
	PutStr("Erase Completed ",1);
}

///////////////////////////////////////////////////////
// ParameterSectorEraseQspiFlash (4KB Sector Erase)
///////////////////////////////////////////////////////
void ParameterSectorEraseQspiFlash(uint32_t EraseStatAdd,uint32_t EraseEndAdd)
{
#if (DEBUG_MSG)
	char		str[64];
#endif
	uint32_t	sectorAd;
	uint32_t	SectorStatTopAdd,SectorEndTopAdd;

	SectorStatTopAdd = EraseStatAdd & 0xFFFFF000;
	SectorEndTopAdd  = EraseEndAdd  & 0xFFFFF000;
#if (DEBUG_MSG)
	PutStr("## ParameterSectorEraseQspiFlash", 1);

	PutStr("SectorStatTopAdd : 0x", 0);
	Data2HexAscii(SectorStatTopAdd, str, 4);
	PutStr(str, 1);
	PutStr("SectorEndTopAdd : 0x", 0);
	Data2HexAscii(SectorEndTopAdd, str, 4);
	PutStr(str, 1);
#endif

	for (sectorAd = SectorStatTopAdd;sectorAd <= SectorEndTopAdd; sectorAd = sectorAd + 0x1000)
	{
		ParameterSectorErase4kbQspiFlash(sectorAd);
		PutStr(".", 0);
	}
	PutStr("Erase Completed ",1);
}

//////////////////////////////////////////
// SectorRdQspiFlash
//////////////////////////////////////////
void SectorRdQspiFlash(uint32_t spiStatAdd, uint32_t distRamAdd)
{
#if (DEBUG_MSG)
	char		str[64];
#endif
	uint32_t	SectorStatTopAdd, readSize;

	SectorStatTopAdd = spiStatAdd & ((~gQspi_sa_size) + 1);
	readSize         = gQspi_sa_size;

#if (DEBUG_MSG)
	PutStr("## SectorRdQspiFlash", 1);

	PutStr("SectorStatTopAdd : 0x", 0);
	Data2HexAscii(SectorStatTopAdd, str, 4);
	PutStr(str, 1);
	PutStr("readSize : 0x", 0);
	Data2HexAscii(readSize, str, 4);
	PutStr(str, 1);
#endif
	if (gQspi_end_addess <= TOTAL_SIZE_16MB)
	{
		FastRdQspiFlash(SectorStatTopAdd, distRamAdd, readSize);
	}
	else
	{
		Fast4RdQspiFlash(SectorStatTopAdd, distRamAdd, readSize);
	}
}
