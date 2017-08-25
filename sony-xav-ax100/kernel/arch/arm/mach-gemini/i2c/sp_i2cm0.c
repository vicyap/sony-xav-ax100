#include <linux/delay.h>
#include <linux/device.h>
#include <linux/slab.h>
#include <linux/kthread.h>

#include <linux/rtc.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/miscdevice.h>
#include <linux/kernel.h>
#include <mach/sp_i2c.h>
#include <mach/gpio_drv.h>
//#include <mach/hal/regmap/reg_system.h>



/**************************************************************************
 *                              M A C R O S                               *
 **************************************************************************/
#define I2C_M0_USE_INT 1
#define I2C_M0_LOCAL_TEST 0
#define I2C_M0_WRITE_MODE 1 //1:interrupt mode; 0:polling mode
#define I2C_M0_IS_READ16BYTE 1 //0:4 bytes, 1:16 bytes if you change this, remember change other define at hal_i2c.h

/**************************************************************************
 *                           C O N S T A N T S                            *
 **************************************************************************/
#define I2C_TIMEOUT 200

#if (I2C_M0_IS_READ16BYTE)
#define GET_BYTES_EACHTIME   16
#define LOOPTO32_LENGTH      2//16*2=32
#define REMAIN               15
#define SQUARE_VALUE         4//2 square 4 = 16
#define BYTE_CLEAR           0xffff
#else
#define GET_BYTES_EACHTIME   4
#define LOOPTO32_LENGTH      8//4*8=32
#define REMAIN               3
#define SQUARE_VALUE         2//2 square 2 = 4
#define BYTE_CLEAR           0xf
#endif

/**************************************************************************
 *                          D A T A    T Y P E S                          *
 **************************************************************************/

/**************************************************************************
 *               F U N C T I O N    D E C L A R A T I O N S               *
 **************************************************************************/

#if (I2C_M0_LOCAL_TEST)
static int _sp_i2cm0_fops_open(struct inode *inode, struct file *pfile);
static int _sp_i2cm0_fops_release(struct inode *inode, struct file *pfile);
static long _sp_i2cm0_fops_ioctl(struct file *pfile, unsigned int cmd, unsigned long arg);

static int _sp_i2cm0_probe(struct platform_device *pdev);
static int _sp_i2cm0_remove(struct platform_device *pdev);
static int _sp_i2cm0_suspend(struct platform_device *pdev, pm_message_t state);
static int _sp_i2cm0_resume(struct platform_device *pdev);
#endif

#if (I2C_M0_USE_INT)
void _i2cm0_do_tasklet(unsigned long arg);
DECLARE_TASKLET(i2cm0_tasklet, _i2cm0_do_tasklet, 0);
#endif

/**************************************************************************
 *                         G L O B A L    D A T A                         *
 **************************************************************************/

#if (I2C_M0_LOCAL_TEST)
static const struct file_operations sp_i2cm0_fops =
{
	.owner			= THIS_MODULE,
	.open			= _sp_i2cm0_fops_open,
	.release			= _sp_i2cm0_fops_release,
	.unlocked_ioctl	= _sp_i2cm0_fops_ioctl,
};

static struct platform_device sp_i2cm0_device = {
	.name			= "sunplus_i2c_m0",
	.id				= -1,
};

static struct platform_driver sp_i2cm0_driver = {
	.probe		= _sp_i2cm0_probe,
	.remove		= _sp_i2cm0_remove,
	.suspend		= _sp_i2cm0_suspend,
	.resume		= _sp_i2cm0_resume,
	.driver		=
	{
		.name	= "sunplus_i2c_m0",
		.owner	= THIS_MODULE,
	},
};

struct miscdevice gMiscI2cM0;
#endif

#if (I2C_M0_USE_INT)
static  I2C_IRQ_EVENT i2cm0_event;
wait_queue_head_t i2cm0_event_wait;
I2C_COM_STATE i2cm0_com_state;
#endif


static INT32 i2cm0_busy = SP_FALSE;
static INT32 Interrupt_count =0;
/**************************************************************************
 *             F U N C T I O N    I M P L E M E N T A T I O N S           *
 **************************************************************************/

