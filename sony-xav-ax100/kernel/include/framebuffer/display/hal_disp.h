#ifndef __DISP_GRAPHIC_HEADER_
#define __DISP_GRAPHIC_HEADER_
#if 0
#include <stdlib.h>
//Include gsl
#include <gsl.h>
#include <gsl/gl_cache.h>
#include <gsl/gl_malloc.h>
//End include gsl
#define GSL_INCLUDE
#endif
#include <framebuffer/display/hal_common.h>

#define USE_ECOS_SET
//#include <asm/atomic.h>
// 1. Common data --------------------------------------------------------------
/**************************************************************************
 *                           C O N S T A N T S                            *
 **************************************************************************/
//#define BIRD_VIEW
//#define VPP_QUEUE
#define TOOL_READY
#define IMG_G2D
#define DISP_LEGACY 0
//#define CANDY
#define WIDTH_TEST 300
#define HEIGHT_TEST 300

#define PANEL_WIDTH 720
#define PANEL_HEIGHT 480
#define DMIX_V_BP 0x14

#ifdef BIRD_VIEW
//#define BIRD_VIEW_SHOW_COUNT
//#define CASTLE_DEBUG
//#define BSD_SCALE   // cam 720x480 --> 320x240
#endif

#define DIVX_TEMP_NEEDED_MOD
#define HD_ICT_PROTECTION
#ifdef HD_ICT_PROTECTION
//#define HD_ICT_VIA_SUBPATH
#define HD_ICT_VIA_SWITCH_RESOLUTION
#define HD_ICT_VIA_WINDOW_MODE
#endif
#define DRV_VENC_USE_VPD_HW
//#define DRV_VENC_ICT_DIVX_CLEAR

#define RCS_SYNC_ID				2
#define MCP_VPP0_QUEUE_ID			3
#define MCP_VPP1_QUEUE_ID			4

//#define VPP_PRINT_CHKSUM
//#define VPP_PATH_OUT			0
#define VPP_TEST_IOCTL			0
//#define UI_ON_VIDEO    // This is using for test when display only have VPP
#define STC_AV0_ID              0
#define STC_AV1_ID              1
#define STC_AV2_ID              2

#define RGST_GRAPHIC_OFFSET 	213*128 			// Graphic HW reg G213-G224
#define RGST_TGEN_OFFSET 		203*128 			// TGEN HW reg G203-G204
#define RGST_DMIX_OFFSET 		205*128 			// TGEN HW reg G203-G204
#define RGST_VENC_OFFSET		209*128			// VENC HW reg G209-G212, DVE HW reg G225,
#define RGST_SUBPATH_OFFSET  	201*128			// HD2SD : G201, AFRC : G202, RTE : G228, TCIW : G229
#define RGST_VPP_OFFSET       	185*128			// VPP RGST = DDFCH(185, 186), DEINT(187, 188, 189, 190), VSCL(191, 192, 193, 194), NR(195,196), IMG(197,198), VPOST(199,200)
#define RGST_STC_OFFSET	       	12*128			// STC RGST : G012
#define RGST_STC_AV0_OFFSET     96*128                  // RGST STC_AV0: group 96
#define RGST_STC_AV1_OFFSET     97*128                  // RGST STC_AV1: group 97
#define RGST_STC_AV2_OFFSET     99*128                  // RGST STC_AV2: group 99
#define RGST_TTX_OFFSET	       	226*128
#define RGST_TNR_OFFSET		291*128		// TNR + TNRIW : G291~G296
#define RGST_TOTAL_SIZE	       	309*128		// TPIW1 : G308

#define DTG_INT_FIELD_START0    	0
#define DTG_INT_FIELD_END0	    	1
#define DTG_INT_FIELD_START1    	2
#define DTG_INT_FIELD_END1       	3
#define DTG_INT_USER1        		4
#define DTG_INT_USER3        		6
#define DTG_INT_AFRC        		9

#define MOON0_A4_MASK_CCIR656		0xfffc3fff
#define MOON0_A4_VAL_CCIR656			0x05
#define MOON0_A4_START_CCIR656		14			//14~17
#define MOON0_A4_MASK_HDMI			0xfff3ffff
#define MOON0_A4_VAL_HDMI_PLLTVX2	0x01
#define MOON0_A4_VAL_HDMI_PLLTV		0x02
#define MOON0_A4_START_HDMI			18			//  18~19
#define MOON0_A4_VAL_HDMI_W_0		0xffdfffff	// 21
#define MOON0_A4_VAL_HDMI_W_1		0x00200000
#define MOON0_A5_VAL_DMIX_PLLTV_0	0xffbfffff	// 22
#define MOON0_A5_VAL_DMIX_PLLDS_1	0x00400000	//22

#define DRV_VPP_IMG_DEGREE 256

typedef	struct {
	UINT32	dColorSpace; // x.v.Color ?						//(D)
	UINT32	dAspectRatio; //Width [31:16], Height [15:0]	//(D)

	UINT8	bMh5Mode;		// mheg5, zoom, or none application mode, (joeypan, 2009/11/03)
	UINT8	bMemMode;	//0: 1D, 1:2D. for 2D buffer, dLinePitch unit is mb_width, otherwise is picture width.
	UINT8	bMbWidth;	//mb_width for 2D mode, picture width for 1D mode
	UINT8	bMbWidthSD;
} SpatialAttr_t;

typedef struct {
	UINT32	dFrmRate;	//0, for free, others MSB 1:1001, 0:1000, LSB:n => n*1000/MSB	//(D, S)
	UINT32	dFrmDur; //predicated field count or stc (free frame rate)...	//(D, S)
	UINT32	dFrmDurCnt; //field duration count for fix frame rate.			//(D, S)
	UINT32	dFrctlRate;
	UINT32	dFrctlScale;
	INT64	slPTS;	// PTS	//(S)
} TemporalAttr_t;

typedef struct {
	UINT32	dXStart;
	UINT32	dYStart;
	UINT32	dXSize;
	UINT32	dYSize;
} Wnd_t;

typedef	struct {
	UINT32 dCodedWndXSize;
	UINT32 dCodedWndYSize;

	UINT32 dSrcWndXSize;
	UINT32 dSrcWndYSize;

	UINT32 dLogoScaleWidth;
	UINT32 dLogoScaleHeight;

	UINT32 dSrcPhaseX;	//for RV/VC1 phase shift information
	UINT32 dSrcPhaseY;

	UINT32	dPanScanNum;
	Wnd_t	stPanScanWnd[4];	//VC-1 may use 4 panscan windows,

	// pic attrib
	UINT32	dFormatTag;
	UINT32	dSrcInfo; // [bit0:first field, 0:top, 1:bot],
					  // [bit1:source type, 0:interlace, 1:progressive]
					  // [bit2:picture type,0:field, 1:frame]
					  // [bit3:repeat first,0:none, 1:repeat]
	UINT32	dPicOption; // PIC_RESET_VIDEO : how to set and use? aspect change, resolution change			//(D,S)
						// PIC_VALID : useless ?
						// PIC_FIRST : first display pic, not first decoded pic
						// PIC_CHK_DTS : reserved, not used now
						// PIC_CHK_PTS : sync reference
						//
	UINT32	dTarget; //display driver or pseudo render, controlled by sync -> 	//(S)
					 // for streaming pause, keep sync timing
					 // for background decode, sync flush to pseudo render as many pics as possible

	SpatialAttr_t stSpaAttr;
	TemporalAttr_t stTimeAttr;

    	UINT32  pic_aspect;
   	 UINT8   pic_active_format;	// EMI add for AFD. 2008.11.05
	UINT8	sml_set;
	UINT8 	key_frame;  //Key Frame flag for display decision (cvs.chen 100518)
	UINT16   dPlayItemId;
        UINT8 update_outtime; // semless_sync_refine
	INT32	dFramePoc; //Frame number info, for H264/MVC

	// afar @ 2010 0108
	UINT32	dDpbMemcfgMode;	// dCodedWndXSize, dSrcWndYSize
	UINT32	dLumaBufAddr;
	UINT32	dChromaBufAddr;
	UINT16	wLumaBufSize;
	UINT16	wChromaBufSize;
	UINT16	wLumaBufSizeSD;		// mc2sd buffer size
	UINT16	wChromaBufSizeSD;		// mc2sd buffer size

	UINT8	bSingleFieldSelect;
	UINT8	bBufType;

	INT32	grant_mc_hd2sd_request;
	UINT32	dCodecCustomCmd;  //decoder can write custom data to it after dec_set_fbvar initialize it
	UINT32	dFrmBufVarIdx;
	UINT32	dFrmBufIdx;
	INT32	view_id;
	UINT32	dCurrInstanceId;

	UINT8 *bPlaneOffsetInfo;// [7]:Plane_offset_direction_flag, [6:0]: Plane_offset_value
	// BD closed caption
	UINT8	cc_parsed;
	UINT8 cc_count;
	UINT8 cc_type[3];
	UINT8 cc_data1[3];
	UINT8 cc_data2[3];
	UINT32 DVB_cc_buffer_save;
	UINT32 DVB_cc_data_size;
	//UINT8 cc_fld_index; //Gavin, 2011-11-07

	UINT8   VC1_Skip_Supported_Flag;
	UINT8   VC1_Skip_Last_Flag;
	UINT8   VC1_Display_Count;
	UINT8   VC1_Decoder_Count;
	UINT8	VideoPatternCount;
} t_framebuf;

typedef struct
{
	INT32 	sdHstart;
	INT32 	sdVstart;
	UINT32 	dHsize;
	UINT32 	dVsize;
} DISP_Win_t;

typedef struct
{
	UINT32 CMD;
	UINT8 bSrcPlane;
	UINT8 bExpMode;
	UINT8 bAspectRatio;
	UINT8 reserved;
	UINT32 dAspectRatioExt;
}mid_package_SrcAspect;

typedef struct
{
	UINT32 CMD;
	UINT8 bAfdMode;
	UINT32 reserved[3];
} mid_package_SrcAfd;

typedef struct
{
	UINT32 CMD;
	UINT32 dAspectRatioExt;
	UINT8 bExpMode;
	UINT8 bAspectRatio;
	UINT8 reserved;
} mid_package_DispAspect;

typedef struct
{
	UINT32 CMD;
	DISP_Win_t stSrcWin;
	UINT8 bSrcPlaneMask;
	UINT8 reserved[3];
} mid_package_SrcWin;

typedef struct
{
	UINT32 CMD;
	UINT32 bDispPlane;
	DISP_Win_t stDispWin;
	UINT8 bSrcPlaneMask;
	UINT8 reserved[3];
} mid_package_DispWin;

typedef struct
{
	UINT32 CMD;
	UINT32 bDispFmt;
	DISP_Win_t stPanelWin;
} mid_package_DmixPlane;

typedef struct
{
	UINT32 CMD;
	UINT32 dInstId;
	UINT32 dLumaAnchor;
	UINT32 dChromaAnchor;
	t_framebuf pFramebuf;
} mid_package_DirectFlipBufferEx;

typedef enum
{
	DRV_OSD0 = 0,
	DRV_OSD1,
	DRV_OSD2,
	DRV_OSD_MAX
} DRV_OsdWindow_e;

typedef enum
{
	DRV_OSD_REGION_FORMAT_8BPP 			= 0x2,		/*!< 8 bit/pixel with CLUT */
	DRV_OSD_REGION_FORMAT_RGB_565		= 0x8,		/*!< 16 bit/pixel RGB 5:6:5 */
	DRV_OSD_REGION_FORMAT_ARGB_1555 	= 0x9,		/*!< 16 bit/pixel ARGB 1:5:5:5 */
	DRV_OSD_REGION_FORMAT_RGBA_4444 	= 0xA,		/*!< 16 bit/pixel RGBA 4:4:4:4 */
	DRV_OSD_REGION_FORMAT_ARGB_4444 	= 0xB,		/*!< 16 bit/pixel ARGB 4:4:4:4 */
	DRV_OSD_REGION_FORMAT_RGBA_8888 	= 0xD,		/*!< 32 bit/pixel RGBA 8:8:8:8 */
	DRV_OSD_REGION_FORMAT_ARGB_8888 	= 0xE		/*!< 32 bit/pixel ARGB 8:8:8:8 */
} DRV_OsdRegionFormat_e;

typedef enum
{
	DRV_OSD_ALIGN_128,
	DRV_OSD_ALIGN_256,
	DRV_OSD_ALIGN_512,
	DRV_OSD_ALIGN_1024,
	DRV_OSD_ALIGN_2048,
	DRV_OSD_ALIGN_4096
} DRV_OsdAlignSel_e;

typedef enum
{
	DRV_OSD_RGB_TO_BT709,	              /*!< OSD color convert is from RGB to YUV BT709 */
	DRV_OSD_RGB_TO_BT601,	          	/*!< OSD color convert is from RGB to YUV BT601 */
	DRV_OSD_BT601_TO_BT709,		      	/*!< OSD color convert is from YUV BT601 to BT709 */
	DRV_OSD_BT709_TO_BT601,		      	/*!< OSD color convert is from YUV BT709 to BT601 */
	DRV_OSD_BYPASS,				      	/*!< OSD color convert is bypass */
	DRV_OSD_YUV_TO_BT709,		       /*!< OSD color convert is from YUV FULL to BT709 */
	DRV_OSD_YUV_TO_BT601,		       /*!< OSD color convert is from YUV FULL to BT601 */
	DRV_OSD_RGB_TO_YUV_FULL,	              /*!< OSD color convert is from RGB to YUV FULL */
	MAX_COLOR_CONVERT,
}DRV_OsdColorConvert_e;

typedef enum
{
	DRV_OSD_GBRA_MODE,	/*!< OSD Color LookUp Table (CLUT) data is filled in GBRA color ordering */
	DRV_OSD_ARGB_MODE,	/*!< OSD Color LookUp Table (CLUT) data is filled in ARGB color ordering */
	MAX_CLUT_MODE,
}DRV_OsdClutMode_e;

typedef enum
{
	DRV_OSD_TRANSPARENCY_DISABLED = 0,	/*!< transparency is disabled */
	DRV_OSD_TRANSPARENCY_ALL			/*!< the whole region is transparent */
} DRV_OsdTransparencyMode_e;

typedef enum
{
	DRV_OSD_BLEND_REPLACE,		/*!< OSD blend method is region alpha replace */
	DRV_OSD_BLEND_MULTIPLIER,	/*!< OSD blend method is region alpha multiplier */
	MAX_BLEND_METHOD,
}DRV_OsdBlendMethod_e;

typedef enum
{
	DRV_OSD_RGB,			    /*!< OSD color space is RGB color domain */
	DRV_OSD_YUV_FULL,		    /*!< OSD color space is YUV full range color domain */
	DRV_OSD_YUV_BT601,		    /*!< OSD color space is YUV BT601 color domain */
	DRV_OSD_YUV_BT709,		    /*!< OSD color space is YUV BT709 color domain */
	MAX_COLOR_SPACE,
}DRV_OsdColorSpace_e;

typedef enum
{
	DRV_OSD_CMD_A16B16,		/*!< DRAM fetch command with A16B16 */
	DRV_OSD_CMD_A32B32,		/*!< DRAM fetch command with A32B32 */
	DRV_OSD_CMD_AUTO,		/*!< transparency is disabled */
	DRV_OSD_CMD_MAX
}DRV_OsdMemFetchCmd_e;

#define DRV_OSD_DEFLICKER_MIN		0
#define DRV_OSD_DEFLICKER_MAX		0x7F

typedef enum
{
	DRV_Disp_Out0 = 0,
	DRV_Disp_Out1,
	DRV_Disp_Out_MAX
} DRV_DispOutID_e;

typedef enum
{
	DRV_FMT_480P = 0,
	DRV_FMT_576P,
	DRV_FMT_720P,
	DRV_FMT_1080P,
	DRV_FMT_4K_2K_24_25_30,
	DRV_FMT_4K_2K_SMPT,
	DRV_FMT_SMALL1,
	DRV_FMT_SMALL2,
	DRV_FMT_USER_MODE,
} DRV_VideoFormat_e;

typedef enum
{
	DRV_FrameRate_60Hz = 0,
	DRV_FrameRate_50Hz,
	DRV_FrameRate_24Hz,
	DRV_FrameRate_30Hz,
	DRV_FrameRate_25Hz,
	DRV_FrameRate_60Hz_3D,
	DRV_FrameRate_50Hz_3D,
	DRV_FrameRate_24Hz_3D,
	DRV_FrameRate_MAX
} DRV_FrameRate_e;

typedef enum
{
	DRV_DMIX_VPP = 0,
	DRV_DMIX_VPP2,
	DRV_DMIX_OSD2,
	DRV_DMIX_OSD0,
	DRV_DMIX_OSD1,
	DRV_DMIX_OSD3,
	DRV_DMIX_BGC,
	DRV_DMIX_DEFAULT,
	DRV_DMIX_BPG,
} DRV_DMIX_InputSel_e;

typedef enum
{
	DRV_DMIX_AlphaBlend,
	DRV_DMIX_Transparent,
	DRV_DMIX_Opacity,
} DRV_DMIX_LayerMode_e;

typedef enum
{
	DRV_DMIX_L1,
	DRV_DMIX_L2,
	DRV_DMIX_L3,
	DRV_DMIX_L4,
	DRV_DMIX_L5,
	DRV_DMIX_L6,
} DRV_DMIX_LayerId_e;

typedef enum
{
	DRV_DMIX_TCIW = 10,
	DRV_DMIX_HD2SD = 11,
	DRV_DMIX_RTE
} DRV_DMIX_SubPath_e;

typedef enum
{
	DRV_TGEN_VPP = 0,
	DRV_TGEN_reserved0,
	DRV_TGEN_VPP2,
	DRV_TGEN_OSD0,
	DRV_TGEN_OSD1,
	DRV_TGEN_OSD2,
	DRV_TGEN_PTG,
	DRV_TGEN_HD2SD,
	DRV_TGEN_TVE,
	DRV_TGEN_ALL
} DRV_TGEN_Input_e;

typedef enum
{
	DisplayInit_e = 0,
	SetSrcAspect_e = 1,
	GetSrcAspect_e = 2,
	GetSrcAfd_e = 3,
	SetDispAspect_e = 4,
	GetDispAspect_e = 5,
	SetSrcWin_e = 6,
	GetSrcWin_e = 7,
	GetCodecSize_e = 8,
	GetSrcMemInfoEx_e = 9,
	GetSrcMemInfo_e = 10,
	SetExtScanInfo_e = 11,
	GetExtScanInfo_e = 12,
	SetDispWin_e = 13,
	GetDispWin_e = 14,
	SetSrcWin_and_SetDispWin_e = 15,
	SetDmixPlane_e = 16,
	GetDmixPlane_e = 17,
	SetAmixPlane_e = 18,
	GetAmixPlane_e = 19,
	EnableOsdScaling_e = 20,
	DisableOsdScaling_e = 21,
	UpdateScaling_e = 22,
	SetDispFrmRate_e = 23,
	GetDispFrmRate_e = 24,
	SetHue_e = 25,
	SetSaturation_e = 26,
	SetContrast_e = 27,
	SetBrightness_e = 28,
	SetBgColor_e = 29,
	SetDispOutput_e = 30,
	GetDispOutput_e = 31,
	SetBleEnable_e = 32,
	SetSrcPlaneEnable_e = 33,
	GetSrcPlaneEnable_e = 34,
	SetDacOutputPath_e = 35,
	SetMaskSel_e = 36,
	SetForceDispMode_e = 37,
	SetDispEnableEx_e = 38,
	GetDispEnableEx_e = 39,
	DirectFlipBufferEx_e = 40,
	GetActivePlane_e = 41,
	GetATGFormat_e = 42,
	GetDTGFormat_e = 43,
	SetVpp2BackgroundLayer_e = 44,
	GetVpp2BackgroundLayer_e = 45,
	SetG2dBackgroundLayer_e = 46,
	GetG2dBackgroundLayer_e = 47,
} DRV_DISP_MID_CMD_e;

//*********
//** VENC **
//*********
#define OUTPUT_ASPECT_NORMAL            (OUTPUT_TV_ASPECT_4_3|OUTPUT_SRC_ASPECT_4_3)
#define OUTPUT_ASPECT_ANAMORPHIC        (OUTPUT_TV_ASPECT_16_9|OUTPUT_SRC_ASPECT_16_9)
#define OUTPUT_ASPECT_LETTERBOX         (OUTPUT_TV_ASPECT_4_3|OUTPUT_SRC_ASPECT_16_9)
#define OUTPUT_ASPECT_PANSCAN           (OUTPUT_TV_ASPECT_4_3|OUTPUT_SRC_ASPECT_16_9|OUTPUT_ENLARGE)
#define OUTPUT_ASPECT_COMPRESSED        (OUTPUT_TV_ASPECT_16_9|OUTPUT_SRC_ASPECT_4_3)
//cvs add for DTG+
#define OUTPUT_ASPECT_ENLARGE_169LB		(OUTPUT_TV_ASPECT_4_3|OUTPUT_SRC_ASPECT_4_3_169LB|OUTPUT_ENLARGE)	// 16:9 LB scale to 16:9 Frame
#define OUTPUT_ASPECT_ENLARGE_149LB		(OUTPUT_TV_ASPECT_4_3|OUTPUT_SRC_ASPECT_4_3_149LB|OUTPUT_ENLARGE)	// Increase display height and reduce width to correct geometry
#define OUTPUT_ASPECT_ENLARGE_149LBn169	(OUTPUT_TV_ASPECT_16_9|OUTPUT_SRC_ASPECT_4_3_149LB|OUTPUT_ENLARGE)

