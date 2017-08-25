/*
 * (C) Copyright 2014
 * Sunplus Technology
 *
 * SPDX-License-Identifier:     GPL-2.0+
 */

#include <common.h>
#include <linux/clk.h>
#include <linux/clkdev.h>
#include <asm/io.h>
#include <asm/arch/regmap.h>

#ifndef BIT
#define BIT(n)		(1 << (n))
#endif

DECLARE_GLOBAL_DATA_PTR;

/* u-boot uses G99 STC_AV2 because G12 STC is used by eCos! */
static volatile struct stc_regs *g_regs = STC_AV2_REG;

// With 1MHz STC, one tick = 1us. Wrap 32 bits in 15s
// With 90KHz STC, one tick ~= 11.1us. Wrap 32 bits in 47721s

void udelay_masked(unsigned long usec)
{
	ulong freq = gd->arch.timer_rate_hz;
	ulong secs = 0xffffffff / freq;
	ulong wait, tick, timeout;

	wait = (freq >= 1000000) ? usec * (freq / 1000000) :
		(usec * (freq / 10000)) / 100;

	if (!wait) {
		wait = 1;
	}

	writel(0x1234, &g_regs->stcl_2);
	tick = (readl(&g_regs->stcl_1) << 16) | readl(&g_regs->stcl_0);

	/* restart timer if counter is going to overflow */
	if (secs * freq - tick < wait) {
		gd->arch.tbl += tick / (freq / CONFIG_SYS_HZ);
		writel(0, &g_regs->stc_15_0);
		writel(0, &g_regs->stc_31_16);
		writel(0, &g_regs->stc_64);
		tick = 0;
	}

	/* now we wait ... */
	timeout = tick + wait;
	do {
		writel(0x1234, &g_regs->stcl_2);
		tick = (readl(&g_regs->stcl_1) << 16) | readl(&g_regs->stcl_0);
	} while (timeout > tick);
}

ulong get_timer_masked(void)
{
	ulong freq = gd->arch.timer_rate_hz;
	ulong secs = 0xffffffff / freq;
	ulong tick;

	writel(0x1234, &g_regs->stcl_2);
	tick = (readl(&g_regs->stcl_1) << 16) | readl(&g_regs->stcl_0);

	if (tick >= secs * freq) {
		tick = 0;
		gd->arch.tbl += secs * CONFIG_SYS_HZ;
		writel(0, &g_regs->stc_15_0);
		writel(0, &g_regs->stc_31_16);
		writel(0, &g_regs->stc_64);
	}

	return gd->arch.tbl + (tick / (freq / CONFIG_SYS_HZ));
}

void reset_timer_masked(void)
{
	writel(0, &g_regs->stc_64);
}

int timer_init(void)
{
	uint32_t hz, clk, divxclk;

	/*
	 * Below setup in u-boot:
	 *
	 *  SYSSLOW (270MHz)
	 *    \-> G99 STC_AV2_Counter            ( 1MHz) = u-boot get_timer()
	 *
	 * -----------------------------------
	 *
	 * Below setup in eCos:
	 *
	 *  DIVXCLK (13.5MHz)
	 *    \-> G12 STC_Timer2                 ( 1KHz) = eCos sched clock
	 *    \-> G12 STC_Counter                (90KHz) = eCos GL_GetStc32()
	 *          \-> G12 STC_RTC              (90KHz) = eCos GL_GetUsrRtc()
	 *
	 * -----------------------------------
	 *
	 * Below setup in Linux:
	 *
	 *  SYSSLOW (270MHz)
	 *    \-> G99 STC_AV2_Counter           (270MHz) = Linux clocksource
	 *    \-> G99 STC_AV2_Timer3            (270MHz) = Linux clock_event/hrtimer
	 */

	/* G99 STC_AV2 */
	clk = clk_get_rate(clk_get_sys(NULL, "TMR")); /* SYSSLOW 270MHz */
	divxclk = 0;
	hz = 1000000;
	g_regs->stc_divisor = (divxclk << 15) | ((clk / hz) - 1); /* STC_Counter = 1MHz */
	gd->arch.timer_rate_hz = hz;

	gd->arch.tbl = 0;
	reset_timer_masked();

	/* G12 STC : below setup in eCos */
	/*
	volatile struct stc_regs *stc_regs     = STC_REG;
	stc_regs->stc_config = 0;
	clk = clk_get_rate(clk_get_sys(NULL, "DIVXCLK"));
	divxclk = 1;
	hz = 90*1000;
	stc_regs->stc_divisor = (divxclk << 15) | ((clk / hz) - 1);
	stc_regs->rtc_divisor = 0;
	*/

	return 0;
}

void reset_timer(void)
{
	reset_timer_masked();
}

ulong get_timer(ulong base)
{
	return get_timer_masked() - base;
}

void __udelay(unsigned long usec)
{
	udelay_masked(usec);
}

unsigned long long get_ticks(void)
{
	unsigned long long ret;

	writel(0x1234, &g_regs->stcl_2);
	ret = (readl(&g_regs->stcl_1) << 16) | readl(&g_regs->stcl_0);
	if (readl(&g_regs->stcl_2) & 1)
		ret |= 0x100000000ull;

	return ret;
}

ulong get_tbclk(void)
{
	return gd->arch.timer_rate_hz;
}
