/*
 * Copyright (c) 2023, Renesas Electronics Corporation
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
#include "mmio.h"
#include "dgtable.h"
#include "ramckmdl.h"
#include "dgmodul4.h"
#include "dgemmc.h"
#include "devdrv.h"
#include "reg_rzg2.h"
#if USB_ENABLE == 1
#include "usb_lib.h"
#endif /* USB_ENABLE == 1 */
#include "secprv.h"
#include "spiflash1drv.h"
#include "emmc_config.h"
#include "emmc_hal.h"
#include "emmc_std.h"
#include "emmc_def.h"
#include "emmc_registers.h"
#include "R_TSIP_Boot_Lib.h"

/*******************************************************************************
 Exported global variables (to be accessed by other files)
 ******************************************************************************/
extern uint32_t gSpiFlashSvArea;
extern uint32_t gQspi_sa_size;
extern uint32_t gQspi_end_addess;
extern uint32_t gUserPrgStartAdd;

extern uint32_t gUserFactProgKey[];

/*******************************************************************************
 Private global variables and functions
 ******************************************************************************/
static struct {
    uint8_t s_inst[1296];
    uint8_t s_ram[512];
} s_shared_mem;

static struct {
    uintptr_t load_adr;
    uintptr_t save_adr;
    uint32_t  data_len;
    uint32_t  partition;
} s_data_list[REENC_BOOT_DATA_MAX];

static char *boot_stage[REENC_BOOT_DATA_MAX] =
{
    "1st", "2nd", "3rd", "4th", "5th", "6th", "7th", "8th",
    "9th", "10th", "11th", "12th", "13th", "14th", "15th", "16th"
};

static uint32_t s_load_mode;
static uint32_t s_flash_type;
static uint32_t s_partition;

static uintptr_t s_work_ram_min;
static uintptr_t s_work_ram_max;
static uintptr_t s_load_ram_min;
static uintptr_t s_load_ram_max;
static uintptr_t s_save_mem_min;
static uintptr_t s_temp_ram_min;

static EMMC_SECTOR emmc_sector;

static int32_t startup(void);
static int32_t inject_keyring(void);
static int32_t reencrypt_data(void);
static int32_t load_image(void);
static int32_t flash_init(void);
static int32_t flash_write(void);
static int32_t tsip_inject_key(void);
static int32_t tsip_reenc_data(int32_t data_count);

void dgG2LoadSpiflash0_2s(void)
{
    s_load_mode = 0;
    s_flash_type = 0;
    startup();
    inject_keyring();
    if (0 == reencrypt_data())
    {
        PutStr("XLS2S Complete!",1);
    }
}

void dgG2LoadSpiflash0_3s(void)
{
    s_load_mode = 1;
    s_flash_type = 0;
    startup();
    inject_keyring();
    if (0 == reencrypt_data())
    {
        PutStr("XLS3S Complete!",1);
    }
}

void dg_emmc_write_mots(void)
{
    s_load_mode = 0;
    s_flash_type = 1;
    startup();
    inject_keyring();
    if (0 == reencrypt_data())
    {
        PutStr("EM_WS Complete!",1);
    }
}

void dg_emmc_write_bins(void)
{
    s_load_mode = 1;
    s_flash_type = 1;
    startup();
    inject_keyring();
    if (0 == reencrypt_data())
    {
        PutStr("EM_WBS Complete!",1);
    }
}

static int32_t startup(void)
{
    gUserPrgStartAdd = LS_WORK_DRAM_SADD;

    s_work_ram_min = LS_WORK_DRAM_SADD;
    s_work_ram_max = LS_WORK_DRAM_EADD_64M;

    s_temp_ram_min = s_work_ram_max + 1;

    s_load_ram_min = 0;
    s_load_ram_max = 0;
    s_save_mem_min = 0;

    /* Init flash memory */
    if(0 != flash_init())
    {
        return -1;
    }

    mmio_write_32(CPG_CPGWPR,  0x5A5AFFFF);
    mmio_write_32(CPG_CPGWPCR, 0xA5A50000);

    return 0;
}

static int32_t inject_keyring(void)
{
    PutStr("",1);
    PutStr(" Injection of Temporary Encrypted Keyring",1);

    if (0 != load_image())
    {
        return -1;
    }

    if (0 != tsip_inject_key())
    {
        return -1;
    }

    if (0 != flash_write())
    {
        return -1;
    }

    return 0;
}