#define OUTPUT_TV_ASPECT_4_3    (0<<4)
#define OUTPUT_TV_ASPECT_16_9   (1<<4)
#define OUTPUT_TV_ASPECT_14_9   (2<<4)
#define OUTPUT_TV_ASPECT_MASK   (0x0f<<4)
#define OUTPUT_SRC_ASPECT_4_3   (0<<0)
#define OUTPUT_SRC_ASPECT_16_9  (1<<0)
// EMI add for DTG + //cvs sync from 1002 codebase
#define OUTPUT_SRC_ASPECT_4_3_169LB  	(2<<0)	// MPEG 4:3 with AFD 2
#define OUTPUT_SRC_ASPECT_4_3_149LB 	(3<<0)	// MPEG 4:3 with AFD 3
#define OUTPUT_SRC_ASPECT_16_9_149LB 	(4<<0)	// MPEG 16:9 with AFD 3
// EMI add for DTG -
#define OUTPUT_SRC_ASPECT_MASK  (0x0f<<0)
#define OUTPUT_ENLARGE          (1<<7)

// EMI add for DTG + //cvs sync from 1002 codebase
// MPEG 4:3, TV 4:3, AFD = 2
#define OUTPUT_ASPECT_NORMAL_169LB		(OUTPUT_TV_ASPECT_4_3|OUTPUT_SRC_ASPECT_4_3_169LB)	// AFD2_16x9
// MPEG 4:3, TV 4:3, AFD = 3
#define OUTPUT_ASPECT_NORMAL_149LB		(OUTPUT_TV_ASPECT_4_3|OUTPUT_SRC_ASPECT_4_3_149LB)	// AFD3_14x9
// MPEG 4:3, TV 4:3, AFD = 5
#define OUTPUT_ASPECT_NORMAL_43SnP149	(OUTPUT_TV_ASPECT_14_9|OUTPUT_SRC_ASPECT_4_3)			// AFD5_4x3_SHOOT_PROTECT_14x9
// MPEG 16:9, TV 4:3, AFD = 3
#define OUTPUT_ASPECT_LETTERBOX_149PB	(OUTPUT_TV_ASPECT_4_3|OUTPUT_SRC_ASPECT_16_9_149LB)	// AFD3_14x9
//MPEG 16:9, TV 4:3, AFD = 5
#define OUTPUT_ASPECT_PANSCAN_43SnP149	(OUTPUT_TV_ASPECT_14_9|OUTPUT_SRC_ASPECT_16_9|OUTPUT_ENLARGE)	// AFD5_4x3_SHOOT_PROTECT_14x9
// EMI add for DTG -

// Logo v2.0 flags
#define HORIZONTAL_CENTERING  	1<<0
#define VERTICAL_CENTERING    	1<<1
#define LOGO_DATA_VFLIP	      	1<<2

//Frame buffer information
typedef struct fb_BitField_
{
	UINT32 offset;			/* beginning of bitfield	*/
	UINT32 length;			/* length of bitfield		*/
	UINT32 msb_right;		/* != 0 : Most significant bit is */
} FB_BitField;

typedef struct
{
	//If you modified this structure don't forget to modify linux side header.
	volatile unsigned int UIRes_width;
	volatile unsigned int UIRes_height;
	volatile unsigned int PanelRes_width;
	volatile unsigned int PanelRes_height;
	volatile unsigned int CVBSRes_width;
	volatile unsigned int CVBSRes_height;
	volatile unsigned int fbsize;
	volatile unsigned int MainbufferAddr;
	volatile unsigned int SecbufferAddr;
	UINT32 bits_per_pixel;
	UINT32 OsdHandle[DRV_OSD_MAX];
	//
	FB_BitField red;		/* bitfield in fb mem if true color, */
	FB_BitField green;	/* else only length is significant */
	FB_BitField blue;
	FB_BitField transp;	/* transparency			*/
} FB_Info;

typedef struct _DRV_Logo_header_t
{
	unsigned int Identifier;  	// Must be 0x4c4f474f = 'LOGO'
	unsigned int x;            	// start position
	unsigned int y;            	// start position
	unsigned int width;      	// logo data width
	unsigned int height;		// logo data height
	unsigned int location_flag; // Horizontal_centering and Vertical_centering
	unsigned int reserved[2];
} DRV_Logo_header_t;

typedef enum
{
	DRV_VENC_HDMI_525_N = 0,         	// 60/59.94/30/29.97
	DRV_VENC_HDMI_625_P = 1,	       	// 50/25
	DRV_VENC_HDMI_1080_N = 2,			// 60/59.94/30/29.97
	DRV_VENC_HDMI_1080_P = 3,			// 50/25
	DRV_VENC_HDMI_1080_P_1250 = 5,  	// 50/25
	DRV_VENC_HDMI_720_N = 6,			// 60/59.94
	DRV_VENC_HDMI_720_P = 7,			// 50
	DRV_VENC_HDMI_1080_24 = 10,
	DRV_VENC_HDMI_720_24 = 14,
	DRV_VENC_HDMI_4K2K_P30 = 15,
	DRV_VENC_HDMI_4K2K_P25 = 16,
	DRV_VENC_HDMI_4K2K_P24 = 17,
	DRV_VENC_HDMI_4K2K_SMPTE = 18,
} DRV_VENC_HDMIfmt_e;

typedef enum
{
	DRV_VENC_HDMI_INTERLACE = 1,
	DRV_VENC_HDMI_PROGRESSIVE,
} DRV_VENC_HDMItype_e;

typedef enum
{
	DRV_VENC_SRC_DMIX = 0,
	DRV_VENC_SRC_HD2SD,
} DRV_VENC_InputSource_e;

typedef enum{
	DRV_VENC_FRM_RATE_60 = 0,	/*!< frame rate is 59.94 */
	DRV_VENC_FRM_RATE_50,	/*!< frame rate is 50 */
	DRV_VENC_FRM_RATE_24,	/*!< frame rate is 24 */
	DRV_VENC_FRM_RATE_30,
	DRV_VENC_FRM_RATE_25,
	DRV_VENC_FRM_RATE_SUB_USR_60,
	DRV_VENC_FRM_RATE_SUB_USR_50,
	DRV_VENC_FRM_RATE_MAX
}DRV_VENC_FrmRate_e;

typedef enum
{
	DRV_VENC_FMT_480P60,		/*!< Main channel output format as 480P60 */
	DRV_VENC_FMT_480I60,		/*!< Main channel output format as 480I60 */
	DRV_VENC_FMT_576P50,		/*!< Main channel output format as 576P50 */
	DRV_VENC_FMT_576I50,		/*!< Main channel output format as 576I50 */
	DRV_VENC_FMT_720P60,		/*!< Main channel output format as 720P60 */
	DRV_VENC_FMT_720P50,		/*!< Main channel output format as 720P50 */
	DRV_VENC_FMT_720P24,		/*!< Main channel output format as 720P24 */
	DRV_VENC_FMT_1080I60,	/*!< Main channel output format as 1080I60 */
	DRV_VENC_FMT_1080I50,	/*!< Main channel output format as 1080I50 */
	DRV_VENC_FMT_1080P60,	/*!< Main channel output format as 1080P60 */
	DRV_VENC_FMT_1080P50,	/*!< Main channel output format as 1080P50 */
	DRV_VENC_FMT_1080P24,	/*!< Main channel output format as 1080P24 */
	DRV_VENC_FMT_3D_720P60, /*!< output format is 720P60 3D */
	DRV_VENC_FMT_3D_720P50, /*!< output format is 720P50 3D*/
	DRV_VENC_FMT_3D_1080P24,/*!< output format is 1080P24  3D*/
	DRV_VENC_FMT_4K2KP30,
	DRV_VENC_FMT_4K2KP25,
	DRV_VENC_FMT_4K2KP24,
	DRV_VENC_FMT_4K2K_SMPTE,
	DRV_VENC_FMT_EXTENDED,	/*!< output format is user defined */
	MAX_FORMAT_TYPE,
} DRV_VENC_GeneralFmt_e;

typedef enum
{
	DRV_VENC_FRM_TYPE_INTERLACE,		/*!< Frame rate is Interalce */
	DRV_VENC_FRM_TYPE_PROGRESSIVE,		/*!< Frame rate is Progressive */
	MAX_FRM_TYPE
} DRV_VENC_FrmType_e;

typedef enum
{
	DRV_VENC_MAIN_PATH_SEL,	/*!< Select main path output */
	DRV_VENC_SUB_PATH_SEL,		/*!< Select sub path output */
	VENC_MAX_PATH_SEL
} DRV_VENC_OutPath_e;

typedef enum
{
	DRV_VENC_DAC_TVRGB,	/*!< Scart RGB (Interlace-RGB) output */
	DRV_VENC_DAC_PCRGB,	/*!< VGA (Progressive-RGB) output */
	DRV_VENC_DAC_YUV,	/*!< YUV mode output */
	DRV_VENC_DAC_SVIDEO, /*!< S-Video mode output */
	DRV_VENC_DAC_CVBS,
	MAX_DAC_TYPE,
} DRV_VENC_DacOutput_e;

typedef enum{
	VENC_FMT_480P = 0,		/*!< plane format is 480P */
	VENC_FMT_576P,		/*!< plane format is 576P */
	VENC_FMT_720P,		/*!< plane format is 720P */
	VENC_FMT_1080P,	/*!< plane format is 1080P */
	VENC_FMT_4k2k,
	VENC_FMT_4k2k_smpte,
	VENC_FMT_MAX
}DRV_VENC_Fmt_e;

typedef enum {
	DRV_VENC_LINE_CTL_TOP = 0,
	DRV_VENC_LINE_CTL_BTM,
	DRV_VENC_LINE_CTL_BOTH
} DRV_VENC_TtxLineCtlSel_e;

typedef enum
{
	DRV_VPP_NORMALSCAN,
	DRV_VPP_OVERSCAN,
	DRV_VPP_UNDERSCAN,
	DRV_VPP_SCANMAX
} DRV_VPP_OverUnderScan_e;

typedef enum
{
	//Ntsc fmt
	TV_FMT_NTSC, 		/*!< NTSC_M (Ntsc fmt) */
	TV_FMT_NTSC_J,		/*!< NTSC_J (Ntsc fmt) */
	TV_FMT_NTSC_443,	/*!< NTSC_443 (Ntsc fmt) */
	TV_FMT_PAL_M,		/*!< PAL_M (Ntsc fmt) */
	TV_FMT_PAL_60,		/*!< PAL_60 (Ntsc fmt) */
	MAX_TV_FMT_NTSC_TYPE,
	//Pal fmt
	TV_FMT_PAL,			/*!< PAL (Pal fmt) */
	TV_FMT_PAL_N,		/*!< PAL_N (Pal fmt) */
	TV_FMT_PAL_NC,		/*!< PAL_Nc (Pal fmt) */
	MAX_TV_FMT_PAL_TYPE,
}DRV_TVE_TveFmt_e;

//Barney added enum from 8602 platform_venc.h

/**
 *@brief   platform_venc  return value enum
 */
typedef enum
{
	PLATFORM_VENC_FAILURE,		/*!< parameters no defined	*/
	PLATFORM_VENC_SUCCESS,		/*!< successful outcome	*/
} DRV_Platform_VENC_Status_e;

/**
 *	\brief Enable/ disable Dac Current
 */
typedef enum
{
	DAC_ENABLE,
	DAC_DISABLE,
	DAC_MAX_SEL,
} DRV_Platform_VENC_DACSel_e;

/**
 *	\brief Dac_ABCD Current mode
 */
typedef enum
{
	FULL_CURRENT,		/*!< Full current mode */
	QUARTER_CURRENT, /*!< 1/4 Current mode */
} DRV_Platform_CurrentMode_e;

/**
 *	\brief On/Off mode
 */
typedef enum
{
	SEL_OFF,
	SEL_ON,
	SEL_MAX,
} DRV_Platform_VENC_Select_e;

/**
 *	\brief AVPD period time, unit is second
 */
typedef enum
{
	PERIOD_TIME_HALF,		/*!< period time is 0.5s	*/
	PERIOD_TIME_ONE,		/*!< period time is 1.0s	*/
	PERIOD_TIME_ONE_HALF,	/*!< period time is 1.5s	*/
	PERIOD_TIME_TWO,		/*!< period time is 2.0s	*/
	PERIOD_TIME_MAX,
} DRV_Platform_VENC_PeriodTimeSel_e;
//End Barney added



//********
//** CPS **
//********
typedef enum
{
	DRV_CPS_ENCRYPTION_MODE,
	DRV_CPS_APSTB,
	DRV_CPS_CGMS,
	DRV_CPS_EMI,	// Encryption Mode Indicator
	DRV_CPS_EPN,	// Encryption Plus Non-assertion
	DRV_CPS_DOT,	// Digital Only Token
	DRV_CPS_ICT,	// Image Constrained Token
	DRV_CPS_ASC,	// AACS Signed CSS
	DRV_CPS_MKB,	// Media Key Block
	DRV_CPS_RCT,	// Redistribution Control Token
	DRV_CPS_DCI_CCI,
	DRV_CPS_DIVX_DRM,
	DRV_CPS_DVD_CP,
	DRV_CPS_MAX_TYPE
} DRV_CPS_InfoType_e;

//********
//** VPP **
//********
typedef enum
{
	DRV_VPP_DisplaySel_MAIN = 0,
	DRV_VPP_DisplaySel_SECOND,
	DRV_VPP_DisplaySel_NUM
} DRV_VPP_DisplaySel_e;

typedef enum
{
	DRV_VPP_MODULE_0 = 0,
	DRV_VPP_MODULE_1,
	DRV_VPP_MODULE_NUM
} DRV_VPP_ModuleID_e;

typedef enum
{
	DRV_VPP_BIST_DDFCH = 0,
	DRV_VPP_BIST_VSCL_COLORBAR,
	DRV_VPP_BIST_VSCL_BOUNDARY,
	DRV_VPP_BIST_VPOST
	//DRV_VPP_BIST_CLEARALL
} DRV_VPP_BistMode_e;

typedef enum
{
	DRV_VPP_YUV420 = 0x0000,
	DRV_VPP_YUV422 = 0x0700,
} DRV_VPP_YuvFmt_e;


typedef enum
{
	DRV_VPP_SWITCH_OFF = 0,
	DRV_VPP_SWITCH_ON,
} DRV_VPP_SwitchOnOff_e;

typedef enum
{
	DRV_VPP_FRAME = 0,
	DRV_VPP_FIELD,
} DRV_VPP_FieldFrameSel_e;

typedef enum
{
	DRV_VPP_TOP = 0,
	DRV_VPP_BOT,
} DRV_VPP_TopBottomSel_e;

typedef enum
{
	DRV_VPP_PROGRESSIVE = 0,
	DRV_VPP_INTERLACED,
} DRV_VPP_PorISel_e;

typedef enum
{
	DRV_VPP_FRAME_0 = 0,
	DRV_VPP_FRAME_1,
	DRV_VPP_FRAME_2,
} DRV_VPP_FrmAddrId_e;

typedef enum
{
	DRV_VPP_DDFCH_CB_OFF = 0x0,
	DRV_VPP_DDFCH_CB_BLACK = 0x01010001,
	DRV_VPP_DDFCH_CB_WHITE = 0x010101ff,
	//DRV_VPP_DDFCH_CB_RED = 0x0,
	DRV_VPP_DDFCH_CB_GREEN = 0x000001ff,
	//DRV_VPP_DDFCH_CB_BLUE = 0x0,
	DRV_VPP_DDFCH_CB_YELLOW = 0x010001ff,
} DRV_VPP_DdfchColorBar_e;

typedef enum
{
	DRV_VPP_CROMA_INTERPOL = 0,
	DRV_VPP_CROMA_REPEAT,		//only used in debug ?
} DRV_VPP_ChromaUpsample_e;

typedef enum
{
	DRV_VPP_DECIMATE_BY_2 = 0,
	DRV_VPP_DECIMATE_BY_4,
} DRV_VPP_DecimationMode_e;

typedef enum
{
	DRV_VPP_DERING_ED_ORTH = 0,	//orthogonal only
	DRV_VPP_DERING_ED_BOTH,		//both orthogonal and diagonal
} DRV_VPP_DeringDetectMode_e;

typedef enum
{
	DRV_VPP_DERING_FIL_BOTH = 0,	//both vertical filter and horizontal filter
	DRV_VPP_DERING_FIL_HOR,		//horizontal filter only
} DRV_VPP_DeringFilterMode_e;

typedef enum
{
	DRV_VPP_DERING_SM_121 = 0,
	DRV_VPP_DERING_SM_565,
} DRV_VPP_DeringSmooth_e;

typedef enum
{
	//DRV_VPP_DEINT_NONE = 0,
	//DRV_VPP_DEINT_2D_MODE,
	//DRV_VPP_DEINT_3D_MODE,
	DRV_VPP_DEINT_2D_MODE = 1,			//single-field deinterlacing
	DRV_VPP_DEINT_3D_MERGE = 2,		//pull-down merge mode
	DRV_VPP_DEINT_3D_MIX = 4,			//normal deinterlacing without motion-information
	DRV_VPP_DEINT_3D_MIX_F = 5,			// forced DRV_VPP_DEINT_3D_MIX
	DRV_VPP_DEINT_3D_MOTION = 6,		//motion adaptive deinterlacing
	DRV_VPP_DEINT_3D_MOTION_PAUSE = 14,
	DRV_VPP_DEINT_PROG_SINGLE = 16,	// direct output frames, turn off HW DEINT module
	DRV_VPP_DEINT_PROG_MERGE = 17,		// normal progressive : 50p, 60p, 24p
	DRV_VPP_DEINT_MAX = 18
} DRV_VPP_DeintMode_e;

typedef enum
{
	DRV_VPP_DEINT_DIR_NEXT = 0,
	DRV_VPP_DEINT_DIR_PREV = 2,
	DRV_VPP_DEINT_DIR_AUTO
} DRV_VPP_DeintDirection_e;

typedef enum
{
	DRV_VPP_DEINT_UPDATE_F0,
	DRV_VPP_DEINT_UPDATE_F1,
//	DRV_VPP_DEINT_UPDATE_CLEAR
} DRV_VPP_DeintUpdate_e;


typedef enum
{
	DRV_VPP_PKDS_WEAK = 0,
	DRV_VPP_PKDS_TYPICAL,
	DRV_VPP_PKDS_STRONG,
	DRV_VPP_PKUS_WEAK,
	DRV_VPP_PKUS_TYPICAL,
	DRV_VPP_PKUS_STRONG,
	DRV_VPP_PK_MAX
} DRV_VPP_PeakingTableIndex_e;

typedef enum
{
	DRV_VPP_TIDS_WEAK = 0,
	DRV_VPP_TIDS_TYPICAL,
	DRV_VPP_TIDS_STRONG,
	DRV_VPP_TIUS_WEAK,
	DRV_VPP_TIUS_TYPICAL,
	DRV_VPP_TIUS_STRONG,
	DRV_VPP_TI_MAX
} DRV_VPP_TransienceTableIndex_e;

typedef enum
{
	DRV_VPP_PK_OFF = 0,
	DRV_VPP_PK_ON_BASIC = 3,
		DRV_VPP_PK_ON_EP1 = 7,
		DRV_VPP_PK_ON_BP1 = 11,
		DRV_VPP_PK_ON_EP1_BP1 = 15,
	DRV_VPP_PK_ON_DEFAULT = 35,//HP1
		DRV_VPP_PK_ON_EP1_HP1 = 39,
		DRV_VPP_PK_ON_BP1_HP1 = 43,
		DRV_VPP_PK_ON_ALL1 = 47,
		DRV_VPP_PK_ON_LPF = 67,
		DRV_VPP_PK_ON_LPF_EP1 = 71,
		DRV_VPP_PK_ON_LPF_BP1 = 75,
		DRV_VPP_PK_ON_LPF_HP1 = 99,
		DRV_VPP_PK_ON_LPF_EP1_HP1 = 103,
		DRV_VPP_PK_ON_LPF_BP1_HP1 = 107,
		DRV_VPP_PK_ON_LPF_ALL1 = 111
} DRV_VPP_PeakingEnable_e;

