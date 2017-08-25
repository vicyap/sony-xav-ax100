#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/io.h>
#include <linux/proc_fs.h>
#include <linux/jiffies.h>
#include <linux/uaccess.h>
#include <linux/hrtimer.h>
#include <linux/ktime.h>
#include <linux/sched.h>
#include <linux/timer.h>
#include <linux/delay.h>
#include <linux/clocksource.h>
#include <linux/clockchips.h>
#include <linux/interrupt.h>
#include <linux/irq.h>
#include <linux/clk.h>

#include <asm/sched_clock.h>
#include <asm/mach/time.h>
#include <asm/smp_twd.h>
#include <mach/hal/regmap/reg_system.h>
#include <mach/sp_clk.h>

// #define SP_STC_AV2_FREQ		(1000*1000)	/* 1 MHz */
#define SP_STC_AV2_FREQ		(270*1000*1000)	/* 270 MHz */

// #define ENABLE_SP_STC_AV2_TIMER2

// #define SP_HRTIMER_TEST
#ifdef SP_HRTIMER_TEST
#if defined(ENABLE_SP_STC_AV2_TIMER2)
#error "SP_STC_AV2_TIMER2 is used in this test, please undefine ENABLE_SP_STC_AV2_TIMER2"
#endif

/* Test command:
for timeout in 1000 2000 3000 4000 5000 6000 12000 25000 50000 100000 200000 300000 600000 1000000 2000000 3000000; do
	echo test for $timeout nsec ...
	echo $timeout > /proc/sp_hrtimer_test && \
	for i in `seq 5` ; do
	    cat /proc/sp_hrtimer_test && echo --- && sleep 1
	done
	echo ================
done
*/

static u32 sp_hrtimer_test_timeout = 1000;
static u32 sp_hrtimer_test_seq = 0;
static struct hrtimer hr_timer;

enum hrtimer_restart sp_hrtimer_test_callback(struct hrtimer *timer)
{
	stc_avReg_t *pstc_avReg = (stc_avReg_t *)(LOGI_ADDR_STC_AV2_REG);

	pstc_avReg->timer2_ctrl = 0;	/* Stop timer */
	sp_hrtimer_test_seq++;

	return HRTIMER_NORESTART;
}

static int sp_hrtimer_test_write(struct file *file, const char __user *buf, unsigned long count, void *data)
{
	char buffer[1024];

	if( (count > sizeof(buffer)) || (copy_from_user(buffer, buf, count)) ) {
		printk("Error: %s:%d", __FILE__, __LINE__);
		return -EFAULT;
	}

	sscanf(buffer, "%u", &sp_hrtimer_test_timeout);
	// printk("set sp_hrtimer_test_timeout: %u nsec\n", sp_hrtimer_test_timeout);

	return count;
}

