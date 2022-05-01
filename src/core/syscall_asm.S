
.global _exit
_exit:
    ldr r0, [r1]
    b _sys_exit

.global _fork
_fork:
    ldr r0, [r1]
    ldr r1, [r1, #4]
    b _sys_fork

.global _thread_create
_thread_create:
    ldr r0, [r1]
    ldr r1, [r1, #4]
    b _sys_thread_create

.global _read
_read:
    b _sys_read

.global _write
_write:
    ldr r0, [r1]
    b _sys_write

.global _sleep
_sleep:
    ldr r0, [r1]
    b _sys_sleep

.global _kprint_num
_kprint_num:
    mov r10, r1
    ldr r0, [r10]
    ldr r1, [r10, #4]
    ldr r2, [r10, #8]
    b _sys_kprint_num