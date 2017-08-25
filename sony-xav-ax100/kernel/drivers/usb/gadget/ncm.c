/*
 * ncm.c -- NCM gadget driver
 *
 * Copyright (C) 2010 Nokia Corporation
 * Contact: Yauheni Kaliuta <yauheni.kaliuta@nokia.com>
 *
 * The driver borrows from ether.c which is:
 *
 * Copyright (C) 2003-2005,2008 David Brownell
 * Copyright (C) 2003-2004 Robert Schwebel, Benedikt Spranger
 * Copyright (C) 2008 Nokia Corporation
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

/* #define DEBUG */
/* #define VERBOSE_DEBUG */

#include <linux/kernel.h>
#include <linux/utsname.h>
#include <linux/kthread.h>
#include <linux/module.h>

#include "u_ether.h"

#define DRIVER_DESC		"letb8700_Android"
#define IAP

/*add char dev*/
struct usb_function *carplay_f;

#ifdef IAP
static u32 iap_debug = 1;
module_param(iap_debug, uint, 0644);
#define IAP_DEBUG(fmt, arg...)	do { if (iap_debug) printk(KERN_ERR fmt,##arg); } while (0)

#define DEV_ADD 2
#define DEV_RM	1
struct class *iapClass;
dev_t dev;

struct usb_gadget *gadget_iap;
static u8 hostaddr[ETH_ALEN];

struct usb_request *iap_alloc_ep_req(struct usb_ep *ep);
static void iap_complete(struct usb_ep *ep, struct usb_request *req);
int enable_iap(struct usb_ep *ep);
#endif
/*-------------------------------------------------------------------------*/

/*
 * Kbuild is not very cooperative with respect to linking separately
 * compiled library objects into one module.  So for now we won't use
 * separate compilation ... ensuring init/exit sections work to shrink
 * the runtime footprint, and giving us at least some parts of what
 * a "gcc --combine ... part1.c part2.c part3.c ... " build would.
 */
#include "composite.c"
#include "usbstring.c"
#include "config.c"
#include "epautoconf.c"

#include "f_ncm.c"
#include "u_ether.c"
#ifdef IAP
#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/wait.h>

#define IAP_BUF_NUM 20
static int iap_major = 0;
static struct cdev iapDevs;
static struct iap_dev {
	struct cdev cdev;
	wait_queue_head_t read_queue;
	wait_queue_head_t main_queue;
	struct task_struct *iap_task;
	char main_condition;
} my_iap_dev;
struct iap_packet {
	unsigned int len;
	unsigned char *buf;
};
struct iap_packet iap_rx[IAP_BUF_NUM], iap_tx[IAP_BUF_NUM];
unsigned char rx_index, tx_index;
unsigned char read_index, write_index;
struct usb_ep *ep_iap_in, *ep_iap_out;
static struct usb_endpoint_descriptor iap_in_desc = {
	.bLength = USB_DT_ENDPOINT_SIZE,
	.bDescriptorType = USB_DT_ENDPOINT,

	.bmAttributes = USB_ENDPOINT_XFER_BULK,
	.wMaxPacketSize = cpu_to_le16(512),
};

static struct usb_endpoint_descriptor iap_out_desc = {
	.bLength = USB_DT_ENDPOINT_SIZE,
	.bDescriptorType = USB_DT_ENDPOINT,

	.bmAttributes = USB_ENDPOINT_XFER_BULK,
	.wMaxPacketSize = cpu_to_le16(512),
};

static int iap_open(struct inode *inode, struct file *file)
{
	IAP_DEBUG("iap: open!\n");
	if (iap_rx[read_index].len == ~0) {
		iap_rx[read_index].len = 0;
	}
	return 0;
}

static int iap_release(struct inode *inode, struct file *file)
{
	IAP_DEBUG("iap: iap_release!\n");
	iap_rx[rx_index].len = ~0;
	wake_up(&(my_iap_dev.read_queue));
	return 0;
}

