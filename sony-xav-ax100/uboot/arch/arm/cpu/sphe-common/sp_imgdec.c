/*
 * (C) Copyright 2016
 * Sunplus Technology. <http://www.sunplus.com/>
 * SPDX-License-Identifier:     GPL-2.0+
 */

#include <common.h>
#include <malloc.h>
#include "sp_imgdec.h"

// #define SP_PNG_DEC_VERBOSE

typedef struct
{
    // Group 0 - 111
    unsigned int g0_111_reserved[112][32];
    // Group 112 : PNG
    unsigned int png_dec_mode;
    unsigned int png_bs_str_addr_m;
    unsigned int png_bs_str_addr_l;
    unsigned int png_bs_end_addr_m;
    unsigned int png_bs_end_addr_l;
    unsigned int png_bs_buf_addr;
    unsigned int png_bs_offset;
    unsigned int png_bs_used_cnt;
    unsigned int png_bs_used_bit;
    unsigned int png_dec_ctrl;
    unsigned int png_img_width;
    unsigned int png_img_height;
    unsigned int png_img_type;
    unsigned int png_chunk_size_m;
    unsigned int png_chunk_size_l;
    unsigned int png_blk_info;
    unsigned int png_huf_tb_addr;
    unsigned int png_huf_tb_ctrl;
    unsigned int png_huf_tb_data;
    unsigned int png_interrupt;
    unsigned int png_scl_ext_addr_m;
    unsigned int png_scl_ext_addr_l;
    unsigned int png_scl_buf_addr;
    unsigned int png_bw_ext_addr_m;
    unsigned int png_bw_ext_addr_l;
    unsigned int png_rem_chunk_size_m;
    unsigned int png_rem_chunk_size_l;
    unsigned int png_pxl_ext_addr_m;
    unsigned int png_pxl_ext_addr_l;
    unsigned int png_pxl_line;
    unsigned int png_probe_addr;
    unsigned int png_probe_value;
    // Group 113 - 255
    unsigned int g113_255_reserved[143][32];
} RegisterFile;

#define RGST_OFFSET 0x9C000000

#define regs0 ((volatile RegisterFile *)(RGST_OFFSET))
#define HWREG_W(reg, value) 	\
	do {						\
		regs0->reg = value;		\
	} while (0)

#define HWREG_R(reg) (regs0->reg)



static const unsigned char g_uiCodeLength[PNG_LEN_SIZE] = {
16, 17, 18, 0, 8, 7, 9, 6, 10, 5, 11, 4, 12, 3, 13, 2, 14, 1, 15
};

void PNG_pull_bs_byte(
	PNG_INFO *png_info_data, 
	unsigned char *bs_buf_ptr)
{
	if(png_info_data->pngIDAT.chunk_remain_bytes == 0)
	{
		png_info_data->pngIDAT.chunk_length = ((unsigned int)bs_buf_ptr[png_info_data->pngIDAT.bs_byte_offset + 4] << 24) | 
											((unsigned int)bs_buf_ptr[png_info_data->pngIDAT.bs_byte_offset + 5] << 16) | 
											((unsigned int)bs_buf_ptr[png_info_data->pngIDAT.bs_byte_offset + 6] << 8) | 
											(unsigned int)bs_buf_ptr[png_info_data->pngIDAT.bs_byte_offset + 7];
		png_info_data->pngIDAT.chunk_remain_bytes = png_info_data->pngIDAT.chunk_length;
		png_info_data->pngIDAT.bs_byte_offset += 12;
	}
	png_info_data->pngIDAT.chunk_remain_bytes -= 1;
	png_info_data->pngIDAT.hold_bits += (unsigned int)bs_buf_ptr[png_info_data->pngIDAT.bs_byte_offset] << png_info_data->pngIDAT.bs_bit_offset;
	png_info_data->pngIDAT.bs_bit_offset += 8;
	png_info_data->pngIDAT.bs_byte_offset += 1;
}

