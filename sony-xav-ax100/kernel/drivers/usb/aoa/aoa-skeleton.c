/*
 * aoa-skeleton.c : kernel module to initialize linux machine as android accessory
 * 			and also talk to accessory interface using bulk transfer
 * Skeleton code for ADK Host driver based on drivers/usb/usb-skeleton.c of
 * kernel source v2.6.3 or higher by Greg Kroah-Hartman (greg@kroah.com) and
 * https://github.com/gibsson/linux-adk by Gary Bisson <bisson.gary@gmail.com>
 *
 * Copyright (C) 2014 - Rajesh Sola <rajeshsola@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
 * for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, see <http://www.gnu.org/licenses/>
 */
#include <linux/workqueue.h>
#include <linux/scatterlist.h>

#include "aoa-skeleton.h"

/* Get a minor range for your devices from the usb maintainer */
#define AOA_SKEL_MINOR_BASE	192
#define MAX_TRANSFER            (PAGE_SIZE - 512)
/*
 * usb class driver info in order to get a minor number from the usb core,
 * and to have the device registered with the driver core
 */
const struct file_operations aoa_fops = {
	.owner = THIS_MODULE,
	.open = adk_aoa_open,
	.release = adk_aoa_release,
	.read = aoa_read,
	.write = aoa_write,
};
struct usb_class_driver aoa_class = {
	.name = "aoa-skel%d",
	.fops = &aoa_fops,
	.minor_base = AOA_SKEL_MINOR_BASE,
};

#ifdef CONFIG_USB_HOST_RESET_SP
extern void Reset_usb_host_ctrler(struct usb_device *udev);
#endif

//#define	AOA_CNT_DEDAY

#ifdef AOA_CNT_DEDAY
long long cnt_1s = 0;
long long cnt_all = 0;
#endif

