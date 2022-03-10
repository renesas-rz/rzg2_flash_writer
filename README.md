# README of the RZ/Five Flash Writer

<Div Align="right">
Renesas Electronics Corporation

Mar-10-2022
</Div>

The RZ/Five flash writer is sample software for Renesas RZ/Five Group MPUs.
The RZ/Five flash writer downloads the some of the raw images from Host PC via SCIF and writes the raw images to the Serial NOR Flash.

## 1. Overview

This document explains about RZ/Five flash writer sample software for Renesas RZ/Five Group MPUs.

The RZ/Five flash writer is downloaded from the Host PC via SCIF by boot ROM.

And the RZ/Five flash writer downloads the some of the raw images from Host PC via SCIF, and writes the raw images to the Serial NOR Flash (hereafter referred to as &ldquo;Serial Flash&rdquo;).

The RZ/Five flash writer's Serial Flash writing support is on-board Serial NOR Flash(i.e. AT25QL128A).

[Chapter 2](#2-operating-environment) describes the operating environment.

[Chapter 3](#3-software) describes the software.

[Chapter 4](#4-how-to-build-the-rzfive-flash-writer) explains example of how to build the RZ/Five flash writer.

[Chapter 5](#5-how-to-run-the-rzfive-flash-writer) explains example of how to perform the RZ/Five flash writer.

[Chapter 6](#6-error-case-to-handle) explains how to handle error case.

[Chapter 7](#7-revision-history) explains revision history.

*Note) This sample software does not support the file system. Therefore, can be write the raw image to the Serial NOR Flash.*

## 1.2. License

BSD-3-Clause (please see file [LICENSE.md](LICENSE.md) for the details)

## 1.3. Notice

The RZ/Five flash writer is distributed as a sample software from Renesas without any warranty or support.

## 1.4. Contributing

To contribute to this layer, you should email patches to renesas-rz@renesas.com. Please send .patch files as email attachments, not embedded in the email body.

## 1.5. References

The following table shows the document related to this function.

### Related Document

| Number | Issuer  | Title                                                       | Edition           |
| ------ | ------- | ----------------------------------------------------------- | ----------------- |
| 1      | JEDEC   | Embedded Multi-Media Card (eMMC) Electrical Standard (5.01) | JESD84-B50.1      |
| 2      | Renesas | RZ/Five Reference Boards Start-up Guide                     | Rev.1.00 or later |

## 2. Operating Environment

### 2.1. Hardware Environment

The following table lists the hardware needed to use this function.

#### Hardware environment

| Name         | Note                                              |
| ------------ | ------------------------------------------------- |
| Target board | RZ/Five SMARC Evaluation Kit(RZ/Five EVK)         |
| Host PC      | Ubuntu Desktop 20.04(64bit) or later              |

The following table shows Serial Flash and eMMC support for each MPU.

##### Serial Flash / eMMC support status of each MPU

| MPU             | Read/Write the Serial Flash | Boot from the Serial Flash | Read/Write the eMMC | Boot from the eMMC |
| --------------- | --------------------------- | -------------------------- | ------------------- | ------------------ |
| RZ/Five EVK     | Support                     | Support                    |                     |                    |

## 2.2. Software Environment

The following table lists the software required to use this sample software.

### Software environment

| Name                             | Note                                                    |
| -------------------------------- | ------------------------------------------------------- |
| RISCV64 Cross-compile Toolchain  | RISCV64 Cross-compile Toolchain Release GCC v11.1.0     |

## 3. Software

### 3.1. Function

This package has the following functions.

- Write to the images to the Serial Flash.
- Erase the Serial Flash.
- Display the command help.

### 3.2. Option setting

The RZ/Five flash writer support the following build options.

#### 3.2.1. BOARD

Select from the following table according to the board settings.

If this option is not selected, the default value is RZFIVE_SMARC.

| BOARD            | BOARD setting                                                 |
| ---------------- | ------------------------------------------------------------- |
| RZFIVE_SMARC     | Generate binary that works on RZ/Five EVK board (default)     |

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
| SUP     | Change the SCIF baud rate setting.                                |
| SUD     | Change the SCIF baud rate setting.                                |
| RESET   | Perform RESET of the CPU.                                         |
| H       | Display the command help.                                         |

#### 3.3.1. Write to the S-record format images to the Serial Flash

This command writes the S-record format image to Serial Flash.

##### Example of writing data for the Serial Flash boot

| Filename                   | Program Top Address | Flash Save Address | Description                                 |
| -------------------------- | ------------------- | ------------------ | ------------------------------------------- |
| spl-`<board_name>`.srec    | H'0000              | H'0000             | Secondary Program Loader file               |
| fit-`<board_name>`.srec    | H'0000              | H'20000            | FIT file（U-BOOT、OpenSBI、DTB)             |

The following shows the procedure of this command.

*Note) The following procedure is an example on RZ/Five EVK board.*

```text
>XLS2
===== Qspi writing of RZ/Five Board Command =============
Load Program to Spiflash
Writes to any of SPI address.
 Dialog : AT25QL128A
Program Top Address & Qspi Save Address
===== Please Input Program Top Address ============
  Please Input : H'
```

Please enter the program top address of the write image in hexadecimal.

```text
>XLS2
===== Qspi writing of RZ/Five Board Command =============
Load Program to Spiflash
Writes to any of SPI address.
 Dialog : AT25QL128A
Program Top Address & Qspi Save Address
===== Please Input Program Top Address ============
  Please Input : H'0000

===== Please Input Qspi Save Address ===
  Please Input : H'
```

Please enter the flash save address in hexadecimal.

```text
>XLS2
===== Qspi writing of RZ/Five Board Command =============
Load Program to Spiflash
Writes to any of SPI address.
 Dialog : AT25QL128A
Program Top Address & Qspi Save Address
===== Please Input Program Top Address ============
  Please Input : H'0000

===== Please Input Qspi Save Address ===
  Please Input : H'0000
Work RAM(H'50000000-H'53FFFFFF) Clear....
please send ! ('.' & CR stop load)
```

Please download the write image in S-record format.

```text
>XLS2
===== Qspi writing of RZ/Five Board Command =============
Load Program to Spiflash
Writes to any of SPI address.
 Dialog : AT25QL128A
Program Top Address & Qspi Save Address
===== Please Input Program Top Address ============
  Please Input : H'0000

===== Please Input Qspi Save Address ===
  Please Input : H'0000
Work RAM(H'50000000-H'53FFFFFF) Clear....
please send ! ('.' & CR stop load)
SPI Data Clear(H'FF) Check :H'00000000-0000FFFF,Clear OK?(y/n)
```

Please enter the 'y' key when asked to clear.

If Flash is erased, it will not be asked.

```text
>XLS2
===== Qspi writing of RZ/Five Board Command =============
Load Program to Spiflash
Writes to any of SPI address.
 Dialog : AT25QL128A
Program Top Address & Qspi Save Address
===== Please Input Program Top Address ============
  Please Input : H'0000

===== Please Input Qspi Save Address ===
  Please Input : H'0000
Work RAM(H'50000000-H'53FFFFFF) Clear....
please send ! ('.' & CR stop load)
SPI Data Clear(H'FF) Check :H'00000000-0000FFFF Erasing..Erase Completed
SAVE SPI-FLASH.......
======= Qspi  Save Information  =================
 SpiFlashMemory Stat Address : H'00000000
 SpiFlashMemory End Address  : H'0000D15A
===========================================================

>
```

Image writing has been completed.

#### 3.3.2. Erase the Serial NOR Flash

This command erases all sectors of Serial Flash.

The following shows the procedure of this command.

*Note) The following procedure is an example on RZ/Five EVK board.*

```text
>XCS
ALL ERASE SpiFlash memory
Clear OK?(y/n)
```

Please enter the 'y' key.

```text
>XCS
ALL ERASE SpiFlash memory
 Dialog : AT25QL128A
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

        eMMC write command
 EM_DCID        display register CID
 EM_DCSD        display register CSD
 EM_DECSD       display register EXT_CSD
 EM_SECSD       change register EXT_CSD byte
 EM_W           write program to eMMC
 EM_WB          write program to eMMC (Binary)
 EM_E           erase program to eMMC

 RESET          Perform RESET of the CPU
 SUP            Scif speed UP (Change to speed up baud rate setting)
 SUD            Scif speed DOWN (Change to speed down baud rate setting)
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

## 4. How to build the RZ/Five flash writer

This chapter is described how to build the RZ/Five flash writer.
Command is executed in the user's home directory (~ /).

### 4.1. Prepare the compiler

Compile and install riscv-toolchain for RISC-V 64bit.
```shell
sudo apt-get install -y autoconf automake autotools-dev curl libmpc-dev libmpfr-dev libgmp-dev gawk build-essential bison flex texinfo gperf libtool patchutils bc zlib1g-dev libexpat-dev libncurses-dev device-tree-compiler libssl-dev gdisk swig

git clone https://github.com/riscv/riscv-gnu-toolchain
cd riscv-gnu-toolchain
./configure --prefix=/opt/riscv
sudo make linux -j
```

### 4.2. Prepare the source code

Get the source code of RZ/Five flash writer.

```shell
cd ~/
git clone https://github.com/renesas-rz/rzg2_flash_writer.git
cd rzg2_flash_writer
git checkout rz_five
```

### 4.3. Build the RZ/Five flash writer

S-record file will be built by the following command.

```shell
export PATH=$PATH:/opt/riscv/bin

make clean
make BOARD=RZFIVE_SMARC
```

Output image will be available in the following directory.

* ./riscv_output/Flash_Writer_SCIF_RZFIVE_SMARC.mot 

The target file name changes depending on the build options.

The following table lists the relationship between build option and target files.

#### Description of build options and target files

| Build options     | Target filename                                      |
| ----------------- | -----------------------------------------------------|
| RZ/Five SMARC     | Flash_Writer_SCIF_RZFIVE_SMARC.mot                   |

## 5. How to run the RZ/Five flash writer

### 5.1. Prepare for write to the Serial Flash and eMMC

Start the target in the SCIF download mode and run the RZ/Five flash writer sample code.

Regarding the DIP switch configuration on the board, refer to [Related Document](#related-document) No.2.

The following table shows the setting of terminal software.

#### Terminal software configuration

| Baud rate | Data bit length | Parity check | Stop bits | Flow control |
| --------: | --------------- | ------------ | --------- | ------------ |
| 115200bps | 8bits           | none         | 1bit      | none         |

Terminal software outputs the following log at power ON the target.

```text
 SCIF Download mode
 (C) Renesas Electronics Corp.
-- Load Program to System RAM ---------------
please send !
```

Transfer S-record file after the log output.

S-record file:

- riscv_output/Flash_Writer_SCIF_`<board_name>`.mot

After the transfer has succeeded, the following log will be shown.

```text
Flash writer for RZ/Five Series V1.00 Mar.10,2022
>
```

Please enter the any key from the console to continue.

For details on how to write to the Serial Flash and eMMC, please refer to [Section 3.4](#34-command-specification).

### 5.2. Prepare for boot from the Serial Flash and eMMC

To boot from the eMMC, need to change the DIP switch setting.

Regarding the DIP switch configuration on the board, refer to [Related Document](#related-document) No.2.

## 6. Error case to handle

TBD 

## 7. Revision history

Describe the revision history of RZ/Five flash writer.

### 7.1. v1.00

- First release.
- Support RZ/Five EVK board.
