#ifndef SCHED_H
#define SCHED_H

#include <no.h>
#include <task.h>

#define SCHED_MAX_TASKS MAX_PROCESSES * (MAX_THREADS + 1)

#define SCHED_PRIO_LEVELS 0x2

// schedule every 500ms
#define SCHED_TIMESLICE_MS 500

void sched_init(void);
int sched_task_add(task_t** task);
void sched_run(uint32_t regs[0x10]);
task_t* sched_get_task(int pid);
void sched_set_status(task_t* task, task_status_t status);
void sched_task_exit(void);
void sched_kill_active_task(void);
void sched_req_reschedule(void);
task_t* sched_get_active(void);
void sched_wakeup_task(task_t* task);

#endif /* SCHED_H */