#include <mem.h>
#include <cpu.h>
#include <no.h>
#include <dbgu.h>
#include <mem_layout.h>
#include <userlib.h>

extern void _set_stack(psr_mode_t mode, uint32_t addr);
extern void _enable_irq(void);
extern void _disable_irq(void);
extern psr_t _get_cpsr(void);
extern psr_t _get_spsr(void);
extern psr_t _set_spsr(psr_t psr);
extern void _get_banked_sp_lr(psr_mode_t mode, uint32_t regs[2]);
extern psr_t _get_banked_spsr(psr_mode_t mode);
extern NORETURN void _idle(void);
extern int calc_store_pc_offset(void);

static void _print_ctx(uint32_t regs[16], psr_t cpsr, psr_t spsr);
static const char *_get_psr_mode_name(psr_mode_t mode);
static void _print_banked_registers(void);
static void _print_registers(uint32_t regs[16]);

// ARM reference manual A2.2
void cpu_init_exception_stacks(void)
{
    // dont set stack for supervisor mode (because we are in it)
    // supervisor mode stack is at RAM_END
    _set_stack(PSR_FIQ, (uint32_t)kmalloc(STACK_SZ, MEM_KER));
    _set_stack(PSR_IRQ, (uint32_t)kmalloc(STACK_SZ, MEM_KER));
    _set_stack(PSR_ABT, (uint32_t)kmalloc(STACK_SZ, MEM_KER));
    _set_stack(PSR_UND, (uint32_t)kmalloc(STACK_SZ, MEM_KER));

	/*
 	 * User- und Systemmodus teilen sich den Stack (siehe ARM Architecture
 	 * Reference Manual, A2.2ff)
 	 */
    _set_stack(PSR_SYS, (uint32_t)kmalloc(STACK_SZ, MEM_KER));
}

void cpu_irq_enable(void)
{
    _enable_irq();
}

void cpu_irq_disable(void)
{
    _disable_irq();
}

void cpu_set_spsr(psr_t psr)
{
	_set_spsr(psr);
}

psr_t cpu_get_spsr(void)
{
	return _get_spsr();
}

static const char *_get_psr_mode_name(psr_mode_t mode)
{
 	switch(mode) {
 	case PSR_USR:
 		return "User";
 	case PSR_FIQ:
 		return "FIQ";
 	case PSR_IRQ:
 		return "IRQ";
 	case PSR_SVC:
 		return "Supervisor";
 	case PSR_ABT:
 		return "Abort";
 	case PSR_UND:
 		return "Undefined";
 	case PSR_SYS:
 		return "System";
 	}
 	return "<unknown>";
}

static void _print_ctx(uint32_t regs[16], psr_t cpsr, psr_t spsr)
{
	psr_mode_t cmode = cpsr & PSR_MODE;

 	_print_registers(regs);
 	printf("CPSR: ");
 	cpu_print_psr(cpsr);
 	if (cmode != PSR_USR && cmode != PSR_SYS) {
 		printf("\nSPSR: ");
 		cpu_print_psr(spsr);
 	}
 	printf("\n");
}

static void _print_banked_registers(void)
{
 	uint32_t regs[6][2];

 	_get_banked_sp_lr(PSR_SVC, regs[0]);
 	_get_banked_sp_lr(PSR_FIQ, regs[1]);
 	_get_banked_sp_lr(PSR_IRQ, regs[2]);
 	_get_banked_sp_lr(PSR_ABT, regs[3]);
 	_get_banked_sp_lr(PSR_UND, regs[4]);
 	_get_banked_sp_lr(PSR_SYS, regs[5]);

 	printf(">>> mode specific registers (except SPSR and R8-R12) <<<\n\n"
 	       "                 SP           LR\n"
 	       "Supervisor:  %p   %p\n"
 	       "FIQ:         %p   %p\n"
 	       "IRQ:         %p   %p\n"
 	       "Abort:       %p   %p\n"
 	       "Undefined:   %p   %p\n"
 	       "User/System: %p   %p\n\n",
 	       (void *)regs[0][0], (void *)regs[0][1],
 	       (void *)regs[1][0], (void *)regs[1][1],
 	       (void *)regs[2][0], (void *)regs[2][1],
 	       (void *)regs[3][0], (void *)regs[3][1],
 	       (void *)regs[4][0], (void *)regs[4][1],
 	       (void *)regs[5][0], (void *)regs[5][1]);
}

