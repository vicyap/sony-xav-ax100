/*define ioctl cmd*/
#include <linux/ioctl.h>
#include <linux/list.h>
#include <linux/usb/gadget.h>

#define IAP_IOC_MAGIC 'i'
#define IAP_IOC_EXIT 	_IO(IAP_IOC_MAGIC,0)
#define IAP_IOC_RFU 	_IOW(IAP_IOC_MAGIC,1,int)
#define IAP_IOC_MAXNR	2

static u32 iap_debug = 1;
module_param(iap_debug, uint, 0644);
#define IAP_DEBUG(fmt, arg...)	do { if (iap_debug) printk(KERN_ERR fmt,##arg); } while (0)

#define DEV_ADD 2
#define DEV_RM	1
struct class *iapClass;
dev_t dev;

struct usb_gadget *gadget_iap;
//static u8 hostaddr[ETH_ALEN];
struct usb_request *iap_alloc_ep_req(struct usb_ep *ep);
static void iap_complete(struct usb_ep *ep, struct usb_request *req);
int enable_iap(struct usb_ep *ep);
#define IAP_BUF_NUM 20
static int iap_major = 0;
static struct cdev iapDevs;

static struct iap_dev {
	wait_queue_head_t read_queue;
	wait_queue_head_t main_queue;
	struct task_struct *iap_task;
	char main_condition;
	atomic_t	busy;
} my_iap_dev;
struct iap_packet {
	unsigned int len;
	unsigned char *buf;
};
struct iap_packet iap_rx[IAP_BUF_NUM], iap_tx[IAP_BUF_NUM];
unsigned char rx_index, tx_index;
unsigned char read_index, write_index;

struct usb_ep *ep_iap_in, *ep_iap_out;
struct usb_request *iap_req;
struct list_head iap_rqueue;

static struct usb_endpoint_descriptor iap_in_desc = {
	.bLength = USB_DT_ENDPOINT_SIZE,
	.bDescriptorType = USB_DT_ENDPOINT,

	.bmAttributes = USB_ENDPOINT_XFER_BULK,
	.wMaxPacketSize = cpu_to_le16(512),
};

static struct usb_endpoint_descriptor iap_out_desc = {
	.bLength = USB_DT_ENDPOINT_SIZE,
	.bDescriptorType = USB_DT_ENDPOINT,

	.bmAttributes = USB_ENDPOINT_XFER_BULK,
	.wMaxPacketSize = cpu_to_le16(512),
};

void iap_bind(void);
int iap_probe(void);
void iap_exit(void);
void iap_dev_distroy(void);
void iap_dev_create(void);

static int iap_open(struct inode *inode, struct file *file)
{
	IAP_DEBUG("iap: open!\n");
	if (atomic_read(&(my_iap_dev.busy)) == 1)
		goto dev_busy;
	else
		atomic_set(&(my_iap_dev.busy), 1);
	if (iap_rx[read_index].len == ~0) {
		iap_rx[read_index].len = 0;
	}
	return 0;
dev_busy:
	return -EBUSY;
}

static int iap_release(struct inode *inode, struct file *file)
{
	IAP_DEBUG("iap: iap_release!\n");
	iap_rx[rx_index].len = ~0;
	atomic_set(&(my_iap_dev.busy), 0);
	wake_up(&(my_iap_dev.read_queue));
	return 0;
}

