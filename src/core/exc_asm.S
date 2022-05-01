
.section .exception_vector

// https://github.com/ARM-software/u-boot/blob/master/arch/arm/lib/vectors.S
exc_vec_start:
  .word	0xE7F000F0		/* reset -> triggers undef instructions */
  ldr pc, jt_undef
  ldr	pc, jt_swi
  ldr	pc, jt_pref_abort
  ldr	pc, jt_data_abort
  .word	0xE7F000F0		/* reset -> triggers undef instructions */
  ldr	pc, jt_irq
  .word	0xE7F000F0		/* reset -> triggers undef instructions */

jt_undef:               .word undef_tramp
jt_swi:	                .word swi_tramp
jt_pref_abort:	        .word pref_abort_tramp
jt_data_abort:		      .word data_abort_tramp
jt_irq:			            .word irq_tramp

;http://osnet.cs.nchu.edu.tw/powpoint/Embedded94_1/Chapter%207%20ARM%20Exceptions.pdf (slide 28)

.macro	TRAMPOLINE name lr_offset id handler needs_regs needs_id
  \name:
  .if \lr_offset
 	sub	lr, lr, #(\lr_offset)
  .endif
 	stmfd	sp!, {lr}

  stmfd sp, {r0-r14}^
  sub sp, #(15*4)

  .if \needs_regs
  	mov	r0, sp
  .endif
  .if \needs_id
  	mov	r1, #(\id)
  .endif
 	bl	\handler

  mov	r0, sp
 	bl	sched_run

  ldmia sp, {r0-r14}^
  add sp, sp, #(15 * 4)

  ldmia sp!, {pc}^

.endm

.section .text

/* no reset */
TRAMPOLINE undef_tramp	4 1 _exc_fault	 1 1
TRAMPOLINE swi_tramp	0 2 _exc_swi 1 0
TRAMPOLINE pref_abort_tramp	4 3 _exc_fault	 1 1
TRAMPOLINE data_abort_tramp	8 4 _exc_fault	 1 1
/* no reserved */
TRAMPOLINE irq_tramp 4 6 _exc_irq 0 0
/* no fast interrupt */