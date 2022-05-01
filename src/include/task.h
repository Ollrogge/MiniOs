#ifndef TASK_H
#define TASK_H

#include <no.h>
#include <list.h>
#include <cpu.h>
#include <st.h>
#include <mem_layout.h>

typedef enum {
    PRIO_IDLE,
    PRIO_NORMAL
} task_prio_t;

typedef enum {
    STATUS_STOPPED,                 /**< has terminated                           */
    STATUS_ZOMBIE,                  /**< has terminated & keeps thread's thread_t */
    STATUS_SLEEPING,                /**< sleeping                                 */
    STATUS_BLOCKED,
    STATUS_WAITING,                 /**< waiting to be scheduled to run           */
    STATUS_RUNNING,                 /**< currently running                        */
} task_status_t;

// task struct forward declaration
typedef struct task task_t;

struct task {
    void *sp;
    uint16_t pid;
    uint16_t tid;
    task_status_t status;
    list_node_t rq_entry; // run queue entry
    list_node_t children; // children of process (threads)
    list_node_t cl_entry; // childrent list entry
    task_t* parent; // process if task is a thread
    task_prio_t priority;
    psr_mode_t psr;
    uint32_t regs[0x10];
    st_timer_t timer; // timer for e.g. sleeping
    uint32_t* pt; // page table
    psr_mode_t mode;
};

#define STATUS_ON_RUNQUEUE STATUS_WAITING

#define MAX_PROCESSES 0xa
/**
 * STACK_SZ is 32KiB, USER_STACK_PAGE maps 1MiB
 * => a process can have max 31 threads for now
 */
#define MAX_THREADS (PAGE_SIZE / STACK_SZ - 1)

typedef void *(*task_func_t)(void *arg);

int process_create(task_func_t fp, void* arg, task_prio_t prio, psr_mode_t psr);
int thread_create(task_func_t fp, void* arg);
void task_load_context(task_t *task);
void task_save_context(task_t *task);
int task_sleep(uint32_t ms);
void task_wakeup(int pid);

#endif /* TASK_H */