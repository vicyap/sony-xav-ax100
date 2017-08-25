/*
 *  linux/arch/arm/kernel/irq.c
 *
 *  Copyright (C) 1992 Linus Torvalds
 *  Modifications for ARM processor Copyright (C) 1995-2000 Russell King.
 *
 *  Support for Dynamic Tick Timer Copyright (C) 2004-2005 Nokia Corporation.
 *  Dynamic Tick Timer written by Tony Lindgren <tony@atomide.com> and
 *  Tuukka Tikkanen <tuukka.tikkanen@elektrobit.com>.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 *  This file contains the code used by various IRQ handling routines:
 *  asking for different IRQ's should be done through these routines
 *  instead of just grabbing them. Thus setups with different IRQ numbers
 *  shouldn't result in any weird surprises, and installing new handlers
 *  should be easier.
 *
 *  IRQ's are in fact implemented a bit like signal handlers for the kernel.
 *  Naturally it's not a 1:1 relation, but there are similarities.
 */
#include <linux/kernel_stat.h>
#include <linux/signal.h>
#include <linux/ioport.h>
#include <linux/interrupt.h>
#include <linux/irq.h>
#include <linux/random.h>
#include <linux/smp.h>
#include <linux/init.h>
#include <linux/seq_file.h>
#include <linux/errno.h>
#include <linux/list.h>
#include <linux/kallsyms.h>
#include <linux/proc_fs.h>

#include <asm/exception.h>
#include <asm/mach/arch.h>
#include <asm/mach/irq.h>
#include <asm/mach/time.h>

/*
 * No architecture-specific irq_finish function defined in arm/arch/irqs.h.
 */
#ifndef irq_finish
#define irq_finish(irq) do { } while (0)
#endif

unsigned long irq_err_count;

int arch_show_interrupts(struct seq_file *p, int prec)
{
#ifdef CONFIG_FIQ
	show_fiq_list(p, prec);
#endif
#ifdef CONFIG_SMP
	show_ipi_list(p, prec);
#endif
	seq_printf(p, "%*s: %10lu\n", prec, "Err", irq_err_count);
	return 0;
}

/* IRQ Profiling
 * echo 1 > /sys/module/irq/irq_profiling_enable		-> enable
 * echo 0 > /sys/module/irq/irq_profiling_enable		-> disable and show profiling result
 * echo xxxx > /sys/module/irq/parameters/irq_threshold	-> set isr time-consuming warning threshold (ns), 0 for disable
 */
//#define IRQ_PROFILING

#ifdef IRQ_PROFILING

#include <linux/module.h>
#include <linux/time.h>
#include <linux/clockchips.h>
#include <asm/smp_plat.h>

#define MAX_IRQ_NUM 256

struct irq_time_rec {
	u32 hits;  // hits in all CPU
	u32 hits1; // hits in other CPU (except CPU0)
	struct timespec max;
	struct timespec total;
};

#define IRQ_HISTS		16 // must be 2 ^ n
#define IRQ_HIST_MASK	(IRQ_HISTS - 1)

struct irq_hist_rec {
	int irq;				// irq number
	struct timespec dur;	// time duration
};

struct irq_hist_rec irq_hist[IRQ_HISTS];
u32 irq_hist_idx = 0;
EXPORT_SYMBOL(irq_hist);
EXPORT_SYMBOL(irq_hist_idx);

static struct irq_time_rec tt[MAX_IRQ_NUM];
static u32 irq_profiling = 0;
static u32 irq_profiling_enable = 0;
static u32 irq_threshold = 0; // ns (0 for disable)
module_param(irq_threshold, uint, 0644);

#define print(args...)	do { result += sprintf(buffer + result, args); } while (0)

static ssize_t show_profiling_enable(struct module_attribute *mattr,
			     struct module_kobject *mk, char *buffer)
{
	ssize_t result = 0;

	print("%d\n", irq_profiling);
	if (irq_profiling) {
		int i = 16;
		struct irq_time_rec *t = &tt[i];

		print("=============================== IRQ PROFILING RESULT ===============================\n");
		print(" IRQ   HITS     HITS_1   MAX            TOTAL       NAME & HANDLER\n");
		print("------------------------------------------------------------------------------------\n");
		while (i < MAX_IRQ_NUM) {
			if (t->hits) {
				struct irqaction *act = irq_to_desc(i)->action;
				if (act) {
					print("[%3d]  %-8d %-8d %ld.%09ld %4ld.%09ld \"%s\" %pf()\n",
						i, t->hits, t->hits1, t->max.tv_sec, t->max.tv_nsec, t->total.tv_sec, t->total.tv_nsec,
						act->name, act->handler);
				} else {
					print("[%3d]  %-8d %-8d %ld.%09ld %4ld.%09ld\n",
						i, t->hits, t->hits1, t->max.tv_sec, t->max.tv_nsec, t->total.tv_sec, t->total.tv_nsec);
				}
			}
			i++, t++;
		}
	}

	return result;
}

static ssize_t store_profiling_enable(struct module_attribute *mattr,
			    struct module_kobject *mk,
			    const char *buffer, size_t count)
{
	if (kstrtouint(buffer, 0, &irq_profiling_enable) == 0) {
		if (irq_profiling != irq_profiling_enable) {
			irq_profiling = irq_profiling_enable;
			if (irq_profiling) {
				printk("============================== IRQ PROFILING START ==============================\n");
				memset(tt, 0, sizeof(tt));
			}
		}
	}
	return count;
}

static struct module_attribute profiling_enable =
	__ATTR(irq_profiling_enable, 0644, show_profiling_enable, store_profiling_enable);

