/* ---------------------------------------------------------------------------------------------- */
// #define FOR_FPGA    /* comment it for ASIC */

#ifdef FOR_FPGA
    #define NUM_UART_IN_FPGA    (5)
#endif /* FOR_FPGA */
/* ---------------------------------------------------------------------------------------------- */
// Definition of USE_THREAD_FOR_CONSOLE_TX is not used anymore (2015/02/06).
// The behavior is based on kernel command's "sp_uart.use_thread_for_console_tx".
/* ---------------------------------------------------------------------------------------------- */
#if defined(CONFIG_MAGIC_SYSRQ)
#define SUPPORT_SYSRQ
#endif

#include <linux/module.h>
#include <linux/ioport.h>
#include <linux/init.h>
#include <linux/console.h>
#include <linux/sysrq.h>
#include <linux/serial_reg.h>
#include <linux/circ_buf.h>
#include <linux/delay.h>
#include <linux/interrupt.h>
#include <linux/platform_device.h>
#include <linux/tty.h>
#include <linux/tty_flip.h>
#include <linux/clk.h>
#include <linux/serial_core.h>
#include <linux/kthread.h>
#include <asm/io.h>
#include <asm/irq.h>
#include <mach/hal/hal_uart.h>
#include <mach/hal/regmap/reg_system.h>
#include <mach/sp_clk.h>
#include "mach/gp_chunkmem.h"
// #include <mach/sp_rst_clk_pwr.h>
// #include <mach/sphe_clock.h>
// #include <mach/gp_chunkmem.h>
// #include <iop_car/drv_gpio.h>
// #include <mach/gpio_drv.h>
// #include <asm/sysconfig.h>
// #include <mach/sp_customize_config.h>
/* ---------------------------------------------------------------------------------------------- */
//#define FOR_DMA_TIMER

#ifdef SYSCONFIG_Support_UADMA0
#define SUPPORT_UADMA0
#define UADMA0_PAIR_UANUM		SYSCONFIG_UADMA0_PairUANum
#endif

#ifdef SYSCONFIG_Support_UADMA1
#define SUPPORT_UADMA1
#define UADMA1_PAIR_UANUM		SYSCONFIG_UADMA1_PairUANum
#endif


/* ---------------------------------------------------------------------------------------------- */
#define TTYS_KDBG_INFO
#define TTYS_KDBG_ERR

#ifdef TTYS_KDBG_INFO
    #define DBG_INFO(fmt, args ...) printk(KERN_INFO "K_TTYS: " fmt, ## args)
#else
    #define DBG_INFO(fmt, args ...)
#endif

#ifdef TTYS_KDBG_ERR
    #define DBG_ERR(fmt, args ...)  printk(KERN_ERR "K_TTYS: " fmt, ## args)
#else
    #define DBG_ERR(fmt, args ...)
#endif
/* ---------------------------------------------------------------------------------------------- */
#define DEVICE_NAME                 "ttyS"
#define SP_UART_MAJOR               (TTY_MAJOR)
#define SP_UART_MINOR_START         (64)
#define NUM_UART                    (UART_PORT_NUM)

#define SP_UART_CREAD_DISABLED    (1 << 16)

// Note:
// When (uart0_as_console == 0), please make sure:
//   * There is no "console=ttyS0,115200", "earlyprintk", ... in kernel command line.
//   * In /etc/inittab, there is no something like "ttyS0::respawn:/bin/sh"
unsigned int uart0_as_console = ~0;
unsigned int use_thread_for_console_tx = 0;
unsigned int force_not_to_use_thread_for_console_tx = 0;
unsigned int uart_enable_status = 0;                 // bit 0: UART0, bit 1: UART1, ...

/* Used for masking uart0 tx output */
#if defined(CONFIG_MAGIC_SYSRQ)
extern unsigned int uart0_mask_tx;
extern int sysrqCheckState(char, struct uart_port *);
#endif

const unsigned int sunpluse_uart_irq[] = {
  #ifdef FOR_FPGA
    #if (NUM_UART_IN_FPGA == 1)
        SP_IRQ_UART0
    #elif (NUM_UART_IN_FPGA == 2)
        SP_IRQ_UART0, SP_IRQ_UART1
    #elif (NUM_UART_IN_FPGA == 5)
        SP_IRQ_UART0, SP_IRQ_UART1, SP_IRQ_UART2,
        SP_IRQ_UART3, SP_IRQ_UART4
    #else
        #error "No such configuration in FPGA"
    #endif
  #else
    SP_IRQ_UART0, SP_IRQ_UART1, SP_IRQ_UART2, SP_IRQ_UART3,
    SP_IRQ_UART4, SP_IRQ_UART5
  #endif /* FOR_FPGA */
};

struct sunplus_uart_reg_val {
    unsigned uart_lcr;
    unsigned uart_div_l;
    unsigned uart_div_h;
    unsigned uart_isc;
    unsigned uart_mcr;
};


struct sunplus_uart_port {
    char                name[16];   /* Sunplus_UARTx */
    struct uart_port    uport;
    struct sunplus_uart_reg_val reg_val;    /* used for suspend/resume */
    struct regs_uadma_s reg_uadma_val;	/* used for suspend/resume */
};

struct sunplus_uart_dma_add{
	UINT8 *st_src_8b;
	UINT8 *end_src_8b;
	UINT8 bSchedule;
};


struct sunplus_uart_port sunplus_uart_ports[NUM_UART];
struct sunplus_uart_dma_add sunplus_uart_dma_adds[2];

// #ifdef USE_THREAD_FOR_CONSOLE_TX
#define BUF_SZ            (1<<14)   // must be pow(2, N)
#define YIELD()         msleep(1)   // Don't use schedule(). It might cause too many DRAM access.
static u8 buf4thread[BUF_SZ*2];
static struct task_struct *task_struct_ptr;

static int index_in =0;
static int index_out = 0;
static int index_console;

// static struct tty_driver *tty_driver_ptr;
// static const struct tty_operations *tty_operations_ptr;
// #endif

//////////////////////////////////

//DMA Enable Select
#define DMA_INT					(31)
#define DMA_MSI_ID				(12)
#define DMA_SEL_UARTX			(8)
#define DMA_SW_RST_B			(7)
#define DMA_INIT					(6)
#define DMA_GO					(5)
#define DMA_AUTO_ENABLE		(4)
#define DMA_TIMEOUT_INT_EN		(3)
#define DMA_P_SAFE_DISABLE		(2)
#define DMA_PBUS_DATA_SWAP	(1)
#define DMA_ENABLE				(0)

//DMA Debug
#define DMA_SDMS_THRESHOLD_FIFO_OVFLOW	(24)
#define DMA_ISSUE_ST	(20)
#define DMA_C_DMA_RUNING	(19)
#define DMA_SDATABYTE_TIMEOUT 	(18)
#define DMA_SMON_DMA_FIFO_OVFLOW	(17)
#define DMA_SDMS_FIFO_OVFLOW	(16)
#define DMA_D_BYTES_BUF_VEC		(8)
#define DMA_S_UPDATE_DONE		(7)
#define DMA_P_ISSUE_MSI_DONE		(6)
#define DMA_SEND_TRANSACT		(5)
#define DMA_S_TIMOUT				(4)
#define DMA_DMA_STATE				(0)

#define UADMA_TIMEOUT_DEF		(0xffff)
#if defined(SYSCONFIG_TARGET_CLASS_CAR)
#define UADMA_LENGTH			(0xC000) //note@20140922 RL:enlarge UART DMA buffer size to 8K
#else
#define UADMA_LENGTH			(0xC000)
#endif
#define UADMA_THREADSHOLD		UADMA_LENGTH/3
#define UADMA_TDELAY_MS		(10000)


#if defined(SUPPORT_UADMA0) || defined(SUPPORT_UADMA1)
struct timer_list uadma0_timer;
struct timer_list uadma1_timer;
static spinlock_t uadma_lock;

static void uadma0_receive(unsigned long data);
static void uadma1_receive(unsigned long data);
static DEFINE_SPINLOCK(uart_dma_lock);
DECLARE_TASKLET(uadma0_receive_tasklet, uadma0_receive, 0);
DECLARE_TASKLET(uadma1_receive_tasklet, uadma1_receive, 1);


