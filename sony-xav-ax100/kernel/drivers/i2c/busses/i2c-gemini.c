
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/i2c.h>

#include <mach/hal/hal_i2c.h>
#include <mach/common.h>
#include <mach/sp_i2c.h>

struct gemini_if {
//	u8 __iomem		*reg;			/* memory mapped registers */
	int				irq;
	spinlock_t		lock;
	struct i2c_msg	*msgs;			/* messages currently handled */
//	int				msgs_num;		/* nb of msgs to do */
//	int				msgs_push;		/* nb of msgs read/written */
//	int				msgs_done;		/* nb of msgs finally handled */
//	unsigned			push;			/* nb of bytes read/written in msg */
//	unsigned			done;			/* nb of bytes finally handled */
	int				timeout_count;	/* timeout retries left */
	struct timer_list	timeout_timer;
	struct i2c_adapter	adap;
	struct completion	complete;
	struct clk		*clk;
	struct resource	*res;
	I2C_CMD_INFO hd;
	int (*sp_i2cm_read) (I2C_CMD_INFO cmd_info);
	int (*sp_i2cm_write) (I2C_CMD_INFO cmd_info);
};

#define BUS_COUNT 2
static struct gemini_if gemini_if[BUS_COUNT]={
	{.sp_i2cm_read = sp_i2cm0_read,.sp_i2cm_write = sp_i2cm0_write},
	{.sp_i2cm_read = sp_i2cm1_read,.sp_i2cm_write = sp_i2cm1_write},
};

int m0_enable = 0, m1_enable = 0;


#if 0
static int gemini_master_xfer(struct i2c_adapter *adap,
				struct i2c_msg *msgs, int num)
{
	int ret, i;
	struct gemini_if *iface = adap->algo_data;
	I2C_CMD_INFO *phd = &(iface->hd);

	if(num==0)
		return 0;

	for(i=0; i<num; i++) {
		if(msgs[i].flags & I2C_M_TEN)
			return 0;

		phd->i2cDev.slaveAddr = msgs[i].addr;
		phd->i2cDev.freq = 100;
		phd->data = msgs[i].buf;
		phd->i2cDev.subAddrLength = 0;
		phd->subAddr = 0;
		phd->dataCnt = msgs[i].len;
		phd->i2cDev.readMode = 0;

		if(msgs[i].flags & I2C_M_RD)
			ret = iface->sp_i2cm_read(*phd);
		else
			ret = iface->sp_i2cm_write(*phd);
		if(ret!=SP_OK)
			return -EIO;
	}

	return num;
}
#else
static int gemini_master_xfer(struct i2c_adapter *adap,
                                struct i2c_msg *msgs, int num)
{
	int ret, i;
	struct gemini_if *iface = adap->algo_data;
	I2C_CMD_INFO *phd = &(iface->hd);

	//printk("[Leo I2C_adapter] gemini_master_xfer \n");
	if(num==0)
		return -EINVAL;

	phd->i2cDev.readMode = 0;
	phd->i2cDev.freq = 100;

	for(i=0; i<num; i++) {
		if(msgs[i].flags & I2C_M_TEN)
			return -EINVAL;

		if(msgs[i].flags & I2C_M_NOSTART){
			int j;
			for(j = 0;j < msgs[i].len; j++)
				phd->subAddr = (phd->subAddr << 8) | (msgs[i].buf[j]);
			phd->i2cDev.subAddrLength = msgs[i].len;
			phd->i2cDev.readMode = 1;
			continue;
		}

		phd->i2cDev.slaveAddr = msgs[i].addr;
		phd->data = msgs[i].buf;
		phd->dataCnt = msgs[i].len;

		if (phd->i2cDev.readMode == 0){
			phd->subAddr = 0;
			phd->i2cDev.subAddrLength = 0;
		}

		if(msgs[i].flags & I2C_M_RD)
			ret = iface->sp_i2cm_read(*phd);
		else
			ret = iface->sp_i2cm_write(*phd);

		if(ret!=SP_OK)
			return -EIO;
	}

	return num;
}
#endif


static u32 gemini_functionality(struct i2c_adapter *adap)
{
	return I2C_FUNC_I2C | I2C_FUNC_SMBUS_EMUL;
}

static struct i2c_algorithm gemini_algorithm = {
	.master_xfer   = gemini_master_xfer,
	.functionality = gemini_functionality,
};


static int gemini_i2c_probe(struct platform_device *pdev)
{
    struct i2c_adapter *p_adap;
	int rc;
	int nr;

	for(nr = 0;nr<BUS_COUNT;nr ++){
		if (nr == 0)
			m0_enable = rc = sp_i2cm0_config();
		else
			m1_enable = rc = sp_i2cm1_config();

		if (rc != 0) continue;


		struct gemini_if *iface = &gemini_if[nr];
		p_adap = &iface->adap;
		sprintf(p_adap->name,"%s%d",pdev->name,nr);
		//strlcpy(p_adap->name, pdev->name, sizeof(p_adap->name));
		p_adap->algo = &gemini_algorithm;
		p_adap->algo_data = iface;
		p_adap->nr = nr;
		p_adap->class = 0;
		p_adap->retries = 5;
		p_adap->dev.parent = &pdev->dev;

		rc = i2c_add_numbered_adapter(p_adap);
		if (rc<0)
			printk("[I2C adapter] error add adapter %s\n",p_adap->name);
		else
			printk("[I2C adapter] add adapter %s success\n",p_adap->name);
	}
	//hal_i2cm1_setpinmux(1);
	//hal_i2cm0_setpinmux(1);

	return 0;
}

static int gemini_i2c_remove(struct platform_device *pdev)
{
	struct gemini_if *iface = platform_get_drvdata(pdev);

	i2c_del_adapter(&iface->adap);
	if(m0_enable)
		sp_i2cm0_remove();
	if(m1_enable)
		sp_i2cm1_remove();
	
	return 0;
}

static int gemini_i2c_suspend(struct platform_device *pdev, pm_message_t state)
{
	if(m0_enable)
		sp_i2cm0_suspend();
	if(m1_enable)
		sp_i2cm1_suspend();

	return 0;
}


static int gemini_i2c_resume(struct platform_device *pdev)
{
	if(m0_enable)
		sp_i2cm0_resume();
	if(m1_enable)
		sp_i2cm1_resume();

	return 0;
}

static struct platform_device gemini_i2c_device = {
    .name   = "gemini-i2c",
    .id = -1,
    .dev    = {
    }
};

static struct platform_driver gemini_i2c_driver = {
	.probe		= gemini_i2c_probe,
	.remove		= gemini_i2c_remove,
	.suspend		= gemini_i2c_suspend,
	.resume		= gemini_i2c_resume,
	.driver		= {
		.owner	= THIS_MODULE,
		.name	= "gemini-i2c",
	},
};

static int __init i2c_adap_gemini_init(void)
{
    //printk("[Leo I2C_adapter],gemini_init~~~ \n" );
    platform_device_register(&gemini_i2c_device);
    return platform_driver_register(&gemini_i2c_driver);
}
//subsys_initcall(i2c_adap_gemini_init);
module_init(i2c_adap_gemini_init);

static void __exit i2c_adap_gemini_exit(void)
{
	platform_driver_unregister(&gemini_i2c_driver);
}
module_exit(i2c_adap_gemini_exit);

MODULE_DESCRIPTION("gemini I2C Bus driver");
MODULE_AUTHOR("Simon Hsu, <Simon@generalplus.com.tw>");
MODULE_LICENSE("GPL");



