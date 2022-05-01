#include <mem_layout.h>

ENTRY(_start)
SECTIONS
{
    . = USER_TEXT_PAGE;

    .user_text : {
        obj/user*.o(.text*)
    }

    .user_rodata : {
        obj/user*.o(.rodata*)
    }

    . = USER_DATA_PAGE;

    .data : {
        obj/user*.o(.data)
    }

    .bss : {
        obj/user*.o(.bss)
        obj/user*.o(COMMON)
    }

    . = KERNEL_TEXT_PAGE;

    .text : {
        *(.exception_vector)
        *(.text*)
    }

    .rodata : {
        *(.rodata*)
    }

    . = KERNEL_DATA_PAGE;

    .data : {
        *(.data)
    }

    .bss : {
        *(.bss) *(COMMON)
    }
}