ssize_t iap_read(struct file * file, char __user * buff, size_t size,
		 loff_t * offp)
{
	unsigned int count = size;

	IAP_DEBUG("%s %d len=%d \n", __FUNCTION__, __LINE__,
		  iap_rx[read_index].len);
#if 1
	//while (!iap_rx[read_index].len) /*no data to read*/
	{
		if (file->f_flags & O_NONBLOCK)
			return -EAGAIN;
		wait_event_interruptible(my_iap_dev.read_queue,
					 iap_rx[read_index].len);
	}
#endif
#if 1
	if (iap_rx[read_index].len == ~0) {
		printk("no data\n");
		iap_rx[read_index].len = 0;
		if (my_iap_dev.main_condition == DEV_RM)
			wake_up(&(my_iap_dev.main_queue));
		return -ENODEV;
	}
	if ((!iap_rx[read_index].len)) {
		printk("no data can be read!\n");
		return 0;
	}
#endif
	if (count > iap_rx[read_index].len)
		count = iap_rx[read_index].len;
	if (copy_to_user(buff, (void *)(iap_rx[read_index].buf), count)) {
		printk("iap: read EFAULT !\n");
		return -EFAULT;
	}
	iap_rx[read_index].len = 0;
	kfree(iap_rx[read_index].buf);
	IAP_DEBUG("iAP read %d bytes(s)\n", count);
	return count;
}

ssize_t iap_write(struct file * file, const char __user * buff, size_t size,
		  loff_t * offp)
{
	struct usb_request *req;
	int status;
	IAP_DEBUG("%s %d size=%d\n", __FUNCTION__, __LINE__, size);

	if ((iap_tx[write_index].len) /*||(iap_rx[write_index].buf) */ ) {
		printk("buffer is full! len=%d\n", iap_tx[write_index].len);
		return 0;
	}
	iap_tx[write_index].buf = kmalloc(size, GFP_ATOMIC);	//TODO
	if (!iap_tx[write_index].buf)
		return -ENOMEM;
	iap_tx[write_index].len = size;
	if (copy_from_user(iap_tx[write_index].buf, buff, size))
		return -EFAULT;
	req = iap_alloc_ep_req(ep_iap_in);
	if (!req)
		return -ENOMEM;
	req->buf = iap_tx[write_index].buf;
	req->length = iap_tx[write_index].len;
	req->complete = iap_complete;
	status = usb_ep_queue(ep_iap_in, req, GFP_ATOMIC);
	if (status) {
		printk("iap: write in queue failed!\n");
	}
	//write_index++;
	IAP_DEBUG("iap: write ok!\n");
	return size;
}

static struct file_operations iap_fops = {
	.owner = THIS_MODULE,
	.open = iap_open,
	//.ioctl  =   iap_ioctl,
	.read = iap_read,
	.write = iap_write,
	.release = iap_release,
};

unsigned int buflen = 2048;
struct usb_request *iap_alloc_ep_req(struct usb_ep *ep)
{
	struct usb_request *req;

	req = usb_ep_alloc_request(ep, GFP_ATOMIC);
	if (!req)
		return req;
	//printk("iap_alloc_ep_req %x %s\n",ep->address,ep->name);
	if (0x09 == (ep->address & 0xff)) {	/*ep-out */
		req->length = buflen;
		req->buf = kmalloc(buflen, GFP_ATOMIC);
		if (!req->buf) {
			usb_ep_free_request(ep, req);
			req = NULL;
		}
	}
	return req;
}

void free_ep_req(struct usb_ep *ep, struct usb_request *req)
{
	kfree(req->buf);
	usb_ep_free_request(ep, req);
}

