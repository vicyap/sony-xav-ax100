#include <linux/types.h>
//#include <common.h>

#include <linux/usb/usb_device.h>
#include <linux/usb/ch9.h>

//config
#define DEBUG_MSG_CMD
//#define DEBUG_MSG
#define UBOOT

#ifdef DEBUG_MSG
#ifdef UBOOT
#define udc_prnstr(fmt, arg...)	do { printf(fmt,##arg); } while (0)
#define udc_prndword(x)	do { printf("%x\n",x); } while (0)
#define udc_prnbyte(x)	do { printf("%x\n",x); } while (0)
#else
#define udc_prnstr(fmt, arg...)	do { printk(KERN_DEBUG fmt,##arg); } while (0)
#define udc_prndword(x)	do { printk("%x\n",x); } while (0)
#define udc_prnbyte(x)	do { printk("%x\n",x); } while (0)
#define prn_string(x)	do { printk(x); } while (0)
#define prn_dword(x)	do { printk("%x\n",x); } while (0)
#endif

#else 
#define udc_prnstr(x) 
#define udc_prndword(x)	
#define udc_prnbyte(x)	
#endif
#define prn_string(x)	do { printf(x); } while (0)
#define prn_dword(x)	do { printf("%x\n",x); } while (0)

#define USB_DEVICE_NOT_SUPPORT_STRING 
#define USBD1	0x9c103800
#define USBD0	0x9c102800

#ifdef UBOOT
#define USBD_BASE	USBD0
#else
static __iomem USBD_BASE;
#endif

static const char *reqname(unsigned r)
{
	switch (r) {
	case USB_REQ_GET_STATUS: return "GET_STATUS";
	case USB_REQ_CLEAR_FEATURE: return "CLEAR_FEATURE";
	case USB_REQ_SET_FEATURE: return "SET_FEATURE";
	case USB_REQ_SET_ADDRESS: return "SET_ADDRESS";
	case USB_REQ_GET_DESCRIPTOR: return "GET_DESCRIPTOR";
	case USB_REQ_SET_DESCRIPTOR: return "SET_DESCRIPTOR";
	case USB_REQ_GET_CONFIGURATION: return "GET_CONFIGURATION";
	case USB_REQ_SET_CONFIGURATION: return "SET_CONFIGURATION";
	case USB_REQ_GET_INTERFACE: return "GET_INTERFACE";
	case USB_REQ_SET_INTERFACE: return "SET_INTERFACE";
	default: return "*UNKNOWN*";
	}
}
static const char *cmdname(u32 r)
{
	switch (r) {
	case ACT_READ: 	return "read";
	case ACT_WRITE: return "write";
	case ACT_EXEC: 	return "exec";
	case ACT_QUIT:	return "QUIT";
	default: return "*undefine*";
	}
}

static const u8 mp01[9] = { 0x01, 0x0A, 0x00, 0x83, 0x00, 0x00, 0x00, 0x00, 0x80 };
static const u8 mp1C[4] = { 0x1C, 0x06, 0x00, 0x05 };
static const u8 mp05[30]= { 0x05, 0x1E, 0x13, 0x88, 0x08, 0x20, 0x02, 0x00,
		     0x01, 0xF4, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		     0x00, 0x00, 0x00, 0x05, 0x1E, 0x00, 0x00, 0x00,
				0x00, 0x00, 0x00, 0x00, 0x01, 0x68
			      };

const u8 USB_MSDC_DeviceDescriptor[] =
{
	/*-------------------------------------------------------------------
	/TotalLength = 0x0012 u8
	/-------------------------------------------------------------------*/

	/*Device (0x12 u8)*/
	0x12,                   /*bLength: 0x12 u8*/
	0x01,                   /*bDescriptorType: DEVICE*/
	0x00, 0x02,             /*bcdUSB: version 2.00*/
	0x00,                   /*bDeviceClass: independent interfaces*/
	0x00,                   /*bDeviceSubClass: 0*/
	0x00,                   /*bDeviceProtocol: class specific protocols NOT used on device basis*/
	0x40,                   /*bMaxPacketSize0: maximum packet size for endpoint zero*/
	0xfc, 0x04,             /*idVendor: vendor ID*/
	0x02, 0x55,             /*idProduct: product ID*/
	0x00, 0x03,             /*bcdDevice: device release number in BCD*/
#ifdef USB_DEVICE_NOT_SUPPORT_STRING
	0,0,0,
#else
	0x01,                   /*iManufacturer: index of string*/
	0x02,                   /*iProduct: index of string*/
	0x03,                   /*iSerialNumber: index of string*/
#endif
	0x01,                   /*bNumConfigurations: 1 configuration*/
};

