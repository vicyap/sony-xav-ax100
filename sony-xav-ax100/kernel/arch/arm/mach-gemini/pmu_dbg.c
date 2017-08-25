#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/io.h>
#include <linux/proc_fs.h>
#include <linux/jiffies.h>
#include <linux/uaccess.h>

// PMU_RAW_MODE, /proc/pmu_dbg
#define PMU_DBG_PLATFORM__CNT64             (1 << 0)
#define PMU_DBG_PLATFORM__PANDABOARD        (1 << 1)
#define PMU_DBG_PLATFORM__GENERIC           (1 << 2)

#define PMU_DBG_PLATFORM_PANDABOARD_CNT32   (PMU_DBG_PLATFORM__PANDABOARD)
#define PMU_DBG_PLATFORM_PANDABOARD_CNT64   (PMU_DBG_PLATFORM__PANDABOARD | PMU_DBG_PLATFORM__CNT64)         
#define PMU_DBG_PLATFORM_GENERIC_CNT32      (PMU_DBG_PLATFORM__GENERIC)
#define PMU_DBG_PLATFORM_GENERIC_CNT64      (PMU_DBG_PLATFORM__GENERIC | PMU_DBG_PLATFORM__CNT64)
// Select one of above options:
#define PMU_DBG_PLATFORM_USED                PMU_DBG_PLATFORM_GENERIC_CNT32 

#define NUM_CPU          (NR_CPUS)
#define NUM_EVENT_CNT    6      // Cortex-A9 has 6 event counters on each CPU. Cortex-A8 has 4 event counters.
#define PMU_DBG_BUF_SIZE (NUM_EVENT_CNT*sizeof(u32))

static u32 cnt_high[NUM_CPU][NUM_EVENT_CNT];    // bit[63:32] of counters
static u32 cnt_cycle_high[NUM_CPU];
static int pmu_dbg_cnt_size;

struct pmu_cnt_info {
    u32 cpu_id;
    u32 jiffies;
    u32 overflow;
    u32 cycle_cnt;
    u32 event_number[NUM_EVENT_CNT];
    u32 hw_cnt[NUM_EVENT_CNT];
};

#if (PMU_DBG_PLATFORM_USED & PMU_DBG_PLATFORM__CNT64)
static irqreturn_t pmu_dbg_isr(int irq, void *dev); // prototype
#endif


// PMU_RAW_MODE, /proc/pmu_dbg
// Start of Platform-dependent --------------------------------------
#if (PMU_DBG_PLATFORM_USED & PMU_DBG_PLATFORM__PANDABOARD)
void enable_omap4460_clk(void)
{
    // TI OMAP4430 needs following clocks (http://e2e.ti.com/support/omap/f/849/p/206438/735409.aspx): 

    if( (__raw_readl(OMAP4430_CM_EMU_CLKSTCTRL) & 0x03) != 2 )
    {
        __raw_writel(2, OMAP4430_CM_EMU_CLKSTCTRL);
        printk("set OMAP4430_CM_EMU_CLKSTCTRL\n");
    }

    // __raw_writel(1, OMAP4430_CM_L3INSTR_L3_3_CLKCTRL);       // Enabled by setting: ENABLE_ON_INIT, kernel/arch/arm/mach-omap2/clock44xx_data.c
    // __raw_writel(1, OMAP4430_CM_L3INSTR_L3_INSTR_CLKCTRL);   // Enabled by setting: ENABLE_ON_INIT, kernel/arch/arm/mach-omap2/clock44xx_data.c

    if( (__raw_readl(OMAP4430_CM_PRM_PROFILING_CLKCTRL) & 0x01) != 1 )
    {
        __raw_writel(1, OMAP4430_CM_PRM_PROFILING_CLKCTRL);
        printk("set OMAP4430_CM_PRM_PROFILING_CLKCTRL\n");
    }

}
#endif

