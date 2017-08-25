
#ifndef __MACH_IMGDECSND_H__
#define __MACH_IMGDECSND_H__

// IMGDECSND IOCTL define:
#define IMGDECSND_CMD_TYPE				'I'
#define IMGDECSND_CMD_IMAGE_PARSE		_IOWR(IMGDECSND_CMD_TYPE, 0x01, unsigned int)
#define IMGDECSND_CMD_IMAGE_DECODE		_IOWR(IMGDECSND_CMD_TYPE, 0x02, unsigned int)
#define IMGDECSND_CMD_IMAGE_SCALE		_IOWR(IMGDECSND_CMD_TYPE, 0x03, unsigned int)
/**************************************************************************************************
Notes for IMGDECSND_CMD_IMAGE_PARSE:
(1) This driver only supports JPEG parser.

Notes for IMGDECSND_CMD_IMAGE_DECODE:
(1) This driver only supports JPEG decoder.
(2) This driver only supports image (input) size in range as 65535 >= width/hieght >= 16.
(3) This driver only supports render (output) size in range as  8192 >= width/hieght >= 16.
(4) This driver supports default Huffman table for Motion-JPEG.
(5) This driver only supports YUV420, RGB565, RGBA8888 and ARGB8888 output formats for JPEG pixel data.
(6) In parameters, all buffers with "phyaddr" word are allocated by chunk memory.

Notes for IMGDECSND_CMD_IMAGE_SCALE:
(1) The align limits of parameters are commented in right side of each parameter. 
(2) This driver extends output line pitch to 8191 pixels at ARGB32.
(3) This driver extends auto-fetch input line pitch to 65535*4 bytes.
(4) src_UVbufphyaddr is only used in SCALE_SRC_YUV420, and dst_UVbufphyaddr is only used in SCALE_DST_YUV420.
(5) The YUV420 data in Ybuf as (width*height bytes) YYY..., and UVbuf as (width*height/2 bytes) UVUVUV....
(6) The YUY2 data in Ybuf as (width*height*2 bytes) YUYVYUYVYUYV....
(7) The YUVBW data in Ybuf as (width*height bytes) YYY..., and UVbuf as (width*height bytes) all 0x80 (= gray image). 
(8) In parameters, all buffers with "phyaddr" word are allocated by chunk memory.
**************************************************************************************************/


// Data structure for IMGDECSND_CMD_IMAGE_PARSE:
typedef struct _PARSE_INFO_
{
	char *filename;
	unsigned int filebuf_phyaddr;
	unsigned int file_size;
	unsigned char parse_info_log;
	unsigned char image_type;
	unsigned int image_width;
	unsigned int image_height;
	unsigned char image_color;
	unsigned char image_pixel;
} PARSE_INFO;
/**************************************************************************************************
Description of Paramters:
(1) filename: before parsing, user must give image file name and path in this parameter.
(2) filebuf_phyaddr: before parsing, user must put image file data into DRAM chunk buffer.
                              And user must give the buffer physical address in this parameter.
(3) file_size: before parsing, user must choose file input type by this parameter.
                   If file_size = 0, driver will use filename to parse file header.
                   If file_size > 0, driver will use filebuf to parse file header, and file_size must be equal to file size.
(4) parse_info_log: before parsing, user must decide whether let driver to print parsing information log.
                             0(false): driver will not print parsing information log. 
                             1(true): driver will print parsing information log. 
(5) image_type: after parsing, driver will write image type in this paramter.
                        0: JPEG
                        x: Other
(6) image_width: after parsing, driver will write image width in this paramter.
(7) image_height: after parsing, driver will write image height in this paramter.
(8) image_color: after parsing, driver will write image encoding color type in this paramter.
                         If image_type = 0 (JPEG), the image encoding color type as follows:
                             15: YUV_RAW     // 0xf = 1111: raw data
                             0: YUV_444_1x1 // 0x0 = 0000: 444, 1x1
                             1: YUV_422_1X2 // 0x1 = 0001: 422, 1x2
                             4: YUV_422_2X1 // 0x4 = 0100: 422, 2x1
                             5: YUV_411_2X2 // 0x5 = 0101: 411, 2x2
                             8: YUV_411_4X1 // 0x8 = 1000: 411, 4x1
                             2: YUV_411_1X4 // 0x2 = 0010: 411, 1x4
                             7: YUV_BW        // 0x7 = 0111: bwmode (black/white)
(9) image_pixel: after parsing, driver will write image pixel type in this paramter.
                         0: Color
                         1: Gray
                         2: BW
**************************************************************************************************/


