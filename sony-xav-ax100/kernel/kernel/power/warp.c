/*
 * Warp!! common driver  Rev. 4.0.8
 *
 *  Copyright (C) 2008-2014  Lineo Solutions, Inc.
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 */

#include <linux/module.h>
#include <linux/suspend.h>
#include <linux/syscalls.h>
#include <linux/console.h>
#include <linux/cpu.h>
#include <linux/fs.h>
#include <linux/proc_fs.h>
#include <linux/version.h>
#include <linux/buffer_head.h>
#include <linux/blkdev.h>
#include <linux/warp_param.h>
#include <asm/uaccess.h>

  #include "mach/apppart.h"

#include <mach/gp_chunkmem.h>

#include <linux/warp.h>

#ifdef WARP_UMOUNT_RW
#include <linux/delay.h>
#include <linux/namei.h>
//#include <linux/mount.h>
#include "../../fs/mount.h"
#endif
#ifdef WARP_AM_INOTIFY
#include <linux/inotify.h>
#endif

#ifdef CONFIG_MTD
#undef DEBUG
#include <linux/mtd/mtd.h>
#endif

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,20)
#include <linux/freezer.h>
#endif

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,30)
#include <asm/suspend.h>
#endif

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,39)
#include <linux/syscore_ops.h>
#endif

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,12)
#include <linux/random.h>
#endif

#include "power.h"

#ifndef WARP_WORK_SIZE
#define WARP_WORK_SIZE          (128 * 1024)
#endif

#ifndef WARP_HD_SAVEAREA_SIZE
#define WARP_HD_SAVEAREA_SIZE   4096
#endif

#define ZONETBL_DEFAULT_NUM     8
#define EXTTBL_DEFAULT_NUM      8

#define ZONETBL_DEFAULT_SIZE    (ZONETBL_DEFAULT_NUM * \
                                 sizeof(struct warp_savetbl))
#define EXTTBL_DEFAULT_SIZE     (EXTTBL_DEFAULT_NUM * \
                                 sizeof(struct warp_savetbl64))

#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,17)
#define SHRINK_BITE             INT_MAX
#else
#define SHRINK_BITE             ULONG_MAX
#endif

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,18) && \
    LINUX_VERSION_CODE <  KERNEL_VERSION(2,6,33)
#ifndef WARP_SHRINK_REPEAT
#define WARP_SHRINK_REPEAT      1
#endif
#else
#ifndef WARP_SHRINK_REPEAT
#define WARP_SHRINK_REPEAT      10
#endif
#ifndef WARP_SHRINK_REPEAT2
#define WARP_SHRINK_REPEAT2     1
#endif
#ifndef WARP_SHRINK_REPEAT3
#define WARP_SHRINK_REPEAT3     2
#endif
#endif

#ifndef WARP_SHRINK_REPEAT_P1
#define WARP_SHRINK_REPEAT_P1   10000
#endif

#ifndef WARP_SHRINK_THRESHOLD
#define WARP_SHRINK_THRESHOLD   1
#endif

#ifndef WARP_SHRINK_THRESHOLD_COUNT
#define WARP_SHRINK_THRESHOLD_COUNT     100
#endif

#if defined(WARP_SUSPEND_ERR_RECOVER) && defined(WARP_SUSPEND_ERR_NO_RECOVER)
#error "duplicate define WARP_SUSPEND_ERR_RECOVER & WARP_SUSPEND_ERR_NO_RECOVER"
#endif

#if !defined(WARP_SUSPEND_ERR_RECOVER) && !defined(WARP_SUSPEND_ERR_NO_RECOVER)
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,27)
#define WARP_SUSPEND_ERR_RECOVER
#endif
#endif

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,31)
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,0,0)
#define pm_device_suspend(x)    dpm_suspend(x)
#define pm_device_resume(x)     dpm_resume(x)
#else
#define pm_device_suspend(x)    dpm_suspend_start(x)
#define pm_device_resume(x)     dpm_resume_end(x)
#endif
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,4,0)
#define pm_device_power_down(x) dpm_suspend_end(x)
#define pm_device_power_up(x)   dpm_resume_start(x)
#else
#define pm_device_power_down(x) dpm_suspend_noirq(x)
#define pm_device_power_up(x)   dpm_resume_noirq(x)
#endif
#else
#define pm_device_suspend(x)    device_suspend(x)
#define pm_device_power_down(x) device_power_down(x)
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,27)
#define pm_device_resume(x)     device_resume(x)
#define pm_device_power_up(x)   device_power_up(x)
#else
#define pm_device_resume(x)     device_resume()
#define pm_device_power_up(x)   device_power_up()
#endif
#endif

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,11)
#define STATE_FREEZE    PMSG_FREEZE
#else
#define STATE_FREEZE    PM_SUSPEND_DISK
#endif

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,27)
#define STATE_RESTORE   (!warp_stat ? (ret ? PMSG_RECOVER : PMSG_THAW) : \
                         PMSG_RESTORE)
#else
#define STATE_RESTORE
#endif

#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,22)
#define hibernate       pm_suspend_disk
#endif

#if LINUX_VERSION_CODE < KERNEL_VERSION(3,9,0)
#define TOTAL_SWAPCACHE_PAGES   total_swapcache_pages
#define NR_SWAP_PAGES           nr_swap_pages
#else
#define TOTAL_SWAPCACHE_PAGES   total_swapcache_pages()
#define NR_SWAP_PAGES           get_nr_swap_pages()
#endif

#ifndef warp_pfn_valid
#define warp_pfn_valid(pfn)     pfn_valid(pfn)
#endif

#ifndef warp_flush_icache_range
#define warp_flush_icache_range(s, e)   flush_icache_range(s, e)
#endif

#ifndef WARP_AMP
#define warp_amp()              0
#define warp_amp_maincpu()      1
#endif
#if 0
#define WARP_SAVEAREA_NUM       warp_savearea_num
struct warp_savearea *warp_savearea;
int warp_savearea_num;
#else
#define WARP_SAVEAREA_NUM       warp_savearea_num
#endif

#ifdef WARP_UMOUNT_RW

#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,2,0)
#define freeze_user_processes   freeze_processes
#else
int freeze_user_processes(void);
int freeze_kernel_threads(void);
void thaw_kernel_threads(void);
#endif

#define WARP_UMOUNT_RW_NUM (sizeof(warp_umount_rw) / sizeof(char *))

static const char *warp_umount_rw[] = {
    WARP_UMOUNT_RW
};

static const char *warp_umount_rw_remount[] = {
    WARP_UMOUNT_RW_REMOUNT
};
#endif

int pm_device_down;
int warp_shrink;
int warp_swapout_disable;
int warp_separate_pass;
int warp_canceled;
struct warp warp_param;
int warp_fastboot=0;

EXPORT_SYMBOL(warp_fastboot);

#ifdef WARP_HIBDRV_AMP

#define WARP_SUBCPU_NUM         (sizeof(warp_mp_info) / \
                                 sizeof(struct warp_mp_info))

static struct warp_mp_info warp_mp_info[] = {
    WARP_MP_INFO
};

#else

#define WARP_SUBCPU_NUM         0

#endif

EXPORT_SYMBOL(warp_savearea);

EXPORT_SYMBOL(pm_device_down);
EXPORT_SYMBOL(warp_shrink);
EXPORT_SYMBOL(warp_swapout_disable);
EXPORT_SYMBOL(warp_separate_pass);
EXPORT_SYMBOL(warp_canceled);
EXPORT_SYMBOL(warp_param);

EXPORT_SYMBOL(hibdrv_snapshot);
EXPORT_SYMBOL(warp_set_savearea);
EXPORT_SYMBOL(warp_save_cancel);
EXPORT_SYMBOL(warp_register_machine);
EXPORT_SYMBOL(warp_unregister_machine);

static struct warp_ops *warp_ops;

static int warp_stat;
static int warp_error;
static int warp_retry;
static int warp_saveno;
static int warp_loadno;
static int warp_separate;

static int warp_save_pages;
static void *warp_work;
static unsigned long warp_work_size;
static unsigned long warp_work_start, warp_work_end;
static unsigned long warp_nosave_area, warp_nosave_size;
static unsigned long warp_lowmem_nosave_area, warp_lowmem_nosave_size;

static struct warp_savetbl *zonetbl, *dramtbl;
static struct warp_savetbl64 *exttbl;
static unsigned long zonetbl_max, dramtbl_max, exttbl_max;

#ifdef WARP_HIBDRV_FLOATING
unsigned char *warp_hibdrv_buf;
#endif
unsigned char *warp_bootflag_buf;

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,22) && defined(WARP_PFN_IS_NOSAVE)

extern const void __nosave_begin, __nosave_end;

int pfn_is_nosave(unsigned long pfn)
{
    unsigned long nosave_begin_pfn, nosave_end_pfn;

    nosave_begin_pfn = __pa(&__nosave_begin) >> PAGE_SHIFT;
    nosave_end_pfn = PAGE_ALIGN(__pa(&__nosave_end)) >> PAGE_SHIFT;
    return (pfn >= nosave_begin_pfn) && (pfn < nosave_end_pfn);
}

#endif

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,29)

bool system_entering_hibernation(void)
{
    return pm_device_down != WARP_STATE_NORMAL;
}

EXPORT_SYMBOL(system_entering_hibernation);

#endif

#ifdef CONFIG_PM_WARP_DEBUG

void warp_putc(char c)
{
    if (warp_ops->putc) {
        if (c == '\n')
            warp_ops->putc('\r');
        warp_ops->putc(c);
    }
}

int warp_printf(const char *fmt, ...)
{
    int i, len;
    va_list args;
    char buf[256];

    va_start(args, fmt);
    len = vsnprintf(buf, sizeof(buf), fmt, args);
    va_end(args);
    for (i = 0; i < len; i++)
        warp_putc(buf[i]);
    return len;
}

