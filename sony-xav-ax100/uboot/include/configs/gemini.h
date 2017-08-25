/*
 * (C) Copyright 2015
 * Sunplus Technology. <http://www.sunplus.com/>
 *
 * SPDX-License-Identifier:     GPL-2.0+
 */

#ifndef __CONFIG_GEMINI_H
#define __CONFIG_GEMINI_H

#ifdef CONFIG_GEMINI_PLATFORM_SDK
#define CONFIG_SYS_THUMB_BUILD
#endif

#include <asm/hardware.h>

//#define CONFIG_SYS_ICACHE_OFF
//#define CONFIG_SYS_DCACHE_OFF

#define CPU1_START_POS      (0x9e810000 - 0x8)

/* SPI Romter image offset */
#define CONFIG_SPI_ECOS_OFFSET     (1 << 20)
#define CONFIG_SPI_KERNEL_OFFSET   (8 << 20)

/* memory configuration */
#define CONFIG_NR_DRAM_BANKS        1

/*
 *  DRAM Memory Map (in MB)
 *
 *  offset   size     User
 *  -----------------------------------------
 *      0      2      eCos ram1 (vector)
 *      2     14      u-boot / Linux ram1
 *     16      X      HW Buffer + eCos ram2 (text, data, bss, heap, etc)
 *   16+X      1      IOP mem
 *   17+X      1      RTCFG mem
 *   18+X   rest      Linux ram2
 *  MAX-Y      Y      Linux Chunkmem
 *    MAX             END
 *
 * Note:
 *    X: CONFIG_SP_HW_BUF_SIZE + CONFIG_SP_ECOS_RAM2_SIZE
 *    Y: CONFIG_SP_CHUNKMEM_SIZE
 */
#define CONFIG_SP_TOTAL_DRAM_SIZE        (256*1024*1024)          // Dram total size 256MB

#define CONFIG_SYS_SDRAM_BASE       0
#define CONFIG_SYS_SDRAM_SIZE       (16 * SZ_1M)      // dram_init()
#define CONFIG_SYS_MALLOC_LEN       ( 5 * SZ_1M)      // Add additional 4MB for UBI, 2015/12/28

// u-boot
#define CONFIG_SYS_TEXT_BASE        0x200000          // 2MB

// eCos ram1
#define CONFIG_SP_ECOS_RAM1_ADDR    0x00000000
#define CONFIG_SP_ECOS_RAM1_SIZE    0x00200000

// HW Buffer start address
#define CONFIG_SP_HW_BUF_ADDR       0x1000000         // 16MB

// HW Buffer size + eCos ram2 size = X
// Note: X is configurable but must match with eCos config
#ifdef CONFIG_SP_GEMINI_DEFAULT
 #define CONFIG_SP_HW_BUF_SIZE      (116 *1024*1024)  // 116MB
 #define CONFIG_SP_ECOS_RAM2_SIZE   (16 *1024*1024)  //  16MB
#elif defined(CONFIG_GEMINI_SINGLEOUT)
 #define CONFIG_SP_HW_BUF_SIZE      (103 *1024*1024)  // 103MB
 #define CONFIG_SP_ECOS_RAM2_SIZE   (16 *1024*1024)  //  16MB
#elif defined(CONFIG_GEMINI_SINGLEOUT_SERVO)
 #define CONFIG_SP_HW_BUF_SIZE      (109 *1024*1024)  // 109MB
 #define CONFIG_SP_ECOS_RAM2_SIZE   (16 *1024*1024)  //  16MB
#endif

#ifdef CONFIG_SP_GEMINI_DEFAULT
#define CONFIG_SP_TCON_OFFSET				0x00004000
#define CONFIG_SP_VI_OFFSET					0x0000F400
#define CONFIG_SP_VIDEO_DECODER_OFFSET		0x00610800
#define CONFIG_SP_MFD_OFFSET		0x00FD2800
#define CONFIG_SP_OSD2_OFFSET		0x06B3A400
#define CONFIG_SP_OSD2_PALETTE_OFFSET		0x06BF6400
#elif defined(CONFIG_GEMINI_SINGLEOUT)
#define CONFIG_SP_TCON_OFFSET				0x00004000
#define CONFIG_SP_VI_OFFSET					0x0000F400
#define CONFIG_SP_VIDEO_DECODER_OFFSET		0x00010800
#define CONFIG_SP_MFD_OFFSET		0x009D2800
#define CONFIG_SP_OSD2_OFFSET		0x05EE6400
#define CONFIG_SP_OSD2_PALETTE_OFFSET		0x05FA2400
#elif defined(CONFIG_GEMINI_SINGLEOUT_SERVO)
#define CONFIG_SP_TCON_OFFSET				0x00004000
#define CONFIG_SP_VI_OFFSET					0x0000F400
#define CONFIG_SP_VIDEO_DECODER_OFFSET		0x00610800
#define CONFIG_SP_MFD_OFFSET		0x00FD2800
#define CONFIG_SP_OSD2_OFFSET		0x064E6400
#define CONFIG_SP_OSD2_PALETTE_OFFSET		0x065A2400
#endif