const u8   USB_MSDC_ConfigurationDescriptor[USB_MSDC_CONFIGURATION_DESCRIPTOR_SIZE] =
{
	/*-------------------------------------------------------------------*/
	/*SubLength = 0x0009 u8 */
	/*-------------------------------------------------------------------*/

	/*Configuration (0x09 u8)*/
	0x09,                   /*bLength: 0x09 u8*/
	0x02,                   /*bDescriptorType: CONFIGURATION*/
	USB_MSDC_CONFIGURATION_DESCRIPTOR_SIZE, 0x00,             /*wTotalLength:*/
	0x01,                   /*bNumInterfaces: 1 interfaces*/
	0x01,                   /*bConfigurationValue: configuration 1*/
	0x00,                   /*iConfiguration: index of string*/
	0xc0,           /*bmAttributes: bus powered b7:bus power ,b6 self power,b5 remote wake,sjh*/
	0x32,/*0xFA,*/                   /*MaxPower: 500 mA*/

	/*-------------------------------------------------------------------*/
	/*SubLength = 0x001e u8*/
	/*-------------------------------------------------------------------*/

	/*Interface 1 (0x09 u8)*/
	0x09,                   /*bLength: 0x09 u8*/
	0x04,                   /*bDescriptorType: INTERFACE*/
	0x00,                   /*bInterfaceNumber: interface 0*/
	0x00,                   /*bAlternateSetting: alternate setting 0*/
	0x02,                   /*bNumEndpoints: 3 endpoints*/
	0x08,                   /*bInterfaceClass: Mass Storage Devices Class*/
	0x06,                   /*bInterfaceSubClass:*/
	0x50,			 /*Bulk Only =50*/
	0x00,                   /*iInterface: index of string*/

	/*Endpoint 1 (0x07 u8)*/
	0x07,                   /*bLength: 0x07 u8*/
	0x05,                   /*DescriptorType: ENDPOINT*/
	(USB_MSDC_IN_EP|0x80),                   /*bEndpointAddress: IN endpoint 1*/
	0x02,                   /*bmAttributes: Bulk*/
	0x00, 0x02,             /*wMaxPacketSize: 512 u8*/
	/*Hight Speed max packet once transfer is 512 u8s*/
	0x00,                   /*bInterval: ignored*/

	/*Endpoint 2 (0x07 u8)*/
	0x07,                   /*bLength: 0x07 u8*/
	0x05,                   /*bDescriptorType: ENDPOINT*/
	USB_MSDC_OUT_EP,                   /*bEndpointAddress: OUT endpoint 2*/
	0x02,                   /*bmAttributes: Bulk*/
	0x00, 0x02,             /*wMaxPacketSize: 512 u8*/
	/*Hight Speed max packet once transfer is 512 u8s*/
	0x00,                   /*bInterval: ignored*/
};
const u8   USB_LanguageIdString[] =
{
	/*----------
	//0x04 UINT8s
	//----------*/
	0x04,           /*bLength*/
	0x03,           /*bDescriptorType: STRING*/
	0x09, 0x04,     /*bString: English (US)*/
};
static const  u8 inquiry[LEN_INQUIRY] =
{
	0x00, 0x80, 0x00, 0x02/*0x01*/,  // 00-03
	0x1F, 0x00, 0x00, 0x00,  // 04-07
	'S', 'u', 'n', 'p',  // 08-11 : VID(00:03) : S u n p
	'l', 'u', 's', ' ',  // 12-15 : VID(04:07) : l u s
	'I', 'S', 'P', '1',  // 16-19 : PID(00:03) : 
	' ', ' ', ' ', ' ',  // 20-23 : PID(04:07) : 
	' ', ' ', ' ', ' ',  // 24-27 : PID(08:11) :  
	' ', ' ', ' ', ' ',  // 28-31 : PID(12:15) :  
	0x30, 0x34, 0x30, 0x38  // 32-35 : PRL(00:03) : 0 4 0 7
};
struct sp_cmd_s g_cmd,*p_cmd;
struct msdc_cbw *r_cmd;
struct msdc_cbw _cbw;


