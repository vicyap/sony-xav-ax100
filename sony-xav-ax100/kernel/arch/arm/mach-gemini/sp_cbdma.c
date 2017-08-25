/*
 * (C) Copyright 2014
 * Sunplus Technology
 * Kuo-Jung Su <dante.su@sunplus.com>
 *
 * SPDX-License-Identifier:     GPL-2.0+
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/delay.h>
#include <linux/sched.h>
#include <linux/device.h>
#include <linux/interrupt.h>
#include <linux/wait.h>
#include <linux/slab.h>
#include <linux/dma-mapping.h>
#include <linux/platform_device.h>
#include <linux/mutex.h>

#include <asm/setup.h>
#include <asm/io.h>

struct sp_cbdma_regs {
    uint32_t revr; /* Revision register */
    uint32_t cr;   /* Control register */
    uint32_t lr;   /* Length register */
    uint32_t src;  /* Source address */
    uint32_t dst;  /* Destination address */
    uint32_t isr;  /* Interrupt status register */
    uint32_t ier;  /* Interrupt enable register */
    uint32_t val;  /* MEMSET value register */
    uint32_t ram;  /* SDRAM config */
};

enum sp_cbdma_mode {
    SP_CBDMA_MEMSET = 0,
    SP_CBDMA_WRITE,
    SP_CBDMA_READ,
    SP_CBDMA_COPY,
};

#define CR_GO       BIT(8)
#define CR_MODE(x)  ((x) & 3)
#define CR_CMDQ(x)  (((x) & 3) << 16)

#define ISR_OVERLAP	BIT(6)
#define ISR_SDRAM_B	BIT(5)
#define ISR_SDRAM_A	BIT(4)
#define ISR_SRAM	BIT(3)
#define ISR_CBSRAM	BIT(2)
#define ISR_ERRORS	0x7c
#define ISR_DONE	BIT(0)

struct sp_cbdma_chip {
	struct device *dev;
	void __iomem *regs;
	int irq;

	int busy;
	struct mutex lock;
	wait_queue_head_t wq;
};

static dma_addr_t phys_to_dma(dma_addr_t addr)
{
	if (addr > 0x7fffffff)
		return addr & 0x1ffff;

	return addr;
}

static void sp_cbdma_reset(struct sp_cbdma_chip *chip)
{
	struct sp_cbdma_regs *regs = chip->regs;

	/* Reset interrupt status */
	writel(0x7d, &regs->ier);
	writel(0x7f, &regs->isr);

	/* SDRAM windows size, both bank A and B are 1GB */
	writel(0x0707, &regs->ram);
}

static int sp_cbdma_wait(struct sp_cbdma_chip *chip)
{
	int ret = 0;

	if (!wait_event_timeout(chip->wq, !chip->busy, HZ))
		ret = -EBUSY;

	return ret;
}

static irqreturn_t sp_cbdma_irq(int irq, void *dev)
{
	struct sp_cbdma_chip *chip = dev;
	struct sp_cbdma_regs *regs = chip->regs;
    uint32_t st;

    st = readl(&regs->isr);
	writel(st, &regs->isr);

	chip->busy = 0;
	wake_up(&chip->wq);

	return IRQ_HANDLED;
}

int sp_cbdma_memset(struct device *dev,
		dma_addr_t s, int c, size_t n)
{
	struct sp_cbdma_chip *chip = dev_get_drvdata(dev);
	struct sp_cbdma_regs *regs = chip->regs;
	int ret;

	n = min_t(int, n, SZ_16M);

	mutex_lock(&chip->lock);

	sp_cbdma_reset(chip);

	writel(c, &regs->val);
	writel(0, &regs->src);
	writel(phys_to_dma(s), &regs->dst); /* DRAM */
	writel(n, &regs->lr);

	chip->busy = 1;
	writel(CR_GO | CR_MODE(SP_CBDMA_MEMSET) | CR_CMDQ(3), &regs->cr);
	ret = sp_cbdma_wait(chip);

	mutex_unlock(&chip->lock);

	if (ret)
		printk("sp_cbdma: memset timeout\n");
	else
		ret = n;

	return ret;
}
EXPORT_SYMBOL(sp_cbdma_memset);