// Parameter type for DECODE_INFO.render_type[3:0]:
typedef enum _RENDER_TYPE_
{
	RENDER_YUV420 = 0,
	RENDER_RGB565 = 8,
	RENDER_RGBA8888 = 14,
	RENDER_ARGB8888 = 15,
}RENDER_TYPE;

// Parameter type for DECODE_INFO.render_type[15:13] and SCALE_INFO.dst_type[15:13]:
typedef enum _ORIENTATION_TYPE_
{
	NORMAL = 0,
	RIGHT_ROTATE_90 = 1,
	LEFT_ROTATE_90 = 2,
	ROTATE_180 = 3,
	UP_DOWN_FLIP = 4,
	LEFT_RIGHT_FLIP = 5,
}ORIENTATION_TYPE;

// Parameter type for DECODE_INFO.render_endian:
typedef enum _OUT_ENDIAN_
{
	OUT_BIG_ENDIAN = 0,
	OUT_LITTLE_ENDIAN = 1,
}OUT_ENDIAN;

// Data structure for IMGDECSND_CMD_IMAGE_DECODE:
typedef struct _DECODE_INFO_
{
	char *filename;
	unsigned int filebuf_phyaddr;
	unsigned int file_size;
	unsigned int outbuf_phyaddr;
	unsigned int outUVbuf_phyaddr;
	unsigned int outbuf_width;
	unsigned int outbuf_height;
	unsigned int render_width;
	unsigned int render_height;
	RENDER_TYPE render_type;
	OUT_ENDIAN render_endian;
} DECODE_INFO;
/**************************************************************************************************
Description of Paramters:
(1) filename: before decoding, user must give image file name and path in this parameter.
(2) filebuf_phyaddr: before decoding, user must put image file data into DRAM chunk buffer.
                              And user must give the buffer physical address in this parameter.
(3) file_size: before decoding, user must choose file input type by this parameter.
                   If file_size = 0, driver will use filename to decode image file.
                   If file_size > 0, driver will use filebuf to decode image file, and file_size must be equal to file size.
(4) outbuf_phyaddr: before decoding, user must give output buffer physical address in this parameter.
                              If render_type is RENDER_YUV420, this parameter is for Y output buffer;
                              if render_type is one of three RGB types, this parameter is for whole output buffer.
(5) outUVbuf_phyaddr: before decoding, user must give UV output buffer physical address in this parameter.
                                  This parameter is only for render_type = RENDER_YUV420.
(6) render_width: before decoding, user must give image output width in this parameter.
                          if render_width < image_width, then JR will scale down output image width.
(7) render_height: before decoding, user must give image output height in this parameter.
                            if render_height < image_height, then JR will scale down output image height.
(8) render_type: before decoding, user must give image output format in this parameter.
                         This driver only supports four output formats: 
                         RENDER_YUV420, RENDER_RGB565, RENDER_RGBA8888 and RENDER_ARGB8888.
(9) render_endian: before decoding, user must give output pixel endian type in this parameter.
                         This parameter is only for RGB format: 
                         OUT_BIG_ENDIAN = RGB565      ->  OUT_LITTLE_ENDIAN = BGR565
                         OUT_BIG_ENDIAN = RGBA8888  ->  OUT_LITTLE_ENDIAN = ABGR8888
                         OUT_BIG_ENDIAN = ARGB8888  ->  OUT_LITTLE_ENDIAN = BGRA8888
**************************************************************************************************/


// Parameter type for SCALE_INFO.dst_endian:
typedef enum _SCALE_ENDIAN_
{
	SCALE_BIG_ENDIAN = 0,
	SCALE_LITTLE_ENDIAN = 1,
}SCALE_ENDIAN;

// Parameter type for SCALE_INFO.src_type:
typedef enum _SCALE_SRC_TYPE_
{
	SCALE_SRC_YUV420 = 3,
	SCALE_SRC_YUY2 = 4,
	SCALE_SRC_RGBA8888 = 6,
	SCALE_SRC_ARGB8888 = 7 ,
}SCALE_SRC_TYPE;

// Parameter type for SCALE_INFO.dst_type[3:0]:
typedef enum _SCALE_DST_TYPE_
{
	SCALE_DST_YUV420 = 0,
	SCALE_DST_YUVBW = 1,
	SCALE_DST_RGB565 = 8,
	SCALE_DST_RGBA8888 = 14,
	SCALE_DST_ARGB8888 = 15,
}SCALE_DST_TYPE;