EXPORT_SYMBOL(warp_putc);
EXPORT_SYMBOL(warp_printf);

#endif

#ifdef CONFIG_MTD

static int warp_mtd_offs(struct mtd_info *mtd, loff_t *offs, loff_t end)
{
    while (*offs < end) {
#if LINUX_VERSION_CODE < KERNEL_VERSION(3,4,0)
        if (!mtd->block_isbad || !mtd->block_isbad(mtd, *offs))
#else
        if (!mtd->_block_isbad || !mtd->_block_isbad(mtd, *offs))
#endif
            return 0;
        *offs += mtd->erasesize;
    }
    return -EIO;
}

#endif

static struct file *warp_dev_open(const char *dev)
{
    struct file *f;
    char name[256];

    name[255] = '\0';
    if (*dev == '/') {
        strncpy(name, dev, 255);
    } else {
        strcpy(name, "/dev/");
        strncpy(name + 5, dev, 250);
    }
    f = filp_open(name, O_RDONLY, 0777);
    if (IS_ERR(f)) {
        if (*dev != '/') {
            /* for Android */
            strcpy(name, "/dev/block/");
            strncpy(name + 11, dev, 244);
            f = filp_open(name, O_RDONLY, 0777);
        }
        if (IS_ERR(f))
            printk("Can't open device %s (%d).\n", dev, (int)PTR_ERR(f));
    }

    return f;
}

#ifdef WARP_HIBDRV_FLOATING

#ifdef CONFIG_MTD

static int warp_load_drv_mtd(struct mtd_info *mtd, loff_t offs, loff_t end,
                             void *buf, size_t size)
{
    int ret;
    size_t read_size, req_size, remain;

    if (IS_ERR(mtd))
        return PTR_ERR(mtd);

    if ((ret = warp_mtd_offs(mtd, &offs, end)) < 0) {
        put_mtd_device(mtd);
        return ret;
    }
#if LINUX_VERSION_CODE < KERNEL_VERSION(3,4,0)
    ret = mtd->read(mtd, offs, 256, &read_size, buf);
#else
    read_size = 0;
    ret = mtd->_read(mtd, offs, 256, &read_size, buf);
#endif
    if (mtd_is_bitflip(ret))
        ret = 0;
    if (ret >= 0) {
        if (WARP_ID(buf) != WARP_ID_DRIVER) {
            put_mtd_device(mtd);
            return 1;
        }
        if (size <= WARP_DRV_COPY_SIZE(buf)) {
            ret = -ENOMEM;
        } else {
            remain = WARP_DRV_COPY_SIZE(buf);
            for (;;) {
                if ((req_size = remain) > mtd->erasesize)
                    req_size = mtd->erasesize;
#if LINUX_VERSION_CODE < KERNEL_VERSION(3,4,0)
                ret = mtd->read(mtd, offs, req_size, &read_size, buf);
#else
                read_size = 0;
                ret = mtd->_read(mtd, offs, req_size, &read_size, buf);
#endif
                if (mtd_is_bitflip(ret))
                    ret = 0;
                if (ret < 0)
                    break;
                if (req_size != read_size) {
                    ret = -EIO;
                    break;
                }
                remain -= req_size;
                offs += req_size;
                buf += req_size;
                if (remain <= 0)
                    break;
                if ((ret = warp_mtd_offs(mtd, &offs, end)) < 0)
                    break;
            }
        }
    }

    put_mtd_device(mtd);
    return ret;
}

int warp_load_drv_mtd_no(int mtdno, loff_t offs, loff_t end,
                         void *buf, size_t size)
{
    return warp_load_drv_mtd(get_mtd_device(NULL, mtdno), offs, end, buf, size);
}

EXPORT_SYMBOL(warp_load_drv_mtd_no);

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,20)

int warp_load_drv_mtd_nm(const char *mtdname, loff_t offs, loff_t end,
                         void *buf, size_t size)
{
    return warp_load_drv_mtd(get_mtd_device_nm(mtdname), offs, end, buf, size);
}

EXPORT_SYMBOL(warp_load_drv_mtd_nm);

#endif

#endif  /* CONFIG_MTD */

#if WARP_HIBDRV_LOAD_MODE == WARP_LOAD_DEV

int warp_load_drv_dev(const char *dev, loff_t offs, void *buf, size_t size)
{
    int ret = 0;
    struct file *f;

    f = warp_dev_open(dev);
    if (IS_ERR(f))
        return PTR_ERR(f);

    if ((ret = kernel_read(f, offs, buf, PAGE_SIZE)) >= 0) {
        if (WARP_ID(buf) != WARP_ID_DRIVER) {
            filp_close(f, NULL);
            return 1;
        }
        if (size <= WARP_DRV_COPY_SIZE(buf)) {
            ret = -ENOMEM;
        } else {
            if (WARP_DRV_COPY_SIZE(buf) <= PAGE_SIZE ||
                (ret = kernel_read(f, offs + PAGE_SIZE, buf + PAGE_SIZE,
                                   WARP_DRV_COPY_SIZE(buf) - PAGE_SIZE)) >= 0) {
                filp_close(f, NULL);
                return 0;
            }
        }
    }

    filp_close(f, NULL);
    return ret;
}

EXPORT_SYMBOL(warp_load_drv_dev);

#endif

#if WARP_HIBDRV_LOAD_MODE == WARP_LOAD_MEM

int warp_load_drv_mem(const void *hibdrv_addr, void *buf, size_t size)
{
    if (WARP_ID(hibdrv_addr) != WARP_ID_DRIVER)
        return 1;
    if (size <= WARP_DRV_COPY_SIZE(hibdrv_addr))
        return -ENOMEM;

    memcpy(buf, hibdrv_addr, WARP_DRV_COPY_SIZE(hibdrv_addr));
    return 0;
}

EXPORT_SYMBOL(warp_load_drv_mem);

#endif

static int warp_load_drv(size_t size)
{
    int ret = -EIO;
    if (warp_ops->drv_load) {
        ret = warp_ops->drv_load(warp_hibdrv_buf, size);
    } else {
#if WARP_HIBDRV_LOAD_MODE == WARP_LOAD_MEM
        ret = warp_load_drv_mem((const void *)WARP_HIBDRV_MEM,
                                warp_hibdrv_buf, size);
#elif WARP_HIBDRV_LOAD_MODE == WARP_LOAD_DEV
        ret = warp_load_drv_dev(WARP_HIBDRV_DEV, WARP_HIBDRV_OFFSET,
                                warp_hibdrv_buf, size);
#elif WARP_HIBDRV_LOAD_MODE == WARP_LOAD_MTD_NO
        ret = warp_load_drv_mtd_no(WARP_HIBDRV_MTD_NO, WARP_HIBDRV_OFFSET,
                                   WARP_HIBDRV_OFFSET + WARP_HIBDRV_AREA_SIZE,
                                   warp_hibdrv_buf, size);
#elif WARP_HIBDRV_LOAD_MODE == WARP_LOAD_MTD_NAME
#if 0
		        ret = warp_load_drv_mtd_nm(WARP_HIBDRV_MTD_NAME, WARP_HIBDRV_OFFSET,
				                                   WARP_HIBDRV_OFFSET + WARP_HIBDRV_AREA_SIZE,
												                                      warp_hibdrv_buf, size);
#else
				       ret = apppart_load_app_image(6, warp_hibdrv_buf, size);
#endif
#endif
    }

    if (ret == 1) {
        printk("Can't find hibernation driver.\n");
        ret = -EIO;
    } else if (ret < 0) {
        printk("Can't load hibernation driver.\n");
    }
    return ret;
}

#endif  /* WARP_HIBDRV_FLOATING */

#ifdef CONFIG_MTD

static int warp_load_bf_mtd(struct mtd_info *mtd, loff_t offs, loff_t end,
                            void *buf)
{
    int ret;
    size_t read_size;

    if (IS_ERR(mtd))
        return PTR_ERR(mtd);

    if ((ret = warp_mtd_offs(mtd, &offs, end)) < 0) {
        put_mtd_device(mtd);
        return ret;
    }

#if LINUX_VERSION_CODE < KERNEL_VERSION(3,4,0)
    ret = mtd->read(mtd, offs, WARP_BF_LEN, &read_size, buf);
#else
    read_size = 0;
    ret = mtd->_read(mtd, offs, WARP_BF_LEN, &read_size, buf);
#endif
    if (mtd_is_bitflip(ret))
        ret = 0;
    put_mtd_device(mtd);
    return ret;
}

int warp_load_bf_mtd_no(int mtdno, loff_t offs, loff_t end, void *buf)
{
    return warp_load_bf_mtd(get_mtd_device(NULL, mtdno), offs, end, buf);
}

EXPORT_SYMBOL(warp_load_bf_mtd_no);

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,20)

int warp_load_bf_mtd_nm(const char *mtdname, loff_t offs, loff_t end, void *buf)
{
    return warp_load_bf_mtd(get_mtd_device_nm(mtdname), offs, end, buf);
}

EXPORT_SYMBOL(warp_load_bf_mtd_nm);

#endif

#endif  /* CONFIG_MTD */

int warp_load_bf_dev(const char *dev, loff_t offs, void *buf)
{
    int ret;
    struct file *f;

    f = warp_dev_open(dev);
    if (IS_ERR(f))
        return PTR_ERR(f);
    ret = kernel_read(f, offs, buf, WARP_BF_LEN);
    filp_close(f, NULL);
    return ret;
}

EXPORT_SYMBOL(warp_load_bf_dev);

