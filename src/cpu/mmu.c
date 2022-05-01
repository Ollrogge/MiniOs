#include <mmu.h>
#include <mem_layout.h>
#include <sched.h>

extern void _set_ttb(void*);
extern void _invalidate_tlb(void);
extern void _disable_caches(void);
extern void _enable(void);
extern uint32_t _get_cr(void);
extern void _set_cr(uint32_t cr);
extern void _set_dac(uint32_t dac);

/* todo: this alignment wastes a lot of space */
static uint32_t _pts[MMU_MAX_PTS][0x4000] __attribute__((aligned(0x4000)));

/* TTP register holds address in bits [31:14] => address needs to be aligned */
static uint32_t _pt[L1_PT_SZ]  __attribute__((aligned(0x4000)));

static void _fault(uint32_t* pt, uint32_t addr);
static void _section(uint32_t* pt, uint32_t virt, uint32_t phys, ap_t ap);

static void _fault(uint32_t* pt, uint32_t addr)
{
    // L1 PTE has 12 bits ([31:20]) for section base address
    pt[addr >> 20] = MMU_FAULT;
}

static void _section(uint32_t* pt, uint32_t virt, uint32_t phys, ap_t ap)
{
    uint32_t pte = 0;

    pte = phys;
    pte |= MMU_SECTION;
    pte |= ap << 10;

    pt[virt >> 20] = pte;
}

void mmu_init(void)
{
    /* initialize l1 table */
    mmu_pt_init(_pt);

    /* set translation table base (TTB) to l1 table */
    _set_ttb(_pt);

    /**
     * Accesses are checked against the access permission bits in
     * the section or page descriptor
     *
     * ARM920T_TRM1_S.pdf sec 3.6
     */
    _set_dac(DAC_CLI);

    uint32_t cr = _get_cr();

    cr |= (CR_ENABLE | CR_ALIGN_FAULT | CR_SYS_PROTECTION);
    cr &= ~(CR_DCACHE | CR_ICACHE | CR_ROM_PROTECTION);

    _set_cr(cr);
}

void mmu_map(uint32_t* pt, uint32_t virt, uint32_t phys, ap_t ap)
{
    _section(pt, virt, phys, ap);
}

void mmu_pt_init(uint32_t* pt)
{
    for (size_t i = 0; i < L1_PT_SZ; i++) {
        _fault(pt, i << 20);
    }

    mmu_map(pt, 0x0, KERNEL_TEXT_PAGE, RO_KER);

    mmu_map(pt, KERNEL_TEXT_PAGE, KERNEL_TEXT_PAGE, RO_KER);
    mmu_map(pt, KERNEL_DATA_PAGE, KERNEL_DATA_PAGE, RW_KER);
    mmu_map(pt, KERNEL_STACK_PAGE, KERNEL_STACK_PAGE, RW_KER);
    mmu_map(pt, PERIPH_PAGE, PERIPH_PAGE, RW_KER);
    mmu_map(pt, IDLE_STACK_PAGE, IDLE_STACK_PAGE, RW_KER);

    mmu_map(pt, USER_TEXT_PAGE, USER_TEXT_PAGE, RO_USR);
    mmu_map(pt, USER_STACK_PAGE, USER_STACK_PAGE, RW_USR);
    mmu_map(pt, USER_DATA_PAGE, USER_DATA_PAGE, RW_USR);
}

void mmu_set_ttb(void* pt)
{
    _invalidate_tlb();
    _set_ttb(pt);
}

void mmu_set_ker_pt(void)
{
    mmu_set_ttb(_pt);
}

void mmu_update_ker_pt(uint32_t virt, uint32_t phys, ap_t ap)
{
    mmu_map(_pt, virt, phys, ap);
}

uint32_t* mmu_get_pt(void)
{
    static int cnt = 0;

    if (cnt >= MMU_MAX_PTS) {
        return NULL;
    }

    return &_pts[cnt++][0];
}