static int sp_hrtimer_test_read(char *page, char **start, off_t off, int count, int *eof, void *data)
{
	int  read_size = 0;
	char *buffer_ptr = page;
	ktime_t ktime;
	stc_avReg_t *pstc_avReg = (stc_avReg_t *)(LOGI_ADDR_STC_AV2_REG);
	u32 div = 0;
	u32 actual_timeout;

	if(*eof || off) {
		return 0;
	}

#if (SP_STC_AV2_FREQ == 270000000)
	if(sp_hrtimer_test_timeout < 242718)	/* ((1 << 16) - 2)*100/27 */
		div = 0;
	else if(sp_hrtimer_test_timeout < 485437)	/* 2*((1 << 16) - 2)*100/27 */
		div = 1;
	else if(sp_hrtimer_test_timeout < 728155)	/* 3*((1 << 16) - 2)*100/27 */
		div = 2;
	else if(sp_hrtimer_test_timeout < 970874)	/* 4*((1 << 16) - 2)*100/27 */
		div = 3;
	else if(sp_hrtimer_test_timeout < 1213592)	/* 5*((1 << 16) - 2)*100/27 */
		div = 4;
	else if(sp_hrtimer_test_timeout < 1456311)	/* 6*((1 << 16) - 2)*100/27 */
		div = 5;
	else if(sp_hrtimer_test_timeout < 1699029)	/* 7*((1 << 16) - 2)*100/27 */
		div = 6;
	else if(sp_hrtimer_test_timeout < 1941748)	/* 8*((1 << 16) - 2)*100/27 */
		div = 7;
	else if(sp_hrtimer_test_timeout < 2184466)	/* 9*((1 << 16) - 2)*100/27 */
		div = 8;
	else if(sp_hrtimer_test_timeout < 2427185)	/* 10*((1 << 16) - 2)*100/27 */
		div = 9;
	else if(sp_hrtimer_test_timeout < 2669903)	/* 11*((1 << 16) - 2)*100/27 */
		div = 10;
	else if(sp_hrtimer_test_timeout < 2912622)	/* 12*((1 << 16) - 2)*100/27 */
		div = 11;
	else if(sp_hrtimer_test_timeout < 3155340)	/* 13*((1 << 16) - 2)*100/27 */
		div = 12;

	pstc_avReg->timer2_ctrl = 0;
	pstc_avReg->timer2_divisor = div;
	pstc_avReg->timer2_reload = 0xffff;
	pstc_avReg->timer2_cnt = pstc_avReg->timer2_reload;
#else
#error Unexpected freq.
#endif

	read_size += sprintf(buffer_ptr+read_size, "timeout = %u nsec, seq = %u, jiffies = %lu, timer2_cnt = 0x%x\n",
			sp_hrtimer_test_timeout, sp_hrtimer_test_seq, jiffies, pstc_avReg->timer2_cnt);

	ktime = ktime_set(0, sp_hrtimer_test_timeout);
	hrtimer_init(&hr_timer, CLOCK_MONOTONIC, HRTIMER_MODE_REL);
	hr_timer.function = &sp_hrtimer_test_callback;

	pstc_avReg->timer2_ctrl = 1;	/* start timer, GO, one-shot, system clock */

	hrtimer_start(&hr_timer, ktime, HRTIMER_MODE_REL);

	while(pstc_avReg->timer2_ctrl);	/* wait for timeout */

	if(pstc_avReg->timer2_cnt) {
		actual_timeout = ((0xffff - pstc_avReg->timer2_cnt) * 100 * (div + 1) / 27);	/* system clk: 270 MHz*/
		read_size += sprintf(buffer_ptr+read_size, "timeout = %u nsec, seq = %u, jiffies = %lu, timer2_cnt = 0x%x\n",
				sp_hrtimer_test_timeout, sp_hrtimer_test_seq, jiffies, pstc_avReg->timer2_cnt);
		read_size += sprintf(buffer_ptr+read_size, "timer2_divisor = %u => interval = %u ns, diff = %u ns\n",
				div, actual_timeout, (actual_timeout - sp_hrtimer_test_timeout));
	} else {
		read_size += sprintf(buffer_ptr+read_size, "Error: timer2_cnt = 0, please adjust timer2_divisor\n");
	}

	*eof = 1;
	return read_size;
}

static int __init sp_hrtimer_test_init(void)
{
	struct proc_dir_entry *ent;

	ent = create_proc_entry("sp_hrtimer_test", 0666, NULL);
	if(!ent) {
		printk("Error: %s:%d", __func__, __LINE__);
	} else {
		ent->write_proc = sp_hrtimer_test_write;
		ent->read_proc = sp_hrtimer_test_read;
	}
	return 0;
}
late_initcall(sp_hrtimer_test_init);

#endif /* SP_HRTIMER_TEST */

