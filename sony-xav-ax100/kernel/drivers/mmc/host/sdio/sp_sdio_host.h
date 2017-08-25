#ifndef __SDIO_HOST_H__
#define __SDIO_HOST_H__

#include <asm/hardware/types.h>
#include <asm/cacheflush.h>
#include <asm/page.h>
#include <asm/mutex.h>

#include <linux/interrupt.h>
#include <linux/mmc/host.h>
#include <linux/swab.h>
#include <linux/slab.h>
#include <linux/types.h>
#include <linux/semaphore.h>
#include <linux/scatterlist.h>
#include <linux/delay.h>
#include <linux/platform_device.h>
#include <../arch/arm/mach-gemini/include/mach/sp_clk.h>
/******************************************************************************
 *                          MACRO Function Define
 *******************************************************************************/

#define MMC_PRINTF_ERROR 	0x01
#define MMC_PRINTF_FLOW 	0x02
#define MMC_PRINTF_DETAIL 	0x04
#define MMC_PRINTF_IF 		0x10
#define MMC_PRINTF_PK 		0x20

#define MMC_PRINTF_COUNTER	0x100
#define MMC_PRINTF_WAITTIME	0x200

#define SDIO_DPLL_CLK  sp_clk_get(SP_CLK_SDIO)
#define SPSD_MAX_CLOCK SDIO_DPLL_CLK
#define SDIO_CLK_DIV    	  4
#define	SDC_MODULE_ENABLE 1
#define letb8388_SDIO_BLOCK_SIZE			512
#define SDIO_MAX_TUNE_DLY_TIMES             9
#define	SDIO_MAX_RETRY_TIME                 3 

//#define SDIO_DEBUG
#ifdef  SDIO_DEBUG
#define PARSE()   while(1)
#else
#define PARSE()   
#endif

