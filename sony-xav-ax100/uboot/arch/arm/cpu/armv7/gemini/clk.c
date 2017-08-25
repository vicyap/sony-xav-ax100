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
#include <asm/arch/regmap.h>
#include "core.h"


static ulong __clk_get_pllsys(void)
{
	static volatile struct moon0_regs *regs = MOON0_REG;
	return (27 * ((readl(&regs->pllsys_cfg_sel) & 0x3f) + 1) * 1000000) >> 1;
}

static ulong __clk_get_sys(void)
{
	return __clk_get_pllsys() >> 1;
}

static ulong __clk_get_sysslow(void)
{
#if defined(CONFIG_PLATFORM_letb123) || defined(CONFIG_FPGA_NEW)
	return __clk_get_sys();
#else
	return (MOON0_REG->pllsys_cfg_sel & (1 << 30)) ? (308500 * 1000) : (270 * 1000000);
#endif
}

static ulong __clk_get_divxclk(void)
{
	return (27 * 1000 * 1000) / (2 * ((STC_REG->stc_config & 0xff) + 1));
}

static struct clk clk_sys = {
	.ops = {
		.get_rate = __clk_get_sys,
	},
};

static struct clk clk_sysslow = {
	.ops = {
		.get_rate = __clk_get_sysslow,
	},
};

static struct clk clk_divxclk = {
	.ops = {
		.get_rate = __clk_get_divxclk,
	},
};

static struct clk_lookup clk_table[] = {
	CLKDEV_INIT(NULL, "SYS", &clk_sys),
	CLKDEV_INIT(NULL, "TMR", &clk_sysslow),
	CLKDEV_INIT(NULL, "UART", &clk_sysslow),
	CLKDEV_INIT(NULL, "SYSSLOW", &clk_sysslow),
	CLKDEV_INIT(NULL, "DIVXCLK", &clk_divxclk),
};

void clk_init(void)
{
	clkdev_add_table(clk_table, ARRAY_SIZE(clk_table));
}