static void uadma_dump_debug_info(UINT32 dDMANum)
{
	printk("===========================================\n");
	printk("uadma num : %d\n", dDMANum);
	printk("DMA_DMA_STATE=%x\n", (pUADMA[dDMANum]->rf_dma_debug_info & (0xF<<DMA_DMA_STATE))>>DMA_DMA_STATE);
	printk("DMA_S_TIMOUT=%x\n",  (pUADMA[dDMANum]->rf_dma_debug_info & (0x1<<DMA_S_TIMOUT))>>DMA_S_TIMOUT);
	printk("DMA_SEND_TRANSACT=%x\n",  (pUADMA[dDMANum]->rf_dma_debug_info & (0x1<<DMA_SEND_TRANSACT))>>DMA_SEND_TRANSACT);
	printk("DMA_P_ISSUE_MSI_DONE=%x\n", (pUADMA[dDMANum]->rf_dma_debug_info & (0x1<<DMA_P_ISSUE_MSI_DONE))>>DMA_P_ISSUE_MSI_DONE);
	printk("DMA_S_UPDATE_DONE=%x\n",  (pUADMA[dDMANum]->rf_dma_debug_info & (0x1<<DMA_S_UPDATE_DONE))>>DMA_S_UPDATE_DONE);
	printk("DMA_D_BYTES_BUF_VEC=%x\n",  (pUADMA[dDMANum]->rf_dma_debug_info & (0xFF<<DMA_D_BYTES_BUF_VEC))>>DMA_D_BYTES_BUF_VEC);
	printk("DMA_SDMS_FIFO_OVFLOW=%x\n",  (pUADMA[dDMANum]->rf_dma_debug_info & (0x1<<DMA_SDMS_FIFO_OVFLOW))>>DMA_SDMS_FIFO_OVFLOW);
	printk("DMA_SMON_DMA_FIFO_OVFLOW=%x\n",  (pUADMA[dDMANum]->rf_dma_debug_info & (0x1<<DMA_SMON_DMA_FIFO_OVFLOW))>>DMA_SMON_DMA_FIFO_OVFLOW);
	printk("DMA_SDATABYTE_TIMEOUT=%x\n",  (pUADMA[dDMANum]->rf_dma_debug_info & (0x1<<DMA_SDATABYTE_TIMEOUT))>>DMA_SDATABYTE_TIMEOUT);
	printk("DMA_C_DMA_RUNING=%x\n",  (pUADMA[dDMANum]->rf_dma_debug_info & (0x1<<DMA_C_DMA_RUNING))>>DMA_C_DMA_RUNING);
	printk("DMA_ISSUE_ST=%x\n",  (pUADMA[dDMANum]->rf_dma_debug_info & (0xF<<DMA_ISSUE_ST))>>DMA_ISSUE_ST);
	printk("DMA_SDMS_THRESHOLD_FIFO_OVFLOW=%x\n",  (pUADMA[dDMANum]->rf_dma_debug_info & (0x1<<DMA_SDMS_THRESHOLD_FIFO_OVFLOW))>>DMA_SDMS_THRESHOLD_FIFO_OVFLOW);
	printk("===========================================\n");
}

static void uadma_dump_register(UINT32 dDMANum)
{
	printk("===========================================\n");
	printk("uadma num : %d\n", dDMANum);
	printk("uadma start address : 0x%x\n", pUADMA[dDMANum]->rf_dma_start_addr);
	printk("uadma end address : 0x%x\n", pUADMA[dDMANum]->rf_dma_end_addr);
	printk("uadma read address : 0x%x\n", pUADMA[dDMANum]->rf_dma_rd_adr);
	printk("uadma write address : 0x%x\n", pUADMA[dDMANum]->rf_dma_wr_adr);
	printk("uadma databytes : 0x%x\n", pUADMA[dDMANum]->rf_dma_databytes);
	printk("uadma circle set : 0x%x\n", pUADMA[dDMANum]->rf_dma_timeout_set);
	printk("uadma threadhold : 0x%x\n", pUADMA[dDMANum]->rf_dma_length_thr);
	printk("uadma enable sel: 0x%x\n", pUADMA[dDMANum]->rf_dma_enable_sel);
	printk("===========================================\n");
}

static void uadma_dump_memory(UINT32 dDMANum, UINT8 *rd_src_8b, UINT8 *wr_src_8b)
{
	int i=0;
	UINT8 temp[UADMA_LENGTH]={0};
	UINT8 *st_src_8b = (UINT8 *)gp_chunk_va(pUADMA[dDMANum]->rf_dma_start_addr);
	UINT8 *end_src_8b = (UINT8 *)gp_chunk_va(pUADMA[dDMANum]->rf_dma_end_addr);
	//UINT8 *rd_src_8b = (UINT8 *)gp_chunk_va(pUADMA[dDMANum]->rf_dma_rd_adr);
	//UINT8 *wr_src_8b = (UINT8 *)gp_chunk_va(pUADMA[dDMANum]->rf_dma_wr_adr);
	int dDMA_DataBytes=0;

	if(rd_src_8b != wr_src_8b)
	{
		//printk("[timer dump] [%d] p[%x, %x]\n", dDMANum, pUADMA[dDMANum]->rf_dma_rd_adr, pUADMA[dDMANum]->rf_dma_wr_adr);
		printk("[timer dump] [%d] v[%x, %x]\n", dDMANum, rd_src_8b, wr_src_8b);
		if(wr_src_8b<rd_src_8b)
			wr_src_8b=wr_src_8b+UADMA_LENGTH;
		dDMA_DataBytes=(int)(wr_src_8b-rd_src_8b);
		for(i=0; i<dDMA_DataBytes; i++)
		{
			temp[i]=*rd_src_8b;
			rd_src_8b++;
			if(rd_src_8b>end_src_8b)
			{
				rd_src_8b=st_src_8b;
			}
		}
		printk("[timer dump %d] %s\n", dDMA_DataBytes, temp);
	}
}

static inline void uadma1_receive(unsigned long data)
{
	//printk("Enter uadma0_receive\n" );
	#ifdef SUPPORT_UADMA1
		struct uart_port *port = &sunplus_uart_ports[UADMA1_PAIR_UANUM].uport;//(struct uart_port *)data;
	#else
		struct uart_port *port = (struct uart_port *)data;
	#endif

	unsigned long flags;
	spin_lock_irqsave(&port->lock, flags);
	UINT8 *rd_src_8b = (UINT8 *)gp_chunk_va(pUADMA[1]->rf_dma_rd_adr);
	UINT8 *wr_src_8b = (UINT8 *)gp_chunk_va(pUADMA[1]->rf_dma_wr_adr);
	if(rd_src_8b == wr_src_8b)
	{
		goto RESUME_UADMA_STATUS;
	}
	int dDMA_DataBytes=0, i=0;
	struct tty_struct *tty = port->state->port.tty;
	unsigned int  lsr = spHalUartGetLineStatus(port->line);
	unsigned int ch;

	if(wr_src_8b<rd_src_8b)
		dDMA_DataBytes=(int)(wr_src_8b + UADMA_LENGTH - rd_src_8b);
	else
		dDMA_DataBytes=(int)(wr_src_8b-rd_src_8b);

	//printk("UADMA0 %d databytes.\n\n", dDMA_DataBytes);
	#if 0
	if( (UADMA_LENGTH-dDMA_DataBytes) <= UADMA_THREADSHOLD )
	{
		printk("\n\(UADMA_LENGTH-dDMA_DataBytes) <= UADMA_THREADSHOLD.......\n");
		uadma_dump_register(1);
		uadma_dump_debug_info(1);
		uadma_dump_memory(1, rd_src_8b, wr_src_8b);
	}
	#endif

	if (tty != NULL)
	{
		for(i=0; i<dDMA_DataBytes; i++)
		{
			uart_insert_char(port, lsr, SP_UART_LSR_OE, *(rd_src_8b++), TTY_NORMAL);
		ignore_char:
			if(rd_src_8b > sunplus_uart_dma_adds[1].end_src_8b)
			{
			  rd_src_8b = sunplus_uart_dma_adds[1].st_src_8b;
			}
		}
		tty_flip_buffer_push(tty);
		pUADMA[1]->rf_dma_rd_adr = (UINT32)gp_chunk_pa((void *)wr_src_8b);
	}
	else
	{
		pUADMA[1]->rf_dma_rd_adr = pUADMA[1]->rf_dma_wr_adr;
	}

	RESUME_UADMA_STATUS:
	pUADMA[1]->rf_dma_enable_sel |= (1<<DMA_GO);
	spin_unlock_irqrestore(&port->lock, flags);

}

static inline void uadma0_receive(unsigned long data)
{
	//printk("Enter uadma0_receive\n" );
	#ifdef SUPPORT_UADMA0
		struct uart_port *port = &sunplus_uart_ports[UADMA0_PAIR_UANUM].uport;//(struct uart_port *)data;
	#else
		struct uart_port *port = (struct uart_port *)data;
	#endif

	unsigned long flags;
	spin_lock_irqsave(&port->lock, flags);
	UINT8 *rd_src_8b = (UINT8 *)gp_chunk_va(pUADMA[0]->rf_dma_rd_adr);
	UINT8 *wr_src_8b = (UINT8 *)gp_chunk_va(pUADMA[0]->rf_dma_wr_adr);

	if(rd_src_8b == wr_src_8b)
	{
		goto RESUME_UADMA_STATUS;
	}
	int dDMA_DataBytes=0, i=0;
	struct tty_struct *tty = port->state->port.tty;
	unsigned int  lsr = spHalUartGetLineStatus(port->line);
	unsigned int ch;

	if(wr_src_8b<rd_src_8b)
		dDMA_DataBytes=(int)(wr_src_8b + UADMA_LENGTH - rd_src_8b);
	else
		dDMA_DataBytes=(int)(wr_src_8b-rd_src_8b);

	//printk("UADMA0 %d databytes.\n\n", dDMA_DataBytes);
	#if 0
	if( (UADMA_LENGTH-dDMA_DataBytes) <= UADMA_THREADSHOLD )
	{
		printk("\n\(UADMA_LENGTH-dDMA_DataBytes) <= UADMA_THREADSHOLD.......\n");
		uadma_dump_register(0);
		uadma_dump_debug_info(0);
		uadma_dump_memory(0, rd_src_8b, wr_src_8b);
	}
	#endif

	if (tty != NULL)
	{
		for(i=0; i<dDMA_DataBytes; i++)
		{
			uart_insert_char(port, lsr, SP_UART_LSR_OE, *(rd_src_8b++), TTY_NORMAL);
		ignore_char:
			if(rd_src_8b > sunplus_uart_dma_adds[0].end_src_8b)
			{
			  rd_src_8b = sunplus_uart_dma_adds[0].st_src_8b;
			}
		}
		tty_flip_buffer_push(tty);
		pUADMA[0]->rf_dma_rd_adr = (UINT32)gp_chunk_pa((void *)wr_src_8b);
	}
	else
	{
		pUADMA[0]->rf_dma_rd_adr = pUADMA[0]->rf_dma_wr_adr;
	}

	RESUME_UADMA_STATUS:
	pUADMA[0]->rf_dma_enable_sel |= (1<<DMA_GO);
	spin_unlock_irqrestore(&port->lock, flags);
}