typedef enum
{
	DRV_VPP_TI_OFF = 0,
	DRV_VPP_TI_ON_DEFAULT = 3,
		DRV_VPP_TI_ON_EP1 = 7,
		DRV_VPP_TI_ON_BP1 = 11,
		DRV_VPP_TI_ON_EP1_BP1 = 15,
		DRV_VPP_TI_ON_ST1 = 19,
		DRV_VPP_TI_ON_ST1_EP1 = 23,
		DRV_VPP_TI_ON_ST1_BP1 = 27,
		DRV_VPP_TI_ON_ST1_EP1_BP1 = 31,
		DRV_VPP_TI_ON_BST1 = 35,
		DRV_VPP_TI_ON_BST1_EP1 = 39,
		DRV_VPP_TI_ON_BST1_BP1 = 43,
		DRV_VPP_TI_ON_BST1_EP1_BP1 = 47,
		DRV_VPP_TI_ON_BST2 = 51,
		DRV_VPP_TI_ON_BST2_EP1 = 55,
		DRV_VPP_TI_ON_BST2_BP1 = 59,
		DRV_VPP_TI_ON_BST2_EP1_BP1 = 63
} DRV_VPP_TransienceEnable_e;


typedef enum
{
	DRV_VPP_CS_601N,
	DRV_VPP_CS_601P,
	DRV_VPP_CS_709,
	DRV_VPP_CS_XV,
	DRV_VPP_CS_JPG,
	DRV_VPP_CS_MAX
} DRV_VPP_ColorSpace_e;

typedef enum
{
	DRV_VPP_FIELDSEL_I = 0,		///< field select decoding:interlace
	DRV_VPP_FIELDSEL_T,			///< field select decoding:top
	DRV_VPP_FIELDSEL_B,			///< field select decoding:bottom
	DRV_VPP_FIELDSEL_P			///< field select decoding:progressive
} DRV_VPP_FieldSelDecode_e;

typedef enum
{	// G185, A23, B0~5
	/*DRV_VPP_PCN_FLD_BTB = 0x1f,
	DRV_VPP_PCN_FLD_TBT = 0x35,
	DRV_VPP_PCN_FRM_BTB = 0x0a,
	DRV_VPP_PCN_FRM_TBT = 0x20,
	DRV_VPP_PCN_FRM_TTT = 0x0*/
	// G185, A23, B1,3,5
	DRV_VPP_PCN_BTB = 0x0a,
	DRV_VPP_PCN_TBT = 0x20,
	DRV_VPP_PCN_TTT = 0x0
} DRV_VPP_PCNTopBot_e;

typedef enum
{	//G185, A24, B21~26,   val << 21
	//DRV_VPP_PCN_ID_001 = 0x04,
	//DRV_VPP_PCN_ID_011 = 0x14
	DRV_VPP_PCN_ID_111 = 0x15,
	DRV_VPP_PCN_ID_011 = 0x14,
	DRV_VPP_PCN_ID_112 = 0x19
} DRV_VPP_PCNAddrId_e;

typedef enum
{
	// G185, A23, B27~29
	DRV_VPP_PCN_SHF_BTB = 0x03,
	DRV_VPP_PCN_SHF_TBT = 0x04
} DRV_VPP_PCNShift_e;

typedef enum
{
	DRV_VPP_MEM_1D,
	DRV_VPP_MEM_2D
} DRV_VPP_Mem1d2d_e;

typedef enum
{
	//G185, A23, B12~17
	DRV_VPP_JUMP_1_LINE = 0x0015,
	DRV_VPP_JUMP_2_LINE = 0x002a
} DRV_VPP_Jump1Or2Line_e;

typedef enum
{
	DRV_VPP_PULLDOWN_TYPE_2323 = 0x037b,
	DRV_VPP_PULLDOWN_TYPE_3232 = 0x01ef,
	DRV_VPP_PULLDOWN_TYPE_4222 = 0x00ff,
	DRV_VPP_PULLDOWN_TYPE_2223_P = 0x03ff,
	DRV_VPP_PULLDOWN_TYPE_2223_A = 0x03fffffb
} DRV_VPP_PullDownType_e;

typedef enum
{
	DRV_VPP_DISABLE_BY_DEFAULT = 0,
	DRV_VPP_DISABLE_BY_CB,
//	DRV_VPP_DISABLE_BY_SIZE,
	DRV_VPP_DISABLE_BY_DMIX,
	DRV_VPP_DISABLE_BY_UI,		//TEST : UI TO VIDEO PATH
	DRV_VPP_DISABLE_OFF
} DRV_VPP_DisableVideoBy_e;

typedef enum
{
	DRV_VPP_UPDATE_BY_MCPLAYER = 0,
	DRV_VPP_UPDATE_BY_FRAMEBUFFER,
	DRV_VPP_UPDATE_BY_REARVIEW,
	DRV_VPP_UPDATE_BY_DISABLE_VPP,	//currently used by RCS NP change issue
//	DRV_VPP_UPDATE_BY_BIRDVIEW,
	DRV_VPP_UPDATE_MAX
} DRV_VPP_UpdatedBy_e;

/*typedef enum
{
	DRV_VPP_PLAYER_STOP = 0,
	DRV_VPP_PLAYER_START,
	DRV_VPP_PLAYER_PAUSE,
	DRV_VPP_PLAYER_FF,
	DRV_VPP_PLAYER_FB
} DRV_VPP_PlayerStatus_e;*/

typedef enum
{
	DRV_VPP_COLOR_RGB565 = 0,
	DRV_VPP_COLOR_RGAB5515,
	DRV_VPP_COLOR_ARGB1555,
	DRV_VPP_COLOR_ARGB4444,
	DRV_VPP_COLOR_RGB888,
	DRV_VPP_COLOR_ARGB8888,
	DRV_VPP_COLOR_BGR565,
	DRV_VPP_COLOR_RGB555,
	DRV_VPP_COLOR_BGAR5515,
	DRV_VPP_COLOR_ABGR1555,
	DRV_VPP_COLOR_ABGR4444,
	DRV_VPP_COLOR_BGR888,
	DRV_VPP_COLOR_ABGR8888,
	DRV_VPP_COLOR_1BPP,
	DRV_VPP_COLOR_2BPP,
	DRV_VPP_COLOR_4BPP,
	DRV_VPP_COLOR_8BPP,
	DRV_VPP_COLOR_YCbCr,
	DRV_VPP_COLOR_YUV,
	DRV_VPP_COLOR_YCbYCr,
	DRV_VPP_COLOR_YUYV,
	DRV_VPP_COLOR_4Y4U4Y4V,
	DRV_VPP_COLOR_4Y4Cb4Y4Cr,
	DRV_VPP_COLOR_YCbCr400,
	DRV_VPP_COLOR_YUV400,
	DRV_VPP_COLOR_YCbCr422,
	DRV_VPP_COLOR_YUV422,
	DRV_VPP_COLOR_YCbCr444,
	DRV_VPP_COLOR_YUV444,
	DRV_VPP_COLOR_YCbCr420,       /*!< @brief Ceva decoder output format */
	DRV_VPP_COLOR_YUV420,         /*!< @brief Ceva decoder output format */
	DRV_VPP_COLOR_SEMI400,
	DRV_VPP_COLOR_SEMI420,
	DRV_VPP_COLOR_SEMI422,
	DRV_VPP_COLOR_SEMI444,
	DRV_VPP_COLOR_RGBA8888,
	DRV_VPP_COLOR_BGRA8888,
	DRV_VPP_COLOR_MAX
} DRV_VPP_FrmColorType_e;

typedef enum
{
	DRV_VPP_DAR_FORBIDDEN = 0,
	DRV_VPP_DAR_SQUARE,
	DRV_VPP_DAR_4_3,
	DRV_VPP_DAR_16_9,
	DRV_VPP_DAR_221_1,
	DRV_VPP_DAR_14_9,
	DRV_VPP_DAR_MAX
} DRV_VPP_DispAspectRatio_e;

typedef enum
{
	DRV_VPP_PAR_FORBIDDEN = 0,
	DRV_VPP_PAR_SQUARE,
	DRV_VPP_PAR_EXTENDED,
	DRV_VPP_PAR_H264_FORBIDDEN,
	DRV_VPP_PAR_H264_SQUARE,
	DRV_VPP_PAR_H264_EXTENDED,
	DRV_VPP_PAR_MAX
} DRV_VPP_PixAspectRatio_e;

typedef enum
{
	DRV_VPP_EXP_NONE = 0,
	DRV_VPP_EXP_DAR,
	DRV_VPP_EXP_PAR
} DRV_VPP_AspectExp_e;

 typedef enum
{
	DRV_VPP_RES_480,		/*!< plane resolution is 480 */
	DRV_VPP_RES_576,		/*!< plane resolution is 576 */
	DRV_VPP_RES_720,		/*!< plane resolution is 720 */
	DRV_VPP_RES_1080,		/*!< plane resolution is 1080 */
	DRV_VPP_RES_3840,
	DRV_VPP_RES_4096,
	DRV_VPP_RES_EXT,		/*!< extended resolution */
	DRV_VPP_RES_MAX
} DRV_VPP_Res_e;


typedef enum
{
	DRV_VPP_FORCED_NONE = 0,
	DRV_VPP_FORCED_2D_CODA,
	DRV_VPP_FORCED_2D_NOMOTION,
	DRV_VPP_FORCED_MAX
} DRV_VPP_ForcedDeintMode_e;

typedef enum
{
	PQ_Read,		/*!< PQ tool read */
	PQ_Write		/*!< PQ tool write */
}DRV_DISP_PQ_Tool_e;

typedef enum{
	Fav_Magenta,
	Fav_Red,
	Fav_Yellow,
	Fav_Green,
	Fav_Cyan,
	Fav_Blue,
	Fav_Max,
} DRV_PQ_Favorite_e;

typedef enum{
	ImgProsDefault		= 0x00,
	ImgProsTool		= 0x01,
	ImgProsBin		= 0x02,
	ImgProsSelect	= 0x03,
} PQ_Scaling_t;

 typedef enum
{
	FMT_480P,		/*!< plane format is 480P */
	FMT_576P,		/*!< plane format is 576P */
	FMT_720P,		/*!< plane format is 720P */
	FMT_1080P,		/*!< plane format is 1080P */
	FMT_EXT,		/*!< extended format */
	MAX_FMT_TYPE
}DRV_DISP_Fmt_e;

typedef enum
{
	DRV_VPP_OUT_KEEP_ASPECT = 0,
	DRV_VPP_OUT_RESIZE_2_FULL,
	DRV_VPP_OUT_PANSCAN_2_FULL,
	DRV_VPP_OUT_WINDOW_MODE,
	DRV_VPP_OUT_WINDOW_ASPECTED,
	DRV_VPP_OUT_DIRECT_CENTER,
	DRV_VPP_OUT_MAX
} DRV_VPP_OutputMode_e;


typedef enum
{
	DRV_VPP_OUTPOS_CENTER = 0,
	DRV_VPP_OUTPOS_UP_L,
	DRV_VPP_OUTPOS_UP_C,
	DRV_VPP_OUTPOS_UP_R,
	DRV_VPP_OUTPOS_BT_L,
	DRV_VPP_OUTPOS_BT_C,
	DRV_VPP_OUTPOS_BT_R,
	DRV_VPP_OUTPOS_USER,
	DRV_VPP_OUTPOS_MAX
} DRV_VPP_OutputPos_e;


//*************
//** SUBPATH **
//*************
typedef enum
{
	SUBPATH_HD2SD,
	SUBPATH_AFRC,
	SUBPATH_RTE,
	SUBPATH_TWIC,
	SUBPATH_NUM
} DRV_SubPathModule_e;

typedef enum
{
	DRV_SUBPATH_NORMAL,
	DRV_SUBPATH_PILLAR,
	DRV_SUBPATH_LETTER,
	DRV_SUBPATH_MODE_MAX
} DRV_SUBPATH_BoxMode_e;

typedef enum
{
	TCIW_NV12 = 0,
	TCIW_YUY2,
	TCIW_RGBA8888,
	TCIW_ARGB8888,
	TCIW_RGB565
} DRV_TCIW_DATA_MODE_e;

typedef enum
{
	DRV_DISP_SINGLE = 0,		// Main display only
	DRV_DISP_MIRROR,			// Using Dual mode to do Mirror
	DRV_DISP_DUAL,				// Dual mode
	DRV_DISP_MIRROR_ORG,		// Mirror mode using HD2SD
	DRV_DISP_NOT_INIT,
} DRV_DISP_SYS_MODE_e;

typedef enum
{
	DRV_ASPECT_RATIO_FULL = 0,
	DRV_ASPECT_RATIO_16_9,
	DRV_ASPECT_RATIO_4_3,
} DRV_DISP_SYS_ASPECT_RATIO_e;

typedef enum
{
	DRV_DISP_UNDERSCAN = 0,
	DRV_DISP_OVERSCAN,
	DRV_DISP_NOSCAN
} DRV_DISP_ScanUsage_e;

typedef enum
{
	DRV_DISP_TARGET_GRAPHIC = 0,
	DRV_DISP_TARGET_VIDEO,
	DRV_DISP_TARGET_BOTH
} DRV_DISP_ScanTarget_e;

typedef enum
{
	IPC_CMD_DISP_INIT = 2,
	IPC_CMD_DISP_GET_FB_INFO = 3,
	IPC_CMD_DISP_QT_DEBUG = 4,
	IPC_CMD_DISP_SWAP_BUFF = 6,
	IPC_CMD_DISP_PQ_PARSE = 7,
	IPC_CMD_DISP_MID_PARSE = 9,
	IPC_CMD_DISP_WAIT_VSYNC = 18,
	IPC_CMD_DISP_QUERY_VSYNC = 19,
} DISP_IPC_CommandID_e;

#define DRV_AFRC_RTE_VFLIP 			1
#define DRV_AFRC_RTE_HFLIP 			(1<< 1)
#define DRV_AFRC_RTE_FROM_HD2SD 	(1<< 2)
#define DRV_AFRC_RTE_INTERLACE		(1 <<3)

// System setting constants
#define MAIN_UILayer			DRV_OSD0
#define MAIN_SubTxtLayer	DRV_OSD1
#define SECOND_UILayer		DRV_OSD1
#define SECOND_SubTxtLayer	DRV_OSD1

#define MAIN_DISP		0
#define SECOND_DISP		1

#define PRIMARY_DISP_ID 0
#define SECOND_DISP_ID 1

/**************************************************************************
 *                              M A C R O S                               *
 **************************************************************************/
#define	SWAP16(x)	((((UINT16)(x))&0x00ff)<<8 | (((UINT16)(x))>>8))
#define	SWAP32(x)	((((UINT32)(x))&0x000000ff)<<24 | (((UINT32)(x))&0x0000ff00)<<8 | (((UINT32)(x))&0x00ff0000)>>8 | (((UINT32)(x))&0xff000000)>>24)

#define ALIGN2(x)	((x+1)&0xFFFFFFFE)
#define ALIGN8(x)	((x+7)&0xFFFFFFF8)
#define ALIGN16(x)	((x+15)&0xFFFFFFF0)
#define ALIGN32(x)  ((x+31)&0xFFFFFFE0)
#define ALIGN64(x)  ((x+63)&0xFFFFFFC0)
#define ALIGN128(x)  ((x+127)&0xFFFFFF80)

//Component main path: select main path and dac output type dac output type is YUV
#define DAC_MAIN_PATH(ComponentOutPath, DacOutType)	((ComponentOutPath==DRV_VENC_MAIN_PATH_SEL)&&(DacOutType==DRV_VENC_DAC_YUV))
#define DAC_SUB_PATH(ComponentOutPath, DacOutType)	(!DAC_MAIN_PATH(ComponentOutPath, DacOutType))

#define VENC_IS_FMT_SD(fmt)			((fmt == DRV_VENC_FMT_480P60) || (fmt == DRV_VENC_FMT_480I60)|| (fmt == DRV_VENC_FMT_576P50) ||(fmt == DRV_VENC_FMT_576I50))
#define VENC_IS_FMT_HD(fmt)		(!VENC_IS_FMT_SD(fmt))
#define VENC_FMT_INTERLACE(fmt)		(fmt == DRV_VENC_FRM_TYPE_INTERLACE)
#define VENC_FMT_PROGRESSIVE(fmt)	(!VENC_FMT_INTERLACE(fmt))
#define VENC_IS_FMT_480P(fmt)		((fmt == DRV_VENC_FMT_480P60)||(fmt == DRV_VENC_FMT_480I60))
#define VENC_IS_FMT_576P(fmt)		((fmt == DRV_VENC_FMT_576P50)||(fmt == DRV_VENC_FMT_576I50))
#define VENC_IS_FMT_720P(fmt)		((fmt == DRV_VENC_FMT_720P60)||(fmt == DRV_VENC_FMT_720P50)||(fmt == DRV_VENC_FMT_720P24))
#define VENC_IS_FMT_1080P(fmt)		((fmt == DRV_VENC_FMT_1080P60)||(fmt == DRV_VENC_FMT_1080P50)||(fmt == DRV_VENC_FMT_1080P24)||(fmt == DRV_VENC_FMT_1080I60)||(fmt == DRV_VENC_FMT_1080I50))
#define VENC_SUPPROT_CGMS(fmt)		((fmt == DRV_VENC_FMT_1080I60)||(fmt == DRV_VENC_FMT_720P60)||(fmt == DRV_VENC_FMT_480P60))

#define VENC_FRM_RATE_24(rate)		(rate == DRV_VENC_FRM_RATE_24)
#define VENC_FRM_RATE_50(rate)		(rate == DRV_VENC_FRM_RATE_50)
#define VENC_FRM_RATE_60(rate)		(rate == DRV_VENC_FRM_RATE_60)
#define VENC_SUB_FMT_NTSC(fmt)		((fmt==SUB_FMT_NTSC)||(fmt==SUB_FMT_PAL_M))
#define VENC_SUB_FMT_PAL(fmt)		((fmt==SUB_FMT_PAL)||(fmt==SUB_FMT_PAL_NC))

#define IS_DRAM_B(PA) (PA & 0xe0000000)

#define memcpy1(dst, src, size) \
{ \
	UINT32 idx; \
	UINT32* ptr_dst = (UINT32*)(dst); \
	UINT32* ptr_src = (UINT32*)(src); \
	for(idx=0;idx<(size)/4;idx++) \
		ptr_dst[idx] = ptr_src[idx]; \
}
//Barney template to apply different version of pq.bin
#define PQ_NEW 5
#define HwPQRecordPoint_TEMPLATE(packgePtr)  \
{\
		int NotAdjustDisp;\
		int id;\
		packgePtr = (DRV_VPP_PQ_Package_Client*)pDispWorkMem->PQPackage[ScenSelect];\
		\
		if(packgePtr->DmixAdj.OutId==DRV_Disp_Out0)\
			{id = gMAIN_DISP;} \
			else {id= gSECOND_DISP;}\
		\
		NotAdjustDisp = 1-id;\
\
		pDispWorkMem->LumaAdj[id].contrast = packgePtr->DmixAdj.contrast;\
		pDispWorkMem->LumaAdj[id].brightness= packgePtr->DmixAdj.brightness;\
		pDispWorkMem->hue = packgePtr->DmixAdj.hue;\
		pDispWorkMem->saturation = packgePtr->DmixAdj.saturation;\
		pDispWorkMem->satsin = packgePtr->DmixAdj.satsin;\
		pDispWorkMem->satcos = packgePtr->DmixAdj.satcos;\
		\
		INFO("Contrast==%d==\n", pDispWorkMem->LumaAdj[id].contrast);\
		INFO("Brightness==%d==\n", pDispWorkMem->LumaAdj[id].brightness);\
		INFO("hue==%d==\n", pDispWorkMem->hue);\
		INFO("Saturation==%d==\n", pDispWorkMem->saturation);\
		\
		\
		pDispWorkMem->LumaAdj[NotAdjustDisp].contrast= 12;\
		pDispWorkMem->LumaAdj[NotAdjustDisp].brightness= 12;\
}

#define DRV_VPP_Fav_Apply_Bin_TEMPLATE(packagePtr) \
{ \
	DRV_VPP_PQ_Favoriate_Bin_Set(DRV_VPP_MODULE_0, &packagePtr->FavInfoVideo);\
	DRV_VPP_PQ_Favoriate_Bin_Set(DRV_VPP_MODULE_1, &packagePtr->FavInfoUI);\
	DRV_VPP_PQ_Favoriate_Enable(DRV_VPP_MODULE_0, packagePtr->FavHeaderVideo.Enable);\
}

#define DRV_VPP_Load_PQ_Bin_TEMPLATE(packagePtr) \
{\
	{\
		if(packagePtr->PeakingHeader_Video.Enable)\
			enable_info.enable = 1;\
			else\
				enable_info.enable = 0;\
		DRV_VPP_Set_PeakingEnable_PQ(&enable_info);\
	}\
	\
	{\
		if(packagePtr->TransientHeader_Video.Enable)\
			enable_info.enable = 1;\
			else enable_info.enable = 0;\
		DRV_VPP_Set_DLTIEnable_PQ(&enable_info);\
	}\
	\
	DRV_VPP_Set_Peaking(&(packagePtr->Peaking_Video));\
	DRV_VPP_Set_Transient(&(packagePtr->Transient_Video));\
	DRV_VPP_Set_DeOvershoot(&(packagePtr->DeOverShoot_Video));\
}

