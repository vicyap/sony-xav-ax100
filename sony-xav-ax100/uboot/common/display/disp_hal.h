#define UBOOT_TCON
#include "drivers/tcon/hal_SPHE8388_tcon.h"
#undef UBOOT_TCON

#define GAMMA_OPEN

#if 0
typedef char				INT8;
typedef short				INT16;
typedef int 				INT32;
typedef long long			INT64;
typedef char				SINT8;
typedef short				SINT16;
typedef int 				SINT32;
typedef long long			SINT64;
typedef unsigned char		UINT8;
typedef unsigned short		UINT16;
typedef unsigned int		UINT32;
typedef unsigned long long	UINT64;
#endif

#define REG_START_8388 			0x9c000000
#define RGST_TNR_0_OFFSET 291*128
#define RGST_TNR_1_OFFSET 293*128

#define	SWAP32(x)	((((UINT32)(x))&0x000000ff)<<24 | (((UINT32)(x))&0x0000ff00)<<8 | (((UINT32)(x))&0x00ff0000)>>8 | (((UINT32)(x))&0xff000000)>>24)
#define BOOT_LOGO_IMG_NUM 12
#if 0
#define PQ_DEBUG(fmt, args...)		printf(fmt, ##args)
#else
#define PQ_DEBUG(...)
#endif
#define DBGPRINT(args...) 	printf(args)

// VPP IMG W/R for  A.24
#define DRV_VPP_IMG_SRAM_WROFF	0x0
#define DRV_VPP_IMG_SRAM_WRITE	0x1
#define DRV_VPP_IMG_SRAM_READ		0x3

#define HORIZONTAL_CENTERING	(1<<0)
#define VERTICAL_CENTERING 		(1<<1)
#define HORIZONTAL_SCALING		(1<<2)
#define VERTICAL_SCALING		(1<<3)

/*=== Dimix def. Start ===*/
#define DIMIX_LUMA_OFFSET_MIN -50
#define DIMIX_LUMA_OFFSET_MAX 50
#define DIMIX_LUMA_SLOPE_MIN 0.60
#define DIMIX_LUMA_SLOPE_MAX 1.40
/*=== Dimix def. End ===*/

//#define ALIGN(n, a)		(((n) + (a) - 1) & (~((a) - 1)))
#define PAGEALIGN(x)	((x + 4095) >> 12 << 12)

#ifndef NULL
#define NULL 0
#endif

typedef struct _DRV_Logo_header_t
{
	unsigned int Identifier;  	// Must be 0x4c4f474f = 'LOGO'
	unsigned int x;			// start position
	unsigned int y;			// start position
	unsigned int width;		// logo data width
	unsigned int height;		// logo data height
	unsigned int location_flag; // Horizontal_centering and Vertical_centering
	unsigned int reserved[2];
} DRV_Logo_header_t;

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
} DRV_VPP_Peaking_Info_u;

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

} DRV_VPP_Transient_Info_u;

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

} DRV_VPP_DeOvershoot_u;

typedef struct
{
	UINT16 hue;
	UINT16 satsin;
	UINT16 satcos;
	INT8 brightness; //8388
	UINT8 saturation;
	UINT8 RecordNode;
	UINT8 reserved;
} DRV_VPP_Favorite_Color_u;

typedef struct
{
	DRV_VPP_Favorite_Color_u ColorInfo[256];
	UINT16 Angle[12];
} DRV_VPP_Fav_Bin_Package_u;

typedef struct
{
	UINT32 hue;
	//For chroma
	UINT16 satsin;
	UINT16 satcos;

	UINT8 OutId;

	//For brightness and contrast
	UINT8 contrast;
	UINT8 brightness;
	UINT8 saturation;
} DRV_Dmix_LumaHue_u;

typedef struct _DRV_DMIX_Chroma_Adj_u
{
	int OutId;
	UINT32 enable;
	UINT16 satcos;
	UINT16 satsin;
} DRV_DMIX_Chroma_Adj_u;

typedef struct _DRV_DMIX_Luma_Adj_u
{
	int OutId;
	UINT32 enable;
	UINT32 brightness;
	UINT32 contrast;

} DRV_DMIX_Luma_Adj_u;

typedef struct
{
	UINT8 enable;
	UINT8 brightness;
	UINT8 contrast;
	UINT8 id;

} VPP_Luma_Adj_u;

typedef struct
{
	UINT8 id;
	UINT8 GainWeight;

} DRV_VPP_DCTI_Info_t;

