# RZ/V2M Flash Writer

<Div Align="right">
Renesas Electronics Corporation

July 30, 2021
</Div>

RZ/V2M flash writer is a sample software to write loader binary images to eMMC on RZ/V2M.  

## 1. Overview

This README explains how to use RZ/V2M flash writer.  
Refer to "RZ/V2M Linux Package Yocto recipe Start-Up Guide" for the detail.

## 1.1. README Contents

[Chapter 2](#2-operating-environment) describes the operating environment.

[Chapter 3](#3-software) describes the software.

[Chapter 4](#4-how-to-build-the-rzv2m-flash-writer) explains an example of how to build the RZ/V2M flash writer.

[Chapter 5](#5-how-to-run-the-rzv2m-flash-writer) explains an example of how to perform the RZ/V2M flash writer.

*Note) This sample software does not support file systems. Therefore, only raw binary images can be downloaded and programmed into eMMC.*

## 1.2. License

BSD-3-Clause (See file [LICENSE.md](LICENSE.md) for details.)

## 1.3. Notice

RZ/V2M flash writer is distributed by Renesas Electronics Corporation as a sample software without any warranty or support.

## 1.4. Contributing

To contribute to this software, you should email patches to renesas-rz@renesas.com. Send .patch files as email attachments, not embedded in the email body.

## 1.5. References

The following table shows the document related to this function.

### Related Document

| Number | Issuer  | Title                                                          | Edition           |
|--------|---------|----------------------------------------------------------------|-------------------|
| 1      | JEDEC   | Embedded Multi-Media Card (eMMC) Electrical Standard (5.01)    | JESD84-B50.1      |
| 2      | Renesas | RZ/V2M Linux Package Relese Note                               | Rev.1.00 or later |
| 3      | Renesas | RZ/V2M Linux Package Start-Up Guide                            | Rev.1.00 or later |
| 4      | Renesas | RZ/V2M Linux Package Yocto recipe Start-Up Guide               | Rev.1.00 or later |

## 2. Operating Environment

### 2.1. Hardware Environment

The following table lists the hardware needed to use this utility.

#### Hardware environment

| Name                   | Note                                                                                 |
|------------------------|--------------------------------------------------------------------------------------|
| Target board           | RZ/V2M Evaluation Kit.                                                               |
| Linux Host PC          | Build and debug environment. Ubuntu Desktop 16.04(64bit).                            |
| Windows PC             | Use to control serial console of the target board.                                   |
| micro SDHC card        | Use for writing flash writer. The card should have 1 partition formatted with FAT32. |
| micro USB serial cable | Connect between the target board and Host PC.                                        |

The eMMC support status of RZ/V2M is as follows.

#### eMMC support status

|  MPU   |       eMMC      | Read/Write the eMMC | Boot from the eMMC | MMC interface |
|--------|-----------------|---------------------|--------------------|---------------|
| RZ/V2M | THGBMJG7C1LBAIL |      Support        |        Support     |     MMC1      |

## 2.2. Software Environment

The following table lists the software required to use this sample software.

### Compile

| Name                | Note                                                             |
|---------------------|------------------------------------------------------------------|
| Linaro Toolchain *1 | Linaro Binary Toolchain Release GCC 7.3-2018.05 for aarch64-elf. |

### Software on Windows PC

| Name                | Note                                                             |
|---------------------|------------------------------------------------------------------|
| Terminal software   | Control serial console of the target board.                      |

## 3. Software

### 3.1. Function

This package has the following capabilities:

- Write binary images to the boot partition of an eMMC.
- Erase the boot partition of an eMMC.

### 3.2 Command specification

The following table shows the command list.

#### Command list

| Command  | Description                                                      |
|----------|------------------------------------------------------------------|
| EM_WB    | Write the raw binary images to the boot partition of eMMC.       |
| EM_E     | Erase the user data area of eMMC and the boot partition of eMMC. |

#### 3.2.1 Write the loader binary images to the eMMC

This command writes the loader binary images to any partition of the eMMC.

##### Example of writing data for the eMMC boot

| Filename             | Program Top Address | eMMC save partition | eMMC save sectors *1 | File size (byte) *2 | Description                   |
|----------------------|---------------------|---------------------|----------------------|---------------------|-------------------------------|
| loader_1st_128kb.bin | H'80100000          | boot partition1     | H'000000             | H'20000             | 1st loader binary             |
| loader_2nd_param.bin | On RAMA area *3     | boot partition1     | H'000100             | H'8                 | Boot parameter for 2nd loader |
| loader_2nd.bin       | H'B6000000          | boot partition1     | H'000101             | *2                  | 2nd loader binary             |
| u-boot_param.bin     | On RAMB area *3     | boot partition1     | H'000901             | H'8                 | Boot parameter for u-boot     |
| u-boot.bin           | H'57F00000          | boot partition1     | H'000902             | *2                  | U-Boot binary                 |

*1: The sector size is 512bytes.  
*2: These file sizes may be different from the loader binary files generated by bitbake. Also, the size of these files may change in future versions. Check the size of each file on your PC.  
*3: These RAM areas are not fixed because these binaries are stored in the local memory.  

The following shows the procedure of this command with the binary file loader_1st_128kb.bin.
Note that the values of eMMC save sector and file size must be entered as **hexadecimal**.

```text
>EM_WB

EM_W Start --------------
---------------------------------------------------------
Please select,eMMC Partition Area.
 0:User Partition Area   : 15388672 KBytes
  eMMC Sector Cnt : H'0 - H'01D59FFF
 1:Boot Partition 1      : 4096 KBytes
  eMMC Sector Cnt : H'0 - H'00001FFF
 2:Boot Partition 2      : 4096 KBytes
  eMMC Sector Cnt : H'0 - H'00001FFF
---------------------------------------------------------
  Select area(0-2)>1                                          <<<< Enter the boot partition to write.

-- Boot Partition 1 Program -----------------------------
Please Input Start Address in sector :000                     <<<< Enter the start address in the sector in hexadecimal.

Work RAM(H'B6000000-H'B60FFFFF) Clear....
Please Input File size(byte) : 20000                          <<<< Enter the file size in hexiadecimal.

please send binary file!                                      <<<< Send the file via the terminal software.
SAVE -FLASH.......
EM_WB Complete!
>EM_WB
```

Image writing has been completed.

#### 3.2.2 Erase the eMMC

This command erases any partition of the eMMC.

The following shows the procedure of this command.  

```text
>EM_E

EM_E Start --------------
---------------------------------------------------------
Please select,eMMC Partition Area.
 0:User Partition Area   : 15388672 KBytes
  eMMC Sector Cnt : H'0 - H'01D59FFF
 1:Boot Partition 1      : 4096 KBytes
  eMMC Sector Cnt : H'0 - H'00001FFF
 2:Boot Partition 2      : 4096 KBytes
  eMMC Sector Cnt : H'0 - H'00001FFF
---------------------------------------------------------
  Select area(0-2)>1                                          <<<< Enter the boot partition to erase

-- Boot Partition 1 Program -----------------------------
EM_E Complete!
```
After running the above command, the selected partition will be erased.  

## 4. How to build the RZ/V2M flash writer

This chapter describes how to build the RZ/V2M flash writer. The following commands should be executed in your work directory {$WORK} on your Host PC.

### 4.1. Prepare the compiler

Get the cross compiler for linaro.

Linaro toolchain:

```shell
$ cd {$WORK}
$ wget https://releases.linaro.org/components/toolchain/binaries/7.3-2018.05/aarch64-elf/gcc-linaro-7.3.1-2018.05-x86_64_aarch64-elf.tar.xz
$ tar xvf gcc-linaro-7.3.1-2018.05-x86_64_aarch64-elf.tar.xz
```

### 4.2. Prepare the source code

Get the source code of the RZ/V2M flash writer from this repository as follows.

```shell
cd ${WORK}
git clone -b rz_v2m https://github.com/renesas-rz/rzg2_flash_writer.git rzv2m_flash_writer
```

### 4.3. Build the RZ/V2M flash writer

The RZ/V2M flash writer will be built by the following command.

Linaro toolchain:

```shell
$ cd ${WORK}
$ make -f makefile.linaro clean
$ CROSS_COMPILE=../gcc-linaro-7.3.1-2018.05-x86_64_aarch64-elf/bin/aarch64-elf- make -f makefile.linaro
```

### 4.4. Check the output file
The output image will be available in the following directory.

| Generated files | File name    | File stored path                           |
|-----------------|--------------|--------------------------------------------|
| Flash writer    | B2_intSW.bin | ${WORK}/rzv2m_flash_writer /AArch64_output |

## 5. How to run the RZ/V2M flash writer
### Step.1 Equipment setting
Connect your PC and the target board via a serial to micro-USB cable.  
Start a terminal software on your PC. Set the configuration of terminal software is as follows and select the Standard COM port.  
| Baud rate  | Data bit | Parity bit | Stop bit | Flow control |
|------------|----------|------------|----------|--------------|
| 115200bps  | 8bits    | none       | 1bit     | none         |

### Step.2 Write the flash writer to the eMMC
Store the Flash writer binary (B2_intSW.bin) in a micro-SDHC Card that has 1 partition formatted with FAT32.  
Insert the micro-SD card into the micro-SD card slot on the RZ/V2M Evaluation Kit.  
Set the Main SW2 on the RZ/V2M Evaluation Kit is as the following table to change the board operation mode to "forced write mode".
| SW1 | SW2 | SW3 | SW4 |
|-----|-----|-----|-----|
| OFF | OFF | OFF | ON  |

Power on the RZ/V2M Evaluation Kit.  
Start RZ/V2M in forced write mode and write the Flash writer binary from the micro-SD card to eMMC.  
Check the lighting of Main LED 2.Note that if the LED is blinking, writing the Flash writer binary is failed.
After checking the LED, power off the RZ/V2M Evaluation Kit.


### Step.3 Start the flash writer
Set the Main SW2 on the RZ/V2M Evaluation Kit is as the following table to change the board operation mode to "normal mode".

| SW1 | SW2 | SW3 | SW4 |
|-----|-----|-----|-----|
| OFF | OFF | OFF | OFF |

Power on the RZ/V2M Evaluation Kit. The following log will appear if RZ/V2M starts in normal mode and run Flash writer successfully.  

```text
Flash writer for RZ/V2M V1.00 Jul 9, 2021
>
```

### Step.4 Write loader binaries to eMMC with Flash writer
Please enter any key from the console to continue.  
For details on how to write to the eMMC, please refer to 3.2 Command specification.  
After completing the writing, turn off the board.

### Step.5 Confirm booting by the boot loader and U-boot
Power on the RZ/V2M Evaluation Kit with the normal mode. And then, confirm that the boot loader and U-boot run normally.