static void uadma1_do_timer(unsigned long data)
{
	//printk("uadma1_do_timer \n");
	tasklet_schedule(&uadma1_receive_tasklet);
	mod_timer(&uadma1_timer, jiffies +  msecs_to_jiffies(UADMA_TDELAY_MS));
}

static void uadma0_do_timer(unsigned long data)
{
	//printk("uadma0_do_timer \n");
	tasklet_schedule(&uadma0_receive_tasklet);
	mod_timer(&uadma0_timer, jiffies +  msecs_to_jiffies(UADMA_TDELAY_MS));
}

static void uadma1_init_timer(struct uart_port *port)
{
	printk("Enter uadma1_init_timer, port=0x%x\n", (UINT32)port);
	setup_timer(&uadma1_timer, uadma1_do_timer, (unsigned long)port);
	mod_timer(&uadma1_timer, jiffies +  msecs_to_jiffies(UADMA_TDELAY_MS));
}

static void uadma0_init_timer(struct uart_port *port)
{
	printk("Enter uadma0_init_timer, port=0x%x\n", (UINT32)port);
	setup_timer(&uadma0_timer, uadma0_do_timer, (unsigned long)port);
	mod_timer(&uadma0_timer, jiffies +  msecs_to_jiffies(UADMA_TDELAY_MS));
}


static irqreturn_t uadma1_interrupt(int irq, void *args)
{
    pUADMA[1]->rf_dma_enable_sel |= (1<<DMA_INT);
    tasklet_schedule(&uadma1_receive_tasklet);
    return IRQ_HANDLED;
}


static irqreturn_t uadma0_interrupt(int irq, void *args)
{
    pUADMA[0]->rf_dma_enable_sel |= (1<<DMA_INT);
    tasklet_schedule(&uadma0_receive_tasklet);
    return IRQ_HANDLED;
}

void UADMA1_Initial(struct uart_port *port)
{
#if defined(SYSCONFIG_TARGET_CLASS_CAR)
	printk("Enter UADMA1_Initial[%d]\n", UADMA_LENGTH );
#endif
	UINT8* pDmaStAdd= NULL;
	UINT32 dDmaStAdd_Phy;
	moonReg_t *pmoonReg = (moonReg_t *)LOGI_ADDR_MOON_REG;

	if (request_irq(SP_IRQ_DMA1, uadma1_interrupt, IRQF_DISABLED, "uadma1", port))  //neslon add
	{
		printk("uadma1 cant get interrupt num.\n" );
	}
	pDmaStAdd = gp_chunk_malloc_nocache(0, 0, UADMA_LENGTH);
	if(!pDmaStAdd)
	{
		printk("chunk memory allocation failed\n");
	}
	memset(pDmaStAdd, 0, UADMA_LENGTH);
	dDmaStAdd_Phy = (UINT32)gp_chunk_pa((void *)pDmaStAdd);

	pUADMA[1]->rf_dma_start_addr = dDmaStAdd_Phy;
	pUADMA[1]->rf_dma_timeout_set = UADMA_TIMEOUT_DEF;
	pUADMA[1]->rf_dma_rd_adr =  pUADMA[1]->rf_dma_start_addr;
	pUADMA[1]->rf_dma_wr_adr =  pUADMA[1]->rf_dma_start_addr;
	pUADMA[1]->rf_dma_length_thr =  UADMA_LENGTH << 16 | UADMA_THREADSHOLD;
	pUADMA[1]->rf_dma_enable_sel |= ((port->line)<<DMA_SEL_UARTX)  |  (1<<DMA_INIT);
	pUADMA[1]->rf_dma_enable_sel &= ~(1<<DMA_INIT);
	pUADMA[1]->rf_dma_enable_sel |= (1<<DMA_SW_RST_B)  |
									(1<<DMA_AUTO_ENABLE)  | (1<<DMA_TIMEOUT_INT_EN)  | (0<<DMA_P_SAFE_DISABLE)  |
									(1<<DMA_ENABLE);
	pUADMA[1]->rf_dma_enable_sel |= (1<<DMA_GO);

	uadma_dump_register(1);

	sunplus_uart_dma_adds[1].st_src_8b = (UINT8 *)gp_chunk_va(pUADMA[1]->rf_dma_start_addr);
	sunplus_uart_dma_adds[1].end_src_8b = (UINT8 *)gp_chunk_va(pUADMA[1]->rf_dma_end_addr);
	sunplus_uart_dma_adds[1].bSchedule = 0;

	#ifdef FOR_DMA_TIMER
		uadma1_init_timer(port);
	#endif
}

void UADMA0_Initial(struct uart_port *port)
{
#if defined(SYSCONFIG_TARGET_CLASS_CAR)
	printk("Enter UADMA0_Initial[%d]\n", UADMA_LENGTH );
#endif
	UINT8* pDmaStAdd= NULL;
	UINT32 dDmaStAdd_Phy;
	moonReg_t *pmoonReg = (moonReg_t *)LOGI_ADDR_MOON_REG;

	if (request_irq(SP_IRQ_DMA0, uadma0_interrupt, IRQF_DISABLED, "uadma0", port))  //neslon add
	{
		printk("uadma0 cant get interrupt num.\n" );
	}
	pDmaStAdd = gp_chunk_malloc_nocache(0, 0, UADMA_LENGTH);
	if(!pDmaStAdd)
	{
		printk("chunk memory allocation failed\n");
	}
	memset(pDmaStAdd, 0, UADMA_LENGTH);
	dDmaStAdd_Phy = (UINT32)gp_chunk_pa((void *)pDmaStAdd);

	pUADMA[0]->rf_dma_start_addr = dDmaStAdd_Phy;
	pUADMA[0]->rf_dma_timeout_set = UADMA_TIMEOUT_DEF;
	pUADMA[0]->rf_dma_rd_adr =  pUADMA[0]->rf_dma_start_addr;
	pUADMA[0]->rf_dma_wr_adr =  pUADMA[0]->rf_dma_start_addr;
	pUADMA[0]->rf_dma_length_thr =  UADMA_LENGTH << 16 | UADMA_THREADSHOLD;
	pUADMA[0]->rf_dma_enable_sel |= ((port->line)<<DMA_SEL_UARTX)  |  (1<<DMA_INIT);
	pUADMA[0]->rf_dma_enable_sel &= ~(1<<DMA_INIT);
	pUADMA[0]->rf_dma_enable_sel |= (1<<DMA_SW_RST_B)  |
									(1<<DMA_AUTO_ENABLE)  | (1<<DMA_TIMEOUT_INT_EN)  | (0<<DMA_P_SAFE_DISABLE)  |
									(1<<DMA_ENABLE);
	pUADMA[0]->rf_dma_enable_sel |= (1<<DMA_GO);

	uadma_dump_register(0);
	sunplus_uart_dma_adds[0].st_src_8b = (UINT8 *)gp_chunk_va(pUADMA[0]->rf_dma_start_addr);
	sunplus_uart_dma_adds[0].end_src_8b = (UINT8 *)gp_chunk_va(pUADMA[0]->rf_dma_end_addr);
	sunplus_uart_dma_adds[0].bSchedule = 0;

	#ifdef FOR_DMA_TIMER
		uadma0_init_timer(port);
	#endif
}

void UART_Initial(UINT32 dUARTNum)
{
	//printk("Enter UART_Initial\n");

	pUartPorts[dUARTNum]->uart_isc 	= 0;			//Disable INT, use polling
	pUartPorts[dUARTNum]->uart_mcr 	= 0;			//SM MCR function disable
	pUartPorts[dUARTNum]->uart_lcr 	= 0x0003;	//default lcr, 8-bit, no parity, 1 stop bit
}

void UADMA1_Init(struct uart_port *port)
{
	//printk("Enter UADMA1_Init, port=0x%x\n", (UINT32) port);
	UART_Initial(port->line);
	UADMA1_Initial(port);
}

void UADMA0_Init(struct uart_port *port)
{
	//printk("Enter UADMA0_Init, port=0x%x\n", (UINT32) port);
	UART_Initial(port->line);
	UADMA0_Initial(port);
}
//////////////////////////////////
#endif

static inline void wait_for_xmitr(struct uart_port *port)
{
    unsigned int status;
    do {
        status = spHalUartGetLineStatus(port->line);
    } while (!(status & SP_UART_LSR_TX));
}

// #ifdef USE_THREAD_FOR_CONSOLE_TX
static int send_data_to_buf(const char *s, unsigned count)
{
    unsigned i;
    char *ptr = (char *)(s);
    int index_in_next;
    int index_out_now = index_out;

    for(i=0; i<count; i++) {
        index_in_next = index_in + 1; index_in_next &= (BUF_SZ-1);

        if(index_in_next == index_out_now)  // no room for tx data
            return i;

        buf4thread[index_in] = *ptr;
        ptr++;
        index_in = index_in_next;
    }

    return count;
}

static void sunplus_uart_console_putchar2(struct uart_port *port, int ch)
{
    unsigned int status;

    while(1) {
        status = spHalUartGetLineStatus(port->line);
        if(!(status & SP_UART_LSR_TX))
            YIELD();
        else
            break;
    }

    spHalUartPutChar(port->line, ch);
}

