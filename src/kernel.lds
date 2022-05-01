
ENTRY(_start)
SECTIONS
{
    . = (((0x20000000) + (((0) << 20))));

    .user_text : {
        obj/user*.o(.text*)
    }

    .user_rodata : {
        obj/user*.o(.rodata*)
    }

    . = (((0x20000000) + (((1) << 20))));

    .data : {
        obj/user*.o(.data)
    }

    .bss : {
        obj/user*.o(.bss)
        obj/user*.o(COMMON)
    }

    . = (((0x20000000) + (((2) << 20))));

    .text : {
        *(.exception_vector)
        *(.text*)
    }

    .rodata : {
        *(.rodata*)
    }

    . = (((0x20000000) + (((3) << 20))));

    .data : {
        *(.data)
    }

    .bss : {
        *(.bss) *(COMMON)
    }
}
