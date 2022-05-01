#include <aic.h>

static volatile aic_t* const aic = (aic_t *)AIC_BASE;

void aic_irq_enable(irq_t type)
{
    /* activate irq */
    aic->iecr = bit_set(aic->iecr, type);

    /* priority 0, level sensitive */
 	aic->smr[type] = 0;
}
