#include <mach/kernel.h>
#include <mach/module.h>
#include <mach/gp_chunkmem.h>
#include <mach/sp_config.h>
#include <linux/fb.h>
#include <linux/timer.h>
#include <framebuffer/display/hardware.h>
#include <framebuffer/display/hal_disp.h>
#include <linux/delay.h>

/**************************************************************************
 *                           C O N S T A N T S                            *
 **************************************************************************/
#define FORMAT_RGB565		0x01
#define FORMAT_ARGB1555		0x02
#define FORMAT_RGBA4444		0x03
#define FORMAT_ARGB4444		0x04
#define FORMAT_RGBA8888		0x05
#define FORMAT_ARGB8888		0x06

//#define SEC_PANEL_W		720
//#define SEC_PANEL_H		480

#define FB_DEFAULT_FMT		FORMAT_ARGB8888
#define FBBUFFER_NUM		2

/**************************************************************************
 *                              M A C R O S                               *
 **************************************************************************/
#define PAGEALIGN(x)	((x + 4095) >> 12 << 12)

#if DEBUG_MSG
	#define DEBUG(fmt, arg...) printk(KERN_ERR "[%s:%d] "fmt, __FUNCTION__, __LINE__, ##arg)
	#define MSG(fmt, arg...) printk(KERN_ERR "[%s:%d] "fmt, __FUNCTION__, __LINE__, ##arg)
#else
	#define DEBUG(fmt, arg...)
	#define MSG(fmt, arg...)
#endif

#define ERRF(fmt, arg...) printk(KERN_ERR "[%s:%d] Error! "fmt, __FUNCTION__, __LINE__, ##arg)
#define WARNING(fmt, arg...) printk(KERN_WARNING "[%s:%d] Warning! "fmt, __FUNCTION__, __LINE__, ##arg)
#define INFO(fmt, arg...) printk(KERN_INFO "[%s:%d] "fmt, __FUNCTION__, __LINE__, ##arg)

/**************************************************************************
 *                          D A T A    T Y P E S                          *
 **************************************************************************/
typedef struct _framebuffer_t
{
	char ID[16];
	struct fb_info *fb;

//	DRV_OsdRegionHandle_t pMainRegion;
//	DRV_OsdRegionHandle_t pSecondRegion;
	UINT32				ColorFmt;
//	display_size_t		panelRes;
//	display_size_t		SecondPanelRes;
	display_size_t		UIRes;
	int					fbsize;
	void __iomem		*fbmem;
//	UINT32				IsMirrorOrg;
//#ifdef SYSCONFIG_ARCH_letb8700
//	stTCON_API_t *TCON;
//#endif
//	#ifdef UI_ON_VIDEO
//	DRV_VPP_Param_t param_t;
//	void* pluma_buf[2];
//	void* pcrma_buf[2];
//	UINT32 luma_buf_PA[2];
//	UINT32 crma_buf_PA[2];
//	UINT32 curr_buf_idx;
//	#endif
} framebuffer_t;

typedef struct _colormode_t
{
	const char *name;
	uint32_t bits_per_pixel;
	uint32_t nonstd;
	struct fb_bitfield red;
	struct fb_bitfield green;
	struct fb_bitfield blue;
	struct fb_bitfield transp;
} colormode_t;

/**************************************************************************
 *                         G L O B A L    D A T A                         *
 **************************************************************************/
//struct fb_info *gFrameBufInfo = 0;

//unsigned int gFB_BUFFER_ALIGN;

extern int IPC_FunctionCall(int cmd, void *data, int len);
//extern unsigned int gMAIN_DISP;
//extern unsigned int gSECOND_DISP;

//EXPORT_SYMBOL(gFrameBufInfo);

