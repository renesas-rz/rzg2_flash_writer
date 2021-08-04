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

#include "emmc_config.h"
#include "emmc_hal.h"
#include "emmc_std.h"
#include "emmc_def.h"
#include "emmc_registers.h"

#include "dgemmc.h"
#include "common.h"
#include "types.h"

#include "ramckmdl.h"
#include "devdrv.h"

#define	SIZE2SECTOR(x)			( (x) >> 9 )	/* 512Byte		*/

#define	EMMC_MAX_SIZE			8		/* 8 Gbyte		*/
#define	EMMC_MAX_SECTOR			((SIZE2SECTOR( EMMC_MAX_SIZE * 1024 )) * 1024 * 1024 ) /* MAX SECTOR (8Gbyte) */
#define	MULTI_PARTITION_SIZE		(128 * 1024)	/* 128 Kbyte	*/

#define BASEADDR_RAMB0		0xB6000000
#define	EMMC_WORK_RAMB_SADD BASEADDR_RAMB0
#define EMMC_WORK_RAMB_MAX_SIZ 0x100000 //1Mbyte
#define	EMMC_WORK_RAMB_EADD 0xB60FFFFF
#define	EMMC_WORK_RAMB_SECTOR_MAX	(EMMC_WORK_RAMB_MAX_SIZ >>9)

#define	EMMC_SECURERAM_SADD		0xE6300000U
#define	EMMC_SECURERAM_EADD		0xE635FFFFU

#define	DMA_TRANSFER_SIZE		(0x20)		/* DMA Transfer size =  32 Bytes*/
#define	DMA_ROUNDUP_VALUE		(0xFFFFFFE0)

#define DATA_RECV_TIMEOUT   3000*1000    /*3000ms*/

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
	EMMC_WRITE_BINARY,
} EMMC_WRITE_COMMAND;

static int32_t dg_emmc_write_bin_serial(uint32_t* workStartAdd, uint32_t fileSize);
static EMMC_ERROR_CODE dg_emmc_init(void);
static uint32_t InputEmmcSector( EMMC_PARTITION partitionArea, uint32_t maxSectorCnt, uint32_t *startSector, uint32_t *sizeSector, EMMC_INPUT_TYPE type );
static uint32_t InputEmmcSectorArea( EMMC_PARTITION *partitionArea );
static uint32_t InputFileSize( uint32_t *fileSize );
static int32_t ChkSectorSize( uint32_t maxSectorCnt, uint32_t startSector, uint32_t sizeSector );
static void SetSectorData(EMMC_SECTOR *sectorData);
static void DispAreaData(EMMC_SECTOR sectorData);

static uint32_t emmcInit;	/* eMMC drv init */

/****************************************************************
	MODULE			: dg_init_emmc			*
	FUNCTION		: Init flag MMC drv Init	*
	COMMAND			: 				*
	INPUT PARAMETER		: 				*
*****************************************************************/
void dg_init_emmc(void)
{
	emmcInit = FALSE;
}


/****************************************************************
	MODULE			: dg_emmc_check_init		*
	FUNCTION		: Check MMC drv Init		*
	COMMAND			: 				*
	INPUT PARAMETER		: 				*
*****************************************************************/
unsigned long dg_emmc_check_init(void)
{
	EMMC_ERROR_CODE result;

	result = EMMC_SUCCESS;

	/* not init */
	if (FALSE == emmcInit)
	{
		result = dg_emmc_init();
		if (EMMC_SUCCESS == result)
		{
			emmcInit = TRUE;
		}
	}

	return (unsigned long)result;
}