static void iap_complete(struct usb_ep *ep, struct usb_request *req)
{
	int status = req->status;
	IAP_DEBUG("%s %d status=%d\n", __FUNCTION__, ep->address & 0xff,
		  status);
	switch (status) {

	case 0:		/* normal completion? */
		if (0x09 == (ep->address & 0xff)) {	/*receive data */
			iap_rx[rx_index].buf = req->buf;
			iap_rx[rx_index].len = req->actual;
			wake_up(&(my_iap_dev.read_queue));
			req->buf = kmalloc(buflen, GFP_ATOMIC);
			req->actual = 0;
			if (!req->buf)
				return;	//-ENOMEM;
		} else {	/*send data */
			iap_tx[tx_index].len = 0;
			iap_tx[tx_index].buf = NULL;
			free_ep_req(ep, req);
			return;
#if 0
			if (iap_tx[tx_index + 1].len = 0) {	/*no data need sent */
				free_ep_req(ep, req);
				return;
			}

			req->buf = iap_tx[tx_index].buf;
			req->length = iap_tx[tx_index].len;
			req->actual = 0;
#endif
		}
		req->complete = iap_complete;
		break;
	case 0xff:		/*used to make /dev/iap */
		req->actual = 0;
		my_iap_dev.main_condition = DEV_ADD;
		iap_rx[rx_index].len = 0;
		req->status = 0;
		wake_up(&(my_iap_dev.main_queue));
		IAP_DEBUG("create /dev/iap\n");
		return;
		/* this endpoint is normally active while we're configured */
	case -ECONNABORTED:	/* hardware forced ep reset */
	case -ECONNRESET:	/* request dequeued */
	case -ESHUTDOWN:	/* disconnect from host */
		IAP_DEBUG("%s gone (%d), %d/%d\n", ep->name, status,
			  req->actual, req->length);
		if (0x09 == (ep->address & 0xff)) {
#if 0
			iap_rx[rx_index].len = 0;
			wake_up(&(my_iap_dev.read_queue));
			kfree(req->buf);
			free_ep_req(ep, req);
			/*remove iap dev node */
			my_iap_dev.main_condition = 1;
			wake_up(&(my_iap_dev.main_queue));
			printk("remove /dev/iap\n");
			return;
#else
			req->actual = 0;
			iap_rx[rx_index].len = ~0;
			wake_up(&(my_iap_dev.read_queue));
			my_iap_dev.main_condition = DEV_RM;
			ncm_intf = 0;
			//wake_up(&(my_iap_dev.main_queue));
			IAP_DEBUG("remove /dev/iap\n");
			return;
#endif
		} else {
			iap_tx[tx_index].len = 0;
			iap_tx[tx_index].buf = NULL;
			free_ep_req(ep, req);
			return;
		}
		req->complete = iap_complete;
		break;

	case -EOVERFLOW:	/* buffer overrun on read means that
				 * we didn't provide a big enough
				 * buffer.
				 */
	default:
#if 1
		IAP_DEBUG("%s complete --> %d, %d/%d\n", ep->name,
			  status, req->actual, req->length);
#endif
	case -EREMOTEIO:	/* short read */
		break;
	}
	IAP_DEBUG("****iAP_complete &&&&&\n");
	status = usb_ep_queue(ep, req, GFP_ATOMIC);
	if (status) {
		printk("kill %s:  resubmit %d bytes --> %d\n",
		       ep->name, req->length, status);
		usb_ep_set_halt(ep);
		/* FIXME recover later ... somehow */
	}

}

/*reveive ep used only */
static int iap_start_ep(bool is_in)
{
	struct usb_ep *ep;
	struct usb_request *req;
	int status;

	if (is_in) {
		printk("err:only reveive ep used only!");
		return 0;
	}

	ep = ep_iap_out;
	ep->driver_data = &my_iap_dev;
	req = iap_alloc_ep_req(ep);
	if (!req)
		return -ENOMEM;

	req->complete = iap_complete;
#if 1
	//printk("is_in = %d ep addr %x\n",is_in,ep->address);
	status = usb_ep_queue(ep, req, GFP_ATOMIC);
	if (status) {
		printk("start %s --> %d , %d %d\n", ep->name, status,
		       ep ? 11 : 0, ep->desc ? 11 : 0);
		free_ep_req(ep, req);
	}

	return status;
#else
	return 0;
#endif

}

