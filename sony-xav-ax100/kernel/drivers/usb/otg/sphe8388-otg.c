#include <linux/init.h>
#include <linux/module.h>
#include <linux/io.h>
#include <linux/slab.h>
#include <linux/interrupt.h>
#include <linux/usb.h>

#include <linux/platform_device.h>
#include "sphe8388-otg.h"

#define DRIVER_NAME		"sphe8388-otg"

struct sphe8388_otg *sphe_otg_host = NULL;

static int sphe8388_start_srp(struct usb_otg *otg)
{
	return 0;
}

static int sphe8388_start_hnp(struct usb_otg *otg)
{
	struct sphe8388_otg *otg_host = (struct sphe8388_otg *)container_of(otg->phy, 
																   struct sphe8388_otg, otg);
	u32 ret;

	ret = ioread32(&otg_host->regs->otg_device_ctrl);
	ret |= A_SET_B_HNP_EN_BIT;
	ret &= ~A_BUS_REQ_BIT;
	iowrite32(ret, &otg_host->regs->otg_device_ctrl);

	otg_host->otg.state = OTG_STATE_A_PERIPHERAL;

	otg_debug("start hnp\n");

	return 0;
}

static int sphe8388_set_vbus(struct usb_otg *otg, bool enabled)
{
	struct sphe8388_otg *otg_host = (struct sphe8388_otg *)container_of(otg->phy,
																   struct sphe8388_otg, otg);
	u32 ret;
	u32 id_pin;

	ret = ioread32(&otg_host->regs->otg_device_ctrl);

	if (enabled) {
		otg_debug("set vbus\n");
		ret |= A_BUS_REQ_BIT;
	} else {
		otg_debug("drop vbus\n");
		ret |= A_BUS_DROP_BIT;

#ifdef	CONFIG_ADP_TIMER
		mod_timer(&otg_host->adp_timer, ADP_TIMER_FREQ + jiffies);
#endif

		id_pin = ioread32(&otg_host->regs->otg_int_st);
		if ((id_pin & ID_PIN) == 0)
			otg_host->otg.state = OTG_STATE_A_WAIT_VFALL;
	}

	iowrite32(ret, &otg_host->regs->otg_device_ctrl);

	return 0;
}

int sphe8388_set_host(struct usb_otg *otg, struct usb_bus *host)
{
	otg->host = host;

	return 0;
}

int sphe8388_set_peripheral(struct usb_otg *otg, struct usb_gadget *gadget)
{
	otg->gadget = gadget;

	return 0;
}

int sphe8388_phy_read(struct usb_phy *x, u32 reg)
{
	return ioread32((u32 *) reg);
}
int sphe8388_phy_write(struct usb_phy *x, u32 val, u32 reg)
{
	iowrite32(val, (u32 *) reg);
	return 0;
}

struct usb_phy_io_ops sphe8388_phy_ios = {
	.read = sphe8388_phy_read,
	.write = sphe8388_phy_write,
};

static void sphe8388_otg_work(struct work_struct *work)
{
	struct sphe8388_otg *otg_host;
	u32 val;

	otg_host = (struct sphe8388_otg *)container_of(work, struct sphe8388_otg,
													work);

	switch (otg_host->otg.state) {
	case OTG_STATE_UNDEFINED:
	case OTG_STATE_B_IDLE:
		break;
	case OTG_STATE_B_SRP_INIT:
		break;
	case OTG_STATE_B_PERIPHERAL:
		break;
	case OTG_STATE_B_WAIT_ACON:
		break;
	case OTG_STATE_B_HOST:
		break;
	case OTG_STATE_A_IDLE:
		otg_debug("FSM: OTG_STATE_A_IDLE\n");

		if (otg_host->fsm.id == 0) {

			val = ioread32(&otg_host->regs->otg_device_ctrl);
			val |= A_BUS_REQ_BIT;
			iowrite32(val, &otg_host->regs->otg_device_ctrl);
		}

		otg_host->otg.state = OTG_STATE_A_WAIT_VRISE;

		break;
	case OTG_STATE_A_WAIT_VRISE:
		break;
	case OTG_STATE_A_WAIT_BCON:
		break;
	case OTG_STATE_A_HOST:
		break;
	case OTG_STATE_A_SUSPEND:
		break;
	case OTG_STATE_A_PERIPHERAL:
		break;
	case OTG_STATE_A_VBUS_ERR:
		otg_debug("FSM: OTG_STATE_A_VBUS_ERR\n");

		if (otg_host->fsm.id == 0) {
			msleep(100);
			val = ioread32(&otg_host->regs->otg_device_ctrl);
			val |= A_BUS_REQ_BIT;
			iowrite32(val, &otg_host->regs->otg_device_ctrl);
		}
		otg_host->otg.state = OTG_STATE_A_WAIT_VRISE;

		break;
	case OTG_STATE_A_WAIT_VFALL:
		break;
	default:
		break;
	}
}

