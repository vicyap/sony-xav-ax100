
#ifndef __REG_IMGDEC_H__
#define __REG_IMGDEC_H__

#include <mach/hardware.h>

/**************************************************************************
 *                           C O N S T A N T S                            *
 **************************************************************************/

// Hardware register memory address by hardware.h:
#define IMG_DEC_HUF_GRP					109
#define IMG_DEC_HUF_BASE					IO0_ADDRESS(IMG_DEC_HUF_GRP*32*4)
#define LOGI_ADDR_IMG_DEC_HUF_REG		(IMG_DEC_HUF_BASE)

// Hardware register memory address:
#define RGST_BASE 							0x40000000		// HW register physical memory base address
#define RGST_SIZE 							0x8000 			// HW register physical memory size
#define RGST_IMG_DEC_SIZE 					0x200 			// HW register physical memory size for IMG_DEC (4x128)
#define RGST_IMG_DEC_HUF_OFFSET 			109*128 			// IMG_DEC/HUF HW reg address offset G109 (0x3680)
#define RGST_IMG_DEC_JDM_OFFSET 			110*128 			// IMG_DEC/JDM HW reg address offset G100 (0x3700)
#define RGST_IMG_DEC_JPG_INVQ_OFFSET 		111*128 			// IMG_DEC/JPG_INVQ HW reg address offset G111 (0x3780)
#define RGST_IMG_DEC_PNG_OFFSET 			112*128 			// IMG_DEC/PNG HW reg address offset G112 (0x3800)

// PNG data RAM address:
#define DATA_RAM_BASE 						0x1FFB0000		// HW PNG data RAM physical memory base address
#define DATA_RAM_SIZE 						0x1000 			// HW PNG data RAM physical memory size = 4096

// HUF register type:
// jpeg_huff_mode[3:0] = The picture YCbCr ratio for motion JPEG use.
#define YUV_RAW			0xf // 1111: raw data
#define YUV_444			0x0 // 0000: 444
#define YUV_422_1X2		0x1 // 0001: 422, 1x2
#define YUV_422_2X1		0x4 // 0100: 422, 2x1
#define YUV_411_2X2		0x5 // 0101: 411, 2x2
#define YUV_411_4X1		0x8 // 1000: 411, 4x1
#define YUV_411_1X4		0x2 // 0010: 411, 1x4
#define YUV_BW			0x7 // 0111: bwmode (black/white)

// JDM register type:
// jdm_mode[4] = JPEG enable.
#define GRAPH_JPEG					(1<<4)
// jdm_mode[5] = JPEG render enable.
#define GRAPH_JPEG_OUT_8			(1<<5)
// jdm_mode[10] = JPEG 32-bit mode.
#define GRAPH_JPEG_32				(1<<10)
// jdm_mode[12] = JPEG saturation mode.
#define GRAPH_JPEG_SAT				(1<<12)
// jdm_dma1_mode[3:0] = DMA operation mode.
#define VADDR_MODE_CLEAR			0
#define VADDR_MODE_GLOBAL			7
// jdm_dma1_mode[10:8] = DMA transfer length. (unit=bytes)
#define DMA_LEN_32					0
#define DMA_LEN_64					1
#define DMA_LEN_128				2
#define DMA_LEN_256				3
#define DMA_LEN_512				4
#define DMA_LEN_1024				5
#define DMA_LEN_2048				6
#define DMA_LEN_4096				7

//INVQ register type:
// jpeg_invq_mode[0] = JPEG bypass mode.
#define INVQ_JPEG_NOBYPASS			(0<<0)
#define INVQ_JPEG_BYPASS			(1<<0)
// jpeg_invq_mode[1] = JPEG mode.
#define INVQ_JPEG_MODE				(1<<1)

// JR register file:
#define JR_COE_V_SET_1_START		0x00
#define JR_COE_V_SET_1_END			0x20
#define JR_COE_V_SET_2_START		0x40
#define JR_COE_V_SET_2_END			0x44
#define JR_COE_V_SET_3_START		0x80
#define JR_COE_V_SET_3_END			0xa0
#define JR_COE_V_SET_4_START		0xc0
#define JR_COE_V_SET_4_END			0xc4

#define JR_COE_H_SET_1_START		0x20
#define JR_COE_H_SET_1_END			0x40
#define JR_COE_H_SET_2_START		0x44
#define JR_COE_H_SET_2_END			0x48
#define JR_COE_H_SET_3_START		0xa0
#define JR_COE_H_SET_3_END			0xc0
#define JR_COE_H_SET_4_START		0xc4
#define JR_COE_H_SET_4_END			0xc8

