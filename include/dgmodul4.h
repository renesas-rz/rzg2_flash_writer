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

#define	CHANGE_OFF			0
#define	CHANGE_ON			1

#define	ADDITION			0x00000000
#define	SUBTRACTION			0x00000001

#define	SYSTEMRAM_SADD			0xE6300000
#define	SYSTEMRAM_IPL_SADD		0xE6302000
#define	PUBLICRAM_EADD			0xE635FFFF

#define	LS_WORK_DRAM_SADD		0x50000000
#define	LS_WORK_DRAM_EADD_192K		0x5002FFFF
#define	LS_WORK_DRAM_EADD_16K		0x50003FFF
#define	LS_WORK_DRAM_EADD_64M		0x53FFFFFF

#define	WORK_SPI_LOAD_AREA		0x58000000

#define	LS_WORK_SRAMBD_SADD		0x04000000
#define	LS_WORK_SRAMBD_EADD_192K	0x0402FFFF
#define	LS_WORK_SRAMBD_EADD_16M		0x04FFFFFF

//Serial Flash ROM
#define	QSPI_SA_SIZE			0x0040000

#define	QSPI_SA00_STARTAD		0x0000000
#define	QSPI_SA01_STARTAD		0x0040000
#define	QSPI_SA02_STARTAD		0x0080000
#define	QSPI_SA03_STARTAD		0x00C0000		//User Add,Size
#define	QSPI_SA04_STARTAD		0x0100000		//User Image
#define	QSPI_END_ADDRESS		0x3FF7FFF

#define	SPIBOOT_UPRG_ST_AD		0x00C0000
#define	SPIBOOT_UPRG_SIZE		0x00C0004

#define	SPIBOOT_BTROM_PARA		0x0000000		//Boot ROM Parameters (4Byte)
#define	SPIBOOT_A_IPL_ADD		0x0000D54		//A-side IPL address  (4Byte)
#define	SPIBOOT_A_IPL_SIZE		0x0000E64		//A-side IPL data size(4Byte)
#define	SPIBOOT_B_IPL_ADD		0x0001154		//B-side IPL address  (4Byte)
#define	SPIBOOT_B_IPL_SIZE		0x0001264		//B-side IPL data size(4Byte)

#define	SA_256KB			0x40000
#define	SA_64KB				0x10000

#define	TOTAL_SIZE_256MB		0x10000000
#define	TOTAL_SIZE_128MB		0x08000000
#define	TOTAL_SIZE_64MB			0x04000000
#define	TOTAL_SIZE_32MB			0x02000000
#define	TOTAL_SIZE_16MB			0x01000000

#define	CYPRESS_MANUFACTURER_ID		0x01	/* Cypress	*/
#define	WINBOND_MANUFACTURER_ID		0xEF	/* Winbond	*/
#define	MACRONIX_MANUFACTURER_ID	0xC2	/* Macronix	*/
#define	MICRON_MANUFACTURER_ID		0x20	/* Micron	*/

#define	DEVICE_ID_S25FS128S		0x2018
#define	DEVICE_ID_S25FS512S		0x0220

#define	DEVICE_ID_W25Q256		0x4019
#define	DEVICE_ID_W25M512JV		0x7119
#define	DEVICE_ID_W25M512JW		0x6119
#define	DEVICE_ID_W25Q512JV		0x4020
#define	DEVICE_ID_W25Q512JV_DTR		0x7020

#define	DEVICE_ID_MX25L12805		0x2018
#define	DEVICE_ID_MX25L25635F		0x2019
#define	DEVICE_ID_MX25L51245G		0x201A
#define	DEVICE_ID_MX66U25635F		0x2539
#define	DEVICE_ID_MX66U51235F		0x253A

#define	DEVICE_ID_MT25QL128		0xBA18
#define	DEVICE_ID_MT25QU128		0xBB18
#define	DEVICE_ID_MT25QL256		0xBA19
#define	DEVICE_ID_MT25QU256		0xBB19
#define	DEVICE_ID_MT25QL512		0xBA20
#define	DEVICE_ID_MT25QU512		0xBB20
#define	DEVICE_ID_MT25QL01G		0xBA21
#define	DEVICE_ID_MT25QU01G		0xBB21
#define	DEVICE_ID_MT25QL02G		0xBA22
#define	DEVICE_ID_MT25QU02G		0xBB22

void dgG2LoadSpiflash0(void);
void InitRPC_Mode(void);
uint32_t CheckQspiFlashId(void);
int32_t CkQspiFlash1ClearSectorSize(uint32_t rdBufAdd,uint32_t spiFlashStatAdd,uint32_t checkSize,uint32_t accessSize);
void mem_copy(uint32_t prgStartAd, uint32_t sector_Ad, uint32_t accessSize);
void dgG2LoadSpiflash0_2(void);
void dgG2LoadSpiflash0_3(void);
void XLoadSpiflash0_2(uint32_t mode);

void SetData(uint32_t *setAdd);
int32_t CkSpiFlashAllF(int32_t sAdd,int32_t cap);
void SetAddInputKey(uint32_t *Address);
void SetSizeInputKey(uint32_t *size);
void dgG2InfoSpiflash0_SA0(void);
int32_t CheckDataChange(uintptr_t checkAdd);
void dgG2InfoSetSpiflash0_SA0(void);
void dgG2InfoSpiflash0(void);
void dgG2InfoSetSpiflash0(void);
void dgClearSpiflash0(void);
char dgLS_Load_Offset2(uint32_t *maxADD ,uint32_t *minADD);
void dgDdrTest(void);
void dgRamTest(void);