static void sp_stc_av2_timer3_start(struct clock_event_device *evt, unsigned long cycles)
{
	stc_avReg_t *pstc_avReg = (stc_avReg_t *)(LOGI_ADDR_STC_AV2_REG);
	u32 div;

	// printk("%s: mode: %d, cycles = %u\n", __func__, (int)(evt->mode), (u32)(cycles));

	pstc_avReg->timer3_ctrl = 0;

	if (evt->mode == CLOCK_EVT_MODE_ONESHOT) {
		/* timer3_cnt only has 16 bits. When (cycles > (1 << 16)),
		 * timer3_divisor is required => the precision is lower.
		 */
		div = (cycles >> 16) + 1;
		pstc_avReg->timer3_divisor = div -1;
		pstc_avReg->timer3_reload = ((u32)(cycles) + div) / div - 1;	/* a little larger than setting, make sure the ISR will see tasks' timeout as expected */
		pstc_avReg->timer3_cnt = pstc_avReg->timer3_reload;
		pstc_avReg->timer3_ctrl = SP_STC_AV_TIMER23_CTL_SRC_STC | SP_STC_AV_TIMER23_CTL_GO;
	} else if (evt->mode == CLOCK_EVT_MODE_PERIODIC) {
#if (SP_STC_AV2_FREQ == 1000000)
		pstc_avReg->timer3_divisor = 0;		/* CLKthis = CLKstc*/
		pstc_avReg->timer3_reload = SP_STC_AV2_FREQ/HZ - 1;
#elif (SP_STC_AV2_FREQ == 270000000)
		pstc_avReg->timer3_divisor = 1000 - 1;		/* CLKthis = CLKstc/1000 */
		pstc_avReg->timer3_reload = ((SP_STC_AV2_FREQ / 1000) / HZ) - 1;
#else
#error Unexpected freq.
#endif
		pstc_avReg->timer3_cnt = pstc_avReg->timer3_reload;
		pstc_avReg->timer3_ctrl = SP_STC_AV_TIMER23_CTL_SRC_STC | SP_STC_AV_TIMER23_CTL_RPT | SP_STC_AV_TIMER23_CTL_GO;
	}
}

static int sp_stc_av2_timer3_event_set_next_event(unsigned long cycles, struct clock_event_device *evt)
{
	// printk("%s: mode: %d\n", __func__, (int)(evt->mode));
	WARN_ON(evt->mode != CLOCK_EVT_MODE_ONESHOT);
	sp_stc_av2_timer3_start(evt, cycles);
	return 0;
}

static void sp_stc_av2_timer3_event_set_mode(enum clock_event_mode mode, struct clock_event_device *evt)
{
	printk("%s: mode: %d\n", __func__, (int)(mode));
	evt->mode = mode;
	sp_stc_av2_timer3_start(evt, ~0);	/* max. cycle, should be updated by .set_next_event() */
}

static struct clock_event_device sp_clockevent_dev_av2_timer3 = {
	.name		= "sp_stc_av2_timer3_event",
	.features       = CLOCK_EVT_FEAT_ONESHOT | CLOCK_EVT_FEAT_PERIODIC,
	.rating		= 350,
	.set_next_event	= sp_stc_av2_timer3_event_set_next_event,
	.set_mode	= sp_stc_av2_timer3_event_set_mode,
};

static irqreturn_t sp_stc_av2_timer3_isr(int irq, void *dev_id)
{
	stc_avReg_t *pstc_avReg = (stc_avReg_t *)(LOGI_ADDR_STC_AV2_REG);
	struct clock_event_device *evt = &sp_clockevent_dev_av2_timer3;

	if (evt->mode == CLOCK_EVT_MODE_PERIODIC) {
		if (pstc_avReg->timer3_ctrl & SP_STC_AV_TIMER23_CTL_GO) {
			evt->event_handler(evt);
			// printk("%s\n", __func__);
		}
	} else if (evt->mode == CLOCK_EVT_MODE_ONESHOT) {
		evt->event_handler(evt);
		// printk("%s\n", __func__);
	}

	return IRQ_HANDLED;
}

static struct irqaction sp_stc_av2_timer3_irq = {
	.name		= "sp_stc_av2_timer3",
	.flags		= IRQF_TIMER | IRQF_IRQPOLL | IRQF_TRIGGER_RISING,
	.handler	= sp_stc_av2_timer3_isr,
	.dev_id		= &sp_clockevent_dev_av2_timer3,
	.irq		= SP_IRQ_AV2_TIMER3,
};

