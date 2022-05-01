#ifndef MC_H
#define MC_H

#include <no.h>

#define MC_BASE (0xFFFFFF00)

// section 16.4
typedef struct
{
    uint32_t rcr;
    uint32_t asr;
    uint32_t aasr;
    uint32_t mpr;
} mc_t;

void mc_remap(void);
void *mc_get_abort_address(void);

#endif