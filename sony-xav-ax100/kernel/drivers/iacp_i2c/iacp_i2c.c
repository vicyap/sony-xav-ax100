#include <linux/init.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/fs.h>
#include <linux/proc_fs.h>
#include <linux/device.h>
#include <linux/uaccess.h>
#include <linux/i2c.h>
#include <linux/delay.h>
//#include <mach/gp_ti2c_bus.h>

#include "iacp_i2c.h"

//#define USE_I2C_MASTER_0
#define USE_I2C_MASTER_1

#define IACP_ADDR (0x22 >> 0x01)
#define TI2C_CLK 100

#define IACP_IOCTL_BASE		'I'
#define	I2C_CMD_READ		_IOR(IACP_IOCTL_BASE, 1, int)
#define	I2C_CMD_WRITE		_IOW(IACP_IOCTL_BASE, 2, int)

#define MAX_DATA_LEN		1024

static int iacp_i2c_major = 0;
static int iacp_i2c_minor = 0;

static struct class* iacp_i2c_class = NULL;
static struct st_iacp_dev* iacp_dev = NULL;

struct iacp_i2c_dev{
struct i2c_client client;
int users; // how many users using the driver
};
struct iacp_i2c_dev g_iacp_i2c_dev; 

typedef struct i2c_param_s{
uint8_t *reg;
uint8_t *len;
uint8_t *value;
}i2c_param;

//ti2c_set_value_t hd;

//char devices..............................

static int iacp_open(struct inode* inode,struct file* filp);
static int iacp_release(struct inode* inode,struct file* filp);
static long iacp_ioctl(struct file *filp, unsigned int cmd, unsigned long arg);
static int iacp_write(u8 addr,u8* data,uint16_t len);
static int iacp_read(u8 addr,u8* data,uint16_t len);
static int iacp_register(void);
static ssize_t iacp_nor_read(struct file *file, char __user *buffer, size_t count, loff_t *ppos);
static ssize_t iacp_nor_write(struct file *file, char __user *buffer, size_t count, loff_t *ppos);

static struct file_operations iacp_fops = {
	.owner = THIS_MODULE,
	.open = iacp_open,
	.release = iacp_release,
	.unlocked_ioctl= iacp_ioctl, 
	.read = iacp_nor_read,
	.write = iacp_nor_write,
	
};

static int iacp_open(struct inode* inode,struct file* filp){
	filp->private_data = &g_iacp_dev;
	printk( KERN_ALERT " open iacp \n" );
	return 0;
}

static int iacp_release(struct inode* inode,struct file* filp){
	printk( KERN_ALERT " release iacp \n" );
	return 0;
}

static ssize_t iacp_nor_read(struct file *file, char __user *buffer, size_t count, loff_t *ppos)
{
	printk( KERN_ALERT " read .................. \n" );
	uint8_t reg = 0;
	i2c_param *userdata;
	uint8_t *data = kmalloc(MAX_DATA_LEN, GFP_KERNEL);
	int ret = 0;
	uint16_t len = 0;
	int i;

	userdata = (i2c_param *)buffer;
	if(!userdata)
	{
		printk("user space data error!!\n");
	}
	printk(" reg:%d\n",*(userdata->reg));
	printk(" len:%d\n",*(userdata->len));
	
	reg = *(userdata->reg);
	len = *(userdata->len);
	ret = iacp_read(reg, data,len);
	memcpy(userdata->value,data,len);
	for(i = 0;i<len;i++)
		printk(" data:0x%3x\n",data[i]);
	return ret;
}

static ssize_t iacp_nor_write(struct file *file, char __user *buffer, size_t count, loff_t *ppos)
{
	printk( KERN_ALERT " write .................. \n" );
		uint8_t reg = 0;
	i2c_param *userdata;
	uint8_t *data = kmalloc(MAX_DATA_LEN, GFP_KERNEL);
	int ret = 0;
	uint16_t len = 0;
	int i;

	userdata = (i2c_param *)buffer;
	if(!userdata)
	{
		printk("user space data error!!\n");
	}
	printk(" reg:%d\n",*(userdata->reg));
	printk(" len:%d\n",*(userdata->len));
	
	reg = *(userdata->reg);
	len = *(userdata->len);
	memcpy(data,userdata->value,len);
	for(i = 0;i<len;i++)
		printk(" data:0x%3x\n",data[i]);
	ret = iacp_write(reg, data,len);
	return ret;
}

