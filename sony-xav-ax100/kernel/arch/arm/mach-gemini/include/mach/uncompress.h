#ifndef __UNCOMPRESS_H_
#define __UNCOMPRESS_H_

#include <linux/types.h>
#include <linux/serial_reg.h>
#include <mach/common.h>
#include <mach/hal/regmap/reg_uart.h>

static regs_uart_t* uart_base = ((regs_uart_t*)(LL_UART_PADDR));

static void putc(int c)
{
	while (!(uart_base->uart_lsr & SP_UART_LSR_TX))
		barrier();
	uart_base->uart_data = c;
	return;
}

static inline void flush(void)
{
}

/*
 * nothing to do
 */
#define arch_decomp_setup()

#endif //endif __UNCOMPRESS_H_
