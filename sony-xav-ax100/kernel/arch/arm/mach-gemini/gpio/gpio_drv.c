#include <linux/types.h>
#include <linux/kdev_t.h>
#include <linux/slab.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <asm/uaccess.h>
#include <linux/platform_device.h>
#include <linux/cdev.h>
#include <asm/io.h>
#include <mach/hardware.h>
#include <mach/gpio_drv.h>
#include <mach/module.h>


//#define GPIO_FUNC_DEBUG
#define GPIO_KDBG_INFO
#define GPIO_KDBG_ERR

#ifdef GPIO_FUNC_DEBUG
    #define FUNC_DEBUG() printk(KERN_INFO "%s: %d %s()\n", __FILE__, __LINE__, __func__)
#else
    #define FUNC_DEBUG()
#endif

#ifdef GPIO_KDBG_INFO
    #define DBG_INFO(fmt, args ...) printk(KERN_INFO "K_GPIO: " fmt, ## args)
#else
    #define DBG_INFO(fmt, args ...)
#endif

#ifdef GPIO_KDBG_ERR
    #define DBG_ERR(fmt, args ...)  printk(KERN_ERR "K_GPIO: " fmt, ## args)
#else
    #define DBG_ERR(fmt, args ...)
#endif

#define NUM_GPIO    (1)

//#define REG_GRP_OFS(GRP, OFFSET)        ((GRP)*32*4+(OFFSET)*4)
#define REG_GRP_OFS(GRP, OFFSET)        ((GRP)*32+(OFFSET))     // When using 32-bit access ( ioread32(), iowrite32())

#define GPIO_FIRST(X)   ( virt_base + REG_GRP_OFS(4, (25+X)))
#define GPIO_MASTER(X)  ( virt_base + REG_GRP_OFS(5, ( 0+X)))
#define GPIO_OE(X)      ( virt_base + REG_GRP_OFS(5, ( 8+X)))
#define GPIO_OUT(X)     ( virt_base + REG_GRP_OFS(5, (16+X)))
#define GPIO_IN(X)      ( virt_base + REG_GRP_OFS(5, (24+X)))

#define GPIO_SFT_CFG(X)      ( virt_base + REG_GRP_OFS(1, (0+X)))

//#define RGST_BASE_ADDRESS               (0x9c000000)

#define DEVICE_NAME             "sp_gpio"


static int dev_major;
static int dev_minor;
static struct cdev *dev_cdevp = NULL;
static struct class *p_gpio_class = NULL;
static u32 *virt_base;
static DEFINE_SPINLOCK(slock_gpio);

#define NUM_GPIO_MAX    (256)
u8 gpio_use_tbl[NUM_GPIO_MAX] = {0xff};


 
typedef struct gpio_pinmux_reg_s
{
	// pinmux
	u32 pinmux[5];

	// gpio first
	u32 gpio_first[7];

	// gpio master
	u32 gpio_master[7];

	// gpio oe
	u32 gpio_oe[7];

	// gpio out
	u32 gpio_out[7];
}gpio_pinmux_reg_t;

typedef void (*irq_callback)(void *);

typedef struct gpio_isr_s
{
	irq_callback cbk;           /*!< @brief callback function */
	void *priv_data;            /*!< @brief private data */
} gpio_isr_t;

typedef struct gpio_handle_s {
	unsigned int pin_index;     /*!< @brief pin index */
	char *name;                 /*!< @brief owner name */
	gpio_isr_t isr;             /*!< @brief pin isr */
} gpio_handle_t;

static gpio_pinmux_reg_t *gpiopmx_reg = NULL;

/**
 * @brief   Gpio input value getting function
 * @param   handle[in]: gpio handle
 * @param   value[out]: input value
 * @return  SP_OK(0)/ERROR_ID
 */
int gpio_get_value(int handle, unsigned int *value)
{
	int ret=0;
	gpio_handle_t *gpio_handle = (gpio_handle_t *)handle;

	*value = GPIO_I_GET(gpio_handle->pin_index);

	return ret;
}
EXPORT_SYMBOL(gpio_get_value);



/**
 * @brief   Gpio GPIO/Normal setting function
 * @param   handle[in]: gpio handle
 * @param   function[in]: pin function, GPIO_FUNC_ORG/GPIO_FUNC_GPIO
 * @return  SP_OK(0)/ERROR_ID
 */
