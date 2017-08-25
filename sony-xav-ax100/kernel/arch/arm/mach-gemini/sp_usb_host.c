/*
 * (C) Copyright 2014
 * Sunplus Technology
 * yong.y <yong.y@sunmedia.com.cn>
 *
 * SPDX-License-Identifier:     GPL-2.0+
 */
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/dma-mapping.h>

//#include <mach/hal/regmap/reg_usb_host.h>
#include <mach/irqs.h>
#include <mach/hardware.h>
#include <linux/delay.h>

#include <mach/gpio_drv.h>
#include <mach/usb_power.h>

#include <linux/usb/ehci_pdriver.h>
#include <linux/usb/ohci_pdriver.h>
#include <linux/usb/sphe8388_uphy.h>

//#include <mach/sp_customize_config.h>

#define USB_HOST_NUM		2

#define USB_HOST_ID_1		1
#define USB_HOST_ID_2		2
#define USB_HOST_ID_3		3

#ifdef CONFIG_USB_USE_PLATFORM_RESOURCE

#define letb123_HE_REG_BASE	(0x9c102000)

#define EHCI_USB0_GRP 			(0)
#define EHCI_USB1_GRP 			(1)
#define EHCI_USB2_GRP 			(2)

#define OHCI_USB0_GRP 			(0)
#define OHCI_USB1_GRP 			(1)
#define OHCI_USB2_GRP 			(2)

#define OTG_GRP_OFFSET			(8)

#define letb123_IRQ_EHCI_USB0  SP_IRQ_EHCI_USB0
#define letb123_IRQ_EHCI_USB1  SP_IRQ_EHCI_USB1
#define letb123_IRQ_EHCI_USB2  SP_IRQ_EHCI_USB2

#define letb123_IRQ_OHCI_USB0  SP_IRQ_OHCI_USB0
#define letb123_IRQ_OHCI_USB1  SP_IRQ_OHCI_USB1
#define letb123_IRQ_OHCI_USB2  SP_IRQ_OHCI_USB2

#define letb123_IRQ_OTG0  SP_IRQ_OTG_0
#define letb123_IRQ_OTG1  SP_IRQ_OTG_1

#endif

/*EHCI REGS*/
static struct usb_ehci_pdata usb_ehci_pdata = {
};

/*EHCI 0*/
static struct resource usb_ehci_res_0[] = {

#ifdef CONFIG_USB_USE_PLATFORM_RESOURCE
	[0] = {
		.start  = letb123_HE_REG_BASE + EHCI_USB0_GRP*0x1000 + 2*32*4,
		.end    = letb123_HE_REG_BASE + EHCI_USB0_GRP*0x1000 + 3*32*4 -1,
		.flags  = IORESOURCE_MEM,
	},
#else
	[0] = {
		.start  = LOGI_ADDR_EHCI_USB0_REG,
		.end    = LOGI_ADDR_EHCI_USB0_REG + 32*4 - 1,
		.flags  = IORESOURCE_MEM,
	},
#endif
	[1] = {
		.start  = letb123_IRQ_EHCI_USB0,
		.end    = letb123_IRQ_EHCI_USB0,
		.flags  = IORESOURCE_IRQ,
	},
};

/*EHCI 1*/
static struct resource usb_ehci_res_1[] = {

#ifdef CONFIG_USB_USE_PLATFORM_RESOURCE
	[0] = {
		.start  = letb123_HE_REG_BASE + EHCI_USB1_GRP*0x1000 + 2*32*4,
		.end    = letb123_HE_REG_BASE + EHCI_USB1_GRP*0x1000 + 3*32*4 - 1,
		.flags  = IORESOURCE_MEM,
	},
#else
	[0] = {
		.start  = LOGI_ADDR_EHCI_USB1_REG,
		.end    = LOGI_ADDR_EHCI_USB1_REG + 32*4 - 1,
		.flags  = IORESOURCE_MEM,
	},
#endif
	[1] = {
		.start  = letb123_IRQ_EHCI_USB1,
		.end    = letb123_IRQ_EHCI_USB1,
		.flags  = IORESOURCE_IRQ,
	},
};

