/*
 * Warp!! parameter define  Rev. 4.0.7
 *
 */

#ifndef _LINUX_WARP_PARAM_H
#define _LINUX_WARP_PARAM_H

#include <linux/warp.h>
#include <asm/warp.h>

#define WARP_PARAM_VER_MAJOR    0x0003
#define WARP_PARAM_VER_MINOR    0x0002

#define WARP_HEADER_ID          0x00
#define WARP_HEADER_COPY_SIZE   0x04
#define WARP_HEADER_DRV_SIZE    0x08
#define WARP_HEADER_VERSION     0x0c
#define WARP_HEADER_SNAPSHOT    0x20
#define WARP_HEADER_HIBERNATE   0x28
#define WARP_HEADER_SWITCH      0x30

#define WARP_ID_DRIVER          0x44483457      /* W4HD */
#define WARP_ID_BOOTFLAG        0x46423457      /* W4BF */

#define WARP_PART_SHIFT         0
#define WARP_LUN_SHIFT          8
#define WARP_DEV_SHIFT          16

#define WARP_PART_MASK          (0xff << WARP_PART_SHIFT)
#define WARP_LUN_MASK           (0xff << WARP_LUN_SHIFT)
#define WARP_DEV_MASK           (0xff << WARP_DEV_SHIFT)

#define WARP_DEV_NOR            (0x01 << WARP_DEV_SHIFT)
#define WARP_DEV_NAND           (0x02 << WARP_DEV_SHIFT)
#define WARP_DEV_ATA            (0x03 << WARP_DEV_SHIFT)
#define WARP_DEV_SD             (0x04 << WARP_DEV_SHIFT)
#define WARP_DEV_MEM            (0x05 << WARP_DEV_SHIFT)
#define WARP_DEV_SPI            (0x06 << WARP_DEV_SHIFT)
#define WARP_DEV_USER           (0x7e << WARP_DEV_SHIFT)
#define WARP_DEV_EXT            (0x7f << WARP_DEV_SHIFT)

#define WARP_DEV(dev, lun, part)        (WARP_DEV_##dev | \
                                         ((lun) << WARP_LUN_SHIFT) | \
                                         ((part) << WARP_PART_SHIFT))

#ifndef WARP_LUN_CONV
#define WARP_LUN_CONV(dev)      (dev)
#endif

#define WARP_DEV_TO_LUN(dev)    WARP_LUN_CONV(((dev) & WARP_LUN_MASK) >> \
                                              WARP_LUN_SHIFT)
#define WARP_DEV_TO_PART(dev)   (((dev) & WARP_PART_MASK) >> WARP_PART_SHIFT)

#define WARP_LOAD_MEM           0
#define WARP_LOAD_DEV           1
#define WARP_LOAD_MTD_NO        2
#define WARP_LOAD_MTD_NAME      3

#define WARP_BF_LEN             0x100

#ifdef WARP_HIBDRV_FLOATING
#define WARP_HIBDRV_VIRT        warp_hibdrv_buf
#endif



#ifndef __ASSEMBLY__

#if BITS_PER_LONG == 64
#define ptr_to_u64(x)           ((u64)(x))
#define u64_to_ptr(x)           ((void *)(x))
#else
#define ptr_to_u64(x)           ((u64)(u32)(x))
#define u64_to_ptr(x)           ((void *)(u32)(x))
#endif

enum warp_progress {
    WARP_PROGRESS_INIT,
    WARP_PROGRESS_SYNC,
    WARP_PROGRESS_FREEZE,
    WARP_PROGRESS_SHRINK,
    WARP_PROGRESS_SUSPEND,
    WARP_PROGRESS_SAVE,
    WARP_PROGRESS_SAVEEND,
    WARP_PROGRESS_RESUME,
    WARP_PROGRESS_THAW,
    WARP_PROGRESS_EXIT,
    WARP_PROGRESS_CANCEL,
};

struct warp_savetbl {
    u32         start;
    u32         end;
};

struct warp_savetbl64 {
    u64         start;
    u64         end;
};

struct warp {
    u16         ver_major;
    u16         ver_minor;
    s16         page_shift;
    s16         switch_mode;
    s16         compress;
    s16         oneshot;
    s16         halt;
    s16         silent;
    s32         console;
    s32         bps;
    u32         bootflag_dev;
    u32         bootflag_area;
    u32         bootflag_size;
    u32         snapshot_dev;
    u32         snapshot_area;
    u32         snapshot_size;
    u64         v2p_offset;
    u64         hd_savearea;
    u64         hd_savearea_end;
    u64         subcpu_info;
    u64         zonetbl;
    u64         dramtbl;
    u64         exttbl;
    s32         zonetbl_num;
    s32         dramtbl_num;
    s32         exttbl_num;
    s32         preload_exttbl;
    u32         lowmem_size;
    u32         maxarea;
    u32         maxsize;
    u32         lowmem_maxarea;
    u32         lowmem_maxsize;
    u32         snapshot_id;
    u32         private[4];
    u64         text_v2p_offset;
    u32         text_pfn;
    u32         text_size;
    u16         subcpu_num;
    u16         cpu_no;
    u32         reserve[3];
    u32         stat;
    u32         retry;
};

