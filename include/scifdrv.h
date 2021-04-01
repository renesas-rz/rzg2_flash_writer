/*
 * Copyright (c) 2015-2020, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

int32_t PutCharSCIF0(char outChar);
int32_t GetCharSCIF0(char *inChar);
void PowerOnScif0(void);
void WaitPutScif0SendEnd(void);
void InitScif0_SCIFCLK(void);
uint32_t SCIF_TerminalInputCheck(char* str);