void PNG_need_bs_bits(
	PNG_INFO *png_info_data, 
	unsigned char *bs_buf_ptr, 
	unsigned int need_bits)
{
	while(png_info_data->pngIDAT.bs_bit_offset < need_bits)
	{
		if(png_info_data->pngIDAT.chunk_remain_bytes == 0)
		{
			png_info_data->pngIDAT.chunk_length = ((unsigned int)bs_buf_ptr[png_info_data->pngIDAT.bs_byte_offset + 4] << 24) | 
												((unsigned int)bs_buf_ptr[png_info_data->pngIDAT.bs_byte_offset + 5] << 16) | 
												((unsigned int)bs_buf_ptr[png_info_data->pngIDAT.bs_byte_offset + 6] << 8) | 
												(unsigned int)bs_buf_ptr[png_info_data->pngIDAT.bs_byte_offset + 7];
			png_info_data->pngIDAT.chunk_remain_bytes = png_info_data->pngIDAT.chunk_length;
			png_info_data->pngIDAT.bs_byte_offset += 12;
		}
		png_info_data->pngIDAT.chunk_remain_bytes -= 1;
		png_info_data->pngIDAT.hold_bits += (unsigned int)bs_buf_ptr[png_info_data->pngIDAT.bs_byte_offset] << png_info_data->pngIDAT.bs_bit_offset;
		png_info_data->pngIDAT.bs_bit_offset += 8;
		png_info_data->pngIDAT.bs_byte_offset += 1;
	}
}

unsigned int PNG_get_bs_bits(
	PNG_INFO *png_info_data, 
	unsigned int len)
{
	return (unsigned int)(png_info_data->pngIDAT.hold_bits & ((0x1 << len) - 1));
}

void PNG_drop_bs_bits(
	PNG_INFO *png_info_data, 
	unsigned int len)
{
	png_info_data->pngIDAT.hold_bits >>= len;
	png_info_data->pngIDAT.bs_bit_offset -= len;
}

void PNG_init_bs_bits(
	PNG_INFO *png_info_data)
{
	png_info_data->pngIDAT.hold_bits = 0;
	png_info_data->pngIDAT.bs_bit_offset = 0;
}

void PNG_inflate_table(
	PNG_INFO *png_info_data, 
	unsigned int codes)
{
	unsigned int i, j, k;
	unsigned short size;
	unsigned short code;

	// accumulate lengths for codes (assumes lens[] all in 0~15)
	for(i=0; i<16; i++)
		png_info_data->pngIDAT.count[i] = 0;

	for(i=0; i<codes; i++)
		png_info_data->pngIDAT.count[png_info_data->pngIDAT.lens[i]] += 1;

	// generate offsets into symbol table for each length for sorting
	png_info_data->pngIDAT.offset[1] = 0;
	for(i=1; i<15; i++)
		png_info_data->pngIDAT.offset[i + 1] = png_info_data->pngIDAT.offset[i] + png_info_data->pngIDAT.count[i];

	// record the number of symbols with zero length
	png_info_data->pngIDAT.zero_length = 0;

	// sort symbols by length, by symbol order within each length
	for(i=0; i<codes; i++)
	{
		if(png_info_data->pngIDAT.lens[i] != 0)
		{
			png_info_data->pngIDAT.sym_table[png_info_data->pngIDAT.offset[png_info_data->pngIDAT.lens[i]]] = i;
			png_info_data->pngIDAT.offset[png_info_data->pngIDAT.lens[i]] += 1;
		}
		else
			png_info_data->pngIDAT.zero_length += 1;
	}

	// generate look ahead table
	for(i=0; i<288; i++)
		png_info_data->pngIDAT.huffcode[i] = 0;

	for (i=0; i<16; i++)
	{
		png_info_data->pngIDAT.mincode[i] = 0;
		png_info_data->pngIDAT.valptr[i] = 0;
	}

	// Figure C.1: Generate Huffman code sizes
	k = 0;
	for(j=1; j<16; j++)
	{
		for(i=1; i<=png_info_data->pngIDAT.count[j]; i++)
		{
			png_info_data->pngIDAT.huffsize[k] = j;
			k++;
		}
	}
	png_info_data->pngIDAT.huffsize[k] = 0;

	// Figure C.2: Generate Huffman codes
	code = 0;
	size =png_info_data->pngIDAT.huffsize[0];
	k = 0;
	while(png_info_data->pngIDAT.huffsize[k])
	{
		while(png_info_data->pngIDAT.huffsize[k] == size)
		{
			png_info_data->pngIDAT.huffcode[k] = code;
			k++;
			code++;

			if(k >= 288)
				break;
		}
		code <<= 1;
		size++;

		if((k >= 288) || (png_info_data->pngIDAT.huffsize[k] < size))
			break;
	}

	// Figure F.15: Decode table generation
	k = 0;
	code = 0;
	for(j=1; j<16; j++)
	{
		if(png_info_data->pngIDAT.count[j])
		{
			png_info_data->pngIDAT.valptr[j] = k;
			png_info_data->pngIDAT.mincode[j] = png_info_data->pngIDAT.huffcode[k];
			code = png_info_data->pngIDAT.huffcode[k] + png_info_data->pngIDAT.count[j];
			k += png_info_data->pngIDAT.count[j];
		}
		else
		{
			code <<= 1;
			png_info_data->pngIDAT.mincode[j] = code;
			png_info_data->pngIDAT.valptr[j] = k;
		}
	}
}