#if (I2C_M0_USE_INT)
void
_i2cm0_do_tasklet(unsigned long arg)
{
	int j,k = 0;
	UINT8  __attribute__ ((aligned (4))) w_data[4] = {0};
	UINT32 i2cm0_int_flag = 0,int0;
	UINT8 __attribute__ ((aligned (4))) r_data[GET_BYTES_EACHTIME] = {0};
	switch(i2cm0_com_state.rw){
		case SP_WRITE_STATE:	//write.
		    if(i2cm0_event.nack) {
		        i2cm0_com_state.ret = SP_FAIL;
		        i2cm0_event.active_done = SP_TRUE;
		        wake_up(&i2cm0_event_wait);
		    }else if(i2cm0_event.active_done) {
                i2cm0_com_state.ret = SP_OK;
                wake_up(&i2cm0_event_wait);
            }
#if I2C_M0_WRITE_MODE
		    else if(i2cm0_com_state.burst_cnt>0) {
                hal_i2cm0_reg_get(5,&i2cm0_int_flag);
                if ((i2cm0_int_flag & 0x00e00000)>0){
                    for (k=0;k<4;k++){
                        for(j=0; j<4; j++){
                            if (i2cm0_com_state.i >= i2cm0_com_state.data_cnt)
                                w_data[j] = 0;
                            else
                                w_data[j] = i2cm0_com_state.data_buf[i2cm0_com_state.i];
                            i2cm0_com_state.i++;
                        }
                        //printk("write 0x%02x%02x%02x%02x!\n",w_data[3],w_data[2],w_data[1],w_data[0]);
                        hal_i2cm0_data0_set((UINT32 *)w_data);
                        i2cm0_com_state.burst_cnt--;
                    }
                    hal_i2cm0_status_clear(I2C_EMPTY_THRESHOLD_FLAG);
                }
                //i2cm0_event.fifo_empty = 0;
                if(i2cm0_com_state.burst_cnt == 0){
                    int0=(I2C_ACTIVE_DONE_FLAG|I2C_ADDRESS_NACK_FLAG);
                    hal_i2cm0_int_en(int0, 0, 0, 1);
                }
            }
#endif
			break;

		case SP_READ_STATE:	//read.
		    if(i2cm0_event.nack) {
		        i2cm0_com_state.ret = SP_FAIL;
		        i2cm0_event.active_done = SP_TRUE;
		        wake_up(&i2cm0_event_wait);
		    } else {
		        while(i2cm0_com_state.burst_cnt>0 && i2cm0_event.rdata_event[i2cm0_com_state.i]) {
		                hal_i2cm0_data_single_get((UINT32 *)(&i2cm0_com_state.data_buf[i2cm0_com_state.k]), i2cm0_com_state.i,GET_BYTES_EACHTIME);
		                hal_i2cm0_rdata_flag_clear(BYTE_CLEAR<<(i2cm0_com_state.i<<SQUARE_VALUE));
		                i2cm0_event.rdata_event[i2cm0_com_state.i] = 0;

		                i2cm0_com_state.burst_cnt--;
		                i2cm0_com_state.i++;
		                i2cm0_com_state.k+=GET_BYTES_EACHTIME;
		                if (i2cm0_com_state.i>=LOOPTO32_LENGTH) //because total Length 32 bytes
		                    i2cm0_com_state.i=0;
		        }

		        if(i2cm0_event.active_done && !(i2cm0_event.roverflow)) {
		            if(i2cm0_com_state.burst_r) {
		                hal_i2cm0_data_single_get((UINT32 *)r_data, i2cm0_com_state.i,GET_BYTES_EACHTIME);

		                for (j=0; j<i2cm0_com_state.burst_r; j++) {
		                    i2cm0_com_state.data_buf[i2cm0_com_state.k+j] = r_data[j];
		                }
		            }
		            i2cm0_com_state.ret = SP_OK;
		            wake_up(&i2cm0_event_wait);
		        }
		    }
		    break;

		default:
			break;
	}
}