static int32_t reencrypt_data(void)
{
    int32_t data_count;

    uintptr_t tmp_data_base;

    PutStr("",1);
    PutStr(" Re-Encryption of Temporary Encrypted Data",1);

    PutStr("",1);
    PutStr(" Number of data to be re-encrypted (H'01 ~ H'0F)",1);
    SetSizeInputKey(&data_count);

    if ((data_count <= 0) || (REENC_BOOT_DATA_MAX < data_count))
    {
        PutStr("",1);
        PutStr("Invalid data count.",1);
        return -1;
    }

    tmp_data_base = s_temp_ram_min;

    PutStr("",1);
    PutStr(" ===========================================",1);
    PutStr("   Note: Please send in the order of the    ",1);
    PutStr("   firmware to be verified by secure boot.  ",1);
    PutStr(" ===========================================",1);

    for (int32_t i = 0; i < data_count; i++)
    {
        PutStr("",1);
        PutStr(boot_stage[i],0);
        PutStr(" firmware to be verified ... ",1);
        if (0 != load_image())
        {
            return -1;
        }

        s_data_list[i].load_adr = tmp_data_base;
        s_data_list[i].save_adr = s_save_mem_min;
        s_data_list[i].data_len = s_load_ram_max - s_load_ram_min + 1;
        s_data_list[i].partition = s_partition;

        memcpy((void *)s_data_list[i].load_adr, (void *)s_load_ram_min, s_data_list[i].data_len);

        tmp_data_base += s_data_list[i].data_len;
    }

    if (0 != tsip_reenc_data(data_count))
    {
        return -1;
    }

    for (int32_t i = 0; i < data_count; i++)
    {
        s_save_mem_min = s_data_list[i].save_adr;
        s_load_ram_min = s_data_list[i].load_adr;
        s_load_ram_max = s_data_list[i].load_adr + s_data_list[i].data_len - 1;
        s_partition    = s_data_list[i].partition;

        if (0 != flash_write())
        {
            return -1;
        }
    }

    return 0;
}

static int32_t spi_flash_init(void)
{
    PutStr(" ",1);
    PutStr("=== Qspi writing of RZ/G2 Board Command ===",1);
    PutStr("Load Program to SPI Flash",1);
    PutStr("Writes to any of SPI address.",1);

    InitRPC_Mode();

    if(0 != CheckQspiFlashId())
    {
        return -1;
    }

    gSpiFlashSvArea = 3;

    s_partition = 0;

    return 0;
}

static int32_t emmc_flash_init(void)
{
    PutStr(" ",1);
    PutStr("=== eMMC writing of RZ/G2 Board Command ===",1);
    PutStr("Load Program to eMMC",1);
    PutStr("Writes to any of eMMC address",1);

    if (EMMC_SUCCESS != dg_emmc_check_init())
    {
        PutStr("eMMC Init ERROR!",1);
        return -1;
    }

    SetSectorData(&emmc_sector);

    s_partition = EMMC_PARTITION_BOOT_1;

    return 0;
}

static int32_t flash_init(void)
{
    switch(s_flash_type)
    {
#if SERIAL_FLASH
    case 0:
        return spi_flash_init();
#endif
#if EMMC
    case 1:
        return emmc_flash_init();
#endif
    default:
        return -1;
    }
}

static int32_t spi_load_mot(void)
{
    PutStr(" ",1);
    PutStr("Program Top Address & Qspi Save Address",1);

    /* WorkMemory Clear (Write H'FF) */
    FillData32Bit((uint32_t *)s_work_ram_min, (uint32_t *)s_work_ram_max, 0xFFFFFFFF);

    PutStr("===== Please Input Program Top Address ====",1);
    SetAddInputKey(&gUserPrgStartAdd);

    PutStr(" ",1);
    PutStr("====== Please Input Qspi Save Address =====",1);
    SetAddInputKey((uint32_t *)&s_save_mem_min);

    PutStr("",1);
    if (gQspi_end_addess < s_save_mem_min)
    {
        PutStr("Address Input Error", 1);
        PutStr("Range of H'000_0000 ~ H'0FF_7FFF", 1);
        return -1;
    }

    if (dgLS_Load_Offset2((uint32_t *)&s_load_ram_max, (uint32_t *)&s_load_ram_min))
    {
        return -1;
    }

    s_save_mem_min = s_save_mem_min + (s_load_ram_min - s_work_ram_min);

    return 0;
}