void PNG_write_len_table(
	PNG_INFO *png_info_data)
{
	unsigned int i, temp32;
	unsigned int time_out = 1000; // 1000 us = 1 ms

	// Set burst mode as '1' to prevent DMA mover writing DMA buffer 1
	temp32 = 0xc000;			// {1'b1, 1'b1, 5'd0, 9'd0}
	HWREG_W(png_huf_tb_ctrl, temp32);

	// Check if DMA buffer 1 is busy (DMA mover is already runing)
	do
	{
		if(time_out == 0)
		{
			printf("WARNING: Wait DMA mover to finish jobs: time out!");
			break;
		}

		udelay(1);

		temp32 = HWREG_R(png_dec_ctrl);
		//DEBUG("png_dec_ctrl = 0x%08x\n", temp32);

		time_out--;
	}while(temp32 & 0x2); // Wait DMA mover to finish jobs

	// Write length symbol
	temp32 = 0xc000; // {1'b1, 1'b1, 5'd0, 9'd0}
	HWREG_W(png_huf_tb_ctrl, temp32);

	// (nlen - zero_lenght) + 1 symobls, write one more symbol to pack last symbol into a word
	for(i=0; i<=(png_info_data->pngIDAT.nlen - png_info_data->pngIDAT.zero_length); i++)
	{
		temp32 = png_info_data->pngIDAT.sym_table[i];
		HWREG_W(png_huf_tb_data, temp32);
	}

	// Write length mincode
	temp32 = 0xc000 + 0x180;	// {1'b1, 1'b1, 5'd0, 9'h180}
	HWREG_W(png_huf_tb_ctrl, temp32);

	for(i=1; i<16; i++)
	{
		temp32 = png_info_data->pngIDAT.mincode[i];
		HWREG_W(png_huf_tb_data, temp32);
	}

	// Write length group base
	temp32 = 0xc000 + 0x190;	// {1'b1, 1'b1, 5'd0, 9'h190}
	HWREG_W(png_huf_tb_ctrl, temp32);

	for(i=1; i<16; i++)
	{
		temp32 = png_info_data->pngIDAT.valptr[i];
		HWREG_W(png_huf_tb_data, temp32);
	}

	// Set burst mode as '0' to allow DMA mover writing DMA buffer 1
	temp32 = 0x0000;			// {1'b0, 1'b0, 5'd0, 9'd0}
	HWREG_W(png_huf_tb_ctrl, temp32);
}

void PNG_write_dis_table(
	PNG_INFO *png_info_data)
{
	unsigned int i, temp32;
	unsigned int time_out = 1000; // 1000 us = 1 ms

	// Set burst mode as '1' to prevent DMA mover writing DMA buffer 1
	temp32 = 0xc000; // {1'b1, 1'b1, 5'd0, 9'd0}
	HWREG_W(png_huf_tb_ctrl, temp32);

	// Check if DMA buffer 1 is busy (DMA mover is already runing)
	do
	{
		if(time_out == 0)
		{
			printf("WARNINGW: ait DMA mover to finish jobs: time out!");
			break;
		}

		udelay(1);

		temp32 = HWREG_R(png_dec_ctrl);
		//DEBUG("png_dec_ctrl = 0x%08x\n", temp32);

		time_out--;
	}while(temp32 & 0x2); // Wait DMA mover to finish jobs

	// Write distance symbol
	temp32 = 0xc000 + 0x1c0; // {1'b1, 1'b1, 5'd0, 9'h1c0}
	HWREG_W(png_huf_tb_ctrl, temp32);

	// (nlen - zero_lenght) + 1 symobls, write one more symbol to pack last symbol into a word
	for(i=0; i<=(png_info_data->pngIDAT.ndist - png_info_data->pngIDAT.zero_length); i++)
	{
		temp32 = png_info_data->pngIDAT.sym_table[i];
		HWREG_W(png_huf_tb_data, temp32);
	}

	// Write distance mincode
	temp32 = 0xc000 + 0x1a0; // {1'b1, 1'b1, 5'd0, 9'h1a0}
	HWREG_W(png_huf_tb_ctrl, temp32);

	for(i=1; i<16; i++)
	{
		temp32 = png_info_data->pngIDAT.mincode[i];
		HWREG_W(png_huf_tb_data, temp32);
	}

	// Write distance group base
	temp32 = 0xc000 + 0x1b0; // {1'b1, 1'b1, 5'd0, 9'h1b0}
	HWREG_W(png_huf_tb_ctrl, temp32);

	for(i=1; i<16; i++)
	{
		temp32 = png_info_data->pngIDAT.valptr[i];
		HWREG_W(png_huf_tb_data, temp32);
	}

	// Set burst mode as '0' to allow DMA mover writing DMA buffer 1
	temp32 = 0x0000; // {1'b0, 1'b0, 5'd0, 9'd0}
	HWREG_W(png_huf_tb_ctrl, temp32);
}

