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
#include "reg_rzg2.h"
#include "boot_init_port.h"

#define PFC_WR(m,n)   *((volatile uint32_t*)PFC_PMMR)=~(n);*((volatile uint32_t*)(m))=(n);

/* SCIF3 Registers for Dummy write */
#define SCIF3_BASE			(0xE6C50000U)
#define SCIF3_SCFCR			(SCIF3_BASE + 0x0018U)
#define SCIF3_SCFDR			(SCIF3_BASE + 0x001CU)
#define SCFCR_DATA			(0x0000U)

/* Realtime module stop control */
#define RMSTPCR0_RTDMAC			(0x00200000U)

/* RT-DMAC Registers */
#define RTDMAC_CH			(0U)		/* choose 0 to 15 */

#define RTDMAC_BASE			(0xFFC10000U)
#define RTDMAC_RDMOR			(RTDMAC_BASE + 0x0060U)
#define RTDMAC_RDMCHCLR			(RTDMAC_BASE + 0x0080U)
#define RTDMAC_RDMSAR(x)		(RTDMAC_BASE + 0x8000U + (0x80U * (x)))
#define RTDMAC_RDMDAR(x)		(RTDMAC_BASE + 0x8004U + (0x80U * (x)))
#define RTDMAC_RDMTCR(x)		(RTDMAC_BASE + 0x8008U + (0x80U * (x)))
#define RTDMAC_RDMCHCR(x)		(RTDMAC_BASE + 0x800CU + (0x80U * (x)))
#define RTDMAC_RDMCHCRB(x)		(RTDMAC_BASE + 0x801CU + (0x80U * (x)))
#define RTDMAC_RDMDPBASE(x)		(RTDMAC_BASE + 0x8050U + (0x80U * (x)))
#define RTDMAC_DESC_BASE		(RTDMAC_BASE + 0xA000U)
#define RTDMAC_DESC_RDMSAR		(RTDMAC_DESC_BASE + 0x0000U)
#define RTDMAC_DESC_RDMDAR		(RTDMAC_DESC_BASE + 0x0004U)
#define RTDMAC_DESC_RDMTCR		(RTDMAC_DESC_BASE + 0x0008U)

#define RDMOR_DME			(0x0001U)	/* DMA Master Enable */
#define RDMCHCR_DPM_INFINITE		(0x30000000U)	/* Infinite repeat mode */
#define RDMCHCR_RPT_TCR			(0x02000000U)	/* enable to update TCR */
#define RDMCHCR_TS_2			(0x00000008U)	/* Word(2byte) units transfer */
#define RDMCHCR_RS_AUTO			(0x00000400U)	/* Auto request */
#define RDMCHCR_DE			(0x00000001U)	/* DMA Enable */
#define RDMCHCRB_DRST			(0x00008000U)	/* Descriptor reset */
#define RDMCHCRB_SLM_256		(0x00000080U)	/* once in 256 clock cycle */
#define RDMDPBASE_SEL_EXT		(0x00000001U)	/* External memory use */

#ifdef RZG2_HIHOPE
static void InitMODSEL(void);
static void InitIPSR_G2M(void);
static void InitGPSR_G2M(void);
static void InitPOCCTRL(void);
static void InitDRVCTRL(void);
static void InitPUD(void);
static void InitPUEN(void);
#endif /* RZG2_HIHOPE */

#ifdef RZG2_EK874
static void InitMODSEL_G2E(void);
static void InitIPSR_G2E(void);
static void InitGPSR_G2E(void);
static void InitIOCTRL_G2E(void);
static void InitPUD_G2E(void);
static void InitPUEN_G2E(void);
#endif /* RZG2_EK874 */

#ifdef RZG2_HIHOPE
static void StartRtDma0_Descriptor(void);
#endif /* RZG2_HIHOPE */

void InitPORT(void)
{
	uint32_t product;
	uint32_t cut;

	product = *((volatile uint32_t*)PRR) & PRR_PRODUCT_MASK;
	cut = *((volatile uint32_t*)PRR) & PRR_CUT_MASK;

	switch (product)
	{
#ifdef RZG2_HIHOPE
		case PRR_PRODUCT_G2M:
			StartRtDma0_Descriptor();
			/* no break */
		case PRR_PRODUCT_G2N:
		case PRR_PRODUCT_G2H:
			InitMODSEL();
			InitIPSR_G2M();
			InitGPSR_G2M();
			InitPOCCTRL();
			InitDRVCTRL();
			InitPUD();
			InitPUEN();
		break;
#endif /* RZG2_HIHOPE */
#ifdef RZG2_EK874
		case PRR_PRODUCT_G2E:
			InitMODSEL_G2E();
			InitIPSR_G2E();
			InitGPSR_G2E();
			InitIOCTRL_G2E();
			InitPUD_G2E();
			InitPUEN_G2E();
		break;
#endif /* RZG2_EK874*/
		default:
		break;
	}
}

