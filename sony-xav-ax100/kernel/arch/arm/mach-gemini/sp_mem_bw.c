// ------------------------------------------------------------------------------------------------
// Info:
// http://wiki2.sunplus.com/wiki/index.php/letb8388/ver_A:Programming_Guide#Masters_ID_of_MBUS
// http://wiki2.sunplus.com/wiki/index.php/MBAR_BW_Setting_Q483
// http://wiki2.sunplus.com/wiki/index.php/IP-Spec:MBUS_Q483/ver_A
// ------------------------------------------------------------------------------------------------
// http://wiki2.sunplus.com/wiki/index.php/letb8388/ver_A:Programming_Guide#Masters_ID_of_MBUS:
//
// ID  Master          Prefix      Priority   Switch Type
//                                            fix0
//  1  CB2MB_BG        C2MBG       LL         fix0
//  2  CEVA            DSPD        RT         fix0
//  3  PBUS            PERI        RT         fix0
//  4  CB_DMA          CBDMA0      NOR        fix0
//  5  GC520           GC520       RT         fix0
//  6  USBC            USBC0       NOR        fix0
//  7  USBC            USBC1       NOR        fix0
//  8  USBH            USBH        NOR        fix0
//  9  SERVO           M2S_SDIF    RT         fix0
// 10  DDFCH0          DDFCH0      RT         fix0
// 11  DDFCH1          DDFCH1      RT         fix0
// 12  OSD0            OSD0        RT         fix0
// 13  OSD1            OSD1        RT         fix0
// 14  OSD2            OSD2        RT         fix0
// 15  AFRC            AFRC        RT         fix0
// 16  TNRIW0          TNRIW0      RT         fix0
// 17  TNRIW1          TNRIW1      RT         fix0
// 18  TPIIW0          V656IN0     RT         fix0
// 19  TPIIW1          V656IN1     RT         fix0
// 20  HOST                                   fix0
// 21  MFD             MR_MIX      RT         fix0
// 22  TVDEC           TVDEC       NOR        fix0
// 23  IMG_DEC         IMG_DEC     NOR        fix0
// 24  MFD             LF_MIX      RT         fix0
// 25  MFD             MR_CHR_MIX  RT         fix0
// 26  MFD             LF_CHR_MIX  RT         fix0
// 27  MFD             MPRG_MIX    LL         fix0
// 28  MFD             DMAG_MIX    RT         fix0
// 29  MFD             HS          RT         fix0
// 30  DUMMY           DUMMY       RT         fix0
// 31  MFD             HS_CHR      RT         fix0
// ------------------------------------------------------------------------------------------------

#include <linux/types.h>
#include <linux/kdev_t.h>
#include <linux/slab.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <asm/uaccess.h>
#include <linux/cdev.h>
#include <asm/io.h>
#include <linux/device.h>
#include <mach/hardware.h>
#include <mach/sp_clk.h>
#include <mach/sp_mem_bw.h>
#include <linux/ioctl.h>
#include <linux/delay.h>
#include <linux/platform_device.h>

// #define MEM_BW_FUNC_DEBUG
#define MEM_BW_KDBG_INFO
// #define MEM_BW_KDBG_ERR

#ifdef MEM_BW_FUNC_DEBUG
    #define FUNC_DEBUG() printk(KERN_INFO "%s: %d %s()\n", __FILE__, __LINE__, __func__)
#else
    #define FUNC_DEBUG()
#endif

#ifdef MEM_BW_KDBG_INFO
    #define DBG_INFO(fmt, args ...) printk(KERN_INFO "K_MEM_BW: " fmt, ## args)
#else
    #define DBG_INFO(fmt, args ...)
#endif

#ifdef MEM_BW_KDBG_ERR
    #define DBG_ERR(fmt, args ...)  printk(KERN_ERR "K_MEM_BW: " fmt, ## args)
#else
    #define DBG_ERR(fmt, args ...)
#endif

#define NUM_MEM_BW      (1)
#define DRAM_WIDTH      (16)    // 16 or 32

#define EFFECTIVE_BW_NUMERATOR      (85)
#define EFFECTIVE_BW_DENOMINATOR    (100)

