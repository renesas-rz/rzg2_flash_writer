#
# Copyright (c) 2021, Renesas Electronics Corporation. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#

ifeq ("$(BOARD)", "")
BOARD = RZG2L_SMARC
endif

ifeq ("$(BOARD)", "RZG2L_SMARC")
#--------------------------------------
# RZ/G2L Smarc board
#--------------------------------------
FILENAME_ADD = _RZG2L_SMARC
DEVICE   = RZG2L
DDR_TYPE = DDR4
DDR_SIZE = 2GB
SWIZZLE  = T1C
else ifeq ("$(BOARD)", "RZG2L_SMARC_PMIC")
#--------------------------------------
# RZ/G2L Smarc PMIC Edition board
#--------------------------------------
FILENAME_ADD = _RZG2L_SMARC_PMIC
DEVICE   = RZG2L
DDR_TYPE = DDR4
DDR_SIZE = 2GB_1PCS
SWIZZLE  = T1BC
else ifeq ("$(BOARD)", "RZG2L_15MMSQ_DEV")
#--------------------------------------
# RZ/G2L 15MMSQ Dev board
#--------------------------------------
FILENAME_ADD = _RZG2L_15MMSQ_DEV
DEVICE   = RZG2L
DDR_TYPE = DDR4
DDR_SIZE = 4GB
SWIZZLE  = T1C
else ifeq ("$(BOARD)", "RZG2L_21MMSQ_DEV")
#--------------------------------------
# RZ/G2L 21MMSQ Dev board
#--------------------------------------
FILENAME_ADD = _RZG2L_21MMSQ_DEV
DEVICE   = RZG2L
DDR_TYPE = DDR4
DDR_SIZE = 4GB
SWIZZLE  = T2C
else ifeq ("$(BOARD)", "RZG2LC_SMARC")
#--------------------------------------
# RZ/G2LC Smarc board
#--------------------------------------
FILENAME_ADD = _RZG2LC_SMARC
DEVICE   = RZG2LC
DDR_TYPE = DDR4
DDR_SIZE = 1GB_1PCS
SWIZZLE  = T3BC
else ifeq ("$(BOARD)", "RZG2LC_DEV")
#--------------------------------------
# RZ/G2LC Dev board
#--------------------------------------
FILENAME_ADD = _RZG2LC_DEV
DEVICE   = RZG2LC
DDR_TYPE = DDR3L
DDR_SIZE = 1GB
SWIZZLE  = T3CL
else ifeq ("$(BOARD)", "RZG2UL_TYPE1_DEV")
#--------------------------------------
# RZ/G2UL Type1 DDR4 Dev board
#--------------------------------------
FILENAME_ADD = _RZG2UL_TYPE1_DEV
DEVICE   = RZG2UL
DDR_TYPE = DDR4
DDR_SIZE = 1GB_1PCS
SWIZZLE  = T3BCUD
DEVICE_TYPE = 1
else ifeq ("$(BOARD)", "RZG2UL_TYPE1_DDR3L_DEV")
#--------------------------------------
# RZ/G2UL Type1 DDR3L Dev board
#--------------------------------------
FILENAME_ADD = _RZG2UL_TYPE1_DEV
DEVICE   = RZG2UL
DDR_TYPE = DDR3L
DDR_SIZE = 512MB_1PCS
SWIZZLE  = T3BCUL
DEVICE_TYPE = 1
else ifeq ("$(BOARD)", "RZG2UL_SMARC")
#--------------------------------------
# RZ/G2UL Smarc board
#--------------------------------------
FILENAME_ADD = _RZG2UL_SMARC
DEVICE   = RZG2UL
DDR_TYPE = DDR4
DDR_SIZE = 1GB_1PCS
SWIZZLE  = T3BCUD2
DEVICE_TYPE = 1
else ifeq ("$(BOARD)", "RZG2UL_TYPE2_DEV")
#--------------------------------------
# RZ/G2UL Type2 Dev board
#--------------------------------------
FILENAME_ADD = _RZG2UL_TYPE2_DEV
DEVICE   = RZG2UL
DDR_TYPE = DDR3L
DDR_SIZE = 1GB
SWIZZLE  = T3CL
DEVICE_TYPE = 2
else ifeq ("$(BOARD)", "RZV2L_SMARC")
#--------------------------------------
# RZ/V2L Smarc board
#--------------------------------------
FILENAME_ADD = _RZV2L_SMARC
DEVICE   = RZV2L
DDR_TYPE = DDR4
DDR_SIZE = 4GB
SWIZZLE  = T1C
else ifeq ("$(BOARD)", "RZV2L_SMARC_PMIC")
#--------------------------------------
# RZ/V2L Smarc PMIC Edition board
#--------------------------------------
FILENAME_ADD = _RZV2L_SMARC_PMIC
DEVICE   = RZV2L
DDR_TYPE = DDR4
DDR_SIZE = 2GB_1PCS
SWIZZLE  = T1BC
else ifeq ("$(BOARD)", "RZV2L_15MMSQ_DEV")
#--------------------------------------
# RZ/V2L 15MMSQ Dev board
#--------------------------------------
FILENAME_ADD = _RZV2L_15MMSQ_DEV
DEVICE   = RZV2L
DDR_TYPE = DDR4
DDR_SIZE = 4GB
SWIZZLE  = T1C
else ifeq ("$(BOARD)", "RZV2L_21MMSQ_DEV")
#--------------------------------------
# RZ/V2L 21MMSQ Dev board
#--------------------------------------
FILENAME_ADD = _RZV2L_21MMSQ_DEV
DEVICE   = RZV2L
DDR_TYPE = DDR4
DDR_SIZE = 4GB
SWIZZLE  = T2C
endif

