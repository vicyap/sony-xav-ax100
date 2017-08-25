#ifndef _USB_POWER_H_
#define _USB_POWER_H_

#include <mach/gpio_drv.h>
#include <mach/hardware.h>
#include <linux/io.h>

#define		VBUS_GPIO_CTRL_1		(87)
#define		VBUS_GPIO_CTRL_0		(7)

typedef enum {
	eHW_GPIO_FIRST_FUNC = 0,
	eHW_GPIO_FIRST_GPIO = 1,
	eHW_GPIO_FIRST_NULL
} eHW_GPIO_FIRST;

typedef enum {
	eHW_GPIO_IOP = 0,
	eHW_GPIO_RISC = 1,
	eHW_GPIO_MASTER_NULL
} eHW_GPIO_Master;

typedef enum {
	eHW_GPIO_IN = 0,
	eHW_GPIO_OUT = 1,
	eHW_GPIO_OE_NULL
} eHW_GPIO_OE;

typedef enum {
	eHW_GPIO_STS_LOW = 0,
	eHW_GPIO_STS_HIGH = 1,
	eHW_GPIO_STS_NULL
} eHW_IO_STS;

#define Set_usb_vbus(gpio,Val)	do { \
							GPIO_F_SET((gpio), eHW_GPIO_FIRST_GPIO);	\
							GPIO_M_SET((gpio), eHW_GPIO_RISC);			\
							GPIO_E_SET((gpio), eHW_GPIO_OUT);			\
							GPIO_O_SET((gpio), (Val)&0x01);				\
						} while(0)

const static u8 USB_PORT_NUM[2] = {VBUS_GPIO_CTRL_0, VBUS_GPIO_CTRL_1};

#define	Enable_vbus_power(port)	Set_usb_vbus(USB_PORT_NUM[port], eHW_GPIO_STS_LOW)
#define	Disable_vbus_power(port)	Set_usb_vbus(USB_PORT_NUM[port], eHW_GPIO_STS_HIGH)

#define	VBUS_POWER_MASK	(1 << 0)
#define	VBUS_RESET		(1 << 1)

#define	UPHY_DISC_OFFSET	(10)
#define	UPHY_DISC_0			(1 << 2)

static inline void uphy_force_disc(int en, int port)
{
	volatile u32 *uphy_disc;
	u32 uphy_val;

	if(port > 2)
		printk("-- err port num %d\n", port);

	uphy_disc = (u32*)IO0_ADDRESS((149 + port)*32*4);
	uphy_val = ioread32(uphy_disc + UPHY_DISC_OFFSET);

	if(en) {
		uphy_val |= UPHY_DISC_0;
	}
	else {
		uphy_val &= ~UPHY_DISC_0;
	}

	iowrite32(uphy_val, uphy_disc + UPHY_DISC_OFFSET);
}

#define Reset_Usb_Power(port)	do {	\
									Disable_vbus_power(port);	\
									uphy_force_disc(1, port);	\
									msleep(500);	\
									uphy_force_disc(0, port);	\
									Enable_vbus_power(port);	\
								} while(0)

#endif /* _USB_POWER_H_ */

