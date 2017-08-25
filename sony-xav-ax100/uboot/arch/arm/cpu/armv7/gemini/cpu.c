/*
 * (C) Copyright 2014
 * Sunplus Technology
 * Kuo-Jung Su <dante.su@sunplus.com>
 *
 * SPDX-License-Identifier:     GPL-2.0+
 */

#include <common.h>
#include <linux/clk.h>
#include <linux/clkdev.h>
#include <asm/io.h>
#include <asm/arch/hardware.h>
#include <asm/arch/regmap.h>
#include "core.h"

//unsigned int iop_bp_value;

#ifdef CONFIG_ARCH_MISC_INIT
int arch_misc_init()
{

    // Save flags stored in iop_bp (IOP breakpoint). 
    // The value will be reset during IOP initialization latter.
    // Bitmap: 
    //   http://wiki2.sunplus.com/sunplus/index.php/letb123/IOP_BP_User_List
    //iop_bp_value = regs0->iop_bp;

    return 0;
}
#endif /* CONFIG_ARCH_MISC_INIT */

void lowlevel_init(void)
{
}

int arch_cpu_init(void)
{
	clk_init();
	return 0;
}

int print_cpuinfo(void)
{
	char name[32];
	uint32_t cid, pid;

	__asm__ __volatile__ (
		"mrc p15, 0, %0, c0, c0, 0\n"
		: "=r"(cid) /* output */
		: /* input */
		: "memory" /* clobber list */
	);

	pid = (cid >>  4) & 0x0fff;

	if ((pid & 0xff0) == 0xc00)
		sprintf(name, "Cortex-A%u", (pid & 0x00f));
	else if (pid >= 0xa00)
		sprintf(name, "ARM%x", 0x1000 + (pid - 0xa00));
	else
		sprintf(name, "ARM%x", pid);

	/* print cpu_info */
	printf("CPU:     %s\n", name);
	printf("SYS:     %lu MHz\n",
		clk_get_rate(clk_get_sys(NULL, "SYS")) / 1000000);
	printf("SYSSLOW: %lu MHz\n",
		clk_get_rate(clk_get_sys(NULL, "SYSSLOW")) / 1000000);
	return 0;
}

void reset_cpu(ulong addr)
{
	static volatile struct moon1_regs *moon1 = MOON1_REG;
	static volatile struct stc_regs   *stc   = STC_REG;

	/* MOON1: enable watchdog reset */
#ifdef CONFIG_PLATFORM_letb123
	writel(BIT(10) | BIT(1), &moon1->sft_cfg[8]);
#else
	writel(BIT(10) | BIT(1), &moon1->sft_cfg[10]);
#endif

	/* STC: watchdog control */
	writel(0x3877, &stc->timerw_ctrl); /* stop */
	writel(0xAB00, &stc->timerw_ctrl); /* unlock */
	writel(0x0001, &stc->timerw_cnt);  /* counter */
	writel(0x4A4B, &stc->timerw_ctrl); /* resume */
}

#ifndef CONFIG_SYS_DCACHE_OFF
void enable_caches(void)
{
	/* Enable D-cache. I-cache is already enabled in start.S */
	dcache_enable();
}

void dram_bank_mmu_setup(int bank)
{
	int	i;

	if (bank)
		return;

	/* 0x00000000 - 0x3fffffff: 1 GBytes */
	for (i = 0; i < 1024; i++) {
#if defined(CONFIG_SYS_ARM_CACHE_WRITETHROUGH)
		set_section_dcache(i, DCACHE_WRITETHROUGH);
#else
		set_section_dcache(i, DCACHE_WRITEBACK);
#endif
	}
}

unsigned long do_go_exec(ulong (*entry)(int, char * const []),
		int argc, char * const argv[])
{
	cleanup_before_linux();
	return entry (argc, argv);
}

void arch_preboot_os(void)
{
	cleanup_before_linux();
}


#if !defined(CONFIG_SYS_L2CACHE_OFF)

void v7_outer_cache_enable(void)
{
    u32 value;
    
	value = *(volatile u32 *)(ADDRESS_PL310 + 0x104);	// reg1_aux_control
	value |=  (1 << 30);    // Early BRESP
	value &= ~(1 << 25);    // pseudo random
	value |=  (1 <<  0);    // Enable full line of zero
	*(volatile u32 *)(ADDRESS_PL310 + 0x104) = value;

	asm volatile("mrc p15, 0, %0, c1, c0, 1" : "=r" (value));   // aux control register
	value |=  (1 << 3);     // enable write full line of zero
	value |=  (1 << 2);     // enable L1 prefetch
	value |=  (1 << 1);     // enable L2 prefetch hint
	asm volatile("mcr p15, 0, %0, c1, c0, 1" : : "r"(value));

	value = *(volatile u32 *)(ADDRESS_PL310 + 0xf60);	// reg15_prefetch_ctrl
	value |=  (1 << 30);    // Double linefill
	value |=  (1 << 29);    // Instruction prefetch enable
	value |=  (1 << 28);    // Data prefetch enable
	value |=  (1 << 27);    // Double linefill on WRAP read disable
	value |=  (1 << 23);    // Incr double linefill enable
	value |=  (1 << 24);    // Prefetch drop enable
	value &= ~(0x1f << 0);  // Prefetch offset
	// value |=  (31 << 0);    // Prefetch offset
	value |=  ( 7 << 0);    // Prefetch offset
	// value |=  ( 3 << 0);    // Prefetch offset
	// value |=  ( 0 << 0);    // Prefetch offset
	*(volatile u32 *)(ADDRESS_PL310 + 0xf60) = value;

	value = *(volatile u32 *)(ADDRESS_PL310 + 0xf80);	// reg15_power_ctrl
	value |=  (1 <<  1);    // dynamic_clk_gating_en
	value |=  (1 <<  0);    // standby_mode_en
	*(volatile u32 *)(ADDRESS_PL310 + 0xf80) = value;

	*(volatile u32 *)(ADDRESS_PL310 + 0x108) = 0;       // reg1_tag_ram_control, recommanded by HW designer
	*(volatile u32 *)(ADDRESS_PL310 + 0x10c) = 0x111;   // reg1_data_ram_control, recommanded by HW designer

	*(volatile u32 *)(ADDRESS_PL310 + 0x100) = 1;		// reg1_control
}

void v7_outer_cache_disable(void)
{
	*(volatile u32 *)(ADDRESS_PL310 + 0x100) = 0;		// reg1_control
}
#endif /* !CONFIG_SYS_L2CACHE_OFF */

#endif
