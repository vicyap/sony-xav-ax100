#ifndef _SP_CLK_H_
#define _SP_CLK_H_

#include <linux/types.h>
#define SP_CLK_SYSSLOW           (0)
#define SP_CLK_SYSFAST           (1)
#define SP_CLK_CA9               (2)
#define SP_CLK_ARM926            (3)
#define SP_CLK_SPI_NAND          (4)
#define SP_CLK_SPI_NOR           (5)
#define SP_CLK_SDCARD            (6)
#define SP_CLK_SDIO              (7)
#define SP_CLK_MFD               (8)
#define SP_CLK_CA9_TRACER        (9)
#define SP_CLK_DRAM             (10)
#define SP_CLK_AUDIO_DSP        (11)
#define SP_CLK_AUDIO_DPLL2      (12)
#define SP_CLK_AUDIO_PLLA       (13)
#define SP_CLK_AUDIO_PLLSPDIF   (14)
#define SP_CLK_DISPLAY          (15)

extern u32 sp_clk_get(u32 which);
extern u32 sp_clk_set(u32 which, u32 freq);

#endif /* _SP_CLK_H_ */