static int _warp_load_bf(int loadno, unsigned char *bootflag_buf)
{
    int ret = -EIO;
    struct warp_savearea *area = &warp_savearea[loadno];

    if (warp_ops->bf_load) {
        ret = warp_ops->bf_load(bootflag_buf, loadno);
    } else if (area->bootflag_load_mode == WARP_LOAD_MEM) {
        memcpy(bootflag_buf, (void *)area->bootflag_offset, WARP_BF_LEN);
        ret = 0;
    } else if (area->bootflag_load_mode == WARP_LOAD_DEV) {
        ret = warp_load_bf_dev(area->bootflag_part, area->bootflag_offset,
                               bootflag_buf);
#ifdef CONFIG_MTD
    } else if (area->bootflag_load_mode == WARP_LOAD_MTD_NO) {
        ret = warp_load_bf_mtd_no((int)area->bootflag_part,
                                  area->bootflag_offset,
                                  area->bootflag_offset + area->bootflag_size,
                                  bootflag_buf);
    } else if (area->bootflag_load_mode == WARP_LOAD_MTD_NAME) {
        ret = warp_load_bf_mtd_nm(area->bootflag_part, area->bootflag_offset,
                                  area->bootflag_offset + area->bootflag_size,
                                  bootflag_buf);
#endif
    }

    if (ret < 0) {
        printk("Can't load bootflag.\n");
    } else if (*(unsigned long *)bootflag_buf != WARP_ID_BOOTFLAG) {
        printk("Can't find bootflag.\n");
        ret = -EIO;
    }
    return ret;
}

static int warp_load_bf(void)
{
    int ret;

    if ((ret = _warp_load_bf(warp_loadno, warp_bootflag_buf)) < 0)
        return ret;

#ifdef WARP_HIBDRV_AMP
    {
        unsigned char *bootflag_buf = warp_bootflag_buf;
        int cpu, nr_subcpus = ((u16 *)warp_bootflag_buf)[0x79];

        for (cpu = 1; cpu <= nr_subcpus; cpu++) {
            bootflag_buf += WARP_BF_LEN;
            if ((ret = _warp_load_bf(warp_loadno + cpu, bootflag_buf)) < 0)
                return ret;
            if ((((u32 *)warp_bootflag_buf)[1] != ((u32 *)bootflag_buf)[1])) {
                printk("No match AMP bootflag.\n");
                return -EIO;
            }
        }
    }
#endif
    return 0;
}

static int warp_work_alloc(void)
{
    int work_size;
    void *hd_savearea;

    for (work_size = WARP_WORK_SIZE; ; work_size >>= 1) {
        if (work_size < PAGE_SIZE) {
            printk("warp: Can't alloc work memory.\n");
            return -ENOMEM;
        }
        if ((warp_work = kmalloc(work_size, GFP_KERNEL)) != NULL)
            break;
    }
    warp_work_size = work_size;

    hd_savearea = kmalloc(WARP_HD_SAVEAREA_SIZE, GFP_KERNEL);
    if (hd_savearea == NULL) {
        printk("warp: Can't alloc HD savearea memory.\n");
        return -ENOMEM;
    }
    warp_param.hd_savearea = (unsigned long)hd_savearea;
    warp_param.hd_savearea_end = warp_param.hd_savearea + WARP_HD_SAVEAREA_SIZE;

    return 0;
}

static int warp_work_init(void)
{
    int table_size;
#ifdef WARP_HIBDRV_FLOATING
    int warp_hibdrv_size;
#endif
    struct warp_savetbl *savetbl;

#ifdef WARP_AMP
    if (warp_amp() && !warp_amp_maincpu()) {
        warp_work = (void *)WARP_AMP_WORK;
        warp_work_size = WARP_AMP_WORK_SIZE;
    } else
#endif
    {
#ifndef WARP_WORK_ALLOC_INIT
        int ret;
        if ((ret = warp_work_alloc()) < 0)
            return ret;
#endif
    }
    printk("Warp work area 0x%p-0x%p\n", warp_work, warp_work + warp_work_size);
    printk("Warp HD savearea 0x%p-0x%p\n",
           (void *)(unsigned long)warp_param.hd_savearea,
           (void *)(unsigned long)(warp_param.hd_savearea +
                                   WARP_HD_SAVEAREA_SIZE));


#ifdef WARP_HIBDRV_FLOATING
#ifdef WARP_AMP
    if (warp_amp()) {
        warp_hibdrv_buf = (void *)WARP_AMP_WORK;
        warp_hibdrv_size = WARP_AMP_WORK_SIZE;
    } else
#endif
    {
        warp_hibdrv_buf = warp_work;
        warp_hibdrv_size = warp_work_size;
    }
    if (warp_amp_maincpu()) {
        int ret;
        if ((ret = warp_load_drv(warp_hibdrv_size)) < 0) {
#ifndef WARP_WORK_ALLOC_INIT
            kfree(warp_work);
#endif
            return ret;
        }
        warp_hibdrv_size = WARP_DRV_COPY_SIZE(warp_hibdrv_buf);
    }
    warp_flush_icache_range((unsigned long)warp_hibdrv_buf,
                            (unsigned long)warp_hibdrv_buf + warp_hibdrv_size);
    if (warp_amp() && warp_amp_maincpu()) {
        savetbl = warp_work;
        table_size = warp_work_size;
    } else {
        savetbl = warp_work + WARP_DRV_COPY_SIZE(warp_hibdrv_buf);
        table_size = warp_work_size - WARP_DRV_COPY_SIZE(warp_hibdrv_buf);
    }
#else
    savetbl = warp_work;
    table_size = warp_work_size;
#endif

    if (warp_param.switch_mode) {
        int ret;
#ifndef WARP_HIBDRV_FLOATING
        if (warp_param.switch_mode == 1) {
            warp_bootflag_buf = (void *)WARP_HIBDRV_VIRT +
                WARP_DRV_COPY_SIZE(WARP_HIBDRV_VIRT);
        } else
#endif
        {
            warp_bootflag_buf = (void *)savetbl;
            savetbl = (void *)savetbl + WARP_BF_LEN * (WARP_SUBCPU_NUM + 1);
            table_size -= WARP_BF_LEN * (WARP_SUBCPU_NUM + 1);
        }
        if ((ret = warp_load_bf()) < 0) {
#ifndef WARP_WORK_ALLOC_INIT
            kfree(warp_work);
#endif
            return ret;
        }
    }

    zonetbl_max = ZONETBL_DEFAULT_NUM;
    exttbl_max = EXTTBL_DEFAULT_NUM;
    dramtbl_max = (table_size - ZONETBL_DEFAULT_SIZE - EXTTBL_DEFAULT_SIZE) /
        sizeof(struct warp_savetbl);
    dramtbl = savetbl;
    exttbl = (void *)(dramtbl + dramtbl_max);
    zonetbl = (void *)(exttbl + EXTTBL_DEFAULT_NUM);
    warp_param.zonetbl_num = 0;
    warp_param.dramtbl_num = 0;
    warp_param.exttbl_num = 0;
    warp_work_start = page_to_pfn(virt_to_page(warp_work));
    warp_work_end = warp_work_start + ((warp_work_size - 1) >> PAGE_SHIFT) + 1;
    warp_nosave_area = (unsigned long)-1;
    warp_nosave_size = 0;
    warp_lowmem_nosave_area = (unsigned long)-1;
    warp_lowmem_nosave_size = 0;
    warp_param.maxarea = 0;
    warp_param.maxsize = 0;
    warp_param.lowmem_maxarea = 0;
    warp_param.lowmem_maxsize = 0;
    warp_save_pages = 0;

#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,12)
    {
        struct timeval tv;
        do_gettimeofday(&tv);
        warp_param.snapshot_id = tv.tv_sec;
    }
#else
    warp_param.snapshot_id = get_random_int();
#endif

    return 0;
}

static void warp_work_free(void)
{
#ifndef WARP_WORK_ALLOC_INIT
    if (warp_amp_maincpu())
        kfree(warp_work);
#endif
}

static void warp_set_tbl(u32 start, u32 end,
                         struct warp_savetbl *tbl, int *num)
{
    if (*num > 0 && start == tbl[*num - 1].end) {
        tbl[*num - 1].end = end;
    } else if (start < end) {
        tbl[*num].start = start;
        tbl[*num].end = end;
        (*num)++;
    }
}

static void warp_set_tbl64(u64 start, u64 end,
                           struct warp_savetbl64 *tbl, int *num)
{
    if (*num > 0 && start == tbl[*num - 1].end) {
        tbl[*num - 1].end = end;
    } else if (start < end) {
        tbl[*num].start = start;
        tbl[*num].end = end;
        (*num)++;
    }
}

int warp_set_savearea(u64 start, u64 end)
{
    struct warp_savetbl64 *tbl;

    if ((start | end) & 3) {
        printk("warp_set_savearea: unsupported alignment\n");
        return -EINVAL;
    }

    if (warp_param.exttbl_num >= exttbl_max) {
        if (warp_param.dramtbl_num + EXTTBL_DEFAULT_NUM * 2 > dramtbl_max) {
            printk("warp: save table overflow\n");
            return -ENOMEM;
        }
        tbl = exttbl - EXTTBL_DEFAULT_NUM;
        memmove(tbl, exttbl, exttbl_max * sizeof(struct warp_savetbl64));
        exttbl = tbl;
        exttbl_max += EXTTBL_DEFAULT_NUM;
        dramtbl_max -= EXTTBL_DEFAULT_NUM;
    }
    warp_set_tbl64(start, end, exttbl, &warp_param.exttbl_num);
    return 0;
}

static int warp_set_save_zones(unsigned long start, unsigned long end)
{
    struct warp_savetbl64 *tbl;

    if (warp_param.zonetbl_num >= zonetbl_max) {
        if (warp_param.dramtbl_num + ZONETBL_DEFAULT_NUM > dramtbl_max) {
            printk("warp: save table overflow\n");
            return -ENOMEM;
        }
        tbl = (void *)exttbl - ZONETBL_DEFAULT_SIZE;
        memmove(tbl, exttbl,
                exttbl_max * sizeof(struct warp_savetbl64) +
                zonetbl_max * sizeof(struct warp_savetbl));
        exttbl = tbl;
        zonetbl -= ZONETBL_DEFAULT_NUM;
        zonetbl_max += ZONETBL_DEFAULT_NUM;
        dramtbl_max -= EXTTBL_DEFAULT_NUM;
    }
    warp_set_tbl(start, end, zonetbl, &warp_param.zonetbl_num);
    return 0;
}