typedef struct
{
	//DeOverShoot
	UINT8 fn_cor_th; //Coring threshold
	UINT8 edge_th_dovs; //Edge detection threshold
	UINT8 ring_th; //Ring detection threshold

	UINT8 ring_L0; //Edge level factor
	UINT8 ring_L1;
	UINT8 ring_L2;
	UINT8 ring_L3;

	UINT8 fn_L0;
	UINT8 fn_L1;
	UINT8 fn_L2;
	UINT8 fn_L3;

	//DeMosquito
	UINT8 drstr_demosq; //Strength
	UINT8 drslope_demosq; //Slpoe
	UINT8 dr_edge_th_demosq; // Edge TH
	UINT8 clamp_demosq; //Clamp TH

	//De-Ring
	UINT8 clamp_dr; //Dering clamp value
	UINT8 dnr_str; //Strength
	UINT8 edge_med_slp; //Slope
	UINT8 dr_edge_th; //Edge_th
	UINT8 dr_edge_range;// Edge range detection

	//SNR edge detection
	UINT8 edge_lrg_slp; //edge slope
	UINT8 line_weight; //line weight
	UINT8 db_str; //Weight strength
	UINT8 clapm_dvb; //Clamping threshold

} DRV_VPP_NR_User_t;

typedef struct //PQTool cmd header
{
	UINT8 token[4];//4 // 4 = 0xfe, 'I', 'M', 'G'.
	UINT32 Len;
	UINT16 ScenarioID;
	UINT8 CmdID;
	UINT8 Enable;
	UINT8 Reserved[3];
	UINT8 CheckSum;
} PQ_Bin_SubHeader_u;

typedef struct //PQTool cmd header
{
	UINT8 token[4];//4 // 4 = 0xfe, 'I', 'M', 'G'.
	UINT32 Len;
	UINT8 ScenarioSelect;
	UINT8 Reserved[7];
} PQ_Bin_MainHeader_u;

/*================================*/
/* TNR PQ Param. */
enum DRV_VPP_Path_e
{
    VPP_MAIN_PATH = 0,
    VPP_SUB_PATH,
    VPP_PATH_NUM
};

enum DRV_VPP_TNR_Clamp_Method_e
{
    TNR_ONE_THR_CLAMP = 0,
    TNR_TWO_THR_CLAMP,
};

enum DRV_VPP_TNR_Motion_Lut_Type_e
{
    TNR_MOTION_LUT_STEP_TYPE = 0,
    TNR_MOTION_LUT_LINEAR_TYPE,
};

enum DRV_VPP_TNR_YC_Motion_Proc_e
{
    TNR_YC_MOTION_MIXING_PROC = 0,
    TNR_YC_MOTION_SEPERATED_PROC,
};

enum DRV_VPP_TNR_YC_Diff_Cal_e
{
    TNR_YC_DIFF_AVERAGE_CAL = 0,
    TNR_YC_DIFF_MAX_CAL,
};

typedef struct
{
    UINT8 tnr_en;
    UINT8 clamp_method; // ref. DRV_VPP_TNR_Clamp_Method_e
    UINT8 motion_lut_type; // ref. DRV_VPP_TNR_Motion_Lut_Type_e
    UINT8 yc_motion_process; // ref. DRV_VPP_TNR_YC_Motion_Proc_e
    UINT8 yc_diff_cal; // ref. ref. DRV_VPP_TNR_YC_Diff_Cal_e
    UINT8 chroma_weight;
    UINT8 rsvd1[2];
    UINT8 y_output_clamp_thr[2];
    UINT8 c_output_clamp_thr[2];
    UINT8 y_motion_threshold[5];
    UINT8 y_blending_factor[5];
    UINT8 rsvd2[2];
    UINT8 c_motion_threshold[5];
    UINT8 c_blending_factor[5];
    UINT8 rsvd3[2];
} DRV_VPP_TNR_Param_u;
/*================================*/

