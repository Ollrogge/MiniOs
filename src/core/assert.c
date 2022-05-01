#include <assert.h>
#include <no.h>
#include <panic.h>

const char assert_crash_message[] = "FAILED ASSERTION.";

void assert_failure(const char *file, unsigned line)
{
    printf("%s:%u => ", file, line);
    panic(PANIC_ASSERT_FAIL, assert_crash_message);
}