/*EHCI 2*/
static struct resource usb_ehci_res_2[] = {

#ifdef CONFIG_USB_USE_PLATFORM_RESOURCE
	[0] = {
		.start  = letb123_HE_REG_BASE + EHCI_USB2_GRP*0x1000 + 2*32*4,
		.end    = letb123_HE_REG_BASE + EHCI_USB2_GRP*0x1000 + 3*32*4 - 1,
		.flags  = IORESOURCE_MEM,
	},
#else
	[0] = {
		.start  = LOGI_ADDR_EHCI_USB2_REG,
		.end    = LOGI_ADDR_EHCI_USB2_REG + 32*4 - 1,
		.flags  = IORESOURCE_MEM,
	},
#endif
	[1] = {
		.start  = letb123_IRQ_EHCI_USB2,
		.end    = letb123_IRQ_EHCI_USB2,
		.flags  = IORESOURCE_IRQ,
	},
};

struct platform_device usb_ehci_device[] = {
	[0] = {
		.name		= "ehci-platform",
		.id			= USB_HOST_ID_1,
		.dev		= {
			.dma_mask			= &usb_ehci_device[0].dev.coherent_dma_mask,
			.coherent_dma_mask	= DMA_BIT_MASK(32),
			.platform_data		= &usb_ehci_pdata,
		},

		.num_resources  = ARRAY_SIZE(usb_ehci_res_0),
		.resource       = usb_ehci_res_0,
	},

	[1] = {
		.name		= "ehci-platform",
		.id			= USB_HOST_ID_2,
		.dev		= {
			.dma_mask			= &usb_ehci_device[1].dev.coherent_dma_mask,
			.coherent_dma_mask	= DMA_BIT_MASK(32),
			.platform_data		= &usb_ehci_pdata,
		},

		.num_resources  = ARRAY_SIZE(usb_ehci_res_1),
		.resource       = usb_ehci_res_1,
	},

	[2] = {
		.name		= "ehci-platform",
		.id			= USB_HOST_ID_3,
		.dev		= {
			.dma_mask			= &usb_ehci_device[2].dev.coherent_dma_mask,
			.coherent_dma_mask	= DMA_BIT_MASK(32),
			.platform_data		= &usb_ehci_pdata,
		},

		.num_resources  = ARRAY_SIZE(usb_ehci_res_2),
		.resource       = usb_ehci_res_2,
	},
};

/*OHCI REGS*/
static struct usb_ohci_pdata usb_ohci_pdata = {
};

/*OHCI 0*/
static struct resource usb_ohci_res_0[] = {

#ifdef CONFIG_USB_USE_PLATFORM_RESOURCE
	[0] = {
		.start  = letb123_HE_REG_BASE + OHCI_USB0_GRP*0x1000 + 1*32*4,
		.end    = letb123_HE_REG_BASE + OHCI_USB0_GRP*0x1000 + 2*32*4 - 1,
		.flags  = IORESOURCE_MEM,
	},
#else
	[0] = {
		.start  = LOGI_ADDR_OHCI_USB0_REG,
		.end    = LOGI_ADDR_OHCI_USB0_REG + 32*4 - 1,
		.flags  = IORESOURCE_MEM,
	},
#endif
	[1] = {
		.start  = letb123_IRQ_OHCI_USB0,
		.end    = letb123_IRQ_OHCI_USB0,
		.flags  = IORESOURCE_IRQ,
	},
};

/*OHCI 1*/
static struct resource usb_ohci_res_1[] = {

#ifdef CONFIG_USB_USE_PLATFORM_RESOURCE
	[0] = {
		.start  = letb123_HE_REG_BASE + OHCI_USB1_GRP*0x1000 + 1*32*4,
		.end    = letb123_HE_REG_BASE + OHCI_USB1_GRP*0x1000 + 2*32*4 - 1,
		.flags  = IORESOURCE_MEM,
	},
#else
	[0] = {
		.start  = LOGI_ADDR_OHCI_USB1_REG,
		.end    = LOGI_ADDR_OHCI_USB1_REG + 32*4 - 1,
		.flags  = IORESOURCE_MEM,
	},
#endif
	[1] = {
		.start  = letb123_IRQ_OHCI_USB1,
		.end    = letb123_IRQ_OHCI_USB1,
		.flags  = IORESOURCE_IRQ,
	},
};

/*OHCI 2*/
static struct resource usb_ohci_res_2[] = {

#ifdef CONFIG_USB_USE_PLATFORM_RESOURCE
	[0] = {
		.start  = letb123_HE_REG_BASE + OHCI_USB2_GRP*0x1000 + 1*32*4,
		.end    = letb123_HE_REG_BASE + OHCI_USB2_GRP*0x1000 + 2*32*4 - 1,
		.flags  = IORESOURCE_MEM,
	},
#else
	[0] = {
		.start  = LOGI_ADDR_OHCI_USB2_REG,
		.end    = LOGI_ADDR_OHCI_USB2_REG + 32*4 - 1,
		.flags  = IORESOURCE_MEM,
	},
#endif
	[1] = {
		.start  = letb123_IRQ_OHCI_USB2,
		.end    = letb123_IRQ_OHCI_USB2,
		.flags  = IORESOURCE_IRQ,
	},
};