static const colormode_t ColorModeTab[] = {
	[FORMAT_RGB565] = {
		.name = "RGB565",
		.bits_per_pixel = 16,
		.red		= { .length = 5, .offset = 11, .msb_right = 0 },
		.green	= { .length = 6, .offset = 5, .msb_right = 0 },
		.blue	= { .length = 5, .offset = 0, .msb_right = 0 },
		.transp	= { .length = 0, .offset = 0, .msb_right = 0 },
	},
	[FORMAT_ARGB1555] = {
		.name = "ARGB1555",
		.bits_per_pixel = 16,
		.red		= { .length = 5, .offset = 10, .msb_right = 0 },
		.green	= { .length = 5, .offset = 5, .msb_right = 0 },
		.blue	= { .length = 5, .offset = 0, .msb_right = 0 },
		.transp	= { .length = 1, .offset = 15, .msb_right = 0 },
	},
	[FORMAT_RGBA4444] = {
		.name = "RGBA4444",
		.bits_per_pixel = 16,
		.red		= { .length = 4, .offset = 12, .msb_right = 0 },
		.green	= { .length = 4, .offset = 8, .msb_right = 0 },
		.blue	= { .length = 4, .offset = 4, .msb_right = 0 },
		.transp	= { .length = 4, .offset = 0, .msb_right = 0 },
	},
	[FORMAT_ARGB4444] = {
		.name = "ARGB4444",
		.bits_per_pixel = 16,
		.red		= { .length = 4, .offset = 8, .msb_right = 0 },
		.green	= { .length = 4, .offset = 4, .msb_right = 0 },
		.blue	= { .length = 4, .offset = 0, .msb_right = 0 },
		.transp	= { .length = 4, .offset = 12, .msb_right = 0 },
	},
	[FORMAT_RGBA8888] = {
		.name = "RGBA8888",
		.bits_per_pixel = 32,
		.red		= { .length = 8, .offset = 24, .msb_right = 0 },
		.green	= { .length = 8, .offset = 16, .msb_right = 0 },
		.blue	= { .length = 8, .offset = 8, .msb_right = 0 },
		.transp	= { .length = 8, .offset = 0, .msb_right = 0 },
	},
	[FORMAT_ARGB8888] = {
		.name = "ARGB8888",
		.bits_per_pixel = 32,
		.red		= { .length = 8, .offset = 16, .msb_right = 0 },
		.green	= { .length = 8, .offset = 8, .msb_right = 0 },
		.blue	= { .length = 8, .offset = 0, .msb_right = 0 },
		.transp	= { .length = 8, .offset = 24, .msb_right = 0 },
	},
};

/**************************************************************************
 *             F U N C T I O N    I M P L E M E N T A T I O N S           *
 **************************************************************************/

static int fbdss_open(struct fb_info *fbinfo, int user)
{
	// Do nothing
	return 0;
}

static int fbdss_release(struct fb_info *fbinfo, int user)
{
	// Not support release
	return 0;
}

static int fbdss_check_var(struct fb_var_screeninfo *var, struct fb_info *fbinfo)
{
	//DEBUG("\n");
	if ((var->bits_per_pixel != 16) && (var->bits_per_pixel != 32))
	{
		ERRF("Frame buffer check var failed. bpp=%d\n", var->bits_per_pixel);
		return -EINVAL;
	}

	return 0;
}

