/*
 * Copyright 2011, Marvell Semiconductor Inc.
 * Lei Wen <leiwen@marvell.com>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 *
 * Back ported to the 8xx platform (from the 8260 platform) by
 * Murray.Jensen@cmst.csiro.au, 27-Jan-01.
 */

#include <common.h>
#include <command.h>
#include <config.h>
#include <net.h>
#include <malloc.h>
#include <asm/byteorder.h>
#include <asm/errno.h>
#include <asm/io.h>
#include <asm/unaligned.h>
#include <linux/types.h>
#include <linux/usb/ch9.h>
#include <linux/usb/gadget.h>
#include <usb/sp_udc.h>
#include "../host/ehci.h"
#include "sp_udc.h"

/*
 * Check if the system has too long cachelines. If the cachelines are
 * longer then 128b, the driver will not be able flush/invalidate data
 * cache over separate QH entries. We use 128b because one QH entry is
 * 64b long and there are always two QH list entries for each endpoint.
 */
#if ARCH_DMA_MINALIGN > 128
#error This driver can not work on systems with caches longer than 128b
#endif
#define DEBUG
#ifndef DEBUG
#define DBG(x...) do {} while (0)
#else
#define DBG(x...) printf(x)
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
#endif

static struct usb_endpoint_descriptor ep0_out_desc = {
	.bLength = sizeof(struct usb_endpoint_descriptor),
	.bDescriptorType = USB_DT_ENDPOINT,
	.bEndpointAddress = 0,
	.bmAttributes =	USB_ENDPOINT_XFER_CONTROL,
};

static struct usb_endpoint_descriptor ep0_in_desc = {
	.bLength = sizeof(struct usb_endpoint_descriptor),
	.bDescriptorType = USB_DT_ENDPOINT,
	.bEndpointAddress = USB_DIR_IN,
	.bmAttributes =	USB_ENDPOINT_XFER_CONTROL,
};

static int sp_pullup(struct usb_gadget *gadget, int is_on);
static int sp_ep_enable(struct usb_ep *ep,
		const struct usb_endpoint_descriptor *desc);
static int sp_ep_disable(struct usb_ep *ep);
static int sp_ep_queue(struct usb_ep *ep,
		struct usb_request *req, gfp_t gfp_flags);
static struct usb_request *
sp_ep_alloc_request(struct usb_ep *ep, unsigned int gfp_flags);
static void sp_ep_free_request(struct usb_ep *ep, struct usb_request *_req);

static struct usb_gadget_ops sp_udc_ops = {
	.pullup = sp_pullup,
};

static struct usb_ep_ops sp_ep_ops = {
	.enable         = sp_ep_enable,
	.disable        = sp_ep_disable,
	.queue          = sp_ep_queue,
	.alloc_request  = sp_ep_alloc_request,
	.free_request   = sp_ep_free_request,
};

/* Init values for USB endpoints. */
static const struct usb_ep sp_ep_init[] = {
	[0] = {	/* EP 0 */
		.maxpacket	= 64,
		.name		= "ep0",
		.ops		= &sp_ep_ops,
	},
	[1] = {	/* EP 1*/
		.maxpacket	= 512,
		.name		= "ep1-bulkin",
		.ops		= &sp_ep_ops,
	},
	[2] = {	/* EP 2 */
		.maxpacket	= 512,
		.name		= "ep2-bulkout",
		.ops		= &sp_ep_ops,
	},
};

static struct sp_drv controller = {
	.gadget	= {
		.name	= "sp_udc",
		.ops	= &sp_udc_ops,
		//.is_dualspeed = 1,
		.speed = USB_SPEED_HIGH,
	},
};
static inline u32 udc_read(u32 reg)
{
	return readl(AHB_USBD_BASE + reg);
}