static int warp_set_save_zone(unsigned long pfn)
{
    return warp_set_save_zones(pfn, pfn + 1);
}

static int warp_set_save_drams(unsigned long start, unsigned long end)
{
    if (warp_param.dramtbl_num >= dramtbl_max) {
        printk("warp: save table overflow\n");
        return -ENOMEM;
    }
    warp_set_tbl(start, end, dramtbl, &warp_param.dramtbl_num);
    return 0;
}

static int warp_set_save_dram(unsigned long pfn)
{
    return warp_set_save_drams(pfn, pfn + 1);
}

static void warp_set_nosave_dram(struct zone *zone, unsigned long pfn)
{
    if (pfn == warp_nosave_area + warp_nosave_size) {
        warp_nosave_size++;
        if (warp_param.maxsize < warp_nosave_size) {
            warp_param.maxarea = warp_nosave_area;
            warp_param.maxsize = warp_nosave_size;
        }
    } else {
        warp_nosave_area = pfn;
        warp_nosave_size = 1;
    }
    if (!is_highmem(zone)) {
        if (pfn == warp_lowmem_nosave_area + warp_lowmem_nosave_size) {
            warp_lowmem_nosave_size++;
            if (warp_param.lowmem_maxsize < warp_lowmem_nosave_size) {
                warp_param.lowmem_maxarea = warp_lowmem_nosave_area;
                warp_param.lowmem_maxsize = warp_lowmem_nosave_size;
            }
        } else {
            warp_lowmem_nosave_area = pfn;
            warp_lowmem_nosave_size = 1;
        }
    }
}

static int warp_make_save_table(void)
{
    int ret;
    struct zone *zone;
    unsigned long pfn, end;
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,11)
    unsigned long pfn2;
#endif

    for_each_zone (zone) {
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,11)
        mark_free_pages(zone);
#endif
        end = zone->zone_start_pfn + zone->spanned_pages;
        for (pfn = zone->zone_start_pfn; pfn < end; pfn++) {
            if (!warp_pfn_valid(pfn) ||
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,22)
                swsusp_page_is_forbidden(pfn_to_page(pfn)) ||
#endif
                (pfn >= warp_work_start && pfn < warp_work_end))
                continue;
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,11)
            pfn2 = pfn;
            if (swsusp_page_is_saveable(zone, &pfn2)) {
                warp_save_pages++;
                if ((ret = warp_set_save_zone(pfn)) < 0)
                    return ret;
                if ((ret = warp_set_save_dram(pfn)) < 0)
                    return ret;
            } else {
                pfn--;
                while (pfn < pfn2) {
                    if ((ret = warp_set_save_zone(++pfn)) < 0)
                        return ret;
                    warp_set_nosave_dram(zone, pfn);
                }
            }
#else
            if ((ret = warp_set_save_zone(pfn)) < 0)
                return ret;
            if (swsusp_page_is_saveable(zone, pfn)) {
                warp_save_pages++;
                if ((ret = warp_set_save_dram(pfn)) < 0)
                    return ret;
            } else {
                warp_set_nosave_dram(zone, pfn);
            }
#endif
        }
    }
    return 0;
}

#ifdef WARP_AMP

static int warp_merge_savearea(struct warp *warp_param1)
{
    int i, ret;
    struct warp_savetbl *p;
    struct warp_savetbl64 *p64;

    p = (void *)warp_param1->zonetbl;
    for (i = 0; i < warp_param1->zonetbl_num; i++, p++) {
        if ((ret = warp_set_save_zones(p->start, p->end)) < 0)
            return ret;
    }
    p = (void *)warp_param1->dramtbl;
    for (i = 0; i < warp_param1->dramtbl_num; i++, p++) {
        if ((ret = warp_set_save_drams(p->start, p->end)) < 0)
            return ret;
    }
    p64 = (void *)warp_param1->exttbl;
    for (i = 0; i < warp_param1->exttbl_num; i++, p64++) {
        if ((ret = warp_set_savearea(p64->start, p64->end)) < 0)
            return ret;
    }

    return 0;
}

#endif

#ifdef WARP_PRINT_SAVETBL

static void warp_print_savetbl32(struct warp_savetbl *tbl, int num, char *name)
{
    int i;

    for (i = 0; i < num; i++, tbl++) {
        printk("%4s area %4d: %08x - %08x  len: %08x\n",
               name, i, tbl->start, tbl->end, tbl->end - tbl->start);
    }
}

static void warp_print_savetbl64(struct warp_savetbl64 *tbl,
                                 int num, char *name)
{
    int i;

    for (i = 0; i < num; i++, tbl++) {
        printk("%4s area %4d: %016llx - %016llx  len: %016llx\n", name, i,
               (unsigned long long)tbl->start, (unsigned long long)tbl->end,
               (unsigned long long)(tbl->end - tbl->start));
    }
}

static void warp_print_savetbl(void)
{
    warp_print_savetbl32(zonetbl, warp_param.zonetbl_num, "Zone");
    warp_print_savetbl32(dramtbl, warp_param.dramtbl_num, "Dram");
    warp_print_savetbl64(exttbl, warp_param.exttbl_num, "Ext");
}

#endif

#ifdef WARP_PRINT_MEMINFO

#define K(x) ((x) << (PAGE_SHIFT - 10))

static void warp_print_meminfo(void)
{
    struct sysinfo si;
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,18)
    struct page_state ps;
#endif
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,21)
    unsigned long active, inactive, free;
#endif
    unsigned long buffers, cached, dirty, mapped;

#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,21)
    get_zone_counts(&active, &inactive, &free);
#endif
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,18)
    get_page_state(&ps);
    dirty = ps.nr_dirty;
    mapped = ps.nr_mapped;
    cached = get_page_cache_size();
#else
    dirty = global_page_state(NR_FILE_DIRTY);
    mapped = global_page_state(NR_FILE_MAPPED);
    cached = global_page_state(NR_FILE_PAGES);
#endif
    buffers = nr_blockdev_pages();
    cached -= TOTAL_SWAPCACHE_PAGES + buffers;
    si_swapinfo(&si);

    printk("Buffers        :%8lu KB\n"
           "Cached         :%8lu KB\n"
           "SwapCached     :%8lu KB\n"
           "SwapUsed       :%8lu KB\n",
           K(buffers),
           K(cached),
           K(TOTAL_SWAPCACHE_PAGES),
           K(si.totalswap - si.freeswap));
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,21)
    printk("Active         :%8lu KB\n"
           "Inactive       :%8lu KB\n",
           K(active),
           K(inactive));
#elif LINUX_VERSION_CODE < KERNEL_VERSION(2,6,28)
    printk("Active         :%8lu KB\n"
           "Inactive       :%8lu KB\n",
           K(global_page_state(NR_ACTIVE)),
           K(global_page_state(NR_INACTIVE)));
#else
    printk("Active(anon)   :%8lu KB\n"
           "Inactive(anon) :%8lu KB\n"
           "Active(file)   :%8lu KB\n"
           "Inactive(file) :%8lu KB\n",
           K(global_page_state(NR_ACTIVE_ANON)),
           K(global_page_state(NR_INACTIVE_ANON)),
           K(global_page_state(NR_ACTIVE_FILE)),
           K(global_page_state(NR_INACTIVE_FILE)));
#endif
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,18)
    printk("AnonPages      :%8lu KB\n",
           K(global_page_state(NR_ANON_PAGES)));
#endif
    printk("Dirty          :%8lu KB\n"
           "Mapped         :%8lu KB\n",
           K(dirty),
           K(mapped));
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,18)
    printk("Slab           :%8lu kB\n"
           "PageTables     :%8lu kB\n",
           K(ps.nr_slab),
           K(ps.nr_page_table_pages));
#elif LINUX_VERSION_CODE < KERNEL_VERSION(2,6,19)
    printk("Slab           :%8lu kB\n"
           "PageTables     :%8lu kB\n",
           K(global_page_state(NR_SLAB)),
           K(global_page_state(NR_PAGETABLE)));
#else
    printk("SReclaimable   :%8lu kB\n"
           "SUnreclaim     :%8lu kB\n"
           "PageTables     :%8lu kB\n",
           K(global_page_state(NR_SLAB_RECLAIMABLE)),
           K(global_page_state(NR_SLAB_UNRECLAIMABLE)),
           K(global_page_state(NR_PAGETABLE)));
#endif
}

#else

#define warp_print_meminfo()

#endif

#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,18)

static void invalidate_filesystems(int invalidate)
{
    struct super_block *sb;
    struct block_device *bdev;

    spin_lock(&sb_lock);
restart:
    list_for_each_entry(sb, &super_blocks, s_list) {
        if (sb->s_bdev) {
            sb->s_count++;
            spin_unlock(&sb_lock);
            fsync_bdev(sb->s_bdev);
            if (invalidate)
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,12)
                __invalidate_device(sb->s_bdev);
#else
                __invalidate_device(sb->s_bdev, 0);
#endif
        } else {
            sb->s_count++;
            spin_unlock(&sb_lock);
            bdev = bdget(sb->s_dev);
            fsync_bdev(bdev);
            fsync_super(sb);
            if (invalidate) {
                shrink_dcache_sb(sb);
                invalidate_inodes(sb);
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,22)
                invalidate_bdev(bdev);
#else
                invalidate_bdev(bdev, 0);
#endif
            }
        }
        spin_lock(&sb_lock);
        if (__put_super_and_need_restart(sb))
            goto restart;
    }
    spin_unlock(&sb_lock);
}

