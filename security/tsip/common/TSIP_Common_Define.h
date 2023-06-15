/*
 * Copyright (c) 2020, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*! *****************************************************************************
    @file    TSIP_Common_Define.h
    @brief   TSIP Common Define
    
    @par    History
    @subsection sub_history 
      - Initial version  \n
 ********************************************************************************/

#ifndef  TSIP_COMMON_DEFINE_H_
#define  TSIP_COMMON_DEFINE_H_

#include <stdint.h>
#include <stddef.h>

#define  ROUNDUP_DWORD_BOUNDARY(n)    ((((n) + 3U) / 4U) * 4U)       /*!< Round up to multiple of 4 for procedural processing */
#define  ROUNDUP_AESBLOCK(n)          ((((n) + 16U) / 16U) * 16U)    /*!< Round up to multiple of 16 for AES (it always needs padding in AES. if the specified bytes are multiple of 16, it should be added 16Byte) */
#define  ROUNDUP_SIGBLOCK(n)          ((((n) + 63U) / 64U) * 64U)    /*!< Round up to multiple of 64 for signature process */

#define  TLS_SESSION_ID_MASK      (0xFFFFFF00)
/*!
    @name Security Driver API Return code
 */
/* @{ */

#define R_PASS                       (0x00000000U)
#define R_INITIALIZATION_FAIL        (0x00000001U)
#define R_PARAMETER_FAIL             (0x00000002U)
#define R_SEQUENCE_FAIL              (0x00000003U)
#define R_RESOURCE_CONFLICT_FAIL     (0x00000004U)
#define R_VERIFICATION_FAIL          (0x00000005U)
#define R_SERVERMESSAGE_FAIL         (0x00000006U)
#define R_SERVER_VERIFICATION_FAIL   (0x00000007U)
#define R_ROOTCA_CERTIFICATION_FAIL  (0x00000008U)
#define R_USEKEY_FAIL                (0x00000009U)
#define R_CONNECTSESSION_FAIL        (0x0000000AU)
#define R_INDATA_OUTDATA_ACCESS_FAIL (0x0000000BU)
#define R_PROVISIONING_KEY_FAIL      (0x0000000CU)
#define R_GENERATION_FAIL            (0x0000000DU)
#define R_KEYRING_FORMAT_FAIL        (0x0000000EU)
#define R_ALLOCATE_MEMORY_FAIL       (0x00000020U)
#define R_MMAP_FAIL                  (0x00000021U)
#define R_FALSIFICATION_ERROR        (0x00000022U)
#define R_PROC_RETRY                 (0x00000040U)
#define R_PROC_CONT                  (0x00000041U)
#define R_INITIALIZATION_ERROR       (0x00000080U)
/* @} */


/*!
    @name Procedure Return Code
 */
/* @{ */
#define RETURN_OK               ((uint8_t)0x00)    /*!< command success */ 
#define ILLEGAL_PARAMETER       ((uint8_t)0x11)    /*!< command parameter error */
#define SELFTEST_FAIL           ((uint8_t)0x51)    /*!< selftest failure */
#define FALSIFICATION_ERROR     ((uint8_t)0x60)    /*!< tamper detection error */

#define FORMAT_ERROR            ((uint8_t)0xF3)    /*!< format error */
#define VERIFY_ERROR            ((uint8_t)0xF5)    /*!< verification failure */

#define HW_BUSY                 ((uint8_t)0x62)    /*!< [internal] hardware busy */

#define PROC_RETRY              ((uint8_t)0x63)    /*!< [internal] retry */
#define PROC_CONT               ((uint8_t)0x64)    /*!< [internal] continue  */

#define INTERNAL_ERROR          ((uint8_t)0x81)    /*!< [internal] internal error */


/* @} */


/*!
    @name API TAG used for Require State Table
 */
/* @{ */
#define TSIP_INIT                   (0)
#define TSIP_INJECT_KEY             (1)
#define TSIP_LIB_INIT               (6)
#define TSIP_LIB_APIS               (7)
#define TSIP_LIB_SECURE_UPDATE_APIS (8)
#define TSIP_INJECT_KEY_SKIP        (9)
#define TSIP_INJECT_ADDON_KEY       (11)
#define TSIP_INJECT_ADDON_KEY_SKIP  (12)
#define TSIP_INJECT_ADDON_KEY_FIXEDSTATE (13)
#define TSIP_KEYRING_VERIFY         (14)
#define TSIP_REENC_BOOTDATA         (15)
#define TSIP_REENC_BOOTDATA_SKIP    (16)
#define TSIP_VERIFYBOOTDATA         (17)
#define TSIP_VERIFYBOOTDATA_SKIP    (18)
#define TSIP_STATE_TAG_MAX          (19)
/* @} */


/*!
    @name State Change Number
 */