#if (PMU_DBG_PLATFORM_USED == PMU_DBG_PLATFORM_GENERIC_CNT32) 
int  pmu_dbg_platform_dependent(int enable)
{
    // Return value: 
    // 1: PMU interrupts have been enabled already or correctly set up.
    // 0: PMU interrupts are not available.

    return 0;
}
#elif (PMU_DBG_PLATFORM_USED == PMU_DBG_PLATFORM_GENERIC_CNT64) 
int  pmu_dbg_platform_dependent(int enable)
{
    // Return value: 
    // 1: PMU interrupts have been enabled already or correctly set up.
    // 0: PMU interrupts are not available.

    // TODO:
    //      Install interrupt(s).
    //      ...
    //      Refer to PandaBoard's code as reference.

    return 1;
}
#elif (PMU_DBG_PLATFORM_USED == PMU_DBG_PLATFORM_PANDABOARD_CNT32) 
int  pmu_dbg_platform_dependent(int enable)
{
    enable_omap4460_clk();

    return 0;
}
#elif (PMU_DBG_PLATFORM_USED == PMU_DBG_PLATFORM_PANDABOARD_CNT64) 
int  pmu_dbg_platform_dependent(int enable)
{
    static int flag_init = 0;
    const int irq_pmu0=86;      // PandaBoard/TI_OMAP4460 CPU0' PMU interrupt number: 86
    const int irq_pmu1=87;      // PandaBoard/TI_OMAP4460 CPU1' PMU interrupt number: 87

    if(enable) {
        enable_omap4460_clk();

        if(flag_init == 2)  // interrupts are enabled correctly.
            return 1;

        if(flag_init == 1)   // interrupts can't be enabled correcly
            return 0;

        flag_init = 1;

        if( irq_set_affinity(irq_pmu0, cpumask_of(0)) ) {
            printk("Error: %s: %d\n", __func__, __LINE__);
            return 0;
        }

        if( irq_set_affinity(irq_pmu1, cpumask_of(1)) ) {
            printk("Error: %s: %d\n", __func__, __LINE__);
            return 0;
        }

        if( request_irq(irq_pmu0, pmu_dbg_isr, IRQF_DISABLED | IRQF_NOBALANCING, "arm-pmu0", NULL) ) {
            printk("Error: %s: %d\n", __func__, __LINE__);
            return 0;
        }

        if( request_irq(irq_pmu1, pmu_dbg_isr, IRQF_DISABLED | IRQF_NOBALANCING, "arm-pmu1", NULL) ) {
            printk("Error: %s: %d\n", __func__, __LINE__);

            free_irq(irq_pmu0, NULL);
            return 0;
        }

        // Check:
        // cat /proc/interrupts
        // cat /proc/irq/86/smp_affinity
        // cat /proc/irq/87/smp_affinity

        flag_init = 2;  // interrupts are enabled correctly.
        return 1;
    } else if (flag_init == 2){
        free_irq(irq_pmu0, NULL);
        free_irq(irq_pmu1, NULL);
        flag_init = 0;

        return 1;
    } else
        return 1;
}
#endif

// End of Platform-dependent ----------------------------------------




// PMU_RAW_MODE, /proc/pmu_dbg

#if (PMU_DBG_PLATFORM_USED & PMU_DBG_PLATFORM__CNT64)
static irqreturn_t pmu_dbg_isr(int irq, void *dev)
{
    u32 value;
    int cpu_id, i;

    // clear all overflow flag
    asm volatile("mrc p15, 0, %0, c9, c12, 3" : "=r" (value));      // PMOVSR, overflow flag
    isb();
    asm volatile("mcr p15, 0, %0, c9, c12, 3" : : "r" (value));     // PMOVSR, overflow flag

    // printk("CPU%d: %s: Overflow: %08x\n", smp_processor_id(), __func__, value);

    cpu_id = (int)(smp_processor_id());

    if(value & 0x80000000)
        cnt_cycle_high[cpu_id]++;

    for(i=0; i<NUM_EVENT_CNT; i++) {
        if(value & 1)
            cnt_high[cpu_id][i]++;
        value >>= 1;
    }
    // value doesn't have overflow flag from now on

    return IRQ_HANDLED;
}
#endif

void pmu_dbg_raw_op_start(void *info)
{
    u32 tmp_mask_bit = 1;
    u32 mask_value = 0;
    u32 i, j, value;
    int *event;

    event = (void *)(info);

    // reset all counters, and set up event number for each counter
    value = 0x00000006; // bit_2: reset cycle count, bit_1: reset all counters except cycle count
    isb();
    asm volatile("mcr p15, 0, %0, c9, c12, 0" : : "r"(value));        // PMCR, control
    isb();

    // clear all overflow flag
    asm volatile("mrc p15, 0, %0, c9, c12, 3" : "=r" (value));      // PMOVSR, overflow flag
    isb();
    asm volatile("mcr p15, 0, %0, c9, c12, 3" : : "r" (value));     // PMOVSR, overflow flag

    value = 0;
    isb();
    asm volatile("mcr p15, 0, %0, c9, c13, 0" : : "r" (value));     // PMCCNTR, cycle count
    isb();

    for(i=0; i<NUM_EVENT_CNT; i++) {
        asm volatile("mcr p15, 0, %0, c9, c12, 5" : : "r" (i));   // PMSELR, event counter selection
        isb();

        asm volatile("mcr p15, 0, %0, c9, c13, 1" : : "r" ((u32)(event[i]))); // PMXEVTYPER, event number
        isb();

        asm volatile("mcr p15, 0, %0, c9, c13, 2" : : "r" (value)); // PMXEVCNTR, event counter
        isb();

        if( event[i] != 0xffff )    // valid event
            mask_value |= tmp_mask_bit;

        tmp_mask_bit <<= 1;
    }
    mask_value |= 0x80000000; // cycle count
    isb();
    asm volatile("mcr p15, 0, %0, c9, c12, 1" : : "r"(mask_value));    // PMCNTENSET, counter enable
    isb();
    asm volatile("mcr p15, 0, %0, c9, c14, 1" : : "r"(mask_value));    // PMINTENSET, interrupt enable
    isb();

    for(i=0; i< NUM_CPU; i++) {
        cnt_cycle_high[i] = 0;
        for(j=0; j< NUM_EVENT_CNT; j++)
            cnt_high[i][j] = 0;
    }

    value = 0x00000001; // bit_0: enable
    isb();
    asm volatile("mcr p15, 0, %0, c9, c12, 0" : : "r"(value));        // PMCR, control
    isb();
}

