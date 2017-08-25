/*
 * input raw for multi-touch device.
 * 2011.10.20 add current_x_filled  for type B multi-touch 
 * 2011.7.18.
 * remove the static variable slot
 * change slot-max from 4 to 10
 * 2011.6.5
 * add multi-touch device support
 * jiangshan00000@gmail.com
 *
 *  tslib/src/ts_read_raw_module.c
 *
 *  Original version:
 *  Copyright (C) 2001 Russell King.
 *
 *  Rewritten for the Linux input device API:
 *  Copyright (C) 2002 Nicolas Pitre
 *
 * This file is placed under the LGPL.  Please see the file
 * COPYING for more details.
 *
 * $Id: input-raw.c,v 1.5 2005/02/26 01:47:23 kergoth Exp $
 *
 * Read raw pressure, x, y, and timestamp from a touchscreen device.
 */


#include "config.h"

#include <errno.h>
#include <stdio.h>
#include <assert.h>

#include <stdlib.h>
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif
#include <sys/time.h>
#include <sys/types.h>

#include <linux/input.h>
#include <linux/fb.h>
#include <fcntl.h>
#ifndef EV_SYN /* 2.4 kernel headers */
# define EV_SYN 0x00
#endif
#ifndef EV_ABS /* 2.4 kernel headers */
# define EV_ABS 0x03
#endif

#include "tslib-private.h"

//----------------------------
#include <string.h>

#define DEBUG_TAG "MTINPUT"
#define dbg(format, ...)
//printf( "DBG:%s:%s:%d:" format  ,DEBUG_TAG, __FUNCTION__, __LINE__,  ##__VA_ARGS__)

/* includes available in 2.6.30-rc5 */
#ifndef BTN_TOOL_QUADTAP
#define BTN_TOOL_QUADTAP	0x14f	/* Four fingers on trackpad */
#define ABS_MT_TOUCH_MAJOR	0x30	/* Major axis of touching ellipse */
#define ABS_MT_TOUCH_MINOR	0x31	/* Minor axis (omit if circular) */
#define ABS_MT_WIDTH_MAJOR	0x32	/* Major axis of approaching ellipse */
#define ABS_MT_WIDTH_MINOR	0x33	/* Minor axis (omit if circular) */
#define ABS_MT_ORIENTATION	0x34	/* Ellipse orientation */
#define ABS_MT_POSITION_X	0x35	/* Center X ellipse position */
#define ABS_MT_POSITION_Y	0x36	/* Center Y ellipse position */
#define ABS_MT_TOOL_TYPE	0x37	/* Type of touching device */
#define ABS_MT_BLOB_ID		0x38	/* Group a set of packets as a blob */
#define ABS_MT_TRACKING_ID	0x39	/* Unique ID of initiated contact */
#define SYN_MT_REPORT		2
#define MT_TOOL_FINGER		0
#define MT_TOOL_PEN		1
#endif

/* includes available in 2.6.33 */
#ifndef ABS_MT_PRESSURE
#define ABS_MT_PRESSURE		0x3a	/* Pressure on contact area */
#endif

/* includes available in 2.6.36 */
#ifndef ABS_MT_SLOT
#define ABS_MT_SLOT		0x2f	/* MT slot being modified */
#endif

/* includes available in 2.6.38 */
#ifndef ABS_MT_DISTANCE
#define ABS_MT_DISTANCE		0x3b	/* Contact hover distance */
#endif

// slot support max
#define SLOT_MAX (10)
struct tslib_mtinput {
	struct tslib_module_info module;

	int	current_x[SLOT_MAX];
	int	current_y[SLOT_MAX];
	int	current_p[SLOT_MAX];

	int	current_x_filled[SLOT_MAX];

	int	checked_fd;
	int	using_syn;
	int current_slot;
};


#define BITS_PER_LONG 32 
#define BIT_WORD(nr) ((nr) / BITS_PER_LONG) 


