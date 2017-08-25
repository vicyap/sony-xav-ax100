#include <linux/platform_device.h>
#include <asm/pmu.h>
#include <mach/irqs.h>

static struct resource sp_pmu_resource = {
	.start	= SP_IRQ_PMU0,
	.end		= SP_IRQ_PMU0,
	.flags	= IORESOURCE_IRQ,
};

static struct platform_device sp_pmu_device = {
	.name		= "arm-pmu",
	.id		= ARM_PMU_DEVICE_CPU,
	.resource	= &sp_pmu_resource,
	.num_resources	= 1,
};

static int __init sp_pmu_init(void)
{
	platform_device_register(&sp_pmu_device);
	return 0;
}

arch_initcall(sp_pmu_init);
