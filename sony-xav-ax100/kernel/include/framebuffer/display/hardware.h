
#ifndef __ASM_ARCH_HARDWARE_H
#define __ASM_ARCH_HARDWARE_H

//#include <asm/sizes.h>


#define letb123_DRAM_BASE				0x00000000
#define letb123_DRAM_A_BASE			(letb123_DRAM_BASE+0x0)
#define letb123_DRAM_B_BASE			(letb123_DRAM_BASE+0x40000000)
#define letb123_SPI_BASE				0x98000000
#define letb123_DEVICE_BASE			0x9C000000
#define letb123_HE_REG_BASE			(letb123_DEVICE_BASE + 0x0)
#define letb123_APB2_BASE				(letb123_DEVICE_BASE + 0x20000)
#define letb123_APB1_BASE				(letb123_DEVICE_BASE + 0x60000)
#define letb123_APB3_BASE				(letb123_DEVICE_BASE + 0x70000)
#define letb123_APB0_BASE				(letb123_DEVICE_BASE + 0x80000)
#define letb123_AXI0_BASE				(letb123_DEVICE_BASE + 0x420000)
#define letb123_AHB0_BASE				(letb123_DEVICE_BASE + 0x820000)
#define letb123_AHB1_BASE				(letb123_DEVICE_BASE + 0x828000)
#define letb123_AHB2_BASE				(letb123_DEVICE_BASE + 0x829000)
#define letb123_AHB3_BASE				(letb123_DEVICE_BASE + 0x82B000)
#define letb123_AHB4_BASE				(letb123_DEVICE_BASE + 0x82C000)
#define letb123_AHB5_BASE				(letb123_DEVICE_BASE + 0x82D000)
#define letb123_AMBA_BASE				(letb123_DEVICE_BASE + 0x1F00000)
#define letb123_BOOTROM_BASE			0x9E000000
#define letb123_CBDMA0_BASE			0x9E800000
#define letb123_CBDMA1_BASE			0x9E820000
#define letb123_IMG_DEC_BASE			0x9E900000
#define letb123_IMG_DEC_SND_BASE		0x9E920000
#define letb123_L2C_BASE				0x9F000000
#define letb123_ARM_CA9_BASE			0x9F100000
#define letb123_SCU_BASE 				(letb123_ARM_CA9_BASE + 0x0)
#define letb123_GIC_BASE				(letb123_ARM_CA9_BASE + 0x100)
#define letb123_GTIMER_BASE			(letb123_ARM_CA9_BASE + 0x200)
#define letb123_TWD_BASE				(letb123_ARM_CA9_BASE + 0x600)
#define letb123_GIC_DIST_BASE			(letb123_ARM_CA9_BASE + 0x1000)
#define letb123_HIVEC_ROM_BASE			0xFFFF0000
#define BIN_NUM_MAX 10



//#define letb123_DEVICE_BASE_OFFSET		0x63000000                 		// VA of IO 	/* 0x9C000000 --> 0xff000000 */
/*
 * Where in virtual memory the IO devices (timers, system controllers
 * and so on)
 */
//#define GPS_VENDOR_REGISTER   //remove currently


#define IO0_START			letb123_HE_REG_BASE            				// PA of IO	
//#define IO0_BASE			(IO0_START + letb123_DEVICE_BASE_OFFSET)       // VA of IO 	/* 0x9C000000 --> 0xFF000000 */
#define IO0_BASE			(0xf0800000)           							// VA of IO 	/* 0x9C000000 --> 0xFF000000 */
#define IO0_SIZE			SZ_128K                 						// How much?	128K

#ifdef GPS_VENDOR_REGISTER
#define IO1_START			letb123_APB1_BASE                 						// PA of IO
#define IO1_BASE			(IO0_BASE+IO0_SIZE)     	           					// VA of IO 
#define IO1_SIZE			SZ_64K           		      				// How much?
#define IO3_START			letb123_APB3_BASE                 						// PA of IO
#define IO3_BASE			(IO1_BASE+IO1_SIZE)      	           					// VA of IO 
#define IO3_SIZE			SZ_64K           		      				// How much?
#endif


/*
#define IO1_START			0x9c020000                 						// PA of IO
#define IO1_BASE			0xfc020000      	           					// VA of IO 
#define IO1_SIZE			0x01fe0000           		      				// How much?

#define IO2_START			0x9f100000                 						// PA of IO
#define IO2_BASE			0xfe000000                 						// VA of IO 
#define IO2_SIZE			0x00002000                 						// How much?
*/
#define L2C_START			letb123_L2C_BASE                				// PA of IO
#ifdef GPS_VENDOR_REGISTER
#define L2C_BASE			(IO3_BASE + IO3_SIZE)           				// VA of IO 	/* 0x9F000000 --> 0xFF020000 */
#else
#define L2C_BASE			(IO0_BASE + IO0_SIZE)           				// VA of IO 	/* 0x9F000000 --> 0xFF020000 */
#endif
#define L2C_SIZE			SZ_4K                 							// How much?	4K

