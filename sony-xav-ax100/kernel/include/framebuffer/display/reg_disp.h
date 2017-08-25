#ifndef __REG_DISP_H__
#define __REG_DISP_H__
#include <linux/module.h>
#include <framebuffer/display/hardware.h>
//#include <asm/sysconfig.h>
#ifdef SYSCONFIG_ARCH_letb8700
#include <mach/hal/hal_letb123_tcon.h>
#endif

/**************************************************************************
 *                           C O N S T A N T S                            *
 **************************************************************************/
#ifndef FPGA_7045
#define CLK_ON_OFF   // Clock setting by Clk_mgr, for enable/disable/setting rate
#endif

/**************************************************************************
 *                          D A T A    T Y P E S                          *
 **************************************************************************/
// Display OSD0(G214), OSD1(G215)
typedef	struct _DISP_OSD_REG_
{
	volatile unsigned int osd_ctrl;					// 00
	volatile unsigned int osd_en;					// 01
	volatile unsigned int osd_tlink_addr_l;			// 02
	volatile unsigned int osd_tlink_addr_h;			// 03
	volatile unsigned int osd_reserved0[4];			// 04-07
	volatile unsigned int osd_req_ctrl;				// 08
	volatile unsigned int osd_deflicker_ctrl;		// 09
	volatile unsigned int osd_reserved1[4];			// 10-13
	volatile unsigned int osd_crc_chksum;			// 14
	volatile unsigned int osd_status;				// 15
	volatile unsigned int osd_hvld_offset;			// 16
	volatile unsigned int osd_hvld_width;			// 17
	volatile unsigned int osd_vvld_offset;			// 18
	volatile unsigned int osd_vvld_height;			// 19
	volatile unsigned int osd_data_fetch_ctrl;		// 20
	volatile unsigned int osd_bist_ctrl;				// 21
	volatile unsigned int osd_reserved2[5];			// 22-26
	volatile unsigned int osd_3d_h_offset;			// 27
	volatile unsigned int osd_reserved3;			// 28
	volatile unsigned int osd_src_decimation_sel;	// 29
	volatile unsigned int osd_reserved4[2];			// 30-31
} DISP_OSD_REG;

// Display GSCL0(G216,G217), GSCL1(G218,G219), GSCL2(G220,G221)
typedef	struct _DISP_GSCL_REG_
{
	volatile unsigned int gscl_config1;				// 00
	volatile unsigned int gscl_config2;				// 01
	volatile unsigned int gscl_reserved0[2];		// 02-03
											
	volatile unsigned int gscl_actrl_i_xlen;			// 04
	volatile unsigned int gscl_actrl_i_ylen;			// 05
	volatile unsigned int gscl_actrl_s_xstart;		// 06
	volatile unsigned int gscl_actrl_s_ystart;		// 07
	volatile unsigned int gscl_actrl_s_xlen;			// 08
	volatile unsigned int gscl_actrl_s_ylen;			// 09
											
	volatile unsigned int gscl_dctrl_o_xlen;			// 10
	volatile unsigned int gscl_dctrl_o_ylen;			// 11
	volatile unsigned int gscl_dctrl_d_xstart;		// 12
	volatile unsigned int gscl_dctrl_d_ystart;		// 13
	volatile unsigned int gscl_dctrl_d_xlen;			// 14
	volatile unsigned int gscl_dctrl_d_ylen;			// 15
	volatile unsigned int gscl_dctrl_bgc_c;			// 16
	volatile unsigned int gscl_dctrl_bgc_y;			// 17
	volatile unsigned int gscl_reserved1;			// 18
											
	volatile unsigned int gscl_hint_ctrl;				// 19
	volatile unsigned int gscl_hint_hfactor_low;		// 20
	volatile unsigned int gscl_hint_hfactor_high;	// 21
	volatile unsigned int gscl_hint_initf_low;		// 22
	volatile unsigned int gscl_hint_initf_high;		// 23
	volatile unsigned int gscl_hint_overshoot;		// 24
	volatile unsigned int gscl_reserved2;			// 25
											
	volatile unsigned int gscl_vint_ctrl;				// 26
	volatile unsigned int gscl_vint_vfactor_low;		// 27
	volatile unsigned int gscl_vint_vfactor_high;	// 28
	volatile unsigned int gscl_vint_initf_low;		// 29
	volatile unsigned int gscl_vint_initf_high;		// 30
	volatile unsigned int gscl_reserved3;			// 31

	//-----------------------------------------------------											
	volatile unsigned int gscl_vimg_ctrl;			// 00
	volatile unsigned int gscl_vimg_vpeak_lut_1;	// 01
	volatile unsigned int gscl_vimg_vpeak_lut_2;	// 02
	volatile unsigned int gscl_vimg_vpeak_lut_3;	// 03
	volatile unsigned int gscl_vimg_vpeak_lut_4;	// 04
	volatile unsigned int gscl_vimg_vpeak_lut_5;	// 05
	volatile unsigned int gscl_vimg_vpeak_lut_6;	// 06
	volatile unsigned int gscl_vimg_vpeak_config;	// 07
	volatile unsigned int gscl_vimg_vpeak_config_1;	// 08
	volatile unsigned int gscl_vimg_vpeak_config_2;	// 09
	volatile unsigned int gscl_reserved4;			// 10

	volatile unsigned int gscl_checksum_select;	// 11
	volatile unsigned int gscl_checksum_result;		// 12
	volatile unsigned int gscl_reserved5;			// 13
	volatile unsigned int gscl_debug_info;			// 14
	volatile unsigned int gscl_reserved6;			// 15

	volatile unsigned int gscl_dr_ctrl;				// 16
	volatile unsigned int gscl_dr_dege_detect1;		// 17
	volatile unsigned int gscl_dr_dege_detect2;		// 18
	volatile unsigned int gscl_dr_clamp_detect;		// 19
	volatile unsigned int gscl_reserved7[12];		// 20-31
} DISP_GSCL_REG;

