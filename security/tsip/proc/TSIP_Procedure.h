/*
 * Copyright (c) 2020, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*! *****************************************************************************
    @file    TSIP_Procedure.h
    @brief   TSIP Procedure

    @par    History
    @subsection sub_history
    - Initial version\n
 ********************************************************************************/

#ifndef  TSIP_PROCEDURE_H_
#define  TSIP_PROCEDURE_H_

/*------------------------------------------------------------------------------*/
/*                        Include Files                                         */
/*------------------------------------------------------------------------------*/

#include  <stdint.h>
#include  <stddef.h>

#define  REG_00H    0U
#define  REG_04H    1U
#define  REG_08H    2U
#define  REG_0CH    3U
#define  REG_10H    4U
#define  REG_14H    5U
#define  REG_18H    6U
#define  REG_1CH    7U
#define  REG_20H    8U
#define  REG_24H    9U
#define  REG_28H    10U
#define  REG_2CH    11U
#define  REG_30H    12U
#define  REG_34H    13U
#define  REG_38H    14U
#define  REG_3CH    15U
#define  REG_40H    16U
#define  REG_44H    17U
#define  REG_48H    18U
#define  REG_4CH    19U
#define  REG_50H    20U
#define  REG_54H    21U
#define  REG_58H    22U
#define  REG_5CH    23U
#define  REG_60H    24U
#define  REG_64H    25U
#define  REG_68H    26U
#define  REG_6CH    27U
#define  REG_70H    28U
#define  REG_74H    29U
#define  REG_78H    30U
#define  REG_7CH    31U
#define  REG_80H    32U
#define  REG_84H    33U
#define  REG_88H    34U
#define  REG_8CH    35U
#define  REG_90H    36U
#define  REG_94H    37U
#define  REG_98H    38U
#define  REG_9CH    39U
#define  REG_A0H    40U
#define  REG_A4H    41U
#define  REG_A8H    42U
#define  REG_ACH    43U
#define  REG_B0H    44U
#define  REG_B4H    45U
#define  REG_B8H    46U
#define  REG_BCH    47U
#define  REG_C0H    48U
#define  REG_C4H    49U
#define  REG_C8H    50U
#define  REG_CCH    51U
#define  REG_D0H    52U
#define  REG_D4H    53U
#define  REG_D8H    54U
#define  REG_DCH    55U
#define  REG_E0H    56U
#define  REG_E4H    57U
#define  REG_E8H    58U
#define  REG_ECH    59U
#define  REG_F0H    60U
#define  REG_F4H    61U
#define  REG_F8H    62U
#define  REG_FCH    63U
#define  REG_100H    64U
#define  REG_104H    65U
#define  REG_108H    66U
#define  REG_10CH    67U
#define  REG_110H    68U
#define  REG_114H    69U
#define  REG_118H    70U
#define  REG_11CH    71U
#define  REG_120H    72U
#define  REG_124H    73U
#define  REG_128H    74U
#define  REG_12CH    75U
#define  REG_130H    76U
#define  REG_134H    77U
#define  REG_138H    78U
#define  REG_13CH    79U
#define  REG_140H    80U
#define  REG_144H    81U
#define  REG_148H    82U
#define  REG_14CH    83U
#define  REG_150H    84U
#define  REG_154H    85U
#define  REG_158H    86U
#define  REG_15CH    87U
#define  REG_160H    88U
#define  REG_164H    89U
#define  REG_168H    90U
#define  REG_16CH    91U
#define  REG_170H    92U
#define  REG_174H    93U
#define  REG_178H    94U
#define  REG_17CH    95U
#define  REG_180H    96U
#define  REG_184H    97U
#define  REG_188H    98U
#define  REG_18CH    99U
#define  REG_190H    100U
#define  REG_194H    101U
#define  REG_198H    102U
#define  REG_19CH    103U
#define  REG_1A0H    104U
#define  REG_1A4H    105U
#define  REG_1A8H    106U
#define  REG_1ACH    107U
#define  REG_1B0H    108U
#define  REG_1B4H    109U
#define  REG_1B8H    110U
#define  REG_1BCH    111U
#define  REG_1C0H    112U
#define  REG_1C4H    113U
#define  REG_1C8H    114U
#define  REG_1CCH    115U
#define  REG_1D0H    116U
#define  REG_1D4H    117U
#define  REG_1D8H    118U
#define  REG_1DCH    119U
#define  REG_1E0H    120U
#define  REG_1E4H    121U
#define  REG_1E8H    122U
#define  REG_1ECH    123U
#define  REG_1F0H    124U
#define  REG_1F4H    125U
#define  REG_1F8H    126U
#define  REG_1FCH    127U

