#include <linux/module.h>
#include <linux/kthread.h>
#include <mach/hardware.h>
#include <linux/timer.h>
#include <linux/list.h>

#include <linux/usb.h>
#include <linux/usb/hcd.h>

#include <asm/io.h>

#include <mach/irqs.h>
#include <mach/hardware.h>
#include <mach/usb_power.h>


struct bc_hcd {

	int port_num;
	int irq;

	struct timer_list		bc_timer;
	struct task_struct		*bc_polling_thread;

	wait_queue_head_t		bc_queue;
	u32 					bc_flag;
};

enum bc_polling_state {
	BC_ST_IDLE,
	BC_ST_POLLING,
};

//#define bc_debug(fmt, args...)	printk("-- BC: "fmt, ##args)
#define bc_debug(fmt, args...)

#define BC_NUM			(1)
#define BC_POLLING_MS	(200)

static struct bc_hcd sphe8388_bc_hcd[2];

#define PRTBL_STS		(6)
#define VDM_SRC 		(5)
#define CDP_OFFSET		(0)

static u32 bc_val = 0; // ns (0 for disable)
module_param(bc_val, uint, 0644);

/*
* call the func when disconnect the port
*/
static void battery_charging_state_init(int port)
{
	volatile u32 *bc_cdp_addr;
	volatile u32 *bc_ref_addr;

	u32 val;

	bc_cdp_addr = (u32*)IO0_ADDRESS((149 + port) * 32 * 4) + 16;
	bc_ref_addr = (u32*)IO0_ADDRESS((149 + port) * 32 * 4) + 17;

	val = ioread32(bc_cdp_addr);
	val |= (3 << 3);
	val &= ~(1u << CDP_OFFSET);
	iowrite32(val, bc_cdp_addr);

	val = ioread32(bc_ref_addr);
	val = 0x92;
	iowrite32(val, bc_ref_addr);

	iowrite32(0x17, bc_cdp_addr - 16 + 3);
}

static void battery_charging_disinit(int port)
{
	volatile u32 *bc_cdp_addr;
	u32 val;

	bc_cdp_addr = (u32*)IO0_ADDRESS((149 + port) * 32 * 4) + 16;

	val = ioread32(bc_cdp_addr);
	val &= ~(1u << CDP_OFFSET);
	iowrite32( val, bc_cdp_addr);
}

#define	EHCI_STA_REG_PORT_0	(0x9C102164)
#define	EHCI_STA_REG_PORT_1	(0x9C103164)

#define	EHCI_STA_REG_UDC_0	(0x9C102b10)
#define	EHCI_STA_REG_UDC_1	(0x9C103b10)

static int is_udc_connect(int port)
{
	u32 *port_sta;
	u32 sta_val[2];

	if (BC_NUM < port) {
		printk("Port from 0~,Max is %d\n", BC_NUM -1);
		return 0;
	}

	if(port == 0)
		port_sta = ioremap_nocache(EHCI_STA_REG_UDC_0, 1);
	else
		port_sta = ioremap_nocache(EHCI_STA_REG_UDC_1, 1);

	if(!port_sta) {
		printk("bc ioremap fail\n");
		return -EBUSY;
	}

	sta_val[0] = ioread32(port_sta);
	msleep(3);
	sta_val[1] = ioread32(port_sta);

	iounmap(port_sta);

	if(sta_val[1] == sta_val[0])
		return 0;
	else
		return 1;
}

static int is_hw_connect(int port)
{
	u32 *port_sta;
	u32 sta_val;

	if (BC_NUM < port) {
		printk("Port from 0~,Max is %d\n", BC_NUM -1);
		return 0;
	}

	if(port == 0)
		port_sta = ioremap_nocache(EHCI_STA_REG_PORT_0, 1);
	else
		port_sta = ioremap_nocache(EHCI_STA_REG_PORT_1, 1);

	if(!port_sta) {
		printk("bc ioremap fail\n");
		return -EBUSY;
	}

	sta_val = ioread32(port_sta);
	iounmap(port_sta);

	//printk("port sta %x\n", sta_val);

	return sta_val&0x01;
}

