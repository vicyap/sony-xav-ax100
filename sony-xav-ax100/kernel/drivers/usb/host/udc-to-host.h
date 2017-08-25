#ifndef USB_TO_HOST_H_
#define USB_TO_HOST_H_






#include "../core/usb.h"

#define UDC_HCD_MAJOR 		0
#define UDC_HCD_NR_DEVS	 	1

static int udc_hcd_major = UDC_HCD_MAJOR;

#define		SET_PORT_TO_UDC		_IO(0xfe,0)
#define 	SET_PORT_TO_HCD 	_IO(0xfe,1)

#define SET_UPHY2_TO_HCD(addr) do{	int ret;							\
									ret = ioread32(addr + 10);			\
									ret |= (1<<4);						\
									iowrite32(ret,addr + 10);			\
							   }while(0)

#define SET_UPHY2_TO_UDC(addr)	do{	int ret;							\
									ret = ioread32(addr + 10);			\
									ret &= ~(1<<4);						\
									iowrite32(ret,addr + 10);			\
							  	}while(0)

#define     DEGUG_LOG(x)	printk(x)

module_param(udc_hcd_major, int, S_IRUGO);

struct udc_hcd_platform_device {
	struct platform_device *p_ehcd;
	struct platform_device *p_ohcd;
	struct platform_device *p_udc;
	
	int state;

#define		RUN_UDC_OHCI_S				 (1<<0)
#define		RUN_UDC_EHCI_S				 (1<<1)

#define		RUN_UDC_S				 	 (RUN_UDC_OHCI_S|RUN_UDC_EHCI_S)
#define		RUN_HCD_S				 	 (1<<2)

#define		SET_TO_UDC_OHCI(state)		 do{							\
											state &= ~RUN_HCD_S; 		\
											state |=  RUN_UDC_OHCI_S;	\
										 }while(0)
											
#define		SET_TO_UDC_EHCI(state)		 do{							\
											state &= ~RUN_HCD_S; 		\
											state |=  RUN_UDC_EHCI_S;	\
										 }while(0)


#define		SET_TO_UDC(state)		 	 (state = RUN_UDC_S & ~RUN_HCD_S)
#define		SET_TO_HCD(state)		 	 (state = RUN_HCD_S & ~RUN_UDC_S)

#define		IS_UDC_STATE(state)		 	 ((state) & RUN_UDC_S)
#define		IS_HCD_STATE(state)		 	 ((state) & RUN_HCD_S)

};





#endif