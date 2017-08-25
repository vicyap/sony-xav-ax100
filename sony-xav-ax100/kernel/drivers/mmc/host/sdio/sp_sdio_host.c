
/******************************************************************************
 *                          Include File
 *******************************************************************************/
#include <mach/hardware.h>
#include "sp_sdio_host.h"

#include <generated/autoconf.h>
#include <linux/mmc/card.h>
//#include "sd.h"
#include "sp_hw_sdio.h"

#include <linux/kthread.h>
#include <linux/module.h>
#include <linux/memstick.h>
#include <linux/dma-mapping.h>
#include <mach/gpio_drv.h>
#include <linux/mmc/mmc.h>
#include <linux/mmc/sd.h>

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/string.h>
#include <linux/vmalloc.h>
#include <asm/uaccess.h>
#include <linux/mmc/sdio.h>
#include <mach/gpio_drv.h>

/******************************************************************************
 *                          Global Variabl
 *******************************************************************************/

static u32 mmc_printk = 0;
static u32 g_sdio_freq = 0;  
struct sphe8388_sdio_priv_host *sphe8388_sdio_host = NULL;
struct platform_device *letb8388_device_sdio = NULL;
u32 g_sdio_pio_dma_mode = 0; // 0: DMA   1:PIO 

/******************************************************************************
 *                          function declaration
 *******************************************************************************/
static irqreturn_t sphe8388_sdio_card_irq(int irq, void *dev_id);
static irqreturn_t sphe8388_sd_cmp_irq(int irq, void *dev_id);
static int hw_sd_send_cmd(void *host_data, u32 cmd, u32 arg, u32 rspType, struct mmc_command *mmc_cmd, u32 cmdType);
u32 debug_get_register_statues(void);
static u32 sdio_data_transfer(struct mmc_request *mrq, struct sphe8388_sdio_priv_host *host, 
		u32 rspType, struct mmc_command *mmc_cmd);
static void hw_sdio_init(void);

/******************************************************************************
 *                         Function Prototype
 ******************************************************************************/


#define DEBUG_GET_FUNCTION_TIME_BEGIN()  struct timespec t0, t1, d;\
	getnstimeofday(&t0)

#define DEBUG_GET_FUNCTION_TIME_END()  \
	getnstimeofday(&t1);\
d = timespec_sub(t1, t0);\
IFPRINTK("sec=%ld, nsec=%ld\n", d.tv_sec, d.tv_nsec)

#define letb123_MS_NAME  "letb123-ms"

#define	SDC_QUIT 	0
#define	SDC_EXIST	1
#define	SDC_ACTIVE	2




void sdio_dump_buffer(unsigned char *addr, unsigned int size)
{
	unsigned int i;

	printk("\nsdio driver addr:0x%p, len:%d", addr, size);
	for(i=0; i<size; i++){
		if (i%16 == 0){ 
			printk("\n[%08x] ",i);
		}
		printk("%02x",addr[i]);
	}
	printk("\n\n");
}




static  void tune_clk_dly(void *host, int clkdly)
{
	struct sphe8388_sdio_priv_host *sdio_host = (struct sphe8388_sdio_priv_host *)host;
	if(!sdio_host->clk_dly_info.tune_begin_flag){
		return ;
	}
	if(clkdly >= 0)
	{
		sdio_host->clk_dly_info.clock_delay = clkdly;
	}
	else
	{
		sdio_host->clk_dly_info.clock_delay = CLK_DELAY_GET();
		sdio_host->clk_dly_info.clock_delay += 1;
	}

	if(sdio_host->clk_dly_info.clock_delay > 7){
		sdio_host->clk_dly_info.clock_delay = 0;
	}

	CLK_DELAY_SET(sdio_host->clk_dly_info.clock_delay);
	//IFPRINTK("\n***CLK_DLY:%d\n", ClockDelay&0x07);
	return;
}

static  void tune_wr_dly(void *host, int wtclk)
{
	struct sphe8388_sdio_priv_host *sdio_host = (struct sphe8388_sdio_priv_host *)host;
	if(!sdio_host->clk_dly_info.tune_begin_flag){
		return ;
	}

	if(wtclk >= 0){
		sdio_host->clk_dly_info.write_delay = wtclk;
	}
	else{
		sdio_host->clk_dly_info.write_delay = SD_WT_CLK_DELAY_TIME_GET();
		sdio_host->clk_dly_info.write_delay += 1;
		if(sdio_host->clk_dly_info.write_delay > 7)	{
			sdio_host->clk_dly_info.write_delay = 0;
		}
	}

	SD_WT_CLK_DELAY_TIME_SET(sdio_host->clk_dly_info.write_delay);
	//IFPRINTK("\n***WT_DLY:%d\n", WriteDelay&0x07);
	return;
}





/*
   if rdclk and hs ge 0, set them as default value
   all the rx data crc error and rx response error should tune the rd_dly
   */
static void tune_rd_dly(void *host, int rdclk, int hs)
{
	struct sphe8388_sdio_priv_host *sdio_host = (struct sphe8388_sdio_priv_host *)host;
	if(!sdio_host->clk_dly_info.tune_begin_flag){
		return ;
	}

	if((rdclk >= 0) && (hs >= 0)){
		sdio_host->clk_dly_info.read_delay = rdclk;
		sdio_host->highspeed_flag = hs;
	}else{
		sdio_host->clk_dly_info.read_delay = SD_RD_CLK_DELAY_TIME_GET();
		sdio_host->highspeed_flag = SD_HIGHSPEED_EN_GET();

		sdio_host->clk_dly_info.read_delay += 1;
		if(sdio_host->clk_dly_info.read_delay > 7){
			sdio_host->clk_dly_info.read_delay = 0;
			//sdio_host->highspeed_flag ^= 1;
		}
	}
	SD_RD_CLK_DELAY_TIME_SET(sdio_host->clk_dly_info.read_delay);
	SD_HIGHSPEED_EN_SET(sdio_host->highspeed_flag);
	//IFPRINTK("***RD_DLY:%d, HIGHSPEED_EN:%d\n", readDelay, sdio_host->highspeed_flag);
}

int controller_state_idle_wait(void)
{
    int state = 0;
    int counter = 0; 
    while ((state = (((u8) SD_STATUS1_GET()) & 0x07))) {
 		if(unlikely(counter++ == 100000)){
             EPRINTK("\n\n\n *****Idle Wait Timeout, state=0x%x status:0x%x*******\n\n\n", state, SD_STATUS0_GET());
             return SP_SD_COUNTER_ERROR;
         }
         udelay(1);
    }
      return SP_SD_SUCCESS;
}
static int tx_dummy(u32 rounds)
{
	int ret = 0;
	set_sd_tx_dummy_num(rounds);
	SD_CTRL_SET(0x02);	/*trigger tx_dummy command*/
	ret = controller_state_idle_wait();
	if(ret != SP_SD_SUCCESS){
		EPRINTK("send %d dummy error", rounds);
	}
	return ret;
}





static  inline void hwSdInit(u32 mmcMode)
{

	if (MMC_MODE == mmcMode){	//MMC
		SD_WAIT_RSP_TIME_SET(0x3ff);
		SD_WAIT_CRC_TIME_SET(0x7ff);
		SD_HW_DMA_CMD13_RSP_CNT_SET(0x7ff);
		SD_CONFIG_SET(0x7000);	//Enable Response Timer & CRC Timer, Card Mode = MMC
		SD_RST();
	} else	if(SD_MODE == mmcMode){		//SD
		SD_WAIT_RSP_TIME_SET(0x3ff);
		SD_WAIT_CRC_TIME_SET(0x7ff);
		SD_HW_DMA_CMD13_RSP_CNT_SET(0x7ff);
		SD_CONFIG_SET(0x03000);	//Enable Response Timer & CRC Timer, Card Mode = SDCard
		SD_RST();
	}else{ // SDIO
		SD_WAIT_RSP_TIME_SET(0x3ff);
		SD_WAIT_CRC_TIME_SET(0x7ff);
		SD_HW_DMA_CMD13_RSP_CNT_SET(0x7ff);
		SD_CONFIG_SET(0x043000);	//Enable Response Timer & CRC Timer, Card Mode = SDIO
		SDIO_RST();
	}
}

static  void hw_sd_freq_set(u32 FreqDivider)
{
	u32 value;
	if(FreqDivider > 0xFFF){
		WPRINTK("FreqDivider:0x%0x is bigger than 0x0fff\n", FreqDivider);
		FreqDivider = 0xFFF;
	}
	value = SD_CONFIG_GET();
	value = (value & 0xfffffc00) | (FreqDivider);
	SD_CONFIG_SET(value);
	HISGH_2BIT_FREQ_SET((FreqDivider>>10)&0x03);
}

static  inline void hw_sd_config_freq(u32 Freq)
{
	u32 clk_div;
	if(0 == Freq){
		Freq = CLOCK_400K;
	}
	clk_div = SPSD_MAX_CLOCK/Freq  - 1;
	hw_sd_freq_set(clk_div);
}


static   inline void set_bus_wide(u32 buswide){
	u32 tmp;

	tmp = SD_CONFIG_GET();
	if(SP_BUS_WIDTH_4BIT == buswide){
		SET_BIT_N(tmp, 10);
	}else{
		CLR_BIT_N(tmp, 10);
	}
	SD_CONFIG_SET(tmp);
}

static  inline void hw_sd_config_bus_width(u32 sdBusWidth)
{
	u32 tmp;
	tmp = SD_CONFIG_GET();
	tmp &= ~(0x10400);	//bit 10 and 16
	tmp |= (sdBusWidth << 10);

	SD_CONFIG_SET(tmp);
}

static  inline void hw_sd_config_mmc_mode(u32 mmcMode)
{
	u32 tmp;

	tmp = SD_CONFIG_GET();	
	if (MMC_MODE == mmcMode){	//MMC
		SET_BIT_N(tmp, 14);// mmc or sd mode
	}else if(SDIO_MODE == mmcMode){
		CLR_BIT_N(tmp, 14);
		SET_BIT_N(tmp, 18);
	}else{
		CLR_BIT_N(tmp, 14);
		CLR_BIT_N(tmp, 18);
	}
	SD_CONFIG_SET(tmp);
}

static inline void hw_sd_config(u32 FreqDivider, u32 sdBusWidth, u32 mmcMode)
{
	u32 tmp;
	u32 bit = 0;
	tmp = SD_CONFIG_GET();

	tmp &= 0xffff8800;
	if((SDIO_MODE == mmcMode) || (SD_MODE == mmcMode)){
		bit = 0;
	}else{
		bit = 1;
	}
	tmp |= ((bit << 14) | (sdBusWidth << 10));
	tmp |= (0x03 << 12); // enable rsp timer enable crc timer
	SD_CONFIG_SET(tmp);
}

u32 rx_resonse_test(void)
{
	u32 crc7;
	u32 value;
	u8 rspBuf[5] = {0};
	value = SD_RSP_BUF0_3_GET;
	rspBuf[0] = ((value & (0xff << 24)) >> 24);
	rspBuf[1] = ((value & (0xff << 16)) >> 16);
	rspBuf[2] = ((value & (0xff << 8)) >> 8);
	rspBuf[3] = ((value & (0xff << 0)) >> 0);

	value = SD_RSP_BUF4_5_GET;
	rspBuf[4] = ((value & (0xff << 8)) >> 8);
	crc7 = ((value & (0xff << 0)) >> 0);

	IFPRINTK("resp:0x%02x %02x%02x%02x%02x %02x\n", rspBuf[0], rspBuf[1], rspBuf[2], rspBuf[3], rspBuf[4], crc7);
	return 0;
}



