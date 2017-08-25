/******************************************************
 IOP

 IOP module, only support  ir

 Copyright (c) 2010 Sunplus, Inc.
 All rights reserved.
*******************************************************/
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/kernel.h>
#include <linux/version.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/interrupt.h>
#include <linux/irq.h>
#include <linux/sched.h>
#include <linux/pm.h>
#include <linux/sysctl.h>
#include <linux/proc_fs.h>
#include <linux/delay.h>
#include <linux/list.h>
#include <linux/interrupt.h>
#include <linux/io.h>

#include <iop_car/platform_define.h> //Kaney add
#include <iop_car/customize.h> //Kaney add
#include <iop_car/iop.h>
#include <iop_car/drv_iop.h> //Kaney add
#include <iop_car/regmap.h> //Kaney add
//#include <mach/gp_chunkmem.h> //Kaney add
#include <linux/miscdevice.h>
#include <asm/uaccess.h>
#include <iop_car/drv_vfd_panel.h>
#include <iop_car/drv_i2c.h>
#include <mach/sp_clk.h>
#include <iop_car/iop_if.h>


//IOP ISR flag define
#define ISR_PORT			(11)
#define ISR_IR				(1 << 8)
#define ISR_VFD				(1 << 10)

extern const unsigned char IOPcode_NEC [] ;

#if defined(SUPPORT_GPIO_MONITOR) && defined(CONFIG_SUNPLUS_GPIO_MONITOR_STB)
extern void DRV_GpMon_IopDsr(void);
#endif

#ifdef SUPPORT_IOP2_STANDBY_WAKEUPTIMER
extern wakeup_timer_t wakeup_timer;
#endif

#if defined(SUPPORT_VFD_DRIVER) && defined(CONFIG_SUNPLUS_VFD_STB)
extern UINT32 VFDProgNo;
extern UINT32 VFDMultimediaNo;
#endif

//#define DPRINTK(fmt, args...) do {} while (0)
#define DPRINTK(fmt, args...)	printk(KERN_DEBUG "[IOP]: " fmt, ## args);

LIST_HEAD(iop_handles);
//spinlock_t iop_lock = SPIN_LOCK_UNLOCKED;
DEFINE_SPINLOCK(iop_lock);


static void iop_do_tasklet(unsigned long);
DECLARE_TASKLET(iop_tasklet, iop_do_tasklet, 0);

irqreturn_t iop_interrupt(int irq, void *dev_id)
{
	//iopReg_t *piopReg = (iopReg_t *)LOGI_ADDR_IOP_REG;
	//piopReg->iop_control = piopReg->iop_control | (0x1<<6); //Kaney mark
	//piopReg->iop_control = piopReg->iop_control & 0xff7f; //Kaney add
	HWREG_W(iop_control, HWREG_R(iop_control)&0xff7f);

	DPRINTK("#####################iop_interrupt0 \n");

#if defined(SUPPORT_VFD_DRIVER) && defined(CONFIG_SUNPLUS_VFD_STB)
	if(HWREG_R(iop_data[ISR_PORT]) & ISR_VFD)
	{
		DRV_VfdDsr();
		HWREG_W(iop_data[ISR_PORT] ,HWREG_R(iop_data[ISR_PORT])&(~ISR_VFD));
	}
#endif
#if defined(SUPPORT_GPIO_MONITOR) && defined(CONFIG_SUNPLUS_GPIO_MONITOR_STB)
	if((HWREG_R(iop_data[ISR_PORT]))&IOP_INTR_GPMON)
	{
		DRV_GpMon_IopDsr();
		HWREG_W(iop_data[ISR_PORT] ,HWREG_R(iop_data[ISR_PORT])&(~IOP_INTR_GPMON));
	}
#endif

	tasklet_schedule(&iop_tasklet);
	return IRQ_HANDLED;
}

irqreturn_t iop_interrupt1(int irq, void *dev_id)
{
	//iopReg_t *piopReg = (iopReg_t *)LOGI_ADDR_IOP_REG;
	//piopReg->iop_control = piopReg->iop_control | (0x1<<7); //Kaney mark
	//piopReg->iop_control = piopReg->iop_control & 0xffbf; //Kaney add
    HWREG_W(iop_control, HWREG_R(iop_control)&0xffbf);

	printk("#####################iop_interrupt1 \n");
	//tasklet_schedule(&iop_tasklet);
	return IRQ_HANDLED;
}