static inline void udc_write(u32 value, u32 reg)
{
	writel(value, AHB_USBD_BASE + reg);
}
void ep0_write(unsigned char * _buf,int size)
{
	
	u8 *buf = _buf;
	u32 n = 0, m = 0 , i = 0;
	udc_write(EP_DIR|CLR_EP0_OUT_VLD, UDEP0CS);
	n = size/4;
	m = size%4;

	if(n > 0){
		udc_write(0xF, UDEP0VB);
		for(i=0;i<n;i++){
			* (volatile u32*)(AHB_USBD_BASE + UDEP0DP) = *((u32 *)(buf+(i*4)));
		}
	}

	if(m > 0){
		udc_write(((1<<m)-1), UDEP0VB);
		* (volatile u32*)(AHB_USBD_BASE + UDEP0DP) = *((u32 *)(buf + n*4));
	}

	udc_write(SET_EP0_IN_VLD | EP_DIR,  UDEP0CS);
}
static inline int sp_udc_read_fifo_packet(int fifo, u8 *buf, int length, int data_vb)
{
	int n = 0, m = 0, i = 0;
	u32 testbuf;
	n = length/4;
	m = length%4;
	udc_write(0xF,data_vb);
	for(i=0;i<n;i++){
		*((u32 *)(buf+(i*4))) =* (volatile u32*)(AHB_USBD_BASE + fifo);
	}
	if(m>0){
		udc_write( ((1<< m) -1), data_vb);
		testbuf =* (volatile u32*)(AHB_USBD_BASE + fifo);
		memcpy((u8 *)(buf+(i*4)),(u8 *)(&testbuf),m);
	}
	return length;
}
static inline int sp_udc_get_ep_fifo_count(u32 ping_base,int is_pingbuf)
{
	int tmp = 0;

	if(is_pingbuf){
		tmp = udc_read(ping_base) & 0x3FF;
	}else{
		tmp = udc_read(ping_base+4) & 0x3FF;
	}

	return tmp;
}

/**
 * sp_get_qh() - return queue head for endpoint
 * @ep_num:	Endpoint number
 * @dir_in:	Direction of the endpoint (IN = 1, OUT = 0)
 *
 * This function returns the QH associated with particular endpoint
 * and it's direction.
 */
static struct ept_queue_head *sp_get_qh(int ep_num, int dir_in)
{
	return &controller.epts[(ep_num * 2) + dir_in];
}

/**
 * sp_get_qtd() - return queue item for endpoint
 * @ep_num:	Endpoint number
 * @dir_in:	Direction of the endpoint (IN = 1, OUT = 0)
 *
 * This function returns the QH associated with particular endpoint
 * and it's direction.
 */
static struct ept_queue_item *sp_get_qtd(int ep_num, int dir_in)
{
	return controller.items[(ep_num * 2) + dir_in];
}

/**
 * sp_flush_qh - flush cache over queue head
 * @ep_num:	Endpoint number
 *
 * This function flushes cache over QH for particular endpoint.
 */
static void sp_flush_qh(int ep_num)
{
	struct ept_queue_head *head = sp_get_qh(ep_num, 0);
	const uint32_t start = (uint32_t)head;
	const uint32_t end = start + 2 * sizeof(*head);

	flush_dcache_range(start, end);
}

/**
 * sp_invalidate_qh - invalidate cache over queue head
 * @ep_num:	Endpoint number
 *
 * This function invalidates cache over QH for particular endpoint.
 */
static void sp_invalidate_qh(int ep_num)
{
	struct ept_queue_head *head = sp_get_qh(ep_num, 0);
	uint32_t start = (uint32_t)head;
	uint32_t end = start + 2 * sizeof(*head);

	invalidate_dcache_range(start, end);
}

/**
 * sp_flush_qtd - flush cache over queue item
 * @ep_num:	Endpoint number
 *
 * This function flushes cache over qTD pair for particular endpoint.
 */
static void sp_flush_qtd(int ep_num)
{
	struct ept_queue_item *item = sp_get_qtd(ep_num, 0);
	const uint32_t start = (uint32_t)item;
	const uint32_t end_raw = start + 2 * sizeof(*item);
	const uint32_t end = roundup(end_raw, ARCH_DMA_MINALIGN);

	flush_dcache_range(start, end);
}

/**
 * sp_invalidate_qtd - invalidate cache over queue item
 * @ep_num:	Endpoint number
 *
 * This function invalidates cache over qTD pair for particular endpoint.
 */
static void sp_invalidate_qtd(int ep_num)
{
	struct ept_queue_item *item = sp_get_qtd(ep_num, 0);
	const uint32_t start = (uint32_t)item;
	const uint32_t end_raw = start + 2 * sizeof(*item);
	const uint32_t end = roundup(end_raw, ARCH_DMA_MINALIGN);

	invalidate_dcache_range(start, end);
}