static irqreturn_t
_i2cm0_irq_handler(
	int irq,
	void* dev_id
)
{
    UINT32 i2cm0_int_flag = 0, i2cm0_rdata_flag = 0;
	UINT8 i = 0;
	//UINT32 overflow;
	Interrupt_count++;
	hal_i2cm0_status_get(&i2cm0_int_flag);
	hal_i2cm0_status_clear(I2C_ALL_FLAG);
	//hal_i2cm0_roverflow_flag_get(&overflow); //because flag always 0 so remark
	hal_i2cm0_rdata_flag_get(&i2cm0_rdata_flag);

    hal_i2cm0_rdata_flag_clear(I2C_BURST_RDATA_FLAG);


	if (i2cm0_int_flag & I2C_ACTIVE_DONE_FLAG){
	    //printk("ACTIVE_DONE_FLAG!\n");
	    i2cm0_event.active_done = SP_TRUE;
	}

	if (i2cm0_int_flag & I2C_ADDRESS_NACK_FLAG){
	    i2cm0_event.nack = SP_TRUE;
	}

	if (i2cm0_int_flag & I2C_EMPTY_THRESHOLD_FLAG){
	    i2cm0_event.fifo_empty = SP_TRUE;
	}


//	if (overflow){
//	    i2cm0_com_state.ret = SP_FAIL;
//	    i2cm0_event.roverflow = SP_TRUE;
//	    i2cm0_event.active_done = SP_TRUE;
//	    printk("[Leo spi2cm0]_i2cm0_do_tasklet overflow!!\n");
//	    wake_up(&i2cm0_event_wait);
//	    break;
//	}
//	else{
	if (i2cm0_com_state.rw==SP_READ_STATE){
	    for (i=0; i<LOOPTO32_LENGTH; i++){//because get 16 each time
	        if (i2cm0_rdata_flag & (1<<(GET_BYTES_EACHTIME*i+REMAIN))){//because get 16 each time
	            i2cm0_event.rdata_event[i] = 1;
	        }
	    }

	}
	 _i2cm0_do_tasklet(1);
//	} //because overflow always 0 so remark this session


	//tasklet_schedule(&i2cm0_tasklet);

	return IRQ_HANDLED;
}


INT32
sp_i2cm0_config(
	void
)
{
	INT32 ret;
 	UINT32 pmx_sel;
	ret = gpio_pin_mux_get(PMX_I2CM0, &pmx_sel);
	if ((0 == ret) && (0 != pmx_sel)) {
		hal_i2cm0_setpinmux(pmx_sel);//pinmux had been initial at uboot
		hal_i2cm0_enable();
		hal_i2cm0_reset();
#if (I2C_M0_USE_INT)
		init_waitqueue_head(&i2cm0_event_wait);
		if (request_irq(SP_IRQ_I2CMASTER0, _i2cm0_irq_handler, IRQF_DISABLED|IRQF_TRIGGER_HIGH, "I2CM0_IRQ", NULL)){
		    printk("[Leo spi2cm0]request_irq fail!!\n");
		    return SP_FAIL;
		}
		//request_irq(SP_IRQ_I2CMASTER0, _i2cm0_irq_handler, IRQF_DISABLED|IRQF_TRIGGER_RISING, "I2CM0_IRQ", NULL);
		memset(&i2cm0_event, 0, sizeof(i2cm0_event));
#endif
		i2cm0_busy = SP_FALSE;
      		return SP_OK;
	}
	else {
		i2cm0_busy = SP_TRUE;
		hal_i2cm0_disable();
      		return SP_FAIL;
	}
}
EXPORT_SYMBOL(sp_i2cm0_config);