u8 g_buf[512];
u32 g_rlen;
u32 g_flag;
u8 g_exit;
static int ep2_handle_(unsigned char *buf_);
u32 phase_cmd(struct msdc_cbw *cbw);

static inline void udc_write_(u32 value, u32 reg)
{
	*((volatile u32 *)(USBD_BASE + reg)) = value;
}
static inline u32 udc_read_(u32 reg)
{
	return *(volatile u32 *)(USBD_BASE + reg);
}

void ep0_write(unsigned char * _buf,int size)
{
	
	u8 *buf = _buf;
	u32 n = 0, m = 0 , i = 0;
	udc_write_(EP_DIR|CLR_EP0_OUT_VLD, UDEP0CS);
	n = size/4;
	m = size%4;

	if(n > 0){
		udc_write_(0xF, UDEP0VB);
		for(i=0;i<n;i++){
			* (volatile u32*)(USBD_BASE + UDEP0DP) = *((u32 *)(buf+(i*4)));
		}
	}

	if(m > 0){
		udc_write_(((1<<m)-1), UDEP0VB);
		* (volatile u32*)(USBD_BASE + UDEP0DP) = *((u32 *)(buf + n*4));
	}

	udc_write_(SET_EP0_IN_VLD | EP_DIR,  UDEP0CS);
}

void Dev_GetDescription(PUSBEp0Container pSetup)
{
	u8* pData = NULL;
	u32 uiSize = 0;
	switch (pSetup->wValue & 0xff00)
	{
		case USB_DEVICE_DESCRIPTOR:
			pData = (u8 *)USB_MSDC_DeviceDescriptor;
			uiSize = 0x12; 
			break;

		case USB_CONFIGURATION_DESCRIPTOR:
			pData = (u8 *)USB_MSDC_ConfigurationDescriptor;
			uiSize = USB_MSDC_CONFIGURATION_DESCRIPTOR_SIZE;
			break;

		case USB_STRING_DESCRIPTOR:
		/*get string descriptor*/
		switch (pSetup->wValue & 0x00ff)
		{
			case 0x0000:    
			/*get language ID string*/
			pData = (u8 *)USB_LanguageIdString;       
			uiSize = USB_LANGUAGE_ID_STRING_SIZE;
			break;

			default:
			uiSize = 0;
			udc_write_(0x1, UDLCSTL);//set ep0 stall
			break;

		}
		break;
		case USB_INTERFACE_DESCRIPTOR:
			pData = (u8 *)(USB_MSDC_ConfigurationDescriptor + 9); 
			uiSize = USB_MSDC_INTERFACE_DESCRIPTOR_SIZE;
			break;
		case USB_ENDPOINT_DESCRIPTOR:
			pData = (u8 *)(USB_MSDC_ConfigurationDescriptor + 9 + \
				USB_MSDC_INTERFACE_DESCRIPTOR_SIZE); 
			uiSize = USB_MSDC_ENDPOINT_DESCRIPTOR_SIZE;
			break;
		default:
			uiSize = 0;
			break;
	}

	if(pSetup->wLength < uiSize)
		uiSize = pSetup->wLength;

	if( (uiSize) && (pData != NULL) ){
		ep0_write(pData,uiSize);
	}
}