#if 0
ssize_t iap_read(struct file * file, char __user * buff, size_t size, loff_t * offp)
{
	unsigned int count = size;

	IAP_DEBUG("%s %d\n", __FUNCTION__, __LINE__);
#if 1
	while (!iap_rx[read_index].len) {	/*no data to read */
		if (file->f_flags & O_NONBLOCK)
			return -EAGAIN;
		wait_event_interruptible(my_iap_dev.read_queue, iap_rx[read_index].len);
	}
#endif
#if 1
	if (iap_rx[read_index].len == ~0) {
		printk("no data\n");
		iap_rx[read_index].len = 0;
		if (my_iap_dev.main_condition == DEV_RM)
			wake_up(&(my_iap_dev.main_queue));	//iap_dev_distroy();//
		return -ENODEV;
	}
	if ((!iap_rx[read_index].len)) {
		printk("no data can be read!\n");
		return 0;
	}
#endif
	if (count > iap_rx[read_index].len)
		count = iap_rx[read_index].len;
	if (copy_to_user(buff, (void *)(iap_rx[read_index].buf), count)) {
		printk("iap: read EFAULT !\n");
		return -EFAULT;
	}
	iap_rx[read_index].len = 0;
	iap_req->actual = 0;

	if (usb_ep_queue(ep_iap_out, iap_req, GFP_ATOMIC)) {
		printk("iap: read quene fail!\n");
		/* FIXME recover later ... somehow */
	}
	//kfree(iap_rx[read_index].buf);
	IAP_DEBUG("iAP read %d bytes(s)\n", count);
	return count;
}
#endif
static void iap_nuke_read(void)
{
	struct usb_request *req;
	printk("%s in\n",__FUNCTION__);
	while (!list_empty(&iap_rqueue)) {
		req = list_entry(iap_rqueue.next, struct usb_request, list);
		list_del_init(&req->list);
		req->actual = 0;
		if (usb_ep_queue(ep_iap_out, req, GFP_ATOMIC)) {
			printk("iap: read nuke quene fail!\n");
			/* FIXME recover later ... somehow */
		}
	}
	printk("%s Done\n",__FUNCTION__);
}
ssize_t iap_read(struct file * file, char __user * buff, size_t size, loff_t * offp)
{
	unsigned int count = size;
	struct usb_request *req;

	IAP_DEBUG("%s %d\n", __FUNCTION__, __LINE__);
	while (list_empty(&iap_rqueue)) {	/*no data to read */
		if (file->f_flags & O_NONBLOCK)
			return -EAGAIN;
		wait_event_interruptible(my_iap_dev.read_queue, !list_empty(&iap_rqueue) || !ncm_intf);
		if (!ncm_intf) {
			if (my_iap_dev.main_condition == DEV_RM)
				wake_up(&(my_iap_dev.main_queue));
			return -ENODEV;
		}
	}

	req = list_entry(iap_rqueue.next, struct usb_request, list);
	list_del_init(&req->list);
	if (req->actual == ~0) {
		printk("no data\n");
		if (my_iap_dev.main_condition == DEV_RM)
			wake_up(&(my_iap_dev.main_queue));	//iap_dev_distroy();
		return -ENODEV;
	}
	if (!(req->actual)) {
		printk("no data can be read!\n");
		return 0;
	}
	if (count > req->actual)
		count = req->actual;
	if (copy_to_user(buff, (void *)(req->buf), count)) {
		printk("iap: read EFAULT !\n");
		return -EFAULT;
	}
	req->length = 2048;
	req->actual = 0;

	if (usb_ep_queue(ep_iap_out, req, GFP_ATOMIC)) {
		printk("iap: read quene fail!\n");
		/* FIXME recover later ... somehow */
	}
	//kfree(iap_rx[read_index].buf);
	IAP_DEBUG("iAP read %d bytes(s)\n", count);
	return count;
}

ssize_t iap_write(struct file * file, const char __user * buff, size_t size, loff_t * offp)
{
	struct usb_request *req;
	int status;
	IAP_DEBUG("%s %d size=%d\n", __FUNCTION__, __LINE__, size);
#ifdef IAP_1
	if ((iap_tx[tx_index].len) /*||(iap_rx[tx_index].buf) */ ) {
		printk("buffer is full! len=%d\n", iap_tx[write_index].len);
		return 0;
	}
#endif
	iap_tx[write_index].buf = kmalloc(size, GFP_ATOMIC);	//TODO
	if (!iap_tx[write_index].buf)
		return -ENOMEM;
	iap_tx[write_index].len = size;
	if (copy_from_user(iap_tx[write_index].buf, buff, size))
		return -EFAULT;
	req = iap_alloc_ep_req(ep_iap_in);
	if (!req)
		return -ENOMEM;
	req->buf = iap_tx[write_index].buf;
	req->length = iap_tx[write_index].len;
	req->complete = iap_complete;
	status = usb_ep_queue(ep_iap_in, req, GFP_ATOMIC);
	if (status) {
		printk("iap: write in queue failed!\n");
	}
	//write_index++;
	IAP_DEBUG("iap: write ok!\n");
	return size;
}

