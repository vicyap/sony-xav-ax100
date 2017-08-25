#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/platform_device.h>
#include <linux/delay.h>
#include <linux/err.h>
#include <linux/clk.h>
#include <linux/io.h>
#include <linux/leds.h>
#include <linux/input.h>
#include <linux/irq.h>
#include <linux/mtd/mtd.h>
#include <linux/mtd/partitions.h>
#include <linux/mtd/nand.h>
#include <linux/regulator/machine.h>
#include <linux/memblock.h>
#include <asm/memory.h>
#include <asm/setup.h>
#include <mach/hardware.h>
#include <asm/mach-types.h>
#include <asm/mach/arch.h>
#include <asm/mach/map.h>
#include <asm/mach/flash.h>
#include <mach/common.h>
// #include <mach/sphe_clock.h>
#include <generated/autoconf.h>
#include <asm/hardware/cache-l2x0.h>
#include <asm/hardware/gic.h>
#include <mach/hal/regmap/reg_system.h>
#include <mach/sp_reservemem.h>
#include <asm/cacheflush.h>
#include "sp_timestamp.h"

/*****************************************************************************/
#define KHZ		1000
#define INPUT_FREQ_KHZ		(CLOCK_TICK_RATE/KHZ)	//(27 * 1000)
#ifndef _BIT
#define _BIT(n)		(1 << (n))
#endif

/* iop start address from sp_reservemem.c */
extern u32 iop_addr_p;
extern volatile u32* iop_addr_v;

int bank_number = 0;

#ifdef CONFIG_CACHE_L2X0
static void __iomem *l2cache_base;

static int __init sp_l2_cache_init(void)
{
	u32 aux_ctrl = 0;
	u32 value;
	early_printk("%s: %s, %d\n", __func__,  __FILE__, __LINE__);

	/* avoid data corruption during l2c init */
	flush_cache_all();

	/* Static mapping, never released */
	l2cache_base = (void __iomem *)L2C_BASE;
	if (WARN_ON(!l2cache_base))
		return -ENOMEM;

	//  aux_ctrl = ((1 << L2X0_AUX_CTRL_ASSOCIATIVITY_SHIFT) |
	//  		(0x1 << 25) |
	//  		(0x1 << L2X0_AUX_CTRL_NS_LOCKDOWN_SHIFT) |
	//  		(0x1 << L2X0_AUX_CTRL_NS_INT_CTRL_SHIFT));
	//  aux_ctrl |= 0x2 << L2X0_AUX_CTRL_WAY_SIZE_SHIFT;

	// l2x0_init(l2cache_base, aux_ctrl, L2X0_AUX_CTRL_MASK);

	value = *(volatile u32 *)(L2C_BASE + 0x0104);   // reg1_aux_control
	value |=  (1 << 30);    // Early BRESP
	value &= ~(1 << 25);    // pseudo random
	value |=  (1 <<  0);    // Enable full line of zero
	*(volatile u32 *)(L2C_BASE + 0x0104) = value;
	aux_ctrl = value;

	asm volatile("mrc p15, 0, %0, c1, c0, 1" : "=r" (value));   // aux control register
	value |=  (1 << 2);     // enable L1 prefetch
	value |=  (1 << 1);     // enable L2 prefetch hint
	asm volatile("mcr p15, 0, %0, c1, c0, 1" : : "r"(value));

	value = *(volatile u32 *)(L2C_BASE + 0x0F60);   // reg15_prefetch_ctrl
	value |=  (1 << 30);    // Double linefill
	value |=  (1 << 29);    // Instruction prefetch enable
	value |=  (1 << 28);    // Data prefetch enable
	value |=  (1 << 27);    // Double linefill on WRAP read disable
	value |=  (1 << 23);    // Incr double linefill enable
	value |=  (1 << 24);    // Prefetch drop enable
	value &= ~(0x1F << 0);  // Prefetch offset
	value |=  ( 7 << 0);    // Prefetch offset
	*(volatile u32 *)(L2C_BASE + 0x0F60) = value;

	value = *(volatile u32 *)(L2C_BASE + 0x0F80);   // reg15_power_ctrl
	value |=  (1 << 1);    // dynamic_clk_gating_en
	value |=  (1 << 0);    // standby_mode_en
	*(volatile u32 *)(L2C_BASE + 0x0F80) = value;

	*(volatile u32 *)(L2C_BASE + 0x0108) = 0;	// reg1_tag_ram_control, recommanded by HW designer
	*(volatile u32 *)(L2C_BASE + 0x010C) = 0x0111;	// reg1_data_ram_control, recommanded by HW designer

	l2x0_init(l2cache_base, aux_ctrl, 0xFFFFFFFF);

	/*
	 * According to L2C-310 TRM r3p3 page 2-37~38,
	 * CA9 "Full line of zero" should be enabled after L2C enable.
	 */
	asm volatile("mrc p15, 0, %0, c1, c0, 1" : "=r" (value));   // aux control register
	value |=  (1 << 3);     // enable write full line of zero
	asm volatile("mcr p15, 0, %0, c1, c0, 1" : : "r"(value));

	return 0;
}
early_initcall(sp_l2_cache_init);
#endif

