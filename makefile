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
FILENAME_ADD = _RZG2L_SMARC
DEVICE   = RZG2L
DDR_TYPE = DDR4
DDR_SIZE = 2GB
SWIZZLE  = T1C
#--------------------------------------
else ifeq ("$(BOARD)", "RZG2L_15MMSQ_DEV")
# RZ/G2L 15MMSQ Dev board
FILENAME_ADD = _RZG2L_15MMSQ_DEV
DEVICE   = RZG2L
DDR_TYPE = DDR4
DDR_SIZE = 4GB
SWIZZLE  = T1C
#--------------------------------------
else ifeq ("$(BOARD)", "RZG2L_21MMSQ_DEV")
# RZ/G2L 21MMSQ Dev board
FILENAME_ADD = _RZG2L_21MMSQ_DEV
DEVICE   = RZG2L
DDR_TYPE = DDR4
DDR_SIZE = 4GB
SWIZZLE  = T2C
#--------------------------------------
else ifeq ("$(BOARD)", "RZG2LC_DEV")
# RZ/G2LC Dev board
FILENAME_ADD = _RZG2LC_DEV
DEVICE   = RZG2LC
DDR_TYPE = DDR3L
DDR_SIZE = 1GB
SWIZZLE  = T3CL
endif

# Select SERIAL_FLASH("ENABLE"or"DISABLE" )
ifeq ("$(SERIAL_FLASH)", "")
SERIAL_FLASH = ENABLE
endif

# Select EMMC("ENABLE"or"DISABLE" )
ifeq ("$(EMMC)", "")
EMMC = DISABLE
endif

ifeq ("$(INTERNAL_MEMORY_ONLY)", "")
INTERNAL_MEMORY_ONLY = DISABLE
endif

ifeq ("$(INTERNAL_MEMORY_ONLY)", "ENABLE")
DDR_TYPE = INTERNAL
endif

#CPU
CPU     = -march=armv8-a
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

CFLAGS += -O0  
BOOT_DEF    = Writer
FILE_NAME   = $(OUTPUT_DIR)/Flash_Writer_SCIF$(FILENAME_ADD)_$(DDR_TYPE)_$(DDR_SIZE)

ifeq ("$(DEVICE)", "RZG2L")
	CFLAGS += -DRZG2L=1
endif

ifeq ("$(DEVICE)", "RZG2LC")
	CFLAGS += -DRZG2LC=1
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
ifeq ("$(DDR_SIZE)", "1GB")
	CFLAGS += -DDDR_SIZE_1GB=1
endif

ifeq ("$(SWIZZLE)", "T1C")
	CFLAGS += -DSWIZZLE_T1C=1
endif

ifeq ("$(SWIZZLE)", "T2C")
	CFLAGS += -DSWIZZLE_T2C=1
endif

ifeq ("$(SWIZZLE)", "T3CL")
	CFLAGS += -DSWIZZLE_T3CL=1
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
DDR_DIR = ddr
TOOL_DEF = "REWRITE_TOOL"

OUTPUT_FILE = $(FILE_NAME).axf

#Object file
OBJ_FILE_BOOT =					\
	$(OBJECT_DIR)/boot_mon.o	\
	$(OBJECT_DIR)/stack.o

SRC_FILE :=						\
	main.c						\
	init_scif.c					\
	scifdrv.c					\
	devdrv.c					\
	common.c					\
	dgtable.c					\
	dgmodul1.c					\
	memory_cmd.c				\
	Message.c					\
	ramckmdl.c					\
	cpudrv.c					\
	bootparam.c					\
	sys/syc.c					\
	sys/cpg.c					\
	sys/pfc.c					\
	sys/tzc_400.c

ifeq ("$(INTERNAL_MEMORY_ONLY)", "DISABLE")
SRC_FILE +=						\
	ddrcheck.c					\
	ddr/ddr.c
endif

ifeq ("$(SERIAL_FLASH)", "ENABLE")
SRC_FILE +=						\
	dgmodul4.c					\
	rpcqspidrv.c				\
	spiflash1drv.c
endif

ifeq ("$(EMMC)", "ENABLE")
SRC_FILE +=						\
	dg_emmc_config.c				\
	dg_emmc_access.c				\
	emmc_cmd.c					\
	emmc_init.c					\
	emmc_interrupt.c				\
	emmc_mount.c					\
	emmc_write.c					\
	emmc_erase.c					\
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
	$(AS)  -g $(CPU) $(AS_NEON) --MD $(patsubst %.o,%.d,$@) -I $(BOOTDIR) -I $(INCLUDE_DIR) -I $(DDR_DIR) $< -o $@ --defsym $(AArch32_64)=0 --defsym $(BOOT_DEF)=0 --defsym $(TOOL_DEF)=0

$(OBJECT_DIR)/%.o:%.c
	@if [ ! -e `dirname $@` ]; then mkdir -p `dirname $@`; fi
	$(CC) -g -Os $(ALIGN) $(CPU) $(CC_NEON) $(THUMB) -MMD -MP -c -I $(BOOTDIR) -I $(INCLUDE_DIR) -I $(DDR_DIR) $< -o $@ -D$(AArch32_64)=0 -D$(BOOT_DEF)=0 -D$(TOOL_DEF)=0 $(CFLAGS) -D$(DDR_DEF)=0

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
	@echo ==========  %date% %time%  ==========
	@echo ========== !!! Compile Complete !!! ==========


.PHONY: clean
clean:
	$(CL)  $(OBJECT_DIR)/* $(OUTPUT_DIR)/*

-include $(DEPEND_FILE)
