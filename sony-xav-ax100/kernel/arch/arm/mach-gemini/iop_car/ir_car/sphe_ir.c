/******************************************************
 IR

 This driver controls the IR component.

 Copyright (c) 2006 Sunplus, Inc.
 All rights reserved.
*******************************************************/
#include <linux/module.h>
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
#include <linux/input.h>

#include <iop_car/platform_define.h> //Kaney add
#include <iop_car/customize.h> //Kaney add
#include <iop_car/regmap.h> //Kaney add
#include <iop_car/drv_iop.h> //Kaney add
#include <iop_car/drv_ir.h> //Kaney add
#include "ir_local.h"
#include <iop_car/iop.h>
#include "ir_keycode_mapping.h"
//#include <asm/sysconfig.h>


//IOP ISR flag define
#define ISR_PORT			(11)
#define ISR_IR				(1 << 8)

//#define DPRINTK(fmt, args...) do {} while (0)
#define DPRINTK(fmt, args...)	printk(KERN_DEBUG "[IR]: " fmt, ## args);

struct input_dev *p_ir_dev;
static void ir_func(void);
static struct iop_handle_node ir_handle_node = { 	.handle = ir_func, };
static struct timer_list keyup_timer; // for key down/up mode
static int trigger_timer_ir_code = -1; // for key down/up mode


#if 0 //Kaney mark and move to ir_keycode_mapping.h 
static const unsigned int ir_keycode[] = 
{
	//0x00,				0x01,				0x02,				0x03,
 	KEY_PREVIOUS,		0,					0,					KEY_NEXT,
 	//0x04,				0x05,				0x06,				0x07,
 	KEY_PAUSECD,		KEY_MUTE,			0,					KEY_SLOW,
	//0x08,				0x09,				0x0a,				0x0b,
 	KEY_AB,				KEY_VOLUMEDOWN,	KEY_6,				KEY_MEDIA_REPEAT,
	//0x0c,				0x0d,				0x0e,				0x0f,
 	KEY_MIXER,			KEY_CHANNELDOWN,	KEY_AUDIO,			KEY_HOME,
	//0x10,				0x11,				0x12,				0x13,
 	KEY_YELLOW,			KEY_LAST,			KEY_PAGEUP,			KEY_BLUE,
	//0x14,				0x15,				0x16,				0x17,
 	KEY_ENTER,			KEY_BACK,			0,					KEY_RIGHT,
	//0x18,				0x19,				0x1a,				0x1b,
 	KEY_DOWN,			KEY_CHANNELUP,		KEY_9,				KEY_PSCAN,
	//0x1c,				0x1d,				0x1e,				0x1f,
 	KEY_UP,				KEY_VOLUMEUP,		KEY_3,				KEY_EPG,
	//0x20,				0x21,				0x22,				0x23,
 	0,					0,					0,					0,
	//0x24,				0x25,				0x26,				0x27,
 	0,					0,					0,					0,
	//0x28,				0x29,				0x2a,				0x2b,
 	0,					0,					0,					0,
	//0x2c,				0x2d,				0x2e,				0x2f,
 	0,					0,					0,					0,
	//0x30,				0x31,				0x32,				0x33,
 	0,					0,					0,					0,
	//0x34,				0x35,				0x36,				0x37,
 	0,					0,					0,					0,
	//0x38,				0x39,				0x3a,				0x3b,
 	0,					0,					0,					0,
	//0x3c,				0x3d,				0x3e,				0x3f,
 	0,					0,					0,					0,
	//0x40,				0x41,				0x42,				0x43,
 	KEY_FRAMEFORWARD,	0,					0,					KEY_FRAMEBACK,
	//0x44,				0x45,				0x46,				0x47,
 	KEY_PLAYCD,			0,					KEY_POWER,			KEY_STOP,
	//0x48,				0x49,				0x4a,				0x4b,
 	KEY_GOTO,			KEY_5,				KEY_4,				KEY_RECORD,
	//0x4c,				0x4d,				0x4e,				0x4f,
 	KEY_VFMT,			KEY_0,				KEY_FAVORITES,		KEY_DEV,
	//0x50,				0x51,				0x52,				0x53,
 	KEY_GREEN,			KEY_PAGEDOWN,		KEY_TV,				KEY_RED,
	//0x54,				0x55,				0x56,				0x57,
 	0,					KEY_MENU,			KEY_INFO,			KEY_LEFT,
	//0x58,				0x59,				0x5a,				0x5b,
 	0,					KEY_8,				KEY_7,				KEY_SUBTITLE,
	//0x5c,				0x5d,				0x5e,				0x5f,
 	0,					KEY_2,				KEY_1,				KEY_TEXT,
	//0x60,				0x61,				0x62,				0x63,
 	0,					0,					0,					0,
	//0x64,				0x65,				0x66,				0x67,
 	0,					0,					0,					0,
	//0x68,				0x69,				0x6a,				0x6b,
 	0,					0,					0,					0,
};

