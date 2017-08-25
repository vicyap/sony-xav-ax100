#ifndef __HAL_UART_H__
#define __HAL_UART_H__

#include <mach/common.h>
//#include <mach/hardware.h>
#include <mach/hal/regmap/reg_uart.h>
#include <linux/err.h>

/**************************************************************************
 *                           C O N S T A N T S                            *
 **************************************************************************/
#define UART_PORT_NUM	(6)
#define UART_DMA_NUM	(2)

/**************************************************************************
 *                              M A C R O S                               *
 **************************************************************************/

/**************************************************************************
 *                          D A T A    T Y P E S                          *
 **************************************************************************/

static regs_uart_t* pUartPorts[UART_PORT_NUM]= {
	((regs_uart_t*)(LOGI_ADDR_UART0_REG)),
	((regs_uart_t*)(LOGI_ADDR_UART1_REG)),
	((regs_uart_t*)(LOGI_ADDR_UART2_REG)),
	((regs_uart_t*)(LOGI_ADDR_UART3_REG)),
	((regs_uart_t*)(LOGI_ADDR_UART4_REG)),
	((regs_uart_t*)(LOGI_ADDR_UART5_REG))
};

static regs_uadma_t* pUADMA[UART_DMA_NUM]= {
	((regs_uadma_t*)(LOGI_ADDR_UADMA0_REG)),
	((regs_uadma_t*)(LOGI_ADDR_UADMA1_REG))
};

/**************************************************************************
 *                 E X T E R N A L    R E F E R E N C E S                 *
 **************************************************************************/
extern u32 get_sysbus_freq(void);
#if defined(CONFIG_MAGIC_SYSRQ)
extern unsigned int uart0_mask_tx;
#endif
#if 0
// standard value
#define	BAUDCC(f,c)		((UINT32)((2*(c)+16*(f)) / (32*(f)))-1)
//#define	BAUDC(f)		BAUDCC(f,SYSCLK)

#define	BAUDC(f)		BAUDCC(f,get_sysbus_freq())

//
// uart clock divisor
// 		81MHz
//	9600	020e
//	115200	002b	3f
//
#define	UART_BAUD_9600	BAUDC(9600)
#define	UART_BAUD_57600	BAUDC(57600)
#define	UART_BAUD_115200	BAUDC(115200)
#define	UART_BAUD_230400	BAUDC(230400)
#define	UART_BAUD_460800	BAUDC(460800)
#define	UART_BAUD_921600	BAUDC(921600)
#define	UART_BAUD_88200 	BAUDC(88200)
#endif

/*calculate by external application file baudcalc.c from headking*/
#define 	UART_DIV_SYS1G_BAUT9600		0x0019706D
#define 	UART_DIV_SYS1G_BAUT57600	0x0004103C
#define 	UART_DIV_SYS1G_BAUT88200	0x0002A0C3
#define 	UART_DIV_SYS1G_BAUT115200 	0x0002901D
#define 	UART_DIV_SYS1G_BAUT230400	0x0001400E
#define 	UART_DIV_SYS1G_BAUT460800	0x0000A086
#define 	UART_DIV_SYS1G_BAUT921600 	0x0000D042

#define 	UART_DIV_SYS50M_BAUT9600	0x00018044
#define 	UART_DIV_SYS50M_BAUT57600	0x00004035
#define 	UART_DIV_SYS50M_BAUT88200	0x00007022
#define 	UART_DIV_SYS50M_BAUT115200 	0x0000201A
#define 	UART_DIV_SYS50M_BAUT230400	0x0000900C
#define 	UART_DIV_SYS50M_BAUT460800	0x0000D005
#define 	UART_DIV_SYS50M_BAUT921600 	0x00006002

#define 	UART_DIV_SYS16M_BAUT9600	0x00003067
#define 	UART_DIV_SYS16M_BAUT57600	0x00006016
#define 	UART_DIV_SYS16M_BAUT88200	0x0000500A
#define 	UART_DIV_SYS16M_BAUT115200 	0x0000B007
#define 	UART_DIV_SYS16M_BAUT230400	0x00005003
#define 	UART_DIV_SYS16M_BAUT460800	0x00003001
#define 	UART_DIV_SYS16M_BAUT921600 	0x00001000


#define 	UART_SYS1G 	1000000000 
#define 	UART_SYS50M 	50000000
#define 	UART_SYS16M 	16000000

/**************************************************************************
p *               F U N C T I O N    D E C L A R A T I O N S               *
 **************************************************************************/

/**
* @brief UART set freq function.
* @param device_id[in] : Index of uart device
* @param freq[in] : baudrate value
*/
void spHalUartSetBaud(UINT32 device_id,UINT32 baud);

