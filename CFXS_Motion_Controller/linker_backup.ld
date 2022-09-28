STACK_SIZE = 8K;

MEMORY
{
    FLASH (rx) : ORIGIN = 0x00000000, LENGTH = 1M
    SRAM (rwx) : ORIGIN = 0x20000000, LENGTH = 256K
}

ENTRY(__cfxs_reset)
SECTIONS
{

    .text :
    {
        _text = .;
        __TEXT_START__ = .;
        KEEP(*(.vector_table))
        *(.text .text* .gnu.linkonce.t.*)
        *(.glue_7t) *(.glue_7)
        *(.rodata .rodata* .gnu.linkonce.r.*)
        *(.ARM.extab* .gnu.linkonce.armextab.*)
        
        . = ALIGN(4);
        KEEP(*(.init))

        . = ALIGN(4);
        __PREINIT_ARRAY_START__ = .;
        KEEP(*(SORT(.preinit_array*)))
        KEEP(*(.preinit_array))
        __PREINIT_ARRAY_END__ = .;
        
        . = ALIGN(4);
        __INIT_ARRAY_START__ = .;
        KEEP(*(SORT(.init_array.*)))
        KEEP(*(.init_array))
        __INIT_ARRAY_END__ = .;
        
        . = ALIGN(4);
        KEEP(*(.fini))

        . = ALIGN(4);
        __FINI_ARRAY_START__ = .;
        KEEP(*(.fini_array))
        KEEP(*(SORT(.fini_array.*)))
        __FINI_ARRAY_END__ = .;

        . = ALIGN(4);
        
        PROVIDE_HIDDEN(__exidx_start = .);
        *(.ARM.exidx* .gnu.linkonce.armexidx.*)
        PROVIDE_HIDDEN(__exidx_end = .);

        . = ALIGN(4);
        __TEXT_END__ = .;
    } > FLASH
    

    __CONST_DATA_START__ = .;
    .data : AT(ADDR(.text) + SIZEOF(.text))
    {
        *(.vtable)
        . = ALIGN(0x200); /*0x200 RAM remapped vector table*/
        __DATA_START__ = .;
        *(.data .data* .ram .ram* .gnu.linkonce.d.*)
        __DATA_END__ = .;
        *(.noinit .noinit*)
    } > SRAM
    
    .bss (NOLOAD):
    {
        . = ALIGN(4);
        __BSS_START__ = .;
        *(.bss*)
        *(COMMON)
        . = ALIGN(4); 
        __BSS_END__ = .;
        . = ALIGN(8);
    } > SRAM
    
    . = ALIGN(4);
    _end = . ;
}

PROVIDE(end = _end);

/* end of allocated ram is start of heap, heap grows up towards stack*/
PROVIDE(__HEAP_START__ = _end);
PROVIDE(__HEAP_END__ = ORIGIN(SRAM) + LENGTH(SRAM) - STACK_SIZE);

/* top of stack starts at end of ram, stack grows down towards heap */
PROVIDE(__STACK_START__ = ORIGIN(SRAM) + LENGTH(SRAM));
PROVIDE(__STACK_END__  = ORIGIN(SRAM) + LENGTH(SRAM) - STACK_SIZE);