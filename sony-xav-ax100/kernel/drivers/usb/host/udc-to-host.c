#include <linux/module.h>
#include <linux/types.h>
#include <linux/fs.h>
#include <linux/errno.h>
#include <linux/mm.h>
#include <linux/sched.h>
#include <linux/init.h>
#include <linux/cdev.h>
#include <asm/io.h>
#include <asm/system.h>
#include <asm/uaccess.h>
#include <linux/platform_device.h>
#include <linux/usb.h>
#include <mach/hardware.h>

#include "udc-to-host.h"

struct udc_hcd_dev {
	struct cdev cdev;

	struct udc_hcd_platform_device pdev;
};

struct udc_hcd_dev udc_hcd_cdev;
struct class *udc_hcd_class;

int udc_hcd_open(struct inode *inode, struct file *filp)
{
	DEGUG_LOG("K: open udc hcd\n");
	filp->private_data = &udc_hcd_cdev;
	
	return 0; 
}

int udc_hcd_release(struct inode *inode, struct file *filp)
{
	return 0;
}

long udc_hcd_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
	struct udc_hcd_dev *pdev = (struct udc_hcd_dev *)filp->private_data;
	u32 *grop1_base = (u32*)IO0_ADDRESS(1*32*4);
	
	DEGUG_LOG("K: ioctl udc cmd hcd\n");

	switch(cmd){
		case SET_PORT_TO_UDC:
			udc_notify_add_device(&pdev->pdev);
			while(!IS_UDC_STATE(pdev->pdev.state)){
				msleep(1);
			}
			SET_UPHY2_TO_UDC(grop1_base);
			break;
			
		case SET_PORT_TO_HCD:
			udc_notify_remove_device(&pdev->pdev);
			break;
			
		default:
			break;
	}
	
	return 0;
}
static const struct file_operations udc_hcd_fops =
{
  .owner = THIS_MODULE,
  .unlocked_ioctl = 	udc_hcd_ioctl,
  .open = 				udc_hcd_open,
  .release = 			udc_hcd_release,
};

int find_ehci_platform_device(struct device *dev, void *arg)
{
	struct udc_hcd_dev *udc_hcd_cdev = (struct udc_hcd_dev *)arg;
	struct platform_device *tmp;

	tmp = to_platform_device(dev);

	if(tmp && (tmp->id == 3))
		udc_hcd_cdev->pdev.p_ehcd = tmp;

	return 0;
}

int find_ohci_platform_device(struct device *dev, void *arg)
{
	struct udc_hcd_dev *udc_hcd_cdev = (struct udc_hcd_dev *)arg;
	struct platform_device *tmp;

	tmp = to_platform_device(dev);
	
	if(tmp && (tmp->id == 3))
		udc_hcd_cdev->pdev.p_ohcd = tmp;

	return 0;
}

int find_udc_platform_device(struct device *dev, void *arg)
{
	struct udc_hcd_dev *udc_hcd_cdev = (struct udc_hcd_dev *)arg;
	
	udc_hcd_cdev->pdev.p_udc = to_platform_device(dev);
		
	return 0;
}

static int udc_hcd_register_platform_entities(struct udc_hcd_dev *pdev)
{
	struct device_driver *driver;

	/*find ehci platform device 2*/
	driver = driver_find("ehci-platform", &platform_bus_type);
	if(driver)
		driver_for_each_device(driver, NULL, pdev, find_ehci_platform_device);

	/*find ohci platform device 2*/
	
	driver = driver_find("ohci-platform", &platform_bus_type);
	if(driver)
		driver_for_each_device(driver, NULL, pdev, find_ohci_platform_device);
	
	/*find udc platform device*/
	driver = driver_find("sp-udc", &platform_bus_type);
	if(driver)
		driver_for_each_device(driver, NULL, pdev, find_udc_platform_device);


	//printk("%s %p %p %p\n",__func__,pdev->pdev.p_ehcd,pdev->pdev.p_ohcd,pdev->pdev.p_udc);
	
	return 0;
}

static int udc_hcd_dev_init(void)
{
	int result;

	dev_t devno = MKDEV(udc_hcd_major, 0);

	if (udc_hcd_major)
		result = register_chrdev_region(devno, UDC_HCD_NR_DEVS, "udc_hcd_dev");
	else
	{
		result = alloc_chrdev_region(&devno, 0, UDC_HCD_NR_DEVS, "udc_hcd_dev");
		udc_hcd_major = MAJOR(devno);
	} 

	if (result < 0)
		return result;

	udc_hcd_class = class_create(THIS_MODULE,"udc_hcd_dev");
	if(IS_ERR(udc_hcd_class)) 
	{
	 	printk("Err: failed in creating class.\n");
		return -1; 
	}
	
	device_create(udc_hcd_class,NULL,devno,NULL,"udc_hcd_dev"); 	 
	if (result < 0) 
	{
		printk ("hello: can't get major number %d\n", udc_hcd_major);
		return result;
	}

	cdev_init(&udc_hcd_cdev.cdev, &udc_hcd_fops);
	udc_hcd_cdev.cdev.owner = THIS_MODULE;
	udc_hcd_cdev.cdev.ops = &udc_hcd_fops;

	cdev_add(&udc_hcd_cdev.cdev, MKDEV(udc_hcd_major, 0), UDC_HCD_NR_DEVS);

	udc_hcd_register_platform_entities(&udc_hcd_cdev);

	return 0;
}

static void udc_hcd_dev_exit(void)
{
	cdev_del(&udc_hcd_cdev.cdev);
	device_destroy(udc_hcd_class, MKDEV(udc_hcd_major,0));
    class_destroy(udc_hcd_class);
	unregister_chrdev_region(MKDEV(udc_hcd_major, 0), 2);
}

late_initcall(udc_hcd_dev_init);
module_exit(udc_hcd_dev_exit);

MODULE_AUTHOR("yong.y@sunmedia.com.cn");
MODULE_LICENSE("GPL");
