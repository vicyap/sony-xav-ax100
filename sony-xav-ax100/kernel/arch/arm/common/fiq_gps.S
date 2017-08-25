#include <linux/linkage.h>
#include <asm/assembler.h>

	.text

	.global fiq_gps_end

	@ r8 : GIC cpu_base
	@ r9 : clear_int_ptr
	@ r10: fiq_stat

ENTRY(fiq_gps)
	stmia	sp, {r11-r12}		@ save r11-r12

	ldr		r11, [r8, #0x18]	@ irqstat = *(cpu_base + GIC_CPU_HIGHPRI)
	bic		r11, r11, #0x1c00	@ irqnr = irqstat & ~0x1c00

	cmp		r11, #75
	bne		wrong_fiq

gps_fiq:
	ldr		r11, [r8, #0x0c]	@ ack

	ldr		r12, [r9]			@ *clear_int_ptr = *clear_int_ptr | 0x10000
	orr		r12, r12, #0x10000
	str		r12, [r9]

	ldr		r12, [r10, #0]		@ fiq_count++
	add		r12, r12, #1
	str		r12, [r10, #0]

	str		r11, [r8, #0x10]	@ eoi
	b		fiq_gps_exit

wrong_fiq:
	mov		r12, #1024			@ irqnr >= 1021
	sub		r12, r12, #3
	cmp		r11, r12
	bge		spurious_fiq

	ldr		r12, [r10, #4]		@ wrong_count++
	add		r12, r12, #1
	str		r12, [r10, #4]
	str		r11, [r10, #8]		@ wrong_fiq = irqnr
	b		fiq_gps_exit

spurious_fiq:
	@ldr		r11, [r8, #0x0c]	@ ack spurious fiq

fiq_gps_exit:
	ldmia	sp, {r11-r12}		@ restore r11-r12
	subs	pc, lr, #4

fiq_gps_end:

ENTRY(fiq_gps_setup)
	mrs		r3, cpsr
	msr		cpsr_c, #(FIQ_MODE | PSR_I_BIT | PSR_F_BIT)
	mov		r8, r0					@ GIC cpu_base
	mov		r9, r1					@ clear_int_ptr
	mov		r10, r2					@ fiq_stat
	msr		cpsr_c, r3
	bx		lr