/*
 * CBUS DMA
 */
static struct resource sp_cbdma0_res[] = {
	{    /* IO base */
		.start = 0x9C000D00,
		.end   = 0x9C000D7F,
		.flags = IORESOURCE_MEM,
	}, { /* Internal SRAM */
		.start = 0x9E800000,
		.end   = 0x9E809FFF,
		.flags = IORESOURCE_MEM,
	}, {
		.start = SP_IRQ_CBDMA0,
		.end   = SP_IRQ_CBDMA0,
		.flags = IORESOURCE_IRQ,
	},
};

static struct platform_device sp_cbdma0_pdev = {
	.name = "sp_cbdma",
	.id = 0,
	.num_resources = ARRAY_SIZE(sp_cbdma0_res),
	.resource = sp_cbdma0_res,
};
#if 0
static struct resource sp_cbdma1_res[] = {
	{    /* IO base */
		.start = 0x9C000D80, // cb-dma1 is for eCos !!!
		.end   = 0x9C000DFF,
		.flags = IORESOURCE_MEM,
	}, { /* Internal SRAM */
		.start = 0x9E820000,
		.end   = 0x9E837FFF,
		.flags = IORESOURCE_MEM,
	}, {
		.start = SP_IRQ_CBDMA1,
		.end   = SP_IRQ_CBDMA1,
		.flags = IORESOURCE_IRQ,
	},
};

static struct platform_device sp_cbdma1_pdev = {
	.name = "sp_cbdma",
	.id = 1,
	.num_resources = ARRAY_SIZE(sp_cbdma1_res),
	.resource = sp_cbdma1_res,
};
#endif

static void __init sp_power_off(void)
{
}

static struct resource sp_bch_res[] = {
	{
		.start = 0x9C101000,
		.end   = 0x9C101FFF,
		.flags = IORESOURCE_MEM,
	}, {
		.start = SP_IRQ_BCH,
		.end   = SP_IRQ_BCH,
		.flags = IORESOURCE_IRQ,
	},
};

static struct platform_device sp_bch_pdev = {
	.name = "sp_bch",
	.id = 0,
	.num_resources = ARRAY_SIZE(sp_bch_res),
	.resource = sp_bch_res,
};

static void __init sp_init(void)
{
	printk("%s\n", __func__);

	pm_power_off = sp_power_off;
	platform_device_register(&sp_cbdma0_pdev);
	//platform_device_register(&sp_cbdma1_pdev);
	platform_device_register(&sp_bch_pdev);

	// remap iop start address
	// iop driver needs these two p/v address
	iop_addr_p = res_iop_addr;

	if (iop_addr_p == -1) {
		printk("%s: iop_addr_p is %08x, do not remap\n", __FUNCTION__, iop_addr_p);
	} else {
            iop_addr_v = ioremap(iop_addr_p, SZ_16K);

	if (iop_addr_v == NULL)
            early_printk("%s: iop_addr_p ioremap() failed\n", __FUNCTION__);
        else
            early_printk("iop remap success: p=%08x v=%08x\n", iop_addr_p, (unsigned int)iop_addr_v);
    }
}