long iap_ioctl(struct file *filep, unsigned int cmd, unsigned long arg)
{
	if (_IOC_TYPE(cmd) != IAP_IOC_MAGIC)
		return -ENOTTY;
	if (_IOC_NR(cmd) > IAP_IOC_MAXNR)
		return -ENOTTY;

	switch (cmd) {
	case IAP_IOC_EXIT:
		//dump_stack();
		if (!ep_iap_out->driver_data) {
			iap_rx[rx_index].len = ~0;
			IAP_DEBUG("%s %d\n", __FUNCTION__, __LINE__);
			wake_up(&(my_iap_dev.read_queue));
		} else {
			IAP_DEBUG("%s %d\n", __FUNCTION__, __LINE__);
		}
		break;
	default:
		return -EINVAL;
	}
	return 0;
}

static struct file_operations iap_fops = {
	.owner = THIS_MODULE,
	.open = iap_open,
	.unlocked_ioctl = iap_ioctl,
	.read = iap_read,
	.write = iap_write,
	.release = iap_release,
};

unsigned int buflen = 2048;
struct usb_request *iap_alloc_ep_req(struct usb_ep *ep)
{
	struct usb_request *req;

	req = usb_ep_alloc_request(ep, GFP_ATOMIC);
	if (!req)
		return req;
	//printk("iap_alloc_ep_req %x %s\n",ep->address,ep->name);
	if (0x09 == (ep->address & 0xff)) {	/*ep-out */
		req->length = buflen;
		req->buf = kmalloc(buflen, GFP_ATOMIC);
		if (!req->buf) {
			usb_ep_free_request(ep, req);
			req = NULL;
		}
	}
	return req;
}

void free_ep_req(struct usb_ep *ep, struct usb_request *req)
{
	kfree(req->buf);
	usb_ep_free_request(ep, req);
}

void iap_disc(void)
{
	iap_rx[rx_index].len = ~0;
	IAP_DEBUG("%s %d\n", __FUNCTION__, __LINE__);
	my_iap_dev.main_condition = DEV_RM;
	ncm_intf = 0;
	if (waitqueue_active(&(my_iap_dev.read_queue)))
		wake_up(&(my_iap_dev.read_queue));
	else
		wake_up(&(my_iap_dev.main_queue));	//iap_dev_distroy();
	IAP_DEBUG("disconnect remove /dev/iap\n");
}

static void iap_complete(struct usb_ep *ep, struct usb_request *req)
{
	int status = req->status;
	IAP_DEBUG("%s %d act_len=%d\n", __FUNCTION__, ep->address & 0x7f, req->actual);
	switch (status) {

	case 0:		/* normal completion? */
		if (0x09 == (ep->address & 0xff)) {	/*receive data */
			list_add_tail(&req->list, &iap_rqueue);

			//iap_rx[rx_index].buf= req->buf;
			//iap_rx[rx_index].len= req->actual;
			wake_up(&(my_iap_dev.read_queue));
			return;
			req->buf = kmalloc(buflen, GFP_ATOMIC);
			req->actual = 0;
			if (!req->buf)
				return;	//-ENOMEM;
		} else {	/*send data */
			iap_tx[tx_index].len = 0;
			iap_tx[tx_index].buf = NULL;
			free_ep_req(ep, req);
			return;
#if 0
			if (iap_tx[tx_index + 1].len = 0) {	/*no data need sent */
				free_ep_req(ep, req);
				return;
			}

			req->buf = iap_tx[tx_index].buf;
			req->length = iap_tx[tx_index].len;
			req->actual = 0;
#endif
		}
		req->complete = iap_complete;
		break;
	case 0xff:		/*used to make /dev/iap */
		req->actual = 0;
		my_iap_dev.main_condition = DEV_ADD;
		iap_rx[rx_index].len = 0;
		req->status = 0;
		ncm_intf = 2;
		wake_up(&(my_iap_dev.main_queue));	//iap_dev_create();//
		IAP_DEBUG("create /dev/iap\n");
		return;
		/* this endpoint is normally active while we're configured */
	case -ECONNABORTED:	/* hardware forced ep reset */
	case -ECONNRESET:	/* request dequeued */
	case -ESHUTDOWN:	/* disconnect from host */
		IAP_DEBUG("%s gone (%d), %d/%d\n", ep->name, status, req->actual, req->length);
		if (0x09 == (ep->address & 0xff)) {
#if 0
			iap_rx[rx_index].len = 0;
			wake_up(&(my_iap_dev.read_queue));
			kfree(req->buf);
			free_ep_req(ep, req);
			/*remove iap dev node */
			my_iap_dev.main_condition = 1;
			wake_up(&(my_iap_dev.main_queue));
			printk("remove /dev/iap\n");
			return;
#else
			req->actual = 0;
			iap_rx[rx_index].len = ~0;

			req->actual = ~0;
			list_add_tail(&req->list, &iap_rqueue);
			my_iap_dev.main_condition = DEV_RM;
			wake_up(&(my_iap_dev.read_queue));
			wake_up(&(my_iap_dev.main_queue));
			IAP_DEBUG("%s %d\n", __FUNCTION__, __LINE__);
			return;
			my_iap_dev.main_condition = DEV_RM;
			wake_up(&(my_iap_dev.read_queue));
			ncm_intf = 0;
			//wake_up(&(my_iap_dev.main_queue));
			IAP_DEBUG("remove /dev/iap\n");
			return;
#endif
		} else {
			iap_tx[tx_index].len = 0;
			iap_tx[tx_index].buf = NULL;
			free_ep_req(ep, req);
			return;
		}
		req->complete = iap_complete;
		break;

	case -EOVERFLOW:	/* buffer overrun on read means that
				 * we didn't provide a big enough
				 * buffer.
				 */
	default:
#if 1
		IAP_DEBUG("%s complete --> %d, %d/%d\n", ep->name, status, req->actual, req->length);
#endif
	case -EREMOTEIO:	/* short read */
		break;
	}
	IAP_DEBUG("****iAP_complete &&&&&\n");
	status = usb_ep_queue(ep, req, GFP_ATOMIC);
	if (status) {
		printk("kill %s:  resubmit %d bytes --> %d\n", ep->name, req->length, status);
		usb_ep_set_halt(ep);
		/* FIXME recover later ... somehow */
	}

}