/* @{ */
#define TSIP_CORE_STATE00      (0x0000)
#define TSIP_CORE_STATE01      (0x0001)
#define TSIP_CORE_STATE02      (0x0002)
#define TSIP_CORE_STATE03      (0x0003)
#define TSIP_CORE_STATE04      (0x0004)
#define TSIP_CORE_STATE05      (0x0005)
#define TSIP_CORE_STATE06      (0x0006)
#define TSIP_CORE_STATE07      (0x0007)
#define TSIP_CORE_STATE08      (0x0008)
#define TSIP_CORE_STATE09      (0x0009)
#define TSIP_CORE_STATE10      (0x000A)
#define TSIP_CORE_STATE11      (0x000B)
#define TSIP_CORE_STATE20      (0x0014)
#define TSIP_CORE_STATE21      (0x0015)
#define TSIP_CORE_STATE22      (0x0016)
#define TSIP_CORE_STATE23      (0x0017)
#define TSIP_CORE_STATE24      (0x0018)
#define TSIP_CORE_STATE25      (0x0019)
#define TSIP_CORE_STATE26      (0x001A)
#define TSIP_CORE_STATE27      (0x001B)
#define TSIP_CORE_STATE_STOP   (0x00F0)
#define TSIP_CORE_STATE_NONE   (0x00FF)
#define TSIP_CORE_STATE_MAX    (0x0011)
/* @} */


/*!
    @name KeyOperation Code Value
 */
/* @{ */
#define KEYOPERATION_CRYPTO_TYPE_MIN       (0x00U)
#define KEYOPERATION_CRYPTO_TYPE_MAX       (0x09U)
#define KEYOPERATION_USE_BITFIELD_UNMASK   (0x07U)
#define KEYOPERATION_CRYPTO_TYPE_OFFSET    (0x00U)
#define KEYOPERATION_USE_BITFIELD_OFFSET   (0x01U)
#define KEYOPERATION_LIFE_TYPE_OFFSET      (0x02U)

#define KEYOPERATION_CRYPTO_AES128         (0x00U)
#define KEYOPERATION_CRYPTO_AES256         (0x01U)
#define KEYOPERATION_CRYPTO_HMAC_SHA256    (0x02U)
#define KEYOPERATION_CRYPTO_HMAC_SHA1      (0x03U)
#define KEYOPERATION_CRYPTO_RSA1024Pub     (0x04U)
#define KEYOPERATION_CRYPTO_RSA2048Pub     (0x05U)
#define KEYOPERATION_CRYPTO_RSA1024Pri_CRT (0x06U)
#define KEYOPERATION_CRYPTO_RSA2048Pri_CRT (0x07U)
#define KEYOPERATION_CRYPTO_RSA1024Pri_STD (0x08U)
#define KEYOPERATION_CRYPTO_RSA2048Pri_STD (0x09U)

#define KEYOPERATION_USE_ENCRYPT           (0x80U)
#define KEYOPERATION_USE_DECRYPT           (0x40U)
#define KEYOPERATION_USE_GENERATE_MAC      (0x20U)
#define KEYOPERATION_USE_VERIFY_MAC        (0x10U)
#define KEYOPERATION_USE_VERIFY_SIGN       (0x08U)


#define KEYOPERATION_CRYPTO_AESEnc_TYPE               (0x00000100U)
#define KEYOPERATION_CRYPTO_AESDec_TYPE               (0x00000200U)
#define KEYOPERATION_CRYPTO_MAC_TYPE                  (0x00000300U)
#define KEYOPERATION_CRYPTO_MAC_HMAC_SHA1_TYPE        (0x00000301U)
#define KEYOPERATION_CRYPTO_MAC_HMAC_SHA1_GEN_TYPE    (0x00000311U)
#define KEYOPERATION_CRYPTO_MAC_HMAC_SHA1_VERI_TYPE   (0x00000321U)
#define KEYOPERATION_CRYPTO_MAC_HMAC_SHA256_TYPE      (0x00000302U)
#define KEYOPERATION_CRYPTO_MAC_HMAC_SHA256_GEN_TYPE  (0x00000312U)
#define KEYOPERATION_CRYPTO_MAC_HMAC_SHA256_VERI_TYPE (0x00000322U)
#define KEYOPERATION_CRYPTO_MAC_CMAC128_TYPE          (0x00000303U)
#define KEYOPERATION_CRYPTO_MAC_CMAC128_GEN_TYPE      (0x00000313U)
#define KEYOPERATION_CRYPTO_MAC_CMAC128_VERI_TYPE     (0x00000323U)
#define KEYOPERATION_CRYPTO_MAC_CMAC256_TYPE          (0x00000304U)
#define KEYOPERATION_CRYPTO_MAC_CMAC256_GEN_TYPE      (0x00000314U)
#define KEYOPERATION_CRYPTO_MAC_CMAC256_VERI_TYPE     (0x00000324U)
#define KEYOPERATION_CRYPTO_RSAPub_TYPE               (0x00000400U)
#define KEYOPERATION_CRYPTO_RSAPri_TYPE               (0x00000500U)
/* @} */


/* Core Layer Define */
#define TSIP_CORE_STATE_MAX_ARRAY    (0x0004)

#define TSIP_TRUE    (0x01)
#define TSIP_FALSE   (0x00)

#define UNUSED(x) ((void)x)

#define REENC_BOOT_DATA_MAX             (16)
#define UPDATE_BOOT_DATA_MAX            (16)

/* REG18H JUDG_FLAG_NG */
#define  MASK_JUDG_FLAG_NG      (0x00008000U)
#define  TSIP_JUDG_FLAG_NG      (0x00008000U)

#endif  /* TSIP_COMMON_DEFINE_H_ */