static  u32 rx_response(void *host_data, struct mmc_command *mmc_cmd, u32 rspType, u32 cmd)
{
	u32 crc7;
	u32 i = 0;
	u32 status = SP_SD_SUCCESS;
	u32 rspNum;
	u32 value;
	int counter = 0;
	struct sphe8388_sdio_priv_host *host = (sphe8388_sdio_priv_host *)host_data;
	u8 *rspBuf1 = (u8 *)&mmc_cmd->resp[0];
	if(RSP_TYPE_NORSP == rspType){
		return 0;
	}
	
	letb8388_SDIO_LOCK(&host->lock, host->flag);
	if (rspType != RSP_TYPE_R2) {
		value = SD_CONFIG_GET();
		SD_CONFIG_SET(value & 0xfffff7ff);	/* Set response type to 6 bytes*/
	} else {
		value = SD_CONFIG_GET();
		SD_CONFIG_SET(value | 0x00000800);	/*Set response type to 17 bytes*/
	}
	value = SD_RSP_BUF0_3_GET;
	//rspBuf[0] = ((value & (0xff << 24)) >> 24);
	rspBuf1[0] = ((value & (0xff << 16)) >> 16);
	rspBuf1[1] = ((value & (0xff << 8)) >> 8);
	rspBuf1[2] = ((value & (0xff << 0)) >> 0);

	value = SD_RSP_BUF4_5_GET;
	rspBuf1[3] = ((value & (0xff << 8)) >> 8);
	crc7 = ((value & (0xff << 0)) >> 0);

	//IFPRINTK("resp:0x%02x %02x%02x%02x%02x %02x\n", rspBuf[0], rspBuf[1], rspBuf[2], rspBuf[3], rspBuf[4], crc7);
	*(u32 *)rspBuf1 = swab32(*(u32 *)rspBuf1);
	if (rspType == RSP_TYPE_R2) {
		rspBuf1[4] = crc7;
		for (rspNum = 6; rspNum < 17; rspNum++) {
			counter = 0;
			while (1) {
				if(unlikely(counter++ == 100000)){
					EPRINTK("counter:%d error, line:%d\n", counter, __LINE__);
					debug_get_register_statues();
					status = SP_SD_RSP_TIMEOUT;
					goto out;
				}

				value = SD_STATUS0_GET();
				if ((value & 0x02) == 0x02) {
					break;	/* Wait until response buffer full*/
				} 
				letb8388_SDIO_UNLOCK(&host->lock, host->flag);
				udelay(1);	
				letb8388_SDIO_LOCK(&host->lock, host->flag);
				if(SD_INT_INIT == host->interrupt.sd_int.irq_sd_stat){
					EPRINTK("Rsp timeout (1)...\n");
					debug_get_register_statues();
					status = SP_SD_RSP_TIMEOUT;
					goto out;
				}
			}
			//CPRINTK("%s, counter2:%d\n", __func__, counter);

			rspBuf1[rspNum-1] = SD_RSP_BUF4_5_GET & 0xff;
		}
		for(i = 1; i < 4; ++i){	
			*(u32 *)(rspBuf1 + i*4) = swab32(*(u32 *)(rspBuf1 + i*4));
		}
	}
out:
	letb8388_SDIO_UNLOCK(&host->lock, host->flag);
	return status;
}




static  inline void set_sd_media_type_mode(u32 ActFlash, u32 PageSize)
{
	CARD_MEDIA_TYPE_SET(ActFlash);
	DMA_SIZE_SET((PageSize - 1) & 0x3ff);	/*setup dma buffer size*/
}


static u32 nomal_dma_err_check(struct sphe8388_sdio_priv_host *host)
{
	u32 state = 0;
	int ret = 0;
	u32 timeout = 100000; // 
	letb8388_SDIO_LOCK(&host->lock, host->flag);	
	if(is_nomal_dma_err_timeout()){	
		state = DMA_SRC_DST_GET();
		if((state&0x07) == DMA_DRAM){
			if (!hwSdCardCrcState()){
				tune_rd_dly((void *)host, -1, -1);
				EPRINTK("Write crc ERR, cmd:%d, sd status:0x%x, sd state:%d\n",
						SD_CMD_BUF0_GET(), SD_STATUS0_GET(), SD_STATUS1_GET());
			}else{
			   // if(GET_BIT_N(SD_STATUS0_GET(), 6)){
                              tune_wr_dly((void *)host, -1);
                            
                       // }
			}
		}else{
			if(SD_CRC16_FLAG_GET()){
				EPRINTK(KERN_ERR "Read CRC ERR, cmd:%d, arg:0x%02x%02x%02x%02x, "
						"status:0x%x, state:%d", 
						SD_CMD_BUF0_GET()&0xff, SD_CMD_BUF1_GET(), SD_CMD_BUF2_GET(),
						SD_CMD_BUF3_GET(),      SD_CMD_BUF4_GET(),  SD_STATUS0_GET(), 
						SD_STATUS1_GET() & 0x0f);
				tune_rd_dly((void *)host, -1, -1);
			}else{
				tune_wr_dly((void *)host, -1);
			}
		}
		debug_get_register_statues();
		clear_sd_cmp_int();
		disable_sd_cmp_int();
		sw_rst_channel();
		while(get_rst_channel_stat() && timeout){
			udelay(1);
			--timeout;
		}
		ret = -ETIMEDOUT;
		goto out;
	}else{
		clear_sd_cmp_int();
		disable_sd_cmp_int();
	}
out:
	letb8388_SDIO_UNLOCK(&host->lock, host->flag);	
	return ret;
}


static int wait_sd_dma_done(void* data)
{
	int ret = SP_SD_SUCCESS;
	struct sphe8388_sdio_priv_host *host = (sphe8388_sdio_priv_host *)data;

#if 0
	if(SD_BLOCKSIZE_GET()<512){
		ret = wait_event_timeout(host->interrupt.sd_int.wq_sd_irq, 
				(host->interrupt.sd_int.irq_sd_stat != SD_INT_DATA_XFER_CMP_DMA), 
				1*HZ);
	}
	else if((SD_CMD_BUF0_GET() == MMC_READ_SINGLE_BLOCK) 
			|| (SD_CMD_BUF0_GET() == MMC_READ_MULTIPLE_BLOCK)){
		ret = wait_event_timeout(host->interrupt.sd_int.wq_sd_irq, 
				(host->interrupt.sd_int.irq_sd_stat != SD_INT_DATA_XFER_CMP_DMA), 
				4*HZ);
	}else{
		ret = wait_event_timeout(host->interrupt.sd_int.wq_sd_irq, 
				(host->interrupt.sd_int.irq_sd_stat != SD_INT_DATA_XFER_CMP_DMA), 
				8*HZ);
	}
#endif
	ret = wait_event_timeout(host->interrupt.sd_int.wq_sd_irq, 
			(host->interrupt.sd_int.irq_sd_stat != SD_INT_DATA_XFER_CMP_DMA), 
			4*HZ);
	ASSERT_GOTO(!ret, out,"wait_event_timeout err\n");	
	
	ret = nomal_dma_err_check(host);
	ASSERT_GOTO(ret, out,"nomal_dma_err_check err\n");	

	return SP_SD_SUCCESS;
out:

	return -ETIMEDOUT;
}

static  inline void hw_sd_dma_init(u32 srcDst, u32 dmaSize,u32  pageNum)
{
	u32 wr = 0;

	SD_BLOCKSIZE_SET(dmaSize -1);//g2.reg21
	SD_PAGE_NUM_SET(pageNum);		// g2.reg13
	if(DMA_DRAM == (srcDst & 0x07)){
		wr = 1;
	}
	config0_for_dma(wr);		// g2.reg14	

	DMA_SRC_DST_SET(srcDst);
	DMA_SIZE_SET((dmaSize - 1) & 0x3ff);	/*setup dma buffer size*/

}


static irqreturn_t sphe8388_sdio_irq(int irq, void *dev_id)
{
	unsigned long flag = 0;	
	struct mmc_host *mmc_host = (struct mmc_host *)dev_id;	
	struct sphe8388_sdio_priv_host *host = (struct sphe8388_sdio_priv_host *)mmc_priv(mmc_host);

	spin_lock_irqsave(&host->interrupt.lock,flag);
#if 0 // hw only support nomal DMA, nomal DMAuse SD_CMP INT
	if (is_dma_int())
	{
		letb8388_sdio_dma_irq(irq, dev_id);
	}
#endif
	if ((SDIO_STATE_DISABLE ==host->interrupt.sdio_int.state_sdio_int)
			&& !is_sdio_int() && !is_sd_cmp_int()){
		printk("unkown int coming\n");
	}

	if (is_sd_cmp_int()){
		sphe8388_sd_cmp_irq(irq, dev_id);
	}

	if (is_sdio_int()){
		sphe8388_sdio_card_irq(irq, dev_id);
	}

	spin_unlock_irqrestore(&host->interrupt.lock,flag);

	return IRQ_HANDLED;
}

static irqreturn_t sphe8388_sdio_card_irq(int irq, void *dev_id)
{
	struct mmc_host *mmc_host = (struct mmc_host *)dev_id;
	struct sphe8388_sdio_priv_host *sdio_host = mmc_priv(mmc_host);
	//IFPRINTK("%s:%d\n", __FUNCTION__, __LINE__);
	//debug_get_register_statues();
	//g_is_card_old = g_is_card_int;
	if(SDIO_STATE_DISABLE ==sdio_host->interrupt.sdio_int.state_sdio_int){
		//disable_sdio_int();
	}else if (mmc_host && mmc_host->sdio_irq_thread){
		mmc_signal_sdio_irq(mmc_host);
		sdio_int_clr();
	}
	return IRQ_HANDLED;
}
static irqreturn_t sphe8388_sd_cmp_irq(int irq, void *dev_id)
{

	struct mmc_host *mmc_host = (struct mmc_host *)dev_id;
	struct sphe8388_sdio_priv_host *host =  (struct sphe8388_sdio_priv_host *)mmc_priv(mmc_host);
	if((SD_INT_CMD_BEFORE_RSP_FULL != host->interrupt.sd_int.irq_sd_stat)
			&& (SD_INT_DATA_XFER_CMP_PIO != host->interrupt.sd_int.irq_sd_stat)){
		wake_up(&host->interrupt.sd_int.wq_sd_irq);
	}
	SD_SET_INT_STATE(host->interrupt.sd_int.lock_Sd_irq, 
			host->interrupt.sd_int.irq_sd_stat,
			SD_INT_INIT);
      //disable_sd_cmp_int();
	clear_sd_cmp_int();
	return IRQ_HANDLED;
}


