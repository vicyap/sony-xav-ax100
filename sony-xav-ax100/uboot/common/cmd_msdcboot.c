/*
 * Copyright 2008 - 2009 Windriver, <www.windriver.com>
 * Author: Tom Rix <Tom.Rix@windriver.com>
 *
 * (C) Copyright 2014 Linaro, Ltd.
 * Rob Herring <robh@kernel.org>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */
#include <common.h>
#include <command.h>
#include <linux/usb/usb_device.h>

static int do_msdcboot(cmd_tbl_t *cmdtp, int flag, int argc, char *const argv[])
{
	int ret;
#if 0

	ret = g_dnl_register("sp");
	if (ret)
		return ret;

	while (1) {
		if (ctrlc())
			break;
		usb_gadget_handle_interrupts();
	}

	g_dnl_unregister();
#endif
	sp_fast_register();
	while (!recv_exit()) {
		if (ctrlc())
			break;
		handle_msg();
	}
	
	return CMD_RET_SUCCESS;
}

U_BOOT_CMD(
	msdcboot,	1,	1,	do_msdcboot,
	"msdcboot - enter USB msdcboot protocol",
	""
);