// Display HTDS0(G222),	HTDS1(G223), HTDS2(G224)
typedef	struct _DISP_HTDS_REG_
{
	volatile unsigned int htds_latch_mode;			// 00
	volatile unsigned int htds_config;				// 01
	volatile unsigned int htds_line_width;			// 02
	volatile unsigned int htds_pix_st_r0;			// 03
	volatile unsigned int htds_pix_ed_r0;			// 04
	volatile unsigned int htds_shift_r0;				// 05
	volatile unsigned int htds_pix_st_r1;			// 06
	volatile unsigned int htds_pix_ed_r1;			// 07
	volatile unsigned int htds_shift_r1;				// 08
	volatile unsigned int htds_pix_st_r2;			// 09
	volatile unsigned int htds_pix_ed_r2;			// 10
	volatile unsigned int htds_shift_r2;				// 11
	volatile unsigned int htds_pix_st_r3;			// 12
	volatile unsigned int htds_pix_ed_r3;			// 13
	volatile unsigned int htds_shift_r3;				// 14
	volatile unsigned int htds_checksum;			// 15
	volatile unsigned int htds_reserved[16];		// 16-31
} DISP_HTDS_REG;

typedef struct _DISP_GRAPHIC_REG_
{
	// Display Graphic
	//G2D(G213), OSD0(G214), OSD1(G215)
	DISP_OSD_REG osd[3];	

	//GSCL0(G216,G217), GSCL1(G218,G219), GSCL2(G220,G221)
	DISP_GSCL_REG gscl[3];
#if 0
	//HTDS0(G222), HTDS1(G223), HTDS2(G224)  
	DISP_HTDS_REG htds[3];
#endif
} DISP_GRAPHIC_REG;

typedef struct _DISP_TGEN_REG_ // TGEN0(G203), TGEN1(G204)
{
    // Group 203 : TGEN
    	volatile unsigned int   tgen_0_config                     ; // 00
    	volatile unsigned int   tgen_0_reset                      ; // 01
    	volatile unsigned int   tgen_0_user_int1_config           ; // 02
    	volatile unsigned int   g203_reserved_0                   ; // 03
    	volatile unsigned int   tgen_dtg_0_config                 ; // 04
    	volatile unsigned int   g203_reserved_1                   ; // 05
    	volatile unsigned int   tgen_dtg_0_adjust2                ; // 06
    	volatile unsigned int   g203_reserved_2                   ; // 07
    	volatile unsigned int   tgen_dtg_0_total_pixel            ; // 08
    	volatile unsigned int   tgen_dtg_0_ds_line_start_cd_point ; // 09
    	volatile unsigned int   tgen_dtg_0_total_line             ; // 10
    	volatile unsigned int   tgen_dtg_0_field_end_line         ; // 11
    	volatile unsigned int   tgen_dtg_0_tve_line_rst_cnt       ; // 12
    	volatile unsigned int   tgen_dtg_0_status1                ; // 13
    	volatile unsigned int   tgen_dtg_0_status2                ; // 14
    	volatile unsigned int   tgen_dtg_0_start_line             ; // 15
    	volatile unsigned int   tgen_atg_0_config                 ; // 16
    	volatile unsigned int   tgen_atg_0_clk_ratio_low          ; // 17
    	volatile unsigned int   tgen_atg_0_clk_ratio_high         ; // 18
    	volatile unsigned int   tgen_atg_0_total_pixel            ; // 19
    	volatile unsigned int   tgen_atg_0_first_line_total_pixel ; // 20
    	volatile unsigned int   tgen_atg_0_field_end_line         ; // 21
    	volatile unsigned int   tgen_atg_0_ds_line_start_cd_point ; // 22
    	volatile unsigned int   tgen_atg_0_adjust1                ; // 23
    	volatile unsigned int   tgen_atg_0_adjust2                ; // 24
    	volatile unsigned int   tgen_atg_0_adjust3                ; // 25
    	volatile unsigned int   tgen_atg_0_adjust4                ; // 26
    	volatile unsigned int   tgen_atg_0_status1                ; // 27
    	volatile unsigned int   tgen_atg_0_status2                ; // 28
    	volatile unsigned int   tgen_atg_0_source_sel             ; // 29
    	volatile unsigned int   g203_reserved3[2]                 ; // 30~31

    // Group 204 : TGEN
    	volatile unsigned int   tgen_1_config                     ; // 00
    	volatile unsigned int   tgen_1_reset                      ; // 01
    	volatile unsigned int   tgen_1_user_int1_config           ; // 02
    	volatile unsigned int   g204_reserved_0                   ; // 03
    	volatile unsigned int   tgen_dtg_1_config                 ; // 04
    	volatile unsigned int   g204_reserved_1                   ; // 05
    	volatile unsigned int   tgen_dtg_1_adjust2                ; // 06
    	volatile unsigned int   g204_reserved_2                   ; // 07
    	volatile unsigned int   tgen_dtg_1_total_pixel            ; // 08
    	volatile unsigned int   tgen_dtg_1_ds_line_start_cd_point ; // 09
    	volatile unsigned int   tgen_dtg_1_total_line             ; // 10
    	volatile unsigned int   tgen_dtg_1_field_end_line         ; // 11
    	volatile unsigned int   tgen_dtg_1_tve_line_rst_cnt       ; // 12
    	volatile unsigned int   tgen_dtg_1_status1                ; // 13
    	volatile unsigned int   tgen_dtg_1_status2                ; // 14
    	volatile unsigned int   tgen_dtg_1_start_line             ; // 15
    	volatile unsigned int   tgen_atg_1_config                 ; // 16
    	volatile unsigned int   tgen_atg_1_clk_ratio_low          ; // 17
    	volatile unsigned int   tgen_atg_1_clk_ratio_high         ; // 18
    	volatile unsigned int   tgen_atg_1_total_pixel            ; // 19
    	volatile unsigned int   tgen_atg_1_first_line_total_pixel ; // 20
    	volatile unsigned int   tgen_atg_1_field_end_line         ; // 21
    	volatile unsigned int   tgen_atg_1_ds_line_start_cd_point ; // 22
    	volatile unsigned int   tgen_atg_1_adjust1                ; // 23
    	volatile unsigned int   tgen_atg_1_adjust2                ; // 24
    	volatile unsigned int   tgen_atg_1_adjust3                ; // 25
    	volatile unsigned int   tgen_atg_1_adjust4                ; // 26
    	volatile unsigned int   tgen_atg_1_status1                ; // 27
    	volatile unsigned int   tgen_atg_1_status2                ; // 28
    	volatile unsigned int   tgen_atg_1_source_sel             ; // 29
    	volatile unsigned int   g204_reserved3[2]                 ; // 30~31
} DISP_TGEN_REG;