u32 debug_get_register_statues(void)
{

	//DPRINTK("%s:%d\n", __FUNCTION__, __LINE__);
	//IFPRINTK("send cmd:%0x, arg:%0x\n\n", cmd, arg);
	/* g2.reg27 - g2.reg31s */
	FPRINTK("g2.reg27:%0x\n", SD_CMD_BUF0_GET());
	FPRINTK("g2.reg28:%0x\n", SD_CMD_BUF1_GET());
	FPRINTK("g2.reg29:%0x\n", SD_CMD_BUF2_GET());
	FPRINTK("g2.reg30:%0x\n", SD_CMD_BUF3_GET());
	FPRINTK("g2.reg31:%0x\n\n", SD_CMD_BUF4_GET());

	// g2.reg13
	FPRINTK("g2.reg13:%0x\n\n", SD_PAGE_NUM_GET());

	// g2.reg14
	FPRINTK("g2.reg14:%0x\n", SD_GET_CONFIG0());

	// g2.reg17
	FPRINTK("g2.reg17:%0x\n", SD_CONFIG_GET());

	//g2.reg12 
	FPRINTK("g2.reg12:%0x\n", SD_INT_CONTROL_GET());

	FPRINTK("g2.19:%0x\n", SD_STATUS0_GET()); 
	FPRINTK("g2.20:%0x\n", SD_STATUS1_GET()); 
	FPRINTK("g2.reg21:%0x\n\n", SD_BLOCKSIZE_GET());

	//g7 1 2 5 6
	FPRINTK("g7.reg1:%0x\n\n", DMA_SRC_DST_GET());
	FPRINTK("g7.reg2:%0x\n\n", DMA_SIZE_GET());
	FPRINTK("g7.reg5:%0x\n\n", DMA_BASE_ADDRL_GET());
	FPRINTK("g7.reg6:%0x\n\n", DMA_BASE_ADDRH_GET());
	FPRINTK("\n\n");
	return 0;
}


static  void config1_for_send_cmd(u32 rsp_type, sdio_cmd_type_t cmd_type)
{
	u32 val = SD_CONFIG_GET();
	// 
	if(RSP_TYPE_R2 == rsp_type){
		SET_BIT_N(val, 11);
	}else{
		CLR_BIT_N(val, 11);
	}

	SD_CONFIG_SET(val);
}


static int hw_prepare_for_send_cmd(sphe8388_sdio_priv_host *host, u32 cmd, u32 arg, u32 rspType, u32 cmdType)
{
	//reset_dut_sdio();
		
	letb8388_SDIO_LOCK(&host->lock,host->flag);
	config_send_buf(cmd, arg);
	config0_for_send_cmd(cmd, rspType, cmdType);
	config1_for_send_cmd(rspType, cmdType);
	#if (CURRENT_USED_CARD_CTRL==4)
		if(SP_BUS_WIDTH_4BIT == host->bus_width){
			if(SD_IO_RW_EXTENDED == cmd){
				set_bus_wide(SP_BUS_WIDTH_4BIT);
			}else{
				set_bus_wide(SP_BUS_WIDTH_1BIT);
			}
		}
	#endif
	letb8388_SDIO_UNLOCK(&host->lock, host->flag);
	return SP_SD_SUCCESS;
}

static u32 wait_for_rsp_buff_full(struct sphe8388_sdio_priv_host *host)
{
	u32 counter = 0;
	u32 ret = SP_SD_SUCCESS;
	while(!sd_rsp_buff_full()){
		if((SD_INT_INIT == host->interrupt.sd_int.irq_sd_stat) // INT coming before rsp buffer full 
				|| unlikely(counter > 1000000)){
			// rsp timeout			
			ret = -ETIMEDOUT;
			return ret;
		}
		++counter;
		udelay(1);	
	}
	return ret;
}



static u32 sd_check_rsp_timeout(void *host, u32 cmd)
{
	u32 val = SD_STATUS0_GET(); 
	if(GET_BIT_N(val, 6)){
		if((12 != cmd) && (8 != cmd)){
			tune_wr_dly(host, -1);	
			EPRINTK("response timeout\n");
			return -ETIMEDOUT;
		}else{
			reset_dut_sdio();
			return 0;
		}
	}else if(GET_BIT_N(val, 9)){
		tune_rd_dly(host, -1, -1);
		EPRINTK("CRC error\n");
		return -EINVAL;
	}else{
		return 0;
	}
}

static u32 xfer_cmd_without_rsp(
		struct sphe8388_sdio_priv_host *host, 
		u32	 cmd, 
		u32 arg, 
		u32 rspType, 
		struct mmc_command *mmc_cmd)
{
	u32 ret = 0;
	SD_SET_INT_STATE(host->interrupt.sd_int.lock_Sd_irq, 
			host->interrupt.sd_int.irq_sd_stat, 
			SD_INT_ONLY_CMD_XFER_CMP);
	
	letb8388_SDIO_LOCK(&host->lock, host->flag);
	enable_sd_cmp_int();
	hw_start_transation();
	letb8388_SDIO_UNLOCK(&host->lock, host->flag);
	ret = wait_event_timeout(host->interrupt.sd_int.wq_sd_irq, 
			(host->interrupt.sd_int.irq_sd_stat != SD_INT_ONLY_CMD_XFER_CMP),
			WAIT_CMD_XFER_TIME);		
	ASSERT_GOTO(!ret, err, "wait_event_timeout err cmd=%d\n",cmd);

	return 0;
err:
	return -ETIMEDOUT;
}

static u32 xfer_cmd_with_17bytes_rsp(
		struct sphe8388_sdio_priv_host *host, 
		u32	 cmd, 
		u32 arg, 
		u32 rspType, 
		struct mmc_command *mmc_cmd)
{
	u32 ret = 0;

	SD_SET_INT_STATE(host->interrupt.sd_int.lock_Sd_irq, 
			host->interrupt.sd_int.irq_sd_stat, 
			SD_INT_CMD_BEFORE_RSP_FULL);
	letb8388_SDIO_LOCK(&host->lock, host->flag);
	enable_sd_cmp_int();
	hw_start_transation();
	ret = wait_for_rsp_buff_full(host);
	letb8388_SDIO_UNLOCK(&host->lock, host->flag);
	ASSERT_GOTO(ret, out, "cmd:%d wait_for_rsp_buff_full time out\n",cmd);

	ret = rx_response(host, mmc_cmd, rspType, cmd);
	ASSERT_GOTO(ret != SP_SD_SUCCESS, out, "cmd%d error %d\n", cmd, ret);
	SD_SET_INT_STATE(host->interrupt.sd_int.lock_Sd_irq, 
			host->interrupt.sd_int.irq_sd_stat, 
			SD_INT_CMD_RSP_XFER_CMP);

	ret = wait_event_timeout(host->interrupt.sd_int.wq_sd_irq, 
			(host->interrupt.sd_int.irq_sd_stat != SD_INT_CMD_RSP_XFER_CMP),
			WAIT_CMD_XFER_TIME);
	ASSERT_GOTO(!ret, out, "wait_event_timeout err cmd = %d\n",cmd);

	ret = sd_check_rsp_timeout((void *)host, cmd);
	ASSERT_GOTO(ret, out, "cmd:%d CRC error\n",cmd);
	return 0;
out:	
	return  -ETIMEDOUT;
}

static u32 xfer_cmd_with_6bytes_rsp(
		struct sphe8388_sdio_priv_host *host, 
		u32	 cmd, 
		u32 arg, 
		u32 rspType, 
		struct mmc_command *mmc_cmd)
{
	u32 ret = 0;

	SD_SET_INT_STATE(host->interrupt.sd_int.lock_Sd_irq, 
			host->interrupt.sd_int.irq_sd_stat, 
			SD_INT_CMD_RSP_XFER_CMP);
	letb8388_SDIO_LOCK(&host->lock, host->flag);
	enable_sd_cmp_int();
	hw_start_transation();
	letb8388_SDIO_UNLOCK(&host->lock, host->flag);
	ret = wait_event_timeout(host->interrupt.sd_int.wq_sd_irq, 
			(host->interrupt.sd_int.irq_sd_stat != SD_INT_CMD_RSP_XFER_CMP),
			WAIT_CMD_XFER_TIME);
	ASSERT_GOTO(!ret, out, "wait_event_timeout err cmd = %d\n",cmd);
	ret = sd_check_rsp_timeout((void *)host, cmd);
	ASSERT_GOTO(ret, out1, "cmd:%d hw rsp time out or crc error\n",cmd);

	ret = rx_response(host, mmc_cmd, rspType, cmd);
	ASSERT_GOTO(ret != SP_SD_SUCCESS, out, "cmd%d error %d\n", cmd, ret);
	return 0;
out:
	return -ETIMEDOUT;
out1:
	return SP_SD_RSP_TIMEOUT;
}

static u32 hw_sd_send_only_cmd(struct sphe8388_sdio_priv_host *host, u32 cmd, u32 arg, u32 rspType, struct mmc_command *mmc_cmd)
{
	u32 ret = 0;

	if(RSP_TYPE_NORSP == rspType){ // cmd without rsp	
		ret = xfer_cmd_without_rsp(host, cmd, arg, rspType, mmc_cmd);
	}else if(RSP_TYPE_R2 == rspType){// cmd with R2 rsp  (17bytes rsp)	
		ret = xfer_cmd_with_17bytes_rsp(host, cmd, arg, rspType, mmc_cmd);
	}else{// cmd with 6 bytes rsp
		ret = xfer_cmd_with_6bytes_rsp(host, cmd, arg, rspType, mmc_cmd);
	}
	ASSERT_GOTO(ret, out, "cmd:%d error:%d in hw_sd_send_cmd\n", cmd, ret);

	return 0;

out:
	return ret;
}

static int hw_sd_send_cmd(void *host_data, u32 cmd, u32 arg, u32 rspType, struct mmc_command *mmc_cmd, u32 cmdType)
{
	u32 ret = 0;
	sphe8388_sdio_priv_host *host = (sphe8388_sdio_priv_host *)host_data;

	hw_prepare_for_send_cmd(host,cmd, arg, rspType, cmdType);
	switch(cmdType){
		case SDIO_CMD_OLY_CMD:
			ret = hw_sd_send_only_cmd(host, cmd, arg, rspType, mmc_cmd);
			break;
		case SDIO_CMD_READ:
		case SDIO_CMD_WRITE:
			ret = sdio_data_transfer(host->mrq, host,  rspType, mmc_cmd);
			break;
		default:
			break;
	}
	ASSERT_GOTO(ret, err, "cmd %d send error\n", cmd);

	return 0;	
err:
	reset_dut_sdio();
	return ret;
}

static unsigned int sphe8388_get_Sd_rsptype(struct mmc_command *cmd)
{
	unsigned int sphe_resp_type;

	switch (mmc_resp_type(cmd)) {
		case MMC_RSP_R1:	/* short CRC, OPCODE */
			sphe_resp_type = RSP_TYPE_R1;
			break;
		case MMC_RSP_R1B:	/* short CRC, OPCODE, BUSY */
			sphe_resp_type = RSP_TYPE_R1;
			break;
		case MMC_RSP_R2:	/* long 136 bit + CRC */
			sphe_resp_type = RSP_TYPE_R2;
			break;
		case MMC_RSP_R3:	/* short */
			sphe_resp_type = RSP_TYPE_R3;
			break;
		default:
			sphe_resp_type = RSP_TYPE_NORSP;
			break;
	}

	DPRINTK("mmc resp type: %d, sphe resp type: %d\n", cmd->flags, sphe_resp_type);

	return sphe_resp_type;
}

static void sphe_mmc_finish_request(struct sphe8388_sdio_priv_host *host,
		struct mmc_request *mrq)
{
	host->mrq = NULL;
	host->cmd = NULL;
	host->data = NULL;
	mmc_request_done(host->mmc, mrq);
}



