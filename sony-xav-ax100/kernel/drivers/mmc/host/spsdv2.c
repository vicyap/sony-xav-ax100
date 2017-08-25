/******************************************************************************
*                          Include File
*******************************************************************************/
#include <linux/module.h>
#include <linux/dma-mapping.h>
#include <linux/delay.h>
#include <linux/kernel.h>
#include <linux/string.h>
#include <linux/vmalloc.h>
#include <asm/uaccess.h>

#include <linux/mmc/sd.h>
#include <linux/mmc/core.h>

#include <linux/miscdevice.h>
#include <asm/cacheflush.h>

#include <mach/sp_clk.h>
#include <mach/gpio_drv.h>
#include <asm/bitops.h>
#include <mach/hardware.h>
#include "spsdv2.h"
#include <mach/sp_config.h> /* For retrieving SD Card Detection GPIO pin # */

/******************************************************************************
*                          MACRO Function Define
*******************************************************************************/
#define SPSD_MAX_CLOCK  CLOCK_45M     /* Max supported SD Card frequency */
#define SPSD_CLK_SOURCE CLOCK_270M    /* Host controller's clk source */
#define SPSDV2_SDC_NAME "spsdv2-sdc"
#define MAX_SDDEVICES   2

#define SPSD_READ_DELAY  3		/* delay for sampling data */
#define SPSD_WRITE_DELAY 3		/* delay for output data   */

/* Disabled fatal error messages temporarily */
static u32 loglevel = 0x000;
//static u32 loglevel = 0x001;
//static u32 loglevel = 0x033;
//static u32 loglevel = 0xefff;
//static u32 loglevel = 0xffff;

#define MMC_LOGLEVEL_FATAL		0x01
#define MMC_LOGLEVEL_ERROR		0x02
#define MMC_LOGLEVEL_DEBUG		0x04


#define MMC_LOGLEVEL_IF 		0x10
#define MMC_LOGLEVEL_PK 		0x20

#define MMC_LOGLEVEL_COUNTER	0x100
#define MMC_LOGLEVEL_WAITTIME	0x200

#define MMC_LOGLEVEL_DUMPREG	0x1000
#define MMC_LOGLEVEL_MINI		0x2000

#if 1
//#define FATAL(fmt, args...) printk(KERN_ERR "[SD FATAL]: %s: " fmt, __func__ , ## args)

