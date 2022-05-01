#ifndef CPU_H
#define CPU_H

/* Prozessor-Statusregister (siehe ARM Architecture Reference Manual, A2.5) */
typedef enum {
 	/* Bedingungsbits */
 	PSR_N = 1 << 31,
 	PSR_Z = 1 << 30,
 	PSR_C = 1 << 29,
 	PSR_V = 1 << 28,

 	/* Kontrollbits */
 	PSR_I = 1 << 7,
 	PSR_F = 1 << 6,
 	PSR_T = 1 << 5,
 	PSR_MODE = 0x1f,
} psr_t;

typedef enum {
 	/* Modusbits aufgeschlüsselt */
 	PSR_USR = 0x10,
 	PSR_FIQ = 0x11,
 	PSR_IRQ = 0x12,
 	PSR_SVC = 0x13,
 	PSR_ABT = 0x17,
 	PSR_UND = 0x1b,
 	PSR_SYS = 0x1f,
} psr_mode_t;

void cpu_init_exception_stacks(void);
void cpu_irq_enable(void);
void cpu_irq_disable(void);
void cpu_set_spsr(psr_t spsr);
psr_t cpu_get_spsr(void);
void cpu_idle(void);

void cpu_print_debug_info(uint32_t regs[16]);
void cpu_print_exception_info(uint32_t regs[16]);
void cpu_print_psr(psr_t psr);
void cpu_print_task_ctx(uint32_t regs[16], psr_t cpsr);

#endif