static u32 hw_xfer_data_pio(struct mmc_data *mmcData, u32 data_len, u32 wr,
		struct sphe8388_sdio_priv_host *host)
{
	volatile u32 val = 0, sg_index=0;
	u32 index = 0;
	u32 timeout = 4000000;
	u8 *data = sg_virt(&mmcData->sg[0]);
	u32 data_cnt = 0;
	sd_int_info_t  *int_info =  &host->interrupt.sd_int;
	SD_SET_INT_STATE(int_info->lock_Sd_irq, int_info->irq_sd_stat, SD_INT_DATA_XFER_CMP_PIO);
	enable_sd_cmp_int();        
        if(wr){
		// write pio tx buf
		SD_PIO_TX_DATA_SET((u32)data[index] | ((u32)data[index + 1] << 8));
		index += 2;
		data_cnt += 2;
	}
	while(int_info->irq_sd_stat == SD_INT_DATA_XFER_CMP_PIO)
	{

		if(sg_index >= mmcData->sg_len){	
			break;
		}
		if(index >= mmcData->sg[sg_index].length){	
			++sg_index;
			if(sg_index < mmcData->sg_len){
				data = sg_virt(&mmcData->sg[sg_index]);
			}else{
				continue;
			}
			index = 0;
		}

		val = SD_STATUS0_GET();
		if(wr){
			if(GET_BIT_N(val, 2)){
				// write pio tx buf
				SD_PIO_TX_DATA_SET(((u32)data[index]) | (((u32)data[index + 1]) << 8));
				index += 2;
				data_cnt += 2;
			}else{
				letb8388_SDIO_UNLOCK(&host->lock, host->flag);				
				udelay(1);						
				--timeout;
				letb8388_SDIO_LOCK(&host->lock, host->flag);
				ASSERT_GOTO(!timeout, err,"soft wait timeout when write data\n");
			}

		}else{
			if(GET_BIT_N(val, 1)){
				// read pio rx buf
				u32 val1  = SD_PIO_RX_DATA_GET();
				memcpy(&data[index], &val1, 4);		
				index += 4;
				data_cnt += 4;	
			}else{
				letb8388_SDIO_UNLOCK(&host->lock, host->flag);	
				udelay(1);	
				--timeout;
				letb8388_SDIO_LOCK(&host->lock, host->flag);	
				ASSERT_GOTO(!timeout ,err,  "soft wait timeout when read data\n");
			}
		}
	}

	if(data_cnt < data_len){
		val = SD_STATUS0_GET();
		if(wr){
			if(GET_BIT_N(val, 2)){
				// write pio tx buf
				SD_PIO_TX_DATA_SET(((u32)data[index]) | (((u32)data[index + 1]) << 8));
				index += 2;
				data_cnt += 2;	
			}else {
				EPRINTK("pio write data error expect write %d bytes, actual write %dbytes\n", data_len, data_cnt);
			}
		}else{
			if(GET_BIT_N(val, 3)){
				// read pio rx buf	
				u32 val1  = SD_PIO_RX_DATA_GET();
				memcpy(&data[index], &val1, 4);		
				index += 4;
				data_cnt += 4;	
			}else{
				EPRINTK("pio read data error expect read %d bytes, actual read %d bytes\n", data_len, data_cnt);
			}
		}

	}

	ASSERT_GOTO(GET_BIT_N(SD_STATUS1_GET(), 13),err,
			"pio xfer %s data hw error or timeout, sd_status0:0x%0x,  sd_status1:0x%0x\n", 
			wr ? "write" : "read", SD_STATUS0_GET(), SD_STATUS1_GET());

	return 0;

err:
	if(wr){
		tune_wr_dly((void *)host, -1);
	}else {
		tune_rd_dly((void *)host, -1, -1);
	}


	return -ETIMEDOUT;
}

static s32 sphe8388_sdio_transfer_data_pio(struct sphe8388_sdio_priv_host *host)
{
	u32 ret = 0, data_len =0;
	struct mmc_data *data = host->data;
	u32 wr = (data->flags & MMC_DATA_WRITE) ?  1 : 0;
	u32 pages = 0;

	ret = hw_prepare_xfer_data_pio(wr);
	ASSERT_GOTO(ret, err, "hw_prepare_xfer_data_pio error\n");

	pages = data->blocks; 
	data_len = data->blocks * data->blksz;
	SD_BLOCKSIZE_SET(data->blksz - 1);
	SD_PAGE_NUM_SET((pages > 0) ? (pages - 1):0);		// g2.reg13	
	hw_start_transation();
	ret = hw_xfer_data_pio(data, data_len, wr, host);

	ASSERT_GOTO(ret, err, "hw_send_data_pio error\n");

	return SP_SD_SUCCESS;

err:
	return ret;

}

static s32 sphe8388_sdio_transfer_data_dma(struct sphe8388_sdio_priv_host *host)
{

	int ret = 0;
	struct mmc_data *data = host->data;
	int data_len = data->blocks * data->blksz;
	u8* data_buf = NULL;
	u32 nrPages = data->blocks;
	u8 * phy_addr = NULL;
	struct scatterlist *sg	= NULL;
	
	if(data->sg_len > 1){
		EPRINTK("data->sg_len = %d\n", data->sg_len);
		return  -EINVAL;
	}

	sg = &data->sg[0];
	data_buf = sg_virt(sg);
	if (!data_buf)
		data_buf = (u8 *) sg_dma_address(sg);

	if(nrPages > 0){
		--nrPages; 
	}
	letb8388_SDIO_LOCK(&host->lock, host->flag);	
	DMA_HW_EN_SET_NORMAL_DMA();
	//SD_AUTODMA_TYPE_SET(DMA_TYPE_SINGLEBLOCK_CMD);
	//SD_HW_CMD13_EN(0);
	//SD_HCSD_EN_SET(0);

	if (host->data->flags & MMC_DATA_WRITE)	{
		hw_sd_dma_init((DMA_FLASH << SP_DMA_DST_SHIFT) | DMA_DRAM, data->blksz, nrPages);
		phy_addr = (unsigned char *) dma_map_single(NULL, (void *)data_buf, data_len, DMA_TO_DEVICE);
		SET_HW_DMA_BASE_ADDR((u32)phy_addr);	
		SD_SET_INT_STATE(host->interrupt.sd_int.lock_Sd_irq, 
								host->interrupt.sd_int.irq_sd_stat,
								SD_INT_DATA_XFER_CMP_DMA); 
		enable_sd_cmp_int();
		hw_start_transation();
		letb8388_SDIO_UNLOCK(&host->lock, host->flag);	
		ret = wait_sd_dma_done(host);
		dma_unmap_single(NULL, (dma_addr_t)phy_addr, data_len, DMA_TO_DEVICE);
	}else if (host->data->flags & MMC_DATA_READ)	{
		hw_sd_dma_init((DMA_DRAM << SP_DMA_DST_SHIFT) | DMA_FLASH, data->blksz, nrPages);
		phy_addr = (unsigned char *) dma_map_single(NULL, (void *)data_buf, data_len, DMA_FROM_DEVICE);
		SET_HW_DMA_BASE_ADDR((unsigned int)phy_addr);
		SD_SET_INT_STATE(host->interrupt.sd_int.lock_Sd_irq, 
								host->interrupt.sd_int.irq_sd_stat,
								SD_INT_DATA_XFER_CMP_DMA); 
		enable_sd_cmp_int();
		hw_start_transation();
		letb8388_SDIO_UNLOCK(&host->lock, host->flag);	
		ret = wait_sd_dma_done(host);
		dma_unmap_single(NULL, (dma_addr_t)phy_addr, data_len, DMA_FROM_DEVICE);
             if(mmc_printk & MMC_PRINTF_IF){
                sdio_dump_buffer(data_buf, data_len);
            }
    }else{
		EPRINTK(KERN_ERR "Err data flags\n");
		ret = SP_SD_FAIL;
	}

	ASSERT_GOTO(ret, err, "DMA Err, ret:0x%x\n", ret);

	data->error = 0;

	return 0;
err:	
	data->error = -ETIMEDOUT;
	return -ETIMEDOUT;
}

static int send_stop_command(struct sphe8388_sdio_priv_host *host)
{
	struct mmc_command *cmd = host->mrq->stop;
	int counter = 0;
	int ret;


	if(cmd)	//determine from if the stop is from the mrq or by the driver itself.
		cmd->error = 0;


	do{
		if (unlikely(counter++ > 15)){
			EPRINTK("retry send stop command fail %d times\n", counter);
			break;	//assume SUCCESS, no more send
		}

		ret = hw_sd_send_cmd((void *)host, cmd->opcode, cmd->arg, RSP_TYPE_R1, cmd, SDIO_CMD_OLY_CMD);
		if(ret == SP_SD_SUCCESS){
			break;
		}

		if (ret == SP_SD_RSP_TIMEOUT)
			break;

	}while(1);
	CPRINTK("%s, counter:%d\n", __func__, counter);
	return SP_SD_SUCCESS;
}


static u32 sdio_data_transfer(struct mmc_request *mrq, struct sphe8388_sdio_priv_host *host, 
		u32 rspType, struct mmc_command *mmc_cmd)
{
	u32 ret = SP_SD_SUCCESS;
	if (!mrq->data){
		return SP_SD_SUCCESS;
	}

	/*Data process */
	if(SDIO_DMA_MODE == g_sdio_pio_dma_mode){// DMA mode
		ret = sphe8388_sdio_transfer_data_dma(host);
	}else{  // pio mode
		ret = sphe8388_sdio_transfer_data_pio(host);
	}
	if (host->data->error == 0){
		host->data->bytes_xfered = host->data->blocks * host->data->blksz;
	}else{
		host->data->bytes_xfered = 0;
	}
	ASSERT_GOTO(ret, err, "receive data error, ret:0x%x\n", ret);
	//tx_dummy(5);
	ret = rx_response(host, mmc_cmd, rspType, mrq->cmd->opcode);
	ASSERT_GOTO(ret, err, "rx_response error\n");
	return ret;
err:
	host->data->error = -ETIMEDOUT;
	host->data->bytes_xfered = 0;
	return -ETIMEDOUT;
}

static u32 sphe8388_get_Sdio_cmdtype(struct sphe8388_sdio_priv_host *host)
{
	if(!host->data){
		return SDIO_CMD_OLY_CMD;
	}else if(host->data->flags & MMC_DATA_WRITE){
		return SDIO_CMD_WRITE;
	}else if(host->data->flags & MMC_DATA_READ){
		return SDIO_CMD_READ;
	}else{
		EPRINTK("cmd %d falg:0x%0x with unsupport cmd type\n", host->cmd->opcode, host->data->flags);
		return SDIO_CMD_STREAM;
	}
}
static int sphe8388_send_sdio_cmd(struct mmc_host *mmc)
{
	u32 rsp_type;
	struct sphe8388_sdio_priv_host *host = mmc_priv(mmc);
	struct mmc_request *mrq = host->mrq;
	struct mmc_command *cmd = host->cmd_is_stop ? mrq->stop : mrq->cmd;
	s32 ret = 0;
	u32 cmdType = 0;

	host->cmd = cmd;
	cmd->error = 0;
	rsp_type = sphe8388_get_Sd_rsptype(cmd);
	cmdType = sphe8388_get_Sdio_cmdtype(host);
	ret = hw_sd_send_cmd((void *)host,cmd->opcode, cmd->arg, rsp_type, cmd, cmdType);
	ASSERT_GOTO(ret, out, "cmd:%d, arg:0x%08x, err:0x%x, reponse:0x%x\n",
			cmd->opcode, cmd->arg, ret, cmd->resp[0]);

	// send stop cmd
	if (mrq->stop){
		ret = send_stop_command(host);
		if(ret != SP_SD_SUCCESS){
			mrq->stop->error = -EINVAL;
			mrq->data->error = -EINVAL;
			mrq->data->bytes_xfered = 0;
		}
	}
	return 0;
out:
	mrq->cmd->error = -ETIMEDOUT;
	if(mrq->data){
		mrq->data->error = -ETIMEDOUT;
	}

	if((8 == host->cmd->opcode) || ((12 == host->cmd->opcode))){
		ret = 0;
	}
	return ret;
}

