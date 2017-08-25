/*
 * (C) Copyright 2014
 * Sunplus Technology
 *
 * SPDX-License-Identifier:     GPL-2.0+
 */

#include <common.h>
#include <malloc.h>
#include <linux/err.h>
#include <linux/clk.h>
#include <linux/clkdev.h>
#include <linux/sizes.h>
#include <asm/io.h>
#include <asm/arch/regmap.h>
#include <sp_bootinfo.h>
#include <asm/arch/hardware.h>
#include <nand.h>

extern void board_pnand_init(void);

#ifdef CONFIG_SP_SPINAND
extern void board_spinand_init(void);
#endif

#ifdef CONFIG_SP_MMC
#include <asm/arch-gemini/sp_sdmmc_defs.h>
#endif

DECLARE_GLOBAL_DATA_PTR;

int board_init(void)
{
	gd->bd->bi_arch_number = CONFIG_MACH_TYPE;
	gd->bd->bi_boot_params = CONFIG_BOOT_PARAM_ADDR;
	return 0;
}

int dram_init(void)
{
	gd->ram_size = CONFIG_SYS_SDRAM_SIZE;
	return 0;
}

void dram_init_banksize(void)
{
	gd->bd->bi_dram[0].start = CONFIG_SYS_SDRAM_BASE;
	gd->bd->bi_dram[0].size  = CONFIG_SYS_SDRAM_SIZE;
}

#ifdef CONFIG_SP_SD
/* 1. Switch SD Card Host Controller clk source to 270Mhz
 * 2. Register SD Card Slots (If pin mux are set)
 */
int board_mmc_init(bd_t *bis)
{
	static u32 dcg_n = 2, clk_sel = 5;

	/* Set SD host controller's clk source to 270Mhz */
	/* Used to set MO_CLK_CARD_SEL, G0.4[16] */
	MOON0_REG->clk_sel[0]  |= (1 << 16);
	/* Setting DCG2's clk source frequency */
	MOON1_REG->sft_cfg[30] &= ~(0x0F << (4 * dcg_n));
	MOON1_REG->sft_cfg[30] |= (clk_sel & 0x0F) << (4 * dcg_n);

	/* Initialize SD Card slot 0 */
	spsdv2_mmc_controller_init(bis, 0);

	/* Initialize SD Card slot 1 */
	spsdv2_mmc_controller_init(bis, 1);
	return 0;
}
#endif

#ifdef CONFIG_SP_MMC

static struct sp_mmc mmc_sd0 = {
	.reg_base	= (struct sp_mmc_regs *)CONFIG_SP_SD0_BASE,
	.id = 0,
	.name = "spsdc0",
	.pinmuxshift = 0,
};

static struct sp_mmc mmc_sd1 = {
	.reg_base	= (struct sp_mmc_regs *)CONFIG_SP_SD1_BASE,
	.id = 1,
	.name = "spsdc1",
	.pinmuxshift = 6,
};

static struct sp_mmc mmc_sd2 = {
	.reg_base	= (struct sp_mmc_regs *)CONFIG_SP_SD2_BASE,
	.id = 2,
	.name = "spsdc2",
	.pinmuxshift = 10,
};

int board_mmc_init(bd_t *bis)
{
	int err=0;
	printf("uboot, %s\n", __func__);

	if((err = sp_mmc_init(bis, &mmc_sd0)))
		return err;

	if((err = sp_mmc_init(bis, &mmc_sd1)))
		return err;

	if((err = sp_mmc_init(bis, &mmc_sd2)))
		return err;

{
	int pinmux = get_val(SPHE_DEVICE_BASE+0x88);
	printf("%s,sft2 pinmux:0x%x\n", __func__, pinmux);
}
	return err;
}

#endif // CONFIG_SP_MMC

// pinmux sft_cfg1[1:0]
#define SET_SNOR_PINMUX(_pin_x) do { \
                MOON1_REG->sft_cfg[1] = ((MOON1_REG->sft_cfg[1] & ~0x3) | (_pin_x)); \
        } while (0)

