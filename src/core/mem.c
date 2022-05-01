#include <mem.h>
#include <mem_layout.h>
#include <assert.h>
#include <cpu.h>
#include <no.h>
#include <task.h>
#include <sched.h>
#include <mmu.h>

/**
 * todo: make this smarter to reclaim stack space when process ends
 */
static void* kernel_alloc(size_t sz)
{
    static uint32_t pos = KERNEL_STACK - STACK_SZ;
    uint32_t old_pos = pos;

    assert(sz % 0x8 == 0);

    if (pos - sz <= RAM_START) {
        return NULL;
    }

    pos -= sz;

    return (void*)old_pos;
}

static void* user_alloc(size_t sz)
{
    task_t* t = sched_get_active();
}

void* kmalloc(size_t sz, mapping_t type)
{
    switch (type) {
        case MEM_KER:
            return kernel_alloc(sz);
        case MEM_USER:
            return user_alloc(sz);
        default:
            return NULL;
    }
}