#if 0
int fix_Sd_cmd_error(struct mmc_request *mrq, int flag)
{
	u8 rsp_buf[17];
	int ret;

	tx_dummy(150);

	ret = hw_sd_send_cmd(MMC_STOP_TRANSMISSION, 0, RSP_TYPE_R1, rsp_buf);
	if (ret != SP_SD_SUCCESS) {
		EPRINTK("Send Stop Error..\n");
		return 1;
	}

	if (flag & 1) {
		EPRINTK("\nCMD = %d Arg = 0x%x err = %d\n", mrq->cmd->opcode, mrq->cmd->arg, mrq->cmd->error);
	}
	if (flag & 2) {
		EPRINTK("\nStop CMD = %d Arg = 0x%x\n", mrq->stop->opcode, mrq->stop->arg);
		tx_dummy(150);
		ret = hw_sd_send_cmd(MMC_STOP_TRANSMISSION, 0, RSP_TYPE_R1, rsp_buf);
		if (ret == SP_SD_SUCCESS)
			return 0;
		EPRINTK("Send Stop Error\n");
		return 1;
	}

	return 0;
}
#endif
#if 0
static bool special_deal_after_send_cmd(s32 retval, struct letb8388_sdio_host* host, s32 retrytime)
{
	struct mmc_request *mrq = host->mrq;
	if((mrq->cmd->opcode == MMC_SWITCH) 
			|| (mrq->cmd->opcode == SD_APP_SD_STATUS) 
			|| (mrq->cmd->opcode == SD_APP_SEND_SCR) ){	
		//CMD6, ACMD13, ACMD51, ACMD22
		//CMD6 should send stop after cmd sent, and wait for trans beofre send,
		int ret;
		Send_Stop_command(host);

		//this is for the next commands after stop command is sent,
		//if next command is ACMD6 and the state is not is trans, there will be response timeout ,
		//When CMD55 is to sent, there is no information for which further app cmd is to be sent.
		//
		//sdcard0: r/w command failed, status = 0xb377c238
		//end_request: I/O error, dev sdcard0, sector 0
		//Buffer I/O error on device sdcard0, logical block 0
		//sdcard0: r/w command failed, status = 0xe0d9c100
		//end_request: I/O error, dev sdcard0, sector 0
		//Buffer I/O error on device sdcard0, logical block 0
		//sdcard0: r/w command failed, status = 0xe0d9c140
		//end_request: I/O error, dev sdcard0, sector 0
		//Buffer I/O error on device sdcard0, logical block 0
		ret = WaitCardStateTrans();
		if (ret != SP_SD_SUCCESS) {
			printk("wait card state trans err....\n");
		}
	}

	if((mrq->data) && (retval & SP_SD_CRC_ERROR)){
		EPRINTK("----SP_SD_CRC_ERROR ret:%d, cmd:%d(0x%x), err:0x%x, data err:0x%x\n", retval, (mrq->cmd->opcode&0xff), mrq->cmd->opcode, mrq->cmd->error, mrq->data->error);
		if(mrq->cmd->opcode == MMC_SWITCH){ //don't tune the clock(tune back), retry several times will be OK
			//for CMD6, do not tune the dly every time, -1:tune back, retrytime/3: tune once every 3 times
			tune_rd_dly(readDelay-1+(retrytime/3), g_sd_highspeed_flag);

			Send_Stop_command(host);	// 4.3 Data Transfer Mode, State Diagram: send cmd12, the state will stay in tranfer_state

			EPRINTK("Retry:%d, cmd:%d, ret:%d\n", retrytime, mrq->cmd->opcode & 0xff, retval);
			goto	err;
		}

		if(mrq->cmd->opcode == SD_APP_SD_STATUS){	//acmd13
			Send_Stop_command(host);	// 4.3 Data Transfer Mode, State Diagram: send cmd12, the state will stay in tranfer_state
			if(mrq->data){
				mrq->data->bytes_xfered = 64;
			}	
			goto err;
		}

		if((mrq->cmd->opcode&0xff) == 51){	//commands that ignore error
			goto out;
		}

		if((mrq->cmd->opcode == MMC_READ_SINGLE_BLOCK) 
				|| (mrq->cmd->opcode == MMC_READ_MULTIPLE_BLOCK)){
			//tune_dynamic_delay(&gv1);
			EPRINTK("---- cmd:%d, retrytime:%d\n", mrq->cmd->opcode, retrytime);
		}

		if( (mrq->cmd->opcode == MMC_WRITE_MULTIPLE_BLOCK) ){
			EPRINTK("----cmd:%d, retrytime:%d\n", mrq->cmd->opcode, retrytime);
			tune_wt_dly(WriteDelay-1+retrytime/2);
		}

		if((mrq->cmd->opcode == MMC_WRITE_BLOCK) ){
			EPRINTK("----cmd:%d, retrytime:%d\n", mrq->cmd->opcode, retrytime);
			tune_wt_dly(WriteDelay-1+retrytime/2);
			WaitCardStateTransAndReady();
			goto err;
		}

		fix_Sd_cmd_error(mrq, retval);
		EPRINTK("retry:%d, cmd:%d, ret:%d\n", retrytime, mrq->cmd->opcode & 0xff, retval);
		goto err;
	}

	if((mrq->data) && (retval == SP_SD_DMA_FAIL)){
		Send_Stop_command(host);
		WaitCardStateTransAndReady();
		EPRINTK("cmd:%d, retrytime:%d\n", mrq->cmd->opcode, retrytime);
		goto err;
	}

	if(retval & SP_SD_RSP_TIMEOUT){
		if((mrq->cmd->opcode == MMC_APP_CMD)){
			tune_wt_dly(WriteDelay-1+retrytime/2);
			goto err;
		}
	}

	//CMD5
	if((mrq->cmd->opcode == MMC_SLEEP_AWAKE)
			||(mrq->cmd->opcode == 52)
			||(mrq->cmd->opcode == 53)
			||(mrq->cmd->opcode == MMC_APP_CMD)
			||(mrq->stop)){
		goto out;//skip send stop command 
	}

	if (retval){
		if(mrq->cmd->opcode == MMC_SWITCH){
			EPRINTK("cmd6 retry:%d, no data, retval:%d\n", retrytime, retval);
			goto err;
		}
	}

out:
	return true;
err:
	return false;
}
#endif



static void sphe8388_sdio_request(struct mmc_host *mmc, struct mmc_request *mrq)
{

	struct sphe8388_sdio_priv_host *host = mmc_priv(mmc);
	s32 retval;
	int i =0;
	s32 retrytime = 0;
	//DEBUG_GET_FUNCTION_TIME_BEGIN();
	if(!host){	
		EPRINTK("get priv host error\n");
		return;
	}

	DPRINTK("\n%s, mrq:0x%p, cmd:%d, arg:0x%08x, data:0x%p, stop:0x%p\n",
			__func__, mrq, mrq->cmd->opcode&0xfff, mrq->cmd->arg,
			mrq->data, mrq->stop);

	//mutex_lock(&host->data_lock);	
	down(&host->sem);
	host->clk_dly_info.tune_begin_flag = 0;
	for(i = 0; i < SDIO_MAX_TUNE_DLY_TIMES; ++i){// tune  delay 8 times from 0 to 7 , tune begin  at i = 1, end at i = 8  
		if(i > 0){
			host->clk_dly_info.tune_begin_flag  = 1;
		}
		retrytime = 0;
		while(retrytime < SDIO_MAX_RETRY_TIME){//  
			if(retrytime > 0){
				host->clk_dly_info.tune_begin_flag  = 0;
			}
			host->mrq = mrq;
			host->data = mrq->data;
			host->cmd_is_stop = 0;
			mrq->cmd->error = 0;
			if(mrq->data){
				mrq->data->error = 0;
			}
			if (mrq->stop){
				mrq->stop->error = 0;
			}
			retval = sphe8388_send_sdio_cmd(mmc);
			if(retval){
				EPRINTK("ERROR ret:0x%x, cmd:%d(0x%x), arg:0x%x, err:0x%x, "
						"response:0x%x, data err:0x%x, retry:%d\n",
						retval, (mrq->cmd->opcode&0xff), mrq->cmd->opcode,
						mrq->cmd->arg, mrq->cmd->error, mrq->cmd->resp[0], 
						mrq->data?mrq->data->error:0, retrytime);
			}else{
				goto out;
			}
			++retrytime;

		}
	}

out:
	DPRINTK("\nmrq:0x%p, cmd:%d, err:%d, response:0x%x, data:0x%p, "
			"data err:%d, len:%d, stop res:%d\n\n",
			mrq, mrq->cmd->opcode&0xfff, 
			mrq->cmd->error, mrq->cmd->resp[0], 
			mrq->data,  
			(mrq->data)?(mrq->data->error):0, 
			(mrq->data)?(mrq->data->bytes_xfered):0, 
			(mrq->stop)?(mrq->stop->error):0);

	host->clk_dly_info.tune_begin_flag  = 1;
	sphe_mmc_finish_request(host, host->mrq);

	//mutex_unlock(&host->data_lock);
	up(&host->sem);
}

static void letb8388_sdio_set_ios(struct mmc_host *mmc, struct mmc_ios *ios)
{
	u32 bus_width;
	//int ret = 0;
	struct sphe8388_sdio_priv_host *host = mmc_priv(mmc);

	DPRINTK("ios->clock=%d\n", ios->clock);
	DPRINTK("ios->bus_mode=%d\n", ios->bus_mode);
	DPRINTK("ios->powermode=%d\n", ios->power_mode);
	DPRINTK("ios->bus_width=%d\n", ios->bus_width);
	/*
	 * Reset the chip on each power off.
	 * Should clear out any weird states.
	 */
	letb8388_SDIO_LOCK(&host->lock, host->flag);
	if (host->power_mode != ios->power_mode) {
		host->power_mode = ios->power_mode;
		switch (ios->power_mode) {
			case MMC_POWER_ON:
				FPRINTK("set MMC_POWER_ON\n");
				break;
			case MMC_POWER_UP:
				/* Set default control register */
				FPRINTK("set MMC_POWER_UP\n");

				set_sd_media_type_mode(MEDIA_SD, 512);
				hwSdInit(MMC_MODE_CHIOCE);

				hw_sd_config(SD_CLK_DIV_FACTOR, SD_1BIT_MODE, MMC_MODE_CHIOCE);
				SD_BLOCKSIZE_SET(512);
				DMA_HW_STOP_RST_SET(7);

				tune_rd_dly((void *)host, host->clk_dly_info.read_delay, host->highspeed_flag);
				tune_wr_dly((void *)host, host->clk_dly_info.write_delay);
				tune_clk_dly((void *)host, host->clk_dly_info.clock_delay);

				set_sd_media_type_mode(MEDIA_SD, 512);
				break;
			case MMC_POWER_OFF:
				SD_RST_seq();
				DMA_STATE_RESET();
				break;
		}
	}


	if (ios->bus_width == MMC_BUS_WIDTH_1){
		bus_width = SP_BUS_WIDTH_1BIT;	/*01bit*/
	}else if (ios->bus_width == MMC_BUS_WIDTH_4){
		bus_width = SP_BUS_WIDTH_4BIT;	/*04bit*/
	}else{
		bus_width = SP_BUS_WIDTH_8BIT;	/*08bit*/
	}
	hw_sd_config_bus_width(bus_width);
	host->bus_width = bus_width;
	
	hw_sd_config_mmc_mode(MMC_MODE_CHIOCE);
	if(ios->clock > 0){
		hw_sd_config_freq(ios->clock);
	}
	if (ios->timing == MMC_TIMING_LEGACY){
		tune_rd_dly((void *)host, host->clk_dly_info.read_delay, 0);
	}else if ((ios->timing == MMC_TIMING_SD_HS) 
			|| (ios->timing == MMC_TIMING_MMC_HS)){
		tune_rd_dly((void *)host, host->clk_dly_info.read_delay, 1);
	}
	letb8388_SDIO_UNLOCK(&host->lock, host->flag);
}

