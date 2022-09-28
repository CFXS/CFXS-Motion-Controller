/* Entry Point */
ENTRY(__cfxs_reset)

/* Memory Regions */
MEMORY {
    ITCMRAM (rwx) : ORIGIN = 0x00000000, LENGTH = 64K
    FLASH   ( rx) : ORIGIN = 0x08000000, LENGTH = 2M
    DTCMRAM (rwx) : ORIGIN = 0x20000000, LENGTH = 128K
    RAM_D1  (rwx) : ORIGIN = 0x24000000, LENGTH = 512K
    RAM_D2  (rwx) : ORIGIN = 0x30000000, LENGTH = 288K
    RAM_D3  (rwx) : ORIGIN = 0x38000000, LENGTH = 64K
}

/* Sections */
SECTIONS {
    .vector_table : {
        PROVIDE(__VECTOR_TABLE_START__ = .)
        . = ALIGN(4)
        KEEP(*(.vector_table))
        PROVIDE(__VECTOR_TABLE_END__ = .)
    } > FLASH

    .text : {
        PROVIDE(__TEXT_START__ = .)
        . = ALIGN(4)
        *(.text .text* .gnu.linkonce.t.*)
        . = ALIGN(4)
        *(.glue_7 .glue_7t)
        . = ALIGN(4)
        KEEP(*(.init .fini))
        PROVIDE(__TEXT_END__ = .)
    } > FLASH

    .rodata : {
        PROVIDE(__RODATA_START__ = .)
        . = ALIGN(4)
        *(.rodata .rodata* .gnu.linkonce.r.*)
        PROVIDE(__RODATA_END__ = .)
    } > FLASH

    .ARM.extab : {
        . = ALIGN(4)
        *(.ARM.extab* .gnu.linkonce.armextab.*)
    } > FLASH

    .ARM : {
        PROVIDE_HIDDEN(__exidx_start = .)
        . = ALIGN(4)
        *(.ARM.exidx* .gnu.linkonce.armexidx.*)
        PROVIDE_HIDDEN(__exidx_end = .)
    } > FLASH

    .preinit_array : {
        PROVIDE(__PREINIT_ARRAY_START__ = .)
        . = ALIGN(4)
        KEEP(*(.preinit_array*))
        PROVIDE(__PREINIT_ARRAY_END__ = .)
    } > FLASH

    .init_array : {
        PROVIDE(__INIT_ARRAY_START__ = .)
        . = ALIGN(4)
        KEEP(*(SORT(.init_array.*)))
        KEEP(*(.init_array))
        PROVIDE(__INIT_ARRAY_END__ = .)
    } > FLASH

    .fini_array : {
        PROVIDE(__FINI_ARRAY_START__ = .)
        . = ALIGN(4)
        KEEP(*(SORT(.fini_array.*)))
        KEEP(*(.fini_array))
        PROVIDE(__FINI_ARRAY_END__ = .)
    } > FLASH

    .data : {
        PROVIDE(__DATA_START__ = .)
        . = ALIGN(4)
        *(.data .data*)
        *(.ram_func .ram_func*)
        PROVIDE(__DATA_END__ = .)
        PROVIDE(__NOINIT_START__ = .)
        . = ALIGN(4)
        *(.noinit .noinit*)
        PROVIDE(__NOINIT_END__ = .)
    } > RAM_D1 AT > FLASH

    .ram_2 : {
        PROVIDE(__RAM_2_START__ = .)
        . = ALIGN(4)
        *(.ram_2 .ram_2*)
        PROVIDE(__RAM_2_END__ = .)
    } > RAM_D2 AT > FLASH

    .ram_3 : {
        PROVIDE(__RAM_3_START__ = .)
        . = ALIGN(4)
        *(.ram_3 .ram_3*)
        PROVIDE(__RAM_3_END__ = .)
    } > RAM_D3 AT > FLASH

    .ram_cache : {
        PROVIDE(__RAM_CACHE_START__ = .)
        . = ALIGN(4)
        *(.ram_cache .ram_cache*)
        PROVIDE(__RAM_CACHE_END__ = .)
    } > DTCMRAM AT > FLASH

    .instruction_cache : {
        PROVIDE(__INSTRUCTION_CACHE_START__ = .)
        . = ALIGN(4)
        *(.fast_ram_func .fast_ram_func*)
        PROVIDE(__INSTRUCTION_CACHE_END__ = .)
    } > ITCMRAM AT > FLASH

    .bss (NOLOAD) : {
        PROVIDE(__BSS_START__ = .)
        . = ALIGN(4)
        *(.bss .bss* COMMON)
        PROVIDE(__BSS_END__ = .)
    } > RAM_D1 AT > FLASH

    .ARM.attributes 0 : { *(.ARM.attributes) }
}