int gpio_set_function(int handle, unsigned int function)
{
	gpio_handle_t *gpio_handle = (gpio_handle_t *)handle;

	GPIO_F_SET(gpio_handle->pin_index, function);

	return 0;
}
EXPORT_SYMBOL(gpio_set_function);

/**
 * @brief   Gpio direction setting function.
 * @param   handle[in]: gpio handle
 * @param   direction[in]: pin direction, GPIO_DIR_INPUT/GPIO_DIR_OUTPUT
 * @return  SP_OK(0)/ERROR_ID
 * @see
 */
int gpio_set_direction(int handle, unsigned int direction)
{
	gpio_handle_t *gpio_handle = (gpio_handle_t *)handle;

	GPIO_E_SET(gpio_handle->pin_index, direction);

	return 0;
}
EXPORT_SYMBOL(gpio_set_direction);



/**
 * @brief   Gpio integrate setting input functin
 * @param   handle[in]: gpio handle
 * @param   pull_level[in]: pull level,
 *  				 GPIO_PULL_LOW/GPIO_PULL_HIGH/GPIO_PULL_FLOATING
 * @return  SP_OK(0)/ERROR_ID
 */
int gpio_set_input(int handle, int pull_level)
{
	int ret;

	ret = gpio_set_function(handle, GPIO_FUNC_GPIO);
	if (ret != 0) {
		goto out;
	}
	ret = gpio_set_direction(handle, GPIO_DIR_INPUT);
	if (ret != 0) {
		goto out;
	}
	if(pull_level!=GPIO_NO_PULL){
	;
	}

out:
	return ret;
}
EXPORT_SYMBOL(gpio_set_input);


/**
 * @brief   Gpio driving current setting function
 * @param   handle[in]: gpio handle
 * @param   current[in]: driving current value(0:4mA, 1:8mA)
 * @return  SP_OK(0)/ERROR_ID
 */
int gpio_set_driving_current(int handle, unsigned int driving_current)
{
	return 0;
}
EXPORT_SYMBOL(gpio_set_driving_current);

long gpio_alloc_tbl_clear(u8 pin_num)
{
	unsigned long flags;

	spin_lock_irqsave(&slock_gpio, flags);
	if(gpio_use_tbl[pin_num] != 0xff){
		gpio_use_tbl[pin_num] = 0xff;
	}
	spin_unlock_irqrestore(&slock_gpio, flags);

	return 0;
}
EXPORT_SYMBOL(gpio_alloc_tbl_clear);


/**
 * @brief   Gpio pin release function.
 * @param   handle[in]: gpio handle to release
 * @return  SP_OK(0)/ERROR_ID
 * @see
 */
int sp_gpio_release(int handle)
{
	unsigned int pin_index;
	gpio_handle_t *h = (gpio_handle_t *)handle;
	pin_index = h->pin_index;
	
  kfree(h);
	/* clear pin requested */
	if((pin_index<NUM_GPIO_MAX)){
		gpio_alloc_tbl_clear(pin_index);
	}
	return 0;
}
EXPORT_SYMBOL(sp_gpio_release);

/**
 * @brief   Gpio output value setting function
 * @param   handle[in]: gpio handle
 * @param   value[in]: output value
 * @return  SP_OK(0)/ERROR_ID
 */
int gpio_set_value(int handle, unsigned int value)
{
	gpio_handle_t *gpio_handle = (gpio_handle_t *)handle;

	GPIO_O_SET(gpio_handle->pin_index, value);

	return 0;
}
EXPORT_SYMBOL(gpio_set_value);



/**
 * @brief   Gpio integrate setting output functin
 * @param   handle[in]: gpio handle
 * @param   value[in]: output value
 * @param   driving_current[in]: driving current value(0:4mA, 1:8mA)
 * @return  SP_OK(0)/ERROR_ID
 */
int gpio_set_output(int handle, unsigned int value, int driving_current)
{
	int ret;

	ret = gpio_set_function(handle, GPIO_FUNC_GPIO);
	if (ret != 0) {
		goto out;
	}
	
	ret = gpio_set_driving_current(handle, driving_current);
	if (ret != 0) {
		goto out;
	}
	
	gpio_set_value(handle, value);	
	ret = gpio_set_direction(handle, GPIO_DIR_OUTPUT);

out:
	return ret;
}
EXPORT_SYMBOL(gpio_set_output);



