/*
 * drivers/usb/generic.c - generic driver for USB devices (not interfaces)
 *
 * (C) Copyright 2005 Greg Kroah-Hartman <gregkh@suse.de>
 *
 * based on drivers/usb/usb.c which had the following copyrights:
 *	(C) Copyright Linus Torvalds 1999
 *	(C) Copyright Johannes Erdfelt 1999-2001
 *	(C) Copyright Andreas Gal 1999
 *	(C) Copyright Gregory P. Smith 1999
 *	(C) Copyright Deti Fliegl 1999 (new USB architecture)
 *	(C) Copyright Randy Dunlap 2000
 *	(C) Copyright David Brownell 2000-2004
 *	(C) Copyright Yggdrasil Computing, Inc. 2000
 *		(usb_device_id matching changes by Adam J. Richter)
 *	(C) Copyright Greg Kroah-Hartman 2002-2003
 *
 */

#include <linux/usb.h>
#include <linux/usb/hcd.h>
#include <linux/usb/otg.h>
#include "usb.h"

static inline const char *plural(int n)
{
	return (n == 1 ? "" : "s");
}

static int is_rndis(struct usb_interface_descriptor *desc)
{
	return desc->bInterfaceClass == USB_CLASS_COMM
		&& desc->bInterfaceSubClass == 2
		&& desc->bInterfaceProtocol == 0xff;
}

static int is_activesync(struct usb_interface_descriptor *desc)
{
	return desc->bInterfaceClass == USB_CLASS_MISC
		&& desc->bInterfaceSubClass == 1
		&& desc->bInterfaceProtocol == 1;
}