struct platform_device usb_ohci_device[] = {

	[0] = {
		.name		= "ohci-platform",
		.id			= USB_HOST_ID_1,
		.dev		= {
			.dma_mask			= &usb_ohci_device[0].dev.coherent_dma_mask,
			.coherent_dma_mask	= DMA_BIT_MASK(32),
			.platform_data		= &usb_ohci_pdata,
		},

		.num_resources  = ARRAY_SIZE(usb_ohci_res_0),
		.resource       = usb_ohci_res_0,
	},

	[1] = {
		.name		= "ohci-platform",
		.id			= USB_HOST_ID_2,
		.dev		= {
			.dma_mask			= &usb_ohci_device[1].dev.coherent_dma_mask,
			.coherent_dma_mask	= DMA_BIT_MASK(32),
			.platform_data		= &usb_ohci_pdata,
		},

		.num_resources  = ARRAY_SIZE(usb_ohci_res_1),
		.resource       = usb_ohci_res_1,
	},

	[2] = {
		.name		= "ohci-platform",
		.id			= USB_HOST_ID_3,
		.dev		= {
			.dma_mask			= &usb_ohci_device[2].dev.coherent_dma_mask,
			.coherent_dma_mask	= DMA_BIT_MASK(32),
			.platform_data		= &usb_ohci_pdata,
		},

		.num_resources  = ARRAY_SIZE(usb_ohci_res_2),
		.resource       = usb_ohci_res_2,
	},
};


#ifdef CONFIG_USB_letb8388_OTG

static struct resource usb_otg_res0[] = {

	[0] = {
		.start  = letb123_HE_REG_BASE + EHCI_USB0_GRP*0x1000 + OTG_GRP_OFFSET*32*4,
		.end    = letb123_HE_REG_BASE + EHCI_USB0_GRP*0x1000 + (OTG_GRP_OFFSET+1)*32*4 - 1,
		.flags  = IORESOURCE_MEM,
	},

	[1] = {
		.start  = letb123_IRQ_OTG0,
		.end    = letb123_IRQ_OTG0,
		.flags  = IORESOURCE_IRQ,
	},
};

static struct resource usb_otg_res1[] = {

	[0] = {
		.start  = letb123_HE_REG_BASE + EHCI_USB1_GRP*0x1000 + OTG_GRP_OFFSET*32*4,
		.end    = letb123_HE_REG_BASE + EHCI_USB1_GRP*0x1000 + (OTG_GRP_OFFSET+1)*32*4 - 1,
		.flags  = IORESOURCE_MEM,
	},

	[1] = {
		.start  = letb123_IRQ_OTG1,
		.end    = letb123_IRQ_OTG1,
		.flags  = IORESOURCE_IRQ,
	},
};

struct platform_device usb_otg_device[] = {

	[0] = {
		.name		= "sphe8388-otg",
		.id			= USB_HOST_ID_1,
		.dev		= {
			.dma_mask			= &usb_otg_device[0].dev.coherent_dma_mask,
			.coherent_dma_mask	= DMA_BIT_MASK(32),
			.platform_data		= &usb_ohci_pdata,
		},

		.num_resources  = ARRAY_SIZE(usb_otg_res0),
		.resource       = usb_otg_res0,
	},

	[1] = {
		.name		= "sphe8388-otg",
		.id			= USB_HOST_ID_2,
		.dev		= {
			.dma_mask			= &usb_otg_device[1].dev.coherent_dma_mask,
			.coherent_dma_mask	= DMA_BIT_MASK(32),
			.platform_data		= &usb_ohci_pdata,
		},

		.num_resources  = ARRAY_SIZE(usb_otg_res1),
		.resource       = usb_otg_res1,
	},
};
#endif

static ssize_t show_usb_power(struct device *dev, struct device_attribute *attr, char *buf)
{
	//struct platform_device *pdev = to_platform_device(dev);

	return 0;
}

