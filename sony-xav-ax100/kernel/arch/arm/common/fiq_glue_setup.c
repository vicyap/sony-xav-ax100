/*
 * Copyright (C) 2010 Google, Inc.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#include <linux/kernel.h>
#include <linux/percpu.h>
#include <linux/slab.h>
#include <asm/fiq.h>
#include <asm/fiq_glue.h>
#include <asm/hardware/gic.h>
#include <linux/io.h>
#include <linux/irq.h>
#include <linux/module.h>

extern unsigned char fiq_glue, fiq_glue_end;
extern void fiq_glue_setup(void *func, void *data, void *sp);

static struct fiq_handler fiq_debbuger_fiq_handler = {
	.name = "fiq_glue",
};
DEFINE_PER_CPU(void *, fiq_stack);
struct fiq_glue_handler *current_handler;
static DEFINE_MUTEX(fiq_glue_lock);

#define DISABLE_FIQ		0x01 // disable fiq when got wrong FIQ

u32 test_flag;
module_param(test_flag, uint, 0644);

struct pt_regs __old_regs, __new_regs;

static void __fiq_glue_handler(struct fiq_glue_handler *h, void *regs, void *svc_sp)
{
	u32 irqstat, irqnr;
	void __iomem *cpu_base;
	struct pt_regs *old_regs = set_irq_regs(regs);

	if (old_regs) memcpy(&__old_regs, old_regs, sizeof(struct pt_regs));
	memcpy(&__new_regs, regs, sizeof(struct pt_regs));

	//nmi_enter();

	cpu_base = gic_base(GIC_CPU_BASE);
	irqstat = readl_relaxed(cpu_base + GIC_CPU_HIGHPRI);
	irqnr = irqstat & ~0x1c00;

	if (irqnr == FIQ_INT) {
		readl_relaxed(cpu_base + GIC_CPU_INTACK);
		current_handler->isr(irqnr, h);
		writel_relaxed(irqstat, cpu_base + GIC_CPU_EOI);
		fiq_stat.fiq_count++;
	}
	else if (irqnr < 1021) {
		// wrong FIQ, need handled in IRQ
		fiq_stat.wrong_count++;
		fiq_stat.wrong_fiq = irqnr;
		if (test_flag & DISABLE_FIQ)	writel_relaxed(7, cpu_base + GIC_CPU_CTRL); // disable FIQ
	}

	//nmi_exit();

	set_irq_regs(old_regs);
}

static void fiq_glue_setup_helper(void *info)
{
	struct fiq_glue_handler *handler = info;
	fiq_glue_setup(handler->fiq, handler,
		__get_cpu_var(fiq_stack) + THREAD_START_SP);
}

int fiq_glue_register_handler(struct fiq_glue_handler *handler)
{
	int ret;
	int cpu;

	if (!handler || (!handler->fiq && !handler->isr))
		return -EINVAL;

	mutex_lock(&fiq_glue_lock);

	if (!handler->fiq) {
		handler->fiq = __fiq_glue_handler; // use built-in fiq handler
	}

	for_each_possible_cpu(cpu) {
		if (!per_cpu(fiq_stack, cpu)) {
			void *stack;
			stack = (void *)__get_free_pages(GFP_KERNEL, THREAD_SIZE_ORDER);
			if (WARN_ON(!stack)) {
				ret = -ENOMEM;
				goto err_alloc_fiq_stack;
			}
			per_cpu(fiq_stack, cpu) = stack;
		}
	}

	ret = claim_fiq(&fiq_debbuger_fiq_handler);
	if (WARN_ON(ret))
		goto err_claim_fiq;

	current_handler = handler;
	on_each_cpu(fiq_glue_setup_helper, handler, true);
	set_fiq_handler(&fiq_glue, &fiq_glue_end - &fiq_glue);

	mutex_unlock(&fiq_glue_lock);
	return 0;

err_claim_fiq:
err_alloc_fiq_stack:
	for_each_possible_cpu(cpu) {
		__free_pages(per_cpu(fiq_stack, cpu), THREAD_SIZE_ORDER);
		per_cpu(fiq_stack, cpu) = NULL;
	}

	mutex_unlock(&fiq_glue_lock);
	return ret;
}
EXPORT_SYMBOL(fiq_glue_register_handler);

void fiq_glue_unregister_handler(void)
{
	mutex_lock(&fiq_glue_lock);
	release_fiq(&fiq_debbuger_fiq_handler);
	current_handler = NULL;
	mutex_unlock(&fiq_glue_lock);
}
EXPORT_SYMBOL(fiq_glue_unregister_handler);

/**
 * fiq_glue_resume - Restore fiqs after suspend or low power idle states
 *
 * This must be called before calling local_fiq_enable after returning from a
 * power state where the fiq mode registers were lost. If a driver provided
 * a resume hook when it registered the handler it will be called.
 */

void fiq_glue_resume(void)
{
	if (!current_handler)
		return;
	fiq_glue_setup(current_handler->fiq, current_handler,
		__get_cpu_var(fiq_stack) + THREAD_START_SP);
	if (current_handler->resume)
		current_handler->resume(current_handler);
}



extern unsigned char fiq_gps, fiq_gps_end;
extern void fiq_gps_setup(void *, void *, void *);

static struct fiq_handler gps_handler = {
	.name = "fiq_gps",
};

static void fiq_gps_setup_helper(void *clear_int_ptr)
{
	fiq_gps_setup(gic_base(GIC_CPU_BASE), clear_int_ptr, &fiq_stat);
}

int fiq_gps_register(void *clear_int_ptr)
{
	int ret = 0;

	mutex_lock(&fiq_glue_lock);

	ret = claim_fiq(&gps_handler);
	if (WARN_ON(ret))
		goto exit_register;

	on_each_cpu(fiq_gps_setup_helper, clear_int_ptr, true);
	set_fiq_handler(&fiq_gps, &fiq_gps_end - &fiq_gps);

exit_register:
	mutex_unlock(&fiq_glue_lock);
	return ret;
}
EXPORT_SYMBOL(fiq_gps_register);