static struct usb_request *
sp_ep_alloc_request(struct usb_ep *ep, unsigned int gfp_flags)
{
	struct sp_ep *sp_ep = container_of(ep, struct sp_ep, ep);
	return &sp_ep->req;
}

static void sp_ep_free_request(struct usb_ep *ep, struct usb_request *_req)
{
	return;
}

static void ep_enable(int num, int in, int maxpacket)
{
	struct sp_udc *udc = (struct sp_udc *)controller.ctrl->hcor;
	
	uint udll_int_en = udc_read(UDLIE);
	
	switch (num)
	{
	case 0:
		/* enable irqs */
		udll_int_en |= EP0S_IF;
	 	break;
	case 1:
		udll_int_en |= EP1I_IF;
		break;
	case 2:
		udll_int_en |= EP2O_IF;
		break;
	defual:
		return -EINVAL;
	}
	udc_write(EP_DIR|EP_ENA|RESET_PIPO_FIFO, UDEP12C);
	udc_write(EP_ENA, UDEP12C);
	printf("%s: %d\n", __func__,__LINE__);
}

static int sp_ep_enable(struct usb_ep *ep,
		const struct usb_endpoint_descriptor *desc)
{
	struct sp_ep *sp_ep = container_of(ep, struct sp_ep, ep);
	int num, in;
	num = desc->bEndpointAddress & USB_ENDPOINT_NUMBER_MASK;
	in = (desc->bEndpointAddress & USB_DIR_IN) != 0;
	sp_ep->desc = desc;

	if (num) {
		int max = get_unaligned_le16(&desc->wMaxPacketSize);

		//if ((max > 64) && (controller.gadget.speed == USB_SPEED_FULL))
		//	max = 64;
		if (ep->maxpacket != max) {
			DBG("%s: from %d to %d\n", __func__,
			    ep->maxpacket, max);
			ep->maxpacket = max;
		}
	}
	ep_enable(num, in, ep->maxpacket);
	DBG("%s: num=%d maxpacket=%d\n", __func__, num, ep->maxpacket);
	return 0;
}

static int sp_ep_disable(struct usb_ep *ep)
{
	struct sp_ep *sp_ep = container_of(ep, struct sp_ep, ep);

	sp_ep->desc = NULL;
	return 0;
}

static int sp_bounce(struct sp_ep *ep, int in)
{
	uint32_t addr = (uint32_t)ep->req.buf;
	uint32_t ba;

	/* Input buffer address is not aligned. */
	if (addr & (ARCH_DMA_MINALIGN - 1))
		goto align;

	/* Input buffer length is not aligned. */
	if (ep->req.length & (ARCH_DMA_MINALIGN - 1))
		goto align;

	/* The buffer is well aligned, only flush cache. */
	ep->b_len = ep->req.length;
	ep->b_buf = ep->req.buf;
	goto flush;

align:
	/* Use internal buffer for small payloads. */
	if (ep->req.length <= 64) {
		ep->b_len = 64;
		ep->b_buf = ep->b_fast;
	} else {
		ep->b_len = roundup(ep->req.length, ARCH_DMA_MINALIGN);
		ep->b_buf = memalign(ARCH_DMA_MINALIGN, ep->b_len);
		if (!ep->b_buf)
			return -ENOMEM;
	}
	if (in)
		memcpy(ep->b_buf, ep->req.buf, ep->req.length);

flush:
	ba = (uint32_t)ep->b_buf;
	flush_dcache_range(ba, ba + ep->b_len);

	return 0;
}

static void sp_debounce(struct sp_ep *ep, int in)
{
	uint32_t addr = (uint32_t)ep->req.buf;
	uint32_t ba = (uint32_t)ep->b_buf;

	if (in) {
		if (addr == ba)
			return;		/* not a bounce */
		goto free;
	}
	invalidate_dcache_range(ba, ba + ep->b_len);

	if (addr == ba)
		return;		/* not a bounce */

	memcpy(ep->req.buf, ep->b_buf, ep->req.length);
free:
	/* Large payloads use allocated buffer, free it. */
	if (ep->b_buf != ep->b_fast)
		free(ep->b_buf);
}
static int sp_ep2_handle(struct usb_ep *ep,struct usb_request *req)
{
	unsigned char *buf = req->buf + req->actual;
	char is_pingbuf;
	int count;
	printf("wait ep2 data\n",udc_read(UDEP2FS));
	while(!(udc_read(UDEP2FS) & EP_OVLD)){
		;//printf("ep2 fs %x\n",udc_read(UDEP2FS));
	}
	printf("ep2 fs %x\n",udc_read(UDEP2FS));
	is_pingbuf = udc_read(UDEP2PPC) & 0x04;
	count = sp_udc_get_ep_fifo_count(UDEP2PIC,is_pingbuf);
	if(count)
		sp_udc_read_fifo_packet(UDEP2FDP,req->buf,count,UDEP2VB);
	udc_write(udc_read(UDEP12C)|CLR_EP_OVLD, UDEP12C);
	printf("ep2 count \n",count);

	req->actual += count;
	
	return 0;
}