#ifdef RZG2_HIHOPE
static void InitMODSEL(void)
{
	PFC_WR(PFC_MOD_SEL0,0x00000000);
	PFC_WR(PFC_MOD_SEL1,0x00000000);
	PFC_WR(PFC_MOD_SEL2,0x00000000);
}

static void InitIPSR_G2M(void)
{
	PFC_WR(PFC_IPSR17,0x00000010);	// selcet SCIF_CLK_A
}

static void InitGPSR_G2M(void)
{
	PFC_WR(PFC_GPSR4, 0x0003FF80);	// SD3_DS, SD3_DAT7,
					// SD3_DAT6, SD3_DAT5, SD3_DAT4, SD3_DAT3, SD2_DAT2, SD3_DAT1, SD3_DAT0, SD3_CMD
					// SD3_CLK
	PFC_WR(PFC_GPSR5, 0x00000C00);	// TX2_A, RX2_A
	PFC_WR(PFC_GPSR6, 0x00800000);	// SCIF_CLK_A
	PFC_WR(PFC_GPSR7, 0x00000000);
}

static void InitPOCCTRL(void)
{
	PFC_WR(PFC_POCCTRL0, 0x0007FFFF);	// SD3_xx 1.8V
}

static void InitDRVCTRL(void)
{
	PFC_WR(PFC_DRVCTRL15,0xFFFFFCCC);
	PFC_WR(PFC_DRVCTRL16,0xCCCCCCCC);
}

static void InitPUD(void)
{
}

static void InitPUEN(void)
{
}
#endif /* RZG2_HIHOPE */

#ifdef RZG2_EK874
static void InitMODSEL_G2E(void)
{
}

static void InitIPSR_G2E(void)
{
}

static void InitGPSR_G2E(void)
{
	PFC_WR(PFC_GPSR2,0x3f);
}

static void InitIOCTRL_G2E(void)
{
}

static void InitPUD_G2E(void)
{
}

static void InitPUEN_G2E(void)
{
}
#endif /* RZG2_EK874*/

#ifdef RZG2_HIHOPE
static void StartRtDma0_Descriptor(void)
{
	uint32_t reg;

	reg = *((volatile uint32_t *)PRR);
	reg &= (PRR_CUT_MASK);
	if (reg == (PRR_CUT_10))
	{
		/* Module stop clear */
		while((*((volatile uint32_t *)CPG_RMSTPCR0) & RMSTPCR0_RTDMAC) != 0U)
		{
			reg = *((volatile uint32_t *)CPG_RMSTPCR0);
			reg &= ~RMSTPCR0_RTDMAC;
			*((volatile uint32_t *)CPG_CPGWPR) = ~reg;
			*((volatile uint32_t *)CPG_RMSTPCR0) = reg;
		}

		/* Initialize ch0, Reset Descriptor */
		*((volatile uint32_t *)RTDMAC_RDMCHCLR) = ((uint32_t)1U << RTDMAC_CH);
		*((volatile uint32_t *)RTDMAC_RDMCHCRB(RTDMAC_CH)) = RDMCHCRB_DRST;

		/* Enable DMA */
		*((volatile uint16_t *)RTDMAC_RDMOR) = RDMOR_DME;

		/* Set first transfer */
		*((volatile uint32_t *)RTDMAC_RDMSAR(RTDMAC_CH)) = PRR;
		*((volatile uint32_t *)RTDMAC_RDMDAR(RTDMAC_CH)) = SCIF3_SCFDR;
		*((volatile uint32_t *)RTDMAC_RDMTCR(RTDMAC_CH)) = 0x00000001U;

		/* Set descriptor */
		*((volatile uint32_t *)RTDMAC_DESC_RDMSAR) = 0x00000000U;
		*((volatile uint32_t *)RTDMAC_DESC_RDMDAR) = 0x00000000U;
		*((volatile uint32_t *)RTDMAC_DESC_RDMTCR) = 0x00200000U;
		*((volatile uint32_t *)RTDMAC_RDMCHCRB(RTDMAC_CH)) = RDMCHCRB_SLM_256;
		*((volatile uint32_t *)RTDMAC_RDMDPBASE(RTDMAC_CH)) = (RTDMAC_DESC_BASE | RDMDPBASE_SEL_EXT);

		/* Set transfer parameter, Start transfer */
		*((volatile uint32_t *)RTDMAC_RDMCHCR(RTDMAC_CH)) = (RDMCHCR_DPM_INFINITE
											| RDMCHCR_RPT_TCR
											| RDMCHCR_TS_2
											| RDMCHCR_RS_AUTO
											| RDMCHCR_DE
											);
	}
}
#endif /* RZG2_HIHOPE */