#define DEVICE_NAME                 "sp_mem_bw"

#define LOGI_ADDR_MBUS_MONITOR      IO0_ADDRESS( 29*32*4 )
#define LOGI_ADDR_MBAR0             IO0_ADDRESS( 31*32*4 )
// #define LOGI_ADDR_MBAR1             IO0_ADDRESS( 44*32*4 )

#if defined(LOGI_ADDR_MBAR1)
    #define NUM_MBAR    2
    const u32 mbar_addr[] = { LOGI_ADDR_MBAR0, LOGI_ADDR_MBAR1 };
#else
    #define NUM_MBAR    1
    const u32 mbar_addr[] = {LOGI_ADDR_MBAR0};
#endif

struct register_group {
    u32 offset[32];
};

static int dev_major;
static int dev_minor;
static struct cdev *dev_cdevp = NULL;
static struct class *class_ptr = NULL;
static struct mutex mutex_mem_bw;
static u32 freq_dram = 0;
static u32 read_what = 0;

static const char *master_name[] = {
    "NONE      ",  //  0
    "C2MBG     ",  //  1
    "DSPD      ",  //  2
    "PERI      ",  //  3
    "CBDMA0    ",  //  4
    "GC520     ",  //  5
    "USBC0     ",  //  6
    "USBC1     ",  //  7
    "USBH      ",  //  8
    "M2S_SDIF  ",  //  9
    "DDFCH0    ",  // 10
    "DDFCH1    ",  // 11
    "OSD0      ",  // 12
    "OSD1      ",  // 13
    "OSD2      ",  // 14
    "AFRC      ",  // 15
    "TNRIW0    ",  // 16
    "TNRIW1    ",  // 17
    "V656IN0   ",  // 18
    "V656IN1   ",  // 19
    "HOST      ",  // 20
    "MR_MIX    ",  // 21
    "TVDEC     ",  // 22
    "IMG_DEC   ",  // 23
    "LF_MIX    ",  // 24
    "MR_CHR_MIX",  // 25
    "LF_CHR_MIX",  // 26
    "MPRG_MIX  ",  // 27
    "DMAG_MIX  ",  // 28
    "HS        ",  // 29
    "DUMMY     ",  // 30
    "HS_CHR    "   // 31
};

static u32 get_dram_freq(void)
{
    freq_dram = sp_clk_get(SP_CLK_DRAM);
    return freq_dram;
}

static u32 get_sys_freq(void)
{
    return sp_clk_get(SP_CLK_SYSFAST);
}

