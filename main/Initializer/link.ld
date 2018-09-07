ENTRY(_initializer_start)

SECTIONS {
	system_start = 1M;
    . = system_start;

    .boot :
    {
        /* ensure that the multiboot header is at the beginning */
        *(.multiboot)
    }

    .text :
    {
        *(.text)
    }

    .rodata : {
            *(.rodata)
    }

    .bss :
    {
        *(.bss)
    }
    system_end = .;
}
