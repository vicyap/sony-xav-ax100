#include <asm/arch/regmap.h>

#define SUPPORT_IOP_CHECKSUM
#define IOP_CODE_SIZE   16384 //16KB

#define IOP_CHECKSUM_START      (1 << 9)
#define IOP_CHECKSUM_READY      (1 << 10)

#define IOP_GO()                IOP_REG->iop_control = (IOP_REG->iop_control)&~0x01;
#define IOP_ENABLE_32BIT_ADDR() IOP_REG->memory_bridge_control = (IOP_REG->memory_bridge_control) | (1 << 0);

#if 0
struct gpioxt_regs {
	unsigned int gpio_master[6];
	unsigned int g5_reserved_0[2];
	unsigned int gpio_oe[6];
	unsigned int g5_reserved_1[2];
	unsigned int gpio_out[6];
	unsigned int g5_reserved_2[2];
	unsigned int gpio_in[6];
	unsigned int g5_reserved_3[2];
};
#define GPIOXT_REG ((volatile struct gpioxt_regs *)RF_GRP(5, 0))

// GPIO macro
#define GPIO_F_SET(a,d) do { if (d) {PAD_CTL_REG->gpio_first[a/32] = (PAD_CTL_REG->gpio_first[a/32])|(1<<(a%32)); \
                            } else {PAD_CTL_REG->gpio_first[a/32] = (PAD_CTL_REG->gpio_first[a/32])& ~(1<<(a%32)); }} while (0)

#define GPIO_M_SET(a,d) do { if (d) {GPIOXT_REG->gpio_master[a/32] = (GPIOXT_REG->gpio_master[a/32])|(1<<(a%32)); \
                            } else  {GPIOXT_REG->gpio_master[a/32] = (GPIOXT_REG->gpio_master[a/32])&~(1<<(a%32)); }} while (0)

#define GPIO_E_SET(a,d) do { if (d) {GPIOXT_REG->gpio_oe[a/32] = (GPIOXT_REG->gpio_oe[a/32])|(1<<(a%32)); \
                            } else  {GPIOXT_REG->gpio_oe[a/32] = (GPIOXT_REG->gpio_oe[a/32])&~(1<<(a%32)); }} while (0)

#define GPIO_O_SET(a,d) do { if (d) {GPIOXT_REG->gpio_out[a/32] = (GPIOXT_REG->gpio_out[a/32])|(1<<(a%32)); \
                            } else  {GPIOXT_REG->gpio_out[a/32] = (GPIOXT_REG->gpio_out[a/32])&~(1<<(a%32)); }} while (0)
#endif 

#define IOP_RESET()     IOP_REG->iop_control = (IOP_REG->iop_control)|0x01;


// fixed me when sysclk is changed
#define SYSCLK  (30 * 1000 * 1000)

typedef enum{	
	PAN_RES_0,		// 0:800X480
	PAN_RES_1,		// 1:800X600
	PAN_RES_2,		// 2:1024X600
	PAN_RES_3,		// 3:1280X720
	PAN_RES_4,		// 4:1366X768
	PAN_RES_5,		// 5:1440X900
	PAN_RES_6,		// 6:1920X1080
}PANNEL_RESOLUTION_T;

//BACKUPCAM TO IOP CMD
typedef enum{	
	BACKUPCAM2IOP_SET_IOPCTRLCAMONOFF,
	BACKUPCAM2IOP_CHECK_ISIOPCTRL,
	BACKUPCAM2IOP_SET_GPIO,	
	BACKUPCAM2IOP_SET_PAN_RES,
}BACKUPCAM2IOP_CMD_T;

//IOP CMD TYPE
typedef enum{
	IOP_CMD_TYPE_IDLE,	
	IOP_CMD_TYPE_UART,	
	IOP_CMD_TYPE_I2C,	
	IOP_CMD_TYPE_ROATE,	
	IOP_CMD_TYPE_KEYBOARD,	
	IOP_CMD_TYPE_TOUCH,	
	IOP_CMD_TYPE_BACKUPCAM,	
	IOP_CMD_TYPE_POWER,
}IOP_CMD_TYPE_T;