static ssize_t store_usb_power(struct device *dev, struct device_attribute *attr,
							   						  const char *buf,size_t count)
{
	struct platform_device *pdev = to_platform_device(dev);
	u32 usb_power;
	int port;

	if (kstrtouint(buf, 0, &usb_power) == 0) {

		printk(KERN_DEBUG "usb power set %x\n", usb_power);

		port = pdev->id - 1;
		if(usb_power&VBUS_RESET) {
			Disable_vbus_power(port);
			uphy_force_disc(1, port);
			msleep(500);
			uphy_force_disc(0, port);
			Enable_vbus_power(port);
		}
		else {
			/* VBUS 0:Disable 1:Enable */
			if(usb_power&VBUS_POWER_MASK) {
				uphy_force_disc(0, port);
				Enable_vbus_power(port);
			}
			else {
				Disable_vbus_power(port);
				uphy_force_disc(1, port);
			}
		}
	}

	return count;
}

static DEVICE_ATTR(usb_power_set, S_IWUSR | S_IRUSR, show_usb_power, store_usb_power);

#ifdef CONFIG_letb123_CUSTOMIZE_CONFIG
char *usb_name[] = {"USB0Enable", "USB1Enable", "USB2Enable"};
#endif

void uphy_init(void)
{
	volatile u32 *grop_base = (u32*)IO0_ADDRESS(1*32*4);
//	volatile u32 *usbc_ptr;
	u32 val;

	// 1. Default value modification
	grop_base[14] = 0x87474002;
	grop_base[15] = 0x87474004;
	grop_base[18] = 0x87474006;

	// 2. PLL power off/on twice
	grop_base[21] = 0x8888;
	msleep(1);
	grop_base[21] = 0x8080;
	msleep(1);
	grop_base[21] = 0x8888;
	msleep(1);
	grop_base[21] = 0x8080;
	msleep(1);
	grop_base[21] = 0;

	// 3. reset UPHY0/1
	grop_base = (u32*)IO0_ADDRESS(0*32*4);

	grop_base[18] |= (3 << 13);
	grop_base[18] &= ~(3 << 13);
	msleep(1);

	// 4. UPHY 0/1 internal register modification
	grop_base = (u32*)IO0_ADDRESS(149*32*4);
	grop_base[7] = 0x8b;

	val = ioread32(grop_base + 9);
	val |= (1 << 6);
	iowrite32(val, grop_base + 9);

	val = ioread32(grop_base + 1);
	val &= ~(1 << 5);
	iowrite32(val, grop_base + 1);

	grop_base = (u32*)IO0_ADDRESS(150*32*4);
	grop_base[7] = 0x8b;

	val = ioread32(grop_base + 9);
	val |= (1 << 6);
	iowrite32(val, grop_base + 9);

	val = ioread32(grop_base + 1);
	val &= ~(1 << 5);
	iowrite32(val, grop_base + 1);

	// 5. USBC 0/1 reset
	grop_base = (u32*)IO0_ADDRESS(0*32*4);
	grop_base[18] |= (3 << 10);
	grop_base[18] &= ~(3 << 10);

	/* port 0 uphy clk fix */
	grop_base = (u32*)IO0_ADDRESS(1*32*4);
	//grop_base[19] |= (1 << 6);
	grop_base[19] |= (1 << 6)|(1 << 14)|(1 << 22);

	/* 6. Power on usbc 0/1 */
	grop_base = (u32*)IO0_ADDRESS(1*32*4);
	grop_base[8] |= (3<<2);

#ifdef CONFIG_USB_BC
	grop_base = (u32*)IO0_ADDRESS(2*32*4);
	val = grop_base[3];

	/* vbus power down */
	val &= ~((7 << 12)|(7 << 4));
	val |= (3 << 12)|(3 << 4);
	grop_base[3] = val;

	msleep(100);

	/* vbus power up */
	val &= ~((7 << 12)|(7 << 4));
	val |= (7 << 12)|(7 << 4);
	grop_base[3] = val;
#endif

#ifdef CONFIG_USB_LOGO_TEST
	Set_uphy_swing(0x8a, 1);
#endif

#ifdef CONFIG_USB_letb8388_OTG
	grop_base = (u32*)IO0_ADDRESS(2*32*4);
	grop_base[3] &= ~((1 << 12) | (1 << 4));

	msleep(1);
#endif

}

