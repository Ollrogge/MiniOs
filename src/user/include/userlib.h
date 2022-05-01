#ifndef USERLIB_H
#define USERLIB_H

#include <task.h>

int exit(int status);
int fork(task_func_t fp, void* arg);
int child_create(task_func_t fp, void* arg);
int readc(void);
int writec(char c);
int read(void* buf, size_t len);
int write(const void *buf, size_t len);
int sleep(uint32_t ms);
void kprint_num(uint32_t val, unsigned base, bool pad);
int handle_call(uint8_t num, const void* args);

// TODO: fix this naming problen
void puts2(const char *str);
size_t strlen2(const char *str);
void printf2(char* fmt, ...);

#endif