long gpio_alloc_tbl_set(u8 pin_num, u8 type)		/* type : 0:buildin driver; 1:Dedicate function; 2:customer gpio */
{
	unsigned long flags;
	
	spin_lock_irqsave(&slock_gpio, flags);
	if(gpio_use_tbl[pin_num] != 0xff){
		spin_unlock_irqrestore(&slock_gpio, flags);
		DBG_ERR("gpio[%d] is used by [%d]\n", pin_num, gpio_use_tbl[pin_num]);
	       return -EINVAL;
	}
	else
		gpio_use_tbl[pin_num] = type;

	spin_unlock_irqrestore(&slock_gpio, flags);

	return 0;
}
EXPORT_SYMBOL(gpio_alloc_tbl_set);







/**
 * @brief   Gpio pin request function.
 * @param   pin_index[in]: gpio channel + function id + gid + pin number
 * @param   name[in]: caller's name
 * @return  gpio handle/ERROR_ID
 * @see
 */
int sp_gpio_request(unsigned int pin_index, char *name)
{
	int ret;
	gpio_handle_t *handle;

	if(gpio_alloc_tbl_set(pin_index,0))
	{
		printk("alloc pin_index=%d failed!,gpiocfgOutDrv failed1\n",pin_index);
		return -EBUSY;/*pin_index has been used!*/
	}

	handle = (gpio_handle_t *)kzalloc(sizeof(gpio_handle_t), GFP_KERNEL);
	if (handle == NULL) {
		ret = -ENOMEM;
		goto out;
	}

	handle->pin_index = pin_index;
	handle->name = name;
	ret = (int)handle;
	
out:
	return ret;
}
EXPORT_SYMBOL(sp_gpio_request);


void gpio_tbl_list(void)	/* type : 0:IOP; 1:Dedicate fun; 2:customer gpio */
{
	u32 i;
	
	DBG_INFO("[GPIO TABLE] : 0:IOP; 1:Dedicate fun; 2:customer gpio; 0xff:unused\n");
	for(i=0; i<NUM_GPIO_MAX; i++){
		DBG_INFO("0x%2x", gpio_use_tbl[i]);
		if ((i%16) == 0) DBG_INFO("\n");
	}
	DBG_INFO("\n");
}
EXPORT_SYMBOL(gpio_tbl_list);


long gpio_first_1(u32 bit)
{
    u32 idx, value;
    unsigned long flags;

    idx = bit >> 5;
    if(idx > 7)
        return -EINVAL;

    value = 1 << (bit & 0x1f);

    spin_lock_irqsave(&slock_gpio, flags);
    iowrite32((ioread32(GPIO_FIRST(idx)) | value), GPIO_FIRST(idx));
    spin_unlock_irqrestore(&slock_gpio, flags);

    return 0;
}
EXPORT_SYMBOL(gpio_first_1);

long gpio_first_0(u32 bit)
{
    u32 idx, value;
    unsigned long flags;

    idx = bit >> 5;
    if(idx > 7)
        return -EINVAL;

    value = 1 << (bit & 0x1f);

    spin_lock_irqsave(&slock_gpio, flags);
    iowrite32((ioread32(GPIO_FIRST(idx)) & (~value)), GPIO_FIRST(idx));
    spin_unlock_irqrestore(&slock_gpio, flags);

    return 0;
}
EXPORT_SYMBOL(gpio_first_0);

long gpio_master_1(u32 bit)
{
    u32 idx, value;
    unsigned long flags;

    idx = bit >> 5;
    if(idx > 7)
        return -EINVAL;

    value = 1 << (bit & 0x1f);

    spin_lock_irqsave(&slock_gpio, flags);
    iowrite32((ioread32(GPIO_MASTER(idx)) | value), GPIO_MASTER(idx));
    spin_unlock_irqrestore(&slock_gpio, flags);

    return 0;
}
EXPORT_SYMBOL(gpio_master_1);

long gpio_master_0(u32 bit)
{
    u32 idx, value;
    unsigned long flags;

    idx = bit >> 5;
    if(idx > 7)
        return -EINVAL;

    value = 1 << (bit & 0x1f);

    spin_lock_irqsave(&slock_gpio, flags);
    iowrite32((ioread32(GPIO_MASTER(idx)) & (~value)), GPIO_MASTER(idx));
    spin_unlock_irqrestore(&slock_gpio, flags);

    return 0;
}
EXPORT_SYMBOL(gpio_master_0);