static int thread_uart_tx(void *arg)
{
    int size;
    int index_in_now;

    while(1) {
        if(kthread_should_stop())
            break;

        index_in_now = index_in;
        if(index_in_now != index_out) {
            if(index_in_now < index_out) {
                size = BUF_SZ - (index_out - index_in_now);
                memcpy(&buf4thread[BUF_SZ], &buf4thread[0], index_in_now);
            } else {
                size = index_in_now - index_out;
            }

            while(size) {
                if(index_in == index_out) // because of console_flush()
                    break;

                uart_console_write(&sunplus_uart_ports[index_console].uport, &buf4thread[index_out], 1, sunplus_uart_console_putchar2);
                index_out += 1; index_out &= (BUF_SZ-1);
                size--;
            }

        } else {
            YIELD();
        }
    }
    return 0;
}

// Because console_flush() might be called in ISR, suspend, resume, or kernel panic, etc.
// spin_lock/mutex is not used between it and thread_uart_tx().
int console_flush(void)
{
    int size;
    int index_in_now;
    unsigned int status;
    u8 *ptr;

    if(!use_thread_for_console_tx)
        return 0;

    index_in_now = index_in;
    if(index_in_now != index_out) {
        if(index_in_now < index_out) {
            size = BUF_SZ - (index_out - index_in_now);
            memcpy(&buf4thread[BUF_SZ], &buf4thread[0], index_in_now);
        } else {
            size = index_in_now - index_out;
        }

        ptr = &buf4thread[index_out];
        while(size) {
            while(1) {
                status = spHalUartGetLineStatus(0);
                if(status & SP_UART_LSR_TX)
                    break;
            }

            spHalUartPutChar(0, *ptr);
            ptr++;
            size--;
        }

        index_out += size; index_out &= (BUF_SZ-1);
    }

    return 0;
}
EXPORT_SYMBOL(console_flush);

int use_thread_for_console_tx_force_disabled(void)
{
    force_not_to_use_thread_for_console_tx = 1;
    return 0;
}
EXPORT_SYMBOL(use_thread_for_console_tx_force_disabled);

int use_thread_for_console_tx_default(void)
{
    force_not_to_use_thread_for_console_tx = 0;
    return 0;
}
EXPORT_SYMBOL(use_thread_for_console_tx_default);

// #endif

// #ifndef USE_THREAD_FOR_CONSOLE_TX
static void sunplus_uart_console_putchar(struct uart_port *port, int ch)
{
    wait_for_xmitr(port);
    spHalUartPutChar(port->line, ch);
}
// #endif

static void sunplus_console_write(struct console *co, const char *s, unsigned count)
{
    unsigned long flags;
    int locked = 1;

    local_irq_save(flags);

    if (sunplus_uart_ports[co->index].uport.sysrq)
        locked = 0;
    else if (oops_in_progress)
        locked = spin_trylock(&sunplus_uart_ports[co->index].uport.lock);
    else
        spin_lock(&sunplus_uart_ports[co->index].uport.lock);

// #ifndef USE_THREAD_FOR_CONSOLE_TX
    if ((!use_thread_for_console_tx) || force_not_to_use_thread_for_console_tx) {
        uart_console_write(&sunplus_uart_ports[co->index].uport, s, count, sunplus_uart_console_putchar);
    }
// #else
    else {
        index_console = co->index;
        send_data_to_buf(s, count);
    }
// #endif
    if (locked)
        spin_unlock(&sunplus_uart_ports[co->index].uport.lock);

    local_irq_restore(flags);
}

static int __init sunplus_console_setup(struct console *co, char *options)
{
    int ret = 0;
    int baud = 115200;
    int bits = 8;
    int parity = 'n';
    int flow = 'n';
    // char string_console_setup[]="\n\nsunplus_console_setup()\n\n";

    if((co->index >= NUM_UART) || (co->index < 0))
        return -EINVAL;

    if(options)
        uart_parse_options(options, &baud, &parity, &bits, &flow);

    ret = uart_set_options(&sunplus_uart_ports[co->index].uport, co, baud, parity, bits, flow);
    // sunplus_console_write(co, string_console_setup, sizeof(string_console_setup));
    return ret;
}

/*
 *  Documentation/serial/driver:
 *  tx_empty(port)
 *  This function tests whether the transmitter fifo and shifter
 *  for the port described by 'port' is empty.  If it is empty,
 *  this function should return TIOCSER_TEMT, otherwise return 0.
 *  If the port does not support this operation, then it should
 *  return TIOCSER_TEMT.
 *
 *  Locking: none.
 *  Interrupts: caller dependent.
 *  This call must not sleep
 */
static unsigned int sunplus_uart_ops_tx_empty(struct uart_port *port)
{
    return ( (spHalUartGetLineStatus(port->line) & SP_UART_LSR_TXE) ? TIOCSER_TEMT : 0 );
}

/*
 *  Documentation/serial/driver:
 *  set_mctrl(port, mctrl)
 *  This function sets the modem control lines for port described
 *  by 'port' to the state described by mctrl.  The relevant bits
 *  of mctrl are:
 *      - TIOCM_RTS     RTS signal.
 *      - TIOCM_DTR     DTR signal.
 *      - TIOCM_OUT1    OUT1 signal.
 *      - TIOCM_OUT2    OUT2 signal.
 *      - TIOCM_LOOP    Set the port into loopback mode.
 *  If the appropriate bit is set, the signal should be driven
 *  active.  If the bit is clear, the signal should be driven
 *  inactive.
 *
 *  Locking: port->lock taken.
 *  Interrupts: locally disabled.
 *  This call must not sleep
 */
static void sunplus_uart_ops_set_mctrl(struct uart_port *port, unsigned int mctrl)
{
    unsigned char mcr = spHalUartGetModemCtrl(port->line);

    if (mctrl&TIOCM_DTR){
        mcr |=SP_UART_MCR_DTS;
    }
    else{
        mcr &=~SP_UART_MCR_DTS;
    }
    if (mctrl&TIOCM_RTS){
        mcr |=SP_UART_MCR_RTS;
    }
    else{
        mcr &=~SP_UART_MCR_RTS;
    }
    if (mctrl&TIOCM_CAR){
        mcr |=SP_UART_MCR_DCD;
        }
    else{
        mcr &=~SP_UART_MCR_DCD;
    }
    if (mctrl&TIOCM_RI){
        mcr |=SP_UART_MCR_RI;
    }
    else{
        mcr &=~SP_UART_MCR_RI;
    }
    if (mctrl&TIOCM_LOOP){
        mcr |=SP_UART_MCR_LB;
    }
    else{
        mcr &=~SP_UART_MCR_LB;
    }

    spHalUartSetModemCtrl(port->line, mcr);
}

/*
 *  Documentation/serial/driver:
 *  get_mctrl(port)
 *  Returns the current state of modem control inputs.  The state
 *  of the outputs should not be returned, since the core keeps
 *  track of their state.  The state information should include:
 *      - TIOCM_CAR     state of DCD signal
 *      - TIOCM_CTS     state of CTS signal
 *      - TIOCM_DSR     state of DSR signal
 *      - TIOCM_RI      state of RI signal
 *  The bit is set if the signal is currently driven active.  If
 *  the port does not support CTS, DCD or DSR, the driver should
 *  indicate that the signal is permanently active.  If RI is
 *  not available, the signal should not be indicated as active.
 *
 *  Locking: port->lock taken.
 *  Interrupts: locally disabled.
 *  This call must not sleep
 */
static unsigned int sunplus_uart_ops_get_mctrl(struct uart_port *port)
{
    unsigned char status;
    unsigned int ret = 0;

    status = spHalUartGetModemCtrl(port->line);

    if (status & SP_UART_MCR_DTS) {
        ret |= TIOCM_DTR;
    }

    if (status & SP_UART_MCR_RTS) {
        ret |= TIOCM_RTS;
    }

    if (status & SP_UART_MCR_DCD) {
        ret |= TIOCM_CAR;
    }

    if (status & SP_UART_MCR_RI) {
        ret |= TIOCM_RI;
    }

    if (status & SP_UART_MCR_LB) {
        ret |= TIOCM_LOOP;
    }

    if (status & SP_UART_MCR_AC) {
        ret |= TIOCM_CTS;
    }

    return ret;
}

/*
 *  Documentation/serial/driver:
 *  stop_tx(port)
 *  Stop transmitting characters.  This might be due to the CTS
 *  line becoming inactive or the tty layer indicating we want
 *  to stop transmission due to an XOFF character.
 *
 *  The driver should stop transmitting characters as soon as
 *  possible.
 *
 *  Locking: port->lock taken.
 *  Interrupts: locally disabled.
 *  This call must not sleep
 */
static void sunplus_uart_ops_stop_tx(struct uart_port *port)
{
    unsigned int isc = spHalUartGetIntEn(port->line);
    isc &= ~SP_UART_ISC_TXM;
    spHalUartSetIntEn(port->line, isc);
}

/*
 *  Documentation/serial/driver:
 *  start_tx(port)
 *  Start transmitting characters.
 *
 *  Locking: port->lock taken.
 *  Interrupts: locally disabled.
 *  This call must not sleep
 */
static void sunplus_uart_ops_start_tx(struct uart_port *port)
{
    unsigned int isc;
    isc = spHalUartGetIntEn(port->line) | SP_UART_ISC_TXM;
    spHalUartSetIntEn(port->line, isc);
}

/*
 *  Documentation/serial/driver:
 *  send_xchar(port,ch)
 *  Transmit a high priority character, even if the port is stopped.
 *  This is used to implement XON/XOFF flow control and tcflow().  If
 *  the serial driver does not implement this function, the tty core
 *  will append the character to the circular buffer and then call
 *  start_tx() / stop_tx() to flush the data out.
 *
 *  Locking: none.
 *  Interrupts: caller dependent.
 */
/*
static void sunplus_uart_ops_send_xchar(struct uart_port *port, char ch)
{
}
*/

