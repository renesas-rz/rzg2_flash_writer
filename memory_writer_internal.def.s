/*
 * Copyright (c) 2015-2021, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

MEMORY {
	RAM2 (rwxa): ORIGIN = 0x00010000, LENGTH = 0x00001E00
#if (TRUSTED_BOARD_BOOT != 1)
	CERT (rwxa): ORIGIN = 0x00011E00, LENGTH = 0x00000200
	RAM  (rwxa): ORIGIN = 0x00012000, LENGTH = 0x0000D000
#else
	RAM  (rwxa): ORIGIN = 0x00013000, LENGTH = 0x0000C000
#endif
	RAM3 (rwxa): ORIGIN = 0x0002F000, LENGTH = 0x00001000
}

SECTIONS
{
#if (TRUSTED_BOARD_BOOT != 1)
	.cert : {
		. = 0x00000000;
		LONG(__LOAD_SIZE__)
		FILL(0xFF)
		. = 0x000001FC;
		LONG(0xAA55FFFF)
	} > CERT
#else
	/* 
		This is the section where the boot parameter and manifests for 
		trusted board boot are located.
		Boot parameter and manifests are created in the external environment.
	*/
#endif

	.text : {
		__RO_START__ = .;
		*(.text*)
		*(.rodata*)
		. = NEXT(64);
		__RO_END__ = .;
	} > RAM

	.data : {
		__DATA_START__ = .;
		 *(.data)
		. = NEXT(64);
		__DATA_END__ = .;
	} > RAM

	__DATA_SIZE__ = SIZEOF(.data);

	__LOAD_SIZE__ = __DATA_END__ - __RO_START__;

	.bss : {
		__BSS_START__ = .;
		 *(.bss)
		 *(COMMON)
		 . = NEXT(64);
		__BSS_END__ = .;
	} > RAM2

	stacks (NOLOAD) : ALIGN(64) {
		__STACKS_START__ = .;
		KEEP(*(writer_stack))
		__STACKS_END__ = .;
	} > RAM2

	__BSS_SIZE__ = SIZEOF(.bss);
}