#define JR_FRAME_WIDTH				0x50
#define JR_FRAME_HEIGHT			0x51
#define JR_LUMA_BASE				0x52
#define JR_CROMA_BASE				0x53
#define JR_TEMP_BUF_BASE			0x54
#define JR_DMA_CP_BASE				0x55
#define JR_DMA1_CP_BASE			JR_DMA_CP_BASE
#define JR_BASE_EXTEND0			0x56
#define JR_DISP_COLOR_MODE			0x57
#define JR_SCALER_INDEX_V			0x58
#define JR_SCALER_INDEX_H			0x59
#define JR_SCALER_INDEX_EXT		0x5a
#define JR_SRC_WIDTH				0x5b
#define JR_SRC_HEIGHT				0x5c
#define JR_MCU_X					0x5d
#define JR_MCU_Y					0x5e
#define JR_SRC_COLOR_MODE			0x5f
#define JR_RENDER_V_INIT			0x61
#define JR_RENDER_H_INIT			0x62
#define JR_RENDER_H_BOUND			0x63
#define JR_RENDER_V_BOUND			0x64
#define JR_WRITE_COE_CHROMA		0x65
#define JR_SRCPP_CTRL				0x67
#define JR_RENDER_V_START			0x68
#define JR_RENDER_H_START			0x69
#define JR_RENDER_LSB				0x6a
#define JR_BASE_EXTEND1			0xd0
#define JR_BASE_EXTEND2			0xd1
#define JR_AUTO_SRC_LUMA_BASE		0xd2
#define JR_AUTO_SRC_CHROMA_BASE	0xd3
#define JR_AUTO_SRC_BASE_EXT0		0xd4
#define JR_AUTO_SRC_BASE_EXT1		0xd5
#define JR_MISC						0xd6
#define JR_MCU_X_START				0xd7
#define JR_MCU_X_END				0xd8
#define JR_MCU_Y_START				0xd9
#define JR_MCU_Y_END				0xda
#define JR_MCU_LINE_NUM			0xdb
#define JR_V_PHASE_OFFSET			0xdc
#define JR_H_PHASE_OFFSET			0xdd
#define JR_AUTO_FETCH_WIDTH		0xde

// JR_DMA_CP_BASE[10:0]: Indicates which ping-pong DMA buffer for JR using.
#define JR_DMA_BUF_BASE  0x100

// JR_DISP_COLOR_MODE[3:0] : display color mode
#define DIS_YUV420			0
#define DIS_YUVBW			1
#define DIS_RGB565			8
//#define DIS_ARGB1555		9 // removed in letb123
//#define DIS_RGBA5551		10 // removed in letb123
//#define DIS_RGB4444		11 // removed in letb123
#define DIS_RGBA8888		14
#define DIS_ARGB8888		15

// JR_SRC_COLOR_MODE[2:0] : source color mode of decoded picture.
#define SRC_YUV444			0
#define SRC_YUV422_2X1		1
#define SRC_YUV422_1X2		2
#define SRC_YUV411			3 // = SRC_YUV420
//#define SRC_RAW			4 // removed in letb123
#define SRC_YUY2			4 // added in letb123
//#define SRC_RGB565		5 // removed in letb123
#define SRC_RGBA8888		6
#define SRC_ARGB8888		7 

// JR_SRCPP_CTRL[4:0]: JR source pre-process, defined as below.
#define PSCTRL_EN			15
#define CHROMA_PS_V		8 // chroma_v_pre_scaling
#define CHROMA_PS_H		4 // chroma_h_pre_scaling
#define LUMA_PS_V			2 // luma_v_pre_scaling
#define LUMA_PS_H			1 // luma_h_pre_scaling


/**************************************************************************
 *                          D A T A    T Y P E S                          *
 **************************************************************************/
 
// JPEG HUF registers: (unit: 32 bits)
typedef	struct _JPEG_HUF_REG_
{
	volatile unsigned int jpeg_huff_mode;				// 00
	volatile unsigned int jpeg_huff_ctrl;				// 01
	volatile unsigned int jpeg_huff_status;				// 02
	volatile unsigned int jpeg_huff_scan_ctrl;			// 03
	volatile unsigned int jpeg_huff_table;				// 04
	volatile unsigned int jpeg_huff_mcuno;				// 05
	volatile unsigned int jpeg_huff_jpeghsize;			// 06
	volatile unsigned int jpeg_huff_jpegvsize;			// 07
	volatile unsigned int jpeg_huff_bitstream;			// 08
	volatile unsigned int jpeg_huff_bits_dma_offset;	// 09
	volatile unsigned int jpeg_huff_wr_huff_info;		// 10
	volatile unsigned int jpeg_huff_bits_info;			// 11
	volatile unsigned int jpeg_huff_rd_huff_info;		// 12
	volatile unsigned int jpeg_huff_dma_offset_0;		// 13
	volatile unsigned int jpeg_huff_dma_offset_1;		// 14
	volatile unsigned int jpeg_huff_acval_offset_0;		// 15
	volatile unsigned int jpeg_huff_acval_offset_1;		// 16
	volatile unsigned int jpeg_huff_acval_offset_2;		// 17
	volatile unsigned int jpeg_huff_acval_offset_3;		// 18
	volatile unsigned int jpeg_huff_wr_dc_01;			// 19
	volatile unsigned int jpeg_huff_wr_dc_23;			// 20
	volatile unsigned int jpeg_huff_wr_acval_0;		// 21
	volatile unsigned int jpeg_huff_wr_acval_1;		// 22
	volatile unsigned int jpeg_huff_wr_acval_2;		// 23
	volatile unsigned int jpeg_huff_wr_acval_3;		// 24
	volatile unsigned int jpeg_huff_rd_addr;			// 25
	volatile unsigned int jpeg_huff_rd_val;				// 26
	volatile unsigned int jpeg_huff_probe;				// 27
	volatile unsigned int jr_control;					// 28
	volatile unsigned int jr_status;						// 29
	volatile unsigned int jr_reg_num;					// 30
	volatile unsigned int jr_reg_data;					// 31
} JPEG_HUF_REG;