#endif

static int warp_shrink_memory(void)
{
    int i, pages, pages_sum, threshold_cnt;
    int shrink_sav = warp_shrink;
    int repeat = WARP_SHRINK_REPEAT;

    if (!warp_swapout_disable) {
        warp_shrink = WARP_SHRINK_ALL;
        repeat = WARP_SHRINK_REPEAT_P1;
    }

    if (warp_shrink != WARP_SHRINK_NONE) {
        warp_print_meminfo();
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,18)
        invalidate_filesystems(1);
#endif
#if LINUX_VERSION_CODE <  KERNEL_VERSION(2,6,18) || \
    LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,33)
        if (warp_shrink == WARP_SHRINK_LIMIT1)
            repeat = WARP_SHRINK_REPEAT2;
        else if (warp_shrink == WARP_SHRINK_LIMIT2)
            repeat = WARP_SHRINK_REPEAT3;
#endif
        printk("Shrinking memory...  ");
        pages_sum = 0;
        threshold_cnt = 0;
        for (i = 0; i < repeat; i++) {
            pages = shrink_all_memory(SHRINK_BITE);
            if (pages <= WARP_SHRINK_THRESHOLD) {
                if (++threshold_cnt >= WARP_SHRINK_THRESHOLD_COUNT)
                    break;
            } else {
                threshold_cnt = 0;
            }
            pages_sum += pages;
        }
        printk("\bdone (%d pages freed)\n", pages_sum);
    }
    warp_print_meminfo();

    warp_shrink = shrink_sav;
    return 0;
}

static void sp_chunk_save_mem(unsigned long addr, unsigned long size)
{
    unsigned long start, end;

    start = addr & ~3;
    end = (addr + size + 3) & ~3;

    if (size < 16) {
        size = 32;
        end = start + 32;
    }
    warp_set_savearea(start, end);
}

int hibdrv_snapshot(void)
{
    int ret;

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,25)
    drain_local_pages(NULL);
#else
    drain_local_pages();
#endif
    if ((ret = warp_make_save_table()) < 0)
        return ret;

	gp_chunk_suspend(sp_chunk_save_mem);

    printk("dram save %d pages\n", warp_save_pages);
    printk("maxarea 0x%08x(0x%08x)  lowmem_maxarea 0x%08x(0x%08x)\n",
           warp_param.maxarea, warp_param.maxsize,
           warp_param.lowmem_maxarea, warp_param.lowmem_maxsize);
    printk("zonetbl %d  exttbl %d  dramtbl %d\n", warp_param.zonetbl_num,
           warp_param.exttbl_num, warp_param.dramtbl_num);

    if (warp_ops->progress)
        warp_ops->progress(WARP_PROGRESS_SAVE);

#ifdef WARP_AMP
    if (warp_amp() && warp_amp_maincpu()) {
        struct warp *warp_param1 = (struct warp *)WARP_AMP_PARAM;
        if ((ret = warp_amp_wait_subcpu_saveend()) >= 0) {
            warp_invalidate_dcache_range(WARP_AMP_PARAM,
                                         WARP_AMP_PARAM + sizeof(struct warp));
            warp_invalidate_dcache_range(WARP_AMP_WORK,
                                         WARP_AMP_WORK + WARP_AMP_WORK_SIZE);
            warp_merge_savearea(warp_param1);
            warp_param.subcpu_info = warp_param1->subcpu_info;
        } else if (ret == -ECANCELED) {
            warp_canceled = 1;
        }
    }
#endif

    if (warp_canceled) {
        ret = -ECANCELED;
    } else {
#ifdef WARP_PRINT_SAVETBL
        warp_print_savetbl();
#endif
        warp_param.zonetbl = __pa(zonetbl);
        warp_param.dramtbl = __pa(dramtbl);
        warp_param.exttbl = __pa(exttbl);
        if (warp_param.switch_mode == 1) {
            ret = WARP_DRV_SWITCH(WARP_HIBDRV_VIRT, &warp_param);
        } else if (warp_param.switch_mode == 2) {
            if ((ret = WARP_DRV_SNAPSHOT(WARP_HIBDRV_VIRT, &warp_param)) < 0) {
                if (ret == -ECANCELED)
                    warp_canceled = 1;
            } else if (warp_param.stat == 0) {
#ifndef WARP_HIBDRV_FLOATING
                memcpy((void *)WARP_HIBDRV_VIRT +
                       WARP_DRV_COPY_SIZE(WARP_HIBDRV_VIRT),
                       warp_bootflag_buf, WARP_BF_LEN);
#endif
                ret = WARP_DRV_SWITCH(WARP_HIBDRV_VIRT, &warp_param);
            }
        } else {
            if ((ret = WARP_DRV_SNAPSHOT(WARP_HIBDRV_VIRT,
                                         &warp_param)) == -ECANCELED)
                warp_canceled = 1;
        }
        if (ret < 0) {
            if (ret == -EIO)
                printk("hibdrv: I/O error\n");
            else if (ret == -ENOMEM)
                printk("hibdrv: Out of memory\n");
            else if (ret == -ENODEV)
                printk("hibdrv: No such device\n");
            else if (ret == -EINVAL)
                printk("hibdrv: Invalid argument\n");
            else if (ret == -ENOSPC)
                printk("hibdrv: No space left on snapshot device\n");
            else if (ret == -ETIMEDOUT)
                printk("hibdrv: Device timed out\n");
            else if (ret == -ECANCELED)
                printk("hibdrv: Operation Canceled\n");
            else
                printk("hibdrv: error %d\n", ret);
        }
    }

#ifdef WARP_AMP
    if (ret >= 0 && warp_amp()) {
        if (warp_amp_maincpu()) {
            if (warp_param.stat == 0)
                warp_amp_send_maincpu_saveend(0);
            else
                warp_amp_boot_subcpu();
        } else {
            if (warp_param.stat == 0) {
                memcpy((void *)WARP_AMP_PARAM, &warp_param,
                       sizeof(struct warp));
                warp_clean_dcache_range(WARP_AMP_PARAM,
                                        WARP_AMP_PARAM + sizeof(struct warp));
                warp_clean_dcache_range(WARP_AMP_WORK,
                                        WARP_AMP_WORK + WARP_AMP_WORK_SIZE);
                warp_amp_send_subcpu_saveend(0);
                if ((ret = warp_amp_wait_maincpu_saveend()) == -ECANCELED)
                    warp_canceled = 1;
            }
        }
    }
#endif

    if (warp_ops->progress)
        warp_ops->progress(WARP_PROGRESS_SAVEEND);

    return ret;
}

void warp_save_cancel(void)
{
    if (pm_device_down == WARP_STATE_SUSPEND || warp_separate_pass == 1) {
        warp_canceled = 1;
        if (warp_ops->progress)
            warp_ops->progress(WARP_PROGRESS_CANCEL);
    }
}

int hibernate(void)
{
    int ret;
#ifdef WARP_UMOUNT_RW
    int i, no, ret2;
    char *mnt_devname[WARP_UMOUNT_RW_NUM];
    char *mnt_type[WARP_UMOUNT_RW_NUM];
    unsigned long s_flags[WARP_UMOUNT_RW_NUM];
    char *mnt_options[WARP_UMOUNT_RW_NUM];
    int mnt_flags;
    mm_segment_t fs;
    struct path path;
    struct vfsmount *v_mnt;
    struct mount *s_mount;
#endif
    warp_fastboot=1;

    if (!warp_ops) {
        printk("Snapshot driver not found.\n");
        return -EIO;
    }

    if (warp_param.switch_mode == 2 && warp_saveno == warp_loadno) {
        printk("Switch error!! Snapshot saveno and loadno is same.\n");
        return -EINVAL;
    }

    BUG_ON(!warp_ops->snapshot);

    warp_stat = 0;
    warp_retry = 0;
    pm_device_down = WARP_STATE_SUSPEND;

#ifdef WARP_AMP
    if (warp_amp() && !warp_amp_maincpu())
        warp_canceled = 0;
#endif

    if (warp_separate == 0) {
        warp_separate_pass = 0;
        warp_swapout_disable = 1;
    } else if (warp_separate == 1) {
        warp_separate_pass = 0;
        warp_swapout_disable = 0;
    } else if (warp_separate == 2) {
        if (warp_separate_pass != 1) {
            warp_separate_pass = 1;
            warp_swapout_disable = 0;
        } else {
            warp_separate_pass = 2;
            warp_swapout_disable = 1;
        }
    }

    if (warp_ops->progress)
        warp_ops->progress(WARP_PROGRESS_INIT);

    if (warp_separate_pass == 2 && warp_canceled) {
        warp_separate_pass = 0;
        ret = -ECANCELED;
        goto warp_work_init_err;
    }

    if ((ret = warp_work_init()) < 0) {
        warp_separate_pass = 0;
        goto warp_work_init_err;
    }

#ifdef WARP_AMP
    if (warp_amp() && warp_amp_maincpu() && warp_separate_pass != 1)
        warp_amp_save_subcpu();
#endif

    if (warp_ops->drv_init && (ret = warp_ops->drv_init()) < 0) {
        warp_separate_pass = 0;
        goto warp_drv_init_err;
    }

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,16) && \
    LINUX_VERSION_CODE <  KERNEL_VERSION(2,6,24)
    system_state = SYSTEM_SUSPEND_DISK;
#endif

#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,3,0)
    lock_system_sleep();
#elif LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,22)
    mutex_lock(&pm_mutex);
#endif

#ifdef WARP_PREPARE_CONSOLE
    pm_prepare_console();
#endif

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,23)
    if ((ret = pm_notifier_call_chain(PM_HIBERNATION_PREPARE))) {
        warp_separate_pass = 0;
        pm_device_down = WARP_STATE_RESUME;
        goto pm_notifier_call_chain_err;
    }