int usb_choose_configuration(struct usb_device *udev)
{
	int i;
	int num_configs;
	int insufficient_power = 0;
	struct usb_host_config *c, *best;

	best = NULL;
	c = udev->config;
	num_configs = udev->descriptor.bNumConfigurations;

	printk(KERN_ALERT"usb_choose configuration in  vid = 0x%x speed = %d (3:HS)\n",udev->descriptor.idVendor,udev->speed);
	if(udev->descriptor.idVendor == 0x5ac)  //shengjun add for apple ipod audio function config
	{
		int apple_config;
		apple_config = udev->config[0].desc.bConfigurationValue;
		for(i = 0; i < udev->descriptor.bNumConfigurations ; i++)
		{
			struct usb_interface_descriptor *desc;
			desc = &udev->config[i].intf_cache[0]->altsetting->desc;
			if(desc->bInterfaceClass == USB_CLASS_AUDIO){
				apple_config = udev->config[i].desc.bConfigurationValue;
				break;
			}
			apple_config = udev->config[i].desc.bConfigurationValue;
		}
		return apple_config;
	}

	for (i = 0; i < num_configs; (i++, c++)) {
		struct usb_interface_descriptor	*desc = NULL;

		/* It's possible that a config has no interfaces! */
		if (c->desc.bNumInterfaces > 0)
			desc = &c->intf_cache[0]->altsetting->desc;

		/*
		 * HP's USB bus-powered keyboard has only one configuration
		 * and it claims to be self-powered; other devices may have
		 * similar errors in their descriptors.  If the next test
		 * were allowed to execute, such configurations would always
		 * be rejected and the devices would not work as expected.
		 * In the meantime, we run the risk of selecting a config
		 * that requires external power at a time when that power
		 * isn't available.  It seems to be the lesser of two evils.
		 *
		 * Bugzilla #6448 reports a device that appears to crash
		 * when it receives a GET_DEVICE_STATUS request!  We don't
		 * have any other way to tell whether a device is self-powered,
		 * but since we don't use that information anywhere but here,
		 * the call has been removed.
		 *
		 * Maybe the GET_DEVICE_STATUS call and the test below can
		 * be reinstated when device firmwares become more reliable.
		 * Don't hold your breath.
		 */
#if 0
		/* Rule out self-powered configs for a bus-powered device */
		if (bus_powered && (c->desc.bmAttributes &
					USB_CONFIG_ATT_SELFPOWER))
			continue;
#endif

		/*
		 * The next test may not be as effective as it should be.
		 * Some hubs have errors in their descriptor, claiming
		 * to be self-powered when they are really bus-powered.
		 * We will overestimate the amount of current such hubs
		 * make available for each port.
		 *
		 * This is a fairly benign sort of failure.  It won't
		 * cause us to reject configurations that we should have
		 * accepted.
		 */

		/* Rule out configs that draw too much bus current */
		if (c->desc.bMaxPower * 2 > udev->bus_mA) {
			insufficient_power++;
			continue;
		}

		/* When the first config's first interface is one of Microsoft's
		 * pet nonstandard Ethernet-over-USB protocols, ignore it unless
		 * this kernel has enabled the necessary host side driver.
		 * But: Don't ignore it if it's the only config.
		 */
		if (i == 0 && num_configs > 1 && desc &&
				(is_rndis(desc) || is_activesync(desc))) {
#if !defined(CONFIG_USB_NET_RNDIS_HOST) && !defined(CONFIG_USB_NET_RNDIS_HOST_MODULE)
			continue;
#else
			best = c;
#endif
		}

		/* From the remaining configs, choose the first one whose
		 * first interface is for a non-vendor-specific class.
		 * Reason: Linux is more likely to have a class driver
		 * than a vendor-specific driver. */
		else if (udev->descriptor.bDeviceClass !=
						USB_CLASS_VENDOR_SPEC &&
				(desc && desc->bInterfaceClass !=
						USB_CLASS_VENDOR_SPEC)) {
			best = c;
			break;
		}

		/* If all the remaining configs are vendor-specific,
		 * choose the first one. */
		else if (!best)
			best = c;
	}

	if (insufficient_power > 0)
		dev_info(&udev->dev, "rejected %d configuration%s "
			"due to insufficient available bus power\n",
			insufficient_power, plural(insufficient_power));

	if (best) {
		i = best->desc.bConfigurationValue;
		dev_dbg(&udev->dev,
			"configuration #%d chosen from %d choice%s\n",
			i, num_configs, plural(num_configs));
	} else {
		i = -1;
		dev_warn(&udev->dev,
			"no configuration chosen from %d choice%s\n",
			num_configs, plural(num_configs));
	}
	return i;
}

#define CONFIG_AOA_SUPPORT
#define CONFIG_SUPPORT_APPLE_ROLE_SWITCH

#ifdef CONFIG_SUPPORT_APPLE_ROLE_SWITCH

#include <linux/platform_device.h>
#include <linux/module.h>
#include <mach/hardware.h>
#include <mach/irqs.h>
#include <linux/io.h>
#include <mach/usb_power.h>

#define APPLE_USB_HOST_MODE	1
#define APPLE_USB_DEV_MODE	0

static u32 apple_dev_default_mode = APPLE_USB_DEV_MODE;
module_param(apple_dev_default_mode, uint, 0644);

#define ADDR_USBCTRL_REG		 (0x9c00010c)

#ifdef CONFIG_USB_GADGET_8388
extern void detech_start(int a);
#endif

static void SWITCH_TO_CARPLAY(void)
{
	u32 __iomem *reg_usb_ctrl = NULL;
	u32 value;

	reg_usb_ctrl = (u32*)ioremap(ADDR_USBCTRL_REG, 0x4);

	value = ioread32(reg_usb_ctrl);
#ifdef CONFIG_GADGET_USB0
	value &= ~(7 << 4);
	value |= (1u << 6)|(0u << 5)|(1u << 4);
#else
	value &= ~(7 << 12);
	value |= (1u << 14)|(0u << 13)|(1u << 12);
#endif
	iowrite32(value, reg_usb_ctrl);

	printk("Enter apple usb host mode %x\n", value);

	iounmap(reg_usb_ctrl);
}

