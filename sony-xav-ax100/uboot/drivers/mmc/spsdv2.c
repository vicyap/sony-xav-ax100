/*
 * This is a driver for the SDHC controller found in Sunplus Gemini SoC
 *
 * Enable CONFIG_MMC_TRACE in uboot configs to trace SD card requests
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <config.h>
#include <common.h>
#include <command.h>
#include <mmc.h>
#include <malloc.h>
#include "spsdv2.h"
#include <asm/arch/hardware.h> /* For definition SPHE_DEVICE_BASE */
#include <sp_config.h>
#include <drivers/gpio/sp_gpio.h>
#include <asm/arch/regmap.h> /* For MOON1_REG macro */
#include <sp_bootinfo.h> /* For ISP from SD Card */

#define RGST_OFFSET	SPHE_DEVICE_BASE
#define MAX_SDDEVICES   2

#define SPSD_CLK_SRC CLOCK_270M    /* Host controller's clk source */
#define SPSD_MAX_CLK CLOCK_45M     /* Max supported SD Card frequency */
/*
 * Freq. for identification mode(SD Spec): 100~400kHz
 * U-Boot uses the host's minimal declared freq. directly
 * Set it to ~150kHz for now
 */
#define SPSD_MIN_CLK (((SPSD_CLK_SRC / (0xFFF + 1)) > CLOCK_150K) \
                      ? (SPSD_CLK_SRC / (0xFFF + 1)) \
                      : CLOCK_150K)

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
		printf(KERN_ERR "[SD FATAL]: %s: " fmt, __func__ , ## args)

#define EPRINTK(fmt, args...) if(unlikely(loglevel & MMC_LOGLEVEL_ERROR)) \
		printf("[SD ERROR]: %s: " fmt, __func__ , ## args)

#define DPRINTK(fmt, args...) if(unlikely(loglevel & MMC_LOGLEVEL_DEBUG)) \
		printf("[SD DBG]: %s: " fmt, __func__ , ## args)

#define IFPRINTK(fmt, args...) if(unlikely(loglevel & MMC_LOGLEVEL_IF)) \
		printf("[SD IF]: %s:" fmt, __func__, ## args)

#define pk(fmt, args...) if(unlikely(loglevel & MMC_LOGLEVEL_PK)) \
		printf("[SD PK]: %s: " fmt, __func__ , ## args)

#define CPRINTK(fmt, args...) if(unlikely(loglevel & MMC_LOGLEVEL_COUNTER)) \
		printf("[SD COUNTER]: %s: " fmt, __func__ , ## args)

#define WPRINTK(fmt, args...) if(unlikely(loglevel & MMC_LOGLEVEL_WAITTIME)) \
		printf("[SD WAITTIME]: %s: " fmt, __func__ , ## args)

#define REGPRINTK(fmt, args...) if(unlikely(loglevel & MMC_LOGLEVEL_DUMPREG)) \
		printf("[SD REG]: %s: " fmt, __func__ , ## args)

#define MPRINTK(fmt, args...) if(unlikely(loglevel & MMC_LOGLEVEL_MINI)) \
		printf("[SD]: %s: " fmt, __func__ , ## args)

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

/*
 * Default GPIO pins used for SD Card detection
 * Preserved as a fallback method if pin settings aren't found
 */
static unsigned int cardDetectGPIO[MAX_SDDEVICES] =
{
	8,
	15
};

/* Physical base address for SD Card Slot 0 & 1 registers */
static unsigned int DEV_SDCTRL_BASE_ADRS[MAX_SDDEVICES] =
{
	(RGST_OFFSET + (118 * 32 * 4)),
	(RGST_OFFSET + (125 * 32 * 4))
};

/* Function declaration */
static int Sd_Bus_Reset_Channel(struct spsdv2_hsmmc_host *host);
static int Reset_Controller(struct spsdv2_hsmmc_host *host);
static void spsdv2_get_HWDMA_rsp(struct spsdv2_hsmmc_host *host, struct mmc_cmd *cmd);
static void spsdv2_prep_cmd_rsp(struct spsdv2_hsmmc_host *host, struct mmc_cmd *cmd);
static void spsdv2_set_data_timeout(struct spsdv2_hsmmc_host *host);
static void spsdv2_prep_hwDMA(struct spsdv2_hsmmc_host *host, struct mmc_cmd *cmd, struct mmc_data *data);
static void spsdv2_trigger_hwDMA(struct spsdv2_hsmmc_host *host);
static void spsdv2_chk_err_hwDMA(struct spsdv2_hsmmc_host *host,int *ret);
static void spsdv2_post_hwDMA(struct spsdv2_hsmmc_host *host, struct mmc_cmd *cmd, struct mmc_data *data, int *ret);
static void spsdv2_prep_normDMA(struct spsdv2_hsmmc_host *host, struct mmc_cmd *cmd, struct mmc_data *data);
static void spsdv2_wait_sdstate_new(struct spsdv2_hsmmc_host *host);

static void spsdv2_dcache_flush_invalidate(struct mmc_data *data, unsigned int phys_start, unsigned int data_len)
{
	unsigned int phys_end = phys_start + data_len;

	phys_end = roundup(phys_end, ARCH_DMA_MINALIGN);
	/*
	 * MMC_DATA_WRITE: memory -> SD Card, flush data to memory
	 * MMC_DATA_READ: SD Card -> memory, invalidate dcache contents
	 */
	if (data->flags & MMC_DATA_WRITE)
		flush_dcache_range((ulong)phys_start, (ulong)phys_end);
	else
		invalidate_dcache_range((ulong)phys_start, (ulong)phys_end);

	return;
}

static int Sd_Bus_Reset_Channel(struct spsdv2_hsmmc_host *host)
{
	int counter = 0;
	//printf("SD Bus reset channel\n");

	//printf("Reset channel 0x%x\n", host->base->rst_cnad);
	host->base->rst_cnad = 1;	/*reset Central FIFO*/
	/* Wait for channel reset to complete */

	while (host->base->rst_cnad == 1) {
		udelay(1);
	}

	CPRINTK("%s, counter:%d\n", __func__, counter);
	return SPSD_SUCCESS;
}

static int Reset_Controller(struct spsdv2_hsmmc_host *host)
{
	//printf("controller reset\n");
	if (host->base->sdstate_new != 0x40) {
		//printf("sd status:0x%x, sd state:0x%x, state_new:0x%x\n", host->base->sdstatus, host->base->sdstate, host->base->sdstate_new);
	}
	SD_RST_seq(host->base);
	return Sd_Bus_Reset_Channel(host);
}

/*
 * Receive 48 bits response, and pass it back to U-Boot
 * Used for HW DMA transactions
 */
static void spsdv2_get_HWDMA_rsp(struct spsdv2_hsmmc_host *host, struct mmc_cmd *cmd)
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
	cmd->response[0] = (rspBuf[1] << 24) | (rspBuf[2] << 16) | (rspBuf[3] << 8) | rspBuf[4];
	cmd->response[1] = rspBuf[5] << 24;

	return;
}

/*
 * Receive 48 bits response, and pass it back to U-Boot
 * Used for cmd+rsp and normal dma requests
 * If error occurs, stop receiving response and return
 */
static void spsdv2_get_rsp_48(struct spsdv2_hsmmc_host *host, struct mmc_cmd *cmd)
{
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

	/* Pass response back to U-Boot */
	cmd->response[0] = (rspBuf[1] << 24) | (rspBuf[2] << 16) | (rspBuf[3] << 8) | rspBuf[4];
	cmd->response[1] = rspBuf[5] << 24;

	return;
}

/*
 * Receive 136 bits response, and pass it back to U-Boot
 * Used for cmd+rsp and normal dma requests
 * If error occurs, stop receiving response and return
 * Note: Host doesn't support Response R2 CRC error check
 */
static void spsdv2_get_rsp_136(struct spsdv2_hsmmc_host *host, struct mmc_cmd *cmd)
{
	unsigned int rspNum;
	unsigned char rspBuf[17] = {0}; /* Used to store 17 bytes(136 bits) or 6 bytes(48 bits) response */

	/* Receive Response */
	while (1) {
		if (host->base->sdstatus & SP_SDSTATUS_RSP_BUF_FULL)
			break;	/* Response buffers are full, break */

		if (host->base->sdstate_new & SDSTATE_NEW_ERROR_TIMEOUT)
			return; /* Return if error occurs */
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

	/* Response R2 is 136 bits long, keep receiving response by reading from rspBuf[5] */
	for (rspNum = 6; rspNum < 17; rspNum++) {
		while (1) {

			if (host->base->sdstatus & SP_SDSTATUS_RSP_BUF_FULL) {
				break;	/* Wait until response buffer full */
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
	 * Pass response back to U-Boot
	 * Function runs to here only if no error occurs
	 */
	cmd->response[0] = (rspBuf[1] << 24) | (rspBuf[2] << 16) | (rspBuf[3] << 8) | rspBuf[4];
	cmd->response[1] = (rspBuf[5] << 24) | (rspBuf[6] << 16) | (rspBuf[7] << 8) | rspBuf[8];
	cmd->response[2] = (rspBuf[9] << 24) | (rspBuf[10] << 16) | (rspBuf[11] << 8) | rspBuf[12];
	cmd->response[3] = (rspBuf[13] << 24) | (rspBuf[14] << 16) | (rspBuf[15] << 8) | rspBuf[16];

	return;
}

/*
 * Retrieve response for cmd+rsp and normal dma requests
 * This function makes sure host returns to it's sdstate_new idle or error state
 * Note: Error handling should be performed afterwards
 */
static void spsdv2_get_rsp(struct spsdv2_hsmmc_host *host, struct mmc_cmd *cmd)
{
	if (cmd->resp_type & MMC_RSP_136)
		spsdv2_get_rsp_136(host, cmd);
	else
		spsdv2_get_rsp_48(host, cmd);

	return;
}

/* Set SD card clock divider value based on the required clock in HZ */
static void spsdv2_set_clock(struct mmc *mmc, uint clock)
{
	struct spsdv2_hsmmc_host *host = mmc->priv;
	uint clkrt, sys_clk, act_clock;

	if (clock < mmc->cfg->f_min)
		clock = mmc->cfg->f_min;
	if (clock > mmc->cfg->f_max)
		clock = mmc->cfg->f_max;

	sys_clk = SPSD_CLK_SRC;
	clkrt = (sys_clk / clock) - 1;

	/* Calculate the actual clock for the divider used */
	act_clock = sys_clk / (clkrt + 1);
	//printf("sys_clk =%u, act_clock=%u, clkrt = %u\n", sys_clk, act_clock, clkrt);
	/* check clock divider boundary and correct it */
	if (clkrt > 0xFFF)
		clkrt = 0xFFF;

	/*
	 * Switch to the requested frequency
	 * freq_divisor[11:10] = sdfreq[1:0]
	 * freq_divisor[9:0] = sdfqsel[9:0]
	 */
	host->base->sdfreq = (clkrt & 0xC00) >> 10;
	host->base->sdfqsel = clkrt & 0x3FF;
	//printf("clkrt %u, act_clock %u\n", clkrt, act_clock);
	//printf("sdfreq 0x%x, sdfqsel 0x%x\n", host->base->sdfreq, host->base->sdfqsel);

	/* Delay 4 msecs for now (wait till clk stabilizes?) */
	mdelay(4);
}

static void spsdv2_prep_cmd_rsp(struct spsdv2_hsmmc_host *host, struct mmc_cmd *cmd)
{
	//printk("Process Command & Response (No Data)\n");
	/* Reset */
	Reset_Controller(host);

	//printf("Configuring registers\n");
	/* Configure Group SD Registers */
	host->base->sd_cmdbuf[0] = (u8)(cmd->cmdidx | 0x40);	//add start bit, according to spec, command format
	host->base->sd_cmdbuf[1] = (u8)((cmd->cmdarg >> 24) & 0x000000ff);
	host->base->sd_cmdbuf[2] = (u8)((cmd->cmdarg >> 16) & 0x000000ff);
	host->base->sd_cmdbuf[3] = (u8)((cmd->cmdarg >>  8) & 0x000000ff);
	host->base->sd_cmdbuf[4] = (u8)((cmd->cmdarg >>  0) & 0x000000ff);

	host->base->sd_trans_mode = 0x0;
	host->base->sdautorsp = 1;
	host->base->sdcmddummy = 1;

	/*
	 * Currently, host is not capable of checking Response R2's CRC7
	 * Because of this, enable response crc7 check only for 48 bit response commands
	 */
	if (cmd->resp_type & MMC_RSP_CRC && !(cmd->resp_type & MMC_RSP_136))
		host->base->sdrspchk_en = 0x1;
	else
		host->base->sdrspchk_en = 0x0;

	if (cmd->resp_type & MMC_RSP_136)
		host->base->sdrsptype = 0x1;
	else
		host->base->sdrsptype = 0x0;

	/* Configure SD INT reg (Disable them) */
	host->base->dmacmpen_interrupt = 0;
	host->base->sdcmpen = 0x0;

	return;
}

/* Process Command + No Response commands (with no data) */
static void spsdv2_prep_cmd(struct spsdv2_hsmmc_host* host, struct mmc_cmd *cmd)
{
	//printf("Process cmd (no rsp)\n");
	/* Reset */
	Reset_Controller(host);

	/* Configure Group SD Registers */
	host->base->sd_cmdbuf[0] = (u8)(cmd->cmdidx | 0x40);	//add start bit, according to spec, command format
	host->base->sd_cmdbuf[1] = (u8)((cmd->cmdarg >> 24) & 0x000000ff);
	host->base->sd_cmdbuf[2] = (u8)((cmd->cmdarg >> 16) & 0x000000ff);
	host->base->sd_cmdbuf[3] = (u8)((cmd->cmdarg >>  8) & 0x000000ff);
	host->base->sd_cmdbuf[4] = (u8)((cmd->cmdarg >>  0) & 0x000000ff);

	host->base->sd_trans_mode = 0x0;
	host->base->sdautorsp = 0;
	host->base->sdcmddummy = 1;

	/*
	 * Configure SD INT reg
	 * Disable HW DMA data transfer complete interrupt (when using sdcmpen)
	 */
	host->base->dmacmpen_interrupt = 0;
	host->base->sdcmpen = 0x0;

	return;
}

/* Get timeout_ns from kernel, and set it to HW DMA's register */
static void spsdv2_set_data_timeout(struct spsdv2_hsmmc_host *host)
{
	unsigned int timeout_clks, cycle_ns, timeout_ns = 500 * 1000000; /* set timeout_ns to 500ms for now */

	cycle_ns = 1000000000 / SPSD_CLK_SRC;
	timeout_clks = timeout_ns / cycle_ns;

	host->base->hw_wait_num_low = (u16)(timeout_clks & 0x0000ffff);
	host->base->hw_wait_num_high = (u16)((timeout_clks >> 16) & 0x0000ffff);
}

/*
 * Although HW DMA supports 8 fragmented memory blocks of data at a time, U-Boot only requires
 * processing 1 memory block of data per request.
 */
static void spsdv2_prep_hwDMA(struct spsdv2_hsmmc_host *host, struct mmc_cmd *cmd, struct mmc_data *data)
{
	unsigned int hw_address = 0, hw_len = 0;

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
	/* Depending on block count, decide on what kind of HW DMA mode we should use */
	if (data->blocks > 1)
		host->base->hw_sd_dma_type = 0x2;
	else
		host->base->hw_sd_dma_type = 0x1;

	/* U-Boot deals with SDSC & SDHC addressing automatically */
	host->base->hw_sd_hcsd_en = 0x1;

	/* Configure Group DMA Registers */
	if (data->flags & MMC_DATA_WRITE) {
		host->base->dmadst = 0x2;
		host->base->dmasrc = 0x1;
	} else {
		host->base->dmadst = 0x1;
		host->base->dmasrc = 0x2;
	}
	DMASIZE_SET(host->base, 512); //host->base->dma_size = 0x01ff;
	SDDATALEN_SET(host->base, 512); // host->base->sddatalen

	//BLOCK0_DMA_PARA_SET(host->base, cmd->cmdarg, data_len/512); /* Sets dma_hw_page_addr & dma_hw_page_num */
	host->base->hw_dma_en = 0x1;
	host->base->hw_block_mode_en = 0x1;
	host->base->hw_block_num = 0x0; /* Only send 1 command per request */

	/* sdcmpen & dmacmpen interrupts, disable both of them for U-Boot */
	host->base->sdcmpen = 0x0;
	host->base->dmacmpen_interrupt = 0x0;

	spsdv2_set_data_timeout(host); /* Sets hw_wait_num_low & hw_wait_num_high */
	host->base->hw_delay_num = 0; /* dpll clock cycle count delay between commands */

	/* retrive physical memory address & size of the fragmented memory blocks */
	if (data->flags & MMC_DATA_WRITE)
		hw_address = (unsigned int) data->src;
	else
		hw_address = (unsigned int) data->dest;

	hw_len = data->blocks * data->blocksize;

	/* Set fragmented memory block's base address, size */
	SET_HW_DMA_BASE_ADDR(host->base, hw_address);
	host->base->sdram_sector_0_size = (hw_len / 512) - 1;

	BLOCK0_DMA_PARA_SET(host->base, cmd->cmdarg, hw_len / 512); /* Sets dma_hw_page_addr & dma_hw_page_num */

	spsdv2_dcache_flush_invalidate(data, hw_address, hw_len);

	return;
}

static void spsdv2_trigger_hwDMA(struct spsdv2_hsmmc_host *host)
{
	host->base->dmastart = 0x1;    /* Trigger HW DMA */

	/* Wait for HW DMA to finish (either done or error occured) */
	while (!(host->base->hwsd_sm & SP_SD_HW_DMA_DONE) && !(host->base->hwsd_sm & SP_SD_HW_DMA_ERROR))
	{

	}
}

static void spsdv2_check_sdstatus_errors(struct spsdv2_hsmmc_host *host, struct mmc_data *data, int *ret)
{
	if (host->base->sdstate_new & SDSTATE_NEW_ERROR_TIMEOUT) {
		/* Response related errors */
		if (host->base->sdstatus & SP_SDSTATUS_WAIT_RSP_TIMEOUT)
			*ret = TIMEOUT;
		if (host->base->sdstatus & SP_SDSTATUS_RSP_CRC7_ERROR)
			*ret = COMM_ERR;

		/* Data transaction related errors */
		if (data != NULL) {
		/* Data transaction related errors */
			if (host->base->sdstatus & SP_SDSTATUS_WAIT_STB_TIMEOUT)
				*ret = TIMEOUT;
			if (host->base->sdstatus & SP_SDSTATUS_WAIT_CARD_CRC_CHECK_TIMEOUT)
				*ret = TIMEOUT;
			if (host->base->sdstatus & SP_SDSTATUS_CRC_TOKEN_CHECK_ERROR)
				*ret = COMM_ERR;
			if (host->base->sdstatus & SP_SDSTATUS_RDATA_CRC16_ERROR)
				*ret = COMM_ERR;

			Sd_Bus_Reset_Channel(host);
		}

		/*
		 * By now, *ret should be set to a known error case, the below code
		 * snippet warns user a unknown error occurred
		 */
		if ((*ret != COMM_ERR) && (*ret != TIMEOUT)) {
			printf("[SDCard] Unknown error occurred!\n");
			*ret = COMM_ERR;
		}
	}

	return;
}

/*
 * All error cases for HW DMA read/write transactions
 * HW DMA Bug: Host doesn't check CRC error for CMD17, 18, 24, 25
 *  command responses, meaning it will continue to perform
 *  the requested transaction.
 *
 * Side effect: If the SD card had issues receiving the command
 *   correctly(CRC7 error), HW DMA's timeout will occur instead
 *   of reporting the CRC7 error indicated in the response from
 *   SD card.
 */
static void spsdv2_chk_err_hwDMA(struct spsdv2_hsmmc_host *host, int *ret)
{
	//printf("host->base->hwsd_sm = 0x%x\n", host->base->hwsd_sm);
	switch (host->base->hwsd_sm) {
	/* Stop command timeout cases */
	case HWSD_SM_CMD12_TIMEOUT:
	case HWSD_W_SM_CMD12_TIMEOUT:
		//printf("CMD12 timeout\n");
		*ret = TIMEOUT;
		break;

	 /* Other command timeout cases */

	 /* Send command timeout cases */
	case HWSD_SM_CMD13_TIMEOUT:
	case HWSD_W_SM_CMD13_TIMEOUT:
	case HWSD_SM_CMD17_TIMEOUT:
	case HWSD_SM_CMD18_TIMEOUT:
	case HWSD_W_SM_CMD24_TIMEOUT:
	case HWSD_W_SM_CMD25_TIMEOUT:
	/* Recieve response timeout cases */
	case HWSD_SM_CMD12_RSP_TIMEOUT:
	case HWSD_W_SM_CMD12_RSP_TIMEOUT:
	case HWSD_SM_CMD13_RSP_TIMEOUT:
	case HWSD_W_SM_CMD13_RSP_TIMEOUT:
	case HWSD_W_SM_CMD24_RSP_TIMEOUT:
	case HWSD_W_SM_CMD25_RSP_TIMEOUT:
	/* CRC timeout cases */
	case HWSD_W_SM_RXCRC_TIMEOUT:
	case HWSD_W_SM_RXCRC_TX_TIMEOUT:
		//printf("Command timeout!\n");
		*ret = TIMEOUT;
		break;

	/* Data transfer timeout cases */
	case HWSD_SM_DMA_TIMEOUT:
	case HWSD_W_SM_DMA_TIMEOUT:
	case HWSD_SM_CMD18_BUSY_TIMEOUT:
	case HWSD_W_SM_CMD13_ST_TX_TIMEOUT:
	case HWSD_W_SM_CMD13_BUSY_ERR:
	case HWSD_SM_CMD13_BUSY_ERR:
		//printf("Data error!\n");
		*ret = TIMEOUT;
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
		//printf("Command CRC error!\n");
		*ret = COMM_ERR;
		break;

	/* Data transfer CRC error */
	case HWSD_SM_CRC_ERR:
	case HWSD_W_SM_CRC_ERR:
		//printf("Data CRC error!\n");
		*ret = COMM_ERR;
		break;

	/* Reset Pbus channel timeout */
	case HWSD_W_SM_RST_CHAN_TIMEOUT:
	case HWSD_SM_RST_CHAN_TIMEOUT:
		/* Pbus channel reset timeout, should somehow be handled in the future */
	default:
		/* Unknown error, tell U-Boot it's timeout error for now */
		//printf("Unknown!\n");
		*ret = TIMEOUT;
	}
}

static void spsdv2_post_hwDMA(struct spsdv2_hsmmc_host *host, struct mmc_cmd *cmd, struct mmc_data *data, int *ret)
{
	if (host->base->hwsd_sm & SP_SD_HW_DMA_DONE) {
		//printf("Success!\n");
		//data->bytes_xfered = host->base->dmasize * (host->base->dma_hw_page_num[0] + 1);
		//printk("data->bytes_xfered = 0x%x\n", data->bytes_xfered);
		//printk("request size : %u\n", data->blocks * data->blksz);
		spsdv2_get_HWDMA_rsp(host, cmd);

		/*
		 * Host doesn't seem to have specific hardware for adjusting the timing to send
		 * CMD12 (STOP_TRANSMISSION), causing the response after reading the last data block
		 * on SD card during multi-block read to indicate out of range error in CMD12's response
		 * Since HW DMA was successful ---> clear the OUT_OF_RANGE status bit for now
		 */
		if (cmd->cmdidx == MMC_CMD_READ_MULTIPLE_BLOCK) {
			if (cmd->response[0] & (1 << 31)) {
				//printf("out of bounds!\n");
				cmd->response[0] = cmd->response[0] & 0x7fffffff;
			}
		}

		*ret = 0;
	} else if (host->base->hwsd_sm & SP_SD_HW_DMA_ERROR) {
		//printf("HW DMA Error!\n");
		spsdv2_chk_err_hwDMA(host, ret); /* fills out ret depending on error case */
		Sd_Bus_Reset_Channel(host);
		*ret = COMM_ERR;
	} else {
		printf("Flow error!\n");
		printf("host->base->hwsd_sm :0x%x\n", host->base->hwsd_sm);
		*ret = COMM_ERR;
	}

	return;
}

/*
 * DMA transfer mode, used for all other data transfer commands besides read/write block commands (cmd17, 18, 24, 25)
 * Due to host limitations, this kind of DMA transfer mode only supports 1 consecution memory area
 */
static void spsdv2_prep_normDMA(struct spsdv2_hsmmc_host *host, struct mmc_cmd *cmd, struct mmc_data *data)
{
	unsigned int hw_address = 0, hw_len = 0;

	/* retreive physical memory address & size of the fragmented memory blocks */
	if (data->flags & MMC_DATA_WRITE)
		hw_address = (unsigned int) data->src;
	else
		hw_address = (unsigned int) data->dest;

	hw_len = data->blocksize * data->blocks;
	//printf("hw_len %u\n", hw_len);
	/* Reset */
	Reset_Controller(host);

	/* Configure Group SD Registers */
	host->base->sd_cmdbuf[0] = (u8)(cmd->cmdidx | 0x40);	//add start bit, according to spec, command format
	host->base->sd_cmdbuf[1] = (u8)((cmd->cmdarg >> 24) & 0x000000ff);
	host->base->sd_cmdbuf[2] = (u8)((cmd->cmdarg >> 16) & 0x000000ff);
	host->base->sd_cmdbuf[3] = (u8)((cmd->cmdarg >>  8) & 0x000000ff);
	host->base->sd_cmdbuf[4] = (u8)((cmd->cmdarg >>  0) & 0x000000ff);

	SD_PAGE_NUM_SET(host->base, 1);
	if (cmd->resp_type & MMC_RSP_CRC && !(cmd->resp_type & MMC_RSP_136))
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
	if(cmd->resp_type & MMC_RSP_136)
		host->base->sdrsptype = 0x1;
	else
		host->base->sdrsptype = 0x0;

	SDDATALEN_SET(host->base, hw_len);

	/* Configure Group DMA Registers */
	if (data->flags & MMC_DATA_WRITE) {
		host->base->dmadst = 0x2;
		host->base->dmasrc = 0x1;
	} else {
		host->base->dmadst = 0x1;
		host->base->dmasrc = 0x2;
	}
	DMASIZE_SET(host->base, hw_len);
	//printf("hw_address 0x%x\n", hw_address);
	SET_HW_DMA_BASE_ADDR(host->base, hw_address);

	/*
	 * Configure SD INT reg
	 * Disable HW DMA data transfer complete interrupt (when using sdcmpen)
	 */
	host->base->dmacmpen_interrupt = 0;
	host->base->sdcmpen = 0x0;

	spsdv2_dcache_flush_invalidate(data, hw_address, hw_len);

	return;
}

static void spsdv2_trigger_sdstate(struct spsdv2_hsmmc_host *host)
{
	host->base->sdctrl_trigger_cmd = 1;   /* Start transaction */
}

static void spsdv2_wait_sdstate_new(struct spsdv2_hsmmc_host *host)
{
	/* Wait transaction to finish (either done or error occured) */
	while (1) {
		//printf("sdstate_new 0x%x\n", host->base->sdstate_new);
		if ((host->base->sdstate_new & SDSTATE_NEW_FINISH_IDLE) == 0x40)
			break;
		if ((host->base->sdstate_new & SDSTATE_NEW_ERROR_TIMEOUT) == 0x20)
			break;
	}
}

/*
 * Single read operation timeout: The typical delay between "the end
 * bit of the read command" and "the start bit of the data block"
 * Since SD Host's read data start bit timer is broken, use a software counter
 * to count "the whole transaction's timeout" instead
 */
static int spsdv2_normDMA_timeout_chk(struct spsdv2_hsmmc_host *host)
{
	unsigned long timeout_us = 300 * 1000; /* Set the whole transaction's timeout to 300ms */
	unsigned long counter = 0;
	int timesup = 0; /* Used to determine if timeout occurred */

	/* Wait till host controller becomes idle or error/timeout occurs */
	while ((host->base->sdstate_new & (SDSTATE_NEW_FINISH_IDLE
					   | SDSTATE_NEW_ERROR_TIMEOUT)) == 0) {
		udelay(5);
		counter += 5;

		if (counter > timeout_us) {
			timesup = 1;
			break;
		}
	}

	/* If software timeout occured, set ret to 1 */
	if (timesup) {
		//printf("Normal DMA timeout occurred\n");
		return TIMEOUT;
	}

	return 0;
}

/*
 * Sends a command out on the bus.  Takes the mmc pointer,
 * a command pointer, and an optional data pointer.
 */
/*
 * For data transfer requests
 * 1. CMD 17, 18, 24, 25 : Use HW DMA
 * 2. Other data transfer commands : Use normal DMA
 */
static int
spsdv2_mmc_send_cmd(struct mmc *mmc, struct mmc_cmd *cmd, struct mmc_data *data)
{
	//printf("Called mmc_send_cmd!\n");
	struct spsdv2_hsmmc_host *host = mmc->priv;
	int ret = 0; /* Returning 1 means success, returning other stuff means error */
	//printf("host->base = 0x%08x\n", host->base);

	/* post process send command requests, trigger transaction */
	if (data == NULL) {
		if (cmd->resp_type & MMC_RSP_PRESENT) {
			spsdv2_prep_cmd_rsp(host, cmd);
			spsdv2_trigger_sdstate(host);
			spsdv2_get_rsp(host, cmd); /* Makes sure host returns to a idle or error state */
		} else {
			spsdv2_prep_cmd(host, cmd);
			spsdv2_trigger_sdstate(host);
			spsdv2_wait_sdstate_new(host);
		}

		spsdv2_check_sdstatus_errors(host, data, &ret);
	} else {
		switch (cmd->cmdidx) {
		case MMC_CMD_READ_SINGLE_BLOCK:
		case MMC_CMD_READ_MULTIPLE_BLOCK:
		case MMC_CMD_WRITE_SINGLE_BLOCK:
		case MMC_CMD_WRITE_MULTIPLE_BLOCK:
			spsdv2_prep_hwDMA(host, cmd, data);
			spsdv2_trigger_hwDMA(host);
			spsdv2_post_hwDMA(host, cmd, data, &ret);
			break;
		default:
			spsdv2_prep_normDMA(host, cmd, data);
			spsdv2_trigger_sdstate(host);
			/* Host's "read data start bit timeout counter" is broken, use
			 * use software to set "the whole transaction's timeout" instead
			 */
			if (data->flags & MMC_DATA_READ) {
				if ((ret = spsdv2_normDMA_timeout_chk(host)) != 0)
					return ret;
			}

			spsdv2_get_rsp(host, cmd); /* Makes sure host returns to a idle or error state */
			spsdv2_check_sdstatus_errors(host, data, &ret);
		}
	}

	return ret;
}

/* Set buswidth or clock as indicated by the GENERIC_MMC framework */
static void spsdv2_mmc_set_ios(struct mmc *mmc)
{
	//printf("Called spsdv2_mmc_set_ios\n");
	struct spsdv2_hsmmc_host *host = mmc->priv;

	/* Set the bus width */
	if (mmc->bus_width == 4)
		host->base->sddatawd = 1;
	else
		host->base->sddatawd = 0;

	/* Set clock speed */
	if (mmc->clock)
		spsdv2_set_clock(mmc, mmc->clock);

	/* clock >  25Mhz : High Speed Mode
	 *       <= 25Mhz : Default Speed Mode
	 */
	if (mmc->clock > 25000000)
		host->base->sd_high_speed_en = 1;
	else
		host->base->sd_high_speed_en = 0;

	/* Write delay: Controls CMD, DATA signals timing to SD Card */
	host->base->sd_wr_dly_sel = SPSD_WRITE_DELAY;
	/* Read delay: Controls timing to sample SD card's CMD, DATA signals */
	host->base->sd_rd_dly_sel = SPSD_READ_DELAY;

	return;
}

/* Initialize MMC/SD controller */
static int spsdv2_mmc_init(struct mmc *mmc)
{
	struct spsdv2_hsmmc_host *host = mmc->priv;

	host->base->sdddrmode = 0;
	host->base->sdiomode = 0;

	host->base->sdrsptmr = 0x7ff;
	host->base->sdcrctmr = 0x7ff;
	host->base->sdcrctmren = 1;
	host->base->sdrsptmren = 1;
	host->base->sdmmcmode = 0;
	host->base->sdrxdattmr_sel = 3;
	host->base->mediatype = 6;

	return 0;
}

/*
 * Card detection, return 1 to indicate card is plugged in
 *                 return 0 to indicate card in not plugged in
 */
static int spsdv2_mmc_getcd(struct mmc *mmc)
{
	struct spsdv2_hsmmc_host *priv_data = mmc->priv;
	int cd_gpio, slot;

	/* Names of card detection GPIO pin # configs found in runtime.cfg */
	char *cardDetectCfg[MAX_SDDEVICES] = {"gpioSD0Sen", "gpioSD1Sen"};

	/* If bootmode is set to "ISP from SD card", always assume card is inserted */
	if (SP_GET_BOOTINFO()->gbootRom_boot_mode == SDCARD_ISP) {
		return 1;
	}

	/* Check base address to decide on if it's Slot 0 or Slot 1 */
	((unsigned int)priv_data->base == DEV_SDCTRL_BASE_ADRS[0])? (slot = 0): (slot = 1);

	/*
	 * retrieve GPIO # for card detection
	 * If settings weren't found, fallback and use default gpio settings (Used on EMU Board V1.0)
	 */
	if (sp_config_get_int(cardDetectCfg[slot], &cd_gpio) == SP_CFG_RET_SUCCESS) {
			printf("[SDCard] Slot %d, using GPIO #%d for card detection\n", slot, cd_gpio);
	} else {
		cd_gpio = cardDetectGPIO[slot];
		printf("[SDCard] Slot %d detect pin settings not found! Using default GPIO pin #%d\n", slot, cd_gpio);
	}

	if (GPIO_I_GET(cd_gpio) == 0) {
		//printf("Card plugged in!\n");
		return 1;
	} else {
		//printf("Card absent!\n");
		return 0;
	}
}

static const struct mmc_ops SPSDV2_sdc_ops = {
	.send_cmd	= spsdv2_mmc_send_cmd,
	.set_ios	= spsdv2_mmc_set_ios,
	.init		= spsdv2_mmc_init,
	.getcd		= spsdv2_mmc_getcd,
};

static struct mmc_config spsdv2_mmc_cfg = {
	.name		= "SD",
	.ops		= &SPSDV2_sdc_ops,
	.host_caps	=  MMC_MODE_4BIT | MMC_MODE_HS_52MHz | MMC_MODE_HS | MMC_MODE_HC,
	.voltages	= MMC_VDD_32_33 | MMC_VDD_33_34,
	.f_min		= SPSD_MIN_CLK,
	.f_max		= SPSD_MAX_CLK,
	.b_max		= CONFIG_SYS_MMC_MAX_BLK_COUNT,    /* Limited by sdram_sector_#_size max value */
};

/*
 * Check pinmux and decide whether or not SD Card Slot should be enabled
 * Passes whether or not a SD Card slot should be enabled through slot_enable function
 */
static void spsdv2_check_pinmux(unsigned int slot_enable[])
{
	int i;
	volatile unsigned int sft_cfg4_value = 0;
	u32 pmx_card_sd[2] = {0}, pmx_card_hb[2] = {0};

	/* For each slot, SD card requires 2 sets of pinmux to be switched */
	/* gpio_pin_mux_get doesn't exist in U-Boot.. Retrieve sft_cfg4 register directly.. */
	sft_cfg4_value = MOON1_REG->sft_cfg[4];
	//printf("sft_cfg4_value:0x%x\n", sft_cfg4_value);

	pmx_card_sd[0] = (sft_cfg4_value >> 13) & 0x3;/* CARD0_SD */
	pmx_card_hb[0] = (sft_cfg4_value >> 15) & 0x3;/* CARD0_HB */
	pmx_card_sd[1] = (sft_cfg4_value >> 23) & 0x3;/* CARD1_SD */
	pmx_card_hb[1] = (sft_cfg4_value >> 25) & 0x3;/* CARD1_SD_HB */

	//printf("pmx_card_sd[0] = 0x%x\n", pmx_card_sd[0]);
	//printf("pmx_card_sd[1] = 0x%x\n", pmx_card_sd[1]);
	//printf("pmx_card_hb[0] = 0x%x\n", pmx_card_hb[0]);
	//printf("pmx_card_hb[1] = 0x%x\n", pmx_card_hb[1]);

	/* Check if pinmux for each slot is set */
	for (i = 0; i < MAX_SDDEVICES; i++) {
		if ((pmx_card_sd[i] != 0) && (pmx_card_hb[i] != 0))
			slot_enable[i] = 1;
		else
			slot_enable[i] = 0;
	}
	return;
}

/*
 * Called from board_mmc_init during startup. Can be called multiple times
 * depending on the number of slots available on board and controller
 * SD Host Controller will register only if pin mux for the SD Card Slot is set
 */
int spsdv2_mmc_controller_init(bd_t *bis, int dev_index)
{
	struct mmc *mmc;
	struct spsdv2_hsmmc_host *host;
	/* Below variables are used to indicate if pinmux for SD card slots are enabled */
	unsigned int slot_enable[2] = {0}, slot = 0;

	/* Check pinmux settings to see if driver needs to be enabled */
	spsdv2_check_pinmux(slot_enable);

	//host = (struct spsdv2_hsmmc_host *)malloc(sizeof(*host));
	host = malloc(sizeof(*host));
	if (host == NULL)
		return -1;

	host->base = (struct spsd_general_regs *)DEV_SDCTRL_BASE_ADRS[dev_index];

	/* Check base address to decide on if it's Slot 0 or Slot 1 */
	((unsigned int)host->base == DEV_SDCTRL_BASE_ADRS[0])? (slot = 0): (slot = 1);

	if (slot_enable[slot] == 1) {
		mmc = mmc_create(&spsdv2_mmc_cfg, host);

		if (mmc == NULL)
			return -1;
	} else {
		printf("[SDCard] Pinmux for Slot %d not set!\n", slot);
		free(host);
	}

	return 0;
}