// for key down/up mode:
static const unsigned int ir_keymode[] = 
{
	//0x00,					0x01,					0x02,					0x03,
 	//KEY_PREVIOUS,			0,						0,						KEY_NEXT,
 	0,						0,						0,						0,
 	//0x04,					0x05,					0x06,					0x07,
 	//KEY_PAUSECD,			KEY_MUTE,				0,						KEY_SLOW,
 	0,						0,						0,						0,
	//0x08,					0x09,					0x0a,					0x0b,
 	//KEY_AB,				KEY_VOLUMEDOWN,		KEY_6,					KEY_MEDIA_REPEAT,
 	0,						0,						0,						0,
	//0x0c,					0x0d,					0x0e,					0x0f,
 	//KEY_MIXER,			KEY_CHANNELDOWN,		KEY_AUDIO,				KEY_SOURCE,
 	0,						0,						0,						0,
	//0x10,					0x11,					0x12,					0x13,
 	//KEY_YELLOW,			KEY_LAST,				KEY_PAGEUP,				KEY_BLUE,
 	0,						0,						0,						0,
	//0x14,					0x15,					0x16,					0x17,
 	//KEY_ENTER,			KEY_BACK,				0,						KEY_RIGHT,
 	0,						KEY_MODE_LONGPRESS,	0,						KEY_MODE_REPEAT,
	//0x18,					0x19,					0x1a,					0x1b,
 	//KEY_DOWN,			KEY_CHANNELUP,			KEY_9,					KEY_PSCAN,
 	KEY_MODE_REPEAT,		0,						0,						0,
	//0x1c,					0x1d,					0x1e,					0x1f,
 	//KEY_UP,				KEY_VOLUMEUP,			KEY_3,					KEY_EPG,
 	KEY_MODE_REPEAT,		0,						0,						0,
	//0x20,					0x21,					0x22,					0x23,
 	0,						0,						0,						0,
	//0x24,					0x25,					0x26,					0x27,
 	0,						0,						0,						0,
	//0x28,					0x29,					0x2a,					0x2b,
 	0,						0,						0,						0,
	//0x2c,					0x2d,					0x2e,					0x2f,
 	0,						0,						0,						0,
	//0x30,					0x31,					0x32,					0x33,
 	0,						0,						0,						0,
	//0x34,					0x35,					0x36,					0x37,
 	0,						0,						0,						0,
	//0x38,					0x39,					0x3a,					0x3b,
 	0,						0,						0,						0,
	//0x3c,					0x3d,					0x3e,					0x3f,
 	0,						0,						0,						0,
	//0x40,					0x41,					0x42,					0x43,
 	//KEY_FRAMEFORWARD,	0,						0,						KEY_FRAMEBACK,
 	0,						0,						0,						0,
	//0x44,					0x45,					0x46,					0x47,
 	//KEY_PLAYCD,			0,						KEY_POWER,				KEY_STOP,
 	0,						0,						0,						0,
	//0x48,					0x49,					0x4a,					0x4b,
 	//KEY_GOTO,				KEY_5,					KEY_4,					KEY_RECORD,
 	0,						0,						0,						0,
	//0x4c,					0x4d,					0x4e,					0x4f,
 	//KEY_VFMT,				KEY_0,					KEY_FAVORITES,			KEY_DEV,
 	0,						0,						0,						0,
	//0x50,					0x51,					0x52,					0x53,
 	//KEY_GREEN,			KEY_PAGEDOWN,			KEY_TV,					KEY_RED,
 	0,						0,						0,						0,
	//0x54,					0x55,					0x56,					0x57,
 	//0,						KEY_MENU,				KEY_INFO,				KEY_LEFT,
 	0,						KEY_MODE_LONGPRESS,	0,						KEY_MODE_REPEAT,
	//0x58,					0x59,					0x5a,					0x5b,
 	//0,						KEY_8,					KEY_7,					KEY_SUBTITLE,
 	0,						0,						0,						0,
	//0x5c,					0x5d,					0x5e,					0x5f,
 	//0,						KEY_2,					KEY_1,					KEY_TEXT,
 	0,						0,						0,						0,
	//0x60,					0x61,					0x62,					0x63,
 	0,						0,						0,						0,
	//0x64,					0x65,					0x66,					0x67,
 	0,						0,						0,						0,
	//0x68,					0x69,					0x6a,					0x6b,
 	0,						0,						0,						0,
};
#endif

#define IR_CODE_MAX    ARRAY_SIZE(ir_keycode)


static void ir_keyup(unsigned long ir_code) // for key down/up mode
{
    input_report_key(p_ir_dev, ir_keycode[ir_code], 0); //key up  
    input_sync(p_ir_dev);
    trigger_timer_ir_code = -1;
}