int set_mbar_n_channel_x(u32 mbar_n, u32 channel, u32 priority, u32 bandwidth_in_MB)
{
    struct register_group *reg_ptr;
    u32 arb_period, word_per_T;
    u32 freq_dram_div_1M, freq_sys_div_1M;
    u32 tmp_u32;

    if(mbar_n >= NUM_MBAR){
        printk("%s error mbar_n %d\n", __FUNCTION__, mbar_n);
        return -EINVAL;
    }
    if( (channel >= 32) || (channel == 0) ){
        printk("%s error channel %d\n", __FUNCTION__, channel);
        return -EINVAL;
    }
    bandwidth_in_MB *= EFFECTIVE_BW_DENOMINATOR;
    bandwidth_in_MB /= EFFECTIVE_BW_NUMERATOR;

    reg_ptr = (struct register_group *)mbar_addr[mbar_n];

    reg_ptr->offset[0] = 3;
    wmb();
    arb_period = reg_ptr->offset[2];

    freq_dram_div_1M = freq_dram / 1000000;
    freq_sys_div_1M = get_sys_freq() / 1000000;

    #if(DRAM_WIDTH == 16)
        // bw = bandwidth_in_MB*1000000*100/(freq_dram * 2 byte * 2 (DDR) );
        word_per_T = 1;
    #elif (DRAM_WIDTH == 32)
        // bw = bandwidth_in_MB*1000000*100/(freq_dram * 4 byte * 2 (DDR) );
        word_per_T = 2;
    #else
        #error "Invalid DRAM_WIDTH"
    #endif

    // DBG_INFO("arb_period: %d, freq_dram_div_1M=%d, freq_sys_div_1M=%d\n", arb_period, freq_dram_div_1M, freq_sys_div_1M);

    reg_ptr->offset[0] = 1;
    wmb();

    #if(DRAM_WIDTH == 16)
        tmp_u32 = bandwidth_in_MB * 8 * word_per_T / freq_sys_div_1M;
    #elif (DRAM_WIDTH == 32)
        tmp_u32 = bandwidth_in_MB * 4 * word_per_T / freq_sys_div_1M;
    #else
        #error "Invalid DRAM_WIDTH"
    #endif

    tmp_u32++;  // unconditionally increased by one.

    if(tmp_u32 >= 0x0000003f)
        tmp_u32 = 0x0000003f;

    // DBG_INFO("Page 1: %08x, ", tmp_u32);
    reg_ptr->offset[channel] = (reg_ptr->offset[channel] & (~0x0000003f)) | tmp_u32;

    reg_ptr->offset[0] = 2;
    wmb();

    #if(DRAM_WIDTH == 16)
        tmp_u32 = bandwidth_in_MB * arb_period * word_per_T / (freq_sys_div_1M << 2);
    #elif (DRAM_WIDTH == 32)
        tmp_u32 = bandwidth_in_MB * arb_period * word_per_T / (freq_sys_div_1M << 3);
    #else
        #error "Invalid DRAM_WIDTH"
    #endif

    tmp_u32++;  // unconditionally increased by one.

    if(tmp_u32 >= 0x00000fff)
        tmp_u32 = 0x00000fff;

    // DBG_INFO("Page 2: %08x\n", tmp_u32);
    reg_ptr->offset[channel] = tmp_u32;

    reg_ptr->offset[0] = 0;
    wmb();
    tmp_u32 = reg_ptr->offset[channel];
    tmp_u32 &= ~PRIORITY_FORCED;
    tmp_u32 &= ~PRIORITY_ABOVE_NORMAL;

    switch(priority) {
        case PRIORITY_LL:               tmp_u32 |= PRIORITY_LL;             break;
        case PRIORITY_RT:               tmp_u32 |= PRIORITY_RT;             break;
        case PRIORITY_NORMAL:           tmp_u32 |= PRIORITY_NORMAL;         break;
        case PRIORITY_ABOVE_NORMAL:     tmp_u32 |= PRIORITY_ABOVE_NORMAL;   break;
        default:    /* PRIORITY_DEFAULT: */
            break;
    }

    reg_ptr->offset[channel] = tmp_u32;

    return 0;
}
EXPORT_SYMBOL(set_mbar_n_channel_x);

static int mem_bw_setup_init(void)
{
    // TODO:
    return 0;
}

int mem_bw_setup(u32 type){

    if(!class_ptr){
        printk("%s can not set bw, because of mem bw drv not init\n", __FUNCTION__);
        return -EPERM;
    }

    switch(type){
        default:
            mem_bw_setup_init();
            break;
    }
    return 0;
}
EXPORT_SYMBOL(mem_bw_setup);

struct platform_device *sunpluse_mem_bw_platform_device;

static int sunplus_mem_bw_platform_driver_probe(struct platform_device *pdev)
{
    // platform_set_drvdata(pdev, );

    mem_bw_setup(0);
    return 0;
}
static int sunplus_mem_bw_platform_driver_resume(struct platform_device *pdev)
{
    // platform_get_drvdata(pdev);

    mem_bw_setup(0);
    return 0;
}

static struct platform_driver sunplus_mem_bw_platform_driver = {
    .probe          = sunplus_mem_bw_platform_driver_probe,
 // .remove         = sunplus_mem_bw_platform_driver_remove,
 // .shutdown       = sunplus_mem_bw_platform_driver_shutdown,
 // .suspend        = sunplus_mem_bw_platform_driver_suspend,
    .resume         = sunplus_mem_bw_platform_driver_resume,
    .driver = {
        .name       = DEVICE_NAME,
        .owner      = THIS_MODULE
    }
};

