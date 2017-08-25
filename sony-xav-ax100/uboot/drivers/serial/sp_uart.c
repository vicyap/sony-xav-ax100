/*
 * (C) Copyright 2015
 * Sunplus Technology. <http://www.sunplus.com/>
 *
 * SPDX-License-Identifier:     GPL-2.0+
 */
#include <common.h>
#include <serial.h>
#include <asm/io.h>
#include <asm/arch/regmap.h>
#include <linux/clk.h>
#include <linux/clkdev.h>
#include "sp_uart.h"

DECLARE_GLOBAL_DATA_PTR;

static int gInit[SP_UART_NUM]; /* uninitialized */

struct sp_uart_regs {
        uint32_t dr;  /* data register */
        uint32_t lsr; /* line status register */
        uint32_t msr; /* modem status register */
        uint32_t lcr; /* line control register */
        uint32_t mcr; /* modem control register */
        uint32_t div[2];
        uint32_t isc; /* interrupt status/control */
        uint32_t txr; /* tx residue */
        uint32_t rxr; /* rx residue */
        uint32_t thr; /* rx threshold */
};

#if 0
#define IO0_BASE                    0x9C000000
#define IO0_ADDRESS(x)              ((x) + IO0_BASE)

#define LOGI_ADDR_UART0_REG         IO0_ADDRESS( 18*32*4)
#define LOGI_ADDR_UART1_REG         IO0_ADDRESS( 19*32*4 + 10*4 )
#define LOGI_ADDR_UART2_REG         IO0_ADDRESS( 16*32*4 + 10*4 )
#define LOGI_ADDR_UART3_REG         IO0_ADDRESS( 17*32*4)
#define LOGI_ADDR_UART4_REG         IO0_ADDRESS(271*32*4)
#define LOGI_ADDR_UART5_REG         IO0_ADDRESS(272*32*4)
#endif

#define IS_FREE_UART(port)         ((port) >= SP_UART_FIRST && (port) <= SP_UART_LAST)

static struct sp_uart_regs *ports[SP_UART_NUM] = {
	(struct sp_uart_regs *) UART0_REG,
	(struct sp_uart_regs *) UART1_REG,
	(struct sp_uart_regs *) UART2_REG,
	(struct sp_uart_regs *) UART3_REG,
	(struct sp_uart_regs *) UART4_REG,
	(struct sp_uart_regs *) UART5_REG,
};

static inline void sp_uart_setbrg(int port, int baud)
{
        uint32_t clk, div, ext;

        clk = clk_get_rate(clk_get_sys(NULL, "UART")) + (baud / 2);
        div = clk / baud;
        ext = div & 0x0f;
        div = ((div >> 4) - 1);

#ifndef CONFIG_FPGA_NEW
        /* baud rate */
        writel((ext << 12) | (div & 0xff), &ports[port]->div[0]);
        writel((div >> 8) & 0xff, &ports[port]->div[1]);

        /* 8-bits, 1-stop, no parity */
        writel(0x03, &ports[port]->lcr);
#endif

	/* bss is writable after relocation */
        if (gd->flags & GD_FLG_RELOC) {
		gInit[port] = 1; /* port initialized */
	}
}

int sp_uart_init(int port, int baud)
{
	//char cfgname[20] = "\0";
	//int value = 1;

	if (!IS_FREE_UART(port))
		return -1;

	/*
	sprintf(cfgname, "UART%dEnable", port);
	if(getConfigInt(cfgname, &value) == SP_CFG_RET_SUCCESS){
		if(value == 0){
			return -1;
		}
	}
	*/

	sp_uart_setbrg(port, baud);

	return 0;
}

void sp_uart_putc(int port, const char c)
{
	if (port && !gInit[port])
		return;

	while (!(readl(&ports[port]->lsr) & BIT(0)))
		;
	writel(c, &ports[port]->dr);
}

int sp_uart_tstc(int port)
{
	if (port && !gInit[port])
		return 0;

        return !!(readl(&ports[port]->lsr) & BIT(1));
}

char sp_uart_getc(int port)
{
	if (port && !gInit[port])
		return 0;

        while (!sp_uart_tstc(port))
                /* NOP */;

        return readl(&ports[port]->dr);
}

/**************************
 * stdio console (UART0)
 **************************/

u32 disable_sp_serial_putc;
static int sp_serial_init0(void)
{
	/*
	int value = 1;
	if(getConfigInt("UART0Enable", &value) == SP_CFG_RET_SUCCESS){
		if(value == 0){
			return -1;
		}
	}
	*/

	serial_setbrg();
	disable_sp_serial_putc = 0;
	return 0;
}

static void sp_serial_setbrg0(void)
{
	sp_uart_setbrg(0, gd->baudrate);
}

static void sp_serial_putc0(const char c)
{
	if(disable_sp_serial_putc)
		return;

	if (c == '\n') {
		while (!(readl(&ports[0]->lsr) & BIT(0)))
			;
		writel('\r', &ports[0]->dr);
	}

	sp_uart_putc(0, c);
}

static int sp_serial_tstc0(void)
{
	return sp_uart_tstc(0);
}

static int sp_serial_getc0(void)
{
	return sp_uart_getc(0);
}

static struct serial_device sp_serial_drv = {
	.name   = "sp_serial",
	.start  = sp_serial_init0,
	.stop   = NULL,
	.setbrg = sp_serial_setbrg0,
	.putc   = sp_serial_putc0,
	.puts   = default_serial_puts,
	.getc   = sp_serial_getc0,
	.tstc   = sp_serial_tstc0,
};

void sp_serial_initialize(void)
{
	serial_register(&sp_serial_drv);
}

__weak struct serial_device *default_serial_console(void)
{
	return &sp_serial_drv;
}
