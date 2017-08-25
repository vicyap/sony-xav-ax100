#include <linux/delay.h>
#include <linux/device.h>
#include <linux/slab.h>
#include <linux/kthread.h>

#include <linux/rtc.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/miscdevice.h>
#include <linux/kernel.h>
#include <mach/sp_i2cddc.h>
#include <mach/gpio_drv.h>
//#include <mach/hal/regmap/reg_system.h>

/**************************************************************************
 *                           C O N S T A N T S                            *
 **************************************************************************/

/**************************************************************************
 *                              M A C R O S                               *
 **************************************************************************/
#define I2C_DDC_DEBUG 0

/**************************************************************************
 *                          D A T A    T Y P E S                          *
 **************************************************************************/

/**************************************************************************
 *               F U N C T I O N    D E C L A R A T I O N S               *
 **************************************************************************/

static int _sp_i2cddc0_fops_open(struct inode *inode, struct file *pfile);
static int _sp_i2cddc0_fops_release(struct inode *inode, struct file *pfile);
static long _sp_i2cddc0_fops_ioctl(struct file *pfile, unsigned int cmd, unsigned long arg);

static int _sp_i2cddc0_probe(struct platform_device *pdev);
static int _sp_i2cddc0_remove(struct platform_device *pdev);
static int _sp_i2cddc0_suspend(struct platform_device *pdev, pm_message_t state);
static int _sp_i2cddc0_resume(struct platform_device *pdev);


/**************************************************************************
 *                         G L O B A L    D A T A                         *
 **************************************************************************/

static const struct file_operations sp_i2cddc0_fops =
{
	.owner			= THIS_MODULE,
	.open			= _sp_i2cddc0_fops_open,
	.release			= _sp_i2cddc0_fops_release,
	.unlocked_ioctl	= _sp_i2cddc0_fops_ioctl,
};

static struct platform_device sp_i2cddc0_device = {
	.name			= "sp_i2c_ddc0",
	.id				= -1,
};

static struct platform_driver sp_i2cddc0_driver = {
	.probe		= _sp_i2cddc0_probe,
	.remove		= _sp_i2cddc0_remove,
	.suspend		= _sp_i2cddc0_suspend,
	.resume		= _sp_i2cddc0_resume,
	.driver		=
	{
		.name	= "sp_i2c_ddc0",
		.owner	= THIS_MODULE,
	},
};

struct miscdevice gMisci2cddc0;


static INT32 i2cddc0_busy = 0;

/**************************************************************************
 *             F U N C T I O N    I M P L E M E N T A T I O N S           *
 **************************************************************************/

static int _sp_i2cddc0_fops_open(struct inode *inode, struct file *pfile)
{
	int minor = iminor(inode);

	INFO("I2C DDC0 module open\n");

	if(minor == gMisci2cddc0.minor)
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


static int _sp_i2cddc0_fops_release(struct inode *inode, struct file *pfile)
{
	INFO("I2C DDC0 module release\n");
	return 0;
}


static long _sp_i2cddc0_fops_ioctl(struct file *pfile, unsigned int cmd, unsigned long arg)
{
	int ret = 0;
	I2C_DDC_SRAM cmd_data;

	switch (cmd)
	{
		case I2C_DDC_SRAM_WRITE:
			if (copy_from_user((void *)&cmd_data, (const void __user *)arg, sizeof(I2C_DDC_SRAM))){
				return -1;
			}

			hal_i2cddc0_sram_write(cmd_data.addr, *(cmd_data.data));
			break;

		case I2C_DDC_SRAM_READ:
			if (copy_from_user((void *)&cmd_data, (const void __user *)arg, sizeof(I2C_DDC_SRAM)))
				return -1;

			hal_i2cddc0_sram_read(cmd_data.addr, cmd_data.data);

			if (copy_to_user((void __user *)arg, (const void *) &cmd_data, sizeof(I2C_DDC_SRAM)))
				return -1;
			break;

		default:
			WARNING("Unknow command\n");
			break;
	}

	return ret;	
}


static int _sp_i2cddc0_probe(struct platform_device *pdev)
{
	INT32 ret;
 	UINT32 pmx_sel;

	INFO("I2C DDC0 driver probe\n");

	gMisci2cddc0.minor	= MISC_DYNAMIC_MINOR;
	gMisci2cddc0.name	= "sp_i2c_ddc0";
	gMisci2cddc0.fops		= &sp_i2cddc0_fops;

	ret = misc_register(&gMisci2cddc0);
	if (ret) {
		ERR("misc_register failed, ret = %d\n", ret);
		return -1;
	}

	hal_i2cddc0_init();
	//hal_i2cddc0_reset();

	i2cddc0_busy = 0;

	return 0;
}


static int _sp_i2cddc0_remove(struct platform_device *pdev)
{
	INFO("I2C DDC0 driver remove\n");

	//hal_i2cddc0_disable();

	misc_deregister(&gMisci2cddc0);

	return 0;
}


static int _sp_i2cddc0_suspend(struct platform_device *pdev, pm_message_t state)
{
	INFO("I2C DDC0 driver suspend\n");
	//hal_i2cddc0_disable();

	return 0;
}


static int _sp_i2cddc0_resume(struct platform_device *pdev)
{
	INFO("I2C DDC0 driver resume\n");
	//hal_i2cddc0_enable();

	return 0;
}


static int __init _sp_i2cddc0_init(void)
{
	int ret = -1;

	INT32 pin_ret;
	UINT32 pmx_sel;
	pin_ret = gpio_pin_mux_get(PMX_I2CDDC0_A, &pmx_sel);
	if ((0 == pin_ret) && (0 != pmx_sel)||I2C_DDC_DEBUG) {
	    INFO("I2C DDC0 module init\n");

	    if (I2C_DDC_DEBUG)
	        gpio_pin_mux_sel(PMX_I2CDDC0_A, 3);

	    ret = platform_device_register(&sp_i2cddc0_device);
	    if (ret)
	    {
	        ERR("i2c device register failed, ret = %d\n", ret);
	        goto device_register_fail;
	    }

	    ret = platform_driver_register(&sp_i2cddc0_driver);
	    if (ret < 0)
	    {
	        ERR("i2c driver register failed, ret = %d\n", ret);
	        goto driver_register_fail;
	    }

	    INFO("Sunplus I2C module init success\n");

	    return 0;
	} else {
	    return ret;
	}

driver_register_fail:
	platform_device_unregister(&sp_i2cddc0_device);
device_register_fail:

	return ret;
}


static void __exit _sp_i2cddc0_exit(void)
{
	INFO("I2C DDC0 module exit\n");

	platform_driver_unregister(&sp_i2cddc0_driver);
	platform_device_unregister(&sp_i2cddc0_device);

	return;
}

module_init(_sp_i2cddc0_init);
module_exit(_sp_i2cddc0_exit);

/**************************************************************************
 *                  M O D U L E    D E C L A R A T I O N                      *
 **************************************************************************/
 
MODULE_AUTHOR("Sunplus");
MODULE_DESCRIPTION("Sunplus I2C DDC 0 Driver");
MODULE_LICENSE("GPL");
MODULE_VERSION("1.0");




