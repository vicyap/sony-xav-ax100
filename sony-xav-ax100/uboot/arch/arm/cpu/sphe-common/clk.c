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

int clk_enable(struct clk *clk)
{
	int ret = 0;

	if (!IS_ERR(clk) && clk->ops.enable)
		ret = clk->ops.enable();

	return ret;
}

void clk_disable(struct clk *clk)
{
	if (!IS_ERR(clk) && clk->ops.disable)
		clk->ops.disable();
}

unsigned long clk_get_rate(struct clk *clk)
{
	unsigned long ret = 0;

	if (!IS_ERR(clk) && clk->ops.get_rate)
		ret = clk->ops.get_rate();

	return ret;
}

long clk_round_rate(struct clk *clk, unsigned long rate)
{
	long ret = -EINVAL;

	if (!IS_ERR(clk) && clk->ops.round_rate)
		ret = clk->ops.round_rate(rate);

	return ret;
}

int clk_set_rate(struct clk *clk, unsigned long rate)
{
	int ret = -EINVAL;

	if (!IS_ERR(clk) && clk->ops.set_rate)
		ret = clk->ops.set_rate(rate);

	return ret;
}

int clk_set_parent(struct clk *clk, struct clk *parent)
{
	return -ENOSYS;
}

struct clk *clk_get_parent(struct clk *clk)
{
	return ERR_PTR(-ENOSYS);
}