#define CONFIG_SP_TCON_ADDR					(CONFIG_SP_HW_BUF_ADDR + CONFIG_SP_TCON_OFFSET)
#define CONFIG_SP_VI_ADDR					(CONFIG_SP_HW_BUF_ADDR + CONFIG_SP_VI_OFFSET)
#define CONFIG_SP_VI_SIZE					(5 * 1024)
#define CONFIG_SP_VIDEO_DECODER_ADDR		(CONFIG_SP_HW_BUF_ADDR + CONFIG_SP_VIDEO_DECODER_OFFSET)
#define CONFIG_SP_MFD_ADDR		(CONFIG_SP_HW_BUF_ADDR + CONFIG_SP_MFD_OFFSET)
#define CONFIG_SP_OSD2_ADDR		(CONFIG_SP_HW_BUF_ADDR + CONFIG_SP_OSD2_OFFSET)
#define CONFIG_SP_OSD2_SIZE		(752 * 1024)
#define CONFIG_SP_OSD2_PALETTE_ADDR		(CONFIG_SP_HW_BUF_ADDR + CONFIG_SP_OSD2_PALETTE_OFFSET)
#define CONFIG_SP_OSD2_PALETTE_SIZE		(2 * 1024)

// eCos end address
#define CONFIG_SP_ECOS_END_ADDR     (CONFIG_SP_HW_BUF_ADDR + CONFIG_SP_HW_BUF_SIZE + CONFIG_SP_ECOS_RAM2_SIZE)

// IOP memory
#define CONFIG_SP_IOP_ADDR          (CONFIG_SP_HW_BUF_ADDR)
#define CONFIG_SP_IOP_SIZE          (1 * 1024*1024)

// Display memory
#define CONFIG_SP_DISPLAY_ADDR      CONFIG_SP_ECOS_END_ADDR
#define CONFIG_SP_DISPLAY_SIZE      (0 * 1024*1024)
#define CONFIG_SP_DISPLAY_END_ADDR  CONFIG_SP_DISPLAY_ADDR + CONFIG_SP_DISPLAY_SIZE

// Platform config reserved memory
#define CONFIG_SP_RTCFG_ADDR        CONFIG_SP_DISPLAY_END_ADDR
#define CONFIG_SP_RTCFG_SIZE        (1 * 1024 * 1024)
#define CONFIG_SP_RTCFG_MAX         (512 * 1024)
#define CONFIG_SP_VD_ADDR           (CONFIG_SP_RTCFG_ADDR + CONFIG_SP_RTCFG_MAX)
#define CONFIG_SP_VD_SIZE           (CONFIG_SP_RTCFG_SIZE - CONFIG_SP_RTCFG_MAX)
#define CONFIG_SP_RTCFG_END_ADDR    CONFIG_SP_RTCFG_ADDR + CONFIG_SP_RTCFG_SIZE
//use the last 512 k from RTCFG for vendor data



// Linux Chunkmem
#ifdef CONFIG_SP_GEMINI_DEFAULT
 #define CONFIG_SP_CHUNKMEM_SIZE     (10 *1024*1024)  //  10MB
#elif defined(CONFIG_SP_GEMINI_SD)
 #define CONFIG_SP_CHUNKMEM_SIZE     (7 *1024*1024)  //  7MB
#endif 
#define CONFIG_SP_CHUNKMEM_ADDR     (CONFIG_SP_TOTAL_DRAM_SIZE - CONFIG_SP_CHUNKMEM_SIZE)

// Linux
#define CONFIG_BOOT_PARAM_ADDR      (CONFIG_SP_RTCFG_END_ADDR + (1*1024*1024)) // linux boot_params
#define CONFIG_SYS_LOAD_ADDR        (CONFIG_BOOT_PARAM_ADDR +  (1 *1024*1024)) // linux kernel
#define CONFIG_INITRD_ADDR          (CONFIG_SYS_LOAD_ADDR  + (11 *1024*1024))  // linux initrd

