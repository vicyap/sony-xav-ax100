/**
 * @file reg_uart.h
 * @brief uart register definition
 * @author zh.l
 */
#ifndef __REG_UART_H__
#define __REG_UART_H__

#include <mach/hardware.h>

/***************************************************/
/************early print******************************/
/***************************************************/

/* For now, all mappings are flat (physical = virtual)
 */

#define UART0_EARLY_PHYS			IO0_PHY_ADDR(18*32*4)
//#define UART0_EARLY_VIRT			UART0_EARLY_PHYS
/*fix the TLB flush fail issue, fix map early print uart to VMALLOC_START*/
#define UART0_EARLY_VIRT                       (IO0_BASE+0x900)

/*
 * Mandatory for CONFIG_LL_DEBUG, UART is mapped virtual = physical
 */
#define LL_UART_PADDR	UART0_EARLY_PHYS
#define LL_UART_VADDR	UART0_EARLY_VIRT



/***************************************************/
/************uart driver******************************/
/***************************************************/
#define LOGI_ADDR_UART0_REG         IO0_ADDRESS( 18*32*4)
#define LOGI_ADDR_UART1_REG         IO0_ADDRESS( 19*32*4)
#define LOGI_ADDR_UART2_REG         IO0_ADDRESS( 16*32*4)
#define LOGI_ADDR_UART3_REG         IO0_ADDRESS( 17*32*4)
#define LOGI_ADDR_UART4_REG         IO0_ADDRESS(271*32*4)
#define LOGI_ADDR_UART5_REG         IO0_ADDRESS(272*32*4)



#define LOGI_ADDR_UADMA0_REG         IO0_ADDRESS(275*32*4)
#define LOGI_ADDR_UADMA1_REG         IO0_ADDRESS(275*32*4 + 16*4 )

//uart register map
#define SP_UART_DATA		(0x0)
#define SP_UART_LSR			(0x4)
#define SP_UART_MSR			(0x8)
#define SP_UART_LCR			(0xc)
#define SP_UART_MCR			(0x10)
#define SP_UART_DIV_L		(0x14)
#define SP_UART_DIV_H		(0x18)
#define SP_UART_ISC			(0x1c)

//lsr
#define SP_UART_LSR_TXE         (1<<6)  // 1: trasmit fifo is empty
//interrupt
#define SP_UART_LSR_BC		(1<<5)  //break condition
#define SP_UART_LSR_FE		(1<<4)  //frame error
#define SP_UART_LSR_OE		(1<<3)  //overrun error
#define SP_UART_LSR_PE		(1<<2)  //parity error
#define SP_UART_LSR_RX		(1<<1)  // 1: receive fifo not empty
#define SP_UART_LSR_TX		(1<<0)  // 1: transmit fifo is not full

#define SP_UART_LSR_BRK_ERROR_BITS	\
	(SP_UART_LSR_PE | SP_UART_LSR_OE | SP_UART_LSR_FE | SP_UART_LSR_BC)


//lcr
#define SP_UART_LCR_WL5		(0 << 0)
#define SP_UART_LCR_WL6		(1 << 0)
#define SP_UART_LCR_WL7		(2 << 0)
#define SP_UART_LCR_WL8		(3 << 0)
#define SP_UART_LCR_ST		(1 << 2)
#define SP_UART_LCR_PE		(1 << 3)
#define SP_UART_LCR_PR		(1 << 4)
#define SP_UART_LCR_BC		(1 << 5)

//isc
#define SP_UART_ISC_MSM		(1<<7)      //Modem status ctrl
#define SP_UART_ISC_LSM		(1<<6)      //line status interrupt  ctrl,when lst interrupt bit setted
#define SP_UART_ISC_RXM		(1<<5)      //RX interrupt  ctrl, when got some input data
#define SP_UART_ISC_TXM		(1<<4)      //TX interrupt  ctrl, when tras start
//flags,for interrupt
#define HAS_UART_ISC_FLAGMASK		(0xf)
#define SP_UART_ISC_MS		(1<<3)
#define SP_UART_ISC_LS		(1<<2)
#define SP_UART_ISC_RX		(1<<1)
#define SP_UART_ISC_TX		(1<<0)

/* modem control register */
#define SP_UART_MCR_AT		(1<<7)
#define SP_UART_MCR_AC		(1<<6)
#define SP_UART_MCR_AR		(1<<5)
#define SP_UART_MCR_LB		(1<<4)
#define SP_UART_MCR_RI		(1<<3)
#define SP_UART_MCR_DCD		(1<<2)
#define SP_UART_MCR_RTS		(1<<1)
#define SP_UART_MCR_DTS		(1<<0)

/**************************************************************************
 *                          D A T A    T Y P E S                          *
 **************************************************************************/
#if !defined(__ASSEMBLY__)
#define UART_SZ 0x80
typedef struct regs_uart_s {
		volatile UINT32 uart_data;
		volatile UINT32 uart_lsr;
		volatile UINT32 uart_msr;
		volatile UINT32 uart_lcr;
		volatile UINT32 uart_mcr;
		volatile UINT32 uart_div_l;
		volatile UINT32 uart_div_h;
		volatile UINT32 uart_isc;
		volatile UINT32 uart_tx_residue;
		volatile UINT32 uart_rx_residue;
		volatile UINT32 uart_rx_threshold;
}regs_uart_t;

typedef struct regs_uadma_s {
		volatile UINT32 rf_dma_enable_sel;
		volatile UINT32 rf_dma_start_addr;
		volatile UINT32 rf_dma_timeout_set;
		volatile UINT32 rf_dma_reserved;  //there is no register
		volatile UINT32 rf_dma_wr_adr;
		volatile UINT32 rf_dma_rd_adr;
		volatile UINT32 rf_dma_length_thr;
		volatile UINT32 rf_dma_end_addr;
		volatile UINT32 rf_dma_databytes;
		volatile UINT32 rf_dma_debug_info;
}regs_uadma_t;
#endif /* __ASSEMBLY__ */


#endif