u8 build_mode_pages(u8 page_code, u8 *mode_page_data)
{
	u8	*mp;
	u8	tmp8;
	
	mp   = NULL;
	tmp8 = 0;
	
	mp = mode_page_data + 4;
	tmp8 = 4;
	switch (page_code)
	{
		case READ_WRITE_ERROR_RECOVERY_PAGE:
			tmp8 += LEN_01;
			memcpy(mp, mp01, 9);
			break;
		case TIMER_AND_PROTECT_PAGE:
			tmp8 += LEN_1C;
			memcpy(mp, mp1C, 4);
			break;
		case FLEXIBLE_DISK_PAGE:
			tmp8 += LEN_05;
			memcpy(mp, mp05, 30);
			break;
		case ALL_PAGES:
			tmp8 += LEN_3F;
			
			memcpy(mp, mp01, 9);
			mp += LEN_01;

			memcpy(mp, mp05, 30);
			mp += LEN_05;
			memcpy(mp, mp1C, 4);
			mp += LEN_1C;
			break;
	}
	// DEBUG_MSG0("page_code %d tmp8=%d\n",page_code,tmp8);

	return tmp8;
}

static void letb123_udc_enable(void)
{
	
	/* usb device controller interrupt flag */
	udc_write_(udc_read_(UDCIF) & 0xFFFF, UDCIF);
	/* usb device link layer interrupt flag */
	udc_write_(0xefffffff, UDLIF);

	udc_write_(VBUS_IF , UDCIE); 
	udc_write_(EP0S_IF | RESET_IF , UDLIE);

	udc_write_(udc_read_(UDLIE) | SUS_IF, UDLIE);
	udc_write_(udc_read_(UDCCS) | UPHY_CLK_CSS, UDCCS);/*PREVENT SUSP*/

	udc_write_((udc_read_(UDLCSET)|8 )& 0xFE,  UDLCSET);
	
	udc_write_(EP_DIR|EP_ENA|RESET_PIPO_FIFO, UDEP12C);
	udc_write_(EP_ENA, UDEP12C);

	prn_string("<<< letb123_udc_enable ip version ");
	prn_dword(udc_read_(0x37c));
	//prn_dword(udc_read_(0));prn_dword(udc_read_(0x80));
}

static inline int letb123_udc_get_ep_fifo_count(u32 ping_base,int is_pingbuf)
{
	int tmp = 0;

	if(is_pingbuf){
		tmp = udc_read_(ping_base) & 0x3FF;
	}else{
		tmp = udc_read_(ping_base+4) & 0x3FF;
	}

	return tmp;
}
static inline int letb123_udc_read_fifo_packet(int fifo, u8 *buf, int length, int data_vb)
{
	int n = 0, m = 0, i = 0;
	u32 testbuf;
	n = length/4;
	m = length%4;
	udc_write_(0xF,data_vb);
	for(i=0;i<n;i++){
		*((u32 *)(buf+(i*4))) =* (volatile u32*)(USBD_BASE + fifo);
	}
	if(m>0){
		udc_write_( ((1<< m) -1), data_vb);
		testbuf =* (volatile u32*)(USBD_BASE + fifo);
		memcpy((u8 *)(buf+(i*4)),(u8 *)(&testbuf),m);
	}
	return length;
}

static void letb123_udc_readep0s_fifo_crq(unsigned char *buf)
{
	*((u32 *)buf)= * (volatile u32*)(UDEP0SDP + USBD_BASE) ;
	*((u32 *)(buf+4))= * (volatile u32*)(UDEP0SDP + USBD_BASE) ;
}

static inline int letb123_udc_write_packet(int fifo,char *buf,int len,int offset)
{
	u32 n = 0, m= 0 , i=0;
	n = len/4;
	m = len%4;

	if(n > 0){
		udc_write_(0xF, offset);
		for(i=0;i<n;i++){
			* (volatile u32*)(USBD_BASE + fifo) = *((u32 *)(buf+(i*4)));
		}
	}

	if(m > 0){
		udc_write_(((1<< m) -1), offset);
		* (volatile u32*)(USBD_BASE + fifo) = *((u32 *)(buf+(n*4)));
	}

	return len;
}