#define PQ_SCENE_SELECT(packagePtr) {\
	if(packagePtr->DmixAdj.OutId==DRV_Disp_Out0)\
		{id = gMAIN_DISP;} \
		else {id= gSECOND_DISP; }\
\
	pDispWorkMem->PQ_Flag[DRV_VPP_MODULE_0] = ImgProsSelect;\
	pDispWorkMem->ScenSelect = select;\
	\
	LumaAdjInfo.enable = 1;\
	LumaAdjInfo.OutId = id;\
	LumaAdjInfo.brightness = packagePtr->DmixAdj.brightness;\
	LumaAdjInfo.contrast = packagePtr->DmixAdj.contrast;\
\
	ChromaAdjInfo.enable = 1;\
	ChromaAdjInfo.OutId = id;\
	ChromaAdjInfo.satsin = packagePtr->DmixAdj.satsin;\
	ChromaAdjInfo.satcos = packagePtr->DmixAdj.satcos;\
\
	pDispWorkMem->LumaAdj[id].contrast = packagePtr->DmixAdj.contrast;\
	pDispWorkMem->LumaAdj[id].brightness = packagePtr->DmixAdj.brightness;\
	pDispWorkMem->hue = packagePtr->DmixAdj.hue;\
	pDispWorkMem->saturation = packagePtr->DmixAdj.saturation;\
\
	DEBUG("ScenSet contrast==%d==\n", pDispWorkMem->LumaAdj[id].contrast);\
	DEBUG("ScenSet brightness==%d==\n", pDispWorkMem->LumaAdj[id].brightness);\
	DEBUG("ScenSet saturation==%d==\n", pDispWorkMem->saturation);\
\
	DEBUG("ScenSet satsin==%d==\n", ChromaAdjInfo.satsin);\
	DEBUG("ScenSet satcos==%d==\n", ChromaAdjInfo.satcos);\
	\
	DRV_DMIX_Luma_Adjust_Set(&LumaAdjInfo);\
	DRV_DMIX_Chroma_Adjust_Set(&ChromaAdjInfo);\
\
	DRV_VPP_PQ_Favoriate_Bin_Select(DRV_VPP_MODULE_0, &packagePtr->FavInfoVideo, packagePtr->FavHeaderVideo.Enable);\
	DRV_VPP_PQ_Favoriate_Bin_Select(DRV_VPP_MODULE_1, &packagePtr->FavInfoUI, packagePtr->FavHeaderUI.Enable);\
}
//End Barney

/**************************************************************************
 *                          D A T A    T Y P E S                          *
 **************************************************************************/
typedef struct _DRV_Sys_Mode_Info_t
{
	DRV_DISP_SYS_MODE_e mode;
	DRV_DispOutID_e DMIX_Idx[2];
} DRV_Sys_Mode_Info_t;

typedef struct _DRV_Sys_Aspect_Rario_t
{
	DRV_DispOutID_e 	DispID;
	DRV_DISP_SYS_ASPECT_RATIO_e Ratio;
} DRV_Sys_Aspect_Rario_t;

typedef struct _DRV_Sys_OutMode_Info_t
{
	DRV_DispOutID_e 	id;
	display_size_t		panelRes;
	DRV_FrameRate_e	fps;
	DRV_VENC_FrmType_e p_i_type;
} DRV_Sys_OutMode_Info_t;

typedef struct _DRV_OsdInit_t
{
	DRV_OsdWindow_e win_id;
	UINT16 win_width;
	UINT16 win_height;
} DRV_OsdInit_t;

typedef UINT32 DRV_OsdRegionHandle_t;

typedef struct _DRV_Region_Info_t
{
	UINT32 bufW;
	UINT32 bufH;
	UINT32 startX;
	UINT32 startY;
	UINT32 actW;
	UINT32 actH;
} DRV_Region_Info_t;

typedef struct _DRV_Region_Create_t
{
	DRV_OsdRegionFormat_e format;
	UINT32 NumBuffers;
	DRV_OsdAlignSel_e align;

	void* pBufferAddr;
	UINT32 BufferSize;
	DRV_Region_Info_t region_info;
} DRV_Region_Create_t;

typedef struct _DRV_SetColorBar_t
{
	DRV_OsdWindow_e win_id;
	UINT32 enable;
	UINT32 mode;
} DRV_SetColorBar_t;

typedef struct _DRV_GetMainRegion_t
{
	DRV_OsdWindow_e win_id;
	DRV_OsdRegionHandle_t pRetRegion;
} DRV_GetMainRegion_t;

typedef struct _DRV_OSD_SetCult_t
{
	DRV_OsdRegionHandle_t region;
	UINT32 ClutDataPtr[256];
} DRV_OSD_SetCult_t;

typedef struct _DRV_Region_Update_t
{
	UINT8 bEnable;
	UINT8 BufferId;
	DRV_Region_Info_t region_info;
} DRV_Region_Update_t;

typedef struct _DRV_HTDS_Pixel_Info_t
{
	UINT16 pix_start;
	UINT16 pix_end;
	UINT16 shift_magn;
	UINT16 shift_dir;
	UINT32 line_width;
} DRV_HTDS_Info_t;

typedef struct _DRV_CreateRegion_t
{
	DRV_OsdWindow_e win_id;
	DRV_Region_Create_t pRegionCreate;
	DRV_OsdRegionHandle_t pRetRegion;
} DRV_CreateRegion_t;

typedef struct _DRV_OsdRegion_Origin_t
{
	DRV_OsdRegionHandle_t region;
	UINT16 X;
	UINT16 Y;
} DRV_OsdRegion_Origin_t;

typedef struct _DRV_SetVidibleBuffer_t
{
	DRV_OsdRegionHandle_t region;
	UINT8 bBufferId;
	UINT32 UpdatePA;
} DRV_SetVisibleBuffer_t;

typedef struct _DRV_SetTransparency_t
{
	DRV_OsdRegionHandle_t region;
	DRV_OsdTransparencyMode_e mode;
} DRV_SetTransparency_t;

typedef struct _DRV_SetBlendLevel_t
{
	DRV_OsdRegionHandle_t region;
	UINT8 bEnable;
	DRV_OsdBlendMethod_e bMethod;
	UINT8 bLevel;
} DRV_SetBlendLevel_t;

typedef struct _DRV_SetByteSwap_t
{
	DRV_OsdRegionHandle_t region;
	UINT8 bEnable;
} DRV_SetByteSwap_t;

typedef struct _DRV_SetColorSpace_t
{
	DRV_OsdWindow_e win_id;
	DRV_OsdColorSpace_e bSrcColor;
} DRV_SetColorSpace_t;

typedef struct _DRV_SetPlaneEnable_t
{
	DRV_OsdWindow_e win_id;
	UINT8 bEnable;
} DRV_SetPlaneEnable_t;

typedef struct _DRV_SetOSDUpdate_t
{
	DRV_OsdWindow_e win_id;
	DRV_Region_Update_t Update;
	DRV_OsdRegionHandle_t pRegion;
} DRV_SetOSDUpdate_t;

typedef struct _DRV_OSD_Test_t
{
	DRV_OsdWindow_e win_id;
	DRV_OsdRegionFormat_e ColorFmt;
} DRV_OSD_Test_t;

typedef struct _DRV_SetGSCL_t
{
	DRV_OsdWindow_e win_id;
	DRV_Region_Info_t InRegionInfo;
	DRV_Region_Info_t OutRegionInfo;
} DRV_SetGSCL_t;

typedef struct _DRV_SetTGEN_t
{
	DRV_DispOutID_e OutId;
	DRV_VideoFormat_e fmt;
	DRV_FrameRate_e fps;
	UINT16 IsProgress;
	UINT16 IsUserMode;
} DRV_SetTGEN_t;

typedef struct _DRV_GetTGEN_t
{
	DRV_VideoFormat_e fmt[2];
	DRV_FrameRate_e fps[2];
} DRV_GetTGEN_t;

typedef struct _DRV_Adj_TGEN_t
{
	DRV_DispOutID_e OutId;
	UINT32 AdjValue;
} DRV_Adj_TGEN;

//=======
//= DMIX   =
//=======

typedef struct _DRV_DMIX_PlaneAlpha_t
{
	DRV_DispOutID_e OutId;
	DRV_DMIX_LayerId_e Layer;
	UINT32 EnPlaneAlpha;
	UINT32 EnFixAlpha;
	UINT32 AlphaValue;
} DRV_DMIX_PlaneAlpha_t;

typedef struct _DRV_DMIX_Luma_Adj_t
{
	DRV_DispOutID_e OutId;
	UINT32 enable;
	UINT32 brightness;
	UINT32 contrast;
	//-------------
	UINT16 CP1_Dst;
	UINT16 CP1_Src;
	UINT16 CP2_Dst;
	UINT16 CP2_Src;
	UINT16 CP3_Dst;
	UINT16 CP3_Src;
	UINT16 Slope0;
	UINT16 Slope1;
	UINT16 Slope2;
	UINT16 Slope3;
} DRV_DMIX_Luma_Adj_t;

typedef struct _DRV_DMIX_PQ_Adj_t
{
	#if 0
	UINT8 CP1_Dst;
	UINT8 CP1_Src;
	UINT8 CP2_Dst;
	UINT8 CP2_Src;
	UINT8 CP3_Dst;
	UINT8 CP3_Src;
	UINT16 Slope0;
	UINT16 Slope1;
	UINT16 Slope2;
	UINT16 Slope3;
	#endif

	UINT32 hue;
	//For chroma
	UINT16 satsin;
	UINT16 satcos;

	UINT8 OutId;

	//For brightness and contrast
	UINT8 contrast;
	UINT8 brightness;
	UINT8 saturation;
} DRV_DMIX_PQ_Adj_t;

typedef struct _DRV_DMIX_Chroma_Adj_t
{
	DRV_DispOutID_e OutId;
	UINT32 enable;
	UINT16 satcos;
	UINT16 satsin;
} DRV_DMIX_Chroma_Adj_t;

typedef struct _DRV_DMIX_Layer_Set_t_
{
	DRV_DispOutID_e id;
	DRV_DMIX_LayerId_e Layer;
	DRV_DMIX_LayerMode_e LayerMode;
	DRV_DMIX_InputSel_e FG_Sel;
} DRV_DMIX_Layer_Set_t;

typedef struct _DRV_DMIX_Src_Sel_t_
{
	DRV_DispOutID_e id;
	DRV_DMIX_InputSel_e FG_Sel;
} DRV_DMIX_Src_Sel_t;

typedef struct _DRV_DMIX_Src_Sel_Get_t_
{
	DRV_DMIX_InputSel_e FG_Sel[5];
} DRV_DMIX_Src_Sel_Get_t;

typedef struct _DRV_DMIX_BG_Sel_t_
{
	DRV_DispOutID_e id;
	DRV_DMIX_InputSel_e BG_Sel;
} DRV_DMIX_BG_Sel_t;

typedef struct _DRV_DMIX_BG_Color_Set_t
{
	DRV_DispOutID_e id;
	UINT32 color;
} DRV_DMIX_BG_Color_Set_t;

typedef struct _DRV_DMIX_General_Set_t
{
	DRV_DispOutID_e OutId;
	UINT8 L1_FG_Sel;		// 0: Layer 1 is FGC; 1: Layer 1 zero (default)
	UINT8 L1_BG_Sel;	// 0: Layer 0 is YC adj output(defatult); 1: Layer 0 is zero
	UINT8 FGC_mode;	// 0: normal blend; 1: transparent(default); 2: opacity
	UINT8 ByPass_DSCL;	// 0: DSCL; 1: AMIX (default)
} DRV_DMIX_General_Set_t;

typedef struct _DRV_DMIX_HD2SD_Set_t
{
	DRV_DispOutID_e OutId;
	UINT8 HD2SD_YC_Sel;// 0: HD2SD output with YC(default); 1: HD2SD output without YC
	UINT8 HD2SD_FGC_Sel;// 0: with FGC; 1: without FGC
} DRV_DMIX_HD2SD_Set_t;


//=======
//= VENC   =
//=======

typedef struct
{
	//TVE & DVE output format & frame rate
	DRV_VideoFormat_e		bFmt;		/*!< Output format */
	DRV_FrameRate_e		bFps;	/*!< Output frame rate */
} DRV_VENC_FmtAndFps_t;

/*typedef struct
{
	DRV_VENC_HDMIfmt_e bFmt;
	DRV_VENC_HDMItype_e bType;
} DRV_VENC_SetHDMIFmtParam_t;
*/

typedef struct
{
	//HD WSS + SD WSS
	UINT8	bWssEn;			/*!< Enable WSS */
	UINT8	bHdWssEn;		/*!< Enable HD WSS */
	UINT8	bAspectRatio;		/*!< WSS aspect ratio */
	UINT8	bCps;			/*! <Copy right protection signal */
	UINT8	bMvType;			/*! <Macrovision type for Ntsc */
	//CGMS-B
	UINT8	bCgmsbEn;		/*!< Enable CGMS type B */
	UINT8	bRcdEnable;		/*! < Redistribution control description of CGMS-B */
	UINT8	bAfdValue;		/*! < AFD value of CGMS-B */
	UINT8	bColorimetry;		/*! < Colorimetry of CGMS-B */
	UINT8	bScanMode;		/*! < Scan mode of CGMS-B */
	UINT8	bConstrainImgEnable;/*!< Enable Constrain image token, turn on after 2011 */
}DRV_CGMS_InputParam_t;

typedef struct
{
	//TVE & DVE P/I select
	DRV_VENC_FrmType_e	bComponentFrmTypeSel;	/*!< Component output P/I */
	DRV_VENC_FrmType_e	bHdmiFrmTypeSel;		/*!< Hdmi output P/I */
	DRV_VENC_FrmType_e	bD656FrmTypeSel;		/*!< D656 output P/I */
}DRV_VENC_FrmTypeSel_t;

typedef struct
{
	//TVE & DVE main/sub path output select
	DRV_VENC_OutPath_e	bComponentOutPathSel;	/*!< Component output path */
	DRV_VENC_OutPath_e	bHdmiOutPathSel;		/*!< Hdmi output path */
	DRV_VENC_OutPath_e	bD656OutPathSel;		/*!< D656 output path */
}DRV_VENC_OutPathSel_t;


typedef struct
{
	//TVE & DVE output format & frame rate
	DRV_VENC_Fmt_e		bFormat;		/*!< Output format */
	DRV_VENC_FrmRate_e	bFrmRate;	/*!< Output frame rate */
	DRV_VENC_FrmType_e	bFrmType;
	DRV_VENC_GeneralFmt_e bGeneralFormat;
}DRV_VENC_Fmt_t;

typedef struct
{
	UINT8	bBlackMaskEnable[2];	/*!< Enable black mask, HD as [0], SD as [1] */
}DRV_VENC_MaskParam_t;

typedef struct
{
	UINT8	bSdTvNtscFmt;	/*!< CVBS output Ntsc format: Ntsc, Ntsc_J, Pal_M, Pal_60 */
	UINT8	bSdTvPalFmt;	/*!< CVBS output Pal format: Pal, Pal_N, Pal_Nc */
} DRV_VENC_SdtveFmt_t;


/**
 *	\brief  Output setting param
 */
typedef struct
{
	UINT8	bClipMode;		/*!< VPP clip mode*/
	UINT8	bHdmiInterface;	/*!< Hdmi interface 444/422 selection */
	UINT8	bHSyncChannel;	/*!< H_SYNC on UV/ RGB channels */
	UINT8	bSyncNoG;		/*!< G sync on/off */
	UINT8	bDacOutPut;		/*!< Dac output video type, YUV, RGB,S-Video */
}DRV_VENC_OutputParam_t;

/**
 *	\brief  Filter setting param
 */
typedef struct
{
	UINT8	bChromaFilter;		/*!< D656 chroma filter by decimate/lowpass selection */
	UINT8	bVppCompensate;	/*!< VPP curve compensate*/
	UINT8	bBleEn;			/*!< Enable blank level for Ntsc*/
	UINT8	bBtbEnable;		/*!< Black then black enable */
	UINT8	bWtwEnable;		/*!< White then white enable */
}DRV_VENC_FilterParam_t;

/**
 *	\brief  Dac up-sample clock param
 */
typedef struct
{
	UINT8	bClk54DacEn[4];	/*!< Enable 54MHz up-sample filter for DAC_ABCD, [A~D] as [0~3] */
}DRV_VENC_UPS_Param_t;

//Barney added structure 8602 platform_venc.h
typedef struct
{
	UINT16	wDacCurrent;		/*!< Dac_ABCD current mode */
	UINT8	bDacEnable;		/*!< Enable Dac_ABCD current module*/
	UINT8	bD656Enable;		/*!< Enable CCIR656 current module*/

	//Auto Video Power Down paramters
	UINT8	bAvpdEnable;		/*!< Enable Auto Video Power Down module*/
	UINT8	bAvpdTurnOffDacEn;	/*!< Turn off VDAC enable when plug out */
	UINT8	bAvpdDacMuxEn;	/*!< Dac_ABCD mux for AVPD enable */
	UINT8	bAvpdTimePeriod;	/*!< Timeout period for each Dac detection */
}DRV_Platform_VENC_Param_t;
//End Barney added structure


//=======
//=   CPS   =
//=======
typedef struct
{
	DRV_CPS_InfoType_e cps_type;
	UINT8 cps_val;
}DRV_CPS_Param_t;


//======
//= VPP =
//======
typedef struct
{
	UINT8 color_y;
	UINT8 color_cb;
	UINT8 color_cr;
} DRV_VPP_YCbCrColor_t;

typedef struct
{
	UINT8 bInited;		// 0 : not inited,   1 : have been inited
	UINT8 bMcpOn;		// 0 : MCP is not running,    1:MCP is running
	UINT8 bRcsOn;		// 0: RCS is not running ,   1: RCS is running
	UINT8 a_vppIsrEn;		// 0 : disable ISR update,    1:enable ISR update
	UINT8 a_isDiableVpp;
	UINT8 bFieldIndex;	// counter for field index
	UINT8 bDmixShown;	// 0 : dmix vpp layer bypass,   1 : dmix vpp layer is shown
	UINT8 bHwEnable;	// 0 : vpp hw is disabled, (safe mode on),     1:vpp hw is enabled (safe mode off)
	UINT8 bDmixSel;	// 0 : dmix 0 and tgen 0,    1 : dmix1 and tgen1
//	UINT8 bIoctlSetOutputMode;
	UINT8 bVdecimOn;
	UINT8 bVdownsizeOn;
	UINT8 bLumaDramSel;
	UINT8 bCrmaDramSel;
//	UINT8 bDeintTopFirst;
//	UINT8 bDeintInterlaced;
	UINT8 bICTOn;
	UINT8 bRcsDisableVpp;
	UINT8 bMcpState;
	UINT8 bForcedDeintMode;

} DRV_VPP_State_t;

typedef struct
{
	UINT8 mcpSusToRCP;
	UINT8 mcpState;
} DR_VPP_Switch_t;

typedef struct
{
	DRV_VPP_SwitchOnOff_e	out_win_valid;
	display_rect_t			out_win_rect;
	DRV_VPP_OutputMode_e	out_mode;
} DRV_VPP_OutputWin_t;


typedef struct
{
	// all are [21:0] and aligned to KB
	//unsigned int mr_addr;
	unsigned int luma_addr0;
	unsigned int luma_addr1;
	unsigned int luma_addr2;
	unsigned int crma_addr0;
	unsigned int crma_addr1;
	unsigned int crma_addr2;
} DRV_VPP_BaseAddr1D_t;

typedef struct
{
	DRV_VPP_ChromaUpsample_e cur;
	DRV_VPP_ChromaUpsample_e nxt;
	DRV_VPP_ChromaUpsample_e pre;
} DRV_VPP_ChromaUpsample_t;

typedef struct
{
	DRV_VPP_SwitchOnOff_e h_dcm_en;
	DRV_VPP_SwitchOnOff_e v_dcm_en;
	DRV_VPP_DecimationMode_e h_dcm_mode;
/*	DRV_VPP_SwitchOnOff_e cur_h_dcm_en;
	DRV_VPP_SwitchOnOff_e nxt_h_dcm_en;
	DRV_VPP_SwitchOnOff_e pre_h_dcm_en;
	DRV_VPP_SwitchOnOff_e cur_v_dcm_en;
	DRV_VPP_SwitchOnOff_e nxt_v_dcm_en;
	DRV_VPP_SwitchOnOff_e pre_v_dcm_en;
	DRV_VPP_DecimationMode_e cur_h_dcm_mode;
	DRV_VPP_DecimationMode_e nxt_h_dcm_mode;
	DRV_VPP_DecimationMode_e pre_h_dcm_mode;*/
} DRV_VPP_DecimationMode_t;