long gpio_set_oe(u32 bit)
{
    u32 idx, value;
    unsigned long flags;
    idx = bit >> 5;
    if(idx > 7)
        return -EINVAL;

    value = 1 << (bit & 0x1f);

    spin_lock_irqsave(&slock_gpio, flags);
    iowrite32((ioread32(GPIO_OE(idx)) | value), GPIO_OE(idx));
    spin_unlock_irqrestore(&slock_gpio, flags);

    return 0;
}
EXPORT_SYMBOL(gpio_set_oe);

long gpio_clr_oe(u32 bit)
{
    u32 idx, value;
    unsigned long flags;
    idx = bit >> 5;
    if(idx > 7)
        return -EINVAL;

    value = 1 << (bit & 0x1f);

    spin_lock_irqsave(&slock_gpio, flags);
    iowrite32((ioread32(GPIO_OE(idx)) & (~value)), GPIO_OE(idx));
    spin_unlock_irqrestore(&slock_gpio, flags);

    return 0;
}
EXPORT_SYMBOL(gpio_clr_oe);

long gpio_out_1(u32 bit)
{
    u32 idx, value;
    unsigned long flags;
    idx = bit >> 5;
    if(idx > 7)
        return -EINVAL;

    value = 1 << (bit & 0x1f);

    spin_lock_irqsave(&slock_gpio, flags);
    iowrite32((ioread32(GPIO_OUT(idx)) | value), GPIO_OUT(idx));
    spin_unlock_irqrestore(&slock_gpio, flags);

    return 0;
}
EXPORT_SYMBOL(gpio_out_1);

long gpio_out_0(u32 bit)
{
    u32 idx, value;
    unsigned long flags;
    idx = bit >> 5;
    if(idx > 7)
        return -EINVAL;

    value = 1 << (bit & 0x1f);

    spin_lock_irqsave(&slock_gpio, flags);
    iowrite32((ioread32(GPIO_OUT(idx)) & (~value)), GPIO_OUT(idx));
    spin_unlock_irqrestore(&slock_gpio, flags);

    return 0;
}
EXPORT_SYMBOL(gpio_out_0);

long gpio_in(u32 bit, u32 *gpio_in_value)
{
    u32 idx, value;
    long err = 0;
    unsigned long flags;
    idx = bit >> 5;
    if(idx > 7)
        return -EINVAL;

    value = 1 << (bit & 0x1f);

    spin_lock_irqsave(&slock_gpio, flags);
    *gpio_in_value = (ioread32(GPIO_IN(idx)) & value) ? 1 : 0;
    spin_unlock_irqrestore(&slock_gpio, flags);

    return err;
}
EXPORT_SYMBOL(gpio_in);

u32 gpio_in_val(u32 bit)
{
    u32 value;

    gpio_in(bit, &value);

    return value;
}
EXPORT_SYMBOL(gpio_in_val);

u32 gpio_in_unlocked(u32 bit)
{
    u32 idx, value, gpio_in_value;
    long err = 0;

    idx = bit >> 5;
    if(idx > 7)
        return -EINVAL;

    value = 1 << (bit & 0x1f);

    gpio_in_value = (ioread32(GPIO_IN(idx)) & value) ? 1 : 0;

    return gpio_in_value;
}
EXPORT_SYMBOL(gpio_in_unlocked);

long gpio_pin_mux_sel(PMXSEL_ID id, u32 sel)
{
    u32 idx, max_value, reg_val, mask, bit_num;
    unsigned long flags;

    idx = (id >> 16)&0xff;
    if (idx > 0x11)
        return -EINVAL;

    max_value = (id >> 8)&0xff;
    if (sel > max_value)
        return -EINVAL;

    bit_num = id & 0xff;

    if (max_value==1)
        mask = 0x01<<bit_num;
    else if ((max_value==2)||(max_value==3))
        mask = 0x03<<bit_num;
    else
        mask = 0x07<<bit_num;
        
    spin_lock_irqsave(&slock_gpio, flags);
    reg_val = ioread32(GPIO_SFT_CFG(idx));
    reg_val &= (~mask);
    reg_val |= (sel<<bit_num);
    iowrite32(reg_val, GPIO_SFT_CFG(idx));
    spin_unlock_irqrestore(&slock_gpio, flags);

    return 0;
}
EXPORT_SYMBOL(gpio_pin_mux_sel);