static int __init sunplus_usb_host_init(void)
{
	int i,j;
	int ret;

#ifdef CONFIG_letb123_CUSTOMIZE_CONFIG
	int isUSB = 0;
#endif

	uphy_init();

#ifdef CONFIG_USB_letb8388_OTG
	for(i = 0;i < USB_HOST_NUM-1;i++){
		ret = platform_device_register(&usb_otg_device[i]);
		if(ret){
			pr_err("Register usb otg device[%d] fail !!!\n", i);
			return -1;
		}
	}
#endif

	/*EHCI REGISTER DEVICE*/
	for(i = 0;i < USB_HOST_NUM;i++){

#ifdef CONFIG_letb123_CUSTOMIZE_CONFIG
		if(getConfigInt(usb_name[i],&isUSB)){
			printk("GET USB-E-%d Fail !\n",i);
			isUSB = 0;
			continue;
		}

		if(isUSB == 0)
			continue;
#endif
		ret = platform_device_register(&usb_ehci_device[i]);
		if(ret){
			pr_err("Register ehci usb host device[%d] fail !!!\n",i);
			goto EHCI_FAIL;
		}

		if(i <= 1) {
			device_create_file(&usb_ehci_device[i].dev, &dev_attr_usb_power_set);
		}
	}

	/*OHCI REGISTER DEVICE*/
	for(j = 0;j < USB_HOST_NUM;j++){

#ifdef CONFIG_letb123_CUSTOMIZE_CONFIG
		if(getConfigInt(usb_name[j],&isUSB)){
			printk("GET USB-O-%d Fail !\n",j);
			isUSB = 0;
			continue;
		}

		if(isUSB == 0)
			continue;
#endif
		ret = platform_device_register(&usb_ohci_device[j]);
		if(ret){
			pr_err("Register ohci usb host device[%d] fail !!!\n",j);
			goto OHCI_FAIL;
		}
	}

	return 0;

OHCI_FAIL:
	for(--j;j >= 0;j--){

#ifdef CONFIG_letb123_CUSTOMIZE_CONFIG
		if(getConfigInt(usb_name[j],&isUSB)){
			printk("GET USB-O-%d Fail !\n",j);
			isUSB = 0;
			continue;
		}

		if(isUSB == 0)
			continue;
#endif
		platform_device_unregister(&usb_ohci_device[j]);
	}

EHCI_FAIL:
	for(--i;i >= 0;i--){

#ifdef CONFIG_letb123_CUSTOMIZE_CONFIG
		if(getConfigInt(usb_name[i],&isUSB)){
			printk("GET USB-E-%d Fail !\n",i);
			isUSB = 0;
			continue;
		}

		if(isUSB == 0)
			continue;
#endif
		platform_device_unregister(&usb_ehci_device[i]);
	}
	return ret;
}

struct workqueue_struct		*usb_init_queue;
struct delayed_work			delay_work;

static void usb_delay_init_uphy(struct work_struct *delay_work)
{
	sunplus_usb_host_init();
}

static int __init sunplus_usb_host_delay_init(void)
{
	usb_init_queue = create_singlethread_workqueue("usb_delay_init");
	if(!usb_init_queue) {
		printk("Usb init DELAY queue fail\n");
	} else {
		INIT_DELAYED_WORK(&delay_work, usb_delay_init_uphy);
		queue_delayed_work(usb_init_queue, &delay_work, 0);
	}

	return 0;
}

static void __exit sp_rtc_exit(void)
{
	int i;
#ifdef CONFIG_letb123_CUSTOMIZE_CONFIG
	int isUSB = 0;
#endif

	for(i = 0;i < USB_HOST_NUM;i++){
#ifdef CONFIG_letb123_CUSTOMIZE_CONFIG

		if(getConfigInt(usb_name[i],&isUSB)){
			printk("GET USB-E-%d Fail !\n",i);
			isUSB = 0;
			continue;
		}

		if(isUSB == 0)
			continue;
#endif
		if (i == 1) {
			device_remove_file(&usb_ehci_device[i].dev, &dev_attr_usb_power_set);
		}

		platform_device_unregister(&usb_ehci_device[i]);
		platform_device_unregister(&usb_ohci_device[i]);
	}

#ifdef CONFIG_USB_letb8388_OTG
	for(i = 0;i < USB_HOST_NUM-1;i++){
		platform_device_unregister(&usb_otg_device[i]);
	}
#endif

	flush_workqueue(usb_init_queue);
	destroy_workqueue(usb_init_queue);
	usb_init_queue = NULL;
}

module_init(sunplus_usb_host_delay_init);
module_exit(sp_rtc_exit);

MODULE_AUTHOR("yong.y@sunmedia.com.cn");
MODULE_DESCRIPTION("Sunplus usb host driver/device");
MODULE_LICENSE("GPL");