static int letb8388_sdio_get_ro(struct mmc_host *mmc)
{
	return 0;
}
/*
 * Return values for the get_cd callback should be:
 *   0 for a absent card
 *   1 for a present card
 *   -ENOSYS when not supported (equal to NULL callback)
 *   or a negative errno value when something bad happened
 */
static int letb8388_sdio_present(struct mmc_host *mmc)
{
	return 1;
}

static void letb8388_enable_sdio_irq(struct mmc_host *host, int enable)
{
	struct sphe8388_sdio_priv_host *sdio_host = mmc_priv(host);
	unsigned long flag;
	spin_lock_irqsave(&sdio_host->interrupt.sdio_int.lock,flag);
	if(enable){	
		enable_sdio_int();
		sdio_host->interrupt.sdio_int.state_sdio_int = SDIO_STATE_ENABLE;	
	}else{	
		disable_sdio_int();
		sdio_host->interrupt.sdio_int.state_sdio_int = SDIO_STATE_DISABLE;
	}
	spin_unlock_irqrestore(&sdio_host->interrupt.sdio_int.lock,flag);
}

const struct mmc_host_ops letb8388_sdio_ops = {
	.request = sphe8388_sdio_request,
	.set_ios = letb8388_sdio_set_ios,
	.get_ro = letb8388_sdio_get_ro,
	.get_cd = letb8388_sdio_present,
	.enable_sdio_irq = letb8388_enable_sdio_irq,
};
#if 0
static void  mmc0_reset_host(struct mmc_host * mmc)
{
#if 0
	DMA_HW_STOP_RST_SET(7);
	udelay(100);
	HWREG_W(reset[0], HWREG_R(reset[0]) | 1<<26);
	udelay(1000);
	HWREG_W(reset[0], HWREG_R(reset[0]) & (~(1<<26)));
#endif

}
#endif

int sdc0_scan_dev(struct device *dev, void *data)
{
	char *name = data;
	if (strcmp(dev_name(dev), name) == 0)
		return (int) dev;
	else
		return 0;
}

static struct mmc_host *device_get_host(struct device *dev)
{
	struct platform_device *pdev;
	struct mmc_host *mmc = NULL;

	pdev = container_of(dev, struct platform_device, dev);
	mmc = platform_get_drvdata(pdev);

	return mmc;
}



static s32 letb8388_init_pri_host_data(struct platform_device *pdev,struct mmc_host *mmc)
{
	struct sphe8388_sdio_priv_host *host = NULL;
	int ret = 0;

	host = mmc_priv(mmc);
	sphe8388_sdio_host = host;
	host->mmc = mmc;
	host->pdev = pdev;

	// init dma_int info
	//init_waitqueue_head(&host->interrupt.dma_int.wq_irq_Sd_dma);
	//host->interrupt.dma_int.irq_dma_completed = SP_SD_COMPLETE_INIT;
	//spin_lock_init(&host->interrupt.dma_int.lock_Sd_irq_dma);

	// init sd_cmp int info
	init_waitqueue_head(&host->interrupt.sd_int.wq_sd_irq);
	host->interrupt.sd_int.irq_sd_stat= 0;

	spin_lock_init(&host->interrupt.lock);
	// init sdio INT info
	host->interrupt.sdio_int.state_sdio_int = SDIO_STATE_DISABLE;
	spin_lock_init(&host->interrupt.sdio_int.lock);
	
	sema_init(&host->sem, 1);
	//sema_init(&host->lock, 1);
#ifdef  	REQEUUEST_USE_SPIN_LOCK
	spin_lock_init(&host->lock);
#endif
	mutex_init(&host->data_lock);
	host->power_mode = 0;
	host->bus_width = 0;
	host->clk = g_sdio_freq;
	/* no resourcc now */
	host->mem = NULL;
	host->base = NULL;
	host->irq = letb8388_IRQ_CARD_CTL0_INT;

	host->clk_dly_info.tune_begin_flag = 1;
	host->clk_dly_info.read_delay = DEF_RD_DLY; 
	host->clk_dly_info.write_delay = DEF_WR_DLY; 	
	host->clk_dly_info.clock_delay = DEF_CLK_DLY; 

	if (request_irq(host->irq, sphe8388_sdio_irq, IRQF_SHARED, 
				letb8388_SDIO_NAME, mmc)) {
		printk("\nFailed to request sdio interrupt.\n");
		ret = -ENOENT;
	}
	host->irq_cd = 0;
	host->dma = 1;
	return ret;
}

#if 0
static int get_mmc0_sense_pin_value(void)	/*sd card*/
{
#if 0
	unsigned int sft_cfg2;
	unsigned long flags;
	int cardexist = 0;

	spin_lock_irqsave(&gpiolock0, flags);
	if (unlikely(IS_ISP_MODE))
	{
		sft_cfg2 = (*((volatile unsigned int *)(RGST_OFFSET+1*32*4+2*4)));
		if (((sft_cfg2>>0)&7) != 0)
		{
			cardexist = 1;
		}
		else
		{
			cardexist = 0;
		}
	}
	else
	{
		if (Card0_sw_remove)
		{
			cardexist = 0;
			gSdcard_status_flag = cardexist;
			spin_unlock_irqrestore(&gpiolock0, flags);
			return cardexist;
		}

		sft_cfg2 = (*((volatile unsigned int *)(RGST_OFFSET+1*32*4+2*4)));
		if (((sft_cfg2>>0)&7) != 0)
		{
			//			GPIO_F_SET(SD_CardInsert_GPIO, 1); //set to GPIO
			//			GPIO_M_SET(SD_CardInsert_GPIO, 1); //control by CPU
			//			GPIO_E_SET(SD_CardInsert_GPIO, 0); //GPIO is input mode
			//			if (!GPIO_I_GET(SD_CardInsert_GPIO) == 1)
			//printk("-------sft_cfg2 = 0x%0x---------\n", sft_cfg2);
			if (!gpio_in_unlocked(SD_CardInsert_GPIO) == 1)
			{
				cardexist = 1;
			}
			else
			{
				cardexist = 0;
			}
		}
	}

	if(gSdcard_status_flag != cardexist)
		IFPRINTK("card 0 %s exist\n", cardexist?"":"not");

	gSdcard_status_flag = cardexist;
	spin_unlock_irqrestore(&gpiolock0, flags);
	//printk("--------cardexist=%d--------\n", cardexist);
	return cardexist;
#endif
	return 1;
}
#endif
#if 0
static void sdc_Card0_insert(void *data)
{
#if 0 
	struct mmc_host *mmc = data;

	if(mmc->card_status == CARD_STATUS_DETECTED)	//prevent from jitter
		return;

	mmc->card_status = CARD_STATUS_DETECTED;
	//500-->50ms, you should not modified, it affect the initialization of the cards, if you modify it
	// test it on different ports of different boards with different cards to make sure all the cards can be read
	mmc_detect_change(mmc, msecs_to_jiffies(50));
#endif
}
#endif
#if 0
static void sdc_Card0_remove(void *data)
{
#if 0
	struct mmc_host *mmc = data;
	//100-->10ms, you should not modified, it affect the initialization of the cards, if you modify it
	// test it on different ports of different boards with different cards to make sure all the cards can be read
	mmc_detect_change(mmc, 0);
#endif
}

#endif
static void letb8388_init_mmc_host_data(struct platform_device *pdev, struct mmc_host *mmc)
{
	mmc->ops = &letb8388_sdio_ops;
	mmc->f_min = CLOCK_400K;
	mmc->f_max =g_sdio_freq;

	mmc->ocr_avail = MMC_VDD_32_33 | MMC_VDD_33_34;

	mmc->caps = MMC_CAP_4_BIT_DATA;

	mmc->caps |= (MMC_CAP_SD_HIGHSPEED | MMC_CAP_MMC_HIGHSPEED);

	mmc->caps |= MMC_CAP_SDIO_IRQ;

	/*
	 * Maximum number of segments. Worst case is one sector per segment
	 * so this will be 64kB/512.
	 */


	/*
	 * Maximum request size. Also limited by 64KB buffer.
	 */
	mmc->max_req_size = (64 * 1024);

	/*
	 * Maximum segment size. Could be one segment with the maximum number
	 * of bytes.
	 */
	mmc->max_seg_size = mmc->max_req_size;

	/*
	 * Maximum block size. We have 11 bits (= 2047) but have to subtract
	 * space for CRC. So the maximum is 2047 - 4*2 = 2039.
	 */
	mmc->max_blk_size = 2039;

	/*
	 * Maximum block count. There is no real limit so the maximum
	 * request size will be the only restriction.
	 */
	mmc->max_blk_count = mmc->max_req_size;
#if 0
	mmc->card_detect = get_mmc0_sense_pin_value;
	mmc->host_reset = mmc0_reset_host;
	mmc->card_detect_type = SDC_EXIST;
	mmc->card_insert = sdc_Card0_insert;
	mmc->card_remove = sdc_Card0_remove;
#endif	//return 0;
}
static int  letb8388_sdio_probe(struct platform_device *pdev)
{
	struct mmc_host *mmc;
	int ret;

	printk("SdIO  driver probe\n");
	
	mmc = mmc_alloc_host(sizeof(struct sphe8388_sdio_priv_host), &pdev->dev);
	if (!mmc) {
		ret = -ENOMEM;
		goto probe_free_host;
	}
      sp_clk_set(SP_CLK_SDIO,  216000000); 
	g_sdio_freq = sp_clk_get(SP_CLK_SDIO)/SDIO_CLK_DIV;
	hw_sdio_init();
	if((ret = letb8388_init_pri_host_data(pdev, mmc))){		
		goto probe_free_host;
	}
	letb8388_init_mmc_host_data(pdev, mmc);

	dev_set_drvdata(&pdev->dev, mmc);
	mmc_add_host(mmc);

	return 0;

probe_free_host:
	if (mmc)
		mmc_free_host(mmc);

	return 0;
}

/* open or close the printf*/
static ssize_t mmc0_printf_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	printk("%s, %s\n", buf, __func__);
	printk("\nError Info:\t%s", (mmc_printk & MMC_PRINTF_ERROR) ? "ON" : "OFF");
	printk("\nFlow Info:\t%s", (mmc_printk & MMC_PRINTF_FLOW) ? "ON" : "OFF");
	printk("\nDetail Info:\t%s", (mmc_printk & MMC_PRINTF_DETAIL) ? "ON" : "OFF");
	printk("\nIF Info:\t%s", (mmc_printk & MMC_PRINTF_IF) ? "ON" : "OFF");
	printk("\nPK Info:\t%s", (mmc_printk & MMC_PRINTF_PK) ? "ON" : "OFF");
	printk("\nclk_dly:%d, rd_dly:%d, HS:%d, wt_dly:%d\n", 
			CLK_DELAY_GET(),       SD_RD_CLK_DELAY_TIME_GET(), 
			SD_HIGHSPEED_EN_GET(), SD_WT_CLK_DELAY_TIME_GET());

	debug_get_register_statues();
	return 0;
}

