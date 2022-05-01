#include <exc.h>
#include <mc.h>
#include <dbgu.h>
#include <no.h>
#include <st.h>
#include <cpu.h>
#include <syscall.h>

typedef enum {
 	E_RESET = 0,
 	E_UNDEF,
 	E_SWI,
 	E_PREFABORT,
 	E_DATAABORT,
 	E_RESERVED,
 	E_IRQ,
 	E_FIQ,
 	E_MAX,
} exception_t;

/* lookup-table for exception-id => name */
static const char *exception_names[] = {
 	"Reset",
 	"Undefined Instruction",
 	"Software Interrupt",
 	"Prefetch Abort",
 	"Data Abort",
 	"Reserved",
 	"Interrupt",
 	"Fast Interrupt",
};

// todo: handle nested interrupts

void _exc_fault(uint32_t regs[16], exception_t e)
{
  	psr_t spsr = cpu_get_spsr();
  	psr_mode_t mode = spsr & PSR_MODE;
  	const char *what;

 	if (mode == PSR_USR) {
 		printf("\n===========================================================================\n\n");
 		what = "user thread";
 	} else {
 		printf("\n###########################################################################\n\n");
 		what = "kernel";
 	}
 	printf("%s crashed!\n\n%s at address %p!\n", what, exception_names[e], (void *)regs[15]);

 	if (e == E_DATAABORT)
 		printf("data abort for address %p.\n\n", mc_get_abort_address());

 	if (e == E_UNDEF || e == E_SWI)
 		printf("swi / undef at address: %p\n\n", (void *)*(unsigned int *)regs[15]);

 	if (mode == PSR_USR) {
 		cpu_print_task_ctx(regs, spsr);
 		printf("\n===========================================================================\n\n");
  		stop_system();
 	} else {
 		cpu_print_exception_info(regs);
 		stop_system();
 	}
}

void _exc_swi(uint32_t regs[16])
{
    psr_mode_t mode = cpu_get_spsr() & PSR_MODE;

	uint8_t num = *(uint32_t *)(regs[15] - 0x4) & 0xff;

	int ret = sys_handle_call(num, regs);

	// move syscall ret into r0 of user registers
	regs[0] = ret;

    if (mode != PSR_USR) {
        regs[15] -= 4;
 		_exc_fault(regs, E_SWI);
    }
}

void _exc_irq(uint32_t regs[16])
{
    (void) regs;
    st_handle_irq();
    dbgu_handle_irq();
}

NORETURN void stop_system()
{
    cpu_irq_disable();
    puts("*** System stopped ***");
    for (;;){}
}