typedef struct
{
	DRV_VPP_TopBottomSel_e cur_fid;
	DRV_VPP_TopBottomSel_e nxt_fid;
	DRV_VPP_TopBottomSel_e pre_fid;
	DRV_VPP_FieldFrameSel_e cur_f_sel;
	DRV_VPP_FieldFrameSel_e nxt_f_sel;
	DRV_VPP_FieldFrameSel_e pre_f_sel;
	DRV_VPP_FrmAddrId_e cur_addr_id;
	DRV_VPP_FrmAddrId_e nxt_addr_id;
	DRV_VPP_FrmAddrId_e pre_addr_id;
} DRV_VPP_DataFetchInfo_t;

typedef struct
{
	UINT16 plane_w;
	UINT16 plane_h;
	UINT16 cropping_x;
	UINT16 cropping_y;
	UINT16 cropping_w;
	UINT16 cropping_h;
	//unsigned int out_screen_width;
	//unsigned int out_screen_height;
	//unsigned int out_active_x;
	//unsigned int out_active_y;
	//unsigned int out_active_w;
	//unsigned int out_active_h;
} DRV_VPP_Plane_t;

typedef struct
{
	DRV_VPP_PCNAddrId_e id_seq;
	DRV_VPP_PCNTopBot_e top_bot_seq;
	DRV_VPP_DeintDirection_e deint_dir;
} DRV_VPP_DeintInfo_t;


/*typedef struct
{
	DRV_VPP_DeintModeSel_e deint_mode;
	DRV_VPP_DeintDirection_e deint_direction;
} DRV_VPP_DeintMode_t;*/

typedef struct
{
	DRV_VPP_SwitchOnOff_e dering_en;
	UINT8 clamp_val;
	UINT8 edge_detect_weight;
	DRV_VPP_DeringDetectMode_e ed_mode;
	DRV_VPP_DeringFilterMode_e filter_mode;
	DRV_VPP_DeringSmooth_e smooth_mode;
	//oterh parameters are ignored right now
} DRV_VPP_DeringParam_t;

typedef struct
{
	DRV_VPP_SwitchOnOff_e vimg_en;
	DRV_VPP_SwitchOnOff_e diag_luma_EALP_filter_en;
	DRV_VPP_SwitchOnOff_e vert_luma_HPC_filter_sel;
	DRV_VPP_SwitchOnOff_e vert_luma_PEAK_en;
	// lots of vimg settings are ignored right now
} DRV_VPP_VimgParam_t;

typedef struct
{
	UINT32 src_adj;
	UINT32 dst_adj;
	UINT32 adj_slope0;
	UINT32 adj_slope1;
	UINT32 adj_slope2;
} DRV_VPP_ConAdjParam_t;

/*typedef struct
{
	DRV_VPP_ModuleID_e id;
	DRV_VPP_VSCLPlane_t plane;
} DRV_VPP_SetScalerInfoParam_t;*/

typedef struct
{
	UINT8	top_bottom	: 1;		// top first or bottom first
	UINT8	prog_inter	: 1;		// progressive source or interlaced source
	UINT8	field_frame	: 1;		// field source or frame source
	UINT8	repeat_first: 1;		// repeat first field or not
	UINT8	key_frame	: 1;		// this frame is a key frame or not
	UINT8	single_field: 1;		// single field diplaying or not
	UINT8	first_pic	: 1;		// first picture of this sequence or not
	UINT8				: 1;
} DRV_VPP_FrameSrcInfo_t;

typedef struct
{
	UINT8	range_map_y		: 3;//0~7
	UINT8	range_map_y_flag	: 1;//0, 1
	UINT8	range_map_uv		: 3;//0~7
	UINT8	range_map_uv_flag	: 1;//0, 1
} DRV_VPP_RangeMap_t;

typedef struct
{
	union
	{
		//DRV_VPP_RangeMap_t pre_range_map;
		//DRV_VPP_RangeMap_t cur_range_map;
		//DRV_VPP_RangeMap_t nxt_range_map;
		struct{
			UINT32	pre_rm_y	: 3;
			UINT32	nxt_rm_y	: 3;
			UINT32	cur_rm_y	: 3;
			UINT32	pre_rm_uv	: 3;
			UINT32	nxt_rm_uv	: 3;
			UINT32	cur_rm_uv	: 3;
			UINT32	pre_rm_y_en	: 1;
			UINT32	nxt_rm_y_en	: 1;
			UINT32	cur_rm_y_en	: 1;
			UINT32	not_used		: 6;
			UINT32	pre_rm_uv_en	: 1;
			UINT32	nxt_rm_uv_en	: 1;
			UINT32	cur_rm_uv_en	: 1;
			UINT32	paddings		: 2;
		}bit;
		UINT32 byte;
	}u;
} DRV_VPP_SetRangeMap_t;



//============
//= Video SYNC =
//============
typedef struct
{
	UINT32	stc_id;
	INT32	clock_state; // 0: run, 1: stop
	UINT32	frame_buf_count;

	long long cur_disp_time;
	long long disp_stc_diff_time;
	INT32 pre_buf_idx;
	INT32 cur_buf_idx;

	INT32 set_pause_time_reted_by_vpp;
	long long pause_time_reted_by_vpp;
	long long start_base;

} SYNC_Info_t;

#define MAX_PANSCAN_NUM	4

typedef struct _DRV_VPP_Luma_Adj_t
{
	UINT8 enable;
	UINT8 brightness;
	UINT8 contrast;
	UINT8 id;
	//-------------
#if 0
	UINT16 CP1_Dst;
	UINT16 CP1_Src;
	UINT16 CP2_Dst;
	UINT16 CP2_Src;
	UINT16 CP3_Dst;
	UINT16 CP3_Src;
	UINT16 Slope0;
	UINT16 Slope1;
	UINT16 Slope2;
	UINT16 Slope3;
#endif
} DRV_VPP_Luma_Adj_t;

typedef struct _DRV_VPP_AP_Luma_Adj_t
{
	UINT8 LumaStart;
	UINT8 brightness;
	UINT8 contrast;
	UINT8 id;
} DRV_VPP_AP_Luma_Adj_t;

/* @brief frame buffer variable for VPP driver */
typedef struct
{
	/* frame layer info. */
	UINT32 pframe_anchor_y;			// value of physical address of CURRENT luma frame buffer
	UINT32 pframe_anchor_u;			// value of physical address of CURRENT chroma frame buffer
	UINT32 pframe_anchor_v;			// the value equals to "pframe_anchor_u"
	display_color_t key_color;			// key color of this frame : red, green, blue, alpha
	UINT32 key_color_idx;				// index of key color (if any)
	display_color_t *pcolor_palette;		// pointing to color palette (if any)
	display_rect_t valid_rect;			// valid rectangle : x, y, width, height  (the real data region)
	UINT32 frame_dur;					// duration of time that this frame should last
	UINT8 pre_frame_valid;			// indicate that the previous frame buffer is valid or not for VPP
	UINT8 next_frame_valid;			// indicate that the addresses provided for next frame is valid
	UINT32 pnext_anchor_y;			// value of physical address of NEXT luma frame buffer
	UINT32 pnext_anchor_u;			// value of physical address of NEXT chroma frame buffer
	UINT32 pnext_anchor_v;			// the value equals to "pnext_anchor_u"
	UINT8 sequence_update;			// 0:sequence layer no change, 1:sequence layer variables need to be updated
	INT64 slPts;                            // ms
	INT32 frame_buffer_idx;
	UINT32 video_sync_in_driver;
//	UINT8 bDisableSync;

	/* sequence layer info. */
	DRV_VPP_FrameSrcInfo_t src_info;
	UINT32 dFormatTag;				// codec type, enum value: STD_AVC, STD_VC1..
	UINT16 frame_width;				// width of panel (output width)
	UINT16 frame_height;			// height of panel (output height)
	UINT16 frame_bpl;				// bytes per line of this frame
	UINT8 frame_color_type;			// SP_BITMAP_YUV422, SP_BITMAP_RGB565, SP_BITMAP_8BPP, SP_BITMAP_1BPP
	UINT16 stride_uv;				// stide of uv data
	UINT16 bitmap_width;			// width of input picture (source width)
	UINT16 bitmap_height;			// height of input picture (source height)
	UINT32 pic_aspect;				// aspect ratio
	UINT32 vdo_aspect_ratio;
	UINT32 frame_rate;				// frame rate
	DRV_VPP_RangeMap_t vc1_range_map;// VC1 range map info. : rmap_y, rmap_y_flag, rmap_uv, rmap_uv_flag
	DRV_VPP_ColorSpace_e frame_cs;	// color space : 601, 709, xvColor, JPG color,
	UINT8 panscan_num;				// the number of pan-scan window
	display_rect_t panscan_win[MAX_PANSCAN_NUM];	//the (x, y) and (width, height) of all pan-scan windows
	UINT32 phase_shift_x;			// phase shift x-coordinate for RV / VC1
	UINT32 phase_shift_y;			// phase shift y-coordinate for RV / VC1
	UINT8 pic_afd;					// frame  active format discription
	UINT8 cc_parsed;
	UINT8 cc_count;
	UINT8 cc_type[3];
	UINT8 cc_data1[3];
	UINT8 cc_data2[3];

	//PQ
	DRV_VPP_Luma_Adj_t aLumaInfo;
	UINT8 doLumaAdjust;
} mcpFrameBufVar_t;

typedef struct
{
	DRV_VPP_BistMode_e 	bist_module;
	DRV_VPP_SwitchOnOff_e 	on_off;
	DRV_VPP_YCbCrColor_t	ddfch_color;
} DRV_VPP_BistMode_t;

typedef struct
{
	DRV_VPP_ModuleID_e id;
	mcpFrameBufVar_t pframebuf;
	DRV_VPP_DisableVideoBy_e video_disable;
	DRV_VPP_OutputWin_t output;
	DRV_VPP_BistMode_t bist_mode;
#if VPP_TEST_IOCTL
	DRV_VPP_Res_e frame_res;
#endif
	DRV_VPP_YuvFmt_e yuv_fmt;
	DRV_VPP_SwitchOnOff_e hw_en;
	//DRV_VPP_DispAspectRatio_e aspect_ratio;
	SYNC_Info_t stSyncInfo;
} DRV_VPP_Param_t;

typedef struct
{
	UINT8 	bSrcAspect; 			///< [in]video source apect ratio
	UINT8 	bSrcExp; 				///< [in]video source apect expansion
	UINT8	bSrcAfd; 				///< [in]video source active format
	UINT8	bDispAspect;			///< [in]display output aspect ratio
	UINT8 	bDispExp; 				///< [in]display output apect expansion
	UINT8 	bDispSpanMode;		///< [in]the panscan mode of output display decision.
} DRV_VPP_GetAspectInfo_t;

typedef struct
{
	// 1st Step(STB side, Decoder Format Conversion)
	UINT32 	h_numerator;			///< [out]numerator factor of horizontal, set 0 if use default calculation by bSrcAspect, bSrcAfd, bDispAspect, bDispSpanMode
	UINT32	h_denominator;			///< [out]denominator factor of vertical
	UINT32	v_numerator;			///< [out]numerator factor of horizontal
	UINT32	v_denominator;			///< [out]denominator factor of vertical

	// 2nd Step(TV side, Display Action)
	UINT32 	h_numerator2;			///< [out]numerator factor of horizontal, set 0 if use default calculation by bSrcAspect, bSrcAfd, bDispAspect, bDispSpanMode
	UINT32	h_denominator2;			///< [out]denominator factor of vertical
	UINT32	v_numerator2;			///< [out]numerator factor of horizontal
	UINT32	v_denominator2;			///< [out]denominator factor of vertical

	UINT8	bSceneAspectMask; 		///< [out]enable/disale black scene aspect mask, default 4:3 mask. currently used for MHEG5
	UINT8	output_aspect;			///< [out] user decision output aspect define value:OUTPUT_ASPECT_NORMAL, OUTPUT_ASPECT_NORMAL_169LB, ....
	UINT8	bScartPin;				///< [out] scart pin value 12 for 4:3 and 9 for 16:9
} DRV_VPP_SetUserAspect_t;

typedef struct
{
	UINT8	enable;
	UINT8	degree;	// not used currently
	UINT16	sinval; //Saturation*Sin
	UINT16	cosval;//Saturation*Cos
} DRV_VPP_ImgHue_t;


typedef struct
{
	//mcpFrameBufVar_t 		mcp_framebuf_var;
	UINT32 				mcp_ioctl;
	DRV_VPP_OutputWin_t 	mcp_output_win;
	UINT8 				mcp_bVdecimOn;
	UINT8 				mcp_bVdownsizeOn;
} DRV_VPP_RcsBackup_t;

//Barney added for VPP PQ tool
typedef struct
{
	UINT16 pkti_lpk_lut_mp1;
	UINT16 pkti_lpk_lut_mp2;

	UINT8 id;
	UINT8 pkti_lpk_gain_neg;
	UINT8 pkti_lpk_gain_ep;
	UINT8 pkti_lpk_gain_bp;

	UINT8 pkti_lpk_gain_hp;
	UINT8 pkti_lpk_lut_xp1;
	UINT8 pkti_lpk_lut_xp0;
	UINT8 pkti_lpk_lut_xp3;

	UINT8 pkti_lpk_lut_xp2;
	UINT8 pkti_lpk_lut_yp1;
	UINT8 pkti_lpk_lut_yp0 ;
	UINT8 pkti_lpk_coring;

	UINT8 pkti_lpk_lut_yp2 ;
	UINT8 pkti_lpk_threshold;
	UINT8	hp_filter;
	UINT8	bp_filter;

	UINT8	ep_filter;
	UINT8	pre_lp_filter;
	UINT8	post_lp_filter;
	UINT8	reserved;
} DRV_VPP_Peaking_Info;

typedef struct
{
	UINT8	id;
	UINT8	enable;
} DRV_VPP_PQ_Enable;

typedef struct
{
	UINT8	id;
	UINT8	enable;
} DRV_VPP_FV_Enable;

typedef struct
{
	UINT8	hp_filter;
	UINT8	bp_filter;
	UINT8	ep_filter;
	UINT8	pre_lp_filter;
	UINT8	post_lp_filter;
} DRV_VPP_Peaking_filter;

typedef struct
{
	UINT8	bp_filter;
	UINT8	ep_filter;
	UINT8	stp_filter; // sp1 = 1 sp0 = 0
	UINT8	post_lp_filter;
} DRV_VPP_transient_filter;

typedef struct
{
	UINT16 hue;
	UINT16 satsin;
	UINT16 satcos;
	UINT8 saturation;
	UINT8 reserved;
} DRV_VPP_Favorite_Color;

typedef struct
{
	DRV_VPP_Favorite_Color ColorInfo[50];
	UINT16 AngleStart;
	UINT16 AngleEnd;
} DRV_VPP_Fav_Write_Package_t;

typedef struct
{
	DRV_VPP_Favorite_Color ColorInfo[12];
} DRV_VPP_Fav_Read_Package_t;

typedef struct
{
	UINT16 Angle[12];
} DRV_VPP_Fav_AngleSet_Package_t;

typedef struct
{
	DRV_VPP_Favorite_Color ColorInfo[256];
	UINT16 Angle[12];
} DRV_VPP_Fav_Bin_Package_t;

typedef struct
{


	UINT16 pkti_lti_lut_mp1;
	UINT16 pkti_lti_lut_mp2;

	UINT8 id;
	UINT8 pkti_lti_gain_ep;
	UINT8 pkti_lti_gain_bp;
	UINT8 pkti_lti_gain_hp;

	UINT8 pkti_lti_lut_xp1;
	UINT8 pkti_lti_lut_xp0;
	UINT8 pkti_lti_lut_xp3;
	UINT8 pkti_lti_lut_xp2;

	UINT8 pkti_lti_lut_yp1;
	UINT8 pkti_lti_lut_yp0;
	UINT8 pkti_lti_coring;
	UINT8 pkti_lti_lut_yp2;

	UINT8 pkti_lti_threshold;
	UINT8	bp_filter;
	UINT8	ep_filter;
	UINT8	stp_filter; // sp1 = 1 sp0 = 0

	UINT8	post_lp_filter;
	UINT8	reserved[3];
} DRV_VPP_Transient_Info;

typedef struct
{
	UINT8 id;
	//Peaking
	UINT8 pkti_lpk_ovrsht_gain_min;
	UINT8 pkti_lpk_ovrsht_gain_max;
	UINT8 pkti_lpk_clip_min;

	UINT8 pkti_lpk_clip_max;
	//DLTI
	UINT8 pkti_lti_ovrsht_gain_min;
	UINT8 pkti_lti_ovrsht_gain_max;
	UINT8 pkti_lti_clip_min;

	UINT8 pkti_lti_clip_max;
	UINT8 reserved[3];

} DRV_VPP_DeOvershoot;

#define DRV_GAMMA_LEN		256
typedef struct _boot_gamma_s {
	UINT8 bGamma_en;							//ref GAMMA_EN
	UINT8 bGamma_table_merge;					//ref GAMMA_TABLE_MERGE
	UINT8 gamma_reserve[2];
	UINT8 arGamma_table[3][DRV_GAMMA_LEN];			//ref GAMMA_TABLE, GAMMA_R_TABLE, GAMMA_G_TABLE, GAMMA_B_TABLE array vars
} DRV_BOOT_GAMMA_T;

//Used for ioctl
typedef struct{
	DRV_VPP_ModuleID_e id;
	DRV_VPP_Peaking_Info peaking_info;
	DRV_VPP_Peaking_filter peaking_filter;
} DRV_VPP_Peaking_Package;

typedef struct{
	DRV_VPP_ModuleID_e id;
	DRV_VPP_Transient_Info transient_info;
	DRV_VPP_transient_filter transient_filter;
} DRV_VPP_Transient_Package;

typedef struct{
	DRV_VPP_ModuleID_e id;
	DRV_VPP_DeOvershoot deovershoot;
} DRV_VPP_DeOvershoot_Package;

typedef struct{
	UINT8 id;
	UINT8 enable;;
} DRV_VPP_PeakingEnable;

typedef struct{
	UINT8 id;
	UINT8 enable;;
} DRV_VPP_TransientEnable;

typedef struct{
	UINT8 id;
	UINT8 val;
} DRV_VPP_PeakingTable;

typedef struct{
	UINT8 id;
	UINT8 val;
} DRV_VPP_TransientTable;

//End Barney added for VPP PQ tool

//=========
//= SUBPATH =
//=========

typedef struct _DRV_HD2SD_Info_t
{
	UINT32 Enable;
	UINT32 HD_Width;
	UINT32 HD_Height;
	UINT32 SD_Width;
	UINT32 SD_Height;
	DRV_SUBPATH_BoxMode_e Mode;
	DRV_VENC_HDMItype_e InDispMode;
	DRV_VENC_HDMItype_e OutDispMode;
	DRV_FrameRate_e FrameRate;
	DRV_DispOutID_e id;
	DRV_DMIX_LayerId_e out_level;
} DRV_HD2SD_Info_t;

typedef struct _DRV_HD2SD_Peak_t
{
	UINT8 Peak_En;
	UINT8 HP_Gain;
	UINT8 BP1_Gain;
	UINT8 BP2_Gain;

	UINT8 LUT_THM1;
	UINT8 LUT_THM2;
	UINT8 LUT_PW1;
	UINT8 LUT_PW2;

	UINT8 Peak_Limit_En;
	UINT8 Peak_Limit_Min_Dec;
	UINT8 Peak_Limit_Max_Inc;
	UINT8 Reserved;
} DRV_HD2SD_Peak_t;

typedef struct _DRV_AFRC_RTE_Info_t
{
	UINT8 enable;
	UINT8 bpp;
	UINT16 flag;
	UINT16 width;
	UINT16 height;
	DRV_DispOutID_e id;
	DRV_DMIX_LayerId_e out_level;
	DRV_SUBPATH_BoxMode_e BoxMode;
} DRV_AFRC_RTE_Info_t;

typedef struct _DRV_VENC_Format_t
{
	DRV_VENC_HDMIfmt_e bFmt;
	DRV_VENC_HDMItype_e bType;
} DRV_VENC_Format_t;

typedef struct _DRV_TCIW_INFO_t
{
	UINT16 enable;
	UINT16 vflip;
	UINT32 pLuma_base_PA;
	UINT32 pCrma_base_PA;
	DRV_TCIW_DATA_MODE_e data_mode;
	UINT16 width;
	UINT16 height;
	DRV_DispOutID_e id;
	DRV_DMIX_LayerId_e out_level;
} DRV_TCIW_INFO_t;

typedef struct _DRV_BirdView_INFO_t
{
	UINT16 enable;
	UINT16 bufnum;
	UINT16 camnum;
	UINT16 width;
	UINT16 height;
	UINT32 pLuma_base_PA;
	UINT32 pCrma_base_PA;

	UINT16 DebugIdx;	// debug
	UINT16 DebugSubIdx; // debug
} DRV_BirdView_INFO_t;

typedef struct DRV_BirdView_DeInterlance_t
{
	DRV_VPP_Param_t vppParam;
	UINT16 BufIdx;
	UINT16 SubIdx;
} DRV_BirdView_DeInterlance_t;

