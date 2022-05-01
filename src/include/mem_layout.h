#ifndef MEM_LAYOUT_H
#define MEM_LAYOUT_H

/***
 * (0x24000000 - 0x20000000) / (1024*1024) = 64 pages with l1 table
 */
#define RAM_LEN (1024 * 1024 * 64)
#define RAM_START (0x20000000)
#define RAM_END (RAM_START + RAM_LEN)

#define L1_PT_SZ (4096)

#define PAGE_SIZE (1024 * 1024)

#define PAGE(x) ((x) << 20)
#define FRAME(x) (RAM_START + (PAGE(x)))

// 32 kb stack size per task
#define STACK_SZ (1024 * 32)

#define KERNEL_TEXT_PAGE (FRAME(2))
#define KERNEL_DATA_PAGE (FRAME(3))
#define KERNEL_STACK_PAGE (FRAME(63))
#define KERNEL_STACK (FRAME(64))

#define IDLE_STACK_PAGE (FRAME(62))
#define IDLE_STACK (FRAME(63))

#define USER_TEXT_PAGE (FRAME(0))
#define USER_DATA_PAGE (FRAME(1))

/**
 * every process has a stack and a data section located next to each other
 */
/* first user pid = 1 */
#define USER_STACK_FRAME(x) (FRAME(63 - (x * 2)))
#define TASK_DATA_FRAME(x) (FRAME(62 - (x * 2)))

#define TASK_DATA_PAGE (FRAME(59))

#define USER_STACK_PAGE (FRAME(60))

#define USER_STACK(x) ((FRAME(61)) - (x) * (STACK_SZ))

#define PERIPH_PAGE (0xFFFFF000)

#endif /* MEM_LAYOUT_H */