typedef struct _DISP_DMIX_REG_	// AMIX(G205,G206), DMIX(G207,G208)
{
    // Group 205 : AMIX
    volatile unsigned int  amix_0_config0                    ; // 00
    volatile unsigned int  amix_0_config1                    ; // 01
    volatile unsigned int  amix_0_config2                    ; // 02
    volatile unsigned int  amix_0_plane_alpha                ; // 03
    volatile unsigned int  amix_0_plane_alpha2               ; // 04
    volatile unsigned int  amix_0_plane_alpha3               ; // 05
    volatile unsigned int  g205_reserved0[8]                 ; // 06~13
    volatile unsigned int  amix_0_bgc_y                      ; // 14
    volatile unsigned int  amix_0_bgc_c                      ; // 15
    volatile unsigned int  g205_reserved1                    ; // 16
    volatile unsigned int  amix_0_ptg_config                 ; // 17
    volatile unsigned int  amix_0_ptg_config_2               ; // 18 
    volatile unsigned int  amix_0_ptg_config_3               ; // 19
    volatile unsigned int  amix_0_ptg_config_4               ; // 20 
    volatile unsigned int  amix_0_ptg_config_5               ; // 21
    volatile unsigned int  amix_0_ptg_config_6               ; // 22
    volatile unsigned int  g205_reserved2[9]                 ; // 23~31

    // Group 206 : AMIX
    volatile unsigned int  amix_1_config0                    ; // 00
    volatile unsigned int  amix_1_config1                    ; // 01
    volatile unsigned int  amix_1_config2                    ; // 02
    volatile unsigned int  amix_1_plane_alpha                ; // 03
    volatile unsigned int  amix_1_plane_alpha2               ; // 04
    volatile unsigned int  amix_1_plane_alpha3               ; // 05
    volatile unsigned int  g206_reserved0[8]                 ; // 06~13
    volatile unsigned int  amix_1_bgc_y                      ; // 14
    volatile unsigned int  amix_1_bgc_c                      ; // 15
    volatile unsigned int  g206_reserved1                    ; // 16
    volatile unsigned int  amix_1_ptg_config                 ; // 17
    volatile unsigned int  amix_1_ptg_config_2               ; // 18 
    volatile unsigned int  amix_1_ptg_config_3               ; // 19
    volatile unsigned int  amix_1_ptg_config_4               ; // 20 
    volatile unsigned int  amix_1_ptg_config_5               ; // 21
    volatile unsigned int  amix_1_ptg_config_6               ; // 22
    volatile unsigned int  g206_reserved2[9]                 ; // 23~31

    // Group 207 : DMIX
    volatile unsigned int  dmix_0_config                     ; // 00
    volatile unsigned int  g207_reserved0[3]                 ; // 01~3
    volatile unsigned int  dmix_0_yc_adjust                  ; // 04
    volatile unsigned int  dmix_0_luma_cp1                   ; // 05
    volatile unsigned int  dmix_0_luma_cp2                   ; // 06
    volatile unsigned int  dmix_0_luma_cp3                   ; // 07
    volatile unsigned int  dmix_0_luma_slope0                ; // 08
    volatile unsigned int  dmix_0_luma_slope1                ; // 09
    volatile unsigned int  dmix_0_luma_slope2                ; // 10
    volatile unsigned int  dmix_0_luma_slope3                ; // 11
    volatile unsigned int  dmix_0_chroma_satcos              ; // 12
    volatile unsigned int  dmix_0_chroma_satsin              ; // 13
    volatile unsigned int  dmix_0_checksum_setting           ; // 14
    volatile unsigned int  dmix_0_chksum_usermode_V          ; // 15
    volatile unsigned int  dmix_0_chksum_usermode_H          ; // 16
    volatile unsigned int  dmix_0_chksum_usermode_start_line ; // 17
    volatile unsigned int  dmix_0_to_venc_chksum             ; // 18
    volatile unsigned int  dmix_0_to_hd2sd_chksum            ; // 19
    volatile unsigned int  dmix_0_fgc_y_alpha                ; // 20
    volatile unsigned int  dmix_0_fgc_c                      ; // 21
    volatile unsigned int  dmix_0_pix_en_sel                 ; // 22
    volatile unsigned int  dmix_0_snr_set                    ; // 23
    volatile unsigned int  dmix_0_snr_start_set              ; // 24
    volatile unsigned int  g207_reserved1[7]                 ; // 25~31

    // Group 208 : DMIX
    volatile unsigned int  dmix_1_config                     ; // 00
    volatile unsigned int  g208_reserved0[3]                 ; // 01~3
    volatile unsigned int  dmix_1_yc_adjust                  ; // 04
    volatile unsigned int  dmix_1_luma_cp1                   ; // 05
    volatile unsigned int  dmix_1_luma_cp2                   ; // 06
    volatile unsigned int  dmix_1_luma_cp3                   ; // 07
    volatile unsigned int  dmix_1_luma_slope0                ; // 08
    volatile unsigned int  dmix_1_luma_slope1                ; // 09
    volatile unsigned int  dmix_1_luma_slope2                ; // 10
    volatile unsigned int  dmix_1_luma_slope3                ; // 11
    volatile unsigned int  dmix_1_chroma_satcos              ; // 12
    volatile unsigned int  dmix_1_chroma_satsin              ; // 13
    volatile unsigned int  dmix_1_checksum_setting           ; // 14
    volatile unsigned int  dmix_1_chksum_usermode_V          ; // 15
    volatile unsigned int  dmix_1_chksum_usermode_H          ; // 16
    volatile unsigned int  dmix_1_chksum_usermode_start_line ; // 17
    volatile unsigned int  dmix_1_to_venc_chksum             ; // 18
    volatile unsigned int  dmix_1_to_hd2sd_chksum            ; // 19
    volatile unsigned int  dmix_1_fgc_y_alpha                ; // 20
    volatile unsigned int  dmix_1_fgc_c                      ; // 21
    volatile unsigned int  dmix_1_pix_en_sel                 ; // 22
    volatile unsigned int  dmix_1_snr_set                    ; // 23
    volatile unsigned int  dmix_1_snr_start_set              ; // 24
    volatile unsigned int  g208_reserved1[7]                 ; // 25~31
} DISP_DMIX_REG;

