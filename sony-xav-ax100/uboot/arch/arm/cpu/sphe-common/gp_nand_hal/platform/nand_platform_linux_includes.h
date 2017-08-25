#ifndef _NAND_PLATFORM_LINUX_H
#define _NAND_PLATFORM_LINUX_H

#if (NAND_RUN_OS_IS == NAND_OS_LINUX || NAND_RUN_OS_IS == NAND_OS_UCLINUX)

#include <linux/version.h>
#include <mach/typedef.h>
#include <asm/cacheflush.h>

#if (NAND_RUN_OS_IS == NAND_OS_LINUX)
#include <mach/general.h>
#include <mach/diag.h>
#if (ASIC_NUM != ASIC_83000)
#if (ASIC_NUM != ASIC_32900)
// Mason@2015-11-17 #include <mach/clk/gp_clk_core.h>
#endif
// Mason@2015-11-17 #include <mach/clock_mgr/gp_clock.h>
// Mason@2015-11-17 #include <mach/gp_version.h>
#endif
#endif

#if (ASIC_NUM != ASIC_83000)
#include <mach/gp_cache.h>
#endif

// 2016-03-08 #include <mach/gp_board.h>
#include <mach/kernel.h>
#include <mach/module.h>
#include <mach/cdev.h>
#include <mach/irqs.h>

#include <linux/sched.h>
#include <linux/interrupt.h>
#include <linux/delay.h>        // ndelay
#include <linux/slab.h>         /* kmalloc() */
#include <linux/dma-mapping.h>
#include <linux/kernel.h>       /* printk() */
#include <linux/hdreg.h>        /* hd_geometry */
#include <linux/vmalloc.h>      /* vfree */
#include <linux/blkdev.h>       /* blk_fetch_request */
//#include <linux/string.h>
//#include <linux/uaccess.h>

#endif 
#endif // _NAND_PLATFORM_LINUX_H