int sp_cbdma_write(struct device *dev,
		dma_addr_t src, dma_addr_t dst, size_t len)
{
	struct platform_device *pdev = to_platform_device(dev);
	struct resource *res = platform_get_resource(pdev, IORESOURCE_MEM, 1);
	struct sp_cbdma_chip *chip = dev_get_drvdata(dev);
	struct sp_cbdma_regs *regs = chip->regs;
	int ret;

	if (!res || src < res->start || src > res->end)
		return -EINVAL;

	len = min_t(size_t, len, res->end - src + 1);

	mutex_lock(&chip->lock);

	sp_cbdma_reset(chip);

	writel(phys_to_dma(src), &regs->src); /* sram */
	writel(phys_to_dma(dst), &regs->dst); /* DRAM */
	writel(len, &regs->lr);

	chip->busy = 1;
	writel(CR_GO | CR_MODE(SP_CBDMA_WRITE) | CR_CMDQ(3), &regs->cr);
	ret = sp_cbdma_wait(chip);

	mutex_unlock(&chip->lock);

	if (ret)
		printk("sp_cbdma: write timeout\n");
	else
		ret = len;

	return ret;
}
EXPORT_SYMBOL(sp_cbdma_write);

int sp_cbdma_read(struct device *dev,
		dma_addr_t src, dma_addr_t dst, size_t len)
{
	struct platform_device *pdev = to_platform_device(dev);
	struct resource *res = platform_get_resource(pdev, IORESOURCE_MEM, 1);
	struct sp_cbdma_chip *chip = dev_get_drvdata(dev);
	struct sp_cbdma_regs *regs = chip->regs;
	int ret;

	if (!res || dst < res->start || dst > res->end)
		return -EINVAL;

	len = min_t(size_t, len, res->end - dst + 1);

	mutex_lock(&chip->lock);

	sp_cbdma_reset(chip);

	writel(phys_to_dma(src), &regs->src); /* DRAM */
	writel(phys_to_dma(dst), &regs->dst); /* sram */
	writel(len, &regs->lr);

	chip->busy = 1;
	writel(CR_GO | CR_MODE(SP_CBDMA_READ) | CR_CMDQ(3), &regs->cr);
	ret = sp_cbdma_wait(chip);

	mutex_unlock(&chip->lock);

	if (ret)
		printk("sp_cbdma: read timeout\n");
	else
		ret = len;

	return ret;
}
EXPORT_SYMBOL(sp_cbdma_read);

int sp_cbdma_memcpy(struct device *dev,
		dma_addr_t src, dma_addr_t dst, size_t len)
{
	struct sp_cbdma_chip *chip = dev_get_drvdata(dev);
	struct sp_cbdma_regs *regs = chip->regs;
	int ret;

	len = min_t(int, len, SZ_16M);

	mutex_lock(&chip->lock);

	sp_cbdma_reset(chip);

	writel(phys_to_dma(src), &regs->src); /* DRAM */
	writel(phys_to_dma(dst), &regs->dst); /* DRAM */
	writel(len, &regs->lr);

	chip->busy = 1;
	writel(CR_GO | CR_MODE(SP_CBDMA_COPY) | CR_CMDQ(3), &regs->cr);
	ret = sp_cbdma_wait(chip);

	mutex_unlock(&chip->lock);

	if (ret)
		printk("sp_cbdma: copy timeout\n");
	else
		ret = len;

	return ret;
}
EXPORT_SYMBOL(sp_cbdma_memcpy);

struct sp_cbdma_lookup {
	const char *name;
	struct device *dev;
};

static int find_cbdma_device(struct device *dev, void *data)
{
	struct platform_device *pdev = to_platform_device(dev);
	struct sp_cbdma_lookup *ctx = data;
	char name[256];

	if (pdev->id >= 0)
		sprintf(name, "%s.%d", pdev->name, pdev->id);
	else
		sprintf(name, "%s", pdev->name);

	if (!strcmp(name, ctx->name))
		ctx->dev = &pdev->dev;

	return 0;
}