typedef struct _DISP_VENC_REG_	//TVE , SDTV1(G209), SDTV2(G210), SDTV3(G211)
{
	//SDTV1(G209), Base : 0x40006880
	volatile unsigned int venc0_tv_mode[6];		// 0 ~ 5
	volatile unsigned int venc0_tv_subc_f[2];	// 6, 7
	volatile unsigned int venc0_tv_subc_p;		// 8
	volatile unsigned int venc0_tv_line_t[2];		// 9, 10
	volatile unsigned int venc0_tv_line_b[2];		// 11, 12
	volatile unsigned int venc0_tv_cc_t;			// 13
	volatile unsigned int venc0_tv_cc_b;			// 14
	volatile unsigned int venc0_tv_cgms[2];		// 15, 16
	volatile unsigned int venc0_tv_id_status;	// 17
	volatile unsigned int venc0_tv_dac;			// 18
	volatile unsigned int venc0_reserved19;		// 19
	volatile unsigned int venc0_tv_macro[9];		// 20~28
	volatile unsigned int venc0_reserved29[2];	// 29~30
	volatile unsigned int venc0_macro9;			// 31
	//SDTV2(G210), Base : 0x40006900
	volatile unsigned int venc1_reserved0[5];	// 0 ~ 4
	volatile unsigned int venc1_tv_pccon[19];	// 5 ~ 23
	volatile unsigned int venc1_tv_temp0_g_1;	// 24
	volatile unsigned int venc1_tv_temp1_g_1;	// 25
	volatile unsigned int venc1_tv_dac_checksum; //26
	volatile unsigned int venc1_tv_mode_5;		// 27
	volatile unsigned int venc1_tv_mode_6;		// 28
	volatile unsigned int venc1_tv_mode_7;		// 29
	volatile unsigned int venc1_reserved30[2];	// 30~31
	//SDTV3(G211), Base : 0x40006980
	volatile unsigned int venc2_tv2_mode_0;	// 0
	volatile unsigned int venc2_tv2_rg_scm_adj;	// 1
	volatile unsigned int venc2_luma_adj_cp1;    	// 2
	volatile unsigned int venc2_luma_adj_cp2;    	// 3
	volatile unsigned int venc2_luma_adj_cp3;	// 4
	volatile unsigned int venc2_reserved5[2];	// 5~6
	volatile unsigned int venc2_tv2_wc_tline;	// 7
	volatile unsigned int venc2_tv2_wc_bline;	// 8
	volatile unsigned int venc2_tv2_wc_psline;	// 9
	volatile unsigned int venc2_tv2_mode_1;	// 10
	volatile unsigned int venc2_tv2_mode_2;	// 11
	volatile unsigned int venc2_tv2_mode_3;	// 12
	volatile unsigned int venc2_tv2_y_range;	// 13
	volatile unsigned int venc2_tv2_c_range;	// 14
	volatile unsigned int venc2_tv2_cgmsb_0;	// 15
	volatile unsigned int venc2_tv2_cgmsb_1;	// 16
	volatile unsigned int venc2_tv2_cgmsb_2;	// 17
	volatile unsigned int venc2_tv2_cgmsb_3;	// 18
	volatile unsigned int venc2_tv2_vpd_mode_0; //19
	volatile unsigned int venc2_tv2_vpd_mode_1; // 20
	volatile unsigned int venc2_tv2_vpd_thd;	// 21
	volatile unsigned int venc2_tv2_vpd_stline;	// 22
	volatile unsigned int venc2_tv2_vpd_edline;	//23
	volatile unsigned int venc2_tv2_vpd_vldcnt;	//24
	volatile unsigned int venc2_tv2_vpd_mode_2; // 25
	volatile unsigned int venc2_reserved26;		//26
	volatile unsigned int venc2_tv_hd2sd_pnt;  	//27
	volatile unsigned int venc2_luma_adj_slope0; // 28
	volatile unsigned int venc2_luma_adj_slope1; // 29
	volatile unsigned int venc2_luma_adj_slope2; // 30
	volatile unsigned int venc2_luma_adj_slope3; // 31
} DISP_VENC_REG;

typedef struct _DISP_DVE_REG_		// DVE0 (G250,251), DVE1 (G252,253)
{
	//DVE0 (G250)
	volatile unsigned int dve0_reserved0[17];	// 0~16
	volatile unsigned int dve0_656_act_0_pix;	// 17
	volatile unsigned int dve0_656_line_0_length; // 18
	volatile unsigned int dve0_656_frame_0_line;// 19
	volatile unsigned int dve0_656_noact_end_tl;// 20
	volatile unsigned int dve0_656_act_end_tl;	// 21
	volatile unsigned int dve0_656_noact_end_bl;// 22
	volatile unsigned int dve0_656_act_end_bl;	// 23
	volatile unsigned int dve0_656_ctrl;			// 24
	volatile unsigned int dve0_656_y_range;		// 25
	volatile unsigned int dve0_656_c_range;		// 26
	volatile unsigned int dve0_reserved27[4];	// 27~30
	volatile unsigned int dve0_config0;			// 31 

	//DVE0 (G251)
	volatile unsigned int dve0_color_bar_mode;		// 0
	volatile unsigned int dve0_color_bar_V_total;		// 1
	volatile unsigned int dve0_color_bar_V_active;	// 2
	volatile unsigned int dve0_color_bar_V_active_start;	// 3
	volatile unsigned int dve0_color_bar_H_total;	// 4
	volatile unsigned int dve0_color_bar_H_active;	// 5
	volatile unsigned int dve0_color_bar_reserved[26];	// 6~31

	//DVE1 (G252)
	volatile unsigned int dve1_reserved0[17];	// 0~16
	volatile unsigned int dve1_656_act_0_pix;	// 17
	volatile unsigned int dve1_656_line_0_length; // 18
	volatile unsigned int dve1_656_frame_0_line;// 19
	volatile unsigned int dve1_656_noact_end_tl;// 20
	volatile unsigned int dve1_656_act_end_tl;	// 21
	volatile unsigned int dve1_656_noact_end_bl;// 22
	volatile unsigned int dve1_656_act_end_bl;	// 23
	volatile unsigned int dve1_656_ctrl;			// 24
	volatile unsigned int dve1_656_y_range;		// 25
	volatile unsigned int dve1_656_c_range;		// 26
	volatile unsigned int dve1_reserved27[4];	// 27~30
	volatile unsigned int dve1_config0;			// 31 

	//DVE0 (G253)
	volatile unsigned int dve1_color_bar_mode;		// 0
	volatile unsigned int dve1_color_bar_V_total;		// 1
	volatile unsigned int dve1_color_bar_V_active;	// 2
	volatile unsigned int dve1_color_bar_V_active_start;	// 3
	volatile unsigned int dve1_color_bar_H_total;	// 4
	volatile unsigned int dve1_color_bar_H_active;	// 5
	volatile unsigned int dve1_color_bar_reserved[26];	// 6~31
} DISP_DVE_REG;

