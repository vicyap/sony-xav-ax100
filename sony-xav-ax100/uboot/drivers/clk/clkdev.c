/*
 * drivers/clk/clkdev.c
 *
 *  Copyright (C) 2008 Russell King.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * Helper for the clk API to assist looking up a struct clk.
 */
#include <common.h>
#include <linux/compiler.h>
#include <linux/list.h>
#include <linux/err.h>
#include <linux/string.h>
#include <linux/clk.h>
#include <linux/clkdev.h>

static LIST_HEAD(clocks);

/*
 * Find the correct struct clk for the device and connection ID.
 * We do slightly fuzzy matching here:
 *  An entry with a NULL ID is assumed to be a wildcard.
 *  If an entry has a device ID, it must match
 *  If an entry has a connection ID, it must match
 * Then we take the most specific entry - with the following
 * order of precedence: dev+con > dev only > con only.
 */
static struct clk_lookup *clk_find(const char *dev_id, const char *con_id)
{
	struct clk_lookup *p, *cl = NULL;
	int match, best_found = 0, best_possible = 0;

	if (dev_id)
		best_possible += 2;
	if (con_id)
		best_possible += 1;

	list_for_each_entry(p, &clocks, node) {
		match = 0;
		if (p->dev_id) {
			if (!dev_id || strcmp(p->dev_id, dev_id))
				continue;
			match += 2;
		}
		if (p->con_id) {
			if (!con_id || strcmp(p->con_id, con_id))
				continue;
			match += 1;
		}

		if (match > best_found) {
			cl = p;
			if (match != best_possible)
				best_found = match;
			else
				break;
		}
	}
	return cl;
}

struct clk *clk_get_sys(const char *dev_id, const char *con_id)
{
	struct clk_lookup *cl;

	cl = clk_find(dev_id, con_id);
	if (cl && !__clk_get(cl->clk))
		cl = NULL;

	return cl ? cl->clk : ERR_PTR(-ENOENT);
}

void clk_put(struct clk *clk)
{
	__clk_put(clk);
}

void clkdev_add(struct clk_lookup *cl)
{
	list_add_tail(&cl->node, &clocks);
}

void clkdev_add_table(struct clk_lookup *cl, size_t num)
{
	while (num--) {
		list_add_tail(&cl->node, &clocks);
		cl++;
	}
}

#define MAX_DEV_ID	20
#define MAX_CON_ID	16

struct clk_lookup_alloc {
	struct clk_lookup cl;
	char dev_id[MAX_DEV_ID];
	char con_id[MAX_CON_ID];
};

static struct clk_lookup *vclkdev_alloc(struct clk *clk, const char *con_id,
	const char *dev_fmt, va_list ap)
{
	struct clk_lookup_alloc *cla;

	cla = __clkdev_alloc(sizeof(*cla));
	if (!cla)
		return NULL;

	cla->cl.clk = clk;
	if (con_id) {
		strlcpy(cla->con_id, con_id, sizeof(cla->con_id));
		cla->cl.con_id = cla->con_id;
	}

	if (dev_fmt) {
		vscnprintf(cla->dev_id, sizeof(cla->dev_id), dev_fmt, ap);
		cla->cl.dev_id = cla->dev_id;
	}

	return &cla->cl;
}

struct clk_lookup *clkdev_alloc(struct clk *clk, const char *con_id,
	const char *dev_fmt, ...)
{
	struct clk_lookup *cl;
	va_list ap;

	va_start(ap, dev_fmt);
	cl = vclkdev_alloc(clk, con_id, dev_fmt, ap);
	va_end(ap);

	return cl;
}

/*
 * clkdev_drop - remove a clock dynamically allocated
 */
void clkdev_drop(struct clk_lookup *cl)
{
	list_del(&cl->node);
	free(cl);
}

/**
 * clk_register_clkdev - register one clock lookup for a struct clk
 * @clk: struct clk to associate with all clk_lookups
 * @con_id: connection ID string on device
 * @dev_id: format string describing device name
 *
 * con_id or dev_id may be NULL as a wildcard, just as in the rest of
 * clkdev.
 *
 * To make things easier for mass registration, we detect error clks
 * from a previous clk_register() call, and return the error code for
 * those.  This is to permit this function to be called immediately
 * after clk_register().
 */
int clk_register_clkdev(struct clk *clk, const char *con_id,
	const char *dev_fmt, ...)
{
	struct clk_lookup *cl;
	va_list ap;

	if (IS_ERR(clk))
		return PTR_ERR(clk);

	va_start(ap, dev_fmt);
	cl = vclkdev_alloc(clk, con_id, dev_fmt, ap);
	va_end(ap);

	if (!cl)
		return -ENOMEM;

	clkdev_add(cl);

	return 0;
}

/**
 * clk_register_clkdevs - register a set of clk_lookup for a struct clk
 * @clk: struct clk to associate with all clk_lookups
 * @cl: array of clk_lookup structures with con_id and dev_id pre-initialized
 * @num: number of clk_lookup structures to register
 *
 * To make things easier for mass registration, we detect error clks
 * from a previous clk_register() call, and return the error code for
 * those.  This is to permit this function to be called immediately
 * after clk_register().
 */
int clk_register_clkdevs(struct clk *clk, struct clk_lookup *cl, size_t num)
{
	unsigned i;

	if (IS_ERR(clk))
		return PTR_ERR(clk);

	for (i = 0; i < num; i++, cl++) {
		cl->clk = clk;
		clkdev_add(cl);
	}

	return 0;
}
