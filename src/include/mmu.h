#ifndef MMU_H
#define MMU_H

#include <no.h>
#include <task.h>

#define MMU_FAULT 0x00

/**
 * 10010
 * 1CB10
 *
 * B - Bufferable
 * C - Cacheable
 *
 * Set C and B bits to 0 to reduce complexity
 */
#define MMU_SECTION 0x12

// max 8 page tables for now
#define MMU_MAX_PTS MAX_PROCESSES

typedef enum {
    CR_ENABLE		    = 1 << 0,
    CR_ALIGN_FAULT		= 1 << 1,
 	CR_DCACHE		    = 1 << 2,
    CR_ENDIANESS        = 1 << 7,
 	CR_SYS_PROTECTION	= 1 << 8,
 	CR_ROM_PROTECTION	= 1 << 9,
 	CR_ICACHE	        = 1 << 12,
    CR_BASE_ECEPTON     = 1 << 13,
    CR_ROUND_ROBIN      = 1 << 14
} cr_t;

typedef enum {
    DAC_NO_ACC,
    DAC_CLI,
    CAC_RES,
    DAC_MNG
} dac_t;

/* RO_KER is enabled by setting the S-Bit in the control reg */
typedef enum
{
    RO_KER,
    RW_KER,
    RO_USR,
    RW_USR
} ap_t;

void mmu_init(void);
void mmu_set_ker_pt(void);
void mmu_update_ker_pt(uint32_t virt, uint32_t phys, ap_t ap);
void mmu_set_ttb(void* pt);
void mmu_map(uint32_t* pt, uint32_t virt, uint32_t phys, ap_t ap);
void mmu_pt_init(uint32_t* pt);
uint32_t* mmu_get_pt(void);

#endif