#if defined(ENABLE_SP_STC_AV2_TIMER2)

static void sp_stc_av2_timer2_start(struct clock_event_device *evt)
{
	stc_avReg_t *pstc_avReg = (stc_avReg_t *)(LOGI_ADDR_STC_AV2_REG);

	printk("%s: mode: %d\n", __func__, (int)(evt->mode));

	if (evt->mode == CLOCK_EVT_MODE_PERIODIC) {
		pstc_avReg->timer2_ctrl = 0;
#if (SP_STC_AV2_FREQ == 1000000)
		pstc_avReg->timer2_divisor = 0;		/* CLKthis = CLKstc*/
		pstc_avReg->timer2_reload = SP_STC_AV2_FREQ/HZ - 1;
#elif (SP_STC_AV2_FREQ == 270000000)
		pstc_avReg->timer2_divisor = 1000 - 1;		/* CLKthis = CLKstc/1000 */
		pstc_avReg->timer2_reload = ((SP_STC_AV2_FREQ / 1000) / HZ) - 1;
#else
#error Unexpected freq.
#endif
		pstc_avReg->timer2_cnt = pstc_avReg->timer2_reload;
		pstc_avReg->timer2_ctrl = SP_STC_AV_TIMER23_CTL_SRC_STC | SP_STC_AV_TIMER23_CTL_RPT | SP_STC_AV_TIMER23_CTL_GO;
	}
	else {
		pstc_avReg->timer2_ctrl = 0;
	}
}

static int sp_stc_av2_timer2_event_set_next_event(unsigned long cycles, struct clock_event_device *evt)
{
	printk("%s\n", __func__);

	/* only CLOCK_EVT_FEAT_PERIODIC is supported */
	sp_stc_av2_timer2_start(evt);
	return 0;
}

static void sp_stc_av2_timer2_event_set_mode(enum clock_event_mode mode, struct clock_event_device *evt)
{
	printk("%s: mode: %d\n", __func__, (int)(mode));
	evt->mode = mode;
	sp_stc_av2_timer2_start(evt);
}

static struct clock_event_device sp_clockevent_dev_av2_timer2 = {
	.name		= "sp_stc_av2_timer2_event",
	.features       = CLOCK_EVT_FEAT_PERIODIC,
	.rating		= 250,
	.set_next_event	= sp_stc_av2_timer2_event_set_next_event,
	.set_mode	= sp_stc_av2_timer2_event_set_mode,
};

static irqreturn_t sp_stc_av2_timer2_isr(int irq, void *dev_id)
{
	stc_avReg_t *pstc_avReg = (stc_avReg_t *)(LOGI_ADDR_STC_AV2_REG);
	struct clock_event_device *evt = &sp_clockevent_dev_av2_timer2;

	if (pstc_avReg->timer2_ctrl & SP_STC_AV_TIMER23_CTL_GO) {
		evt->event_handler(evt);
		// printk("%s\n", __func__);
	}

	return IRQ_HANDLED;
}

static struct irqaction sp_stc_av2_timer2_irq = {
	.name		= "sp_stc_av2_timer2",
	.flags		= IRQF_TIMER | IRQF_IRQPOLL | IRQF_TRIGGER_RISING,
	.handler	= sp_stc_av2_timer2_isr,
	.dev_id		= &sp_clockevent_dev_av2_timer2,
	.irq		= SP_IRQ_AV2_TIMER2,
};
#endif /* ENABLE_SP_STC_AV2_TIMER2 */

