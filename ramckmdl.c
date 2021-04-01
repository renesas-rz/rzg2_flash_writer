/*
 * Copyright (c) 2015-2017, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "dgtable.h"

#pragma GCC optimize ("Ofast")

#include "common.h"

int32_t FillData32Bit( uint32_t *startAddr, uint32_t *endAddr, uint32_t writeData )
{
	volatile uint32_t *pData;
	pData = (uint32_t *)startAddr;
	while(1)
	{
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
		if (pData >= endAddr) 
		{
			return NORMAL_END;
		}
	}
	return NORMAL_END;
}
