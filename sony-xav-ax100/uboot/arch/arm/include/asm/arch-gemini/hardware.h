#ifndef __ASM_ARCH_HARDWARE_H
#define __ASM_ARCH_HARDWARE_H

#include <linux/sizes.h>

#ifndef BIT
#define BIT(n)                          (1 << (n))
#endif

#define	REG(addr)	(*(volatile unsigned int *)(addr))
#define REG_P(addr)	((volatile unsigned int *)(addr))


#define SPHE_DEVICE_BASE       0x9C000000
#define RF_GRP(_grp, _reg)     ((((_grp)*32 + (_reg))*4) + SPHE_DEVICE_BASE)

#define AHB0_REG_BASE     0x9c100000
#define AHB_GRP(_ahb_grp, _grp, _reg) \
        ((((_grp) * 32 + (_reg)) * 4) + ((_ahb_grp) * 0x1000) + AHB0_REG_BASE)

/* L2 cache PL310 */
#define ADDRESS_PL310          (0x9f000000)

//FIXME: replace with RF_GRP() below

/* SPI Flash */
#define SPHE_SPI_FLASH_BASE             0x98000000

/* DMA */
#define CONFIG_SP_CBDMA_BASE            0x9c000d00

/* SRAM */
#define CONFIG_SRAM_BASE                0x9e800000

/* MOON (System Control) */
//#define CONFIG_SP_MOON_BASE             0x9c000000

/* Timer */
//#define CONFIG_SP_TIMER_BASE            0x9c000600

/* UART */
//#define CONFIG_SP_SERIAL_BASE           0x9c000900

/* NAND */
#define CONFIG_SP_NAND_SRAM_BASE        CONFIG_SRAM_BASE

/* SD */
#define CONFIG_SP_SD0_BASE             (SPHE_DEVICE_BASE+(118*32*4))
#define CONFIG_SP_SD1_BASE             (SPHE_DEVICE_BASE+(125*32*4))

#endif	/* __ASM_ARCH_HARDWARE_H */