static void sp_clockevent_init(void)
{
	int ret;

#if defined(ENABLE_SP_STC_AV2_TIMER2)
	ret = setup_irq(sp_stc_av2_timer2_irq.irq, &sp_stc_av2_timer2_irq);
	if (ret) {
		printk(KERN_ERR "%s, %d: reg = %d\n", __FILE__, __LINE__, ret);
		BUG();
	}

	/* No support for one-shot mode, min_delta, max_delta: don't care */
	clockevents_config_and_register(&sp_clockevent_dev_av2_timer2, HZ, 1, 1);
#endif /* ENABLE_SP_STC_AV2_TIMER2 */

	ret = setup_irq(sp_stc_av2_timer3_irq.irq, &sp_stc_av2_timer3_irq);
	if (ret) {
		printk(KERN_ERR "%s, %d: reg = %d\n", __FILE__, __LINE__, ret);
		BUG();
	}
#if (SP_STC_AV2_FREQ == 1000000)
	clockevents_config_and_register(&sp_clockevent_dev_av2_timer3, SP_STC_AV2_FREQ, 1, ((1 << 16) - 1));
#elif (SP_STC_AV2_FREQ == 270000000)
	clockevents_config_and_register(&sp_clockevent_dev_av2_timer3, SP_STC_AV2_FREQ, 1, ((1 << 28) - 1));	/* 28 bits > 16 bits counter => timer3_divisor is required. */
#else
#error Unexpected freq.
#endif


}

u32 sp_read_sched_clock(void)
{
	stc_avReg_t *pstc_avReg = (stc_avReg_t *)(LOGI_ADDR_STC_AV2_REG);

	pstc_avReg->stcl_2 = 0;
	wmb();
	return ((pstc_avReg->stcl_0) | ((pstc_avReg->stcl_1)<<16));
}

cycle_t sp_clocksource_hz_read(struct clocksource *cs)
{
	return (cycle_t)(sp_read_sched_clock());
}

static void sp_clocksource_hw_init(void)
{
	u32 system_clk = 0;
	stc_avReg_t *pstc_avReg = (stc_avReg_t *)(LOGI_ADDR_STC_AV2_REG);

	system_clk = sp_clk_get(SP_CLK_SYSSLOW);
	printk("%s: system_clk = %d\n", __func__, system_clk);

	/*
	 *  STC_AV2 (Group 99):
	 *  STC: clock source for scheduler and jiffies (HZ)
	 *  timer0/1: Unused.
	 *  timer2: event for jiffies (if ENABLE_SP_STC_AV2_TIMER2 defined)
	 *  timer3: event for jiffies/hrtimer
	 */
#if (SP_STC_AV2_FREQ == 1000000)
	pstc_avReg->stc_divisor	= (system_clk / SP_STC_AV2_FREQ) - 1;	/* CLKstc = 1 us */
#elif (SP_STC_AV2_FREQ == 270000000)
	pstc_avReg->stc_divisor	= (system_clk / SP_STC_AV2_FREQ) - 1;	/* CLKstc = CLKsys */
#else
#error Unexpected freq.
#endif
	pstc_avReg->stc_64	= 0;					/* reset STC and start counting*/
}

static void sp_clocksource_resume(struct clocksource *cs)
{
	sp_clocksource_hw_init();
}

struct clocksource sp_clocksource_hz = {
	.name		= "sp_clocksource_hz",
#if (SP_STC_AV2_FREQ == 1000000)
	.rating		= 250,
#elif (SP_STC_AV2_FREQ == 270000000)
	.rating		= 350,
#else
#error Unexpected freq.
#endif
	.read		= sp_clocksource_hz_read,
	.mask           = CLOCKSOURCE_MASK(32),
	.flags		= CLOCK_SOURCE_IS_CONTINUOUS,
	.resume		= sp_clocksource_resume,
};

static void __init sp_clocksource_init(void)
{
	u32 (*func_ptr)(void);
	func_ptr = sp_read_sched_clock;

	sp_clocksource_hw_init();
	setup_sched_clock(func_ptr, 32, SP_STC_AV2_FREQ);
	if (clocksource_register_hz(&sp_clocksource_hz, SP_STC_AV2_FREQ))
		panic("%s: can't register clocksource\n", sp_clocksource_hz.name);
}

static void __init sp_timer_init(void)
{
	sp_clocksource_init();
	sp_clockevent_init();
}

struct sys_timer sp_timer = {
	.init		= sp_timer_init,
};