/* host/client notify transceiver when event affects HNP state */
void sphe_otg_update_transceiver(struct sphe8388_otg *otg_host)
{
	if (unlikely(!otg_host || !otg_host->qwork))
		return;

	queue_work(otg_host->qwork, &otg_host->work);
}

EXPORT_SYMBOL(sphe_otg_update_transceiver);

static void otg_hw_init(struct sphe8388_otg *otg_host)
{
	u32 val;

	/* Set adp charge precision */
	iowrite32(0x3f, &otg_host->regs->adp_chng_precision);

	/* Set adp dis-charge time */
	//iowrite32(0x898, &otg_host->regs->adp_chrg_time);

	/* Set wait power rise timer */
	iowrite32(0x1ffff, &otg_host->regs->a_wait_vrise_tmr);

	/* Set wait b-connect time  */
	iowrite32(0x1fffff, &otg_host->regs->a_wait_bcon_tmr);

	/* Enbale ADP&SRP  */
	val = ioread32(&otg_host->regs->otg_int_st);
	if (val & ID_PIN)
		iowrite32(~OTG_SIM & (OTG_SRP | OTG_20),
			  &otg_host->regs->mode_select);
	else
		iowrite32(~OTG_SIM & (OTG_ADP | OTG_SRP | OTG_20),
			  &otg_host->regs->mode_select);
}

static void otg_hsm_init(struct sphe8388_otg *otg_host)
{
	int val32;

	val32 = ioread32(&otg_host->regs->otg_int_st);

	/* set init state */
	if (val32 & ID_PIN) {
		otg_debug("Init is B device\n");
		otg_host->fsm.id = 1;
		otg_host->otg.otg->default_a = 0;
		otg_host->otg.state = OTG_STATE_B_IDLE;
	} else {
		otg_debug("Init is A device\n");
		otg_host->fsm.id = 0;
		otg_host->otg.otg->default_a = 1;
		otg_host->otg.state = OTG_STATE_A_IDLE;
	}

	otg_host->fsm.drv_vbus = 0;
	otg_host->fsm.loc_conn = 0;
	otg_host->fsm.loc_sof = 0;

	/* defautly power the bus */
	otg_host->fsm.a_bus_req = 0;
	otg_host->fsm.a_bus_drop = 0;
	/* defautly don't request bus as B device */
	otg_host->fsm.b_bus_req = 0;
	/* no system error */
	otg_host->fsm.a_clr_err = 0;

}

int sphe8388_notifier_call(struct notifier_block *self, unsigned long action,
			   void *dev)
{
	struct sphe8388_otg *otg_host = (struct sphe8388_otg *)container_of(self, struct sphe8388_otg, notifier);

	struct usb_device *udev = (struct usb_device *)dev;
	u32 val;

	otg_debug("%p %p\n", udev->bus, otg_host->otg.otg->host);

	if (udev->bus != otg_host->otg.otg->host)
		return 0;

	if (udev->parent != udev->bus->root_hub)
		return 0;

	if ((action == USB_DEVICE_REMOVE)
	    && (otg_host->otg.state != OTG_STATE_A_PERIPHERAL)) {
		otg_debug(" usb device remove\n");
		val = ioread32(&otg_host->regs->otg_device_ctrl);
		val |= A_BUS_DROP_BIT;
		iowrite32(val, &otg_host->regs->otg_device_ctrl);

#ifdef	CONFIG_ADP_TIMER
		mod_timer(&otg_host->adp_timer, ADP_TIMER_FREQ + jiffies);
#endif
	}

	return 0;
}