typedef struct
{
	// 1
	PQ_Bin_SubHeader_u PeakingHeader_Video;
	DRV_VPP_Peaking_Info_u PeakingInfo_Video;

	// 2
	PQ_Bin_SubHeader_u PeakingHeader_UI;
	DRV_VPP_Peaking_Info_u PeakingInfo_UI;

	// 3
	PQ_Bin_SubHeader_u TransientHeader_Video;
	DRV_VPP_Transient_Info_u TransientInfo_Video;

	// 4
	PQ_Bin_SubHeader_u TransientHeader_UI;
	DRV_VPP_Transient_Info_u TransientInfo_UI;

	// 5
	PQ_Bin_SubHeader_u DeOverShootHeader_Video;
	DRV_VPP_DeOvershoot_u DeOverShootInfo_Video;

	// 6
	PQ_Bin_SubHeader_u DeOverShootHeader_UI;
	DRV_VPP_DeOvershoot_u DeOverShootInfo_UI;

	// 7
	PQ_Bin_SubHeader_u LumaHueHeader;
	DRV_Dmix_LumaHue_u LumaHueInfo;
#if 0
	// 8
	PQ_Bin_SubHeader_u GammaHeader;
	UBOOT_GAMMA_T GammaInfo;
#else
	PQ_Bin_SubHeader_u    GammaHeader;
	TCON_PAT_GAMMA_t	GammaInfo;
#endif
	// 9
	PQ_Bin_SubHeader_u FavHeaderVideo;
	DRV_VPP_Fav_Bin_Package_u favoriteVideo;

	// 10
	PQ_Bin_SubHeader_u FavHeaderUI;
	DRV_VPP_Fav_Bin_Package_u favoriteUI;

	// 11
	PQ_Bin_SubHeader_u LumaVideoHeader;
	VPP_Luma_Adj_u LumaVideo;

	// 12
	PQ_Bin_SubHeader_u DCTI_Header_Video;
	DRV_VPP_DCTI_Info_t DCTI_Info_Video;
	// 13
	PQ_Bin_SubHeader_u DCTI_Header_UI;
	DRV_VPP_DCTI_Info_t DCTI_Info_UI;
	// 14
	PQ_Bin_SubHeader_u NR_Header;
	DRV_VPP_NR_User_t NR_Info;

	PQ_Bin_SubHeader_u TNR_Header;
	DRV_VPP_TNR_Param_u TNR_Info;

} PQ_Bin_BinPackageClient_u;

typedef struct _IMG_REG_
{
	volatile unsigned int img_config;				// 0
	volatile unsigned int img_pkti_ctrl;				// 1
	volatile unsigned int img_pkti_bp1_coef_1;
	volatile unsigned int img_pkti_bp1_coef_2;
	volatile unsigned int img_pkti_bp1_coef_3;
	struct {
		volatile unsigned int pkti_flat_th : 8;
		volatile unsigned int pkti_lpk_gain_neg : 4;
		volatile unsigned int	: 20;
	} img_pkti_lpk_config;		// 5
	struct {
		volatile unsigned int pkti_lpk_gain_ep : 4;
		volatile unsigned int pkti_lpk_gain_bp : 4;
		volatile unsigned int pkti_lpk_gain_hp : 4;
		volatile unsigned int 	: 20;
	} img_pkti_lpk_gain;
	struct {
		volatile unsigned int pkti_lpk_lut_mp1 : 12;
		volatile unsigned int	: 20;
	} img_pkti_lpk_lut_1;				// 7
	struct {
		volatile unsigned int pkti_lpk_lut_mp2 : 12;
		volatile unsigned int	: 20;
	} img_pkti_lpk_lut_2;
	struct {
		volatile unsigned int pkti_lpk_lut_xp1 : 8;
		volatile unsigned int pkti_lpk_lut_xp0 : 8;
		volatile unsigned int	: 16;
	} img_pkti_lpk_lut_3;
	struct {
		volatile unsigned int pkti_lpk_lut_xp3 : 8;
		volatile unsigned int pkti_lpk_lut_xp2 : 8;
		volatile unsigned int	: 16;
	} img_pkti_lpk_lut_4;
	struct {
		volatile unsigned int pkti_lpk_lut_yp1 : 8;
		volatile unsigned int pkti_lpk_lut_yp0 : 8;
		volatile unsigned int	: 16;
	} img_pkti_lpk_lut_5;
	struct {
		volatile unsigned int pkti_lpk_coring : 8;
		volatile unsigned int pkti_lpk_lut_yp2 : 8;
		volatile unsigned int	: 16;
	} img_pkti_lpk_lut_6;
	struct {
		volatile unsigned int pkti_lpk_ovrsht_gain_min : 3;
		volatile unsigned int 	: 1;
		volatile unsigned int pkti_lpk_ovrsht_gain_max : 3;
		volatile unsigned int 	: 1;
		volatile unsigned int pkti_lpk_hi_coring : 7;
		volatile unsigned int	: 17;
	} img_pkti_lpk_ovsht_1;		// 13
	struct {
		volatile unsigned int pkti_lpk_over_min : 8;
		volatile unsigned int pkti_lpk_over_max : 8;
		volatile unsigned int	: 16;
	} img_pkti_lpk_ovsht_2;
	struct {
		volatile unsigned int pkti_lti_gain_ep : 4;
		volatile unsigned int pkti_lti_gain_bp : 4;
		volatile unsigned int pkti_lti_gain_hp : 4;
		volatile unsigned int	: 20;
	} img_pkti_lti_gain;
	struct {
		volatile unsigned int pkti_lti_lut_mp1 : 12;
		volatile unsigned int	: 20;
	} img_pkti_lti_lut_1;			// 16
	struct {
		volatile unsigned int pkti_lti_lut_mp2 : 12;
		volatile unsigned int	: 20;
	}img_pkti_lti_lut_2;
	struct {
		volatile unsigned int pkti_lti_lut_xp1 : 8;
		volatile unsigned int pkti_lti_lut_xp0 : 8;
		volatile unsigned int	: 16;
	} img_pkti_lti_lut_3;
	struct {
		volatile unsigned int pkti_lti_lut_xp3 : 8;
		volatile unsigned int pkti_lti_lut_xp2 : 8;
		volatile unsigned int	: 16;
	} img_pkti_lti_lut_4;
	struct {
		volatile unsigned int pkti_lti_lut_yp1 : 8;
		volatile unsigned int pkti_lti_lut_yp0 : 8;
		volatile unsigned int	: 16;
	} img_pkti_lti_lut_5;
	struct {
		volatile unsigned int pkti_lti_coring : 8;
		volatile unsigned int pkti_lti_lut_yp2 : 8;
		volatile unsigned int	: 16;
	} img_pkti_lti_lut_6;
	struct {
		volatile unsigned int pkti_lti_ovrsht_gain_min : 3;
		volatile unsigned int reserved1 : 1;
		volatile unsigned int pkti_lti_ovrsht_gain_max : 3;
		volatile unsigned int reserved2 : 1;
		volatile unsigned int pkti_lti_hi_coring : 7;
		volatile unsigned int	: 17;
	} img_pkti_lti_ovsht_1;		// 22
	struct {
		volatile unsigned int pkti_lti_over_min : 8;
		volatile unsigned int pkti_lti_over_max : 8;
		volatile unsigned int	: 16;
	} img_pkti_lti_ovsht_2;
	volatile unsigned int img_sram_ctrl;
	volatile unsigned int img_sram_addr;
	volatile unsigned int img_sram_write_data;
	volatile unsigned int img_sram_read_data;
	volatile unsigned int img_pix_no;				// 28
	volatile unsigned int img_line_no;
	volatile unsigned int img_checksum_ans;
	volatile unsigned int img_reserved31;			// 31

} IMG_REG;