static long iacp_ioctl(struct file *filp, unsigned int cmd, unsigned long __user arg)  {
	uint8_t reg = 0;
	i2c_param *userdata;
	uint8_t *data = kmalloc(MAX_DATA_LEN, GFP_KERNEL);
	int ret = 0;
	uint16_t len = 0;
	int i;

	//printk( KERN_ALERT " ioctl iacp \n" );
	userdata = (i2c_param *)arg;
	if(!userdata)
	{
		printk("user space data error!!\n");
	}
	//printk( KERN_ALERT " reg:%d\n",*(userdata->reg));
	//printk( KERN_ALERT " len:%d\n",*(userdata->len));
	
	reg = *(userdata->reg);
	len = *(userdata->len);
	
	//printk( KERN_ALERT " ioctl cmd:%d\n",cmd);
	switch(cmd){

		case I2C_CMD_READ:
			{
				//printk( KERN_ALERT " iacp read\n" );
				ret = iacp_read(reg, data,len);
				memcpy(userdata->value,data,len);
				/*
				for(i = 0;i<len;i++)
					printk( KERN_ALERT " data:0x%3x\n",data[i]);
				*/	
				break;
			}
		
		case I2C_CMD_WRITE:
			{
				//printk( KERN_ALERT " iacp write\n" );
				memcpy(data,userdata->value,len);
				/*
				for(i = 0;i<len;i++)
					printk( KERN_ALERT " data:0x%3x\n",data[i]);
				*/	
				ret = iacp_write(reg, data,len);
				break;
			}
		
		default:
			break;
		}
	kfree(data);
	return ret;
} 

static int iacp_write(u8 addr,u8* data,uint16_t len)
{
	struct i2c_adapter *adap = g_iacp_i2c_dev.client.adapter;
	struct i2c_msg msg;
	int retry = 0;
	int i;
	u8 temp[30] = {0};
	temp[0] = addr;
	for(i = 0;i < len;i++)
	{
		temp[i+1] = data[i];
	}
	len += 1;
	msg.addr = g_iacp_i2c_dev.client.addr;
	msg.flags = 0;
	msg.len = len;
	msg.buf = (char *)temp;
	
	while((1 != i2c_transfer(adap, &msg, 1))  && (retry < 10)) {
		retry ++;
		printk( KERN_ERR " iacp_i2c_write fail! \n" );
	}
	if(retry >= 10)
		return -1;
	return 0;
}

static int iacp_read(u8 addr,u8* data,uint16_t len)
{
	struct i2c_adapter *adap = g_iacp_i2c_dev.client.adapter;
	struct i2c_msg msg;
	int retry  = 0;
	int r_retry  = 0;

retry_entry:
	
	msg.addr = g_iacp_i2c_dev.client.addr;
	msg.flags = 0;
	msg.len = 1;
	msg.buf = (char *)&addr;

	while((1 != i2c_transfer(adap, &msg, 1))  && (retry < 10)) {
		retry ++;
		mdelay(20);
	}
	if(retry >= 10)
	{
		printk( KERN_ALERT " write addr fail! retry>=10\n");
		return -1;
	}
	retry = 0;
	// read 
	msg.flags |= I2C_M_RD;
	msg.len = len;
	msg.buf = (char *)data;
	mdelay(20);
	if(1 !=i2c_transfer(adap, &msg, 1)){
		mdelay(20);
		r_retry++;
		
		if(r_retry < 5){
			goto retry_entry;
		}else{
			printk( KERN_ALERT " read addr fail! r_retry>=5\n");	
			return -1;
		}
	}
	/*
	while((1 !=i2c_transfer(adap, &msg, 1)) && (retry < 10)) {
		retry ++;
		printk( KERN_ALERT " read addr fail! retry= %d\n" ,retry);
	}
	if(retry >= 10)
		return -1;
	*/
	return 0;
} 

static int  __iacp_setup_dev(struct st_iacp_dev* dev) {
	int err;
	dev_t devno = MKDEV(iacp_i2c_major, iacp_i2c_minor);

	cdev_init(&(dev->dev), &iacp_fops);
	dev->dev.owner = THIS_MODULE;
	dev->dev.ops = &iacp_fops;

	err = cdev_add(&(dev->dev),devno, 1);
	if(err) {
		return err;
	}	

	return 0;
}

//i2c driver.............................
static const struct i2c_device_id iacp_id[] = {
	{ "iacp", 0, },
};