int sp_i2cm0_write (I2C_CMD_INFO cmd_info)
{
	UINT8 w_data[32] = {0};
    UINT32 write_cnt, data_cnt, burst_cnt, tmp;
	UINT8 i, j;
	INT32 ret;
	UINT32 int0, int1,int2;
	if (i2cm0_busy){
		return SP_FAIL;
	}
	Interrupt_count =0;
	i2cm0_busy = SP_TRUE;

	write_cnt = cmd_info.dataCnt + (UINT32)cmd_info.i2cDev.subAddrLength;

	if (write_cnt > 65535){
		i2cm0_busy = 0;
		//printk("[Leo spi2cm0]sp_i2cm0_write write_cnt > 65535 is %d\n",write_cnt);
      		return SP_FAIL;
	}

	tmp = cmd_info.subAddr;
	for(i=0; i<cmd_info.i2cDev.subAddrLength; i++){
		w_data[cmd_info.i2cDev.subAddrLength-1-i] = (UINT8)tmp;
		tmp >>= 8;
	}

	if (write_cnt > 32){
		data_cnt = 32 - cmd_info.i2cDev.subAddrLength;
		burst_cnt = (write_cnt-32)/4;
		if ((write_cnt-32)%4)
			burst_cnt += 1;

	}
	else{
		data_cnt = cmd_info.dataCnt;
		burst_cnt = 0;
	}
	
	for(i=0; i<data_cnt; i++){
		w_data[i+cmd_info.i2cDev.subAddrLength] = cmd_info.data[i];
	}

	int1 = 0;
	int2 =0;
	int0 = (I2C_ACTIVE_DONE_FLAG|I2C_ADDRESS_NACK_FLAG);
#if (I2C_M0_WRITE_MODE)
	if (burst_cnt)
	    int0 = (int0|I2C_EMPTY_THRESHOLD_FLAG|I2C_EMPTY_THRESHOLD_CTL);
#endif

	memset(&i2cm0_event, 0, sizeof(i2cm0_event));
	i2cm0_com_state.i = i;
	i2cm0_com_state.data_buf = cmd_info.data;
	i2cm0_com_state.data_cnt = cmd_info.dataCnt;
	i2cm0_com_state.burst_cnt = burst_cnt;
	i2cm0_com_state.rw = SP_WRITE_STATE;

	hal_i2cm0_reset();
	hal_i2cm0_trans_cnt_set(0, 0);
	hal_i2cm0_active_mode_set(I2C_TRIGGER);
	hal_i2cm0_status_clear(I2C_ALL_FLAG);
	hal_i2cm0_data_set((UINT32 *)w_data);
	hal_i2cm0_trans_cnt_set(write_cnt, 0);
	hal_i2cm0_rw_mode_set(0, 0, 0);
	hal_i2cm0_clock_freq_set(cmd_info.i2cDev.freq);
	hal_i2cm0_slave_add_set(cmd_info.i2cDev.slaveAddr);
	hal_i2cm0_int_en(int0, int1, int2, 1); //register interrupt
	hal_i2cm0_manual_trigger(); //start send data
#if (I2C_M0_WRITE_MODE==0) //this session is polling mode
	while (burst_cnt){
		hal_i2cm0_status_get(&tmp);
		if (tmp&I2C_ADDRESS_NACK_FLAG){
			hal_i2cm0_reset();
			i2cm0_busy = SP_FALSE;
			return SP_FAIL;
		}
		for(j=0; j<4; j++){
			if (i >= cmd_info.dataCnt)
				w_data[j] = 0;
			else
				w_data[j] = cmd_info.data[i];
			i++;
		}
		hal_i2cm0_ring_wait();
		hal_i2cm0_data0_set((UINT32 *)w_data);
		burst_cnt--;
	}
#endif
	ret = wait_event_timeout(i2cm0_event_wait, i2cm0_event.active_done, (I2C_TIMEOUT * HZ) / 20) ;
	if (ret == 0){
	    printk("[Leo spi2cm0]wait_event_timeout!!!!!!!\n");
	    ret = SP_FAIL;
	}
	else
		ret = i2cm0_com_state.ret;
	//printk("Interrupt_count is %d",Interrupt_count);
	i2cm0_com_state.rw = SP_IDLE_STATE;
	hal_i2cm0_reset();
	i2cm0_busy = SP_FALSE;
	return ret;
}
EXPORT_SYMBOL(sp_i2cm0_write);


