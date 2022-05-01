#ifndef ASSERT_H
#define ASSERT_H

#include <panic.h>

extern const char assert_crash_message[];

void assert_failure(const char *file, unsigned line);

#define assert(cond) ((cond) ? (void)0 : assert_failure(__FILE__, __LINE__))

#endif /* ASSERT_H */