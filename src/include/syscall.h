#ifndef SYSCALL_H
#define SYSCALL_H

#include <no.h>
#include <task.h>

int sys_exit(int status);
int sys_fork(task_func_t fp, void* arg);
int sys_read(void* buf, size_t len);
int sys_write(const void *buf, size_t len);
int sys_sleep(uint32_t ms);
int sys_handle_call(uint8_t num, const void* args);

#endif /* SYSCALL_H */