// Linux end address
#define CONFIG_SP_LINUX_SIZE     (CONFIG_SP_CHUNKMEM_ADDR/1024/1024)

// boot/isp/pat logo
#define CONFIG_SP_OSD_LOGO_MAX_SIZE	(9*1024*1024)		//limit: 1920x1200, 1920x1200x4, 1024 alignment=9*1024*1024

// isp mode memory
/* Check for ISP Update on boot */
#define CONFIG_SP_ISP_UPDT_CHECK
#define CONFIG_SP_ISP_LOGO
/* Check for ISP Update on boot */

#define CONFIG_SP_ISP_ADDR	(CONFIG_SP_HW_BUF_ADDR)
#define CONFIG_SP_ISP_SIZE   (16*1024*1024)

#define CONFIG_SP_OSD_HEADER_ADDR	(CONFIG_SP_ISP_ADDR + CONFIG_SP_ISP_SIZE)
#define CONFIG_SP_OSD_HEADER_SIZE	(2048)	//128+256*4, 1024 alignment = 2048
#define CONFIG_SP_OSD_LOGO_ADDR		(CONFIG_SP_OSD_HEADER_ADDR + CONFIG_SP_OSD_HEADER_SIZE)
#define CONFIG_SP_OSD_LOGO_SIZE		(CONFIG_SP_OSD_LOGO_MAX_SIZE*2)

#define CONFIG_SP_ISP_LOGO_ADDR		(CONFIG_SP_OSD_LOGO_ADDR + CONFIG_SP_OSD_LOGO_SIZE)
#define CONFIG_SP_ISP_LOGO_SIZE		(CONFIG_SP_OSD_LOGO_MAX_SIZE*3)

#define CONFIG_SP_OLD_VERSION_ADDR	(CONFIG_SP_ISP_LOGO_ADDR + CONFIG_SP_ISP_LOGO_SIZE)
#define CONFIG_SP_OLD_VERSION_SIZE   (1024)
#define CONFIG_SP_NEW_VERSION_ADDR	(CONFIG_SP_OLD_VERSION_ADDR + CONFIG_SP_OLD_VERSION_SIZE)
#define CONFIG_SP_NEW_VERSION_SIZE   (1024)
// isp mode memory

/* Boot */
#define CONFIG_BOOTARGS             " "
// append CONFIG_SP_CMDARGS to bootargs
#define CONFIG_SP_CMDARGS           "res_arm926_addr=0 res_arm926_size=0x%x "        \
                                    "res_iop_addr=0x%x res_iop_size=0x%x "           \
                                    "res_display_addr=0x%x res_display_size=0x%x "   \
                                    "res_rtcfg_addr=0x%x res_rtcfg_size=0x%x "       \
                                    "mem=%dM@0x0 "                                   \
                                    "res_chunkmem_addr=0x%x res_chunkmem_size=0x%x " \
                                    "vmalloc=320M "
#define CONFIG_CMD_BOOTZ

#define CONFIG_FAT_WRITE
#define CONFIG_ARCH_MISC_INIT

/* Console */
#define CONFIG_SILENT_CONSOLE       // Define this and set environment variable 'silent=1' => No console output

/* To partially quiet u-boot (silent after loading NAND env), don't set CONFIG_EXTRA_ENV_SETTINGS below
 * but use "setenv silent 1 ; saveenv".
 *
 * To completely quiet u-boot, set prebuilt env "silent=1" via CONFIG_EXTRA_ENV_SETTINGS
 * because initial prints happen before loading NAND env.
 */
//#define CONFIG_EXTRA_ENV_SETTINGS   "silent=1"


// #define CONFIG_SP_CACHEBENCH

/* Autoboot */
#ifndef CONFIG_SP_CACHEBENCH
#define CONFIG_BOOTDELAY            0           // (-2): autoboot without delay and not check for abort
#define CONFIG_BOOTCOMMAND          "bootsp"
//#define CONFIG_BOOTCOMMAND          " "
#define CONFIG_SYS_L2CACHE_OFF
#else
#define CONFIG_BOOTDELAY            2
#define CONFIG_BOOTCOMMAND          "cachebench"
// #define CONFIG_SYS_L2CACHE_OFF
#endif

#if !defined(CONFIG_SYS_L2CACHE_OFF)
#define CONFIG_SYS_L2_PL310
#define CONFIG_SYS_PL310_BASE       ADDRESS_PL310
#endif