/****************************************************************
	MODULE			: dg_emmc_init			*
	FUNCTION		: Init eMMC			*
	COMMAND			: EMMC_INIT			*
	INPUT PARAMETER		: 				*
*****************************************************************/
static EMMC_ERROR_CODE	dg_emmc_init(void)
{
	EMMC_ERROR_CODE result;

	/* eMMC driver initialize */
	result = emmc_init(FALSE);      /* Normal clock mode */
	if (result != EMMC_SUCCESS)
	{
		return result;
	}

	/* Card power on
	 * MMC host cannot control Vcc&Vccq. RST_n asserted (high level).
	*/

	result = emmc_memcard_power(TRUE);
	if (result != EMMC_SUCCESS)
	{
		return result;
	}

	/* Card mount (user area)
	 * (1) Init card
	 * (2) Hi-speed mode
	 * (3) change Buswidth
	*/

	result = emmc_mount();
	if (result != EMMC_SUCCESS)
	{
		return result;
	}
	return EMMC_SUCCESS;
}

/****************************************************************
	MODULE			: dg_emmc_write			*
	FUNCTION		: Write Memory to eMMC		*
	COMMAND			: EMMC_W			*
	INPUT PARAMETER		: EMMC_W			*
*****************************************************************/
void	dg_emmc_write(EMMC_WRITE_COMMAND wc)
{

	EMMC_ERROR_CODE result;
	EMMC_SECTOR	sectorData;
	EMMC_PARTITION	partitionArea;

	uint32_t	*Load_workStartAdd;
	uint32_t	*Load_workEndAdd;

	uint32_t	flags = 0x00000000;
	uint32_t	sectorStartAddress;
	uint32_t	sectorSize;
	uint32_t	prgStartAdd;
	uint32_t	chkInput;

	uint32_t	workAdd_Min;		//mot file Start address
	uint32_t	workAdd_Max;		//mot file end address
	uint32_t	mmcPrgStartAdd;		//program Start address
	uint32_t	mmcPrgEndAdd;		//program end address
	uint32_t	saveSize;

	uint32_t	SecStartAdd;
	uint32_t	SecEndAdd;
	uint32_t	SecCnt;

	uint32_t	totalDownloadSize;
	uint32_t	fileSize;

	int32_t chCnt;
	int8_t buf[16];
	int8_t motLoad = 1;
	int8_t oldPartitionConfig;

	static const int8_t startMessage[][32] = {"EM_WB Start --------------"};

	static const int8_t endMessage[][32] =	{"EM_WB Complete!"};

	result = dg_emmc_check_init();
	if (EMMC_SUCCESS != result)
	{
		PutStr("eMMC Init ERROR!",1);
		return;
	}

	PutStr(startMessage[wc], 1);

//sector data disp
	SetSectorData( &sectorData );
	DispAreaData( sectorData );

//Select Partition
	chkInput = InputEmmcSectorArea( &partitionArea );
	if (1 != chkInput)
	{
		return;
	}

	Load_workStartAdd = (uint32_t*)EMMC_WORK_RAMB_SADD;
	switch(partitionArea)
	{
		case EMMC_PARTITION_USER_AREA:		//User Partition Area Program
			PutStr("-- User Partition Area Program --------------------------",1);
		break;
		case EMMC_PARTITION_BOOT_1:		//Boot Partition 1 Program
			PutStr("-- Boot Partition 1 Program -----------------------------",1);
		break;
		case EMMC_PARTITION_BOOT_2:		//Boot Partition 2 Program
			PutStr("-- Boot Partition 2 Program -----------------------------",1);
		break;
	}
	Load_workEndAdd		= (uint32_t*)EMMC_WORK_RAMB_EADD;

//Input address(mmc sector)
	chkInput = InputEmmcSector( partitionArea, sectorData.maxSectorCount[partitionArea], 
								&sectorStartAddress, &sectorSize, EMMC_INPUT_TYPE_WRITE );
	if (1 != chkInput)
	{
		return;
	}

	mmcPrgStartAdd = sectorStartAddress<<9;

//Input address( program start address)

// WorkMemory CLEAR (Write H'00000000)
	PutStr("Work RAM(H'B6000000-H'B60FFFFF) Clear....",1);
	FillData32Bit((uint32_t *)Load_workStartAdd,(uint32_t *)Load_workEndAdd,0x00000000);

	{
		chkInput = InputFileSize( &fileSize );
		if (1 != chkInput)
		{
			return;
		}
		PutStr("please send binary file!",1);

        //If send a file smaller than the specified file size,
	    //Flash writer will output the message and command exit force.
	    if (dg_emmc_write_bin_serial(Load_workStartAdd, fileSize) != 0)
	    {
    		PutStr("Time out! Unable to receive data for the specified size!",1);
	        return;
	    }
		workAdd_Min = (uintptr_t)Load_workStartAdd;
		workAdd_Max = workAdd_Min + fileSize - 1;
	}

#ifdef EMMC_DEBUG
	PutStr("workAdd_Min    = 0x",0);
	Hex2Ascii(workAdd_Min,buf,&chCnt);
	PutStr(buf,1);
	PutStr("workAdd_Max    = 0x",0);
	Hex2Ascii(workAdd_Max,buf,&chCnt);
	PutStr(buf,1);
#endif /* EMMC_DEBUG */

//transfer data calc
	mmcPrgStartAdd = mmcPrgStartAdd + (workAdd_Min - (uintptr_t)Load_workStartAdd);
	mmcPrgEndAdd   = mmcPrgStartAdd + (workAdd_Max - workAdd_Min);
	saveSize       = (mmcPrgEndAdd-mmcPrgStartAdd)+1;
	SecStartAdd    = (mmcPrgStartAdd >> 9);		//sector top add
	SecEndAdd      = (mmcPrgEndAdd   >> 9);		//sector bottom add
	SecCnt         = (SecEndAdd - SecStartAdd) + 1;
#ifdef EMMC_DEBUG
	PutStr("mmcPrgStartAdd = 0x",0);
	Hex2Ascii(mmcPrgStartAdd,buf,&chCnt);
	PutStr(buf,1);
	PutStr("mmcPrgEndAdd   = 0x",0);
	Hex2Ascii(mmcPrgEndAdd,buf,&chCnt);
	PutStr(buf,1);
	PutStr("saveSize       = 0x",0);
	Hex2Ascii(saveSize,buf,&chCnt);
	PutStr(buf,1);
	PutStr("SecStartAdd    = 0x",0);
	Hex2Ascii(SecStartAdd,buf,&chCnt);
	PutStr(buf,1);
	PutStr("SecEndAdd      = 0x",0);
	Hex2Ascii(SecEndAdd,buf,&chCnt);
	PutStr(buf,1);
	PutStr("secCnt         = 0x",0);
	Hex2Ascii(SecCnt,buf,&chCnt);
	PutStr(buf,1);
#endif /* EMMC_DEBUG */

// Select Partition
	result = emmc_select_partition( partitionArea );
	if (result != EMMC_SUCCESS)
	{
		PutStr("EM_W Partition select FAIL",1);
		return;
	}

	SETR_32( SD_SIZE, EMMC_BLOCK_LENGTH );
// Write eMMC
	PutStr("SAVE -FLASH.......",1);
	//emmc_write_sector
	result = emmc_write_sector( Load_workStartAdd, sectorStartAddress, SecCnt, flags );
	if (result != EMMC_SUCCESS)
	{
		PutStr("EM_W ERR",1);
		return;
	}

// Change original EXT_CSD
	PutStr(endMessage[wc], 1);
}