int enable_iap(struct usb_ep *ep)
{
	int result = 0;

	result = iap_start_ep(false);
	if (result < 0) {
		usb_ep_disable(ep_iap_out);
		ep_iap_out->driver_data = NULL;
	}

	IAP_DEBUG("iap enabled\n");
	return result;
}

#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/dirent.h>
#include <linux/syscalls.h>
int iap_task_handle(void *data)
{
	while (1) {
		wait_event_interruptible(my_iap_dev.main_queue,
					 my_iap_dev.main_condition);
		printk("create = %d condition=%d\n",
		       ep_iap_out->driver_data ? 0 : 1,
		       my_iap_dev.main_condition);
		if (my_iap_dev.main_condition == DEV_RM
		    && (ep_iap_out->driver_data)) {
			//ncm_disable(carplay_f);
			if (carplay_f) {
				ncm_disable(carplay_f);
				carplay_f = NULL;
				printk("disconnect \n");
			}
			//gether_setup_iap(gadget_iap, hostaddr,"usb");
			//while(iap_rx[rx_index].len == ~0)
			//      msleep(1);
			device_destroy(iapClass, MKDEV(iap_major, 0));
			class_destroy(iapClass);
			printk("remove /dev/iap ok!!!\n");
			ep_iap_out->driver_data = NULL;
		} else if (!ep_iap_out->driver_data) {	//DEV_ADD
			iapClass = class_create(THIS_MODULE, "apple");
			device_create(iapClass, NULL, MKDEV(iap_major, 0), NULL,
				      "iap");
			//gether_setup_iap(gadget_iap, hostaddr,"apple_usb");
			printk("create /dev/iap ok\n");
			ep_iap_out->driver_data = (void *)1;
		}
		my_iap_dev.main_condition = 0;
	}
	return 0;
}

int iap_probe(void)
{
	int result;
	int devno;
	char dev_name[] = "iap";
	printk("%s %d \n", __FUNCTION__, __LINE__);
	result = alloc_chrdev_region(&dev, 0, 1, dev_name);
	if (result < 0) {
		printk(KERN_WARNING "iap: unable to get major %d\n", iap_major);
		return result;
	}
	iap_major = MAJOR(dev);
	devno = MKDEV(iap_major, 0);
	printk("%s %d \n", __FUNCTION__, __LINE__);
	cdev_init(&iapDevs, &iap_fops);
	iapDevs.owner = THIS_MODULE;
	iapDevs.ops = &iap_fops;
	printk("%s %d \n", __FUNCTION__, __LINE__);
	result = cdev_add(&iapDevs, devno, 1);
	printk("%s %d \n", __FUNCTION__, __LINE__);
	if (result) {
		printk(KERN_NOTICE "Error %d adding \n", result);
		goto err1;
	}
	printk("%s %d \n", __FUNCTION__, __LINE__);
	init_waitqueue_head(&(my_iap_dev.read_queue));
	init_waitqueue_head(&(my_iap_dev.main_queue));
	printk("%s %d \n", __FUNCTION__, __LINE__);
	ep_iap_out->driver_data = 0;
	/*auto create dev node */
#if 0
	iapClass = class_create(THIS_MODULE, dev_name);
	if (IS_ERR(iapClass)) {
		printk("Err: failed in create iap class\n");
		//return -1;
	}
	device_create(iapClass, NULL, MKDEV(iap_major, 0), NULL, dev_name);
#endif
	printk("%s %d \n", __FUNCTION__, __LINE__);
	my_iap_dev.iap_task =
	    kthread_create(iap_task_handle, NULL, "iapkthread");
	read_index = write_index = 0;
	iap_rx[write_index].len = 0;
	iap_tx[read_index].len = 0;
	printk("%s %d \n", __FUNCTION__, __LINE__);
	wake_up_process(my_iap_dev.iap_task);
	IAP_DEBUG("iap char dev add ok \n");
	return 0;
err1:
	printk("iap_probe err!\n");
	unregister_chrdev_region(dev, 1);
	return result;
}