static long mem_bw_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
    long err = 0;
    u32 bit;

    FUNC_DEBUG();

    if ( unlikely(copy_from_user(&bit, (void *)arg, sizeof(bit))))
        return -EFAULT;

    switch (cmd) {
        default:                err = -EINVAL;                      break;
    }

    return err;
}

static int mem_bw_open(struct inode *inode, struct file *filp)
{
    FUNC_DEBUG();

    get_dram_freq();

    #if 0 // test code
        set_mbar_n_channel_x(0, MEM_BW_C2MBG     , PRIORITY_LL,               1);
        set_mbar_n_channel_x(0, MEM_BW_DSPD      , PRIORITY_RT,               2);
        set_mbar_n_channel_x(0, MEM_BW_PERI      , PRIORITY_NORMAL,           4);
        set_mbar_n_channel_x(0, MEM_BW_CBDMA0    , PRIORITY_DEFAULT,          8);
        set_mbar_n_channel_x(0, MEM_BW_GC520     , PRIORITY_ABOVE_NORMAL,    16);
        set_mbar_n_channel_x(0, MEM_BW_USBC0     , PRIORITY_LL,              32);
        set_mbar_n_channel_x(0, MEM_BW_USBC1     , PRIORITY_RT,              64);
        set_mbar_n_channel_x(0, MEM_BW_USBH      , PRIORITY_NORMAL,          70);
        set_mbar_n_channel_x(0, MEM_BW_M2S_SDIF  , PRIORITY_DEFAULT,         80);
        set_mbar_n_channel_x(0, MEM_BW_DDFCH0    , PRIORITY_ABOVE_NORMAL,    90);
        set_mbar_n_channel_x(0, MEM_BW_DDFCH1    , PRIORITY_LL,             100);
        set_mbar_n_channel_x(0, MEM_BW_OSD0      , PRIORITY_RT,             200);
        set_mbar_n_channel_x(0, MEM_BW_OSD1      , PRIORITY_NORMAL,         300);
        set_mbar_n_channel_x(0, MEM_BW_OSD2      , PRIORITY_DEFAULT,        400);
        set_mbar_n_channel_x(0, MEM_BW_AFRC      , PRIORITY_ABOVE_NORMAL,   500);
        set_mbar_n_channel_x(0, MEM_BW_TNRIW0    , PRIORITY_LL,             600);
        set_mbar_n_channel_x(0, MEM_BW_TNRIW1    , PRIORITY_RT,             700);
        set_mbar_n_channel_x(0, MEM_BW_V656IN0   , PRIORITY_NORMAL,         800);
        set_mbar_n_channel_x(0, MEM_BW_V656IN1   , PRIORITY_DEFAULT,        900);
        set_mbar_n_channel_x(0, MEM_BW_HOST      , PRIORITY_ABOVE_NORMAL,  1000);
        set_mbar_n_channel_x(0, MEM_BW_MR_MIX    , PRIORITY_LL,            1100);
        set_mbar_n_channel_x(0, MEM_BW_TVDEC     , PRIORITY_RT,            1200);
        set_mbar_n_channel_x(0, MEM_BW_IMG_DEC   , PRIORITY_NORMAL,        1300);
        set_mbar_n_channel_x(0, MEM_BW_LF_MIX    , PRIORITY_DEFAULT,       1400);
        set_mbar_n_channel_x(0, MEM_BW_MR_CHR_MIX, PRIORITY_ABOVE_NORMAL,  1500);
        set_mbar_n_channel_x(0, MEM_BW_LF_CHR_MIX, PRIORITY_LL,            1600);
        set_mbar_n_channel_x(0, MEM_BW_MPRG_MIX  , PRIORITY_RT,            1700);
        set_mbar_n_channel_x(0, MEM_BW_DMAG_MIX  , PRIORITY_NORMAL,        1800);
        set_mbar_n_channel_x(0, MEM_BW_HS        , PRIORITY_DEFAULT,       1900);
        set_mbar_n_channel_x(0, MEM_BW_DUMMY     , PRIORITY_ABOVE_NORMAL,  2000);
        set_mbar_n_channel_x(0, MEM_BW_HS_CHR    , PRIORITY_ABOVE_NORMAL,  2100);
    #endif

    return 0;
}

