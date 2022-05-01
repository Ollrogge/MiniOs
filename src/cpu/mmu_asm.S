
.global _set_ttb
_set_ttb:
    mcr p15, 0, r0, c2, c0, 0
    bx lr

.global _get_cr
_get_cr:
    mrc p15, 0, r0, c1, c0, 0
    bx lr

.global _set_cr
_set_cr:
    mcr p15, 0, r0, c1, c0, 0
    bx lr

.global _set_dac
_set_dac:
    mcr p15, 0, r0, c3, c0, 0
    bx lr

.global _invalidate_tlb
_invalidate_tlb:
    mcr p15, 0, r0, c8, c7, 0
    bx lr