/****************************************************************
	MODULE			: dg_emmc_write_bin		*
	FUNCTION		: Write Memory to eMMC (Binary)	*
	COMMAND			: EMMC_WB			*
	INPUT PARAMETER		: EMMC_WB			*
*****************************************************************/
void	dg_emmc_write_bin(void)
{
	dg_emmc_write(EMMC_WRITE_BINARY);
}

/************************************************************************
	MODULE			: dg_emmc_write_bin_serial		*
	FUNCTION		: Write Memory to eMMC (Binary(Serial))	*
	COMMAND			: EMMC_WB				*
	INPUT PARAMETER		: EMMC_WB				*
*************************************************************************/
static int32_t dg_emmc_write_bin_serial(uint32_t* workStartAdd, uint32_t fileSize)
{
	uint32_t i;
	int8_t byteData = 0;
	uintptr_t ptr;
    int32_t isTimeout;
    
	ptr = (uintptr_t)workStartAdd;
	for (i = 0; i < fileSize; i++)
	{
		if( i != 0 ) {
			isTimeout = GetCharTimeOut(&byteData,DATA_RECV_TIMEOUT);
			if (isTimeout == -1) //if time out is occured.
				return -1;
		}
		else {
			GetChar(&byteData);
		}
		*((uint8_t *)ptr) = byteData;
		ptr++;
	}
    return 0;
}