static int32_t spi_load_bin(void)
{
    char bin_data;
    uint32_t image_offset = 0U;
    uint32_t image_size = 0U;

    PutStr(" ",1);
    PutStr("Program size & Qspi Save Address",1);

    /* WorkMemory Clear (Write H'FF) */
    FillData32Bit((uint32_t *)s_work_ram_min, (uint32_t *)s_work_ram_max, 0xFFFFFFFF);

    PutStr("======== Please Input Program size ========",1);
    SetSizeInputKey(&image_size);

    PutStr(" ",1);
    PutStr("====== Please Input Qspi Save Address =====",1);
    SetAddInputKey((uint32_t *)&s_save_mem_min);

    PutStr(" ",1);
    if (gQspi_end_addess < s_save_mem_min)
    {
        PutStr("Address Input Error", 1);
        PutStr("Range of H'000_0000 ~ H'0FF_7FFF", 1);
        return -1;
    }

    /* Load binary image */
    PutStr("please send ! (binary)", 1);

#if USB_ENABLE == 1
    if (gTerminal == USB_TERMINAL)
    {
        image_offset = ((image_size + (DMA_TRANSFER_SIZE - 1)) & DMA_ROUNDUP_VALUE);
        USB_ReadDataWithDMA((unsigned long)s_work_ram_min, image_offset);
    }
    else
    {
        while (image_offset < image_size)
        {
            GetChar(&bin_data);
            *(uint8_t *)(s_work_ram_min + image_offset) = bin_data;
            image_offset++;
        }
    }
#else  /* USB_ENABLE == 1 */
    while (image_offset < image_size)
    {
        GetChar(&bin_data);
        *(uint8_t *)(s_work_ram_min + image_offset) = bin_data;
        image_offset++;
    }
#endif /* USB_ENABLE == 1 */

    s_load_ram_min = s_work_ram_min;
    s_load_ram_max = s_work_ram_min + image_size - 1;

    return 0;
}

static int32_t emmc_load_mot(void)
{
    uint32_t sector_size;

    PutStr(" ",1);
    DispAreaData(emmc_sector);

    if (1 != InputEmmcSectorArea(&s_partition))
    {
        return -1;
    }

    switch(s_partition)
    {
        case EMMC_PARTITION_USER_AREA:	//User Partition Area Program
            PutStr("-- User Partition Area Program --------------------------",1);
            break;
        case EMMC_PARTITION_BOOT_1:		//Boot Partition 1 Program
            PutStr("-- Boot Partition 1 Program -----------------------------",1);
            break;
        case EMMC_PARTITION_BOOT_2:		//Boot Partition 2 Program
            PutStr("-- Boot Partition 2 Program -----------------------------",1);
            break;
        default:
            return -1;
    }

    if (!InputEmmcSector(s_partition, emmc_sector.maxSectorCount[s_partition],
                        (uint32_t *)&s_save_mem_min, &sector_size, EMMC_INPUT_TYPE_WRITE))
    {
        return -1;
    }

    if (!InputEmmcPrgStartAdd(&gUserPrgStartAdd))
    {
        return -1;
    }

    PutStr("",1);
    if (dg_emmc_mot_load((uint32_t *)&s_load_ram_max, (uint32_t *)&s_load_ram_min, gUserPrgStartAdd))
    {
        return -1;
    }

    s_save_mem_min = (s_save_mem_min << 9) + (s_load_ram_min - s_work_ram_min);

    return 0;
}

static int32_t emmc_load_bin(void)
{
    uint32_t sector_size;
    uint32_t image_offset = 0U;
    uint32_t image_size = 0U;

    if (1 != InputEmmcSectorArea(&s_partition))
    {
        return -1;
    }

    switch(s_partition)
    {
        case EMMC_PARTITION_USER_AREA:	//User Partition Area Program
            PutStr("-- User Partition Area Program --------------------------",1);
            break;
        case EMMC_PARTITION_BOOT_1:		//Boot Partition 1 Program
            PutStr("-- Boot Partition 1 Program -----------------------------",1);
            break;
        case EMMC_PARTITION_BOOT_2:		//Boot Partition 2 Program
            PutStr("-- Boot Partition 2 Program -----------------------------",1);
            break;
        default:
            return -1;
    }

    if (!InputEmmcSector(s_partition, emmc_sector.maxSectorCount[s_partition],
                        (uint32_t *)&s_save_mem_min, &sector_size, EMMC_INPUT_TYPE_WRITE))
    {
        return -1;
    }

    s_save_mem_min = s_save_mem_min << 9;

    if (!InputFileSize(&image_size))
    {
        return -1;
    }
    PutStr("please send binary file!",1);

#if USB_ENABLE == 1
    if (gTerminal == USB_TERMINAL)
    {
        image_offset = ((image_size + (DMA_TRANSFER_SIZE - 1)) & DMA_ROUNDUP_VALUE);
        USB_ReadDataWithDMA((unsigned long)s_work_ram_min, image_offset);
    }
    else
    {
        dg_emmc_write_bin_serial((uint32_t *)s_work_ram_min, image_size);
    }
#else  /* USB_ENABLE == 1 */
    dg_emmc_write_bin_serial((uint32_t *)s_work_ram_min, image_size);
#endif /* USB_ENABLE == 1 */

    s_load_ram_min = s_work_ram_min;
    s_load_ram_max = s_work_ram_min + image_size - 1;

    return 0;
}

