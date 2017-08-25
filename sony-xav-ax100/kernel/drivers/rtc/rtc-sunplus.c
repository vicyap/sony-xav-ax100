/*
 * How to test RTC:
 *
 * hwclock - query and set the hardware clock (RTC)
 *
 * (for i in `seq 5`; do (echo ------ && echo -n 'date      : ' && date && echo -n 'hwclock -r: ' && hwclock -r; sleep 1); done)
 * date 121209002014 # Set system to 2014/Dec/12 09:00
 * (for i in `seq 5`; do (echo ------ && echo -n 'date      : ' && date && echo -n 'hwclock -r: ' && hwclock -r; sleep 1); done)
 * hwclock -s # Set the System Time from the Hardware Clock
 * (for i in `seq 5`; do (echo ------ && echo -n 'date      : ' && date && echo -n 'hwclock -r: ' && hwclock -r; sleep 1); done)
 * date 121213002014 # Set system to 2014/Dec/12 13:00
 * (for i in `seq 5`; do (echo ------ && echo -n 'date      : ' && date && echo -n 'hwclock -r: ' && hwclock -r; sleep 1); done)
 * hwclock -w # Set the Hardware Clock to the current System Time
 * (for i in `seq 10000`; do (echo ------ && echo -n 'date      : ' && date && echo -n 'hwclock -r: ' && hwclock -r; sleep 1); done)
 *
 */

#include <linux/module.h>
#include <linux/err.h>
#include <linux/rtc.h>
#include <linux/platform_device.h>
#include <mach/kernel.h>
#include <mach/hardware.h>

static struct platform_device *sp_rtc_device0;

#define LOGI_ADDR_RTC		IO0_ADDRESS( 116*32*4 )

struct rtc_reg {
	u32 reserved_0;
	u32 rtc_ctrl;
	u32 rtc_timer;
	u32 rtc_ontime_set;
	u32 rtc_clock_set;
};

static void sp_get_seconds(unsigned long *secs)
{
	struct rtc_reg *ptr;

	ptr = (struct rtc_reg *)(LOGI_ADDR_RTC);
	*secs = (unsigned long)(ptr->rtc_timer);
}

static void sp_set_seconds(unsigned long secs)
{
	struct rtc_reg *ptr;

	ptr = (struct rtc_reg *)(LOGI_ADDR_RTC);
	ptr->rtc_clock_set = (u32)(secs);
}

static int sp_rtc_read_time(struct device *dev, struct rtc_time *tm)
{
	unsigned long secs;

	sp_get_seconds(&secs);
	rtc_time_to_tm(secs, tm);

	/*
	printk("%s:  RTC date/time to %d-%d-%d, %02d:%02d:%02d.\r\n",
		__func__, tm->tm_mday, tm->tm_mon + 1, tm->tm_year, tm->tm_hour, tm->tm_min, tm->tm_sec);
	*/

	return rtc_valid_tm(tm);
}

int sp_rtc_get_time(struct rtc_time *tm)
{
	unsigned long secs;

	sp_get_seconds(&secs);
	rtc_time_to_tm(secs, tm);

	return 0;
}
EXPORT_SYMBOL(sp_rtc_get_time);

static int sp_rtc_suspend(struct platform_device *pdev, pm_message_t state)
{
	struct rtc_reg *ptr;

	printk("%s\n", __func__);

	ptr = (struct rtc_reg *)(LOGI_ADDR_RTC);
	ptr->rtc_ctrl |= 1 << 4;	// Keep RTC from system reset

	return 0;
}

static int sp_rtc_resume(struct platform_device *pdev)
{
	struct rtc_reg *ptr;

	// Because RTC is still powered during suspend,
	// there is nothing to do here.

	printk("%s\n", __func__);

	ptr = (struct rtc_reg *)(LOGI_ADDR_RTC);
	ptr->rtc_ctrl |= 1 << 4;	// Keep RTC from system reset

	return 0;
}

static int sp_rtc_set_mmss(struct device *dev, unsigned long secs)
{
	dev_info(dev, "%s, secs = %lu\n", __func__, secs);
	sp_set_seconds(secs);
	return 0;
}

static const struct rtc_class_ops sp_rtc_ops = {
	.read_time = sp_rtc_read_time,
	.set_mmss = sp_rtc_set_mmss,
};

static int sp_rtc_probe(struct platform_device *plat_dev)
{
	int err;
	struct rtc_reg *ptr;
	struct rtc_device *rtc;

	ptr = (struct rtc_reg *)(LOGI_ADDR_RTC);
	ptr->rtc_ctrl |= 1 << 4;	// Keep RTC from system reset

	rtc = rtc_device_register("sp-rtc", &plat_dev->dev, &sp_rtc_ops, THIS_MODULE);
	if (IS_ERR(rtc)) {
		err = PTR_ERR(rtc);
		return err;
	}

	platform_set_drvdata(plat_dev, rtc);

	return 0;
}

static int __devexit sp_rtc_remove(struct platform_device *plat_dev)
{
	struct rtc_device *rtc = platform_get_drvdata(plat_dev);

	rtc_device_unregister(rtc);

	return 0;
}

static struct platform_driver sp_rtc_driver = {
	.probe		= sp_rtc_probe,
	.remove 	= __devexit_p(sp_rtc_remove),
	.suspend	= sp_rtc_suspend,
	.resume 	= sp_rtc_resume,
	.driver 	= {
		.name = "sp-rtc",
		.owner = THIS_MODULE,
	},
};

static int __init sp_rtc_init(void)
{
	int err;

	if ((err = platform_driver_register(&sp_rtc_driver)))
		return err;

	if ((sp_rtc_device0 = platform_device_alloc("sp-rtc", 0)) == NULL) {
		err = -ENOMEM;
		goto exit_driver_unregister;
	}

	if ((err = platform_device_add(sp_rtc_device0)))
		goto exit_free_sp_rtc_device0;

	return 0;

exit_free_sp_rtc_device0:
	platform_device_put(sp_rtc_device0);

exit_driver_unregister:
	platform_driver_unregister(&sp_rtc_driver);
	return err;
}

static void __exit sp_rtc_exit(void)
{
	platform_device_unregister(sp_rtc_device0);
	platform_driver_unregister(&sp_rtc_driver);
}

MODULE_AUTHOR("Sunplus");
MODULE_DESCRIPTION("Sunplus RTC driver");
MODULE_LICENSE("GPL");

module_init(sp_rtc_init);
module_exit(sp_rtc_exit);