#ifdef	CONFIG_ADP_TIMER
static void adp_watchdog(unsigned long param)
{
	struct sphe8388_otg *otg_host = (struct sphe8388_otg *)param;
	u32 val;

	otg_debug("adp timer %d\n", otg_host->irq);

	/* request a-bus by iphone */
	val = ioread32(&otg_host->regs->otg_device_ctrl);
	val |= A_BUS_REQ_BIT;
	iowrite32(val, &otg_host->regs->otg_device_ctrl);
}
#endif

static irqreturn_t otg_irq(int irq, void *dev_priv)
{
	struct sphe8388_otg *otg_host = (struct sphe8388_otg *)dev_priv;
	u32 int_status;
	u32 val;

//	struct timespec	t0;
//	getnstimeofday(&t0);

	otg_debug(" otg irq in \n");

	/* clear interrupt status */
	int_status = ioread32(&otg_host->regs->otg_int_st);
	iowrite32((int_status & INT_MASK), &otg_host->regs->otg_int_st);

	if (int_status & ADP_CHANGE_IF) {
		otg_debug("ADP_CHANGE_IF %d %x %x\n", irq,
			  ioread32(&otg_host->regs->otg_debug_reg)
			  , ioread32(&otg_host->regs->adp_debug_reg));
		otg_host->fsm.adp_change = 1;
		val = ioread32(&otg_host->regs->otg_device_ctrl);
		val |= A_BUS_REQ_BIT;
		iowrite32(val, &otg_host->regs->otg_device_ctrl);

		//otg_debug("adp in %09ld.%09ld\n", t0.tv_sec, t0.tv_nsec);

		//otg_host->fsm.id = (int_status&ID_PIN) ? 1 : 0;
		//otg_host->otg.state = OTG_STATE_A_IDLE;
	}

	if (int_status & A_BIDL_ADIS_IF) {
		otg_debug("A_BIDL_ADIS_IF\n");

		val = ioread32(&otg_host->regs->otg_device_ctrl);
		val |= A_BUS_DROP_BIT;
		iowrite32(val, &otg_host->regs->otg_device_ctrl);
	}

	if (int_status & A_SRP_DET_IF) {
		otg_debug("A_SRP_DET_IF\n");
		otg_host->fsm.a_srp_det = 1;
	}

	if (int_status & B_AIDL_BDIS_IF) {
		otg_debug("B_AIDL_BDIS_IF\n");
	}

	if (int_status & A_AIDS_BDIS_TOUT_IF) {
		otg_debug("A_AIDS_BDIS_TOUT_IF\n");
		otg_host->fsm.a_aidl_bdis_tmout = 1;
	}

	if (int_status & B_SRP_FAIL_IF) {
		otg_debug("B_SRP_FAIL_IF id %x\n", int_status);
	}

	if (int_status & BDEV_CONNT_TOUT_IF) {
		otg_debug("BDEV_CONNT_TOUT_IF\n");

		val = ioread32(&otg_host->regs->otg_device_ctrl);
		val |= A_BUS_DROP_BIT;
		iowrite32(val, &otg_host->regs->otg_device_ctrl);

#ifdef	CONFIG_ADP_TIMER
		mod_timer(&otg_host->adp_timer, ADP_TIMER_FREQ + jiffies);
#endif
		otg_host->fsm.a_wait_bcon_tmout = 1;

		//otg_debug("adp in %09ld.%09ld\n", t0.tv_sec, t0.tv_nsec);
	}

	if (int_status & VBUS_RISE_TOUT_IF) {
		otg_debug("VBUS_RISE_TOUT_IF\n");
		otg_host->fsm.a_wait_vrise_tmout = 1;
	}

	if (int_status & ID_CHAGE_IF) {
		otg_debug("ID_CHAGE_IF\n");

		otg_host->fsm.id = (int_status & ID_PIN) ? 1 : 0;
		if ((int_status & ID_PIN) == 0) {
			iowrite32(~OTG_SIM & (OTG_ADP | OTG_SRP | OTG_20),
				  &otg_host->regs->mode_select);
#ifdef	CONFIG_ADP_TIMER
			mod_timer(&otg_host->adp_timer,
				  ADP_TIMER_FREQ + jiffies);
#endif
		} else {
			iowrite32(~OTG_SIM & (OTG_SRP | OTG_20),
				  &otg_host->regs->mode_select);
		}
	}

	if (int_status & OVERCURRENT_IF) {
		otg_debug("OVERCURRENT_IF\n");

		val = ioread32(&otg_host->regs->otg_device_ctrl);
		val &= ~A_CLE_ERR_BIT;
		val |= A_BUS_DROP_BIT;
		iowrite32(val, &otg_host->regs->otg_device_ctrl);

		//otg_debug("oct in %09ld.%09ld\n", t0.tv_sec, t0.tv_nsec);

#ifdef	CONFIG_ADP_TIMER
		mod_timer(&otg_host->adp_timer, (HZ/10) + jiffies);
#endif

		//otg_host->fsm.id = (int_status&ID_PIN) ? 1 : 0;
		//otg_host->otg.state = OTG_STATE_A_VBUS_ERR;
	}

	otg_debug(" otg irq out\n");

	sphe_otg_update_transceiver(otg_host);

	return IRQ_HANDLED;
}

