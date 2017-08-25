#include <linux/device.h>
#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/input.h>
#include <linux/delay.h>

#define DEVNUM_COUNT 		1
#define DEVNUM_NAME 		("V_MOUSE_DEV")
#define INPUT_NAME 		  ("V_MOUSE_INPUT")
#define DEVNUM_MINOR_START 	0

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Kiwi");
MODULE_DESCRIPTION("a virtual mouse driver sample");

static dev_t v_mouse_num = 0;
static struct cdev v_mouse_cdev;
static struct class* dev_class = NULL;
struct input_dev *input_v_mouse;

static int input_v_mouse_open(struct input_dev *dev)
{
  printk("@@@ input device opened callback!!\n");
  return 0;
}

static int input_v_mouse_close(struct input_dev *dev)
{
  printk("@@@ input device closed callback!!\n");
  return 0;
}

static int input_v_mouse_event(struct input_dev *dev)
{
  printk("@@@ input device get event callback!!\n");
  return 0;
}



static int v_mouse_open(struct inode *ind, struct file *filp)
{
  printk("@@@@ v_mouse opened\n");
  return 0;
}

static int v_mouse_close(struct inode *ind, struct file *filp)
{
  printk("@@@@ v_mouse closed\n");
  return 0;
}

static unsigned int ori_x = 0;
static unsigned int ori_y = 0;

typedef struct _TOUCH_EVENT
{
	unsigned int x;
  unsigned int y;
  unsigned int click_type;
}TOUCH_EVENT;

enum
{
  DO_NOTHING  = 0,
  CLICK,
  LONG_CLICK,
};

static long v_mouse_ioctl(struct file *file, unsigned int para1, unsigned long para2)
{
  TOUCH_EVENT *pEvent = (TOUCH_EVENT *)para2;
    
  int i;
  int dif_x;
  int dif_y;

  printk("@@@@ driver get X: %d, Y: %d, Click: %d\n", pEvent->x, pEvent->y, pEvent->click_type );
  printk("[CY]Original x: %d, y: %d\n", ori_x, ori_y);

 
  
#ifdef MOVE_RELEVANT
	
  //  dif_x = pEvent->x - ori_x;
  //  dif_y = pEvent->y - ori_y;

  // report rel x
  if( pEvent->x > ori_x)
  {
  	dif_x = pEvent->x - ori_x;
	
  	for(i = 0; i < dif_x; i++)
  	{
      input_report_rel(input_v_mouse, REL_X, 1);
      input_sync(input_v_mouse);
      mdelay(1);
  	}
  }
  else
  {
  	dif_x = ori_x - pEvent->x;

  	for(i = 0; i < dif_x; i++)
  	{
      input_report_rel(input_v_mouse, REL_X, -1);
      input_sync(input_v_mouse);
      mdelay(1);
  	}
  }
  
  // report rel y
  if(pEvent->y > ori_y)
  {
  	dif_y = pEvent->y - ori_y;
	
  	for(i = 0; i < dif_y; i++)
  	{
      input_report_rel(input_v_mouse, REL_Y, 1);
      input_sync(input_v_mouse);
      mdelay(1);
  	}
  }
  else
  {
  	dif_y = ori_y - pEvent->y;

  	for(i = 0; i < dif_y; i++)
  	{
  		input_report_rel(input_v_mouse, REL_Y, -1);
      input_sync(input_v_mouse);
  		mdelay(1);
  	}
  }
#else

  input_report_rel(input_v_mouse, REL_X, -1000);
  input_report_rel(input_v_mouse, REL_Y, -500);
  input_sync(input_v_mouse);
  mdelay(500);
  
  input_report_rel(input_v_mouse, REL_X, pEvent->x);
  input_report_rel(input_v_mouse, REL_Y, pEvent->y);
  input_sync(input_v_mouse);
  
#endif

  ori_x = pEvent->x;
  ori_y = pEvent->y;
  
  switch ( pEvent->click_type )
  {
    case 1:
      input_report_key(input_v_mouse, BTN_LEFT, 1);
      input_sync(input_v_mouse);
      
      mdelay(100);
      
      input_report_key(input_v_mouse, BTN_LEFT, 0);
      input_sync(input_v_mouse);

      break;

    case 2:
      input_report_key(input_v_mouse, BTN_LEFT, 1);
      input_sync(input_v_mouse);
      
      mdelay(1500);
      
      input_report_key(input_v_mouse, BTN_LEFT, 0);
      input_sync(input_v_mouse);

      break;

    default:
      break;
      
  }
  
  
  return 0;
}

