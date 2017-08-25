#ifndef _IACP_I2C_H_
#define _IACP_I2C_H_

#include <linux/cdev.h>
#include <linux/semaphore.h>

#define IACP_DEVICE_NODE_NAME	"iacp"
#define IACP_DEVICE_FILE_NAME	"iacp"
#define IACP_DEVICE_CLASS_NAME	"iacp"

struct st_iacp_dev{
struct cdev dev;
int    major;
unsigned char mem[1024];
struct semaphore semLock;
};
// global dev object
struct st_iacp_dev g_iacp_dev;

#endif