// Data structure for IMGDECSND_CMD_IMAGE_SCALE:
typedef struct _SCALE_INFO_
{
	unsigned int src_Ybufphyaddr; // must be 8-align number
	unsigned int src_UVbufphyaddr; // must be 8-align number
	unsigned int src_bufbpl; // must be 8-align number
	unsigned int src_width; // no align number limit
	unsigned int src_height; // no align number limit
	SCALE_SRC_TYPE src_type; // must be one of SCALE_SRC_TYPE
	SCALE_ENDIAN src_endian; // must be one of SCALE_ENDIAN

	unsigned int clip_x; // must be 4-align number in YUY2, and 2-align number in other
	unsigned int clip_y; // must be 2-align number
	unsigned int clip_width; // must be 2-align number
	unsigned int clip_height; // must be 2-align number

	unsigned int dst_Ybufphyaddr; // must be 8-align number
	unsigned int dst_UVbufphyaddr; // must be 8-align number
	unsigned int dst_bufbpl; // must be 8-align number
	unsigned int dst_bufline; // no align number limit
	SCALE_DST_TYPE dst_type; // must be one of SCALE_DST_TYPE
	SCALE_ENDIAN dst_endian; // must be one of SCALE_ENDIAN

	unsigned int rect_x; // must be 2-align number
	unsigned int rect_y; // must be 2-align number
	unsigned int rect_width; // must be 2-align number
	unsigned int rect_height; // must be 2-align number
} SCALE_INFO;
/**************************************************************************************************
Description of Paramters:
(01) src_Ybufphyaddr: before scaling, user must give source image Y data input buffer physical address in this parameter.
(02) src_UVbufphyaddr: before scaling, user must give source image UV data input buffer physical address in this parameter.
                                   This parameter is only for src_type = SCALE_SRC_YUV420.
(03) src_bufbpl: before scaling, user must give source buffer bpl (bytes per line) in this parameter.
(04) src_width: before scaling, user must give source image width in this parameter.
(05) src_height: before scaling, user must give source image height in this parameter.
(06) src_type: before scaling, user must give source image color type in this parameter.
                     Source image color type must be one of SCALE_SRC_TYPE.
(07) src_endian: before scaling, user must give source pixel endian type in this parameter.
                         This parameter is only for RGB format: 
                         SCALE_BIG_ENDIAN = RGBA8888  ->  SCALE_LITTLE_ENDIAN = ABGR8888
                         SCALE_BIG_ENDIAN = ARGB8888  ->  SCALE_LITTLE_ENDIAN = BGRA8888

(08) clip_x: before scaling, user must give clip start pixel number for source image width in this parameter.
(09) clip_y: before scaling, user must give clip start line number for source image height in this parameter.
(10) clip_width: before scaling, user must give clip width for source image in this parameter.
(11) clip_height: before scaling, user must give clip height for source image in this parameter.

(12) dst_Ybufphyaddr: before scaling, user must give destination image Y data output buffer physical address in this parameter.
(13) dst_UVbufphyaddr: before scaling, user must give destination image UV data output buffer physical address in this parameter.
                                   This parameter is only for dst_type = SCALE_DST_YUV420.
(14) dst_bufbpl: before scaling, user must give destination buffer bpl (bytes per line) in this parameter.
(15) dst_bufline: before scaling, user must give destination buffer total line number in this parameter.
(16) dst_type: before scaling, user must give destination image color type in this parameter.
                     Destination image color type must be one of SCALE_DST_TYPE.
(17) dst_endian: before scaling, user must give destination pixel endian type in this parameter.
                         This parameter is only for RGB format: 
                         SCALE_BIG_ENDIAN = RGB565      ->  SCALE_LITTLE_ENDIAN = BGR565
                         SCALE_BIG_ENDIAN = RGBA8888  ->  SCALE_LITTLE_ENDIAN = ABGR8888
                         SCALE_BIG_ENDIAN = ARGB8888  ->  SCALE_LITTLE_ENDIAN = BGRA8888

(18) rect_x: before scaling, user must give destination buffer start pixel number for image width in this parameter.
(19) rect_y: before scaling, user must give destination buffer start line number for image height in this parameter.
(20) rect_width: before scaling, user must give destination image width in this parameter.
(21) rect_height: before scaling, user must give destination image height in this parameter.
**************************************************************************************************/

#endif // __MACH_IMGDECSND_H__

