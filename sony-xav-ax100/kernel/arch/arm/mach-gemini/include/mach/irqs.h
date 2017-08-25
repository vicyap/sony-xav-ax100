
#ifndef __ASM_ARCH_IRQS_H
#define __ASM_ARCH_IRQS_H

#define SP_IRQ_LOCALTIMER			29
//#define SP_IRQ_LOCALTIMER			13
#define SP_IRQ_LOCALWDT				30
#define SP_IRQ_GIC_START			32

#define FIQ_START	0

/*
 * These are decoded via the IP bits of the cause
 * register when an external interrupt is delivered.
 */

// CPU level interrupts
#define SP_IRQ_GMAC               (SP_IRQ_GIC_START + 11)
#define SP_IRQ_NAND               (SP_IRQ_GIC_START + 12)
#define SP_IRQ_OTG_0				(SP_IRQ_GIC_START + 11)
#define SP_IRQ_OTG_1				(SP_IRQ_GIC_START + 10)
#define SP_IRQ_EHCI_USB0			(SP_IRQ_GIC_START + 14)	// E
#define SP_IRQ_OHCI_USB0			(SP_IRQ_GIC_START + 15)	// E
#define SP_IRQ_EHCI_USB1			(SP_IRQ_GIC_START + 17)	// E
#define SP_IRQ_OHCI_USB1			(SP_IRQ_GIC_START + 18)	// E
#define SP_IRQ_EHCI_USB2			(SP_IRQ_GIC_START + 19)	// E
#define SP_IRQ_OHCI_USB2			(SP_IRQ_GIC_START + 23)	// E

#define SP_IRQ_IMGDEC_SND_INT	(SP_IRQ_GIC_START + 31)	// edge
#define SP_IRQ_IMGDEC_PRI_INT	(SP_IRQ_GIC_START + 34)	// edge

#define SP_IRQ_IOP_2_MIPZ_INT0	(SP_IRQ_GIC_START + 41)
#define SP_IRQ_IOP_2_MIPZ_INT1	(SP_IRQ_GIC_START + 42)

#define SP_IRQ_V656IN_INT0 		(SP_IRQ_GIC_START + 49)
#define SP_IRQ_V656IN_INT1 		(SP_IRQ_GIC_START + 50)
#define SP_IRQ_V656IN_INT2 		(SP_IRQ_GIC_START + 51)
#define SP_IRQ_V656IN_INT3 		(SP_IRQ_GIC_START + 52)

#define SP_IRQ_CARD_CTL0_INT		(SP_IRQ_GIC_START + 20)
#define SP_IRQ_CARD_CTL1_INT              (SP_IRQ_GIC_START + 21)

#define SP_IRQ_UART0				(SP_IRQ_GIC_START + 53)	// level
#define SP_IRQ_UART1				(SP_IRQ_GIC_START + 54)	// level
#define SP_IRQ_UART2				(SP_IRQ_GIC_START + 55)	// level
#define SP_IRQ_UART3				(SP_IRQ_GIC_START + 56)	// level
#define SP_IRQ_BCH				(SP_IRQ_GIC_START + 58)

#define SP_IRQ_GPIO_INT0 		(SP_IRQ_GIC_START + 120)
#define SP_IRQ_GPIO_INT1 		(SP_IRQ_GIC_START + 121)
#define SP_IRQ_GPIO_INT2 		(SP_IRQ_GIC_START + 122)
#define SP_IRQ_GPIO_INT3 		(SP_IRQ_GIC_START + 123)
#define SP_IRQ_GPIO_INT4 		(SP_IRQ_GIC_START + 124)
#define SP_IRQ_GPIO_INT5 		(SP_IRQ_GIC_START + 125)
#define SP_IRQ_GPIO_INT6 		(SP_IRQ_GIC_START + 126)
#define SP_IRQ_GPIO_INT7 		(SP_IRQ_GIC_START + 127)

#define SP_IRQ_CBDMA0				(SP_IRQ_GIC_START + 128)	// level
#define SP_IRQ_CBDMA1				(SP_IRQ_GIC_START + 129)	// level

#define SP_IRQ_SAR4				(SP_IRQ_GIC_START + 131)	// level
#define SP_IRQ_UART4				(SP_IRQ_GIC_START + 134)	// level
#define SP_IRQ_UART5				(SP_IRQ_GIC_START + 135)	// level
#define SP_IRQ_DMA0				(SP_IRQ_GIC_START + 138)	// level
#define SP_IRQ_DMA1				(SP_IRQ_GIC_START + 139)	// level

#define SP_IRQ_I2CMASTER0 		(SP_IRQ_GIC_START + 146)
#define SP_IRQ_I2CMASTER1 		(SP_IRQ_GIC_START + 147)

#define SP_IRQ_TIMERW				(SP_IRQ_GIC_START + 150)	// E
#define SP_IRQ_TIMER0				(SP_IRQ_GIC_START + 151)	// E
#define SP_IRQ_TIMER1				(SP_IRQ_GIC_START + 152)	// E
#define SP_IRQ_TIMER2				(SP_IRQ_GIC_START + 153)	// E
#define SP_IRQ_TIMER3				(SP_IRQ_GIC_START + 154)	// E

#define SP_IRQ_AV2_TIMER0			(SP_IRQ_GIC_START + 164)	// E
#define SP_IRQ_AV2_TIMER1			(SP_IRQ_GIC_START + 165)	// E
#define SP_IRQ_AV2_TIMER2			(SP_IRQ_GIC_START + 166)	// E
#define SP_IRQ_AV2_TIMER3			(SP_IRQ_GIC_START + 167)	// E

#define SP_IRQ_PMU0                               (SP_IRQ_GIC_START + 200)  //  level
#define SP_IRQ_PMU1                               (SP_IRQ_GIC_START + 201)  //  level
#define SP_IRQ_L2CC                               (SP_IRQ_GIC_START + 202)  //  level

//#define NR_IRQS		128
#define NR_IRQS			256

#endif