struct device *sp_cbdma_getbyname(const char *name)
{
	struct sp_cbdma_lookup ctx;

	ctx.name = name;
	ctx.dev = NULL;
	bus_for_each_dev(&platform_bus_type, NULL, &ctx, find_cbdma_device);
	return ctx.dev;
}
EXPORT_SYMBOL(sp_cbdma_getbyname);

static int sp_cbdma_probe(struct platform_device *pdev)
{
	struct sp_cbdma_chip *chip;
	struct resource *res;
	int ret = 0;

	/*
	 * use reasonable defaults so platforms don't have to provide these.
	 * with DT probing on ARM, none of these are set.
	 */
	if (!pdev->dev.dma_mask)
		pdev->dev.dma_mask = &pdev->dev.coherent_dma_mask;
	if (!pdev->dev.coherent_dma_mask)
		pdev->dev.coherent_dma_mask = DMA_BIT_MASK(32);

	chip = kzalloc(sizeof(*chip), GFP_KERNEL);
	if (!chip)
		return -ENOMEM;

	mutex_init(&chip->lock);
	init_waitqueue_head(&chip->wq);

	chip->dev = &pdev->dev;

	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (!res) {
		ret = -ENXIO;
		goto err;
	}
	if (!request_mem_region(res->start, resource_size(res), "sp_cbdma")) {
		ret = -EBUSY;
		goto err;
	}
	chip->regs = ioremap(res->start, resource_size(res));
	if (!chip->regs) {
		ret = -ENOMEM;
		goto err;
	}

	chip->irq = platform_get_irq(pdev, 0);
	if (chip->irq <= 0) {
		ret = -ENXIO;
		goto err;
	}

	printk("sp_cbdma: mmio=0x%p, irq=%d\n", chip->regs, chip->irq);

	sp_cbdma_reset(chip);

	if (request_irq(chip->irq, sp_cbdma_irq, 0, "sp_cbdma", chip)) {
		printk("sp_cbdma: unable to register IRQ(%d)\n", chip->irq);
		ret = -EBUSY;
		goto err;
	}

	platform_set_drvdata(pdev, chip);

	return ret;

err:
	release_mem_region(res->start, resource_size(res));
	return ret;
}

static int sp_cbdma_remove(struct platform_device *pdev)
{
	struct sp_cbdma_chip *chip = platform_get_drvdata(pdev);
	struct resource *res;

	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);

	if (res)
		release_mem_region(res->start, resource_size(res));

	if (chip->regs)
		iounmap(chip->regs);

	if (chip->irq)
		free_irq(chip->irq, "sp_cbdma");

	wake_up(&chip->wq);
	mutex_destroy(&chip->lock);

	platform_set_drvdata(pdev, NULL);

	return 0;
}

#ifdef CONFIG_PM
static int sp_cbdma_suspend(struct platform_device *pdev, pm_message_t mesg)
{
	return 0;
}

static int sp_cbdma_resume(struct platform_device *pdev)
{
	return 0;
}
#else
#define sp_cbdma_suspend	NULL
#define sp_cbdma_resume		NULL
#endif

/* work with "modprobe sp_cbdma" from hotplugging or coldplugging */
MODULE_ALIAS("platform:sp_cbdma");

static struct platform_driver sp_cbdma_driver = {
	.probe		= sp_cbdma_probe,
	.remove		= sp_cbdma_remove,
	.suspend	= sp_cbdma_suspend,
	.resume		= sp_cbdma_resume,
	.driver		= {
		.name	= "sp_cbdma",
		.owner	= THIS_MODULE,
	},
};

static int __init sp_cbdma_modinit(void)
{
	return platform_driver_register(&sp_cbdma_driver);
}

static void __exit sp_cbdma_modexit(void)
{
	platform_driver_unregister(&sp_cbdma_driver);
}

module_init(sp_cbdma_modinit);
module_exit(sp_cbdma_modexit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Dante Su");
MODULE_DESCRIPTION("Sunplus CBUS DMA controller");