/*reveive ep used only */
static int iap_start_ep(bool is_in)
{
	struct usb_ep *ep;
	struct usb_request *req;
	int status;
	int i = 0;

	if (is_in) {
		printk("err:only reveive ep used only!");
		return 0;
	}

	ep = ep_iap_out;
	ep->driver_data = &my_iap_dev;
	req = iap_alloc_ep_req(ep);
	iap_req = req;
	if (!req)
		return -ENOMEM;

	req->complete = iap_complete;
#if 1
	//printk("is_in = %d ep addr %x\n",is_in,ep->address);
	status = usb_ep_queue(ep, req, GFP_ATOMIC);
	if (status) {
		printk("start %s --> %d , %d %d\n", ep->name, status, ep ? 11 : 0, ep->desc ? 11 : 0);
		free_ep_req(ep, req);
		return status;
	}

	while (i++ < 15) {
		req = iap_alloc_ep_req(ep);
		if (!req)
			return -ENOMEM;
		req->complete = iap_complete;
		status = usb_ep_queue(ep, req, GFP_ATOMIC);

		if (status) {
			printk("start %s --> %d , %d %d\n", ep->name, status, ep ? 11 : 0, ep->desc ? 11 : 0);
			free_ep_req(ep, req);
			return status;
		}
	}
	return status;
#else
	return 0;
#endif

}

int enable_iap(struct usb_ep *ep)
{
	int result = 0;

	result = iap_start_ep(false);
	if (result < 0) {
		usb_ep_disable(ep_iap_out);
		ep_iap_out->driver_data = NULL;
	}

	IAP_DEBUG("iap enabled\n");
	return result;
}

void iap_bind(void)
{
	if (!gadget_iap) {
		printk("gadget_iap is NULL!\n");
	} else {
		ep_iap_in = find_ep(gadget_iap, "ep8in-bulk");
		ep_iap_out = find_ep(gadget_iap, "ep9out-bulk");

		ep_iap_in->desc = &iap_in_desc;
		ep_iap_out->desc = &iap_out_desc;
		ep_iap_in->address = 0x88;
		ep_iap_out->address = 0x09;
		ep_iap_in->maxpacket = 0x200;
		ep_iap_out->maxpacket = 0x200;
		enable_iap(ep_iap_out);
		printk("iap add \n");
	}
	return;
}