void iap_exit(void)
{
	cdev_del(&iapDevs);
	device_destroy(iapClass, MKDEV(iap_major, 0));
	class_destroy(iapClass);
	unregister_chrdev_region(dev, 1);
	IAP_DEBUG("iap module removed !\n");
}

#endif

/*-------------------------------------------------------------------------*/

/* DO NOT REUSE THESE IDs with a protocol-incompatible driver!!  Ever!!
 * Instead:  allocate your own, using normal USB-IF procedures.
 */

/* Thanks to NetChip Technologies for donating this product ID.
 * It's for devices with only CDC Ethernet configurations.
 */
#if 1
#define CDC_VENDOR_NUM		0x0194	/* Apple */
#define CDC_PRODUCT_NUM		0x08e4	/* Apple 0x12 */
#else
#define CDC_VENDOR_NUM		0x0525	/* NetChip */
#define CDC_PRODUCT_NUM		0xa4a1	/* Linux-USB Ethernet Gadget */
#endif

/*-------------------------------------------------------------------------*/
#if 1
static struct usb_device_descriptor device_desc = {
	.bLength = sizeof device_desc,
	.bDescriptorType = USB_DT_DEVICE,

	.bcdUSB = cpu_to_le16(0x0200),

	.bDeviceClass = USB_CLASS_COMM,	//0xff,
	.bDeviceSubClass = 0,
	.bDeviceProtocol = 0,
	/* .bMaxPacketSize0 = f(hardware) */

	/* Vendor and product id defaults change according to what configs
	 * we support.  (As does bNumConfigurations.)  These values can
	 * also be overridden by module parameters.
	 */
	.idVendor = cpu_to_le16(CDC_VENDOR_NUM),
	.idProduct = cpu_to_le16(CDC_PRODUCT_NUM),
	/* .bcdDevice = f(hardware) */
	/* .iManufacturer = DYNAMIC */
	/* .iProduct = DYNAMIC */
	/* NO SERIAL NUMBER */
	.bNumConfigurations = 1,
};
#endif

static struct usb_otg_descriptor otg_descriptor = {
	.bLength = sizeof otg_descriptor,
	.bDescriptorType = USB_DT_OTG,

	/* REVISIT SRP-only hardware is possible, although
	 * it would not be called "OTG" ...
	 */
	.bmAttributes = USB_OTG_SRP | USB_OTG_HNP,
};

static const struct usb_descriptor_header *otg_desc[] = {
	(struct usb_descriptor_header *)&otg_descriptor,
	NULL,
};

#if 1

/* string IDs are assigned dynamically */

#define STRING_MANUFACTURER_IDX		0
#define STRING_PRODUCT_IDX		1

static char manufacturer[50];

static struct usb_string strings_dev[] = {
	[STRING_MANUFACTURER_IDX].s = manufacturer,
	[STRING_PRODUCT_IDX].s = DRIVER_DESC,
	{}			/* end of list */
};

static struct usb_gadget_strings stringtab_dev = {
	.language = 0x0409,	/* en-us */
	.strings = strings_dev,
};

static struct usb_gadget_strings *dev_strings[] = {
	&stringtab_dev,
	NULL,
};
#endif

/*-------------------------------------------------------------------------*/

static int __init ncm_do_config(struct usb_configuration *c)
{
	/* FIXME alloc iConfiguration string, set it in c->strings */

	if (gadget_is_otg(c->cdev->gadget)) {
		c->descriptors = otg_desc;
		c->bmAttributes |= USB_CONFIG_ATT_WAKEUP;
	}

	return ncm_bind_config(c, hostaddr);
}

static struct usb_configuration ncm_config_driver = {
	/* .label = f(hardware) */
	.label = "Carplay",
	.bConfigurationValue = 1,
	/* .iConfiguration = DYNAMIC */
	.bmAttributes = USB_CONFIG_ATT_SELFPOWER,
};