static int usb_carplay_configuration(struct usb_device *dev)
{
	int ret;
	struct usb_hcd *hcd;
#ifdef CONFIG_USB_letb8388_OTG
	struct usb_phy *otg_phy;
#endif

	hcd = bus_to_hcd(dev->bus);

	if (dev->descriptor.idVendor != 0x5ac ){
		printk("Not a apple device\n");
		return 1;
	}

#ifdef CONFIG_GADGET_USB0
	if((hcd->irq != SP_IRQ_EHCI_USB0) && (hcd->irq != SP_IRQ_OHCI_USB0)) {
		printk("Apple Only support by host 0\n");
		return 1;
	}
#else
	if((hcd->irq != SP_IRQ_EHCI_USB1) && (hcd->irq != SP_IRQ_OHCI_USB1)) {
		printk("Apple Only support by host 1\n");
		return 1;
	}
#endif

	if(dev->parent != dev->bus->root_hub){
		printk("Not in roothub\n");
		return 1;
	}

	if (!(dev->descriptor.idProduct>>8) == 0x12 ){
		printk("This apple device not support host mode\n");
		return 1;
	}

#ifdef CONFIG_USB_letb8388_OTG
	otg_phy = usb_get_transceiver_8388(dev->bus->busnum-1);
	if(!otg_phy){
		printk("Get otg control fail(busnum:%d)!\n", dev->bus->busnum);
		return 1;
	}
	otg_start_hnp(otg_phy->otg);
#endif

	//USB Role switch
	ret = usb_control_msg(dev, usb_sndctrlpipe(dev, 0), 0x51, 0x40,		\
								cpu_to_be16(0x0100), cpu_to_be16(0), NULL, 0, 1000);
	if(ret < 0){
		return ret;
	}

#ifndef CONFIG_USB_letb8388_OTG
	/*SWITCH TO DEVICE*/
	SWITCH_TO_CARPLAY();

#ifdef CONFIG_USB_GADGET_8388
#ifndef	CONFIG_USB_HOST_RESET_SP
	detech_start(1);
#endif
#endif

#endif

	return 0;
}
#endif

#ifdef CONFIG_AOA_SUPPORT

#include <linux/usb/linux-adk.h>
#include <linux/module.h>


#define ANDROID_DEF_MODE	0
#define ANDROID_AOA_MODE	1

static u32 android_default_mode = ANDROID_AOA_MODE;
module_param(android_default_mode, uint, 0644);

static u32 android_interface_check = 1;
module_param(android_interface_check, uint, 0644);

#ifdef CONFIG_USB_HOST_RESET_SP
extern void Usb_dev_power_reset(struct usb_device *udev,int delayms);

static int  is_aa_cp_device(struct usb_device *udev)
{
	struct usb_host_config *c;

	c = udev->config;
	if (NULL == c){
#ifdef CONFIG_USB_HOST_RESET_SP
		printk("\n\n [USB RECOVER]udev->config=NULL ,enum fail retry enum \n");
		/*ehci deamon thread  do */
#endif
		/*not valid device,may get error,normal probe*/
		return 0;
	}
	if (c->desc.bNumInterfaces > 1){
		return 1;/*not msdc/hub only*/
	}

	return 0;
}

