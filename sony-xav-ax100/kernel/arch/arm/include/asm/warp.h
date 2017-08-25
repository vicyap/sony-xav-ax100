/*
 * include/asm/warp.h
 */

#ifndef _ASM_WARP_H
#define _ASM_WARP_H

// #include <asm/sysconfig.h>

#ifdef CONFIG_MACH_GEMINI

#define WARP_HIBDRV_FLOATING
#define WARP_HIBDRV_MTD_NAME	"snapshot"
#define WARP_HIBDRV_DEV		"sdcardb"
#define WARP_HIBDRV_LOAD_MODE	WARP_LOAD_DEV
#define WARP_HIBDRV_OFFSET	warp_hibdrv_area[warp_devno].offset
#define WARP_HIBDRV_AREA_SIZE	warp_hibdrv_area[warp_devno].area_size

#define WARP_HIBDRV_AREA                                                \
	/* nand_id, load_mode, offset, area_size, */			\
{                                                                       \
	0x2c44, WARP_LOAD_MTD_NAME, 0x00000000, 0x00600000,		\
}, {								        \
	0xecd7, WARP_LOAD_MTD_NAME, 0x00000000, 0x00300000,		\
}, {			 						\
	0x0000, WARP_LOAD_DEV,      0x00000000, 0x00010000,		\
},

/* 0xffffffff ones are automatically calculated in warp_drv_init*/
#define WARP_SAVEAREA0							\
	/* load mode,	offset,	name,	: Bootflag(for load)		\
	   device,	area,	size,	: Bootflag(for save)		\
	   device,	area,	size,	: Snapshot(for save) */		\
{	/* Micron */							\
	WARP_LOAD_MTD_NAME,	0x00600000,	"snapshot",		\
	WARP_DEV_NAND,		0xffffffff,	0x00600000,		\
	WARP_DEV_NAND,		0xffffffff,	0xffffffff,		\
},

#define WARP_SAVEAREA1							\
{	/* SAMSUNG */							\
	WARP_LOAD_MTD_NAME,	0x00300000,	"snapshot",		\
	WARP_DEV_NAND,		0xffffffff,	0x00300000,		\
	WARP_DEV_NAND,		0xffffffff,	0xffffffff,		\
},
#define WARP_SAVEAREA2							\
{	/* eMMC */							\
	WARP_LOAD_DEV,		0xffffffff,	"sdcardb",		\
	WARP_DEV(USER,3,0),	0xffffffff,	0x00000001,		\
	WARP_DEV(USER,3,0),	0xffffffff,	0xffffffff,		\
},

#define WARP_CONSOLE		0

/* See if fastboot should be used as android or linux mode */
/* This is under the assumption that letb6700 uses linux fastboot mode */
/* And all other platforms use android fastboot mode */
#ifndef SYSCONFIG_ARCH_letb6700
#define WARP_ANDROID_MODE
#endif

#ifdef WARP_ANDROID_MODE
#define WARP_AM_INOTIFY
#if defined(SYSCONFIG_WARP_UMOUNT_MODE)
#define WARP_UMOUNT_RW          "/data", "/cache", "/recovery", "/mnt/mtd/userdata"
#define WARP_UMOUNT_RW_REMOUNT  1, 1, 1, 1
#else
#define WARP_UMOUNT_RW          "/data", "/cache"
#define WARP_UMOUNT_RW_REMOUNT  1, 1
#endif	
#endif

#define WARP_BPS		115200

#define WARP_MTD_BB_CONV

#define WARP_MMC_SNAPSHOT_COPY

#endif	/* CONFIG_MACH_GEMINI */

#if 0
#define WARP_PRINT_MEMINFO     /* use only debugging */
#endif

#if 0
#define WARP_DRAM_CLEAR
#define WARP_DRAM_CLEAR_END    (PHYS_OFFSET + SZ_64M)
#endif

#define WARP_PFN_IS_NOSAVE

#endif	/* _ASM_WARP_H */