void __init sp_init_irq(void)
{
	void __iomem *sp_irq_base;
	void __iomem *gic_dist_base_addr;

	printk("%s\n", __func__);
#if defined(SUPPORT_SP_TIMESTAMP)
        SP_TIMESTAMP( __FUNCTION__ , 165000, USE_PRINTK);
#endif

	/* Static mapping, never released */
	gic_dist_base_addr = ioremap(SP_GIC_DIST_BASE, SZ_4K);
	BUG_ON(!gic_dist_base_addr);

	/* Static mapping, never released */
	sp_irq_base = ioremap(SP_GIC_BASE, SZ_256);
	BUG_ON(!sp_irq_base);

	gic_init(0, 29, gic_dist_base_addr, sp_irq_base);
}

static struct map_desc sp_io_desc[] __initdata = {
	{
		.virtual	= IO0_BASE,
		.pfn		= __phys_to_pfn(IO0_START),
		.length		= IO0_SIZE,
		.type		= MT_DEVICE
	},
#ifdef GPS_VENDOR_REGISTER
	{
		.virtual	= IO1_BASE,
		.pfn		= __phys_to_pfn(IO1_START),
		.length		= IO1_SIZE,
		.type		= MT_DEVICE
	},
	{
		.virtual	= IO3_BASE,
		.pfn		= __phys_to_pfn(IO3_START),
		.length		= IO3_SIZE,
		.type		= MT_DEVICE
	},
#endif
	{
		.virtual	= L2C_BASE,
		.pfn		= __phys_to_pfn(L2C_START),
		.length		= L2C_SIZE,
		.type		= MT_DEVICE
	},
	{
		.virtual	= SRAM0_BASE,
		.pfn		= __phys_to_pfn(SRAM0_START),
		.length 	= SRAM0_SIZE,
		/* modify the type in order to execute c code on SRAM. */
		.type		= MT_MEMORY_NONCACHED
	},
	{
		.virtual	= SRAM1_BASE,
		.pfn		= __phys_to_pfn(SRAM1_START),
		.length 	= SRAM1_SIZE,
		.type		= MT_DEVICE
	},
	{
		.virtual	= SCU_BASE,
		.pfn		= __phys_to_pfn(SCU_START),
		.length 	= SCU_SIZE,
		.type		= MT_DEVICE
	},
	{
		.virtual	= AHB_USBD_BASE,
		.pfn		= __phys_to_pfn(AHB_USBD_START),
		.length 	= AHB_USBD_SIZE,
		.type		= MT_DEVICE
	},
	{
		.virtual	= AHB_BCH_BASE,
		.pfn		= __phys_to_pfn(AHB_BCH_START),
		.length 	= AHB_BCH_SIZE,
		.type		= MT_DEVICE
	},
	{
		.virtual	= AHB_NAND_BASE,
		.pfn		= __phys_to_pfn(AHB_NAND_START),
		.length 	= AHB_NAND_SIZE,
		.type		= MT_DEVICE
	},
	{
		.virtual	= AHB_SPU_BASE,
		.pfn		= __phys_to_pfn(AHB_SPU_START),
		.length 	= AHB_SPU_SIZE,
		.type		= MT_DEVICE
	},
	{
		.virtual	= AHB_SCALER_BASE,
		.pfn		= __phys_to_pfn(AHB_SCALER_START),
		.length 	= AHB_SCALER_SIZE,
		.type		= MT_DEVICE
	},
	{
		.virtual	= AHB_PPU_BASE,
		.pfn		= __phys_to_pfn(AHB_PPU_START),
		.length 	= AHB_PPU_SIZE,
		.type		= MT_DEVICE
	},
	{
		.virtual	= AXI_SGX540_BASE,
		.pfn		= __phys_to_pfn(AXI_SGX540_START),
		.length 	= AXI_SGX540_SIZE,
		.type		= MT_DEVICE
	},
};

static void __init sp_map_io(void)
{
	printk("%s\n", __func__);

	iotable_init( sp_io_desc, ARRAY_SIZE( sp_io_desc));
#if defined(SUPPORT_SP_TIMESTAMP)
        SP_TIMESTAMP("Kernel Start", 158000, USE_EARLY_PRINTK);
#endif

//	sp_register_baseclocks(0);
//	sp_setup_clocks();
}