static int __init irq_sysfs_init(void)
{
	struct kobject *kobj = kset_find_obj(module_kset, "irq");
	sysfs_create_file(kobj, &profiling_enable.attr);
	return 0;
}
late_initcall(irq_sysfs_init);

#endif

/*
 * handle_IRQ handles all hardware IRQ's.  Decoded IRQs should
 * not come via this function.  Instead, they should provide their
 * own 'handler'.  Used by platform code implementing C-based 1st
 * level decoding.
 */
void handle_IRQ(unsigned int irq, struct pt_regs *regs)
{
	struct pt_regs *old_regs = set_irq_regs(regs);

	irq_enter();

	/*
	 * Some hardware gives randomly wrong interrupts.  Rather
	 * than crashing, do something sensible.
	 */
	if (unlikely(irq >= nr_irqs)) {
		if (printk_ratelimit())
			printk(KERN_WARNING "Bad IRQ%u\n", irq);
		ack_bad_irq(irq);
	} else {
#ifdef IRQ_PROFILING
		struct timespec t0, t1, d;
		getnstimeofday(&t0);
#endif
		generic_handle_irq(irq);
#ifdef IRQ_PROFILING
		getnstimeofday(&t1);
		d = timespec_sub(t1, t0);

		if (irq_threshold && (d.tv_sec || d.tv_nsec > irq_threshold)) {
			struct irqaction *act = irq_to_desc(irq)->action;
			trace_printk("WARN: [%d] ISR time-consuming %ld > %d ns!    \"%s\" %pf()\n",
				irq, d.tv_sec * NSEC_PER_SEC + d.tv_nsec, irq_threshold,
				act->name, act->handler);
		}

		if (irq_profiling) {
			struct irq_time_rec *t = &tt[irq];
			if (timespec_compare(&t->max, &d) < 0) t->max = d;
			t->total = timespec_add(t->total, d);
			t->hits++;
			if (cpu_logical_map(smp_processor_id())) t->hits1++;

			irq_hist[irq_hist_idx].irq = irq;
			irq_hist[irq_hist_idx].dur = d;
			irq_hist_idx = (irq_hist_idx + 1) & IRQ_HIST_MASK;
		}
#endif
	}

	/* AT91 specific workaround */
	irq_finish(irq);

	irq_exit();
	set_irq_regs(old_regs);
}

/*
 * asm_do_IRQ is the interface to be used from assembly code.
 */
asmlinkage void __exception_irq_entry
asm_do_IRQ(unsigned int irq, struct pt_regs *regs)
{
	handle_IRQ(irq, regs);
}

void set_irq_flags(unsigned int irq, unsigned int iflags)
{
	unsigned long clr = 0, set = IRQ_NOREQUEST | IRQ_NOPROBE | IRQ_NOAUTOEN;

	if (irq >= nr_irqs) {
		printk(KERN_ERR "Trying to set irq flags for IRQ%d\n", irq);
		return;
	}

	if (iflags & IRQF_VALID)
		clr |= IRQ_NOREQUEST;
	if (iflags & IRQF_PROBE)
		clr |= IRQ_NOPROBE;
	if (!(iflags & IRQF_NOAUTOEN))
		clr |= IRQ_NOAUTOEN;
	/* Order is clear bits in "clr" then set bits in "set" */
	irq_modify_status(irq, clr, set & ~clr);
}

void __init init_IRQ(void)
{
	machine_desc->init_irq();
}

#ifdef CONFIG_SPARSE_IRQ
int __init arch_probe_nr_irqs(void)
{
	nr_irqs = machine_desc->nr_irqs ? machine_desc->nr_irqs : NR_IRQS;
	return nr_irqs;
}
#endif

#ifdef CONFIG_HOTPLUG_CPU

static bool migrate_one_irq(struct irq_desc *desc)
{
	struct irq_data *d = irq_desc_get_irq_data(desc);
	const struct cpumask *affinity = d->affinity;
	struct irq_chip *c;
	bool ret = false;

	/*
	 * If this is a per-CPU interrupt, or the affinity does not
	 * include this CPU, then we have nothing to do.
	 */
	if (irqd_is_per_cpu(d) || !cpumask_test_cpu(smp_processor_id(), affinity))
		return false;

	if (cpumask_any_and(affinity, cpu_online_mask) >= nr_cpu_ids) {
		affinity = cpu_online_mask;
		ret = true;
	}

	c = irq_data_get_irq_chip(d);
	if (!c->irq_set_affinity)
		pr_debug("IRQ%u: unable to set affinity\n", d->irq);
	else if (c->irq_set_affinity(d, affinity, true) == IRQ_SET_MASK_OK && ret)
		cpumask_copy(d->affinity, affinity);

	return ret;
}

/*
 * The current CPU has been marked offline.  Migrate IRQs off this CPU.
 * If the affinity settings do not allow other CPUs, force them onto any
 * available CPU.
 *
 * Note: we must iterate over all IRQs, whether they have an attached
 * action structure or not, as we need to get chained interrupts too.
 */
void migrate_irqs(void)
{
	unsigned int i;
	struct irq_desc *desc;
	unsigned long flags;

	local_irq_save(flags);

	for_each_irq_desc(i, desc) {
		bool affinity_broken;

		raw_spin_lock(&desc->lock);
		affinity_broken = migrate_one_irq(desc);
		raw_spin_unlock(&desc->lock);

		if (affinity_broken && printk_ratelimit())
			pr_warning("IRQ%u no longer affine to CPU%u\n", i,
				smp_processor_id());
	}

	local_irq_restore(flags);
}
#endif /* CONFIG_HOTPLUG_CPU */
