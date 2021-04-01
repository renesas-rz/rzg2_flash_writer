/*
 * Copyright (c) 2015-2019, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#define SPIREG_CR3V 0x00800004			//Volatile Status and Configuration Registers (CR3V)

void Fast4RdQspiFlash(uint32_t sourceSpiAdd,uint32_t destinationAdd,uint32_t byteCount);
void FastRdQspiFlash(uint32_t sourceSpiAdd,uint32_t destinationAdd,uint32_t byteCount);
int32_t BulkEraseQspiFlash(void);
void PageProgramWithBuffeQspiFlashNoWait(uint32_t addr, uint32_t source_addr);
void PageProgramWithBuffeQspiFlash(uint32_t addr, uint32_t source_addr);
void ParameterSectorErase4kbQspiFlash(uint32_t addr);
void SaveDataWithBuffeQspiFlash(uint32_t srcAdd,uint32_t svFlashAdd,uint32_t svSize);
void SectorEraseQspi_Flash(uint32_t EraseStatAdd, uint32_t EraseEndAdd);
void ParameterSectorEraseQspiFlash(uint32_t EraseStatAdd,uint32_t EraseEndAdd);
void SectorRdQspiFlash(uint32_t spiStatAdd, uint32_t distRamAdd);