// JPEG JDM registers: (unit: 32 bits)
typedef	struct _JPEG_JDM_REG_
{
	volatile unsigned int jdm_mode;					// 00
	volatile unsigned int jdm_status;					// 01
	volatile unsigned int jdm_dma1_mode;				// 02
	volatile unsigned int jdm_dma1_xaddr;				// 03
	volatile unsigned int jdm_dma1_yaddr;				// 04
	volatile unsigned int jdm_dma1_done;				// 05
	volatile unsigned int jdm_dma1_start_addr;		// 06
	volatile unsigned int jdm_dma1_block_num;		// 07
	volatile unsigned int jdm_reserved1[21];			// 08 - 28
	volatile unsigned int jdm_jpeg_offset;				// 29
	volatile unsigned int jdm_reserved2[2];				// 30 - 31
} JPEG_JDM_REG;

// JPEG INVQ registers: (unit: 32 bits)
typedef	struct _JPEG_INVQ_REG_
{
	volatile unsigned int jpeg_invq_qmx_par;			// 00
	volatile unsigned int jpeg_invq_mode;				// 01
	volatile unsigned int jpeg_invq_chksum;			// 02
	volatile unsigned int jpeg_invq_reserved[29];		// 03 - 31
} JPEG_INVQ_REG;

// PNG registers: (unit: 32 bits)
typedef	struct _PNG_REG_
{
	volatile unsigned int png_dec_mode;				// 00
	volatile unsigned int png_bs_str_addr_m;			// 01
	volatile unsigned int png_bs_str_addr_l;			// 02
	volatile unsigned int png_bs_end_addr_m;			// 03
	volatile unsigned int png_bs_end_addr_l;			// 04
	volatile unsigned int png_bs_buf_addr;				// 05
	volatile unsigned int png_bs_offset;				// 06
	volatile unsigned int png_bs_used_cnt;				// 07
	volatile unsigned int png_bs_used_bit;				// 08
	volatile unsigned int png_dec_ctrl;					// 09
	volatile unsigned int png_img_width;				// 10
	volatile unsigned int png_img_height;				// 11
	volatile unsigned int png_img_type;				// 12
	volatile unsigned int png_chunk_size_m;			// 13
	volatile unsigned int png_chunk_size_l;				// 14
	volatile unsigned int png_blk_info;					// 15
	volatile unsigned int png_huf_tb_addr;				// 16
	volatile unsigned int png_huf_tb_ctrl;				// 17
	volatile unsigned int png_huf_tb_data;				// 18
	volatile unsigned int png_interrupt;				// 19
	volatile unsigned int png_scl_ext_addr_m;			// 20
	volatile unsigned int png_scl_ext_addr_l;			// 21
	volatile unsigned int png_scl_buf_addr;				// 22
	volatile unsigned int png_bw_ext_addr_m;			// 23
	volatile unsigned int png_bw_ext_addr_l;			// 24
	volatile unsigned int png_rem_chunk_size_m;		// 25
	volatile unsigned int png_rem_chunk_size_l;		// 26
	volatile unsigned int png_pxl_ext_addr_m;			// 27
	volatile unsigned int png_pxl_ext_addr_l;			// 28
	volatile unsigned int png_pxl_line;					// 29
	volatile unsigned int png_probe_addr;				// 30
	volatile unsigned int png_probe_value;				// 31
} PNG_REG;

// IMG_DEC all registers: (unit: 32 bits)
typedef struct _IMG_DEC_REG_
{
	JPEG_HUF_REG huf;						// 00 - 31
	JPEG_JDM_REG jdm;						// 31 - 63
	JPEG_INVQ_REG invq;						// 64 - 95
	PNG_REG png;							// 96 - 127
} IMG_DEC_REG;

#endif //__REG_IMGDEC_H__