#endif
static int is_in_accessory_mode(struct usb_aoa* pdev)
{
	static struct timespec t0, t1, d;
	#define POWER_RESET_TIME 1300
	struct usb_device *udev;
	struct usb_hcd *hcd;

	int vid = pdev->vendor_id;
	int pid = pdev->product_id;

	u8 usb_class;

	udev = pdev->udev;

	/* Skip standard usb class device and full/low speed */
	if(udev->descriptor.bDeviceClass != USB_CLASS_PER_INTERFACE ||
						udev->speed != USB_SPEED_HIGH) {
		if (udev->speed == USB_SPEED_FULL){/* udisk already filter b4 here*/
			if (is_aa_cp_device(udev)){
				getnstimeofday(&t1);
				d = timespec_sub(t1, t0);
				printk(KERN_DEBUG "d.tv_sec=%ld\n",d.tv_sec);
				if (d.tv_sec < 3){
					printk("Same device to FS, not HS, not retry\n");
					return 1;
				}
				printk(KERN_DEBUG "\n\n [@OHCI] reset PORT power!\n");
				#ifdef CONFIG_USB_HOST_RESET_SP
				Usb_dev_power_reset(udev,POWER_RESET_TIME);
				#endif
				getnstimeofday(&t0);
			}
		}
		return 1;
	}

	/* Only support EHCI */
	hcd = bus_to_hcd(udev->bus);
	if(hcd->irq != SP_IRQ_EHCI_USB0 && hcd->irq != SP_IRQ_EHCI_USB1 &&
								hcd->irq != SP_IRQ_EHCI_USB2) {
		return 1;
	}

	/* Skip apple device */
	if(vid == 0x05ac)
		return 1;

	/* Skip standard usb interface class */
	if((android_interface_check == 1) && udev->config->intf_cache[0]) {
		usb_class = udev->config->intf_cache[0]->altsetting->desc.bInterfaceClass;
		if((usb_class != USB_SUBCLASS_VENDOR_SPEC) && (usb_class != USB_CLASS_COMM) &&
						 (usb_class != USB_CLASS_STILL_IMAGE))
			return 1;
	}

	if(vid == AOA_ACCESSORY_VID && (pid == AOA_ACCESSORY_PID || pid == AOA_ACCESSORY_ADB_PID
	  || pid == AOA_AUDIO_PID || pid == AOA_AUDIO_ADB_PID || pid == AOA_ACCESSORY_AUDIO_PID
	  || pid == AOA_ACCESSORY_AUDIO_ADB_PID))
		return 1;
	else
		return 0;
}

static int aoa_get_version(struct usb_aoa* dev)
{
	int ret;
	int version;
	int timeout = 500;
	int value = 0;
	int index = 0;

	char ver_buf[2];//two byte data to support versions > 255 in future

	ret = usb_control_msg(dev->udev, usb_rcvctrlpipe(dev->udev, 0),
					      AOA_GET_PROTOCOL,USB_TYPE_VENDOR | USB_DIR_IN,
				          value, index, ver_buf, sizeof(ver_buf), timeout);
	if(ret < 0) {
		printk("AOA::error in getting protocol\n");
		return -1;
	}

	version = (ver_buf[1] << 8) | ver_buf[0];

	return version;
}

static int aoa_send_identity(struct usb_aoa* dev,int ident_id,void* data,int size)
{
	int ret;
	int timeout = 500;
	int value = 0;
	int index = ident_id;
	int request = AOA_SEND_IDENT;

	ret = usb_control_msg(dev->udev, usb_sndctrlpipe(dev->udev, 0),
							request, USB_TYPE_VENDOR | USB_DIR_OUT,
							value, index, data, size, timeout);
	return ret;
}

static int aoa_start_mode(struct usb_aoa* dev,int aoa_mode,int value)
{
	int ret;
	int timeout= 500;
	int index=0;

	ret = usb_control_msg(dev->udev, usb_sndctrlpipe(dev->udev,0),
                  		aoa_mode, USB_TYPE_VENDOR | USB_DIR_OUT, value,
                 		index, NULL, 0,timeout);

	if(ret < 0)
		printk("AOA::error in starting acessory/audio\n");

	return ret;
}

