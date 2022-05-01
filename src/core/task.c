#include <task.h>
#include <mem.h>
#include <cpu.h>
#include <no.h>
#include <sched.h>
#include <syscall.h>
#include <userlib.h>
#include <mmu.h>

static int task_create(task_t* t, task_func_t fp, void* arg, task_prio_t prio, psr_mode_t psr);

static void _init_pt(task_t* t)
{
    mmu_pt_init(t->pt);
    mmu_map(t->pt, USER_STACK_PAGE, USER_STACK_FRAME(t->tid), RW_USR);

    mmu_map(t->pt, TASK_DATA_PAGE, TASK_DATA_FRAME(t->tid), RW_USR);
}

static void _init_pt_idle(task_t* t)
{
    mmu_pt_init(t->pt);
    mmu_map(t->pt, IDLE_STACK_PAGE, IDLE_STACK_PAGE, RW_KER);

    t->sp = (void *)IDLE_STACK;
}

int process_create(task_func_t fp, void* arg, task_prio_t prio, psr_mode_t psr)
{
    task_t* t;

    int pid = sched_task_add(&t);

    if (pid < 0) {
        return pid;
    }

    t->tid = t->pid = pid;
    t->pt = mmu_get_pt();

    if (t->pt == NULL) {
        puts("Max amount of processes reached");
        return ERR;
    }

    t->sp = (void *)USER_STACK(0) - 0x4;

    /* idle task is the only task running in sys mode */
    if (psr == PSR_SYS) {
        _init_pt_idle(t);
    }
    else {
        _init_pt(t);
    }

    //printf("Process: pid: %u, tid: %u, sp: %p \n", t->pid, t->tid, t->sp);

    return task_create(t, fp, arg, prio, psr);
}

int thread_create(task_func_t fp, void* arg)
{
    task_t* parent = sched_get_active();
    task_t* child;
    int children_count = list_count(&parent->children);

    if (children_count >= MAX_THREADS) {
        puts("Max amount of threads reached");
        return ERR;
    }

    int pid = sched_task_add(&child);

    if (pid < 0) {
        return ERR;
    }

    child->pid = pid;
    child->tid = parent->pid;
    child->pt = parent->pt;
    child->parent = parent;
    child->sp = (void *)USER_STACK(children_count + 1);

    list_rpush(&parent->children, &child->cl_entry);

    //printf("Thread: pid: %u, tid: %u, data frame: %p \n", child->pid, child->tid, TASK_DATA_FRAME(child->tid));

    return task_create(child, fp, arg, parent->priority, parent->psr);
}

int task_create(task_t* t, task_func_t fp, void* arg, task_prio_t prio, psr_mode_t psr)
{
    t->psr = psr;
    t->priority = prio;
    t->status = STATUS_STOPPED;
    t->mode = psr;

    *(uint32_t*)t->sp = (uint32_t)arg;

    t->regs[0] = *(uint32_t*)t->sp;

    for (int i = 1; i <= 12; i++) {
        t->regs[i] = 0x0;
    }
    t->regs[13] = (uint32_t)t->sp;
    t->regs[14] = (uint32_t)exit;
    t->regs[15] = (uint32_t)fp;

    sched_set_status(t, STATUS_WAITING);

    return OK;
}

int task_sleep(uint32_t ms)
{
    task_t* task = sched_get_active();

    if (task == NULL) {
        return -1;
    }

    sched_set_status(task, STATUS_SLEEPING);

    st_timer_sleep(&task->timer, ms, task_wakeup, task->pid);

    // trigger reschedule
    sched_req_reschedule();

    return 0;
}

void task_wakeup(int pid)
{
    task_t* task = sched_get_task(pid);

    if (task == NULL) {
        return;
    }

    sched_wakeup_task(task);
}