#define	AOA_WR_TIMEOUT	(3*HZ)
static ssize_t aoa_read(struct file *file, char *buffer, size_t count, loff_t * ppos)
{
	struct usb_adk_aoa *dev;
	int ret, nactual;
	int cpcnt = 0;
	int len = 0;

	struct scatterlist *sg;
	struct sg_table table;
	int nents = 0;
	int i;

#ifdef AOA_CNT_DEDAY
	struct timespec t0, t1, d;
#endif

	//printk(KERN_DEBUG "AOA:Read request of %d bytes,off=%d\n", count, (int)*ppos);
	dev = file->private_data;

	ret = mutex_lock_interruptible(&dev->io_mutex);
	if (ret < 0)
		goto exit;

	if (!dev->interface) {	/* disconnect() was called */
		ret = -ENODEV;
		goto exit;
	}

	if (dev->rdbuf.cache_flag != 0) {

		//copy to user
		cpcnt = min(count, dev->rdbuf.length);
		//printk(KERN_DEBUG "AOA:: cp data form cache count = %d clen = %d\n", count, dev->rdbuf.length);

		ret = copy_to_user(buffer, dev->rdbuf.buf + dev->rdbuf.offset, cpcnt);
		if (ret) {
			printk("aoa: Fail to copy cache buffer to user !\n");
			ret = -EFAULT;
			goto exit;
		}

		dev->rdbuf.length -= cpcnt;
		dev->rdbuf.offset += cpcnt;

		if (dev->rdbuf.length <= 0) {
			dev->rdbuf.cache_flag = 0;
			dev->rdbuf.offset = 0;
			dev->rdbuf.length = 0;
		}
	}

	if (cpcnt < count) {

		len = count - cpcnt;
		//printk(KERN_DEBUG "AOA: Read data form usb device request_count = %d left_len = %d\n", count, len);

		if (len <= dev->bulk_in_size) {
#ifdef AOA_CNT_DEDAY
			getnstimeofday(&t0);
#endif
			ret = usb_bulk_msg(dev->udev, usb_rcvbulkpipe(dev->udev, dev->bulk_in_endpointAddr),
							   		dev->rdbuf.buf, dev->bulk_in_size, &nactual, 0);
#ifdef AOA_CNT_DEDAY
			getnstimeofday(&t1);
			cnt_all++;
			d = timespec_sub(t1, t0);
			if(d.tv_sec >= 10) {
				cnt_1s++;
				printk("-- aoa read time %09ld , %lld/%lld\n", d.tv_sec, cnt_1s, cnt_all);
			}
#endif
			if ((ret < 0) && (ret != -EOVERFLOW)) {
				printk("aoa: read-usb_bulk_msg(%d):in failed\n", ret);
#ifdef CONFIG_USB_HOST_RESET_SP
				if((dev->need_reset == 1) && (ret == -ETIMEDOUT)) {
					Reset_usb_host_ctrler(dev->udev);
				}
#endif
				goto exit;
			}

			if (!dev->interface) {	/* disconnect() was called */
				ret = -ENODEV;
				goto exit;
			}

			if(unlikely(dev->need_reset == 0)) {
				dev->need_reset = 1;
				wake_up_process(dev->watchdog);
			}

			ret = copy_to_user(buffer + cpcnt, dev->rdbuf.buf, len);
			if (ret) {
				printk("aoa: Fail to copy device data to user !\n");
				ret = -EFAULT;
				goto exit;
			}

			cpcnt += (len > nactual) ? nactual : len;
			//printk(KERN_DEBUG "AOA: Rx data form usb device nactual = %d cpcnt= %d ret=%d\n", nactual, cpcnt, ret);

			if (nactual > len) {
				//printk(KERN_DEBUG "AOA: Revice more data %d\n", nactual - len);
				dev->rdbuf.length = nactual - len;
				dev->rdbuf.offset = len;
				dev->rdbuf.cache_flag = 1;
			}
		} else {
			struct usb_sg_request io;
			u8 *pbuf;

			//printk(KERN_DEBUG "AOA: The read data(%d) is bigger than %d", len, dev->bulk_in_size);

			/*Get sg number */
			nents = len / dev->bulk_in_size;
			if (len % dev->bulk_in_size)
				nents++;
			//printk( KERN_DEBUG "AOA: Need sg num %d\n", nents);

			ret = sg_alloc_table(&table, nents, GFP_KERNEL);
			if (ret) {
				printk("aoa: malloc mem for sg fail\n");
				ret = -ENOMEM;
				goto exit;
			}
			dev->rdbuf.sg = table.sgl;

			/*alloc transport data bufs */
			for_each_sg(dev->rdbuf.sg, sg, nents, i) {

				if (i == (nents - 1)) {
					sg_set_buf(sg, dev->rdbuf.buf, dev->bulk_in_size);
					break;
				}

				pbuf = kmalloc(dev->bulk_in_size, GFP_KERNEL);
				if (!pbuf) {
					int n;

					//printk("aoa: malloc mem for transport buffers fail\n");
					for_each_sg(dev->rdbuf.sg, sg, i, n) {
						kfree(sg_virt(sg));
					}
					sg_free_table(&table);

					ret = -ENOMEM;
					goto exit;
				}
				sg_set_buf(sg, pbuf, dev->bulk_in_size);
			}

			ret = usb_sg_init(&io, dev->udev, usb_rcvbulkpipe(dev->udev, dev->bulk_in_endpointAddr), 0, dev->rdbuf.sg, nents, len, GFP_KERNEL);
			if (ret) {
				printk("aoa: usb sg init fail !\n");
				goto free_sg;
			}

			queue_delayed_work(dev->timeout_queue, &dev->delay_work, AOA_WR_TIMEOUT);
			usb_sg_wait(&io);
			cancel_delayed_work(&dev->delay_work);

			/*copy to user */
			ret = 0;
			for_each_sg(dev->rdbuf.sg, sg, nents - 1, i) {

				if (ret) {
					printk("aoa: Fail to copy device data to user !\n");
					ret = -EFAULT;
					goto free_sg;
				} else {
					ret = copy_to_user(buffer + cpcnt, sg_virt(sg), sg->length);
					cpcnt += sg->length;
				}
				kfree(sg_virt(sg));
			}

			if (len < (nents * dev->bulk_in_size)) {
				dev->rdbuf.sg[nents - 1].length = len - (nents - 1) * dev->bulk_in_size;
				//printk(KERN_DEBUG "AOA: SG last need %d %d\n", dev->rdbuf.sg[nents-1].length, io.bytes);
			}

			ret = copy_to_user(buffer + cpcnt, dev->rdbuf.buf, dev->rdbuf.sg[nents - 1].length);
			if (ret)
				printk("aoa: Fail to copy sg last data to user !\n");
			else
				cpcnt += dev->rdbuf.sg[nents - 1].length;

			if (io.bytes > len) {
				//printk(KERN_DEBUG "aoa: Revice more data %d form sg\n", io.bytes - len);
				dev->rdbuf.length = io.bytes - len;
				dev->rdbuf.offset = dev->rdbuf.sg[nents - 1].length;
				dev->rdbuf.cache_flag = 1;
			}

			sg_free_table(&table);
			dev->need_reset = 1;
		}
	}

	mutex_unlock(&dev->io_mutex);
	//printk( KERN_DEBUG "AOA:: read end %d %d!\n\n", count, cpcnt);

	return cpcnt;

free_sg:
	for_each_sg(dev->rdbuf.sg, sg, nents - 1, i) {
		kfree(sg_virt(sg));
	}
	sg_free_table(&table);

exit:
	mutex_unlock(&dev->io_mutex);
	return ret;
}

static ssize_t aoa_write(struct file *file, const char *user_buffer, size_t count, loff_t * ppos)
{
	struct usb_adk_aoa *dev;
	int ret, nactual;
	char *buf;
	//size_t writesize = min(count, (size_t) MAX_TRANSFER);
	size_t writesize = count;

#ifdef AOA_CNT_DEDAY
		struct timespec t0, t1, d;
#endif

	//printk("AOA: Write request of %d bytes,off=%d\n",count,(int)*ppos);

	dev = file->private_data;

	buf = kmalloc(writesize, GFP_KERNEL);
	if (buf == NULL) {
		ret = -ENOMEM;
		goto error;
	}

	if (copy_from_user(buf, user_buffer, writesize)) {
		ret = -EFAULT;
		goto error;
	}
#ifdef AOA_CNT_DEDAY
	getnstimeofday(&t0);
#endif

	ret = usb_bulk_msg(dev->udev, usb_sndbulkpipe(dev->udev, dev->bulk_out_endpointAddr),
									buf, writesize, &nactual, 1100);
	if (ret < 0) {
		printk("AOA: write-usb_bulk_msg(%d):out failed\n", ret);
#ifdef CONFIG_USB_HOST_RESET_SP
		if((dev->need_reset == 1) && (ret == -ETIMEDOUT)) {
			Reset_usb_host_ctrler(dev->udev);
		}
#endif
		goto error;
	}

#ifdef AOA_CNT_DEDAY
	getnstimeofday(&t1);
	d = timespec_sub(t1, t0);
	if(d.tv_sec >= 1) {
		printk("-- aoa read time %09ld s", d.tv_sec);
	}
#endif

	kfree(buf);

	return nactual;

error:

	kfree(buf);
	return ret;
}