/*
 *  Documentation/serial/driver:
 *  stop_rx(port)
 *  Stop receiving characters; the port is in the process of
 *  being closed.
 *
 *  Locking: port->lock taken.
 *  Interrupts: locally disabled.
 *  This call must not sleep
 */
static void sunplus_uart_ops_stop_rx(struct uart_port *port)
{
    unsigned int isc;

    isc = spHalUartGetIntEn(port->line);
    isc &= ~SP_UART_ISC_RXM;
    spHalUartSetIntEn(port->line, isc);
}

/*
 *  Documentation/serial/driver:
 *
 *  enable_ms(port)
 *  Enable the modem status interrupts.
 *
 *  This method may be called multiple times.  Modem status
 *  interrupts should be disabled when the shutdown method is
 *  called.
 *
 *  Locking: port->lock taken.
 *  Interrupts: locally disabled.
 *  This call must not sleep
 */
static void sunplus_uart_ops_enable_ms(struct uart_port *port)
{
    /* Do nothing */
}

/*
 *  Documentation/serial/driver:
 *  break_ctl(port,ctl)
 *  Control the transmission of a break signal.  If ctl is
 *  nonzero, the break signal should be transmitted.  The signal
 *  should be terminated when another call is made with a zero
 *  ctl.
 *
 *  Locking: none.
 *  Interrupts: caller dependent.
 *  This call must not sleep
 */
static void sunplus_uart_ops_break_ctl(struct uart_port *port, int ctl)
{
    unsigned long flags;
    unsigned int h_lcr;

    spin_lock_irqsave(&port->lock, flags);

    h_lcr = spHalUartGetLineCtrl(port->line);
    if (ctl != 0)
        h_lcr |= SP_UART_LCR_BC;      /*start break*/
    else
        h_lcr &= ~SP_UART_LCR_BC;     /*stop break*/
    spHalUartSetLineCtrl(port->line, h_lcr);

    spin_unlock_irqrestore(&port->lock, flags);
}

static void transmit_chars(struct uart_port *port)  /* called by ISR */
{
    struct circ_buf *xmit = &port->state->xmit;
    int count;

    if (port->x_char) {
        spHalUartPutChar(port->line, port->x_char);
        port->icount.tx++;
        port->x_char = 0;
        return;
    }

    if (uart_circ_empty(xmit) || uart_tx_stopped(port)) {
        sunplus_uart_ops_stop_tx(port);
        return;
    }

    count = port->fifosize /2;
    do {
        spHalUartPutChar(port->line, xmit->buf[xmit->tail]);
        xmit->tail = (xmit->tail + 1) & (UART_XMIT_SIZE - 1);
        port->icount.tx++;

        if (uart_circ_empty(xmit))
            break;

        if(--count <=0)
			break;
    } while ((spHalUartGetLineStatus(port->line) & (SP_UART_LSR_TX)) != 0); // transmit FIFO is not full

    if (uart_circ_chars_pending(xmit) < WAKEUP_CHARS)
        uart_write_wakeup(port);

    if (uart_circ_empty(xmit))
        sunplus_uart_ops_stop_tx(port);
}

static inline void receive_chars(struct uart_port *port)    /* called by ISR */
{
    struct tty_struct *tty = port->state->port.tty;
    unsigned char  lsr = spHalUartGetLineStatus(port->line);
    unsigned int ch, flag;
    int max_count = 256;
	//printk("%s\n", __FUNCTION__);


    do {
        ch = spHalUartGetChar(port->line);

#if defined(CONFIG_MAGIC_SYSRQ)
            if( sysrqCheckState(ch, port) != 0)
                goto ignore_char;
#endif

        flag = TTY_NORMAL;
        port->icount.rx++;

        if (unlikely(lsr & SP_UART_LSR_BRK_ERROR_BITS)) {
            if (port->cons == NULL)
                printk("%s,ERR: LSR Error bit (0x%x),line=%d eflt\n",__FUNCTION__,lsr,port->line);

            if (lsr & SP_UART_LSR_BC) {
                lsr &= ~(SP_UART_LSR_FE | SP_UART_LSR_PE);
                port->icount.brk++;
                /*
                 * We do the SysRQ and SAK checking
                 * here because otherwise the break
                 * may get masked by ignore_status_mask
                 * or read_status_mask.
                 */
                if (uart_handle_break(port))
                    goto ignore_char;
            } else if (lsr & SP_UART_LSR_PE) {
                if (port->cons == NULL)
                    printk("%s,ERR: LSR PE bit,line=%d eflt\n",__FUNCTION__,port->line);
                port->icount.parity++;
            } else if (lsr & SP_UART_LSR_FE) {
                if (port->cons == NULL)
                    printk("%s,ERR: LSR FE bit,line=%d eflt\n",__FUNCTION__,port->line);
                port->icount.frame++;
            }
            if (lsr & SP_UART_LSR_OE)
            {
                if (port->cons == NULL)
                    printk("%s,ERR: LSR OE bit,line=%d eflt\n",__FUNCTION__,port->line);
                port->icount.overrun++;
            }


            /*
             * Mask off conditions which should be ignored.
             */
            // lsr &= port->read_status_mask;

            if (lsr & SP_UART_LSR_BC)
                flag = TTY_BREAK;
            else if (lsr & SP_UART_LSR_PE)
                flag = TTY_PARITY;
            else if (lsr & SP_UART_LSR_FE)
                flag = TTY_FRAME;
        }

        if (port->ignore_status_mask & SP_UART_CREAD_DISABLED) {
            goto ignore_char;
        }

        if (uart_handle_sysrq_char(port, ch))
            goto ignore_char;

        uart_insert_char(port, lsr, SP_UART_LSR_OE, ch, flag);

    ignore_char:
        lsr = spHalUartGetLineStatus(port->line);
    } while ((lsr & SP_UART_LSR_RX) && (max_count-- > 0));

    //spin_unlock(&port->lock); --already have spin_lock_irqsave at irq
    tty_flip_buffer_push(tty);
    //spin_lock(&port->lock);
}

static inline irqreturn_t sunplus_uart_irq(int irq, void *args)
{
    struct uart_port *port = (struct uart_port *)args;
    unsigned long flags;

    spin_lock_irqsave(&port->lock, flags);

    //transmit_chars(port);


    if (spHalUartGetIntEn(port->line) & SP_UART_ISC_RX)
    {
        receive_chars(port);
    }

	if (spHalUartGetIntEn(port->line) & SP_UART_ISC_TX)
	{
		transmit_chars(port);
	}
    spin_unlock_irqrestore(&port->lock, flags);

    return IRQ_HANDLED;
}

/*
 *  Documentation/serial/driver:
 *  startup(port)
 *  Grab any interrupt resources and initialise any low level driver
 *  state.  Enable the port for reception.  It should not activate
 *  RTS nor DTR; this will be done via a separate call to set_mctrl.
 *
 *  This method will only be called when the port is initially opened.
 *
 *  Locking: port_sem taken.
 *  Interrupts: globally disabled.
 */
static int sunplus_uart_ops_startup(struct uart_port *port)
{
    int ret;

	#ifdef SUPPORT_UADMA0
    static int IsDMA0Init=0;
	//printk("In sunplus_uart_ops_startup IsDMA0Init=%d\n", IsDMA0Init);
	if(sunplus_uart_ports[UADMA0_PAIR_UANUM].uport.line == UADMA0_PAIR_UANUM && IsDMA0Init==0)
	{
		printk("sunplus_uart_ports[UADMA0_UANUM].uport.line=%d\n", sunplus_uart_ports[UADMA0_PAIR_UANUM].uport.line);
		UADMA0_Init(&sunplus_uart_ports[UADMA0_PAIR_UANUM].uport);
		IsDMA0Init=1;
	}
	#endif

	#ifdef SUPPORT_UADMA1
    static int IsDMA1Init=0;
	//printk("In sunplus_uart_ops_startup IsDMA1Init=%d\n", IsDMA1Init);
	if(sunplus_uart_ports[UADMA1_PAIR_UANUM].uport.line == UADMA1_PAIR_UANUM && IsDMA1Init==0)
	{
		printk("sunplus_uart_ports[UADMA1_UANUM].uport.line=%d\n", sunplus_uart_ports[UADMA1_PAIR_UANUM].uport.line);
		UADMA1_Init(&sunplus_uart_ports[UADMA1_PAIR_UANUM].uport);
		IsDMA1Init=1;
	}
	#endif

    ret = request_irq(port->irq, sunplus_uart_irq, 0, ((struct sunplus_uart_port *)(port->private_data))->name, port);
    if(ret)
        return ret;

    spin_lock_irq(&port->lock);     /* don't need to use spin_lock_irqsave() because interrupts are globally disabled */
	#ifdef SUPPORT_UADMA0
    	if(port->line == UADMA0_PAIR_UANUM)
    	{
			spHalUartSetIntEn(port->line, SP_UART_ISC_TXM);
    		goto END_HAL_UART_EN;
    	}
	#endif
	#ifdef SUPPORT_UADMA1
    	if(port->line == UADMA1_PAIR_UANUM)
    	{
			spHalUartSetIntEn(port->line, SP_UART_ISC_TXM);
    		goto END_HAL_UART_EN;
    	}
	#endif
    spHalUartSetIntEn(port->line, SP_UART_ISC_TXM | SP_UART_ISC_RXM);
    END_HAL_UART_EN:
    spin_unlock_irq(&port->lock);
    return 0;
}

/*
 *  Documentation/serial/driver:
 *  shutdown(port)
 *  Disable the port, disable any break condition that may be in
 *  effect, and free any interrupt resources.  It should not disable
 *  RTS nor DTR; this will have already been done via a separate
 *  call to set_mctrl.
 *
 *  Drivers must not access port->info once this call has completed.
 *
 *  This method will only be called when there are no more users of
 *  this port.
 *
 *  Locking: port_sem taken.
 *  Interrupts: caller dependent.
 */