#if 1
#define CONFIG_ZERO_BOOTDELAY_CHECK
#define CONFIG_AUTOBOOT_KEYED
#define CONFIG_AUTOBOOT_STOP_STR    "\x1b"
#undef  CONFIG_AUTOBOOT_DELAY_STR
#define CONFIG_AUTOBOOT_PROMPT      "\n\nAutobooting in %d sec, press \"<Esc>\" to stop\n\n", bootdelay
#endif

/* UART */
#define CONFIG_BAUDRATE             115200

/* NAND */
#define CONFIG_SP_NAND
#define CONFIG_SP_NAND_CSNR       1
//#define CONFIG_SP_NAND_SCRAMBLER // Dis-able randomizer for SPI NAND & parallel NAND
#define CONFIG_SP_NAND_RW_BOOT_BLOCK
#define CONFIG_SP_MTD_SCAN_BBT_FAST // To enable MTD scan Bad Block Table fast
//#define CONFIG_SP_ERASECNT_TABLE // The erase counter for each block
#define CONFIG_SP_NAND_PING_PONG_BUFFER // To enable Nand ping-pong arch for reading performance
#define CONFIG_SP_NAND_PAGE_RELOAD // To enable reload the same Nand page if something wrong.
#ifdef CONFIG_SP_NAND_PAGE_RELOAD
#define SP_NAND_RELOAD_RETURN_ZERO // return zero to MTD
#endif
// #define CONFIG_SP_BCH_REPORT // To report BCH ECC status

/* Compressions */

// Define this to boot compressed ecos.img
//#define CONFIG_UNCOMPRESS_ECOS_IMG

// GZIP
//#define CONFIG_ZLIB
//#define CONFIG_GZIP

// LZO
//#define CONFIG_LZO

// LZ4
//#define CONFIG_LZ4

/* SP_PNG_DECODER */
// #define CONFIG_SP_PNG_DECODER

#if !defined(CONFIG_SP_GEMINI_SD)
#define CONFIG_SP_SPINAND       // To enable SPI NAND. board_nand_init() decides using SPI NAND or parallel NAND
#define CONFIG_SP_NFTL          // To enable SP NAND-FTL
#endif

/* NAND FTL */
#ifdef CONFIG_SP_NFTL
#undef NAND_OS_LINUX
#define NAND_OS_BL		2 // mean non os
#define NAND_OS_BL2
#undef NAND_OS_UCLINUX
#define NAND_RUN_OS_IS   2
#define CONFIG_ARCH_letb456
#define CONFIG_DOS_PARTITION
#endif

/* SD MMC */
#define CONFIG_SP_SD
#define CONFIG_GENERIC_MMC
#define CONFIG_MMC
#define CONFIG_CMD_MMC
//#define CONFIG_MMC_TRACE /* SD framework debugging */

#define CONFIG_CMD_READ

/* CBUS DMA */
#define CONFIG_SP_CBDMA

#ifdef CONFIG_SP_CBDMA
#define CONFIG_UBOOT_RELOC_COPY_EARLY    /* speed up relocation by cbdma copy */
#endif

/* USB EHCI */
#define CONFIG_SP_USB_EHCI
#define CONFIG_USB_MAX_CONTROLLER_COUNT 2
#define CONFIG_USB_PORT_FASTEN

/* u-boot environment variables */
/* CONFIG_ENV_IS_NOWHERE is used when uboot env isn't stored in any storage medium */
#if defined(CONFIG_SP_NAND)
# define CONFIG_ENV_IS_IN_SP_NAND
#else
# define CONFIG_ENV_IS_NOWHERE
#endif

