#include "mem_layout.h"

.global _start
_start:
 	ldr	sp,     =KERNEL_STACK
        bl      start_kernel
.Lend:
        b       .Lend