extern uint32_t *S_RAM;
extern uint32_t *S_INST;
extern uint32_t *S_HEAP;

extern void TSIPPrc_p00(void);
extern uint8_t TSIPPrc_p01(void);
extern uint8_t TSIPPrc_p02(void);
extern uint8_t TSIPPrc_p03(void);
extern uint8_t TSIPPrc_p04(const uint32_t InData_SharedKeyIndex[],
                           const uint32_t InData_SessionKey[],
                           const uint32_t InData_IV[],
                           const uint32_t InData_InstData[],
                                 uint32_t OutData_InstData[]);
extern uint8_t TSIPPrc_p10(void);
extern uint8_t TSIPPrc_p11(const uint32_t InData_Operation[],
                           const uint32_t InData_IV[],
                           const uint32_t InData_KeyOperationCode[],
                                 uint32_t OutData_KeyIndex[]);
extern uint8_t TSIPPrc_p12(void);
extern uint8_t TSIPPrc_p13(void);
extern uint8_t TSIPPrc_p14(const uint32_t InData_Prog_Num[]);
extern uint8_t TSIPPrc_p15(const uint32_t InData_Prog[],
                           const uint32_t InData_ProgLength[],
                                 uint32_t OutData_Prog[],
                                 uint32_t OutData_Header[]);
extern uint8_t TSIPPrc_p16(const uint32_t InData_Header[]);
extern uint8_t TSIPPrc_p17(const uint32_t InData_Prog[],
                           const uint32_t InData_ProgLength[],
                                 uint32_t OutData_Prog[]);


extern uint8_t TSIPPrc_p40(void);
extern uint8_t TSIPPrc_p43(const uint32_t InData_IV[],
                           const uint32_t InData_InstData[],
                                 uint32_t OutData_InstData[]);
extern uint8_t TSIPPrc_p44(const uint32_t InData_Prog_Num[]);
extern uint8_t TSIPPrc_p45(const uint32_t InData_ProgNo[],
                           const uint32_t InData_Prog[],
                           const uint32_t InData_ProgLength[],
                                 uint32_t OutData_Prog[]);

extern uint8_t TSIPPrc_p30(const uint32_t InData_Text[],
                           const uint32_t MAX_CNT,
                           const uint32_t InData_Text_Last[],
                           const uint32_t InData_Text_Last_LEN[],
                                 uint32_t OutData_Text[],
                                 uint32_t OutData_Text_Last[],
                                 uint32_t OutData_Header[]);
extern uint8_t TSIPPrc_p31(const uint32_t InData_Header[],
                           const uint32_t InData_Text[],
                           const uint32_t MAX_CNT,
                           const uint32_t InData_Text_Last[],
                           const uint32_t InData_Text_Last_LEN[],
                                 uint32_t OutData_Text[],
                                 uint32_t OutData_Text_Last[]);

extern uint8_t TSIPPrc_p3c(const uint32_t InData_KeyIndex[],
                           const uint32_t InData_IV[],
                           const uint32_t MAX_CNT,
                           const uint32_t InData_Text[],
                                 uint32_t OutData_Text[]);
extern uint8_t TSIPPrc_p3d(const uint32_t InData_KeyIndex[],
                           const uint32_t InData_IV[],
                           const uint32_t MAX_CNT,
                           const uint32_t InData_Text[],
                                 uint32_t OutData_Text[]);
extern uint8_t TSIPPrc_p4c(const uint32_t InData_KeyIndex[],
                           const uint32_t InData_IV[],
                           const uint32_t MAX_CNT,
                           const uint32_t InData_Text[],
                                 uint32_t OutData_Text[]);
extern uint8_t TSIPPrc_p4d(const uint32_t InData_KeyIndex[],
                           const uint32_t InData_IV[],
                           const uint32_t MAX_CNT,
                           const uint32_t InData_Text[],
                                 uint32_t OutData_Text[]);

extern uint8_t TSIPPrc_p7a(const uint32_t InData_KeyIndex[],
                           const uint32_t InData_Text[],
                                 uint32_t OutData_Text[]);
extern uint8_t TSIPPrc_p7b(const uint32_t InData_KeyIndex[],
                           const uint32_t InData_Text[],
                                 uint32_t OutData_Text[]);
extern uint8_t TSIPPrc_p7c(const uint32_t InData_KeyIndex[],
                           const uint32_t InData_Text[],
                                 uint32_t OutData_Text[]);