//Command ID for PQ tool Adjuster Tool
typedef enum{
	Panel_Adjust_Cmd		= 0x00,
	Peaking_Cmd		= 0x01,
	DLTI_Cmd		= 0x02,
	DeOver_Cmd				= 0x07,
	Peaking_Table_Cmd		= 0x10,
	DLTI_Table_Cmd		= 0x11,
	Peaking_Enable_Cmd = 0x12,
	DLTI_Enable_Cmd,
	GammaCmd,
	ColorBar_Cmd,
	SingleColor_Cmd,
	PGDisable_Cmd,
	Favorite_Cmd,
	FvEnable_Cmd,
	FvAngleSet_Cmd,
	FvDefault,
	LumaAdjustSep
} PQTOOL_Cmd_t;

typedef struct //PQTool cmd header
{
	UINT8 token[4];//4 // 4 = 0xfe, 'I', 'M', 'G'.
	UINT32 Len;
	UINT16 Version;
	UINT8 CmdID;
	UINT8 RWCmd;
	UINT8 Reserved[3];
	UINT8 CheckSum;
} PQ_HeaderCmd_t;

typedef struct //PQTool cmd header
{
	UINT8 token[4];//4 // 4 = 0xfe, 'I', 'M', 'G'.
	UINT32 Len;
	UINT8 ScenarioSelect;
	UINT8 Reserved[7];
} DRV_PQ_Bin_MainHeader;

typedef struct //PQTool cmd header
{
	UINT8 token[4];//4 // 4 = 0xfe, 'I', 'M', 'G'.
	UINT32 Len;
	UINT16 ScenarioID;
	UINT8 CmdID;
	UINT8 Enable;
	UINT8 Reserved[3];
	UINT8 CheckSum;
} DRV_PQ_Bin_SubHeader;

typedef struct DRV_SYS_PQ_PARSER_s
{
	PQ_HeaderCmd_t header;
	union {
		UINT8							raw_buf[816];
		DRV_DMIX_PQ_Adj_t				panel_adj;
		DRV_VPP_Peaking_Info				peaking_info;
		DRV_VPP_Transient_Info			transient_info;
		DRV_VPP_DeOvershoot				deover_info;
		DRV_VPP_PeakingTable				peaking_table;
		DRV_VPP_TransientTable			transient_table;
		DRV_VPP_PQ_Enable				pq_enable;
		DRV_VPP_YCbCrColor_t			color_info;
		DRV_VPP_Favorite_Color 			FavInfo;//For favorite color write
		DRV_VPP_Fav_Write_Package_t		FavWritePackage; //For favorite color write
		DRV_VPP_Fav_Read_Package_t		FavReadPackage; //For favorite color read
		DRV_VPP_FV_Enable				fv_enable;
		DRV_VPP_Fav_AngleSet_Package_t	FavSetAngle;
		DRV_VPP_Luma_Adj_t				LumaPackage;
	};
} DRV_SYS_PQ_PARSER_t;

typedef struct _DRV_Under_Scan_Info_t
{
	UINT32 ID;	// 0: main disp, 1: second disp
	UINT32 lf;	// left factor in %
	UINT32 rf;	// right factor in %
	UINT32 tf;	// top factor in %
	UINT32 bf;	// bottom factor in %
} DRV_Under_Scan_Info_t;

typedef struct _DRV_DISP_Scan_Info_t
{
	UINT32 ID;	// 0: main disp, 1: second disp
	UINT32 lf;		// left factor in %
	UINT32 rf;		// right factor in %
	UINT32 tf;		// top factor in %
	UINT32 bf;	// bottom factor in %
	DRV_DISP_ScanTarget_e target;
	DRV_DISP_ScanUsage_e usage;
} DRV_DISP_Scan_Info_t;

typedef struct _DRV_AFRC_Disp_Rgn_t
{
	UINT32 tgr_hact;
	UINT32 tgr_htot;
	UINT32 tgr_vact_st_t;
	UINT32 tgr_vact_ed_t;
	UINT32 tgr_vact_st_b;
	UINT32 tgr_vact_ed_b;
	UINT32 tgr_vtot;
} DRV_AFRC_Disp_Rgn_t;

typedef struct
{
	DRV_PQ_Bin_SubHeader PeakingHeader_Video;
	DRV_VPP_Peaking_Info Peaking_Video;

	DRV_PQ_Bin_SubHeader TransientHeader_Video;
	DRV_VPP_Transient_Info Transient_Video;


	DRV_PQ_Bin_SubHeader DeOverShootHeader_Video;
	DRV_VPP_DeOvershoot DeOverShoot_Video;

	DRV_PQ_Bin_SubHeader DmixAdjHeader;
	DRV_DMIX_PQ_Adj_t DmixAdj;
	DRV_PQ_Bin_SubHeader GammaHeader;
	DRV_BOOT_GAMMA_T GammaInfo;
	#ifdef TOOL_READY
	DRV_PQ_Bin_SubHeader FavHeaderVideo;
	DRV_VPP_Fav_Bin_Package_t FavInfoVideo;
	DRV_PQ_Bin_SubHeader FavHeaderUI;
	DRV_VPP_Fav_Bin_Package_t FavInfoUI;
	#endif
} DRV_VPP_PQ_Package;

//This package is customized for FYT corporation.
typedef struct
{
	DRV_PQ_Bin_SubHeader PeakingHeader_Video;
	DRV_VPP_Peaking_Info Peaking_Video;

	DRV_PQ_Bin_SubHeader PeakingHeader_UI;
	DRV_VPP_Peaking_Info Peaking_UI;

	DRV_PQ_Bin_SubHeader TransientHeader_Video;
	DRV_VPP_Transient_Info Transient_Video;

	DRV_PQ_Bin_SubHeader TransientHeader_UI;
	DRV_VPP_Transient_Info Transient_UI;

	DRV_PQ_Bin_SubHeader DeOverShootHeader_Video;
	DRV_VPP_DeOvershoot DeOverShoot_Video;

	DRV_PQ_Bin_SubHeader DeOverShootHeader_UI;
	DRV_VPP_DeOvershoot DeOverShoot_UI;

	DRV_PQ_Bin_SubHeader DmixAdjHeader;
	DRV_DMIX_PQ_Adj_t DmixAdj;
	DRV_PQ_Bin_SubHeader GammaHeader;
	DRV_BOOT_GAMMA_T GammaInfo;

	DRV_PQ_Bin_SubHeader FavHeaderVideo;
	DRV_VPP_Fav_Bin_Package_t FavInfoVideo;
	DRV_PQ_Bin_SubHeader FavHeaderUI;
	DRV_VPP_Fav_Bin_Package_t FavInfoUI;

	//Barney add for luma adjust in VPP
	DRV_PQ_Bin_SubHeader LumaHeaderVideo;
	DRV_VPP_Luma_Adj_t LumaVideo;


} DRV_VPP_PQ_Package_Client;


//----------------------------------------------------------------------
// Display IOCTL define
#define DISPIO_TYPE 'd'

// VENC ioctl
#define DISPIO_VENC_Format_Set			_IOW(DISPIO_TYPE, 0x60, DRV_VENC_Format_t)
#define DISPIO_VENC_InSrc_Set			_IOW(DISPIO_TYPE, 0x61, DRV_VENC_InputSource_e)
#define DISPIO_VENC_OutputParam_Get		_IOW(DISPIO_TYPE, 0x62, DRV_VENC_OutputParam_t)
#define DISPIO_VENC_OutputParam_Set		_IOW(DISPIO_TYPE, 0x63, DRV_VENC_OutputParam_t)
#define DISPIO_VENC_DispOutput_Set		_IOW(DISPIO_TYPE, 0x64, DRV_VENC_DacOutput_e)
#define DISPIO_VENC_BleEnable_Set			_IOW(DISPIO_TYPE, 0x65, UINT8)
#define DISPIO_VENC_BtbEnable_Set		_IOW(DISPIO_TYPE, 0x66, UINT8)
#define DISPIO_VENC_WtwEnable_Set		_IOW(DISPIO_TYPE, 0x67, UINT8)
#define DISPIO_VENC_DacOutputPath_Set 	_IOW(DISPIO_TYPE, 0x68, DRV_VENC_OutPath_e)
#define DISPIO_VENC_GSyncOff_Set			_IOW(DISPIO_TYPE, 0x69, UINT8)
#define DISPIO_VENC_UPS54Clock_Set		_IOW(DISPIO_TYPE, 0x6a, DRV_VENC_UPS_Param_t)
#define DISPIO_VENC_UPS108Clock_Set		_IOW(DISPIO_TYPE, 0x6b, DRV_VENC_UPS_Param_t)
#define DISPIO_VENC_MaskSel_Set			_IOW(DISPIO_TYPE, 0x6c, DRV_VENC_MaskParam_t)
#define DISPIO_VENC_SdTveFormat_Set		_IOW(DISPIO_TYPE, 0x6d, DRV_TVE_TveFmt_e)
#define DISPIO_VENC_DispOutput_Get		_IOW(DISPIO_TYPE, 0x6e, DRV_VENC_DacOutput_e)
#define DISPIO_VENC_BleEnable_Get		_IOW(DISPIO_TYPE, 0x6f, UINT8)
#define DISPIO_VENC_BtbEnable_Get		_IOW(DISPIO_TYPE, 0xc5, UINT8)
#define DISPIO_VENC_WtwEnable_Get		_IOW(DISPIO_TYPE, 0xc6, UINT8)
#define DISPIO_VENC_DacOutputPath_Get 	_IOW(DISPIO_TYPE, 0xc7, DRV_VENC_OutPath_e)
#define DISPIO_VENC_GSyncOff_Get		_IOW(DISPIO_TYPE, 0xc8, UINT8)
#define DISPIO_VENC_UPS54Clock_Get		_IOW(DISPIO_TYPE, 0xc9, DRV_VENC_UPS_Param_t)
#define DISPIO_VENC_UPS108Clock_Get		_IOW(DISPIO_TYPE, 0xca, DRV_VENC_UPS_Param_t)
#define DISPIO_VENC_MaskSel_Get			_IOW(DISPIO_TYPE, 0xcb, DRV_VENC_MaskParam_t)
#define DISPIO_VENC_SdTveFormat_Get		_IOW(DISPIO_TYPE, 0xcc, DRV_TVE_TveFmt_e)

// VPP ioctl
#define DISPIO_MID_DISP_SetDispWin			_IOW(DISPIO_TYPE, 0x70, mid_package_DispWin)
#define DISPIO_MID_DISP_GetDispWin			_IOW(DISPIO_TYPE, 0x71, mid_package_DispWin)
#define DISPIO_VPP_Luma_Get			_IOW(DISPIO_TYPE, 0x81, DRV_VPP_Luma_Adj_t)
//  DISPIO,   0x80 ~ 0xBF ,  are used by Hdmitx  ioctl

// HDMI I2C ioctl
#ifdef FPGA_7045 	//--- HDMI 7045 I2C ----------------------------------------------
#define DISPIO_I2C_DUMP				_IOW(DISPIO_TYPE, 0xc0, int)
#define DISPIO_I2C_SetAudioSampleRate44k	_IOW(DISPIO_TYPE, 0xc1, int)
#define DISPIO_I2C_SetAudioSampleRate48k	_IOW(DISPIO_TYPE, 0xc2, int)
#else
//#define DISPIO_VPP_SetAspectRatio			_IOW(DISPIO_TYPE, 0xc0, DRV_VPP_Param_t)
//#define DISPIO_VPP_GetAspectRatio			_IOW(DISPIO_TYPE, 0xc1, DRV_VPP_Param_t)
#endif
// TTX
#define DISPIO_TTX_Start					_IOW(DISPIO_TYPE, 0xc3, int)
#define DISPIO_TTX_Stop					_IOW(DISPIO_TYPE, 0xc4, int)
//---------   DISPIO_TYPE ,  0xc5~0xcf   ared used by VENC-------------------------

// Display System Mode
#define DISPIO_Sys_Mode						_IOW(DISPIO_TYPE, 0xd0, DRV_DISP_SYS_MODE_e)
#define DISPIO_Sys_ModeInfo_Get				_IOR(DISPIO_TYPE, 0xd1, DRV_Sys_Mode_Info_t)
#define DISPIO_Sys_UnderScan				_IOW(DISPIO_TYPE, 0xd2, DRV_Under_Scan_Info_t)
#define DISPIO_Sys_OutMode_Set				_IOW(DISPIO_TYPE, 0xd3, DRV_Sys_OutMode_Info_t)
#define DISPIO_Sys_OutMode_Get				_IOWR(DISPIO_TYPE, 0xd4, DRV_Sys_OutMode_Info_t)
#define DISPIO_Sys_AspectRatio_Set			_IOW(DISPIO_TYPE, 0xd5, DRV_Sys_Aspect_Rario_t)
#define DISPIO_Sys_AspectRatio_Get			_IOWR(DISPIO_TYPE, 0xd6, DRV_Sys_Aspect_Rario_t)
#define DISPIO_Sys_N_or_P_Set				_IOW(DISPIO_TYPE, 0xd7, unsigned int)
#define DISPIO_Sys_N_or_P_Get				_IOWR(DISPIO_TYPE, 0xd8, unsigned int)
#define DISPIO_Sys_CVBS_or_YUV_Set			_IOW(DISPIO_TYPE, 0xd9, unsigned int)
#define DISPIO_Sys_CVBS_or_YUV_Get			_IOWR(DISPIO_TYPE, 0xda, unsigned int)
#define DISPIO_Sys_Project_Info				_IOR(DISPIO_TYPE, 0xdb, unsigned int)
#define DISPIO_Sys_BirdView_Set				_IOW(DISPIO_TYPE, 0xdc, DRV_BirdView_INFO_t)
#define DISPIO_Sys_BirdView_Get				_IOWR(DISPIO_TYPE, 0xde, unsigned int)
#define DISPIO_Sys_PQ_PARSER				_IOW(DISPIO_TYPE, 0xdf, DRV_SYS_PQ_PARSER_t)


// Gavin Hung add test for suspend/resume
#define DISPIO_Sys_Test						_IOW(DISPIO_TYPE, 0xe0, int)
#if VPP_TEST_IOCTL
#define DISPIO_VPP_TestLayerSetting			_IOW(DISPIO_TYPE, 0xe2, DRV_VPP_Param_t)
#define DISPIO_VPP_GetOutputWindow			_IOW(DISPIO_TYPE, 0xe3, DRV_VPP_Param_t)
#define DISPIO_VPP_TestCreateFrame			_IOW(DISPIO_TYPE, 0xe4, DRV_VPP_Param_t)
#else
#define DISPIO_VPP_SetHwEnable				_IOW(DISPIO_TYPE, 0xe1, DRV_VPP_Param_t)
#define DISPIO_VPP_GetHwEnable				_IOW(DISPIO_TYPE, 0xe2, DRV_VPP_Param_t)
//#define DISPIO_VPP_TestBIST				_IOW(DISPIO_TYPE, 0xe1, DRV_VPP_Param_t)
//#define DISPIO_VPP_TestLayerSetting			_IOW(DISPIO_TYPE, 0xe2, DRV_VPP_Param_t)
#define DISPIO_VPP_GetOutputWindow			_IOW(DISPIO_TYPE, 0xe3, DRV_VPP_Param_t)
//#define DISPIO_VPP_TestCreateFrame			_IOW(DISPIO_TYPE, 0xe4, DRV_VPP_Param_t)
#define DISPIO_VPP_SetColorbarOnOff			_IOW(DISPIO_TYPE, 0xe4, DRV_VPP_Param_t)
#endif
#define DISPIO_VPP_GetOutputMode			_IOW(DISPIO_TYPE, 0xe5, DRV_VPP_Param_t)
#define DISPIO_VPP_SetSrcYuvFmt				_IOW(DISPIO_TYPE, 0xe6, DRV_VPP_Param_t)
#define DISPIO_VPP_GetCurFrameBufIdx			_IOW(DISPIO_TYPE, 0xe7, DRV_VPP_Param_t)
#define DISPIO_VPP_GetPreFrameBufIdx			_IOW(DISPIO_TYPE, 0xe8, DRV_VPP_Param_t)
#define DISPIO_VPP_UpdateDisplayBypassSync		_IOW(DISPIO_TYPE, 0xe9, DRV_VPP_Param_t)

#define DISPIO_VPP_BirdViewDeInterlance			_IOW(DISPIO_TYPE, 0xea, DRV_BirdView_DeInterlance_t)

//Defined by Barney. ioctl used for 8700 PQ tool VPP part ---------------------------
#define DISPIO_PQ_SELECT 					_IOW(DISPIO_TYPE, 0xeb, UINT32)
#define DISPIO_PQ_SCENNUM_GET 				_IOW(DISPIO_TYPE, 0xec, UINT32)

//--------------- for RCS black screen 20150730-----------
#define DISPIO_VPP_SetVppBlackScreenByRCS		_IOW(DISPIO_TYPE, 0xed, UINT32)
#define DISPIO_VPP_Luma_Adjust_Set _IOW(DISPIO_TYPE, 0xf4, UINT32)
//--- VENC DVD CP ioctls : gavin 20150514----------------------------------
#define DISPIO_CPS_SET						_IOW(DISPIO_TYPE, 0xef, DRV_CPS_Param_t)
#define DISPIO_CPS_GET						_IOW(DISPIO_TYPE, 0xf0, DRV_CPS_Param_t)
#define DISPIO_CPS_RESET					_IOW(DISPIO_TYPE, 0xf1, UINT8)
#define DISPIO_CPS_CLEAR_EncryptionMode		_IOW(DISPIO_TYPE, 0xf2, UINT8)
//------------------------------------------------------------------

#define DISPIO_VPP_UpdateClockStartBase	_IOW(DISPIO_TYPE, 0xF3, DRV_VPP_Param_t)

/**************************************************************************
 *               F U N C T I O N    D E C L A R A T I O N S               *
 **************************************************************************/
//Frame buffer driver
FB_Info* fb_GetFramebufferInfo(void);
void FB_DISP_GetSrcMemInfoEx(UINT32 disp_ID, UINT16 *wWidth, UINT16 *wHeight, UINT16 *wLinePitch);


// Graphic window semaphore
void WinSemaphoreInit(void);
int WinUpdateLock(DRV_OsdWindow_e win_id);
int WinUpdateLockWithTimeOut(DRV_OsdWindow_e win_id);
int disp_wait_for_field_end(DRV_DispOutID_e OutId);
int disp_wait_for_field_start(DRV_DispOutID_e OutId);

// GSCL function prototype
DRV_Status_e DRV_GSCL_Init(void* pInDispReg);
DRV_Status_e DRV_GSCL_Clear(DRV_OsdWindow_e id);
DRV_Status_e DRV_GSCL_Set(DRV_OsdWindow_e id, DRV_Region_Info_t* pIn, DRV_Region_Info_t* pOut);
DRV_Status_e DRV_GSCL_Get(DRV_SetGSCL_t* GetGSCL);
DRV_Status_e DRV_GSCL_ColorBar(DRV_OsdWindow_e id, UINT32 enable, UINT32 mode);
DRV_Status_e DRV_GSCL_Suspend(void);
DRV_Status_e DRV_GSCL_Resume(void);
DRV_SetGSCL_t DRV_Mid_Get_SetGSCL_Info(DRV_OsdWindow_e id);
// OSD function prototype
DRV_Status_e DRV_OSD_Init(void* pInDispReg);
DRV_Status_e DRV_OSD_InitWindow(DRV_OsdInit_t *pInit, UINT32 On_Android);
DRV_Status_e DRV_OSD_UninitWindow(DRV_OsdWindow_e win_id);
DRV_Status_e DRV_OSD_SetDispPlaneEnable(DRV_OsdWindow_e win_id, UINT8 bEnable);
DRV_Status_e DRV_OSD_GetDispPlaneEnable(DRV_OsdWindow_e win_id, UINT8* bEnable);
DRV_Status_e DRV_OSD_CreateRegion(DRV_OsdWindow_e win_id, DRV_Region_Create_t* pRegionCreate, DRV_OsdRegionHandle_t * pRetRegion, UINT32 On_Android);
DRV_Status_e DRV_OSD_DestroyRegion(DRV_OsdRegionHandle_t region);
DRV_Status_e DRV_OSD_SetOrigin(DRV_OsdRegion_Origin_t* Info);
DRV_Status_e DRV_OSD_GetOrigin(DRV_OsdRegion_Origin_t* Info);
DRV_Status_e DRV_OSD_SetVisibleBuffer(DRV_OsdRegionHandle_t region, UINT8 bBufferId, UINT32 UpdatePA);
DRV_Status_e DRV_OSD_SetClut(DRV_OsdRegionHandle_t region, UINT32* pClutDataPtr);

