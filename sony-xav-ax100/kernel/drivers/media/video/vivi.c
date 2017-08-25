/*
 * Virtual Video driver - This code emulates a real video device with v4l2 api
 *
 * Copyright (c) 2006 by:
 *      Mauro Carvalho Chehab <mchehab--a.t--infradead.org>
 *      Ted Walther <ted--a.t--enumera.com>
 *      John Sokol <sokol--a.t--videotechnology.com>
 *      http://v4l.videotechnology.com/
 *
 *      Conversion to videobuf2 by Pawel Osciak & Marek Szyprowski
 *      Copyright (c) 2010 Samsung Electronics
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the BSD Licence, GNU General Public License
 * as published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version
 */
#include <linux/module.h>
#include <linux/errno.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/sched.h>
#include <linux/slab.h>
#include <linux/font.h>
#include <linux/mutex.h>
#include <linux/videodev2.h>
#include <linux/kthread.h>
#include <linux/freezer.h>
#include <media/videobuf2-vmalloc.h>
#include <media/v4l2-device.h>
#include <media/v4l2-ioctl.h>
#include <media/v4l2-ctrls.h>
#include <media/v4l2-fh.h>
#include <media/v4l2-event.h>
#include <media/v4l2-common.h>

#define VIVI_MODULE_NAME "vivi"

/* Wake up at about 30 fps */
#define WAKE_NUMERATOR 30
#define WAKE_DENOMINATOR 1001
#define BUFFER_TIMEOUT     msecs_to_jiffies(500)  /* 0.5 seconds */

#define MAX_WIDTH 1920
#define MAX_HEIGHT 1200

#define VIVI_VERSION "0.8.1"

MODULE_DESCRIPTION("Video Technology Magazine Virtual Video Capture Board");
MODULE_AUTHOR("Mauro Carvalho Chehab, Ted Walther and John Sokol");
MODULE_LICENSE("Dual BSD/GPL");
MODULE_VERSION(VIVI_VERSION);

static unsigned video_nr = -1;
module_param(video_nr, uint, 0644);
MODULE_PARM_DESC(video_nr, "videoX start number, -1 is autodetect");

static unsigned n_devs = 2;
module_param(n_devs, uint, 0644);
MODULE_PARM_DESC(n_devs, "number of video devices to create");

static unsigned debug;
module_param(debug, uint, 0644);
MODULE_PARM_DESC(debug, "activates debug info");

static unsigned int vid_limit = 16;
module_param(vid_limit, uint, 0644);
MODULE_PARM_DESC(vid_limit, "capture memory limit in megabytes");

/* Global font descriptor */
static const u8 *font8x16;