int sp_i2cm0_read (I2C_CMD_INFO cmd_info)
{
	UINT8 w_data[32] = {0};
	UINT32 write_cnt, read_cnt, burst_cnt = 0, burst_r = 0, tmp,correct_interrupt_cnt=0;;
	UINT8 i, k;
	INT32 ret;
	UINT8 sub_add_en;
	UINT32 int0, int1,int2;
	//printk("[Leo spi2cm0]sp_i2cm0_read\n");
	Interrupt_count =0;
	if (i2cm0_busy){
		return SP_FAIL;
	}
	
	i2cm0_busy = SP_TRUE;

	write_cnt = (UINT32)cmd_info.i2cDev.subAddrLength;
	read_cnt = cmd_info.dataCnt;

	if ((write_cnt > 32)||(read_cnt > 65535)){
		i2cm0_busy = 0;
      		return SP_FAIL;
	}
	
	burst_cnt = read_cnt/GET_BYTES_EACHTIME;
	burst_r = read_cnt%GET_BYTES_EACHTIME;

	correct_interrupt_cnt=burst_cnt+1;

	tmp = cmd_info.subAddr;
	for(i=0; i<cmd_info.i2cDev.subAddrLength; i++){
		w_data[cmd_info.i2cDev.subAddrLength-1-i] = (UINT8)tmp;
		tmp >>= 8;
	}

	if (write_cnt)
		sub_add_en = 1;
	else
		sub_add_en = 0;

	int1 = int2 =0;
	int0 = (I2C_ACTIVE_DONE_FLAG|I2C_ADDRESS_NACK_FLAG);
	if (burst_cnt){
		int1 = I2C_BURST_RDATA_FLAG;
	    int2 = I2C_BURST_RDATA_ALL_FLAG;
	}


	memset(&i2cm0_event, 0, sizeof(i2cm0_event));
	i = 0;
	k = 0;
	i2cm0_com_state.i = i;
	i2cm0_com_state.k = k;
	i2cm0_com_state.data_buf = cmd_info.data;
	i2cm0_com_state.burst_cnt = burst_cnt;
	i2cm0_com_state.burst_r = burst_r;
	i2cm0_com_state.rw = SP_READ_STATE;

	hal_i2cm0_reset();
	hal_i2cm0_trans_cnt_set(0, 0);
	hal_i2cm0_active_mode_set(I2C_TRIGGER);		
	hal_i2cm0_status_clear(I2C_ALL_FLAG);
	hal_i2cm0_clock_freq_set(cmd_info.i2cDev.freq);
	hal_i2cm0_slave_add_set(cmd_info.i2cDev.slaveAddr);
	if (write_cnt)
		hal_i2cm0_data_set((UINT32 *)w_data);
	hal_i2cm0_trans_cnt_set(write_cnt, read_cnt);
	hal_i2cm0_rw_mode_set(1, sub_add_en, (cmd_info.i2cDev.readMode & sub_add_en));
	hal_i2cm0_int_en(int0, int1, int2, 1);
	hal_i2cm0_manual_trigger();

	ret = wait_event_timeout(i2cm0_event_wait, i2cm0_event.active_done, (I2C_TIMEOUT * HZ) / 1000) ;
	if (ret == 0)
		ret = SP_FAIL;
	else
		ret = i2cm0_com_state.ret;

	i2cm0_com_state.rw = SP_IDLE_STATE;
	hal_i2cm0_reset();
	i2cm0_busy = SP_FALSE;

	if (Interrupt_count != correct_interrupt_cnt){ //overflow
	    printk("read Interrupt_count is %d,correct is %d\n",Interrupt_count,correct_interrupt_cnt);
	    ret = SP_FAIL;
	}

	Interrupt_count =0;

	return ret;
}
EXPORT_SYMBOL(sp_i2cm0_read);

#else

INT32
sp_i2cm0_config(
    void
)
{
    INT32 ret;
    UINT32 pmx_sel;
    ret = gpio_pin_mux_get(PMX_I2CM0, &pmx_sel);
    if ((0 == ret) && (0 != pmx_sel)) {
        hal_i2cm0_setpinmux(pmx_sel);
        hal_i2cm0_enable();
        hal_i2cm0_reset();
#if (I2C_M0_USE_INT)
        init_waitqueue_head(&i2cm0_event_wait);
        request_irq(SP_IRQ_I2CMASTER0, _i2cm0_irq_handler, IRQF_DISABLED|IRQF_TRIGGER_HIGH, "I2CM0_IRQ", NULL);
        //request_irq(SP_IRQ_I2CMASTER0, _i2cm0_irq_handler, IRQF_DISABLED|IRQF_TRIGGER_RISING, "I2CM0_IRQ", NULL);
        memset(&i2cm0_event, 0, sizeof(i2cm0_event));
#endif
        i2cm0_busy = SP_FALSE;
            return SP_OK;
    }
    else {
        i2cm0_busy = SP_TRUE;
        hal_i2cm0_disable();
            return SP_FAIL;
    }
}
EXPORT_SYMBOL(sp_i2cm0_config);