static int32_t load_image(void)
{
    switch(s_flash_type)
    {
#if SERIAL_FLASH
    case 0:
        if (0U == s_load_mode)
            return spi_load_mot();
        else
            return spi_load_bin();
#endif
#if EMMC
    case 1:
        if (0U == s_load_mode)
            return emmc_load_mot();
        else
            return emmc_load_bin();
#endif
    default:
        return -1;
    }
}

static int32_t spi_flash_write(void)
{
    char str[64];

    uint32_t image_save_min;
    uint32_t image_save_max;
    uint32_t image_save_len;

    uint32_t sector_mask;
    uint32_t save_sector_top;
    uint32_t save_sector_end;
    uint32_t save_sector_len;

    image_save_min = (uint32_t)s_save_mem_min;
    image_save_max = (uint32_t)s_save_mem_min + (s_load_ram_max - s_load_ram_min);
    image_save_len = image_save_max - image_save_min + 1;

    if (image_save_max > gQspi_end_addess)
    {
        PutStr("Program over size Error", 1);
        PutStr(" SpiFlashMemory Stat Address : H'", 0);
        Data2HexAscii(image_save_min, str, 4);
        PutStr(str ,1);
        PutStr(" SpiFlashMemory End Address  : H'", 0);
        Data2HexAscii(image_save_max, str, 4);
        PutStr(str, 1);
        return -1;
    }
    sector_mask = (~gQspi_sa_size) + 1;

    save_sector_top = image_save_min & sector_mask;
    save_sector_end = image_save_max | (~sector_mask);
    save_sector_len = save_sector_end - save_sector_top;

    if (CkQspiFlash1ClearSectorSize(WORK_SPI_LOAD_AREA, save_sector_top, save_sector_len, 1))
    {
        return -1;
    }

    PutStr("",1);
    PutStr("SAVE SPI-FLASH.......",0);
    SaveDataWithBuffeQspiFlash(s_load_ram_min, image_save_min, image_save_len);
    PutStr("",1);
    PutStr("========= Qspi Save Information  ==========",1);
    PutStr(" SpiFlashMemory Stat Address : H'",0);
    Data2HexAscii(image_save_min, str, 4);
    PutStr(str,1);
    PutStr(" SpiFlashMemory End Address  : H'",0);
    Data2HexAscii(image_save_max, str, 4);
    PutStr(str,1);
    PutStr("===========================================",1);
    PutStr("",1);
    return 0;
}

static int32_t emmc_flash_write(void)
{
    char str[64];

    uint32_t image_save_min;
    uint32_t image_save_max;
    uint32_t image_save_len;

    uint32_t save_image_ofs;

    uint32_t save_sector_top;
    uint32_t save_sector_end;
    uint32_t save_sector_cnt;

    image_save_min = (uint32_t)s_save_mem_min;
    image_save_max = (uint32_t)s_save_mem_min + (s_load_ram_max - s_load_ram_min);
    image_save_len = image_save_max - image_save_min + 1;

    save_image_ofs = image_save_min & ((1 << 9) - 1);

    memcpy((void *)(s_temp_ram_min + save_image_ofs), (void *)s_load_ram_min, image_save_len);

    save_sector_top = image_save_min >> 9;
    save_sector_end = image_save_max >> 9;
    save_sector_cnt = (save_sector_end - save_sector_top) + 1;

    if (EMMC_SUCCESS != emmc_select_partition(s_partition))
    {
        PutStr("eMMC Partition select FAIL",1);
        return -1;
    }

    SETR_32(SD_SIZE, EMMC_BLOCK_LENGTH);

    PutStr("",1);
    PutStr("SAVE eMMC.......",1);
    if (EMMC_SUCCESS != emmc_write_sector((uint32_t *)s_temp_ram_min, save_sector_top, save_sector_cnt, 0x00000001))
    {
        PutStr("eMMC Write ERR",1);
        return -1;
    }

    PutStr("========= eMMC Save Information  ==========",1);
    PutStr(" eMMC Partition              : H'",0);
    Data2HexAscii(s_partition, str, 4);
    PutStr(str,1);
    PutStr(" eMMC Save Start Address     : H'",0);
    Data2HexAscii(image_save_min, str, 4);
    PutStr(str,1);
    PutStr(" eMMC Save End Address       : H'",0);
    Data2HexAscii(image_save_max, str, 4);
    PutStr(str,1);
    PutStr("===========================================",1);

    return 0;
}