/*Makesure BC status back to normal,  Call the func every 100 ms  */
static int battery_charging_reset(int port)
{
	volatile u32* bc_cdp_addr;
	u32 val;

	if (BC_NUM < port) {
		printk("Port from 0~,Max is %d\n", BC_NUM -1);
		return 0;
	}

	bc_cdp_addr = (u32*)IO0_ADDRESS((149 + port) * 32 * 4) + 16;
	/*disable & enable CDP to reset bc status*/
	val = ioread32(bc_cdp_addr);

	val &= ~(0x1);
	iowrite32(val, bc_cdp_addr);

	msleep(1);

	val |= (0x1);
	iowrite32(val, bc_cdp_addr);

	//otg_debug(KERN_DEBUG "BC fsm - %x\n", (ioread32(bc_cdp_addr - 4)&0x7));

	return 1;
}



static int battery_charging_enable(int port,int enable)
{
	volatile u32* bc_cdp_addr;
	u32 val;

	if (BC_NUM < port){
		printk("Port from 0~,Max is %d\n", BC_NUM -1);
		return 0;
	}

	bc_cdp_addr = (u32*)IO0_ADDRESS((149 + port) * 32 * 4) + 16;
	/*disable & enable CDP to reset bc status*/
	val = ioread32(bc_cdp_addr);

	if(enable)
		val |= (0x1);
	else
		val &= ~(0x1);

	iowrite32(val, bc_cdp_addr);

	return 1;
}

/* 0: have device  1: empty */
static int is_usb_bus_empty(struct usb_bus *bus)
{
	int i;

	if(!bus)
		return 1;

	for (i = 1; i < 128 / (8 * sizeof(unsigned long)); i++) {
		if (bus->devmap.devicemap[i]) {
			break;
		}
	}

	if (i < (128 / (8 * sizeof(unsigned long))))
		return 0;

	if (bus->devmap.devicemap[0] > 2)
		return 0;

	return 1;
}

/*
* 1:change 0:empty
*/
#ifdef CONFIG_USB_LOGO_TEST
static int	battery_charging_current_state(int port)
{
	volatile u32* bc_status_fsm;
	u32 val;

	if (BC_NUM < port){
		printk("Port from 0~,Max is %d\n", BC_NUM -1);
		return 0;
	}

	bc_status_fsm = (u32*)IO0_ADDRESS((149 + port) * 32 * 4);

	val = ioread32(bc_status_fsm + 12);

	if(((val&0x07) == 0x04) && (is_hw_connect(port) == 0) && (is_udc_connect(port) == 0)) {
		battery_charging_reset(port);
		val = ioread32(bc_status_fsm + 12);
	}

	if((val&0x07) == 0x03){
		bc_debug("-- BC[%d] change 0x%x\n", port, val&0x07);
		return 1;
	}
	else
		return 0;
}
#else
static int	battery_charging_current_state(int port)
{
	volatile u32* bc_status_fsm;
	u32 val;

	if (BC_NUM < port) {
		printk("Port from 0~,Max is %d\n", BC_NUM -1);
		return 0;
	}

	bc_status_fsm = (u32*)IO0_ADDRESS((149 + port) * 32 * 4);

	val = ioread32(bc_status_fsm + 12);
	//bc_debug("-- BC[%d] change 0x%x\n", port, val);

	if((val&0x07) >= 0x03) {
		bc_debug("-- BC[%d] change 0x%x\n", port, val&0x07);
		return 1;
	}
	else
		return 0;
}
#endif

#if 0
void vbus_ctrl(int en, int port)
{
	   volatile u32 *grop_base;
	   u32 val;

		printk("%s %d\n", __func__, __LINE__);

	   grop_base = (u32*)IO0_ADDRESS(2*32*4);
	   val = grop_base[3];

	   if(en == 0) {
			   /* vbus power down */
			   val &= ~(7 << (8*port + 4));
			   val |=  (3 << (8*port + 4));
			   grop_base[3] = val;
	   } else {
			   /* vbus power up  */
			   val &= ~(7 << (8*port + 4));
			   val |=  (7 << (8*port + 4));
			   grop_base[3] = val;
	   }
}
#else
void vbus_ctrl(int en, int port)
{
	int port_gpio;

	switch(port) {
	case 0:
		port_gpio = VBUS_GPIO_CTRL_0;
		break;
	case 1:
		port_gpio = VBUS_GPIO_CTRL_1;
		break;
	default:
		printk("BC: bad port number\n");
		return;
	}

	if(en == 0) {
		/* vbus power down */
		Set_usb_vbus(port_gpio, eHW_GPIO_STS_LOW);
	} else {
		/* vbus power up  */
		Set_usb_vbus(port_gpio, eHW_GPIO_STS_HIGH);
	}
}
#endif

