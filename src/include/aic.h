#ifndef AIC_H
#define AIC_H

#include <exc.h>
#include <no.h>

#define AIC_BASE (0xFFFFF000)
#define IRQ_VEC (INTERNAL_RAM + 0x00000018)

#define AIC_IECR_FIQ (0x0)
#define AIC_IECR_SYS (0x1)

typedef enum {
    FAST_IRQ,
    SYS_IRQ
} irq_t;

// section 22.8
typedef struct
{
    uint32_t smr[0x20];
    uint32_t svr[0x20];
    uint32_t ivr;
    uint32_t fvr;
    uint32_t isr;
    uint32_t ipr;
    uint32_t imr;
    uint32_t cisr;
    uint32_t res1;
    uint32_t res2;
    uint32_t iecr;
    uint32_t idcr;
    uint32_t iccr;
    uint32_t iscr;
    uint32_t eoicr;
    uint32_t spu;
    uint32_t dcr;
    uint32_t res3;
} aic_t;

void aic_irq_enable(irq_t type);

#endif
