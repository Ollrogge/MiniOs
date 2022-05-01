#include <panic.h>
#include <no.h>
#include <exc.h>
#include <cpu.h>

NORETURN void panic(panic_t crash_code, const char *message)
{
    (void)crash_code;
    cpu_irq_disable();
    printf("Panic: %s \n", message);
    stop_system();
}