static int sp_ep_queue(struct usb_ep *ep,
		struct usb_request *req, gfp_t gfp_flags)
{
	struct sp_ep *sp_ep = container_of(ep, struct sp_ep, ep);
	struct sp_udc *udc = (struct sp_udc *)controller.ctrl->hcor;
	struct ept_queue_item *item;
	struct ept_queue_head *head;
	int bit, num, len, in, ret;
	int is_pingbuf;
	u32 count;
	num = sp_ep->desc->bEndpointAddress & USB_ENDPOINT_NUMBER_MASK;
	in = (sp_ep->desc->bEndpointAddress & USB_DIR_IN) != 0;

	printf("%s num %d in=%d len %d\n",__func__, num, in, req->length);
	if(in){
		if(num){//bulk in
		}else{//ep0
			ep0_write(req->buf,req->length);
		}
		
	}else{
		//sp_ep2_handle(ep,req);
	}
	return 0;
	item = sp_get_qtd(num, in);
	head = sp_get_qh(num, in);
	len = req->length;

	ret = sp_bounce(sp_ep, in);
	if (ret)
		return ret;

	item->next = TERMINATE;
	item->info = INFO_BYTES(len) | INFO_IOC | INFO_ACTIVE;
	item->page0 = (uint32_t)sp_ep->b_buf;
	item->page1 = ((uint32_t)sp_ep->b_buf & 0xfffff000) + 0x1000;
	sp_flush_qtd(num);

	head->next = (unsigned) item;
	head->info = 0;

	DBG("ept%d %s queue len %x, buffer %p\n",
	    num, in ? "in" : "out", len, sp_ep->b_buf);
	sp_flush_qh(num);

	if (in)
		bit = EPT_TX(num);
	else
		bit = EPT_RX(num);

	writel(bit, &udc->epprime);

	return 0;
}

static void handle_ep_complete(struct sp_ep *ep)
{
	struct ept_queue_item *item;
	int num, in, len;
	num = ep->desc->bEndpointAddress & USB_ENDPOINT_NUMBER_MASK;
	in = (ep->desc->bEndpointAddress & USB_DIR_IN) != 0;
	if (num == 0)
		ep->desc = &ep0_out_desc;
	item = sp_get_qtd(num, in);
	sp_invalidate_qtd(num);

	if (item->info & 0xff)
		printf("EP%d/%s FAIL info=%x pg0=%x\n",
		       num, in ? "in" : "out", item->info, item->page0);

	len = (item->info >> 16) & 0x7fff;
	ep->req.length -= len;
	sp_debounce(ep, in);

	DBG("ept%d %s complete %x\n",
			num, in ? "in" : "out", len);
	ep->req.complete(&ep->ep, &ep->req);
	if (num == 0) {
		ep->req.length = 0;
		usb_ep_queue(&ep->ep, &ep->req, 0);
		ep->desc = &ep0_in_desc;
	}
}

#define SETUP(type, request) (((type) << 8) | (request))