static int __devinit sp8338_otg_probe(struct platform_device *dev)
{
	struct sphe8388_otg *otg_host;
	struct resource *res_mem;
	int ret;

	otg_host = kzalloc(sizeof(struct sphe8388_otg), GFP_KERNEL);
	if (!otg_host) {
		printk("Alloc mem for otg host fail\n");
		return -ENOMEM;
	}

	sphe_otg_host = otg_host;

	otg_host->otg.otg = kzalloc(sizeof(struct usb_otg), GFP_KERNEL);
	if (!otg_host->otg.otg) {
		kfree(otg_host);
		printk("Alloc mem for otg fail\n");
		return -ENOMEM;
	}

	otg_host->irq = platform_get_irq(dev, 0);
	if (otg_host->irq < 0) {
		pr_err("otg no irq provieded\n");
		ret = otg_host->irq;
		goto release_mem;
	}
	res_mem = platform_get_resource(dev, IORESOURCE_MEM, 0);
	if (!res_mem) {
		pr_err("otg no memory recourse provieded\n");
		ret = -ENXIO;
		goto release_mem;
	}

	if (!request_mem_region
	    (res_mem->start, resource_size(res_mem), DRIVER_NAME)) {
		pr_err("Otg controller already in use\n");
		ret = -EBUSY;
		goto release_mem;
	}

	otg_host->regs =
	    (struct sphe8388_regs *)ioremap_nocache(res_mem->start,
						    resource_size(res_mem));
	if (!otg_host->regs) {
		ret = -EBUSY;
		goto err_release_region;
	}

	otg_debug("@@@ otg reg %x %d irq %d %x\n", res_mem->start,
		  resource_size(res_mem), otg_host->irq,
		  ioread32(&otg_host->regs->otg_int_st));

	otg_host->qwork = create_singlethread_workqueue(DRIVER_NAME);
	if (!otg_host->qwork) {
		dev_dbg(&dev->dev, "cannot create workqueue %s\n", DRIVER_NAME);
		ret = -ENOMEM;
		goto err_ioumap;
	}
	INIT_WORK(&otg_host->work, sphe8388_otg_work);

	otg_host->notifier.notifier_call = sphe8388_notifier_call;
	usb_register_notify(&otg_host->notifier);

	otg_host->otg.otg->set_host = sphe8388_set_host;
	otg_host->otg.otg->set_peripheral = sphe8388_set_peripheral;
	otg_host->otg.otg->set_vbus = sphe8388_set_vbus;
	otg_host->otg.otg->start_hnp = sphe8388_start_hnp;
	otg_host->otg.otg->start_srp = sphe8388_start_srp;

	otg_host->otg.otg->phy = &otg_host->otg;

	otg_host->otg.io_priv = otg_host->regs;
	otg_host->otg.io_ops = &sphe8388_phy_ios;

#ifdef	CONFIG_ADP_TIMER
	init_timer(&otg_host->adp_timer);
	otg_host->adp_timer.function = adp_watchdog;
	otg_host->adp_timer.data = (unsigned long)otg_host;
#endif

	usb_set_transceiver_8388(&otg_host->otg, dev->id - 1);

	otg_hw_init(otg_host);
	otg_hsm_init(otg_host);

	if (request_irq
	    (otg_host->irq, otg_irq, IRQF_SHARED, DRIVER_NAME, otg_host) != 0) {
		printk("OTG: Request irq fail\n");
		goto err_ioumap;
	}

	ENABLE_OTG_INT(&otg_host->regs->otg_init_en);

	platform_set_drvdata(dev, otg_host);

	if (otg_host->fsm.id == 0) {
		sphe_otg_update_transceiver(otg_host);
	}

	return 0;

err_ioumap:
	iounmap(otg_host->regs);
err_release_region:
	release_mem_region(res_mem->start, resource_size(res_mem));
release_mem:
	kfree(otg_host);

	return ret;
}