/****************************************************************
	MODULE			: dg_emmc_erase			*
	FUNCTION		: Erase Memory to eMMC		*
	COMMAND			: EMMC_E			*
	INPUT PARAMETER		: EMMC_E			*
*****************************************************************/
void	dg_emmc_erase(void)
{

	EMMC_ERROR_CODE result;
	EMMC_SECTOR	sectorData;
	EMMC_PARTITION	partitionArea;

	uint32_t	erase_start_address;
	uint32_t	erase_end_address;
	uint32_t	chkInput;

	result = dg_emmc_check_init();
	if (EMMC_SUCCESS != result)
	{
		PutStr("eMMC Init ERROR!",1);
		return;
	}

	PutStr("EM_E Start --------------",1);

//sector data disp
	SetSectorData( &sectorData );
	DispAreaData( sectorData );

//Select Partition
	chkInput = InputEmmcSectorArea( &partitionArea );
	if (1 != chkInput)
	{
		return;
	}

	switch(partitionArea)
	{
		case EMMC_PARTITION_USER_AREA:		//User Partition Area Program
			PutStr("-- User Partition Area Program --------------------------",1);
		break;
		case EMMC_PARTITION_BOOT_1:		//Boot Partition 1 Program
			PutStr("-- Boot Partition 1 Program -----------------------------",1);
		break;
		case EMMC_PARTITION_BOOT_2:		//Boot Partition 2 Program
			PutStr("-- Boot Partition 2 Program -----------------------------",1);
		break;
	}
// Select Partition
	result = emmc_select_partition( partitionArea );
	if (result != EMMC_SUCCESS)
	{
		PutStr("EM_E Partition select FAIL",1);
		return;
	}

//emmc_erase_sector
	erase_start_address = 0;
	erase_end_address   = sectorData.maxSectorCount[partitionArea] - 1;

	result = emmc_erase_sector( erase_start_address, erase_end_address );
	if( result != EMMC_SUCCESS )
	{
		PutStr("EM_E ERR",1);
		return;
	}

// Change original EXT_CSD
	PutStr("EM_E Complete!",1);
}

/************************************************************************
	MODULE			: InputEmmcSector			*
	FUNCTION		: Input eMMC address and size in sector	*
	COMMAND			: 					*
	INPUT PARAMETER		: 					*
*************************************************************************/
static uint32_t InputEmmcSector( EMMC_PARTITION partitionArea, uint32_t maxSectorCnt, uint32_t *startSector, uint32_t *sizeSector, EMMC_INPUT_TYPE type )
{
	uint32_t loop = EMMC_INPUT_SECTOR_ADDRESS;
	uint32_t wrData;
	int32_t chkInput;
	int8_t str[64];
	int8_t buf[16];
	int8_t chCnt = 0;
	int8_t chPtr;

	//Input Start Address and Size (sector)
	while(loop)
	{
		if (EMMC_INPUT_SECTOR_ADDRESS == loop)
		{
			PutStr("Please Input Start Address in sector :",0);
		}
		else
		{
			PutStr("Please Input Size in sector :",0);
		}

		wrData = 0xFFFFFFFFU;
		GetStr(str,&chCnt);
		chPtr = 0;
		if (!GetStrBlk(str,buf,&chPtr,0))
		{
			if (chPtr == 1)
			{						/* Case Return */
				return(0);
			}
			else if (chPtr > (int8_t)((SIZE_32BIT<<1)+1))	/* Case Data Size Over */
			{
				PutStr("Syntax Error",1);
			}
			else
			{
				if (HexAscii2Data((uint8_t*)buf,&wrData))
				{
					PutStr("Syntax Error",1);
				}
				else
				{
					/* Check Param */
					if (EMMC_INPUT_SECTOR_ADDRESS == loop)		/* case Address */
					{
						if (maxSectorCnt <= wrData)		/* Param Error */
						{
							PutStr("Param Error",1);
						}
						else
						{
							*startSector = wrData;
							if (EMMC_INPUT_TYPE_WRITE == type)		/* case Write */
							{
								loop = EMMC_INPUT_SECTOR_END;		/* Input End */
							}
							else
							{
								loop--;
							}
						}
					}
					else								/* case Size */
					{
						chkInput = ChkSectorSize(maxSectorCnt, *startSector, wrData);
						if (-1 == chkInput)	/* Size Over */
						{
							PutStr("Partition Size Over",1);
						}
						else if (0 == chkInput)	/* Param Error */
						{
							PutStr("Param Error",1);
						}
						else
						{
							*sizeSector = wrData;
							loop--;
						}
					}
				}
			}
		}
	}

	return(1);
}