void PNG_create_Huffman_tables(
	PNG_INFO *png_info_data, 
	unsigned char *bs_buf_ptr)
{
	unsigned int i, k;
	unsigned int root;
	unsigned int max;
	unsigned int min;
	unsigned int huff;
	unsigned int len;
	unsigned int val;
	unsigned int incr;
	unsigned int fill;
	unsigned int copy;

	PNG_need_bs_bits(png_info_data, bs_buf_ptr, 14);

	png_info_data->pngIDAT.nlen = PNG_get_bs_bits(png_info_data, 5) + 257;
	PNG_drop_bs_bits(png_info_data, 5);
	//printf("DEBUG: nlen = 0x%08x\n", png_info_data->pngIDAT.nlen);

	png_info_data->pngIDAT.ndist = PNG_get_bs_bits(png_info_data, 5) + 1;
	PNG_drop_bs_bits(png_info_data, 5);
	//printf("DEBUG: ndist = 0x%08x\n", png_info_data->pngIDAT.ndist);

	png_info_data->pngIDAT.ncode = PNG_get_bs_bits(png_info_data, 4) + 4;
	PNG_drop_bs_bits(png_info_data, 4);
	//printf("DEBUG: ncode = 0x%08x\n", png_info_data->pngIDAT.ncode);

	for(i=0; i<png_info_data->pngIDAT.ncode; i++)
	{
		PNG_need_bs_bits(png_info_data, bs_buf_ptr, 3);
		png_info_data->pngIDAT.lens[g_uiCodeLength[i]] = PNG_get_bs_bits(png_info_data, 3);
		PNG_drop_bs_bits(png_info_data, 3);
	}

	while(i < 19)
	{
		png_info_data->pngIDAT.lens[g_uiCodeLength[i]] = 0;
		i++;
	}

	// accumulate lengths for codes (assumes lens[] all in 0~15)
	for(i=0; i<16; i++)
		png_info_data->pngIDAT.count[i] = 0;

	for(i=0; i<19; i++)
		png_info_data->pngIDAT.count[png_info_data->pngIDAT.lens[i]]++;

	// generate offsets into symbol table for each length for sorting
	png_info_data->pngIDAT.offset[1] = 0;
	for(i=1; i<15; i++)
		png_info_data->pngIDAT.offset[i + 1] = png_info_data->pngIDAT.offset[i] + png_info_data->pngIDAT.count[i];

	// sort symbols by length, by symbol order within each length
	for(i=0; i<19; i++)
	{
		if(png_info_data->pngIDAT.lens[i] != 0)
		{
			png_info_data->pngIDAT.sym_table[png_info_data->pngIDAT.offset[png_info_data->pngIDAT.lens[i]]] = i;
			png_info_data->pngIDAT.offset[png_info_data->pngIDAT.lens[i]]++;
		}
	}

	root = 7;

	max = 15;
	while(png_info_data->pngIDAT.count[max] == 0 && max > 1)
		max--;

	//if(root > max)
	//	root = max;

	min = 1;
	while(png_info_data->pngIDAT.count[min] == 0 && min < 15)
		min++;

	if(root < min)
		root = min;

	/* Create Huffman table for code-length alphabet */
	/* Huffman codes are detected from "right" to "left", the Huffman
		table here is built with constant length entries, so that we can
		use constant length stream as table address to get a symbol */
	huff = 0;				/* starting code */
	k = 0;				/* starting code symbol */
	len = min;			/* starting code length */

	while(!(png_info_data->pngIDAT.count[len] == 0 && len == max))
	{
		val = png_info_data->pngIDAT.sym_table[k];

		/* replicate for those indices with low len bits equal to huff */
		incr = 1 << len;
		fill = 1 << root;
		min = fill;				/* save offset to next table */

		fill = fill - incr;
		png_info_data->pngIDAT.len_table_val[huff + fill] = val;
		png_info_data->pngIDAT.len_table_bits[huff + fill] = len;

		while(fill != 0)
		{
			fill = fill - incr;
			png_info_data->pngIDAT.len_table_val[huff + fill] = val;
			png_info_data->pngIDAT.len_table_bits[huff + fill] = len;
		}

		/* backwards increment the len-bit code huff */
		// Huffman codes are detected from 'right' to 'left'
		incr = 1 << (len - 1);
		while(huff & incr)
			incr >>= 1;
		if(incr != 0)
		{
			huff &= (incr - 1);
			huff += incr;
		}
		else
			huff = 0;

		/* go to next symbol, update count, len */
		k++;
		png_info_data->pngIDAT.count[len]--;

		/* get length of next symbol in sym_table */
		while (png_info_data->pngIDAT.count[len] == 0 && len < max)
			len = png_info_data->pngIDAT.lens[png_info_data->pngIDAT.sym_table[k]];
	}

	i = 0;
	while(i < (png_info_data->pngIDAT.nlen + png_info_data->pngIDAT.ndist))
	{
		// Can NOT use PNG_need_bs_bits(7) because it may across chunks
		// if root is set as max, the index bits should be dynamically adjusted
		len = png_info_data->pngIDAT.len_table_bits[PNG_get_bs_bits(png_info_data, 7)];

		while(len > png_info_data->pngIDAT.bs_bit_offset)
		{
			PNG_pull_bs_byte(png_info_data, bs_buf_ptr);
			len = png_info_data->pngIDAT.len_table_bits[PNG_get_bs_bits(png_info_data, 7)];
		}

		val = png_info_data->pngIDAT.len_table_val[PNG_get_bs_bits(png_info_data, 7)];

		if (val < 16)
		{
			png_info_data->pngIDAT.lens[i] = val;
			PNG_need_bs_bits(png_info_data, bs_buf_ptr, len);
			PNG_drop_bs_bits(png_info_data, len);
			i++;
		}
		else
		{
			if(val == 16)
			{
				PNG_need_bs_bits(png_info_data, bs_buf_ptr, (len + 2));
				PNG_drop_bs_bits(png_info_data, len);
				copy = 3 + PNG_get_bs_bits(png_info_data, 2);
				PNG_drop_bs_bits(png_info_data, 2);
				len = png_info_data->pngIDAT.lens[i - 1];
			}
			else if(val == 17)
			{
				PNG_need_bs_bits(png_info_data, bs_buf_ptr, (len + 3));
				PNG_drop_bs_bits(png_info_data, len);
				copy = 3 + PNG_get_bs_bits(png_info_data, 3);
				PNG_drop_bs_bits(png_info_data, 3);
				len = 0;
			}
			else // val = 18
			{
				PNG_need_bs_bits(png_info_data, bs_buf_ptr, (len + 7));
				PNG_drop_bs_bits(png_info_data, len);
				copy = 11 + PNG_get_bs_bits(png_info_data, 7);
				PNG_drop_bs_bits(png_info_data, 7);
				len = 0;
			}

			while(copy != 0)
			{
				png_info_data->pngIDAT.lens[i] = len;
				copy--;
				i++;
			}
		}
	}

	// build literal/lenght table
	PNG_inflate_table(png_info_data, png_info_data->pngIDAT.nlen);

	PNG_write_len_table(png_info_data);

	// build distance table
	for(i=0; i<png_info_data->pngIDAT.ndist; i++)
		png_info_data->pngIDAT.lens[i] = png_info_data->pngIDAT.lens[png_info_data->pngIDAT.nlen + i];

	PNG_inflate_table(png_info_data, png_info_data->pngIDAT.ndist);

	PNG_write_dis_table(png_info_data);
}