# Select SERIAL_FLASH("ENABLE"or"DISABLE" )
ifeq ("$(SERIAL_FLASH)", "")
SERIAL_FLASH = ENABLE
endif

# Select EMMC("ENABLE"or"DISABLE" )
ifeq ("$(EMMC)", "")
EMMC = DISABLE
endif

# Select QSPI IO Voltage("1_8V"or"3_3V" )
ifeq ("$(QSPI_IOV)", "")
QSPI_IOV=1_8V
endif

# Select eMMC IO Voltage("1_8V"or"3_3V" )
ifeq ("$(EMMC_IOV)", "")
EMMC_IOV=1_8V
endif

ifeq ("$(INTERNAL_MEMORY_ONLY)", "")
INTERNAL_MEMORY_ONLY = DISABLE
endif

ifeq ("$(INTERNAL_MEMORY_ONLY)", "ENABLE")
DDR_TYPE = INTERNAL
endif

#CPU
CPU     = 
AArch   = 64
THUMB   = 
AS_NEON = 
CC_NEON = -mgeneral-regs-only
ALIGN   = -mstrict-align
AArch32_64  = AArch64
BOOTDIR     = AArch64_boot
OUTPUT_DIR  = AArch64_output
OBJECT_DIR  = AArch64_obj
CROSS_COMPILE ?= aarch64-elf-

CFLAGS += -O0 -fno-stack-protector -fno-exceptions -fno-unwind-tables -fno-asynchronous-unwind-tables
BOOT_DEF    = Writer
FILE_NAME   = $(OUTPUT_DIR)/Flash_Writer_SCIF$(FILENAME_ADD)_$(DDR_TYPE)_$(DDR_SIZE)

ifeq ("$(DEVICE)", "RZG2L")
	CFLAGS += -DRZG2L=1
endif
ifeq ("$(DEVICE)", "RZG2LC")
	CFLAGS += -DRZG2LC=1
endif
ifeq ("$(DEVICE)", "RZG2UL")
	CFLAGS += -DRZG2UL=1
endif
ifeq ("$(DEVICE)", "RZV2L")
	CFLAGS += -DRZV2L=1
endif

ifeq ("$(DDR_TYPE)", "DDR4")
	CFLAGS += -DDDR4=1
endif
ifeq ("$(DDR_TYPE)", "DDR3L")
	CFLAGS += -DDDR3L=1
endif

ifeq ("$(DDR_SIZE)", "4GB")
	CFLAGS += -DDDR_SIZE_4GB=1
endif
ifeq ("$(DDR_SIZE)", "2GB")
	CFLAGS += -DDDR_SIZE_2GB=1
endif
ifeq ("$(DDR_SIZE)", "2GB_1PCS")
	CFLAGS += -DDDR_SIZE_2GB_1PCS=1
endif
ifeq ("$(DDR_SIZE)", "1GB")
	CFLAGS += -DDDR_SIZE_1GB=1
endif
ifeq ("$(DDR_SIZE)", "1GB_1PCS")
	CFLAGS += -DDDR_SIZE_1GB_1PCS=1