#endif

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,28) && \
    LINUX_VERSION_CODE <  KERNEL_VERSION(3,4,0)
    if ((ret = usermodehelper_disable())) {
        warp_separate_pass = 0;
        pm_device_down = WARP_STATE_RESUME;
        goto usermodehelper_disable_err;
    }
#endif

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,22)
    /* Allocate memory management structures */
    if ((ret = create_basic_memory_bitmaps())) {
        warp_separate_pass = 0;
        pm_device_down = WARP_STATE_RESUME;
        goto create_basic_memory_bitmaps_err;
    }
#endif

#ifdef WARP_AM_INOTIFY
    printk("inotify_warp_wait Start.\n");
    inotify_warp_wait();
    printk("inotify_warp_wait Exit.\n");
#endif

    if (warp_ops->progress)
        warp_ops->progress(WARP_PROGRESS_SYNC);

    printk("Syncing filesystems ... ");
    sys_sync();
    printk("done.\n");

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,13) && \
    LINUX_VERSION_CODE <  KERNEL_VERSION(2,6,21)
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,19)
    if ((ret = disable_nonboot_cpus())) {
        warp_separate_pass = 0;
        pm_device_down = WARP_STATE_RESUME;
        goto disable_nonboot_cpus_err;
    }
#else
    disable_nonboot_cpus();
#endif
#endif

    if (warp_ops->progress)
        warp_ops->progress(WARP_PROGRESS_FREEZE);

#ifdef WARP_UMOUNT_RW
    for (no = 0; no < WARP_UMOUNT_RW_NUM; no++) {
        mnt_devname[no] = NULL;
        mnt_type[no] = NULL;
        mnt_options[no] = NULL;
    }
    fs = get_fs();
    for (no = 0; no < WARP_UMOUNT_RW_NUM; no++) {
        set_fs(KERNEL_DS);
        ret = user_path_at(AT_FDCWD, warp_umount_rw[no], 0, &path);
        set_fs(fs);
        if (ret < 0) {
            printk("user_path_at %s error %d\n", warp_umount_rw[no], ret);
            goto user_path_at_err;
        }

        v_mnt = path.mnt;
        s_mount = real_mount(v_mnt);
        mnt_devname[no] = kstrdup(s_mount->mnt_devname, GFP_KERNEL);
        if (!mnt_devname[no]) {
            printk("kstrdup %s error %d\n", s_mount->mnt_devname, ret);
            path_put(&path);
            goto user_path_at_err;
        }
        mnt_type[no] = kstrdup(path.mnt->mnt_sb->s_type->name, GFP_KERNEL);
        if (!mnt_type[no]) {
            printk("kstrdup %s error %d\n",
                   path.mnt->mnt_sb->s_type->name, ret);
            path_put(&path);
            goto user_path_at_err;
        }
        s_flags[no] = path.mnt->mnt_sb->s_flags;
        mnt_flags = path.mnt->mnt_flags;
        if (mnt_flags & MNT_NOSUID)
            s_flags[no] |= MS_NOSUID;
        if (mnt_flags & MNT_NODEV)
            s_flags[no] |= MS_NODEV;
        if (mnt_flags & MNT_NOEXEC)
            s_flags[no] |= MS_NOEXEC;
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,16)
        if (mnt_flags & MNT_NODIRATIME)
            s_flags[no] |= MS_NODIRATIME;
        if (mnt_flags & MNT_NOATIME)
            s_flags[no] |= MS_NOATIME;
#endif
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,30)
        else if (!(mnt_flags & MNT_RELATIME))
            s_flags[no] |= MS_STRICTATIME;
#elif LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,20)
        if (mnt_flags & MNT_RELATIME)
            s_flags[no] |= MS_RELATIME;
#endif
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,26)
        if (mnt_flags & MNT_READONLY)
            s_flags[no] |= MS_RDONLY;
#endif
        if (path.mnt->mnt_sb->s_op->show_options) {
            struct seq_file m;
            memset(&m, 0, sizeof(m));
            m.size = PAGE_SIZE - 1;
            m.buf = kzalloc(PAGE_SIZE, GFP_KERNEL);
            if (!m.buf) {
                printk("kzalloc %s options error %d\n",
                       warp_umount_rw[no], ret);
                path_put(&path);
                goto user_path_at_err;
            }
            mnt_options[no] = m.buf;
            if (path.mnt->mnt_sb->s_op->show_options(&m, path.dentry)) {
                printk("show_options %s error %d\n", warp_umount_rw[no], ret);
                path_put(&path);
                goto user_path_at_err;
            }
        }
        path_put(&path);
    }
    for (i = 0; i < 100; i++) {
        if ((ret = freeze_user_processes())) {
#if LINUX_VERSION_CODE < KERNEL_VERSION(3,3,0)
            ret = -EBUSY;
#endif
            break;
        }
        set_fs(KERNEL_DS);
        for (no = WARP_UMOUNT_RW_NUM - 1; no >= 0; no--) {
            if (mnt_devname[no]) {
                printk("umount %s ... ", warp_umount_rw[no]);
                ret = sys_umount((void *)warp_umount_rw[no], 0);
                if (ret == -EINVAL) {           /* none mount */
                    kfree(mnt_devname[no]);
                    mnt_devname[no] = NULL;
                    ret = 0;
                    printk("skip.\n");
                } else if (ret < 0) {
                    if (ret != -EBUSY)
                        printk("error %d\n", ret);
                    else
                        printk("busy.\n");
                    break;
                } else {
                    printk("done.\n");
                }
            }
        }
        set_fs(fs);
        if (ret >= 0)
            break;
        ret2 = 0;
        set_fs(KERNEL_DS);
        while (++no < WARP_UMOUNT_RW_NUM) {
            if (mnt_devname[no] && warp_umount_rw_remount[no]) {
                if ((ret2 = sys_mount(mnt_devname[no],
                                      (void *)warp_umount_rw[no],
                                      mnt_type[no], s_flags[no],
                                      mnt_options[no])) < 0) {
                    printk("mount %s %s error %d\n",
                           mnt_devname[no], warp_umount_rw[no], ret);
                    break;
                }
            }
        }
        set_fs(fs);
        if (ret != -EBUSY)
            break;
        if (ret2 < 0) {
            ret = ret2;
            break;
        }
        thaw_processes();
        msleep(10);
    }
    if (ret < 0) {
        warp_separate_pass = 0;
        pm_device_down = WARP_STATE_RESUME;
        goto freeze_processes_err;
    }
#if LINUX_VERSION_CODE < KERNEL_VERSION(3,2,0)
    if ((ret = freeze_kernel_threads())) {
         warp_separate_pass = 0;
         pm_device_down = WARP_STATE_RESUME;
         goto umount_rw_err;
     }
#endif
#else
    if (freeze_processes()) {
        ret = -EBUSY;
        warp_separate_pass = 0;
        pm_device_down = WARP_STATE_RESUME;
        goto freeze_processes_err;
    }
#endif

#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,0,0) && \
    LINUX_VERSION_CODE <  KERNEL_VERSION(3,2,0)
    if ((ret = dpm_prepare(PMSG_FREEZE))) {
        warp_separate_pass = 0;
        pm_device_down = WARP_STATE_RESUME;
        goto dpm_prepare_err;
    }
#endif

    if (warp_ops->progress)
        warp_ops->progress(WARP_PROGRESS_SHRINK);

    if (warp_param.switch_mode != 1) {
        /* Free memory before shutting down devices. */
        if ((ret = warp_shrink_memory()) || warp_separate_pass == 1 ||
            warp_canceled) {
            pm_device_down = WARP_STATE_RESUME;
            goto warp_shrink_memory_err;
        }
    }

#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,2,0)
    if ((ret = freeze_kernel_threads())) {
        warp_separate_pass = 0;
        pm_device_down = WARP_STATE_RESUME;
        goto freeze_kernel_threads_err;
    }

    if ((ret = dpm_prepare(PMSG_FREEZE))) {
        warp_separate_pass = 0;
        pm_device_down = WARP_STATE_RESUME;
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,4,0)
        thaw_kernel_threads();
#endif
        goto dpm_prepare_err;
    }
#endif

    if (warp_ops->progress)
        warp_ops->progress(WARP_PROGRESS_SUSPEND);

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,19)
    suspend_console();
#endif

#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,6,0)
    ftrace_stop();
#endif

#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,13)
    disable_nonboot_cpus();
#endif

    if (warp_ops->device_suspend_early &&
        (ret = warp_ops->device_suspend_early())) {
        pm_device_down = WARP_STATE_RESUME;
        goto warp_device_suspend_early_err;
    }

    if ((ret = pm_device_suspend(STATE_FREEZE))) {
        pm_device_down = WARP_STATE_RESUME;
        goto pm_device_suspend_err;
    }

    if (warp_ops->device_suspend && (ret = warp_ops->device_suspend())) {
        pm_device_down = WARP_STATE_RESUME;
        goto warp_device_suspend_err;
    }

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,21) && \
    LINUX_VERSION_CODE <  KERNEL_VERSION(2,6,30)
    if ((ret = disable_nonboot_cpus())) {
        pm_device_down = WARP_STATE_RESUME;
        goto disable_nonboot_cpus_err;
    }
#endif

#if LINUX_VERSION_CODE < KERNEL_VERSION(3,0,0)
    if ((ret = arch_prepare_suspend())) {
        pm_device_down = WARP_STATE_RESUME;
        goto arch_prepare_suspend_err;
    }
#endif

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,30)
    if ((ret = pm_device_power_down(STATE_FREEZE))) {
        pm_device_down = WARP_STATE_RESUME;
        goto pm_device_power_down_err;
    }