static void clearHwState_UDC_(void)
{
	//EP0 control status
	udc_write_(CLR_EP0_OUT_VLD, UDEP0CS); //clear ep0 out vld=1, clear set epo in vld=0, set ctl dir to OUT direction=0
	udc_write_(0x0,UDEP0CS);
	//EP2
	udc_write_(CLR_EP_OVLD | RESET_PIPO_FIFO|udc_read_(UDEP12C),UDEP12C);

	/*Clear Stall Status*/
	udc_write_(( CLREP2STL | CLREP1STL | CLREP0STL),UDLCSTL);
}

static int letb123_set_up(unsigned char *buf)
{
	struct _USBEp0Container* crq = (struct _USBEp0Container*)buf;
	unsigned char cmd = crq->bRequest;
	//DEBUG_MSG0("letb123_set_up:cmd=%d\n",cmd);
	if(cmd < USB_STDREQ_SYNCH_FRAME){
		if(cmd == 6){/**/
			Dev_GetDescription(crq);
		}else{
			if(cmd !=5 )
				ep0_write(buf,0);
			#ifdef DEBUG_MSG
			printf("StdReqFunc[%d] is null\n",cmd);
			#endif
		}
	}else if(cmd == 0xfe && crq->bmRequestType == 0xA1){
		buf[0] = 0;
		ep0_write(buf,1);
	}
	return 0;
}

static void letb123_udc_handle_ep0s_idle(unsigned char *buf)
{
	int ret;
	struct usb_ctrlrequest *r = (struct usb_ctrlrequest *)buf;
	udc_prnstr(">>>letb123_udc_handle_ep0s_idle...\n");
	
	letb123_udc_readep0s_fifo_crq(buf);
	#ifdef DEBUG_MSG
	printf("handle setup %s, %x, %x index %x value %x\n", reqname(r->bRequest),
	    r->bRequestType, r->bRequest, r->wIndex, r->wValue);
	#endif
	/* deliver the request to the gadget driver */
	ret = letb123_set_up(buf);
	if (ret < 0) {
		udc_write_(0x1, UDLCSTL);//set ep0 stall
	} 
}
#if 0
void delay_udc(u32 period) /*1/90 ms*/
{
#ifndef CSIM_NEW
	// STC Timer 90K/s
	//prn_string("@@@");
	u32 delay = (period);
	STC_REG->stc_15_0 = 0;
	while (STC_REG->stc_15_0 < delay);
#endif
}
#endif

void ep1_bulkin(u8 *buf,int len)
{
	//u8 i=0;
	//udc_write_(0x0, 0X334);
	//udc_write_(0x7, UDEP12C);/*DIR for bulk in*/
	udc_write_(EP1I_IF,UDLIF);
	if((udc_read_(UDEP12FS)&0x01)==0){
		letb123_udc_write_packet(UDEP12FDP,(char *) buf, len, UDEP12VB);
		udc_write_(SET_EP_IVLD | udc_read_(UDEP12C), UDEP12C);
		//udc_write_(0x2, 0X334);
	}
	udc_prnstr("[ep1 count=");
	udc_prndword(len);
	//delay_udc(30);
	//while((!udc_read_(UDC_LLCIF_OFST))&UDLC_EP1I_IF){
	while((udc_read_(UDEP12FS) & 0x11) ||((!udc_read_(UDLIF))&EP1I_IF)){
		//udelay(100);
		#ifdef DEBUG_MSG
		printf("*%d&%x:%x",udc_read_(UDEP12FS),letb123_udc_get_ep_fifo_count(UDEP12PIC,1),letb123_udc_get_ep_fifo_count(UDEP12PIC,0));
		#endif
		if(udc_read_(UDLIF)&EP1I_IF)
			break;
		//if(i++>9)
		//	break;
	}
	//while(letb123_udc_get_ep12_fifo_count(((udc_read_(UDEP12PPC) & 0x04) ? 1:0))){
	//	printf("ep1 fifo count=");
	//	prn_dword(letb123_udc_get_ep12_fifo_count(((udc_read_(UDEP12PPC) & 0x04) ? 1:0)));
	//}
		
	udc_prnstr("]\n");
	//udc_write_(0x1, 0X334);
	//udc_write_(0x2, UDC_EP12C_OFST);
}
static int ep2_handle_(u8 *buf_)
{
	unsigned char *buf = buf_;
	char is_pingbuf;
	int count;
	while(!(udc_read_(UDEP2FS) & EP_OVLD)){
		udc_prnstr("ep2 fs ");
		udc_prndword(udc_read_(UDEP2FS));
	}
	is_pingbuf = (udc_read_(UDEP2PPC) & 0x04) ? 1:0 ;
	count = letb123_udc_get_ep_fifo_count(UDEP2PIC,is_pingbuf);
	letb123_udc_read_fifo_packet(UDEP2FDP,buf,count,UDEP2VB);
	udc_write_(udc_read_(UDEP12C)|CLR_EP_OVLD, UDEP12C);
	udc_prnstr("ep2 count ");
	udc_prndword(count);
	if(count == 31 && (((struct msdc_cbw *)buf)->dCBWSignature == CBWSignature)){
		phase_cmd(r_cmd);
	}
	return 0;
}