#define SRAM0_START			letb123_CBDMA0_BASE							// PA of IO
#define SRAM0_BASE			(L2C_BASE + L2C_SIZE)                			// VA of IO 	/* 0x9E800000 --> 0xFF021000 */
#define SRAM0_SIZE			SZ_128K                 						// How much?	128K

#define SRAM1_START			letb123_CBDMA1_BASE							// PA of IO
#define SRAM1_BASE			(SRAM0_BASE + SRAM0_SIZE)						// VA of IO 	/* 0x9E820000 --> 0xFF041000 */
#define SRAM1_SIZE			SZ_128K                							// How much?	128K


#define AHB_USBD_START		letb123_AHB5_BASE                				// PA of IO
#define AHB_USBD_BASE		(SRAM1_BASE + SRAM1_SIZE)           				// VA of IO 	/* 0x9F000000 --> 0xFF020000 */
#define AHB_USBD_SIZE		SZ_4K                 							// How much?	4K

#define AHB_BCH_START		letb123_AHB4_BASE                				// PA of IO
#define AHB_BCH_BASE		(AHB_USBD_BASE + AHB_USBD_SIZE)           		// VA of IO 	/* 0x9F000000 --> 0xFF020000 */
#define AHB_BCH_SIZE		SZ_4K                 							// How much?	4K

#define AHB_NAND_START		letb123_AHB3_BASE                				// PA of IO
#define AHB_NAND_BASE		(AHB_BCH_BASE + AHB_BCH_SIZE)           		// VA of IO 	/* 0x9F000000 --> 0xFF020000 */
#define AHB_NAND_SIZE		SZ_4K                 							// How much?	4K

#define AHB_SPU_START		letb123_AHB2_BASE                				// PA of IO
#define AHB_SPU_BASE		(AHB_NAND_BASE + AHB_NAND_SIZE)           		// VA of IO 	/* 0x9F000000 --> 0xFF020000 */
#define AHB_SPU_SIZE		SZ_8K                 							// How much?	4K

#define AHB_SCALER_START	letb123_AHB1_BASE                				// PA of IO
#define AHB_SCALER_BASE		(AHB_SPU_BASE + AHB_SPU_SIZE)           		// VA of IO 	/* 0x9F000000 --> 0xFF020000 */
#define AHB_SCALER_SIZE		SZ_4K                 							// How much?	4K

#define AHB_PPU_START		letb123_AHB0_BASE                				// PA of IO
#define AHB_PPU_BASE		(AHB_SCALER_BASE + AHB_SCALER_SIZE)           	// VA of IO 	/* 0x9F000000 --> 0xFF020000 */
#define AHB_PPU_SIZE		SZ_32K                 							// How much?	4K

#define AXI_SGX540_START	letb123_AXI0_BASE                				// PA of IO
#define AXI_SGX540_BASE		(AHB_PPU_BASE + AHB_PPU_SIZE)           		// VA of IO 	/* 0x9F000000 --> 0xFF020000 */
#define AXI_SGX540_SIZE		SZ_4K                 							// How much?	4K

#define SCU_START			letb123_SCU_BASE                				// PA of IO
#define SCU_BASE			(AXI_SGX540_BASE + AXI_SGX540_SIZE)           			// VA of IO 	/* 0x9F100000 --> 0xFF061000 */
#define SCU_SIZE			SZ_256                 							// How much?	256B

/* macro to get at IO space when running virtually */
#define IO0_ADDRESS(x) ((x) + IO0_BASE)
#ifdef GPS_VENDOR_REGISTER
#define IO1_ADDRESS(x) ((x) + IO1_BASE)
#define IO3_ADDRESS(x) ((x) + IO3_BASE)
#endif
//#define IO2_ADDRESS(x) ((x) + IO2_BASE)
#define SRAM0_ADDRESS(x) ((x) + SRAM0_BASE)
#define SRAM0_ADDR_END   (SRAM0_BASE + SRAM0_SIZE)
#define SRAM1_ADDRESS(x) ((x) + SRAM1_BASE)
#define SRAM1_ADDR_END   (SRAM1_BASE + SRAM1_SIZE)

#define IO0_PHY_ADDR(x) ((x) + IO0_START)
//#define IO1_PHY_ADDR(x) ((x) + IO1_START)
//#define IO2_PHY_ADDR(x) ((x) + IO2_START)

/* define register base address */
//#define LOGI_ADDR_REG				(IO0_BASE)
//#define LOGI_ADDR_REG_RANGE		(0x10000000)

#endif