/****************************************************************
	MODULE			: InputEmmcSectorArea		*
	FUNCTION		: Input eMMC Partition Area	*
	COMMAND			: 				*
	INPUT PARAMETER		: 				*
*****************************************************************/
static uint32_t InputEmmcSectorArea( EMMC_PARTITION *partitionArea )
{
	uint32_t loop;
	uint32_t wrData;
	int8_t str[64];
	int8_t buf[16];
	int8_t chCnt = 0;
	int8_t chPtr;

	loop = 1;
	while(loop)
	{
		PutStr("  Select area(0-2)>",0);
		GetStr(str,&chCnt);
		chPtr = 0;
		if (!GetStrBlk(str,buf,&chPtr,0))
		{
			if (chPtr == 1)
			{
				/* Case Return */
				return(0);
			}
			else if (chPtr > (int8_t)((SIZE_32BIT<<1)+1))	/* Case Data Size Over */
			{
				PutStr("Syntax Error",1);
			}
			else
			{
				if (HexAscii2Data((uint8_t*)buf,&wrData))
				{
					PutStr("Syntax Error",1);
				}
				else
				{
					switch(wrData)
					{
						case 0:			//User Partition Area Program
							*partitionArea	= EMMC_PARTITION_USER_AREA;
							loop=0;
						break;
						case 1:			//Boot Partition 1 Program
							*partitionArea	= EMMC_PARTITION_BOOT_1;
							loop=0;
						break;
						case 2:			//Boot Partition 2 Program
							*partitionArea	= EMMC_PARTITION_BOOT_2;
							loop=0;
						break;
						default:
							PutStr("Palam Error",1);
						break;
					}
				}
			}
		}
	}

	return(1);
}

/****************************************************************
	MODULE			: InputFileSize			*
	FUNCTION		: Input Binary File Size	*
	COMMAND			: 				*
	INPUT PARAMETER		: 				*
*****************************************************************/
static uint32_t InputFileSize( uint32_t *fileSize )
{
	uint32_t loop;
	uint32_t wrData;
	int8_t str[16];
	int8_t buf[16];
	int8_t chCnt = 0;
	int8_t chPtr;

	loop = 1;
	while(loop)
	{
		PutStr("Please Input File size(byte) : ",0);
		GetStr(str,&chCnt);
		chPtr = 0;
		if (!GetStrBlk(str,buf,&chPtr,0))
		{
			if (chPtr == 1)
			{	/* Case Return */
				return(0);
			}
			else
			{
				if (HexAscii2Data((uint8_t*)buf,&wrData))
				{
					PutStr("Syntax Error",1);
				}
				else
				{
					if ( wrData > EMMC_WORK_RAMB_MAX_SIZ )
					{
						PutStr("Size Parameter Error",1);
					}
					else{
						*fileSize = wrData;
						loop = 0;
					}
				}
			}
		}
	}

	return(1);
}


