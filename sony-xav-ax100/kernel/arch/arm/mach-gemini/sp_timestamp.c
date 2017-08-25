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

/*
 * STC_AV2
 */
unsigned int AV1_GetStc32(void)
{
        unsigned int stc_value = 0;
        stc_avReg_t *pstc_AV1Reg = (stc_avReg_t *)LOGI_ADDR_STC_AV1_REG;

        /**
         * strictly read sequence, should not change this order.
         */
        // smart latch
        stc_value = pstc_AV1Reg->stc_15_0;
        stc_value = 0;

        // read
        stc_value |= (unsigned int)pstc_AV1Reg->stc_31_16 << 16;
        stc_value |= pstc_AV1Reg->stc_15_0;

        return stc_value;
}

void SP_TIMESTAMP( char *s, unsigned int th , unsigned char use_early_printk )
{
        #define COLOR_RED    "\033[0;1;31;40m"
        #define COLOR_GREEN  "\033[0;1;32;40m"
        #define COLOR_YELLOW "\033[0;1;33;40m"
        #define COLOR_ORIGIN "\033[0m"
        unsigned int val = AV1_GetStc32();
        if(use_early_printk)
        {
          if ( th < val )
           early_printk("%s[TS] %s %d ms%s\n", COLOR_RED, s, val/90, COLOR_ORIGIN);
          else
           early_printk("%s[TS] %s %d ms%s\n", COLOR_GREEN, s, val/90, COLOR_ORIGIN);
        }
        else
        {
          if ( th < val )
            printk("%s[TS] %s %d ms%s\n", COLOR_RED, s, val/90, COLOR_ORIGIN);
          else
            printk("%s[TS] %s %d ms%s\n", COLOR_GREEN, s, val/90, COLOR_ORIGIN);
        }
}