static const struct file_operations v_mouse_fops = 
{
  .open           = v_mouse_open,
  .release        = v_mouse_close,
  .unlocked_ioctl = v_mouse_ioctl,
};

static int __init v_mouse_init(void)
{
//  printk("@@@@ test 777 @@@@ \n");
  printk("Insert v_mouse driver!\n");

  int i = 0;

  // Request a device number from kernel
  int ret = 0;
  ret = alloc_chrdev_region( &v_mouse_num, DEVNUM_MINOR_START, DEVNUM_COUNT, DEVNUM_NAME );
  if (ret) 
  {
    printk(KERN_WARNING "%s : could not allocate device\n", __func__);
    return ret;
  }
  else 
  {
    printk(KERN_WARNING "%s : registered with major number:%i, minor number:%i\n",
        __func__, MAJOR(v_mouse_num), MINOR(v_mouse_num));
  }

  // Initialize device parameters
  cdev_init( &v_mouse_cdev, &v_mouse_fops);
  v_mouse_cdev.owner = THIS_MODULE;
  v_mouse_cdev.ops = &v_mouse_fops;
  ret = cdev_add( &v_mouse_cdev, v_mouse_num, DEVNUM_COUNT );

  // sys class related
  dev_class = class_create(THIS_MODULE, "v_mouse");
  if (IS_ERR(dev_class)) 
  {
    printk("class_simple_create fail %s\n", "v_mouse_class");
    return -1;
  }
  printk("class_create %s\n", "v_mouse_class");
	
  device_create( dev_class, NULL, v_mouse_num,NULL ,DEVNUM_NAME );
  printk("device_create %s\n", DEVNUM_NAME);


  int error = 0;
  
  // Request a input device
  input_v_mouse = input_allocate_device();
	if ( !input_v_mouse )
	{
    printk("input device %s create fail\n", INPUT_NAME);
    goto fail1;
  }

  input_v_mouse->name = INPUT_NAME;

  // report key mask
  set_bit(EV_KEY, input_v_mouse->evbit);
  set_bit(BTN_LEFT, input_v_mouse->keybit);
  set_bit(BTN_RIGHT, input_v_mouse->keybit);
  

  // report rel mask
  set_bit(EV_REL, input_v_mouse->evbit);
  set_bit(REL_X,  input_v_mouse->relbit);
  set_bit(REL_Y,  input_v_mouse->relbit);
	
  input_v_mouse->open  = input_v_mouse_open;
  input_v_mouse->close = input_v_mouse_close;
  input_v_mouse->event = input_v_mouse_event;

  error = input_register_device(input_v_mouse);
  if (error) 
  {
    printk("Failed to register device\n");
    goto fail1;
  }
  
  return 0;

fail1:	
  input_unregister_device(input_v_mouse);
  input_free_device(input_v_mouse);
	
}

static void v_mouse_exit(void)
{
  printk("Exit v_mouse driver!\n");

  device_destroy(dev_class, v_mouse_num);
  class_destroy(dev_class);

  cdev_del( &v_mouse_cdev );
  unregister_chrdev_region( v_mouse_num, DEVNUM_COUNT );

  input_unregister_device(input_v_mouse);
  input_free_device(input_v_mouse);
}

module_init(v_mouse_init);
module_exit(v_mouse_exit);