typedef struct _DISP_HD2SD_REG_		//SUBPATH HD2SD : G201
{
	volatile unsigned int hd2sd_config;				// 00
	volatile unsigned int hd2sd_user_hexp0;		// 01
	volatile unsigned int hd2sd_user_hexp1;		// 02
	volatile unsigned int hd2sd_user_vexp0;		// 03
	volatile unsigned int hd2sd_user_vexp1;		// 04
	volatile unsigned int hd2sd_user_vexp2;		// 05
	volatile unsigned int hd2sd_user_sd_reset0;	// 06
	volatile unsigned int hd2sd_user_sd_reset1;	// 07
	volatile unsigned int hd2sd_status;				// 08
	volatile unsigned int hd2sd_peaking_config;	// 09
	volatile unsigned int hd2sd_peaking_lut0;		// 10
	volatile unsigned int hd2sd_peaking_lut1;		// 11
	volatile unsigned int hd2sd_peaking_limit;		// 12
	volatile unsigned int hd2sd_hd_checksum;		// 13
	volatile unsigned int hd2sd_sd_checksum;		// 14
	volatile unsigned int hd2sd_user_sdtve_vrst0;	// 15
	volatile unsigned int hd2sd_user_sdtve_vrst1;	// 16
	volatile unsigned int hd2sd_user_sdtve_hrst;	// 17
	volatile unsigned int hd2sd_bgc_y;				// 18
	volatile unsigned int hd2sd_bgc_c;				// 19
	volatile unsigned int hd2sd_config2;			// 20
	volatile unsigned int hd2sd_user_hact;			// 21
	volatile unsigned int hd2sd_user_vact;			// 22
	volatile unsigned int hd2sd_tve_user_mode;	// 23
	volatile unsigned int hd2sd_reserved[8];		// 24-31
} DISP_HD2SD_REG;

typedef struct _DISP_AFRC_REG_		//SUBPATH AFRC : G202
{
	volatile unsigned int afrc_mode0;				// 00
	volatile unsigned int afrc_fb_ctrl0;				// 01
	volatile unsigned int afrc_fb_ctrl1;				// 02
	volatile unsigned int afrc_bist_mode0;			// 03
	volatile unsigned int afrc_bist_mode1;			// 04
	volatile unsigned int afrc_urgent_thd;			// 05
	volatile unsigned int afrc_iw_req_clkcnt;		// 06
	volatile unsigned int afrc_faddr0_lw;			// 07
	volatile unsigned int afrc_faddr0_hw;			// 08
	volatile unsigned int afrc_fb_step;				// 09
	volatile unsigned int afrc_tgr_hact;				// 10
	volatile unsigned int afrc_tgr_htot;				// 11
	volatile unsigned int afrc_tgr_vact_st_t;		// 12
	volatile unsigned int afrc_tgr_vact_ed_t;		// 13
	volatile unsigned int afrc_tgr_vact_st_b;		// 14
	volatile unsigned int afrc_tgr_vact_ed_b;		// 15
	volatile unsigned int afrc_tgr_vtot;				// 16
	volatile unsigned int afrc_sync_vcnt;			// 17
	volatile unsigned int afrc_sync_hcnt;			// 18
	volatile unsigned int afrc_out_vact_st_t;		// 19
	volatile unsigned int afrc_out_vact_st_b;		// 20
	volatile unsigned int afrc_irq_line;				// 21
	volatile unsigned int afrc_debug;				// 22
	volatile unsigned int afrc_chkout_a;			// 23
	volatile unsigned int afrc_chkout_b;			// 24
	volatile unsigned int afrc_iw_req2gnt;			// 25
	volatile unsigned int afrc_iw_gnt2req;			// 26
	volatile unsigned int afrc_ir_req2gnt;			// 27
	volatile unsigned int afrc_ir_gnt2req;			// 28
	volatile unsigned int afrc_mode1;				// 29
	volatile unsigned int afrc_mode2;				// 30
	volatile unsigned int afrc_reserved;			// 31
} DISP_AFRC_REG;

/*-------------------------
|    VPP   REGISTER GROUPS          |
|    (G185~G200)                         |
---------------------------*/

typedef struct _DISP_DDFCH_REG_		//VPP - DDFCH0 : G185, DDFCH1 : G186
{
	volatile unsigned int ddfch_latch_en;			// 0
	volatile unsigned int ddfch_mode_option;
	volatile unsigned int ddfch_enable;
	volatile unsigned int ddfch_urgent_thd;
	volatile unsigned int ddfch_cmdq_thd;
	volatile unsigned int ddfch_mr_base_addr;
	volatile unsigned int ddfch_luma_base_addr_0; 	// 6
	volatile unsigned int ddfch_luma_base_addr_1;
	volatile unsigned int ddfch_luma_base_addr_2;
	volatile unsigned int ddfch_crma_base_addr_0; 	// 9
	volatile unsigned int ddfch_crma_base_addr_1;
	volatile unsigned int ddfch_crma_base_addr_2;
	volatile unsigned int ddfch_indx_base_addr_0;  	// 12
	volatile unsigned int ddfch_indx_base_addr_1;
	volatile unsigned int ddfch_indx_base_addr_2;
	volatile unsigned int ddfch_pip_mask_size;		// 15
	volatile unsigned int ddfch_pip_mask_offset;
	volatile unsigned int ddfch_mr_frame_size;
	volatile unsigned int ddfch_mr_crop_size;
	volatile unsigned int ddfch_mr_crop_offset;
	volatile unsigned int ddfch_vdo_frame_size;		// 20
	volatile unsigned int ddfch_vdo_crop_size;
	volatile unsigned int ddfch_vdo_crop_offset;
	volatile unsigned int ddfch_config_0;			// 23
	volatile unsigned int ddfch_config_1;			// 24
	volatile unsigned int ddfch_checksum_sel;
	volatile unsigned int ddfch_checksum_info;
	volatile unsigned int ddfch_error_flag_info;		// 27
	volatile unsigned int ddfch_bist;
	volatile unsigned int ddfch_mbus_info_pre;		// 29
	volatile unsigned int ddfch_mbus_info_nxt;
	volatile unsigned int ddfch_mbus_info_cur;

} DISP_DDFCH_REG;