/* Section Range Defines */
PROVIDE(__CONST_VECTOR_TABLE_START__      = LOADADDR(.vector_table))
PROVIDE(__CONST_VECTOR_TABLE_END__        = LOADADDR(.vector_table) + SIZEOF(.vector_table))
PROVIDE(__CONST_VECTOR_TABLE_SIZE__       = SIZEOF(.vector_table))
PROVIDE(__CONST_TEXT_START__              = LOADADDR(.text))
PROVIDE(__CONST_TEXT_END__                = LOADADDR(.text) + SIZEOF(.text))
PROVIDE(__CONST_TEXT_SIZE__               = SIZEOF(.text))
PROVIDE(__CONST_RODATA_START__            = LOADADDR(.rodata))
PROVIDE(__CONST_RODATA_END__              = LOADADDR(.rodata) + SIZEOF(.rodata))
PROVIDE(__CONST_RODATA_SIZE__             = SIZEOF(.rodata))
PROVIDE(__CONST_ARM_START__               = LOADADDR(.ARM))
PROVIDE(__CONST_ARM_END__                 = LOADADDR(.ARM) + SIZEOF(.ARM))
PROVIDE(__CONST_ARM_SIZE__                = SIZEOF(.ARM))
PROVIDE(__CONST_PREINIT_ARRAY_START__     = LOADADDR(.preinit_array))
PROVIDE(__CONST_PREINIT_ARRAY_END__       = LOADADDR(.preinit_array) + SIZEOF(.preinit_array))
PROVIDE(__CONST_PREINIT_ARRAY_SIZE__      = SIZEOF(.preinit_array))
PROVIDE(__CONST_INIT_ARRAY_START__        = LOADADDR(.init_array))
PROVIDE(__CONST_INIT_ARRAY_END__          = LOADADDR(.init_array) + SIZEOF(.init_array))
PROVIDE(__CONST_INIT_ARRAY_SIZE__         = SIZEOF(.init_array))
PROVIDE(__CONST_FINI_ARRAY_START__        = LOADADDR(.fini_array))
PROVIDE(__CONST_FINI_ARRAY_END__          = LOADADDR(.fini_array) + SIZEOF(.fini_array))
PROVIDE(__CONST_FINI_ARRAY_SIZE__         = SIZEOF(.fini_array))
PROVIDE(__CONST_DATA_START__              = LOADADDR(.data))
PROVIDE(__CONST_DATA_END__                = LOADADDR(.data) + SIZEOF(.data))
PROVIDE(__CONST_DATA_SIZE__               = SIZEOF(.data))
PROVIDE(__CONST_RAM_2_START__             = LOADADDR(.ram_2))
PROVIDE(__CONST_RAM_2_END__               = LOADADDR(.ram_2) + SIZEOF(.ram_2))
PROVIDE(__CONST_RAM_2_SIZE__              = SIZEOF(.ram_2))
PROVIDE(__CONST_RAM_3_START__             = LOADADDR(.ram_3))
PROVIDE(__CONST_RAM_3_END__               = LOADADDR(.ram_3) + SIZEOF(.ram_3))
PROVIDE(__CONST_RAM_3_SIZE__              = SIZEOF(.ram_3))
PROVIDE(__CONST_RAM_CACHE_START__         = LOADADDR(.ram_cache))
PROVIDE(__CONST_RAM_CACHE_END__           = LOADADDR(.ram_cache) + SIZEOF(.ram_cache))
PROVIDE(__CONST_RAM_CACHE_SIZE__          = SIZEOF(.ram_cache))
PROVIDE(__CONST_INSTRUCTION_CACHE_START__ = LOADADDR(.instruction_cache))
PROVIDE(__CONST_INSTRUCTION_CACHE_END__   = LOADADDR(.instruction_cache) + SIZEOF(.instruction_cache))
PROVIDE(__CONST_INSTRUCTION_CACHE_SIZE__  = SIZEOF(.instruction_cache))
PROVIDE(__CONST_BSS_START__               = LOADADDR(.bss))
PROVIDE(__CONST_BSS_END__                 = LOADADDR(.bss) + SIZEOF(.bss))
PROVIDE(__CONST_BSS_SIZE__                = SIZEOF(.bss))