//DRV_Status_t DRV_IOPIrGetValue(UINT32 * pdRetIrCode, UINT32 dTimeout); //Kaney mark
static void ir_func(void)
{
	UINT32 ir_code, ir_mode, ir_key; // ir_mode for key down/up mode

	if((HWREG_R(iop_data[ISR_PORT]))&ISR_IR)
	{
		HWREG_W(iop_data[ISR_PORT] ,HWREG_R(iop_data[ISR_PORT])&(~ISR_IR));
		
		if (DRV_SUCCESS != DRV_IOPIrGetValue(&ir_code , &ir_mode, 0)) // ir_mode for key down/up mode
		{
			DPRINTK("[ir] DRV_IOPIrGetValue get error!\n");
			return;
		}

		ir_key = ir_keycode[ir_code];
		DPRINTK("[ir] ir_code = 0x%x, ir_mode = 0x%x, ir_key = %d\n", ir_code, ir_mode, ir_key); // ir_mode for key down/up mode

		if (ir_key && (ir_code <  IR_CODE_MAX))
		{
			/* if we don't define ANT_BROWSER, the left ctrl key is not need special processed */        

			// for key down/up mode: ir_mode = 0 as first; 0x4000 as repeat.
			if ((ir_keymode[ir_code] == KEY_MODE_NORMAL) && (!ir_mode))
			{
				input_report_key(p_ir_dev, ir_keycode[ir_code], 1); //key down
				input_sync(p_ir_dev);
				input_report_key(p_ir_dev, ir_keycode[ir_code], 0); //key up  
				input_sync(p_ir_dev);
			}
			else if (ir_keymode[ir_code] == KEY_MODE_REPEAT)
			{
				input_report_key(p_ir_dev, ir_keycode[ir_code], 1); //key down
				input_sync(p_ir_dev);
				input_report_key(p_ir_dev, ir_keycode[ir_code], 0); //key up  
				input_sync(p_ir_dev);
			}
			else if (ir_keymode[ir_code] == KEY_MODE_LONGPRESS)
			{
				if (!ir_mode)
				{
					if(trigger_timer_ir_code >= 0)
					{
						del_timer(&keyup_timer);
						ir_keyup((unsigned long)trigger_timer_ir_code);
					}
					input_report_key(p_ir_dev, ir_keycode[ir_code], 1); //key down
					input_sync(p_ir_dev);
				}

				keyup_timer.data = (unsigned long)ir_code;
				mod_timer(&keyup_timer, jiffies + msecs_to_jiffies(IR_REPEAT_MS));
				trigger_timer_ir_code = (int)ir_code;
			}
		}
	}
}

//DRV_Status_t DRV_IOPIrInit(void); //Kaney mark
//DRV_Status_t DRV_IOPIrSetCID(UINT32 dCustomerID); //Kaney mark
//DRV_Status_t DRV_IOPIrEnable(void); //Kaney mark

static int ir_open(struct input_dev *dev)
{
    printk(KERN_INFO "[ir] : ir open\n");
    
    if (DRV_SUCCESS != DRV_IOPIrInit()) 
        goto __fail0;
    if (DRV_SUCCESS != DRV_IOPIrSetCID(0xfe)) 
        goto  __fail0;
	
    DRV_IOPIrEnable();

    iop_register_handle(&ir_handle_node);

	setup_timer(&keyup_timer, ir_keyup, 0); // for key down/up mode
    return 0;

__fail0:
    printk(KERN_ERR"[ir]: sphe_ir_init error\n");
    return EIO;
}


static int __init sphe_ir_init(void)
{
    int i;
    
    printk(KERN_INFO "[ir]: Init letb ir Driver\n");

    p_ir_dev =  input_allocate_device();
    if (!p_ir_dev) 
    { 
        printk(KERN_ERR "[ir]: Not enough memory\n");
	goto __fail2; 
    }

    #ifdef SYSCONFIG_IR_NAME
        p_ir_dev->name = SYSCONFIG_IR_NAME;
    #else
        p_ir_dev->name = "letb_IR",
    #endif
    p_ir_dev->evbit[0] = BIT_MASK(EV_KEY); // remove | BIT_MASK(EV_REP) for key down/up mode
    for (i = 0; i < IR_CODE_MAX; i++)
        set_bit(ir_keycode[i], p_ir_dev->keybit);
	set_bit(KEY_LEFTCTRL, p_ir_dev->keybit); //for rotate
	set_bit(KEY_RIGHTCTRL, p_ir_dev->keybit); //for rotate
	set_bit(KEY_BACKUP_CAM, p_ir_dev->keybit); //for backup camera
    clear_bit(0, p_ir_dev->keybit);
    
    p_ir_dev->open = ir_open;

    if (input_register_device(p_ir_dev)) 
    { 
        printk(KERN_ERR "[ir] : Failed to register input device\n");
        goto __fail2;
    }
	
    return 0;

__fail2:
    input_free_device(p_ir_dev);
    return ENOMEM;
}


//DRV_Status_t DRV_IOPIrUninit(void); //Kaney mark
static void __exit sphe_ir_exit(void)
{
    printk(KERN_INFO "[ir]: Exit letb ir Driver\n");

    iop_unregister_handle(&ir_handle_node);
    DRV_IOPIrUninit();

    input_unregister_device(p_ir_dev);
    //input_free_device(p_ir_dev); //Kaney mark
}

module_init(sphe_ir_init);
module_exit(sphe_ir_exit);

MODULE_AUTHOR("letb IR_CAR");
MODULE_DESCRIPTION("letb IR_CAR Driver");
MODULE_LICENSE("GPL");