typedef struct _DISP_DEINT_REG_		// VPP - DEINT0 : G187, G188, DEINT1 : G189, G190
{
	volatile unsigned int deint_param_1;			// 0
	volatile unsigned int deint_cnt_y_his_r;
	volatile unsigned int deint_gclk_param_1;
	volatile unsigned int deint_gclk_param_2;
	volatile unsigned int deint_checksum;
	volatile unsigned int deint_cnt_y_pre;			// 5
	volatile unsigned int deint_cnt_y_cur;
	volatile unsigned int deint_cnt_y_nxt;
	volatile unsigned int deint_scene_change_param_1;	// 8
	volatile unsigned int deint_scene_change_param_2;
	volatile unsigned int deint_cnt_y_his_w;
	volatile unsigned int deint_pulldown22_param_1;		// 11
	volatile unsigned int deint_pulldown22_param_2;
	volatile unsigned int deint_pulldown22_param_3;
	volatile unsigned int deint_pulldown22_param_4;
	volatile unsigned int deint_pulldown22_param_5;		// 15
	volatile unsigned int deint_pulldown22_param_6;
	volatile unsigned int deint_pulldown22_param_7;
	volatile unsigned int deint_pulldown22_param_8;
	volatile unsigned int deint_x_size;					// 19
	volatile unsigned int deint_pulldown32_param_1;		// 20
	volatile unsigned int deint_pulldown32_param_2;
	volatile unsigned int deint_pulldown32_param_3;
	volatile unsigned int deint_pulldown32_param_4;
	volatile unsigned int deint_pulldown32_param_5;
	volatile unsigned int deint_pulldown32_param_6;
	volatile unsigned int deint_pulldown32_param_7;
	volatile unsigned int deint_pulldown32_param_8;
	volatile unsigned int deint_y_size;					// 28
	volatile unsigned int deint_bad_edit_param_1;		// 29
	volatile unsigned int deint_bad_edit_param_2;
	volatile unsigned int deint_bad_edit_param_3;

	volatile unsigned int deint_decision_param_1;		// 0
	volatile unsigned int deint_decision_param_2;
	volatile unsigned int deint_decision_param_3;
	volatile unsigned int deint_decision_param_4;
	volatile unsigned int deint_decision_param_5;
	volatile unsigned int deint_decision_param_6;
	volatile unsigned int deint_decision_param_7;
	volatile unsigned int deint_decision_param_8;
	volatile unsigned int deint_decision_param_9;
	volatile unsigned int deint_decision_param_a;
	volatile unsigned int deint_decision_param_b;
	volatile unsigned int deint_val_field_luma_l;			// 11
	volatile unsigned int deint_val_field_luma_h;
	volatile unsigned int deint_intra_param_1;			// 13
	volatile unsigned int deint_intra_param_2;
	volatile unsigned int deint_intra_param_3;
	volatile unsigned int deint_intra_param_4;
	volatile unsigned int deint_intra_param_5;
	volatile unsigned int deint_intra_param_6;
	volatile unsigned int deint_intra_param_7;
	volatile unsigned int deint_intra_param_8;			// 20
	volatile unsigned int deint_read_cnt_y;
	volatile unsigned int deint_inter_param_1;
	volatile unsigned int deint_mixing_param_1;
	volatile unsigned int deint_val_22_sawtooth;			// 24
	volatile unsigned int deint_val_32_motion;
	volatile unsigned int deint_val_be_tmp;				// 26
	volatile unsigned int deint_val_be_spt;
	volatile unsigned int deint_val_be_mix;
	volatile unsigned int deint_val_be_info1;
	volatile unsigned int deint_val_be_info2;
	volatile unsigned int deint_val_scene_change;

} DISP_DEINT_REG;

typedef struct _DISP_VSCL_REG_
{
	volatile unsigned int vscl_config;				// 0
	volatile unsigned int vscl_config2;
	volatile unsigned int vscl_reserved2;			// 2
	volatile unsigned int vscl_actrl_i_xlen;			// 3
	volatile unsigned int vscl_actrl_i_ylen;
	volatile unsigned int vscl_actrl_s_xstart;
	volatile unsigned int vscl_actrl_s_ystart;
	volatile unsigned int vscl_actrl_s_xlen;
	volatile unsigned int vscl_actrl_s_ylen;
	volatile unsigned int vscl_dctrl_o_xlen;			// 9
	volatile unsigned int vscl_dctrl_o_ylen;
	volatile unsigned int vscl_dctrl_d_xstart;
	volatile unsigned int vscl_dctrl_d_ystart;
	volatile unsigned int vscl_dctrl_d_xlen;
	volatile unsigned int vscl_dctrl_d_ylen;
	volatile unsigned int vscl_dctrl_bgc_c;			// 15
	volatile unsigned int vscl_dctrl_bgc_y;
	volatile unsigned int vscl_reserved17;			// 17
	volatile unsigned int vscl_hint_ctrl;
	volatile unsigned int vscl_hint_hfactor_low;
	volatile unsigned int vscl_hint_hfactor_high;
	volatile unsigned int vscl_hint_initf_low;
	volatile unsigned int vscl_hint_initf_high;
	//volatile unsigned int vscl_reserved23_0[2];		// 23, 24
	volatile unsigned int vscl_hint_c_initf_low;		// 23
	volatile unsigned int vscl_hint_c_initf_high;		// 24
	volatile unsigned int vscl_hint_overshoot;
	volatile unsigned int vscl_reserved26[2];		// 26, 27
	volatile unsigned int vscl_sram_ctrl;
	volatile unsigned int vscl_sram_addr;
	volatile unsigned int vscl_sram_write_data;
	volatile unsigned int vscl_sram_read_data;

	volatile unsigned int vscl_vint_ctrl;				// 0
	volatile unsigned int vscl_vint_vfactor_low;
	volatile unsigned int vscl_vint_vfactor_high;
	volatile unsigned int vscl_vint_initf_low;
	volatile unsigned int vscl_vint_initf_high;
	volatile unsigned int vscl_dering_ctrl;			// 5
	volatile unsigned int vscl_dering_edge_detect_1;
	volatile unsigned int vscl_dering_edge_detect_2;
	volatile unsigned int vscl_dering_clamp;
	volatile unsigned int vscl_vimg_ctrl;			// 9
	volatile unsigned int vscl_vimg_vpeak_lut_1;		// 10
	volatile unsigned int vscl_vimg_vpeak_lut_2;
	volatile unsigned int vscl_vimg_vpeak_lut_3;
	volatile unsigned int vscl_vimg_vpeak_lut_4;
	volatile unsigned int vscl_vimg_vpeak_lut_5;
	volatile unsigned int vscl_vimg_vpeak_lut_6;		// 15
	volatile unsigned int vscl_vimg_vpeak_config;
	volatile unsigned int vscl_vimg_diag_config_1;
	volatile unsigned int vscl_vimg_diag_config_2;
	volatile unsigned int vscl_reserved19[2];		// 19, 20,
	volatile unsigned int vscl_checksum_select;		// 21
	volatile unsigned int vscl_checksum_result;			// 22
	volatile unsigned int vscl_reserved23[9];		// 23 ~ 31

} DISP_VSCL_REG;

