#include <mach/kernel.h>
#include <mach/module.h>
#include <linux/memblock.h> // memblock related

/* assigned by early parameter
 * these variables are used in .reserve() of machine_desc */
unsigned int res_arm926_addr = -1;
unsigned int res_arm926_size = -1;
unsigned int res_iop_addr = -1;
unsigned int res_iop_size = -1;
unsigned int res_rtcfg_addr = -1;
unsigned int res_rtcfg_size = -1;
unsigned int res_display_addr = -1;
unsigned int res_display_size = -1;
unsigned int res_chunkmem_addr = -1;
unsigned int res_chunkmem_size = -1;

/* iop start address */
u32 iop_addr_p = -1;
volatile u32* iop_addr_v = NULL;

/* parameter handlers */
static int __init parse_res_arm926_addr(char *arg)
{
    res_arm926_addr = simple_strtoul(arg, NULL, 0);
    return 0;
}
early_param("res_arm926_addr", parse_res_arm926_addr);

static int __init parse_res_arm926_size(char *arg)
{
    res_arm926_size = simple_strtoul(arg, NULL, 0);
    return 0;
}
early_param("res_arm926_size", parse_res_arm926_size);

static int __init parse_res_iop_addr(char *arg)
{
    res_iop_addr = simple_strtoul(arg, NULL, 0);
    return 0;
}
early_param("res_iop_addr", parse_res_iop_addr);

static int __init parse_res_iop_size(char *arg)
{
    res_iop_size = simple_strtoul(arg, NULL, 0);
    return 0;
}
early_param("res_iop_size", parse_res_iop_size);

static int __init parse_res_rtcfg_addr(char *arg)
{
    res_rtcfg_addr = simple_strtoul(arg, NULL, 0);
    return 0;
}
early_param("res_rtcfg_addr", parse_res_rtcfg_addr);

static int __init parse_res_rtcfg_size(char *arg)
{
    res_rtcfg_size = simple_strtoul(arg, NULL, 0);
    return 0;
}
early_param("res_rtcfg_size", parse_res_rtcfg_size);

static int __init parse_res_display_addr(char *arg)
{
    res_display_addr = simple_strtoul(arg, NULL, 0);
    return 0;
}
early_param("res_display_addr", parse_res_display_addr);

static int __init parse_res_display_size(char *arg)
{
    res_display_size = simple_strtoul(arg, NULL, 0);
    return 0;
}
early_param("res_display_size", parse_res_display_size);

static int __init parse_res_chunkmem_addr(char *arg)
{
    res_chunkmem_addr = simple_strtoul(arg, NULL, 0);
    return 0;
}
early_param("res_chunkmem_addr", parse_res_chunkmem_addr);

static int __init parse_res_chunkmem_size(char *arg)
{
    res_chunkmem_size = simple_strtoul(arg, NULL, 0);
    return 0;
}
early_param("res_chunkmem_size", parse_res_chunkmem_size);

/* iop start address */
volatile u32* sp_get_iop_addr_v(void)
{
    return iop_addr_v;
}

u32 sp_get_iop_addr_p(void)
{
    return iop_addr_p;
}

/* reserve default memory map */
void __init default_memory_reservation(void)
{
    uint32_t size, pa;
    printk("reservation parameter : %08x, %08x, %08x, %08x, %08x, %08x\n", 
            res_arm926_addr, res_arm926_size, res_iop_addr, res_iop_size, res_display_addr, res_display_size);

    /* default memory reservation map (can be configurable):
     * 
     *  +--- Max address           = 0x1000 0000 ---
     *     < left M for linux memory >
     *  +--- chunk address end     = 0x0B60 0000 ---
     *     < 32 M for chunk memory >                 (res_chunkmem_size, old name gp_chunkmem.memsize1)
     *  +--- chunk start address   = 0x0960 0000 --- (res_chunkmem_addr, old name gp_chunkmem.membase1)
     *     < 6 M for display >                       (res_display_size)
     *  +--- display start address = 0x0900 0000 --- (res_display_addr)
     *     < 2 M for IOP >                           (res_iop_size)
     *  +--- IOP start address     = 0x08E0 0000 --- (res_iop_addr)
     *
     *  below 0x8E0 0000(res_arm926_size, 142MB), all are reserved, used by arm926 
     *
     *  +--- arm926 start address  = 0x0000 0000 --- (res_arm926_addr) */
    
    if(res_arm926_addr != -1 && res_arm926_size != -1) {
        pa = res_arm926_addr;
        size = res_arm926_size;
        if (memblock_reserve(pa, size) < 0)
            panic("unable to reserve %08x memory at %08x\n", size, pa);
        printk("reserved %08x memory at %08x\n", size, pa);
    }

    if(res_iop_addr != -1 && res_iop_size != -1) {
        pa = res_iop_addr;
        size = res_iop_size;
        if (memblock_remove(pa, size) < 0)
            panic("unable to reserve %08x memory at %08x\n", size, pa);
        printk("removed %08x memory at %08x\n", size, pa);
    }

    if(res_display_addr != -1 && res_display_size != -1) {
        pa = res_display_addr;
        size = res_display_size;
        if (memblock_remove(pa, size) < 0)
            panic("unable to reserve %08x memory at %08x\n", size, pa);
        printk("removed %08x memory at %08x\n", size, pa);
    }

		if(res_rtcfg_addr != -1 && res_rtcfg_size != -1) {
        pa = res_rtcfg_addr;
        size = res_rtcfg_size;
        if (memblock_remove(pa, size) < 0)
            panic("unable to reserve %08x memory at %08x\n", size, pa);
        printk("removed %08x memory at %08x\n", size, pa);
    }
    /*
    if(res_chunkmem_addr != -1 && res_chunkmem_size != -1) {
        // this memblock will be ioremap in chunkmem module
        pa = res_chunkmem_addr;
        size = res_chunkmem_size;
        if (memblock_remove(pa, size) < 0)
            panic("unable to reserve %08x memory at %08x\n", size, pa);
        printk("removed %08x memory at %08x\n", size, pa);
    }
    */
}