void __init sp_reserve(void)
{
	//uint32_t size,pa_reserve;
	//struct membank *bank;
	//void __iomem *regs = (void __iomem *)SYSTEM_BASE;
	//uint32_t cfg7 = readl(regs + 0x9c);

	printk("%s\n", __func__);
	default_memory_reservation();

	return;
}

#if defined(CONFIG_MTD_SP_NAND)
/*
 * Sunplus NAND controller
 */
static struct resource sp_nand_res[] = {
	{
		.start = 0x9C100000,
		.end   = 0x9C100FFF,
		.flags = IORESOURCE_MEM,
	}, {
		.start = SP_IRQ_NAND,
		.end   = SP_IRQ_NAND,
		.flags = IORESOURCE_IRQ,
	},
};

static struct platform_device sp_nand_pdev = {
	.name 		= "sp_nand",
	.id		= 0,
	.num_resources 	= ARRAY_SIZE(sp_nand_res),
	.resource	= sp_nand_res,
};

static int __init sp_nand_init(void)
{
	/*
	void *hdr = (void *)(SRAM0_BASE + 0x99f8);

	if (readl(hdr) == 0x334b3847)
		sp_nand_pdev.dev.platform_data = hdr;
	else
		printk("letb123: boot header not found\n");
	*/

	platform_device_register(&sp_nand_pdev);
	return 0;
}

/* Sunplus SPI-NAND controller  */
static struct resource sp_spinand_res[] = {
	{
		.start = 0x9C002B80, //uboot's CONFIG_SRAM_BASE ,according register file
		.end   = 0x9C002BFF,
		.flags = IORESOURCE_MEM,
	}, {
		.start =  0x0,
		.end   =  0x0,
		.flags = IORESOURCE_IRQ,
	},
};

static struct platform_device sp_spinand_pdev = {
	.name = "sp_spinand",
	.id = 0,
	.num_resources = ARRAY_SIZE(sp_spinand_res),
	.resource = sp_spinand_res,
};

static int __init sp_spinand_init(void)
{

	platform_device_register(&sp_spinand_pdev);
	return 0;
}
/* Sunplus SPI-NAND controller  */
#else
static int __init sp_nand_init(void)
{
}

static int __init sp_spinand_init(void)
{
}

#endif

static int __init sp_late_init(void)
{
	sp_nand_init();
	sp_spinand_init();
	return 0;
}

late_initcall(sp_late_init);

static void __init sp_fixup(struct tag *tags, char **from,
	struct meminfo *meminfo)
{
//	void __iomem *regs = (void __iomem *)SYSTEM_BASE;
//	uint32_t cfg7 = readl(regs + 0x9c);

	/*
	 * Default memory configuration
	 * It might be later modified by early_mem() in setup.c.
	 */
	meminfo->nr_banks = 1;
	meminfo->bank[0].start = PHYS_OFFSET;
	// meminfo->bank[0].size  = 1 << (27 + ((cfg7 >> 21) & 3));
	meminfo->bank[0].size  = SZ_256M;
	bank_number = 1;

#if 0 /* This is still buggy, be carefull ! */
	if (!(readl(regs + 0x1888) & 0x0D00)) {
		meminfo->nr_banks = 2;
		meminfo->bank[1].start = PHYS_OFFSET + meminfo->bank[0].size;
		meminfo->bank[1].size  = 1 << (27 + ((cfg7 >> 23) & 3));
		bank_number = 2;
	}
#endif
}

static void sp_restart(char str, const char *cmd)
{
	void __iomem *regs = (void __iomem *)SYSTEM_BASE;

	/* MOON1: enable watchdog reset */
        writel(BIT(10) | BIT(1), regs + 0xA8); // G1.10

	/* STC: watchdog control */
	writel(0x3877, regs + 0x0630); /* stop */
	writel(0xAB00, regs + 0x0630); /* unlock */
	writel(0x0001, regs + 0x0634); /* counter */
	writel(0x4A4B, regs + 0x0630); /* resume */
}

extern struct sys_timer sp_timer;

MACHINE_START(GEMINI, "GEMINI")
	.map_io		= sp_map_io,
	.fixup		= sp_fixup,
	.restart	= sp_restart,
	.reserve	= sp_reserve,
	.init_irq	= sp_init_irq,
	.init_machine	= sp_init,
	.handle_irq	= gic_handle_irq,
	.timer		= &sp_timer,
MACHINE_END


