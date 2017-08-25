#include <linux/device.h>
#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/input.h>
#include <linux/delay.h>

#define DEVNUM_COUNT 		1
#define DEVNUM_NAME 		("V_KEY_DEV")
#define INPUT_NAME 		  ("V_KEY_INPUT")
#define DEVNUM_MINOR_START 	0

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Kiwi");
MODULE_DESCRIPTION("a virtual keyboard driver sample");

static dev_t v_key_num = 0;
static struct cdev v_key_cdev;
static struct class* dev_class = NULL;
struct input_dev *input_v_key;

static int input_v_key_open(struct input_dev *dev)
{
  printk("@@@ input device opened callback!!\n");
  return 0;
}

static int input_v_key_close(struct input_dev *dev)
{
  printk("@@@ input device closed callback!!\n");
  return 0;
}

static int input_v_key_event(struct input_dev *dev)
{
  printk("@@@ input device get event callback!!\n");
  return 0;
}



static int v_key_open(struct inode *ind, struct file *filp)
{
  printk("@@@@ v_key opened\n");
  return 0;
}

static int v_key_close(struct inode *ind, struct file *filp)
{
  printk("@@@@ v_key closed\n");
  return 0;
}

static long v_key_ioctl(struct file *file, unsigned int keycode, unsigned long sent_value2)
{
//  printk("@@@@ Get ioctl keycode: %d\n", keycode);

  if ( (keycode < 0) || (keycode >= KEY_MAX ) )
  {
    printk("@@@@ This keycode is over range, Ignored!\n");
    return 0;
  }
  else if ( keycode < 130 )  // 0 <= keycode < 130
  {
    printk("@@@@ This keycode is default!\n");
  }
  else if ( keycode <= 248 ) // 130 <= keycode <= 248
  {
    printk("@@@@ This keycode is user defined!\n");
  }
  else
  {
    printk("@@@@ This keycode is not defined!\n");
  }  
  
  // report key, simulate a key pressed and released
  input_report_key(input_v_key, keycode, 1);
  mdelay(100);
  input_report_key(input_v_key, keycode, 0);
  
  // report rel
//  input_report_rel(input_v_key, REL_X, sent_value1);  
//  input_report_rel(input_v_key, REL_Y, sent_value2);  

  input_sync(input_v_key);
  
  return 0;
}

static const struct file_operations v_key_fops = 
{
  .open           = v_key_open,
  .release        = v_key_close,
  .unlocked_ioctl = v_key_ioctl,
};

static int __init v_key_init(void)
{
//  printk("@@@@ test 777 @@@@ \n");
  printk("Insert v_key driver!\n");

  int i = 0;

  // Request a device number from kernel
  int ret = 0;
  ret = alloc_chrdev_region( &v_key_num, DEVNUM_MINOR_START, DEVNUM_COUNT, DEVNUM_NAME );
  if (ret) 
  {
    printk(KERN_WARNING "%s : could not allocate device\n", __func__);
    return ret;
  }
  else 
  {
    printk(KERN_WARNING "%s : registered with major number:%i, minor number:%i\n",
        __func__, MAJOR(v_key_num), MINOR(v_key_num));
  }

  // Initialize device parameters
  cdev_init( &v_key_cdev, &v_key_fops);
  v_key_cdev.owner = THIS_MODULE;
  v_key_cdev.ops = &v_key_fops;
  ret = cdev_add( &v_key_cdev, v_key_num, DEVNUM_COUNT );

  // sys class related
  dev_class = class_create(THIS_MODULE, "v_key");
  if (IS_ERR(dev_class)) 
  {
    printk("class_simple_create fail %s\n", "v_key_class");
    return -1;
  }
  printk("class_create %s\n", "v_key_class");
	
  device_create( dev_class, NULL, v_key_num,NULL ,DEVNUM_NAME );
  printk("device_create %s\n", DEVNUM_NAME);


  int error = 0;
  
  // Request a input device
  input_v_key = input_allocate_device();
	if ( !input_v_key )
	{
    printk("input device %s create fail\n", INPUT_NAME);
    goto fail1;
  }

  input_v_key->name = INPUT_NAME;

  // report key mask
  set_bit(EV_KEY, input_v_key->evbit);
  for ( i = 0; i < KEY_MAX; i++)
  {
    set_bit( i, input_v_key->keybit);
  }
//  set_bit(BTN_0, input_v_key->keybit);
//  set_bit(BTN_1, input_v_key->keybit);

  // report rel mask
//  set_bit(EV_REL, input_v_key->evbit);
//  set_bit(REL_X,  input_v_key->relbit);
//  set_bit(REL_Y,  input_v_key->relbit);
    
  input_v_key->open  = input_v_key_open;
  input_v_key->close = input_v_key_close;
  input_v_key->event = input_v_key_event;

  error = input_register_device(input_v_key);
  if (error) 
  {
    printk("Failed to register device\n");
    goto fail1;
  }
  
  return 0;

fail1:	
  input_unregister_device(input_v_key);
  input_free_device(input_v_key);
	
}

static void v_key_exit(void)
{
  printk("Exit v_key driver!\n");

  device_destroy(dev_class, v_key_num);
  class_destroy(dev_class);

  cdev_del( &v_key_cdev );
  unregister_chrdev_region( v_key_num, DEVNUM_COUNT );

  input_unregister_device(input_v_key);
  input_free_device(input_v_key);
}

module_init(v_key_init);
module_exit(v_key_exit);