void PNG_parse_block_headers(
	PNG_INFO *png_info_data, 
	unsigned char *bs_buf_ptr)
{
	PNG_need_bs_bits(png_info_data, bs_buf_ptr, 3);

	png_info_data->pngIDAT.block_final = (unsigned char)(PNG_get_bs_bits(png_info_data, 1)); // BFINAL
	PNG_drop_bs_bits(png_info_data, 1);
	//printf("DEBUG: block_final = 0x%02x\n", png_info_data->pngIDAT.block_final);

	png_info_data->pngIDAT.block_type = (unsigned char)(PNG_get_bs_bits(png_info_data, 2)); // BTYPE
	PNG_drop_bs_bits(png_info_data, 2);
	//printf("DEBUG: block_type = 0x%02x\n", png_info_data->pngIDAT.block_type);

	// Set block type, and last block
	HWREG_W(png_blk_info, ((unsigned int)(png_info_data->pngIDAT.block_type & 0x3) << 1) | 
													(unsigned int)(png_info_data->pngIDAT.block_final & 0x1));

	switch(png_info_data->pngIDAT.block_type)
	{
		case 0:
			//printf("DEBUG: Uncomporessed block.\n");
			break;

		case 1:
			//printf("DEBUG: Comporessed block with fixed Huffman codes.\n");
			break;

		case 2:
			//printf("DEBUG: Comporessed block with dynamic Huffman codes.\n");
			PNG_create_Huffman_tables(png_info_data, bs_buf_ptr);
			break;

		default:
			printf("WARNING: Invalid block type.\n");
			break;
	}
}