static int __devinit iacp_i2c_probe(struct i2c_client *client,const struct i2c_device_id *id)
{
	
	int err = -1;
	dev_t devno = MKDEV(iacp_i2c_major, iacp_i2c_minor);
	struct device* temp = NULL;
	
	err = alloc_chrdev_region(&devno, 0, 1, IACP_DEVICE_NODE_NAME);
	if(err < 0) {
		printk(KERN_ALERT"Failed to alloc char dev region.\n");
		goto fail;
	}
	iacp_i2c_major = MAJOR(devno);
	iacp_i2c_minor = MINOR(devno);

	iacp_dev = kmalloc(sizeof(struct st_iacp_dev), GFP_KERNEL);
	if(!iacp_dev) {
		err = -ENOMEM;
		printk(KERN_ALERT"Failed to alloc iacp device.\n");
		goto unregister;
	}
	
	memset(iacp_dev,0,sizeof(struct st_iacp_dev));
	err = __iacp_setup_dev(iacp_dev);
	if(err) {
		printk(KERN_ALERT"Failed to setup iacp device: %d.\n", err);
		goto unregister;
	}
	
	iacp_i2c_class = class_create(THIS_MODULE, IACP_DEVICE_CLASS_NAME);
	if(IS_ERR(iacp_i2c_class)) {
		err = PTR_ERR(iacp_i2c_class);
		printk(KERN_ALERT"Failed to create iacp device class.\n");
		goto destroy_cdev;
	}

	temp = device_create(iacp_i2c_class, NULL, devno, "%s", IACP_DEVICE_FILE_NAME);
	if(IS_ERR(temp)) {
		err = PTR_ERR(temp);
		printk(KERN_ALERT"Failed to create iacp device.\n");
		goto destroy_class;
	}

	if(err != 0) {
		printk("[%s], Open TI2C device fail.\n", __FUNCTION__);
		err = -EIO;
		goto cleanup;
	}
	printk(KERN_ALERT"Succedded to initialize iacp device.\n");
	
	return 0;

destroy_class:
	class_destroy(iacp_i2c_class);
destroy_cdev:
	cdev_del(&(iacp_dev->dev));	
cleanup:
	kfree(iacp_dev);
unregister:
	unregister_chrdev_region(MKDEV(iacp_i2c_major, iacp_i2c_minor), 1);	
fail:
	return err;
}

static int __devinit iacp_i2c_remove (struct i2c_client *client)
{
	
	
	dev_t devno = MKDEV(iacp_i2c_major, iacp_i2c_minor);

	class_destroy(iacp_i2c_class);
	cdev_del(&(iacp_dev->dev));
	kfree(iacp_dev);

	unregister_chrdev_region(devno, 1);
	return 0;
}

static struct i2c_driver iacp_i2c_driver = {
	.driver = {
		.name = "iacp",
		.owner = THIS_MODULE,
	},
	.probe      = iacp_i2c_probe,   
    .remove     = iacp_i2c_remove,
    .id_table= iacp_id,
};


static int iacp_register(void)
{
	struct i2c_board_info info;
	struct i2c_adapter *adapter;
	struct i2c_client *client;
	memset(&info, 0, sizeof(struct i2c_board_info));
	info.addr = IACP_ADDR;
	strlcpy(info.type, "iacp", I2C_NAME_SIZE);
#if defined(USE_I2C_MASTER_0)
	adapter = i2c_get_adapter(0); 
#elif defined(USE_I2C_MASTER_1)
	adapter = i2c_get_adapter(1); 
#else
	#error "Please Choice I2C Master Device First !"
#endif
	if (!adapter) {
		printk("get adapter err!!\n");
		return -ENODEV;
	}
	printk(KERN_ALERT"Succedded to get adapter 1\n");
	client = i2c_new_device(adapter, &info);
	g_iacp_i2c_dev.client.addr = client->addr;
	g_iacp_i2c_dev.client.adapter = client->adapter;
	g_iacp_i2c_dev.client.flags = client->flags;
	g_iacp_i2c_dev.client.driver =	&iacp_i2c_driver;
 
	return 0;
}

/* init the driver */
static int __init iacp_init(void)
{
	int ret = 0;
	iacp_register();

	ret = i2c_add_driver(&iacp_i2c_driver);

	return ret;
}

/* cleanup the driver */
static void __exit iacp_exit(void)
{
	i2c_del_driver(&iacp_i2c_driver);
}

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("iacp");

module_init(iacp_init);
module_exit(iacp_exit);