DRV_Status_e DRV_OSD_SetTransparency(DRV_OsdRegionHandle_t region, DRV_OsdTransparencyMode_e mode);
DRV_Status_e DRV_OSD_GetTransparency(DRV_OsdRegionHandle_t region, DRV_OsdTransparencyMode_e * pRetMode);
DRV_Status_e DRV_OSD_SetBlendLevel(DRV_OsdRegionHandle_t region, UINT8 bEnable, DRV_OsdBlendMethod_e bMethod, UINT8 bLevel);
DRV_Status_e DRV_OSD_GetBlendLevel(DRV_OsdRegionHandle_t region, UINT8 *pbEnable, DRV_OsdBlendMethod_e *pbMethod, UINT8 *pbRetLevel);
DRV_Status_e DRV_OSD_SetByteSwap(DRV_OsdRegionHandle_t region, UINT8 bEnable);
DRV_Status_e DRV_OSD_GetByteSwap(DRV_OsdRegionHandle_t region, UINT8* pbEnable);

DRV_Status_e DRV_OSD_SetSrcColorSpace(DRV_OsdWindow_e win_id, DRV_OsdColorSpace_e bSrcColor);
DRV_Status_e DRV_OSD_GetSrcColorSpace(DRV_OsdWindow_e win_id, DRV_OsdColorSpace_e *bSrcColor);
DRV_Status_e DRV_OSD_SetDispColorSpace(DRV_OsdWindow_e win_id, DRV_OsdColorSpace_e bDispColor);
DRV_Status_e DRV_OSD_GetDispColorSpace(DRV_OsdWindow_e win_id, DRV_OsdColorSpace_e *bDispColor);

DRV_Status_e DRV_OSD_SetColorBar(DRV_OsdWindow_e win_id, UINT32 enable, UINT32 mode);
DRV_Status_e DRV_OSD_GetMainRegion(DRV_OsdWindow_e win_id, DRV_OsdRegionHandle_t * pRetRegion);
DRV_Status_e DRV_OSD_SetCrop(DRV_OsdWindow_e win_id, DRV_Region_Info_t* src_crop);
DRV_Status_e DRV_OSD_GetCrop(DRV_OsdWindow_e win_id, DRV_Region_Info_t* src_crop);
DRV_Status_e DRV_OSD_Suspend(void);
DRV_Status_e DRV_OSD_Resume(void);
//DRV_Status_e DRV_OSD_IRQ(UINT32 NumTGEN, UINT32 tgen_src_sel);
DRV_Status_e DRV_OSD_IRQ(void);
DRV_Status_e DRV_OSD_Update(DRV_OsdWindow_e win_id, DRV_Region_Update_t* pUpdate, DRV_OsdRegionHandle_t pRegion);
DRV_Status_e DRV_OSD_SetBufferPA(DRV_OsdWindow_e win_id, UINT32 UpdatePA);

DRV_Status_e DRV_OSD_Test(DRV_OsdWindow_e win_id, UINT32 ColorFmt);

// TGEN function prototype
DRV_Status_e DRV_TGEN_Init(void* pInHWReg);
DRV_Status_e DRV_TGEN_Set(DRV_SetTGEN_t* SetTGEN);
DRV_Status_e DRV_TGEN_Get(DRV_SetTGEN_t* GetTGEN);
DRV_Status_e DRV_TGEN_Clear(DRV_DispOutID_e OutId);
DRV_Status_e DRV_TGEN_Adjust(DRV_DispOutID_e OutId, DRV_TGEN_Input_e Input, UINT32 Adjust);
DRV_Status_e DRV_TGEN_Source_Sel(DRV_TGEN_Input_e Input, UINT32 Sel);
DRV_Status_e DRV_TGEN_Get_Source_Sel(DRV_TGEN_Input_e Input, UINT32* Sel);
DRV_Status_e DRV_TGEN_Suspend(void);
DRV_Status_e DRV_TGEN_Resume(void);
DRV_Status_e DRV_TGEN_GetFmtFps(DRV_DispOutID_e OutId, DRV_VideoFormat_e *fmt, DRV_FrameRate_e *fps);
DRV_Status_e DRV_TGEN_GetFidStart(DRV_DispOutID_e OutId, UINT8 * fid);
DRV_Status_e DRV_TGEN_GetFidEnd(DRV_DispOutID_e OutId, UINT8 * fid);
DRV_Status_e DRV_TGEN_GetLineCntStart(DRV_DispOutID_e OutId, UINT16 * line_cnt);
DRV_Status_e DRV_TGEN_GetLineCntEnd(DRV_DispOutID_e OutId, UINT16 * line_cnt);
DRV_Status_e DRV_TGEN_UpdateDtgStatusFs(DRV_DispOutID_e OutId);
DRV_Status_e DRV_TGEN_UpdateDtgStatusFe(DRV_DispOutID_e OutId);
DRV_Status_e DRV_TGEN_SetDtgPscan(DRV_DispOutID_e OutId, UINT8 en);
unsigned int DRV_TGEN_GetLineCntNow(DRV_DispOutID_e OutId);
unsigned char DRV_TGEN_IsSafeLineCnt(DRV_DispOutID_e OutId, UINT8 start_point, UINT32 start_offset, UINT8 end_point, UINT32 end_offset);

// DMIX function prototype
DRV_Status_e DRV_DMIX_Init(void* pInReg);
DRV_Status_e DRV_DMIX_Clear(DRV_DispOutID_e id);
DRV_Status_e DRV_DMIX_BG_Sel(DRV_DispOutID_e id, DRV_DMIX_InputSel_e BG_Sel);
DRV_Status_e DRV_DMIX_BG_Sel_Get(DRV_DispOutID_e id, DRV_DMIX_InputSel_e* BG_Sel);
DRV_Status_e DRV_DMIX_VppBypass_En(DRV_DispOutID_e id, DRV_DMIX_LayerId_e Layer, UINT8 enable, UINT8 * status);
DRV_Status_e DRV_DMIX_Layer_Set(DRV_DispOutID_e id, DRV_DMIX_LayerId_e Layer, DRV_DMIX_LayerMode_e LayerMode, DRV_DMIX_InputSel_e FG_Sel);
DRV_Status_e DRV_DMIX_Layer_Get(DRV_DMIX_Layer_Set_t* pLayerInfo);
DRV_Status_e DRV_DMIX_BG_Color_Set(DRV_DispOutID_e id, UINT32 color);
DRV_Status_e DRV_DMIX_Plane_Alpha_Set(DRV_DMIX_PlaneAlpha_t* PlaneAlphaInfo);
DRV_Status_e DRV_DMIX_Luma_Adjust_Set(DRV_DMIX_Luma_Adj_t* LumaAdjInfo);
DRV_Status_e DRV_DMIX_Luma_Adjust_Get(DRV_DMIX_Luma_Adj_t* LumaAdjInfo);
DRV_Status_e DRV_DMIX_Chroma_Adjust_Set(DRV_DMIX_Chroma_Adj_t* ChromaAdjInfo);
DRV_Status_e DRV_DMIX_Chroma_Adjust_Get(DRV_DMIX_Chroma_Adj_t* ChromaAdjInfo);
DRV_Status_e DRV_DMIX_General_Set(DRV_DMIX_General_Set_t* GeneralSet);
DRV_Status_e DRV_DMIX_General_Get(DRV_DMIX_General_Set_t* GeneralSet);
DRV_Status_e DRV_DMIX_Suspend(void);
DRV_Status_e DRV_DMIX_Resume(void);

// VENC and DVE function prototype
DRV_Status_e DRV_VENC_Init(void* pInVencReg);
DRV_Status_e DRV_VENC_Set_ColorbarEnableSD(UINT8 en);
DRV_Status_e DRV_VENC_Set_ColorbarEnableHD(UINT8 en);
DRV_Status_e DRV_VENC_Set_HDMIFormat(DRV_VENC_HDMIfmt_e bFmt, DRV_VENC_HDMItype_e bType);
DRV_Status_e DRV_VENC_Get_HDMIFormat(DRV_VENC_HDMIfmt_e* bFmt, DRV_VENC_HDMItype_e* bType);
DRV_Status_e DRV_VENC_Set_InputSource(DRV_VENC_InputSource_e bSrcSelect);
DRV_Status_e DRV_VENC_Get_HdmiAFD(UINT8 *AfdType);
DRV_Status_e DRV_VENC_Set_HdmiAFD(UINT8 AfdType);
DRV_Status_e DRV_VENC_Get_CopyProtectParam(DRV_CGMS_InputParam_t *stCgmsParam);
DRV_Status_e DRV_VENC_Set_CopyProtectParam(DRV_CGMS_InputParam_t stCgmsParam);
DRV_Status_e DRV_VENC_Set_Format(DRV_VENC_Fmt_t stFormat);
DRV_Status_e DRV_VENC_Get_Format(DRV_VENC_Fmt_t *stFormat);
DRV_Status_e DRV_VENC_Set_MaskParam(DRV_VENC_MaskParam_t stMaskParam);
DRV_Status_e DRV_VENC_Get_MaskParam(DRV_VENC_MaskParam_t *stMaskParam);
DRV_Status_e DRV_VENC_LatchCcValues(mcpFrameBufVar_t *pFrameBuf);
DRV_Status_e DRV_VENC_Set_TtxLineControl(DRV_VENC_TtxLineCtlSel_e sel, unsigned int line_ctl_top, unsigned int line_ctl_btm);
DRV_Status_e DRV_VENC_BackupMacrovision(void);
DRV_Status_e DRV_VENC_RestoreMacrovision(void);
DRV_Status_e DRV_VENC_Set_TtxSignalEnable(UINT8 en);
void DRV_VENC_UpdateTvFormatHandler(void);
void DRV_VENC_UpdateAfrcFormatHandler(void);
void DRV_VENC_UpdateCgmsHandler(void);
void DRV_VENC_UpdateClosedCaptionHandler(void);
DRV_Status_e DRV_VENC_DVE_Suspend(void);
DRV_Status_e DRV_VENC_DVE_Resume(void);
DRV_Status_e DRV_VENC_FieldEndISRHandler(void);
DRV_Status_e DRV_VENC_FieldStartISRHandler(void);
//--VENC external interface ---
DRV_Status_e DRV_VENC_Set_FrameType(DRV_VENC_FrmTypeSel_t stFrmType);
DRV_Status_e DRV_VENC_Get_FrameType(DRV_VENC_FrmTypeSel_t *stFrmType);
DRV_Status_e DRV_VENC_Set_OutputPath(DRV_VENC_OutPathSel_t stOutPathSel);
DRV_Status_e DRV_VENC_Get_OutputPath(DRV_VENC_OutPathSel_t *stOutPathSel);
DRV_Status_e DRV_VENC_Set_SdTveFormat(DRV_VENC_SdtveFmt_t stSdtvFmt);
DRV_Status_e DRV_VENC_Get_SdTveFormat(DRV_VENC_SdtveFmt_t * stSdtvFmt);
DRV_Status_e DRV_VENC_Set_OutputParam(DRV_VENC_OutputParam_t stOutputParam);
DRV_Status_e DRV_VENC_Get_OutputParam(DRV_VENC_OutputParam_t *stOutputParam);
DRV_Status_e DRV_VENC_Set_FilterParam(DRV_VENC_FilterParam_t stFilterParam);
DRV_Status_e DRV_VENC_Get_FilterParam(DRV_VENC_FilterParam_t *stFilterParam);
DRV_Status_e DRV_VENC_Set_UpSampleClockEnable(DRV_VENC_UPS_Param_t stUpSampleParam);
DRV_Status_e DRV_VENC_Get_UpSampleClockEnable(DRV_VENC_UPS_Param_t *stUpSampleParam);
DRV_Status_e DRV_VENC_Set_FramePacketEnable(void /*DRV_VENC_FramePacket_e bFramePacketEn*/);
DRV_Status_e DRV_VENC_Set_SubpathFrmRate(DRV_VENC_FrmRate_e sub_fr);
//--VENC ioctl interface----------
DRV_Status_e DRV_VENC_Ioctl_Set_DispOutput(DRV_VENC_DacOutput_e bOutput);
DRV_Status_e DRV_VENC_Ioctl_Get_DispOutput(DRV_VENC_DacOutput_e* bOutput);
DRV_Status_e DRV_VENC_Ioctl_Set_BleEnable(UINT8 bEnable);
DRV_Status_e DRV_VENC_Ioctl_Get_BleEnable(UINT8* bEnable);
DRV_Status_e DRV_VENC_Ioctl_Set_BtbEnable(UINT8 bEnable);
DRV_Status_e DRV_VENC_Ioctl_Get_BtbEnable(UINT8 *bEnable);
DRV_Status_e DRV_VENC_Ioctl_Set_WtwEnable(UINT8 bEnable);
DRV_Status_e DRV_VENC_Ioctl_Get_WtwEnable(UINT8* bEnable);
DRV_Status_e DRV_VENC_Ioctl_Set_DacOutputPath(DRV_VENC_OutPath_e bDacSel);
DRV_Status_e DRV_VENC_Ioctl_Get_DacOutputPath(DRV_VENC_OutPath_e* bDacSel);
DRV_Status_e DRV_VENC_Ioctl_Set_GSyncOff(UINT8 bEnableSyncNoG);
DRV_Status_e DRV_VENC_Ioctl_Get_GSyncOff(UINT8* bEnableSyncNoG);
DRV_Status_e DRV_VENC_Ioctl_Set_UPS54Clock(UINT8 bADacEn, UINT8 bBDacEn, UINT8 bCDacEn, UINT8 bDDacEn);
DRV_Status_e DRV_VENC_Ioctl_Get_UPS54Clock(UINT8* bADacEn, UINT8* bBDacEn, UINT8* bCDacEn, UINT8* bDDacEn);
DRV_Status_e DRV_VENC_Ioctl_Set_UPS108Clock(UINT8 bADacEn, UINT8 bBDacEn, UINT8 bCDacEn, UINT8 bDDacEn);
DRV_Status_e DRV_VENC_Ioctl_Get_UPS108Clock(UINT8* bADacEn, UINT8* bBDacEn, UINT8* bCDacEn, UINT8* bDDacEn);
DRV_Status_e DRV_VENC_Ioctl_Set_MaskSel(UINT8 bHdEnable, UINT8 bSdEnable);
DRV_Status_e DRV_VENC_Ioctl_Get_MaskSel(UINT8* bHdEnable, UINT8* bSdEnable);
DRV_Status_e DRV_VENC_Ioctl_Set_SdTveFormat(DRV_TVE_TveFmt_e bSdtveFmt);
DRV_Status_e DRV_VENC_Ioctl_Get_SdTveFormat(DRV_TVE_TveFmt_e* bSdtveFmt);
DRV_Status_e SetDacDisable(DRV_Platform_VENC_DACSel_e bADisable, DRV_Platform_VENC_DACSel_e bBDisable, DRV_Platform_VENC_DACSel_e bCDisable, DRV_Platform_VENC_DACSel_e bDDisable);
DRV_Status_e GetDacDisable(DRV_Platform_VENC_DACSel_e *bADisable,DRV_Platform_VENC_DACSel_e *bBDisable, DRV_Platform_VENC_DACSel_e *bCDisable, DRV_Platform_VENC_DACSel_e *bDDisable);
DRV_Status_e SetDacMode(DRV_Platform_CurrentMode_e bAMode, DRV_Platform_CurrentMode_e bBMode, DRV_Platform_CurrentMode_e bCMode, DRV_Platform_CurrentMode_e bDMode);
DRV_Status_e GetDacMode(DRV_Platform_CurrentMode_e *bAMode, DRV_Platform_CurrentMode_e *bBMode, DRV_Platform_CurrentMode_e *bCMode, DRV_Platform_CurrentMode_e *bDMode);
DRV_Status_e SetAvpdEnable(DRV_Platform_VENC_Select_e bEnable);
DRV_Status_e GetAvpdEnable(DRV_Platform_VENC_Select_e *bEnable);
DRV_Status_e SetAvpdDacTurnOff(DRV_Platform_VENC_Select_e bEnable);
DRV_Status_e GetAvpdDacTurnOff(DRV_Platform_VENC_Select_e *bEnable);
DRV_Status_e SetAvpdDacMuxEnable(DRV_Platform_VENC_Select_e bAEnable, DRV_Platform_VENC_Select_e bBEnable, DRV_Platform_VENC_Select_e bCEnable, DRV_Platform_VENC_Select_e bDEnable);
DRV_Status_e GetAvpdDacMuxEnable(DRV_Platform_VENC_Select_e *bAEnable, DRV_Platform_VENC_Select_e *bBEnable, DRV_Platform_VENC_Select_e *bCEnable, DRV_Platform_VENC_Select_e *bDEnable);
DRV_Status_e SetAvpdTimePeriod(DRV_Platform_VENC_PeriodTimeSel_e bTimePeriod);
DRV_Status_e GetAvpdTimePeriod(DRV_Platform_VENC_PeriodTimeSel_e *bTimePeriod);
void AvpdDetectThead(void *pThreadData);
//--- End of VENC external interface


// TTX-P2S
//DRV_Status_e DRV_Ttx_Get_BufferAddress(UINT8 * ttx_buf_addr);
DRV_Status_e DRV_Ttx_Init(void* pInTtxReg);
DRV_Status_e DRV_Ttx_SetBaseAddr(UINT32 b_addr);
DRV_Status_e DRV_Ttx_Start(void);
DRV_Status_e DRV_Ttx_Stop(void);
void DRV_Ttx_FieldEndISRHandler(void);
int DRV_Ttx_Parser(const char *user_buf);
DRV_Status_e DRV_Ttx_Suspend(void);
DRV_Status_e DRV_Ttx_Resume(void);

// CPS
DRV_Status_e DRV_CPS_Set(DRV_CPS_Param_t cps_param);
DRV_Status_e DRV_CPS_Get(DRV_CPS_Param_t * cps_param);
DRV_Status_e DRV_CPS_Reset(void);
DRV_Status_e DRV_CPS_Clear_EncryptionMode(UINT8 val);

// Subpath (HD2SD, AFRC, RTE, TCIW) function prototype
DRV_Status_e DRV_SUBPATH_Init(void* pInSubpathReg);
DRV_Status_e DRV_SUBPATH_HD2SD_Set(DRV_HD2SD_Info_t* Info);
DRV_Status_e DRV_SUBPATH_HD2SD_LumaPeaking(DRV_HD2SD_Peak_t* Peak);
DRV_Status_e DRV_SUBPATH_AFRC_Set(DRV_AFRC_RTE_Info_t* Info);
DRV_Status_e DRV_SUBPATH_RTE_Set(DRV_AFRC_RTE_Info_t* Info);
DRV_Status_e DRV_SUBPATH_RTE_State(UINT32* enable);
DRV_Status_e DRV_SUBPATH_RTE_PN_Switch_TCON(DRV_FrameRate_e fps);
DRV_Status_e DRV_SUBPATH_RTE_PN_Switch(DRV_FrameRate_e fps);
DRV_Status_e DRV_SUBPATH_TCIW_Set(DRV_TCIW_INFO_t* Info);
DRV_Status_e DRV_SUBPATH_TCIW_Get(DRV_TCIW_INFO_t* Info);
DRV_Status_e DRV_SUBPATH_TCIW_BirdView_Set(DRV_TCIW_INFO_t* Info);
DRV_Status_e DRV_SUBPATH_TCIW_IRQ(void);

DRV_Status_e DRV_SUBPATH_Suspend(void);
DRV_Status_e DRV_SUBPATH_Resume(void);

DRV_Status_e DRV_SUBPATH_AFRC_Get_FbAddr(UINT32 *faddr0_lw, UINT32 *faddr0_hw);
DRV_Status_e DRV_SUBPATH_AFRC_Set_FbAddr(UINT32 faddr0_lw, UINT32 faddr0_hw);
DRV_Status_e DRV_SUBPATH_AFRC_Get_FbCtrl(UINT32 *fb_ctrl0, UINT32 *fb_ctrl1);
DRV_Status_e DRV_SUBPATH_AFRC_Set_FbCtrl(UINT32 fb_ctrl0, UINT32 fb_ctrl1);
DRV_Status_e DRV_SUBPATH_AFRC_Get_CnterResetVal(UINT32 *vcnt, UINT32 *hcnt);
DRV_Status_e DRV_SUBPATH_AFRC_Set_CnterResetVal(UINT32 vcnt, UINT32 hcnt);
DRV_Status_e DRV_SUBPATH_AFRC_Set_DispRgn(DRV_AFRC_Disp_Rgn_t *rgn);
DRV_Status_e DRV_SUBPATH_AFRC_Set_RWLimiter(UINT32 req_cnt);
DRV_Status_e DRV_SUBPATH_AFRC_Get_RWLimiter(UINT32 *req_cnt);
DRV_Status_e DRV_SUBPATH_AFRC_Set_Mode(UINT32 mode0, UINT32 mode1, UINT32 mode2);
DRV_Status_e DRV_SUBPATH_AFRC_Get_Mode(UINT32 *mode0, UINT32 *mode1, UINT32 *mode2);

