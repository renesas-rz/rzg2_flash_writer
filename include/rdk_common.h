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
/*******************************************************************************
 * File Name    : rdk_common.h
 * Description  : information and API for common functions
 ******************************************************************************/

#ifndef RDK_COMMON_H
#define RDK_COMMON_H

/*
 * Macro definitions
 */
#ifndef TRUE
#define TRUE		1U
#endif

#ifndef FALSE
#define FALSE		0U
#endif

#ifndef NULL
#define NULL    ((void*)0)
#endif  /* !defined(NULL) */

/*
 * Global Typedef definitions
 */
typedef enum
{
    CMN_SUCCESS = 0,
    CMN_ERROR   = -1
} e_rdk_cmn_error_no_t;

/******************************************************************************
 Prototype define
 *****************************************************************************/
void CMN_InitSysCnt(void);
void CMN_DelayInUSec(uint64_t us);
uint64_t CMN_GetSysCnt(void);
uint32_t CMN_GetFreq4SysCnt(void);


/*******************************************************************************
 * Function Name: CMN_REG_Read32
 * Description  : read access to Register in uint32_t.
 *
 * Arguments    : addr -
 *                    address for read access
 * Return Value : value -
 *                    read data.
 ******************************************************************************/
static inline uint32_t CMN_REG_Read32(uintptr_t addr)
{
    return *((volatile uint32_t *)addr);
}


/*******************************************************************************
 * Function Name: CMN_REG_Write32
 * Description  : write access to Register in uint32_t.
 *
 * Arguments    : addr -
 *                    address for write access
 *                value -
 *                    write data
 * Return Value : non -
 *
 ******************************************************************************/
static inline void CMN_REG_Write32(uintptr_t addr, uint32_t value)
{
    /** sizeof(uintptr_t) == sizeof(uint32_t *) */
    *((volatile uint32_t *)addr) = value;
}

/*******************************************************************************
 * Function Name: CMN_DelayInMS
 * Description  : Does an example task. Making this longer just to see how it
 *                wraps.
 * Arguments    : index -
 *                    Where to start looking
 *                p_output -
 *                    Pointer of where to put the output data
 * Return Value : count -
 *                    How many entries were found
 ******************************************************************************/
static inline void CMN_DelayInMS(uint64_t ms)
{
    CMN_DelayInUSec(ms * 1000ULL);
}

/*******************************************************************************
 * Function Name: CMN_DelayInUS
 * Description  : Does an example task. Making this longer just to see how it
 *                wraps.
 * Arguments    : index -
 *                    Where to start looking
 *                p_output -
 *                    Pointer of where to put the output data
 * Return Value : count -
 *                    How many entries were found
 ******************************************************************************/
static inline void CMN_DelayInUS(uint64_t us)
{
    CMN_DelayInUSec(us);
}


#endif /* !defined RDK_COMMON_H */