#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/dirent.h>
#include <linux/syscalls.h>
void iap_dev_create(void)
{
	if (!ep_iap_out->driver_data) {	//DEV_ADD
		iapClass = class_create(THIS_MODULE, "apple");
		device_create(iapClass, NULL, MKDEV(iap_major, 0), NULL, "iap");
		printk("line %d create /dev/iap ok\n", __LINE__);
		ep_iap_out->driver_data = (void *)1;
	}

}

void iap_dev_distroy(void)
{
	if (ep_iap_out->driver_data) {
		if (carplay_f) {
			carplay_f = NULL;
			printk("iap_dev_distroy disconnect \n");
		}
		device_destroy(iapClass, MKDEV(iap_major, 0));
		class_destroy(iapClass);
		ep_iap_out->driver_data = NULL;
		printk("iap_dev_distroy remove /dev/iap ok!!!\n");
	}
}

int iap_task_handle(void *data)
{
	while (1) {
		wait_event_interruptible(my_iap_dev.main_queue, my_iap_dev.main_condition);
		printk("create = %d condition=%d\n", ep_iap_out->driver_data ? 0 : 1, my_iap_dev.main_condition);
		if (my_iap_dev.main_condition == DEV_RM && (ep_iap_out->driver_data)) {
			//ncm_disable(carplay_f);
			if (carplay_f) {
				ncm_disable(carplay_f);
				carplay_f = NULL;
				printk("disconnect \n");
			}
			//gether_setup_iap(gadget_iap, hostaddr,"usb");
			//while(iap_rx[rx_index].len == ~0)
			//      msleep(1);
			iap_nuke_read();
			device_destroy(iapClass, MKDEV(iap_major, 0));
			class_destroy(iapClass);
			ep_iap_out->driver_data = NULL;
			printk("remove /dev/iap ok!!!\n");
			//ep_iap_out->driver_data = NULL;
		} else if (!ep_iap_out->driver_data && my_iap_dev.main_condition == DEV_ADD) {	//DEV_ADD
			iapClass = class_create(THIS_MODULE, "apple");
			device_create(iapClass, NULL, MKDEV(iap_major, 0), NULL, "iap");
			//gether_setup_iap(gadget_iap, hostaddr,"apple_usb");
			printk("t create /dev/iap ok\n");
			ep_iap_out->driver_data = (void *)1;
		}
		my_iap_dev.main_condition = 0;
	}
	return 0;
}

int iap_probe(void)
{
	int result;
	int devno;
	char dev_name[] = "iap";
	result = alloc_chrdev_region(&dev, 0, 1, dev_name);
	if (result < 0) {
		printk(KERN_WARNING "iap: unable to get major %d\n", iap_major);
		return result;
	}
	iap_major = MAJOR(dev);
	devno = MKDEV(iap_major, 0);
	cdev_init(&iapDevs, &iap_fops);
	iapDevs.owner = THIS_MODULE;
	iapDevs.ops = &iap_fops;
	result = cdev_add(&iapDevs, devno, 1);
	if (result) {
		printk(KERN_NOTICE "Error %d adding \n", result);
		goto err1;
	}
	init_waitqueue_head(&(my_iap_dev.read_queue));
	init_waitqueue_head(&(my_iap_dev.main_queue));
	ep_iap_out->driver_data = 0;
	/*auto create dev node */
#if 0
	iapClass = class_create(THIS_MODULE, dev_name);
	if (IS_ERR(iapClass)) {
		printk("Err: failed in create iap class\n");
		//return -1;
	}
	device_create(iapClass, NULL, MKDEV(iap_major, 0), NULL, dev_name);
#endif
	my_iap_dev.iap_task = kthread_create(iap_task_handle, 0, "iapkthread");
	read_index = write_index = 0;
	iap_rx[write_index].len = 0;
	iap_tx[read_index].len = 0;
	INIT_LIST_HEAD(&iap_rqueue);
	atomic_set(&(my_iap_dev.busy), 0);
	wake_up_process(my_iap_dev.iap_task);
	IAP_DEBUG("iap char dev add ok \n");
	return 0;
err1:
	printk("iap_probe err!\n");
	unregister_chrdev_region(dev, 1);
	return result;
}

void iap_exit(void)
{
	cdev_del(&iapDevs);
	if (ep_iap_out->driver_data) {
		device_destroy(iapClass, MKDEV(iap_major, 0));
		class_destroy(iapClass);
	}
	unregister_chrdev_region(dev, 1);
	IAP_DEBUG("iap module removed !\n");
}
