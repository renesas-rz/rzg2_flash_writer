/*
 * Copyright (c) 2015-2020, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "common.h"
#include "devdrv.h"
#include "scifdrv.h"

/************************
	PutChar		*
*************************/

int32_t PutChar(char outChar)
{
	PutCharSCIF0(outChar);
	return(0);
}

/************************
	GetChar		*
*************************/

int32_t GetChar(char *inChar)
{
	GetCharSCIF0(inChar);
	return(0);
}

int32_t WaitPutCharSendEnd(void)
{
	WaitPutScif0SendEnd();
}
