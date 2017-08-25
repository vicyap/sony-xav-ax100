/*
 * (C) Copyright 2014
 * Sunplus Technology. <http://www.sunplus.com/>
 * Dante Su <dante.su@sunplus.com>
 *
 * SPDX-License-Identifier:     GPL-2.0+
 */

#ifndef __CONFIG_SPHE_COMMON_H
#define __CONFIG_SPHE_COMMON_H

#define CONFIG_SPHE /* which is a SPHE generic machine */
#define CONFIG_ARCH_CPU_INIT /* we need arch_cpu_init() for hw timers */

#define CONFIG_SYS_NO_FLASH

/* timer ticks per second */
#define CONFIG_SYS_HZ               1000

/* see linux/arch/arm/tools/mach-types */
#define CONFIG_MACH_TYPE            3892

/* cortex-A8 use 64 bytes and cortex-A9 use 32 bytes cache line size */
#define CONFIG_SYS_CACHELINE_SIZE   32

/* write-thru if MMU/d-cache is enabled */
#ifndef CONFIG_SYS_DCACHE_OFF
#define CONFIG_SYS_ARM_CACHE_WRITETHROUGH
#endif

#define CONFIG_DISPLAY_CPUINFO

#define CONFIG_CLKDEV_LOOKUP

#define CONFIG_HAVE_ARCH_CLKDEV

#define CONFIG_SP_GPIO

/*
 * UART
 */
#define CONFIG_SP_UART

/*
 * MON : utility for testing register/memory
 */
#define CONFIG_SP_MON

/*
 * SD MMC
 */
#ifdef CONFIG_SP_SD_MMC

#define CONFIG_GENERIC_MMC
#define CONFIG_SP_MMC y
#define CONFIG_CMD_MMC 		//for compile cmd_mmc.c
#define CONFIG_MMC 			//for fatls mmc command

#endif

/*
 * NAND
 */
#ifdef CONFIG_SP_NAND
#define CONFIG_SYS_MAX_NAND_DEVICE 1
#define CONFIG_SYS_NAND_BASE       CONFIG_SP_NAND_BASE
#define CONFIG_SYS_NAND_SELF_INIT
#define CONFIG_SYS_NAND_ONFI_DETECTION
#define CONFIG_CMD_NAND
//#define CONFIG_CMD_NAND_YAFFS
//#define CONFIG_YAFFS2
#define CONFIG_CMD_NAND_TRIMFFS

#define CONFIG_MTD_DEVICE
#define CONFIG_MTD_PARTITIONS
#define MTDIDS_DEFAULT          "nand0=sp_nand.0"
#define MTDPARTS_DEFAULT        "mtdparts=sp_nand.0:-(whole_nand)"
#define CONFIG_CMD_MTDPARTS

#if 0
#define CONFIG_CMD_UBI
#define CONFIG_RBTREE
// #define CONFIG_UBI_SILENCE_MSG
#define CONFIG_CMD_UBIFS
#define CONFIG_LZO
// #define CONFIG_UBIFS_SILENCE_MSG
#endif

#ifndef CONFIG_SYS_ARM_CACHE_WRITETHROUGH
// In write-back mode, flush before nand read to avoid data lost on unaligned buffer.
#define CONFIG_SP_NAND_READ_HELP_FLUSH_BUF
#endif

#endif

/*
 * USB EHCI
 */
#ifdef CONFIG_SP_USB_EHCI
# define CONFIG_SYS_USB_EHCI_MAX_ROOT_PORTS 1
# define CONFIG_USB_HUB_MIN_POWER_ON_DELAY  500
# define CONFIG_USB_EHCI
# define CONFIG_USB_EHCI_SPHE
# define CONFIG_CMD_USB
# define CONFIG_USB_STORAGE
#endif

/*
 * FAT Filesystem
 */
#if defined(CONFIG_USB_STORAGE) || defined(CONFIG_MMC)
# define CONFIG_DOS_PARTITION
# define CONFIG_CMD_FAT
#endif

/*
 * Initial stack pointer: GENERATED_GBL_DATA_SIZE in internal SRAM.
 * Inside the board_init_f, the gd is first assigned to
 * (CONFIG_SYS_INIT_SP_ADDR) & ~0x07) and then relocated to DRAM
 * while calling relocate_code.
 */
#define CONFIG_SYS_INIT_SP_ADDR \
	(CONFIG_SYS_SDRAM_BASE + SZ_4K - GENERATED_GBL_DATA_SIZE)

#define CONFIG_SYS_MONITOR_BASE     CONFIG_SYS_TEXT_BASE
#define CONFIG_SYS_MONITOR_LEN      SZ_512K

/* Default entry point */
#ifndef CONFIG_SYS_UBOOT_START
#define CONFIG_SYS_UBOOT_START      CONFIG_SYS_TEXT_BASE
#endif

/* Default load address */
#ifndef CONFIG_SYS_LOAD_ADDR
#define CONFIG_SYS_LOAD_ADDR        (CONFIG_SYS_SDRAM_BASE + SZ_8M)
#endif

#define CONFIG_AUTO_COMPLETE
#define CONFIG_CMDLINE_EDITING
#define CONFIG_SYS_LONGHELP
#define CONFIG_SYS_HUSH_PARSER
#define CONFIG_SYS_MAXARGS          16

/* Console I/O Buffer Size */
#define CONFIG_SYS_CBSIZE           0x800

/* Shell Prompt */
#define CONFIG_SYS_PROMPT           "=> "

/* Print Buffer Size */
#define CONFIG_SYS_PBSIZE \
	(CONFIG_SYS_CBSIZE + sizeof(CONFIG_SYS_PROMPT) + 16)

#define CONFIG_CMD_SOURCE       /* 'source' command */
#define CONFIG_CMD_BDI          /* bdinfo */
#define CONFIG_CMD_ECHO         /* echo arguments */
#define CONFIG_CMD_ENV          /* printenv */
#define CONFIG_CMD_MEMORY       /* md mm nm mw ... */
//#define CONFIG_CMD_NET          /* bootp, tftpboot, rarpboot */
#define CONFIG_CMD_RUN          /* run command in env variable */
//#define CONFIG_CMD_ELF          /* support ELF files */
//#define CONFIG_CMD_LOADB        /* xyzModem */
#ifndef CONFIG_ENV_IS_NOWHERE
# define CONFIG_CMD_SAVEENV     /* saveenv */
#endif
#define CONFIG_CMD_SETEXPR      /* shell's expr */

/*
 * Linux Kernel Command Line
 */
#define CONFIG_INITRD_TAG
#define CONFIG_CMDLINE_TAG      /* support ATAGs */
//#define CONFIG_SETUP_MEMORY_TAGS

#endif	/* __CONFIG_SPHE_COMMON_H */
