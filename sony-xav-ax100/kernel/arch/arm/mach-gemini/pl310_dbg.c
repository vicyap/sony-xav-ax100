#ifdef CONFIG_CACHE_L2X0

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/io.h>
#include <linux/proc_fs.h>
#include <linux/jiffies.h>
#include <linux/uaccess.h>
#include <mach/hardware.h>
#include <asm/hardware/cache-l2x0.h>

#define NUM_L2_EVENT    2
#define L2_DBG_BUF_SIZE (NUM_L2_EVENT*sizeof(u32))
static u32 *pl310_dbg_buf_ptr;

static int pl310_dbg_read(char *page, char **start, off_t off, int count, int *eof, void *data)
{
    void __iomem *base = (void __iomem *)L2C_BASE;
    u32 value[NUM_L2_EVENT], current_jiffies;
    char *buffer_ptr;
    int i;
    int  read_size = 0;
    int event[NUM_L2_EVENT];


    current_jiffies = jiffies;
    value[0] = readl_relaxed(base + L2X0_EVENT_CNT0_VAL);
    value[1] = readl_relaxed(base + L2X0_EVENT_CNT1_VAL);

    memcpy(event, data,  sizeof(event));
    buffer_ptr = page;

    read_size += sprintf(buffer_ptr+read_size, ": <jiffies>");

    for(i=0; i<NUM_L2_EVENT; i++)
    {
        if(event[i] == 0xffff)  // Invalid event
            break;

        read_size += sprintf(buffer_ptr+read_size, "        <0x%02x>", event[i]);
    }
    read_size += sprintf(buffer_ptr+read_size, "\n");

    read_size += sprintf(buffer_ptr+read_size, "%10u,", current_jiffies);

    for(i=0; i<NUM_L2_EVENT; i++)
    {
        if(event[i] == 0xffff)  // Invalid event
            break;

        read_size += sprintf(buffer_ptr+read_size, "%s", ((value[i]==0xffffffff) ? "OF_" : "   "));
        read_size += sprintf(buffer_ptr+read_size, "%10u,", value[i]);
    }

    read_size += sprintf(buffer_ptr+read_size, "\n");

    return read_size;
}

static int pl310_dbg_write(struct file *file, const char __user *buf, unsigned long count, void *data)
{

    void __iomem *base = (void __iomem *)L2C_BASE;
    char buffer[L2_DBG_BUF_SIZE];
    int i, event[NUM_L2_EVENT], num_event;
    u32 value;

    if( (count > sizeof(buffer)) || (copy_from_user(buffer, buf, count)) ) {
        printk("Error: %s:%d", __FILE__, __LINE__);
        return -EFAULT;
    }

    num_event = sscanf(buffer, "%x %x", &event[0], &event[1]);
    for(i=num_event; i<NUM_L2_EVENT; i++)
        event[i] = 0xffff;  // invalid event.

    memcpy(data, event, sizeof(event));


    value = 0x00000006;     // reset two event counters.
    writel_relaxed(value, base + L2X0_EVENT_CNT_CTRL);

    if(event[0] != 0xffff)
        value = (u32)(event[0] << 2);
    else
        value = 0;
    writel_relaxed(value, base + L2X0_EVENT_CNT0_CFG);

    if(event[1] != 0xffff)
        value = (u32)(event[1] << 2);
    else
        value = 0;
    writel_relaxed(value, base + L2X0_EVENT_CNT1_CFG);

    value = 0x00000001;     // enable counters.
    writel_relaxed(value, base + L2X0_EVENT_CNT_CTRL);

    return count;
}

static int __init pl310_dbg_init(void)
{
    struct proc_dir_entry *ent;

    pl310_dbg_buf_ptr = kzalloc(L2_DBG_BUF_SIZE, GFP_KERNEL);

    if(!pl310_dbg_buf_ptr)
        printk("Error: %s:%d", __func__, __LINE__);
    else {
        ent = create_proc_entry("pl310_dbg", 0666, NULL);
        if(!ent) {
            printk("Error: %s:%d", __func__, __LINE__);
            kfree(pl310_dbg_buf_ptr);
        } else {
            ent->write_proc = pl310_dbg_write;
            ent->read_proc = pl310_dbg_read;
            ent->data = pl310_dbg_buf_ptr;
        }
    }

    return 0;
}
early_initcall(pl310_dbg_init);

#endif /* CONFIG_CACHE_L2X0 */