static void handle_setup(void)
{
	struct usb_request *req = &controller.ep[0].req;
	struct sp_udc *udc = (struct sp_udc *)controller.ctrl->hcor;
	struct ept_queue_head *head;
	struct usb_ctrlrequest r;
	int status = 0;
	int num, in, _num, _in, i;
	char *buf;
	head = sp_get_qh(0, 0);	/* EP0 OUT */

	sp_invalidate_qh(0);
	memcpy(&r, head->setup_data, sizeof(struct usb_ctrlrequest));
	writel(EPT_RX(0), &udc->epstat);
	DBG("handle setup %s, %x, %x index %x value %x\n", reqname(r.bRequest),
	    r.bRequestType, r.bRequest, r.wIndex, r.wValue);

	switch (SETUP(r.bRequestType, r.bRequest)) {
	case SETUP(USB_RECIP_ENDPOINT, USB_REQ_CLEAR_FEATURE):
		_num = r.wIndex & 15;
		_in = !!(r.wIndex & 0x80);

		if ((r.wValue == 0) && (r.wLength == 0)) {
			req->length = 0;
			for (i = 0; i < NUM_ENDPOINTS; i++) {
				struct sp_ep *ep = &controller.ep[i];

				if (!ep->desc)
					continue;
				num = ep->desc->bEndpointAddress
						& USB_ENDPOINT_NUMBER_MASK;
				in = (ep->desc->bEndpointAddress
						& USB_DIR_IN) != 0;
				if ((num == _num) && (in == _in)) {
					ep_enable(num, in, ep->ep.maxpacket);
					usb_ep_queue(controller.gadget.ep0,
							req, 0);
					break;
				}
			}
		}
		return;

	case SETUP(USB_RECIP_DEVICE, USB_REQ_SET_ADDRESS):
		/*
		 * write address delayed (will take effect
		 * after the next IN txn)
		 */
		writel((r.wValue << 25) | (1 << 24), &udc->devaddr);
		req->length = 0;
		usb_ep_queue(controller.gadget.ep0, req, 0);
		return;

	case SETUP(USB_DIR_IN | USB_RECIP_DEVICE, USB_REQ_GET_STATUS):
		req->length = 2;
		buf = (char *)req->buf;
		buf[0] = 1 << USB_DEVICE_SELF_POWERED;
		buf[1] = 0;
		usb_ep_queue(controller.gadget.ep0, req, 0);
		return;
	}
	/* pass request up to the gadget driver */
	if (controller.driver)
		status = controller.driver->setup(&controller.gadget, &r);
	else
		status = -ENODEV;

	if (!status)
		return;
	DBG("STALL reqname %s type %x value %x, index %x\n",
	    reqname(r.bRequest), r.bRequestType, r.wValue, r.wIndex);
	writel((1<<16) | (1 << 0), &udc->epctrl[0]);
}

static void stop_activity(void)
{
	int i, num, in;
	struct ept_queue_head *head;
	struct sp_udc *udc = (struct sp_udc *)controller.ctrl->hcor;
	writel(readl(&udc->epcomp), &udc->epcomp);
	writel(readl(&udc->epstat), &udc->epstat);
	writel(0xffffffff, &udc->epflush);

	/* error out any pending reqs */
	for (i = 0; i < NUM_ENDPOINTS; i++) {
		if (i != 0)
			writel(0, &udc->epctrl[i]);
		if (controller.ep[i].desc) {
			num = controller.ep[i].desc->bEndpointAddress
				& USB_ENDPOINT_NUMBER_MASK;
			in = (controller.ep[i].desc->bEndpointAddress
				& USB_DIR_IN) != 0;
			head = sp_get_qh(num, in);
			head->info = INFO_ACTIVE;
			sp_flush_qh(num);
		}
	}
}
static void sp_udc_handle_ep0s_idle(unsigned char *buf)
{
	int ret;
	struct usb_ctrlrequest *r = (struct usb_ctrlrequest *)buf;
	
	printf(">>>%s\n",__func__);	
	*((u32 *)buf)= * (volatile u32*)(UDEP0SDP + AHB_USBD_BASE) ;
	*((u32 *)(buf+4))= * (volatile u32*)(UDEP0SDP + AHB_USBD_BASE) ;
	printf("handle setup %s, %x, %x index %x value %x\n", reqname(r->bRequest),
	    r->bRequestType, r->bRequest, r->wIndex, r->wValue);
	
	switch (SETUP(r->bRequestType, r->bRequest)) {
	case SETUP(USB_RECIP_ENDPOINT, USB_REQ_CLEAR_FEATURE):
		printf("USB_REQ_CLEAR_FEATURE .\n");
		return;

	case SETUP(USB_RECIP_DEVICE, USB_REQ_SET_ADDRESS):
		//ep0_write(buf, 0);
		return;

	case SETUP(USB_DIR_IN | USB_RECIP_DEVICE, USB_REQ_GET_STATUS):
		printf("USB_REQ_GET_STATUS .\n");
		buf[0] = 1 << USB_DEVICE_SELF_POWERED;
		buf[1] = 0;
		ep0_write(buf, 2);;
		return;
	}
	/* deliver the request to the gadget driver */
	//ret = letb123_set_up(buf);
	if (controller.driver)
		ret = controller.driver->setup(&controller.gadget, r);
	else
		ret = -ENODEV;

	if (!ret)
		return;
	printf("set ep0 stall .\n");
	if (ret < 0) {
		udc_write(0x1, UDLCSTL);//set ep0 stall
	} 
}