//===============================================================

void imgdec_clock_enable(void)
{
}

void imgdec_clock_disable(void)
{
}

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
	unsigned int *exe_mode)
{
	// for png_dec_mode:
	//unsigned int bs_endian = 0; // DMA bitstream use 0: big-endian, 1: little-endian.
	unsigned int bs_dma_size = 4; // 32 x 4byte DMA size (128 bytes)
	//unsigned int gray_out_format = 0; // PNG_ARGB
	unsigned int zlib_mode = 1; // 0: decoding PNG images, 1: decoding ZLIB streams.
	//unsigned int dma_pixel_buffers = 0; // 0: single pixel buffer, 1: ping-pong pixel buffer.
	//unsigned int pixel_buffer_select = 0; // 0: use 1st 512B DMA SRAM as pixel buffer, 1: use 2nd ...
	//unsigned int palette_output = 1; // 0: output palette index, 1: output RGB value (in ARGB format).
	//unsigned int output_swap = 0; // 0: big-endian, 1: little-endian.
	int ret = 0;

	// for png_img_height:
	unsigned int height;

	// for bistream address and size:
	unsigned int bsbuf_phyaddr_startptr;
	unsigned int bsbuf_phyaddr_endptr;
	unsigned int chunk_length = 0;

	// for decoding loop:
	unsigned int bs_offset;
	unsigned char loop_stop;
	unsigned int time_out = 1000000; // 1000000 us = 1 sec
	unsigned int png_interrupt;
	unsigned int reg_read_data;
	unsigned int adler_chksum;

	if((exe_mode) && (*exe_mode == 2))
		goto check_png_dec_completion;

	if(bs_block_num == 0) // for first bitstream block
	{
		// init. PNG_INFO data structure
		memset((void *)png_info_data, 0, sizeof(PNG_INFO));

		// check ZLIB/Scramble headers  
		png_info_data->pngIDAT.zlib_header = (unsigned short)bs_buf_ptr[0] << 8 | (unsigned short)bs_buf_ptr[1];
		if(png_info_data->pngIDAT.zlib_header != 0x5350)
		{
			printf("ERR: Invalid ZLIB/Scrambler header\n");
			return -1;
		}

		// enable IMGDEC clock
		imgdec_clock_enable();

		// Assert software reset
		HWREG_W(png_dec_mode, 0x0001);

		// Set DMA buffer size and zlib mode
		HWREG_W(png_dec_mode, //(bs_endian << 1) | 
									((bs_dma_size & 0x7) << 2) |
									//((gray_out_format & 0x3) << 5) |
									(zlib_mode << 7)); // |
									//(dma_pixel_buffers << 9) |
									//(pixel_buffer_select << 10) |
									//(palette_output << 11) |
									//(output_swap << 12));

		// Set bitstream buffer base address
		HWREG_W(png_bs_buf_addr, 0x0000);

		// Set Huffman table base address
		HWREG_W(png_huf_tb_addr, 0x0020); // size = 0x0020 - 0x0000 = 32 words

		// Set scanline buffer base address
		HWREG_W(png_scl_buf_addr, 0x00c0); // size = 0x00c0 - 0x0020 = 160 words

		// Set backward data external base address
		HWREG_W(png_bw_ext_addr_m, LZ_backward_buf_phyaddr >> 16);
		HWREG_W(png_bw_ext_addr_l, LZ_backward_buf_phyaddr & 0xffff);

		// Set scanline data external base address
		HWREG_W(png_scl_ext_addr_m, scanline_buf_phyaddr >> 16);
		HWREG_W(png_scl_ext_addr_l, scanline_buf_phyaddr & 0xffff);

		// Set decoded pixel data external base address
		HWREG_W(png_pxl_ext_addr_m, decoded_buf_phyaddr >> 16);
		HWREG_W(png_pxl_ext_addr_l, decoded_buf_phyaddr & 0xffff); // bit[0]: reset flag

		// Set scramble key
		HWREG_W(png_rem_chunk_size_m, (unsigned int)scramble_key);

		// Set decompressed data size, unit: K-byte
		height = (unsigned int)bs_buf_ptr[2] << 8 | (unsigned int)bs_buf_ptr[3];
		HWREG_W(png_img_width, 1024 & 0xffff);
		HWREG_W(png_img_height, height & 0xffff);

		// Set number of render lines (minus 1)
		HWREG_W(png_pxl_line, height - 1);

		// Set color type and bit-depth as 8-bit palette to avoid output translation
		HWREG_W(png_img_type, ((3&0x7) << 5) | 8); // 0xe8

		chunk_length = (unsigned int)bs_buf_ptr[4] << 24 | 
									(unsigned int)bs_buf_ptr[5] << 16 | 
									(unsigned int)bs_buf_ptr[6] << 8 | 
									(unsigned int)bs_buf_ptr[7];
		chunk_length = chunk_length + 8 + (chunk_length % 2); // 8 = 2(header) + 2(height) + 4(chunk_length), %2 for 2-bytes scramble unit

		// Set chunk size
		chunk_length = chunk_length + 6; // ???
		HWREG_W(png_chunk_size_m, chunk_length >> 16);
		HWREG_W(png_chunk_size_l, chunk_length & 0xffff);
	}

	// Set bistream external memory start address
	bsbuf_phyaddr_startptr = bs_buf_phyaddr;
	HWREG_W(png_bs_str_addr_m, bsbuf_phyaddr_startptr >> 16);
	HWREG_W(png_bs_str_addr_l, bsbuf_phyaddr_startptr & 0xffff);

	// Set bistream external memory end address
	//bsbuf_phyaddr_endptr = (bsbuf_phyaddr_startptr + chunk_length + 0x7f) & ~0x7f;
	bsbuf_phyaddr_endptr = (bsbuf_phyaddr_startptr + bs_block_size + 0x7f) & ~0x7f;
	HWREG_W(png_bs_end_addr_m, bsbuf_phyaddr_endptr >> 16);
	HWREG_W(png_bs_end_addr_l, bsbuf_phyaddr_endptr & 0xffff);

	if(bs_block_num == 0) // for first bitstream block
	{
		// for decoding:
		PNG_init_bs_bits(png_info_data);
		png_info_data->pngIDAT.bs_byte_offset = 8;
		png_info_data->pngIDAT.chunk_remain_bytes = chunk_length - 8;

		PNG_parse_block_headers(png_info_data, bs_buf_ptr);

		// Write offset value of current position from chunk type
		bs_offset = (png_info_data->pngIDAT.bs_byte_offset << 3) - png_info_data->pngIDAT.bs_bit_offset;
		if((bs_offset%32) != 0)
			bs_offset = bs_offset + (32 - (bs_offset%32));
		HWREG_W(png_bs_offset, bs_offset & 0x7fff); // [14:0]

		// Start PNG decoder
		HWREG_W(png_dec_ctrl, 0x0001);
	}
	else
	{
		// Clear interrupt to continue loop after the last "Bitstream empty interrupt".
		HWREG_W(png_interrupt, 0x0100);
	}

	if((exe_mode) && (*exe_mode == 1))
		return 0;   // Just start PNG-DEC h/w, don't wait for completion.

check_png_dec_completion:

	loop_stop = false;
	while(!loop_stop)
	{
		if(time_out == 0)
		{
			printf("ERR: ZLIB decoding time out!");
			// disable IMGDEC clock for decoding fail!
			imgdec_clock_disable();
			return -1;
		}
		udelay(1);
		time_out--;

		png_interrupt = HWREG_R(png_interrupt);

		if(png_interrupt & 0x1) // Bitstream empty interrupt.
		{
			// If input file has already been loaded into SDRAM, it means decode error!
#if defined(SP_PNG_DEC_VERBOSE)
			printf("DEBUG: Bitstream empty interrupt!\n");
#endif

			// ret = 0;

			// finish decoding this zlib bitstream block
			loop_stop = true;

			// Clear interrupt
			//HWREG_W(png_interrupt, 0x0100); // do it next API calling
		}
		else if(png_interrupt & 0x2) // End of block interrupt.
		{
#if defined(SP_PNG_DEC_VERBOSE)
			printf("DEBUG: End of block interrupt!\n");
#endif

			ret = 1;

			// Clear interrupt
			HWREG_W(png_interrupt, 0x0200);
		}
		else if(png_interrupt & 0x4) // End of chunk interrupt.
		{
			// No need to start decoder becase EOC does NOT de-assert dec_start
#if defined(SP_PNG_DEC_VERBOSE)
			printf("DEBUG: End of chunk interrupt!\n");
#endif

			ret = 2;

			// Clear interrupt
			HWREG_W(png_interrupt, 0x0400);
		}
		else if(png_interrupt & 0x8) // End of ZLIB interrupt.
		{
			//DEBUG("PNG end of ZLIB interrupt!\n");
#if defined(SP_PNG_DEC_VERBOSE)
			printf("DEBUG: End of ZLIB interrupt!\n");
#endif

			ret = 3;

			// finish decoding whole zlib
			loop_stop = true;

			// Clear interrupt
			HWREG_W(png_interrupt, 0x0800);

			// disable IMGDEC clock to finish decoding
			imgdec_clock_disable();
		}
		else if(png_interrupt & 0x10) // End of image interrupt.
		{
#if defined(SP_PNG_DEC_VERBOSE)
			printf("DEBUG: End of image interrupt!\n");
#endif

			ret = 4;

			// finish decoding whole picture
			loop_stop = true;

			// Clear interrupt
			HWREG_W(png_interrupt, 0x1000);

			// Read Adler32 checksum value
			HWREG_W(png_probe_addr, 0x0000);
			reg_read_data = HWREG_R(png_probe_value);
			HWREG_W(png_probe_addr, 0x0001);
			adler_chksum = HWREG_R(png_probe_value);
			adler_chksum = (reg_read_data << 16) | adler_chksum;
#if defined(SP_PNG_DEC_VERBOSE)
			printf("DEBUG: PNG Adler32 checksum: 0x%08x\n", adler_chksum);
#endif
			if(png_interrupt & 0x40) // Decode error interrupt.
			{
				// Check error type	
				reg_read_data = HWREG_R(png_dec_ctrl);
				if(reg_read_data & 0x02)
					printf("ERR: PNG decode error interrupt: Error Type = DMA busy!\n");
				else if(reg_read_data & 0x04)
					printf("ERR: PNG decode error interrupt: Error Type = Chunk Type Error!\n");
				else if(reg_read_data & 0x08)
					printf("ERR: PNG decode error interrupt: Error Type = Adler Checksum Error!\n");
				else if(reg_read_data & 0x10)
					printf("ERR: PNG decode error interrupt: Error Type = Decode Suspending Error!\n");
				else if(reg_read_data & 0x20)
					printf("ERR: PNG decode error interrupt: Error Type = Non-compressed Block Length Error!\n");
				else
					printf("ERR: PNG decode error interrupt: Error Type = Unknown Type Error!\n");

				// disable IMGDEC clock for decoding fail!
				imgdec_clock_disable();
				return -1;
			}

			// disable IMGDEC clock to finish decoding
			imgdec_clock_disable();
		}
		else if(png_interrupt & 0x20) // Pixel buffer full (ready) interrupt.
		{
			// If ouput buffer has enough big size in SDRAM, it means decode error!
			printf("ERR: PNG Pixel buffer full (ready) interrupt!\n");

			// Clear interrupt
			HWREG_W(png_interrupt, 0x2000);

			// finish decoding whole picture
			loop_stop = true;

			// disable IMGDEC clock for decoding fail!
			imgdec_clock_disable();
			return -1;
		}
		else if(png_interrupt & 0x40) // Decode error interrupt.
		{
			// Check error type
			reg_read_data =  HWREG_R(png_dec_ctrl);
			if(reg_read_data & 0x02)
				printf("ERR: PNG decode error interrupt: Error Type = DMA busy!\n");
			else if(reg_read_data & 0x04)
				printf("ERR: PNG decode error interrupt: Error Type = Chunk Type Error!\n");
			else if(reg_read_data & 0x08)
				printf("ERR: PNG decode error interrupt: Error Type = Adler Checksum Error!\n");
			else if(reg_read_data & 0x10)
				printf("ERR: PNG decode error interrupt: Error Type = Decode Suspending Error!\n");
			else if(reg_read_data & 0x20)
				printf("ERR: PNG decode error interrupt: Error Type = Non-compressed Block Length Error!\n");
			else
				printf("ERR: PNG decode error interrupt: Error Type = Unknown Type Error!\n");

			// disable IMGDEC clock for decoding fail!
			imgdec_clock_disable();
			return -1;
		}

		if((exe_mode) && (*exe_mode == 2)) {
			if(loop_stop == true) {
				*exe_mode |= (1 << 31); // Decode is done.
			}
			return ret; // Just check if decoding is done, don't wait for completion.
		}

	}

	return ret;
}