static void sunplus_uart_ops_shutdown(struct uart_port *port)
{
    unsigned long flags;

    spin_lock_irqsave(&port->lock, flags);
    spHalUartSetIntEn(port->line, 0);       // disable all interrupt
    spin_unlock_irqrestore(&port->lock, flags);

    free_irq(port->irq, port);
}

/*
 *  Documentation/serial/driver:
 *  flush_buffer(port)
 *  Flush any write buffers, reset any DMA state and stop any
 *  ongoing DMA transfers.
 *
 *  This will be called whenever the port->info->xmit circular
 *  buffer is cleared.
 *
 *  Locking: port->lock taken.
 *  Interrupts: locally disabled.
 *  This call must not sleep
 *
 */
/*
static void sunplus_uart_ops_flush_buffer(struct uart_port *port)
{
}
*/

/*
 *  Documentation/serial/driver:
 *  set_termios(port,termios,oldtermios)
 *  Change the port parameters, including word length, parity, stop
 *  bits.  Update read_status_mask and ignore_status_mask to indicate
 *  the types of events we are interested in receiving.  Relevant
 *  termios->c_cflag bits are:
 *      CSIZE   - word size
 *      CSTOPB  - 2 stop bits
 *      PARENB  - parity enable
 *      PARODD  - odd parity (when PARENB is in force)
 *      CREAD   - enable reception of characters (if not set,
 *                still receive characters from the port, but
 *                throw them away.
 *      CRTSCTS - if set, enable CTS status change reporting
 *      CLOCAL  - if not set, enable modem status change
 *                reporting.
 *  Relevant termios->c_iflag bits are:
 *      INPCK   - enable frame and parity error events to be
 *                passed to the TTY layer.
 *      BRKINT
 *      PARMRK  - both of these enable break events to be
 *                passed to the TTY layer.
 *
 *      IGNPAR  - ignore parity and framing errors
 *      IGNBRK  - ignore break errors,  If IGNPAR is also
 *                set, ignore overrun errors as well.
 *  The interaction of the iflag bits is as follows (parity error
 *  given as an example):
 *  Parity error    INPCK   IGNPAR
 *  n/a     0       n/a     character received, marked as
 *                          TTY_NORMAL
 *  None            1       n/a character received, marked as
 *                          TTY_NORMAL
 *  Yes     1       0       character received, marked as
 *                          TTY_PARITY
 *  Yes     1       1       character discarded
 *
 *  Other flags may be used (eg, xon/xoff characters) if your
 *  hardware supports hardware "soft" flow control.
 *
 *  Locking: none.
 *  Interrupts: caller dependent.
 *  This call must not sleep
 */

static void sunplus_uart_ops_set_termios(struct uart_port *port, struct ktermios *termios, struct ktermios *oldtermios)
{
    u32 clk, ext, div, div_l, div_h, baud;
    u32 lcr;
    unsigned long flags;

    clk = sp_clk_get(SP_CLK_SYSSLOW);

    baud = uart_get_baud_rate(port, termios, oldtermios, 0, (clk/(1<<4)));
    //printk("UART clock: %d, baud: %d\n", clk, baud);
#if defined(SYSCONFIG_TARGET_CLASS_CAR)	//Raymond Liu add for Bluetooth
	if(baud == 921600)
	{
		baud = 964000;
		printk("UART clock: %d, baud up to %d\n", clk, baud);
	}
#endif
    clk += baud >> 1;
    div = clk/baud;
    ext = div & 0xf;
    div = ((div >> 4) - 1);
    div_l = (div & 0xff) | (ext << 12);
    div_h = (div >> 8);
    // printk("div_l = %x, div_h: %x\n", div_l, div_h);

    switch (termios->c_cflag & CSIZE) {
        case CS5:   lcr = SP_UART_LCR_WL5;   break;
        case CS6:   lcr = SP_UART_LCR_WL6;   break;
        case CS7:   lcr = SP_UART_LCR_WL7;   break;
        default:    lcr = SP_UART_LCR_WL8;   break;  /* CS8 */
    }

    if (termios->c_cflag & CSTOPB)
        lcr |= SP_UART_LCR_ST;

    if (termios->c_cflag & PARENB) {
        lcr |= SP_UART_LCR_PE;

        if (!(termios->c_cflag & PARODD))
            lcr |= SP_UART_LCR_PR;
    }
    // printk("lcr = %x, \n", lcr);

    // printk("Updating UART registers...\n");
    spin_lock_irqsave(&port->lock, flags);

    uart_update_timeout(port, termios->c_cflag, baud);

    port->read_status_mask = 0;
    if (termios->c_iflag & INPCK)
        port->read_status_mask |= SP_UART_LSR_PE | SP_UART_LSR_FE ;
    if (termios->c_iflag & (BRKINT | PARMRK))
        port->read_status_mask |= SP_UART_LSR_BC;

    /*
     * Characters to ignore
     */
    port->ignore_status_mask = 0;
    if (termios->c_iflag & IGNPAR)
        port->ignore_status_mask |= SP_UART_LSR_FE | SP_UART_LSR_PE;
    if (termios->c_iflag & IGNBRK) {
        port->ignore_status_mask |= SP_UART_LSR_BC;

        if (termios->c_iflag & IGNPAR)
            port->ignore_status_mask |= SP_UART_LSR_OE;
    }

    /*
     * Ignore all characters if CREAD is not set.
     */
    if ((termios->c_cflag & CREAD) == 0) {
        port->ignore_status_mask |= SP_UART_CREAD_DISABLED;
        spHalUartSetRxResidue(port->line, 0);   // flush rx data FIFO
    }

    /* No modem pin in chip:
    if (UART_ENABLE_MS(port, termios->c_cflag))
        enable_modem_status_irqs(port);
    */

    if (termios->c_cflag & CRTSCTS) {
        spHalUartSetModemCtrl(port->line, spHalUartGetModemCtrl(port->line) | (SP_UART_MCR_AC | SP_UART_MCR_AR));
    } else {
        spHalUartSetModemCtrl(port->line, spHalUartGetModemCtrl(port->line) & (~(SP_UART_MCR_AC | SP_UART_MCR_AR)));
    }

#ifndef FOR_FPGA /* In FPGA, just uses the setting from boot code. */
    spHalUartSetDividerHighRegister(port->line, div_h);
    spHalUartSetDividerLowRegister(port->line, div_l);
#endif
    spHalUartSetLineCtrl(port->line, lcr);

    spin_unlock_irqrestore(&port->lock, flags);
}

/*
 *  Documentation/serial/driver:
 *  N/A.
 */
static void sunplus_uart_ops_set_ldisc(struct uart_port *port, int new)
{
    if (new == N_PPS) {
        port->flags |= UPF_HARDPPS_CD;
        sunplus_uart_ops_enable_ms(port);
    } else
        port->flags &= ~UPF_HARDPPS_CD;
}

/*
 *  Documentation/serial/driver:
 *  pm(port,state,oldstate)
 *  Perform any power management related activities on the specified
 *  port.  State indicates the new state (defined by
 *  enum uart_pm_state), oldstate indicates the previous state.
 *
 *  This function should not be used to grab any resources.
 *
 *  This will be called when the port is initially opened and finally
 *  closed, except when the port is also the system console.  This
 *  will occur even if CONFIG_PM is not set.
 *
 *  Locking: none.
 *  Interrupts: caller dependent.
 */
/*
static void sunplus_uart_ops_pm(struct uart_port *port, unsigned int state, unsigned int oldstate)
{
}
*/

/*
 *  Documentation/serial/driver:
 *  set_wake(port,state)
 *  Enable/disable power management wakeup on serial activity.  Not
 *  currently implemented.
 */
/*
static int sunplus_uart_ops_set_wake(struct uart_port *port, unsigned int state)
{
}
*/

/*
 *  Documentation/serial/driver:
 *  type(port)
 *  Return a pointer to a string constant describing the specified
 *  port, or return NULL, in which case the string 'unknown' is
 *  substituted.
 *
 *  Locking: none.
 *  Interrupts: caller dependent.
 */
static const char *sunplus_uart_ops_type(struct uart_port *port)
{
    struct sunplus_uart_port *sunplus_uart_port  = (struct sunplus_uart_port *)port->private_data;
    return (sunplus_uart_port->name);
}

/*
 *  Documentation/serial/driver:
 *  release_port(port)
 *  Release any memory and IO region resources currently in use by
 *  the port.
 *
 *  Locking: none.
 *  Interrupts: caller dependent.
 */
static void sunplus_uart_ops_release_port(struct uart_port *port)
{
    release_mem_region((resource_size_t)port->mapbase, UART_SZ);
}

/*
 *  Documentation/serial/driver:
 *  request_port(port)
 *  Request any memory and IO region resources required by the port.
 *  If any fail, no resources should be registered when this function
 *  returns, and it should return -EBUSY on failure.
 *
 *  Locking: none.
 *  Interrupts: caller dependent.
 */
static int sunplus_uart_ops_request_port(struct uart_port *port)
{
    struct sunplus_uart_port *sunplus_uart_port  = (struct sunplus_uart_port *)port->private_data;
    return request_mem_region((resource_size_t)port->mapbase, UART_SZ, sunplus_uart_port->name) != NULL ? 0 : -EBUSY;   /* /proc/iomem */
}