typedef struct _BRI_REG_
{
	//BRI. G305 G306
	volatile unsigned int img_bri_en;
	volatile unsigned int img_bri_sram_ctrl;
	volatile unsigned int img_bri_sram_addr;
	volatile unsigned int img_bri_sram_write_data;
	volatile unsigned int img_bri_sram_read_data;
	volatile unsigned int img_bri_latch_en;
} BRI_REG;

typedef struct _DCTI_REG_
{
	//Group. G298 width = 16 ???
	struct
	{
		volatile unsigned int LATCH_EN : 1; //Kernel space
		volatile unsigned int DCTI_EN : 1; //User modify
		volatile unsigned int DCTI_BW_CAL_EN : 1; //Default closed
		volatile unsigned int DCTI_SMOOTH_EN : 1; //User modify
		volatile unsigned int DCTI_PRE_LP : 4; //User modify
		volatile unsigned int DCTI_WEIGHTX4 : 3; //User modify
		volatile unsigned int DCTI_Reserved : 1; //Reserved
		volatile unsigned int DCTI_BND_LEN : 3; // User modify
		volatile unsigned int	: 17;
	} dcti_config1; // 0
	struct
	{
		volatile unsigned int DCTI_MAX_MIN_METHOD : 1;
		volatile unsigned int DCTI_STFILTER_NEW : 1;
		volatile unsigned int DCTI_SLA0_MAS1 : 1;
		volatile unsigned int DCTI_CHKSUM_EN : 1;
		volatile unsigned int DCTI_SEL_BPFILTER : 2;
		volatile unsigned int	: 26;
	} dcti_config2;
	volatile unsigned int dcti_noise;
	volatile unsigned int dcti_gain_weight; // 3
	volatile unsigned int dcti_gain_th;
	volatile unsigned int dcti_bw_th1;
	volatile unsigned int dcti_bw_th2;
	volatile unsigned int dcti_bw_th3; // 7
	volatile unsigned int dcti_h_region;
	volatile unsigned int dcti_v_region;
	volatile unsigned int dcti_colorbar;
	volatile unsigned int dcti_h_size;
	volatile unsigned int dcti_v_size; // 12
	volatile unsigned int reverse[3];		//13, 14, 15
	volatile unsigned int dcti_cb_bw_cal; // 16
	volatile unsigned int dcti_cb_bw_m;
	volatile unsigned int dcti_cb_bw_m_p;
	volatile unsigned int dcti_cr_bw_cal;
	volatile unsigned int dcti_cr_bw_m;
	volatile unsigned int dcti_cr_bw_m_p;// 21
	volatile unsigned int dcti_chksum_out;
} DCTI_REG;
