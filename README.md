# README of the RZ/G2L Flash Writer

<Div Align="right">
Renesas Electronics Corporation

Mar-07-2022
</Div>

The RZ/G2L flash writer is sample software for Renesas RZ/G2L Group MPUs.
The RZ/G2L flash writer downloads the some of the raw images from Host PC via SCIF and writes the raw images to the Serial NOR Flash.

## 1. Overview

This document explains about RZ/G2L flash writer sample software for Renesas RZ/G2L Group MPUs.

The RZ/G2L flash writer is downloaded from the Host PC via SCIF by boot ROM.

And the RZ/G2L flash writer downloads the some of the raw images from Host PC via SCIF, and writes the raw images to the Serial NOR Flash (hereafter referred to as &ldquo;Serial Flash&rdquo;).

The RZ/G2L flash writer's Serial Flash writing support is on-board Serial NOR Flash(i.e. MT25QU512).

[Chapter 2](#2-operating-environment) describes the operating environment.

[Chapter 3](#3-software) describes the software.

[Chapter 4](#4-how-to-build-the-rzg2-flash-writer) explains example of how to build the RZ/G2L flash writer.

[Chapter 5](#5-how-to-run-the-rzg2-flash-writer) explains example of how to perform the RZ/G2L flash writer.

[Chapter 6](#6-error-case-to-handle) explains how to handle error case.

[Chapter 7](#7-revision-history) explains revision history.

*Note) This sample software does not support the file system. Therefore, can be write the raw image to the Serial NOR Flash.*

## 1.2. License

BSD-3-Clause (please see file [LICENSE.md](LICENSE.md) for the details)

## 1.3. Notice

The RZ/G2L flash writer is distributed as a sample software from Renesas without any warranty or support.

## 1.4. Contributing

To contribute to this layer, you should email patches to renesas-rz@renesas.com. Please send .patch files as email attachments, not embedded in the email body.

## 1.5. References

The following table shows the document related to this function.

### Related Document

| Number | Issuer  | Title                                                       | Edition           |
| ------ | ------- | ----------------------------------------------------------- | ----------------- |
| 1      | JEDEC   | Embedded Multi-Media Card (eMMC) Electrical Standard (5.01) | JESD84-B50.1      |
| 2      | Renesas | Release Note for Verified Linux Package for 64bit kernel    | Rev.1.01 or later |
| 3      | Renesas | RZ/G2L Yocto recipe Start-Up Guide                          | Rev.1.01 or later |
| 4      | Renesas | RZ/G2L Reference Boards Start-up Guide                      | Rev.1.01 or later |

## 2. Operating Environment

### 2.1. Hardware Environment

The following table lists the hardware needed to use this function.

#### Hardware environment

| Name         | Note                                              |
| ------------ | ------------------------------------------------- |
| Target board | RZ/G2L SMARC Concrete Evaluation Kit(RZ/G2L EVK)  |
|              | RZ/G2L SMARC PMIC Evaluation Kit(RZ/G2L PMIC EVK) |
|              | RZ/V2L SMARC Concrete Evaluation Kit(RZ/V2L EVK)  |
|              | RZ/V2L SMARC PMIC Evaluation Kit(RZ/V2L PMIC EVK) |
| Host PC      | Ubuntu Desktop 20.04(64bit) or later              |

The following table shows Serial Flash and eMMC support for each MPU.

##### Serial Flash / eMMC support status of each MPU

| MPU             | Read/Write the Serial Flash | Boot from the Serial Flash | Read/Write the eMMC | Boot from the eMMC | MMC interface |
| --------------- | --------------------------- | -------------------------- | ------------------- | ------------------ | ------------- |
| RZ/G2L EVK      | Support                     | Support                    |                     |                    |               |
| RZ/G2L PMIC EVK | Support                     | Support                    |                     |                    |               |
| RZ/V2L EVK      | Support                     | Support                    |                     |                    |               |
| RZ/V2L PMIC EVK | Support                     | Support                    |                     |                    |               |

## 2.2. Software Environment

The following table lists the software required to use this sample software.

### Software environment

| Name                             | Note                                                    |
| -------------------------------- | ------------------------------------------------------- |
| ARM64 Cross-compile Toolchain *1 | ARM64 Cross-compile Toolchain Release GCC v10.2         |
| Yocto SDK *1 *2                  | Yocto SDK built from Yocto environment for RZ/G2L Group |

\*1: One of the above environments is required.

\*2: Regarding how to get the Yocto SDK, refer to [Related Document](#related-document) No.2 or No.3.

## 3. Software

### 3.1. Function

This package has the following functions.

- Write to the images to the Serial Flash.
- Erase the Serial Flash.
- Display the command help.

### 3.2. Option setting

The RZ/G2L flash writer support the following build options.

#### 3.2.1. BOARD

Select from the following table according to the board settings.

If this option is not selected, the default value is RZG2L_SMARC.

| BOARD            | BOARD setting                                                 |
| ---------------- | ------------------------------------------------------------- |
| RZG2L_SMARC      | Generate binary that works on RZ/G2L EVK board (default)      |
| RZG2L_SMARC_PMIC | Generate binary that works on RZ/G2L PMIC EVK board           |
| RZV2L_SMARC      | Generate binary that works on RZ/V2L EVK board                |
| RZV2L_SMARC_PMIC | Generate binary that works on RZ/V2L PMIC EVK board           |

#### 3.2.2. SERIAL_FLASH

Select from the following table according to the Serial Flash writing function.

If this option is not selected, the default value is ENABLE.

##### Association table for the SERIAL_FLASH value and valid Serial Flash writing function settings

| SERIAL_FLASH | Serial Flash writing setting                          |
| ------------ | ----------------------------------------------------- |
| ENABLE       | Serial Flash writing function is available. (default) |
| DISABLE      | Serial Flash writing function is not available.       |

### 3.3. Command specification

The following table shows the command list.

#### Command list

| Command | Description                                                       |
| ------- | ----------------------------------------------------------------- |
| XLS2    | Write the S-record format images to the Serial Flash(SREC format) |
| XLS3    | Write the S-record format images to the Serial Flash(BIN format)  |
| XCS     | Erase the Serial Flash.                                           |
| RESET   | Perform RESET of the CPU.                                         |
| H       | Display the command help.                                         |

#### 3.3.1. Write to the S-record format images to the Serial Flash

This command writes the S-record format image to Serial Flash.

##### Example of writing data for the Serial Flash boot

| Filename                   | Program Top Address | Flash Save Address | Description                                 |
| -------------------------- | ------------------- | ------------------ | ------------------------------------------- |
| bl2_bp-`<board_name>`.srec | H'11E0              | H'0000             | Loader                                      |
| fip-`<board_name>`.srec    | H'0000              | H'1D200            | ARM Trusted Firmware and U-boot in FIP file |

The following shows the procedure of this command.

*Note) The following procedure is an example on RZ/G2L EVK board.*

```text
>XLS2
===== Qspi writing of RZ/G2 Board Command =============
Load Program to Spiflash
Writes to any of SPI address.
 Micron : MT25QU512
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
 Micron : MT25QU512
Program Top Address & Qspi Save Address
===== Please Input Program Top Address ============
  Please Input : H'11E0

===== Please Input Qspi Save Address ===
  Please Input : H'
```

Please enter the flash save address in hexadecimal.

```text
===== Qspi writing of RZ/G2 Board Command =============
Load Program to Spiflash
Writes to any of SPI address.
 Micron : MT25QU512
Program Top Address & Qspi Save Address
===== Please Input Program Top Address ============
  Please Input : H'11E0

===== Please Input Qspi Save Address ===
  Please Input : H'0
Work RAM(H'50000000-H'53FFFFFF) Clear....
please send ! ('.' & CR stop load)
```

Please download the write image in S-record format.

```text
>XLS2
===== Qspi writing of RZ/G2 Board Command =============
Load Program to Spiflash
Writes to any of SPI address.
 Micron : MT25QU512
Program Top Address & Qspi Save Address
===== Please Input Program Top Address ============
  Please Input : H'11E0

===== Please Input Qspi Save Address ===
  Please Input : H'0
Work RAM(H'50000000-H'53FFFFFF) Clear....
please send ! ('.' & CR stop load)
ORER
SPI Data Clear(H'FF) Check :H'00000000-0001FFFF,Clear OK?(y/n)
```

Please enter the 'y' key when asked to clear.

If Flash is erased, it will not be asked.

```text
>XLS2
===== Qspi writing of RZ/G2 Board Command =============
Load Program to Spiflash
Writes to any of SPI address.
 Micron : MT25QU512
Program Top Address & Qspi Save Address
===== Please Input Program Top Address ============
  Please Input : H'11E0

===== Please Input Qspi Save Address ===
  Please Input : H'0
Work RAM(H'50000000-H'53FFFFFF) Clear....
please send ! ('.' & CR stop load)
ORER
SPI Data Clear(H'FF) Check :H'00000000-0001FFFF Erasing...Erase Completed
SAVE SPI-FLASH.......
======= Qspi  Save Information  =================
 SpiFlashMemory Stat Address : H'00000000
 SpiFlashMemory End Address  : H'00018680
===========================================================

>
```

Image writing has been completed.

#### 3.3.2. Erase the Serial NOR Flash

This command erases all sectors of Serial Flash.

The following shows the procedure of this command.

*Note) The following procedure is an example on RZ/G2L EVK board.*

```text
>XCS
ALL ERASE SpiFlash memory
Clear OK?(y/n)
```

Please enter the 'y' key.

```text
>XCS
ALL ERASE SpiFlash memory
 Micron : MT25QU512
 ERASE QSPI-FLASH (60sec[typ]).... complete!
>
```

Selected Serial Flash has been erased.

#### 3.3.3. Display the command help

Displays a description of the commands.

The following shows the procedure of this command.

```text
        SPI Flash write command
 XCS            erase program to SPI Flash
 XLS2           write program to SPI Flash
 XLS3           write program to SPI Flash(Binary)

 SUP            Scif speed UP (Change to speed up baud rate setting)
 H              help
 D  {sadr {eadr}}          memory dump  (DM sets dump size)
 DM {B|W|L|X}              set&disp dump mode
 M  [adr]                  set memory(BYTE)
 MW [adr]                  set memory(WORD)
 ML [adr]                  set memory(LONG)
 MX [adr]                  set memory(LONG LONG)
 F  [sadr] [eadr] [data]   fill memory
 FL [sadr] [eadr] [data]   fill memory(LONG)
 FX [sadr] [eadr] [data]   fill memory(LONG LONG)
>
```

## 4. How to build the RZ/G2L flash writer

This chapter is described how to build the RZ/G2L flash writer.
Command is executed in the user's home directory (~ /).

### 4.1. Prepare the compiler

Gets cross compiler for linaro or setup the Yocto SDK.

ARM64 Cross-compile toolchain:

```shell
cd ~/
wget https://developer.arm.com/-/media/Files/downloads/gnu-a/10.2-2020.11/binrel/gcc-arm-10.2-2020.11-x86_64-aarch64-none-elf.tar.xz
tar xvf gcc-arm-10.2-2020.11-x86_64-aarch64-none-elf.tar.xz
```

Yocto SDK:

```shell
source /opt/poky/3.1.5/environment-setup-aarch64-poky-linux
```

### 4.2. Prepare the source code

Get the source code of RZ/G2L flash writer.

```shell
cd ~/
git clone https://github.com/renesas-rz/rzg2_flash_writer.git
cd rzg2_flash_writer
git checkout rz_g2l
```

### 4.3. Build the RZ/G2L flash writer

S-record file will be built by the following command.

ARM64 Cross-compile toolchain:

```shell
make -f makefile.linaro clean
CROSS_COMPILE=~/gcc-arm-10.2-2020.11-x86_64-aarch64-none-elf/bin/aarch64-none-elf- make -f makefile.linaro BOARD=RZG2L_SMARC
```

Yocto SDK:

```shell
make clean
make BOARD=RZG2L_SMARC
```

Output image will be available in the following directory.

* ./AArch64_output/Flash_Writer_SCIF_RZG2L_SMARC_DDR4_2GB.mot

The target file name changes depending on the build options.

The following table lists the relationship between build option and target files.

#### Description of build options and target files

| Build options     | Target filename                                      |
| ----------------- | -----------------------------------------------------|
| RZ/G2L SMARC      | Flash_Writer_SCIF_RZG2L_SMARC_DDR4_2GB.mot           |
| RZ/G2L SMARC PMIC | Flash_Writer_SCIF_RZG2L_SMARC_PMIC_DDR4_2GB_1PCS.mot |
| RZ/V2L SMARC      | Flash_Writer_SCIF_RZV2L_SMARC_DDR4_4GB.mot           |
| RZ/V2L SMARC PMIC | Flash_Writer_SCIF_RZV2L_SMARC_PMIC_DDR4_2GB_1PCS.mot |

## 5. How to run the RZ/G2L flash writer

### 5.1. Prepare for write to the Serial Flash and eMMC

Start the target in the SCIF download mode and run the RZ/G2L flash writer sample code.

Regarding the DIP switch configuration on the board, refer to [Related Document](#related-document) No.4.

The following table shows the setting of terminal software.

#### Terminal software configuration

| Baud rate | Data bit length | Parity check | Stop bits | Flow control |
| --------: | --------------- | ------------ | --------- | ------------ |
| 115200bps | 8bits           | none         | 1bit      | none         |

Terminal software outputs the following log at power ON the target.

```text
SCIF Download mode (w/o verification)
(C) Renesas Electronics Corp.

-- Load Program to SystemRAM ---------------
please send !
```

Transfer S-record file after the log output.

S-record file:

- AArch64_output/Flash_Writer_SCIF_`<board_name>`.mot

After the transfer has succeeded, the following log will be shown.

```text
Flash writer for RZ/G2L Series V0.86 Feb.26,2021
>
```

Please enter the any key from the console to continue.

For details on how to write to the Serial Flash and eMMC, please refer to [Section 3.4](#34-command-specification).

### 5.2. Prepare for boot from the Serial Flash and eMMC

To boot from the eMMC, need to change the DIP switch setting.

Regarding the DIP switch configuration on the board, refer to [Related Document](#related-document) No.4.

## 6. Error case to handle

TBD 

## 7. Revision history

Describe the revision history of RZ/G2L flash writer.

### 7.1. v1.00

- First release.
- Support RZ/G2L Concrete/PMIC EVK board.

### 7.2. v1.01

- Support RZ/G2UL EVK board.

### 7.3. v1.02

- Enable eMMC writing function.

### 7.4. v1.03

- Support the RESET command.