long gpio_pin_mux_get(PMXSEL_ID id, u32 *sel)
{
    u32 idx, max_value, reg_val, mask, bit_num;
    unsigned long flags;
    idx = (id >> 16)&0xff;
    if (idx > 0x11)
        return -EINVAL;

    max_value = (id >> 8)&0xff;
//    if (sel > max_value)
//        return -EINVAL;

    bit_num = id & 0xff;

    if (max_value==1)
        mask = 0x01<<bit_num;
    else if ((max_value==2)||(max_value==3))
        mask = 0x03<<bit_num;
    else
        mask = 0x07<<bit_num;

    spin_lock_irqsave(&slock_gpio, flags);
    reg_val = ioread32(GPIO_SFT_CFG(idx));
    reg_val &= mask;
    *sel = (reg_val >> bit_num);
    spin_unlock_irqrestore(&slock_gpio, flags);

    return 0;
}
EXPORT_SYMBOL(gpio_pin_mux_get);


static long gpio_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
    long err = 0;
    u32 bit;
    u32 gpio_in_value;
    PMXSEL_T pin_mux;
		
    FUNC_DEBUG();


    if (cmd == GPIO_IOC_PIN_MUX_SEL){
        if ( unlikely(copy_from_user(&pin_mux, (void *)arg, sizeof(pin_mux))))
            return -EFAULT;
    }
    else{
        if ( unlikely(copy_from_user(&bit, (void *)arg, sizeof(bit))))
            return -EFAULT;
    }

    switch (cmd) {
        case GPIO_IOC_FIRST_1:  err = gpio_first_1(bit);            break;
        case GPIO_IOC_FIRST_0:  err = gpio_first_0(bit);            break;
        case GPIO_IOC_MASTER_1: err = gpio_master_1(bit);           break;
        case GPIO_IOC_MASTER_0: err = gpio_master_0(bit);           break;
        case GPIO_IOC_SET_OE:   err = gpio_set_oe(bit);             break;
        case GPIO_IOC_CLR_OE:   err = gpio_clr_oe(bit);             break;
        case GPIO_IOC_OUT_1:    err = gpio_out_1(bit);              break;
        case GPIO_IOC_OUT_0:    err = gpio_out_0(bit);              break;
        case GPIO_IOC_IN:       err  = gpio_in(bit, &gpio_in_value);
                                err |= copy_to_user((void *)arg, &gpio_in_value, sizeof(gpio_in_value));
                                break;
        case GPIO_IOC_PIN_MUX_SEL:    err = gpio_pin_mux_sel(pin_mux.id, pin_mux.val);              break;
        default:                err = -EINVAL;                      break;
    }

    return err;
}

static int gpio_open(struct inode *inode, struct file *filp)
{
    FUNC_DEBUG();
    return 0;
}

static int gpio_release(struct inode *inode, struct file *filp)
{
    FUNC_DEBUG();
    return 0;
}

static int __devinit
gpio_probe(
    struct platform_device *pdev
)
{
    return 0;
}

static int __devexit
gpio_remove(
    struct platform_device *pdev
)
{
    return 0;
}

static int
gpio_suspend(
	struct platform_device *pdev,
		pm_message_t state
)
{
	u32 i, j;

	DBG_INFO("[GPIO] suspend\n");

	gpiopmx_reg = (gpio_pinmux_reg_t *)kmalloc(sizeof(gpio_pinmux_reg_t), GFP_KERNEL);

	j=1;
	for(i=0; i<5; i++){
		gpiopmx_reg->pinmux[i] = ioread32(GPIO_SFT_CFG(j));
		j++;
		if (j == 5)
			j++;
	}

	for(i=0; i<7; i++){
		gpiopmx_reg->gpio_first[i] = ioread32(GPIO_FIRST(i));
	}

	for(i=0; i<7; i++){
		gpiopmx_reg->gpio_master[i] = ioread32(GPIO_MASTER(i));
	}

	for(i=0; i<7; i++){
		gpiopmx_reg->gpio_oe[i] = ioread32(GPIO_OE(i));
	}

	for(i=0; i<7; i++){
		gpiopmx_reg->gpio_out[i] = ioread32(GPIO_OUT(i));
	}

	return 0;
}

