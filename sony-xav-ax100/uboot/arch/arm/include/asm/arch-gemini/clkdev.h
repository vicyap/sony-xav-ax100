/*
 * (C) Copyright 2014
 * Sunplus Technology
 * Kuo-Jung Su <dante.su@sunplus.com>
 *
 * SPDX-License-Identifier:     GPL-2.0+
 */

#ifndef __ASM_ARCH_CLKDEV_H
#define __ASM_ARCH_CLKDEV_H

struct sphe_clk_ops {
	int (*enable)(void);
	void (*disable)(void);
	unsigned long (*get_rate)(void);
	long (*round_rate)(unsigned long rate);
	int (*set_rate)(unsigned long rate);
};

struct clk {
	struct sphe_clk_ops ops;
};

#define __clk_get(clk) ({ 1; })
#define __clk_put(clk) do { } while (0)

#endif /* __ASM_ARCH_CLKDEV_H */
