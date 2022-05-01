#ifndef PANIC_H
#define PANIC_H

typedef enum {
    PANIC_ASSERT_FAIL,
} panic_t;

void panic(panic_t crash_code, const char *message);

#endif