#define CONFIG_ENV_SIZE             SZ_64K
/*
   uboot env for sp_nand is designed so that
   1. uboot env stored on nand can be scanned dynamically upon bootup
   2. can saveenv by having mtd partition ENV_PART_NAME
   3. A redundant copy can be declared if it is needed (ENV_PART_REDUND)
   (must have mtd partition(s) pre-defined in u-boot env for saveenv to work)
   (Ex: setenv mtdparts mtdparts=sp_nand.0:2m(nand_header),3m(env),3m(env_redund))
   CONFIG_ENV_IS_IN_SP_NAND - define this and the below items to use this function
   ENV_MAGICNUM & ENV_MAGICNUM_SIZE -  required when scanning nand for uboot env
   ENV_BLK_SCAN_START - Specify the starting block # to start and search for uboot env
   ENV_BLK_SCAN_END   - Specify the ending block # to end searching for uboot env
                        !!!The longer range to search for u-boot env on NAND, the longer
                           it may take to bootup system. For optimization, start & end blocks
                           should be tweaked to a range sufficient for searching u-boot env partitions!!!
   NUM_BLKS_FOR_ENV - the pre-defined nand blocks count for uboot env partition
   CONFIG_SP_ENV_SIZE - necessary because now S+'s u-boot env format is different from uboot's generic format
   ENV_PART_NAME - mtd partition name declared for u-boot env partition
   ENV_PART_NAME_REDUND - mtd partition name declared u-boot env partition (redundant copy)
                          (comment ENV_PART_NAME_REDUND to disable saving redundant copy of u-boot env)
   !!!ENV_PARTITIONS MUST BE WITHIN THE ENV_BLK_SCAN_START & ENV_BLK_SCAN_END range!!!!
*/
#if defined(CONFIG_ENV_IS_IN_SP_NAND) && defined(CONFIG_ENV_IS_NOWHERE)
# error "CONFIG_ENV_IS_IN_SP_NAND & CONFIG_ENV_IS_NOWHERE can't be defined at the same time"
#endif
#if defined(CONFIG_ENV_IS_IN_SP_NAND) && defined(CONFIG_SP_NAND)
# define CONFIG_CMD_SAVEENV	/* saveenv */
# define ENV_MAGICNUM		0xFACEFEED	/* magic number for checking if read data is uboot env */
# define ENV_MAGICNUM_SIZE	0x4		/* magic number size */
# define ENV_BLK_SCAN_START     0               /* starting block # to start and scan for uboot env */
# define ENV_BLK_SCAN_END	100		/* ending block # to scan for uboot env */
# define ENV_PART_NAME		"env"		/* mtd partition name for uboot env */
# define ENV_PART_NAME_REDUND	"env_redund"	/* mtd partition name for uboot env (redundant copy) */
# define CONFIG_SP_ENV_SIZE	(CONFIG_ENV_SIZE + ENV_MAGICNUM_SIZE)
# define CONFIG_SP_ENV_LOAD_BY_HINT             /* load env from hint offset (fallback to scanning) */
#endif


#include "sphe-common.h"

/* md5 */
#define CONFIG_MD5
#define CONFIG_CMD_MD5SUM

/* AES */
// #define CONFIG_AES
#define CONFIG_AES_FAST
#define CONFIG_CMD_AES

/* fast_boot */
//#define CONFIG_CMD_FASTBOOT
#define CONFIG_USB_GADGET
#define CONFIG_USBDOWNLOAD_GADGET
//#define CONFIG_SP_UDC
#define CONFIG_CMD_MSDC
#define CONFIG_USB_FASTBOOT_BUF_SIZE (8<<10)
#define CONFIG_USB_FASTBOOT_BUF_ADDR  g_buf
#define  CONFIG_USB_GADGET_DUALSPEED

#ifdef CONFIG_SP_ISP_LOGO
//ISP flow state using led flash
#define HIGH_ON 1
#define LOW_ON 0
#ifdef CONFIG_SP_GEMINI_DEFAULT
//#define ISP_FLOW_STATE_LED_FLASH
#define ISP_LED_GPIO 56
#define ISP_LED_ON HIGH_ON
#elif defined(CONFIG_SP_GEMINI_SD)
#define ISP_FLOW_STATE_LED_FLASH
#define ISP_LED_GPIO 56
#define ISP_LED_ON HIGH_ON
#endif
//ISP flow state using led flash
#endif

//let customer modified update file name and path
#define REMAIN_STR_LEN (256-86)
#define BUILD_BUG_ON(condition) ((void)sizeof(char[1 - 2*!!(condition)]))
#ifdef CONFIG_SP_GEMINI_DEFAULT
#define UPDT_PATH "ISP_UPDT.BIN"
#define PACK_PATH "GEMINI_PACK.BIN"
#elif defined(CONFIG_SP_GEMINI_SD)
#define UPDT_PATH "update/CUST_UPDT.BIN"
#define PACK_PATH "update/CUST_PACK.BIN"
#endif
//let customer modified update file name and path

//ISP update retry max times
#ifdef CONFIG_SP_GEMINI_DEFAULT
#define UPDATE_MAX_TIME 5
#elif defined(CONFIG_SP_GEMINI_SD)
#define UPDATE_MAX_TIME 5
#endif
//ISP update retry max times

// #define DEBUG

////////////////
// BOOT TIMESTAMP
#define UBOOT_TIMESTAMP
#ifdef UBOOT_TIMESTAMP
#define UBOOT_START "UBOOT Start"
#define UBOOT_START_TH (40000) // 0.4444444444444444 sec
#endif

#endif	/* __CONFIG_GEMINI_H */