#define  ASSERT_GOTO(cond, label, info, arg...) \
	do{\
		if(unlikely(cond))\
		{\
			EPRINTK(info,##arg);\
			debug_get_register_statues();\
			goto label;\
		}\
	}while(0)

#if 1

#define EPRINTK(fmt, args...) if(unlikely(mmc_printk & MMC_PRINTF_ERROR)) \
	printk(KERN_ERR "[SDIO ERROR]: %s: " fmt, __func__ , ## args)

#define FPRINTK(fmt, args...) if(unlikely(mmc_printk & MMC_PRINTF_FLOW)) \
	printk(KERN_INFO fmt,  ## args)

#define DPRINTK(fmt, args...) if(unlikely(mmc_printk & MMC_PRINTF_DETAIL)) \
	printk(KERN_INFO "[SDIO DETAIL]: %s: " fmt, __func__ , ## args)

#define IFPRINTK(fmt, args...) if(unlikely(mmc_printk & MMC_PRINTF_IF)) \
	printk(KERN_INFO "[SDIO IF]: %s: " fmt, __func__ , ## args)

#define pk(fmt, args...) if(unlikely(mmc_printk & MMC_PRINTF_PK)) \
	printk(KERN_NOTICE "[SDIO PK]: %s: " fmt, __func__ , ## args)

#define CPRINTK(fmt, args...) if(unlikely(mmc_printk & MMC_PRINTF_COUNTER)) \
	printk(KERN_INFO "[SDIO Counter]: %s: " fmt, __func__ , ## args)

#define WPRINTK(fmt, args...) if(unlikely(mmc_printk & MMC_PRINTF_WAITTIME)) \
	printk(KERN_INFO "[SDIO WAITTIME]: %s: " fmt, __func__ , ## args)
#endif

#define CARD_ACCELERATE_EN 0  /*enable hw DMA accelerate */

#define HW_BLOCK_MODE_EN   0   /*1  means  hw DMA full  accelerate
								*0  means  hw DMA half accelerate
								*/
#ifndef 	swab32
#define swab32(x) \
	((unsigned int)( \
		(((unsigned int)(x) & (unsigned int)0x000000ffUL) << 24) | \
		(((unsigned int)(x) & (unsigned int)0x0000ff00UL) <<  8) | \
		(((unsigned int)(x) & (unsigned int)0x00ff0000UL) >>  8) | \
		(((unsigned int)(x) & (unsigned int)0xff000000UL) >> 24) ))
#endif

#define DEF_RD_DLY     0
#define DEF_WR_DLY     0
#define DEF_CLK_DLY    0

#define SetSd0MediaTypeMode set_sd_media_type_mode
#define hwSd0IdleWait controller_state_idle_wait
#define hwSd0Init hwSdInit
#define hwSd0DmaInit hwSdDmaInit
#define hwSd0DmaStart hwSdDmaStart
#define Sd0TranStateWait SdTranStateWait
#define Sd0ReadyBitWait SdCardReadyBitWait
#define Sd0StateCheck SdStateCheck

#define SDIO_DMA_MODE  0
#define SDIO_PIO_MODE  1

#define HW_SD_PIO_BUF_SIZE 	   2
#define HW_SDIO_BLK_SIZE       512

#define letb8388_SDIO_NAME  "sphe8388-sdio" 
#define REQEUUEST_USE_SPIN_LOCK 
#ifdef  	REQEUUEST_USE_SPIN_LOCK
#define letb8388_SDIO_LOCK(lock, flag)       spin_lock_irqsave(lock, flag)  
#else
#define letb8388_SDIO_LOCK(lock, flag)
#endif
//down(lock)
#ifdef  	REQEUUEST_USE_SPIN_LOCK
#define letb8388_SDIO_UNLOCK(lock, flag)    spin_unlock_irqrestore(lock, flag)//up(lock) 
#else
#define letb8388_SDIO_UNLOCK(lock, flag) 
#endif
#define WAIT_CMD_XFER_TIME   4*HZ
#define SD_CLK_DIV_FACTOR    (SDIO_DPLL_CLK/CLOCK_400K - 1) /* set to 400kHz */

#define SD_SET_INT_STATE(lock, dst_state, src_state)  \
	do{\
		dst_state = src_state;\
	}while(0)


/******************************************************************************
 *                          D A T A    T Y P E S
 ******************************************************************************/

enum  SD_INT_STATE
{
	SD_INT_INIT,
	SD_INT_DATA_XFER_CMP_DMA, 		// n*datalen bytes Data Transfer Complete for data in DMA mode

	SD_INT_DATA_XFER_CMP_PIO_ERR,		// n*datalen bytes Data Transfer Complete for data in PIO mode
	SD_INT_DATA_XFER_CMP_PIO,		// n*datalen bytes Data Transfer Complete for data in PIO mode
	SD_INT_ONLY_CMD_XFER_CMP,		// Cmd Transfer Complete for only cmd
	SD_INT_CMD_RSP_XFER_CMP,		// Cmd & Rsp Transfer Complete for cmd with rsp
	SD_INT_CMD_BEFORE_RSP_FULL,		// state at send cmd but rsp buffer not full
	SD_INT_ERR_TIMEOUT,				// Error or timeout
	SD_INT_OPERRATION_SUSPENDED,	// Operation suspended(for SDIO Read Wait orSuspend flow)
};

typedef struct sd_int_info_t
{
	enum  SD_INT_STATE irq_sd_stat;// the coming  INT type in the future 
	wait_queue_head_t wq_sd_irq;
	//spinlock_t lock_Sd_irq;
}sd_int_info_t;

typedef struct dma_int_info_t
{
	u32 irq_dma_completed;
	wait_queue_head_t wq_irq_Sd_dma;
	spinlock_t lock_Sd_irq_dma;
}dma_int_info_t;


#define SDIO_STATE_ENABLE   1
#define SDIO_STATE_DISABLE  0
typedef struct sdio_int_info_t
{
	spinlock_t lock;
	u32 		state_sdio_int;
}sdio_int_info_t;

typedef struct int_info_t
{
	int irq;// the three interrupt share one handle 
	sdio_int_info_t sdio_int;
	//dma_int_info_t   dma_int;
	sd_int_info_t    sd_int;
	spinlock_t lock;
}int_info_t;


typedef struct clk_dly_into_t{
	u32 tune_begin_flag;		// the flag of tune rd_dly wr_dly and clk_dly
	u32 clock_delay;
	u32 write_delay;	
	u32 read_delay;  
}clk_dly_into_t;


typedef struct sphe8388_sdio_priv_host {
	struct platform_device *pdev;
	struct mmc_host *mmc;
	struct resource *mem;
	struct mmc_data *data;
	struct mmc_command *cmd;
	void __iomem *base;

	s32 irq;
	s32 irq_cd;
	s32 dma;
	struct mmc_request *mrq;
	s32 cmd_is_stop;
	
#ifdef  	REQEUUEST_USE_SPIN_LOCK
	spinlock_t lock;
#endif	
	//struct semaphore lock;
	struct mutex data_lock;
	unsigned long flag;
	struct semaphore sem;
	int bus_width;
	u32 clk;
	u8 power_mode;
	s32 highspeed_flag;
	int_info_t  interrupt;
	clk_dly_into_t clk_dly_info;
}sphe8388_sdio_priv_host;



#endif //  #ifndef __SDIO_HOST_H__