static int mem_bw_release(struct inode *inode, struct file *filp)
{
    FUNC_DEBUG();
    return 0;
}

static ssize_t read_current_setting(struct file *filp, char __user *buffer, size_t length, loff_t *offset)
{
    ssize_t data_size = 0;
    void *mem_ptr;
    static u32 flag_read_end = 0;
    static u32 flag_header = 0;
    static u32 mbar_n=0;
    static u32 mbar_offset=1;
    u32 k;
    struct register_group *reg_ptr;
    u32 page_value[3], tmp_u32, total_bandwidth, available_bandwidth;
    u32 arb_period, word_per_T;
    u32 freq_dram_div_1M, freq_sys_div_1M;


    FUNC_DEBUG();

    if(flag_read_end) {
        flag_read_end = 0;
        return 0;
    }

    freq_dram_div_1M = freq_dram / 1000000;
    freq_sys_div_1M = get_sys_freq() / 1000000;

    #if(DRAM_WIDTH == 16)
        word_per_T = 1;
        available_bandwidth = freq_dram_div_1M * 2 * 2 * EFFECTIVE_BW_NUMERATOR / EFFECTIVE_BW_DENOMINATOR; // 2 bytes, DDR
    #elif (DRAM_WIDTH == 32)
        word_per_T = 2;
        available_bandwidth = freq_dram_div_1M * 4 * 2 * EFFECTIVE_BW_NUMERATOR / EFFECTIVE_BW_DENOMINATOR; // 4 bytes, DDR
    #else
        #error "Invalid DRAM_WIDTH"
    #endif

    mem_ptr = kmalloc(length, GFP_KERNEL);
    if(mem_ptr) {
        if(flag_header == 0) {
            data_size += sprintf(mem_ptr+data_size, "DRAM freq: %d\n", freq_dram);
            flag_header = 1;
        }

        while(1) {
            mbar_n %= NUM_MBAR; /* Avoid  Coverity warning */
            reg_ptr = (struct register_group *)mbar_addr[mbar_n];
            reg_ptr->offset[0] = 3;
            wmb();
            arb_period = reg_ptr->offset[2];

            total_bandwidth = 0;
            while(1) {
                data_size += sprintf(mem_ptr+data_size, "MBAR%d, ", mbar_n);
                // data_size += sprintf(mem_ptr+data_size, "Base address: 0x%08x, ", (u32)reg_ptr);
                data_size += sprintf(mem_ptr+data_size, "%2d: ", mbar_offset);

                for(k=0; k<3; k++) {
                    reg_ptr->offset[0] = k;
                    wmb();
                    page_value[k] = reg_ptr->offset[mbar_offset];
                    data_size += sprintf(mem_ptr+data_size, "0x%08x ", page_value[k]);
                }

                #if(DRAM_WIDTH == 16)
                    tmp_u32 = freq_sys_div_1M * (freq_dram_div_1M << 2) * page_value[2];
                    tmp_u32 /= arb_period * word_per_T * freq_dram_div_1M;
                #elif (DRAM_WIDTH == 32)
                    tmp_u32 = freq_sys_div_1M * (freq_dram_div_1M << 3) * page_value[2];
                    tmp_u32 /= arb_period * word_per_T * freq_dram_div_1M;
                #else
                    #error "Invalid DRAM_WIDTH"
                #endif

                tmp_u32 *= EFFECTIVE_BW_NUMERATOR;
                tmp_u32 /= EFFECTIVE_BW_DENOMINATOR;
                total_bandwidth += tmp_u32;

                data_size += sprintf(mem_ptr+data_size, " => %s %4d MB/sec, ", master_name[mbar_offset], tmp_u32);

                if(page_value[0] & PRIORITY_ABOVE_NORMAL)
                    data_size += sprintf(mem_ptr+data_size, "AboveNormal");
                else if(!(page_value[0] & PRIORITY_FORCED))
                    data_size += sprintf(mem_ptr+data_size, "Default    ");
                else {
                    page_value[0] &= 0x7;
                    switch(page_value[0]) {
                        case PRIORITY_LL:       data_size += sprintf(mem_ptr+data_size, "LowLatency "); break;
                        case PRIORITY_RT:       data_size += sprintf(mem_ptr+data_size, "RealTime   "); break;
                        case PRIORITY_NORMAL:   data_size += sprintf(mem_ptr+data_size, "Normal     "); break;
                        default: break;
                    }
                }

                data_size += sprintf(mem_ptr+data_size, "\n");

                if(mbar_offset==31) {
                    data_size += sprintf(mem_ptr+data_size, "Total: %d MB/sec, Available: %d MB/sec\n", total_bandwidth, available_bandwidth);

                    if(total_bandwidth > available_bandwidth)
                        data_size += sprintf(mem_ptr+data_size, "Warning: Total BW is too high!!\n");

                    total_bandwidth = 0;

                    mbar_offset = 1;

                    if(mbar_n == (NUM_MBAR-1)) {
                        mbar_n = 0;
                        flag_read_end = 1;
                        flag_header = 0;
                        goto return_read_data;
                    }
                    else {
                        mbar_n++;
                        break;
                    }

                }
                else {
                    mbar_offset++;
                }

                if(data_size > (length - 300))  /* Assume that each line has less than 300 characters */
                    goto return_read_data;
            }
        }

return_read_data:
        copy_to_user(buffer, mem_ptr, data_size);
        *offset += data_size;
        kfree(mem_ptr);
    }

    return data_size;
}