/*
 *  Documentation/serial/driver:
 *  config_port(port,type)
 *  Perform any autoconfiguration steps required for the port.  `type`
 *  contains a bit mask of the required configuration.  UART_CONFIG_TYPE
 *  indicates that the port requires detection and identification.
 *  port->type should be set to the type found, or PORT_UNKNOWN if
 *  no port was detected.
 *
 *  UART_CONFIG_IRQ indicates autoconfiguration of the interrupt signal,
 *  which should be probed using standard kernel autoprobing techniques.
 *  This is not necessary on platforms where ports have interrupts
 *  internally hard wired (eg, system on a chip implementations).
 *
 *  Locking: none.
 *  Interrupts: caller dependent.
 */
static void sunplus_uart_ops_config_port(struct uart_port *port, int type)
{
    if (type & UART_CONFIG_TYPE) {
        port->type = PORT_SP;
        sunplus_uart_ops_request_port(port);
    }
}

/*
 *  Documentation/serial/driver:
 *  verify_port(port,serinfo)
 *  Verify the new serial port information contained within serinfo is
 *  suitable for this port type.
 *
 *  Locking: none.
 *  Interrupts: caller dependent.
 */
static int sunplus_uart_ops_verify_port(struct uart_port *port, struct serial_struct *serial)
{
    return -EINVAL;     /* Modification *serial is not allowed */
}

/*
 *  Documentation/serial/driver:
 *  ioctl(port,cmd,arg)
 *  Perform any port specific IOCTLs.  IOCTL commands must be defined
 *  using the standard numbering system found in <asm/ioctl.h>
 *
 *  Locking: none.
 *  Interrupts: caller dependent.
 */
/*
static int sunplus_uart_ops_ioctl(struct uart_port *port, unsigned int cmd, unsigned long arg)
{
}
*/

#ifdef CONFIG_CONSOLE_POLL

/*
 *  Documentation/serial/driver:
 *  poll_init(port)
 *  Called by kgdb to perform the minimal hardware initialization needed
 *  to support poll_put_char() and poll_get_char().  Unlike ->startup()
 *  this should not request interrupts.
 *
 *  Locking: tty_mutex and tty_port->mutex taken.
 *  Interrupts: n/a.
 */
static int sunplus_uart_ops_poll_init(struct uart_port *port)
{
    return 0;
}

/*
 *  Documentation/serial/driver:
 *  poll_put_char(port,ch)
 *  Called by kgdb to write a single character directly to the serial
 *  port.  It can and should block until there is space in the TX FIFO.
 *
 *  Locking: none.
 *  Interrupts: caller dependent.
 *  This call must not sleep
 */
static void sunplus_uart_ops_poll_put_char(struct uart_port *port, unsigned char data)
{
    wait_for_xmitr(port);
    spHalUartPutChar( port->line, data);
}

/*
 *  Documentation/serial/driver:
 *  poll_get_char(port)
 *  Called by kgdb to read a single character directly from the serial
 *  port.  If data is available, it should be returned; otherwise
 *  the function should return NO_POLL_CHAR immediately.
 *
 *  Locking: none.
 *  Interrupts: caller dependent.
 *  This call must not sleep
 */
static int sunplus_uart_ops_poll_get_char(struct uart_port *port)
{
    unsigned int status;
    unsigned char data;

    do {
        status = spHalUartGetLineStatus(port->line);
    } while (!(status & SP_UART_LSR_RX));

    data = spHalUartGetChar(port->line);
    return (int)data;
}

#endif /* CONFIG_CONSOLE_POLL */

static struct uart_ops sunplus_uart_ops = {
    .tx_empty                   = sunplus_uart_ops_tx_empty,
    .set_mctrl                  = sunplus_uart_ops_set_mctrl,
    .get_mctrl                  = sunplus_uart_ops_get_mctrl,
    .stop_tx                    = sunplus_uart_ops_stop_tx,
    .start_tx                   = sunplus_uart_ops_start_tx,
 // .send_xchar                 = sunplus_uart_ops_send_xchar,
    .stop_rx                    = sunplus_uart_ops_stop_rx,
    .enable_ms                  = sunplus_uart_ops_enable_ms,
    .break_ctl                  = sunplus_uart_ops_break_ctl,
    .startup                    = sunplus_uart_ops_startup,
    .shutdown                   = sunplus_uart_ops_shutdown,
 // .flush_buffer               = sunplus_uart_ops_flush_buffer,
    .set_termios                = sunplus_uart_ops_set_termios,
    .set_ldisc                  = sunplus_uart_ops_set_ldisc,
 // .pm                         = sunplus_uart_ops_pm,
 // .set_wake                   = sunplus_uart_ops_set_wake,
    .type                       = sunplus_uart_ops_type,
    .release_port               = sunplus_uart_ops_release_port,
    .request_port               = sunplus_uart_ops_request_port,
    .config_port                = sunplus_uart_ops_config_port,
    .verify_port                = sunplus_uart_ops_verify_port,
 // .ioctl                      = sunplus_uart_ops_ioctl,
#ifdef CONFIG_CONSOLE_POLL
    .poll_init                  = sunplus_uart_ops_poll_init,
    .poll_put_char              = sunplus_uart_ops_poll_put_char,
    .poll_get_char              = sunplus_uart_ops_poll_get_char,
#endif /* CONFIG_CONSOLE_POLL */
};

static struct uart_driver sunplus_uart_driver;

static struct console sunplus_console = {
    .name         = DEVICE_NAME,
    .write        = sunplus_console_write,
    .device       = uart_console_device,    /* default */
    .setup        = sunplus_console_setup,
 // .early_setup  = ,
    .flags        = CON_PRINTBUFFER,    /* CON_ENABLED, CON_CONSDEV: preferred console, CON_BOOT: primary boot console, CON_PRINTBUFFER: used for printk buffer */
    .index        = -1,
 // .cflag        = ,
    .data         = &sunplus_uart_driver
};

static struct uart_driver sunplus_uart_driver = {
    .owner          = THIS_MODULE,
    .driver_name    = "Sunplus_UART",
    .dev_name       = DEVICE_NAME,
    .major          = SP_UART_MAJOR,
    .minor          = SP_UART_MINOR_START,
    .nr             = NUM_UART,
    .cons           = &sunplus_console
  //.cons           = NULL
};

struct platform_device *sunpluse_uart_platform_device;

static void sunplus_uart_init_config(void)
{
    moon1Reg_t *pmoon1Reg = (moon1Reg_t *)LOGI_ADDR_MOON1_REG;
    u32 value_1_1, value_1_2;

    value_1_1 = pmoon1Reg->sft_cfg_1;
    value_1_2 = pmoon1Reg->sft_cfg_2;

    if(((value_1_1 >> 12) & 0x7) == 1) /* UART0 */
        uart_enable_status |= 1 << 0;
    if(((value_1_2 >> 20) & 0x7) == 1) /* UART1 */
        uart_enable_status |= 1 << 1;
    if(((value_1_2 >>  0) & 0x7) == 1) /* UART2 */
        uart_enable_status |= 1 << 2;

    /* UART3: used by eCos */

    if(((value_1_2 >> 12) & 0x3) == 1) /* UART4 */
        uart_enable_status |= 1 << 4;
    if(((value_1_1 >> 18) & 0x7) == 1) /* UART5 */
        uart_enable_status |= 1 << 5;

    // printk("%s, uart_enable_status: 0x%x\n", __FUNCTION__, uart_enable_status);
}

static int sunplus_uart_platform_driver_probe(struct platform_device *pdev)
{
    int i, ret;
    sunplus_uart_init_config();

    memset(sunplus_uart_ports, 0x00, sizeof(sunplus_uart_ports));
    for(i=0; i<NUM_UART; i++) {
        int enable = uart_enable_status & (1 << i);

        sprintf(sunplus_uart_ports[i].name, "Sunplus_UART%d", i);

        sunplus_uart_ports[i].uport.membase       = (unsigned char __iomem *)(pUartPorts[i]);
     // sunplus_uart_ports[i].uport.mapbase       = (resource_size_t)virt_to_phys((const volatile void *)(sunplus_uart_ports[i].uport.membase));    // Incorrect.
     // sunplus_uart_ports[i].uport.mapbase       = (resource_size_t)(__pa((u32)(sunplus_uart_ports[i].uport.membase)));                            // Incorrect.
        sunplus_uart_ports[i].uport.mapbase       = (resource_size_t)(((u32)(sunplus_uart_ports[i].uport.membase))-(IO0_BASE-IO0_START));
        sunplus_uart_ports[i].uport.iotype        = UPIO_MEM32;
        sunplus_uart_ports[i].uport.fifosize      = 16;
        sunplus_uart_ports[i].uport.flags         = UPF_BOOT_AUTOCONF;

        if(uart0_as_console && (uart_enable_status & 0x1))
            sunplus_uart_ports[i].uport.cons      = &sunplus_console;

        sunplus_uart_ports[i].uport.irq           = sunpluse_uart_irq[i];
        sunplus_uart_ports[i].uport.ops           = &sunplus_uart_ops;
        sunplus_uart_ports[i].uport.line          = i;
        sunplus_uart_ports[i].uport.dev           = &pdev->dev;
        sunplus_uart_ports[i].uport.private_data  = container_of(&sunplus_uart_ports[i].uport, struct sunplus_uart_port, uport);

        if(enable){
            ret = uart_add_one_port(&sunplus_uart_driver, &sunplus_uart_ports[i].uport);  /* /proc/(DEVICE_NAME[0-NUM_UART]) */
            if(ret != 0)
                return ret;
        } else {
            DBG_INFO("Keep UART%d in reset.\n", i);
        }

        #ifdef FOR_FPGA
            if(i == (NUM_UART_IN_FPGA-1))
                break;      /* Only have NUM_UART_IN_FPGA available */
        #endif /* FOR_FPGA */
    }

    platform_set_drvdata(pdev, &sunplus_uart_ports);

    return 0;
}