static int __devexit sp8338_otg_remove(struct platform_device *dev)
{
	struct resource *res_mem;
	struct sphe8388_otg *otg_host = platform_get_drvdata(dev);

	if (otg_host->qwork) {
		flush_workqueue(otg_host->qwork);
		destroy_workqueue(otg_host->qwork);
	}
#ifdef	CONFIG_ADP_TIMER
	del_timer_sync(&otg_host->adp_timer);
#endif

	free_irq(otg_host->irq, otg_host);
	iounmap(otg_host->regs);

	res_mem = platform_get_resource(dev, IORESOURCE_MEM, 0);
	if (!res_mem) {
		pr_err("otg  release get recourse fail\n");
		goto free_mem;
	}

	release_mem_region(res_mem->start, resource_size(res_mem));

free_mem:
	kfree(otg_host->otg.otg);
	kfree(otg_host);

	return 0;
}

static int sp8338_otg_suspend(struct platform_device *dev, pm_message_t state)
{
	return 0;
}

static int sp8338_otg_resume(struct platform_device *dev)
{
	return 0;
}

struct platform_driver sphe8388_otg_driver = {
	.probe = sp8338_otg_probe,
	.remove = __devexit_p(sp8338_otg_remove),
	.suspend = sp8338_otg_suspend,
	.resume = sp8338_otg_resume,

	.driver = {
		   .name = DRIVER_NAME,
		   .owner = THIS_MODULE,
		   //.of_match_table = sp_sdio_of_match,
		   },
};

static __init int sphe8388_otg_init(void)
{
	int ret;

	otg_debug("--(---- init 8388 otg drvier \n");

	ret = platform_driver_register(&sphe8388_otg_driver);
	if (ret) {
		printk("Register sphe8388 otg driver fail(%d)\n", ret);
	}

	return ret;
}

static __exit void sphe8388_otg_exit(void)
{
	platform_driver_unregister(&sphe8388_otg_driver);
}

module_init(sphe8388_otg_init);
module_exit(sphe8388_otg_exit);

MODULE_AUTHOR("Yong.y");
MODULE_DESCRIPTION("sphe-8388 OTG driver");
MODULE_LICENSE("GPL");