static int cdp[2] = {1,1};

#ifdef CONFIG_USB_LOGO_TEST
static u32 bc_polling_enable = 1; // ns (0 for disable)
module_param(bc_polling_enable, uint, 0644);
#endif

static int bc_reset_thread(void *arg)
{
	struct bc_hcd *bc_hcd = (struct bc_hcd *)arg;

	struct usb_bus *bus;
	struct usb_bus *ehci_bus = NULL;
	struct usb_bus *ohci_bus = NULL;
	struct usb_hcd *hcd = NULL;

	int port;

	port = bc_hcd->port_num;

	battery_charging_reset(port);
	Reset_Usb_Power(port);

	do {
		if(cdp[port])
			battery_charging_enable(port, 1);

#ifdef CONFIG_USB_LOGO_TEST
		while(bc_polling_enable) {
			u32 ret;

			ret = battery_charging_current_state(port);
			if (cdp[port] && ret) {
				msleep(100);
				bc_hcd->bc_flag = 1;
				battery_charging_enable(port, 0);
				break;
			}

			msleep(5);
		}

		msleep(150);
		continue;
#endif
		wait_event_interruptible(bc_hcd->bc_queue,
								 (bc_hcd->bc_flag == 1)|| kthread_should_stop());

		if (kthread_should_stop()) {
			printk("%s is stoped!\n", __func__);
			break;
		}

		bc_hcd->bc_flag = 0;
		bc_debug("BC[%d] in\n", port);

		mutex_lock(&usb_bus_list_lock);
		list_for_each_entry(bus, &usb_bus_list, bus_list) {

			if(!bus)
				break;

			hcd = bus_to_hcd(bus);
			if(bc_hcd->irq == hcd->irq) {
				ehci_bus = bus;
				//printk("-(----Bc echi in irq %d %d\n", bc_hcd->irq, hcd->irq);
			}

			if(bc_hcd->irq == (hcd->irq - 1)) {
				ohci_bus = bus;
				//printk("-(----Bc ohci in irq %d %d\n", bc_hcd->irq, hcd->irq);
			}
		}
		mutex_unlock(&usb_bus_list_lock);

		if(!ehci_bus || !ohci_bus) {
			printk("BC[%d]: no usb bus\n", port);
			battery_charging_reset(port);
			mod_timer(&bc_hcd->bc_timer, (HZ/10) + jiffies);
			continue;
		}

		do {
			msleep(3000);

			if((ehci_bus->iphone_host == 1) || (ohci_bus->iphone_host == 1)) {
				bc_debug("-- BC[%d] android/iphone in\n", port);
				break;
			}

			if(is_usb_bus_empty(ehci_bus) == 0 || is_usb_bus_empty(ohci_bus) == 0)
				break;

			/* if bc fail , reset BC & restart usb device */
			battery_charging_reset(port);
			Reset_Usb_Power(port);
			msleep(300);

			bc_debug("BC[%d] Enum usb dev fail, and retry\n", port);
			break;
		} while(1);

		if((ehci_bus->iphone_host == 1) || (ohci_bus->iphone_host == 1)) {

			do {
				msleep(300);
			} while(is_hw_connect(port) ||  is_udc_connect(port));

			bc_debug("BC[%d] phone disc\n", port);

			ehci_bus->iphone_host = 0;
			ohci_bus->iphone_host = 0;
			battery_charging_reset(port);
			mod_timer(&bc_hcd->bc_timer, (HZ/10) + jiffies);
			continue;
		}

		while (1) {

			/* scan all EHCI with BC */
			/* if roothub have device ,then check it */
			if((is_usb_bus_empty(ehci_bus) == 1) && (is_usb_bus_empty(ohci_bus) == 1))
				break;

			msleep(BC_POLLING_MS);
		}
		bc_debug("BC[%d]usb dev out\n", port);

		battery_charging_reset(port);
		mod_timer(&bc_hcd->bc_timer, (HZ/10) + jiffies);
	}while(!kthread_should_stop());

	return 0;
}

