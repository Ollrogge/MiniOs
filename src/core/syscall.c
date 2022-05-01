#include <syscall.h>
#include <no.h>
#include <sched.h>
#include <dbgu.h>
#include <task.h>

#include <mem_layout.h>
#include <userlib.h>

extern void _exit(void);
extern void _fork(void);
extern void _read(void);
extern void _write(void);
extern void _sleep(void);
extern void _thread_create(void);
extern void _kprint_num(void);

static uintptr_t syscalls[] = {
    (uintptr_t)_exit,
    (uintptr_t)_fork,
    (uintptr_t)_read,
    (uintptr_t)_write,
    (uintptr_t)_sleep,
    (uintptr_t)_thread_create,
    (uintptr_t)_kprint_num
};

/**
 * @brief Entry point for syscalls after entering kernel
 *
 * @ref args points to the saved user registers and is used by specific
 * assembler stubs defined in syscall_asm.S to move the arguments for the
 * syscalls into the appropriate registers
 *
 * @param num
 * @param args
 */
int sys_handle_call(uint8_t num, const void* args)
{
    (void) args;

    ((void (*)(void))syscalls[num])();
}

int _sys_exit(int status)
{
    (void) status;

    sched_kill_active_task();

    return OK;
}

int _sys_fork(task_func_t fp, void* arg)
{
    return process_create(fp, arg, PRIO_NORMAL, PSR_USR);
}

int _sys_thread_create(task_func_t fp, void* arg)
{
    return thread_create(fp, arg);
}

int _sys_read(void)
{
    int ret = dbgu_getc();

    if (ret > 0) {
        return ret;
    }
    else {
        task_t* task = sched_get_active();
        sched_set_status(task, STATUS_SLEEPING);
        dbgu_waitc(&task->rq_entry);
        sched_req_reschedule();
    }

    return OK;
}

int _sys_write(char c)
{
    return dbgu_putc(c);
}

/* todo: pretty sure sleep with the ALARM interrupt is not very precise atm */
int _sys_sleep(uint32_t ms)
{
    return task_sleep(ms);
}

void _sys_kprint_num(uint32_t val, unsigned base, bool pad)
{
    print_num(val, base, pad);
}