/* Memory Region Range Defines */
PROVIDE(__REGION_FLASH_START__             = ORIGIN(FLASH));
PROVIDE(__REGION_FLASH_END__               = ORIGIN(FLASH) + LENGTH(FLASH));
PROVIDE(__REGION_FLASH_SIZE__              = LENGTH(FLASH));
PROVIDE(__REGION_DTCMRAM_START__           = ORIGIN(DTCMRAM));
PROVIDE(__REGION_DTCMRAM_END__             = ORIGIN(DTCMRAM) + LENGTH(DTCMRAM));
PROVIDE(__REGION_DTCMRAM_SIZE__            = LENGTH(DTCMRAM));
PROVIDE(__REGION_RAM_3_START__             = ORIGIN(RAM_3));
PROVIDE(__REGION_RAM_3_END__               = ORIGIN(RAM_3) + LENGTH(RAM_3));
PROVIDE(__REGION_RAM_3_SIZE__              = LENGTH(RAM_3));
PROVIDE(__REGION_RAM_D3_START__            = ORIGIN(RAM_D3));
PROVIDE(__REGION_RAM_D3_END__              = ORIGIN(RAM_D3) + LENGTH(RAM_D3));
PROVIDE(__REGION_RAM_D3_SIZE__             = LENGTH(RAM_D3));
PROVIDE(__REGION_INSTRUCTION_CACHE_START__ = ORIGIN(INSTRUCTION_CACHE));
PROVIDE(__REGION_INSTRUCTION_CACHE_END__   = ORIGIN(INSTRUCTION_CACHE) + LENGTH(INSTRUCTION_CACHE));
PROVIDE(__REGION_INSTRUCTION_CACHE_SIZE__  = LENGTH(INSTRUCTION_CACHE));
PROVIDE(__REGION_RAM_START__               = ORIGIN(RAM));
PROVIDE(__REGION_RAM_END__                 = ORIGIN(RAM) + LENGTH(RAM));
PROVIDE(__REGION_RAM_SIZE__                = LENGTH(RAM));
PROVIDE(__REGION_RAM_D1_START__            = ORIGIN(RAM_D1));
PROVIDE(__REGION_RAM_D1_END__              = ORIGIN(RAM_D1) + LENGTH(RAM_D1));
PROVIDE(__REGION_RAM_D1_SIZE__             = LENGTH(RAM_D1));
PROVIDE(__REGION_ITCMRAM_START__           = ORIGIN(ITCMRAM));
PROVIDE(__REGION_ITCMRAM_END__             = ORIGIN(ITCMRAM) + LENGTH(ITCMRAM));
PROVIDE(__REGION_ITCMRAM_SIZE__            = LENGTH(ITCMRAM));
PROVIDE(__REGION_RAM_D2_START__            = ORIGIN(RAM_D2));
PROVIDE(__REGION_RAM_D2_END__              = ORIGIN(RAM_D2) + LENGTH(RAM_D2));
PROVIDE(__REGION_RAM_D2_SIZE__             = LENGTH(RAM_D2));
PROVIDE(__REGION_RAM_CACHE_START__         = ORIGIN(RAM_CACHE));
PROVIDE(__REGION_RAM_CACHE_END__           = ORIGIN(RAM_CACHE) + LENGTH(RAM_CACHE));
PROVIDE(__REGION_RAM_CACHE_SIZE__          = LENGTH(RAM_CACHE));
PROVIDE(__REGION_RAM_2_START__             = ORIGIN(RAM_2));
PROVIDE(__REGION_RAM_2_END__               = ORIGIN(RAM_2) + LENGTH(RAM_2));
PROVIDE(__REGION_RAM_2_SIZE__              = LENGTH(RAM_2));