endif
ifeq ("$(DDR_SIZE)", "512MB_1PCS")
	CFLAGS += -DDDR_SIZE_512MB_1PCS=1
endif

ifeq ("$(SWIZZLE)", "T1C")
	CFLAGS += -DSWIZZLE_T1C=1
endif

ifeq ("$(SWIZZLE)", "T1BC")
	CFLAGS += -DSWIZZLE_T1BC=1
endif

ifeq ("$(SWIZZLE)", "T2C")
	CFLAGS += -DSWIZZLE_T2C=1
endif

ifeq ("$(SWIZZLE)", "T3BC")
	CFLAGS += -DSWIZZLE_T3BC=1
endif

ifeq ("$(SWIZZLE)", "T3CL")
	CFLAGS += -DSWIZZLE_T3CL=1
endif

ifeq ("$(SWIZZLE)", "T3BCUL")
	CFLAGS += -DSWIZZLE_T3BCUL=1
endif

ifeq ("$(SWIZZLE)", "T3BCUD")
	CFLAGS += -DSWIZZLE_T3BCUD=1
endif

ifeq ("$(SWIZZLE)", "T3BCUD2")
	CFLAGS += -DSWIZZLE_T3BCUD2=1
endif

ifeq ("$(DEVICE_TYPE)", "1")
	CFLAGS += -DDEVICE_TYPE=1
endif

ifeq ("$(DEVICE_TYPE)", "2")
	CFLAGS += -DDEVICE_TYPE=2
endif

ifeq ("$(SERIAL_FLASH)", "ENABLE")
	CFLAGS += -DSERIAL_FLASH=1
endif
ifeq ("$(SERIAL_FLASH)", "DISABLE")
	CFLAGS += -DSERIAL_FLASH=0
endif

ifeq ("$(EMMC)", "ENABLE")
	CFLAGS += -DEMMC=1
endif
ifeq ("$(EMMC)", "DISABLE")
	CFLAGS += -DEMMC=0
endif

ifeq ("$(QSPI_IOV)", "1_8V")
	CFLAGS += -DQSPI_IO_1_8V=1
endif
ifeq ("$(EMMC_IOV)", "1_8V")
	CFLAGS += -DEMMC_IO_1_8V=1
endif

ifeq ("$(INTERNAL_MEMORY_ONLY)", "DISABLE")
	CFLAGS += -DINTERNAL_MEMORY_ONLY=0
	MEMORY_DEF = memory_writer.def
endif
ifeq ("$(INTERNAL_MEMORY_ONLY)", "ENABLE")
	CFLAGS += -DINTERNAL_MEMORY_ONLY=1
	MEMORY_DEF = memory_writer_internal.def
endif

DDR_DEF = ddr_qos_init_setting

LIBS        = -L$(subst libc.a, ,$(shell $(CC) -print-file-name=libc.a 2> /dev/null)) -lc
LIBS        += -L$(subst libgcc.a, ,$(shell $(CC) -print-libgcc-file-name 2> /dev/null)) -lgcc

INCLUDE_DIR = include
DDR_COMMON = ddr/common
ifeq ("$(DEVICE)", "RZV2L")
DDR_SOC    = ddr/v2l
else ifeq ("$(DEVICE)", "RZG2UL")
DDR_SOC    = ddr/g2ul
else
DDR_SOC    = ddr/g2l
endif
TOOL_DEF = "REWRITE_TOOL"

OUTPUT_FILE = $(FILE_NAME).axf

#Object file
OBJ_FILE_BOOT =				\
	$(OBJECT_DIR)/boot_mon.o	\
	$(OBJECT_DIR)/stack.o

SRC_FILE :=				\
	main.c				\
	init_scif.c			\
	scifdrv.c			\
	devdrv.c			\
	common.c			\
	dgtable.c			\
	dgmodul1.c			\
	memory_cmd.c			\
	Message.c			\
	ramckmdl.c			\
	cpudrv.c			\
	sys/syc.c			\
	sys/sysc.c			\
	sys/cpg.c			\
	sys/pfc.c			\
	sys/tzc_400.c

ifeq ("$(INTERNAL_MEMORY_ONLY)", "DISABLE")
SRC_FILE +=				\
	ddrcheck.c			\
	ddr/common/ddr.c
endif
ifeq ("$(DEVICE)", "RZV2L")
SRC_FILE +=				\
	ddr/v2l/ddr_v2l.c