void pmu_dbg_raw_op_get_value(void *info)
{
    u32 value, i;
    struct pmu_cnt_info *pmu_cnt_info_ptr;

    pmu_cnt_info_ptr = (struct pmu_cnt_info *) info;

    value = 0x00000000; // bit_0: disable
    isb();
    asm volatile("mcr p15, 0, %0, c9, c12, 0" : : "r"(value));        // PMCR, control
    isb();

    pmu_cnt_info_ptr->cpu_id = smp_processor_id();
    pmu_cnt_info_ptr->jiffies = jiffies;

    // Read overflow flag
    isb();
    asm volatile("mrc p15, 0, %0, c9, c12, 3" : "=r" (value));      // PMOVSR, overflow flag
    isb();
    // Clear overflow flag
    // asm volatile("mcr p15, 0, %0, c9, c12, 3" : : "r" (value));     // PMOVSR, overflow flag
    pmu_cnt_info_ptr->overflow = value;

    isb();
    asm volatile("mrc p15, 0, %0, c9, c13, 0" : "=r" (value));   // PMCCNTR, cycle count
    isb();
    pmu_cnt_info_ptr->cycle_cnt = value;

    for(i=0; i<NUM_EVENT_CNT; i++) {
        isb();
        asm volatile("mcr p15, 0, %0, c9, c12, 5" : : "r" (i));   // PMSELR, event counter selection
        isb();

        asm volatile("mrc p15, 0, %0, c9, c13, 1" : "=r" (value)); // PMXEVTYPER, event number
        isb();
        pmu_cnt_info_ptr->event_number[i] = value;

        asm volatile("mrc p15, 0, %0, c9, c13, 2" : "=r" (value));  // PMXEVCNTR, event counter
        isb();
        pmu_cnt_info_ptr->hw_cnt[i] = value;

    }

    value = 0x00000001; // bit_0: enable
    isb();
    asm volatile("mcr p15, 0, %0, c9, c12, 0" : : "r"(value));        // PMCR, control
    isb();
}