static void iop_do_tasklet(unsigned long unused)
{
	struct list_head *pos;
	struct iop_handle_node *pnode;

	DPRINTK("iop tasklet test !\n");

	//DPRINTK("iop iop_handles = %x, iop_handles.next = %x, iop_handles.pre = %x\n", &iop_handles, iop_handles.next, iop_handles.prev);
	list_for_each(pos, &iop_handles)
	{
		pnode = list_entry(pos, struct iop_handle_node, list);
		pnode->handle();
	}

}

void  iop_register_handle(struct iop_handle_node *pnode)
{
	unsigned long flags;

	spin_lock_irqsave(&iop_lock, flags);
	list_add(&pnode->list, &iop_handles);
	spin_unlock_irqrestore( &iop_lock, flags);
}
EXPORT_SYMBOL(iop_register_handle);


void iop_unregister_handle(struct iop_handle_node *pnode)
{
	unsigned long flags;

	spin_lock_irqsave(&iop_lock, flags);
	list_del(&pnode->list);
	spin_unlock_irqrestore( &iop_lock, flags);
}
EXPORT_SYMBOL(iop_unregister_handle);

extern UINT32 gIOPBufAddr;
extern UINT32 gIOPBufAddr_phys;


void iop_get_buffer_addr(unsigned int *buf_va, unsigned int *buf_pa)
{
	*buf_va = gIOPBufAddr;
	*buf_pa = gIOPBufAddr_phys;
}
EXPORT_SYMBOL(iop_get_buffer_addr);


static int sp_iop_open(struct inode *inode, struct file *pfile)
{
	printk("Sunplus IOP module open\n");

	return 0;
}

static int sp_iop_release(struct inode *inode, struct file *pfile)
{
	printk("Sunplus IOP module release\n");
	return 0;
}