int sp_i2cm0_write (I2C_CMD_INFO cmd_info)
{
	UINT8 w_data[32] = {0};
	UINT32 write_cnt, data_cnt, burst_cnt, tmp;
	UINT8 i, j;
	INT32 ret;
	UINT32 device_ack;
	if (i2cm0_busy){
		return SP_FAIL;
	}
	
	i2cm0_busy = 1;

	write_cnt = cmd_info.dataCnt + (UINT32)cmd_info.i2cDev.subAddrLength;

	if (write_cnt > 65535){
		i2cm0_busy = 0;
      		return SP_FAIL;
	}

	tmp = cmd_info.subAddr;
	for(i=0; i<cmd_info.i2cDev.subAddrLength; i++){
		w_data[cmd_info.i2cDev.subAddrLength-1-i] = (UINT8)tmp;
		tmp >>= 8;
	}

	if (write_cnt > 32){
		data_cnt = 32 - cmd_info.i2cDev.subAddrLength;
		burst_cnt = (write_cnt-32)/4;

		if ((write_cnt-32)%4)
			burst_cnt += 1;
	}
	else{
		data_cnt = cmd_info.dataCnt;
		burst_cnt = 0;
	}
	
	for(i=0; i<data_cnt; i++){
		w_data[i+cmd_info.i2cDev.subAddrLength] = cmd_info.data[i];
	}

	hal_i2cm0_reset();
	hal_i2cm0_trans_cnt_set(0, 0);
	hal_i2cm0_active_mode_set(I2C_TRIGGER);
	hal_i2cm0_status_clear(I2C_ALL_FLAG);
	hal_i2cm0_data_set((UINT32 *)w_data);
	hal_i2cm0_trans_cnt_set(write_cnt, 0);
	hal_i2cm0_rw_mode_set(0, 0, 0);
	hal_i2cm0_clock_freq_set(cmd_info.i2cDev.freq);
	hal_i2cm0_slave_add_set(cmd_info.i2cDev.slaveAddr);
	
	hal_i2cm0_manual_trigger();
	
	while (burst_cnt){
		hal_i2cm0_status_get(&device_ack);
		if (device_ack&I2C_ADDRESS_NACK_FLAG){
			hal_i2cm0_reset();

			i2cm0_busy = 0;
			return SP_FAIL;
		}
		for(j=0; j<4; j++){
			if (i >= cmd_info.dataCnt)
				w_data[j] = 0;
			else
				w_data[j] = cmd_info.data[i];
			i++;
		}
		hal_i2cm0_ring_wait();
		hal_i2cm0_data0_set((UINT32 *)w_data);
		burst_cnt--;
	}
	ret = hal_i2cm0_complete_wait();
	if (ret == 0)
		ret = hal_i2cm0_address_ack_wait();

	hal_i2cm0_reset();

	i2cm0_busy = 0;
	return ret;
}
EXPORT_SYMBOL(sp_i2cm0_write);