static ssize_t bandwidth_and_urgent_monitor(struct file *filp, char __user *buffer, size_t length, loff_t *offset)
{
    struct register_group *reg_ptr;
    ssize_t data_size = 0;
    void *mem_ptr;
    u32 master_a, master_b, master_c;
    u32 freq_dram_div_1M, freq_sys_div_1M;
    u32 tmp_u32;
    u32 mbar_n, word_per_T;
    static u32 flag_read_end = 0;

    FUNC_DEBUG();

    if(flag_read_end) {
        flag_read_end = 0;
        return 0;
    }

    freq_dram_div_1M = freq_dram / 1000000;
    freq_sys_div_1M = get_sys_freq() / 1000000;

    #if(DRAM_WIDTH == 16)
        word_per_T = 1;
    #elif (DRAM_WIDTH == 32)
        word_per_T = 2;
    #else
        #error "Invalid DRAM_WIDTH"
    #endif

    // http://wiki2.sunplus.com/wiki/index.php/IP-Spec:MBUS_Q483/ver_A#Bandwidth.2FUrgent_Monitor
    // change master here:
    master_a = 6;
    master_b = 7;
    master_c = 8;

    reg_ptr = (struct register_group *)LOGI_ADDR_MBUS_MONITOR;
    reg_ptr->offset[0] |= 1 << 4;

    for(mbar_n=0; mbar_n<NUM_MBAR; mbar_n++) {
        reg_ptr = (struct register_group *)mbar_addr[mbar_n];

        reg_ptr->offset[0] = 3;
        wmb();

        reg_ptr->offset[15] = ( master_a << 16 ) |  // MA_SEL, MB_SEL, MC_SEL
                              ( master_b <<  8 ) |
                              ( master_c <<  0 );

        reg_ptr->offset[30] = 0;                    // any value to clear monitor
    }

    for(mbar_n=0; mbar_n<NUM_MBAR; mbar_n++) {
        reg_ptr = (struct register_group *)mbar_addr[mbar_n];

        reg_ptr->offset[31] = 1;                    // LONG_TERM_MON_EN
    }

    msleep(1000);                                   // run for a while

    for(mbar_n=0; mbar_n<NUM_MBAR; mbar_n++) {
        reg_ptr = (struct register_group *)mbar_addr[mbar_n];

        reg_ptr->offset[31] = 0;                    // LONG_TERM_MON_EN
    }

    mem_ptr = kmalloc(length, GFP_KERNEL);
    if(!mem_ptr)
        return data_size;

    data_size += sprintf(mem_ptr+data_size, "%s\n", __func__);

    for(mbar_n=0; mbar_n<NUM_MBAR; mbar_n++) {
        reg_ptr = (struct register_group *)mbar_addr[mbar_n];

        tmp_u32 = reg_ptr->offset[30];      // TOTAL_LONG_ALL

        data_size += sprintf(mem_ptr+data_size, "MBAR%d: Percentage of each master: A: %d, B: %d, C: %d, All: %d\n", mbar_n,
            (100 * reg_ptr->offset[17] * freq_sys_div_1M  / (tmp_u32 * word_per_T * freq_dram_div_1M)),
            (100 * reg_ptr->offset[21] * freq_sys_div_1M  / (tmp_u32 * word_per_T * freq_dram_div_1M)),
            (100 * reg_ptr->offset[25] * freq_sys_div_1M  / (tmp_u32 * word_per_T * freq_dram_div_1M)),
            (100 * reg_ptr->offset[28] * freq_sys_div_1M  / (tmp_u32 * word_per_T * freq_dram_div_1M)) );

        data_size += sprintf(mem_ptr+data_size, "MBAR%d: Number of urgent request: A: %d, B: %d, C: %d, All: %d\n", mbar_n,
            reg_ptr->offset[18],        // MA_LONG_URG_CNT
            reg_ptr->offset[22],        // MB_LONG_URG_CNT
            reg_ptr->offset[26],        // MC_LONG_URG_CNT
            reg_ptr->offset[29] );      // TOTAL_LONG_URG_CNT
    }

    copy_to_user(buffer, mem_ptr, data_size);
    *offset += data_size;
    kfree(mem_ptr);
    flag_read_end = 1;

    return data_size;
}


