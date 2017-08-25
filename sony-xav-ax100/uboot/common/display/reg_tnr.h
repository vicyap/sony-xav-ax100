#ifndef __REG_TNR_H__
#define __REG_TNR_H__

/**
 * @file reg_tnr.h
 * @brief TNR register Header file for 8388
 * @author zongsina.du
 */

/**************************************************************************
 *                          D A T A    T Y P E S                          *
 **************************************************************************/

typedef struct TNR_REG_s
{


	union
	{
		struct
		{
			UINT32 latch_en                : 1;		/*(grp307_00[0]) Enable The v656in Function 0: disable 1: enable*/
			UINT32 vsp_adj                 : 1;
			UINT32 tnr_en                   : 1;
			UINT32 tnr_yonly              : 1;
			UINT32 weber_on              : 1;
			UINT32 diffmax_on            : 1;
			UINT32 yc_motion_sep       : 1;
			UINT32                           : 1;
			UINT32 chroma_weight       : 4;
			UINT32 chroma_round        : 1;
			UINT32 modify_round         : 1;
			UINT32 weight_new            : 1;
			UINT32 clamp_diff              : 1;
			UINT32                         : 16;
		};
		UINT32 grp0_00;
	};


	union
	{
		struct
		{
			UINT32 signchk_on            : 1;		/*(grp307_00[0]) Enable The v656in Function 0: disable 1: enable*/
			UINT32 color_factor_en     : 1;
			UINT32                          : 5;
			UINT32 diffone_en             : 1;
			UINT32 edge_th                : 8;
			UINT32                        : 16;
		};
		UINT32 grp0_01;
	};


	union
	{
		struct
		{
			UINT32 y_motion_th1            : 8;		/*(grp307_00[0]) Enable The v656in Function 0: disable 1: enable*/
			UINT32 c_motion_th1     : 8;
			UINT32                          : 16;
		};
		UINT32 grp0_02;
	};

	union
	{
		struct
		{
			UINT32 y_motion_th2             : 8;		/*(grp307_00[0]) Enable The v656in Function 0: disable 1: enable*/
			UINT32 c_motion_th2     : 8;
			UINT32                          : 16;
		};
		UINT32 grp0_03;
	};

	union
	{
		struct
		{
			UINT32 y_motion_th3            : 8;		/*(grp307_00[0]) Enable The v656in Function 0: disable 1: enable*/
			UINT32 c_motion_th3     : 8;
			UINT32                          : 16;
		};
		UINT32 grp0_04;
	};

	union
	{
		struct
		{
			UINT32 y_motion_th4            : 8;		/*(grp307_00[0]) Enable The v656in Function 0: disable 1: enable*/
			UINT32 c_motion_th4    : 8;
			UINT32                          : 16;
		};
		UINT32 grp0_05;
	};


	union
	{
		struct
		{
			UINT32 y_motion_th5            : 8;		/*(grp307_00[0]) Enable The v656in Function 0: disable 1: enable*/
			UINT32 c_motion_th5    : 8;
			UINT32                          : 16;
		};
		UINT32 grp0_06;
	};


	union
	{
		struct
		{
			UINT32 y_motion_12_slope            : 8;		/*(grp307_00[0]) Enable The v656in Function 0: disable 1: enable*/
			UINT32 c_motion_12_slope    : 8;
			UINT32                          : 16;
		};
		UINT32 grp0_07;
	};

	union
	{
		struct
		{
			UINT32 y_motion_23_slope            : 8;		/*(grp307_00[0]) Enable The v656in Function 0: disable 1: enable*/
			UINT32 c_motion_23_slope    : 8;
			UINT32                          : 16;
		};
		UINT32 grp0_08;
	};

	union
	{
		struct
		{
			UINT32 y_motion_34_slope            : 8;		/*(grp307_00[0]) Enable The v656in Function 0: disable 1: enable*/
			UINT32 c_motion_34_slope    : 8;
			UINT32                          : 16;
		};
		UINT32 grp0_09;
	};


	union
	{
		struct
		{
			UINT32 y_motion_45_slope            : 8;		/*(grp307_00[0]) Enable The v656in Function 0: disable 1: enable*/
			UINT32 c_motion_45_slope    : 8;
			UINT32                          : 16;
		};
		UINT32 grp0_10;
	};

	union
	{
		struct
		{
			UINT32 y_factor1          : 5;		/*(grp307_00[0]) Enable The v656in Function 0: disable 1: enable*/
			UINT32             : 3;
			UINT32 c_factor1                 : 5;
			UINT32             : 19;
		};
		UINT32 grp0_11;
	};

	union
	{
		struct
		{
			UINT32 y_factor2          : 5;		/*(grp307_00[0]) Enable The v656in Function 0: disable 1: enable*/
			UINT32             : 3;
			UINT32 c_factor2                 : 5;
			UINT32             : 19;
		};
		UINT32 grp0_12;
	};

	union
	{
		struct
		{
			UINT32 y_factor3          : 5;		/*(grp307_00[0]) Enable The v656in Function 0: disable 1: enable*/
			UINT32             : 3;
			UINT32 c_factor3                 : 5;
			UINT32             : 19;
		};
		UINT32 grp0_13;
	};

	union
	{
		struct
		{
			UINT32 y_factor4          : 5;		/*(grp307_00[0]) Enable The v656in Function 0: disable 1: enable*/
			UINT32             : 3;
			UINT32 c_factor4                : 5;
			UINT32             : 19;
		};
		UINT32 grp0_14;
	};

	union
	{
		struct
		{
			UINT32 y_factor5          : 5;		/*(grp307_00[0]) Enable The v656in Function 0: disable 1: enable*/
			UINT32             : 3;
			UINT32 c_factor5                 : 5;
			UINT32             : 19;
		};
		UINT32 grp0_15;
	};


	union
	{
		struct
		{
			UINT32 y_start          : 10;		/*(grp307_00[0]) Enable The v656in Function 0: disable 1: enable*/
			UINT32             : 22;

		};
		UINT32 grp0_16;
	};

	union
	{
		struct
		{
			UINT32 y_end          : 10;		/*(grp307_00[0]) Enable The v656in Function 0: disable 1: enable*/
			UINT32             : 22;

		};
		UINT32 grp0_17;
	};

	union
	{
		struct
		{
			UINT32 cb_start          : 10;		/*(grp307_00[0]) Enable The v656in Function 0: disable 1: enable*/
			UINT32             : 22;

		};
		UINT32 grp0_18;
	};

	union
	{
		struct
		{
			UINT32 cb_end          : 10;		/*(grp307_00[0]) Enable The v656in Function 0: disable 1: enable*/
			UINT32             : 22;

		};
		UINT32 grp0_19;
	};

	union
	{
		struct
		{
			UINT32 cr_start          : 10;		/*(grp307_00[0]) Enable The v656in Function 0: disable 1: enable*/
			UINT32             : 22;

		};
		UINT32 grp0_20;
	};

	union
	{
		struct
		{
			UINT32 cr_end           : 10;		/*(grp307_00[0]) Enable The v656in Function 0: disable 1: enable*/
			UINT32             : 22;

		};
		UINT32 grp0_21;
	};

	union
	{
		struct
		{
			UINT32 y_color_factor1           : 5;		/*(grp307_00[0]) Enable The v656in Function 0: disable 1: enable*/
			UINT32             : 3;
			UINT32 c_color_factor1           : 5;		/*(grp307_00[0]) Enable The v656in Function 0: disable 1: enable*/
			UINT32             : 19;
		};
		UINT32 grp0_22;
	};

	union
	{
		struct
		{
			UINT32 y_color_factor2           : 5;		/*(grp307_00[0]) Enable The v656in Function 0: disable 1: enable*/
			UINT32             : 3;
			UINT32 c_color_factor2           : 5;		/*(grp307_00[0]) Enable The v656in Function 0: disable 1: enable*/
			UINT32             : 19;
		};
		UINT32 grp0_23;
	};

	union
	{
		struct
		{
			UINT32 y_color_factor3           : 5;		/*(grp307_00[0]) Enable The v656in Function 0: disable 1: enable*/
			UINT32             : 3;
			UINT32 c_color_factor3           : 5;		/*(grp307_00[0]) Enable The v656in Function 0: disable 1: enable*/
			UINT32             : 19;
		};
		UINT32 grp0_24;
	};

	union
	{
		struct
		{
			UINT32 y_color_factor4           : 5;		/*(grp307_00[0]) Enable The v656in Function 0: disable 1: enable*/
			UINT32             : 3;
			UINT32 c_color_factor4           : 5;		/*(grp307_00[0]) Enable The v656in Function 0: disable 1: enable*/
			UINT32             : 19;
		};
		UINT32 grp0_25;
	};

	union
	{
		struct
		{
			UINT32 y_color_factor5           : 5;		/*(grp307_00[0]) Enable The v656in Function 0: disable 1: enable*/
			UINT32             : 3;
			UINT32 c_color_factor5           : 5;		/*(grp307_00[0]) Enable The v656in Function 0: disable 1: enable*/
			UINT32             : 19;
		};
		UINT32 grp0_26;
	};


	union
	{
		struct
		{
			UINT32 yclamp_value_th           : 8;		/*(grp307_00[0]) Enable The v656in Function 0: disable 1: enable*/
			UINT32 cclamp_value_th    : 8;
			UINT32             : 16;
		};
		UINT32 grp0_27;
	};

	union
	{
		struct
		{
			UINT32 yclamp_value_th2           : 8;		/*(grp307_00[0]) Enable The v656in Function 0: disable 1: enable*/
			UINT32 cclamp_value_th2    : 8;
			UINT32             : 16;
		};
		UINT32 grp0_28;
	};

	union
	{
		struct
		{
			UINT32 tnr_p_mode_en           : 1;		/*(grp307_00[0]) Enable The v656in Function 0: disable 1: enable*/
			UINT32 tnr_mas0_en     : 1;
			UINT32 tnr_mas1_en     : 1;
			UINT32                 : 5;
			UINT32 tnr_pre_en     : 1;
			UINT32 nr_nxt_en      : 1;
			UINT32               : 22;
		};
		UINT32 grp0_29;
	};

	union
	{
		struct
		{
			UINT32 tnr_mas_xlen           : 13;		/*(grp307_00[0]) Enable The v656in Function 0: disable 1: enable*/
			UINT32             : 19;

		};
		UINT32 grp0_30;
	};

	union
	{
		struct
		{
			UINT32 tnr_mas_ylen           : 12;		/*(grp307_00[0]) Enable The v656in Function 0: disable 1: enable*/
			UINT32             : 20;

		};
		UINT32 grp0_31;
	};

	union
	{
		struct
		{
			UINT32 chksum_en                : 1;		/*(grp307_00[0]) Enable The v656in Function 0: disable 1: enable*/
			UINT32 luma_info_en                 : 1;
			UINT32 noise_meter_en                   : 1;
			UINT32                        : 29;
		};
		UINT32 grp1_00;
	};


	union
	{
		struct
		{
			UINT32 w1_sta_x            : 12;		/*(grp307_00[0]) Enable The v656in Function 0: disable 1: enable*/
			UINT32                          : 20;
		};
		UINT32 grp1_01;
	};


	union
	{
		struct
		{
			UINT32 w1_sta_y            : 12;		/*(grp307_00[0]) Enable The v656in Function 0: disable 1: enable*/
			UINT32                          : 20;
		};
		UINT32 grp1_02;
	};

	union
	{
		struct
		{
			UINT32 w5_sta_x             : 12;		/*(grp307_00[0]) Enable The v656in Function 0: disable 1: enable*/
			UINT32                          : 20;
		};
		UINT32 grp1_03;
	};

	union
	{
		struct
		{
			UINT32 w5_sta_y            : 12;		/*(grp307_00[0]) Enable The v656in Function 0: disable 1: enable*/
			UINT32                          : 20;
		};
		UINT32 grp1_04;
	};

	union
	{
		struct
		{
			UINT32 w_width_sel              : 3;		/*(grp307_00[0]) Enable The v656in Function 0: disable 1: enable*/
			UINT32                          :1;
			UINT32 w_height_sel              : 3;		/*(grp307_00[0]) Enable The v656in Function 0: disable 1: enable*/
			UINT32                          :25;
		};
		UINT32 grp1_05;
	};


	union
	{
		struct
		{
			UINT32 yt_border            : 8;		/*(grp307_00[0]) Enable The v656in Function 0: disable 1: enable*/
			UINT32 yb_border    : 8;
			UINT32                          : 16;
		};
		UINT32 grp1_06;
	};


	union
	{
		struct
		{
			UINT32 xl_border             : 8;		/*(grp307_00[0]) Enable The v656in Function 0: disable 1: enable*/
			UINT32 xr_border    : 8;
			UINT32                          : 16;
		};
		UINT32 grp1_07;
	};

	union
	{
		struct
		{
			UINT32 max1_h_weight            : 8;		/*(grp307_00[0]) Enable The v656in Function 0: disable 1: enable*/
			UINT32 max1_v_weight     : 8;
			UINT32                          : 16;
		};
		UINT32 grp1_08;
	};

	union
	{
		struct
		{
			UINT32 max2_h_weight            : 8;		/*(grp307_00[0]) Enable The v656in Function 0: disable 1: enable*/
			UINT32 max2_v_weight    : 8;
			UINT32                          : 16;
		};
		UINT32 grp1_09;
	};


	union
	{
		struct
		{
			UINT32 min1_h_weight            : 8;		/*(grp307_00[0]) Enable The v656in Function 0: disable 1: enable*/
			UINT32 min1_v_weight     : 8;
			UINT32                          : 16;
		};
		UINT32 grp1_10;
	};

	union
	{
		struct
		{
			UINT32 min2_h_weight            : 8;		/*(grp307_00[0]) Enable The v656in Function 0: disable 1: enable*/
			UINT32 min2_v_weight     : 8;
			UINT32                          : 16;
		};
		UINT32 grp1_11;
	};

	union
	{
		struct
		{
			UINT32 start_sel           : 1;		/*(grp307_00[0]) Enable The v656in Function 0: disable 1: enable*/
			UINT32             : 3;
			UINT32 h_sum_opt                 : 1;
			UINT32 h_max_min_opt     : 2;
			UINT32 v_max_min_opt           : 1;		/*(grp307_00[0]) Enable The v656in Function 0: disable 1: enable*/
			UINT32 offset    : 8;
			UINT32                          : 16;
		};
		UINT32 grp1_12;
	};

	union
	{
		struct
		{
			UINT32 w1_diffy_l           : 16;		/*(grp307_00[0]) Enable The v656in Function 0: disable 1: enable*/
			UINT32             : 16;
		};
		UINT32 grp1_13;
	};

	union
	{
		struct
		{
			UINT32 w1_diffy_h          : 8;		/*(grp307_00[0]) Enable The v656in Function 0: disable 1: enable*/
			UINT32 w1_sign    : 8;
			UINT32             : 16;
		};
		UINT32 grp1_14;
	};

	union
	{
		struct
		{
			UINT32 w2_diffy_l          : 16;		/*(grp307_00[0]) Enable The v656in Function 0: disable 1: enable*/
			UINT32             : 16;
		};
		UINT32 grp1_15;
	};


	union
	{
		struct
		{
			UINT32 w2_diffy_h           : 8;		/*(grp307_00[0]) Enable The v656in Function 0: disable 1: enable*/
			UINT32 w2_sign    : 8;
			UINT32             : 16;

		};
		UINT32 grp1_16;
	};

	union
	{
		struct
		{
			UINT32 w3_diffy_l          : 16;		/*(grp307_00[0]) Enable The v656in Function 0: disable 1: enable*/
			UINT32             : 16;

		};
		UINT32 grp1_17;
	};

	union
	{
		struct
		{
			UINT32 w3_diffy_h          :8;		/*(grp307_00[0]) Enable The v656in Function 0: disable 1: enable*/
			UINT32 w3_sign          : 8;		/*(grp307_00[0]) Enable The v656in Function 0: disable 1: enable*/
			UINT32             : 16;

		};
		UINT32 grp1_18;
	};

	union
	{
		struct
		{
			UINT32 w4_diffy_l          : 16;		/*(grp307_00[0]) Enable The v656in Function 0: disable 1: enable*/
			UINT32             : 16;

		};
		UINT32 grp1_19;
	};

	union
	{
		struct
		{
			UINT32 w4_diffy_h           : 8;		/*(grp307_00[0]) Enable The v656in Function 0: disable 1: enable*/
			UINT32 w4_sign           : 8;		/*(grp307_00[0]) Enable The v656in Function 0: disable 1: enable*/
			UINT32             : 16;

		};
		UINT32 grp1_20;
	};

	union
	{
		struct
		{
			UINT32 w5_diffy_l            : 16;		/*(grp307_00[0]) Enable The v656in Function 0: disable 1: enable*/
			UINT32             : 16;

		};
		UINT32 grp1_21;
	};

	union
	{
		struct
		{
			UINT32 w5_diffy_h           : 8;		/*(grp307_00[0]) Enable The v656in Function 0: disable 1: enable*/
			UINT32 w5_sign    : 8;
			UINT32             : 16;
		};
		UINT32 grp1_22;
	};

	union
	{
		struct
		{
			UINT32 y_max1           : 8;		/*(grp307_00[0]) Enable The v656in Function 0: disable 1: enable*/
			UINT32 y_max2     : 8;
			UINT32             : 16;
		};
		UINT32 grp1_23;
	};

	union
	{
		struct
		{
			UINT32 y_min1           : 8;		/*(grp307_00[0]) Enable The v656in Function 0: disable 1: enable*/
			UINT32 y_min2    : 8;
			UINT32             : 16;
		};
		UINT32 grp1_24;
	};

	union
	{
		struct
		{
			UINT32 y_sum_l           : 16;		/*(grp307_00[0]) Enable The v656in Function 0: disable 1: enable*/
			UINT32             : 16;
		};
		UINT32 grp1_25;
	};

	union
	{
		struct
		{
			UINT32 y_sum_h           : 4;		/*(grp307_00[0]) Enable The v656in Function 0: disable 1: enable*/
			UINT32             : 28;
		};
		UINT32 grp1_26;
	};


	union
	{
		struct
		{
			UINT32 chksum_out           : 16;		/*(grp307_00[0]) Enable The v656in Function 0: disable 1: enable*/
			UINT32             : 16;
		};
		UINT32 grp1_27;
	};

	UINT32 grp1_28_reserved;
	UINT32 grp1_29_reserved;
	UINT32 grp1_30_reserved;
	UINT32 grp1_31_reserved;
} TNR_REG_T;


#endif //__REG_TNR_H__