typedef struct _DISP_NR_REG_
{
	volatile unsigned int nr_en;					// 0
	volatile unsigned int nr_deovs_win;
	volatile unsigned int nr_deovs_cor_th;
	volatile unsigned int nr_deovs_ring_edge_th;
	volatile unsigned int nr_deovs_ring_l01;
	volatile unsigned int nr_deovs_ring_l23;
	volatile unsigned int nr_deovs_fn_l0123;
	volatile unsigned int nr_dr_ctrl;
	volatile unsigned int nr_dr_edge_det_1;		// 8
	volatile unsigned int nr_dr_edge_det_2;		// 9
	volatile unsigned int nr_dr_clamp_det;
	volatile unsigned int nr_dm_edge_det_1;		// 11
	volatile unsigned int nr_dm_edge_det_2;
	volatile unsigned int nr_dm_clamp_det_1;
	volatile unsigned int nr_dm_clamp_det_2;
	volatile unsigned int nr_snr_ctrl;				// 15
	volatile unsigned int nr_snr_edge_det_1;
	volatile unsigned int nr_snr_edge_det_2;
	volatile unsigned int nr_snr_edge_det_3;
	volatile unsigned int nr_snr_clamp_value;
	volatile unsigned int nr_snr_masnr_factor_1;
	volatile unsigned int nr_snr_masnr_factor_2;
	volatile unsigned int nr_pix_no;				// 22
	volatile unsigned int nr_line_no;
	volatile unsigned int nr_gated_ctrl;
	volatile unsigned int nr_checksum_en;
	volatile unsigned int nr_checksum_ans;			// 26
	volatile unsigned int nr_reserved27[5];			// 27~31

} DISP_NR_REG;

typedef struct _DISP_IMG_REG_
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

} DISP_IMG_REG;

typedef struct _DISP_VPOST_REG_
{
	volatile unsigned int vpost_config;
	volatile unsigned int vpost_adj_config;
	volatile unsigned int vpost_adj_src;
	volatile unsigned int vpost_adj_des;
	volatile unsigned int vpost_adj_slope0;			// 4
	volatile unsigned int vpost_adj_slope1;
	volatile unsigned int vpost_adj_slope2;
	volatile unsigned int vpost_adj_bound;
	volatile unsigned int vpost_cspace_config;		// 8
	volatile unsigned int vpost_opif_config;			// 9
	volatile unsigned int vpost_opif_bgy;
	volatile unsigned int vpost_opif_bguv;
	volatile unsigned int vpost_opif_alpha;
	volatile unsigned int vpost_opif_msktop;		// 13
	volatile unsigned int vpost_opif_mskbot;
	volatile unsigned int vpost_xlen;
	volatile unsigned int vpost_ylen;
	volatile unsigned int vpost_checksum_ans;		// 17
	volatile unsigned int vpost_reserved18[14];		// 18 - 31

} DISP_VPOST_REG;


typedef struct _DISP_VPP_REG_		// Display Video Post Processing
{
	// DDFCH0 (G185), DDFCH1 (G186)
	DISP_DDFCH_REG ddfch[2];

	// DEINT0 (G187, G188), DEINT1 (G189, G190)
	DISP_DEINT_REG deint[2];

	// VSCL0 (G191, G192), VSCL1 (G193, G194)
	DISP_VSCL_REG vscl[2];

	// NR0 (G195), NR1 (G196)
	DISP_NR_REG nr[2];

	// IMG0 (G197), IMG1 (G198)
	DISP_IMG_REG img[2];

	// VPOST0 (G199), VPOST1 (G200)
	DISP_VPOST_REG vpost[2];

} DISP_VPP_REG;



typedef struct _DISP_TNR_REG_
{
	// group 291 / group 293
	volatile unsigned int tnr_config1;	//	0
	volatile unsigned int tnr_config2;
	volatile unsigned int motion_th1;
	volatile unsigned int motion_th2;
	volatile unsigned int motion_th3;
	volatile unsigned int motion_th4;	//	5
	volatile unsigned int motion_th5;	//	6
	volatile unsigned int motion_12_slope;
	volatile unsigned int motion_23_slope;
	volatile unsigned int motion_34_slope;
	volatile unsigned int motion_45_slope;	//	10
	volatile unsigned int tnr_factor1;
	volatile unsigned int tnr_factor2;
	volatile unsigned int tnr_factor3;
	volatile unsigned int tnr_factor4;
	volatile unsigned int tnr_factor5;	//	15
	volatile unsigned int tnr_color_ystart;
	volatile unsigned int tnr_color_yend;
	volatile unsigned int tnr_color_cbstart;
	volatile unsigned int tnr_color_cbend;
	volatile unsigned int tnr_color_crstart;	//	20
	volatile unsigned int tnr_color_crend;;
	volatile unsigned int tnr_color_factor1;
	volatile unsigned int tnr_color_factor2;
	volatile unsigned int tnr_color_factor3;
	volatile unsigned int tnr_color_factor4;	//	25
	volatile unsigned int tnr_color_factor5;	//	26
	volatile unsigned int clamp_threshold;
	volatile unsigned int clamp_threshold2;
	volatile unsigned int tnr_config3;
	volatile unsigned int tnr_xlen;			//	30
	volatile unsigned int tnr_ylen;			//	31

	// group 292 / group 294
	volatile unsigned int tnr_info;	//	0
	volatile unsigned int w1_sta_x;
	volatile unsigned int w1_sta_y;
	volatile unsigned int w5_sta_x;
	volatile unsigned int w5_sta_y;
	volatile unsigned int w_sel;	//	5
	volatile unsigned int dca_info_yregion;	//	6
	volatile unsigned int dca_info_xregion;
	volatile unsigned int dca_max1_weight;
	volatile unsigned int dca_max2_weight;
	volatile unsigned int dca_min1_weight;	//	10
	volatile unsigned int dca_min2_weight;
	volatile unsigned int dca_info_config;
	volatile unsigned int tnr_w1_info1;
	volatile unsigned int tnr_w1_info2;
	volatile unsigned int tnr_w2_info1;	//	15
	volatile unsigned int tnr_w2_info2;
	volatile unsigned int tnr_w3_info1;
	volatile unsigned int tnr_w3_info2;
	volatile unsigned int tnr_w4_info1;
	volatile unsigned int tnr_w4_info2;	//	20
	volatile unsigned int tnr_w5_info1;;
	volatile unsigned int tnr_w5_info2;
	volatile unsigned int dca_y_max;
	volatile unsigned int dca_y_min;
	volatile unsigned int dca_y_sum_l;	//	25
	volatile unsigned int dca_y_sum_h;	//	26
	volatile unsigned int chksum_out;
	volatile unsigned int reserved_28;
	volatile unsigned int reserved_29;
	volatile unsigned int reserved_30;			//	30
	volatile unsigned int reserved_31;			//	31

} DISP_TNR_REG;

