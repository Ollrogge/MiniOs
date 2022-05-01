#include <mc.h>

static volatile mc_t * const mc = (mc_t *)MC_BASE;

// section 16.3.3
// After the remap command, the internal SRAM at address
// 0x200000 is mapped into Internal Memory Area 0

void mc_remap(void)
{
    mc->rcr = 1;
}

/*
 * mc_get_abort_address() - letzte Abort-Adresse ermitteln
 *
 * Der Memory-Controller merkt sich welche Adresse warum zu einem Data-Abort
 * geführt hat (siehe AT91RM9200 Handbuch, Kap. 16.3.4). Wir werten hier nur
 * die Adresse selbst aus.
 */
void *mc_get_abort_address(void)
{
 	return (void *)mc->aasr;
}