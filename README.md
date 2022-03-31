# README of the RZ/Five Flash Writer

<Div Align="right">
Renesas Electronics Corporation

Mar-31-2022
</Div>

The RZ/Five flash writer is sample software for Renesas RZ/Five Series MPUs.
The RZ/Five flash writer downloads the some of the raw images from Host PC via SCIF and writes the raw images to the Serial NOR Flash.

## 1. Overview

This document explains about RZ/Five flash writer sample software for Renesas RZ/Five Series MPUs.

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
| RZ/Five EVK     | Support                     | Support                    | Support             | Support            |

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
- Display the CID/CSD/EXT_CSD registers of an eMMC.
- Modify the EXT_CSD registers of an eMMC.
- Write binary images to the boot partition of an eMMC.
- Write binary images to the user data area of an eMMC.
- Erase the boot partition of an eMMC.
- Erase the user data area of an eMMC.
- Change the SCIF baud rate setting.
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

#### 3.2.3 eMMC

Select from the following table according to the eMMC writing function.

If this option is not selected, the default value is ENABLE.

##### Association table for the eMMC value and valid eMMC writing function settings

| EMMC    | eMMC writing setting                                        |
|---------|-------------------------------------------------------------|
| ENABLE  | eMMC writing function is available. (default)               |
| DISABLE | eMMC writing function is not available.                     |

### 3.3. Command specification

The following table shows the command list.

#### Command list

| Command  | Description                                                                                        |
| -------- | -------------------------------------------------------------------------------------------------- |
| XLS2     | Write the S-record format images to the Serial Flash(SREC format)                                  |
| XLS3     | Write the S-record format images to the Serial Flash(BIN format)                                   |
| XCS      | Erase the Serial Flash.                                                                            |
| SUP      | Change the SCIF baud rate setting.                                                                 |
| EM_DCID  | Display the CID registers of eMMC.                                                                 |
| EM_DCSD  | Display the CSD registers of eMMC.                                                                 |
| EM_DECSD | Display the EXT_CSD registers of eMMC.                                                             |
| EM_SECSD | Modify the EXT_CSD registers of eMMC.                                                              |
| EM_W     | Write to the S-record format images to the user data area of eMMC, and the boot partition of eMMC. |
| EM_WB    | Write to the raw binary images to the user data area of eMMC, and the boot partition of eMMC.      |
| EM_E     | Erase the user data area of eMMC, and the boot partition of eMMC.                                  |
| SUP      | Change the SCIF baud rate setting.                                                                 |
| SUD      | Change the SCIF baud rate setting.                                                                 |
| RESET    | Perform RESET of the CPU.                                                                          |
| H        | Display the command help.                                                                          |

#### 3.3.1. Write to the S-record format images to the Serial Flash

This command writes the S-record format image to Serial Flash.

##### Example of writing data for the Serial Flash boot

| Filename                   | Program Top Address | Flash Save Address | Description                                 |
| -------------------------- | ------------------- | ------------------ | ------------------------------------------- |
| spl-`<board_name>`.srec    | H'11E00             | H'00000            | Secondary Program Loader file               |
| fit-`<board_name>`.srec    | H'00000             | H'20000            | FIT file（U-BOOT、OpenSBI、DTB)             |

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
  Please Input : H'11E00

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
  Please Input : H'11E00

===== Please Input Qspi Save Address ===
  Please Input : H'00000
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
  Please Input : H'11E00

===== Please Input Qspi Save Address ===
  Please Input : H'00000
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
  Please Input : H'11E00

===== Please Input Qspi Save Address ===
  Please Input : H'00000
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

#### 3.3.3. Display the CID registers command

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

#### 3.3.4. Display the CSD registers command

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

#### 3.3.5. Display the EXT_CSD registers command

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

#### 3.3.6. Modify the EXT_CSD registers of eMMC command

This command modifies the contents of the EXT_CSD registers in the eMMC.

The values must be entered as **hexadecimal**.

Example:

```text
>EM_SECSD
  Please Input EXT_CSD Index(H'00 - H'1FF) :b1
  EXT_CSD[B1] = 0x00
  Please Input Value(H'00 - H'FF) :2
  EXT_CSD[B1] = 0x02
```

#### eMMC Boot Settings

Please note that for eMMC booting, the following EXT_CSD registers need to be modified:
 - EXT_CSD[**B1**] = **0x02**
 - EXT_CSD[**B3**] = **0x08**

#### 3.3.7. Write to the S-record format images to the eMMC

This command writes the S-record format image to any partition of the eMMC.

##### Example of writing data for the eMMC boot

| Filename                | eMMC Save Partition | eMMC Save Sectors | Program Top Address | Description                                 |
|-------------------------|---------------------|-------------------|---------------------|---------------------------------------------|
| spl-`<board_name>`.srec | boot partition1     | H'00000001        | H'00011E00          | Secondary Program Loader file               |
| fit-`<board_name>`.srec | boot partition1     | H'00000100        | H'00000000          | FIT file（U-BOOT、OpenSBI、DTB)             |

The following shows the procedure of this command.
The values must be entered as **hexadecimal**.
Please enter the start sector number of the write image in hexadecimal. Sector size is 512 bytes.
Please enter the program top address of the write image in hexadecimal.
Please download the write image in S-record format.

```text
>EM_W
EM_W Start --------------
---------------------------------------------------------
Please select,eMMC Partition Area.
 0:User Partition Area   : 62160896 KBytes
  eMMC Sector Cnt : H'0 - H'0768FFFF
 1:Boot Partition 1      : 32256 KBytes
  eMMC Sector Cnt : H'0 - H'0000FBFF
 2:Boot Partition 2      : 32256 KBytes
  eMMC Sector Cnt : H'0 - H'0000FBFF
---------------------------------------------------------
  Select area(0-2)>1                                       <<<< Enter "1" here
-- Boot Partition 1 Program -----------------------------
Please Input Start Address in sector :1                    <<<< Enter "1" here
Please Input Program Start Address : 11E00                 <<<< Enter "11E00" here
Work RAM(H'50000000-H'50FFFFFF) Clear....
please send ! ('.' & CR stop load)
SAVE -FLASH.......
EM_W Complete!
```

Image writing has been completed.

#### 3.3.8. Erase the eMMC

This command erases any partition of the eMMC.

The following shows the procedure of this command.

```text
>EM_E
EM_E Start --------------
---------------------------------------------------------
Please select,eMMC Partition Area.
 0:User Partition Area   : 62160896 KBytes
  eMMC Sector Cnt : H'0 - H'0768FFFF
 1:Boot Partition 1      : 32256 KBytes
  eMMC Sector Cnt : H'0 - H'0000FBFF
 2:Boot Partition 2      : 32256 KBytes
  eMMC Sector Cnt : H'0 - H'0000FBFF
---------------------------------------------------------
  Select area(0-2)>0                                       <<<< Enter "0" here
-- User Partition Area Program --------------------------
EM_E Complete!
```

Selected partition has been erased.

#### 3.3.9. Change the SCIF baud rate setting

This command will change the baud rate of the SCIF.

Baud rate depends on the MPU and the SCIF clock setting on the board.

##### Baud rate settings after command execution

| Command | Baud rate at After command execution |
|---------|-------------------------------------:|
| SUP     | 921600bps                            |
| SUD     | 115200bps                            |

*Note) The baud rate that has been changed in this command cannot be undone until the power is turned off.*

The following shows the procedure of this command.

```text
>SUP
Scif speed UP
Please change to 921.6Kbps baud rate setting of the terminal.
```

```text
>SUD
Scif speed DOWN
Please change to 115.2Kbps baud rate setting of the terminal.
```

#### 3.3.10. Display the command help

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
Flash writer for RZ/Five Series V1.01 Mar.18,2022
>
```

Please enter the any key from the console to continue.

For details on how to write to the Serial Flash and eMMC, please refer to [Section 3.3](#33-command-specification).

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

### 7.2. v1.01

- Enable eMMC writing function.