else ifeq ("$(DEVICE)", "RZG2UL")
SRC_FILE +=				\
	ddr/g2ul/ddr_g2ul.c
else
SRC_FILE +=				\
	ddr/g2l/ddr_g2l.c
endif

ifeq ("$(SERIAL_FLASH)", "ENABLE")
SRC_FILE +=				\
	dgmodul4.c			\
	rpcqspidrv.c			\
	spiflash1drv.c
endif

ifeq ("$(EMMC)", "ENABLE")
SRC_FILE +=				\
	dg_emmc_config.c		\
	dg_emmc_access.c		\
	emmc_cmd.c			\
	emmc_init.c			\
	emmc_interrupt.c		\
	emmc_mount.c			\
	emmc_write.c			\
	emmc_erase.c			\
	emmc_utility.c
endif

OBJ_FILE := $(addprefix $(OBJECT_DIR)/,$(patsubst %.c,%.o,$(SRC_FILE)))

#Dependency File
DEPEND_FILE = $(patsubst %.lib, ,$(OBJ_FILE:%.o=%.d))

###################################################
#C compiler
CC ?= $(CROSS_COMPILE)gcc
#Assembler
AS ?= $(CROSS_COMPILE)as
#Linker
LD ?= $(CROSS_COMPILE)ld
#Liblary
AR ?= $(CROSS_COMPILE)ar
#Object dump
OBJDUMP ?= $(CROSS_COMPILE)objdump
#Object copy
OBJCOPY ?= $(CROSS_COMPILE)objcopy

#clean
CL = rm -rf

###################################################
# Suffixes
.SUFFIXES : .s .c .o

###################################################
# Command

.PHONY: all
all: $(OBJECT_DIR) $(OUTPUT_DIR) $(OBJ_FILE_BOOT) $(OBJ_FILE) $(OUTPUT_FILE)

#------------------------------------------
# Make Directory
#------------------------------------------
$(OBJECT_DIR):
	-mkdir "$(OBJECT_DIR)"

$(OUTPUT_DIR):
	-mkdir "$(OUTPUT_DIR)"

#------------------------------------------
# Compile
#------------------------------------------
$(OBJECT_DIR)/%.o:$(BOOTDIR)/%.s
	$(AS)  -g $(CPU) $(AS_NEON) --MD $(patsubst %.o,%.d,$@) -I $(BOOTDIR) -I $(INCLUDE_DIR) -I $(DDR_COMMON) -I $(DDR_SOC) $< -o $@ --defsym $(AArch32_64)=0 --defsym $(BOOT_DEF)=0 --defsym $(TOOL_DEF)=0

$(OBJECT_DIR)/%.o:%.c
	@if [ ! -e `dirname $@` ]; then mkdir -p `dirname $@`; fi
	$(CC) -g -Os $(ALIGN) $(CPU) $(CC_NEON) $(THUMB) -MMD -MP -c -I $(BOOTDIR) -I $(INCLUDE_DIR) -I $(DDR_COMMON) -I $(DDR_SOC) $< -o $@ -D$(AArch32_64)=0 -D$(BOOT_DEF)=0 -D$(TOOL_DEF)=0 $(CFLAGS) -D$(DDR_DEF)=0

#------------------------------------------
# Linker
#------------------------------------------
$(OUTPUT_FILE): $(OBJ_FILE_BOOT) $(OBJ_FILE) $(MEMORY_DEF)
	$(LD) $(OBJ_FILE_BOOT) $(OBJ_FILE) 	\
	-T '$(MEMORY_DEF)'			\
	-o '$(OUTPUT_FILE)'			\
	-Map '$(FILE_NAME).map' 		\
	-static					\
	$(LIBS)

#   Make MOT file
	$(OBJCOPY) -O srec --srec-forceS3 "$(OUTPUT_FILE)" "$(FILE_NAME).mot"

#   Make Binary file
	$(OBJCOPY) -O binary "$(OUTPUT_FILE)" "$(FILE_NAME).bin"

#   Dis assemble
	$(OBJDUMP) -d -S "$(OUTPUT_FILE)" > "$(FILE_NAME)_disasm.txt"

#	Time Stamp
	@echo ==========  `date`  ==========
	@echo ========== !!! Compile Complete !!! ==========


.PHONY: clean
clean:
	$(CL)  $(OBJECT_DIR)/* $(OUTPUT_DIR)/*

-include $(DEPEND_FILE)
