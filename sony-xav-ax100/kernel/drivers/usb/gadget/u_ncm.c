/*
 * File Name : u_ncm.c
 *
 * ncm utilities for composite USB gadgets.
 * This utilitie can support to connect head unit for mirror link
 *
 * Copyright (C) 2011 Samsung Electronics
 * Author: SoonYong, Cho <soonyong.cho@samsung.com>
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */
#define IAP
void iap_disc(void);

#include "f_ncm.c"
#include "ncm.h"

/* Support dynamic tethering mode.
 * if ncm_connect is true, device is received vendor specific request
 * from head unit.
 */
static bool ncm_connect;

/* terminal version using vendor specific request */
u16 terminal_mode_version;
u16 terminal_mode_vendor_id;

struct ncm_function_config {
	u8 ethaddr[ETH_ALEN];
};

static int ncm_function_init(struct android_usb_function *f,
			     struct usb_composite_dev *cdev)
{
	printk("%s %d \n", __FUNCTION__, __LINE__);
	f->config = kzalloc(sizeof(struct ncm_function_config), GFP_KERNEL);
	return 0;
}

static void ncm_function_cleanup(struct android_usb_function *f)
{
	printk("%s %d \n", __FUNCTION__, __LINE__);

	kfree(f->config);
	f->config = NULL;
}

static int ncm_function_bind_config(struct android_usb_function *f,
				    struct usb_configuration *c)
{
	int ret;
	int i;
	char *src;
	struct ncm_function_config *ncm = f->config;

	if (!ncm) {
		pr_err("%s: ncm_pdata\n", __func__);
		return -1;
	}

	ncm = f->config;
	if (!f->config)
		return -ENOMEM;

	for (i = 0; i < ETH_ALEN; i++)
		ncm->ethaddr[i] = 0;
	/* create a fake MAC address from our serial number.
	 * first byte is 0x02 to signify locally administered.
	 */
	ncm->ethaddr[0] = 0x02;
	src = serial_string;
	for (i = 0; (i < 256) && *src; i++) {
		/* XOR the USB serial across the remaining bytes */
		ncm->ethaddr[i % (ETH_ALEN - 1) + 1] ^= *src++;
	}

	printk(KERN_DEBUG "usb: %s MAC:%02X:%02X:%02X:%02X:%02X:%02X\n",
	       __func__, ncm->ethaddr[0], ncm->ethaddr[1],
	       ncm->ethaddr[2], ncm->ethaddr[3], ncm->ethaddr[4],
	       ncm->ethaddr[5]);

	printk(KERN_DEBUG "usb: %s before MAC:%02X:%02X:%02X:%02X:%02X:%02X\n",
	       __func__, ncm->ethaddr[0], ncm->ethaddr[1],
	       ncm->ethaddr[2], ncm->ethaddr[3], ncm->ethaddr[4],
	       ncm->ethaddr[5]);
	/* we have to use trick.
	 * rndis name will be used for ethernet interface name.
	 */
	ret = gether_setup_name(c->cdev->gadget, ncm->ethaddr, "apple_usb");
	printk(KERN_DEBUG "usb: %s after MAC:%02X:%02X:%02X:%02X:%02X:%02X\n",
	       __func__, ncm->ethaddr[0], ncm->ethaddr[1],
	       ncm->ethaddr[2], ncm->ethaddr[3], ncm->ethaddr[4],
	       ncm->ethaddr[5]);
	if (ret) {
		pr_err("%s: gether_setup failed\n", __func__);
		return ret;
	}
	printk("%s %d \n", __FUNCTION__, __LINE__);
	ret = ncm_bind_config(c, ncm->ethaddr);
	if (ret) {
		pr_err("%s: ncm_bind_config failed\n", __func__);
	} else

		iap_bind();

	iap_probe();
	printk("name = %s\n", carplay_f->name);
	//nswitch = 0;
	//ncm_disable(carplay_f);
	return ret;

}

static void ncm_function_unbind_config(struct android_usb_function *f,
				       struct usb_configuration *c)
{
	printk("%s %d \n", __FUNCTION__, __LINE__);
	//nswitch = 1;

	gether_cleanup();
	iap_exit();
}

static struct android_usb_function ncm_function = {
	.name = "ncm",
	.init = ncm_function_init,
	.cleanup = ncm_function_cleanup,
	.bind_config = ncm_function_bind_config,
	.unbind_config = ncm_function_unbind_config,
};

bool is_ncm_ready(char *name)
{
	/* Enable ncm function */
	if (!strcmp(name, "rndis") || !strcmp(name, "ncm")) {
		if (ncm_connect) {
			printk(KERN_DEBUG "usb: %s ncm ready (%s)\n",
			       __func__, name);
			return true;
		}
	}
	return false;
}

void set_ncm_device_descriptor(struct usb_device_descriptor *desc)
{
	desc->idProduct = 0x685d;
	desc->bDeviceClass = USB_CLASS_COMM;
	printk(KERN_DEBUG "usb: %s idProduct=0x%x, DeviceClass=0x%x\n",
	       __func__, desc->idProduct, desc->bDeviceClass);

}

void set_ncm_ready(bool ready)
{
	if (ready != ncm_connect)
		printk(KERN_DEBUG "usb: %s old status=%d, new status=%d\n",
		       __func__, ncm_connect, ready);
	ncm_connect = ready;
	if (ready == false) {
		terminal_mode_version = 0;
		terminal_mode_vendor_id = 0;
	}
}