/****************************************************************
	MODULE			: ChkSectorSize			*
	FUNCTION		: check Sector Size		*
	COMMAND			: 				*
	INPUT PARAMETER		: 				*
*****************************************************************/
static int32_t ChkSectorSize( uint32_t maxSectorCnt, uint32_t startSector, uint32_t sizeSector )
{
	uint32_t sumSector;

	if (EMMC_WORK_RAMB_SECTOR_MAX < sizeSector)
	{
		return(-1);	/* Size Over */
	}

	if ((1 <= sizeSector) && (maxSectorCnt >= sizeSector))
	{
		sumSector = startSector + sizeSector;

		if (sumSector <= maxSectorCnt)
		{
			return(1);	/* OK */
		}
		else
		{
			return(-1);	/* Size Over */
		}
	}
	else
	{
		return(0);	/* Param Error */
	}

}

/****************************************************************
	MODULE			: SetSectorData			*
	FUNCTION		: Set eMMC Sector Data		*
	COMMAND			: 				*
	INPUT PARAMETER		: 				*
*****************************************************************/
static void SetSectorData(EMMC_SECTOR *sectorData)
{
	uint32_t multiSectorCount;
	uint32_t bootPartitionSector;
	uint32_t userDataAreaSector;

	multiSectorCount = mmc_drv_obj.ext_csd_data[EMMC_EXT_CSD_BOOT_SIZE_MULTI];
	bootPartitionSector = ( SIZE2SECTOR( MULTI_PARTITION_SIZE ) * multiSectorCount);

	userDataAreaSector  = mmc_drv_obj.ext_csd_data[EMMC_EXT_CSD_SEC_COUNT-0] << 24;
	userDataAreaSector |= mmc_drv_obj.ext_csd_data[EMMC_EXT_CSD_SEC_COUNT-1] << 16;
	userDataAreaSector |= mmc_drv_obj.ext_csd_data[EMMC_EXT_CSD_SEC_COUNT-2] << 8;
	userDataAreaSector |= mmc_drv_obj.ext_csd_data[EMMC_EXT_CSD_SEC_COUNT-3] << 0;

	sectorData->maxSectorCount[EMMC_PARTITION_BOOT_1] = bootPartitionSector;
	sectorData->maxSectorCount[EMMC_PARTITION_BOOT_2] = bootPartitionSector;
	sectorData->maxSectorCount[EMMC_PARTITION_USER_AREA] = userDataAreaSector;

	return;
}

/****************************************************************
	MODULE			: DispAreaData			*
	FUNCTION		: Display eMMC Sector Data	*
	COMMAND			: 				*
	INPUT PARAMETER		: 				*
*****************************************************************/
static void DispAreaData(EMMC_SECTOR sectorData)
{
	uint32_t loop;
	int8_t str[64];
	int32_t chCnt;

	PutStr("---------------------------------------------------------",1);
	PutStr("Please select,eMMC Partition Area.",1);

	for (loop = EMMC_PARTITION_USER_AREA; loop < EMMC_PARTITION_MAX; loop++)
	{
		if (EMMC_PARTITION_USER_AREA == loop)
		{
			PutStr(" 0:User Partition Area   : ",0);
		}
		else if(EMMC_PARTITION_BOOT_1 == loop)
		{
			PutStr(" 1:Boot Partition 1      : ",0);
		}
		else
		{
			PutStr(" 2:Boot Partition 2      : ",0);
		}
		Hex2DecAscii(((sectorData.maxSectorCount[loop])>>1),str,&chCnt);
		PutStr(str,0);
		PutStr(" KBytes",1);
		PutStr("  eMMC Sector Cnt : H'0 - H'",0);
		Hex2Ascii((sectorData.maxSectorCount[loop] - 1),str,&chCnt);
		PutStr(str,1);
	}

	PutStr("---------------------------------------------------------",1);

}