void udc_irq(void)
{
	struct sp_udc *udc = (struct sp_udc *)controller.ctrl->hcor;
	unsigned int udl_if = udc_read(UDLIF);
	static u8 buf[8];
	//writel(n, &udc->usbsts);
	//printf("IRQ:udc_irq\n");
	int bit, i, num, in;
	if(udl_if & RESET_IF){
		printf("IRQ:UDC reset IRQ\n");
		//g_exit = 1;
		udc_write(RESET_IF, UDLIF);
		/*Allow LNK to suspend PHY*/
		udc_write((udc_read(UDLCSET)|8 )& 0xFE,  UDLCSET);
		udc_write(udc_read(UDCCS) &(~UPHY_CLK_CSS), UDCCS);
		
		udc_write(CLR_EP0_OUT_VLD, UDEP0CS); 
		udc_write(0x0,UDEP0CS);
		//EP2
		udc_write(CLR_EP_OVLD | RESET_PIPO_FIFO|udc_read(UDEP12C),UDEP12C);		
		/*Clear Stall Status*/
		udc_write(( CLREP2STL | CLREP1STL | CLREP0STL),UDLCSTL);
	}
	if (udl_if & EP0S_IF){
		udc_write(EP0S_IF , UDLIF);
		printf("IRQ:EP0 setup int\n");
		if((udc_read(UDEP0CS)&(EP0_OVLD|EP0_OUT_EMPTY)) == (EP0_OVLD|EP0_OUT_EMPTY))
			udc_write(udc_read(UDEP0CS)|CLR_EP0_OUT_VLD,UDEP0CS);
		sp_udc_handle_ep0s_idle(buf);
	}else if(udl_if & EP0I_IF){
		udc_write(EP0I_IF, UDLIF);
		printf("IRQ:EP0 in\n");
		udc_write(udc_read(UDEP0CS) & ~(EP_DIR), UDEP0CS);
	}else if(udl_if & EP2O_IF){
		printf("IRQ:ep2 Bulk Out Ack interrupt ");
		udc_write(EP2O_IF,UDLIF);
		//if(udc_read(UDEP2FS) & EP_OVLD)
		//	ep2_handle_((u8 *)r_cmd);
	}
#if 0	
	n &= (STS_SLI | STS_URI | STS_PCI | STS_UI | STS_UEI);
	if (n == 0)
		return;

	if (n & STS_URI) {
		DBG("-- reset --\n");
		stop_activity();
	}
	if (n & STS_SLI)
		DBG("-- suspend --\n");

	if (n & STS_PCI) {
		int max = 64;
		int speed = USB_SPEED_FULL;

		bit = (readl(&udc->portsc) >> 26) & 3;
		DBG("-- portchange %x %s\n", bit, (bit == 2) ? "High" : "Full");
		if (bit == 2) {
			speed = USB_SPEED_HIGH;
			max = 512;
		}
		controller.gadget.speed = speed;
		for (i = 1; i < NUM_ENDPOINTS; i++) {
			if (controller.ep[i].ep.maxpacket > max)
				controller.ep[i].ep.maxpacket = max;
		}
	}

	if (n & STS_UEI)
		printf("<UEI %x>\n", readl(&udc->epcomp));

	if ((n & STS_UI) || (n & STS_UEI)) {
		n = readl(&udc->epstat);
		if (n & EPT_RX(0))
			handle_setup();

		n = readl(&udc->epcomp);
		if (n != 0)
			writel(n, &udc->epcomp);

		for (i = 0; i < NUM_ENDPOINTS && n; i++) {
			if (controller.ep[i].desc) {
				num = controller.ep[i].desc->bEndpointAddress
					& USB_ENDPOINT_NUMBER_MASK;
				in = (controller.ep[i].desc->bEndpointAddress
						& USB_DIR_IN) != 0;
				bit = (in) ? EPT_TX(num) : EPT_RX(num);
				if (n & bit)
					handle_ep_complete(&controller.ep[i]);
			}
		}
	}
#endif
}