struct warp_savearea {
    int         bootflag_load_mode;
    u32         bootflag_offset;
    char        *bootflag_part;
    u32         bootflag_dev;
    u32         bootflag_area;
    u32         bootflag_size;
    u32         snapshot_dev;
    u32         snapshot_area;
    u32         snapshot_size;
};

#ifdef WARP_HIBDRV_AMP

struct warp_mp_info {
    s8          cpu;
    s8          percent;
    s8          silent;
    s8          console;
    s32         bps;
    u32         bootflag_dev;
    u32         bootflag_area;
    u32         bootflag_size;
    u32         snapshot_dev;
    u32         snapshot_area;
    u32         snapshot_size;
};

#endif

struct warp_hibdrv_area {
    unsigned int nand_id;
    int load_mode;
    u32 offset;
    u32 area_size;
};

struct warp_ops {
    int (*drv_load)(void *buf, size_t size);
    int (*bf_load)(void *buf, int loadno);
    int (*drv_init)(void);
    int (*device_suspend_early)(void);
    int (*device_suspend)(void);
    int (*pre_snapshot)(void);
    int (*snapshot)(void);
    void (*post_snapshot)(void);
    void (*device_resume)(void);
    void (*device_resume_late)(void);
    void (*drv_uninit)(void);
    void (*putc)(char c);
    void (*progress)(int val);
};

int swsusp_page_is_saveable(struct zone *zone, unsigned long pfn);

int hibdrv_snapshot(void);
int warp_register_machine(struct warp_ops *ops);
int warp_unregister_machine(struct warp_ops *ops);

#ifdef CONFIG_MTD
int warp_mtd_load(int mtdno, void *buf, size_t size);
int warp_mtd_load_nm(const char *mtdname, void *buf, size_t size);
int warp_mtd_bf_load(int mtdno, void *buf, loff_t offs, size_t size);
int warp_mtd_bf_load_nm(const char *mtdname, void *buf,
                        loff_t offs, size_t size);
#endif

#ifdef WARP_HIBDRV_DEV_LOAD
int warp_dev_load(const char *dev, void *buf, size_t size);
#endif

int warp_dev_bf_load(const char *dev, void *buf, loff_t offs, size_t size);

extern struct warp warp_param;
#if 0
extern struct warp_savearea *warp_savearea;
#else
extern struct warp_savearea *warp_savearea;
extern int warp_savearea_num;
#endif

#define WARP_HIBDRVAREA_NUM     (sizeof(warp_hibdrv_area) / \
                                 sizeof(struct warp_hibdrv_area))
extern struct warp_hibdrv_area warp_hibdrv_area[];
extern int warp_devno;

void warp_chip_select (unsigned int id);

extern unsigned char *warp_hibdrv_buf;
extern unsigned char *warp_bootflag_buf;

#ifdef WARP_DEBUG_NO_HIBDRV

#define WARP_ID(drv)                    WARP_ID_DRIVER
#define WARP_DRV_COPY_SIZE(drv)         0x00008000
#define _WARP_DRV_SNAPSHOT(drv, x)      0
#define _WARP_DRV_SWITCH(drv, x)        0

#else   /* WARP_DEBUG_NO_HIBDRV */

#define WARP_ID(drv)            (*(u32 *)((void *)(drv) + WARP_HEADER_ID))
#define WARP_DRV_COPY_SIZE(drv) \
    (*(u32 *)((void *)(drv) + WARP_HEADER_COPY_SIZE))

#define _WARP_DRV_SNAPSHOT(drv, x) \
    ((int (*)(void *))((void *)(drv) + WARP_HEADER_SNAPSHOT))(x)
#define _WARP_DRV_SWITCH(drv, x) \
    ((int (*)(void *))((void *)(drv) + WARP_HEADER_SWITCH))(x)

#endif  /* WARP_DEBUG_NO_HIBDRV */

#ifndef WARP_DRV_SNAPSHOT
#define WARP_DRV_SNAPSHOT(drv, x)       _WARP_DRV_SNAPSHOT(drv, x)
#endif

#ifndef WARP_DRV_SWITCH
#define WARP_DRV_SWITCH(drv, x)         _WARP_DRV_SWITCH(drv, x)
#endif

#endif  /* __ASSEMBLY__ */

#endif  /* _LINUX_WARP_PARAM_H */