#endif

    if (warp_ops->pre_snapshot && (ret = warp_ops->pre_snapshot())) {
        pm_device_down = WARP_STATE_RESUME;
        goto warp_pre_snapshot_err;
    }

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,30)
    if ((ret = disable_nonboot_cpus())) {
        pm_device_down = WARP_STATE_RESUME;
        goto disable_nonboot_cpus_err;
    }
#endif

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,27) && \
    LINUX_VERSION_CODE <  KERNEL_VERSION(2,6,30)
    device_pm_lock();
#endif

    local_irq_disable();

#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,30)
    if ((ret = pm_device_power_down(STATE_FREEZE))) {
        pm_device_down = WARP_STATE_RESUME;
        goto pm_device_power_down_err;
    }
#endif

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,29) && \
    LINUX_VERSION_CODE <  KERNEL_VERSION(3,0,0)
    if ((ret = sysdev_suspend(STATE_FREEZE))) {
        pm_device_down = WARP_STATE_RESUME;
        goto sysdev_suspend_err;
    }
#endif

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,39)
    if ((ret = syscore_suspend())) {
        pm_device_down = WARP_STATE_RESUME;
        goto syscore_suspend_err;
    }
#endif

    save_processor_state();

    warp_param.bootflag_dev  = warp_savearea[warp_saveno].bootflag_dev;
    warp_param.bootflag_area = warp_savearea[warp_saveno].bootflag_area;
    warp_param.bootflag_size = warp_savearea[warp_saveno].bootflag_size;
    warp_param.snapshot_dev  = warp_savearea[warp_saveno].snapshot_dev;
    warp_param.snapshot_area = warp_savearea[warp_saveno].snapshot_area;
    warp_param.snapshot_size = warp_savearea[warp_saveno].snapshot_size;

#ifdef WARP_HIBDRV_AMP
    {
        int cpu;
        warp_param.subcpu_info = __pa(warp_mp_info);
        warp_param.subcpu_num = WARP_SUBCPU_NUM;
        warp_param.cpu_no = 0;
        for (cpu = 0; cpu < WARP_SUBCPU_NUM; cpu++) {
            int no = warp_saveno + cpu + 1;
            warp_mp_info[cpu].bootflag_dev  = warp_savearea[no].bootflag_dev;
            warp_mp_info[cpu].bootflag_area = warp_savearea[no].bootflag_area;
            warp_mp_info[cpu].bootflag_size = warp_savearea[no].bootflag_size;
            warp_mp_info[cpu].snapshot_dev  = warp_savearea[no].snapshot_dev;
            warp_mp_info[cpu].snapshot_area = warp_savearea[no].snapshot_area;
            warp_mp_info[cpu].snapshot_size = warp_savearea[no].snapshot_size;
        }
    }
#endif

#ifdef CONFIG_SWAP
    if (!warp_param.oneshot && total_swap_pages > NR_SWAP_PAGES)
        warp_swapout_disable = 1;
    else
        warp_swapout_disable = 0;
#endif
    if (WARP_ID(WARP_HIBDRV_VIRT) != WARP_ID_DRIVER) {
        printk("Can't find hibernation driver.\n");
        ret = -EIO;
    } else {
        if ((ret = warp_ops->snapshot()) == 0) {
            warp_stat = warp_param.stat;
            warp_retry = warp_param.retry;
        }
    }

    pm_device_down = WARP_STATE_RESUME;
    restore_processor_state();

    if (warp_ops->progress)
        warp_ops->progress(WARP_PROGRESS_RESUME);

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,39)
    syscore_resume();
syscore_suspend_err:
#endif

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,29) && \
    LINUX_VERSION_CODE <  KERNEL_VERSION(3,0,0)
    sysdev_resume();
sysdev_suspend_err:
#endif

#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,30)
    pm_device_power_up(STATE_RESTORE);
pm_device_power_down_err:
#endif

    local_irq_enable();

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,27) && \
    LINUX_VERSION_CODE <  KERNEL_VERSION(2,6,30)
    device_pm_unlock();
#endif

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,30)
disable_nonboot_cpus_err:
    enable_nonboot_cpus();
#endif

    if (warp_ops->post_snapshot)
        warp_ops->post_snapshot();
warp_pre_snapshot_err:

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,30)
    pm_device_power_up(STATE_RESTORE);
pm_device_power_down_err:
#endif

#if LINUX_VERSION_CODE < KERNEL_VERSION(3,0,0)
arch_prepare_suspend_err:
#endif

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,21) && \
    LINUX_VERSION_CODE <  KERNEL_VERSION(2,6,30)
disable_nonboot_cpus_err:
    enable_nonboot_cpus();
#endif

    if (warp_ops->device_resume)
        warp_ops->device_resume();
warp_device_suspend_err:

#ifdef WARP_SUSPEND_ERR_RECOVER
pm_device_suspend_err:
#endif
    pm_device_resume(STATE_RESTORE);
#ifndef WARP_SUSPEND_ERR_RECOVER
pm_device_suspend_err:
#endif

    if (warp_ops->device_resume_late)
        warp_ops->device_resume_late();
warp_device_suspend_early_err:

#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,13)
    enable_nonboot_cpus();
#endif

#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,6,0)
    ftrace_start();
#endif

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,19)
    resume_console();
#endif

#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,2,0)
dpm_prepare_err:
    dpm_complete(STATE_RESTORE);

freeze_kernel_threads_err:
#endif

warp_shrink_memory_err:

#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,0,0) && \
    LINUX_VERSION_CODE <  KERNEL_VERSION(3,2,0)
dpm_prepare_err:
    dpm_complete(STATE_RESTORE);

#endif

#ifdef WARP_UMOUNT_RW

#if LINUX_VERSION_CODE < KERNEL_VERSION(3,2,0)
umount_rw_err:
#endif
    thaw_kernel_threads();
    fs = get_fs();
    for (no = 0; no < WARP_UMOUNT_RW_NUM; no++) {
        if (mnt_devname[no] && warp_umount_rw_remount[no]) {
            set_fs(KERNEL_DS);
            if ((ret2 = sys_mount(mnt_devname[no], (void *)warp_umount_rw[no],
                                  mnt_type[no], s_flags[no],
                                  mnt_options[no])) < 0) {
                printk("mount %s error %d\n", warp_umount_rw[no], ret);
                if (ret >= 0)
                    ret = ret2;
            }
            set_fs(fs);
        }
    }
#endif

freeze_processes_err:
    if (warp_ops->progress)
        warp_ops->progress(WARP_PROGRESS_THAW);
    thaw_processes();

#ifdef WARP_UMOUNT_RW
user_path_at_err:
    for (no = 0; no < WARP_UMOUNT_RW_NUM; no++) {
        if (mnt_devname[no])
            kfree(mnt_devname[no]);
        if (mnt_type[no])
            kfree(mnt_type[no]);
        if (mnt_options[no])
            kfree(mnt_options[no]);
    }
#endif
    
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,13) && \
    LINUX_VERSION_CODE <  KERNEL_VERSION(2,6,21)
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,19)
disable_nonboot_cpus_err:
#endif
    enable_nonboot_cpus();
#endif

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,22)
    free_basic_memory_bitmaps();
create_basic_memory_bitmaps_err:
#endif

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,28) && \
    LINUX_VERSION_CODE <  KERNEL_VERSION(3,4,0)
    usermodehelper_enable();
usermodehelper_disable_err:
#endif

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,23)
    pm_notifier_call_chain(PM_POST_HIBERNATION);
pm_notifier_call_chain_err:
#endif

#ifdef WARP_PREPARE_CONSOLE
    pm_restore_console();
#endif

#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,3,0)
    unlock_system_sleep();
#elif LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,22)
    mutex_unlock(&pm_mutex);
#endif

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,16) && \
    LINUX_VERSION_CODE <  KERNEL_VERSION(2,6,24)
    system_state = SYSTEM_RUNNING;
#endif

    if (warp_ops->drv_uninit)
        warp_ops->drv_uninit();

#ifdef WARP_AM_INOTIFY
    printk("inotify_warp_restart Start.\n");
    inotify_warp_restart();
    printk("inotify_warp_restart Exit.\n");
#endif

warp_drv_init_err:

    warp_work_free();
warp_work_init_err:

    if (warp_separate_pass == 2)
        warp_separate_pass = 0;

    pm_device_down = WARP_STATE_NORMAL;

    if (warp_ops->progress)
        warp_ops->progress(WARP_PROGRESS_EXIT);

    if (warp_canceled)
        ret = -ECANCELED;
    warp_error = ret;
    if (ret < 0) {
#ifdef WARP_AMP
        if (warp_amp()) {
            if (warp_amp_maincpu())
                warp_amp_send_maincpu_saveend(ret);
            else
                warp_amp_send_subcpu_saveend(ret);
        }
#endif
        printk(KERN_ERR "Warp!! error %d\n", ret);
    }
	warp_fastboot=0;
    return ret;
}

#ifdef CONFIG_PROC_FS

static struct proc_dir_entry *proc_warp;
static struct proc_dir_entry *proc_warp_stat;
static struct proc_dir_entry *proc_warp_error;
static struct proc_dir_entry *proc_warp_retry;
static struct proc_dir_entry *proc_warp_canceled;
static struct proc_dir_entry *proc_warp_saveno;
static struct proc_dir_entry *proc_warp_loadno;
static struct proc_dir_entry *proc_warp_switch;
static struct proc_dir_entry *proc_warp_compress;
static struct proc_dir_entry *proc_warp_shrink;
static struct proc_dir_entry *proc_warp_separate;
static struct proc_dir_entry *proc_warp_oneshot;
static struct proc_dir_entry *proc_warp_halt;
static struct proc_dir_entry *proc_warp_silent;

static int read_proc_warp(char __user *buffer, size_t count,
                          loff_t *offset, int value)
{
    int len, pos;
    char buf[16];

    pos = *offset;
    if (pos < 0 || (ssize_t)count < 0)
        return -EIO;

    len = sprintf(buf, "%d\n", value) - pos;
    if (len <= 0)
        return 0;

    if (copy_to_user(buffer, buf + pos, len))
        return -EFAULT;

    *offset = pos + len;
    return len;
}