static int fbdss_set_par(struct fb_info *fbinfo)
{
#if 0
	framebuffer_t *fbWorkMem;
	colormode_t* colormode;
	UINT32 ret = 0;

	DEBUG("\n");
	if (fbinfo == NULL || fbinfo->par == NULL)
	{
		ERRF("fbdss_set_par invalid parameter\n");
		return -1;
	}

	fbWorkMem = fbinfo->par;
	colormode = (colormode_t*)&ColorModeTab[fbWorkMem->ColorFmt];

	if ((fbinfo->var.xres != fbWorkMem->UIRes.width) || (fbinfo->var.yres != fbWorkMem->UIRes.height) || (fbinfo->var.bits_per_pixel != colormode->bits_per_pixel))
	{
		DEBUG("Resolution Modify from %dx%d to %dx%d\n", fbWorkMem->UIRes.width, fbWorkMem->UIRes.height, fbinfo->var.xres, fbinfo->var.yres);
		DEBUG("BPP form %d to %d\n", colormode->bits_per_pixel, fbinfo->var.bits_per_pixel);

		//if (fbWorkMem->pMainRegion) MainDisp_Release(fbinfo);

		fbWorkMem->UIRes.width = fbinfo->var.xres;
		fbWorkMem->UIRes.height = fbinfo->var.yres;

		// [TODO] Get panel resolution
		fbWorkMem->panelRes.width = fbinfo->var.xres;
		fbWorkMem->panelRes.height = fbinfo->var.yres;

		fbWorkMem->fbsize = PAGEALIGN(fbWorkMem->UIRes.width * fbWorkMem->UIRes.height * fbinfo->var.bits_per_pixel / 8) * FBBUFFER_NUM;
		fbWorkMem->fbmem = chunk_malloc(DRAM_A, fbWorkMem->fbsize);

		if (fbWorkMem->fbmem == NULL) {
			ERRF("Failed to allocate chunk memory for framebuffer device\n");
			return -1;
		}

		memset(fbWorkMem->fbmem, 0, fbWorkMem->fbsize);

		fbinfo->screen_base	= fbWorkMem->fbmem;
		fbinfo->screen_size	= fbWorkMem->fbsize;
		fbinfo->fix.smem_start = chunk_pa(fbWorkMem->fbmem);
		fbinfo->fix.smem_len	= fbWorkMem->fbsize;
		fbinfo->fix.line_length	= (fbinfo->var.xres_virtual * fbinfo->var.bits_per_pixel) / 8;
		fbinfo->fix.type		= FB_TYPE_PACKED_PIXELS;

		if (fbinfo->var.nonstd)
			fbinfo->fix.visual = FB_VISUAL_PSEUDOCOLOR;
		else
			fbinfo->fix.visual = FB_VISUAL_TRUECOLOR;

		fbinfo->fix.accel		= FB_ACCEL_NONE;
		fbinfo->fix.xpanstep = 1;
		fbinfo->fix.ypanstep = 1;

		if (fbinfo->var.bits_per_pixel != colormode->bits_per_pixel)
		{
			if (fbinfo->var.bits_per_pixel == 32)
			{
				if (fbinfo->var.red.offset == 24)
					fbWorkMem->ColorFmt = FORMAT_RGBA8888;
				else
					fbWorkMem->ColorFmt = FORMAT_ARGB8888;
			}
			else
			{
				if (fbinfo->var.red.offset == 12)
					fbWorkMem->ColorFmt = FORMAT_RGBA4444;
				else if (fbinfo->var.red.offset == 8)
					fbWorkMem->ColorFmt = FORMAT_ARGB4444;
				else if (fbinfo->var.red.offset == 10)
					fbWorkMem->ColorFmt = FORMAT_ARGB1555;
				else
					fbWorkMem->ColorFmt = FORMAT_RGB565;
			}
		}

		DEBUG("UI Res %dx%d, Panel Res %dx%d, ColorFmt %d\n", fbWorkMem->UIRes.width, fbWorkMem->UIRes.height, fbWorkMem->panelRes.width, fbWorkMem->panelRes.height, fbWorkMem->ColorFmt);

		ret = MainDisp_Init(fbinfo, 1);
		if (ret != 0)
		{
			ERRF("MainDisp_Init failed");
			return -1;
		}

	}
#endif
	return 0;
}

int fbdss_pan_display(struct fb_var_screeninfo *var, struct fb_info *fbinfo)
{
	UINT32 retIPC = 0;
	UINT32 ID = 0;

	fbinfo->var.xoffset = var->xoffset;
	fbinfo->var.yoffset = var->yoffset;

	ID = fbinfo->var.yoffset/fbinfo->var.yres;

//	INFO("Swap buffer ID:=%d\n", ID);

	retIPC = IPC_FunctionCall(IPC_CMD_DISP_SWAP_BUFF, &ID, sizeof(UINT32));

	if (retIPC)
		INFO("IPC function call fails (IPC_CMD_DISP_SWAP_BUFF)=%d\n", retIPC);

	return retIPC;
}