#define FATAL(fmt, args...) if(unlikely(loglevel & MMC_LOGLEVEL_FATAL)) \
		printk(KERN_ERR "[SD FATAL]: %s: " fmt, __func__ , ## args)

#define EPRINTK(fmt, args...) if(unlikely(loglevel & MMC_LOGLEVEL_ERROR)) \
		printk(KERN_ERR "[SD ERROR]: %s: " fmt, __func__ , ## args)

#define DPRINTK(fmt, args...) if(unlikely(loglevel & MMC_LOGLEVEL_DEBUG)) \
		printk(KERN_INFO "[SD DBG]: %s: " fmt, __func__ , ## args)

#define IFPRINTK(fmt, args...) if(unlikely(loglevel & MMC_LOGLEVEL_IF)) \
		printk(KERN_NOTICE "[SD IF]: %s:" fmt, __func__, ## args)

#define pk(fmt, args...) if(unlikely(loglevel & MMC_LOGLEVEL_PK)) \
		printk(KERN_NOTICE "[SD PK]: %s: " fmt, __func__ , ## args)

#define CPRINTK(fmt, args...) if(unlikely(loglevel & MMC_LOGLEVEL_COUNTER)) \
		printk(KERN_INFO "[SD COUNTER]: %s: " fmt, __func__ , ## args)

#define WPRINTK(fmt, args...) if(unlikely(loglevel & MMC_LOGLEVEL_WAITTIME)) \
		printk(KERN_INFO "[SD WAITTIME]: %s: " fmt, __func__ , ## args)

#define REGPRINTK(fmt, args...) if(unlikely(loglevel & MMC_LOGLEVEL_DUMPREG)) \
		printk(KERN_INFO "[SD REG]: %s: " fmt, __func__ , ## args)

#define MPRINTK(fmt, args...) if(unlikely(loglevel & MMC_LOGLEVEL_MINI)) \
		printk(KERN_INFO "[SD]: %s: " fmt, __func__ , ## args)

#else

#define FATAL(fmt, args...)
#define EPRINTK(fmt, args...)
#define DPRINTK(fmt, args...)

#define IFPRINTK(fmt, args...)
#define pk(fmt, args...)

#define CPRINTK(fmt, args...)
#define WPRINTK(fmt, args...)


#define REGPRINTK(fmt, args...)
#define MPRINTK(fmt, args...)

#endif

/******************************************************************************
*                          Global Variables
*******************************************************************************/
struct platform_device *spsdv2_device_sdc[MAX_SDDEVICES];

/* Default GPIO pins used for SD Card detection */
/* Preserved as a fallback method if pin settings aren't found */
static unsigned int cardDetectGPIO[MAX_SDDEVICES] =
{
	8,
	15
};

/* IRQ numbers are defined statically in gemini platform's irq.h */
static unsigned int spsdv2_irq_num[MAX_SDDEVICES] =
{
	SP_IRQ_CARD_CTL0_INT,
	SP_IRQ_CARD_CTL1_INT
};

/* Virtual base address for SD Card Slot 0 & 1 registers */
static unsigned int DEV_SDCTRL_BASE_ADRS[MAX_SDDEVICES] =
{
	(IO0_BASE + (118 * 32 * 4)),
	(IO0_BASE + (125 * 32 * 4))
};

/******************************************************************************
*                         Function Prototype
*******************************************************************************/
static int spsdv2_get_dma_dir(SPSDHOST *, struct mmc_data *);
static void sphe_mmc_finish_request(SPSDHOST *, struct mmc_request *);

static int Sd_Bus_Reset_Channel(SPSDHOST *host)
{
	int counter = 0;

	host->base->rst_cnad = 1;	/*reset Central FIFO*/
	/* Wait for channel reset to complete */
	while (host->base->rst_cnad == 1) {
	}

	CPRINTK("%s, counter:%d\n", __func__, counter);
	return SPSD_SUCCESS;
}

static int Reset_Controller(SPSDHOST *host)
{
	DPRINTK("controller reset\n");
	SD_RST_seq(host->base);
	return Sd_Bus_Reset_Channel(host);
}

static void spsdv2_controller_init(SPSDHOST *host)
{
	host->base->sdddrmode = 0;
	host->base->sdpiomode = 1;
	host->base->rx4b_en = 1;
	host->base->sdiomode = 0;

	host->base->sdrsptmr = 0x7ff;
	host->base->sdrsptmren = 1;
	host->base->sdcrctmr = 0x7ff;
	host->base->sdcrctmren = 1;
	host->base->sdmmcmode = 0;
	host->base->sdrxdattmr_sel = 3;
	host->base->mediatype = 6;
}

/*
 * Set SD card clock divider value based on the required clock in HZ
 * TODO: Linux passes clock as 0, look into it
 */
static void spsdv2_set_clock(SPSDHOST *host, u32 clock)
{
	struct mmc_host *mmc = host->mmc;
	uint clkrt, sys_clk, act_clock;

	/* Check if requested frequency is above/below supported range */
	if (clock < mmc->f_min)
		clock = mmc->f_min;
	else if (clock > mmc->f_max)
		clock = mmc->f_max;

	sys_clk = sp_clk_get(SP_CLK_SDCARD);
	clkrt = (sys_clk / clock) - 1;

	/* Calculate the actual clock for the divider used */
	act_clock = sys_clk / (clkrt + 1);
	//printk("sys_clk =%u, act_clock=%u, clkrt = %u\n", sys_clk, act_clock, clkrt);
	/* check clock divider boundary and correct it */
	if (clkrt > 0xFFF)
		clkrt = 0xFFF;

	/*
	 * Switch to the requested frequency
	 * freq_divisor[11:10] = sdfreq
	 * freq_divisor[9:0] = sdfqsel
	 */
	host->base->sdfreq = (clkrt & 0xC00) >> 10;
	host->base->sdfqsel = clkrt & 0x3FF;
	//printf("clkrt %u, act_clock %u, sdfqsel %u\n", clkrt, act_clock);
	//printf("sdfqsel %u\n", host->base->sdfqsel);

	/* Delay 4 msecs for now (wait till clk stabilizes?) */
	mdelay(4);

	return;
}

/* Sets bus width accordingly */
static void spsdv2_set_bus_width(SPSDHOST *host, u32 bus_width)
{
	switch (bus_width) {
	case MMC_BUS_WIDTH_4:
		host->base->sddatawd = 1;
		break;
	case MMC_BUS_WIDTH_1:
		host->base->sddatawd = 0;
		break;
	}

	return;
}

static void spsdv2_trigger_sdstate(SPSDHOST *host)
{
	host->base->sdctrl_trigger_cmd = 1;   /* Start transaction */
}

/*
 * Receive 136 bits response, and pass it back to Linux
 * Used for cmd+rsp and normal dma requests
 * If error occurs, stop receiving response and return
 * Note: Host doesn't support Response R2 CRC error check
 */
static void spsdv2_get_rsp_136(SPSDHOST *host)
{
	struct mmc_command *cmd = host->mrq->cmd;
	unsigned int rspNum;
	unsigned char rspBuf[17] = {0}; /* Used to store 17 bytes(136 bits) or 6 bytes(48 bits) response */

	/* Receive Response */
	while (1) {
		if (host->base->sdstatus & SP_SDSTATUS_RSP_BUF_FULL)
			break;	/* Response buffers are full, break*/

		if (host->base->sdstate_new & SDSTATE_NEW_ERROR_TIMEOUT)
			return; /* Return if error occurs */
	}
	/*
	 * Store received response buffer data.
	 * Function runs to here only if no error occurs
	 */
	rspBuf[0] = host->base->sdrspbuf0;
	rspBuf[1] = host->base->sdrspbuf1;
	rspBuf[2] = host->base->sdrspbuf2;
	rspBuf[3] = host->base->sdrspbuf3;
	rspBuf[4] = host->base->sdrspbuf4;
	rspBuf[5] = host->base->sdrspbuf5;

	/* Response R2 is 136 bits long, keep receiving response by reading from rspBuf[5] */
	for (rspNum = 6; rspNum < 17; rspNum++) {
		while (1) {

			if (host->base->sdstatus & SP_SDSTATUS_RSP_BUF_FULL) {
				break;	/* Wait until response buffer full*/
			}
			if (host->base->sdstate_new & SDSTATE_NEW_ERROR_TIMEOUT)
				return;
		}
		rspBuf[rspNum] = host->base->sdrspbuf5;
	}
	/*
	 * Wait till host controller becomes idle or error occurs
	 * The host may be busy sending 8 clk cycles for the end of a request
	 */
	while (1) {
		if (host->base->sdstate_new & SDSTATE_NEW_FINISH_IDLE)
			break;
		if (host->base->sdstate_new & SDSTATE_NEW_ERROR_TIMEOUT)
			return;
	}

	/*
	 * Pass response back to Linux
	 * Function runs to here only if no error occurs
	 */
	cmd->resp[0] = (rspBuf[1] << 24) | (rspBuf[2] << 16) | (rspBuf[3] << 8) | rspBuf[4];
	cmd->resp[1] = (rspBuf[5] << 24) | (rspBuf[6] << 16) | (rspBuf[7] << 8) | rspBuf[8];
	cmd->resp[2] = (rspBuf[9] << 24) | (rspBuf[10] << 16) | (rspBuf[11] << 8) | rspBuf[12];
	cmd->resp[3] = (rspBuf[13] << 24) | (rspBuf[14] << 16) | (rspBuf[15] << 8) | rspBuf[16];

	return;
}

/*
 * Receive 48 bits response, and pass it back to Linux
 * Used for cmd+rsp and normal dma requests
 * If error occurs, stop receiving response and return
 */
static void spsdv2_get_rsp_48(SPSDHOST *host)
{
	struct mmc_command *cmd = host->mrq->cmd;
	unsigned char rspBuf[6] = {0}; /* Used to store 6 bytes(48 bits) response */

	/* Wait till host controller becomes idle or error occurs */
	while (1) {
		if (host->base->sdstate_new & SDSTATE_NEW_FINISH_IDLE)
			break;
		if (host->base->sdstate_new & SDSTATE_NEW_ERROR_TIMEOUT)
			return;
	}
	/*
	 * Store received response buffer data
	 * Function runs to here only if no error occurs
	 */
	rspBuf[0] = host->base->sdrspbuf0;
	rspBuf[1] = host->base->sdrspbuf1;
	rspBuf[2] = host->base->sdrspbuf2;
	rspBuf[3] = host->base->sdrspbuf3;
	rspBuf[4] = host->base->sdrspbuf4;
	rspBuf[5] = host->base->sdrspbuf5;

	/* Pass response back to Linux */
	cmd->resp[0] = (rspBuf[1] << 24) | (rspBuf[2] << 16) | (rspBuf[3] << 8) | rspBuf[4];
	cmd->resp[1] = rspBuf[5] << 24;

	return;
}

/*
 * Retrieve response for cmd+rsp and normal dma request
 * This function makes sure host returns to it's sdstate_new idle or error state
 * Note: Error handling should be performed afterwards
 */
static void spsdv2_get_rsp(SPSDHOST *host)
{
	struct mmc_command *cmd = host->mrq->cmd;

	if (cmd->flags & MMC_RSP_136)
		spsdv2_get_rsp_136(host);
	else
		spsdv2_get_rsp_48(host);

	return;
}

/*
 * All error cases for HW DMA read/write transactions
 * HW DMA Bug: host doesn't check CRC error for CMD17, 18, 24, 25
 * command responses, meaning it will continue to perform
 * the requested transaction.
 *
 * Side effect: if the SD card has issues receiving the command
 * correctly (CRC7 error), HW DMA's timeout will occur instead
 * of reporting the CRC7 error indicated in the response from
 * SD card.
 */
static void spsdv2_chk_err_hwDMA(SPSDHOST *host)
{
	switch (host->base->hwsd_sm) {
	/* Stop command timeout cases */
	case HWSD_SM_CMD12_TIMEOUT:
	case HWSD_W_SM_CMD12_TIMEOUT:
		EPRINTK("CMD12 timeout\n");
		host->mrq->stop->error = -ETIMEDOUT;
		break;
	/* Other command timeout cases */
	/* Send command timeout cases */
	case HWSD_SM_CMD13_TIMEOUT:
	case HWSD_W_SM_CMD13_TIMEOUT:
	case HWSD_SM_CMD17_TIMEOUT:
	case HWSD_SM_CMD18_TIMEOUT:
	case HWSD_W_SM_CMD24_TIMEOUT:
	case HWSD_W_SM_CMD25_TIMEOUT:
	/* Receive response timeout cases */
	case HWSD_SM_CMD12_RSP_TIMEOUT:
	case HWSD_W_SM_CMD12_RSP_TIMEOUT:
	case HWSD_SM_CMD13_RSP_TIMEOUT:
	case HWSD_W_SM_CMD13_RSP_TIMEOUT:
	case HWSD_W_SM_CMD24_RSP_TIMEOUT:
	case HWSD_W_SM_CMD25_RSP_TIMEOUT:
	/* CRC timeout cases */
	case HWSD_W_SM_RXCRC_TIMEOUT:
	case HWSD_W_SM_RXCRC_TX_TIMEOUT:
		EPRINTK("Command timeout!\n");
		host->mrq->cmd->error = -ETIMEDOUT;
		break;

	/* Data transfer timeout cases */
	case HWSD_SM_DMA_TIMEOUT:
	case HWSD_W_SM_DMA_TIMEOUT:
	case HWSD_SM_CMD18_BUSY_TIMEOUT:
	case HWSD_W_SM_CMD13_ST_TX_TIMEOUT:
	case HWSD_W_SM_CMD13_BUSY_ERR:
	case HWSD_SM_CMD13_BUSY_ERR:
		EPRINTK("Data error!\n");
		host->mrq->data->error = -ETIMEDOUT;
		break;

	/* Others */
	case HWSD_W_SM_CMD12_CMD_ERR:
	case HWSD_SM_CMD12_CMD_ERR:
	case HWSD_W_SM_CMD13_CMD_ERR:
	case HWSD_SM_CMD13_CMD_ERR:
	/* CRC error related */

	case HWSD_W_SM_CMD12_CRCERR:
	case HWSD_SM_CMD12_CRCERR:
	case HWSD_W_SM_RXCRC_ERR:
	case HWSD_W_SM_CMD13_CRCERR:
	case HWSD_SM_CMD13_CRCERR:
		EPRINTK("Command CRC error!\n");
		host->mrq->cmd->error = -EILSEQ;
		break;

	/* Data transfer CRC error */
	case HWSD_SM_CRC_ERR:
	case HWSD_W_SM_CRC_ERR:
		EPRINTK("Data CRC error!\n");
		host->mrq->data->error = -EILSEQ;
		break;

	/* Reset Pbus channel timeout */
	case HWSD_W_SM_RST_CHAN_TIMEOUT:
	case HWSD_SM_RST_CHAN_TIMEOUT:
		/* Pbus channel reset timeout, should somehow be handled in the future */
	default:
		/* Unknown error, tell kernel it's timeout error for now */
		EPRINTK("Unknown!\n");
		host->mrq->cmd->error = -ETIMEDOUT;
	}
}
#if 0
ETIMEDOUT    Card took too long to respond
EILSEQ       Basic format problem with the received or sent data
(e.g. CRC check failed, incorrect opcode in response or bad end bit)
#endif

static void spsdv2_check_sdstatus_errors(SPSDHOST *host)
{
	if (host->base->sdstate_new & SDSTATE_NEW_ERROR_TIMEOUT) {
		/* Response related errors */
		if (host->base->sdstatus & SP_SDSTATUS_WAIT_RSP_TIMEOUT)
			host->mrq->cmd->error = -ETIMEDOUT;
		if (host->base->sdstatus & SP_SDSTATUS_RSP_CRC7_ERROR)
			host->mrq->cmd->error = -EILSEQ;

		/* Only check the below error flags if data transaction is involved */
		if(host->mrq->data) {
			/* Data transaction related errors */
			if (host->base->sdstatus & SP_SDSTATUS_WAIT_STB_TIMEOUT)
				host->mrq->data->error = -ETIMEDOUT;
			if (host->base->sdstatus & SP_SDSTATUS_WAIT_CARD_CRC_CHECK_TIMEOUT)
				host->mrq->data->error = -ETIMEDOUT;

			if (host->base->sdstatus & SP_SDSTATUS_CRC_TOKEN_CHECK_ERROR)
				host->mrq->data->error = -EILSEQ;
			if (host->base->sdstatus & SP_SDSTATUS_RDATA_CRC16_ERROR)
				host->mrq->data->error = -EILSEQ;

			/* Reset PBus channel */
			Sd_Bus_Reset_Channel(host);
		}
	}

	return;
}

/*
 * Receive 48 bits response, and pass it back to kernel
 * Used for interrupt transactions (don't need to wait sdstate_new to become idle)
 */
static void spsdv2_get_response_48(SPSDHOST *host)
{
	unsigned char rspBuf[6] = {0}; /* Used to store 6 bytes(48 bits) response */

	/* Store received response buffer data */
	rspBuf[0] = host->base->sdrspbuf0;
	rspBuf[1] = host->base->sdrspbuf1;
	rspBuf[2] = host->base->sdrspbuf2;
	rspBuf[3] = host->base->sdrspbuf3;
	rspBuf[4] = host->base->sdrspbuf4;
	rspBuf[5] = host->base->sdrspbuf5;

	/* Pass response back to kernel */
	host->mrq->cmd->resp[0] = (rspBuf[1] << 24) | (rspBuf[2] << 16) | (rspBuf[3] << 8) | rspBuf[4];
	host->mrq->cmd->resp[1] = rspBuf[5] << 24;

	return;
}

/* Process HW DMA IRQ */
static void spsdv2_irq_hwDMA(SPSDHOST *host)
{
	struct mmc_data *data = host->mrq->data;

	//printk("incnt: %d bytes, outcnt: %d bytes", host->base->incnt, host->base->outcnt);
	if (host->base->hwsd_sm & SP_SD_HW_DMA_DONE) {
		//printk("Success!\n");
		data->bytes_xfered = data->blocks * data->blksz;
		//data->bytes_xfered = host->base->dmasize * (host->base->dma_hw_page_num[0] + 1);
		//printk("data->bytes_xfered = 0x%x\n", data->bytes_xfered);
		//printk("request size : %u\n", data->blocks * data->blksz);
		spsdv2_get_response_48(host);

		/*
		 * Host doesn't seem to have specific hardware for adjusting the timing to send
		 * CMD12 (STOP_TRANSMISSION), causing the response after reading the last data block
		 * on SD card during multi-block read to indicate out of range error in CMD12's response
		 * Since HW DMA was successful ---> clear the OUT_OF_RANGE status bit for now
		 */
		if (host->mrq->cmd->opcode == MMC_READ_MULTIPLE_BLOCK) {
			if (host->mrq->cmd->resp[0] & (1 << 31)) {
				EPRINTK("out of bounds!\n");
				host->mrq->cmd->resp[0] = host->mrq->cmd->resp[0] & 0x7fffffff;
			}
		}
	} else if (host->base->hwsd_sm & SP_SD_HW_DMA_ERROR) {
		//printk("HW DMA Error!\n");
		spsdv2_chk_err_hwDMA(host);
		Sd_Bus_Reset_Channel(host);
		data->bytes_xfered = 0;
	} else {
		printk("Flow error!\n");
		printk("host->base->hwsd_sm :0x%x\n", host->base->hwsd_sm);
	}

	host->base->dma_cmp = 0;

	return;
}

static void spsdv2_irq_normDMA(SPSDHOST *host)
{
	struct mmc_data *data = host->mrq->data;

	/* Get response */
	spsdv2_get_response_48(host);
	/* Check error flags */
	spsdv2_check_sdstatus_errors(host);

	if (host->base->sdstate_new & SDSTATE_NEW_FINISH_IDLE) {
		//data->bytes_xfered = host->base->dmasize * SD_PAGE_NUM_GET(host->base);
		data->bytes_xfered = data->blocks * data->blksz;
		//printk("bytes transferred :%u\n", hw_len[0]);
	} else {
		EPRINTK("normal DMA error!\n");
		Sd_Bus_Reset_Channel(host);
		data->bytes_xfered = 0;
	}

	host->base->sd_cmp_clr = 1;

	return;
}

static void spsdv2_irq_cmd_rsp(SPSDHOST *host)
{
	/* Get response */
	spsdv2_get_response_48(host);
	/* Check error flags */
	spsdv2_check_sdstatus_errors(host);

	host->base->sd_cmp_clr = 1;

	return;
}

/* Interrupt Handler */
irqreturn_t spsdv2_irq(int irq, void *dev_id)
{
	struct mmc_host *mmc = dev_id;

	SPSDHOST *host = (SPSDHOST *)mmc_priv(mmc);

	if (host->mrq->data != NULL) {
		switch (host->mrq->cmd->opcode) {
		case MMC_READ_SINGLE_BLOCK:
		case MMC_READ_MULTIPLE_BLOCK:
		case MMC_WRITE_BLOCK:
		case MMC_WRITE_MULTIPLE_BLOCK:
			spsdv2_irq_hwDMA(host);
			break;
		default:
			spsdv2_irq_normDMA(host);
		}
	} else { /* Cmd + Rsp(48 bits) IRQ */
		spsdv2_irq_cmd_rsp(host);
	}

	sphe_mmc_finish_request(host, host->mrq);
	return IRQ_HANDLED;
}

/*
 * 1. Releases semaphore for mmc_request
 * 2. Notifies kernel that mmc_request is done
 */
static void sphe_mmc_finish_request(SPSDHOST *host, struct mmc_request *mrq)
{
	if (mrq->data) {
		/*
		 * The original sg_len may differ after dma_map_sg function callback.
		 * When executing dma_unmap_sg, the memory segment count value returned
		 * by dma_map_sg should be used instead (value is stored in host->dma_sgcount)
		 */
		dma_unmap_sg(mmc_dev(host->mmc), mrq->data->sg,
					host->dma_sgcount,
					spsdv2_get_dma_dir(host, mrq->data));
	}
	host->mrq = NULL;

	up(&host->req_sem);
	mmc_request_done(host->mmc, mrq);
}

static int spsdv2_get_dma_dir(SPSDHOST *host, struct mmc_data *data)
{
	if (data->flags & MMC_DATA_WRITE)
		return DMA_TO_DEVICE;
	else
		return DMA_FROM_DEVICE;
}

/* Get timeout_ns from kernel, and set it to HW DMA's register */
static inline void spsdv2_set_data_timeout(SPSDHOST *host)
{
	unsigned int timeout_clks, cycle_ns;

	cycle_ns = 1000000000 / sp_clk_get(SP_CLK_SDCARD);
	timeout_clks = host->mrq->data->timeout_ns / cycle_ns;

	host->base->hw_wait_num_low = (u16)(timeout_clks & 0x0000ffff);
	host->base->hw_wait_num_high = (u16)((timeout_clks >> 16) & 0x0000ffff);
}
/*
 * Due to Host Controller's Design, it can only process 8 fragmented memory blocks of data at a time.
 * Since Linux kernel deals with PARTIAL R/W, if fragmented memory blocks exceed this limit, only perform
 * R/W on the first 8 memory blocks (pass the total size of the first 8 memory blocks as bytes_xfered to kernel)
 * Note that only 1 HW DMA Block is used (Only 1 command is sent per request)
 */
static int spsdv2_proc_hwDMA(SPSDHOST *host)
{
	struct mmc_request *mrq = host->mrq;
	int ret = SPSD_SUCCESS;
	struct mmc_data *data = mrq->data;
	int i, count = dma_map_sg(mmc_dev(host->mmc), data->sg,
							  data->sg_len,
							  spsdv2_get_dma_dir(host, data));
	struct scatterlist *sg;
	unsigned int hw_address[SP_HW_DMA_MEMORY_SECTORS] = {0}, hw_len[SP_HW_DMA_MEMORY_SECTORS] = {0};
	unsigned int xfered_bytes_cnt = 0;

	/* Store the dma_mapped memory segment count, it will be used when calling dma_unmap_sg */
	host->dma_sgcount = count;

	/* Reset */
	Reset_Controller(host);
	//host->base->sd_rst = 0x3;
	host->base->hw_dma_rst = 0x1;
	host->base->dmaidle = 0x1;
	host->base->dmaidle = 0x0;
	host->base->dma_cmp = 0x0;

	/* Configure Group SD Registers */
	host->base->hwsd_stb_en = 0x0;
	host->base->hw_sd_cmd13_en = 0x0;
	/*
	 * By checking if sending CMD12 (STOP_TRANSMISSION) is required after data transmission,
	 * we decide on what kind of HW DMA mode we should use
	 */
	if (host->mrq->stop)
		host->base->hw_sd_dma_type = 0x2;
	else
		host->base->hw_sd_dma_type = 0x1;

	host->base->hw_sd_hcsd_en = 0x1;

	/* Configure Group DMA Registers */
	if (host->mrq->data->flags & MMC_DATA_WRITE) {
		host->base->dmadst = 0x2;
		host->base->dmasrc = 0x1;
	} else {
		host->base->dmadst = 0x1;
		host->base->dmasrc = 0x2;
	}
	DMASIZE_SET(host->base, 512); //host->base->dma_size = 0x01ff;
	SDDATALEN_SET(host->base, 512); // host->base->sddatalen

	//BLOCK0_DMA_PARA_SET(host->base, mrq->cmd->arg, data_len/512); /* Sets dma_hw_page_addr & dma_hw_page_num */
	host->base->hw_dma_en = 0x1;
	host->base->hw_block_mode_en = 0x1;
	host->base->hw_block_num = 0x0; /* Only send 1 command per request */

	/* sdcmpen & dmacmpen interrupts, only 1 can be enabled at 1 time */
	host->base->sdcmpen = 0x0;
	host->base->dmacmpen_interrupt = 0x1;

	spsdv2_set_data_timeout(host); /* Sets hw_wait_num_low & hw_wait_num_high */
	host->base->hw_delay_num = 0; /* dpll clock cycle count delay between commands */

	/* retrive physical memory address & size of the fragmented memory blocks */
	for_each_sg(data->sg, sg, count, i) {
		hw_address[i] = sg_dma_address(sg);
		hw_len[i] = sg_dma_len(sg);
	}

	/* Set fragmented memory block's base address, size */
	for (i = 0; i < count && i < SP_HW_DMA_MEMORY_SECTORS; i++) {
		switch(i) {
		case 0:
		SET_HW_DMA_BASE_ADDR(host->base, hw_address[0]);
		host->base->sdram_sector_0_size = (hw_len[0] / 512) - 1;
			break;
		case 1:
		host->base->SDRAM_SECTOR_1_ADDR = hw_address[1];
		host->base->SDRAM_SECTOR_1_SIZE = (hw_len[1] / 512) - 1;
			break;
		case 2:
		host->base->SDRAM_SECTOR_2_ADDR = hw_address[2];
		host->base->SDRAM_SECTOR_2_SIZE = (hw_len[2] / 512) - 1;
			break;
		case 3:
		host->base->SDRAM_SECTOR_3_ADDR = hw_address[3];
		host->base->SDRAM_SECTOR_3_SIZE = (hw_len[3] / 512) - 1;
			break;
		case 4:
		host->base->SDRAM_SECTOR_4_ADDR = hw_address[4];
		host->base->SDRAM_SECTOR_4_SIZE = (hw_len[4] / 512) - 1;
			break;
		case 5:
		host->base->SDRAM_SECTOR_5_ADDR = hw_address[5];
		host->base->SDRAM_SECTOR_5_SIZE = (hw_len[5] / 512) - 1;
			break;
		case 6:
		host->base->SDRAM_SECTOR_6_ADDR = hw_address[6];
		host->base->SDRAM_SECTOR_6_SIZE = (hw_len[6] / 512) - 1;
			break;
		case 7:
		host->base->SDRAM_SECTOR_7_ADDR = hw_address[7];
		host->base->SDRAM_SECTOR_7_SIZE = (hw_len[7] / 512) - 1;
			break;
		default:
			printk("Error!\n");
		}
		xfered_bytes_cnt += hw_len[i];
	}

	BLOCK0_DMA_PARA_SET(host->base, mrq->cmd->arg, xfered_bytes_cnt / 512); /* Sets dma_hw_page_addr & dma_hw_page_num */

	/*
	 * mmc_test has r/w test cases which requests multi-blocks
	 * while telling host to send a single block r/w command.
	 * Normally this shouldn't occur, WARN_ON for now.
	 */
	WARN_ON((mrq->data->blocks > 1) && mrq->cmd->opcode == MMC_READ_SINGLE_BLOCK);
	WARN_ON((mrq->data->blocks > 1) && mrq->cmd->opcode == MMC_WRITE_BLOCK);

	/* Trigger host to start HW DMA request */
	host->base->dmastart = 0x1;

	return ret;
}
/*
 * DMA transfer mode, used for all other data transfer commands besides read/write block commands (cmd17, 18, 24, 25)
 * Due to host limitations, this kind of DMA transfer mode only supports 1 consecutive memory area
 */
static void spsdv2_proc_normDMA(SPSDHOST *host)
{
	struct mmc_request *mrq = host->mrq;
	struct mmc_data *data = mrq->data;
	int i, count = dma_map_sg(mmc_dev(host->mmc), data->sg,
							  data->sg_len,
							  spsdv2_get_dma_dir(host, data));
	struct scatterlist *sg;
	unsigned int hw_address[SP_HW_DMA_MEMORY_SECTORS] = {0}, hw_len[SP_HW_DMA_MEMORY_SECTORS] = {0};

	/* Store the dma_mapped memory segment count, it will be used when calling dma_unmap_sg */
	host->dma_sgcount = count;

	/* retreive physical memory address & size of the fragmented memory blocks */
	for_each_sg(data->sg, sg, count, i) {
		hw_address[i] = sg_dma_address(sg);
		hw_len[i] = sg_dma_len(sg);
	}

	/* Due to host limitations, normal DMA transfer mode only supports 1 consecutive physical memory area */
	if (count == 1) {
		/* Reset */
		Reset_Controller(host);

		/* Configure Group SD Registers */
		host->base->sd_cmdbuf[0] = (u8)(host->mrq->cmd->opcode | 0x40);	//add start bit, according to spec, command format
		host->base->sd_cmdbuf[1] = (u8)((host->mrq->cmd->arg >> 24) & 0x000000ff);
		host->base->sd_cmdbuf[2] = (u8)((host->mrq->cmd->arg >> 16) & 0x000000ff);
		host->base->sd_cmdbuf[3] = (u8)((host->mrq->cmd->arg >>  8) & 0x000000ff);
		host->base->sd_cmdbuf[4] = (u8)((host->mrq->cmd->arg >>  0) & 0x000000ff);

		SD_PAGE_NUM_SET(host->base, 1);
		if (host->mrq->cmd->flags & MMC_RSP_CRC && !(host->mrq->cmd->flags & MMC_RSP_136))
			host->base->sdrspchk_en = 0x1;
		else
			host->base->sdrspchk_en = 0x0;

		if (data->flags & MMC_DATA_READ) {
			host->base->sdcmddummy = 0;
			host->base->sdautorsp = 0;
			host->base->sd_trans_mode = 2;
		} else {
			host->base->sdcmddummy = 1;
			host->base->sdautorsp = 1;
			host->base->sd_trans_mode = 1;
		}
		host->base->sd_len_mode = 1;
		host->base->sdpiomode = 0;

		host->base->sdcrctmren = 1;
		host->base->sdrsptmren = 1;
		host->base->hw_dma_en = 0;
		/* Set response type */
		if(host->mrq->cmd->flags & MMC_RSP_136)
			host->base->sdrsptype = 0x1;
		else
			host->base->sdrsptype = 0x0;

		SDDATALEN_SET(host->base, hw_len[0]);

		/* Configure Group DMA Registers */
		if (data->flags & MMC_DATA_WRITE) {
			host->base->dmadst = 0x2;
			host->base->dmasrc = 0x1;
		} else {
			host->base->dmadst = 0x1;
			host->base->dmasrc = 0x2;
		}
		DMASIZE_SET(host->base, hw_len[0]);
		SET_HW_DMA_BASE_ADDR(host->base, hw_address[0]);

		/* Configure SD INT reg */
		/* Disable HW DMA data transfer complete interrupt (when using sdcmpen) */
		if (data->flags & MMC_DATA_WRITE) {
			host->base->dmacmpen_interrupt = 0;
			host->base->sdcmpen = 0x1;
		} else {
			host->base->dmacmpen_interrupt = 0;
			host->base->sdcmpen = 0;
		}
		/* Start Transaction */
		//spsdv2_print_regs_dma(host);
		host->base->sdctrl_trigger_cmd = 1;
	} else {
		/* Should be implemented to fallback and use PIO transfer mode in the future */
		printk("Error! SD Card DMA memory segment > 1, not supported!\n");
		data->error = -EINVAL;
		sphe_mmc_finish_request(host, host->mrq);
	}


	return;
}

/* Prepare Command + Response commands (with no data), polling mode */
static void spsdv2_prepare_cmd_rsp(SPSDHOST *host)
{
	struct mmc_request *mrq = host->mrq;

	//printk("Process Command & Response (No Data)\n");
	/* Reset */
	Reset_Controller(host);

	/* Configure Group SD Registers */
	host->base->sd_cmdbuf[0] = (u8)(mrq->cmd->opcode | 0x40);	//add start bit, according to spec, command format
	host->base->sd_cmdbuf[1] = (u8)((mrq->cmd->arg >> 24) & 0x000000ff);
	host->base->sd_cmdbuf[2] = (u8)((mrq->cmd->arg >> 16) & 0x000000ff);
	host->base->sd_cmdbuf[3] = (u8)((mrq->cmd->arg >>  8) & 0x000000ff);
	host->base->sd_cmdbuf[4] = (u8)((mrq->cmd->arg >>  0) & 0x000000ff);

	host->base->sd_trans_mode = 0x0;
	host->base->sdautorsp = 1;
	host->base->sdcmddummy = 1;

	/*
	 * Currently, host is not capable of checking Response R2's CRC7
	 * Because of this, enable response crc7 check only for 48 bit response commands
	 */
	if (mrq->cmd->flags & MMC_RSP_CRC && !(mrq->cmd->flags & MMC_RSP_136))
		host->base->sdrspchk_en = 0x1;
	else
		host->base->sdrspchk_en = 0x0;

	if (mrq->cmd->flags & MMC_RSP_136)
		host->base->sdrsptype = 0x1;
	else
		host->base->sdrsptype = 0x0;

	/* Configure SD INT reg (Disable them) */
	host->base->dmacmpen_interrupt = 0;
	host->base->sdcmpen = 0x0;

	return;
}

/* Process Command + Response commands (with no data) , interrupt mode */
static void spsdv2_mmc_proc_cmd_rsp_intr(SPSDHOST *host)
{
	struct mmc_request *mrq = host->mrq;

	DPRINTK("Process Command & Response (No Data)\n");
	/* Reset */
	Reset_Controller(host);

	/* Configure Group SD Registers */
	host->base->sd_cmdbuf[0] = (u8)(mrq->cmd->opcode | 0x40);	//add start bit, according to spec, command format
	host->base->sd_cmdbuf[1] = (u8)((mrq->cmd->arg >> 24) & 0x000000ff);
	host->base->sd_cmdbuf[2] = (u8)((mrq->cmd->arg >> 16) & 0x000000ff);
	host->base->sd_cmdbuf[3] = (u8)((mrq->cmd->arg >>  8) & 0x000000ff);
	host->base->sd_cmdbuf[4] = (u8)((mrq->cmd->arg >>  0) & 0x000000ff);

	host->base->sd_trans_mode = 0x0;
	host->base->sdautorsp = 1;
	host->base->sdcmddummy = 1;

	/* Currently, host is not capable of checking Response R2's CRC7
	   Because of this, enable response crc7 check only for 48 bit response commands
	 */
	if (mrq->cmd->flags & MMC_RSP_CRC && !(mrq->cmd->flags & MMC_RSP_136))
		host->base->sdrspchk_en = 0x1;
	else
		host->base->sdrspchk_en = 0x0;

	if (mrq->cmd->flags & MMC_RSP_136)
		host->base->sdrsptype = 0x1;
	else
		host->base->sdrsptype = 0x0;

	/* Configure SD INT reg */
	/* Disable HW DMA data transfer complete interrupt (when using sdcmpen) */
	host->base->dmacmpen_interrupt = 0;
	host->base->sdcmpen = 0x1;

	/* Start Transaction */
	host->base->sdctrl_trigger_cmd = 1;

	return;
}

/* Process Command + No Response commands (with no data) */
static void spsdv2_mmc_proc_cmd(SPSDHOST *host)
{
	struct mmc_request *mrq = host->mrq;

	//printk("Process cmd (no rsp)\n");
	/* Reset */
	Reset_Controller(host);

	/* Configure Group SD Registers */
	host->base->sd_cmdbuf[0] = (u8)(mrq->cmd->opcode | 0x40);	//add start bit, according to spec, command format
	host->base->sd_cmdbuf[1] = (u8)((mrq->cmd->arg >> 24) & 0x000000ff);
	host->base->sd_cmdbuf[2] = (u8)((mrq->cmd->arg >> 16) & 0x000000ff);
	host->base->sd_cmdbuf[3] = (u8)((mrq->cmd->arg >>  8) & 0x000000ff);
	host->base->sd_cmdbuf[4] = (u8)((mrq->cmd->arg >>  0) & 0x000000ff);

	host->base->sd_trans_mode = 0x0;
	host->base->sdautorsp = 0;
	host->base->sdcmddummy = 1;

	/* Configure SD INT reg */
	/* Disable HW DMA data transfer complete interrupt (when using sdcmpen) */
	host->base->dmacmpen_interrupt = 0;
	host->base->sdcmpen = 0x0;

	/* Start Transaction */
	host->base->sdctrl_trigger_cmd = 1;

	while((host->base->sdstate_new & SDSTATE_NEW_FINISH_IDLE) != 0x40) {
	//while(host->base->hwsd_sm != SP_SD_HW_DMA_DONE && host->base->hwsd_sm != SP_SD_HW_DMA_ERROR) {
		//printk("Waiting! sd_hw_state : 0x%x   LMST_SM:0x%x   Data In Counter :%u Data Out Counter: %u\n", host->base->hwsd_sm, host->base->lmst_sm, host->base->incnt, host->base->outcnt);
		//printk("sd status:0x%x, state:0x%x, state new:0x%x\n", host->base->sdstatus, host->base->sdstate, host->base->sdstate_new);
		//printk("Waiting\n");
	}

	sphe_mmc_finish_request(host, host->mrq);
}

/* Single read operation timeout: The typical delay between "the end
 * bit of the read command" and "the start bit of the data block"
 * Since SD Host's read data start bit timer is broken, use jiffies to
 * count "the whole transaction's timeout" instead
 */
static void spsdv2_post_normDMA_poll(SPSDHOST *host) {
	unsigned long timeout_ms = 0;
	unsigned long deadline = 0;
	int timesup = 0; /* Used to determine if timeout occurred */

	/* Check if timeout is smaller than 1 msec */
	if (host->mrq->data->timeout_ns / 1000000)
		timeout_ms = host->mrq->data->timeout_ns / 1000000;
	else
		timeout_ms = 1;

	/* timeout_ms value is set to 3 times the value given to us via Linux for now */
	timeout_ms = timeout_ms * 3;

	deadline = jiffies + msecs_to_jiffies(timeout_ms);

	/* Wait till host controller becomes idle or error/timeout occurs */
	while ((host->base->sdstate_new
	       & (SDSTATE_NEW_FINISH_IDLE | SDSTATE_NEW_ERROR_TIMEOUT)) == 0) {
		if (time_after(jiffies, deadline)) {
			timesup = 1;
			break;
		}
	}

	/* If software timeout occured, deal with it */
	if (timesup) {
		host->mrq->data->error = -ETIMEDOUT;
	} else
		spsdv2_irq_normDMA(host);

	return;
}

/* For mmc_requests
 * ================ Data transfer requests ===========================
 * 1. Data transfer WRITE requests : use interrupt mode
 * 2. Data block transfer READ requests (CMD17, CMD18) : use interrupt mode
 * 3. Other data transfer READ requests : use polling mode
 *    (normal DMA's read data start bit timeout counter is broken)
 * ================= Non data transfer requests =======================
 * 1. Command + Response (48 bit response) requests : use interrupt mode
 * 2. Command + Response (136 bit response) requests : use polling mode
 * 3. Command (no response) requests : use polling mode
 */
void spsdv2_mmc_request(struct mmc_host *mmc, struct mmc_request *mrq)
{
	SPSDHOST *host = mmc_priv(mmc);

	IFPRINTK("\n<----- mrq:0x%x, cmd:%d, arg:0x%08x, data len:%d, stop:0x%x\n",
		(unsigned int)mrq, mrq->cmd->opcode&0xfff, mrq->cmd->arg,
		(mrq->data)?(mrq->data->blocks*mrq->data->blksz):0, (unsigned int)mrq->stop);

	/*
	 * The below semaphore is released when "sphe_mmc_finish_request" is called
	 * TODO: Observe if the below semaphore is necessary
	 */
	down(&host->req_sem);

	host->mrq = mrq;
	DPRINTK("host->mrq->cmd->opcode:%d\n", host->mrq->cmd->opcode);

	if (host->mrq->data == NULL) {
		if (host->mrq->cmd->flags & MMC_RSP_PRESENT) {
			if (host->mrq->cmd->flags & MMC_RSP_136) {
				spsdv2_prepare_cmd_rsp(host);
				spsdv2_trigger_sdstate(host);
				spsdv2_get_rsp(host); /* Makes sure host returns to an idle or error state */
				spsdv2_check_sdstatus_errors(host);
				sphe_mmc_finish_request(host, host->mrq);
			} else
				spsdv2_mmc_proc_cmd_rsp_intr(host);
		} else
			spsdv2_mmc_proc_cmd(host);
	} else {
		switch (host->mrq->cmd->opcode) {
		case MMC_READ_SINGLE_BLOCK:
		case MMC_READ_MULTIPLE_BLOCK:
		case MMC_WRITE_BLOCK:
		case MMC_WRITE_MULTIPLE_BLOCK:
			spsdv2_proc_hwDMA(host);
			break;
		default:
			spsdv2_proc_normDMA(host);
			/* Normal DMA read: polling */
			if (host->mrq->data->flags & MMC_DATA_READ) {
				spsdv2_post_normDMA_poll(host);
				sphe_mmc_finish_request(host, host->mrq);
			}
		}
	}

	return;
}

/* set_ios -
 * 1) Set/Disable clock
 * 2) Power on/off to offer SD card or not
 * 3) Set SD Card Bus width to 1 or 4
 */
void spsdv2_mmc_set_ios(struct mmc_host *mmc, struct mmc_ios *ios)
{
	SPSDHOST *host = (SPSDHOST *)mmc_priv(mmc);

	IFPRINTK("\n<----- sd%d: clk:%d, buswidth:%d(2^n), bus_mode:%d, powermode:%d(0:off, 1:up, 2:on), timing:%d\n",
		host->id, ios->clock, ios->bus_width, ios->bus_mode, ios->power_mode, ios->timing);

	down(&host->req_sem);

	/* TODO: Cleanup power_mode functions */
	switch (ios->power_mode) {
	//power off->up->on
	case MMC_POWER_ON:
		DPRINTK("set MMC_POWER_ON\n");
		spsdv2_controller_init(host);
		break;
	case MMC_POWER_UP:
		/* Set default control register */
		DPRINTK("set MMC_POWER_UP\n");
		Reset_Controller(host);
		break;
	case MMC_POWER_OFF:
		Reset_Controller(host);
		DPRINTK("set MMC_POWER_OFF\n");
		break;
	}

	spsdv2_set_clock(host, ios->clock);
	spsdv2_set_bus_width(host, ios->bus_width);

	/* Write delay: Controls CMD, DATA signals timing to SD Card */
	host->base->sd_wr_dly_sel = SPSD_WRITE_DELAY;
	/* Read delay: Controls timing to sample SD card's CMD, DATA signals */
	host->base->sd_rd_dly_sel = SPSD_READ_DELAY;

	/*
	 * Host adjusts the data sampling edge depending on the
	 * speed mode used.
	 */
	if (ios->timing == MMC_TIMING_SD_HS)
		host->base->sd_high_speed_en = 1;
	else
		host->base->sd_high_speed_en = 0;

	up(&host->req_sem);
	IFPRINTK("----- \n\n");

	return;
}

int spsdv2_mmc_read_only(struct mmc_host *mmc)
{
	//return  > 0 :support read only
	//        < 0 :not support RO
	//        = 0 :no action
	return -ENOSYS;
}

/*
 * Return values for the get_cd callback should be:
 *   0 for a absent card
 *   1 for a present card
 *   -ENOSYS when not supported (equal to NULL callback)
 *   or a negative errno value when something bad happened
 */
int spsdv2_mmc_card_detect(struct mmc_host *mmc)
{
	SPSDHOST *host = mmc_priv(mmc);
	IFPRINTK("<----- \n");

	//printk("host->InsertGPIO :%d\n", host->InsertGPIO);
	if (GPIO_I_GET(host->InsertGPIO) == 0) {
		//printk("Card Plugged in!\n");
		return 1;
	} else {
		//printk("Card absent!\n");
		return 0;
	}
}

const struct mmc_host_ops spsdv2_sdc_ops = {
	.request = spsdv2_mmc_request,
	.set_ios = spsdv2_mmc_set_ios,
	.get_ro = spsdv2_mmc_read_only,
	.get_cd = spsdv2_mmc_card_detect,
};

int spsdv2_drv_probe(struct platform_device *pdev)
{
	int ret, gpioPin;
	struct mmc_host *mmc = platform_get_drvdata(pdev);
	SPSDHOST *host = (SPSDHOST *)mmc_priv(mmc);
	/* Names of card detection GPIO pin # configs found in runtime.cfg */
	char *cardDetectCfg[MAX_SDDEVICES] = {"gpioSD0Sen", "gpioSD1Sen"};

	DPRINTK("SD card driver probe, sd %d, base:0x%x, host size:%d\n", pdev->id, DEV_SDCTRL_BASE_ADRS[pdev->id], sizeof(SPSDHOST));

	mmc = mmc_alloc_host(sizeof(SPSDHOST), &pdev->dev);
	if (!mmc) {
		ret = -ENOMEM;
		goto probe_free_host;
	}

	host = (SPSDHOST *)mmc_priv(mmc);
	host->mmc = mmc;
	host->pdev = pdev;

	host->id = pdev->id;

	sema_init(&host->req_sem, 1);

	host->base = (struct spsd_general_regs *)DEV_SDCTRL_BASE_ADRS[host->id];

	host->irq = spsdv2_irq_num[host->id];

	/*
	 * Retrieve card detection gpio settings from configs (found in application/configs/xxx_cfg/runtime.cfg)
	 * If settings weren't found, fallback and use default gpio settings (Used on EMU Board V1.0)
	 */
	if (sp_config_get_int(cardDetectCfg[host->id], &gpioPin) == SP_CFG_RET_SUCCESS) {
		printk("[SDCard] Slot %d, using GPIO #%d for card detection\n", host->id, gpioPin);
		host->InsertGPIO = gpioPin;
	} else {
		printk("[SDCard] Error! Slot %d detect pin settings not found! Using default GPIO pin #%d\n", host->id, cardDetectGPIO[host->id]);
		host->InsertGPIO = cardDetectGPIO[host->id];
	}

	if (request_irq(host->irq, spsdv2_irq, IRQF_SHARED, dev_name(&pdev->dev), mmc)) {
		printk("\nFailed to request sd card interrupt.\n");
		ret = -ENOENT;
		goto probe_free_host;
	}

	DPRINTK("[SDCard] Slot %d driver probe, host:0x%x, base:0x%x\n", host->id, (unsigned int)host,(unsigned int)host->base);

	mmc->ops = &spsdv2_sdc_ops;

	/*
	 * freq_divisor[11:10] = sdfreq[1:0]
	 * freq_divisor[9:0] = sdfqsel[9:0]
	 */
	mmc->f_min = (SPSD_CLK_SOURCE / (0x0FFF + 1));
	mmc->f_max = SPSD_MAX_CLOCK;

	mmc->ocr_avail = MMC_VDD_32_33 | MMC_VDD_33_34;

	mmc->max_seg_size = 65536 * 512;            /* Size per segment is limited via host controller's
	                                               ((sdram_sector_#_size max value) * 512) */
	mmc->max_segs = SP_HW_DMA_MEMORY_SECTORS;   /* Host controller supports up to "SP_HW_DMA_MEMORY_SECTORS",
	                                               a.k.a. max scattered memory segments per request */
	mmc->max_req_size = 65536 * 512;			/* Decided by hw_page_num0 * SDHC's blk size */
	mmc->max_blk_size = 512;                   /* Limited by host's dma_size & data_length max value, set it to 512 bytes for now */
	mmc->max_blk_count = 65536;                 /* Limited by sdram_sector_#_size max value */

	mmc->caps = MMC_CAP_4_BIT_DATA | MMC_CAP_SD_HIGHSPEED | MMC_CAP_NEEDS_POLL;
	mmc->caps2 |= MMC_CAP2_DETECT_ON_ERR;		/* Check card removal upon I/O err */

	dev_set_drvdata(&pdev->dev, mmc);

	mmc_add_host(mmc);

	return 0;

probe_free_host:
	if (mmc)
		mmc_free_host(mmc);

	return 0;
}

int spsdv2_drv_remove(struct platform_device *dev)
{
	struct mmc_host *mmc = platform_get_drvdata(dev);
	SPSDHOST *host = (SPSDHOST *)mmc_priv(mmc);

	DPRINTK("Remove sd card\n");
	platform_set_drvdata(dev, NULL);

	mmc_remove_host(mmc);
	free_irq(host->irq, mmc);
	mmc_free_host(mmc);

 	return 0;
}

struct platform_driver spsdv2_driver_sdc = {
	.probe = spsdv2_drv_probe,
	.remove = spsdv2_drv_remove,
	.shutdown = NULL,
//	.suspend = spsdv2_drv_suspend,
//	.resume = spsdv2_drv_resume,
	.driver.name = SPSDV2_SDC_NAME,
	.driver.owner = THIS_MODULE,
};


#ifdef CONFIG_PM

int spsdv2_pm_suspend(struct device *dev)
{
	struct mmc_host *mmc = platform_get_drvdata(to_platform_device(dev));
	SPSDHOST *host = (SPSDHOST *)mmc_priv(mmc);

	printk("Sunplus SD %d driver suspend.\n", host->id);
	down(&host->req_sem);
	up(&host->req_sem);

	mmc_suspend_host(mmc);

	return 0;
}

int spsdv2_pm_resume(struct device *dev)
{
	struct mmc_host *mmc = platform_get_drvdata(to_platform_device(dev));
	SPSDHOST *host = (SPSDHOST *)mmc_priv(mmc);

	printk("Sunplus SD%d driver resume.\n", host->id);
	mmc_resume_host(mmc);

	return 0;
}

const struct dev_pm_ops sphe_mmc2_pm_ops = {
	.suspend	= spsdv2_pm_suspend,
	.resume		= spsdv2_pm_resume,
};
#endif


int spsdv2_drv_suspend(struct platform_device *dev)
{
	struct mmc_host *mmc = platform_get_drvdata(dev);
	SPSDHOST *host = (SPSDHOST *)mmc_priv(mmc);

	printk("Sunplus SD %d driver suspend.\n", host->id);
	down(&host->req_sem);
	up(&host->req_sem);

	mmc_suspend_host(mmc);

	return 0;
}

int spsdv2_drv_resume(struct platform_device *dev)
{
	struct mmc_host *mmc = platform_get_drvdata(dev);
	SPSDHOST *host = (SPSDHOST *)mmc_priv(mmc);

	printk("Sunplus SD%d driver resume.\n", host->id);
	mmc_resume_host(mmc);

	return 0;
}

/* Check pinmux and decide whether or not SD Card Slot should be enabled */
static int spsdv2_check_pinmux(int slot_enable[])
{
	int i;
	u32 pmx_card_sd[2] = {0}, pmx_card_hb[2] = {0};
	int pmx_card_sd_ret[2] = {0}, pmx_card_hb_ret[2] = {0};

	/* For each slot, SD card requires 2 sets of pinmux to be switched */
	pmx_card_sd_ret[0] = gpio_pin_mux_get(PMX_CARD0_SD, &pmx_card_sd[0]);
	pmx_card_hb_ret[0] = gpio_pin_mux_get(PMX_CARD0_HB, &pmx_card_hb[0]);
	pmx_card_sd_ret[1] = gpio_pin_mux_get(PMX_CARD1_SD, &pmx_card_sd[1]);
	pmx_card_hb_ret[1] = gpio_pin_mux_get(PMX_CARD1_SD_HB, &pmx_card_hb[1]);

	/* Check if pinmux for each slot is set */
	for (i = 0; i < MAX_SDDEVICES; i++) {
		if ((pmx_card_sd_ret[i] == 0) && (pmx_card_sd[i] != 0) && (pmx_card_hb_ret[i] == 0) && (pmx_card_hb[i] != 0))
			slot_enable[i] = 1;
		else
			slot_enable[i] = 0;
	}

	/* Don't register driver if pinmux for SD Card slot's aren't set */
	if (slot_enable[0] == 0 && slot_enable[1] == 0) {
		printk("[SDCard] Warning! Pinmux for both SD Card slots aren't set\n");
		return 1;
	} else
		return 0;
}

int __init spsdv2_sdc_init(void)
{
	int ret;
	int i=0;
	/* Below variables are used to indicate if pinmux for SD card slots are enabled */
	unsigned int slot_enable[2] = {0};

	/* Check pinmux settings to see if driver needs to be enabled */
	if (spsdv2_check_pinmux(slot_enable))
		return 0;

	DPRINTK(KERN_INFO "Init SPSD SDC Driver, register size:%d\n", sizeof(SDREG));

	sp_clk_set(SP_CLK_SDCARD, SPSD_CLK_SOURCE);

	ret = platform_driver_register(&spsdv2_driver_sdc);
	if (ret < 0) {
		EPRINTK("Register sd card driver fail\n");
		return ret;
	}
	DPRINTK(KERN_INFO "platform_driver_register\n");

	for (i = 0; i < MAX_SDDEVICES; i++) {
		struct platform_device *pdev;
		/* If pinmux not set, don't enable SD Card slot */
		if (slot_enable[i] == 0) {
			printk("[SDCard] Pinmux for Slot %d is not set\n", i);
			continue;
		} else
			printk("[SDCard] Pinmux for Slot %d is set\n", i);

		pdev = platform_device_alloc(SPSDV2_SDC_NAME, i);

		if (!pdev) {	/*-1means device has only one*/
			EPRINTK("Alloc sd card device fail\n");
			platform_driver_unregister(&spsdv2_driver_sdc);
			return -ENOMEM;
		}
		ret = platform_device_add(pdev);
		if (ret) {
			EPRINTK("Add sd card device fail\n");

			platform_device_put(pdev);
			platform_driver_unregister(&spsdv2_driver_sdc);
			return ret;
		}

		spsdv2_device_sdc[i] = pdev;
	}

	return 0;
}

void __exit spsdv2_sdc_exit(void)
{
	int i=0;
	struct platform_device *pdev;

	DPRINTK(KERN_INFO "Exit SDC Driver\n");
	for (i = 0; i < MAX_SDDEVICES; i++) {
		pdev = spsdv2_device_sdc[i];
		/* If a certain slot isn't enabled during initialization, skip device unregister */
		if (pdev == NULL)
			continue;

		platform_device_unregister(pdev);
	}
	platform_driver_unregister(&spsdv2_driver_sdc);

}

module_init(spsdv2_sdc_init);
module_exit(spsdv2_sdc_exit);

MODULE_AUTHOR("letb B1");
MODULE_DESCRIPTION("letb MMC/SD Card Interface Driver");
MODULE_LICENSE("GPL v2");