void cpu_print_psr(psr_t psr)
{
  	psr_mode_t mode = psr & PSR_MODE;

 	printf("%c%c%c%c %c%c%c %s",
 	       (psr & PSR_N) ? 'N' : '_',
 	       (psr & PSR_Z) ? 'Z' : '_',
 	       (psr & PSR_C) ? 'C' : '_',
 	       (psr & PSR_V) ? 'V' : '_',
 	       (psr & PSR_I) ? 'I' : '_',
 	       (psr & PSR_F) ? 'F' : '_',
 	       (psr & PSR_T) ? 'T' : '_',
 	       _get_psr_mode_name(mode));
}

static void _print_registers(uint32_t regs[16])
{
 	printf(">>> register snapshot (current mode, at %p) <<<\n\n"
 	       "R0:  %p    R4:  %p    R8:  %p    R12: %p\n"
 	       "R1:  %p    R5:  %p    R9:  %p    SP:  %p\n"
 	       "R2:  %p    R6:  %p    R10: %p    LR:  %p\n"
 	       "R3:  %p    R7:  %p    R11: %p    PC:  %p\n\n",
 	       (void *)(regs[15] - calc_store_pc_offset()),
 	       (void *)regs[0], (void *)regs[4], (void *)regs[8],  (void *)regs[12],
 	       (void *)regs[1], (void *)regs[5], (void *)regs[9],  (void *)regs[13],
 	       (void *)regs[2], (void *)regs[6], (void *)regs[10], (void *)regs[14],
 	       (void *)regs[3], (void *)regs[7], (void *)regs[11], (void *)regs[15]);
}

/*
 * print_debug_info() - aktuellen Systemzustand ausgeben
 *
 * @regs: gesicherte General-Purpose-Register des aktuellen Modus
 *
 * Nicht ganz perfekt, aber ein guter Anfang.
 */
void cpu_print_debug_info(uint32_t regs[16])
{
  	psr_t cpsr = _get_cpsr();
  	psr_mode_t cmode = cpsr & PSR_MODE;

 	/* normale Register */
 	_print_registers(regs);

 	/* Statusregister im aktuellen Modus */
 	printf(">>> current status register (SPSR of current mode) <<<\n\n"
 	       "CPSR: ");
 	cpu_print_psr(cpsr);
 	if (cmode != PSR_USR && cmode != PSR_SYS) {
 		/* SPSR ist nur in Ausnahmemodi verfügbar */
  		psr_t spsr = _get_spsr();
  		psr_mode_t smode = spsr & PSR_MODE;

 		printf("\nSPSR: ");
 		cpu_print_psr(spsr);

 		if (smode == cmode) {
 			printf("\n\n   ==> Exception aus demselben CPU-Modus, urspruengliches\n"
 			       "       LR und SPSR sind verloren gegangen!");
 		}
 	}
 	printf("\n\n");

 	/* ggf. modusspezifische Register (außer SPSR und R8-R12) */
 	if (cmode != PSR_USR)
 		_print_banked_registers();
 	else
 		printf("User-Modus => keine modusspezifischen Register abrufbar!\n\n");
}

void cpu_print_exception_info(uint32_t regs[16])
{
  	psr_t cpsr = _get_spsr();
  	psr_mode_t cmode = cpsr & PSR_MODE;
  	psr_t spsr = _get_banked_spsr(cmode);
  	psr_mode_t smode = spsr & PSR_MODE;

 	/* Registersatz reparieren */
 	_get_banked_sp_lr(cmode, &regs[13]);

 	_print_ctx(regs, cpsr, spsr);

 	if (smode == cmode)
 		printf("\n==> Exception aus demselben CPU-Modus: urspruengliches\n"
 		       "    LR und SPSR sind verloren gegangen!\n");
 	if (smode == PSR_FIQ && cmode != PSR_FIQ)
 		printf("\n==> Exception in FIQ-Modus: beim Kontext stimmen R8-R12 nicht!\n");

 	printf("\n");

 	_print_banked_registers();
}

void cpu_print_task_ctx(uint32_t regs[16], psr_t cpsr)
{
	printf("*** Task ctx ***\n");
	_print_ctx(regs, cpsr, 0);
}

void cpu_idle(void)
{
	_idle();
}