/*
 * (C) Copyright 2015
 * Sunplus Technology. <http://www.sunplus.com/>
 *
 * SPDX-License-Identifier:     GPL-2.0+
 */

#ifndef __SP_UART_H
#define __SP_UART_H

/**
 * UART0: used by Debug port
 * UART1: free
 * UART2: free
 * UART3: pinmux conflict with GPIO[27] - acfg.gpio_car_reverse_detect=27 (8700-BGA-DEMO-BOARD)
 * UART4: pinmux conflict with SPI_D[1] - acfg.pmSPIFlashInterface=1
 * UART5: pinmux conflict with SPI_D[0] - acfg.pmSPIFlashInterface=1
 *
 * Note: To use UART3~5, please disable/modify the conflicting pinmux in config.txt
 */
#define SP_UART_FIRST       1
#define SP_UART_LAST        5
#define SP_UART_NUM         6

/**
 * sp_uart_init - Initialize UART port
 * @port: UART port (1~5)
 * @baud: Baud rate (eg. 115200)
 *
 * Return 0 if success ; otherwise, failed.
 */
int  sp_uart_init(int port, int baud);

/**
 * sp_uart_putc - Transmit data byte
 * @port: UART port
 * @c: Data byte
 */
void sp_uart_putc(int port, char c);

/**
 * sp_uart_tstc - Check if rx buffer has data
 * @port: UART port
 *
 * Return 1 if rx buffer has data; return 0 otherwise.
 */
int sp_uart_tstc(int port);

/**
 * sp_uart_getc - Receive data byte
 * @port: UART port
 *
 * Return received data byte
 */
char sp_uart_getc(int port);


#endif	/* __SP_UART_H */