int sp_i2cm0_read (I2C_CMD_INFO cmd_info)
{
	UINT8 w_data[32] = {0};
	UINT8 __attribute__ ((aligned (4))) r_data[4] = {0};
	UINT32 write_cnt, read_cnt, burst_cnt = 0, burst_r = 0, tmp;
	UINT8 i, j, k;
	INT32 ret;
	UINT8 sub_add_en;
	UINT32 device_ack;

	if (i2cm0_busy){
		return SP_FAIL;
	}
	
	i2cm0_busy = 1;

	write_cnt = (UINT32)cmd_info.i2cDev.subAddrLength;
	read_cnt = cmd_info.dataCnt;

	if ((write_cnt > 32)||(read_cnt > 65535)){
		i2cm0_busy = 0;
      		return -1;
	}
	
	burst_cnt = read_cnt/4;
	burst_r = read_cnt%4;

	tmp = cmd_info.subAddr;
	for(i=0; i<cmd_info.i2cDev.subAddrLength; i++){
		w_data[cmd_info.i2cDev.subAddrLength-1-i] = (UINT8)tmp;
		tmp >>= 8;
	}

	if (write_cnt)
		sub_add_en = 1;
	else
		sub_add_en = 0;

	hal_i2cm0_reset();
	hal_i2cm0_trans_cnt_set(0, 0);
	hal_i2cm0_active_mode_set(I2C_TRIGGER);		
	hal_i2cm0_status_clear(I2C_ALL_FLAG);
	hal_i2cm0_clock_freq_set(cmd_info.i2cDev.freq);
	hal_i2cm0_slave_add_set(cmd_info.i2cDev.slaveAddr);
	if (write_cnt)
		hal_i2cm0_data_set((UINT32 *)w_data);
	hal_i2cm0_trans_cnt_set(write_cnt, read_cnt);
	hal_i2cm0_rw_mode_set(1, sub_add_en, (cmd_info.i2cDev.readMode & sub_add_en));
	hal_i2cm0_manual_trigger();

	i = 0;
	k = 0;
	while (burst_cnt){
		if (hal_i2cm0_rdara_ready_wait((i<<2)+3)){
			hal_i2cm0_reset();
			i2cm0_busy = 0;
			return SP_FAIL;
		}
		hal_i2cm0_data_single_get((UINT32 *)r_data, i,4);

		for (j=0; j<4; j++){
			cmd_info.data[k+j] = r_data[j] ;
		}

		burst_cnt--;
		i++;
		k+=4;
		if (i>=8)
			i=0;

		hal_i2cm0_status_get(&device_ack);
		if (device_ack&I2C_ADDRESS_NACK_FLAG){
			hal_i2cm0_reset();
			i2cm0_busy = 0;
			return SP_FAIL;
		}

	}

	ret = hal_i2cm0_complete_wait();
	if (ret == 0)
		ret = hal_i2cm0_address_ack_wait();

	if (burst_r){
		hal_i2cm0_data_single_get((UINT32 *)r_data, i,4);
		for (j=0; j<burst_r; j++){
			cmd_info.data[k+j] = r_data[j] ;
		}
	}

	hal_i2cm0_reset();

	i2cm0_busy = 0;
	return ret;
}
EXPORT_SYMBOL(sp_i2cm0_read);
#endif


void sp_i2cm0_suspend(void)
{
	INFO("Sunplus I2C driver suspend\n");
	hal_i2cm0_disable();
}
EXPORT_SYMBOL(sp_i2cm0_suspend);


void sp_i2cm0_resume(void)
{
	INFO("Sunplus I2C driver resume\n");
	hal_i2cm0_enable();
}
EXPORT_SYMBOL(sp_i2cm0_resume);


void sp_i2cm0_remove(void)
{
	INFO("Sunplus I2C driver remove\n");

	hal_i2cm0_disable();

#if (I2C_M0_USE_INT)
	free_irq(SP_IRQ_I2CMASTER0, NULL);
#endif
}
EXPORT_SYMBOL(sp_i2cm0_remove);


#if (I2C_M0_LOCAL_TEST)
static int _sp_i2cm0_fops_open(struct inode *inode, struct file *pfile)
{
	int minor = iminor(inode);

	INFO("Sunplus I2C module open\n");

	if(minor == gMiscI2cM0.minor)
	{
		return 0;
	}
	else
	{
		ERR("invalid inode\n");
		pfile->private_data = NULL;
		return -1;
	}
	return 0;
}


static int _sp_i2cm0_fops_release(struct inode *inode, struct file *pfile)
{
	INFO("Sunplus I2C module release\n");
	return 0;
}