static void bc_polling_status_change(unsigned long param)
{
	struct bc_hcd *bc_hcd = (struct bc_hcd *)(param);
	int ret;

	ret = battery_charging_current_state(bc_hcd->port_num);
	if (ret) {
		/* start check host enum thread */
		/* Tell the thread to start working */
		bc_hcd->bc_flag = 1;
		wake_up_interruptible(&bc_hcd->bc_queue);
	} else {
		mod_timer(&bc_hcd->bc_timer, (HZ/60) + jiffies);
	}
}

static u32 bc_switch = 0;

static ssize_t show_bc_switch(struct module_attribute *mattr,
				 struct module_kobject *mk, char *buffer)
{
	return 0;
}

static ssize_t store_bc_switch(struct module_attribute *mattr,
				struct module_kobject *mk,
				const char *buffer, size_t count)
{
	volatile u32 *uphy;

	if (kstrtouint(buffer, 0, &bc_switch) == 0) {
		printk("bc switch %x %x\n", bc_switch,(bc_switch >> 8)&0xff);

		uphy = (u32*)IO0_ADDRESS((149 + (bc_switch&0x1)) * 32 * 4);

		switch((bc_switch >> 8)&0xff) {
		case 0:		//cdp
			printk("Enable port %d CDP\n", bc_switch&0x1);
			cdp[bc_switch&0x1] = 1;
			iowrite32(0x92, uphy+16);
			iowrite32(0x01, uphy+15);
			break;
		case 1:		//dcp
			printk("Enable port %d DCP\n", bc_switch&0x1);
			cdp[bc_switch&0x1] = 0;
			iowrite32(0x0, uphy+15);
			iowrite32(0x93, uphy+16);
			break;
		case 2:		//scp
			cdp[bc_switch&0x1] = 0;
			printk("Enable port %d SDP\n", bc_switch&0x1);
			iowrite32(0x0, uphy+15);
			iowrite32(0x0, uphy+16);
			break;
		default:
			break;
		}
	}

	return count;
}

static struct module_attribute bc_switch_enable =
	__ATTR(bc_switch, 0644, show_bc_switch, store_bc_switch);

static int __init sphe8388_bc_init(void)
{
	struct bc_hcd *bc_hcd = NULL;
	int i;

	struct kobject *kobj;

	for(i = 0;i < 2;i++) {

		printk("Init bc controller %d\n", i);
		bc_hcd = &sphe8388_bc_hcd[i];

		bc_hcd->port_num = i;
		bc_hcd->irq = SP_IRQ_EHCI_USB0 + i*3;
		printk("Bc init irq %d\n", bc_hcd->irq);

		bc_hcd->bc_polling_thread = kthread_create(bc_reset_thread,
															bc_hcd,
													 "bc_kthread"
													);
		if (IS_ERR(bc_hcd->bc_polling_thread)) {
			pr_err("Create BC(%d) thread fail!\n", i);
			//return err;
		}

		bc_hcd->bc_flag = 0;
		init_waitqueue_head(&bc_hcd->bc_queue);

		init_timer(&bc_hcd->bc_timer);
		bc_hcd->bc_timer.function = bc_polling_status_change;
		bc_hcd->bc_timer.data = (unsigned long)(bc_hcd);

		battery_charging_state_init(i);

		wake_up_process(bc_hcd->bc_polling_thread);

#ifndef CONFIG_USB_LOGO_TEST
		bc_hcd->bc_timer.expires = (HZ/10) + jiffies;
		add_timer(&bc_hcd->bc_timer);
#endif
	}


	kobj = kset_find_obj(module_kset, "sphe8388_bc");
	sysfs_create_file(kobj, &bc_switch_enable.attr);

	return 0;
}

static void __exit sphe8388_bc_cleanup(void)
{
	int i;
	struct bc_hcd *bc_hcd = NULL;

	for(i = 0;i < 2;i++){

		bc_hcd = &sphe8388_bc_hcd[i];

		battery_charging_disinit(i);
		kthread_stop(bc_hcd->bc_polling_thread);
		del_timer(&bc_hcd->bc_timer);
	}
}

module_init(sphe8388_bc_init);
module_exit(sphe8388_bc_cleanup);

MODULE_AUTHOR("Yaong yong.y@sunmeida.com");
MODULE_LICENSE("GPL");