DRV_Status_e DRV_SUBPATH_HD2SD_Get_Configs(UINT32 *config, UINT32 *config2, UINT32 *peak_config);
DRV_Status_e DRV_SUBPATH_HD2SD_Set_Configs(UINT32 config, UINT32 config2, UINT32 peak_config);
DRV_Status_e DRV_SUBPATH_HD2SD_Get_UsrExpansion(UINT32 *user_hexp0, UINT32 *user_hexp1, UINT32 *user_vexp0, UINT32 *user_vexp1, UINT32 *user_vexp2);
DRV_Status_e DRV_SUBPATH_HD2SD_Set_UsrExpansion(UINT32 user_hexp0, UINT32 user_hexp1, UINT32 user_vexp0, UINT32 user_vexp1, UINT32 user_vexp2);
DRV_Status_e DRV_BirdView_Set(DRV_BirdView_INFO_t* BirdViewInfo);

//Barney VPP 8700 PQ tool
DRV_Status_e DRV_VPP_Set_Peaking(DRV_VPP_Peaking_Info *peaking_info);
DRV_Status_e DRV_VPP_Get_Peaking(DRV_VPP_Peaking_Info *peaking_info);
DRV_Status_e DRV_VPP_Set_Transient(DRV_VPP_Transient_Info *transient_info);
DRV_Status_e DRV_VPP_Get_Transient(DRV_VPP_Transient_Info *transient_info);
DRV_Status_e DRV_VPP_Set_DeOvershoot(DRV_VPP_DeOvershoot  *deovershoot);
DRV_Status_e DRV_VPP_Get_DeOvershoot(DRV_VPP_DeOvershoot *deovershoot);
DRV_Status_e DRV_DMIX_PQ_Tool_Write(DRV_DMIX_PQ_Adj_t* PQ_Adj);
DRV_Status_e DRV_DMIX_PQ_Tool_Read(DRV_DMIX_PQ_Adj_t* PQ_Adj);
void PQ_Write_Function_Switch(PQTOOL_Cmd_t CmdID, UINT8 *RWTableBuf, UINT8 FCA_Flag);
UINT16 PQ_Read_Function_Switch(PQTOOL_Cmd_t CmdID, UINT8 *RWTableBuf, UINT32 len, UINT8 FCA_Flag);
DRV_Status_e DRV_VPP_Set_PeakingEnable_PQ(DRV_VPP_PQ_Enable* enable_info);
DRV_Status_e DRV_VPP_Set_DLTIEnable_PQ(DRV_VPP_PQ_Enable* enable_info);
DRV_Status_e DRV_VPP_PQTOOL_Set_PeakingTable(DRV_VPP_PeakingTable* package);
DRV_Status_e DRV_VPP_PQTOOLSet_TransienceTable(DRV_VPP_TransientTable* package);
DRV_Status_e DRV_VPP_Load_PQ_Bin(int SceneSelect);
//End Barney

// VPP function prototype
DRV_Status_e DRV_VPP_BaseInit(void* pInVppReg);
DRV_Status_e DRV_VPP_Fav_Apply_Bin(int ScenarioSelect);
DRV_Status_e DRV_VPP_HwInit(DRV_VPP_ModuleID_e id);
DRV_Status_e DRV_VPP_SwInit(DRV_VPP_ModuleID_e id);
//DRV_Status_e DRV_VPP_Init(void* pInVppReg, unsigned short frame_buf_width, unsigned short frame_buf_height);
DRV_Status_e DRV_VPP_FPGA_CreateSingleFrame(DRV_VPP_ModuleID_e id);
DRV_Status_e DRV_VPP_FPGA_CreateSingleFrame1(DRV_VPP_ModuleID_e id, UINT32 BufIdx, UINT32 SubIdx);
DRV_Status_e DRV_VPP_FPGA_CreateSingleFrameEx(DRV_VPP_ModuleID_e id, DRV_VPP_Res_e res_fmt);
DRV_Status_e DRV_VPP_Set_BaseAddr1D(DRV_VPP_ModuleID_e id, DRV_VPP_BaseAddr1D_t* base);
//DRV_Status_e DRV_VPP_Set_BaseAddr2D(DRV_VPP_ModuleID_e id, DRV_VPP_BaseAddr2D_t* base);
//DRV_Status_e DRV_VPP_Set_DataFetchInfo(DRV_VPP_ModuleID_e id, DRV_VPP_DataFetchInfo_t* val);
//DRV_Status_e DRV_VPP_Get_DataFetchInfo(DRV_VPP_ModuleID_e id, DRV_VPP_DataFetchInfo_t* val);
DRV_Status_e DRV_VPP_Set_VC1RangeMap(DRV_VPP_ModuleID_e id, DRV_VPP_RangeMap_t* val);
DRV_Status_e DRV_VPP_Get_VC1RangeMap(DRV_VPP_ModuleID_e id, DRV_VPP_SetRangeMap_t* val);
DRV_Status_e DRV_VPP_Set_SourceCropping(DRV_VPP_ModuleID_e id, DRV_VPP_Plane_t* val);
DRV_Status_e DRV_VPP_Get_SourceCropping(DRV_VPP_ModuleID_e id, DRV_VPP_Plane_t* val);
DRV_Status_e DRV_VPP_Set_DecimationMode(DRV_VPP_ModuleID_e id, DRV_VPP_DecimationMode_t* val);
//DRV_Status_e DRV_VPP_Get_DecimationMode(DRV_VPP_ModuleID_e id, DRV_VPP_DecimationMode_t* val);
//DRV_Status_e DRV_VPP_Set_MotionRWUserMode(DRV_VPP_ModuleID_e id);
//DRV_Status_e DRV_VPP_Set_ChromaUpsample(DRV_VPP_ModuleID_e id, DRV_VPP_ChromaUpsample_t* val);
//DRV_Status_e DRV_VPP_Get_ChromaUpsample(DRV_VPP_ModuleID_e id, DRV_VPP_ChromaUpsample_t* val);
//DRV_Status_e DRV_VPP_Set_MotionCropUserMode(DRV_VPP_ModuleID_e id, DRV_VPP_SwitchOnOff_e val);
DRV_Status_e DRV_VPP_Set_OutputScaling(DRV_VPP_ModuleID_e id, DRV_VPP_Plane_t* val, DRV_VPP_UpdatedBy_e up_src);
DRV_Status_e DRV_VPP_Set_DeringInfo(DRV_VPP_ModuleID_e id, DRV_VPP_DeringParam_t* val);
DRV_Status_e DRV_VPP_Set_VimgInfo(DRV_VPP_ModuleID_e id, DRV_VPP_VimgParam_t* val);
DRV_Status_e DRV_VPP_Set_DeinterlaceEnable(DRV_VPP_ModuleID_e id);
DRV_Status_e DRV_VPP_Set_DeinterlaceDisable(DRV_VPP_ModuleID_e id);
DRV_Status_e DRV_VPP_Set_DeinterlaceMode(DRV_VPP_ModuleID_e id, DRV_VPP_DeintMode_e val);
DRV_Status_e DRV_VPP_Set_DeinterlaceUpdate(DRV_VPP_ModuleID_e id, DRV_VPP_DeintMode_e mode, DRV_VPP_DeintUpdate_e val, DRV_VPP_DeintDirection_e forced_dir);

//DRV_Status_e DRV_VPP_Set_DeinterlaceMode(DRV_VPP_ModuleID_e id, DRV_VPP_DeintModeSel_e val);
//DRV_Status_e DRV_VPP_Set_DeinterlaceInfo(DRV_VPP_ModuleID_e id, DRV_VPP_DeintInfo_t* val);
//DRV_Status_e DRV_VPP_DeinterlaceInit(DRV_VPP_ModuleID_e id);
//DRV_Status_e DRV_VPP_Set_ProgressiveNormal(DRV_VPP_ModuleID_e id);
//DRV_Status_e DRV_VPP_Set_Progressive24P(DRV_VPP_ModuleID_e id);
//DRV_Status_e DRV_VPP_Set_PauseMode(DRV_VPP_ModuleID_e id);
DRV_Status_e DRV_VPP_DetectPulldownType(DRV_VPP_ModuleID_e id); //put it into field isr, trace several fields to decide pull down pattern (RG_VAL_32_MOTION)
DRV_Status_e DRV_VPP_DetectBadEdit(DRV_VPP_ModuleID_e id); // put it into field isr, check the bit : RG_VAL_BE_DETECTED, if 1, enable deinterlace
//DRV_Status_e DRV_VPP_Detect2D3DTendency(DRV_VPP_ModuleID_e id); // put it into field isr,
DRV_Status_e DRV_VPP_Set_AutoSceneChangeDetectEnable(DRV_VPP_ModuleID_e id); // RG_HIS_SC_RESET : G188,A0:deint_decision_param_1, B5
DRV_Status_e DRV_VPP_Set_AutoSceneChangeDetectDisable(DRV_VPP_ModuleID_e id); // RG_HIS_SC_RESET : G188,A0:deint_decision_param_1, B5
DRV_Status_e DRV_VPP_Set_PeakingTable(DRV_VPP_ModuleID_e id, DRV_VPP_PeakingTableIndex_e val);
DRV_Status_e DRV_VPP_Set_PeakingEnableEx(DRV_VPP_ModuleID_e id, DRV_VPP_PeakingEnable_e val);
DRV_Status_e DRV_VPP_Set_PeakingEnable(DRV_VPP_ModuleID_e id);
DRV_Status_e DRV_VPP_Set_PeakingDisable(DRV_VPP_ModuleID_e id);
DRV_Status_e DRV_VPP_Set_TransienceTable(DRV_VPP_ModuleID_e id, DRV_VPP_TransienceTableIndex_e val);
DRV_Status_e DRV_VPP_Set_TransienceEnableEx(DRV_VPP_ModuleID_e id, DRV_VPP_TransienceEnable_e val);
DRV_Status_e DRV_VPP_Set_TransienceEnable(DRV_VPP_ModuleID_e id);
DRV_Status_e DRV_VPP_Set_TransienceDisable(DRV_VPP_ModuleID_e id);
DRV_Status_e HwPQRecordPoint(void);
DRV_Status_e HwPQScenSelect(UINT32 select);
//DRV_Status_e DRV_VPP_UpdateDeintStatus(DRV_VPP_ModuleID_e id);
DRV_Status_e DRV_VPP_UpdateDisplayMcp(DRV_VPP_Param_t * vpp_param);
DRV_Status_e DRV_VPP_UpdateDisplayEx(DRV_VPP_ModuleID_e id, mcpFrameBufVar_t *pframebuf_var, DRV_VPP_UpdatedBy_e up_src, UINT32 DisableMirrorVPP);
DRV_Status_e DRV_VPP_UpdateDisplayEx1(DRV_VPP_ModuleID_e id, mcpFrameBufVar_t *pframebuf_var, DRV_VPP_UpdatedBy_e up_src);
DRV_Status_e DRV_VPP_UpdateDisplay(DRV_VPP_ModuleID_e id, mcpFrameBufVar_t *pframebuf_var);
DRV_Status_e DRV_VPP_Set_VideoDisable(DRV_VPP_ModuleID_e id, DRV_VPP_UpdatedBy_e up_src);
DRV_Status_e DRV_VPP_Set_OutputMode(DRV_VPP_ModuleID_e id, DRV_VPP_OutputMode_e out_mode, display_rect_t out_win);
DRV_Status_e DRV_VPP_Get_OutputMode(DRV_VPP_ModuleID_e id, DRV_VPP_OutputMode_e * out_mode, display_rect_t * out_win);
#if 0
DRV_Status_e DRV_VPP_Set_OutputWindow(DRV_VPP_ModuleID_e id, DRV_VPP_OutputWin_t * win);
DRV_Status_e DRV_VPP_Get_OutputWindow(DRV_VPP_ModuleID_e id, DRV_VPP_OutputWin_t * win);
#endif
DRV_Status_e DRV_VPP_Set_SrcYuvFmt(DRV_VPP_ModuleID_e id, DRV_VPP_YuvFmt_e yuv);
DRV_Status_e DRV_VPP_Set_DctrlBgc(DRV_VPP_ModuleID_e id, UINT8 bg_y, UINT8 bg_u, UINT8 bg_v);
DRV_Status_e DRV_VPP_Set_VpostBgc(DRV_VPP_ModuleID_e id, UINT8 bg_y, UINT8 bg_u, UINT8 bg_v);
DRV_Status_e DRV_VPP_Set_VpostLumaKey(DRV_VPP_ModuleID_e id, UINT8 on_off, UINT16 luma_key);
DRV_Status_e DRV_VPP_RearView_IRQ(DRV_VPP_ModuleID_e id);
DRV_Status_e DRV_VPP_IRQ(unsigned char module_id);
DRV_Status_e DRV_VPP_FieldEndISRHandler(DRV_VPP_ModuleID_e id);
DRV_Status_e DRV_VPP_FieldStartISRHandler(DRV_VPP_ModuleID_e id);
DRV_Status_e DRV_VPP_Set_DDFCH_Enable(DRV_VPP_ModuleID_e id);
DRV_Status_e DRV_VPP_Set_DDFCH_Disable(DRV_VPP_ModuleID_e id);
DRV_Status_e DRV_VPP_Set_VSCL_Enable(DRV_VPP_ModuleID_e id);
DRV_Status_e DRV_VPP_Set_VSCL_Disable(DRV_VPP_ModuleID_e id);
DRV_Status_e DRV_VPP_Set_DEINT_Enable(DRV_VPP_ModuleID_e id);
DRV_Status_e DRV_VPP_Set_DEINT_Disable(DRV_VPP_ModuleID_e id);
DRV_Status_e DRV_VPP_Set_VPOST_Enable(DRV_VPP_ModuleID_e id);
DRV_Status_e DRV_VPP_Set_VPOST_Disable(DRV_VPP_ModuleID_e id);
DRV_Status_e DRV_VPP_Set_IMG_Enable(DRV_VPP_ModuleID_e id);
DRV_Status_e DRV_VPP_Set_IMG_Disable(DRV_VPP_ModuleID_e id);
DRV_Status_e DRV_VPP_Set_NR_Enable(DRV_VPP_ModuleID_e id);
DRV_Status_e DRV_VPP_Set_NR_Disable(DRV_VPP_ModuleID_e id);
DRV_Status_e DRV_VPP_Set_AllModules_Enable(DRV_VPP_ModuleID_e id);
DRV_Status_e DRV_VPP_Set_AllModules_Disable(DRV_VPP_ModuleID_e id);
DRV_Status_e DRV_VPP_Suspend(void);
DRV_Status_e DRV_VPP_Resume(void);
DRV_Status_e DRV_VPP_Set_UnderscanUpdate(void);
DRV_Status_e DRV_VPP_Set_ContrastAdjust(DRV_VPP_ModuleID_e id, DRV_VPP_ConAdjParam_t *val);
DRV_Status_e DRV_VPP_Get_ContrastAdjust(DRV_VPP_ModuleID_e id, DRV_VPP_ConAdjParam_t *val);
DRV_Status_e DRV_VPP_Luma_Adjust_Set(DRV_VPP_ModuleID_e id, DRV_VPP_Luma_Adj_t* LumaAdjInfo);
DRV_Status_e DRV_VPP_8700_Luma_Adjust_Set(DRV_VPP_ModuleID_e id, DRV_VPP_Luma_Adj_t* LumaAdjInfo, UINT8 LumaStart);
DRV_Status_e DRV_VPP_Luma_Write(DRV_VPP_Luma_Adj_t* LumaAdjust);
DRV_Status_e DRV_VPP_Luma_Read(DRV_VPP_Luma_Adj_t* LumaAdjust);
DRV_Status_e DRV_VPP_Get_FieldIndex(DRV_VPP_ModuleID_e id, UINT8 *f_index);
DRV_Status_e DRV_VPP_Get_AspectInfo(DRV_VPP_ModuleID_e id, DRV_VPP_GetAspectInfo_t * aspect_info);
DRV_Status_e DRV_VPP_Set_UserAspectInfo(DRV_VPP_ModuleID_e id, DRV_VPP_SetUserAspect_t * user_aspect);
DRV_Status_e DRV_VPP_Set_HwEnable(DRV_VPP_ModuleID_e id, DRV_VPP_SwitchOnOff_e on_off);
DRV_Status_e DRV_VPP_Get_HwEnable(DRV_VPP_ModuleID_e id, DRV_VPP_SwitchOnOff_e * on_off);
DRV_Status_e DRV_VPP_IMG1_For_G2D(void);
DRV_Status_e DRV_VPP_PQ_Favoriate_Enable(DRV_VPP_ModuleID_e id, int enable);
DRV_Status_e DRV_VPP_PQ_Favoriate_Init(DRV_VPP_ModuleID_e id);
DRV_Status_e DRV_VPP_PQ_Favoriate_Bin_Select(DRV_VPP_ModuleID_e id, DRV_VPP_Fav_Bin_Package_t* FavInfo, UINT32 enable);
DRV_Status_e DRV_VPP_PQ_Favoriate_Bin_Set(DRV_VPP_ModuleID_e id, DRV_VPP_Fav_Bin_Package_t* FavInfo);
DRV_Status_e DRV_VPP_PQ_Favorite_Angle_Set(UINT32 SetByUser, DRV_VPP_Fav_AngleSet_Package_t* set, DRV_VPP_ModuleID_e id);
DRV_Status_e DRV_VPP_PQ_Favoriate_Set(DRV_VPP_ModuleID_e id, DRV_VPP_Fav_Write_Package_t *FavInfo);
DRV_Status_e DRV_VPP_PQ_Favoriate_Get(DRV_VPP_ModuleID_e id, DRV_VPP_Fav_Read_Package_t *FavPackage);
DRV_Status_e DRV_VPP_Set_ImgHue(DRV_VPP_ModuleID_e id, DRV_VPP_ImgHue_t hue);
DRV_Status_e DRV_VPP_Get_ImgHue(DRV_VPP_ModuleID_e id, DRV_VPP_ImgHue_t* hue);
DRV_Status_e DRV_VPP_Set_ToSpecificDisplay(DRV_VPP_ModuleID_e id, DRV_VPP_DisplaySel_e display);
DRV_Status_e DRV_VPP_Get_ToSpecificDisplay(DRV_VPP_ModuleID_e id, DRV_VPP_DisplaySel_e * display);
UINT8 DRV_VPP_Get_Inited(DRV_VPP_ModuleID_e id);
DRV_Status_e DRV_VPP_Set_Pause(DRV_VPP_ModuleID_e id, DRV_VPP_SwitchOnOff_e on_off);
DRV_Status_e DRV_VPP_Set_VpostLumaSupress(DRV_VPP_ModuleID_e id, DRV_VPP_SwitchOnOff_e on_off);
DRV_Status_e DRV_VPP_Set_ColorbarOnOff(DRV_VPP_ModuleID_e id, DRV_VPP_BistMode_e bist_mode, DRV_VPP_SwitchOnOff_e on_off, DRV_VPP_YCbCrColor_t* ddfch_color);
#if VPP_TEST_IOCTL
DRV_Status_e DRV_VPP_Test_LayerSetting(DRV_VPP_ModuleID_e id);
#endif
DRV_Status_e DRV_VPP_Set_AvsyncFieldInterval(DRV_VPP_ModuleID_e id, UINT8 user_mode_en, UINT32 tcon_fps, UINT32 tcon_factor, DRV_FrameRate_e normal_fps);
DRV_Status_e DRV_VPP_Set_WindowForICTOn(DRV_VPP_ModuleID_e id);
DRV_Status_e DRV_VPP_Set_RestoreFromICTOff(DRV_VPP_ModuleID_e id);
void SetVar(FB_Info *var, DRV_OsdRegionFormat_e Fmt);

#ifdef FPGA_7045
//HDMI I2C test functions
void DRV_I2C_INIT(void);
void DRV_I2C_Print(void);
void DRV_I2C_DUMP(void);
int DRV_I2C_Get_ResWidth(void);
int DRV_I2C_Get_ResHeight(void);
void DRV_I2C_Set_SampleRate44kHz(void);
void DRV_I2C_Set_SampleRate48kHz(void);
#endif

//--
void SystemInit(DRV_DISP_SYS_MODE_e SysMode, display_size_t* pMainRes, display_size_t* pSecRes, UINT32 On_Android);
void UnderScanUpdate(DRV_DISP_Scan_Info_t* UnderScanInfo);
void GetScanInfo(DRV_DISP_Scan_Info_t* UnderScanInfo);
void GetUnderScanInfo(DRV_DISP_Scan_Info_t* UnderScanInfo);
int SwitchOutMode(DRV_Sys_OutMode_Info_t* pOutMode);

//#define EXPORT_SYMBOL(x)
//Fbtest
int display_boot(void *pTmpRegBase);
void PanelDisp(void *pTmpRegBase);

#define DRAM_A 1
#define DRAM_B 0
#define chunk_pa(x) gp_chunk_pa(x)
#define chunk_va(x) gp_chunk_va(x)
#define chunk_usr_va_to_pa(x) gp_user_va_to_pa(x)

#ifdef FPGA_7045
#define chunk_malloc(id, x) gp_chunk_malloc_nocache(0, (x))
#define chunk_num() 1
#else
#define chunk_num() gp_chunk_num()
#define chunk_malloc(id, x) gp_chunk_malloc_nocache((id), 0, (x));
#endif
#define chunk_free(x) gp_chunk_free(x)

#endif //__DISP_GRAPHIC_HEADER_