int fbdss_ioctl(struct fb_info *fbinfo, unsigned int cmd, unsigned long arg)
{
	return 0;
}

int fbdss_waitVsync(struct fb_info *info)
{
	int retIPC;
	UINT32 zero = 0;

	DEBUG("Wait Vsync\n");

	retIPC = IPC_FunctionCall(IPC_CMD_DISP_WAIT_VSYNC, &zero, sizeof(UINT32));

	if (retIPC)
		INFO("IPC function call fails (IPC_CMD_DISP_WAIT_VSYNC)=%d\n", retIPC);

	return 0;
}

int fbdss_QueryVsync(struct fb_info *info)
{

	int retIPC;
	UINT32 CheckIsUpdate = 0;

	retIPC = IPC_FunctionCall(IPC_CMD_DISP_QUERY_VSYNC, &CheckIsUpdate, sizeof(UINT32));

	return 0;
}

static struct fb_ops framebuffer_ops = {
	.owner				= THIS_MODULE,
	.fb_open			= fbdss_open,
	.fb_release			= fbdss_release,
	.fb_check_var		= fbdss_check_var,
	.fb_set_par			= fbdss_set_par,
	.fb_pan_display		= fbdss_pan_display,
	.fb_ioctl			= fbdss_ioctl,
	.fb_sync			= fbdss_waitVsync,
	.fb_QueryVsync		= fbdss_QueryVsync,
};

static void WakeUpEcosDisplay(UINT32 MainBuffAddr, UINT32 SecBuffAddr)
{
	UINT32 BufferHandle[2] = {0};
	UINT32 retIPC = 0;
//	FB_Info fbWork;

//	memset(&fbWork, 0, sizeof(FB_Info));

	BufferHandle[MAIN_DISP] = MainBuffAddr;
	BufferHandle[SECOND_DISP] = SecBuffAddr;

	DEBUG("Buffer handle:=(0x%x, 0x%x)\n", BufferHandle[MAIN_DISP], BufferHandle[SECOND_DISP]);

	retIPC = IPC_FunctionCall(IPC_CMD_DISP_INIT, BufferHandle, sizeof(BufferHandle));

	if (retIPC)
		INFO("IPC function call (IPC_CMD_DISP_INIT) fails=%d\n", retIPC);
	return;
}

#if 0
static int fb_Get_Panel_Res(UINT32 *PanelWidth, UINT32 *PanelHeight, UINT32 *CVBS_Width, UINT32 *CVBS_Height)
{
	FB_Info fbWork;
	UINT32 retIPC = 0;

	retIPC = IPC_FunctionCall(IPC_CMD_DISP_GET_FB_INFO, &fbWork, sizeof(FB_Info));

	if (retIPC) {
		INFO("IPC function call (IPC_CMD_DISP_INIT) fails=%d\n", retIPC);
		return -1;
	}

	*PanelWidth = fbWork.PanelRes_width;
	*PanelHeight = fbWork.PanelRes_height;
	*CVBS_Width = fbWork.CVBSRes_width;
	*CVBS_Height = fbWork.CVBSRes_height;

	return 0;
}
#endif

static int fb_Get_UI_Res(UINT32 *UIWidth, UINT32 *UIHeight)
{
	FB_Info fbWork;
	UINT32 retIPC = 0;

	retIPC = IPC_FunctionCall(IPC_CMD_DISP_GET_FB_INFO, &fbWork, sizeof(FB_Info));

	if (retIPC) {
		INFO("IPC function call (IPC_CMD_DISP_INIT) fails=%d\n", retIPC);
		return -1;
	}

	*UIWidth = fbWork.UIRes_width;
	*UIHeight = fbWork.UIRes_height;

	return 0;
}

