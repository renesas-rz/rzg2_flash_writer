/*
 * Copyright (c) 2015-2016, Renesas Electronics Corporation
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

#ifndef __DG_EMMC_H__
#define __DG_EMMC_H__

#define	SIZE2SECTOR(x)			( (x) >> 9 )	/* 512Byte		*/

#define	EMMC_MAX_SIZE			8		/* 8 Gbyte		*/
#define	EMMC_MAX_SECTOR			((SIZE2SECTOR( EMMC_MAX_SIZE * 1024 )) * 1024 * 1024 ) /* MAX SECTOR (8Gbyte) */
#define	MULTI_PARTITION_SIZE		(128 * 1024)	/* 128 Kbyte	*/

#define	EMMC_WORK_DRAM_SADD		0x50000000U
#define	EMMC_WORK_DRAM_EADD_2M		0x501FFFFFU
#define	EMMC_WORK_DRAM_EADD_4M		0x503FFFFFU
#define	EMMC_WORK_DRAM_EADD_16M		0x50FFFFFFU
#define	EMMC_WORK_DRAM_EADD_64M		0x53FFFFFFU
#define	EMMC_WORK_DRAM_EADD_512M	0x6FFFFFFFU
#define	EMMC_WORK_DRAM_SECTOR_MAX	((EMMC_WORK_DRAM_EADD_512M - EMMC_WORK_DRAM_SADD + 1)>>9)

#define	EMMC_SECURERAM_SADD		0xE6300000U
#define	EMMC_SECURERAM_EADD		0xE635FFFFU

#define	DMA_TRANSFER_SIZE		(0x20)		/* DMA Transfer size =  32 Bytes*/
#define	DMA_ROUNDUP_VALUE		(0xFFFFFFE0)

typedef enum
{
	EMMC_PARTITION_USER_AREA = 0,
	EMMC_PARTITION_BOOT_1,
	EMMC_PARTITION_BOOT_2,
	EMMC_PARTITION_MAX
} EMMC_PARTITION;

typedef enum
{
	EMMC_INPUT_SECTOR_END = 0,
	EMMC_INPUT_SECTOR_SIZE,
	EMMC_INPUT_SECTOR_ADDRESS
} EMMC_INPUT_SECTOR;

typedef enum
{
	EMMC_INPUT_TYPE_READ = 0,
	EMMC_INPUT_TYPE_WRITE,
	EMMC_INPUT_TYPE_ERASE
} EMMC_INPUT_TYPE;

typedef struct
{
	uint32_t	maxSectorCount[EMMC_PARTITION_MAX];
} EMMC_SECTOR;

typedef enum
{
	EMMC_WRITE_MOT = 0,
	EMMC_WRITE_BINARY,
} EMMC_WRITE_COMMAND;


extern void	dg_emmc_disp_cid(void);
extern void	dg_emmc_disp_csd(void);
extern void	dg_emmc_disp_ext_csd(void);
extern void	dg_emmc_set_ext_csd(void);

extern void dg_init_emmc(void);
extern unsigned long dg_emmc_check_init(void);
extern void	dg_emmc_write_mot(void);
extern void	dg_emmc_write_bin(void);
extern void dg_emmc_write_bin_serial(uint32_t* workStartAdd, uint32_t fileSize);
extern void	dg_emmc_erase(void);
extern int8_t dg_emmc_mot_load(uint32_t *maxADD ,uint32_t *minADD, uint32_t gUserPrgStartAdd );

extern uint32_t InputEmmcSector( EMMC_PARTITION partitionArea, uint32_t maxSectorCnt, uint32_t *startSector, uint32_t *sizeSector, EMMC_INPUT_TYPE type );
extern uint32_t InputEmmcSectorArea( EMMC_PARTITION *partitionArea );
extern uint32_t InputEmmcPrgStartAdd( uint32_t *prgStartAdd );
extern uint32_t InputFileSize( uint32_t *fileSize );
extern int32_t ChkSectorSize( uint32_t maxSectorCnt, uint32_t startSector, uint32_t sizeSector );
extern void SetSectorData(EMMC_SECTOR *sectorData);
extern void DispAreaData(EMMC_SECTOR sectorData);

#endif /* __DG_EMMC_H__ */
