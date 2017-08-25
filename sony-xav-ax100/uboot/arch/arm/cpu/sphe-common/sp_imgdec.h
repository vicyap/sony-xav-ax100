/*
 * (C) Copyright 2016
 * Sunplus Technology. <http://www.sunplus.com/>
 * SPDX-License-Identifier:     GPL-2.0+
 */

#ifndef __SP_IMGDEC_H
#define __SP_IMGDEC_H

/**************************************************************************
 *                           C O N S T A N T S                            *
 **************************************************************************/

#define MOON_IMG_MISC_RESET	0x1 << 20
#define MOON_HUFF_RESET		0x1 << 21
#define MOON_JPG_IQT_RESET		0x1 << 22
#define MOON_JR_RESET			0x1 << 23
#define MOON_PNG_RESET			0x1 << 24

#define MOON_IMG_MISC_CLKEN	0x1 << 20
#define MOON_HUFF_CLKEN		0x1 << 21
#define MOON_JPG_IQT_CLKEN		0x1 << 22
#define MOON_JR_CLKEN			0x1 << 23
#define MOON_PNG_CLKEN			0x1 << 24

#define MOON_IMG_MISC_GCLKEN	0x1 << 20
#define MOON_HUFF_GCLKEN		0x1 << 21
#define MOON_JPG_IQT_GCLKEN	0x1 << 22
#define MOON_JR_GCLKEN			0x1 << 23
#define MOON_PNG_GCLKEN		0x1 << 24

#define PNG_PLTE_SIZE		768
#define PNG_LEN_SIZE		19 //the max size of code length tree

/**************************************************************************
 *                          D A T A    T Y P E S                          *
 **************************************************************************/

typedef struct _PNG_IHDR_
{
	unsigned char has_IHDR;
	unsigned int width;
	unsigned int height;
	unsigned char bit_depth;
	unsigned char color_type;
	unsigned char compression_method;
	unsigned char filter_method;
	unsigned char interlace_method;
	unsigned char reserved[2];
} PNG_IHDR;

typedef struct _PNG_PLTE_
{
	unsigned char has_PLTE;
	unsigned char pColorMap[PNG_PLTE_SIZE];
	unsigned char reserved[3];
} PNG_PLTE;

typedef struct _PNG_BKGD_
{
	unsigned char has_bKGD;
	unsigned char bKGD_red;
	unsigned char bKGD_green;
	unsigned char bKGD_blue;
} PNG_BKGD;

typedef struct _PNG_IDAT_
{
	unsigned int has_IDAT;
	unsigned int chunk_length;
	unsigned short zlib_header;
	unsigned char block_final;
	unsigned char block_type;
	unsigned int bs_byte_offset;
	unsigned int bs_bit_offset;
	unsigned int hold_bits;
	unsigned int chunk_remain_bytes;
	unsigned int nlen;
	unsigned int ndist;
	unsigned int ncode;
	unsigned int dma_count_ext;
	unsigned int lens[321];
	unsigned int count[16];
	unsigned int offset[16];
	unsigned int sym_table[288];
	unsigned int len_table_val[256];
	unsigned int len_table_bits[256];
	unsigned int zero_length;
	unsigned short huffcode[288];
	unsigned short huffsize[288];
	unsigned short mincode[17];
	unsigned short valptr[17];
} PNG_IDAT;

typedef struct _PNG_DECODE_
{
	char *filename;
	unsigned int file_size;
	unsigned int bitstream_start;
	unsigned int bitstream_end;
	unsigned char output_run_time;
	unsigned int bs_dma_size;
	unsigned char reserved[3];
} PNG_DECODE;

typedef struct _PNG_INFO_
{
	PNG_IHDR pngIHDR;
	PNG_PLTE pngPLTE;
	PNG_BKGD pngBKGD;
	PNG_IDAT pngIDAT;
	PNG_DECODE pngDecode;
} PNG_INFO;

/**************************************************************************
 *               F U N C T I O N    D E C L A R A T I O N S               *
 **************************************************************************/

void imgdec_clock_enable(void);
void imgdec_clock_disable(void);
int imgdec_png_decode_zlib(
	unsigned int bs_block_num,
	unsigned int bs_block_size,
	unsigned char *bs_buf_ptr, 
	unsigned int bs_buf_phyaddr, 
	unsigned int LZ_backward_buf_phyaddr,
	unsigned int scanline_buf_phyaddr, 
	unsigned int decoded_buf_phyaddr,
	unsigned short scramble_key,
	PNG_INFO *png_info_data,
	unsigned int *exe_mode);
/* Note for imgdec_png_decode_zlib()!!! 
The following parameters are used only in first bitstream block(bs_block_num = 0):
bs_buf_ptr, 
LZ_backward_buf_phyaddr, 
scanline_buf_phyaddr, 
decoded_buf_phyaddr, 
scramble_key.
*/

#endif	/* __SP_IMGDEC_H */