static int
gpio_resume(
    struct platform_device *pdev
)
{
	u32 i, j;

	DBG_INFO("[GPIO] resume\n");

	for(i=0; i<7; i++){
		iowrite32(gpiopmx_reg->gpio_out[i], GPIO_OUT(i));
	}

	for(i=0; i<7; i++){
		iowrite32(gpiopmx_reg->gpio_oe[i], GPIO_OE(i));
	}

	for(i=0; i<7; i++){
		iowrite32(gpiopmx_reg->gpio_master[i], GPIO_MASTER(i));
	}

	for(i=0; i<7; i++){
		iowrite32(gpiopmx_reg->gpio_first[i], GPIO_FIRST(i));
	}

	j=1;
	for(i=0; i<5; i++){
		iowrite32(gpiopmx_reg->pinmux[i], GPIO_SFT_CFG(j));
		j++;
		if (j == 5)
			j++;
	}

	kfree(gpiopmx_reg);

	return 0;
}

static struct file_operations fops = {
    .owner          = THIS_MODULE,
    .unlocked_ioctl = gpio_ioctl,
    .open           = gpio_open,
    .release        = gpio_release
};

static struct platform_device sp_gpio_device = {
	.name	= "sp_gpio",
	.id	= 0,
};

static struct platform_driver sp_gpio_driver = {
	.probe		= gpio_probe,
	.remove 		= gpio_remove,
	.suspend		= gpio_suspend,
	.resume		= gpio_resume,
	.driver		= {
		.name	= "sp_gpio",
		.owner	= THIS_MODULE,
	},
};

static int __init gpio_drv_init(void)
{
    dev_t dev;
    int ret;

    FUNC_DEBUG();

    ret = alloc_chrdev_region(&dev, 0, NUM_GPIO, DEVICE_NAME);
    if (ret) {
        DBG_ERR("Error: alloc_chrdev_region()\n");
        return ret;
    }

    dev_major = MAJOR(dev);
    dev_minor = MINOR(dev);
    DBG_INFO("register chrdev(%d,%d)\n", dev_major, dev_minor);

    // Script:
    // rm -f /dev/sp_gpio
    // insmod gpio_drv.ko
    // MAJOR_GPIO=`cat /proc/devices | awk '/sp_gpio/ {print $1}'`
    // MAJOR_GPIO=`grep sp_gpio /proc/devices | sed 's/ *sp_gpio.*//'`
    // mknod /dev/sp_gpio c $MAJOR_GPIO 0

    dev_cdevp = kzalloc(sizeof(struct cdev), GFP_KERNEL);
    if (dev_cdevp == NULL) {
        DBG_ERR("Error: kzalloc().\n");
        goto failed00;
    }

    p_gpio_class = class_create(THIS_MODULE, DEVICE_NAME);
    cdev_init(dev_cdevp, &fops);
    dev_cdevp->owner = THIS_MODULE;
    ret = cdev_add(dev_cdevp, dev, NUM_GPIO);
    if (ret < 0) {
        DBG_ERR("add chr dev failed\n");
        goto failed00;
    }
    device_create(p_gpio_class, NULL, dev, NULL, DEVICE_NAME);

    /*
    virt_base = ioremap_nocache(RGST_BASE_ADDRESS, 32*256*4);   // Map 256 groups of registers.
    if(!virt_base){
        DBG_ERR("Error: ioremap_nocache().\n");
        goto failed01;
    }
    DBG_INFO("virt_base: %p.\n", virt_base);
    */
    virt_base = (u32 *)IO0_BASE;
    platform_device_register(&sp_gpio_device);
    platform_driver_register(&sp_gpio_driver);

    return 0;
	/*
	failed01:
    if(dev_cdevp)
        cdev_del(dev_cdevp);
    */

failed00:
    if(dev_cdevp) {
        kfree(dev_cdevp);
        dev_cdevp = NULL;
    }

    unregister_chrdev_region(dev, NUM_GPIO);
    return -ENODEV;
}

static void __exit gpio_drv_exit(void)
{
    dev_t dev;

    FUNC_DEBUG();
    /*
    if(virt_base)
        iounmap(virt_base);
    */

    dev = MKDEV(dev_major, dev_minor);
    if (dev_cdevp) {
        cdev_del(dev_cdevp);
        kfree(dev_cdevp);
        dev_cdevp = NULL;
    }

    unregister_chrdev_region(dev, NUM_GPIO);

    platform_device_unregister(&sp_gpio_device); // for platform driver
    platform_driver_unregister(&sp_gpio_driver); // for platform driver
}

module_init(gpio_drv_init);
module_exit(gpio_drv_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Sunplus Technology");
MODULE_DESCRIPTION("letb123 GPIO driver");