static long sp_iop_ioctl(struct file *pfile, unsigned int cmd, unsigned long arg)
{
	int ret = 0;
	wakeup_timer_t cmd_data;
    UINT32 vfd_data;
	I2C_CMD_PARAM_T i2c_cmd;
    int i;
	UINT8 value;

	switch (cmd)
	{
		case IOP_CMD_STANDBY_WAKEUPTIMER:
			if (copy_from_user(&cmd_data, (void __user *)arg, sizeof(cmd_data)))
			{
				ret = -1;
				break;
			}
		#ifdef SUPPORT_IOP2_STANDBY_WAKEUPTIMER
			wakeup_timer = cmd_data;
			printk("cmd_data.bCurYear = %d, wakeup_timer.bCurYear = %d\n", cmd_data.bCurYear, wakeup_timer.bCurYear);
			printk("cmd_data.bCurMon = %d, wakeup_timer.bCurMon = %d\n", cmd_data.bCurMon, wakeup_timer.bCurMon);
			printk("cmd_data.bCurDay = %d, wakeup_timer.bCurDay = %d\n", cmd_data.bCurDay, wakeup_timer.bCurDay);
			printk("cmd_data.bCurHour = %d, wakeup_timer.bCurHour = %d\n", cmd_data.bCurHour, wakeup_timer.bCurHour);
			printk("cmd_data.bCurMin = %d, wakeup_timer.bCurMin = %d\n", cmd_data.bCurMin, wakeup_timer.bCurMin);
			printk("cmd_data.bCurSec = %d, wakeup_timer.bCurSec = %d\n", cmd_data.bCurSec, wakeup_timer.bCurSec);
			printk("cmd_data.bWakeupYear = %d, wakeup_timer.bWakeupYear = %d\n", cmd_data.bWakeupYear, wakeup_timer.bWakeupYear);
			printk("cmd_data.bWakeupMon = %d, wakeup_timer.bWakeupMon = %d\n", cmd_data.bWakeupMon, wakeup_timer.bWakeupMon);
			printk("cmd_data.bWakeupDay = %d, wakeup_timer.bWakeupDay = %d\n", cmd_data.bWakeupDay, wakeup_timer.bWakeupDay);
			printk("cmd_data.bWakeupHour = %d, wakeup_timer.bWakeupHour = %d\n", cmd_data.bWakeupHour, wakeup_timer.bWakeupHour);
			printk("cmd_data.bWakeupMin = %d, wakeup_timer.bWakeupMin = %d\n", cmd_data.bWakeupMin, wakeup_timer.bWakeupMin);		
		#endif
			break;

		case IOP_CMD_STANDBY:
			MAINAPP_FinalizePlatform();
			MAINAPP_FinalizePlatform2();
			break;

     #if defined(SUPPORT_VFD_DRIVER) && defined(CONFIG_SUNPLUS_VFD_STB)
		case IOP_CMD_VFD_EVENT_CURPROG_NO:
			if (copy_from_user(&vfd_data, (void __user *)arg, sizeof(vfd_data)))
			{
				ret = -1;
				break;
			}
			VFDProgNo = vfd_data;
			printk("VFDProgNo = %d\n", VFDProgNo);
			DRV_VfdUpdateData(VFD_EVENT_CURPROG_NO);
			break;

		case IOP_CMD_VFD_EVENT_CURMULTIMEDIA_NO:
			if (copy_from_user(&vfd_data, (void __user *)arg, sizeof(vfd_data)))
			{
				ret = -1;
				break;
			}
			VFDMultimediaNo = vfd_data;
			printk("VFDMultimediaNo = %d\n", VFDMultimediaNo);
			DRV_VfdUpdateData(VFD_EVENT_CURMULTIMEDIA_NO);
			break;
     #endif

	 #if defined(CONFIG_SUNPLUS_I2C_CAR)
	 	case I2C_CMD_WRITE:
			if (copy_from_user(&i2c_cmd, (void __user *)arg, sizeof(I2C_CMD_PARAM_T)))
			{
				ret = -1;
				break;
			}
            printk("i2c_cmd.bus = %d\n", i2c_cmd.bus);
			printk("i2c_cmd.eDevSel = %d\n", i2c_cmd.eDevSel);
			printk("i2c_cmd.slaveAddr = %x\n", i2c_cmd.slaveAddr);
			printk("i2c_cmd.DevSetting = %d\n", i2c_cmd.DevSetting);
			printk("i2c_cmd.subAddrLength = %d\n", i2c_cmd.subAddrLength);
			printk("i2c_cmd.subAddr = %x\n", *(i2c_cmd.subAddr));
			printk("i2c_cmd.dataCnt = %d\n", i2c_cmd.dataCnt);
			printk("i2c_cmd.data = %x\n", *(i2c_cmd.data));
			printk("i2c_cmd.Timeout = %d\n", i2c_cmd.Timeout);
			printk("i2c_cmd.flag = %d\n", i2c_cmd.flag);
			DRV_I2cWrite(i2c_cmd.bus, i2c_cmd.eDevSel, i2c_cmd.slaveAddr,
							i2c_cmd.DevSetting, i2c_cmd.subAddrLength, i2c_cmd.subAddr,
							i2c_cmd.dataCnt, i2c_cmd.data, i2c_cmd.Timeout, i2c_cmd.flag);
			break;

		case I2C_CMD_READ:
			if (copy_from_user(&i2c_cmd, (void __user *)arg, sizeof(I2C_CMD_PARAM_T)))
			{
				ret = -1;
				break;
			}
			printk("i2c_cmd.bus = %d\n", i2c_cmd.bus);
			printk("i2c_cmd.eDevSel = %d\n", i2c_cmd.eDevSel);
			printk("i2c_cmd.slaveAddr = %x\n", i2c_cmd.slaveAddr);
			printk("i2c_cmd.DevSetting = %d\n", i2c_cmd.DevSetting);
			printk("i2c_cmd.subAddrLength = %d\n", i2c_cmd.subAddrLength);
			printk("i2c_cmd.subAddr = %x\n", *(i2c_cmd.subAddr));
			printk("i2c_cmd.dataCnt = %d\n", i2c_cmd.dataCnt);
			printk("i2c_cmd.data = %x\n", *(i2c_cmd.data));
			printk("i2c_cmd.Timeout = %d\n", i2c_cmd.Timeout);
			printk("i2c_cmd.flag = %d\n", i2c_cmd.flag);
			DRV_I2cRead(i2c_cmd.bus, i2c_cmd.eDevSel, i2c_cmd.slaveAddr,
							i2c_cmd.DevSetting, i2c_cmd.subAddrLength, i2c_cmd.subAddr,
							i2c_cmd.dataCnt, i2c_cmd.data, i2c_cmd.Timeout, i2c_cmd.flag);

			for(i=0; i<i2c_cmd.dataCnt; i++)
			{
			    printk("i2c_cmd.data[%d] = %x\n", i, i2c_cmd.data[i]);
			}
			if (copy_to_user((void __user *)arg, &i2c_cmd, sizeof(I2C_CMD_PARAM_T)))
			{
				ret = -1;
				break;
			}
			break;
	 #endif

	 #if defined(CONFIG_SUNPLUS_BACKUP_CAM_CAR)
	 	case IOP_CMD_IS_BACKUP_CAM:
			if (DRV_SUCCESS != DRV_BackupCam_CheckBackupState(&value))
			{
			    printk(KERN_INFO "[IOP] ioctl: DRV_BackupCam_CheckBackupState fail!\n");
				value = 2;
			}

			DRV_BackupCam_IOPControlCamDisable();
		
			printk("IOP ioctl: IOP_CMD_IS_BACKUP_CAM: value = %d\n", value);
			if (copy_to_user((void __user *)arg, &value, sizeof(value)))
			{
				ret = -1;
				break;
			}
			break;
	 #endif

		default:
			printk("Unknow command\n");
			break;
	}

	return ret;
}