static long _sp_i2cm0_fops_ioctl(struct file *pfile, unsigned int cmd, unsigned long arg)
{
	int ret = 0;
	I2C_CMD_INFO cmd_data;

	switch (cmd)
	{
		case I2C_CMD_WRITE:
			if (copy_from_user((void *)&cmd_data, (const void __user *)arg, sizeof(I2C_CMD_INFO))){
				return -1;
			}

			ret = sp_i2cm0_write(cmd_data);
			break;

		case I2C_CMD_READ:
			if (copy_from_user((void *)&cmd_data, (const void __user *)arg, sizeof(I2C_CMD_INFO)))
				return -1;

			ret = sp_i2cm0_read(cmd_data);
			if(ret == 0)
			{
				if (copy_to_user((void __user *)arg, (const void *) &cmd_data, sizeof(I2C_CMD_INFO)))
					return -1;
			}
			break;

		default:
			WARNING("Unknow command\n");
			break;
	}

	return ret;	
}


static int _sp_i2cm0_probe(struct platform_device *pdev)
{
	INT32 ret;
 	UINT32 pmx_sel;

	INFO("Sunplus I2C driver probe\n");
    ret = gpio_pin_mux_get(PMX_I2CM0, &pmx_sel);
	printk("i2cm0_pinmux = %d\n", pmx_sel);
	if ((0 == ret) && (0 != pmx_sel)) {
		hal_i2cm0_setpinmux(pmx_sel);
	}
	// Registering misc device
	gMiscI2cM0.minor	= MISC_DYNAMIC_MINOR;
	gMiscI2cM0.name	= "sunplus_i2c_m0";
	gMiscI2cM0.fops	= &sp_i2cm0_fops;

	ret = misc_register(&gMiscI2cM0);
	if (ret) {
		ERR("misc_register failed, ret = %d\n", ret);
		return -1;
	}
	hal_i2cm0_enable();
	hal_i2cm0_reset();

#if (I2C_M0_USE_INT)
	init_waitqueue_head(&i2cm0_event_wait);
	request_irq(SP_IRQ_I2CMASTER0, _i2cm0_irq_handler, IRQF_DISABLED|IRQF_TRIGGER_HIGH, "I2CM0_IRQ", NULL);
	memset(&i2cm0_event, 0, sizeof(i2cm0_event));
#endif

	i2cm0_busy = 0;

	return 0;
}


static int _sp_i2cm0_remove(struct platform_device *pdev)
{
	INFO("Sunplus I2C driver remove\n");

	hal_i2cm0_disable();

#if (I2C_M0_USE_INT)
	free_irq(SP_IRQ_I2CMASTER0, NULL);
#endif

	misc_deregister(&gMiscI2cM0);

	return 0;
}


static int _sp_i2cm0_suspend(struct platform_device *pdev, pm_message_t state)
{
	INFO("Sunplus I2C driver suspend\n");
	hal_i2cm0_disable();

	return 0;
}


static int _sp_i2cm0_resume(struct platform_device *pdev)
{
	INFO("Sunplus I2C driver resume\n");
	hal_i2cm0_enable();

	return 0;
}


static int __init _sp_i2cm0_init(void)
{
	int ret = -1;

	INFO("Sunplus I2C module init\n");

	ret = platform_device_register(&sp_i2cm0_device);
	if (ret)
	{
		ERR("i2c device register failed, ret = %d\n", ret);
		goto device_register_fail;
	}

	ret = platform_driver_register(&sp_i2cm0_driver);
	if (ret < 0)
	{
		ERR("i2c driver register failed, ret = %d\n", ret);
		goto driver_register_fail;
	}

	INFO("Sunplus I2C module init success\n");

	return 0;

driver_register_fail:
	platform_device_unregister(&sp_i2cm0_device);
device_register_fail:

	return ret;
}


static void __exit _sp_i2cm0_exit(void)
{
	INFO("Sunplus I2C module exit\n");

	platform_driver_unregister(&sp_i2cm0_driver);
	platform_device_unregister(&sp_i2cm0_device);

	return;
}

module_init(_sp_i2cm0_init);
module_exit(_sp_i2cm0_exit);
#endif

/**************************************************************************
 *                  M O D U L E    D E C L A R A T I O N                      *
 **************************************************************************/
 
MODULE_AUTHOR("Sunplus");
MODULE_DESCRIPTION("Sunplus I2C Master 0 Driver");
MODULE_LICENSE("GPL");
MODULE_VERSION("1.0");