extern uint8_t TSIPPrc_p7d(const uint32_t InData_KeyIndex[],
                           const uint32_t InData_Text[],
                                 uint32_t OutData_Text[]);
extern uint8_t TSIPPrc_p7e(const uint32_t InData_KeyIndex[],
                           const uint32_t InData_Text[],
                                 uint32_t OutData_Text[]);
extern uint8_t TSIPPrc_p7f(const uint32_t InData_KeyIndex[],
                           const uint32_t InData_Text[],
                                 uint32_t OutData_Text[]);

extern uint8_t TSIPPrc_p5f(const uint32_t InData_SHA1InitVal[],
                           const uint32_t InData_PaddedMsg[],
                           const uint32_t MAX_CNT,
                           const uint32_t Last_Data[],
                           const uint32_t Last_Data_Cnt,
                                 uint32_t OutData_MsgDigest[]);
extern uint8_t TSIPPrc_p6f(const uint32_t InData_SHA256InitVal[],
                           const uint32_t InData_PaddedMsg[],
                           const uint32_t MAX_CNT,
                           const uint32_t Last_Data[],
                           const uint32_t Last_Data_Cnt,
                                 uint32_t OutData_MsgDigest[]);

extern uint8_t TSIPPrc_p8a(const uint32_t InData_KeyIndex[],
                           const uint32_t InData_IV[],
                           const uint32_t MAX_CNT,
                           const uint32_t InData_PaddedMsg[],
                                 uint32_t OutData_MAC[]);
extern uint8_t TSIPPrc_p8b(const uint32_t InData_KeyIndex[],
                           const uint32_t InData_IV[],
                           const uint32_t MAX_CNT,
                           const uint32_t InData_PaddedMsg[],
                           const uint32_t Last_Data[],
                           const uint32_t InData_MsgType[],
                                 uint32_t OutData_MAC[]);
extern uint8_t TSIPPrc_p8c(const uint32_t InData_KeyIndex[],
                           const uint32_t InData_IV[],
                           const uint32_t MAX_CNT,
                           const uint32_t InData_PaddedMsg[],
                           const uint32_t Last_Data[],
                           const uint32_t InData_MsgType[],
                           const uint32_t InData_CompareMAC[],
                           const uint32_t InData_CompareMACLen[]);
extern uint8_t TSIPPrc_p9a(const uint32_t InData_KeyIndex[],
                           const uint32_t InData_IV[],
                           const uint32_t MAX_CNT,
                           const uint32_t InData_PaddedMsg[],
                                 uint32_t OutData_MAC[]);
extern uint8_t TSIPPrc_p9b(const uint32_t InData_KeyIndex[],
                           const uint32_t InData_IV[],
                           const uint32_t MAX_CNT,
                           const uint32_t InData_PaddedMsg[],
                           const uint32_t Last_Data[],
                           const uint32_t InData_MsgType[],
                                 uint32_t OutData_MAC[]);
extern uint8_t TSIPPrc_p9c(const uint32_t InData_KeyIndex[],
                           const uint32_t InData_IV[],
                           const uint32_t MAX_CNT,
                           const uint32_t InData_PaddedMsg[],
                           const uint32_t Last_Data[],
                           const uint32_t InData_MsgType[],
                           const uint32_t InData_CompareMAC[],
                           const uint32_t InData_CompareMACLen[]);

extern uint8_t TSIPPrc_p5a(const uint32_t InData_KeyIndex[],
                                 uint32_t OutData_MAC[]);
extern uint8_t TSIPPrc_p5b(const uint32_t InData_MAC[],
                           const uint32_t MAX_CNT,
                           const uint32_t InData_PaddedMsg[],
                                 uint32_t OutData_MAC[]);
extern uint8_t TSIPPrc_p5c(const uint32_t InData_KeyIndex[],
                           const uint32_t InData_MAC[],
                           const uint32_t MAX_CNT,
                           const uint32_t InData_PaddedMsg[],
                           const uint32_t Last_Data[],
                           const uint32_t Last_Data_Cnt,
                                 uint32_t OutData_MAC[]);
extern uint8_t TSIPPrc_p5d(const uint32_t InData_KeyIndex[],
                           const uint32_t InData_MAC[],
                           const uint32_t MAX_CNT,
                           const uint32_t InData_PaddedMsg[],
                           const uint32_t Last_Data[],
                           const uint32_t Last_Data_Cnt,
                           const uint32_t InData_CompareMAC[],
                           const uint32_t InData_CompareMACLen[]);