static int __init fbdss_init(void)
{
	int ret = 0;
	int BPP;
	struct fb_info *fbinfo = 0;
	framebuffer_t *fbWorkMem = 0;
	colormode_t* colormode;
	UINT32 UIWidth = 0, UIHeight = 0;
	//UINT32 PanelWidth = 0, PanelHeight = 0, CVBS_Width = 0, CVBS_Height = 0;
	UINT32 *SecondBuffer = NULL;
	UINT32 IsDualDisplay = 0;

	//ret = fb_Get_Panel_Res(&PanelWidth, &PanelHeight, &CVBS_Width, &CVBS_Height);
	ret = fb_Get_UI_Res(&UIWidth, &UIHeight);

	if (ret)
	{
		ERRF("Get UI resolution fails");
		goto ERROR_HANDLE_FB_INIT;
	}

	INFO("UI_Width=%d, UI_Height=%d\n", UIWidth, UIHeight);

	if (sp_config_get_int("is_dual_display", &IsDualDisplay))
	{
		INFO("No customer_type config in runtime.cfg\n");
		IsDualDisplay = 0;
	}

	//UIWidth = PanelWidth;
	//UIHeight = PanelHeight;

	fbinfo = framebuffer_alloc(sizeof(framebuffer_t), NULL);

	if (fbinfo == NULL)
	{
		ERRF("framebuffer_alloc failed\n");
		goto ERROR_HANDLE_FB_INIT;
	}

//	gFrameBufInfo = fbinfo;

	fbWorkMem = fbinfo->par;
	memset(fbWorkMem, 0, sizeof(framebuffer_t));
	fbWorkMem->fb = fbinfo;

	// Initial our framebuffer context
	strncpy(fbWorkMem->ID, "framebuffer", 6);
	fbWorkMem->ColorFmt = FB_DEFAULT_FMT;
	fbWorkMem->UIRes.width = UIWidth;
	fbWorkMem->UIRes.height = UIHeight;

	//fbWorkMem->panelRes.width = PanelWidth;
	//fbWorkMem->panelRes.height = PanelHeight;

	//fbWorkMem->SecondPanelRes.width = CVBS_Width;
	//fbWorkMem->SecondPanelRes.height = CVBS_Height;

	colormode = (colormode_t*)&ColorModeTab[fbWorkMem->ColorFmt];

	BPP = colormode->bits_per_pixel / 8;

	fbWorkMem->fbsize = PAGEALIGN(fbWorkMem->UIRes.width * fbWorkMem->UIRes.height * BPP) * FBBUFFER_NUM;

	//Allocate Buffer for MainDisplay and SecondDisplay
	fbWorkMem->fbmem = chunk_malloc(DRAM_B, fbWorkMem->fbsize);

	if (fbWorkMem->fbmem == NULL)
	{
		ERRF("chunk malloc failed, request size %d\n", fbWorkMem->fbsize);
		goto ERROR_HANDLE_FB_INIT;
	}

	memset(fbWorkMem->fbmem, 0, fbWorkMem->fbsize);

	if (IsDualDisplay)
	{
		DEBUG("Dual display\n");
		SecondBuffer = chunk_malloc(DRAM_B, fbWorkMem->fbsize);
	}
	else
	{
		DEBUG("Only main display\n");
		SecondBuffer = fbWorkMem->fbmem;
	}

	if (SecondBuffer == NULL)
	{
		ERRF("Second buffer is NULL, request size %d\n", fbWorkMem->fbsize);
		goto ERROR_HANDLE_FB_INIT;
	}
	else
	{
		memset(SecondBuffer, 0, fbWorkMem->fbsize);
	}

//	INFO("PanelRes %d x %d\n", fbWorkMem->panelRes.width, fbWorkMem->panelRes.height);
//	INFO("SecondPanelRes %d x %d\n", fbWorkMem->SecondPanelRes.width, fbWorkMem->SecondPanelRes.height);

	fbinfo->fbops = &framebuffer_ops;
	fbinfo->flags = FBINFO_FLAG_DEFAULT;
	fbinfo->pseudo_palette = NULL;
	fbinfo->screen_base = fbWorkMem->fbmem;
	fbinfo->screen_size = fbWorkMem->fbsize;

	// Resolution
	fbinfo->var.xres = fbWorkMem->UIRes.width;
	fbinfo->var.yres = fbWorkMem->UIRes.height;
	fbinfo->var.xres_virtual = fbinfo->var.xres;
	fbinfo->var.yres_virtual = fbinfo->var.yres * FBBUFFER_NUM;

	// Timing
	// [TODO] These setting base on panel info????
	fbinfo->var.left_margin		= 60;		// HBProch
	fbinfo->var.right_margin	= 16;		// HFPorch
	fbinfo->var.upper_margin	= 30;		// VBPorch
	fbinfo->var.lower_margin	= 9;		// VFPorch
	fbinfo->var.hsync_len		= 62;		// HSWidth
	fbinfo->var.vsync_len		= 6;		// VSWidth
	fbinfo->var.pixclock		= 37037;	// 1000000000000/27000000

	fbinfo->var.activate		= FB_ACTIVATE_FORCE;
	fbinfo->var.accel_flags		= 0;
	fbinfo->var.vmode			= FB_VMODE_NONINTERLACED;

	// color format
	fbinfo->var.bits_per_pixel	= colormode->bits_per_pixel;
	fbinfo->var.nonstd			= colormode->nonstd;
	fbinfo->var.red				= colormode->red;
	fbinfo->var.green			= colormode->green;
	fbinfo->var.blue			= colormode->blue;
	fbinfo->var.transp			= colormode->transp;

	// fixed info
	strcpy(fbinfo->fix.id, "framebuffer");
	fbinfo->fix.mmio_start	= letb123_DEVICE_BASE;
	fbinfo->fix.mmio_len	= 0x8000;
	fbinfo->fix.type		= FB_TYPE_PACKED_PIXELS;
	fbinfo->fix.type_aux	= 0;
	fbinfo->fix.visual		= FB_VISUAL_TRUECOLOR;
	fbinfo->fix.xpanstep	= 0;
	fbinfo->fix.ypanstep	= 1;
	fbinfo->fix.ywrapstep	= 0;
	fbinfo->fix.accel		= FB_ACCEL_NONE;
	fbinfo->fix.smem_start	= chunk_pa(fbWorkMem->fbmem);
	fbinfo->fix.smem_len	= fbWorkMem->fbsize;
	fbinfo->fix.line_length	= (fbinfo->var.xres_virtual * colormode->bits_per_pixel) / 8;

	DEBUG("fbmem VA 0x%x, PA 0x%x\n", (UINT32)fbWorkMem->fbmem, (UINT32)fbinfo->fix.smem_start);

	WakeUpEcosDisplay(chunk_pa(fbWorkMem->fbmem), chunk_pa(SecondBuffer));

	ret = register_framebuffer(fbinfo);

	//fbinfo->fix.smem_start	= chunk_pa(SecondBuffer);

	//ret = register_framebuffer(fbinfo);

	if (ret != 0)
	{
		ERRF("register framebuffer failed\n");
		goto ERROR_HANDLE_FB_INIT;
	}
	DEBUG("out, UI Res %dx%d\n", fbWorkMem->UIRes.width, fbWorkMem->UIRes.height);

	ret = 0;
	return ret;

ERROR_HANDLE_FB_INIT:

	if (fbWorkMem && (fbWorkMem->fbmem))
		chunk_free(fbWorkMem->fbmem);
	if (SecondBuffer)
		chunk_free(SecondBuffer);

	return -1;
}

static void __exit fbdss_exit(void)
{
	//Do nothing
	return;
}

module_init(fbdss_init);
module_exit(fbdss_exit);
MODULE_LICENSE("GPL");