static void sp_cmd(u8 * buf)
{
	p_cmd = (struct sp_cmd_s *)buf;
	g_cmd.addr = p_cmd->addr;
	g_cmd.len = (p_cmd->len + 511)/512;
	g_rlen = p_cmd->len%512;
	g_cmd.action = p_cmd->action;
	g_flag = 0;
	if(g_cmd.action & ACT_EXEC){
		// TODO: jump to ADDR to exec
		prn_string("will jump add exec addr\n");
	}
	if(g_cmd.action & ACT_QUIT)
		g_exit = USBD_EXIT_FLAG; /*will exit UDC*/
	#ifdef DEBUG_MSG_CMD
	printf("action:%s \n",cmdname(g_cmd.action));
	prn_string("addr: ");
	prn_dword(g_cmd.addr);
	prn_string("len: ");
	if(g_cmd.len >= 512){
		printf("%d Bytes (%d KB)\n",(g_cmd.len<<9),(g_cmd.len)>>1);
	}else{
		printf("%d Bytes\n",g_rlen);
	}
	#endif
}
static void scsi_status(unsigned int dCBWTag)
{
	struct	_USBMSDC_CSW pScsiCSW;
	pScsiCSW.dCswSignature = CSWSignature;
	pScsiCSW.dCsWTag = dCBWTag;
	pScsiCSW.dCsWDataResidue =0;
	pScsiCSW.bCsWStatus = 0;
	ep1_bulkin((u8 *)(&pScsiCSW),0x0d);
}
u32 phase_cmd(struct msdc_cbw *cbw)
{
	u8 bOperCode = cbw->CBWCB[0];
	u32 i = 0;
	u32 gdwLba = 0;
	u32 gdwLen = 0;
	
	if(cbw->dCBWSignature != CBWSignature){
		prn_string("CBWSignature err!\n");
		return 0;
	}

	udc_prnstr("phase_cmd bOperCode ");
	udc_prnbyte(bOperCode);
	switch (bOperCode){
		case SCSI_PREVENT_ALLOW_MEDIUM_REMOVAL:
		case SCSI_VERIFY_10:
			scsi_status(cbw->dCBWTag);
			break;
		case SCSI_TEST_UNIT_READY:
			scsi_status(cbw->dCBWTag);
			break;
		case SCSI_READ_10:
			gdwLba = GET_UINT32_B(cbw->CBWCB + 2);
			gdwLen = ((u32)GET_UINT16_B(cbw->CBWCB + 7)) << 9;
			//sp_data_init(g_buf);
			udc_prnstr("READ:lba len ");
			udc_prndword(gdwLba);
			udc_prndword(gdwLen);
			if(!gdwLba|| !g_cmd.len){
				;//DEBUG_MSG0("\t \tread note g_cmd.len=%d len=%d\n",g_cmd.len , gdwLen);
		    	}
		    while(gdwLen){
			if(!gdwLba || !g_cmd.len){
				ep1_bulkin(g_buf,512);
			}else{
				//ep1_bulkin(g_buf+i%8,512);
				if(g_rlen && g_cmd.len == g_flag+1){
					#ifdef DEBUG_MSG
					prn_string("\trrrr------len= ");
					prn_dword(g_rlen);
					#endif
					memcpy(g_buf,(u8 *)(g_cmd.addr+g_flag*512),g_rlen);
					memset(g_buf+g_rlen,0x00,512-g_rlen);
					ep1_bulkin(g_buf,512);
				}else{
					ep1_bulkin((u8*)(g_cmd.addr+g_flag*512),512);
					 prn_string("read 512");
					 prn_dword(gdwLen);
				}
				g_flag++;
			}
			gdwLen -= 512;
			i++;
			
		    }
		     if(g_flag == g_cmd.len)
				g_cmd.len = 0;
		     udc_prnstr("\t read g_flag=");
		     udc_prndword(g_flag);
			scsi_status(cbw->dCBWTag);
		    break;
		case SCSI_READ_CAPACITY_10:
			{u8 UsbDevBuffer[8];
			memset(UsbDevBuffer, 0, 8);
			UsbDevBuffer[0x00] = 0x00;
			UsbDevBuffer[0x01] = 0x00;
			UsbDevBuffer[0x02] = 0x87;
			UsbDevBuffer[0x03] = 0x00;
			
			UsbDevBuffer[0x06] = 0x02;
			ep1_bulkin(UsbDevBuffer,8);
			scsi_status(cbw->dCBWTag);
			}
		    break;
		case SCSI_READ_FORMAT_CAPACITIES:
			{u8 UsbDevBuffer[12];
			memset(UsbDevBuffer, 0, 20);
			UsbDevBuffer[0x03] = 0x08;
			
			UsbDevBuffer[0x04] = 0x00;
			UsbDevBuffer[0x05] = 0x00;
			UsbDevBuffer[0x06] = 0x87;
			UsbDevBuffer[0x07] = 0x00;
			
			UsbDevBuffer[0x08] = 0x02; // descriptor type: formatted media
			UsbDevBuffer[0x0A] = 0x02; // block length in UINT8 is 512
			ep1_bulkin(UsbDevBuffer,12);
			
			scsi_status(cbw->dCBWTag);
			}
		    break;
		case SCSI_MODE_SENSE_6:
			{
			u8 UsbDevBuffer[64];
			    memset(UsbDevBuffer, 0, 4);
			    UsbDevBuffer[0] = build_mode_pages(cbw->CBWCB[2] & 0x3F, UsbDevBuffer);
			    UsbDevBuffer[0] = MIN(UsbDevBuffer[0], cbw->CBWCB[2]);
			    if(UsbDevBuffer[0] > cbw->CBWCB[4])
			    	UsbDevBuffer[0] = cbw->CBWCB[4];
			    ep1_bulkin(UsbDevBuffer,UsbDevBuffer[0]);
		    
			scsi_status(cbw->dCBWTag);
			}
		    break;
		case SCSI_INQUIRY:
			ep1_bulkin((u8 *)inquiry,0x24);
			scsi_status(cbw->dCBWTag);
			break;
		case SCSI_WRITE_10:
		    {gdwLba = GET_UINT32_B(cbw->CBWCB+2);
		    gdwLen = ((u32)GET_UINT16_B(cbw->CBWCB+7)) << 9;
		    udc_prnstr("WRITE:lba len ");
		    udc_prndword(gdwLba);
		    udc_prndword(gdwLen);
		    while(gdwLen){
			while(!(udc_read_(UDLIF)|EP2O_IF)){
				if(udc_read_(UDEP2FS)&0x2){
					prn_string("error!\n");
					break;
				}
			}
			ep2_handle_(g_buf);
			if(!i && gdwLba==1 && !g_cmd.len/*&& (g_buf[13]==0x73)*/)
				sp_cmd(g_buf);
			else if(g_cmd.action== ACT_WRITE){
				//sp_data_check(g_buf);
				if(g_cmd.len == g_flag + 1){
					//DEBUG_MSG0("\trrrr------len=%d\n",g_rlen);
					#if 0 //read reg debug
					if(g_rlen == 4)
						printf("data 4 ==%x\n",*(u32*)g_buf);
					#endif
				}
				if(g_rlen && g_flag+1 == g_cmd.len){
					memcpy((u8 *)(g_cmd.addr+g_flag*512),g_buf,g_rlen);
					printf("write end\n");
				}else{
					memcpy((u8 *)(g_cmd.addr+g_flag*512),g_buf,512);
				}
				//DEBUG_MSG0("data rcv %d\n",g_flag);
				g_flag++;
			}
			gdwLen -= 512;
			i++;
			
		    }
		    //DEBUG_MSG0("data rcv %d\n",g_flag);
		    if(g_flag == g_cmd.len)
			g_cmd.len = 0;
			scsi_status(cbw->dCBWTag);
			}
		    break;
		default:
			prn_string("WRITE:lba len ");
			prn_dword(gdwLba);
	    }
	return 0;
}
int handle_msg(void)
{
	u32 udl_if;
	unsigned char buf[8];
	udl_if = udc_read_(UDLIF);
	if(udl_if & RESET_IF){
		udc_prnstr("IRQ:UDC reset IRQ\n");
		g_exit = 1;
		udc_write_(RESET_IF, UDLIF);
		/*Allow LNK to suspend PHY*/
		udc_write_((udc_read_(UDLCSET)|8 )& 0xFE,  UDLCSET);
		udc_write_(udc_read_(UDCCS) &(~UPHY_CLK_CSS), UDCCS);
		clearHwState_UDC_();
	}
	if (udl_if & EP0S_IF){
		udc_write_(EP0S_IF , UDLIF);
		udc_prnstr("IRQ:EP0 setup int\n");
		if((udc_read_(UDEP0CS)&(EP0_OVLD|EP0_OUT_EMPTY)) == (EP0_OVLD|EP0_OUT_EMPTY))
			udc_write_(udc_read_(UDEP0CS)|CLR_EP0_OUT_VLD,UDEP0CS);
		letb123_udc_handle_ep0s_idle(buf);
	}else if(udl_if & EP0I_IF){
		udc_write_(EP0I_IF, UDLIF);
		udc_prnstr("IRQ:EP0 in int\n");
		udc_write_(udc_read_(UDEP0CS) & ~(EP_DIR), UDEP0CS);
	}else if(udl_if & EP2O_IF){
		udc_prnstr("IRQ:ep2 Bulk Out Ack interrupt ");
		udc_write_(EP2O_IF,UDLIF);
		if(udc_read_(UDEP2FS) & EP_OVLD)
			ep2_handle_((u8 *)r_cmd);
	}
	return 0;
}
int recv_exit()
{
	return g_exit == USBD_EXIT_FLAG;
}
int sp_fast_register(void)
{
	//struct msdc_cbw _cbw;	
	*((volatile u32 *)(0x9c00010c)) = 0x1010;
	prn_string("\n USBD_BASE:\n");
	prn_dword((u32)USBD_BASE);
	letb123_udc_enable();
	r_cmd = &_cbw;
	
	return 0;
}

int letb123_udc_probe(void)
{
	struct msdc_cbw _cbw;
	
	//USBH0_SYS_REG->uhpowercs_port &= ~(1 << 10);
	//USBH0_SYS_REG->uhpowercs_port |= (1 << 8);

	//uphy_init();
	//MOON2_REG->sft_cfg_3 &=(~ (3<<5));/*device*/
	//prn_string("\n MOON2_REG->sft_cfg_3:\n");
	//prn_dword(MOON2_REG->sft_cfg_3);
	//EHCI_USBCMD |= EHCI_SFT_RESET;
	#ifndef UBOOT
	USBD_BASE = base_addr;
	#endif
	prn_string("\n USBD_BASE:\n");
	prn_dword((u32)USBD_BASE);
	letb123_udc_enable();
	r_cmd = &_cbw;
	
	//STC_REG->stc_15_0 = 0;
	while(g_exit != USBD_EXIT_FLAG){
		handle_msg();		
		//if((!g_exit) && (STC_REG->stc_15_0 > EXIT_TIME)){
		//	prn_string("\n UDC exit :timeout\n");
		//	return -1;
		//}
	}
	prn_string("\n UDC exit \n");
	return 0;
}