static int pmu_dbg_read(char *page, char **start, off_t off, int count, int *eof, void *data)
{
    char *buffer_ptr;
    int i, j;
    int  read_size = 0;
    struct pmu_cnt_info pmu_cnt_data;
    u32 tmp_mask_bit;
    int event[NUM_EVENT_CNT];
    u64 value_64bit;

    memcpy(event, data,  sizeof(event));
    buffer_ptr = page;

    if(pmu_dbg_cnt_size == 32) {
        for(i=0; i<NUM_CPU; i++)
        {
            read_size += sprintf(buffer_ptr+read_size, ":<cpu>     <jiffies>       <cycle>");

            for(j=0; j<NUM_EVENT_CNT; j++)
            {
                if(event[j] == 0xffff)  // Invalid event
                    break;

                read_size += sprintf(buffer_ptr+read_size, "        <0x%02x>", event[j]);
            }
        }
        read_size += sprintf(buffer_ptr+read_size, "\n");

        for(i=0; i<NUM_CPU; i++)
        {
            smp_call_function_single(i, pmu_dbg_raw_op_get_value, (void *)(&pmu_cnt_data), true);

            read_size += sprintf(buffer_ptr+read_size, "%5u,   %10u,", pmu_cnt_data.cpu_id, pmu_cnt_data.jiffies);

            read_size += sprintf(buffer_ptr+read_size, "%s", ((pmu_cnt_data.overflow & 0x80000000) ? "OF_" : "   ") );
            read_size += sprintf(buffer_ptr+read_size, "%10u,", pmu_cnt_data.cycle_cnt);

            tmp_mask_bit = 0x00000001;
            for(j=0; j<NUM_EVENT_CNT; j++)
            {
                if(event[j] == 0xffff)  // Invalid event
                    break;

                read_size += sprintf(buffer_ptr+read_size, "%s", ((pmu_cnt_data.overflow & tmp_mask_bit) ? "OF_" : "   "));
                read_size += sprintf(buffer_ptr+read_size, "%10u,", pmu_cnt_data.hw_cnt[j]);
                tmp_mask_bit <<= 1;
            }
        }
        read_size += sprintf(buffer_ptr+read_size, "\n");
    }
    else { // pmu_dbg_cnt_size == 64
        for(i=0; i<NUM_CPU; i++)
        {
            read_size += sprintf(buffer_ptr+read_size, ":<cpu>     <jiffies>                 <cycle>");

            for(j=0; j<NUM_EVENT_CNT; j++)
            {
                if(event[j] == 0xffff)  // Invalid event
                    break;

                read_size += sprintf(buffer_ptr+read_size, "                  <0x%02x>", event[j]);
            }
        }
        read_size += sprintf(buffer_ptr+read_size, "\n");

        for(i=0; i<NUM_CPU; i++)
        {
            smp_call_function_single(i, pmu_dbg_raw_op_get_value, (void *)(&pmu_cnt_data), true);

            read_size += sprintf(buffer_ptr+read_size, "%5u,   %10u,", pmu_cnt_data.cpu_id, pmu_cnt_data.jiffies);

            read_size += sprintf(buffer_ptr+read_size, "%s", ((pmu_cnt_data.overflow & 0x80000000) ? "OF_" : "   ") );
            value_64bit = (u64)(pmu_cnt_data.cycle_cnt) + ((u64)(cnt_cycle_high[i]) << 32 );
            read_size += sprintf(buffer_ptr+read_size, "%20llu,", value_64bit);

            tmp_mask_bit = 0x00000001;
            for(j=0; j<NUM_EVENT_CNT; j++)
            {
                if(event[j] == 0xffff)  // Invalid event
                    break;

                read_size += sprintf(buffer_ptr+read_size, "%s", ((pmu_cnt_data.overflow & tmp_mask_bit) ? "OF_" : "   "));
                value_64bit = (u64)(pmu_cnt_data.hw_cnt[j]) + ((u64)(cnt_high[i][j]) << 32 );
                read_size += sprintf(buffer_ptr+read_size, "%20llu,", value_64bit);
                tmp_mask_bit <<= 1;
            }
        }
        read_size += sprintf(buffer_ptr+read_size, "\n");
    }

    return read_size;
}

static int pmu_dbg_write(struct file *file, const char __user *buf, unsigned long count, void *data)
{
    char buffer[PMU_DBG_BUF_SIZE];
    int i, event[NUM_EVENT_CNT], num_event;

    if( (count > sizeof(buffer)) || (copy_from_user(buffer, buf, count)) ) {
        printk("Error: %s:%d", __FILE__, __LINE__);
        return -EFAULT;
    }

    num_event = sscanf(buffer, "%x %x %x %x %x %x", &event[0], &event[1], &event[2], &event[3], &event[4], &event[5]);
    for(i=num_event; i<NUM_EVENT_CNT; i++)
        event[i] = 0xffff;  // invalid event.

    memcpy(data, event, sizeof(event));

    if( (num_event>0) && (event[0]==0xffff) ) {
        pmu_dbg_platform_dependent(0);      // remove interrupt vector(s)
        return count;
    }

    // If interrupts can be enabled correctly => 64 bits, otherwise, 32 bits.
    pmu_dbg_cnt_size =  pmu_dbg_platform_dependent(1) ? 64 : 32;

    for(i=0; i<NUM_CPU; i++)
        smp_call_function_single(i, pmu_dbg_raw_op_start, (void *)(event) , true);

    return count;
}

static int __init pmu_dbg_init(void)
{
    static u32 *pmu_dbg_buf_ptr;
    struct proc_dir_entry *ent;

    pmu_dbg_buf_ptr = kzalloc(PMU_DBG_BUF_SIZE, GFP_KERNEL);

    if(!pmu_dbg_buf_ptr)
        printk("Error: %s:%d", __func__, __LINE__);
    else {
        ent = create_proc_entry("pmu_dbg", 0666, NULL);
        if(!ent) {
            printk("Error: %s:%d", __func__, __LINE__);
            kfree(pmu_dbg_buf_ptr);
        } else {
            ent->write_proc = pmu_dbg_write;
            ent->read_proc = pmu_dbg_read;
            ent->data = pmu_dbg_buf_ptr;
        }
    }


    return 0;
}
early_initcall(pmu_dbg_init);


