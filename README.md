# README of the RZ/G2 Flash Writer

<Div Align="right">
Renesas Electronics Corporation

Sep-24-2019
</Div>

The RZ/G2 flash writer is sample software for Renesas RZ/G2 Group MPUs.
The RZ/G2 flash writer downloads the some of the raw images from Host PC via SCIF and writes the raw images to the Serial NOR Flash or eMMC.

## 1. Overview

This document explains about RZ/G2 flash writer sample software for Renesas RZ/G2 Group MPUs.

The RZ/G2 flash writer is downloaded from the Host PC via SCIF by boot ROM.

And the RZ/G2 flash writer downloads the some of the raw images from Host PC via SCIF, and writes the raw images to the Serial NOR Flash (hereafter referred to as &ldquo;Serial Flash&rdquo;) or eMMC.

The RZ/G2 flash writer's Serial Flash writing support is on-board Serial NOR Flash(i.e. W25M512JW).

The RZ/G2 flash writer eMMC writing support is High Speed SDR(i.e. 50MHz) and x8 bus width mode.

[Chapter 2](#2-operating-environment) describes the operating environment.

[Chapter 3](#3-software) describes the software.

[Chapter 4](#4-how-to-build-the-rzg2-flash-writer) explains example of how to build the RZ/G2 flash writer.

[Chapter 5](#5-how-to-run-the-rzg2-flash-writer) explains example of how to perform the RZ/G2 flash writer.

[Chapter 6](#6-error-case-to-handle) explains how to handle error case.

[Chapter 7](#7-revision-history) explains revision history.

*Note) This sample software does not support the file system. Therefore, can be write the raw image to the Serial NOR Flash or eMMC.*

## 1.2. License

BSD-3-Clause (please see file [LICENSE.md](LICENSE.md) for the details)

## 1.3. Notice

The RZ/G2 flash writer is distributed as a sample software from Renesas without any warranty or support.

## 1.4. Contributing

To contribute to this layer, you should email patches to renesas-rz@renesas.com. Please send .patch files as email attachments, not embedded in the email body.

## 1.5. References

The following table shows the document related to this function.

### Related Document

| Number | Issuer  | Title                                                          | Edition           |
|--------|---------|----------------------------------------------------------------|-------------------|
| 1      | JEDEC   | Embedded Multi-Media Card (eMMC) Electrical Standard (5.01)    | JESD84-B50.1      |
| 2      | Renesas | Release Note for Verified Linux Package for 64bit kernel       | Rev.1.01 or later |
| 3      | Renesas | RZ/G2 Yocto recipe Start-Up Guide                              | Rev.1.01 or later |
| 4      | Renesas | RZ/G2 Reference Boards Start-up Guide                          | Rev.1.01 or later |

## 2. Operating Environment

### 2.1. Hardware Environment

The following table lists the hardware needed to use this function.

#### Hardware environment

| Name                          | Note                                      |
|-------------------------------|-------------------------------------------|
| Target board                  | Hoperun HiHope RZ/G2[MN] platform         |
|                               | Silicon Linux RZ/G2E evaluation kit       |
| Host PC                       | Ubuntu Desktop 14.04(64bit) or later      |

The following table shows Serial Flash and eMMC support for each MPU.

##### Serial Flash / eMMC support status of each MPU

| MPU    | Read/Write the Serial Flash | Boot from the Serial Flash | Read/Write the eMMC | Boot from the eMMC | MMC interface |
|--------|-----------------------------|----------------------------|---------------------|--------------------|---------------|
| RZ/G2M | Support                     | Support                    | Support             | Support *1         | MMC1          |
| RZ/G2N | Support                     | Support                    | Support             | Support            | MMC1          |
| RZ/G2E | Support                     | Support                    | Support             | Support            | MMC1          |

\*1: Some early samples are not support booting from the eMMC. Please contact to Renesas sales for further details.

## 2.2. Software Environment

The following table lists the software required to use this sample software.

### Software environment

| Name                | Note                                                             |
|---------------------|------------------------------------------------------------------|
| Linaro Toolchain *1 | Linaro Binary Toolchain Release GCC 7.3-2018.05 for aarch64-elf. |
| Yocto SDK *1 *2     | Yocto SDK built from Yocto environment for RZ/G2 Group           |

\*1: One of the above environments is required.

\*2: Regarding how to get the Yocto SDK, refer to [Related Document](#related-document) No.2 or No.3.

## 3. Software

### 3.1. Function

This package has the following functions.

- Write to the images to the Serial Flash.
- Erase the Serial Flash.
- Display the CID/CSD/EXT_CSD registers of eMMC.
- Modify the EXT_CSD registers of eMMC.
- Write to the images to the boot partition of eMMC.
- Write to the images to the user data area of eMMC.
- Erase the boot partition of eMMC.
- Erase the user data area of eMMC.
- Change to the SCIF baud rate setting.
- Display the command help.

### 3.3. Option setting

The RZ/G2 flash writer support the following build options.

#### 3.3.2. BOARD

Select from the following table according to the board settings.

If this option is not selected, the default value is HIHOPE.

| BOARD    | BOARD setting                                                              |
|----------|----------------------------------------------------------------------------|
| HIHOPE   | Generate binary that works on Hoperun HiHope RZ/G2[MN] platform. (default) |
| EK874    | Generate binary that works on Silicon Linux RZ/G2E evaluation kit.         |

#### 3.3.5. SERIAL_FLASH

Select from the following table according to the Serial Flash writing function.

If this option is not selected, the default value is ENABLE.

##### Association table for the SERIAL_FLASH value and valid Serial Flash writing function settings

| SERIAL_FLASH | Serial Flash writing setting                          |
|--------------|-------------------------------------------------------|
| ENABLE       | Serial Flash writing function is available. (default) |
| DISABLE      | Serial Flash writing function is not available.       |

#### 3.3.6 eMMC

Select from the following table according to the eMMC writing function.

If this option is not selected, the default value is ENABLE.

##### Association table for the eMMC value and valid eMMC writing function settings

| EMMC    | Serial Flash writing setting                  |
|---------|-----------------------------------------------|
| ENABLE  | eMMC writing function is available. (default) |
| DISABLE | eMMC writing function is not available.       |

### 3.4. Command specification

The following table shows the command list.

#### Command list

| Command  | Description                                                                                        |
|----------|----------------------------------------------------------------------------------------------------|
| XLS2     | Write the S-record format images to the Serial Flash.                                              |
| XCS      | Erase the Serial Flash.                                                                            |
| EM_DCID  | Display the CID registers of eMMC.                                                                 |
| EM_DCSD  | Display the CSD registers of eMMC.                                                                 |
| EM_DECSD | Display the EXT_CSD registers of eMMC.                                                             |
| EM_SECSD | Modify the EXT_CSD registers of eMMC.                                                              |
| EM_W     | Write to the S-record format images to the user data area of eMMC, and the boot partition of eMMC. |
| EM_WB    | Write to the raw binary images to the user data area of eMMC, and the boot partition of eMMC.      |
| EM_E     | Erase the user data area of eMMC, and the boot partition of eMMC.                                  |
| SUP      | Change the SCIF baud rate setting.                                                                 |
| H        | Display the command help.                                                                          |

#### 3.4.1. Write to the S-record format images to the Serial Flash

This command writes the S-record format image to Serial Flash.

##### Example of writing data for the Serial Flash boot

| Filename                       | Program Top Address | Flash Save Address | Description            |
|--------------------------------|---------------------|--------------------|------------------------|
| bootparam_sa0.srec             | H'E6320000          | H'000000           | Loader(Boot parameter) |
| bl2-`<board_name>`.srec        | H'E6304000          | H'040000           | Loader                 |
| cert_header_sa6.srec           | H'E6320000          | H'180000           | Loader(Certification)  |
| bl31-`<board_name>`.srec       | H'44000000          | H'1C0000           | ARM Trusted Firmware   |
| u-boot-elf-`<board_name>`.srec | H'50000000          | H'300000           | U-boot                 |

The following shows the procedure of this command.

*Note) The following procedure is an example on HiHope RZ/G2M board.*

```text
>XLS2
===== Qspi writing of RZ/G2 Board Command =============
Load Program to Spiflash
Writes to any of SPI address.
 Winbond : W25M512JW
Program Top Address & Qspi Save Address
===== Please Input Program Top Address ============
  Please Input : H'
```

Please enter the program top address of the write image in hexadecimal.

```text
>XLS2
===== Qspi writing of RZ/G2 Board Command =============
Load Program to Spiflash
Writes to any of SPI address.
 Winbond : W25M512JW
Program Top Address & Qspi Save Address
===== Please Input Program Top Address ============
  Please Input : H'e6304000

===== Please Input Qspi Save Address ===
  Please Input : H'
```

Please enter the flash save address in hexadecimal.

```text
>XLS2
===== Qspi writing of RZ/G2 Board Command =============
Load Program to Spiflash
Writes to any of SPI address.
 Winbond : W25M512JW
Program Top Address & Qspi Save Address
===== Please Input Program Top Address ============
  Please Input : H'e6304000

===== Please Input Qspi Save Address ===
  Please Input : H'40000
Work RAM(H'50000000-H'53FFFFFF) Clear....
please send ! ('.' & CR stop load)
```

Please download the write image in S-record format.

```text
>XLS2
===== Qspi writing of RZ/G2 Board Command =============
Load Program to Spiflash
Writes to any of SPI address.
 Winbond : W25M512JW
Program Top Address & Qspi Save Address
===== Please Input Program Top Address ============
  Please Input : H'e6304000

===== Please Input Qspi Save Address ===
  Please Input : H'40000
Work RAM(H'50000000-H'53FFFFFF) Clear....
please send ! ('.' & CR stop load)
SPI Data Clear(H'FF) Check :H'00040000-0005FFFF,Clear OK?(y/n)
```

Please enter the 'y' key when asked to clear.

If Flash is erased, it will not be asked.

```text
>XLS2
===== Qspi writing of RZ/G2 Board Command =============
Load Program to Spiflash
Writes to any of SPI address.
 Winbond : W25M512JW
Program Top Address & Qspi Save Address
===== Please Input Program Top Address ============
  Please Input : H'e6304000

===== Please Input Qspi Save Address ===
  Please Input : H'40000
Work RAM(H'50000000-H'53FFFFFF) Clear....
please send ! ('.' & CR stop load)
SPI Data Clear(H'FF) Check :H'00040000-0005FFFF Erasing...Erase Completed
SAVE SPI-FLASH.......
======= Qspi  Save Information  =================
 SpiFlashMemory Stat Address : H'00040000
 SpiFlashMemory End Address  : H'0005B0E3
===========================================================

>
```

Image writing has been completed.

#### 3.4.3. Erase the Serial NOR Flash

This command erases all sectors of Serial Flash.

The following shows the procedure of this command.

*Note) The following procedure is an example on HiHope RZ/G2M board.*

```text
>XCS
ALL ERASE SpiFlash memory
Clear OK?(y/n)
```

Please enter the 'y' key.

```text
>XCS
ALL ERASE SpiFlash memory
 Winbond : W25M512JW
 ERASE QSPI-FLASH (60sec[typ]).... complete!
>
```

Selected Serial Flash has been erased.

#### 3.4.4. Display the CID registers command

This command displays the contents of the CID registers of the eMMC.

The following shows the procedure of this command.

```text
EM_DCID

[CID Field Data]
[127:120]  MID  0x13
[113:112]  CBX  0x01
[111:104]  OID  0x4E
[103: 56]  PNM  0x52314A35374C
[ 55: 48]  PRV  0x10
[ 47: 16]  PSN  0x18E3D660
[ 15:  8]  MDT  0x73
[  7:  1]  CRC  0x00
```

#### 3.4.5. Display the CSD registers command

This command displays the contents of the CSD registers of eMMC.

The following shows the procedure of this command.

```text
>EM_DCSD

[CSD Field Data]
[127:126]  CSD_STRUCTURE       0x03
[125:122]  SPEC_VERS           0x04
[119:112]  TAAC                0x7F
...
[ 11: 10]  FILE_FORMAT         0x00
[  9:  8]  ECC                 0x00
[  7:  1]  CRC                 0x00
```

#### 3.4.6. Display the EXT_CSD registers command

This command displays the contents of the EXT_CSD registers of the eMMC.

The following shows the procedure of this command.

```text
>EM_DECSD

[EXT_CSD Field Data]
[505:505]  EXT_SECURITY_ERR                           0x00
[504:504]  S_CMD_SET                                  0x01
[503:503]  HPI_FEATURES                               0x01
...
[142:140]  ENH_SIZE_MULT                              0x000000
[139:136]  ENH_START_ADDR                             0x00000000
[134:134]  SEC_BAD_BLK_MGMNT                          0x00
```

#### 3.4.7. Modify the EXT_CSD registers of eMMC command

This command modifies the contents of the registers of EXT_CSD of the eMMC.

The following shows the procedure of this command.

```text
>EM_SECSD
  Please Input EXT_CSD Index(H'00 - H'1FF) :
```

Enter the address of the EXT_CSD register in hexadecimal.

```text
>EM_SECSD
  Please Input EXT_CSD Index(H'00 - H'1FF) :b1
  EXT_CSD[B1] = 0x00
  Please Input Value(H'00 - H'FF) :
```

Enter the settings of EXT_CSD register in hexadecimal.

```text
>EM_SECSD
  Please Input EXT_CSD Index(H'00 - H'1FF) :b1
  EXT_CSD[B1] = 0x00
  Please Input Value(H'00 - H'FF) :a
  EXT_CSD[B1] = 0x0A
```

The EXT_CSD register has been modified.

#### 3.4.8. Write to the S-record format images to the eMMC

This command writes the S-record format image to any partition of the eMMC.

##### Example of writing data for the eMMC boot

| Filename                       | Program Top Address | eMMC Save Partition | eMMC Save Sectors | Description            |
|--------------------------------|---------------------|---------------------|-------------------|------------------------|
| bootparam_sa0.srec             | H'E6320000          | boot partition1     | H'000000          | Loader(Boot parameter) |
| bl2-`<board_name>`.srec        | H'E6304000          | boot partition1     | H'00001E          | Loader                 |
| cert_header_sa6.srec           | H'E6320000          | boot partition1     | H'000180          | Loader(Certification)  |
| bl31-`<board_name>`.srec       | H'44000000          | boot partition1     | H'000200          | ARM Trusted Firmware   |
| u-boot-elf-`<board_name>`.srec | H'50000000          | boot partition2     | H'000000          | U-boot                 |

The following shows the procedure of this command.

```text
>EM_W
EM_W Start --------------
---------------------------------------------------------
Please select,eMMC Partition Area.
 0:User Partition Area   : 30535680 KBytes
  eMMC Sector Cnt : H'0 - H'03A3DFFF
 1:Boot Partition 1      : 16384 KBytes
  eMMC Sector Cnt : H'0 - H'00007FFF
 2:Boot Partition 2      : 16384 KBytes
  eMMC Sector Cnt : H'0 - H'00007FFF
---------------------------------------------------------
  Select area(0-2)>
```

Please enter the partition number.

```text
>EM_W
EM_W Start --------------
---------------------------------------------------------
Please select,eMMC Partition Area.
 0:User Partition Area   : 30535680 KBytes
  eMMC Sector Cnt : H'0 - H'03A3DFFF
 1:Boot Partition 1      : 16384 KBytes
  eMMC Sector Cnt : H'0 - H'00007FFF
 2:Boot Partition 2      : 16384 KBytes
  eMMC Sector Cnt : H'0 - H'00007FFF
---------------------------------------------------------
  Select area(0-2)>1
-- Boot Partition 1 Program -----------------------------
Please Input Start Address in sector :
```

Please enter the start sector number of the write image in hexadecimal. Sector size is 512 bytes.

```text
>EM_W
EM_W Start --------------
---------------------------------------------------------
Please select,eMMC Partition Area.
 0:User Partition Area   : 30535680 KBytes
  eMMC Sector Cnt : H'0 - H'03A3DFFF
 1:Boot Partition 1      : 16384 KBytes
  eMMC Sector Cnt : H'0 - H'00007FFF
 2:Boot Partition 2      : 16384 KBytes
  eMMC Sector Cnt : H'0 - H'00007FFF
---------------------------------------------------------
  Select area(0-2)>1
-- Boot Partition 1 Program -----------------------------
Please Input Start Address in sector :0000
Please Input Program Start Address :
```

Please enter the program top address of the write image in hexadecimal.

```text
>EM_W
EM_W Start --------------
---------------------------------------------------------
Please select,eMMC Partition Area.
 0:User Partition Area   : 30535680 KBytes
  eMMC Sector Cnt : H'0 - H'03A3DFFF
 1:Boot Partition 1      : 16384 KBytes
  eMMC Sector Cnt : H'0 - H'00007FFF
 2:Boot Partition 2      : 16384 KBytes
  eMMC Sector Cnt : H'0 - H'00007FFF
---------------------------------------------------------
  Select area(0-2)>1
-- Boot Partition 1 Program -----------------------------
Please Input Start Address in sector :0000
Please Input Program Start Address : E6320000
Work RAM(H'50000000-H'50FFFFFF) Clear....
please send ! ('.' & CR stop load)
```

Please download the write image in S-record format.

```text
>EM_W
EM_W Start --------------
---------------------------------------------------------
Please select,eMMC Partition Area.
 0:User Partition Area   : 30535680 KBytes
  eMMC Sector Cnt : H'0 - H'03A3DFFF
 1:Boot Partition 1      : 16384 KBytes
  eMMC Sector Cnt : H'0 - H'00007FFF
 2:Boot Partition 2      : 16384 KBytes
  eMMC Sector Cnt : H'0 - H'00007FFF
---------------------------------------------------------
  Select area(0-2)>1
-- Boot Partition 1 Program -----------------------------
Please Input Start Address in sector :0000
Please Input Program Start Address : E6320000
Work RAM(H'50000000-H'50FFFFFF) Clear....
please send ! ('.' & CR stop load)
SAVE -FLASH.......
EM_W Complete!
```

Image writing has been completed.

#### 3.4.10. Erase the eMMC

This command erases any partition of the eMMC.

The following shows the procedure of this command.

```text
>EM_E
Start --------------
---------------------------------------------------------
Please select,eMMC Partition Area.
 0:User Partition Area   : 30212096 KBytes
  eMMC Sector Cnt : H'0 - H'0399FFFF
 1:Boot Partition 1      : 4096 KBytes
  eMMC Sector Cnt : H'0 - H'00001FFF
 2:Boot Partition 2      : 4096 KBytes
  eMMC Sector Cnt : H'0 - H'00001FFF
---------------------------------------------------------
```

Please enter the partition number.

```text
>EM_E
EM_E Start --------------
---------------------------------------------------------
Please select,eMMC Partition Area.
 0:User Partition Area   : 30539776 KBytes
  eMMC Sector Cnt : H'0 - H'03A3FFFF
 1:Boot Partition 1      : 8192 KBytes
  eMMC Sector Cnt : H'0 - H'00003FFF
 2:Boot Partition 2      : 8192 KBytes
  eMMC Sector Cnt : H'0 - H'00003FFF
---------------------------------------------------------
  Select area(0-2)>0
-- User Partition Area Program --------------------------
EM_E Complete!
```

Selected partition has been erased.

#### 3.4.11. Change the SCIF baud rate setting

This command will change the baud rate of the SCIF.

Baud rate depends on the MPU and the SCIF clock setting on the board.

##### Baud rate settings after command execution

| Board         | Baud rate at startup | Baud rate at After command execution |
|---------------|---------------------:|-------------------------------------:|
| HIHOPE, EK874 | 115200bps            |                            921600bps |

*Note) The baud rate that has been changed in this command cannot be undone until the power is turned off.*

The following shows the procedure of this command.

```text
>SUP
Scif speed UP
Please change to 921.6Kbps baud rate setting of the terminal.
```

#### 3.4.12. Display the command help

Displays a description of the commands.

The following shows the procedure of this command.

```text
>H
        HyperFlash/SPI Flash write command
 XCS            erase program to HyperFlash/SPI Flash
 XLS2           write program to HyperFlash/SPI Flash
 XLS3           write program to HyperFlash/SPI Flash(Binary)

        eMMC write command
 EM_DCID        display register CID
 EM_DCSD        display register CSD
 EM_DECSD       display register EXT_CSD
 EM_SECSD       change register EXT_CSD byte
 EM_W           write program to eMMC
 EM_WB          write program to eMMC (Binary)
 EM_E           erase program to eMMC
 SUP            Scif speed UP (Change to speed up baud rate setting)
 H              help
>
```

## 4. How to build the RZ/G2 flash writer

This chapter is described how to build the RZ/G2 flash writer.
Command is executed in the user's home directory (~ /).

### 4.1. Prepare the compiler

Gets cross compiler for linaro or setup the Yocto SDK.

Linaro toolchain:

```shell
cd ~/
wget https://releases.linaro.org/components/toolchain/binaries/7.3-2018.05/aarch64-elf/gcc-linaro-7.3.1-2018.05-x86_64_aarch64-elf.tar.xz
tar xvf gcc-linaro-7.3.1-2018.05-x86_64_aarch64-elf.tar.xz
```

Yocto SDK:

```shell
source /opt/poky/2.4.3/environment-setup-aarch64-poky-linux
```

### 4.2. Prepare the source code

Get the source code of RZ/G2 flash writer.

```shell
cd ~/
git clone https://github.com/renesas-rz/rzg2_flash_writer.git
cd rzg2_flash_writer
git checkout -b v1.00 v1.00
```

### 4.3. Build the RZ/G2 flash writer

S-record file will be built by the following command.

Linaro toolchain:

```shell
make clean
CROSS_COMPILE=~/gcc-linaro-7.3.1-2018.05-x86_64_aarch64-elf/bin/aarch64-elf- make BOARD=HIHOPE
```

Yocto SDK:

```shell
make clean
make BOARD=HIHOPE
```

Output image will be available in the following directory.

* ./AArch64_output/AArch64_Flash_writer_SCIF_DUMMY_CERT_E6300400_hihope.mot

The target file name changes depending on the build options.

The following table lists the relationship between build option and target files.

#### Description of build options and target files

| Build options | Target filename                                          |
|---------------|----------------------------------------------------------|
| HIHOPE        | AArch64_Flash_writer_SCIF_DUMMY_CERT_E6300400_hihope.mot |
| EK874         | AArch64_Flash_writer_SCIF_DUMMY_CERT_E6300400_ek874.mot  |

## 5. How to run the RZ/G2 flash writer

### 5.1. Prepare for write to the Serial Flash and eMMC

Start the target in the SCIF download mode and run the RZ/G2 flash writer sample code.

Regarding the DIP switch configuration on the board, refer to [Related Document](#related-document) No.4.

The following table shows the setting of terminal software.

#### Terminal software configuration

| Baud rate  | Data bit length | Parity check | Stop bits | Flow control |
|-----------:|-----------------|--------------|-----------|--------------|
| 115200bps  | 8bits           | none         | 1bit      | none         |

Terminal software outputs the following log at power ON the target.

```text
SCIF Download mode (w/o verification)
(C) Renesas Electronics Corp.

-- Load Program to SystemRAM ---------------
please send !
```

Transfer S-record file after the log output.

S-record file:

- AArch64_output/AArch64_Flash_writer_SCIF_DUMMY_CERT_E6300400_`<board_name>`.mot

After the transfer has succeeded, the following log will be shown.

```text
Flash writer for RZ/G2M V1.00 Sep.24,2018
>
```

Please enter the any key from the console to continue.

For details on how to write to the Serial Flash and eMMC, please refer to [Section 3.4](#34-command-specification).

### 5.2. Prepare for boot from the Serial Flash and eMMC

To boot from the eMMC, need to change the DIP switch setting.

Regarding the DIP switch configuration on the board, refer to [Related Document](#related-document) No.4.

## 6. Error case to handle

### 6.1. EXT_CSD incorrect setting case

If error of eMMC command is occurred, please check the following description and restart.

- Please Check the correct setting of EXT_CSD. If the wrong setting is present, to set the correct setting using EM_SECSD command.
- Program start address error of S-record file.

The following shows the setting of High speed SDR(50MHz) x8 bus width mode, Boot partition 1 enable.

| Address      | Register Name       | Filed name             | Bit filed | Settings |
|--------------|---------------------|------------------------|-----------|----------|
| EXT_CSD[179] | PARTITION_CONFIG    | BOOT_ACK               | [6]       | 0x0      |
|              |                     | BOOT_PARTITION_ENABLE  | [5:3]     | 0x1      |
| EXT_CSD[177] | BOOT_BUS_CONDITIONS | BOOT_MODE              | [4:3]     | 0x1      |
|              |                     | BOOT_BUS_WIDTH         | [1:0]     | 0x2      |

For details of EXT_CSD, please refer to [Related Document](#related-document) No.1.

### 6.2. Program start address error of S-record format file

After the message "Please Input User Program Start Address" has been displayed, input a start address of the S-record format file to be loaded (smallest value) as the start address of the program. (This address is treated as the start address and branch address of the data transfer destination from the eMMC device in the program.)

Please check the program start address and write again program using EM_W command.

## 7. Revision history

Describe the revision history of RZ/G2 flash writer.

### 7.1. v1.00

- First release.