typedef struct _DISP_TNRIW_REG_
{
	// group 295 /  group 296
	volatile unsigned int tnriw_latch_en;	//	0
	volatile unsigned int tnriw_mode_option;
	volatile unsigned int tnriw_enable;
	volatile unsigned int tnriw_urgent_thd;
	volatile unsigned int tnriw_cmdq_thd;
	volatile unsigned int reserved_5;	//	5
	volatile unsigned int reserved_6;	//	6
	volatile unsigned int reserved_7;
	volatile unsigned int reserved_8;
	volatile unsigned int reserved_9;
	volatile unsigned int tnriw_luma_iw_base_addr_0;	//	10
	volatile unsigned int tnriw_luma_iw_base_addr_1;
	volatile unsigned int tnriw_crma_iw_base_addr_0;
	volatile unsigned int tnriw_crma_iw_base_addr_1;
	volatile unsigned int tnriw_vdo_frame_size;
	volatile unsigned int tnriw_vdo_size;	//	15
	volatile unsigned int tnriw_debug_mode;
	volatile unsigned int tnriw_chksum_info;
	volatile unsigned int reserved_18;
	volatile unsigned int reserved_19;
	volatile unsigned int reserved_20;	//	20
	volatile unsigned int reserved_21;;
	volatile unsigned int reserved_22;
	volatile unsigned int reserved_23;
	volatile unsigned int reserved_24;
	volatile unsigned int reserved_25;	//	25
	volatile unsigned int reserved_26;	//	26
	volatile unsigned int tnriw_error_flag_info;
	volatile unsigned int rsdu_data_info;
	volatile unsigned int reserved_29;
	volatile unsigned int reserved_30;			//	30
	volatile unsigned int reserved_31;			//	31
} DISP_TNRIW_REG;

typedef struct _DISP_TNR_TNRIW_REG_
{
	// TNR0(G291, G292), TNR1(G293, G294)
	DISP_TNR_REG tnr[2];

	// TNRIW0(G295), TNRIW1(G296)
	DISP_TNRIW_REG tnriw[2];

} DISP_TNR_TNRIW_REG;



typedef struct
{
//	struct semaphore UpdateLock;
	void* pHWRegBase;

	// ACKs
	int field_end[2];
	int field_start[2];

	DRV_DISP_SYS_MODE_e SysMode;

	DRV_HD2SD_Info_t HD2SD_Info;
	DRV_AFRC_RTE_Info_t AFRC_Info;
	DRV_AFRC_RTE_Info_t RTE_Info;

	DRV_DISP_Scan_Info_t OutputScanInfo[4];// 0 and 1 : original scan info,    2 : NTSC cvbs out,  3:PAL cvbs out
	DRV_SetGSCL_t 	SetGSCL[3];
	display_size_t		panelRes[2];
	display_size_t		panelPitch[2];	
	DRV_FrameRate_e	curr_panel_fps;
	DRV_DMIX_Luma_Adj_t LumaAdj[2];
	DRV_DISP_SYS_ASPECT_RATIO_e AspectRatio[2];
	UINT32 IsMirrorOrg;
	DRV_VPP_OutputWin_t VPP1_output;
		
//	TCIW_Info TCIWInfo;
	UINT32 TestCount0;	// gavin hung test for bird view
	UINT32 TestCount1;	// gavin hung test for bird view

	UINT32 ScenNum; //Barney used for PQ tool. Number of scenario
	UINT32 OrgScene;
	UINT32 PQ_Flag[2];
	UINT32 AP_Modified_Luma;
	UINT32 hue;        //Mainly used for main display. 8700 PQ tool
	UINT16 satsin;
	UINT16 satcos;
	UINT32 ScenSelect;
	UINT8 *PQBuff;
	UINT8 saturation; //Mainly used for main display. 8700 PQ tool
	void *PQPackage[BIN_NUM_MAX]; //PQPackage[0] is reserved
	UINT8 PQ_Version_ID;
	DRV_Sys_OutMode_Info_t DispOutMode;
	UINT8 DispOutModeUpdated;
	DRV_VPP_Param_t* pgVppInfo[4];
	UINT32 RCS_BufCount;
	UINT32 IsRearView;
	void* pBakOSD1Region;
	DRV_VPP_Luma_Adj_t VPP_Luma[2];
	UINT8 vpostBrightness;
	UINT8 acfg_overscan_by_scaling;
	int acfg_overscan_ntsc;
	int acfg_overscan_pal;
  

   #ifdef CLK_ON_OFF
	struct sp_clk *plltv_clk;
	UINT32 clktv;
	struct sp_clk *plldis_clk;
	UINT32 clkdis;
   #endif
  UINT8 AV_IN;
  UINT32 DisableAndroidUI;
  UINT32 Lastest_AndroidUI;
} DISPLAY_WORKMEM;

#endif //__REG_DISP_H__