static int32_t flash_write(void)
{
    switch (s_flash_type)
    {
#if SERIAL_FLASH
    case 0:
        return spi_flash_write();
#endif
#if EMMC
    case 1:
        return emmc_flash_write();
#endif
    default:
        return -1;
    }
}

static int32_t tsip_inject_key(void)
{
    PutStr("",1);
    PutStr("==================================================",1);

    PutStr(" R_TSIP_Init .....",0);
    if (R_PASS != R_TSIP_Init((uint8_t *)&s_shared_mem.s_ram, (uint8_t *)&s_shared_mem.s_inst))
    {
        PutStr(" Error!", 1);
        return -1;
    }
    else
    {
        PutStr(" Success!", 1);
    }

    PutStr(" R_TSIP_Inject_Key ..... ",0);
    if (R_PASS != R_TSIP_Inject_Key((uint8_t *)gUserFactProgKey, (uint8_t *)s_load_ram_min))
    {
        PutStr("Error!", 1);
        return -1;
    }
    else
    {
        PutStr("Success!", 1);
    }

    s_load_ram_max = s_load_ram_min + sizeof(s_shared_mem.s_inst) + sizeof(uint64_t) - 1;

    /* WorkMemory Clear (Write H'FF) */
    FillData32Bit((uint32_t *)s_load_ram_min, (uint32_t *)s_load_ram_max, 0xFFFFFFFF);

    *(uint64_t *)s_load_ram_min = sizeof(s_shared_mem.s_inst);

    memcpy((void *)(s_load_ram_min + sizeof(uint64_t)), s_shared_mem.s_inst, sizeof(s_shared_mem.s_inst));

    PutStr("==================================================",1);
    PutStr("",1);

    return 0;
}

static int32_t tsip_reenc_data(int32_t data_count)
{
    uintptr_t enc_data_base = 0;

    TSIP_REENC_BOOT_DATA reenc_data;

    /* WorkMemory Clear (Write H'FF) */
    FillData32Bit((uint32_t *)s_work_ram_min, (uint32_t *)s_work_ram_max, 0xFFFFFFFF);

    memset(&reenc_data, 0, sizeof(reenc_data));

    PutStr("",1);
    PutStr("==================================================",1);

    PutStr(" R_TSIP_Init .....",0);
    if (R_PASS != R_TSIP_Init((uint8_t *)&s_shared_mem.s_ram, (uint8_t *)&s_shared_mem.s_inst))
    {
        PutStr(" Error!", 1);
        return -1;
    }
    else
    {
        PutStr(" Success!", 1);
    }

    enc_data_base = s_work_ram_min;

    for (int32_t i = 0; i < data_count; i++)
    {
        uint64_t reenc_data_size;

        if (0 == i)
            reenc_data_size = s_data_list[i].data_len + 64;
        else
            reenc_data_size = s_data_list[i].data_len + 16;

        reenc_data[i].InData_BootData = (uint8_t *)s_data_list[i].load_adr;
        reenc_data[i].InData_BootData_ByteSize = s_data_list[i].data_len;
        reenc_data[i].OutData_BootData = (uint8_t *)enc_data_base + sizeof(reenc_data_size);

        s_data_list[i].load_adr = enc_data_base;
        s_data_list[i].data_len = reenc_data_size + sizeof(reenc_data_size);

        *(uint64_t *)enc_data_base = reenc_data_size;

        enc_data_base += s_data_list[i].data_len;
    }

    PutStr(" R_TSIP_ReEncBootData ..... ",0);
    if (R_PASS != R_TSIP_ReEncBootData(&reenc_data))
    {
        PutStr("Error!", 1);
        return -1;
    }
    else
    {
        PutStr("Success!", 1);
    }
    PutStr("==================================================",1);
    PutStr("",1);

    return 0;
}