static int aoa_init_accessory(struct usb_aoa* dev,const accessory_t* acc)
{
	int ret,ver;

	//get protocol(51)
	ver = aoa_get_version(dev);
	if(ver <= 0)
		goto err;

	printk("AOA::Device supports version no.%d.0!\n",ver);
	dev->aoa_version = ver;

	//send identity-Manufacturer(52)
	ret=aoa_send_identity(dev,AOA_STRING_MAN_ID,
			acc->manufacturer,strlen(acc->manufacturer) + 1);
	if(ret<0) goto err;

	//send identity-ModelName(52)
	ret=aoa_send_identity(dev,AOA_STRING_MOD_ID,acc->model,strlen(acc->model) + 1);
	if(ret<0) goto err;

	//send identity-Description(52)
	ret=aoa_send_identity(dev,AOA_STRING_DSC_ID,
			acc->description,strlen(acc->description) + 1);
	if(ret<0) goto err;

	//send identity-version(52)
	ret=aoa_send_identity(dev,AOA_STRING_VER_ID,acc->version,strlen(acc->version) + 1);
	if(ret<0) goto err;

	//send identity-url(52)
	ret=aoa_send_identity(dev,AOA_STRING_URL_ID,acc->url,strlen(acc->url) + 1);
	if(ret<0) goto err;

	//send identity-serial no.(52)
	ret=aoa_send_identity(dev,AOA_STRING_SER_ID,acc->serial,strlen(acc->serial) + 1);
	if(ret<0) goto err;

#if 0
	if(dev->aoa_version >=2 && (acc->mode & AOA_AUDIO_MODE)) {
		printk("ADKSKEL::requesting audio support from the device\n");
		ret=aoa_start_mode(dev,AOA_AUDIO_SUPPORT,1); //request audio support
		if(ret<0) printk("ADKSKEL::failed to get audio support\n");
	}
#endif

	printk("AOA::starting device in accessory mode\n");
	ret=aoa_start_mode(dev,AOA_START_ACCESSORY,1); // start in accessory mode
	if(ret<0) goto err;
	printk("AOA::Accessory got initialized successfully\n");

	return 0;
err:
	//printk("ADKSKEL::Accessory initialization got failed\n");
	return -EINVAL;
}

#endif
static int is_normal_cfg(struct usb_device *udev)
{
	int i;
	int num_configs;
	int insufficient_power = 0;
	struct usb_host_config *c;

	c = udev->config;
	if (NULL == c){
#ifdef CONFIG_USB_HOST_RESET_SP
		printk("\n\n [USB RECOVER]udev->config=NULL ,enum fail retry enum \n");
		/*ehci deamon thread  do */
#endif
		/*not valid device,may get error,normal probe*/
		return 1;
	}
	num_configs = udev->descriptor.bNumConfigurations;

	#if 0
	printk("DeviceClass=%x subc=%x\n",udev->descriptor.bDeviceClass,
						udev->descriptor.bDeviceSubClass);
	printk("num_configs=%d bNumInterfaces=%d\n",num_configs,c->desc.bNumInterfaces);
	#endif
	if (num_configs > 1){
		return 0;/*not msdc/hub only*/
	}
	if (c->desc.bNumInterfaces > 1){
		return 0;/*not msdc/hub only*/
	}
	for (i = 0; i < num_configs; (i++, c++)) {
		struct usb_interface_descriptor *desc = NULL;

		/* It's possible that a config has no interfaces! */
		if (c->desc.bNumInterfaces > 0)
			desc = &c->intf_cache[0]->altsetting->desc;

		if (c->desc.bMaxPower * 2 > udev->bus_mA) {
			insufficient_power++;
			continue;
		}
		//printk("desc=%p\n",desc);
		if (NULL == desc){
			continue;
		}
		/*printk("bInterfaceClass=%x bInterfaceSubClass=%x\n",
				desc->bInterfaceClass,desc->bInterfaceSubClass);*/

		/*MSDC*/
		if ( (desc->bInterfaceClass == USB_CLASS_MASS_STORAGE)
				   &&(desc->bInterfaceSubClass == 6)){
			printk("\t Find Mass storate interface\n");
			return 1;
		}

		/*HUB*/
		if ( (desc->bInterfaceClass == USB_CLASS_HUB)
				   &&(desc->bInterfaceSubClass == 0)){
			//printk("\t Find HUB interface\n");
			return 1;
		}
	}

	return 0;
}

