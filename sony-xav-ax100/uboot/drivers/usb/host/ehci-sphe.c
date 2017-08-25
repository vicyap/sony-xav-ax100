/*
 * (C) Copyright 2014
 * Sunplus Technology
 *
 * SPDX-License-Identifier:     GPL-2.0+
 */

#include <common.h>
#include <asm/io.h>
#include <usb.h>
#include "ehci.h"
#include <asm/arch/regmap.h>

#define CONFIG_USB_EHCI_BASE ((uint32_t) EHCI0_REG)

#ifndef CONFIG_USB_EHCI_BASE_LIST
#define CONFIG_USB_EHCI_BASE_LIST	{ EHCI0_REG, EHCI1_REG }
#endif


#ifndef CONFIG_PLATFORM_letb123

int uphy_initialized;

#define _delay_1ms(_n) mdelay((_n))

#define DEFAULT_UPHY_DISC   0xd   // 13 (=619.5mV)
#define ORIG_UPHY_DISC      0xb   // 11 (=586.5mV)

// UPHY 0 & 1 init (dh_feng)
void uphy_init(void)
{
        volatile unsigned int *reg;
	unsigned int val, set;

        // 1. Default value modification
        MOON1_REG->sft_cfg[14] = 0x87474002;
        MOON1_REG->sft_cfg[15] = 0x87474004;

        // 2. PLL power off/on twice
        MOON1_REG->sft_cfg[21] = 0x8888;
        _delay_1ms(1);
        MOON1_REG->sft_cfg[21] = 0x8080;
        _delay_1ms(1);
        MOON1_REG->sft_cfg[21] = 0x8888;
        _delay_1ms(1);
        MOON1_REG->sft_cfg[21] = 0x8080;
        _delay_1ms(1);
        MOON1_REG->sft_cfg[21] = 0;

        // 3. reset UPHY0/1
        MOON0_REG->reset[1] |= (3 << 13);
        MOON0_REG->reset[1] &= ~(3 << 13);
        _delay_1ms(1);

        // 4. UPHY 0/1 internal register modification
        reg = (volatile unsigned int *)RF_GRP(149, 7);
        *reg = 0x8b;
        reg = (volatile unsigned int *)RF_GRP(150, 7);
        *reg = 0x8b;

        // 5. USBC 0/1 reset
        MOON0_REG->reset[1] |= (3 << 10);
	MOON0_REG->reset[1] &= ~(3 << 10);

        // Backup solution to workaround real IC USB clock issue
        if (HB_GP_REG->hb_otp_data2 & 0x1) { // G350.2 bit[0]
                MOON1_REG->sft_cfg[19] |= (1 << 6);
        }
        if (HB_GP_REG->hb_otp_data2 & 0x2) { // G350.2 bit[1]
                MOON1_REG->sft_cfg[19] |= (1 << 14);
        }

	//
	// OTP for USB DISC (disconnect voltage)
	//
	val = HB_GP_REG->hb_otp_data6;

	// UPHY0 DISC
	set = val & 0x1F;
	if (!set) {
		set = DEFAULT_UPHY_DISC;
	} else if (set <= ORIG_UPHY_DISC) {
		set += 2;
	}
	UPHY0_RN_REG->cfg[7] = (UPHY0_RN_REG->cfg[7] & ~0x1F) | set;

	// UPHY1 DISC
	set = (val >> 5) & 0x1F;
	if (!set) {
		set = DEFAULT_UPHY_DISC;
	} else if (set <= ORIG_UPHY_DISC) {
		set += 2;
	}
	UPHY1_RN_REG->cfg[7] = (UPHY1_RN_REG->cfg[7] & ~0x1F) | set;
}

void usb_power_init(int is_host)
{
        // a. enable pin mux control (sft_cfg_8, bit2/bit3)
        //    Host: enable
        //    Device: disable
        if (is_host) {
                MOON1_REG->sft_cfg[8] |= (3 << 2);
        } else {
                MOON1_REG->sft_cfg[8] &= ~(3 << 2);
        }

        // b. USB control register:
        //    Host:   ctrl=1, host sel=1, type=1
        //    Device  ctrl=1, host sel=0, type=0
        if (is_host) {
                MOON2_REG->sft_cfg[3] |= ((7 << 12) | (7 << 4));
        } else {
                MOON2_REG->sft_cfg[3] |= ((1 << 12) | (1 << 4));
                MOON2_REG->sft_cfg[3] &= ~((3 << 13) | (3 << 5));
        }
}
#endif

/*
 * Create the appropriate control structures to manage
 * a new EHCI host controller.
 */
int ehci_hcd_init(int index, enum usb_init_type init,
		struct ehci_hccr **ret_hccr, struct ehci_hcor **ret_hcor)
{
	struct ehci_hccr *hccr;
	struct ehci_hcor *hcor;
	volatile struct ehci_regs *base_list[] = CONFIG_USB_EHCI_BASE_LIST;
	volatile struct ehci_regs *regs;

	if (index < 0 || index >= ARRAY_SIZE(base_list))
		return -1;

	debug("ehci[%d] base=%p\n", index, (void *)base_list[index]);

	regs = (volatile struct ehci_regs *)base_list[index];

	hccr = (struct ehci_hccr *)(&regs->ehci_len_rev);
	hcor = (struct ehci_hcor *)(&regs->ehci_usbcmd);

#ifdef CONFIG_PLATFORM_letb123
	if(index == 1){
		// UPHY1_DIG_DPDM_INV = 0
		clrbits_le32(&MOON1_REG->sft_cfg[20], BIT(26)); // 0x9c0000d0;
	}

	if(index == 2){
		// UPHY2_USB0_SEL = 1 (host)
		setbits_le32(&MOON1_REG->sft_cfg[10], BIT(4)); // 0x9c0000a8;
	}

	/* Make sure PHY is in normal state */
	if (!(readl(&regs->ehci_sys_debug) & BIT(0))) {
		setbits_le32(&regs->ehci_sys_debug, BIT(0));
		mdelay(3);
	}

	/* Make sure MBUS mode is enabled */
	clrbits_le32(&regs->ehci_sys_debug, BIT(1));

	/* Max. sleep counter */
	setbits_le32(&regs->ehci_sleep_cnt, 0x0fff);
#else
	// init uphy & power
	if (!uphy_initialized) {
		uphy_init();
		usb_power_init(1);
		uphy_initialized = 1;
	}
#endif

	*ret_hccr = hccr;
	*ret_hcor = hcor;
	return 0;
}

/*
 * Destroy the appropriate control structures corresponding
 * the the EHCI host controller.
 */
int ehci_hcd_stop(int index)
{
#ifndef CONFIG_PLATFORM_letb123
	if (uphy_initialized) {
		usb_power_init(0);
		uphy_initialized = 0;
	}
#endif
	return 0;
}
