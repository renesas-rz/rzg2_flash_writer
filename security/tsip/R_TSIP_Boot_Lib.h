/*
 * Copyright (c) 2020, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*! *****************************************************************************
    @file    R_TSIP_BOOT_LIB.h
    @brief   TSIP Security Driver Boot Library API(No OS)
    
    @par    History
    @subsection sub_history 
      - Initial version\n
 ********************************************************************************/

#ifndef  R_TSIP_BOOT_LIB_H_
#define  R_TSIP_BOOT_LIB_H_

/* Define */
#define R_PASS                       (0x00000000)
#define R_INITIALIZATION_FAIL        (0x00000001)
#define R_PARAMETER_FAIL             (0x00000002)
#define R_SEQUENCE_FAIL              (0x00000003)
#define R_RESOURCE_CONFLICT_FAIL     (0x00000004)
#define R_VERIFICATION_FAIL          (0x00000005)
#define R_SERVERMESSAGE_FAIL         (0x00000006)
#define R_SERVER_VERIFICATION_FAIL   (0x00000007)
#define R_ROOTCA_CERTIFICATION_FAIL  (0x00000008)
#define R_USEKEY_FAIL                (0x00000009)
#define R_CONNECTSESSION_FAIL        (0x0000000A)
#define R_INDATA_OUTDATA_ACCESS_FAIL (0x0000000B)
#define R_PROVISIONING_KEY_FAIL      (0x0000000C)
#define R_GENERATION_FAIL            (0x0000000D)
#define R_KEYRING_FORMAT_FAIL        (0x0000000E)
#define R_ALLOCATE_MEMORY_FAIL       (0x00000020)
#define R_MMAP_FAIL                  (0x00000021)
#define R_INITIALIZATION_ERROR       (0x00000080)



#define REENC_BOOT_DATA_MAX             (16)

typedef struct str_tsip_reenc_boot_data {
    unsigned char *InData_BootData;
    unsigned long  InData_BootData_ByteSize;
    unsigned char *OutData_BootData;
} TSIP_REENC_BOOT_DATA[REENC_BOOT_DATA_MAX];


/* API */
unsigned long R_TSIP_Init(unsigned char *S_RAMData, unsigned char *S_INSTData);
unsigned long R_TSIP_Inject_Key(unsigned char *InData_ProvisioningKeyOperationCode, unsigned char *InData_KeyRingOperationCode);
unsigned long R_TSIP_ReEncBootData(TSIP_REENC_BOOT_DATA  *tsip_reenc_bootdata);
unsigned long R_TSIP_Inject_Addon_Key(unsigned long InData_Operation, unsigned char *InData_IV, unsigned char *InData_KeyOperationCode, unsigned char *OutData_KeyOperationCode);
unsigned long R_TSIP_VerifyBootData(unsigned char *InData_BootData, unsigned long InData_BootData_ByteSize, unsigned char *InData_BootData_HeapArea, unsigned char *OutData_BootData);
unsigned long R_TSIP_KeyRing_Verify(void);

#endif  /* R_TSIP_BOOT_LIB_H_ */
