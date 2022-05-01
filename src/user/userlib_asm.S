#include <syscall_nums.h>

/* syscall user interface */

.global exit
exit:
    svc SYS_EXIT
    bx lr

.global fork
fork:
    svc SYS_FORK
    bx lr

.global child_create
child_create:
    svc SYS_THREAD_CREATE
    bx lr

.global kprint_num
kprint_num:
    svc SYS_PRINT_NUM
    bx lr

.global readc
readc:
    svc SYS_READ
    bx lr

.global writec
writec:
    svc SYS_WRITE
    bx lr

.global sleep
sleep:
    svc SYS_SLEEP
    bx lr