static int write_proc_warp(const char __user *buffer, size_t count,
                           loff_t *offset, int max, const char *str)
{
    int val;
    char buf[16];

    if (count == 0 || count >= 16)
        return -EINVAL;
    if (*offset != 0)
        return -EIO;

    if (copy_from_user(buf, buffer, count))
        return -EFAULT;
    buf[count] = '\0';

    sscanf(buf, "%d", &val);

    if (val > max) {
        printk("warp: %s too large !!\n", str);
        return -EINVAL;
    }
    return val;
}

#define PROC_READ(name, var)                                            \
static int read_proc_warp_##name(struct file *file, char __user *buffer,\
                                 size_t count, loff_t *offset)          \
{                                                                       \
    return read_proc_warp(buffer, count, offset, var);                  \
}                                                                       \
                                                                        \
static const struct file_operations proc_warp_##name##_fops =           \
{                                                                       \
    .read  = read_proc_warp_##name,                                     \
};

#define PROC_RW(name, var, str, max, func)                              \
static int read_proc_warp_##name(struct file *file, char __user *buffer,\
                                 size_t count, loff_t *offset)          \
{                                                                       \
    return read_proc_warp(buffer, count, offset, var);                  \
}                                                                       \
                                                                        \
static int write_proc_warp_##name(struct file *file,                    \
                                  const char __user *buffer,            \
                                  size_t count, loff_t *offset)         \
{                                                                       \
    int val;                                                            \
                                                                        \
    if ((val = write_proc_warp(buffer, count, offset, max, str)) < 0)   \
        return val;                                                     \
    var = val;                                                          \
    func();                                                             \
    return count;                                                       \
}                                                                       \
                                                                        \
static const struct file_operations proc_warp_##name##_fops =           \
{                                                                       \
    .read  = read_proc_warp_##name,                                     \
    .write = write_proc_warp_##name,                                    \
};

static inline void dummy(void)
{
}

static inline void separate_pass_init(void)
{
    warp_separate_pass = 0;
}

PROC_READ(stat, warp_stat)
PROC_READ(error, warp_error)
PROC_READ(retry, warp_retry)
PROC_RW(canceled, warp_canceled, "canceled", 1, dummy)
PROC_RW(saveno, warp_saveno, "saveno",
        WARP_SAVEAREA_NUM - 1 - WARP_SUBCPU_NUM, dummy)
PROC_RW(loadno, warp_loadno, "loadno",
        WARP_SAVEAREA_NUM - 1 - WARP_SUBCPU_NUM, dummy)
PROC_RW(switch, warp_param.switch_mode, "switch", 2, dummy)
PROC_RW(compress, warp_param.compress, "compress", 2, dummy)
PROC_RW(shrink, warp_shrink, "shrink", 3, dummy)
PROC_RW(separate, warp_separate, "separate", 2, separate_pass_init)
PROC_RW(oneshot, warp_param.oneshot, "oneshot", 1, dummy)
PROC_RW(halt, warp_param.halt, "halt", 1, dummy)
PROC_RW(silent, warp_param.silent, "silent", 3, dummy)

#endif

int warp_register_machine(struct warp_ops *ops)
{
    warp_ops = ops;
    return 0;
}

int warp_unregister_machine(struct warp_ops *ops)
{
    warp_ops = NULL;
    return 0;
}

static inline struct proc_dir_entry *warp_proc_create(
    const char *name, int rw, const struct file_operations *proc_fops)
{
    static struct proc_dir_entry *p;
    int mode = rw ? S_IRUGO | S_IWUSR : S_IRUGO;

#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,25)
    if ((p = create_proc_entry(name, mode, proc_warp)))
        p->proc_fops = proc_fops;
#else
    p = proc_create(name, mode, proc_warp, proc_fops);
#endif
    return p;
}

static int __init warp_init(void)
{
#ifdef CONFIG_PROC_FS
    if ((proc_warp = proc_mkdir("warp", NULL))) {
        proc_warp_stat = warp_proc_create("stat", 0, &proc_warp_stat_fops);
        proc_warp_error = warp_proc_create("error", 0, &proc_warp_error_fops);
        proc_warp_retry = warp_proc_create("retry", 0, &proc_warp_retry_fops);
        proc_warp_canceled =
            warp_proc_create("canceled", 1, &proc_warp_canceled_fops);
        proc_warp_saveno =
            warp_proc_create("saveno", 1, &proc_warp_saveno_fops);
        proc_warp_loadno =
            warp_proc_create("loadno", 1, &proc_warp_loadno_fops);
        proc_warp_switch =
            warp_proc_create("switch", 1, &proc_warp_switch_fops);
        proc_warp_compress =
            warp_proc_create("compress", 1, &proc_warp_compress_fops);
        proc_warp_shrink =
            warp_proc_create("shrink", 1, &proc_warp_shrink_fops);
        proc_warp_separate =
            warp_proc_create("separate", 1, &proc_warp_separate_fops);
        proc_warp_oneshot =
            warp_proc_create("oneshot", 1, &proc_warp_oneshot_fops);
        proc_warp_halt = warp_proc_create("halt", 1, &proc_warp_halt_fops);
        proc_warp_silent = warp_proc_create("silent", 1, &proc_warp_silent_fops);
    }
#endif

    warp_saveno = CONFIG_PM_WARP_SAVENO;
    warp_loadno = CONFIG_PM_WARP_LOADNO;
    warp_param.switch_mode = 0;
    warp_param.compress = CONFIG_PM_WARP_COMPRESS;
    warp_param.silent = CONFIG_PM_WARP_SILENT;
    warp_shrink = CONFIG_PM_WARP_SHRINK;
    warp_separate = CONFIG_PM_WARP_SEPARATE;
#ifdef CONFIG_PM_WARP_ONESHOT
    warp_param.oneshot = 1;
#else
    warp_param.oneshot = 0;
#endif
#ifdef CONFIG_PM_WARP_HALT
    warp_param.halt = 1;
#else
    warp_param.halt = 0;
#endif
    warp_param.ver_major = WARP_PARAM_VER_MAJOR;
    warp_param.ver_minor = WARP_PARAM_VER_MINOR;

#ifdef WARP_PRELOAD_EXTTBL
    warp_param.preload_exttbl = 1;
#else
    warp_param.preload_exttbl = 0;
#endif

    warp_param.v2p_offset = PAGE_OFFSET - __pa(PAGE_OFFSET);
#ifdef WARP_HIBDRV_FLOATING
    warp_param.text_v2p_offset = warp_param.v2p_offset;
#else
    warp_param.text_v2p_offset = WARP_HIBDRV_VIRT - WARP_HIBDRV_PHYS;
    warp_param.text_pfn = WARP_HIBDRV_PHYS >> PAGE_SHIFT;
    warp_param.text_size = WARP_HIBDRV_SIZE;
#endif
    warp_param.lowmem_size = (unsigned long)high_memory - PAGE_OFFSET;
    warp_param.page_shift = PAGE_SHIFT;

    warp_param.console = WARP_CONSOLE;
    warp_param.bps = WARP_BPS;

    warp_swapout_disable = 0;
    warp_separate_pass = 0;
    warp_canceled = 0;

#ifdef WARP_WORK_ALLOC_INIT
    if (warp_amp_maincpu()) {
        int ret;
        if ((ret = warp_work_alloc()) < 0)
            return ret;
    }
#endif
    printk(KERN_INFO "Lineo Warp!! module loaded\n");

    return 0;
}

static void __exit warp_exit(void)
{
#ifdef WARP_WORK_ALLOC_INIT
    if (warp_amp_maincpu())
        kfree(warp_work);
#endif

#ifdef CONFIG_PROC_FS
    if (proc_warp_stat) {
        remove_proc_entry("stat", proc_warp_stat);
        proc_warp_stat = NULL;
    }
    if (proc_warp_error) {
        remove_proc_entry("error", proc_warp_error);
        proc_warp_error = NULL;
    }
    if (proc_warp_retry) {
        remove_proc_entry("retry", proc_warp_retry);
        proc_warp_retry = NULL;
    }
    if (proc_warp_canceled) {
        remove_proc_entry("canceled", proc_warp_canceled);
        proc_warp_canceled = NULL;
    }
    if (proc_warp_saveno) {
        remove_proc_entry("saveno", proc_warp_saveno);
        proc_warp_saveno = NULL;
    }
    if (proc_warp_loadno) {
        remove_proc_entry("loadno", proc_warp_loadno);
        proc_warp_loadno = NULL;
    }
    if (proc_warp_switch) {
        remove_proc_entry("switch", proc_warp_switch);
        proc_warp_switch = NULL;
    }
    if (proc_warp_compress) {
        remove_proc_entry("compress", proc_warp_compress);
        proc_warp_compress = NULL;
    }
    if (proc_warp_shrink) {
        remove_proc_entry("shrink", proc_warp_shrink);
        proc_warp_shrink = NULL;
    }
    if (proc_warp_separate) {
        remove_proc_entry("separate", proc_warp_separate);
        proc_warp_separate = NULL;
    }
    if (proc_warp_oneshot) {
        remove_proc_entry("oneshot", proc_warp_oneshot);
        proc_warp_oneshot = NULL;
    }
    if (proc_warp_halt) {
        remove_proc_entry("halt", proc_warp_halt);
        proc_warp_halt = NULL;
    }
    if (proc_warp_silent) {
        remove_proc_entry("silent", proc_warp_silent);
        proc_warp_silent = NULL;
    }
    if (proc_warp) {
        remove_proc_entry("warp", proc_warp);
        proc_warp = NULL;
    }
#endif
}

module_init(warp_init);
module_exit(warp_exit);

MODULE_LICENSE("GPL");