// pinmux sft_cfg1[3:2]
#define SET_SNAND_PINMUX(_pin_x) do { \
        MOON1_REG->sft_cfg[1] = ((MOON1_REG->sft_cfg[1] & ~0xC) | ((_pin_x) << 2)); \
        } while (0)

// pinmux sft_cfg1[4]
#define SET_PNAND_PINMUX(_pin_x) do { \
        MOON1_REG->sft_cfg[1] = ((MOON1_REG->sft_cfg[1] & ~0x10) | ((_pin_x) << 4)); \
        } while (0)

void SetBootDev(unsigned int bootdev, unsigned int pin_x)
{
	debug("%s: bootdev=0x%x pin=%d\n", __func__, bootdev, pin_x);

        switch(bootdev) {
                default:
                        printf("unknown type!\n");
                case DEVICE_NAND:
#ifdef CONFIG_PLATFORM_letb123
                        MOON1_REG->sft_cfg[6] |= (1<<11); /* switch NAND pin mux on */
                        MOON1_REG->sft_cfg[2] &= ~(1<<14); /* switch SD/eMMC pin mux off */

                        PAD_CTL_REG->pad_ctrl[3] |= 0x1fffe000; //G4.3[28..13] set 1, NAND pin slew rate set fast
#else
                        SET_PNAND_PINMUX(1);
                        SET_SNAND_PINMUX(0); // PNAND_X1 ON -> SNAND_X2 must be OFF

                        // NAND pin: high slew rate mode
                        PAD_CTL_REG->pad_ctrl[3] |= (0x7fff << 7);
                        PAD_CTL_REG->pad_ctrl[4] |= (1 << 1);
#endif
                        break;
		case DEVICE_SPI_NOR:
			SET_SNOR_PINMUX(pin_x);
			break;
                case DEVICE_SPI_NAND:
#ifndef CONFIG_PLATFORM_letb123
                        // SNAND_X1 ON -> SNOR_X1 must be OFF
                        if (pin_x == 1) {
                                SET_SNOR_PINMUX(2);
                        }
                        SET_SNAND_PINMUX(pin_x);

                        // Undo high slew rate mode of PNAND pins
                        PAD_CTL_REG->pad_ctrl[3] &= ~(0x7fff << 7);
                        PAD_CTL_REG->pad_ctrl[4] &= ~(0x1 << 1);
#endif
                        break;
        }
}

// Called from drivers/mtd/nand/nand.c
void board_nand_init(void)
{
	struct sp_bootinfo *bootinfo = SP_GET_BOOTINFO();

	// For ISP boot, bootdev will be USB/SDCARD
	// For normal boot, bootdev will be NAND/SPINAND
	debug("Bootdev=%u\n", bootinfo->bootdev);

	// 1. Detect Parallel NAND
	if (bootinfo->bootdev != DEVICE_SPI_NAND) {
		printf("Init Parallel NAND\n");
		SetBootDev(DEVICE_NAND, 1);
		board_pnand_init();

		if (nand_curr_device != -1) {
			return; // detected
		}
		printf("No parallel nand\n");
	}

#ifdef CONFIG_SP_SPINAND
	// 2. Detect SPI NAND
	if (bootinfo->bootdev != DEVICE_NAND) {
		printf("Init SPI NAND\n");
		SetBootDev(DEVICE_SPI_NAND, 1 + (bootinfo->hw_bootmode == SPI_NAND_X2_BOOT));
		board_spinand_init();
		if (nand_curr_device != -1) {
			return; // detected
		}
		printf("No spi nand\n");
		SetBootDev(DEVICE_SPI_NOR, 1 + (bootinfo->hw_bootmode == SPI_NOR_X2_BOOT));
	}
#endif
}

int board_should_load_env(void)
{
	struct sp_bootinfo *bootinfo = SP_GET_BOOTINFO();

	// Load env only if we're booting from NAND, SPI_NAND, or Romter
	//
	// For other boot devices (eg. USB, SDCARD), don't load env to simplify
	// u-boot behavior in ISP mode
	if (bootinfo->bootdev == DEVICE_NAND ||
		bootinfo->bootdev == DEVICE_SPI_NAND ||
		bootinfo->bootdev == DEVICE_SPI_NOR) {
		return 1;
	}

	printf("skip env load\n");
	return 0;
}