//S+
#define DEFAULT_FBDEVICE "/dev/fb0"
int gWidth = 0;
int gHeight = 0;
struct input_absinfo gAbsInfoX = {0};
struct input_absinfo gAbsInfoY = {0};
int gTrackingId[SLOT_MAX] = {-1};

void clean_tracking_id(){
	memset(gTrackingId, -1, sizeof(gTrackingId));
}

int set_tracking_id(int id){
	int i = 0;

	for (i = 0; i < SLOT_MAX; i++){
		if(gTrackingId[i] == -1) break;
		if(gTrackingId[i] == id) return i;
	}

	for (i = 0; i < SLOT_MAX; i++){
		if(gTrackingId[i] == (-1)){
			gTrackingId[i] = id;
			return i;
		}
	}

	//if queue gTrackingId is full....clean up!
	dbg("warning: tracking id is full....\n");
	clean_tracking_id();
	gTrackingId[0] = id;
	return 0;
}

void get_screen_info(){
	char *fbpath;
	struct fb_var_screeninfo vinfo;
	int fbdev = 0;
	memset(&vinfo, 0, sizeof(vinfo));
	if((fbpath = getenv("TSLIB_FBDEVICE")) == NULL) {
		fbpath = DEFAULT_FBDEVICE;
	}

	fbdev = open(fbpath, O_RDONLY);
	if(fbdev){
		if (ioctl(fbdev, FBIOGET_VSCREENINFO, &vinfo)) {
			dbg("Error reading variable information");
		}else{
			gWidth = vinfo.xres;
			gHeight = vinfo.yres;
		}
		close(fbdev);
	}

	dbg(" w=%d h=%d\n", gWidth, gHeight);

}

int normalize_x(int x){
	int norx = x;

	// the touch width is the same as framebuffer width and it starts from 0
	if(gWidth == (gAbsInfoX.maximum - gAbsInfoX.minimum) && gWidth == gAbsInfoX.maximum)
		return x;

	if(gWidth > 0 && gAbsInfoX.minimum != gAbsInfoX.maximum){
		norx = (gWidth * (x - gAbsInfoX.minimum))/(gAbsInfoX.maximum - gAbsInfoX.minimum);
	}

	dbg("x=%d norx=%d\n", x, norx);
	return norx;
}

int normalize_y(int y){
	int nory = y;

	if(gHeight == (gAbsInfoY.maximum - gAbsInfoY.minimum) && gHeight == gAbsInfoY.maximum)
		return y;

	if(gHeight > 0 && gAbsInfoY.minimum != gAbsInfoY.maximum){
		nory = (gHeight * (y - gAbsInfoY.minimum))/(gAbsInfoY.maximum - gAbsInfoY.minimum);
	}

	dbg("y=%d nory=%d\n", y, nory);
	return nory;
}

static int test_bit(int nr, const volatile unsigned long *addr) 

{ 
	return 1UL & (addr[BIT_WORD(nr)] >> (nr & (BITS_PER_LONG-1))); 
} 