/**
* @brief UART set modem function.
* @param device_id[in] : Index of uart device
* @param modem [in] : modem value
*/
static inline void spHalUartSetModemStatus(UINT32 device_id,UINT32 modem)
{
	//UINT32 temp;
	if (likely(device_id < UART_PORT_NUM))
		pUartPorts[device_id]->uart_msr = modem;
}

/**
* @brief UART get modem status register
* @param device_id[in] : Index of uart device
* @param modem [out] : modem status
*/
static inline UINT32 spHalUartGetModemStatus(UINT32 device_id)
{

	if (likely(device_id < UART_PORT_NUM))
		return pUartPorts[device_id]->uart_msr;
	else
		return 0;
}

/**
* @brief UART set interrupt enable register
* @param device_id[in] : Index of uart device
* @param int_state[out] : interrupt enable state
*/
static inline void spHalUartSetIntEn(unsigned device_id, unsigned int_state)
{
	if( likely(device_id < UART_PORT_NUM) ) {
		pUartPorts[device_id]->uart_isc =  int_state;
	}
}

/**
* @brief UART get interrupt enable register
* @param device_id[in] : Index of uart device
* @return interrupt enable state
*/
static inline unsigned spHalUartGetIntEn(unsigned device_id)
{
	if( likely(device_id < UART_PORT_NUM) ) {
		return pUartPorts[device_id]->uart_isc;
	} else
		return 0;
}

static inline int spHalUartGetChar(unsigned device_id)
{
	if( likely(device_id < UART_PORT_NUM) ) {
		return pUartPorts[device_id]->uart_data;
	} else
		return -1;
}

static inline void spHalUartPutChar(unsigned device_id, unsigned ch)
{
	if( likely(device_id < UART_PORT_NUM) ) {
#if defined(CONFIG_MAGIC_SYSRQ)
        if(likely(!((uart0_mask_tx == 1) && (device_id == 0))))
            pUartPorts[device_id]->uart_data = ch;
#else
		pUartPorts[device_id]->uart_data = ch;
#endif
	}
}

static inline unsigned spHalUartGetLineStatus( unsigned device_id)
{
	if( likely(device_id < UART_PORT_NUM) ) {
		return pUartPorts[device_id]->uart_lsr;
	} else
		return 0;
}

static inline void spHalUartSetLineCtrl( unsigned device_id, unsigned ctrl)
{
	if( likely(device_id < UART_PORT_NUM) ) {
		pUartPorts[device_id]->uart_lcr = ctrl;
	}
}

static inline unsigned spHalUartGetLineCtrl(unsigned device_id)
{
	if( likely(device_id < UART_PORT_NUM) ) {
		return pUartPorts[device_id]->uart_lcr;
	} else
		return 0;
}

static inline void spHalUartSetDividerLowRegister( unsigned device_id, unsigned val)
{
	if( likely(device_id < UART_PORT_NUM) ) {
		pUartPorts[device_id]->uart_div_l = val;
	}
}

static inline unsigned spHalUartGetDividerLowRegister(unsigned device_id)
{
	if( likely(device_id < UART_PORT_NUM) ) {
		return pUartPorts[device_id]->uart_div_l;
	} else
		return 0;
}

static inline void spHalUartSetDividerHighRegister( unsigned device_id, unsigned val)
{
	if( likely(device_id < UART_PORT_NUM) ) {
		pUartPorts[device_id]->uart_div_h = val;
	}
}

static inline unsigned spHalUartGetDividerHighRegister(unsigned device_id)
{
	if( likely(device_id < UART_PORT_NUM) ) {
		return pUartPorts[device_id]->uart_div_h;
	} else
		return 0;
}

static inline void spHalUartSetRxResidue( unsigned device_id, unsigned val)
{
	if( likely(device_id < UART_PORT_NUM) ) {
		pUartPorts[device_id]->uart_rx_residue = val;
	}
}

static inline unsigned spHalUartGetRxResidue(unsigned device_id)
{
	if( likely(device_id < UART_PORT_NUM) ) {
		return pUartPorts[device_id]->uart_rx_residue;
	} else
		return 0;
}

static inline void spHalUartSetModemCtrl( unsigned device_id, unsigned val)
{
	if( likely(device_id < UART_PORT_NUM) ) {
		pUartPorts[device_id]->uart_mcr = val;
	}
}

static inline unsigned spHalUartGetModemCtrl(unsigned device_id)
{
	if( likely(device_id < UART_PORT_NUM) ) {
		return pUartPorts[device_id]->uart_mcr; 
	} else
		return 0;
}

#endif