#define dprintk(dev, level, fmt, arg...) \
	v4l2_dbg(level, debug, &dev->v4l2_dev, fmt, ## arg)

/* ------------------------------------------------------------------
	Basic structures
   ------------------------------------------------------------------*/

struct vivi_fmt {
	char  *name;
	u32   fourcc;          /* v4l2 format id */
	int   depth;
};

static struct vivi_fmt formats[] = {
	{
		.name     = "4:2:2, packed, YUYV",
		.fourcc   = V4L2_PIX_FMT_YUYV,
		.depth    = 16,
	},
	{
		.name     = "4:2:2, packed, UYVY",
		.fourcc   = V4L2_PIX_FMT_UYVY,
		.depth    = 16,
	},
	{
		.name     = "RGB565 (LE)",
		.fourcc   = V4L2_PIX_FMT_RGB565, /* gggbbbbb rrrrrggg */
		.depth    = 16,
	},
	{
		.name     = "RGB565 (BE)",
		.fourcc   = V4L2_PIX_FMT_RGB565X, /* rrrrrggg gggbbbbb */
		.depth    = 16,
	},
	{
		.name     = "RGB555 (LE)",
		.fourcc   = V4L2_PIX_FMT_RGB555, /* gggbbbbb arrrrrgg */
		.depth    = 16,
	},
	{
		.name     = "RGB555 (BE)",
		.fourcc   = V4L2_PIX_FMT_RGB555X, /* arrrrrgg gggbbbbb */
		.depth    = 16,
	},
};

static struct vivi_fmt *get_format(struct v4l2_format *f)
{
	struct vivi_fmt *fmt;
	unsigned int k;

	for (k = 0; k < ARRAY_SIZE(formats); k++) {
		fmt = &formats[k];
		if (fmt->fourcc == f->fmt.pix.pixelformat)
			break;
	}

	if (k == ARRAY_SIZE(formats))
		return NULL;

	return &formats[k];
}

/* buffer for one video frame */
struct vivi_buffer {
	/* common v4l buffer stuff -- must be first */
	struct vb2_buffer	vb;
	struct list_head	list;
	struct vivi_fmt        *fmt;
};

struct vivi_dmaqueue {
	struct list_head       active;

	/* thread for generating video stream*/
	struct task_struct         *kthread;
	wait_queue_head_t          wq;
	/* Counters to control fps rate */
	int                        frame;
	int                        ini_jiffies;
};

static LIST_HEAD(vivi_devlist);

struct vivi_dev {
	struct list_head           vivi_devlist;
	struct v4l2_device 	   v4l2_dev;
	struct v4l2_ctrl_handler   ctrl_handler;

	/* controls */
	struct v4l2_ctrl	   *brightness;
	struct v4l2_ctrl	   *contrast;
	struct v4l2_ctrl	   *saturation;
	struct v4l2_ctrl	   *hue;
	struct {
		/* autogain/gain cluster */
		struct v4l2_ctrl	   *autogain;
		struct v4l2_ctrl	   *gain;
	};
	struct v4l2_ctrl	   *volume;
	struct v4l2_ctrl	   *button;
	struct v4l2_ctrl	   *boolean;
	struct v4l2_ctrl	   *int32;
	struct v4l2_ctrl	   *int64;
	struct v4l2_ctrl	   *menu;
	struct v4l2_ctrl	   *string;
	struct v4l2_ctrl	   *bitmask;

	spinlock_t                 slock;
	struct mutex		   mutex;

	/* various device info */
	struct video_device        *vfd;

	struct vivi_dmaqueue       vidq;

	/* Several counters */
	unsigned 		   ms;
	unsigned long              jiffies;
	unsigned		   button_pressed;

	int			   mv_count;	/* Controls bars movement */

	/* Input Number */
	int			   input;

	/* video capture */
	struct vivi_fmt            *fmt;
	unsigned int               width, height;
	struct vb2_queue	   vb_vidq;
	enum v4l2_field		   field;
	unsigned int		   field_count;

	u8 			   bars[9][3];
	u8 			   line[MAX_WIDTH * 4];
};

/* ------------------------------------------------------------------
	DMA and thread functions
   ------------------------------------------------------------------*/

/* Bars and Colors should match positions */

enum colors {
	WHITE,
	AMBER,
	CYAN,
	GREEN,
	MAGENTA,
	RED,
	BLUE,
	BLACK,
	TEXT_BLACK,
};

/* R   G   B */
#define COLOR_WHITE	{204, 204, 204}
#define COLOR_AMBER	{208, 208,   0}
#define COLOR_CYAN	{  0, 206, 206}
#define	COLOR_GREEN	{  0, 239,   0}
#define COLOR_MAGENTA	{239,   0, 239}
#define COLOR_RED	{205,   0,   0}
#define COLOR_BLUE	{  0,   0, 255}
#define COLOR_BLACK	{  0,   0,   0}

struct bar_std {
	u8 bar[9][3];
};

/* Maximum number of bars are 10 - otherwise, the input print code
   should be modified */
static struct bar_std bars[] = {
	{	/* Standard ITU-R color bar sequence */
		{ COLOR_WHITE, COLOR_AMBER, COLOR_CYAN, COLOR_GREEN,
		  COLOR_MAGENTA, COLOR_RED, COLOR_BLUE, COLOR_BLACK, COLOR_BLACK }
	}, {
		{ COLOR_WHITE, COLOR_AMBER, COLOR_BLACK, COLOR_WHITE,
		  COLOR_AMBER, COLOR_BLACK, COLOR_WHITE, COLOR_AMBER, COLOR_BLACK }
	}, {
		{ COLOR_WHITE, COLOR_CYAN, COLOR_BLACK, COLOR_WHITE,
		  COLOR_CYAN, COLOR_BLACK, COLOR_WHITE, COLOR_CYAN, COLOR_BLACK }
	}, {
		{ COLOR_WHITE, COLOR_GREEN, COLOR_BLACK, COLOR_WHITE,
		  COLOR_GREEN, COLOR_BLACK, COLOR_WHITE, COLOR_GREEN, COLOR_BLACK }
	},
};

#define NUM_INPUTS ARRAY_SIZE(bars)

#define TO_Y(r, g, b) \
	(((16829 * r + 33039 * g + 6416 * b  + 32768) >> 16) + 16)
/* RGB to  V(Cr) Color transform */
#define TO_V(r, g, b) \
	(((28784 * r - 24103 * g - 4681 * b  + 32768) >> 16) + 128)
/* RGB to  U(Cb) Color transform */
#define TO_U(r, g, b) \
	(((-9714 * r - 19070 * g + 28784 * b + 32768) >> 16) + 128)

/* precalculate color bar values to speed up rendering */
static void precalculate_bars(struct vivi_dev *dev)
{
	u8 r, g, b;
	int k, is_yuv;

	for (k = 0; k < 9; k++) {
		r = bars[dev->input].bar[k][0];
		g = bars[dev->input].bar[k][1];
		b = bars[dev->input].bar[k][2];
		is_yuv = 0;

		switch (dev->fmt->fourcc) {
		case V4L2_PIX_FMT_YUYV:
		case V4L2_PIX_FMT_UYVY:
			is_yuv = 1;
			break;
		case V4L2_PIX_FMT_RGB565:
		case V4L2_PIX_FMT_RGB565X:
			r >>= 3;
			g >>= 2;
			b >>= 3;
			break;
		case V4L2_PIX_FMT_RGB555:
		case V4L2_PIX_FMT_RGB555X:
			r >>= 3;
			g >>= 3;
			b >>= 3;
			break;
		}

		if (is_yuv) {
			dev->bars[k][0] = TO_Y(r, g, b);	/* Luma */
			dev->bars[k][1] = TO_U(r, g, b);	/* Cb */
			dev->bars[k][2] = TO_V(r, g, b);	/* Cr */
		} else {
			dev->bars[k][0] = r;
			dev->bars[k][1] = g;
			dev->bars[k][2] = b;
		}
	}
}

#define TSTAMP_MIN_Y	24
#define TSTAMP_MAX_Y	(TSTAMP_MIN_Y + 15)
#define TSTAMP_INPUT_X	10
#define TSTAMP_MIN_X	(54 + TSTAMP_INPUT_X)

static void gen_twopix(struct vivi_dev *dev, u8 *buf, int colorpos)
{
	u8 r_y, g_u, b_v;
	int color;
	u8 *p;

	r_y = dev->bars[colorpos][0]; /* R or precalculated Y */
	g_u = dev->bars[colorpos][1]; /* G or precalculated U */
	b_v = dev->bars[colorpos][2]; /* B or precalculated V */

	for (color = 0; color < 4; color++) {
		p = buf + color;

		switch (dev->fmt->fourcc) {
		case V4L2_PIX_FMT_YUYV:
			switch (color) {
			case 0:
			case 2:
				*p = r_y;
				break;
			case 1:
				*p = g_u;
				break;
			case 3:
				*p = b_v;
				break;
			}
			break;
		case V4L2_PIX_FMT_UYVY:
			switch (color) {
			case 1:
			case 3:
				*p = r_y;
				break;
			case 0:
				*p = g_u;
				break;
			case 2:
				*p = b_v;
				break;
			}
			break;
		case V4L2_PIX_FMT_RGB565:
			switch (color) {
			case 0:
			case 2:
				*p = (g_u << 5) | b_v;
				break;
			case 1:
			case 3:
				*p = (r_y << 3) | (g_u >> 3);
				break;
			}
			break;
		case V4L2_PIX_FMT_RGB565X:
			switch (color) {
			case 0:
			case 2:
				*p = (r_y << 3) | (g_u >> 3);
				break;
			case 1:
			case 3:
				*p = (g_u << 5) | b_v;
				break;
			}
			break;
		case V4L2_PIX_FMT_RGB555:
			switch (color) {
			case 0:
			case 2:
				*p = (g_u << 5) | b_v;
				break;
			case 1:
			case 3:
				*p = (r_y << 2) | (g_u >> 3);
				break;
			}
			break;
		case V4L2_PIX_FMT_RGB555X:
			switch (color) {
			case 0:
			case 2:
				*p = (r_y << 2) | (g_u >> 3);
				break;
			case 1:
			case 3:
				*p = (g_u << 5) | b_v;
				break;
			}
			break;
		}
	}
}

static void precalculate_line(struct vivi_dev *dev)
{
	int w;

	for (w = 0; w < dev->width * 2; w += 2) {
		int colorpos = (w / (dev->width / 8) % 8);

		gen_twopix(dev, dev->line + w * 2, colorpos);
	}
}

static void gen_text(struct vivi_dev *dev, char *basep,
					int y, int x, char *text)
{
	int line;

	/* Checks if it is possible to show string */
	if (y + 16 >= dev->height || x + strlen(text) * 8 >= dev->width)
		return;

	/* Print stream time */
	for (line = y; line < y + 16; line++) {
		int j = 0;
		char *pos = basep + line * dev->width * 2 + x * 2;
		char *s;

		for (s = text; *s; s++) {
			u8 chr = font8x16[*s * 16 + line - y];
			int i;

			for (i = 0; i < 7; i++, j++) {
				/* Draw white font on black background */
				if (chr & (1 << (7 - i)))
					gen_twopix(dev, pos + j * 2, WHITE);
				else
					gen_twopix(dev, pos + j * 2, TEXT_BLACK);
			}
		}
	}
}

static void vivi_fillbuff(struct vivi_dev *dev, struct vivi_buffer *buf)
{
	int wmax = dev->width;
	int hmax = dev->height;
	struct timeval ts;
	void *vbuf = vb2_plane_vaddr(&buf->vb, 0);
	unsigned ms;
	char str[100];
	int h, line = 1;
	s32 gain;

	if (!vbuf)
		return;

	for (h = 0; h < hmax; h++)
		memcpy(vbuf + h * wmax * 2, dev->line + (dev->mv_count % wmax) * 2, wmax * 2);

	/* Updates stream time */

	dev->ms += jiffies_to_msecs(jiffies - dev->jiffies);
	dev->jiffies = jiffies;
	ms = dev->ms;
	snprintf(str, sizeof(str), " %02d:%02d:%02d:%03d ",
			(ms / (60 * 60 * 1000)) % 24,
			(ms / (60 * 1000)) % 60,
			(ms / 1000) % 60,
			ms % 1000);
	gen_text(dev, vbuf, line++ * 16, 16, str);
	snprintf(str, sizeof(str), " %dx%d, input %d ",
			dev->width, dev->height, dev->input);
	gen_text(dev, vbuf, line++ * 16, 16, str);

	gain = v4l2_ctrl_g_ctrl(dev->gain);
	mutex_lock(&dev->ctrl_handler.lock);
	snprintf(str, sizeof(str), " brightness %3d, contrast %3d, saturation %3d, hue %d ",
			dev->brightness->cur.val,
			dev->contrast->cur.val,
			dev->saturation->cur.val,
			dev->hue->cur.val);
	gen_text(dev, vbuf, line++ * 16, 16, str);
	snprintf(str, sizeof(str), " autogain %d, gain %3d, volume %3d ",
			dev->autogain->cur.val, gain, dev->volume->cur.val);
	gen_text(dev, vbuf, line++ * 16, 16, str);
	snprintf(str, sizeof(str), " int32 %d, int64 %lld, bitmask %08x ",
			dev->int32->cur.val,
			dev->int64->cur.val64,
			dev->bitmask->cur.val);
	gen_text(dev, vbuf, line++ * 16, 16, str);
	snprintf(str, sizeof(str), " boolean %d, menu %s, string \"%s\" ",
			dev->boolean->cur.val,
			dev->menu->qmenu[dev->menu->cur.val],
			dev->string->cur.string);
	mutex_unlock(&dev->ctrl_handler.lock);
	gen_text(dev, vbuf, line++ * 16, 16, str);
	if (dev->button_pressed) {
		dev->button_pressed--;
		snprintf(str, sizeof(str), " button pressed!");
		gen_text(dev, vbuf, line++ * 16, 16, str);
	}

	dev->mv_count += 2;

	buf->vb.v4l2_buf.field = dev->field;
	dev->field_count++;
	buf->vb.v4l2_buf.sequence = dev->field_count >> 1;
	do_gettimeofday(&ts);
	buf->vb.v4l2_buf.timestamp = ts;
}

static void vivi_thread_tick(struct vivi_dev *dev)
{
	struct vivi_dmaqueue *dma_q = &dev->vidq;
	struct vivi_buffer *buf;
	unsigned long flags = 0;

	dprintk(dev, 1, "Thread tick\n");

	spin_lock_irqsave(&dev->slock, flags);
	if (list_empty(&dma_q->active)) {
		dprintk(dev, 1, "No active queue to serve\n");
		spin_unlock_irqrestore(&dev->slock, flags);
		return;
	}

	buf = list_entry(dma_q->active.next, struct vivi_buffer, list);
	list_del(&buf->list);
	spin_unlock_irqrestore(&dev->slock, flags);

	do_gettimeofday(&buf->vb.v4l2_buf.timestamp);

	/* Fill buffer */
	vivi_fillbuff(dev, buf);
	dprintk(dev, 1, "filled buffer %p\n", buf);

	vb2_buffer_done(&buf->vb, VB2_BUF_STATE_DONE);
	dprintk(dev, 2, "[%p/%d] done\n", buf, buf->vb.v4l2_buf.index);
}

#define frames_to_ms(frames)					\
	((frames * WAKE_NUMERATOR * 1000) / WAKE_DENOMINATOR)

static void vivi_sleep(struct vivi_dev *dev)
{
	struct vivi_dmaqueue *dma_q = &dev->vidq;
	int timeout;
	DECLARE_WAITQUEUE(wait, current);

	dprintk(dev, 1, "%s dma_q=0x%08lx\n", __func__,
		(unsigned long)dma_q);

	add_wait_queue(&dma_q->wq, &wait);
	if (kthread_should_stop())
		goto stop_task;

	/* Calculate time to wake up */
	timeout = msecs_to_jiffies(frames_to_ms(1));

	vivi_thread_tick(dev);

	schedule_timeout_interruptible(timeout);

stop_task:
	remove_wait_queue(&dma_q->wq, &wait);
	try_to_freeze();
}

static int vivi_thread(void *data)
{
	struct vivi_dev *dev = data;

	dprintk(dev, 1, "thread started\n");

	set_freezable();

	for (;;) {
		vivi_sleep(dev);

		if (kthread_should_stop())
			break;
	}
	dprintk(dev, 1, "thread: exit\n");
	return 0;
}

static int vivi_start_generating(struct vivi_dev *dev)
{
	struct vivi_dmaqueue *dma_q = &dev->vidq;

	dprintk(dev, 1, "%s\n", __func__);

	/* Resets frame counters */
	dev->ms = 0;
	dev->mv_count = 0;
	dev->jiffies = jiffies;

	dma_q->frame = 0;
	dma_q->ini_jiffies = jiffies;
	dma_q->kthread = kthread_run(vivi_thread, dev, dev->v4l2_dev.name);

	if (IS_ERR(dma_q->kthread)) {
		v4l2_err(&dev->v4l2_dev, "kernel_thread() failed\n");
		return PTR_ERR(dma_q->kthread);
	}
	/* Wakes thread */
	wake_up_interruptible(&dma_q->wq);

	dprintk(dev, 1, "returning from %s\n", __func__);
	return 0;
}

static void vivi_stop_generating(struct vivi_dev *dev)
{
	struct vivi_dmaqueue *dma_q = &dev->vidq;

	dprintk(dev, 1, "%s\n", __func__);

	/* shutdown control thread */
	if (dma_q->kthread) {
		kthread_stop(dma_q->kthread);
		dma_q->kthread = NULL;
	}

	/*
	 * Typical driver might need to wait here until dma engine stops.
	 * In this case we can abort imiedetly, so it's just a noop.
	 */

	/* Release all active buffers */
	while (!list_empty(&dma_q->active)) {
		struct vivi_buffer *buf;
		buf = list_entry(dma_q->active.next, struct vivi_buffer, list);
		list_del(&buf->list);
		vb2_buffer_done(&buf->vb, VB2_BUF_STATE_ERROR);
		dprintk(dev, 2, "[%p/%d] done\n", buf, buf->vb.v4l2_buf.index);
	}
}
/* ------------------------------------------------------------------
	Videobuf operations
   ------------------------------------------------------------------*/
static int queue_setup(struct vb2_queue *vq, const struct v4l2_format *fmt,
				unsigned int *nbuffers, unsigned int *nplanes,
				unsigned int sizes[], void *alloc_ctxs[])
{
	struct vivi_dev *dev = vb2_get_drv_priv(vq);
	unsigned long size;

	size = dev->width * dev->height * 2;

	if (0 == *nbuffers)
		*nbuffers = 32;

	while (size * *nbuffers > vid_limit * 1024 * 1024)
		(*nbuffers)--;

	*nplanes = 1;

	sizes[0] = size;

	/*
	 * videobuf2-vmalloc allocator is context-less so no need to set
	 * alloc_ctxs array.
	 */

	dprintk(dev, 1, "%s, count=%d, size=%ld\n", __func__,
		*nbuffers, size);

	return 0;
}

static int buffer_init(struct vb2_buffer *vb)
{
	struct vivi_dev *dev = vb2_get_drv_priv(vb->vb2_queue);

	BUG_ON(NULL == dev->fmt);

	/*
	 * This callback is called once per buffer, after its allocation.
	 *
	 * Vivi does not allow changing format during streaming, but it is
	 * possible to do so when streaming is paused (i.e. in streamoff state).
	 * Buffers however are not freed when going into streamoff and so
	 * buffer size verification has to be done in buffer_prepare, on each
	 * qbuf.
	 * It would be best to move verification code here to buf_init and
	 * s_fmt though.
	 */

	return 0;
}

static int buffer_prepare(struct vb2_buffer *vb)
{
	struct vivi_dev *dev = vb2_get_drv_priv(vb->vb2_queue);
	struct vivi_buffer *buf = container_of(vb, struct vivi_buffer, vb);
	unsigned long size;

	dprintk(dev, 1, "%s, field=%d\n", __func__, vb->v4l2_buf.field);

	BUG_ON(NULL == dev->fmt);

	/*
	 * Theses properties only change when queue is idle, see s_fmt.
	 * The below checks should not be performed here, on each
	 * buffer_prepare (i.e. on each qbuf). Most of the code in this function
	 * should thus be moved to buffer_init and s_fmt.
	 */
	if (dev->width  < 48 || dev->width  > MAX_WIDTH ||
	    dev->height < 32 || dev->height > MAX_HEIGHT)
		return -EINVAL;

	size = dev->width * dev->height * 2;
	if (vb2_plane_size(vb, 0) < size) {
		dprintk(dev, 1, "%s data will not fit into plane (%lu < %lu)\n",
				__func__, vb2_plane_size(vb, 0), size);
		return -EINVAL;
	}

	vb2_set_plane_payload(&buf->vb, 0, size);

	buf->fmt = dev->fmt;

	precalculate_bars(dev);
	precalculate_line(dev);

	return 0;
}

static int buffer_finish(struct vb2_buffer *vb)
{
	struct vivi_dev *dev = vb2_get_drv_priv(vb->vb2_queue);
	dprintk(dev, 1, "%s\n", __func__);
	return 0;
}

static void buffer_cleanup(struct vb2_buffer *vb)
{
	struct vivi_dev *dev = vb2_get_drv_priv(vb->vb2_queue);
	dprintk(dev, 1, "%s\n", __func__);

}

static void buffer_queue(struct vb2_buffer *vb)
{
	struct vivi_dev *dev = vb2_get_drv_priv(vb->vb2_queue);
	struct vivi_buffer *buf = container_of(vb, struct vivi_buffer, vb);
	struct vivi_dmaqueue *vidq = &dev->vidq;
	unsigned long flags = 0;

	dprintk(dev, 1, "%s\n", __func__);

	spin_lock_irqsave(&dev->slock, flags);
	list_add_tail(&buf->list, &vidq->active);
	spin_unlock_irqrestore(&dev->slock, flags);
}

static int start_streaming(struct vb2_queue *vq, unsigned int count)
{
	struct vivi_dev *dev = vb2_get_drv_priv(vq);
	dprintk(dev, 1, "%s\n", __func__);
	return vivi_start_generating(dev);
}

/* abort streaming and wait for last buffer */
static int stop_streaming(struct vb2_queue *vq)
{
	struct vivi_dev *dev = vb2_get_drv_priv(vq);
	dprintk(dev, 1, "%s\n", __func__);
	vivi_stop_generating(dev);
	return 0;
}

static void vivi_lock(struct vb2_queue *vq)
{
	struct vivi_dev *dev = vb2_get_drv_priv(vq);
	mutex_lock(&dev->mutex);
}

static void vivi_unlock(struct vb2_queue *vq)
{
	struct vivi_dev *dev = vb2_get_drv_priv(vq);
	mutex_unlock(&dev->mutex);
}


static struct vb2_ops vivi_video_qops = {
	.queue_setup		= queue_setup,
	.buf_init		= buffer_init,
	.buf_prepare		= buffer_prepare,
	.buf_finish		= buffer_finish,
	.buf_cleanup		= buffer_cleanup,
	.buf_queue		= buffer_queue,
	.start_streaming	= start_streaming,
	.stop_streaming		= stop_streaming,
	.wait_prepare		= vivi_unlock,
	.wait_finish		= vivi_lock,
};

/* ------------------------------------------------------------------
	IOCTL vidioc handling
   ------------------------------------------------------------------*/
static int vidioc_querycap(struct file *file, void  *priv,
					struct v4l2_capability *cap)
{
	struct vivi_dev *dev = video_drvdata(file);

	strcpy(cap->driver, "vivi");
	strcpy(cap->card, "vivi");
	strlcpy(cap->bus_info, dev->v4l2_dev.name, sizeof(cap->bus_info));
	cap->device_caps = V4L2_CAP_VIDEO_CAPTURE | V4L2_CAP_STREAMING |
			    V4L2_CAP_READWRITE;
	cap->capabilities = cap->device_caps | V4L2_CAP_DEVICE_CAPS;
	return 0;
}

static int vidioc_enum_fmt_vid_cap(struct file *file, void  *priv,
					struct v4l2_fmtdesc *f)
{
	struct vivi_fmt *fmt;

	if (f->index >= ARRAY_SIZE(formats))
		return -EINVAL;

	fmt = &formats[f->index];

	strlcpy(f->description, fmt->name, sizeof(f->description));
	f->pixelformat = fmt->fourcc;
	return 0;
}

static int vidioc_g_fmt_vid_cap(struct file *file, void *priv,
					struct v4l2_format *f)
{
	struct vivi_dev *dev = video_drvdata(file);

	f->fmt.pix.width        = dev->width;
	f->fmt.pix.height       = dev->height;
	f->fmt.pix.field        = dev->field;
	f->fmt.pix.pixelformat  = dev->fmt->fourcc;
	f->fmt.pix.bytesperline =
		(f->fmt.pix.width * dev->fmt->depth) >> 3;
	f->fmt.pix.sizeimage =
		f->fmt.pix.height * f->fmt.pix.bytesperline;
	if (dev->fmt->fourcc == V4L2_PIX_FMT_YUYV ||
	    dev->fmt->fourcc == V4L2_PIX_FMT_UYVY)
		f->fmt.pix.colorspace = V4L2_COLORSPACE_SMPTE170M;
	else
		f->fmt.pix.colorspace = V4L2_COLORSPACE_SRGB;
	return 0;
}

static int vidioc_try_fmt_vid_cap(struct file *file, void *priv,
			struct v4l2_format *f)
{
	struct vivi_dev *dev = video_drvdata(file);
	struct vivi_fmt *fmt;
	enum v4l2_field field;

	fmt = get_format(f);
	if (!fmt) {
		dprintk(dev, 1, "Fourcc format (0x%08x) invalid.\n",
			f->fmt.pix.pixelformat);
		return -EINVAL;
	}

	field = f->fmt.pix.field;

	if (field == V4L2_FIELD_ANY) {
		field = V4L2_FIELD_INTERLACED;
	} else if (V4L2_FIELD_INTERLACED != field) {
		dprintk(dev, 1, "Field type invalid.\n");
		return -EINVAL;
	}

	f->fmt.pix.field = field;
	v4l_bound_align_image(&f->fmt.pix.width, 48, MAX_WIDTH, 2,
			      &f->fmt.pix.height, 32, MAX_HEIGHT, 0, 0);
	f->fmt.pix.bytesperline =
		(f->fmt.pix.width * fmt->depth) >> 3;
	f->fmt.pix.sizeimage =
		f->fmt.pix.height * f->fmt.pix.bytesperline;
	if (fmt->fourcc == V4L2_PIX_FMT_YUYV ||
	    fmt->fourcc == V4L2_PIX_FMT_UYVY)
		f->fmt.pix.colorspace = V4L2_COLORSPACE_SMPTE170M;
	else
		f->fmt.pix.colorspace = V4L2_COLORSPACE_SRGB;
	return 0;
}

static int vidioc_s_fmt_vid_cap(struct file *file, void *priv,
					struct v4l2_format *f)
{
	struct vivi_dev *dev = video_drvdata(file);
	struct vb2_queue *q = &dev->vb_vidq;

	int ret = vidioc_try_fmt_vid_cap(file, priv, f);
	if (ret < 0)
		return ret;

	if (vb2_is_streaming(q)) {
		dprintk(dev, 1, "%s device busy\n", __func__);
		return -EBUSY;
	}

	dev->fmt = get_format(f);
	dev->width = f->fmt.pix.width;
	dev->height = f->fmt.pix.height;
	dev->field = f->fmt.pix.field;

	return 0;
}

static int vidioc_reqbufs(struct file *file, void *priv,
			  struct v4l2_requestbuffers *p)
{
	struct vivi_dev *dev = video_drvdata(file);
	return vb2_reqbufs(&dev->vb_vidq, p);
}

static int vidioc_querybuf(struct file *file, void *priv, struct v4l2_buffer *p)
{
	struct vivi_dev *dev = video_drvdata(file);
	return vb2_querybuf(&dev->vb_vidq, p);
}

static int vidioc_qbuf(struct file *file, void *priv, struct v4l2_buffer *p)
{
	struct vivi_dev *dev = video_drvdata(file);
	return vb2_qbuf(&dev->vb_vidq, p);
}

static int vidioc_dqbuf(struct file *file, void *priv, struct v4l2_buffer *p)
{
	struct vivi_dev *dev = video_drvdata(file);
	return vb2_dqbuf(&dev->vb_vidq, p, file->f_flags & O_NONBLOCK);
}

static int vidioc_streamon(struct file *file, void *priv, enum v4l2_buf_type i)
{
	struct vivi_dev *dev = video_drvdata(file);
	return vb2_streamon(&dev->vb_vidq, i);
}

static int vidioc_streamoff(struct file *file, void *priv, enum v4l2_buf_type i)
{
	struct vivi_dev *dev = video_drvdata(file);
	return vb2_streamoff(&dev->vb_vidq, i);
}

static int vidioc_s_std(struct file *file, void *priv, v4l2_std_id *i)
{
	return 0;
}

/* only one input in this sample driver */
static int vidioc_enum_input(struct file *file, void *priv,
				struct v4l2_input *inp)
{
	if (inp->index >= NUM_INPUTS)
		return -EINVAL;

	inp->type = V4L2_INPUT_TYPE_CAMERA;
	inp->std = V4L2_STD_525_60;
	sprintf(inp->name, "Camera %u", inp->index);
	return 0;
}

static int vidioc_g_input(struct file *file, void *priv, unsigned int *i)
{
	struct vivi_dev *dev = video_drvdata(file);

	*i = dev->input;
	return 0;
}

static int vidioc_s_input(struct file *file, void *priv, unsigned int i)
{
	struct vivi_dev *dev = video_drvdata(file);

	if (i >= NUM_INPUTS)
		return -EINVAL;

	if (i == dev->input)
		return 0;

	dev->input = i;
	precalculate_bars(dev);
	precalculate_line(dev);
	return 0;
}

/* --- controls ---------------------------------------------- */

static int vivi_g_volatile_ctrl(struct v4l2_ctrl *ctrl)
{
	struct vivi_dev *dev = container_of(ctrl->handler, struct vivi_dev, ctrl_handler);

	if (ctrl == dev->autogain)
		dev->gain->val = jiffies & 0xff;
	return 0;
}

static int vivi_s_ctrl(struct v4l2_ctrl *ctrl)
{
	struct vivi_dev *dev = container_of(ctrl->handler, struct vivi_dev, ctrl_handler);

	if (ctrl == dev->button)
		dev->button_pressed = 30;
	return 0;
}

/* ------------------------------------------------------------------
	File operations for the device
   ------------------------------------------------------------------*/

static ssize_t
vivi_read(struct file *file, char __user *data, size_t count, loff_t *ppos)
{
	struct vivi_dev *dev = video_drvdata(file);

	dprintk(dev, 1, "read called\n");
	return vb2_read(&dev->vb_vidq, data, count, ppos,
		       file->f_flags & O_NONBLOCK);
}

static unsigned int
vivi_poll(struct file *file, struct poll_table_struct *wait)
{
	struct vivi_dev *dev = video_drvdata(file);
	struct v4l2_fh *fh = file->private_data;
	struct vb2_queue *q = &dev->vb_vidq;
	unsigned int res;

	dprintk(dev, 1, "%s\n", __func__);
	res = vb2_poll(q, file, wait);
	if (v4l2_event_pending(fh))
		res |= POLLPRI;
	else
		poll_wait(file, &fh->wait, wait);
	return res;
}

static int vivi_close(struct file *file)
{
	struct video_device  *vdev = video_devdata(file);
	struct vivi_dev *dev = video_drvdata(file);

	dprintk(dev, 1, "close called (dev=%s), file %p\n",
		video_device_node_name(vdev), file);

	if (v4l2_fh_is_singular_file(file))
		vb2_queue_release(&dev->vb_vidq);
	return v4l2_fh_release(file);
}

static int vivi_mmap(struct file *file, struct vm_area_struct *vma)
{
	struct vivi_dev *dev = video_drvdata(file);
	int ret;

	dprintk(dev, 1, "mmap called, vma=0x%08lx\n", (unsigned long)vma);

	ret = vb2_mmap(&dev->vb_vidq, vma);
	dprintk(dev, 1, "vma start=0x%08lx, size=%ld, ret=%d\n",
		(unsigned long)vma->vm_start,
		(unsigned long)vma->vm_end - (unsigned long)vma->vm_start,
		ret);
	return ret;
}

static const struct v4l2_ctrl_ops vivi_ctrl_ops = {
	.g_volatile_ctrl = vivi_g_volatile_ctrl,
	.s_ctrl = vivi_s_ctrl,
};

#define VIVI_CID_CUSTOM_BASE	(V4L2_CID_USER_BASE | 0xf000)

static const struct v4l2_ctrl_config vivi_ctrl_button = {
	.ops = &vivi_ctrl_ops,
	.id = VIVI_CID_CUSTOM_BASE + 0,
	.name = "Button",
	.type = V4L2_CTRL_TYPE_BUTTON,
};

static const struct v4l2_ctrl_config vivi_ctrl_boolean = {
	.ops = &vivi_ctrl_ops,
	.id = VIVI_CID_CUSTOM_BASE + 1,
	.name = "Boolean",
	.type = V4L2_CTRL_TYPE_BOOLEAN,
	.min = 0,
	.max = 1,
	.step = 1,
	.def = 1,
};

static const struct v4l2_ctrl_config vivi_ctrl_int32 = {
	.ops = &vivi_ctrl_ops,
	.id = VIVI_CID_CUSTOM_BASE + 2,
	.name = "Integer 32 Bits",
	.type = V4L2_CTRL_TYPE_INTEGER,
	.min = 0x80000000,
	.max = 0x7fffffff,
	.step = 1,
};

static const struct v4l2_ctrl_config vivi_ctrl_int64 = {
	.ops = &vivi_ctrl_ops,
	.id = VIVI_CID_CUSTOM_BASE + 3,
	.name = "Integer 64 Bits",
	.type = V4L2_CTRL_TYPE_INTEGER64,
};

static const char * const vivi_ctrl_menu_strings[] = {
	"Menu Item 0 (Skipped)",
	"Menu Item 1",
	"Menu Item 2 (Skipped)",
	"Menu Item 3",
	"Menu Item 4",
	"Menu Item 5 (Skipped)",
	NULL,
};

static const struct v4l2_ctrl_config vivi_ctrl_menu = {
	.ops = &vivi_ctrl_ops,
	.id = VIVI_CID_CUSTOM_BASE + 4,
	.name = "Menu",
	.type = V4L2_CTRL_TYPE_MENU,
	.min = 1,
	.max = 4,
	.def = 3,
	.menu_skip_mask = 0x04,
	.qmenu = vivi_ctrl_menu_strings,
};

static const struct v4l2_ctrl_config vivi_ctrl_string = {
	.ops = &vivi_ctrl_ops,
	.id = VIVI_CID_CUSTOM_BASE + 5,
	.name = "String",
	.type = V4L2_CTRL_TYPE_STRING,
	.min = 2,
	.max = 4,
	.step = 1,
};

static const struct v4l2_ctrl_config vivi_ctrl_bitmask = {
	.ops = &vivi_ctrl_ops,
	.id = VIVI_CID_CUSTOM_BASE + 6,
	.name = "Bitmask",
	.type = V4L2_CTRL_TYPE_BITMASK,
	.def = 0x80002000,
	.min = 0,
	.max = 0x80402010,
	.step = 0,
};

static const struct v4l2_file_operations vivi_fops = {
	.owner		= THIS_MODULE,
	.open           = v4l2_fh_open,
	.release        = vivi_close,
	.read           = vivi_read,
	.poll		= vivi_poll,
	.unlocked_ioctl = video_ioctl2, /* V4L2 ioctl handler */
	.mmap           = vivi_mmap,
};

static const struct v4l2_ioctl_ops vivi_ioctl_ops = {
	.vidioc_querycap      = vidioc_querycap,
	.vidioc_enum_fmt_vid_cap  = vidioc_enum_fmt_vid_cap,
	.vidioc_g_fmt_vid_cap     = vidioc_g_fmt_vid_cap,
	.vidioc_try_fmt_vid_cap   = vidioc_try_fmt_vid_cap,
	.vidioc_s_fmt_vid_cap     = vidioc_s_fmt_vid_cap,
	.vidioc_reqbufs       = vidioc_reqbufs,
	.vidioc_querybuf      = vidioc_querybuf,
	.vidioc_qbuf          = vidioc_qbuf,
	.vidioc_dqbuf         = vidioc_dqbuf,
	.vidioc_s_std         = vidioc_s_std,
	.vidioc_enum_input    = vidioc_enum_input,
	.vidioc_g_input       = vidioc_g_input,
	.vidioc_s_input       = vidioc_s_input,
	.vidioc_streamon      = vidioc_streamon,
	.vidioc_streamoff     = vidioc_streamoff,
	.vidioc_log_status    = v4l2_ctrl_log_status,
	.vidioc_subscribe_event = v4l2_ctrl_subscribe_event,
	.vidioc_unsubscribe_event = v4l2_event_unsubscribe,
};

static struct video_device vivi_template = {
	.name		= "vivi",
	.fops           = &vivi_fops,
	.ioctl_ops 	= &vivi_ioctl_ops,
	.release	= video_device_release,

	.tvnorms              = V4L2_STD_525_60,
	.current_norm         = V4L2_STD_NTSC_M,
};

/* -----------------------------------------------------------------
	Initialization and module stuff
   ------------------------------------------------------------------*/

static int vivi_release(void)
{
	struct vivi_dev *dev;
	struct list_head *list;

	while (!list_empty(&vivi_devlist)) {
		list = vivi_devlist.next;
		list_del(list);
		dev = list_entry(list, struct vivi_dev, vivi_devlist);

		v4l2_info(&dev->v4l2_dev, "unregistering %s\n",
			video_device_node_name(dev->vfd));
		video_unregister_device(dev->vfd);
		v4l2_device_unregister(&dev->v4l2_dev);
		v4l2_ctrl_handler_free(&dev->ctrl_handler);
		kfree(dev);
	}

	return 0;
}

static int __init vivi_create_instance(int inst)
{
	struct vivi_dev *dev;
	struct video_device *vfd;
	struct v4l2_ctrl_handler *hdl;
	struct vb2_queue *q;
	int ret;

	dev = kzalloc(sizeof(*dev), GFP_KERNEL);
	if (!dev)
		return -ENOMEM;

	snprintf(dev->v4l2_dev.name, sizeof(dev->v4l2_dev.name),
			"%s-%03d", VIVI_MODULE_NAME, inst);
	ret = v4l2_device_register(NULL, &dev->v4l2_dev);
	if (ret)
		goto free_dev;

	dev->fmt = &formats[0];
	dev->width = 640;
	dev->height = 480;
	hdl = &dev->ctrl_handler;
	v4l2_ctrl_handler_init(hdl, 11);
	dev->volume = v4l2_ctrl_new_std(hdl, &vivi_ctrl_ops,
			V4L2_CID_AUDIO_VOLUME, 0, 255, 1, 200);
	dev->brightness = v4l2_ctrl_new_std(hdl, &vivi_ctrl_ops,
			V4L2_CID_BRIGHTNESS, 0, 255, 1, 127);
	dev->contrast = v4l2_ctrl_new_std(hdl, &vivi_ctrl_ops,
			V4L2_CID_CONTRAST, 0, 255, 1, 16);
	dev->saturation = v4l2_ctrl_new_std(hdl, &vivi_ctrl_ops,
			V4L2_CID_SATURATION, 0, 255, 1, 127);
	dev->hue = v4l2_ctrl_new_std(hdl, &vivi_ctrl_ops,
			V4L2_CID_HUE, -128, 127, 1, 0);
	dev->autogain = v4l2_ctrl_new_std(hdl, &vivi_ctrl_ops,
			V4L2_CID_AUTOGAIN, 0, 1, 1, 1);
	dev->gain = v4l2_ctrl_new_std(hdl, &vivi_ctrl_ops,
			V4L2_CID_GAIN, 0, 255, 1, 100);
	dev->button = v4l2_ctrl_new_custom(hdl, &vivi_ctrl_button, NULL);
	dev->int32 = v4l2_ctrl_new_custom(hdl, &vivi_ctrl_int32, NULL);
	dev->int64 = v4l2_ctrl_new_custom(hdl, &vivi_ctrl_int64, NULL);
	dev->boolean = v4l2_ctrl_new_custom(hdl, &vivi_ctrl_boolean, NULL);
	dev->menu = v4l2_ctrl_new_custom(hdl, &vivi_ctrl_menu, NULL);
	dev->string = v4l2_ctrl_new_custom(hdl, &vivi_ctrl_string, NULL);
	dev->bitmask = v4l2_ctrl_new_custom(hdl, &vivi_ctrl_bitmask, NULL);
	if (hdl->error) {
		ret = hdl->error;
		goto unreg_dev;
	}
	v4l2_ctrl_auto_cluster(2, &dev->autogain, 0, true);
	dev->v4l2_dev.ctrl_handler = hdl;

	/* initialize locks */
	spin_lock_init(&dev->slock);

	/* initialize queue */
	q = &dev->vb_vidq;
	memset(q, 0, sizeof(dev->vb_vidq));
	q->type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	q->io_modes = VB2_MMAP | VB2_USERPTR | VB2_READ;
	q->drv_priv = dev;
	q->buf_struct_size = sizeof(struct vivi_buffer);
	q->ops = &vivi_video_qops;
	q->mem_ops = &vb2_vmalloc_memops;

	vb2_queue_init(q);

	mutex_init(&dev->mutex);

	/* init video dma queues */
	INIT_LIST_HEAD(&dev->vidq.active);
	init_waitqueue_head(&dev->vidq.wq);

	ret = -ENOMEM;
	vfd = video_device_alloc();
	if (!vfd)
		goto unreg_dev;

	*vfd = vivi_template;
	vfd->debug = debug;
	vfd->v4l2_dev = &dev->v4l2_dev;
	set_bit(V4L2_FL_USE_FH_PRIO, &vfd->flags);

	/*
	 * Provide a mutex to v4l2 core. It will be used to protect
	 * all fops and v4l2 ioctls.
	 */
	vfd->lock = &dev->mutex;

	ret = video_register_device(vfd, VFL_TYPE_GRABBER, video_nr);
	if (ret < 0)
		goto rel_vdev;

	video_set_drvdata(vfd, dev);

	/* Now that everything is fine, let's add it to device list */
	list_add_tail(&dev->vivi_devlist, &vivi_devlist);

	if (video_nr != -1)
		video_nr++;

	dev->vfd = vfd;
	v4l2_info(&dev->v4l2_dev, "V4L2 device registered as %s\n",
		  video_device_node_name(vfd));
	return 0;

rel_vdev:
	video_device_release(vfd);
unreg_dev:
	v4l2_ctrl_handler_free(hdl);
	v4l2_device_unregister(&dev->v4l2_dev);
free_dev:
	kfree(dev);
	return ret;
}

static const unsigned char fontdata_8x16_[4096] = { // FONTDATAMAX=4096

	/* 0 0x00 '^@' */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 1 0x01 '^A' */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x7e, /* 01111110 */
	0x81, /* 10000001 */
	0xa5, /* 10100101 */
	0x81, /* 10000001 */
	0x81, /* 10000001 */
	0xbd, /* 10111101 */
	0x99, /* 10011001 */
	0x81, /* 10000001 */
	0x81, /* 10000001 */
	0x7e, /* 01111110 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 2 0x02 '^B' */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x7e, /* 01111110 */
	0xff, /* 11111111 */
	0xdb, /* 11011011 */
	0xff, /* 11111111 */
	0xff, /* 11111111 */
	0xc3, /* 11000011 */
	0xe7, /* 11100111 */
	0xff, /* 11111111 */
	0xff, /* 11111111 */
	0x7e, /* 01111110 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 3 0x03 '^C' */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x6c, /* 01101100 */
	0xfe, /* 11111110 */
	0xfe, /* 11111110 */
	0xfe, /* 11111110 */
	0xfe, /* 11111110 */
	0x7c, /* 01111100 */
	0x38, /* 00111000 */
	0x10, /* 00010000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 4 0x04 '^D' */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x10, /* 00010000 */
	0x38, /* 00111000 */
	0x7c, /* 01111100 */
	0xfe, /* 11111110 */
	0x7c, /* 01111100 */
	0x38, /* 00111000 */
	0x10, /* 00010000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 5 0x05 '^E' */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x18, /* 00011000 */
	0x3c, /* 00111100 */
	0x3c, /* 00111100 */
	0xe7, /* 11100111 */
	0xe7, /* 11100111 */
	0xe7, /* 11100111 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x3c, /* 00111100 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 6 0x06 '^F' */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x18, /* 00011000 */
	0x3c, /* 00111100 */
	0x7e, /* 01111110 */
	0xff, /* 11111111 */
	0xff, /* 11111111 */
	0x7e, /* 01111110 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x3c, /* 00111100 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 7 0x07 '^G' */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x18, /* 00011000 */
	0x3c, /* 00111100 */
	0x3c, /* 00111100 */
	0x18, /* 00011000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 8 0x08 '^H' */
	0xff, /* 11111111 */
	0xff, /* 11111111 */
	0xff, /* 11111111 */
	0xff, /* 11111111 */
	0xff, /* 11111111 */
	0xff, /* 11111111 */
	0xe7, /* 11100111 */
	0xc3, /* 11000011 */
	0xc3, /* 11000011 */
	0xe7, /* 11100111 */
	0xff, /* 11111111 */
	0xff, /* 11111111 */
	0xff, /* 11111111 */
	0xff, /* 11111111 */
	0xff, /* 11111111 */
	0xff, /* 11111111 */

	/* 9 0x09 '^I' */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x3c, /* 00111100 */
	0x66, /* 01100110 */
	0x42, /* 01000010 */
	0x42, /* 01000010 */
	0x66, /* 01100110 */
	0x3c, /* 00111100 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 10 0x0a '^J' */
	0xff, /* 11111111 */
	0xff, /* 11111111 */
	0xff, /* 11111111 */
	0xff, /* 11111111 */
	0xff, /* 11111111 */
	0xc3, /* 11000011 */
	0x99, /* 10011001 */
	0xbd, /* 10111101 */
	0xbd, /* 10111101 */
	0x99, /* 10011001 */
	0xc3, /* 11000011 */
	0xff, /* 11111111 */
	0xff, /* 11111111 */
	0xff, /* 11111111 */
	0xff, /* 11111111 */
	0xff, /* 11111111 */

	/* 11 0x0b '^K' */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x1e, /* 00011110 */
	0x0e, /* 00001110 */
	0x1a, /* 00011010 */
	0x32, /* 00110010 */
	0x78, /* 01111000 */
	0xcc, /* 11001100 */
	0xcc, /* 11001100 */
	0xcc, /* 11001100 */
	0xcc, /* 11001100 */
	0x78, /* 01111000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 12 0x0c '^L' */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x3c, /* 00111100 */
	0x66, /* 01100110 */
	0x66, /* 01100110 */
	0x66, /* 01100110 */
	0x66, /* 01100110 */
	0x3c, /* 00111100 */
	0x18, /* 00011000 */
	0x7e, /* 01111110 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 13 0x0d '^M' */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x3f, /* 00111111 */
	0x33, /* 00110011 */
	0x3f, /* 00111111 */
	0x30, /* 00110000 */
	0x30, /* 00110000 */
	0x30, /* 00110000 */
	0x30, /* 00110000 */
	0x70, /* 01110000 */
	0xf0, /* 11110000 */
	0xe0, /* 11100000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 14 0x0e '^N' */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x7f, /* 01111111 */
	0x63, /* 01100011 */
	0x7f, /* 01111111 */
	0x63, /* 01100011 */
	0x63, /* 01100011 */
	0x63, /* 01100011 */
	0x63, /* 01100011 */
	0x67, /* 01100111 */
	0xe7, /* 11100111 */
	0xe6, /* 11100110 */
	0xc0, /* 11000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 15 0x0f '^O' */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0xdb, /* 11011011 */
	0x3c, /* 00111100 */
	0xe7, /* 11100111 */
	0x3c, /* 00111100 */
	0xdb, /* 11011011 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 16 0x10 '^P' */
	0x00, /* 00000000 */
	0x80, /* 10000000 */
	0xc0, /* 11000000 */
	0xe0, /* 11100000 */
	0xf0, /* 11110000 */
	0xf8, /* 11111000 */
	0xfe, /* 11111110 */
	0xf8, /* 11111000 */
	0xf0, /* 11110000 */
	0xe0, /* 11100000 */
	0xc0, /* 11000000 */
	0x80, /* 10000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 17 0x11 '^Q' */
	0x00, /* 00000000 */
	0x02, /* 00000010 */
	0x06, /* 00000110 */
	0x0e, /* 00001110 */
	0x1e, /* 00011110 */
	0x3e, /* 00111110 */
	0xfe, /* 11111110 */
	0x3e, /* 00111110 */
	0x1e, /* 00011110 */
	0x0e, /* 00001110 */
	0x06, /* 00000110 */
	0x02, /* 00000010 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 18 0x12 '^R' */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x18, /* 00011000 */
	0x3c, /* 00111100 */
	0x7e, /* 01111110 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x7e, /* 01111110 */
	0x3c, /* 00111100 */
	0x18, /* 00011000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 19 0x13 '^S' */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x66, /* 01100110 */
	0x66, /* 01100110 */
	0x66, /* 01100110 */
	0x66, /* 01100110 */
	0x66, /* 01100110 */
	0x66, /* 01100110 */
	0x66, /* 01100110 */
	0x00, /* 00000000 */
	0x66, /* 01100110 */
	0x66, /* 01100110 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 20 0x14 '^T' */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x7f, /* 01111111 */
	0xdb, /* 11011011 */
	0xdb, /* 11011011 */
	0xdb, /* 11011011 */
	0x7b, /* 01111011 */
	0x1b, /* 00011011 */
	0x1b, /* 00011011 */
	0x1b, /* 00011011 */
	0x1b, /* 00011011 */
	0x1b, /* 00011011 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 21 0x15 '^U' */
	0x00, /* 00000000 */
	0x7c, /* 01111100 */
	0xc6, /* 11000110 */
	0x60, /* 01100000 */
	0x38, /* 00111000 */
	0x6c, /* 01101100 */
	0xc6, /* 11000110 */
	0xc6, /* 11000110 */
	0x6c, /* 01101100 */
	0x38, /* 00111000 */
	0x0c, /* 00001100 */
	0xc6, /* 11000110 */
	0x7c, /* 01111100 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 22 0x16 '^V' */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0xfe, /* 11111110 */
	0xfe, /* 11111110 */
	0xfe, /* 11111110 */
	0xfe, /* 11111110 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 23 0x17 '^W' */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x18, /* 00011000 */
	0x3c, /* 00111100 */
	0x7e, /* 01111110 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x7e, /* 01111110 */
	0x3c, /* 00111100 */
	0x18, /* 00011000 */
	0x7e, /* 01111110 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 24 0x18 '^X' */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x18, /* 00011000 */
	0x3c, /* 00111100 */
	0x7e, /* 01111110 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 25 0x19 '^Y' */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x7e, /* 01111110 */
	0x3c, /* 00111100 */
	0x18, /* 00011000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 26 0x1a '^Z' */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x18, /* 00011000 */
	0x0c, /* 00001100 */
	0xfe, /* 11111110 */
	0x0c, /* 00001100 */
	0x18, /* 00011000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 27 0x1b '^[' */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x30, /* 00110000 */
	0x60, /* 01100000 */
	0xfe, /* 11111110 */
	0x60, /* 01100000 */
	0x30, /* 00110000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 28 0x1c '^\' */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0xc0, /* 11000000 */
	0xc0, /* 11000000 */
	0xc0, /* 11000000 */
	0xfe, /* 11111110 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 29 0x1d '^]' */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x28, /* 00101000 */
	0x6c, /* 01101100 */
	0xfe, /* 11111110 */
	0x6c, /* 01101100 */
	0x28, /* 00101000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 30 0x1e '^^' */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x10, /* 00010000 */
	0x38, /* 00111000 */
	0x38, /* 00111000 */
	0x7c, /* 01111100 */
	0x7c, /* 01111100 */
	0xfe, /* 11111110 */
	0xfe, /* 11111110 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 31 0x1f '^_' */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0xfe, /* 11111110 */
	0xfe, /* 11111110 */
	0x7c, /* 01111100 */
	0x7c, /* 01111100 */
	0x38, /* 00111000 */
	0x38, /* 00111000 */
	0x10, /* 00010000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 32 0x20 ' ' */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 33 0x21 '!' */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x18, /* 00011000 */
	0x3c, /* 00111100 */
	0x3c, /* 00111100 */
	0x3c, /* 00111100 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x00, /* 00000000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 34 0x22 '"' */
	0x00, /* 00000000 */
	0x66, /* 01100110 */
	0x66, /* 01100110 */
	0x66, /* 01100110 */
	0x24, /* 00100100 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 35 0x23 '#' */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x6c, /* 01101100 */
	0x6c, /* 01101100 */
	0xfe, /* 11111110 */
	0x6c, /* 01101100 */
	0x6c, /* 01101100 */
	0x6c, /* 01101100 */
	0xfe, /* 11111110 */
	0x6c, /* 01101100 */
	0x6c, /* 01101100 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 36 0x24 '$' */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x7c, /* 01111100 */
	0xc6, /* 11000110 */
	0xc2, /* 11000010 */
	0xc0, /* 11000000 */
	0x7c, /* 01111100 */
	0x06, /* 00000110 */
	0x06, /* 00000110 */
	0x86, /* 10000110 */
	0xc6, /* 11000110 */
	0x7c, /* 01111100 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 37 0x25 '%' */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0xc2, /* 11000010 */
	0xc6, /* 11000110 */
	0x0c, /* 00001100 */
	0x18, /* 00011000 */
	0x30, /* 00110000 */
	0x60, /* 01100000 */
	0xc6, /* 11000110 */
	0x86, /* 10000110 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 38 0x26 '&' */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x38, /* 00111000 */
	0x6c, /* 01101100 */
	0x6c, /* 01101100 */
	0x38, /* 00111000 */
	0x76, /* 01110110 */
	0xdc, /* 11011100 */
	0xcc, /* 11001100 */
	0xcc, /* 11001100 */
	0xcc, /* 11001100 */
	0x76, /* 01110110 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 39 0x27 ''' */
	0x00, /* 00000000 */
	0x30, /* 00110000 */
	0x30, /* 00110000 */
	0x30, /* 00110000 */
	0x60, /* 01100000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 40 0x28 '(' */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x0c, /* 00001100 */
	0x18, /* 00011000 */
	0x30, /* 00110000 */
	0x30, /* 00110000 */
	0x30, /* 00110000 */
	0x30, /* 00110000 */
	0x30, /* 00110000 */
	0x30, /* 00110000 */
	0x18, /* 00011000 */
	0x0c, /* 00001100 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 41 0x29 ')' */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x30, /* 00110000 */
	0x18, /* 00011000 */
	0x0c, /* 00001100 */
	0x0c, /* 00001100 */
	0x0c, /* 00001100 */
	0x0c, /* 00001100 */
	0x0c, /* 00001100 */
	0x0c, /* 00001100 */
	0x18, /* 00011000 */
	0x30, /* 00110000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 42 0x2a '*' */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x66, /* 01100110 */
	0x3c, /* 00111100 */
	0xff, /* 11111111 */
	0x3c, /* 00111100 */
	0x66, /* 01100110 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 43 0x2b '+' */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x7e, /* 01111110 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 44 0x2c ',' */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x30, /* 00110000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 45 0x2d '-' */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0xfe, /* 11111110 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 46 0x2e '.' */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 47 0x2f '/' */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x02, /* 00000010 */
	0x06, /* 00000110 */
	0x0c, /* 00001100 */
	0x18, /* 00011000 */
	0x30, /* 00110000 */
	0x60, /* 01100000 */
	0xc0, /* 11000000 */
	0x80, /* 10000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 48 0x30 '0' */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x38, /* 00111000 */
	0x6c, /* 01101100 */
	0xc6, /* 11000110 */
	0xc6, /* 11000110 */
	0xd6, /* 11010110 */
	0xd6, /* 11010110 */
	0xc6, /* 11000110 */
	0xc6, /* 11000110 */
	0x6c, /* 01101100 */
	0x38, /* 00111000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 49 0x31 '1' */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x18, /* 00011000 */
	0x38, /* 00111000 */
	0x78, /* 01111000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x7e, /* 01111110 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 50 0x32 '2' */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x7c, /* 01111100 */
	0xc6, /* 11000110 */
	0x06, /* 00000110 */
	0x0c, /* 00001100 */
	0x18, /* 00011000 */
	0x30, /* 00110000 */
	0x60, /* 01100000 */
	0xc0, /* 11000000 */
	0xc6, /* 11000110 */
	0xfe, /* 11111110 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 51 0x33 '3' */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x7c, /* 01111100 */
	0xc6, /* 11000110 */
	0x06, /* 00000110 */
	0x06, /* 00000110 */
	0x3c, /* 00111100 */
	0x06, /* 00000110 */
	0x06, /* 00000110 */
	0x06, /* 00000110 */
	0xc6, /* 11000110 */
	0x7c, /* 01111100 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 52 0x34 '4' */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x0c, /* 00001100 */
	0x1c, /* 00011100 */
	0x3c, /* 00111100 */
	0x6c, /* 01101100 */
	0xcc, /* 11001100 */
	0xfe, /* 11111110 */
	0x0c, /* 00001100 */
	0x0c, /* 00001100 */
	0x0c, /* 00001100 */
	0x1e, /* 00011110 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 53 0x35 '5' */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0xfe, /* 11111110 */
	0xc0, /* 11000000 */
	0xc0, /* 11000000 */
	0xc0, /* 11000000 */
	0xfc, /* 11111100 */
	0x06, /* 00000110 */
	0x06, /* 00000110 */
	0x06, /* 00000110 */
	0xc6, /* 11000110 */
	0x7c, /* 01111100 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 54 0x36 '6' */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x38, /* 00111000 */
	0x60, /* 01100000 */
	0xc0, /* 11000000 */
	0xc0, /* 11000000 */
	0xfc, /* 11111100 */
	0xc6, /* 11000110 */
	0xc6, /* 11000110 */
	0xc6, /* 11000110 */
	0xc6, /* 11000110 */
	0x7c, /* 01111100 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 55 0x37 '7' */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0xfe, /* 11111110 */
	0xc6, /* 11000110 */
	0x06, /* 00000110 */
	0x06, /* 00000110 */
	0x0c, /* 00001100 */
	0x18, /* 00011000 */
	0x30, /* 00110000 */
	0x30, /* 00110000 */
	0x30, /* 00110000 */
	0x30, /* 00110000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 56 0x38 '8' */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x7c, /* 01111100 */
	0xc6, /* 11000110 */
	0xc6, /* 11000110 */
	0xc6, /* 11000110 */
	0x7c, /* 01111100 */
	0xc6, /* 11000110 */
	0xc6, /* 11000110 */
	0xc6, /* 11000110 */
	0xc6, /* 11000110 */
	0x7c, /* 01111100 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 57 0x39 '9' */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x7c, /* 01111100 */
	0xc6, /* 11000110 */
	0xc6, /* 11000110 */
	0xc6, /* 11000110 */
	0x7e, /* 01111110 */
	0x06, /* 00000110 */
	0x06, /* 00000110 */
	0x06, /* 00000110 */
	0x0c, /* 00001100 */
	0x78, /* 01111000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 58 0x3a ':' */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 59 0x3b ';' */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x30, /* 00110000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 60 0x3c '<' */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x06, /* 00000110 */
	0x0c, /* 00001100 */
	0x18, /* 00011000 */
	0x30, /* 00110000 */
	0x60, /* 01100000 */
	0x30, /* 00110000 */
	0x18, /* 00011000 */
	0x0c, /* 00001100 */
	0x06, /* 00000110 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 61 0x3d '=' */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x7e, /* 01111110 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x7e, /* 01111110 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 62 0x3e '>' */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x60, /* 01100000 */
	0x30, /* 00110000 */
	0x18, /* 00011000 */
	0x0c, /* 00001100 */
	0x06, /* 00000110 */
	0x0c, /* 00001100 */
	0x18, /* 00011000 */
	0x30, /* 00110000 */
	0x60, /* 01100000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 63 0x3f '?' */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x7c, /* 01111100 */
	0xc6, /* 11000110 */
	0xc6, /* 11000110 */
	0x0c, /* 00001100 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x00, /* 00000000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 64 0x40 '@' */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x7c, /* 01111100 */
	0xc6, /* 11000110 */
	0xc6, /* 11000110 */
	0xde, /* 11011110 */
	0xde, /* 11011110 */
	0xde, /* 11011110 */
	0xdc, /* 11011100 */
	0xc0, /* 11000000 */
	0x7c, /* 01111100 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 65 0x41 'A' */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x10, /* 00010000 */
	0x38, /* 00111000 */
	0x6c, /* 01101100 */
	0xc6, /* 11000110 */
	0xc6, /* 11000110 */
	0xfe, /* 11111110 */
	0xc6, /* 11000110 */
	0xc6, /* 11000110 */
	0xc6, /* 11000110 */
	0xc6, /* 11000110 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 66 0x42 'B' */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0xfc, /* 11111100 */
	0x66, /* 01100110 */
	0x66, /* 01100110 */
	0x66, /* 01100110 */
	0x7c, /* 01111100 */
	0x66, /* 01100110 */
	0x66, /* 01100110 */
	0x66, /* 01100110 */
	0x66, /* 01100110 */
	0xfc, /* 11111100 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 67 0x43 'C' */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x3c, /* 00111100 */
	0x66, /* 01100110 */
	0xc2, /* 11000010 */
	0xc0, /* 11000000 */
	0xc0, /* 11000000 */
	0xc0, /* 11000000 */
	0xc0, /* 11000000 */
	0xc2, /* 11000010 */
	0x66, /* 01100110 */
	0x3c, /* 00111100 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 68 0x44 'D' */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0xf8, /* 11111000 */
	0x6c, /* 01101100 */
	0x66, /* 01100110 */
	0x66, /* 01100110 */
	0x66, /* 01100110 */
	0x66, /* 01100110 */
	0x66, /* 01100110 */
	0x66, /* 01100110 */
	0x6c, /* 01101100 */
	0xf8, /* 11111000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 69 0x45 'E' */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0xfe, /* 11111110 */
	0x66, /* 01100110 */
	0x62, /* 01100010 */
	0x68, /* 01101000 */
	0x78, /* 01111000 */
	0x68, /* 01101000 */
	0x60, /* 01100000 */
	0x62, /* 01100010 */
	0x66, /* 01100110 */
	0xfe, /* 11111110 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 70 0x46 'F' */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0xfe, /* 11111110 */
	0x66, /* 01100110 */
	0x62, /* 01100010 */
	0x68, /* 01101000 */
	0x78, /* 01111000 */
	0x68, /* 01101000 */
	0x60, /* 01100000 */
	0x60, /* 01100000 */
	0x60, /* 01100000 */
	0xf0, /* 11110000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 71 0x47 'G' */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x3c, /* 00111100 */
	0x66, /* 01100110 */
	0xc2, /* 11000010 */
	0xc0, /* 11000000 */
	0xc0, /* 11000000 */
	0xde, /* 11011110 */
	0xc6, /* 11000110 */
	0xc6, /* 11000110 */
	0x66, /* 01100110 */
	0x3a, /* 00111010 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 72 0x48 'H' */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0xc6, /* 11000110 */
	0xc6, /* 11000110 */
	0xc6, /* 11000110 */
	0xc6, /* 11000110 */
	0xfe, /* 11111110 */
	0xc6, /* 11000110 */
	0xc6, /* 11000110 */
	0xc6, /* 11000110 */
	0xc6, /* 11000110 */
	0xc6, /* 11000110 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 73 0x49 'I' */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x3c, /* 00111100 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x3c, /* 00111100 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 74 0x4a 'J' */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x1e, /* 00011110 */
	0x0c, /* 00001100 */
	0x0c, /* 00001100 */
	0x0c, /* 00001100 */
	0x0c, /* 00001100 */
	0x0c, /* 00001100 */
	0xcc, /* 11001100 */
	0xcc, /* 11001100 */
	0xcc, /* 11001100 */
	0x78, /* 01111000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 75 0x4b 'K' */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0xe6, /* 11100110 */
	0x66, /* 01100110 */
	0x66, /* 01100110 */
	0x6c, /* 01101100 */
	0x78, /* 01111000 */
	0x78, /* 01111000 */
	0x6c, /* 01101100 */
	0x66, /* 01100110 */
	0x66, /* 01100110 */
	0xe6, /* 11100110 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 76 0x4c 'L' */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0xf0, /* 11110000 */
	0x60, /* 01100000 */
	0x60, /* 01100000 */
	0x60, /* 01100000 */
	0x60, /* 01100000 */
	0x60, /* 01100000 */
	0x60, /* 01100000 */
	0x62, /* 01100010 */
	0x66, /* 01100110 */
	0xfe, /* 11111110 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 77 0x4d 'M' */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0xc6, /* 11000110 */
	0xee, /* 11101110 */
	0xfe, /* 11111110 */
	0xfe, /* 11111110 */
	0xd6, /* 11010110 */
	0xc6, /* 11000110 */
	0xc6, /* 11000110 */
	0xc6, /* 11000110 */
	0xc6, /* 11000110 */
	0xc6, /* 11000110 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 78 0x4e 'N' */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0xc6, /* 11000110 */
	0xe6, /* 11100110 */
	0xf6, /* 11110110 */
	0xfe, /* 11111110 */
	0xde, /* 11011110 */
	0xce, /* 11001110 */
	0xc6, /* 11000110 */
	0xc6, /* 11000110 */
	0xc6, /* 11000110 */
	0xc6, /* 11000110 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 79 0x4f 'O' */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x7c, /* 01111100 */
	0xc6, /* 11000110 */
	0xc6, /* 11000110 */
	0xc6, /* 11000110 */
	0xc6, /* 11000110 */
	0xc6, /* 11000110 */
	0xc6, /* 11000110 */
	0xc6, /* 11000110 */
	0xc6, /* 11000110 */
	0x7c, /* 01111100 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 80 0x50 'P' */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0xfc, /* 11111100 */
	0x66, /* 01100110 */
	0x66, /* 01100110 */
	0x66, /* 01100110 */
	0x7c, /* 01111100 */
	0x60, /* 01100000 */
	0x60, /* 01100000 */
	0x60, /* 01100000 */
	0x60, /* 01100000 */
	0xf0, /* 11110000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 81 0x51 'Q' */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x7c, /* 01111100 */
	0xc6, /* 11000110 */
	0xc6, /* 11000110 */
	0xc6, /* 11000110 */
	0xc6, /* 11000110 */
	0xc6, /* 11000110 */
	0xc6, /* 11000110 */
	0xd6, /* 11010110 */
	0xde, /* 11011110 */
	0x7c, /* 01111100 */
	0x0c, /* 00001100 */
	0x0e, /* 00001110 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 82 0x52 'R' */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0xfc, /* 11111100 */
	0x66, /* 01100110 */
	0x66, /* 01100110 */
	0x66, /* 01100110 */
	0x7c, /* 01111100 */
	0x6c, /* 01101100 */
	0x66, /* 01100110 */
	0x66, /* 01100110 */
	0x66, /* 01100110 */
	0xe6, /* 11100110 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 83 0x53 'S' */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x7c, /* 01111100 */
	0xc6, /* 11000110 */
	0xc6, /* 11000110 */
	0x60, /* 01100000 */
	0x38, /* 00111000 */
	0x0c, /* 00001100 */
	0x06, /* 00000110 */
	0xc6, /* 11000110 */
	0xc6, /* 11000110 */
	0x7c, /* 01111100 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 84 0x54 'T' */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x7e, /* 01111110 */
	0x7e, /* 01111110 */
	0x5a, /* 01011010 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x3c, /* 00111100 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 85 0x55 'U' */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0xc6, /* 11000110 */
	0xc6, /* 11000110 */
	0xc6, /* 11000110 */
	0xc6, /* 11000110 */
	0xc6, /* 11000110 */
	0xc6, /* 11000110 */
	0xc6, /* 11000110 */
	0xc6, /* 11000110 */
	0xc6, /* 11000110 */
	0x7c, /* 01111100 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 86 0x56 'V' */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0xc6, /* 11000110 */
	0xc6, /* 11000110 */
	0xc6, /* 11000110 */
	0xc6, /* 11000110 */
	0xc6, /* 11000110 */
	0xc6, /* 11000110 */
	0xc6, /* 11000110 */
	0x6c, /* 01101100 */
	0x38, /* 00111000 */
	0x10, /* 00010000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 87 0x57 'W' */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0xc6, /* 11000110 */
	0xc6, /* 11000110 */
	0xc6, /* 11000110 */
	0xc6, /* 11000110 */
	0xd6, /* 11010110 */
	0xd6, /* 11010110 */
	0xd6, /* 11010110 */
	0xfe, /* 11111110 */
	0xee, /* 11101110 */
	0x6c, /* 01101100 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 88 0x58 'X' */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0xc6, /* 11000110 */
	0xc6, /* 11000110 */
	0x6c, /* 01101100 */
	0x7c, /* 01111100 */
	0x38, /* 00111000 */
	0x38, /* 00111000 */
	0x7c, /* 01111100 */
	0x6c, /* 01101100 */
	0xc6, /* 11000110 */
	0xc6, /* 11000110 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 89 0x59 'Y' */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x66, /* 01100110 */
	0x66, /* 01100110 */
	0x66, /* 01100110 */
	0x66, /* 01100110 */
	0x3c, /* 00111100 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x3c, /* 00111100 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 90 0x5a 'Z' */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0xfe, /* 11111110 */
	0xc6, /* 11000110 */
	0x86, /* 10000110 */
	0x0c, /* 00001100 */
	0x18, /* 00011000 */
	0x30, /* 00110000 */
	0x60, /* 01100000 */
	0xc2, /* 11000010 */
	0xc6, /* 11000110 */
	0xfe, /* 11111110 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 91 0x5b '[' */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x3c, /* 00111100 */
	0x30, /* 00110000 */
	0x30, /* 00110000 */
	0x30, /* 00110000 */
	0x30, /* 00110000 */
	0x30, /* 00110000 */
	0x30, /* 00110000 */
	0x30, /* 00110000 */
	0x30, /* 00110000 */
	0x3c, /* 00111100 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 92 0x5c '\' */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x80, /* 10000000 */
	0xc0, /* 11000000 */
	0xe0, /* 11100000 */
	0x70, /* 01110000 */
	0x38, /* 00111000 */
	0x1c, /* 00011100 */
	0x0e, /* 00001110 */
	0x06, /* 00000110 */
	0x02, /* 00000010 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 93 0x5d ']' */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x3c, /* 00111100 */
	0x0c, /* 00001100 */
	0x0c, /* 00001100 */
	0x0c, /* 00001100 */
	0x0c, /* 00001100 */
	0x0c, /* 00001100 */
	0x0c, /* 00001100 */
	0x0c, /* 00001100 */
	0x0c, /* 00001100 */
	0x3c, /* 00111100 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 94 0x5e '^' */
	0x10, /* 00010000 */
	0x38, /* 00111000 */
	0x6c, /* 01101100 */
	0xc6, /* 11000110 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 95 0x5f '_' */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0xff, /* 11111111 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 96 0x60 '`' */
	0x00, /* 00000000 */
	0x30, /* 00110000 */
	0x18, /* 00011000 */
	0x0c, /* 00001100 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 97 0x61 'a' */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x78, /* 01111000 */
	0x0c, /* 00001100 */
	0x7c, /* 01111100 */
	0xcc, /* 11001100 */
	0xcc, /* 11001100 */
	0xcc, /* 11001100 */
	0x76, /* 01110110 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 98 0x62 'b' */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0xe0, /* 11100000 */
	0x60, /* 01100000 */
	0x60, /* 01100000 */
	0x78, /* 01111000 */
	0x6c, /* 01101100 */
	0x66, /* 01100110 */
	0x66, /* 01100110 */
	0x66, /* 01100110 */
	0x66, /* 01100110 */
	0x7c, /* 01111100 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 99 0x63 'c' */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x7c, /* 01111100 */
	0xc6, /* 11000110 */
	0xc0, /* 11000000 */
	0xc0, /* 11000000 */
	0xc0, /* 11000000 */
	0xc6, /* 11000110 */
	0x7c, /* 01111100 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 100 0x64 'd' */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x1c, /* 00011100 */
	0x0c, /* 00001100 */
	0x0c, /* 00001100 */
	0x3c, /* 00111100 */
	0x6c, /* 01101100 */
	0xcc, /* 11001100 */
	0xcc, /* 11001100 */
	0xcc, /* 11001100 */
	0xcc, /* 11001100 */
	0x76, /* 01110110 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 101 0x65 'e' */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x7c, /* 01111100 */
	0xc6, /* 11000110 */
	0xfe, /* 11111110 */
	0xc0, /* 11000000 */
	0xc0, /* 11000000 */
	0xc6, /* 11000110 */
	0x7c, /* 01111100 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 102 0x66 'f' */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x1c, /* 00011100 */
	0x36, /* 00110110 */
	0x32, /* 00110010 */
	0x30, /* 00110000 */
	0x78, /* 01111000 */
	0x30, /* 00110000 */
	0x30, /* 00110000 */
	0x30, /* 00110000 */
	0x30, /* 00110000 */
	0x78, /* 01111000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 103 0x67 'g' */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x76, /* 01110110 */
	0xcc, /* 11001100 */
	0xcc, /* 11001100 */
	0xcc, /* 11001100 */
	0xcc, /* 11001100 */
	0xcc, /* 11001100 */
	0x7c, /* 01111100 */
	0x0c, /* 00001100 */
	0xcc, /* 11001100 */
	0x78, /* 01111000 */
	0x00, /* 00000000 */

	/* 104 0x68 'h' */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0xe0, /* 11100000 */
	0x60, /* 01100000 */
	0x60, /* 01100000 */
	0x6c, /* 01101100 */
	0x76, /* 01110110 */
	0x66, /* 01100110 */
	0x66, /* 01100110 */
	0x66, /* 01100110 */
	0x66, /* 01100110 */
	0xe6, /* 11100110 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 105 0x69 'i' */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x00, /* 00000000 */
	0x38, /* 00111000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x3c, /* 00111100 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 106 0x6a 'j' */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x06, /* 00000110 */
	0x06, /* 00000110 */
	0x00, /* 00000000 */
	0x0e, /* 00001110 */
	0x06, /* 00000110 */
	0x06, /* 00000110 */
	0x06, /* 00000110 */
	0x06, /* 00000110 */
	0x06, /* 00000110 */
	0x06, /* 00000110 */
	0x66, /* 01100110 */
	0x66, /* 01100110 */
	0x3c, /* 00111100 */
	0x00, /* 00000000 */

	/* 107 0x6b 'k' */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0xe0, /* 11100000 */
	0x60, /* 01100000 */
	0x60, /* 01100000 */
	0x66, /* 01100110 */
	0x6c, /* 01101100 */
	0x78, /* 01111000 */
	0x78, /* 01111000 */
	0x6c, /* 01101100 */
	0x66, /* 01100110 */
	0xe6, /* 11100110 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 108 0x6c 'l' */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x38, /* 00111000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x3c, /* 00111100 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 109 0x6d 'm' */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0xec, /* 11101100 */
	0xfe, /* 11111110 */
	0xd6, /* 11010110 */
	0xd6, /* 11010110 */
	0xd6, /* 11010110 */
	0xd6, /* 11010110 */
	0xc6, /* 11000110 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 110 0x6e 'n' */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0xdc, /* 11011100 */
	0x66, /* 01100110 */
	0x66, /* 01100110 */
	0x66, /* 01100110 */
	0x66, /* 01100110 */
	0x66, /* 01100110 */
	0x66, /* 01100110 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 111 0x6f 'o' */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x7c, /* 01111100 */
	0xc6, /* 11000110 */
	0xc6, /* 11000110 */
	0xc6, /* 11000110 */
	0xc6, /* 11000110 */
	0xc6, /* 11000110 */
	0x7c, /* 01111100 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 112 0x70 'p' */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0xdc, /* 11011100 */
	0x66, /* 01100110 */
	0x66, /* 01100110 */
	0x66, /* 01100110 */
	0x66, /* 01100110 */
	0x66, /* 01100110 */
	0x7c, /* 01111100 */
	0x60, /* 01100000 */
	0x60, /* 01100000 */
	0xf0, /* 11110000 */
	0x00, /* 00000000 */

	/* 113 0x71 'q' */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x76, /* 01110110 */
	0xcc, /* 11001100 */
	0xcc, /* 11001100 */
	0xcc, /* 11001100 */
	0xcc, /* 11001100 */
	0xcc, /* 11001100 */
	0x7c, /* 01111100 */
	0x0c, /* 00001100 */
	0x0c, /* 00001100 */
	0x1e, /* 00011110 */
	0x00, /* 00000000 */

	/* 114 0x72 'r' */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0xdc, /* 11011100 */
	0x76, /* 01110110 */
	0x66, /* 01100110 */
	0x60, /* 01100000 */
	0x60, /* 01100000 */
	0x60, /* 01100000 */
	0xf0, /* 11110000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 115 0x73 's' */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x7c, /* 01111100 */
	0xc6, /* 11000110 */
	0x60, /* 01100000 */
	0x38, /* 00111000 */
	0x0c, /* 00001100 */
	0xc6, /* 11000110 */
	0x7c, /* 01111100 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 116 0x74 't' */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x10, /* 00010000 */
	0x30, /* 00110000 */
	0x30, /* 00110000 */
	0xfc, /* 11111100 */
	0x30, /* 00110000 */
	0x30, /* 00110000 */
	0x30, /* 00110000 */
	0x30, /* 00110000 */
	0x36, /* 00110110 */
	0x1c, /* 00011100 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 117 0x75 'u' */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0xcc, /* 11001100 */
	0xcc, /* 11001100 */
	0xcc, /* 11001100 */
	0xcc, /* 11001100 */
	0xcc, /* 11001100 */
	0xcc, /* 11001100 */
	0x76, /* 01110110 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 118 0x76 'v' */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0xc6, /* 11000110 */
	0xc6, /* 11000110 */
	0xc6, /* 11000110 */
	0xc6, /* 11000110 */
	0xc6, /* 11000110 */
	0x6c, /* 01101100 */
	0x38, /* 00111000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 119 0x77 'w' */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0xc6, /* 11000110 */
	0xc6, /* 11000110 */
	0xd6, /* 11010110 */
	0xd6, /* 11010110 */
	0xd6, /* 11010110 */
	0xfe, /* 11111110 */
	0x6c, /* 01101100 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 120 0x78 'x' */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0xc6, /* 11000110 */
	0x6c, /* 01101100 */
	0x38, /* 00111000 */
	0x38, /* 00111000 */
	0x38, /* 00111000 */
	0x6c, /* 01101100 */
	0xc6, /* 11000110 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 121 0x79 'y' */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0xc6, /* 11000110 */
	0xc6, /* 11000110 */
	0xc6, /* 11000110 */
	0xc6, /* 11000110 */
	0xc6, /* 11000110 */
	0xc6, /* 11000110 */
	0x7e, /* 01111110 */
	0x06, /* 00000110 */
	0x0c, /* 00001100 */
	0xf8, /* 11111000 */
	0x00, /* 00000000 */

	/* 122 0x7a 'z' */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0xfe, /* 11111110 */
	0xcc, /* 11001100 */
	0x18, /* 00011000 */
	0x30, /* 00110000 */
	0x60, /* 01100000 */
	0xc6, /* 11000110 */
	0xfe, /* 11111110 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 123 0x7b '{' */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x0e, /* 00001110 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x70, /* 01110000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x0e, /* 00001110 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 124 0x7c '|' */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 125 0x7d '}' */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x70, /* 01110000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x0e, /* 00001110 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x70, /* 01110000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 126 0x7e '~' */
	0x00, /* 00000000 */
	0x76, /* 01110110 */
	0xdc, /* 11011100 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 127 0x7f '' */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x10, /* 00010000 */
	0x38, /* 00111000 */
	0x6c, /* 01101100 */
	0xc6, /* 11000110 */
	0xc6, /* 11000110 */
	0xc6, /* 11000110 */
	0xfe, /* 11111110 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 128 0x80 '€' */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x3c, /* 00111100 */
	0x66, /* 01100110 */
	0xc2, /* 11000010 */
	0xc0, /* 11000000 */
	0xc0, /* 11000000 */
	0xc0, /* 11000000 */
	0xc0, /* 11000000 */
	0xc2, /* 11000010 */
	0x66, /* 01100110 */
	0x3c, /* 00111100 */
	0x18, /* 00011000 */
	0x70, /* 01110000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 129 0x81 '? */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0xcc, /* 11001100 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0xcc, /* 11001100 */
	0xcc, /* 11001100 */
	0xcc, /* 11001100 */
	0xcc, /* 11001100 */
	0xcc, /* 11001100 */
	0xcc, /* 11001100 */
	0x76, /* 01110110 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 130 0x82 '? */
	0x00, /* 00000000 */
	0x0c, /* 00001100 */
	0x18, /* 00011000 */
	0x30, /* 00110000 */
	0x00, /* 00000000 */
	0x7c, /* 01111100 */
	0xc6, /* 11000110 */
	0xfe, /* 11111110 */
	0xc0, /* 11000000 */
	0xc0, /* 11000000 */
	0xc6, /* 11000110 */
	0x7c, /* 01111100 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 131 0x83 '? */
	0x00, /* 00000000 */
	0x10, /* 00010000 */
	0x38, /* 00111000 */
	0x6c, /* 01101100 */
	0x00, /* 00000000 */
	0x78, /* 01111000 */
	0x0c, /* 00001100 */
	0x7c, /* 01111100 */
	0xcc, /* 11001100 */
	0xcc, /* 11001100 */
	0xcc, /* 11001100 */
	0x76, /* 01110110 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 132 0x84 '? */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0xcc, /* 11001100 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x78, /* 01111000 */
	0x0c, /* 00001100 */
	0x7c, /* 01111100 */
	0xcc, /* 11001100 */
	0xcc, /* 11001100 */
	0xcc, /* 11001100 */
	0x76, /* 01110110 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 133 0x85 '? */
	0x00, /* 00000000 */
	0x60, /* 01100000 */
	0x30, /* 00110000 */
	0x18, /* 00011000 */
	0x00, /* 00000000 */
	0x78, /* 01111000 */
	0x0c, /* 00001100 */
	0x7c, /* 01111100 */
	0xcc, /* 11001100 */
	0xcc, /* 11001100 */
	0xcc, /* 11001100 */
	0x76, /* 01110110 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 134 0x86 '? */
	0x00, /* 00000000 */
	0x38, /* 00111000 */
	0x6c, /* 01101100 */
	0x38, /* 00111000 */
	0x00, /* 00000000 */
	0x78, /* 01111000 */
	0x0c, /* 00001100 */
	0x7c, /* 01111100 */
	0xcc, /* 11001100 */
	0xcc, /* 11001100 */
	0xcc, /* 11001100 */
	0x76, /* 01110110 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 135 0x87 '? */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x7c, /* 01111100 */
	0xc6, /* 11000110 */
	0xc0, /* 11000000 */
	0xc0, /* 11000000 */
	0xc0, /* 11000000 */
	0xc6, /* 11000110 */
	0x7c, /* 01111100 */
	0x18, /* 00011000 */
	0x70, /* 01110000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 136 0x88 '? */
	0x00, /* 00000000 */
	0x10, /* 00010000 */
	0x38, /* 00111000 */
	0x6c, /* 01101100 */
	0x00, /* 00000000 */
	0x7c, /* 01111100 */
	0xc6, /* 11000110 */
	0xfe, /* 11111110 */
	0xc0, /* 11000000 */
	0xc0, /* 11000000 */
	0xc6, /* 11000110 */
	0x7c, /* 01111100 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 137 0x89 '? */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0xc6, /* 11000110 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x7c, /* 01111100 */
	0xc6, /* 11000110 */
	0xfe, /* 11111110 */
	0xc0, /* 11000000 */
	0xc0, /* 11000000 */
	0xc6, /* 11000110 */
	0x7c, /* 01111100 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 138 0x8a '? */
	0x00, /* 00000000 */
	0x60, /* 01100000 */
	0x30, /* 00110000 */
	0x18, /* 00011000 */
	0x00, /* 00000000 */
	0x7c, /* 01111100 */
	0xc6, /* 11000110 */
	0xfe, /* 11111110 */
	0xc0, /* 11000000 */
	0xc0, /* 11000000 */
	0xc6, /* 11000110 */
	0x7c, /* 01111100 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 139 0x8b '? */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x66, /* 01100110 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x38, /* 00111000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x3c, /* 00111100 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 140 0x8c '? */
	0x00, /* 00000000 */
	0x18, /* 00011000 */
	0x3c, /* 00111100 */
	0x66, /* 01100110 */
	0x00, /* 00000000 */
	0x38, /* 00111000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x3c, /* 00111100 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 141 0x8d '? */
	0x00, /* 00000000 */
	0x60, /* 01100000 */
	0x30, /* 00110000 */
	0x18, /* 00011000 */
	0x00, /* 00000000 */
	0x38, /* 00111000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x3c, /* 00111100 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 142 0x8e '? */
	0x00, /* 00000000 */
	0xc6, /* 11000110 */
	0x00, /* 00000000 */
	0x10, /* 00010000 */
	0x38, /* 00111000 */
	0x6c, /* 01101100 */
	0xc6, /* 11000110 */
	0xc6, /* 11000110 */
	0xfe, /* 11111110 */
	0xc6, /* 11000110 */
	0xc6, /* 11000110 */
	0xc6, /* 11000110 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 143 0x8f '? */
	0x38, /* 00111000 */
	0x6c, /* 01101100 */
	0x38, /* 00111000 */
	0x10, /* 00010000 */
	0x38, /* 00111000 */
	0x6c, /* 01101100 */
	0xc6, /* 11000110 */
	0xfe, /* 11111110 */
	0xc6, /* 11000110 */
	0xc6, /* 11000110 */
	0xc6, /* 11000110 */
	0xc6, /* 11000110 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 144 0x90 '? */
	0x0c, /* 00001100 */
	0x18, /* 00011000 */
	0x00, /* 00000000 */
	0xfe, /* 11111110 */
	0x66, /* 01100110 */
	0x62, /* 01100010 */
	0x68, /* 01101000 */
	0x78, /* 01111000 */
	0x68, /* 01101000 */
	0x62, /* 01100010 */
	0x66, /* 01100110 */
	0xfe, /* 11111110 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 145 0x91 '? */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0xec, /* 11101100 */
	0x36, /* 00110110 */
	0x36, /* 00110110 */
	0x7e, /* 01111110 */
	0xd8, /* 11011000 */
	0xd8, /* 11011000 */
	0x6e, /* 01101110 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 146 0x92 '? */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x3e, /* 00111110 */
	0x6c, /* 01101100 */
	0xcc, /* 11001100 */
	0xcc, /* 11001100 */
	0xfe, /* 11111110 */
	0xcc, /* 11001100 */
	0xcc, /* 11001100 */
	0xcc, /* 11001100 */
	0xcc, /* 11001100 */
	0xce, /* 11001110 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 147 0x93 '? */
	0x00, /* 00000000 */
	0x10, /* 00010000 */
	0x38, /* 00111000 */
	0x6c, /* 01101100 */
	0x00, /* 00000000 */
	0x7c, /* 01111100 */
	0xc6, /* 11000110 */
	0xc6, /* 11000110 */
	0xc6, /* 11000110 */
	0xc6, /* 11000110 */
	0xc6, /* 11000110 */
	0x7c, /* 01111100 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 148 0x94 '? */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0xc6, /* 11000110 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x7c, /* 01111100 */
	0xc6, /* 11000110 */
	0xc6, /* 11000110 */
	0xc6, /* 11000110 */
	0xc6, /* 11000110 */
	0xc6, /* 11000110 */
	0x7c, /* 01111100 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 149 0x95 '? */
	0x00, /* 00000000 */
	0x60, /* 01100000 */
	0x30, /* 00110000 */
	0x18, /* 00011000 */
	0x00, /* 00000000 */
	0x7c, /* 01111100 */
	0xc6, /* 11000110 */
	0xc6, /* 11000110 */
	0xc6, /* 11000110 */
	0xc6, /* 11000110 */
	0xc6, /* 11000110 */
	0x7c, /* 01111100 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 150 0x96 '? */
	0x00, /* 00000000 */
	0x30, /* 00110000 */
	0x78, /* 01111000 */
	0xcc, /* 11001100 */
	0x00, /* 00000000 */
	0xcc, /* 11001100 */
	0xcc, /* 11001100 */
	0xcc, /* 11001100 */
	0xcc, /* 11001100 */
	0xcc, /* 11001100 */
	0xcc, /* 11001100 */
	0x76, /* 01110110 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 151 0x97 '? */
	0x00, /* 00000000 */
	0x60, /* 01100000 */
	0x30, /* 00110000 */
	0x18, /* 00011000 */
	0x00, /* 00000000 */
	0xcc, /* 11001100 */
	0xcc, /* 11001100 */
	0xcc, /* 11001100 */
	0xcc, /* 11001100 */
	0xcc, /* 11001100 */
	0xcc, /* 11001100 */
	0x76, /* 01110110 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 152 0x98 '? */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0xc6, /* 11000110 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0xc6, /* 11000110 */
	0xc6, /* 11000110 */
	0xc6, /* 11000110 */
	0xc6, /* 11000110 */
	0xc6, /* 11000110 */
	0xc6, /* 11000110 */
	0x7e, /* 01111110 */
	0x06, /* 00000110 */
	0x0c, /* 00001100 */
	0x78, /* 01111000 */
	0x00, /* 00000000 */

	/* 153 0x99 '? */
	0x00, /* 00000000 */
	0xc6, /* 11000110 */
	0x00, /* 00000000 */
	0x7c, /* 01111100 */
	0xc6, /* 11000110 */
	0xc6, /* 11000110 */
	0xc6, /* 11000110 */
	0xc6, /* 11000110 */
	0xc6, /* 11000110 */
	0xc6, /* 11000110 */
	0xc6, /* 11000110 */
	0x7c, /* 01111100 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 154 0x9a '? */
	0x00, /* 00000000 */
	0xc6, /* 11000110 */
	0x00, /* 00000000 */
	0xc6, /* 11000110 */
	0xc6, /* 11000110 */
	0xc6, /* 11000110 */
	0xc6, /* 11000110 */
	0xc6, /* 11000110 */
	0xc6, /* 11000110 */
	0xc6, /* 11000110 */
	0xc6, /* 11000110 */
	0x7c, /* 01111100 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 155 0x9b '? */
	0x00, /* 00000000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x7c, /* 01111100 */
	0xc6, /* 11000110 */
	0xc0, /* 11000000 */
	0xc0, /* 11000000 */
	0xc0, /* 11000000 */
	0xc6, /* 11000110 */
	0x7c, /* 01111100 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 156 0x9c '? */
	0x00, /* 00000000 */
	0x38, /* 00111000 */
	0x6c, /* 01101100 */
	0x64, /* 01100100 */
	0x60, /* 01100000 */
	0xf0, /* 11110000 */
	0x60, /* 01100000 */
	0x60, /* 01100000 */
	0x60, /* 01100000 */
	0x60, /* 01100000 */
	0xe6, /* 11100110 */
	0xfc, /* 11111100 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 157 0x9d '? */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x66, /* 01100110 */
	0x66, /* 01100110 */
	0x3c, /* 00111100 */
	0x18, /* 00011000 */
	0x7e, /* 01111110 */
	0x18, /* 00011000 */
	0x7e, /* 01111110 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 158 0x9e '? */
	0x00, /* 00000000 */
	0xf8, /* 11111000 */
	0xcc, /* 11001100 */
	0xcc, /* 11001100 */
	0xf8, /* 11111000 */
	0xc4, /* 11000100 */
	0xcc, /* 11001100 */
	0xde, /* 11011110 */
	0xcc, /* 11001100 */
	0xcc, /* 11001100 */
	0xcc, /* 11001100 */
	0xc6, /* 11000110 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 159 0x9f '? */
	0x00, /* 00000000 */
	0x0e, /* 00001110 */
	0x1b, /* 00011011 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x7e, /* 01111110 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0xd8, /* 11011000 */
	0x70, /* 01110000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 160 0xa0 '? */
	0x00, /* 00000000 */
	0x18, /* 00011000 */
	0x30, /* 00110000 */
	0x60, /* 01100000 */
	0x00, /* 00000000 */
	0x78, /* 01111000 */
	0x0c, /* 00001100 */
	0x7c, /* 01111100 */
	0xcc, /* 11001100 */
	0xcc, /* 11001100 */
	0xcc, /* 11001100 */
	0x76, /* 01110110 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 161 0xa1 '? */
	0x00, /* 00000000 */
	0x0c, /* 00001100 */
	0x18, /* 00011000 */
	0x30, /* 00110000 */
	0x00, /* 00000000 */
	0x38, /* 00111000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x3c, /* 00111100 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 162 0xa2 '? */
	0x00, /* 00000000 */
	0x18, /* 00011000 */
	0x30, /* 00110000 */
	0x60, /* 01100000 */
	0x00, /* 00000000 */
	0x7c, /* 01111100 */
	0xc6, /* 11000110 */
	0xc6, /* 11000110 */
	0xc6, /* 11000110 */
	0xc6, /* 11000110 */
	0xc6, /* 11000110 */
	0x7c, /* 01111100 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 163 0xa3 '? */
	0x00, /* 00000000 */
	0x18, /* 00011000 */
	0x30, /* 00110000 */
	0x60, /* 01100000 */
	0x00, /* 00000000 */
	0xcc, /* 11001100 */
	0xcc, /* 11001100 */
	0xcc, /* 11001100 */
	0xcc, /* 11001100 */
	0xcc, /* 11001100 */
	0xcc, /* 11001100 */
	0x76, /* 01110110 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 164 0xa4 '? */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x76, /* 01110110 */
	0xdc, /* 11011100 */
	0x00, /* 00000000 */
	0xdc, /* 11011100 */
	0x66, /* 01100110 */
	0x66, /* 01100110 */
	0x66, /* 01100110 */
	0x66, /* 01100110 */
	0x66, /* 01100110 */
	0x66, /* 01100110 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 165 0xa5 '? */
	0x76, /* 01110110 */
	0xdc, /* 11011100 */
	0x00, /* 00000000 */
	0xc6, /* 11000110 */
	0xe6, /* 11100110 */
	0xf6, /* 11110110 */
	0xfe, /* 11111110 */
	0xde, /* 11011110 */
	0xce, /* 11001110 */
	0xc6, /* 11000110 */
	0xc6, /* 11000110 */
	0xc6, /* 11000110 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 166 0xa6 '? */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x3c, /* 00111100 */
	0x6c, /* 01101100 */
	0x6c, /* 01101100 */
	0x3e, /* 00111110 */
	0x00, /* 00000000 */
	0x7e, /* 01111110 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 167 0xa7 '? */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x38, /* 00111000 */
	0x6c, /* 01101100 */
	0x6c, /* 01101100 */
	0x38, /* 00111000 */
	0x00, /* 00000000 */
	0x7c, /* 01111100 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 168 0xa8 '? */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x30, /* 00110000 */
	0x30, /* 00110000 */
	0x00, /* 00000000 */
	0x30, /* 00110000 */
	0x30, /* 00110000 */
	0x60, /* 01100000 */
	0xc0, /* 11000000 */
	0xc6, /* 11000110 */
	0xc6, /* 11000110 */
	0x7c, /* 01111100 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 169 0xa9 '? */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0xfe, /* 11111110 */
	0xc0, /* 11000000 */
	0xc0, /* 11000000 */
	0xc0, /* 11000000 */
	0xc0, /* 11000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 170 0xaa '? */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0xfe, /* 11111110 */
	0x06, /* 00000110 */
	0x06, /* 00000110 */
	0x06, /* 00000110 */
	0x06, /* 00000110 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 171 0xab '? */
	0x00, /* 00000000 */
	0x60, /* 01100000 */
	0xe0, /* 11100000 */
	0x62, /* 01100010 */
	0x66, /* 01100110 */
	0x6c, /* 01101100 */
	0x18, /* 00011000 */
	0x30, /* 00110000 */
	0x60, /* 01100000 */
	0xdc, /* 11011100 */
	0x86, /* 10000110 */
	0x0c, /* 00001100 */
	0x18, /* 00011000 */
	0x3e, /* 00111110 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 172 0xac '? */
	0x00, /* 00000000 */
	0x60, /* 01100000 */
	0xe0, /* 11100000 */
	0x62, /* 01100010 */
	0x66, /* 01100110 */
	0x6c, /* 01101100 */
	0x18, /* 00011000 */
	0x30, /* 00110000 */
	0x66, /* 01100110 */
	0xce, /* 11001110 */
	0x9a, /* 10011010 */
	0x3f, /* 00111111 */
	0x06, /* 00000110 */
	0x06, /* 00000110 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 173 0xad '? */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x00, /* 00000000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x3c, /* 00111100 */
	0x3c, /* 00111100 */
	0x3c, /* 00111100 */
	0x18, /* 00011000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 174 0xae '? */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x36, /* 00110110 */
	0x6c, /* 01101100 */
	0xd8, /* 11011000 */
	0x6c, /* 01101100 */
	0x36, /* 00110110 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 175 0xaf '? */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0xd8, /* 11011000 */
	0x6c, /* 01101100 */
	0x36, /* 00110110 */
	0x6c, /* 01101100 */
	0xd8, /* 11011000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 176 0xb0 '? */
	0x11, /* 00010001 */
	0x44, /* 01000100 */
	0x11, /* 00010001 */
	0x44, /* 01000100 */
	0x11, /* 00010001 */
	0x44, /* 01000100 */
	0x11, /* 00010001 */
	0x44, /* 01000100 */
	0x11, /* 00010001 */
	0x44, /* 01000100 */
	0x11, /* 00010001 */
	0x44, /* 01000100 */
	0x11, /* 00010001 */
	0x44, /* 01000100 */
	0x11, /* 00010001 */
	0x44, /* 01000100 */

	/* 177 0xb1 '? */
	0x55, /* 01010101 */
	0xaa, /* 10101010 */
	0x55, /* 01010101 */
	0xaa, /* 10101010 */
	0x55, /* 01010101 */
	0xaa, /* 10101010 */
	0x55, /* 01010101 */
	0xaa, /* 10101010 */
	0x55, /* 01010101 */
	0xaa, /* 10101010 */
	0x55, /* 01010101 */
	0xaa, /* 10101010 */
	0x55, /* 01010101 */
	0xaa, /* 10101010 */
	0x55, /* 01010101 */
	0xaa, /* 10101010 */

	/* 178 0xb2 '? */
	0xdd, /* 11011101 */
	0x77, /* 01110111 */
	0xdd, /* 11011101 */
	0x77, /* 01110111 */
	0xdd, /* 11011101 */
	0x77, /* 01110111 */
	0xdd, /* 11011101 */
	0x77, /* 01110111 */
	0xdd, /* 11011101 */
	0x77, /* 01110111 */
	0xdd, /* 11011101 */
	0x77, /* 01110111 */
	0xdd, /* 11011101 */
	0x77, /* 01110111 */
	0xdd, /* 11011101 */
	0x77, /* 01110111 */

	/* 179 0xb3 '? */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */

	/* 180 0xb4 '? */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0xf8, /* 11111000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */

	/* 181 0xb5 '? */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0xf8, /* 11111000 */
	0x18, /* 00011000 */
	0xf8, /* 11111000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */

	/* 182 0xb6 '? */
	0x36, /* 00110110 */
	0x36, /* 00110110 */
	0x36, /* 00110110 */
	0x36, /* 00110110 */
	0x36, /* 00110110 */
	0x36, /* 00110110 */
	0x36, /* 00110110 */
	0xf6, /* 11110110 */
	0x36, /* 00110110 */
	0x36, /* 00110110 */
	0x36, /* 00110110 */
	0x36, /* 00110110 */
	0x36, /* 00110110 */
	0x36, /* 00110110 */
	0x36, /* 00110110 */
	0x36, /* 00110110 */

	/* 183 0xb7 '? */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0xfe, /* 11111110 */
	0x36, /* 00110110 */
	0x36, /* 00110110 */
	0x36, /* 00110110 */
	0x36, /* 00110110 */
	0x36, /* 00110110 */
	0x36, /* 00110110 */
	0x36, /* 00110110 */
	0x36, /* 00110110 */

	/* 184 0xb8 '? */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0xf8, /* 11111000 */
	0x18, /* 00011000 */
	0xf8, /* 11111000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */

	/* 185 0xb9 '? */
	0x36, /* 00110110 */
	0x36, /* 00110110 */
	0x36, /* 00110110 */
	0x36, /* 00110110 */
	0x36, /* 00110110 */
	0xf6, /* 11110110 */
	0x06, /* 00000110 */
	0xf6, /* 11110110 */
	0x36, /* 00110110 */
	0x36, /* 00110110 */
	0x36, /* 00110110 */
	0x36, /* 00110110 */
	0x36, /* 00110110 */
	0x36, /* 00110110 */
	0x36, /* 00110110 */
	0x36, /* 00110110 */

	/* 186 0xba '? */
	0x36, /* 00110110 */
	0x36, /* 00110110 */
	0x36, /* 00110110 */
	0x36, /* 00110110 */
	0x36, /* 00110110 */
	0x36, /* 00110110 */
	0x36, /* 00110110 */
	0x36, /* 00110110 */
	0x36, /* 00110110 */
	0x36, /* 00110110 */
	0x36, /* 00110110 */
	0x36, /* 00110110 */
	0x36, /* 00110110 */
	0x36, /* 00110110 */
	0x36, /* 00110110 */
	0x36, /* 00110110 */

	/* 187 0xbb '? */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0xfe, /* 11111110 */
	0x06, /* 00000110 */
	0xf6, /* 11110110 */
	0x36, /* 00110110 */
	0x36, /* 00110110 */
	0x36, /* 00110110 */
	0x36, /* 00110110 */
	0x36, /* 00110110 */
	0x36, /* 00110110 */
	0x36, /* 00110110 */
	0x36, /* 00110110 */

	/* 188 0xbc '? */
	0x36, /* 00110110 */
	0x36, /* 00110110 */
	0x36, /* 00110110 */
	0x36, /* 00110110 */
	0x36, /* 00110110 */
	0xf6, /* 11110110 */
	0x06, /* 00000110 */
	0xfe, /* 11111110 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 189 0xbd '? */
	0x36, /* 00110110 */
	0x36, /* 00110110 */
	0x36, /* 00110110 */
	0x36, /* 00110110 */
	0x36, /* 00110110 */
	0x36, /* 00110110 */
	0x36, /* 00110110 */
	0xfe, /* 11111110 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 190 0xbe '? */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0xf8, /* 11111000 */
	0x18, /* 00011000 */
	0xf8, /* 11111000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 191 0xbf '? */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0xf8, /* 11111000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */

	/* 192 0xc0 '? */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x1f, /* 00011111 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 193 0xc1 '? */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0xff, /* 11111111 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 194 0xc2 '? */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0xff, /* 11111111 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */

	/* 195 0xc3 '? */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x1f, /* 00011111 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */

	/* 196 0xc4 '? */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0xff, /* 11111111 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 197 0xc5 '? */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0xff, /* 11111111 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */

	/* 198 0xc6 '? */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x1f, /* 00011111 */
	0x18, /* 00011000 */
	0x1f, /* 00011111 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */

	/* 199 0xc7 '? */
	0x36, /* 00110110 */
	0x36, /* 00110110 */
	0x36, /* 00110110 */
	0x36, /* 00110110 */
	0x36, /* 00110110 */
	0x36, /* 00110110 */
	0x36, /* 00110110 */
	0x37, /* 00110111 */
	0x36, /* 00110110 */
	0x36, /* 00110110 */
	0x36, /* 00110110 */
	0x36, /* 00110110 */
	0x36, /* 00110110 */
	0x36, /* 00110110 */
	0x36, /* 00110110 */
	0x36, /* 00110110 */

	/* 200 0xc8 '? */
	0x36, /* 00110110 */
	0x36, /* 00110110 */
	0x36, /* 00110110 */
	0x36, /* 00110110 */
	0x36, /* 00110110 */
	0x37, /* 00110111 */
	0x30, /* 00110000 */
	0x3f, /* 00111111 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 201 0xc9 '? */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x3f, /* 00111111 */
	0x30, /* 00110000 */
	0x37, /* 00110111 */
	0x36, /* 00110110 */
	0x36, /* 00110110 */
	0x36, /* 00110110 */
	0x36, /* 00110110 */
	0x36, /* 00110110 */
	0x36, /* 00110110 */
	0x36, /* 00110110 */
	0x36, /* 00110110 */

	/* 202 0xca '? */
	0x36, /* 00110110 */
	0x36, /* 00110110 */
	0x36, /* 00110110 */
	0x36, /* 00110110 */
	0x36, /* 00110110 */
	0xf7, /* 11110111 */
	0x00, /* 00000000 */
	0xff, /* 11111111 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 203 0xcb '? */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0xff, /* 11111111 */
	0x00, /* 00000000 */
	0xf7, /* 11110111 */
	0x36, /* 00110110 */
	0x36, /* 00110110 */
	0x36, /* 00110110 */
	0x36, /* 00110110 */
	0x36, /* 00110110 */
	0x36, /* 00110110 */
	0x36, /* 00110110 */
	0x36, /* 00110110 */

	/* 204 0xcc '? */
	0x36, /* 00110110 */
	0x36, /* 00110110 */
	0x36, /* 00110110 */
	0x36, /* 00110110 */
	0x36, /* 00110110 */
	0x37, /* 00110111 */
	0x30, /* 00110000 */
	0x37, /* 00110111 */
	0x36, /* 00110110 */
	0x36, /* 00110110 */
	0x36, /* 00110110 */
	0x36, /* 00110110 */
	0x36, /* 00110110 */
	0x36, /* 00110110 */
	0x36, /* 00110110 */
	0x36, /* 00110110 */

	/* 205 0xcd '? */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0xff, /* 11111111 */
	0x00, /* 00000000 */
	0xff, /* 11111111 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 206 0xce '? */
	0x36, /* 00110110 */
	0x36, /* 00110110 */
	0x36, /* 00110110 */
	0x36, /* 00110110 */
	0x36, /* 00110110 */
	0xf7, /* 11110111 */
	0x00, /* 00000000 */
	0xf7, /* 11110111 */
	0x36, /* 00110110 */
	0x36, /* 00110110 */
	0x36, /* 00110110 */
	0x36, /* 00110110 */
	0x36, /* 00110110 */
	0x36, /* 00110110 */
	0x36, /* 00110110 */
	0x36, /* 00110110 */

	/* 207 0xcf '? */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0xff, /* 11111111 */
	0x00, /* 00000000 */
	0xff, /* 11111111 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 208 0xd0 '? */
	0x36, /* 00110110 */
	0x36, /* 00110110 */
	0x36, /* 00110110 */
	0x36, /* 00110110 */
	0x36, /* 00110110 */
	0x36, /* 00110110 */
	0x36, /* 00110110 */
	0xff, /* 11111111 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 209 0xd1 '? */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0xff, /* 11111111 */
	0x00, /* 00000000 */
	0xff, /* 11111111 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */

	/* 210 0xd2 '? */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0xff, /* 11111111 */
	0x36, /* 00110110 */
	0x36, /* 00110110 */
	0x36, /* 00110110 */
	0x36, /* 00110110 */
	0x36, /* 00110110 */
	0x36, /* 00110110 */
	0x36, /* 00110110 */
	0x36, /* 00110110 */

	/* 211 0xd3 '? */
	0x36, /* 00110110 */
	0x36, /* 00110110 */
	0x36, /* 00110110 */
	0x36, /* 00110110 */
	0x36, /* 00110110 */
	0x36, /* 00110110 */
	0x36, /* 00110110 */
	0x3f, /* 00111111 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 212 0xd4 '? */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x1f, /* 00011111 */
	0x18, /* 00011000 */
	0x1f, /* 00011111 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 213 0xd5 '? */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x1f, /* 00011111 */
	0x18, /* 00011000 */
	0x1f, /* 00011111 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */

	/* 214 0xd6 '? */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x3f, /* 00111111 */
	0x36, /* 00110110 */
	0x36, /* 00110110 */
	0x36, /* 00110110 */
	0x36, /* 00110110 */
	0x36, /* 00110110 */
	0x36, /* 00110110 */
	0x36, /* 00110110 */
	0x36, /* 00110110 */

	/* 215 0xd7 '? */
	0x36, /* 00110110 */
	0x36, /* 00110110 */
	0x36, /* 00110110 */
	0x36, /* 00110110 */
	0x36, /* 00110110 */
	0x36, /* 00110110 */
	0x36, /* 00110110 */
	0xff, /* 11111111 */
	0x36, /* 00110110 */
	0x36, /* 00110110 */
	0x36, /* 00110110 */
	0x36, /* 00110110 */
	0x36, /* 00110110 */
	0x36, /* 00110110 */
	0x36, /* 00110110 */
	0x36, /* 00110110 */

	/* 216 0xd8 '? */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0xff, /* 11111111 */
	0x18, /* 00011000 */
	0xff, /* 11111111 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */

	/* 217 0xd9 '? */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0xf8, /* 11111000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 218 0xda '? */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x1f, /* 00011111 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */

	/* 219 0xdb '? */
	0xff, /* 11111111 */
	0xff, /* 11111111 */
	0xff, /* 11111111 */
	0xff, /* 11111111 */
	0xff, /* 11111111 */
	0xff, /* 11111111 */
	0xff, /* 11111111 */
	0xff, /* 11111111 */
	0xff, /* 11111111 */
	0xff, /* 11111111 */
	0xff, /* 11111111 */
	0xff, /* 11111111 */
	0xff, /* 11111111 */
	0xff, /* 11111111 */
	0xff, /* 11111111 */
	0xff, /* 11111111 */

	/* 220 0xdc '? */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0xff, /* 11111111 */
	0xff, /* 11111111 */
	0xff, /* 11111111 */
	0xff, /* 11111111 */
	0xff, /* 11111111 */
	0xff, /* 11111111 */
	0xff, /* 11111111 */
	0xff, /* 11111111 */
	0xff, /* 11111111 */

	/* 221 0xdd '? */
	0xf0, /* 11110000 */
	0xf0, /* 11110000 */
	0xf0, /* 11110000 */
	0xf0, /* 11110000 */
	0xf0, /* 11110000 */
	0xf0, /* 11110000 */
	0xf0, /* 11110000 */
	0xf0, /* 11110000 */
	0xf0, /* 11110000 */
	0xf0, /* 11110000 */
	0xf0, /* 11110000 */
	0xf0, /* 11110000 */
	0xf0, /* 11110000 */
	0xf0, /* 11110000 */
	0xf0, /* 11110000 */
	0xf0, /* 11110000 */

	/* 222 0xde '? */
	0x0f, /* 00001111 */
	0x0f, /* 00001111 */
	0x0f, /* 00001111 */
	0x0f, /* 00001111 */
	0x0f, /* 00001111 */
	0x0f, /* 00001111 */
	0x0f, /* 00001111 */
	0x0f, /* 00001111 */
	0x0f, /* 00001111 */
	0x0f, /* 00001111 */
	0x0f, /* 00001111 */
	0x0f, /* 00001111 */
	0x0f, /* 00001111 */
	0x0f, /* 00001111 */
	0x0f, /* 00001111 */
	0x0f, /* 00001111 */

	/* 223 0xdf '? */
	0xff, /* 11111111 */
	0xff, /* 11111111 */
	0xff, /* 11111111 */
	0xff, /* 11111111 */
	0xff, /* 11111111 */
	0xff, /* 11111111 */
	0xff, /* 11111111 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 224 0xe0 '? */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x76, /* 01110110 */
	0xdc, /* 11011100 */
	0xd8, /* 11011000 */
	0xd8, /* 11011000 */
	0xd8, /* 11011000 */
	0xdc, /* 11011100 */
	0x76, /* 01110110 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 225 0xe1 '? */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x78, /* 01111000 */
	0xcc, /* 11001100 */
	0xcc, /* 11001100 */
	0xcc, /* 11001100 */
	0xd8, /* 11011000 */
	0xcc, /* 11001100 */
	0xc6, /* 11000110 */
	0xc6, /* 11000110 */
	0xc6, /* 11000110 */
	0xcc, /* 11001100 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 226 0xe2 '? */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0xfe, /* 11111110 */
	0xc6, /* 11000110 */
	0xc6, /* 11000110 */
	0xc0, /* 11000000 */
	0xc0, /* 11000000 */
	0xc0, /* 11000000 */
	0xc0, /* 11000000 */
	0xc0, /* 11000000 */
	0xc0, /* 11000000 */
	0xc0, /* 11000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 227 0xe3 '? */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0xfe, /* 11111110 */
	0x6c, /* 01101100 */
	0x6c, /* 01101100 */
	0x6c, /* 01101100 */
	0x6c, /* 01101100 */
	0x6c, /* 01101100 */
	0x6c, /* 01101100 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 228 0xe4 '? */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0xfe, /* 11111110 */
	0xc6, /* 11000110 */
	0x60, /* 01100000 */
	0x30, /* 00110000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x30, /* 00110000 */
	0x60, /* 01100000 */
	0xc6, /* 11000110 */
	0xfe, /* 11111110 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 229 0xe5 '? */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x7e, /* 01111110 */
	0xd8, /* 11011000 */
	0xd8, /* 11011000 */
	0xd8, /* 11011000 */
	0xd8, /* 11011000 */
	0xd8, /* 11011000 */
	0x70, /* 01110000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 230 0xe6 '? */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x66, /* 01100110 */
	0x66, /* 01100110 */
	0x66, /* 01100110 */
	0x66, /* 01100110 */
	0x66, /* 01100110 */
	0x66, /* 01100110 */
	0x7c, /* 01111100 */
	0x60, /* 01100000 */
	0x60, /* 01100000 */
	0xc0, /* 11000000 */
	0x00, /* 00000000 */

	/* 231 0xe7 '? */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x76, /* 01110110 */
	0xdc, /* 11011100 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 232 0xe8 '? */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x7e, /* 01111110 */
	0x18, /* 00011000 */
	0x3c, /* 00111100 */
	0x66, /* 01100110 */
	0x66, /* 01100110 */
	0x66, /* 01100110 */
	0x66, /* 01100110 */
	0x3c, /* 00111100 */
	0x18, /* 00011000 */
	0x7e, /* 01111110 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 233 0xe9 '? */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x38, /* 00111000 */
	0x6c, /* 01101100 */
	0xc6, /* 11000110 */
	0xc6, /* 11000110 */
	0xfe, /* 11111110 */
	0xc6, /* 11000110 */
	0xc6, /* 11000110 */
	0xc6, /* 11000110 */
	0x6c, /* 01101100 */
	0x38, /* 00111000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 234 0xea '? */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x38, /* 00111000 */
	0x6c, /* 01101100 */
	0xc6, /* 11000110 */
	0xc6, /* 11000110 */
	0xc6, /* 11000110 */
	0x6c, /* 01101100 */
	0x6c, /* 01101100 */
	0x6c, /* 01101100 */
	0x6c, /* 01101100 */
	0xee, /* 11101110 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 235 0xeb '? */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x1e, /* 00011110 */
	0x30, /* 00110000 */
	0x18, /* 00011000 */
	0x0c, /* 00001100 */
	0x3e, /* 00111110 */
	0x66, /* 01100110 */
	0x66, /* 01100110 */
	0x66, /* 01100110 */
	0x66, /* 01100110 */
	0x3c, /* 00111100 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 236 0xec '? */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x7e, /* 01111110 */
	0xdb, /* 11011011 */
	0xdb, /* 11011011 */
	0xdb, /* 11011011 */
	0x7e, /* 01111110 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 237 0xed '? */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x03, /* 00000011 */
	0x06, /* 00000110 */
	0x7e, /* 01111110 */
	0xdb, /* 11011011 */
	0xdb, /* 11011011 */
	0xf3, /* 11110011 */
	0x7e, /* 01111110 */
	0x60, /* 01100000 */
	0xc0, /* 11000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 238 0xee '? */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x1c, /* 00011100 */
	0x30, /* 00110000 */
	0x60, /* 01100000 */
	0x60, /* 01100000 */
	0x7c, /* 01111100 */
	0x60, /* 01100000 */
	0x60, /* 01100000 */
	0x60, /* 01100000 */
	0x30, /* 00110000 */
	0x1c, /* 00011100 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 239 0xef '? */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x7c, /* 01111100 */
	0xc6, /* 11000110 */
	0xc6, /* 11000110 */
	0xc6, /* 11000110 */
	0xc6, /* 11000110 */
	0xc6, /* 11000110 */
	0xc6, /* 11000110 */
	0xc6, /* 11000110 */
	0xc6, /* 11000110 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 240 0xf0 '? */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0xfe, /* 11111110 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0xfe, /* 11111110 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0xfe, /* 11111110 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 241 0xf1 '? */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x7e, /* 01111110 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x7e, /* 01111110 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 242 0xf2 '? */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x30, /* 00110000 */
	0x18, /* 00011000 */
	0x0c, /* 00001100 */
	0x06, /* 00000110 */
	0x0c, /* 00001100 */
	0x18, /* 00011000 */
	0x30, /* 00110000 */
	0x00, /* 00000000 */
	0x7e, /* 01111110 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 243 0xf3 '? */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x0c, /* 00001100 */
	0x18, /* 00011000 */
	0x30, /* 00110000 */
	0x60, /* 01100000 */
	0x30, /* 00110000 */
	0x18, /* 00011000 */
	0x0c, /* 00001100 */
	0x00, /* 00000000 */
	0x7e, /* 01111110 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 244 0xf4 '? */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x0e, /* 00001110 */
	0x1b, /* 00011011 */
	0x1b, /* 00011011 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */

	/* 245 0xf5 '? */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0xd8, /* 11011000 */
	0xd8, /* 11011000 */
	0xd8, /* 11011000 */
	0x70, /* 01110000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 246 0xf6 '? */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x18, /* 00011000 */
	0x00, /* 00000000 */
	0x7e, /* 01111110 */
	0x00, /* 00000000 */
	0x18, /* 00011000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 247 0xf7 '? */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x76, /* 01110110 */
	0xdc, /* 11011100 */
	0x00, /* 00000000 */
	0x76, /* 01110110 */
	0xdc, /* 11011100 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 248 0xf8 '? */
	0x00, /* 00000000 */
	0x38, /* 00111000 */
	0x6c, /* 01101100 */
	0x6c, /* 01101100 */
	0x38, /* 00111000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 249 0xf9 '? */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 250 0xfa '? */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x18, /* 00011000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 251 0xfb '? */
	0x00, /* 00000000 */
	0x0f, /* 00001111 */
	0x0c, /* 00001100 */
	0x0c, /* 00001100 */
	0x0c, /* 00001100 */
	0x0c, /* 00001100 */
	0x0c, /* 00001100 */
	0xec, /* 11101100 */
	0x6c, /* 01101100 */
	0x6c, /* 01101100 */
	0x3c, /* 00111100 */
	0x1c, /* 00011100 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 252 0xfc '? */
	0x00, /* 00000000 */
	0x6c, /* 01101100 */
	0x36, /* 00110110 */
	0x36, /* 00110110 */
	0x36, /* 00110110 */
	0x36, /* 00110110 */
	0x36, /* 00110110 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 253 0xfd '? */
	0x00, /* 00000000 */
	0x3c, /* 00111100 */
	0x66, /* 01100110 */
	0x0c, /* 00001100 */
	0x18, /* 00011000 */
	0x32, /* 00110010 */
	0x7e, /* 01111110 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 254 0xfe '? */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x7e, /* 01111110 */
	0x7e, /* 01111110 */
	0x7e, /* 01111110 */
	0x7e, /* 01111110 */
	0x7e, /* 01111110 */
	0x7e, /* 01111110 */
	0x7e, /* 01111110 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 255 0xff 'ÿ' */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

};

/* This routine allocates from 1 to n_devs virtual drivers.

   The real maximum number of virtual drivers will depend on how many drivers
   will succeed. This is limited to the maximum number of devices that
   videodev supports, which is equal to VIDEO_NUM_DEVICES.
 */
static int __init vivi_init(void)
{

#if 0
	const struct font_desc *font = find_font("VGA8x16");
	int ret = 0, i;

	if (font == NULL) {
		printk(KERN_ERR "vivi: could not find font\n");
		return -ENODEV;
	}
	font8x16 = font->data;
#else
	int i, ret = 0;
	font8x16 = (u8*)fontdata_8x16_;
#endif

	if (n_devs <= 0)
		n_devs = 1;

	printk("[%s][%s:%d] n_devs=%d\n", __FILE__, __FUNCTION__, __LINE__, n_devs);

	for (i = 0; i < n_devs; i++) {
		ret = vivi_create_instance(i);
		if (ret) {
			/* If some instantiations succeeded, keep driver */
			if (i)
				ret = 0;
			break;
		}
	}

	if (ret < 0) {
		printk(KERN_ERR "vivi: error %d while loading driver\n", ret);
		return ret;
	}

	printk(KERN_INFO "Video Technology Magazine Virtual Video "
			"Capture Board ver %s successfully loaded.\n",
			VIVI_VERSION);

	/* n_devs will reflect the actual number of allocated devices */
	n_devs = i;

	return ret;
}

static void __exit vivi_exit(void)
{
	vivi_release();
}

module_init(vivi_init);
module_exit(vivi_exit);