static ssize_t mmc0_printf_store(struct device *dev, struct device_attribute *attr,
		const char *buf, size_t count)
{
	//echo 49 >./sys/devices/platform/sphe8388-sdio/mmc0_printf
	//echo 1  >./sys/devices/platform/sphe8388-sdio/mmc0_printf

	u32 value = 0;
	char *token;
	char *pch = (char *)buf;
	char *dlm = " ";

	printk("%s, count:%d\n", buf, count);

	token  = strsep(&pch, dlm);
	if (token == NULL)
		goto exit_store;
	value = (u32)simple_strtoul(token, NULL, 10);
	mmc_printk = value;

	token  = strsep(&pch, dlm);
	if (token == NULL)
		goto exit_store;


exit_store:
	printk("%s, count:%d, value:%d\n", buf, count, value);

	return count;
}

DEVICE_ATTR(sdio_printf, S_IWUSR | S_IRUSR, mmc0_printf_show, mmc0_printf_store );

static ssize_t mmc_maxfreq_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	//usage:
	//cat ./sys/devices/platform/letb123-sdc0/maxfreq

	printk("\nmax frequency:%d\n", g_sdio_freq);
	return 0;
}

static ssize_t mmc_maxfreq_store(struct device *dev, struct device_attribute *attr,
		const char *buf, size_t count)
{
	//usage: there is a blank before > , you can set gv1 gv2 gv3 gv4 to variables you want to tune
	//echo 25000000 >./sys/devices/platform/letb123-sdc0/maxfreq

	u32 value = 0;
	struct mmc_host *mmc;

	char *token;
	char *pch = (char *)buf;
	char *dlm = " ";

	token  = strsep(&pch, dlm);
	if (token == NULL)
		return count;

	value = (u32)simple_strtoul(token, NULL, 10);
	if (value <= 0)	//while parse error(ex: input char not number), the return value is 0
	{
		printk("Invalid frequency:%s\n", buf);
		return count;
	}

	g_sdio_freq = value;
	mmc = device_get_host(dev);
	if(mmc == NULL)
	{
		printk("get host fail\n");
		return count;
	}

	mmc->f_max = g_sdio_freq;
	hw_sd_config_freq(g_sdio_freq);

	printk("max frequency:%d\n", g_sdio_freq);

	return count;
}

DEVICE_ATTR(sdio_max_freq, S_IWUSR | S_IRUSR, mmc_maxfreq_show, mmc_maxfreq_store );


static ssize_t mmc_interrupt_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	int_info_t *int_info = &sphe8388_sdio_host->interrupt;
	printk("sdio int:\n");	
	printk("state:%d\n", int_info->sdio_int.state_sdio_int);
	printk("sd cmp int:\n");
	printk("state:%d\n", int_info->sd_int.irq_sd_stat);
	return 0;	
}

static ssize_t mmc_interrupt_store(struct device *dev, struct device_attribute *attr,
		const char *buf, size_t count)
{
	printk("%s:%d\n", __FUNCTION__, __LINE__);
	return count;
}


DEVICE_ATTR(sdio_interrupt, S_IWUSR | S_IRUSR, mmc_interrupt_show, mmc_interrupt_store );



/*static int __devexit letb8388_sdio_remove(struct platform_device *dev)*/
static int  letb8388_sdio_remove(struct platform_device *dev)
{

	struct mmc_host *mmc = platform_get_drvdata(dev);
	struct sphe8388_sdio_priv_host *host = mmc_priv(mmc);

	FPRINTK("Remove sd card\n");
	platform_set_drvdata(dev, NULL);

	//mmc->card_detect_type = SDC_QUIT;
	mmc_remove_host(mmc);
	free_irq(host->irq, mmc);
	mmc_free_host(mmc);
	return 0;
}

#ifdef CONFIG_PM
static int sphe_drv_sdio_resume(struct platform_device *dev)
{
	struct mmc_host *mmc = (struct mmc_host *)platform_get_drvdata(dev);
	sphe8388_sdio_priv_host *host = (sphe8388_sdio_priv_host *)mmc_priv(mmc);

	printk("Sunplus SD0 driver resume.\n");
	
	down(&host->sem);
	mmc_resume_host(mmc);
	up(&host->sem);
	return 0;
}


static int sphe_drv_sdio_suspend(struct platform_device *dev, pm_message_t state)
{
	struct mmc_host *mmc = (struct mmc_host *)platform_get_drvdata(dev);
	sphe8388_sdio_priv_host *host = (sphe8388_sdio_priv_host *)mmc_priv(mmc);
	printk("Sunplus SD0 driver suspend.\n");
	down(&host->sem);
	mmc_suspend_host(mmc);
	up(&host->sem);
	return 0;
}


#endif

struct platform_driver letb8388_driver_sdio = {
	.probe = letb8388_sdio_probe,
	.remove = letb8388_sdio_remove,
	.shutdown = NULL,
	.suspend = sphe_drv_sdio_suspend,
	.resume = sphe_drv_sdio_resume,
	.driver.name = letb8388_SDIO_NAME,
	.driver.owner = THIS_MODULE,
};

static int proc_sdio_read( char *page, char **start, off_t off,
		int count,  int *eof,     void *data )
{
	printk("%s:%d\n", __FUNCTION__, __LINE__);
	printk(" highspeed_flag = %d\n", sphe8388_sdio_host->highspeed_flag);
	printk(" readDelay = %d\n", sphe8388_sdio_host->clk_dly_info.read_delay);
	printk(" ClockDelay = %d\n", sphe8388_sdio_host->clk_dly_info.clock_delay);
	printk(" WriteDelay = %d\n", sphe8388_sdio_host->clk_dly_info.write_delay);
    
	printk(" sdio int state = %d\n", sphe8388_sdio_host->interrupt.sdio_int.state_sdio_int);
	debug_get_register_statues();
	return 0;
}

static ssize_t proc_sdio_write( struct file *filp, const char __user *buff,
		unsigned long len, void *data )
{
	char *token;
	char *pch = (char *)buff;
	char *dlm = " ";
	u32 value = 0;

	printk("%s, count:%ld\n", buff, len);

	token  = strsep(&pch, dlm);
	if (token == NULL)
		goto exit;
	value = (u32)simple_strtoul(token, NULL, 10);
	sphe8388_sdio_host->clk_dly_info.read_delay = value;

	token  = strsep(&pch, dlm);
	if (token == NULL)
		goto exit;
	value = (u32)simple_strtoul(token, NULL, 10);
	sphe8388_sdio_host->clk_dly_info.write_delay = value;

	token  = strsep(&pch, dlm);
	if (token == NULL)
		goto exit;
	value = (u32)simple_strtoul(token, NULL, 10);
	sphe8388_sdio_host->clk_dly_info.clock_delay = value;

exit:
	tune_rd_dly(sphe8388_sdio_host, sphe8388_sdio_host->clk_dly_info.read_delay, sphe8388_sdio_host->highspeed_flag);
	tune_wr_dly(sphe8388_sdio_host, sphe8388_sdio_host->clk_dly_info.write_delay);
	tune_clk_dly(sphe8388_sdio_host, sphe8388_sdio_host->clk_dly_info.clock_delay);
	//printk("para is not right\n");
	return len;
}


static int proc_sdio_printk_read( char *page, char **start, off_t off,
		int count,  int *eof,     void *data )
{
	printk("%s:%d\n", __FUNCTION__, __LINE__);	
	printk("mmc_printk = %d\n", mmc_printk);
	return 0;
}

static ssize_t proc_sdio_printk_write( struct file *filp, const char __user *buff,
		unsigned long len, void *data )
{
	printk("%s:%d\n", __FUNCTION__, __LINE__);	
	mmc_printk = simple_strtoul(buff, NULL, 10);
	return len;
}


static int proc_sdio_send_cmd_read( char *page, char **start, off_t off,
		int count,  int *eof,     void *data )
{
	struct sphe8388_sdio_priv_host *host = sphe8388_sdio_host;
	if(host->interrupt.sdio_int.state_sdio_int == SDIO_STATE_ENABLE)
	{
		printk("sdio card int is enable\n");
	}else{
		printk("sdio card int is disable\n");
	}
	debug_get_register_statues();
	//printk("mmc_printk = %d\n", mmc_printk);
	return 0;
}

//u8 buf[4096] = {0};
static ssize_t proc_sdio_send_cmd_write( struct file *filp, const char __user *buff,
		unsigned long len, void *data )
{

	//u32 i = 0;
	//	u8 buf[20] = {0};
	//	u32 cmd  = simple_strtoul(buff, NULL, 10);
	debug_get_register_statues();
	printk("send cmd\n");
	//hw_sd_send_cmd(cmd, 0, RSP_TYPE_R1, buf,SDIO_CMD_OLY_CMD);
	printk("send cmd end\n");
	debug_get_register_statues();
	return len;
}


static int proc_sdio_send_dummy_read( char *page, char **start, off_t off,
		int count,  int *eof,     void *data )
{
	struct sphe8388_sdio_priv_host *host = sphe8388_sdio_host;
	if(host->interrupt.sdio_int.state_sdio_int == SDIO_STATE_ENABLE)
	{
		printk("sdio card int is enable\n");
	}else{
		printk("sdio card int is disable\n");
	}
	//printk("mmc_printk = %d\n", mmc_printk);
	return 0;
}






//u8 buf[4096] = {0};
static ssize_t proc_sdio_send_dummy_write( struct file *filp, const char __user *buff,
		unsigned long len, void *data )
{

	//u32 i = 0;
	u32 clks  = simple_strtoul(buff, NULL, 10);
	struct sphe8388_sdio_priv_host *host = sphe8388_sdio_host;
	if(host->interrupt.sdio_int.state_sdio_int == SDIO_STATE_ENABLE)
	{
		printk("sdio card int is enable\n");
	}else{
		printk("sdio card int is disable\n");
	}
	tx_dummy(clks);
	return len;
}



static int proc_sdio_signal_towifidriver_rd( char *page, char **start, off_t off,
		int count,  int *eof,     void *data )
{
	struct sphe8388_sdio_priv_host *host = sphe8388_sdio_host;
	if(host->interrupt.sdio_int.state_sdio_int == SDIO_STATE_ENABLE)
	{
		printk("sdio card int is enable\n");
	}else{
		printk("sdio card int is disable\n");
	}
	//printk("mmc_printk = %d\n", mmc_printk);
	return 0;
}


static ssize_t proc_sdio_signal_towifidriver_wr( struct file *filp, const char __user *buff,
		unsigned long len, void *data )
{
	struct sphe8388_sdio_priv_host *host = sphe8388_sdio_host;
	mmc_signal_sdio_irq(host->mmc);
	printk("mmc_signal_sdio_irq done\n");
	return len;
}


void set_drive_strength(int drv_strength)
{
	//sdc0 at 4.11
#define DRIVE_STRENGTH_REG (RGST_OFFSET+4*32*4+11*4)
	u32 value = 0;

	value = *((volatile unsigned int *)(DRIVE_STRENGTH_REG));
	// clear R_MX5_DS -- 5 6 7 8 9 10 11
	value &= ~(0xfff<<10);
	//value |= (0x555 <<10);
	value |= ((drv_strength<<10) | (drv_strength<<12) | (drv_strength << 14) | (drv_strength << 16) \
			| (drv_strength<<18) | (drv_strength<<20) | (drv_strength<<22));
	*((volatile unsigned int *)(DRIVE_STRENGTH_REG)) = value;
}


