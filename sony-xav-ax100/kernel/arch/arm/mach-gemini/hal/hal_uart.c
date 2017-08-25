#include <linux/module.h>	/*for EXPORT_SYMBOL*/
#include <linux/clk.h>
#include <mach/hal/hal_uart.h>	/*for function declare*/
#include <mach/hardware.h>
/**
* @brief UART set freq function.
* @param device_id[in] : Index of uart device
* @param freq[in] : freq value(?? what frequency??)
*/
void spHalUartSetBaud(UINT32 device_id,UINT32 baud)
{
	if( likely(device_id < UART_PORT_NUM) ) {
		pUartPorts[device_id]->uart_div_h = (baud>>8);	
		pUartPorts[device_id]->uart_div_l = (baud);	
	}
}
EXPORT_SYMBOL(spHalUartSetBaud);