static int check_fd(struct tslib_mtinput *i)
{
	dbg("enter, i=0x%08x\n", (unsigned int)i);
	struct tsdev *ts = i->module.dev;
	//android ndk-r7 build will hangs here?
	// meanwhile ndk-r6b build OK
	dbg("ioctl1\n");
	if (ioctl(ts->fd, EVIOCGRAB, 1)) 
	{
		fprintf(stderr, "error: could not grab the device. not a multi-touch?\n");
		return -1;
	}
	unsigned long absbits[(ABS_MAX+31)/32];
	int rc;

	dbg("ioctl2\n");
	rc = ioctl(ts->fd, EVIOCGBIT(EV_ABS, sizeof(absbits)), absbits);
	if (rc < 0)
		return rc;

	if ( ( 0 == test_bit(ABS_MT_POSITION_X, absbits) ) ||
		 ( 0 == test_bit(ABS_MT_POSITION_Y, absbits) ) ||
		 ( 0 == test_bit(ABS_MT_TOUCH_MAJOR, absbits) ) 
		 )
	{
		fprintf(stderr, "error: could not support the device\n");
		fprintf(stderr, "EV_SYN=%d\n", test_bit(EV_SYN, absbits));
		fprintf(stderr, "EV_ABS=%d\n", test_bit(EV_ABS, absbits));
		fprintf(stderr, "ABS_MT_POSITION_X=%d\n", test_bit(ABS_MT_POSITION_X, absbits));
		fprintf(stderr, "ABS_MT_POSITION_Y=%d\n", test_bit(ABS_MT_POSITION_Y, absbits));
		fprintf(stderr, "ABS_MT_TOUCH_MAJOR=%d\n", test_bit(ABS_MT_TOUCH_MAJOR, absbits));
		return -1;
	}

	//fixme...
	if (1)//( 0 == (test_bit(EV_SYN, absbits)||test_bit(SYN_MT_REPORT, absbits)) )
	{
		i->using_syn = 1;
	}
	else
	{
		i->using_syn = 0;
	}

	i->checked_fd = 1;

	if (ioctl(ts->fd, EVIOCGABS(ABS_MT_POSITION_X), &gAbsInfoX) >= 0) {
		dbg(" x min = %d max = %d resolution = %d\n", gAbsInfoX.minimum, gAbsInfoX.maximum, gAbsInfoX.resolution);
	}
	if (ioctl(ts->fd, EVIOCGABS(ABS_MT_POSITION_Y), &gAbsInfoY) >= 0) {
		dbg(" y min = %d max = %d resolution = %d\n", gAbsInfoY.minimum, gAbsInfoY.maximum, gAbsInfoY.resolution);
	}

	dbg("leave successfully\n");
	return 0;
}

int PushSample( struct tslib_mtinput * i, struct input_event *ev, struct ts_sample * samp ) 
{
	/* Fill out a new complete event */
	// sync only when curr pressure ==0

	dbg("enter\n");
	int num = 0;
	int islot = 0;
	for (islot=0;islot<SLOT_MAX;islot++)
	{
		if (!i->current_x_filled[islot])
		{
			continue;
		}
		samp[num].x = i->current_x[islot];
		samp[num].y = i->current_y[islot];
		samp[num].pressure = i->current_p[islot];
		samp[num].tv = ev->time;
		i->current_x_filled[islot] = 0;
		num++;
	}
	dbg("leave. num=%d\n", num);
	return num;
}


/// for multi touch 
static int ts_input_read2(struct tslib_module_info *inf,
			 struct ts_sample *samp, int nr)
{
	dbg("enter. info=%08x samp=%08x. nr=%d\n", (unsigned int)inf, (unsigned int)samp, nr);
	struct tslib_mtinput *i = (struct tslib_mtinput *)inf;
	struct tsdev *ts = inf->dev;
	struct input_event ev;
	int ret = nr;
	int total = 0;


	if (i->checked_fd == 0)
	{
		check_fd(i);
	}
	if (i->checked_fd == -1)
		return 0;

	if(gWidth == 0 || gHeight == 0)
		get_screen_info();