static int sp_iop_probe(struct platform_device *pdev)
{
	//printk("Sunplus IOP driver probe\n");
	return 0;
}

static int sp_iop_remove(struct platform_device *pdev)
{
	//printk("Sunplus IOP driver remove\n");
	return 0;
}

static int sp_iop_suspend(struct platform_device *pdev, pm_message_t state)
{
	printk("Sunplus IOP driver suspend\n");
#if 0 /* move to sp_pm.c for suspend to ram */
	MAINAPP_FinalizePlatform(); 
#endif
	return 0;
}

static int sp_iop_resume(struct platform_device *pdev)
{
	printk("Sunplus IOP driver resume\n");

#if 0 /* init IOP move to u-boot, and move to sp_pm.c for suspend to ram */	
	u32 system_clk;
	system_clk = sp_clk_get(SP_CLK_SYSFAST);
	system_clk = system_clk/1000000;
	printk("system_clk = %d\n", system_clk);
	
    DRV_IOP_InitByClk((UINT8 *) IOPcode_NEC, system_clk);
#endif
	return 0;
}

static struct file_operations sp_iop_fops = {
	.owner          = THIS_MODULE,
	.open           = sp_iop_open,
	.release        = sp_iop_release,
	.unlocked_ioctl = sp_iop_ioctl,
};

struct miscdevice gMiscIOP;

static struct platform_device sp_iop_device = {
	.name			= "sunplus_iop",
	.id				= -1,
};

static struct platform_driver sp_iop_driver = {
	.probe		= sp_iop_probe,
	.remove		= sp_iop_remove,
	.suspend		= sp_iop_suspend,
	.resume		= sp_iop_resume,
	.driver		=
	{
		.name	= "sunplus_iop",
		.owner	= THIS_MODULE,
	},
};

static int __init sphe_iop_init(void)
{
    int ret;
    printk(KERN_INFO "Init letb IOP Driver\n");

	ret = platform_device_register(&sp_iop_device);
	if (ret)
	{
		printk("iop device register failed, ret = %d\n", ret);
		goto device_register_fail;
	}

	ret = platform_driver_register(&sp_iop_driver);
	if (ret < 0)
	{
		printk("iop driver register failed, ret = %d\n", ret);
		goto driver_register_fail;
	}
	
	/*u32 system_clk;
	system_clk = sp_clk_get(SP_CLK_SYSFAST);
	system_clk = system_clk/1000000;
	printk("system_clk = %d\n", system_clk);
	
    if (DRV_SUCCESS != DRV_IOPInitByClk((UINT8 *) IOPcode_NEC, 30)) //system_clk))
    {
        printk(KERN_ERR"[IOP]:DRV_IOPInitByClk fail\n");
        return ENOMEM;
    }*/

    if (DRV_SUCCESS != DRV_IOPInit_without_fw())
    {
        printk(KERN_ERR"[IOP]:DRV_IOPInit_without_fw fail\n");
        return ENOMEM;
    }

    // Registering misc device
	gMiscIOP.minor	= MISC_DYNAMIC_MINOR;
	gMiscIOP.name	= "sp_iop";
	gMiscIOP.fops	= &sp_iop_fops;
    ret = misc_register(&gMiscIOP);
	if (ret) {
		printk("misc_register failed, ret = %d\n", ret);
		return -1;
	}
    return 0;
	
driver_register_fail:
	platform_device_unregister(&sp_iop_device);
device_register_fail:

	return ret;
}

static void __exit sphe_iop_exit(void)
{
    printk(KERN_INFO "Exit letb IOP Driver\n");
	DRV_IOPUninit();
	//gp_chunk_free((void *)gIOPBufAddr); //Kaney add
	misc_deregister(&gMiscIOP);
	platform_driver_unregister(&sp_iop_driver);
	platform_device_unregister(&sp_iop_device);
}

module_init(sphe_iop_init);
module_exit(sphe_iop_exit);

MODULE_AUTHOR("letb IOP_CAR");
MODULE_DESCRIPTION("letb IOP_CAR Driver");
MODULE_LICENSE("GPL");
