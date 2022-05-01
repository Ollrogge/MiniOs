#include <sched.h>
#include <task.h>
#include <list.h>
#include <cpu.h>
#include <st.h>
#include <mem.h>
#include <mem_layout.h>
#include <mmu.h>

static task_t *_ptasks[SCHED_MAX_TASKS];
static task_t _tasks[SCHED_MAX_TASKS];
static list_node_t _runqueues[SCHED_PRIO_LEVELS];

/*
todo:
- implement waiting queue for sleep
-
*/

static uint32_t _runqueue_bitcache = 0;
volatile task_t* sched_active_task = NULL;

static bool _reschedule;

static void _set_runqueue_bit(task_t *task);
static void _clear_runqueue_bit(task_t *task);
static unsigned _get_prio_queue_from_runqueue(void);
static void _unschedule_task(task_t* task);
static void* _run_idle_task(void* arg);

static void* _run_idle_task(void* arg)
{
    (void)arg;

    cpu_idle();

    return NULL;
}

void sched_req_reschedule(void)
{
    _reschedule = true;
}

static void _set_runqueue_bit(task_t *task)
{
    _runqueue_bitcache = bit_set(_runqueue_bitcache, task->priority);
}

static void _clear_runqueue_bit(task_t *task)
{
    _runqueue_bitcache = bit_clear(_runqueue_bitcache, task->priority);
}

task_t* sched_get_active(void)
{
    return (task_t*)sched_active_task;
}

task_t* sched_get_task(int pid)
{
    return &_tasks[pid];
}

static unsigned _get_prio_queue_from_runqueue(void)
{
    // get highest priority queue with atleast 1 task waiting
    return bit_msb(_runqueue_bitcache);
}

void sched_init(void)
{
    process_create(_run_idle_task, NULL, PRIO_IDLE, PSR_SYS);
    st_set_pit_interval(SCHED_TIMESLICE_MS);
}

int sched_task_add(task_t** task)
{
    for (int i = 0; i < SCHED_MAX_TASKS; i++) {
        if (_ptasks[i] == NULL) {
            memset(&_tasks[i], 0x00, sizeof(task_t));
            *task = &_tasks[i];
            _ptasks[i] = *task;
            return i;
        }
    }

    return -1;
}

void sched_run(uint32_t regs[0x10])
{
    if (!_reschedule) {
        return;
    }

    task_t* active = sched_get_active();

    unsigned prio = _get_prio_queue_from_runqueue();

    task_t* next_task = container_of(list_lpeek(&_runqueues[prio]), task_t, rq_entry);

    list_lpoprpush(&_runqueues[prio]);
    // don't need to schedule already running task
    if (next_task == active) {
        _reschedule = false;
        return;
    }

    if (active) {
        memcpy(active->regs, regs, sizeof(uint32_t[0x10]));
        _unschedule_task(active);
    }

    next_task->status = STATUS_RUNNING;

    memcpy(regs, next_task->regs, sizeof(uint32_t[0x10]));

    /* update page table */
    mmu_set_ttb(next_task->pt);

    cpu_set_spsr(next_task->psr);
    sched_active_task = next_task;

    _reschedule = false;

    // reset timeslice
    st_set_pit_interval(SCHED_TIMESLICE_MS);
}

void sched_set_status(task_t* task, task_status_t status)
{
    if (status >= STATUS_ON_RUNQUEUE) {
        if (!(task->status >= STATUS_ON_RUNQUEUE)) {
            list_lpush(&_runqueues[task->priority], &task->rq_entry);

            _set_runqueue_bit(task);
        }
    }
    else {
        if (task->status >= STATUS_ON_RUNQUEUE) {
            list_remove(&_runqueues[task->priority], &task->rq_entry);

            if (list_is_empty(&_runqueues[task->priority])) {
                _clear_runqueue_bit(task);
            }
        }
    }

    task->status = status;
}

void sched_kill_active_task(void)
{
    sched_task_exit();
}

void sched_task_exit(void)
{
    task_t* active = sched_get_active();

    _ptasks[active->pid] = NULL;

    sched_set_status(active, STATUS_STOPPED);

    sched_active_task = NULL;

    sched_req_reschedule();
}

static void _unschedule_task(task_t* task)
{
    if (task->status == STATUS_RUNNING) {
        task->status = STATUS_WAITING;
    }
}

void sched_wakeup_task(task_t* task)
{
    if (!(task->status >= STATUS_ON_RUNQUEUE)) {
        // push to start of runqueue => has priority
        list_lpush(&_runqueues[task->priority], &task->rq_entry);
        _set_runqueue_bit(task);

        task->status = STATUS_WAITING;

        sched_req_reschedule();
    }
}


