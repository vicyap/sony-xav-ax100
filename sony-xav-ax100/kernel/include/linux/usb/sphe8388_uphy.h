#ifndef __LINUX_letb8388_UPHY_H_
#define __LINUX_letb8388_UPHY_H_

#include <asm/io.h>
#include <mach/irqs.h>
#include <mach/hardware.h>

//u32 sony_board_swing[] = {85,89};

#define SWING_REG_OFFSET	(2)

static inline int Get_uphy_swing(int port)
{
	u32 *swing_reg;
	u32 val;

	if(port > 2) {
		return -1;
	}

	swing_reg = (u32*)IO0_ADDRESS(2*32*4);
	val = ioread32(swing_reg + SWING_REG_OFFSET);

	return (val >> (port*8))&0xff;
}

static inline int Set_uphy_swing(u32 swing, int port)
{
	u32 *swing_reg;
	u32 val;

	if(port > 2) {
		return -1;
	}

	swing_reg = (u32*)IO0_ADDRESS(2*32*4);

	val = ioread32(swing_reg + SWING_REG_OFFSET);
	val &= ~(0x3f << (port*8));
	val |= (swing&0x3f) << (port*8);
	val |= 1 << (port*8+7);
	iowrite32(val, swing_reg + SWING_REG_OFFSET);

	return 0;
}
#define USB_UPHY_OTG_REG	(3)

/*
*	return : 0 = device,  1 = host
*/
static inline int Get_uphy_owner(int port)
{
	volatile u32 *grop = (u32 *) IO0_ADDRESS(2 * 32 * 4);
	u32 val;

	val = ioread32(grop + USB_UPHY_OTG_REG);

	return (val&(1 <<( port * 8 + 5)))? 1 : 0;
}

#define UPHY_DISCONNECT_LEVEL	(0x8B)
static u32 uphy_disc_level = UPHY_DISCONNECT_LEVEL; // ns (0 for disable)
module_param(uphy_disc_level, uint, 0644);

static void reinit_uphy(int port)
{
	volatile u32 *uphy_base = (u32*)IO0_ADDRESS((149 + port) * 32 * 4);
#ifdef CONFIG_USB_BC
	volatile u32 *bc_cdp_addr;
	volatile u32 *bc_ref_addr;
#endif

	u32 val;

	val = ioread32(uphy_base + 9);
	val |= (1 << 6);
	iowrite32(val, uphy_base + 9);

	val = ioread32(uphy_base + 1);
	val &= ~(1 << 5);
	iowrite32(val, uphy_base + 1);

	iowrite32(uphy_disc_level, uphy_base + 7);

#ifdef CONFIG_USB_BC
	bc_cdp_addr = (u32*)IO0_ADDRESS((149 + port) * 32 * 4) + 16;
	bc_ref_addr = (u32*)IO0_ADDRESS((149 + port) * 32 * 4) + 17;

	val = ioread32(bc_cdp_addr);
	val |= (1u)|(3 << 3);
	iowrite32(val, bc_cdp_addr);

	val = ioread32(bc_ref_addr);
	val = 0x92;
	iowrite32(val, bc_ref_addr);

	iowrite32(0x17, bc_cdp_addr - 16 + 3);
#endif
}

#endif /* __LINUX_letb8388_UPHY_H_ */