	if (i->using_syn) 
	{
		while (total < nr) 
		{
			dbg("total=%d. nr=%d\n", total, nr);
			ret = read(ts->fd, &ev, sizeof(struct input_event));
			if (ret < (int)sizeof(struct input_event)) 
			{// no more data
				break;
			}

			switch (ev.type) 
			{
			case EV_KEY:
				dbg("event=EV_KEY:\n");
				switch (ev.code) 
				{
				case BTN_TOUCH:
					if (ev.value == 0) 
					{
						/* pen up */
						samp->x = 0;
						samp->y = 0;
						samp->pressure = 0;
						samp->tv = ev.time;
						samp++;
						total++;
					}
					break;
				}
				break;
			case EV_SYN:
				switch(ev.code) 
				{
				case SYN_REPORT:
					//Submit all the samples collected
					//break;
				case SYN_MT_REPORT:
				default:
					dbg("event=%d.code=%d\n", ev.type, ev.code);

					//send current xy
					{
						int mPushed = PushSample(i, &ev, samp);
						samp += mPushed;
						total += mPushed;
					}
					break;
				}
				break;

			case EV_ABS:
				dbg("event=EV_ABS\n");
				switch (ev.code) 
				{
				case ABS_X:
					dbg("event.code=ABS_X.v=%d\n", ev.value);
					i->current_x[i->current_slot] = ev.value;
					i->current_x_filled[i->current_slot] = 1;
					break;
				case ABS_Y:
					dbg("event.code=ABS_Y.v=%d\n", ev.value);
					i->current_y[i->current_slot] = ev.value;
					break;
				case ABS_PRESSURE:
					dbg("event.code=ABS_PRESSURE.v=%d\n", ev.value);
					i->current_p[i->current_slot] = ev.value;
					break;
				//---------------------------
				//jiangshan added 
				case ABS_MT_SLOT:
					dbg("event.code=ABS_MT_SLOT.v=%d\n", ev.value);

					if (ev.value<0)
					{
						// means that just a sync report?
						break;
					}
					i->current_slot = ev.value;
					if (i->current_slot>=SLOT_MAX)
					{
						fprintf(stderr, "slot limit error. SLOT_MAX=%d. current slot=%d\n",SLOT_MAX,  i->current_slot);
						i->current_slot = 0;
					}
					break;
				case ABS_MT_TOUCH_MAJOR:
					dbg("event.code=ABS_MT_TOUCH_MAJOR.v=%d\n", ev.value);
					i->current_p[i->current_slot] = ev.value;
					i->current_x_filled[i->current_slot] = 1;
					break;
				case ABS_MT_POSITION_X:
					dbg("event.code=ABS_MT_POSITION_X.v=%d\n", ev.value);
					//S+
					//i->current_x[i->current_slot] = ev.value;
					i->current_x[i->current_slot] = normalize_x(ev.value);
					i->current_x_filled[i->current_slot] = 1;
					
					break;
				case ABS_MT_POSITION_Y:
					dbg("event.code=ABS_MT_POSITION_Y.v=%d\n", ev.value);
					//S+
					//i->current_y[i->current_slot] = ev.value;
					i->current_y[i->current_slot] = normalize_y(ev.value);
					break;
				//---------------------------
				//S+
				//driver would use tracking id or slot to reserve the touch point. If using tracking id, map it to be a slot. 
				case ABS_MT_TRACKING_ID:
					dbg("event.code=ABS_MT_TRACKING_ID.v=%d\n", ev.value);
					i->current_x_filled[i->current_slot] = 1;
					if (ev.value == -1) {
						//release
						i->current_p[i->current_slot] = 0;
					}else{
						i->current_slot = set_tracking_id(ev.value);
						if(!i->current_p[i->current_slot]){
							i->current_p[i->current_slot] = 255;
						}
					}
					dbg("event.code= current_slot=%d\n", i->current_slot);
					break;
				}
				break;
			}
		}
		ret = total;
	} else {
		unsigned char *p = (unsigned char *) &ev;
		int len = sizeof(struct input_event);
	
		while (total < nr) {
			ret = read(ts->fd, p, len);
			if (ret == -1) {
				if (errno == EINTR) {
					continue;
				}
				break;
			}
	
			if (ret < (int)sizeof(struct input_event)) {
				/* short read
				 * restart read to get the rest of the event
				 */
				p += ret;
				len -= ret;
				continue;
			}
			/* successful read of a whole event */
	
			if (ev.type == EV_ABS) {
				switch (ev.code) {
				case ABS_X:
					if (ev.value != 0) {
						samp->x = i->current_x[i->current_slot] = ev.value;
						samp->y = i->current_y[i->current_slot];
						samp->pressure = i->current_p[i->current_slot];
					} else {
						fprintf(stderr, "tslib: dropped x = 0\n");
						continue;
					}
					break;
				case ABS_Y:
					if (ev.value != 0) {
						samp->x = i->current_x[i->current_slot];
						samp->y = i->current_y[i->current_slot] = ev.value;
						samp->pressure = i->current_p[i->current_slot];
					} else {
						fprintf(stderr, "tslib: dropped y = 0\n");
						continue;
					}
					break;
				case ABS_PRESSURE:
					samp->x = i->current_x[i->current_slot];
					samp->y = i->current_y[i->current_slot];
					samp->pressure = i->current_p[i->current_slot] = ev.value;
					break;
					//---------------------------
					//jiangshan added 
				case ABS_MT_SLOT:

					if (ev.value<0)
					{
						// means that just a sync report?
						break;
					}
					i->current_slot = ev.value;
					if (i->current_slot>=SLOT_MAX)
					{
						fprintf(stderr, "slot limit error. SLOT_MAX=%d. current slot=%d\n",SLOT_MAX,  i->current_slot);
						i->current_slot = 0;
					}
					break;
				case ABS_MT_TOUCH_MAJOR:
					i->current_p[i->current_slot] = ev.value;
					break;
				case ABS_MT_POSITION_X:
					//S+
					//i->current_x[i->current_slot] = ev.value;
					i->current_x[i->current_slot] = normalize_x(ev.value);
					break;
				case ABS_MT_POSITION_Y:
					//S+
					//i->current_y[i->current_slot] = ev.value;
					i->current_y[i->current_slot] = normalize_y(ev.value);
					break;
						//---------------------------
				//S+
				//driver would use tracking id or slot to reserve the touch point. If using tracking id, map it to be a slot. 
				case ABS_MT_TRACKING_ID:
					dbg("event.code=ABS_MT_TRACKING_ID.v=%d\n", ev.value);
					i->current_x_filled[i->current_slot] = 1;
					if (ev.value == -1) {
						//release
						i->current_p[i->current_slot] = 0;
					}else{
						i->current_slot = set_tracking_id(ev.value);
						if(!i->current_p[i->current_slot]){
							i->current_p[i->current_slot] = 255;
						}
					}
					dbg("event.code= current_slot=%d\n", i->current_slot);
					break;
				}
				samp->tv = ev.time;
	#ifdef DEBUG
				fprintf(stderr, "RAW---------------------------> %d %d %d\n",
					samp->x, samp->y, samp->pressure);
	#endif	 /*DEBUG*/
				samp++;
				total++;
			} else if (ev.type == EV_KEY) {
				switch (ev.code) {
				case BTN_TOUCH:
					if (ev.value == 0) {
						/* pen up */
						samp->x = 0;
						samp->y = 0;
						samp->pressure = 0;
						samp->tv = ev.time;
						samp++;
						total++;
					}
					break;
				}
			} else {
				fprintf(stderr, "tslib: Unknown event type %d\n", ev.type);
			}
			p = (unsigned char *) &ev;
		}
		ret = total;
	}

	dbg("leave\n");
	return ret;
}

static int ts_input_fini2(struct tslib_module_info *inf)
{
	struct tslib_mtinput *i = (struct tslib_mtinput *)inf;
	ioctl(i->module.dev->fd, EVIOCGRAB, 0);
	//close(i->module.dev->fd); open outside, close outside
	free(inf);
	return 0;
}

static const struct tslib_ops __ts_input_ops2 = 
{
	.read	= ts_input_read2,
	.fini	= ts_input_fini2,
};

TSAPI struct tslib_module_info *mod_init(struct tsdev *dev, const char *params)
{
	dbg("enter. dev=%08x\n", (unsigned int)dev);;
	assert(NULL != dev);
	struct tslib_mtinput *i;

	i = (struct tslib_mtinput *)malloc(sizeof(struct tslib_mtinput));
	if (i == NULL)
		return NULL;

	memset(i, 0, sizeof(struct tslib_mtinput));

	i->module.ops = &__ts_input_ops2;
	i->checked_fd = 0;
	i->using_syn = 0;
	i->current_slot = 0;
	dbg("leave\n");
	return &(i->module);
}