/*-------iap init -------------------------------------------------------------*/
void iap_bind(void)
{
	if (!gadget_iap) {
		printk("gadget_iap is NULL!\n");
	} else {
		ep_iap_in = find_ep(gadget_iap, "ep8in-bulk");
		ep_iap_out = find_ep(gadget_iap, "ep9out-bulk");

		ep_iap_in->desc = &iap_in_desc;
		ep_iap_out->desc = &iap_out_desc;
		ep_iap_in->address = 0x88;
		ep_iap_out->address = 0x09;
		ep_iap_in->maxpacket = 0x200;
		ep_iap_out->maxpacket = 0x200;
		enable_iap(ep_iap_out);
		printk("iap add \n");
	}
	return;
}

/*-------------------------------------------------------------------------*/
int carplay_bind(struct usb_composite_dev *cdev)
{
	int gcnum;
	struct usb_gadget *gadget = cdev->gadget;
	int status;
	/* set up network link layer */
	status = gether_setup(cdev->gadget, hostaddr);
	if (status < 0)
		return status;

	gcnum = usb_gadget_controller_number(gadget);
	if (gcnum >= 0)
		device_desc.bcdDevice = cpu_to_le16(0x0300 | gcnum);
	else {
		/* We assume that can_support_ecm() tells the truth;
		 * but if the controller isn't recognized at all then
		 * that assumption is a bit more likely to be wrong.
		 */
		dev_warn(&gadget->dev,
			 "controller '%s' not recognized; trying %s\n",
			 gadget->name, ncm_config_driver.label);
		device_desc.bcdDevice = cpu_to_le16(0x0300 | 0x0099);
	}

	device_desc.bcdDevice = cpu_to_le16(0x0200);

	/* Allocate string descriptor numbers ... note that string
	 * contents can be overridden by the composite_dev glue.
	 */

	/* device descriptor strings: manufacturer, product */
	/*snprintf(manufacturer, sizeof manufacturer, "%s %s with %s",
	   init_utsname()->sysname, init_utsname()->release,
	   gadget->name); */
	snprintf(manufacturer, sizeof manufacturer, "SunPlus");
	status = usb_string_id(cdev);
	if (status < 0)
		goto fail;
	strings_dev[STRING_MANUFACTURER_IDX].id = status;
	device_desc.iManufacturer = status;

	status = usb_string_id(cdev);
	if (status < 0)
		goto fail;
	strings_dev[STRING_PRODUCT_IDX].id = status;
	device_desc.iProduct = status;

	status = usb_add_config(cdev, &ncm_config_driver, ncm_do_config);
	if (status < 0)
		goto fail;

	dev_info(&gadget->dev, "%s\n", DRIVER_DESC);
	/*iap */
	iap_bind();
	return 0;

fail:
	gether_cleanup();
	return status;
}

#if 0
static int /*__exit*/ gncm_unbind(struct usb_composite_dev *cdev)
{
	gether_cleanup();
	return 0;
}
#endif
static struct usb_composite_driver carplay_driver = {
	.name = "g_ncm",
	.dev = &device_desc,
	.strings = dev_strings,
	.max_speed = USB_SPEED_HIGH,
	.unbind = __exit_p(gncm_unbind),
};

MODULE_DESCRIPTION(DRIVER_DESC);
MODULE_AUTHOR("Sunmedia");
MODULE_LICENSE("GPL");

static int /*__init*/ init_carplay(void)
{
	int ret;
	ret = usb_composite_probe(&carplay_driver, carplay_bind);
	if (ret < 0) {
		printk(KERN_WARNING "iap: unable to usb_composite_probe \n");
		return ret;
	}
	iap_probe();
	printk("name = %s\n", carplay_f->name);
	ncm_disable(carplay_f);
	return ret;
}

module_init(init_carplay);

static void /*__exit*/ cleanup_carplay(void)
{
	usb_composite_unregister(&carplay_driver);
	iap_exit();
}

module_exit(cleanup_carplay);