static ssize_t latency_monitor(struct file *filp, char __user *buffer, size_t length, loff_t *offset)
{
    struct register_group *reg_ptr;
    ssize_t data_size = 0;
    void *mem_ptr;
    static u32 flag_read_end = 0;
    u32 LatencyEndPoint, LatencyStartPoint, MonitorCommand, MoniorSDRAMID, MonitorMasterID, MonitorMode;

    FUNC_DEBUG();

    if(flag_read_end) {
        flag_read_end = 0;
        return 0;
    }

    // http://wiki2.sunplus.com/wiki/index.php/IP-Spec:MBUS_Q483/ver_A#Latency_Monitor

    // change something here:
    LatencyStartPoint = 0;  // Refer to waveform in wiki page.
    LatencyEndPoint   = 3;
    MonitorCommand    = 0;  // Monitor all command: 0, Monitor write command: 1, Monitor read command: 2
    MoniorSDRAMID     = 0;  // Monitor on MBUS0: 0, Monitor on MBUS1: 1
    MonitorMasterID   = 1;  // master, 1 - 31
    MonitorMode       = 0;  // 0 : Maximum Latency monitor mode, 2: Average latency monitor mode, 3: Command monitor mode

    reg_ptr = (struct register_group *)LOGI_ADDR_MBUS_MONITOR;
    reg_ptr->offset[1] = (LatencyEndPoint << 24) | (LatencyStartPoint << 16) | (MonitorCommand << 8) | (MoniorSDRAMID << 5) | (MonitorMasterID <<0);
    reg_ptr->offset[0] = (reg_ptr->offset[0] & ~0x3) | MonitorMode;
    reg_ptr->offset[0] |= 1 << 4;   // MONITOR_GCLK_EN
    reg_ptr->offset[4] = 0;         // any value to clear result.
    reg_ptr->offset[2] = 1;         // MONITOR_EN

    msleep(1000);                   // run for a while

    reg_ptr->offset[2] = 0;         // MONITOR_EN

    mem_ptr = kmalloc(length, GFP_KERNEL);
    if(!mem_ptr)
        return data_size;

    data_size += sprintf(mem_ptr+data_size, "%s\n", __func__);

    if(MonitorMode == 0) {
        data_size += sprintf(mem_ptr+data_size, "Max. latency: %d, MAX_LATENCY_ID: %d\n",
            (reg_ptr->offset[4] & 0x00ffffff),
            (reg_ptr->offset[4] >> 24) );       // MAX_LATENCY_ID
    }else if(MonitorMode == 2) {
        data_size += sprintf(mem_ptr+data_size, "Avg. latency: %d\n", reg_ptr->offset[5]);
    }

    copy_to_user(buffer, mem_ptr, data_size);
    *offset += data_size;
    kfree(mem_ptr);

    flag_read_end = 1;

    return data_size;
}