extern uint8_t TSIPPrc_p6a(const uint32_t InData_KeyIndex[],
                                 uint32_t OutData_MAC[]);
extern uint8_t TSIPPrc_p6b(const uint32_t InData_MAC[],
                           const uint32_t MAX_CNT,
                           const uint32_t InData_PaddedMsg[],
                                 uint32_t OutData_MAC[]);
extern uint8_t TSIPPrc_p6c(const uint32_t InData_KeyIndex[],
                           const uint32_t InData_MAC[],
                           const uint32_t MAX_CNT,
                           const uint32_t InData_PaddedMsg[],
                           const uint32_t Last_Data[],
                           const uint32_t Last_Data_Cnt,
                                 uint32_t OutData_MAC[]);
extern uint8_t TSIPPrc_p6d(const uint32_t InData_KeyIndex[],
                           const uint32_t InData_MAC[],
                           const uint32_t MAX_CNT,
                           const uint32_t InData_PaddedMsg[],
                           const uint32_t Last_Data[],
                           const uint32_t Last_Data_Cnt,
                           const uint32_t InData_CompareMAC[],
                           const uint32_t InData_CompareMACLen[]);

extern uint8_t TSIPPrc_p3f(uint32_t OutData_Text[]);

extern uint8_t TSIPPrc_p3e(const uint32_t InData_Operation[],
                           const uint32_t InData_IV[],
                           const uint32_t InData_KeyOperationCode[],
                                 uint32_t OutData_KeyIndex[]);

extern uint8_t TSIPPrc_p50(const uint32_t InData_Certificates[],
                           const uint32_t InData_CertificatesLength[],
                           const uint32_t InData_Signature[],
                           const uint32_t InData_CertificatesInfo[],
                                 uint32_t OutData_PubKey[]);
extern uint8_t TSIPPrc_p51(const uint32_t InData_PubKey[],
                           const uint32_t InData_TBSCertificate[],
                           const uint32_t InData_TBSCertificateLength[],
                           const uint32_t InData_Signature[],
                           const uint32_t InData_TBSCertificateInfo[],
                                 uint32_t OutData_PubKey[]);
extern uint8_t TSIPPrc_p52(const uint32_t InData_PubKey[],
                           const uint32_t InData_TBSCertificate[],
                           const uint32_t InData_TBSCertificateLength[],
                           const uint32_t InData_Signature[],
                           const uint32_t InData_TBSCertificateInfo[],
                                 uint32_t OutData_PubKey[]);

extern uint8_t TSIPPrc_p61(const uint32_t InData_PubKey[],
                           const uint32_t InData_PreMasterSecret[],
                                 uint32_t OutData_PreMasterSecret[]);

extern uint8_t TSIPPrc_p70(uint32_t OutData_PreMasterSecret[]);
extern uint8_t TSIPPrc_p71(const uint32_t InData_PreMasterSecret[],
                           const uint32_t InData_ClientRandom[],
                           const uint32_t InData_ServerRandom[],
                                 uint32_t OutData_MasterSecret[]);
extern uint8_t TSIPPrc_p72(const uint32_t InData_Sel_CipherSuite[],
                           const uint32_t InData_MasterSecret[],
                           const uint32_t InData_ClientRandom[],
                           const uint32_t InData_ServerRandom[],
                                 uint32_t OutData_ClientMACKeyOperationCode[],
                                 uint32_t OutData_ServerMACKeyOperationCode[],
                                 uint32_t OutData_ClientEncKeyOperationCode[],
                                 uint32_t OutData_ServerEncKeyOperationCode[],
                                 uint32_t OutData_ClientIV[],
                                 uint32_t OutData_ServerIV[]);
extern uint8_t TSIPPrc_p73(const uint32_t InData_Sel_VerifyData[],
                           const uint32_t InData_MasterSecret[],
                           const uint32_t InData_HandShakeHash[],
                                 uint32_t OutData_VerifyData[]);
extern uint8_t TSIPPrc_p74(const uint32_t MAX_CNT,
                                 uint32_t OutData_KeyIndex[],
                                 uint32_t OutData_Modulus[],
                                 uint32_t OutData_PubExp[]);

extern void TSIPPrc_subprc01(void);
extern void TSIPPrc_subprc02(void);
extern void TSIPPrc_subprc18(void);

extern void TSIPPrc_subprc22(void);
extern void TSIPPrc_subprc70(void);
extern void TSIPPrc_subprc71(void);
extern void TSIPPrc_subprc72(void);

#endif  /* TSIP_PROCEDURE_H */