static int generic_probe(struct usb_device *udev)
{
	int err, c;
#ifdef CONFIG_AOA_SUPPORT
	struct usb_aoa aoa_dev;
#endif
	/*Some MSDC device will stall,when use AOA get version cmd*/
	if(is_normal_cfg(udev)){
		printk("None AA/CP config found ... not try AA or cp \n");
		goto normal_probe;
	}

#ifdef CONFIG_AOA_SUPPORT
	aoa_dev.udev = udev;
	aoa_dev.product_id = udev->descriptor.idProduct;
	aoa_dev.vendor_id = udev->descriptor.idVendor;

	if((android_default_mode == ANDROID_AOA_MODE)&& !is_in_accessory_mode(&aoa_dev)){
		if(aoa_init_accessory(&aoa_dev, &acc_default) == 0){
			printk("Usb geric: AOA mode starting!");

#ifdef	CONFIG_USB_BC
			udev->bus->iphone_host = 1;
#endif
			return 0;
		}
	}
#endif

#ifdef CONFIG_SUPPORT_APPLE_ROLE_SWITCH
	if((apple_dev_default_mode == APPLE_USB_HOST_MODE) && (usb_carplay_configuration(udev) == 0)){
		printk("Usb geric: Carplay mode starting!");

#ifdef	CONFIG_USB_BC
		udev->bus->iphone_host = 1;
#endif
		return 0;
	}
#endif

normal_probe:

	/* Choose and set the configuration.  This registers the interfaces
	 * with the driver core and lets interface drivers bind to them.
	 */
	if (usb_device_is_owned(udev))
		;		/* Don't configure if the device is owned */
	else if (udev->authorized == 0)
		dev_err(&udev->dev, "Device is not authorized for usage\n");
	else {
		c = usb_choose_configuration(udev);
		if (c >= 0) {
			err = usb_set_configuration(udev, c);
			if (err) {
				dev_err(&udev->dev, "can't set config #%d, error %d\n",
					c, err);
				/* This need not be fatal.  The user can try to
				 * set other configurations. */
			}
		}
	}
	/* USB device state == configured ... usable */
	usb_notify_add_device(udev);

	return 0;
}

static void generic_disconnect(struct usb_device *udev)
{
	usb_notify_remove_device(udev);

	/* if this is only an unbind, not a physical disconnect, then
	 * unconfigure the device */
	if (udev->actconfig)
		usb_set_configuration(udev, -1);
}

#ifdef	CONFIG_PM

static int generic_suspend(struct usb_device *udev, pm_message_t msg)
{
	int rc;

	/* Normal USB devices suspend through their upstream port.
	 * Root hubs don't have upstream ports to suspend,
	 * so we have to shut down their downstream HC-to-USB
	 * interfaces manually by doing a bus (or "global") suspend.
	 */
	if (!udev->parent)
		rc = hcd_bus_suspend(udev, msg);

	/* Non-root devices don't need to do anything for FREEZE or PRETHAW */
	else if (msg.event == PM_EVENT_FREEZE || msg.event == PM_EVENT_PRETHAW)
		rc = 0;
	else
		rc = usb_port_suspend(udev, msg);

	return rc;
}

static int generic_resume(struct usb_device *udev, pm_message_t msg)
{
	int rc;

	/* Normal USB devices resume/reset through their upstream port.
	 * Root hubs don't have upstream ports to resume or reset,
	 * so we have to start up their downstream HC-to-USB
	 * interfaces manually by doing a bus (or "global") resume.
	 */
	if (!udev->parent)
		rc = hcd_bus_resume(udev, msg);
	else
		rc = usb_port_resume(udev, msg);
	return rc;
}

#endif	/* CONFIG_PM */

struct usb_device_driver usb_generic_driver = {
	.name =	"usb",
	.probe = generic_probe,
	.disconnect = generic_disconnect,
#ifdef	CONFIG_PM
	.suspend = generic_suspend,
	.resume = generic_resume,
#endif
	.supports_autosuspend = 1,
};