static ssize_t mem_bw_read(struct file *filp, char __user *buffer, size_t length, loff_t *offset)
{
    FUNC_DEBUG();

    switch(read_what) {
        case 0:  return read_current_setting(filp, buffer, length, offset);
        case 1:  return bandwidth_and_urgent_monitor(filp, buffer, length, offset);
        case 2:  return latency_monitor(filp, buffer, length, offset);
        default: return 0;
    }
}

static ssize_t mem_bw_write(struct file *filp, const char __user *buff, size_t length, loff_t *offset)
{
    u8 write_data[4];
    size_t copy_length;

    copy_length = (sizeof(write_data) > length) ? length : sizeof(write_data);

    if(copy_from_user(write_data, buff, copy_length)) {
        DBG_ERR("Error: copy_from_user().\n");
        return -EFAULT;
    }

    if( (write_data[0] >= '0') && (write_data[0] <= '9') ) {
        read_what = (u32)( write_data[0] - '0' );
        // DBG_INFO("read_what = %d\n", read_what);
    }

    return length;
}

static struct file_operations fops = {
    .owner          = THIS_MODULE,
    .unlocked_ioctl = mem_bw_ioctl,
    .open           = mem_bw_open,
    .read           = mem_bw_read,
    .write          = mem_bw_write,
    .release        = mem_bw_release
};

static int __init mem_bw_drv_init(void)
{
    dev_t dev;
    int ret;

    FUNC_DEBUG();

    ret = alloc_chrdev_region(&dev, 0, NUM_MEM_BW, DEVICE_NAME);
    if (ret) {
        DBG_ERR("Error: alloc_chrdev_region()\n");
        return ret;
    }

    dev_major = MAJOR(dev);
    dev_minor = MINOR(dev);
    DBG_INFO("register chrdev(%d,%d)\n", dev_major, dev_minor);

    dev_cdevp = kzalloc(sizeof(struct cdev), GFP_KERNEL);
    if (dev_cdevp == NULL) {
        DBG_ERR("Error: kzalloc().\n");
        goto failed00;
    }

    cdev_init(dev_cdevp, &fops);
    dev_cdevp->owner = THIS_MODULE;
    ret = cdev_add(dev_cdevp, dev, NUM_MEM_BW);
    if (ret < 0) {
        DBG_ERR("add chr dev failed\n");
        goto failed00;
    }

    class_ptr = class_create(THIS_MODULE, DEVICE_NAME);
    device_create(class_ptr, NULL, dev, NULL, DEVICE_NAME);

    mutex_init(&mutex_mem_bw);

    get_dram_freq();

    sunpluse_mem_bw_platform_device = platform_device_register_simple(DEVICE_NAME, -1, NULL, 0);
    if( IS_ERR(sunpluse_mem_bw_platform_device) ) {
        ret = PTR_ERR(sunpluse_mem_bw_platform_device);
        goto failed00;
    }

    ret = platform_driver_register(&sunplus_mem_bw_platform_driver);
    if(ret != 0) {
        platform_device_unregister(sunpluse_mem_bw_platform_device);
        goto failed01;
    }

    return 0;

failed01:
    platform_device_unregister(sunpluse_mem_bw_platform_device);

failed00:
    if(dev_cdevp) {
        kfree(dev_cdevp);
        dev_cdevp = NULL;
    }

    unregister_chrdev_region(dev, NUM_MEM_BW);
    return -ENODEV;
}

static void __exit mem_bw_drv_exit(void)
{
    dev_t dev;

    FUNC_DEBUG();

    mutex_destroy(&mutex_mem_bw);

    dev = MKDEV(dev_major, dev_minor);
    if (dev_cdevp) {
        cdev_del(dev_cdevp);
        kfree(dev_cdevp);
        dev_cdevp = NULL;
    }

    unregister_chrdev_region(dev, NUM_MEM_BW);
}

module_init(mem_bw_drv_init);
module_exit(mem_bw_drv_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Sunplus Technology");
MODULE_DESCRIPTION("Memory Bandwidth Adjustment Driver");