/*
static int sunplus_uart_platform_driver_remove(struct platform_device *pdev)
{
    return 0;
}
*/
#if defined(SUPPORT_UADMA0) || defined(SUPPORT_UADMA1)
static void sunplus_uart_dma_suspend(int uart_dma_port, int dma_id)
{
   printk("sunplus_uart_dma_suspend uart_port=%d, dma_id=%d\n", uart_dma_port, dma_id);
   pUADMA[dma_id]->rf_dma_enable_sel &= ~(1<<DMA_ENABLE);//disable DMA
   sunplus_uart_ports[uart_dma_port].reg_uadma_val.rf_dma_start_addr = pUADMA[dma_id]->rf_dma_start_addr;
   sunplus_uart_ports[uart_dma_port].reg_uadma_val.rf_dma_timeout_set = pUADMA[dma_id]->rf_dma_timeout_set;
   sunplus_uart_ports[uart_dma_port].reg_uadma_val.rf_dma_length_thr = pUADMA[dma_id]->rf_dma_length_thr;
   uadma_dump_register(dma_id);
}

static void sunplus_uart_dma_resume(int uart_dma_port, int dma_id)
{
   printk("sunplus_uart_dma_resume uart_port=%d, dma_id=%d\n", uart_dma_port, dma_id);
   UART_Initial(uart_dma_port);
   pUADMA[dma_id]->rf_dma_start_addr = sunplus_uart_ports[uart_dma_port].reg_uadma_val.rf_dma_start_addr;
   pUADMA[dma_id]->rf_dma_timeout_set = sunplus_uart_ports[uart_dma_port].reg_uadma_val.rf_dma_timeout_set;
   pUADMA[dma_id]->rf_dma_rd_adr = pUADMA[dma_id]->rf_dma_start_addr;
   pUADMA[dma_id]->rf_dma_wr_adr = pUADMA[dma_id]->rf_dma_start_addr;
   pUADMA[dma_id]->rf_dma_length_thr = sunplus_uart_ports[uart_dma_port].reg_uadma_val.rf_dma_length_thr;
   pUADMA[dma_id]->rf_dma_enable_sel |= ((uart_dma_port)<<DMA_SEL_UARTX)  |  (1<<DMA_INIT);
   pUADMA[dma_id]->rf_dma_enable_sel &= ~(1<<DMA_INIT);
	pUADMA[dma_id]->rf_dma_enable_sel |= (1<<DMA_SW_RST_B)  |
									(1<<DMA_AUTO_ENABLE)  | (1<<DMA_TIMEOUT_INT_EN)  | (0<<DMA_P_SAFE_DISABLE)  |
									(1<<DMA_ENABLE);
	pUADMA[dma_id]->rf_dma_enable_sel |= (1<<DMA_GO);

   sunplus_uart_dma_adds[dma_id].st_src_8b = (UINT8 *)gp_chunk_va(pUADMA[dma_id]->rf_dma_start_addr);
   sunplus_uart_dma_adds[dma_id].end_src_8b = (UINT8 *)gp_chunk_va(pUADMA[dma_id]->rf_dma_end_addr);
   sunplus_uart_dma_adds[dma_id].bSchedule = 0;
   spHalUartSetIntEn(uart_dma_port, SP_UART_ISC_TXM);
   uadma_dump_register(dma_id);
}
#endif

static void sunplus_uart_platform_driver_shutdown(struct platform_device *pdev)
{
    DBG_INFO("sunplus_uart_platform_driver_shutdown(): %s, %d, %p, %d, %p\n",
        pdev->name, pdev->id, &pdev->dev, pdev->num_resources, pdev->resource );
}

static int sunplus_uart_platform_driver_suspend(struct platform_device *pdev, pm_message_t state)
{
    struct sunplus_uart_port *sunplus_uart_ports_ptr;
    int i;

    DBG_INFO("sunplus_uart_platform_driver_suspend(): %s, %d, %p, %d, %p\n",
        pdev->name, pdev->id, &pdev->dev, pdev->num_resources, pdev->resource );

    sunplus_uart_ports_ptr = (struct sunplus_uart_port *)platform_get_drvdata(pdev);
    for(i=0; i<NUM_UART; i++) {
        if(uart_enable_status & (1 << i)) {
            sunplus_uart_ports_ptr->reg_val.uart_lcr    = spHalUartGetLineCtrl(i);
            sunplus_uart_ports_ptr->reg_val.uart_div_l  = spHalUartGetDividerLowRegister(i);
            sunplus_uart_ports_ptr->reg_val.uart_div_h  = spHalUartGetDividerHighRegister(i);
            sunplus_uart_ports_ptr->reg_val.uart_isc    = spHalUartGetIntEn(i);
            sunplus_uart_ports_ptr->reg_val.uart_mcr    = spHalUartGetModemCtrl(i);
            uart_suspend_port(&sunplus_uart_driver, &sunplus_uart_ports_ptr->uport);
        }

        sunplus_uart_ports_ptr++;

        #ifdef FOR_FPGA
            if(i == (NUM_UART_IN_FPGA-1))
                break;      /* Only have NUM_UART_IN_FPGA available */
        #endif /* FOR_FPGA */
    }

	#ifdef SUPPORT_UADMA0
		sunplus_uart_dma_suspend(UADMA0_PAIR_UANUM, 0);
	#endif

	#ifdef SUPPORT_UADMA1
		sunplus_uart_dma_suspend(UADMA1_PAIR_UANUM, 1);
	#endif


    return 0;
}

static int sunplus_uart_platform_driver_resume(struct platform_device *pdev)
{
    struct sunplus_uart_port *sunplus_uart_ports_ptr;
    int i;

    sunplus_uart_init_config();

    sunplus_uart_ports_ptr = (struct sunplus_uart_port *)platform_get_drvdata(pdev);
    for(i=0; i<NUM_UART; i++) {
        int enable = uart_enable_status & (1 << i);
        if(enable) {
            spHalUartSetLineCtrl(i, sunplus_uart_ports_ptr->reg_val.uart_lcr);
            spHalUartSetDividerLowRegister(i, sunplus_uart_ports_ptr->reg_val.uart_div_l);
            spHalUartSetDividerHighRegister(i, sunplus_uart_ports_ptr->reg_val.uart_div_h);
            spHalUartSetIntEn(i, sunplus_uart_ports_ptr->reg_val.uart_isc);
            spHalUartSetModemCtrl(i, sunplus_uart_ports_ptr->reg_val.uart_mcr);
            uart_resume_port(&sunplus_uart_driver, &sunplus_uart_ports_ptr->uport);
        }

        sunplus_uart_ports_ptr++;

        #ifdef FOR_FPGA
            if(i == (NUM_UART_IN_FPGA-1))
                break;      /* Only have NUM_UART_IN_FPGA available */
        #endif /* FOR_FPGA */
    }

    DBG_INFO("sunplus_uart_platform_driver_resume(): %s, %d, %p, %d, %p\n",
        pdev->name, pdev->id, &pdev->dev, pdev->num_resources, pdev->resource );

	#ifdef SUPPORT_UADMA0
		sunplus_uart_dma_resume(UADMA0_PAIR_UANUM, 0);
	#endif

	#ifdef SUPPORT_UADMA1
		sunplus_uart_dma_resume(UADMA1_PAIR_UANUM, 1);
	#endif



    return 0;
}

static struct platform_driver sunplus_uart_platform_driver = {
    .probe          = sunplus_uart_platform_driver_probe,
 // .remove         = sunplus_uart_platform_driver_remove,
    .shutdown       = sunplus_uart_platform_driver_shutdown,
    .suspend        = sunplus_uart_platform_driver_suspend,
    .resume         = sunplus_uart_platform_driver_resume,
    .driver = {
        .name       = DEVICE_NAME,
        .owner      = THIS_MODULE
    }
};

static int __init sunplus_uart_init(void)
{
    int ret;

    // printk("uart0_as_console: %x\n", uart0_as_console);
    // printk("use_thread_for_console_tx: %x\n", use_thread_for_console_tx);


    sunplus_uart_init_config();
    if(!uart0_as_console || !(uart_enable_status & 0x1))
        sunplus_uart_driver.cons = NULL;

    ret = uart_register_driver(&sunplus_uart_driver);   /* /proc/tty/driver/(sunplus_uart_driver->driver_name) */
    if(ret < 0)
        return ret;

    sunpluse_uart_platform_device = platform_device_register_simple(DEVICE_NAME, -1, NULL, 0);  /* /sys/devices/platform/(DEVICE_NAME) */
    if( IS_ERR(sunpluse_uart_platform_device) ) {
        uart_unregister_driver(&sunplus_uart_driver);
        ret = PTR_ERR(sunpluse_uart_platform_device);
        return ret;
    }

    ret = platform_driver_register(&sunplus_uart_platform_driver);
    if(ret != 0) {
        platform_device_unregister(sunpluse_uart_platform_device);
        uart_unregister_driver(&sunplus_uart_driver);
        return ret;
    }

// #ifdef USE_THREAD_FOR_CONSOLE_TX
    if (use_thread_for_console_tx) {
        task_struct_ptr = kthread_create(thread_uart_tx, NULL, "thread_uart_tx");
        if(IS_ERR(task_struct_ptr)) {
            task_struct_ptr = NULL;
            DBG_ERR("%s: %d\n", __FILE__, __LINE__);
        }
        wake_up_process(task_struct_ptr);
    }
// #endif

    return 0;
}
__initcall(sunplus_uart_init);

module_param(uart0_as_console, uint, S_IRUGO);
module_param(use_thread_for_console_tx, uint, S_IRUGO);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Sunplus Technology");
MODULE_DESCRIPTION("Sunplus UART driver");