int usb_gadget_handle_interrupts(void)
{
	u32 value;
	struct sp_udc *udc = (struct sp_udc *)controller.ctrl->hcor;

	value = udc_read(UDLIF);
	if (value)
		udc_irq();

	return value;
}

static int sp_pullup(struct usb_gadget *gadget, int is_on)
{
	struct sp_udc *udc = (struct sp_udc *)controller.ctrl->hcor;
	printf("%s: %d\n", __func__,is_on);
	if (is_on) {
		printf("Force to Connect\n");
		/*Force to Connect*/
		//udc_write(ISSUE_RESUME, UDCCS);
		//soft connect
		udc_write((udc_read(UDLCSET)|8 )& 0xFE,  UDLCSET);
	}else {
		printf("Force to Disconnect\n");
		/*Force to Disconnect*/
		udc_write(udc_read(UDLCSET) | SOFT_DISC,  UDLCSET);
	}

	return 0;
}

void udc_disconnect(void)
{
	struct sp_udc *udc = (struct sp_udc *)controller.ctrl->hcor;
	/* disable pullup */
	stop_activity();
	writel(USBCMD_FS2, &udc->usbcmd);
	udelay(800);
	if (controller.driver)
		controller.driver->disconnect(&controller.gadget);
}


static void sp_udc_enable(struct sp_udc *udc)
{
	/*
		usb device interrupt enable
		---force usb bus disconnect enable
		---force usb bus connect interrupt enable
		---vbus interrupt enable
	*/
	
	/* usb device controller interrupt flag */
	udc_write(udc_read(UDCIF) & 0xFFFF, UDCIF);
	/* usb device link layer interrupt flag */
	udc_write(0xefffffff, UDLIF);

	udc_write(VBUS_IF , UDCIE); 
	udc_write(EP0S_IF | RESET_IF , UDLIE);
	udc_write(udc_read(UDLIE) | SUS_IF, UDLIE);
	udc_write(udc_read(UDCCS) | UPHY_CLK_CSS, UDCCS);/*PREVENT SUSP*/
	printf("<<<sp8388_udc_enable ip version ",udc_read(0x37c),udc_read(0x80));
}
 
static int sp_udc_probe(void)
{
	struct ept_queue_head *head;
	uint8_t *imem;
	int i;
	sp_udc_enable((struct sp_udc *)controller.ctrl->hcor);
	controller.gadget.speed = USB_SPEED_HIGH;

	const int num = NUM_ENDPOINTS;
	INIT_LIST_HEAD(&controller.gadget.ep_list);
	/* Init EP 0 */
	memcpy(&controller.ep[0].ep, &sp_ep_init[0], sizeof(*sp_ep_init));
	controller.ep[0].desc = &ep0_in_desc;
	controller.gadget.ep0 = &controller.ep[0].ep;
	INIT_LIST_HEAD(&controller.gadget.ep0->ep_list);

	/* Init EP 1..n */
	for (i = 1; i < NUM_ENDPOINTS; i++) {
		INIT_LIST_HEAD(&(sp_ep_init[i].ep_list));
		memcpy(&controller.ep[i].ep, &sp_ep_init[i],
		       sizeof(*sp_ep_init));
		list_add_tail(&controller.ep[i].ep.ep_list,
			      &controller.gadget.ep_list);
	}

	printf("%s: %d\n", __func__,__LINE__);
	return 0;
}

int usb_gadget_register_driver(struct usb_gadget_driver *driver)
{
	struct sp_udc *udc;
	int ret;

	if (!driver)
		return -EINVAL;
	if (!driver->bind || !driver->setup || !driver->disconnect)
		return -EINVAL;
	if (driver->speed != USB_SPEED_FULL && driver->speed != USB_SPEED_HIGH)
		return -EINVAL;

	writel(0x1010, 0x9c00010c);
	sp_udc_probe();
	printf("%s: %d\n", __func__,__LINE__);

	ret = driver->bind(&controller.gadget);
	if (ret) {
		DBG("driver->bind() returned %d\n", ret);
		return ret;
	}
	controller.driver = driver;

	return 0;
}

int usb_gadget_unregister_driver(struct usb_gadget_driver *driver)
{
	return 0;
}
