/*
 * (C) Copyright 2014
 * Sunplus Technology. <http://www.sunplus.com/>
 * Dante Su <dante.su@sunplus.com>
 * Robert Chen <yusheng.chen/Sunplus>
 *
 * SPDX-License-Identifier:     GPL-2.0+
 */

#include <common.h>
#include <asm/io.h>
#include <sp_uart.h>

static int g_baud = 115200;

static int do_uartsp(cmd_tbl_t *cmdtp, int flag, int argc, char *const argv[])
{
	int port;
	char c;
	int i = 0;

	if (argc < 3)
		return CMD_RET_USAGE;

	port = simple_strtoul(argv[2], NULL, 10);

	// init <port> <baud>
	if (strcmp(argv[1], "init") == 0 && argc >= 4) {
		g_baud = simple_strtoul(argv[3], NULL, 10);

		printf("Init port=%d baud=%d\n", port, g_baud);
		if (sp_uart_init(port, g_baud)) {
			printf("Init port %d failed\n", port);
		}
	// tx <port> <string>
	} else if (strcmp(argv[1], "tx") == 0 && argc >= 4) {
		printf("Port %d tx:%s\n", port, argv[3]);
		for (; argv[3][i]; i++)
			sp_uart_putc(port, argv[3][i]);
	// rx <port>
	} else if (strcmp(argv[1], "rx") == 0) {
		printf("Port %d rx:\n", port);
		while (sp_uart_tstc(port)) {
			c = sp_uart_getc(port);
			(c >= 0x20 && c <= 0x7E) ? printf("%c (0x%02x)\n", c, c)
				: printf("?? (0x%02x)\n", c);
		}
		printf("\n");
	// tstc <port>
	} else if (strcmp(argv[1], "tstc") == 0) {
		printf("Check port %d tstc:%d\n", port, sp_uart_tstc(port));
	} else
		return CMD_RET_USAGE;

	return 0;
}

U_BOOT_CMD(
	uartsp, 4, 0, do_uartsp,
	"Sunplus UART utility",
	       "init <port> <baud>\n"
	"uartsp tx   <port> <string>\n"
	"uartsp rx   <port>\n"
	"uartsp tstc <port>\n"
);