static u32 sdio_create_sys_debug_file(void)
{
	u32 ret = 0;
	ret = device_create_file(&(letb8388_device_sdio->dev), &dev_attr_sdio_printf);
	ASSERT_GOTO(ret, mmc0_printf_err, "create dev_attr_mmc0_printf file fail.\n");

	ret = device_create_file(&(letb8388_device_sdio->dev), &dev_attr_sdio_max_freq);
	ASSERT_GOTO(ret, maxFreq_err, "create dev_attr_maxfreq  file fail.\n");

	ret = device_create_file(&(letb8388_device_sdio->dev), &dev_attr_sdio_interrupt);
	ASSERT_GOTO(ret,interrupt_err, "create interrupt file fail.\n");

	return ret;
interrupt_err:
	device_remove_file(&(letb8388_device_sdio->dev), &dev_attr_sdio_max_freq);
maxFreq_err:
	device_remove_file(&(letb8388_device_sdio->dev), &dev_attr_sdio_printf);
mmc0_printf_err:
	return -ENODEV;
}

static void sdio_remove_sys_debug_file(void)
{
	device_remove_file(&(letb8388_device_sdio->dev), &dev_attr_sdio_interrupt);
	device_remove_file(&(letb8388_device_sdio->dev), &dev_attr_sdio_max_freq);
	device_remove_file(&(letb8388_device_sdio->dev), &dev_attr_sdio_printf);
}




static u32 sdio_create_proc_debug_file(void)
{

	struct proc_dir_entry *proc_entry_Sdcard0;

	proc_entry_Sdcard0 = create_proc_entry( "proc_sdio", 0644, NULL );
	ASSERT_GOTO(!proc_entry_Sdcard0, err_proc_sdio, "create proc entry \"proc_sdio\" fail\n");
	proc_entry_Sdcard0->read_proc  = proc_sdio_read;
	proc_entry_Sdcard0->write_proc = proc_sdio_write;

	proc_entry_Sdcard0 = create_proc_entry( "proc_mmc_printk", 0644, NULL );
	ASSERT_GOTO(!proc_entry_Sdcard0, err_mmc_printk, "create proc entry \"proc_mmc_printk\" fail\n");
	proc_entry_Sdcard0->read_proc  = proc_sdio_printk_read;
	proc_entry_Sdcard0->write_proc = proc_sdio_printk_write;

	proc_entry_Sdcard0 = create_proc_entry( "proc_send_dummy", 0644, NULL );
	ASSERT_GOTO(!proc_entry_Sdcard0, err_send_dummy, "create proc entry \"proc_send_dummy\" fail\n");
	proc_entry_Sdcard0->read_proc  = proc_sdio_send_dummy_read;
	proc_entry_Sdcard0->write_proc = proc_sdio_send_dummy_write;

	proc_entry_Sdcard0 = create_proc_entry( "proc_send_cmd", 0644, NULL );
	ASSERT_GOTO(!proc_entry_Sdcard0, err_send_cmd, "create proc entry \"proc_send_cmd\" fail\n");
	proc_entry_Sdcard0->read_proc  = proc_sdio_send_cmd_read;
	proc_entry_Sdcard0->write_proc = proc_sdio_send_cmd_write;

	proc_entry_Sdcard0 = create_proc_entry( "proc_signal_towifi", 0644, NULL );
	ASSERT_GOTO(!proc_entry_Sdcard0, err_signal_towifi, "create proc entry \"proc_signal_towifi\" fail\n");
	proc_entry_Sdcard0->read_proc  = proc_sdio_signal_towifidriver_rd;
	proc_entry_Sdcard0->write_proc = proc_sdio_signal_towifidriver_wr;

	return 0;	
err_signal_towifi:
	remove_proc_entry("proc_send_cmd", NULL);
err_send_cmd:	
	remove_proc_entry("proc_send_dummy", NULL);	
err_send_dummy:
	remove_proc_entry("proc_mmc_printk", NULL);
err_mmc_printk:
	remove_proc_entry("proc_sdio", NULL);
err_proc_sdio:
	return -ENOMEM;
}

static void sdio_remove_proc_debug_file(void)
{
	remove_proc_entry("proc_sdio", NULL);
	remove_proc_entry("proc_mmc_printk", NULL);
	remove_proc_entry("proc_send_dummy", NULL);	
	remove_proc_entry("proc_send_cmd", NULL);
	remove_proc_entry("proc_signal_towifi", NULL);
}

static void test_read_default_reg_val(u32 grp, u32 reg, u32 def)
{
	u32 *addr =(u32 *)IO0_ADDRESS(grp*32*4 + reg*4);
	u32 val = ioread32(addr);
	DPRINTK("\n");
	DPRINTK("g%d.reg%d = 0x%0x\n",grp, reg, val);
}

static void test_write_reg_val(u32 grp, u32 reg, u32 val)
{
	u32 *addr = (u32 *)IO0_ADDRESS(grp*32*4 + reg*4);
	u32 rd = 0;
	DPRINTK("\n");
	iowrite32(val, addr);
	DPRINTK("write:g%d.reg%d = 0x%0x\n",grp, reg, val);
	rd = ioread32(addr);
	DPRINTK("read:g%d.reg%d = 0x%0x\n",grp, reg, rd);
}

static void test_read_write_one_grp(u32 grp, u32 begin, u32 end)
{
	u32 i = 0;
	for(i = begin; i <= end; ++i){
		test_read_default_reg_val(grp, i, 0);
		test_write_reg_val(grp, i, ~0x0);		
		test_write_reg_val(grp, i, 0x0);
	}	
}
s32 test_read_write_register(void)
{

	volatile u32 *g1 = (volatile u32 *)IO0_ADDRESS(1*32*4);
	//bit [10:9] [12:11]  mux set
	u32 val = ioread32(g1 + 4);
	val &= ~(0xf<<9);
	val |=  (0x5<<9);
	iowrite32(val, g1 + 4);
	udelay(1000);
	printk("g1.4=0x%0x\n", ioread32(g1 + 4));

	test_read_write_one_grp(264,  0,  21);
	test_read_write_one_grp(265, 10,  31);
	test_read_write_one_grp(266,  0,  31);
	test_read_write_one_grp(268,  0,  31);

	return 0;
}


static void hw_sdio_init(void)
{
#if 0
	u32 *g0 = (u32 *)IO0_ADDRESS(0*32*4);
	u32 val = 0;
	// g0.19 bit2 = 0  set card reset 0
	val =  ioread32(g0 + 19);
	IFPRINTK("g0.19=0x%0x\n\n", val);
	if(GET_BIT_N(val, 2)){
		CLR_BIT_N(val, 2);		
		iowrite32(val, g0 + 19);
	}
	
	// g0.14 bit2 = 1  open clk gate
	val =  ioread32(g0 + 14);	
	IFPRINTK("g0.14=0x%0x\n\n", val);
	if(!GET_BIT_N(val, 2)){
		SET_BIT_N(val, 2);
		iowrite32(val, g0 + 14);
	}
	
	//G1.4 bit [10:9] [12:11]  mux set
	val = ioread32(g0 + 4 + 32);
	val &= ~(0xF<<9);
	val |=  (0x5<<9);
	iowrite32(val, g0 + 4);
#endif
#if 0
      GPIO_F_SET(48, 1);
      GPIO_M_SET(48, 1);
      GPIO_E_SET(48, 1);
      GPIO_O_SET(48, 0);
      udelay(5);
      GPIO_O_SET(48, 1);

      GPIO_F_SET(47, 1);
      GPIO_M_SET(47, 1);
      GPIO_E_SET(47, 1);
      GPIO_O_SET(47, 0);        
      udelay(5);
      GPIO_O_SET(47, 1);
#endif

	//set default value
	SD_HIGHSPEED_EN_SET(0); //highspeed disable	
	SD_RD_CLK_DELAY_TIME_SET(2);
	SD_WR_CLK_DELAY_TIME_SET(1);
	DMA_SIZE_SET((512 - 1) & 0x3ff); //setup dma buffer size
	/*sd hardware initalize*/
	SD_CONFIG_SET(0x7000);
	SD_CTRL_SET(0);
	SD_DIV_2_DIS();
	SD_UTRL_HSPEED_DIS();	
	hw_sd_config(SD_CLK_DIV_FACTOR, SD_1BIT_MODE, SDIO_MODE);	
	SD_BLOCKSIZE_SET(512);
	SD_AUTODMA_TYPE_SET(DMA_TYPE_SINGLEBLOCK_CMD);
	SD_HW_CMD13_EN(0);
	SD_HCSD_EN_SET(0);
	reset_dut_sdio();
	disable_sdio_int();
	disable_sd_cmp_int();
}

static int   sphe8388_sdio_init(void)
{
	int ret;
	u032 mux1=0, mux2=0;
	if((ret = gpio_pin_mux_get(PMX_SDIO_CTL_SD, &mux1))){
		printk("get sdio pinmux  PMX_SDIO_CTL_SD error \n");
		return ret;
	}
	if((ret= gpio_pin_mux_get(PMX_SDIO_CTL_HB, &mux2))){		
		printk("get sdio pinmux  PMX_SDIO_CTL_HB error \n");
		return ret;
	}
	if (!((1 == mux1) && (1 == mux2))) {
		printk("sdio pinmux is not select, do not start sdio driver\n");
		return 0; //sdio pinmux not select 
	}
	ret = platform_driver_register(&letb8388_driver_sdio);
	ASSERT_GOTO(ret, drv_register_err,  "Register sdio driver fail\n");

	letb8388_device_sdio = platform_device_alloc(letb8388_SDIO_NAME, -1);
	if (!letb8388_device_sdio) {	/*-1means device has only one*/
		ret = -ENOMEM;	
	}
	ASSERT_GOTO(ret, device_alloc_err,"Alloc sdio device fail\n");

	ret = platform_device_add(letb8388_device_sdio);
	ASSERT_GOTO(ret, device_add_err,  "Add sdio device fail\n");		

	// create sys debug file
	ret = sdio_create_sys_debug_file();
	ASSERT_GOTO(ret, create_sys_file_err, "sdio_create_sys_debug_file fail\n");

	ret = sdio_create_proc_debug_file();
	ASSERT_GOTO(ret, create_proc_file_err, "create_proc_debug_file fail\n");

	return ret;

create_proc_file_err:
	sdio_remove_sys_debug_file();

create_sys_file_err:
	platform_device_del(letb8388_device_sdio);

device_add_err:	
	platform_device_put(letb8388_device_sdio);

device_alloc_err:	
	platform_driver_unregister(&letb8388_driver_sdio);

drv_register_err:
	return ret;
}


void __exit sphe8388_sdio_exit(void)
{
	u32 ret;
	u032 mux1=0, mux2=0;	
	printk(KERN_INFO "Exit letb8388 SDIO Driver\n");
	if((ret = gpio_pin_mux_get(PMX_SDIO_CTL_SD, &mux1))){
		return ;
	}
	if((ret= gpio_pin_mux_get(PMX_SDIO_CTL_HB, &mux2))){		
		return ;
	}
	if (!((1 == mux1) && (1 == mux2))) {
		return ; // sdio pinmux not select 
	}
#if 1
	sdio_remove_sys_debug_file();
	platform_device_unregister(letb8388_device_sdio);
	platform_driver_unregister(&letb8388_driver_sdio);
	sdio_remove_proc_debug_file();
#endif
}

//__initcall(sphe8388_sdio_init);

module_init(sphe8388_sdio_init);
module_exit(sphe8388_sdio_exit);


module_param(g_sdio_pio_dma_mode, uint, S_IRUGO);
module_param(g_sdio_freq, uint, S_IRUGO);

MODULE_AUTHOR("letb8388 jz.xiang");
MODULE_DESCRIPTION("letb8388 SDIO Interface Driver");
MODULE_LICENSE("GPL");
