
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/miscdevice.h>
#include <mach/kernel.h>
#include <mach/sp_config.h> // for letb8388 module driver enable/disable config
#include <mach/hal/hal_imgdec.h>

/**************************************************************************
 *                           C O N S T A N T S                            *
 **************************************************************************/

#define DONE_DEFAULT_TIMEOUT   30000 // (ms)

static UINT8 MJPGDHTSeg[0x1A4] ={
0xFF,0xC4,0x01,0xA2,
0x00,0x00,0x01,0x05,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,
0x10,0x00,0x02,0x01,0x03,0x03,0x02,0x04,0x03,0x05,0x05,0x04,0x04,0x00,0x00,0x01,
0x7D,0x01,0x02,0x03,0x00,0x04,0x11,0x05,0x12,0x21,0x31,0x41,0x06,0x13,0x51,0x61,
0x07,0x22,0x71,0x14,0x32,0x81,0x91,0xA1,0x08,0x23,0x42,0xB1,0xC1,0x15,0x52,0xD1,
0xF0,0x24,0x33,0x62,0x72,0x82,0x09,0x0A,0x16,0x17,0x18,0x19,0x1A,0x25,0x26,0x27,
0x28,0x29,0x2A,0x34,0x35,0x36,0x37,0x38,0x39,0x3A,0x43,0x44,0x45,0x46,0x47,0x48,
0x49,0x4A,0x53,0x54,0x55,0x56,0x57,0x58,0x59,0x5A,0x63,0x64,0x65,0x66,0x67,0x68,
0x69,0x6A,0x73,0x74,0x75,0x76,0x77,0x78,0x79,0x7A,0x83,0x84,0x85,0x86,0x87,0x88,
0x89,0x8A,0x92,0x93,0x94,0x95,0x96,0x97,0x98,0x99,0x9A,0xA2,0xA3,0xA4,0xA5,0xA6,
0xA7,0xA8,0xA9,0xAA,0xB2,0xB3,0xB4,0xB5,0xB6,0xB7,0xB8,0xB9,0xBA,0xC2,0xC3,0xC4,
0xC5,0xC6,0xC7,0xC8,0xC9,0xCA,0xD2,0xD3,0xD4,0xD5,0xD6,0xD7,0xD8,0xD9,0xDA,0xE1,
0xE2,0xE3,0xE4,0xE5,0xE6,0xE7,0xE8,0xE9,0xEA,0xF1,0xF2,0xF3,0xF4,0xF5,0xF6,0xF7,
0xF8,0xF9,0xFA,
0x01,0x00,0x03,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x00,0x00,0x00,
0x00,0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,
0x11,0x00,0x02,0x01,0x02,0x04,0x04,0x03,0x04,0x07,0x05,0x04,0x04,0x00,0x01,0x02,
0x77,0x00,0x01,0x02,0x03,0x11,0x04,0x05,0x21,0x31,0x06,0x12,0x41,0x51,0x07,0x61,
0x71,0x13,0x22,0x32,0x81,0x08,0x14,0x42,0x91,0xA1,0xB1,0xC1,0x09,0x23,0x33,0x52,
0xF0,0x15,0x62,0x72,0xD1,0x0A,0x16,0x24,0x34,0xE1,0x25,0xF1,0x17,0x18,0x19,0x1A,
0x26,0x27,0x28,0x29,0x2A,0x35,0x36,0x37,0x38,0x39,0x3A,0x43,0x44,0x45,0x46,0x47,
0x48,0x49,0x4A,0x53,0x54,0x55,0x56,0x57,0x58,0x59,0x5A,0x63,0x64,0x65,0x66,0x67,
0x68,0x69,0x6A,0x73,0x74,0x75,0x76,0x77,0x78,0x79,0x7A,0x82,0x83,0x84,0x85,0x86,
0x87,0x88,0x89,0x8A,0x92,0x93,0x94,0x95,0x96,0x97,0x98,0x99,0x9A,0xA2,0xA3,0xA4,
0xA5,0xA6,0xA7,0xA8,0xA9,0xAA,0xB2,0xB3,0xB4,0xB5,0xB6,0xB7,0xB8,0xB9,0xBA,0xC2,
0xC3,0xC4,0xC5,0xC6,0xC7,0xC8,0xC9,0xCA,0xD2,0xD3,0xD4,0xD5,0xD6,0xD7,0xD8,0xD9,
0xDA,0xE2,0xE3,0xE4,0xE5,0xE6,0xE7,0xE8,0xE9,0xEA,0xF2,0xF3,0xF4,0xF5,0xF6,0xF7,
0xF8,0xF9,0xFA
};

/**************************************************************************
 *                              M A C R O S                               *
 **************************************************************************/

/**************************************************************************
 *                          D A T A    T Y P E S                          *
 **************************************************************************/

/**************************************************************************
 *               F U N C T I O N    D E C L A R A T I O N S               *
 **************************************************************************/

static int imgdec_fops_open(struct inode *inode, struct file *pfile);
static int imgdec_fops_release(struct inode *inode, struct file *pfile);
static long imgdec_fops_ioctl(struct file *pfile, unsigned int cmd, unsigned long arg);

static int imgdec_probe(struct platform_device *pdev);
static int imgdec_remove(struct platform_device *pdev);
static int imgdec_suspend(struct platform_device *pdev, pm_message_t state);
static int imgdec_resume(struct platform_device *pdev);

/**************************************************************************
 *                         G L O B A L    D A T A                         *
 **************************************************************************/

static const struct file_operations imgdec_fops =
{
	.owner			= THIS_MODULE,
	.open			= imgdec_fops_open,
	.release			= imgdec_fops_release,
	.unlocked_ioctl	= imgdec_fops_ioctl,
};

static struct platform_device imgdec_device = {
	.name			= "imgdec",
	.id				= -1,
};

static struct platform_driver imgdec_driver = {
	.probe		= imgdec_probe,
	.remove		= imgdec_remove,
	.suspend		= imgdec_suspend,
	.resume		= imgdec_resume,
	.driver		=
	{
		.name	= "imgdec",
		.owner	= THIS_MODULE,
	},
};

struct miscdevice g_imgdec_misc;

JPEG_INFO g_imgdec_jpeg_info;
PNG_INFO g_imgdec_png_info;

wait_queue_head_t g_imgdec_done_wait;
unsigned char g_imgdec_done;

/**************************************************************************
 *             F U N C T I O N    I M P L E M E N T A T I O N S           *
 **************************************************************************/

#if 0
void IMGDEC_Parse_JPEG_APP0(UINT8 *pBufPtr, UINT8 parse_info_log)
{
	UINT32 i, size;

	g_imgdec_jpeg_info.jpegAPP0.length = ((UINT16)pBufPtr[0] << 8) | (UINT16)pBufPtr[1];
	if(parse_info_log)	INFO("length = %d\n", g_imgdec_jpeg_info.jpegAPP0.length);

	memcpy((void *)g_imgdec_jpeg_info.jpegAPP0.identifier, (void *)(pBufPtr+2), 5);
	if(parse_info_log)	INFO("identifier = %s\n", g_imgdec_jpeg_info.jpegAPP0.identifier);

	if( (g_imgdec_jpeg_info.jpegAPP0.identifier[0] != 'J') || (g_imgdec_jpeg_info.jpegAPP0.identifier[1] != 'F') ||
		(g_imgdec_jpeg_info.jpegAPP0.identifier[2] != 'I') || (g_imgdec_jpeg_info.jpegAPP0.identifier[3] != 'F') )
	{
		if(parse_info_log)	WARNING("identifier is not JFIF\n");
	}

	g_imgdec_jpeg_info.jpegAPP0.version = ((UINT16)pBufPtr[7] << 8) | (UINT16)pBufPtr[8];
	if(parse_info_log)	INFO("version = 0x%04x\n", g_imgdec_jpeg_info.jpegAPP0.version);

	g_imgdec_jpeg_info.jpegAPP0.units = pBufPtr[9];
	if(parse_info_log)	INFO("units = %d\n", g_imgdec_jpeg_info.jpegAPP0.units);

	g_imgdec_jpeg_info.jpegAPP0.Xdenity = ((UINT16)pBufPtr[10] << 8) | (UINT16)pBufPtr[11];
	if(parse_info_log)	INFO("Xdenity = %d\n", g_imgdec_jpeg_info.jpegAPP0.Xdenity);

	g_imgdec_jpeg_info.jpegAPP0.Ydenity = ((UINT16)pBufPtr[12] << 8) | (UINT16)pBufPtr[13];
	if(parse_info_log)	INFO("Ydenity = %d\n", g_imgdec_jpeg_info.jpegAPP0.Ydenity);

	g_imgdec_jpeg_info.jpegAPP0.Xthumbnail = pBufPtr[14];
	if(parse_info_log)	INFO("Xthumbnail = %d\n", g_imgdec_jpeg_info.jpegAPP0.Xthumbnail);

	g_imgdec_jpeg_info.jpegAPP0.Ythumbnail = pBufPtr[15];
	if(parse_info_log)	INFO("Ythumbnail = %d\n", g_imgdec_jpeg_info.jpegAPP0.Ythumbnail);

	g_imgdec_jpeg_info.jpegAPP0.RGBn=NULL; // for thumbnail data output ??????
	if(g_imgdec_jpeg_info.jpegAPP0.Xthumbnail && g_imgdec_jpeg_info.jpegAPP0.Ythumbnail)
	{
		size = (UINT32)g_imgdec_jpeg_info.jpegAPP0.Xthumbnail*(UINT32)g_imgdec_jpeg_info.jpegAPP0.Ythumbnail*3; // 3 for RGB
		g_imgdec_jpeg_info.jpegAPP0.RGBn = gp_chunk_malloc_nocache(CHUNK_POOL_INDEX, 0, size);
		if(g_imgdec_jpeg_info.jpegAPP0.RGBn == NULL)
			WARNING("gp_chunk_malloc_nocache (g_imgdec_jpeg_info.jpegAPP0.RGBn) fail!\n");
		else
		{
			for(i=0; i<size; i++)
			{
				g_imgdec_jpeg_info.jpegAPP0.RGBn[i] = pBufPtr[16 + i];
			}
		}
	}
}

void IMGDEC_Parse_JPEG_APP1(UINT8 *pBufPtr, UINT8 parse_info_log) // this function is not finished !!!
{
	UINT32 i, j, pos, TIFF_pos_base;
	UINT8 exif_header[6];
	UINT8 tiff_header[4];
	UINT16 tag, type, exif_ifd_num;
	UINT32 size, value, offset, exif_ifd_value = 0;

	g_imgdec_jpeg_info.jpegAPP1.length = ((UINT16)pBufPtr[0] << 8) | (UINT16)pBufPtr[1];
	if(parse_info_log)	INFO("length = %d\n", g_imgdec_jpeg_info.jpegAPP1.length);

	memcpy((void *)exif_header, (void *)(pBufPtr+2), 6);
	if(parse_info_log)	INFO("exif_header = %s\n", exif_header);

	if((exif_header[0] =='E') && (exif_header[1] =='x') && (exif_header[2] =='i') && (exif_header[3] =='f'))
	{
		g_imgdec_jpeg_info.jpegAPP1.jpeg_exif_format = 1;
	}
	else
	{
		g_imgdec_jpeg_info.jpegAPP1.jpeg_exif_format = 0;
		WARNING("exif_header is not Exif\n");
		return;
	}

	TIFF_pos_base = 8;  // for all TIFF offsets later

	memcpy((void *)tiff_header, (void *)(pBufPtr+8), 4);
	if(parse_info_log)	INFO("Tiff header = %s\n", tiff_header);

	if((tiff_header[0] == 'I') && (tiff_header[1] == 'I'))
	{
		g_imgdec_jpeg_info.jpegAPP1.little_endian = 1;  //TIFF file Intel format (Little-endian)
	}
	else if((tiff_header[0] == 'M') && (tiff_header[1] == 'M'))
	{
		g_imgdec_jpeg_info.jpegAPP1.little_endian = 0;  //TIFF file Motorolla format (Big-endian)
	}
	else
	{
		WARNING("tiff_header is not II or MM\n");
		return;
	}

	g_imgdec_jpeg_info.jpegAPP1.ifd0_offset = 0;
	for(i=0; i<4; i++)
	{
		if(g_imgdec_jpeg_info.jpegAPP1.little_endian == 1)  // Intel format (Little-endian)
			g_imgdec_jpeg_info.jpegAPP1.ifd0_offset = g_imgdec_jpeg_info.jpegAPP1.ifd0_offset | (UINT32)pBufPtr[12+i] << i;
		else // Motorolla format (Big-endian)
			g_imgdec_jpeg_info.jpegAPP1.ifd0_offset = (g_imgdec_jpeg_info.jpegAPP1.ifd0_offset << 8) | (UINT32)pBufPtr[12+i];
	}
	if(parse_info_log)	INFO("ifd0_offset = 0x%08x\n", g_imgdec_jpeg_info.jpegAPP1.ifd0_offset);

	pos = TIFF_pos_base + g_imgdec_jpeg_info.jpegAPP1.ifd0_offset;

	g_imgdec_jpeg_info.jpegAPP1.ifd0_entry_no = 0;
	for(i=0; i<2; i++)
	{
		if(g_imgdec_jpeg_info.jpegAPP1.little_endian == 1)  // Intel format (Little-endian)
			g_imgdec_jpeg_info.jpegAPP1.ifd0_entry_no = g_imgdec_jpeg_info.jpegAPP1.ifd0_entry_no | (UINT16)pBufPtr[pos+i] << i;
		else // Motorolla format (Big-endian)
			g_imgdec_jpeg_info.jpegAPP1.ifd0_entry_no = (g_imgdec_jpeg_info.jpegAPP1.ifd0_entry_no << 8) | (UINT16)pBufPtr[pos+i];
	}
	//if Exif maker not data
	if(!g_imgdec_jpeg_info.jpegAPP1.ifd0_entry_no)
	{
		WARNING("ifd0_entry_no = %d\n", g_imgdec_jpeg_info.jpegAPP1.ifd0_entry_no);
		return;
	}

	pos += 2;
	//Get some information from IFD0
	for(j=0; j<g_imgdec_jpeg_info.jpegAPP1.ifd0_entry_no; j++)
	{
		tag = 0;
		for(i=0; i<2; i++)
		{
			if(g_imgdec_jpeg_info.jpegAPP1.little_endian == 1)  // Intel format (Little-endian)
				tag = tag | (UINT16)pBufPtr[pos+i] << i;
			else // Motorolla format (Big-endian)
				tag = (tag << 8) | (UINT16)pBufPtr[pos+i];
		}
		if(parse_info_log)	INFO("IFD0 tag = 0x%04x\n", tag);

		switch(tag)
		{
			case JPEG_EXIF_IMAGEDESCRIPTION:
			case JPEG_EXIF_MAKE:
			//...
			default:
				for(i=2; i<4; i++)
				{
					if(g_imgdec_jpeg_info.jpegAPP1.little_endian == 1)  // Intel format (Little-endian)
						type = type | (UINT16)pBufPtr[pos+i] << i;
					else // Motorolla format (Big-endian)
						type = (type << 8) | (UINT16)pBufPtr[pos+i];
				}
				for(i=4; i<8; i++)
				{
					if(g_imgdec_jpeg_info.jpegAPP1.little_endian == 1)  // Intel format (Little-endian)
						size = size | (UINT16)pBufPtr[pos+i] << i;
					else // Motorolla format (Big-endian)
						size = (size << 8) | (UINT16)pBufPtr[pos+i];
				}
				value = 0;
				for(i=8; i<12; i++)
				{
					if(g_imgdec_jpeg_info.jpegAPP1.little_endian == 1)  // Intel format (Little-endian)
						value = value | (UINT16)pBufPtr[pos+i] << i;
					else // Motorolla format (Big-endian)
						value = (value << 8) | (UINT16)pBufPtr[pos+i];
				}
				break;
			case JPEG_EXIF_EXIFOFFSET:
				for(i=2; i<4; i++)
				{
					if(g_imgdec_jpeg_info.jpegAPP1.little_endian == 1)  // Intel format (Little-endian)
						type = type | (UINT16)pBufPtr[pos+i] << i;
					else // Motorolla format (Big-endian)
						type = (type << 8) | (UINT16)pBufPtr[pos+i];
				}
				for(i=4; i<8; i++)
				{
					if(g_imgdec_jpeg_info.jpegAPP1.little_endian == 1)  // Intel format (Little-endian)
						size = size | (UINT16)pBufPtr[pos+i] << i;
					else // Motorolla format (Big-endian)
						size = (size << 8) | (UINT16)pBufPtr[pos+i];
				}
				value = 0;
				for(i=8; i<12; i++)
				{
					if(g_imgdec_jpeg_info.jpegAPP1.little_endian == 1)  // Intel format (Little-endian)
						value = value | (UINT16)pBufPtr[pos+i] << i;
					else // Motorolla format (Big-endian)
						value = (value << 8) | (UINT16)pBufPtr[pos+i];
				}
				exif_ifd_value = value;
				break;
		}
		pos += 12;
	}

	if(exif_ifd_value)
	{
		pos = TIFF_pos_base + exif_ifd_value;

		exif_ifd_num = 0;
		for(i=0; i<2; i++)
		{
			if(g_imgdec_jpeg_info.jpegAPP1.little_endian == 1)  // Intel format (Little-endian)
				exif_ifd_num = exif_ifd_num | (UINT16)pBufPtr[pos+i] << i;
			else // Motorolla format (Big-endian)
				exif_ifd_num = (exif_ifd_num << 8) | (UINT16)pBufPtr[pos+i];
		}

		pos += 2;
		//Get some information from EXIF IFD
		for(j=0; j<exif_ifd_num; j++)
		{
			tag = 0;
			for(i=0; i<2; i++)
			{
				if(g_imgdec_jpeg_info.jpegAPP1.little_endian == 1)  // Intel format (Little-endian)
					tag = tag | (UINT16)pBufPtr[pos+i] << i;
				else // Motorolla format (Big-endian)
					tag = (tag << 8) | (UINT16)pBufPtr[pos+i];
			}
			if(parse_info_log)	INFO("EXIF IFD tag = 0x%04x\n", tag);

			switch(tag)
			{
				case JPEG_EXIF_EXPOSURETIME:
				case JPEG_EXIF_FNUMBER:
				//...
				default:
					for(i=2; i<4; i++)
					{
						if(g_imgdec_jpeg_info.jpegAPP1.little_endian == 1)  // Intel format (Little-endian)
							type = type | (UINT16)pBufPtr[pos+i] << i;
						else // Motorolla format (Big-endian)
							type = (type << 8) | (UINT16)pBufPtr[pos+i];
					}
					for(i=4; i<8; i++)
					{
						if(g_imgdec_jpeg_info.jpegAPP1.little_endian == 1)  // Intel format (Little-endian)
							size = size | (UINT16)pBufPtr[pos+i] << i;
						else // Motorolla format (Big-endian)
							size = (size << 8) | (UINT16)pBufPtr[pos+i];
					}
					for(i=8; i<12; i++)
					{
						if(g_imgdec_jpeg_info.jpegAPP1.little_endian == 1)  // Intel format (Little-endian)
							offset = offset | (UINT16)pBufPtr[pos+i] << i;
						else // Motorolla format (Big-endian)
							offset = (offset << 8) | (UINT16)pBufPtr[pos+i];
					}
					break;
			}
			pos += 12;
		}
	}
}
#endif

void IMGDEC_Parse_JPEG_DQT(UINT8 *pBufPtr, UINT8 parse_info_log)
{
	UINT32 i, j, count, pos;

	g_imgdec_jpeg_info.jpegDQT.length = ((UINT16)pBufPtr[0] << 8) | (UINT16)pBufPtr[1];
	if(parse_info_log)	INFO("length = %d\n", g_imgdec_jpeg_info.jpegDQT.length);

	pos = 2;

	count = (g_imgdec_jpeg_info.jpegDQT.length-2)/65;
	for(i=0; i<count; i++)
	{
		if(g_imgdec_jpeg_info.jpegDQT.count > 1)
			//g_imgdec_jpeg_info.jpegDQT.count = 1;
			break;

		g_imgdec_jpeg_info.jpegDQT.Pq[g_imgdec_jpeg_info.jpegDQT.count] = pBufPtr[pos] >> 4; // precision=0 -> 8-bit DQT; =1 -> 16-bit DQT
		if(parse_info_log)	INFO("Pq[%d] = %d\n", g_imgdec_jpeg_info.jpegDQT.count, g_imgdec_jpeg_info.jpegDQT.Pq[g_imgdec_jpeg_info.jpegDQT.count]);

		g_imgdec_jpeg_info.jpegDQT.Tq[g_imgdec_jpeg_info.jpegDQT.count] = pBufPtr[pos] & 0x0f; // table ID
		if(g_imgdec_jpeg_info.jpegDQT.Tq[g_imgdec_jpeg_info.jpegDQT.count] > 1)
			g_imgdec_jpeg_info.jpegDQT.Tq[g_imgdec_jpeg_info.jpegDQT.count] = 1;
		if(parse_info_log)	INFO("Tq[%d] = %d\n", g_imgdec_jpeg_info.jpegDQT.count, g_imgdec_jpeg_info.jpegDQT.Tq[g_imgdec_jpeg_info.jpegDQT.count]);

		pos++;

		if(parse_info_log)	INFO("Qs[%d] =\n", g_imgdec_jpeg_info.jpegDQT.count);
		for(j=0; j<64; j++)
		{
			if(g_imgdec_jpeg_info.jpegDQT.Pq[g_imgdec_jpeg_info.jpegDQT.count] == 0) // for 8-bit DQT
			{
				g_imgdec_jpeg_info.jpegDQT.Qs[g_imgdec_jpeg_info.jpegDQT.count][j] = (UINT16)pBufPtr[pos];
				pos++;
			}
			else if(g_imgdec_jpeg_info.jpegDQT.Pq[g_imgdec_jpeg_info.jpegDQT.count]  == 1) // for 16-bit DQT
			{
				g_imgdec_jpeg_info.jpegDQT.Qs[g_imgdec_jpeg_info.jpegDQT.count][j] = ((UINT16)pBufPtr[pos] << 8) | (UINT16)pBufPtr[pos + 1];
				pos += 2;
			}

			if(parse_info_log)
			{
				printk("0x%04x, ", g_imgdec_jpeg_info.jpegDQT.Qs[g_imgdec_jpeg_info.jpegDQT.count][j]);
				if(j%8 == 7)
					printk("\n");
			}
		}

		g_imgdec_jpeg_info.jpegDQT.count++;
		if(parse_info_log)	INFO("count = %d\n", g_imgdec_jpeg_info.jpegDQT.count);

		if(pos >= g_imgdec_jpeg_info.jpegDQT.length)
			break;
	}
}

void IMGDEC_Parse_JPEG_SOF(UINT8 *pBufPtr, UINT8 parse_info_log)
{
	UINT32 i;

	g_imgdec_jpeg_info.jpegSOF.length = ((UINT16)pBufPtr[0] << 8) | (UINT16)pBufPtr[1];
	if(parse_info_log)	INFO("length = %d\n", g_imgdec_jpeg_info.jpegSOF.length);

	g_imgdec_jpeg_info.jpegSOF.precision = pBufPtr[2]; // bits per pixel per color component
	if(parse_info_log)	INFO("precision = %d\n", g_imgdec_jpeg_info.jpegSOF.precision);

	g_imgdec_jpeg_info.jpegSOF.height = ((UINT16)pBufPtr[3] << 8) | (UINT16)pBufPtr[4]; // image height
	if(parse_info_log)	INFO("height = %d\n", g_imgdec_jpeg_info.jpegSOF.height);

	g_imgdec_jpeg_info.jpegSOF.width = ((UINT16)pBufPtr[5] << 8) | (UINT16)pBufPtr[6]; // image width
	if(parse_info_log)	INFO("width = %d\n", g_imgdec_jpeg_info.jpegSOF.width);

	g_imgdec_jpeg_info.jpegSOF.components = pBufPtr[7]; // number of color components: 1 -> Gray; 3 -> YCbCr/YIQ; 4 -> CMYK;
	if(parse_info_log)	INFO("components = %d\n", g_imgdec_jpeg_info.jpegSOF.components);

	if((g_imgdec_jpeg_info.jpegSOF.components != 1) && (g_imgdec_jpeg_info.jpegSOF.components != 3))
	{
		WARNING("g_imgdec_jpeg_info.jpegSOF.components = %d\n", g_imgdec_jpeg_info.jpegSOF.components);
	}

	for(i=0; i<4; i++)
	{
		g_imgdec_jpeg_info.jpegSOF.identifier[i] =i; // 1:Y; 2:Cb; 3:Cr; 4->I; 5:Q
		g_imgdec_jpeg_info.jpegSOF.Hfactor[i]   = 0; // horizontal sampling factor
		g_imgdec_jpeg_info.jpegSOF.Vfactor[i]   = 0; // vertical sampling factor
		g_imgdec_jpeg_info.jpegSOF.Qtable[i]    = 0;  // QDT ID
	}

	g_imgdec_jpeg_info.jpegSOF.image_type = 0;
	for(i=0; i<g_imgdec_jpeg_info.jpegSOF.components; i++)
	{
		g_imgdec_jpeg_info.jpegSOF.identifier[i] = pBufPtr[8 + i*3]; // 1:Y; 2:Cb; 3:Cr; 4->I; 5:Q
		g_imgdec_jpeg_info.jpegSOF.Hfactor[i]   = pBufPtr[8 + i*3 + 1] >> 4; // horizontal sampling factor
		g_imgdec_jpeg_info.jpegSOF.Vfactor[i]   = pBufPtr[8 + i*3 + 1] & 0x0f; // vertical sampling factor
		g_imgdec_jpeg_info.jpegSOF.Qtable[i]    = pBufPtr[8 + i*3 + 2];  // QDT ID
		if(parse_info_log)	INFO("components[%d] Ci=%d, Hi=%d, Vi=%d, Tqi=%d\n", i, g_imgdec_jpeg_info.jpegSOF.identifier[i],
																				g_imgdec_jpeg_info.jpegSOF.Hfactor[i],
																				g_imgdec_jpeg_info.jpegSOF.Vfactor[i],
																				g_imgdec_jpeg_info.jpegSOF.Qtable[i]);
		if(g_imgdec_jpeg_info.jpegSOF.Qtable[i] > 1)
		{
			g_imgdec_jpeg_info.jpegSOF.Qtable[i] = 1;
			if(parse_info_log)	INFO("reset components[%d] Ci=%d, Hi=%d, Vi=%d, Tqi=%d\n", i, g_imgdec_jpeg_info.jpegSOF.identifier[i],
																				g_imgdec_jpeg_info.jpegSOF.Hfactor[i],
																				g_imgdec_jpeg_info.jpegSOF.Vfactor[i],
																				g_imgdec_jpeg_info.jpegSOF.Qtable[i]);
		}

		g_imgdec_jpeg_info.jpegSOF.image_type = g_imgdec_jpeg_info.jpegSOF.image_type | ((UINT32)pBufPtr[8 + i*3 + 1] << (16-(i<<3)));
	}

	//follow spec., if Ns=1, regardless of the values g_imgdec_jpeg_info.jpegSOF.Hfactor[0] and g_imgdec_jpeg_info.jpegSOF.Vfactor[0]
	//set g_imgdec_jpeg_info.jpegSOF.Hfactor[0] = g_imgdec_jpeg_info.jpegSOF.Vfactor[0] = 1
	if((g_imgdec_jpeg_info.jpegSOF.image_type & 0xffff) == 0)
	{
		g_imgdec_jpeg_info.jpegSOF.image_type = 0x110000;
		g_imgdec_jpeg_info.jpegSOF.Hfactor[0] = 1;
		g_imgdec_jpeg_info.jpegSOF.Vfactor[0] = 1;
	}
	// check image format:
	// YUV400 => 0x110000 or 0x220000,
	// YUV444 => 0x111111,
	// YUV422 => 0x211111 or 0x221212 or 0x121111 or 0x222121,
	// YUV420 => 0x221111
	if((g_imgdec_jpeg_info.jpegSOF.image_type != 0x110000) &&
		(g_imgdec_jpeg_info.jpegSOF.image_type != 0x220000) &&
		(g_imgdec_jpeg_info.jpegSOF.image_type != 0x111111) &&
		(g_imgdec_jpeg_info.jpegSOF.image_type != 0x211111) &&
		(g_imgdec_jpeg_info.jpegSOF.image_type != 0x221212) &&
		(g_imgdec_jpeg_info.jpegSOF.image_type != 0x121111) &&
		(g_imgdec_jpeg_info.jpegSOF.image_type != 0x222121) &&
		(g_imgdec_jpeg_info.jpegSOF.image_type != 0x221111))
	{
		WARNING("JPEG image type = 0x%08x\n", g_imgdec_jpeg_info.jpegSOF.image_type);
	}


}

void IMGDEC_Parse_JPEG_DHT(UINT8 *pBufPtr, UINT8 parse_info_log)
{
	UINT32 i, k;
	UINT8 Tc, Th;
	UINT16 pos;
	UINT16 number, val, offset;

	pos = 0;

	g_imgdec_jpeg_info.jpegDHT.length = ((UINT16)pBufPtr[0] << 8) | (UINT16)pBufPtr[1];
	if(parse_info_log)	INFO("length = %d\n", g_imgdec_jpeg_info.jpegDHT.length);
	pos += 2;

	do
	{
		Tc = pBufPtr[pos] >> 4;
		Th = pBufPtr[pos] & 0x0f;
		pos++;

		val = 0;
		offset = 0;

		if(Tc == 0)  // DC Huffman table
		{
			memset(g_imgdec_jpeg_info.jpegDHT.DC_mincode[Th], 0xff, sizeof(g_imgdec_jpeg_info.jpegDHT.DC_mincode[Th]));
			memset(g_imgdec_jpeg_info.jpegDHT.DC_offset[Th], 0xff, sizeof(g_imgdec_jpeg_info.jpegDHT.DC_offset[Th]));
			memset(g_imgdec_jpeg_info.jpegDHT.DC_value[Th], 0xff, sizeof(g_imgdec_jpeg_info.jpegDHT.DC_value[Th]));
			for(i=0; i<16; i++)
			{
				number = (UINT16)pBufPtr[pos];
				pos++;

				if(i < 8)
				{
					g_imgdec_jpeg_info.jpegDHT.DC_mincode[Th][i] = (UINT8)(val & 0xff);
					if(parse_info_log)	INFO("DC_mincode[%d][%d]=0x%02x\n", Th, i, g_imgdec_jpeg_info.jpegDHT.DC_mincode[Th][i]);
				}
				else
				{
					g_imgdec_jpeg_info.jpegDHT.DC_mincode[Th][i+i-8] = (UINT8)(val & 0xff);
					if(parse_info_log)	INFO("DC_mincode[%d][%d]=0x%02x\n", Th, i+i-8, g_imgdec_jpeg_info.jpegDHT.DC_mincode[Th][i+i-8]);
					g_imgdec_jpeg_info.jpegDHT.DC_mincode[Th][i+i-8+1] = (UINT8)(val >> 8);
					if(parse_info_log)	INFO("DC_mincode[%d][%d]=0x%02x\n", Th, i+i-8+1, g_imgdec_jpeg_info.jpegDHT.DC_mincode[Th][i+i-8+1]);
				}

				k = i >> 1;
				if(i & 0x01)
				{
					g_imgdec_jpeg_info.jpegDHT.DC_offset[Th][k] |= ((UINT8)offset << 4);
				}
				else
				{
					g_imgdec_jpeg_info.jpegDHT.DC_offset[Th][k] = ((UINT8)offset & 0x0f);
				}
				if(parse_info_log)	INFO("DC_offset[%d][%d]=0x%02x\n", Th, k, g_imgdec_jpeg_info.jpegDHT.DC_offset[Th][k]);

				val += number;
				offset += number;
				val <<= 1;
			}

			if(offset > 16)
			{
				if(parse_info_log)	WARNING("DC_value_cnt[%d] = %d > 16, reset it to be 16!", Th, offset);
				offset = 16;
			}
			else
				if(parse_info_log)	INFO("DC_value_cnt[%d] = %d", Th, offset);
			g_imgdec_jpeg_info.jpegDHT.DC_value_cnt[Th] = offset;

			for(i=0; i<offset; i++)
			{
				k = i >> 1;
				if(i & 0x01)
				{
					g_imgdec_jpeg_info.jpegDHT.DC_value[Th][k] |= (pBufPtr[pos]<<4);
				}
				else
				{
					g_imgdec_jpeg_info.jpegDHT.DC_value[Th][k] = (pBufPtr[pos]&0x0f);
				}
				pos++;
				if(parse_info_log)	INFO("DC_value[%d][%d]=0x%02x\n", Th, k, g_imgdec_jpeg_info.jpegDHT.DC_value[Th][k]);
			}
		}
		else  // AC Huffman table
		{
			memset(g_imgdec_jpeg_info.jpegDHT.AC_mincode[Th], 0xff, sizeof(g_imgdec_jpeg_info.jpegDHT.AC_mincode[Th]));
			memset(g_imgdec_jpeg_info.jpegDHT.AC_offset[Th], 0xff, sizeof(g_imgdec_jpeg_info.jpegDHT.AC_offset[Th]));
			memset(g_imgdec_jpeg_info.jpegDHT.AC_value[Th], 0xff, sizeof(g_imgdec_jpeg_info.jpegDHT.AC_value[Th]));
			for(i=0; i<16; i++)
			{
				number = (UINT16)pBufPtr[pos];
				pos++;

				if(i < 8)
				{
					g_imgdec_jpeg_info.jpegDHT.AC_mincode[Th][i] = (UINT8)(val & 0xff);
					if(parse_info_log)	INFO("AC_mincode[%d][%d]=0x%02x\n", Th, i, g_imgdec_jpeg_info.jpegDHT.AC_mincode[Th][i]);
				}
				else
				{
					g_imgdec_jpeg_info.jpegDHT.AC_mincode[Th][i+i-8] = (UINT8)(val & 0xff);
					if(parse_info_log)	INFO("AC_mincode[%d][%d]=0x%02x\n", Th, i+i-8, g_imgdec_jpeg_info.jpegDHT.AC_mincode[Th][i+i-8]);
					g_imgdec_jpeg_info.jpegDHT.AC_mincode[Th][i+i-8+1] = (UINT8)(val >> 8);
					if(parse_info_log)	INFO("AC_mincode[%d][%d]=0x%02x\n", Th, i+i-8+1, g_imgdec_jpeg_info.jpegDHT.AC_mincode[Th][i+i-8+1]);
				}

				g_imgdec_jpeg_info.jpegDHT.AC_offset[Th][i] = offset;
				if(parse_info_log)	INFO("AC_offset[%d][%d]=0x%04x\n", Th, i, g_imgdec_jpeg_info.jpegDHT.AC_offset[Th][i]);

				val += number;
				offset += number;
				val <<= 1;
			}

			if(offset > 256)
			{
				if(parse_info_log)	WARNING("AC_value_cnt[%d] = %d > 256, reset it to be 256!", Th, offset);
				offset = 256;
			}
			else
				if(parse_info_log)	INFO("AC_value_cnt[%d] = %d", Th, offset);
			g_imgdec_jpeg_info.jpegDHT.AC_value_cnt[Th] = offset;

			for(i=0; i<offset; i++)
			{
				g_imgdec_jpeg_info.jpegDHT.AC_value[Th][i] = pBufPtr[pos];
				pos++;
				if(parse_info_log)	INFO("AC_value[%d][%d]=0x%02x\n", Th, i, g_imgdec_jpeg_info.jpegDHT.AC_value[Th][i]);
			}
		}
	}while(pos < g_imgdec_jpeg_info.jpegDHT.length);
}

void IMGDEC_Parse_JPEG_DRI(UINT8 *pBufPtr, UINT8 parse_info_log)
{
	g_imgdec_jpeg_info.jpegDRI.length = ((UINT16)pBufPtr[0] << 8) | (UINT16)pBufPtr[1];
	if(parse_info_log)	INFO("length = %d\n", g_imgdec_jpeg_info.jpegDRI.length);

	g_imgdec_jpeg_info.jpegDRI.dri_count = ((UINT16)pBufPtr[2] << 8) | (UINT16)pBufPtr[3];
	if(parse_info_log)	INFO("dri_count = %d\n", g_imgdec_jpeg_info.jpegDRI.dri_count);
}

void IMGDEC_Parse_JPEG_SOS(UINT8 *pBufPtr, UINT8 parse_info_log)
{
	UINT32 i, id;

	g_imgdec_jpeg_info.jpegSOS.length = ((UINT16)pBufPtr[0]<<8) | (UINT16)pBufPtr[1];
	if(parse_info_log)	INFO("length = %d\n", g_imgdec_jpeg_info.jpegSOS.length);

	g_imgdec_jpeg_info.jpegSOS.components = pBufPtr[2];
	if(parse_info_log)	INFO("components = %d\n", g_imgdec_jpeg_info.jpegSOS.components);

	for(i=0; i<g_imgdec_jpeg_info.jpegSOS.components; i++)
	{
		g_imgdec_jpeg_info.jpegSOS.identifier[i] = pBufPtr[3 + i*2]; // 1->Y, 2->Cb, 3->Cr, 4->K
		if(g_imgdec_jpeg_info.jpegSOF.identifier[0] == 0) // 0->Y, 1->Cb, 2->Cr, 3->K
			g_imgdec_jpeg_info.jpegSOS.identifier[i] += 1; // 1->Y, 2->Cb, 3->Cr, 4->K
		if((g_imgdec_jpeg_info.jpegSOS.identifier[i] >= 1) && (g_imgdec_jpeg_info.jpegSOS.identifier[i] <= 4))
		{
			id = g_imgdec_jpeg_info.jpegSOS.identifier[i] - 1; // 0->Y, 1->Cb, 2->Cr, 3->K
			g_imgdec_jpeg_info.jpegSOS.DCtable[id]  = pBufPtr[3 + i*2 + 1] >> 4;
			g_imgdec_jpeg_info.jpegSOS.ACtable[id]  = pBufPtr[3 + i*2 + 1] & 0x0f;
			if(parse_info_log)	INFO("components[%d] identifier=%d, DCtable=%d, ACtable=%d\n", i,
									g_imgdec_jpeg_info.jpegSOS.identifier[i],
									g_imgdec_jpeg_info.jpegSOS.DCtable[id],
									g_imgdec_jpeg_info.jpegSOS.ACtable[id]);
		}
		else
			WARNING("g_imgdec_jpeg_info.jpegSOS.identifier[%d]= %d\n",i,  g_imgdec_jpeg_info.jpegSOS.identifier[i]);
	}

	g_imgdec_jpeg_info.jpegSOS.spectral_selection_start = pBufPtr[3 + g_imgdec_jpeg_info.jpegSOS.components*2];
	if(parse_info_log)	INFO("spectral_selection_start = %d\n", g_imgdec_jpeg_info.jpegSOS.spectral_selection_start);

	g_imgdec_jpeg_info.jpegSOS.spectral_selection_end = pBufPtr[3 + g_imgdec_jpeg_info.jpegSOS.components*2 + 1];
	if(parse_info_log)	INFO("spectral_selection_end = %d\n", g_imgdec_jpeg_info.jpegSOS.spectral_selection_end);

	g_imgdec_jpeg_info.jpegSOS.successive_approximation_high = pBufPtr[3 + g_imgdec_jpeg_info.jpegSOS.components*2 + 2] >> 4;
	if(parse_info_log)	INFO("successive_approximation_high = %d\n", g_imgdec_jpeg_info.jpegSOS.successive_approximation_high);

	g_imgdec_jpeg_info.jpegSOS.successive_approximation_low = pBufPtr[3 + g_imgdec_jpeg_info.jpegSOS.components*2 + 2] & 0x0f;
	if(parse_info_log)	INFO("successive_approximation_low = %d\n", g_imgdec_jpeg_info.jpegSOS.successive_approximation_low);
}

int IMGDEC_Parse_JPEG_Header(PARSE_INFO *parse_info_data)
{
	UINT8 *file_chunk_buf = (UINT8 *)gp_chunk_va(parse_info_data->filebuf_phyaddr);
	UINT32 file_size = parse_info_data->file_size;
	UINT8 byte_data;
	UINT16 maker = 0;
	UINT8 *pBufPtr;
	UINT16 maker_len = 0;
	UINT32 bitstream_offset = 0;

	//INFO("Parse JPEG Header!");

	if(file_chunk_buf == NULL)
	{
		ERR("file_chunk_buf is NULL!");
		return -1;
	}

	pBufPtr = file_chunk_buf;
	memset(&g_imgdec_jpeg_info, 0, sizeof(g_imgdec_jpeg_info));

	while(bitstream_offset < file_size)
	{
		while(bitstream_offset < file_size)
		{
			byte_data = *pBufPtr++;
			bitstream_offset++;
			maker = (maker << 8) | byte_data;
			if((maker > M_NULL1) && (maker < M_NULL2))
			{
				break;
			}
		}

		switch(maker)
		{
			case SOI:
				if(parse_info_data->parse_info_log)	INFO("maker = SOI\n");
				break;
			case APP0: // JFIF
				if(parse_info_data->parse_info_log)	INFO("maker = APP0\n");
				byte_data = pBufPtr[0];
				maker_len = byte_data;
				byte_data = pBufPtr[1];
				maker_len = (maker_len<<8) | byte_data;
				//IMGDEC_Parse_JPEG_APP0(pBufPtr, parse_info_data->parse_info_log);
				pBufPtr += maker_len;
				bitstream_offset += maker_len;
				break;
			case APP1: // EXIF
				if(parse_info_data->parse_info_log)	INFO("maker = APP1\n");
				byte_data = pBufPtr[0];
				maker_len = byte_data;
				byte_data = pBufPtr[1];
				maker_len = (maker_len<<8) | byte_data;
				//IMGDEC_Parse_JPEG_APP1(pBufPtr, parse_info_data->parse_info_log);
				pBufPtr += maker_len;
				bitstream_offset += maker_len;
				break;
			case DQT:
				if(parse_info_data->parse_info_log)	INFO("maker = DQT\n");
				byte_data = pBufPtr[0];
				maker_len = byte_data;
				byte_data = pBufPtr[1];
				maker_len = (maker_len<<8) | byte_data;
				IMGDEC_Parse_JPEG_DQT(pBufPtr, parse_info_data->parse_info_log);
				pBufPtr += maker_len;
				bitstream_offset += maker_len;
				break;
			case SOF0:
				if(parse_info_data->parse_info_log)	INFO("maker = SOF0\n");
				g_imgdec_jpeg_info.jpegSOF.sof_n = 0;
				byte_data = pBufPtr[0];
				maker_len = byte_data;
				byte_data = pBufPtr[1];
				maker_len = (maker_len<<8) | byte_data;
				if(g_imgdec_jpeg_info.jpegSOF.length == 0) // only get 1st SOF maker, then give up later others
					IMGDEC_Parse_JPEG_SOF(pBufPtr, parse_info_data->parse_info_log);
				pBufPtr += maker_len;
				bitstream_offset += maker_len;
				break;
			case SOF1:
				if(parse_info_data->parse_info_log)	INFO("maker = SOF1\n");
				g_imgdec_jpeg_info.jpegSOF.sof_n = 1;
				byte_data = pBufPtr[0];
				maker_len = byte_data;
				byte_data = pBufPtr[1];
				maker_len = (maker_len<<8) | byte_data;
				if(g_imgdec_jpeg_info.jpegSOF.length == 0) // only get 1st SOF maker, then give up later others
					IMGDEC_Parse_JPEG_SOF(pBufPtr, parse_info_data->parse_info_log);
				pBufPtr += maker_len;
				bitstream_offset += maker_len;
				break;
			case SOF2:
				if(parse_info_data->parse_info_log)	INFO("maker = SOF2\n");
				g_imgdec_jpeg_info.jpegSOF.sof_n = 2;
				byte_data = pBufPtr[0];
				maker_len = byte_data;
				byte_data = pBufPtr[1];
				maker_len = (maker_len<<8) | byte_data;
				if(g_imgdec_jpeg_info.jpegSOF.length == 0) // only get 1st SOF maker, then give up later others
					IMGDEC_Parse_JPEG_SOF(pBufPtr, parse_info_data->parse_info_log);
				pBufPtr += maker_len;
				bitstream_offset += maker_len;
				break;
			case DHT:
				if(parse_info_data->parse_info_log)	INFO("maker = DHT\n");
				byte_data = pBufPtr[0];
				maker_len = byte_data;
				byte_data = pBufPtr[1];
				maker_len = (maker_len<<8) | byte_data;
				if(g_imgdec_jpeg_info.jpegSOS.length == 0) // only get DHT makers before 1st SOS maker
					IMGDEC_Parse_JPEG_DHT(pBufPtr, parse_info_data->parse_info_log);
				pBufPtr += maker_len;
				bitstream_offset += maker_len;
				break;
			case DRI:
				if(parse_info_data->parse_info_log)	INFO("maker = DRI\n");
				byte_data = pBufPtr[0];
				maker_len = byte_data;
				byte_data = pBufPtr[1];
				maker_len = (maker_len<<8) | byte_data;
				if(g_imgdec_jpeg_info.jpegDRI.length == 0) // only get 1st DRI maker, then give up later others
					IMGDEC_Parse_JPEG_DRI(pBufPtr, parse_info_data->parse_info_log);
				pBufPtr += maker_len;
				bitstream_offset += maker_len;
				break;
			case SOS:
				if(parse_info_data->parse_info_log)	INFO("maker = SOS\n");
				byte_data = pBufPtr[0];
				maker_len = byte_data;
				byte_data = pBufPtr[1];
				maker_len = (maker_len<<8) | byte_data;
				if(g_imgdec_jpeg_info.jpegSOS.length == 0) // only get 1st SOS maker
				{
					IMGDEC_Parse_JPEG_SOS(pBufPtr, parse_info_data->parse_info_log);
					pBufPtr += maker_len;
					bitstream_offset += maker_len;
					g_imgdec_jpeg_info.jpegDecode.bitstream_start = bitstream_offset;
					if(parse_info_data->parse_info_log)	INFO("bitstream_start = 0x%08x\n", g_imgdec_jpeg_info.jpegDecode.bitstream_start);
				}
				else
				{
					pBufPtr += maker_len;
					bitstream_offset += maker_len;
				}
				pBufPtr = file_chunk_buf + file_size - 2; // jump to EOI for speed up
				bitstream_offset = file_size - 2; // jump to EOI for speed up
				break;
			case RD0:
			case RD1:
			case RD2:
			case RD3:
			case RD4:
			case RD5:
			case RD6:
			case RD7:
				if(parse_info_data->parse_info_log)	INFO("maker = RDx = 0x%04x\n", maker);
				break;
			case EOI:
				if(parse_info_data->parse_info_log)	INFO("maker = EOI\n");
				g_imgdec_jpeg_info.jpegDecode.bitstream_end = bitstream_offset; // set bitstream_end as the last EOI
				if(parse_info_data->parse_info_log)	INFO("bitstream_end = 0x%08x\n", g_imgdec_jpeg_info.jpegDecode.bitstream_end);
				break;
			default:
				if(parse_info_data->parse_info_log)	WARNING("maker = 0x%04x\n", maker);
				byte_data = pBufPtr[0];
				maker_len = byte_data;
				byte_data = pBufPtr[1];
				maker_len = (maker_len<<8) | byte_data;
				pBufPtr += maker_len;
				bitstream_offset += maker_len;
				break;
		}
	}

	if(g_imgdec_jpeg_info.jpegDecode.bitstream_end <= g_imgdec_jpeg_info.jpegDecode.bitstream_start)
	{
		DEBUG("WARNING: g_imgdec_jpeg_info.jpegDecode.bitstream_end <= g_imgdec_jpeg_info.jpegDecode.bitstream_start!\n");
		DEBUG("WARNING: set g_imgdec_jpeg_info.jpegDecode.bitstream_end = file_size!\n");
		g_imgdec_jpeg_info.jpegDecode.bitstream_end = file_size;
		if(parse_info_data->parse_info_log)	INFO("bitstream_end = 0x%08x\n", g_imgdec_jpeg_info.jpegDecode.bitstream_end);
	}

	if(g_imgdec_jpeg_info.jpegDHT.length== 0)
	{
		if(parse_info_data->parse_info_log)	INFO("use default huffman table!\n");
		IMGDEC_Parse_JPEG_DHT(MJPGDHTSeg+2, parse_info_data->parse_info_log);
	}

	g_imgdec_jpeg_info.jpegDecode.filename = parse_info_data->filename;
	g_imgdec_jpeg_info.jpegDecode.file_size = file_size;

	parse_info_data->image_type = 0; // JPEG
	parse_info_data->image_width = g_imgdec_jpeg_info.jpegSOF.width;
	parse_info_data->image_height = g_imgdec_jpeg_info.jpegSOF.height;
	if(g_imgdec_jpeg_info.jpegSOF.components == 1)
	{
		parse_info_data->image_color = YUV_444;
		parse_info_data->image_pixel = 1; // Gray
	}
	else
	{
		if((g_imgdec_jpeg_info.jpegSOF.Hfactor[0] * g_imgdec_jpeg_info.jpegSOF.Vfactor[0]) == 4)
		{
			if(g_imgdec_jpeg_info.jpegSOF.Hfactor[0] == 2)
			{
				parse_info_data->image_color = YUV_411_2X2;
			}
			else if(g_imgdec_jpeg_info.jpegSOF.Hfactor[0] == 1)
			{
				parse_info_data->image_color = YUV_411_1X4;
			}
			else
			{
				parse_info_data->image_color = YUV_411_4X1;
			}
		}
		else if((g_imgdec_jpeg_info.jpegSOF.Hfactor[0] * g_imgdec_jpeg_info.jpegSOF.Vfactor[0]) == 2)
		{
			if(g_imgdec_jpeg_info.jpegSOF.Hfactor[0] == 2)
			{
				parse_info_data->image_color = YUV_422_2X1;
			}
			else
			{
				parse_info_data->image_color = YUV_422_1X2;
			}
		}
		else // (g_imgdec_jpeg_info.jpegSOF.Hfactor[0] * g_imgdec_jpeg_info.jpegSOF.Vfactor[0]) = 1
		{
			parse_info_data->image_color = YUV_444;
		}
		parse_info_data->image_pixel = 0; // Color
	}
	parse_info_data->image_bkgd = 0;

	if(g_imgdec_jpeg_info.jpegAPP0.RGBn != NULL) // for thumbnail data output ??????
		gp_chunk_free(g_imgdec_jpeg_info.jpegAPP0.RGBn);

	if((g_imgdec_jpeg_info.jpegSOF.width == 0) || (g_imgdec_jpeg_info.jpegSOF.height == 0))
	{
		ERR("jpegSOF.width = 0 or jpegSOF.height = 0 !!!");
		return -1;
	}

	return 0;
}

int IMGDEC_Parse_JPEG_Header_MS(UINT8 *file_chunk_buf, UINT32 file_size)
{
	UINT8 *pBufPtr;
	UINT8 byte_data;
	UINT16 maker, maker_len;
	UINT32 bitstream_offset;

	if(file_chunk_buf == NULL)
	{
		ERR("file_chunk_buf is NULL!");
		return -1;
	}

	pBufPtr = (UINT8 *)((UINT32)file_chunk_buf + g_imgdec_jpeg_info.jpegDecode.bitstream_start);
	bitstream_offset = g_imgdec_jpeg_info.jpegDecode.bitstream_start;
	maker = 0;

	while(bitstream_offset < file_size)
	{
		while(bitstream_offset < file_size)
		{
			byte_data = *pBufPtr++;
			bitstream_offset++;
			maker = (maker << 8) | byte_data;
			if((maker > M_NULL1) && (maker < M_NULL2))
			{
				break;
			}
		}

		switch(maker)
		{
			case SOI:
				WARNING("maker = SOI\n");
				break;
			case APP0: // JFIF
				byte_data = pBufPtr[0];
				maker_len = byte_data;
				byte_data = pBufPtr[1];
				maker_len = (maker_len<<8) | byte_data;
				WARNING("maker = APP0\n");
				pBufPtr += maker_len;
				bitstream_offset += maker_len;
				break;
			case APP1: // EXIF
				byte_data = pBufPtr[0];
				maker_len = byte_data;
				byte_data = pBufPtr[1];
				maker_len = (maker_len<<8) | byte_data;
				WARNING("maker = APP1\n");
				pBufPtr += maker_len;
				bitstream_offset += maker_len;
				break;
			case DQT:
				byte_data = pBufPtr[0];
				maker_len = byte_data;
				byte_data = pBufPtr[1];
				maker_len = (maker_len<<8) | byte_data;
				WARNING("maker = DQT\n");
				pBufPtr += maker_len;
				bitstream_offset += maker_len;
				break;
			case SOF0:
				byte_data = pBufPtr[0];
				maker_len = byte_data;
				byte_data = pBufPtr[1];
				maker_len = (maker_len<<8) | byte_data;
				WARNING("maker = SOF0\n");
				pBufPtr += maker_len;
				bitstream_offset += maker_len;
				break;
			case SOF1:
				byte_data = pBufPtr[0];
				maker_len = byte_data;
				byte_data = pBufPtr[1];
				maker_len = (maker_len<<8) | byte_data;
				WARNING("maker = SOF1\n");
				pBufPtr += maker_len;
				bitstream_offset += maker_len;
				break;
			case SOF2:
				byte_data = pBufPtr[0];
				maker_len = byte_data;
				byte_data = pBufPtr[1];
				maker_len = (maker_len<<8) | byte_data;
				WARNING("maker = SOF2\n");
				pBufPtr += maker_len;
				bitstream_offset += maker_len;
				break;
			case DHT:
				byte_data = pBufPtr[0];
				maker_len = byte_data;
				byte_data = pBufPtr[1];
				maker_len = (maker_len<<8) | byte_data;
				IMGDEC_Parse_JPEG_DHT(pBufPtr, false);
				pBufPtr += maker_len;
				bitstream_offset += maker_len;
				break;
			case DRI:
				byte_data = pBufPtr[0];
				maker_len = byte_data;
				byte_data = pBufPtr[1];
				maker_len = (maker_len<<8) | byte_data;
				IMGDEC_Parse_JPEG_DRI(pBufPtr, false);
				pBufPtr += maker_len;
				bitstream_offset += maker_len;
				break;
			case SOS:
				byte_data = pBufPtr[0];
				maker_len = byte_data;
				byte_data = pBufPtr[1];
				maker_len = (maker_len<<8) | byte_data;
				IMGDEC_Parse_JPEG_SOS(pBufPtr, false);
				pBufPtr += maker_len;
				bitstream_offset += maker_len;
				g_imgdec_jpeg_info.jpegDecode.bitstream_start = bitstream_offset;
				//INFO("bitstream_start = 0x%08x\n", g_imgdec_jpeg_info.jpegDecode.bitstream_start);
				return 0;
			case RD0:
			case RD1:
			case RD2:
			case RD3:
			case RD4:
			case RD5:
			case RD6:
			case RD7:
				WARNING("maker = RDx = 0x%04x\n", maker);
				break;
			case EOI:
				//INFO("maker = EOI\n");
				return -1;
				break;
			default:
				byte_data = pBufPtr[0];
				maker_len = byte_data;
				byte_data = pBufPtr[1];
				maker_len = (maker_len<<8) | byte_data;
				pBufPtr += maker_len;
				bitstream_offset += maker_len;
				WARNING("maker = 0x%04x\n", maker);
				break;
		}
	}

	return -1;
}

void IMGDEC_Parse_PNG_IHDR(UINT8 *pBufPtr, UINT8 parse_info_log, UINT32 chunk_length)
{
	g_imgdec_png_info.pngIHDR.has_IHDR += 1;
	if(g_imgdec_png_info.pngIHDR.has_IHDR > 1)
	{
		WARNING("Duplicate IHDR chunk! (has_IHDR = %d)\n", g_imgdec_png_info.pngIHDR.has_IHDR);
	}

	if(chunk_length != 13)
	{
		WARNING("IHDR chunk length warning! (length = %d)\n", chunk_length);
	}

	g_imgdec_png_info.pngIHDR.width = ((UINT32)pBufPtr[0] << 24) |
								((UINT32)pBufPtr[1] << 16) |
								((UINT32)pBufPtr[2] << 8) |
								(UINT32)pBufPtr[3];
	if(parse_info_log)	INFO("width = %d\n", g_imgdec_png_info.pngIHDR.width);

	g_imgdec_png_info.pngIHDR.height = ((UINT32)pBufPtr[4] << 24) |
								((UINT32)pBufPtr[5] << 16) |
								((UINT32)pBufPtr[6] << 8) |
								(UINT32)pBufPtr[7];
	if(parse_info_log)	INFO("height = %d\n", g_imgdec_png_info.pngIHDR.height);

	g_imgdec_png_info.pngIHDR.bit_depth = pBufPtr[8];
	if(parse_info_log)	INFO("bit_depth = %d\n", g_imgdec_png_info.pngIHDR.bit_depth);

	g_imgdec_png_info.pngIHDR.color_type = pBufPtr[9];
	if(parse_info_log)	INFO("color_type = %d\n", g_imgdec_png_info.pngIHDR.color_type);

	g_imgdec_png_info.pngIHDR.compression_method = pBufPtr[10];
	if(parse_info_log)	INFO("compression_method = %d\n", g_imgdec_png_info.pngIHDR.compression_method);

	g_imgdec_png_info.pngIHDR.filter_method = pBufPtr[11];
	if(parse_info_log)	INFO("filter_method = %d\n", g_imgdec_png_info.pngIHDR.filter_method);

	g_imgdec_png_info.pngIHDR.interlace_method = pBufPtr[12];
	if(parse_info_log)	INFO("interlace_method = %d\n", g_imgdec_png_info.pngIHDR.interlace_method);
}

void IMGDEC_Parse_PNG_PLTE(UINT8 *pBufPtr, UINT8 parse_info_log, UINT32 chunk_length)
{
	UINT32 i;

	if(!g_imgdec_png_info.pngIHDR.has_IHDR)
	{
		WARNING("Missing IHDR chunk before PLTE!\n");
	}

	if(g_imgdec_png_info.pngIDAT.has_IDAT)
	{
		WARNING("Invalid PLTE chunk after IDAT!\n");
	}

	g_imgdec_png_info.pngPLTE.has_PLTE += 1;
	if(g_imgdec_png_info.pngPLTE.has_PLTE > 1)
	{
		WARNING("Duplicate PLTE chunk! (has_PLTE = %d)\n", g_imgdec_png_info.pngPLTE.has_PLTE);
	}

	if((chunk_length > PNG_PLTE_SIZE) || (chunk_length % 3))
	{
		WARNING("PLTE chunk length error! (length = %d)\n", chunk_length);

		if(chunk_length > PNG_PLTE_SIZE)
		{
			chunk_length = PNG_PLTE_SIZE;
			WARNING("PLTE chunk length is reset to be PNG_PLTE_SIZE(%d)! \n", PNG_PLTE_SIZE);
		}
	}

	memcpy((void *)g_imgdec_png_info.pngPLTE.pColorMap, (void *)pBufPtr, chunk_length);

	if(parse_info_log)
	{
		for(i=0; i<(chunk_length/3); i++)
		{
			printk("pColorMap[%d] = 0x%02x, 0x%02x, 0x%02x\n", i,
							g_imgdec_png_info.pngPLTE.pColorMap[i*3],
							g_imgdec_png_info.pngPLTE.pColorMap[i*3 + 1],
							g_imgdec_png_info.pngPLTE.pColorMap[i*3 + 2]);
		}
	}
}

void IMGDEC_Parse_PNG_bKGD(UINT8 *pBufPtr, UINT8 parse_info_log, UINT32 chunk_length)
{
	g_imgdec_png_info.pngBKGD.has_bKGD += 1;
	if(g_imgdec_png_info.pngBKGD.has_bKGD > 1)
	{
		WARNING("Duplicate bKGD chunk! (has_bKGD = %d)\n", g_imgdec_png_info.pngBKGD.has_bKGD);
	}

	switch(g_imgdec_png_info.pngIHDR.color_type)
	{
		/*Grayscale*/
		case PNG_GRAYSCALE_IMAGE:
		case PNG_GRAYSCALE_ALPHA_IMAGE:
			if(chunk_length==2)
			{
				if(g_imgdec_png_info.pngIHDR.bit_depth == 8) // get low byte
				{
					g_imgdec_png_info.pngBKGD.bKGD_red = pBufPtr[1];
					g_imgdec_png_info.pngBKGD.bKGD_green = pBufPtr[1];
					g_imgdec_png_info.pngBKGD.bKGD_blue= pBufPtr[1];
				}
				else // if(g_imgdec_png_info.pngIHDR.bit_depth == 16) // get high byte
				{
					g_imgdec_png_info.pngBKGD.bKGD_red = pBufPtr[0];
					g_imgdec_png_info.pngBKGD.bKGD_green = pBufPtr[0];
					g_imgdec_png_info.pngBKGD.bKGD_blue= pBufPtr[0];
				}
			}
			else
			{
				g_imgdec_png_info.pngBKGD.bKGD_red = 255;
				g_imgdec_png_info.pngBKGD.bKGD_green = 255;
				g_imgdec_png_info.pngBKGD.bKGD_blue= 255;
				WARNING("bKGD chunk length error!(color_type=%d, chunk_length=%d)\n",
												g_imgdec_png_info.pngIHDR.color_type,
												chunk_length);
			}
			break;

		/*Plattle RGB*/
		case PNG_RGB_INDEX_IMAGE:
			if(chunk_length==1)
			{
				g_imgdec_png_info.pngBKGD.bKGD_red = g_imgdec_png_info.pngPLTE.pColorMap[(UINT32)pBufPtr[0]*3];
				g_imgdec_png_info.pngBKGD.bKGD_green = g_imgdec_png_info.pngPLTE.pColorMap[(UINT32)pBufPtr[0]*3 + 1];
				g_imgdec_png_info.pngBKGD.bKGD_blue= g_imgdec_png_info.pngPLTE.pColorMap[(UINT32)pBufPtr[0]*3 + 2];
			}
			else
			{
				g_imgdec_png_info.pngBKGD.bKGD_red = 255;
				g_imgdec_png_info.pngBKGD.bKGD_green = 255;
				g_imgdec_png_info.pngBKGD.bKGD_blue= 255;
				WARNING("bKGD chunk length error!(color_type=%d, chunk_length=%d)\n",
												g_imgdec_png_info.pngIHDR.color_type,
												chunk_length);
			}
			break;

		case PNG_RGB_IMAGE:
		case PNG_RGB_ALPHA_IMAGE:
			if(chunk_length==6)
			{
				if(g_imgdec_png_info.pngIHDR.bit_depth == 8) // get low byte
				{
					g_imgdec_png_info.pngBKGD.bKGD_red = pBufPtr[1];
					g_imgdec_png_info.pngBKGD.bKGD_green = pBufPtr[3];
					g_imgdec_png_info.pngBKGD.bKGD_blue= pBufPtr[5];
				}
				else // if(g_imgdec_png_info.pngIHDR.bit_depth == 16) // get high byte
				{
					g_imgdec_png_info.pngBKGD.bKGD_red = pBufPtr[0];
					g_imgdec_png_info.pngBKGD.bKGD_green = pBufPtr[2];
					g_imgdec_png_info.pngBKGD.bKGD_blue= pBufPtr[4];
				}
			}
			else
			{
				g_imgdec_png_info.pngBKGD.bKGD_red = 255;
				g_imgdec_png_info.pngBKGD.bKGD_green = 255;
				g_imgdec_png_info.pngBKGD.bKGD_blue= 255;
				WARNING("bKGD chunk length error!(color_type=%d, chunk_length=%d)\n",
												g_imgdec_png_info.pngIHDR.color_type,
												chunk_length);
			}
			break;

		default:
			g_imgdec_png_info.pngBKGD.bKGD_red = 255;
			g_imgdec_png_info.pngBKGD.bKGD_green = 255;
			g_imgdec_png_info.pngBKGD.bKGD_blue= 255;
			break;
	}
}

void IMGDEC_Parse_PNG_IDAT(UINT8 *pBufPtr, UINT8 parse_info_log, UINT32 chunk_length, UINT32 file_size, UINT32 bitstream_offset)
{
	UINT8 zlib_header_bytes = 0;
	UINT16 pos = 0;

	if(!g_imgdec_png_info.pngIHDR.has_IHDR)
	{
		WARNING("Missing IHDR chunk before IDAT!\n");
	}

	if((g_imgdec_png_info.pngIHDR.color_type & 0x1) && (g_imgdec_png_info.pngPLTE.has_PLTE == 0))
	{
		WARNING("Missing PLTE chunk before IDAT!\n");
	}

	g_imgdec_png_info.pngIDAT.has_IDAT += 1;
	if(g_imgdec_png_info.pngIDAT.has_IDAT > 1)
	{
		if(parse_info_log)	INFO("Muitiple IDAT chunk! (has_IDAT = %d, chunk_length = 0x%08x)\n", g_imgdec_png_info.pngIDAT.has_IDAT, chunk_length);
	}

	if(g_imgdec_png_info.pngIDAT.has_IDAT == 1) // 1st IDAT chunk
	{
		// collecting at least 2 bytes for check zlib header
		g_imgdec_png_info.pngIDAT.zlib_header = 0;

		while((zlib_header_bytes < 2) && ((bitstream_offset + pos) < file_size))
		{
			if(chunk_length == 0)
			{
				chunk_length = ((UINT32)pBufPtr[pos + 4] << 24) |
								((UINT32)pBufPtr[pos + 5] << 16) |
								((UINT32)pBufPtr[pos + 6] << 8) |
								(UINT32)pBufPtr[pos + 7];
				pos += 12; // CRC, length, type,
			}
			else if(chunk_length == 1)
			{
				if(zlib_header_bytes == 0)
					g_imgdec_png_info.pngIDAT.zlib_header = (UINT16)pBufPtr[pos] << 8;
				else	// zlib_header_bytes == 1
					g_imgdec_png_info.pngIDAT.zlib_header = (g_imgdec_png_info.pngIDAT.zlib_header & 0xff00) | (UINT16)pBufPtr[pos];

				zlib_header_bytes += 1;
				pos += 1; // zlib_header one byte

				chunk_length = ((UINT32)pBufPtr[pos + 4] << 24) |
								((UINT32)pBufPtr[pos + 5] << 16) |
								((UINT32)pBufPtr[pos + 6] << 8) |
								(UINT32)pBufPtr[pos + 7];
				pos += 12; // CRC, length, type,
			}
			else // chunk_length >= 2
			{
				if(zlib_header_bytes == 0)
				{
					g_imgdec_png_info.pngIDAT.zlib_header = ((UINT32)pBufPtr[pos] << 8) | (UINT32)pBufPtr[pos + 1];
					zlib_header_bytes += 2;
					pos += 2; // zlib_header two bytes

					if(chunk_length == 2)
					{
						chunk_length = ((UINT32)pBufPtr[pos + 4] << 24) |
										((UINT32)pBufPtr[pos + 5] << 16) |
										((UINT32)pBufPtr[pos + 6] << 8) |
										(UINT32)pBufPtr[pos + 7];
						pos += 12; // CRC, length, type,
					}
				}
				else // zlib_header_bytes == 1
				{
					g_imgdec_png_info.pngIDAT.zlib_header = (g_imgdec_png_info.pngIDAT.zlib_header & 0xff00) | (UINT16)pBufPtr[pos];
					zlib_header_bytes += 1;
					pos += 1; // zlib_header one byte
				}
			}
		}

		if((g_imgdec_png_info.pngIDAT.zlib_header % 31) != 0)
			WARNING("Invalid ZLIB header check: mod 31 is not 0 \n");

		if(((g_imgdec_png_info.pngIDAT.zlib_header >> 8) & 0xf) != 8) // [11:8] != 8
			WARNING("Invalid ZLIB header check: [11:8] is not 8 \n");

		if(parse_info_log)	INFO("zlib_header = 0x%04x\n", g_imgdec_png_info.pngIDAT.zlib_header);

		g_imgdec_png_info.pngIDAT.chunk_length = chunk_length;
		if(parse_info_log)	INFO("chunk_length = 0x%08x\n", g_imgdec_png_info.pngIDAT.chunk_length);

		g_imgdec_png_info.pngIDAT.bs_byte_offset = 4 + pos; // 4 for IDAT chunk type, pos for zlib_header
		if(parse_info_log)	INFO("bs_byte_offset = 0x%08x\n", g_imgdec_png_info.pngIDAT.bs_byte_offset);

		g_imgdec_png_info.pngIDAT.chunk_remain_bytes = chunk_length - pos;
		if(parse_info_log)	INFO("chunk_remain_bytes = 0x%08x\n", g_imgdec_png_info.pngIDAT.chunk_remain_bytes);

		g_imgdec_png_info.pngIDAT.block_final = 0;
		g_imgdec_png_info.pngIDAT.block_type = 0;
		g_imgdec_png_info.pngIDAT.bs_bit_offset = 0;
		g_imgdec_png_info.pngIDAT.hold_bits = 0;
		g_imgdec_png_info.pngIDAT.nlen = 0;
		g_imgdec_png_info.pngIDAT.ndist = 0;
		g_imgdec_png_info.pngIDAT.ncode = 0;
		g_imgdec_png_info.pngIDAT.dma_count_ext = 0;
	}
}

int IMGDEC_Parse_PNG_Header(PARSE_INFO *parse_info_data)
{
	UINT8 *file_chunk_buf = (UINT8 *)gp_chunk_va(parse_info_data->filebuf_phyaddr);
	UINT32 file_size = parse_info_data->file_size;
	UINT8 *pBufPtr;
	UINT8 parse_image_end = false;
	UINT32 chunk_length = 0;
	UINT32 chunk_type = 0;
	UINT32 bitstream_offset = 0;

	//INFO("Parse PNG Header!");

	if(file_chunk_buf == NULL)
	{
		ERR("file_chunk_buf is NULL!");
		return -1;
	}

	pBufPtr = file_chunk_buf + 8; // ignore PNG file signature
	bitstream_offset += 8;
	memset(&g_imgdec_png_info, 0, sizeof(g_imgdec_png_info));

	while(!parse_image_end)
	{
		chunk_length = ((UINT32)pBufPtr[0] << 24) | ((UINT32)pBufPtr[1] << 16) | ((UINT32)pBufPtr[2] << 8) | (UINT32)pBufPtr[3];
		pBufPtr += 4;
		bitstream_offset += 4;
		chunk_type = ((UINT32)pBufPtr[0] << 24) | ((UINT32)pBufPtr[1] << 16) | ((UINT32)pBufPtr[2] << 8) | (UINT32)pBufPtr[3];
		pBufPtr += 4;
		bitstream_offset += 4;

		switch(chunk_type)
		{
			case PNG_CHUNK_IHDR:
				IMGDEC_Parse_PNG_IHDR(pBufPtr, parse_info_data->parse_info_log, chunk_length);
				pBufPtr += chunk_length + 4; // +4 to ignore 4 bytes CRC
				bitstream_offset += chunk_length + 4;
				break;
			case PNG_CHUNK_PLTE:
				IMGDEC_Parse_PNG_PLTE(pBufPtr, parse_info_data->parse_info_log, chunk_length);
				pBufPtr += chunk_length + 4; // +4 to ignore 4 bytes CRC
				bitstream_offset += chunk_length + 4;
				break;
			case PNG_CHUNK_bKGD:
				IMGDEC_Parse_PNG_bKGD(pBufPtr, parse_info_data->parse_info_log, chunk_length);
				pBufPtr += chunk_length + 4; // +4 to ignore 4 bytes CRC
				bitstream_offset += chunk_length + 4;
				break;
			case PNG_CHUNK_IDAT:
				IMGDEC_Parse_PNG_IDAT(pBufPtr, parse_info_data->parse_info_log, chunk_length, file_size, bitstream_offset);
				if(g_imgdec_png_info.pngIDAT.has_IDAT == 1) // 1st IDAT chunk
				{
					g_imgdec_png_info.pngDecode.bitstream_start = bitstream_offset - 4; // -4: bitstream includes IDAT chunk type
					if(parse_info_data->parse_info_log)	INFO("bitstream_start = 0x%08x\n", g_imgdec_png_info.pngDecode.bitstream_start);
				}
				pBufPtr += chunk_length + 4; // +4 to ignore 4 bytes CRC
				bitstream_offset += chunk_length + 4;
				break;
			case PNG_CHUNK_IEND:
				pBufPtr += chunk_length + 4; // +4 to ignore 4 bytes CRC
				bitstream_offset += chunk_length + 4;
				g_imgdec_png_info.pngDecode.bitstream_end = bitstream_offset; // +4: bitstream must exceed IEND chunk type (includes IEND chunk CRC)
				if(parse_info_data->parse_info_log)	INFO("bitstream_end = 0x%08x\n", g_imgdec_png_info.pngDecode.bitstream_end);
				parse_image_end = true;
				if(parse_info_data->parse_info_log)	INFO("chunk_type = IEND\n");
				break;
			default:
				pBufPtr += chunk_length + 4; // +4 to ignore 4 bytes CRC
				bitstream_offset += chunk_length + 4;
				if(parse_info_data->parse_info_log)	WARNING("chunk_type = 0x%08x\n", chunk_type);
				break;
		}

		if(bitstream_offset >= file_size)
		{
			if(!parse_image_end)
				WARNING("This PNG file can't find IEND!");
			break;
		}
	}

	if(g_imgdec_png_info.pngDecode.bitstream_end <= g_imgdec_png_info.pngDecode.bitstream_start)
	{
		DEBUG("WARNING: g_imgdec_png_info.pngDecode.bitstream_end <= g_imgdec_png_info.pngDecode.bitstream_start!\n");
		DEBUG("WARNING: set g_imgdec_png_info.pngDecode.bitstream_end = file_size!\n");
		g_imgdec_png_info.pngDecode.bitstream_end = file_size;
		if(parse_info_data->parse_info_log)	INFO("bitstream_end = 0x%08x\n", g_imgdec_png_info.pngDecode.bitstream_end);
	}

	g_imgdec_png_info.pngDecode.filename = parse_info_data->filename;
	g_imgdec_png_info.pngDecode.file_size = file_size;

	parse_info_data->image_type = 1; // PNG
	parse_info_data->image_width = g_imgdec_png_info.pngIHDR.width;
	parse_info_data->image_height = g_imgdec_png_info.pngIHDR.height;
	parse_info_data->image_color = g_imgdec_png_info.pngIHDR.color_type;
	switch(g_imgdec_png_info.pngIHDR.color_type)
	{
		case PNG_GRAYSCALE_IMAGE:
		case PNG_GRAYSCALE_ALPHA_IMAGE:
			parse_info_data->image_pixel = 1; // Gray
			break;
		case PNG_RGB_IMAGE:
		case PNG_RGB_INDEX_IMAGE:
		case PNG_RGB_ALPHA_IMAGE:
		default:
			parse_info_data->image_pixel = 0; // Color
			break;
	}
	if(g_imgdec_png_info.pngBKGD.has_bKGD)
	{
		parse_info_data->image_bkgd = ((UINT32)g_imgdec_png_info.pngBKGD.has_bKGD << 24) |
									((UINT32)g_imgdec_png_info.pngBKGD.bKGD_red << 16) |
									((UINT32)g_imgdec_png_info.pngBKGD.bKGD_green << 8) |
									(UINT32)g_imgdec_png_info.pngBKGD.bKGD_blue;
	}
	else
	{
		parse_info_data->image_bkgd = 0;
	}

	if((g_imgdec_png_info.pngIHDR.width == 0) || (g_imgdec_png_info.pngIHDR.height == 0))
	{
		ERR("pngIHDR.width = 0 or pngIHDR.height = 0 !!!");
		return -1;
	}

	return 0;
}

int IMGDEC_Parse_Image_Header(PARSE_INFO *parse_info_data)
{
	struct file *filp;
	mm_segment_t old_fs;

	UINT8 *file_chunk_buf = NULL;
	UINT32 file_size = 0;
	UINT8 file_chunk_malloc = false;

	int ret = 0;

	if((parse_info_data->file_size == 0) ||(parse_info_data->filebuf_phyaddr == 0))
	{
		if(parse_info_data->filename == NULL)
		{
			ERR("parse_info_data->filename = NULL!\n");
			return -1;
		}

		filp = filp_open(parse_info_data->filename, O_RDONLY, 0);
		if(IS_ERR(filp))
		{
			ERR("filp_open(%s) error!\n", parse_info_data->filename);
			return -1;
		}
		else
		{
			old_fs = get_fs();
			set_fs(KERNEL_DS);
			filp->f_op->llseek(filp, 0, 2); // seek to end of file
			file_size = filp->f_pos; // file size
			filp->f_op->llseek(filp, 0, 0); // seek back to beginning of file
			file_chunk_buf = gp_chunk_malloc_nocache(CHUNK_POOL_INDEX, 0, file_size);
			if(file_chunk_buf == NULL)
			{
				ERR("gp_chunk_malloc_nocache (file_chunk_buf) fail!\n");
				return -1;
			}
			else
			{
				file_chunk_malloc = true;
				filp->f_op->read(filp, file_chunk_buf, file_size, &filp->f_pos);
			}
			set_fs(old_fs);
		}
		filp_close(filp, NULL);

		parse_info_data->filebuf_phyaddr = gp_chunk_pa(file_chunk_buf);
		parse_info_data->file_size = file_size;
	}
	else
	{
		file_chunk_buf = (UINT8 *)gp_chunk_va(parse_info_data->filebuf_phyaddr);
		file_size = parse_info_data->file_size;
	}

	if((file_chunk_buf[0] == 0xFF) && (file_chunk_buf[1] == 0xD8)) // JPEG
	{
		ret = IMGDEC_Parse_JPEG_Header(parse_info_data);
	}
	else if ((file_chunk_buf[0] == 0x89) && (file_chunk_buf[1] == 0x50) &&
			(file_chunk_buf[2] == 0x4E) && (file_chunk_buf[3] == 0x47) &&
			(file_chunk_buf[4] == 0x0D) && (file_chunk_buf[5] == 0x0A) &&
			(file_chunk_buf[6] == 0x1A) && (file_chunk_buf[7] == 0x0A)) // PNG
	{
		ret = IMGDEC_Parse_PNG_Header(parse_info_data);
	}
	else
	{
		parse_info_data->image_type = 0xff; // Other image type
		WARNING("Other image format: header first 8 bytes = 0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x\n",
					file_chunk_buf[0], file_chunk_buf[1], file_chunk_buf[2], file_chunk_buf[3],
					file_chunk_buf[4], file_chunk_buf[5], file_chunk_buf[6], file_chunk_buf[7]);
		ret = -1;
	}

	if(file_chunk_malloc)
		gp_chunk_free(file_chunk_buf);

	return ret;
}

int IMGDEC_Decode_JPEG_Bitstream(DECODE_INFO *decode_info_data)
{
	PARSE_INFO parse_info_data;

	UINT8 i, ms_time_out = 64;
	UINT8 *file_chunk_buf = NULL;
	UINT8 *bitstream_chunk_buf = NULL;
	UINT8 *pBsBufPtr = NULL;
	UINT8 *working_chunk_buf = NULL;
	UINT8 file_chunk_malloc = false;
	UINT32 size = 0, w, h;
	UINT32 bs_start_offset, bsbuf_phyaddr_start;
	UINT32 horizontal_MCU_num, wb_render_size;
	UINT32 wb_start_offset, wb_phyaddr_start;

	struct file *filp;
	mm_segment_t old_fs;


	// ------------ check decoding information data ------------ //

	if((decode_info_data->file_size == 0) ||(decode_info_data->filebuf_phyaddr == 0))
	{
		if(decode_info_data->filename == NULL)
		{
			ERR("decode_info_data>filename = NULL!\n");
			return -1;
		}

		filp = filp_open(decode_info_data->filename, O_RDONLY, 0);
		if(IS_ERR(filp))
		{
			ERR("filp_open(%s) error!\n", decode_info_data->filename);
			return -1;
		}
		else
		{
			old_fs = get_fs();
			set_fs(KERNEL_DS);
			filp->f_op->llseek(filp, 0, 2); // seek to end of file
			size = filp->f_pos; // file size
			filp->f_op->llseek(filp, 0, 0); // seek back to beginning of file
			file_chunk_buf = gp_chunk_malloc_nocache(CHUNK_POOL_INDEX, 0, size);
			if(file_chunk_buf == NULL)
			{
				ERR("gp_chunk_malloc_nocache (file_chunk_buf) fail!\n");
				set_fs(old_fs);
				filp_close(filp, NULL);
				return -1;
			}
			else
			{
				//DEBUG("file_chunk_buf.addr=0x%08x, file_chunk_buf.phy_addr=0x%08x, size=%d\n",
				//				(UINT32)file_chunk_buf, gp_chunk_pa(file_chunk_buf), size);
				file_chunk_malloc = true;
				filp->f_op->read(filp, file_chunk_buf, size, &filp->f_pos);
			}
			set_fs(old_fs);
		}
		filp_close(filp, NULL);

		decode_info_data->filebuf_phyaddr = gp_chunk_pa(file_chunk_buf);
		decode_info_data->file_size = size;
	}
	else
	{
		file_chunk_buf = (UINT8 *)gp_chunk_va(decode_info_data->filebuf_phyaddr);
		//size = decode_info_data->file_size; // unnecessary!
	}

	//if((strlen(decode_info_data->filename) != strlen(g_imgdec_jpeg_info.jpegDecode.filename)) ||
	//	memcmp(decode_info_data->filename, g_imgdec_jpeg_info.jpegDecode.filename, strlen(decode_info_data->filename))
	//	(decode_info_data->filename = NULL) || (g_imgdec_jpeg_info.jpegDecode.filename = NULL)) // this image is not parsed
	{
		parse_info_data.filename = decode_info_data->filename;
		parse_info_data.filebuf_phyaddr = decode_info_data->filebuf_phyaddr;
		parse_info_data.file_size = decode_info_data->file_size;
		parse_info_data.parse_info_log = false;
		parse_info_data.image_type = 0;
		parse_info_data.image_width = 0;
		parse_info_data.image_height = 0;
		parse_info_data.image_color = 0;
		parse_info_data.image_pixel = 0;
		parse_info_data.image_bkgd = 0;
		if(IMGDEC_Parse_JPEG_Header(&parse_info_data) != 0)
		{
			if(file_chunk_malloc)
				gp_chunk_free(file_chunk_buf);
			return -1;
		}
	}

	if((g_imgdec_jpeg_info.jpegSOF.width < 16) || (g_imgdec_jpeg_info.jpegSOF.height < 16))
	{
		ERR("JPEG decoder can not support image size < 16! (this image size as %d x %d)\n",
								g_imgdec_jpeg_info.jpegSOF.width, g_imgdec_jpeg_info.jpegSOF.height);
		if(file_chunk_malloc)
			gp_chunk_free(file_chunk_buf);
		return -1;
	}

	if((g_imgdec_jpeg_info.jpegSOF.width > 65535) || (g_imgdec_jpeg_info.jpegSOF.height > 65535))
	{
		ERR("JPEG decoder can not support image size > 65535! (this image size as %d x %d)\n",
								g_imgdec_jpeg_info.jpegSOF.width, g_imgdec_jpeg_info.jpegSOF.height);
		if(file_chunk_malloc)
			gp_chunk_free(file_chunk_buf);
		return -1;
	}


	// ------------ allocate bitstream buffer ------------ //

	size = (((g_imgdec_jpeg_info.jpegDecode.bitstream_end - g_imgdec_jpeg_info.jpegDecode.bitstream_start) + 0x3ff) & 0xfffffc00) + 2048; // + 2048 for mj_stop_kbyte_addr
	bitstream_chunk_buf = gp_chunk_malloc_nocache(CHUNK_POOL_INDEX, 0, size);
	if(bitstream_chunk_buf == NULL)
	{
		ERR("gp_chunk_malloc_nocache (bitstream_chunk_buf) fail!\n");
		if(file_chunk_malloc)
			gp_chunk_free(file_chunk_buf);
		return -1;
	}
	else
	{
		//DEBUG("bitstream_chunk_buf.addr=0x%08x, bitstream_chunk_buf.phy_addr=0x%08x, size=%d\n",
		//	(UINT32)bitstream_chunk_buf, gp_chunk_pa(bitstream_chunk_buf), size);
		//memset(bitstream_chunk_buf, 0, size); // mark to debug flash in letb123
	}
	if(gp_chunk_pa(bitstream_chunk_buf) & 0x1f)
		bs_start_offset = 32 - (gp_chunk_pa(bitstream_chunk_buf) & 0x1f);
	else
		bs_start_offset = 0;
	pBsBufPtr = (UINT8 *)((UINT32)bitstream_chunk_buf + bs_start_offset);
	bsbuf_phyaddr_start = gp_chunk_pa(bitstream_chunk_buf) + bs_start_offset; // MJ decoder auto-starts at 32byte-align position
	size = g_imgdec_jpeg_info.jpegDecode.bitstream_end - g_imgdec_jpeg_info.jpegDecode.bitstream_start;  // bistream after SOS chunck
	memcpy((void *)pBsBufPtr, (void *)((UINT32)file_chunk_buf + g_imgdec_jpeg_info.jpegDecode.bitstream_start), size);


	// ------------ allocate working buffer ------------ //

	horizontal_MCU_num = (g_imgdec_jpeg_info.jpegSOF.Hfactor[0] == 2) ? ((g_imgdec_jpeg_info.jpegSOF.width+15)>>4) : ((g_imgdec_jpeg_info.jpegSOF.width+7)>>3);
	if(g_imgdec_jpeg_info.jpegSOF.Hfactor[0] == 2) // YUV411, YUV422 (2x1)
		wb_render_size = ((horizontal_MCU_num * 304 + 1023) >> 10) << 10; // wb_render_size  is 1024-bytes align
	else
		wb_render_size = ((horizontal_MCU_num * 240 + 1023) >> 10) << 10; // wb_render_size  is 1024-bytes align
	size = wb_render_size + 1024; // 1024 for wb_start_offset
	if(g_imgdec_jpeg_info.jpegSOF.sof_n == 2) // working buffer is too large ?????? use Multi-Scan to reduce buffer size (but, Multi-Scan takes too long time!)
	{
		w = ((g_imgdec_jpeg_info.jpegSOF.width + 15) >> 4) << 4;
		h =  ((g_imgdec_jpeg_info.jpegSOF.height + 15) >> 4) << 4;
		size += w * h * 3 * 2; // for saving HUF decoded data (3 for YCbCr, 2 for 16-bits) by MS-progressive JPEG decoder
		size += 1024; // 1024 for wb_start_offset
	}
	working_chunk_buf = gp_chunk_malloc_nocache(CHUNK_POOL_INDEX, 0, size);
	if(working_chunk_buf == NULL)
	{
		ERR("gp_chunk_malloc_nocache (working_chunk_buf) fail!\n");
		gp_chunk_free(bitstream_chunk_buf);
		if(file_chunk_malloc)
			gp_chunk_free(file_chunk_buf);
		return -1;
	}
	else
	{
		//DEBUG("working_chunk_buf.addr=0x%08x, working_chunk_buf.phy_addr=0x%08x, size=%d\n",
		//	(UINT32)working_chunk_buf, gp_chunk_pa(working_chunk_buf), size);
		//memset(working_chunk_buf, 0, size); // mark to debug flash in letb123
	}
	if(gp_chunk_pa(working_chunk_buf) & 0x3ff)
		wb_start_offset = 1024 - (gp_chunk_pa(working_chunk_buf) & 0x3ff);
	else
		wb_start_offset = 0;
	wb_phyaddr_start = gp_chunk_pa(working_chunk_buf) + wb_start_offset; // wb_phyaddr_start is 1024-bytes align


	// ------------ start to decode image bitstream ------------ //

	g_imgdec_png_info.pngDecode.output_run_time = 0; // let JR working only for JPEG
	g_imgdec_jpeg_info.jpegDecode.output_run_time = 0x01; // bit[3:0] = total run times
	if(decode_info_data->render_width > 4096)	g_imgdec_jpeg_info.jpegDecode.output_run_time <<= 1;
	if(decode_info_data->render_height > 4096)	g_imgdec_jpeg_info.jpegDecode.output_run_time <<= 1;
	if(decode_info_data->render_width > 4096)	g_imgdec_jpeg_info.jpegDecode.output_run_time += 0x40; // bit[6] = 1: horizontal run 2 times
	if(decode_info_data->render_height > 4096)	g_imgdec_jpeg_info.jpegDecode.output_run_time += 0x80; // bit[7] = 1: vertical run 2 times

	for(i=0; i<(g_imgdec_jpeg_info.jpegDecode.output_run_time & 0x0f); i++)
	{
		imgdec_hal_jr_setup(&g_imgdec_jpeg_info, &g_imgdec_png_info, decode_info_data, wb_phyaddr_start);

		imgdec_hal_jpeg_hw_init(&g_imgdec_jpeg_info);

		if(g_imgdec_jpeg_info.jpegSOF.sof_n == 2) // Progressive JPEG decode (Multi-scan mode)
		{
			g_imgdec_jpeg_info.jpegDecode.cur_sos_run = 0;
			while(ms_time_out)
			{
				//DEBUG("cur_sos_run = %d\n", g_imgdec_jpeg_info.jpegDecode.cur_sos_run);

				imgdec_hal_jpeg_fill_ms_tables(&g_imgdec_jpeg_info);

				imgdec_hal_jpeg_decode_ms_init(&g_imgdec_jpeg_info, &bsbuf_phyaddr_start, (wb_phyaddr_start+wb_render_size));

				if(g_imgdec_jpeg_info.jpegSOS.components == 1)
				{
					if(imgdec_hal_jpeg_decode_ms_noninterleave(&g_imgdec_jpeg_info, &bsbuf_phyaddr_start) < 0)
					{
						gp_chunk_free(working_chunk_buf);
						gp_chunk_free(bitstream_chunk_buf);
						if(file_chunk_malloc)
							gp_chunk_free(file_chunk_buf);
						return -1;
					}
				}
				else
				{
					if(imgdec_hal_jpeg_decode_ms_interleave(&g_imgdec_jpeg_info, &bsbuf_phyaddr_start) < 0)
					{
						gp_chunk_free(working_chunk_buf);
						gp_chunk_free(bitstream_chunk_buf);
						if(file_chunk_malloc)
							gp_chunk_free(file_chunk_buf);
						return -1;
					}
				}

				imgdec_hal_jpeg_decode_ms_end(&g_imgdec_jpeg_info, &bsbuf_phyaddr_start);

				if(IMGDEC_Parse_JPEG_Header_MS(file_chunk_buf, g_imgdec_jpeg_info.jpegDecode.file_size) < 0)
					break;

				bsbuf_phyaddr_start = gp_chunk_pa(bitstream_chunk_buf) + bs_start_offset; // &bsbuf_phyaddr_start for Multi-Scan ??????
				size = g_imgdec_jpeg_info.jpegDecode.bitstream_end - g_imgdec_jpeg_info.jpegDecode.bitstream_start;
				memcpy((void *)pBsBufPtr, (void *)((UINT32)file_chunk_buf + g_imgdec_jpeg_info.jpegDecode.bitstream_start), size);

				ms_time_out--;
				if(ms_time_out == 0)
					WARNING("Progressive JPEG decoding don't finish yet, but it's time out!");
			}

			if(imgdec_hal_jpeg_decode_ms_scale(&g_imgdec_jpeg_info) < 0)
			{
				gp_chunk_free(working_chunk_buf);
				gp_chunk_free(bitstream_chunk_buf);
				if(file_chunk_malloc)
					gp_chunk_free(file_chunk_buf);
				return -1;
			}

			DEBUG("Multi-scan progressive JPEG decoding is done!\n");
		}
		else // Baseline JPEG decode (Motion-JPEG mode)
		{
			imgdec_hal_jpeg_fill_bs_tables(&g_imgdec_jpeg_info);

			size = g_imgdec_jpeg_info.jpegDecode.bitstream_end - g_imgdec_jpeg_info.jpegDecode.bitstream_start + 1024; // + 1024 for mj_stop_kbyte_addr
			if(imgdec_hal_jpeg_decode_bs_MJmode(&g_imgdec_jpeg_info, bsbuf_phyaddr_start, size) < 0)
			{
				gp_chunk_free(working_chunk_buf);
				gp_chunk_free(bitstream_chunk_buf);
				if(file_chunk_malloc)
					gp_chunk_free(file_chunk_buf);
				return -1;
			}
		}

		g_imgdec_jpeg_info.jpegDecode.output_run_time += 0x10; // bit[5:4] = current run time
	}


	// ------------ end to decode image bitstream ------------ //

	gp_chunk_free(working_chunk_buf);
	gp_chunk_free(bitstream_chunk_buf);
	if(file_chunk_malloc)
		gp_chunk_free(file_chunk_buf);

	return 0;
}

int IMGDEC_Decode_PNG_Bitstream(DECODE_INFO *decode_info_data)
{
	PARSE_INFO parse_info_data;

	UINT8 i, file_chunk_malloc = false;
	UINT8 *file_chunk_buf = NULL;
	UINT8 *bitstream_chunk_buf = NULL;
	UINT8 *pBsBufPtr = NULL;
	UINT8 *LZ_backward_chunk_buf = NULL;
	UINT8 *scanline_chunk_buf = NULL;
	UINT8 *working_chunk_buf = NULL;
	UINT8 *decoded_chunk_buf = NULL;
	UINT32 size = 0;
	UINT32 bs_start_offset, bsbuf_phyaddr_start;
	UINT32 horizontal_MCU_num, wb_render_size;
	UINT32 wb_start_offset, wb_phyaddr_start;
	UINT32 decoded_start_offset, decodedbuf_phyaddr_start;

	struct file *filp;
	mm_segment_t old_fs;


	// ------------ check decoding information data ------------ //

	if((decode_info_data->file_size == 0) ||(decode_info_data->filebuf_phyaddr == 0))
	{
		if(decode_info_data->filename == NULL)
		{
			ERR("decode_info_data->filename = NULL!\n");
			return -1;
		}

		filp = filp_open(decode_info_data->filename, O_RDONLY, 0);
		if(IS_ERR(filp))
		{
			ERR("filp_open(%s) error!\n", decode_info_data->filename);
			return -1;
		}
		else
		{
			old_fs = get_fs();
			set_fs(KERNEL_DS);
			filp->f_op->llseek(filp, 0, 2); // seek to end of file
			size = filp->f_pos; // file size
			filp->f_op->llseek(filp, 0, 0); // seek back to beginning of file
			file_chunk_buf = gp_chunk_malloc_nocache(CHUNK_POOL_INDEX, 0, size);
			if(file_chunk_buf == NULL)
			{
				ERR("gp_chunk_malloc_nocache (file_chunk_buf) fail!\n");
				set_fs(old_fs);
				filp_close(filp, NULL);
				return -1;
			}
			else
			{
				//DEBUG("file_chunk_buf.addr=0x%08x, file_chunk_buf.phy_addr=0x%08x, size=%d\n",
				//				(UINT32)file_chunk_buf, gp_chunk_pa(file_chunk_buf), size);
				file_chunk_malloc = true;
				filp->f_op->read(filp, file_chunk_buf, size, &filp->f_pos);
			}
			set_fs(old_fs);
		}
		filp_close(filp, NULL);

		decode_info_data->filebuf_phyaddr = gp_chunk_pa(file_chunk_buf);
		decode_info_data->file_size = size;
	}
	else
	{
		file_chunk_buf = (UINT8 *)gp_chunk_va(decode_info_data->filebuf_phyaddr);
		//size = decode_info_data->file_size; // unnecessary!
	}

	//if((strlen(decode_info_data->filename) != strlen(g_imgdec_png_info.pngDecode.filename)) ||
	//	memcmp(decode_info_data->filename, g_imgdec_png_info.pngDecode.filename, strlen(decode_info_data->filename))
	//	(decode_info_data->filename = NULL) || (g_imgdec_png_info.pngDecode.filename = NULL)) // this image is not parsed
	{
		parse_info_data.filename = decode_info_data->filename;
		parse_info_data.filebuf_phyaddr = decode_info_data->filebuf_phyaddr;
		parse_info_data.file_size = decode_info_data->file_size;
		parse_info_data.parse_info_log = false;
		parse_info_data.image_type = 0;
		parse_info_data.image_width = 0;
		parse_info_data.image_height = 0;
		parse_info_data.image_color = 0;
		parse_info_data.image_pixel = 0;
		parse_info_data.image_bkgd = 0;
		if(IMGDEC_Parse_PNG_Header(&parse_info_data) != 0)
		{
			if(file_chunk_malloc)
				gp_chunk_free(file_chunk_buf);
			return -1;
		}
	}

	if((g_imgdec_png_info.pngIHDR.width < 16) || (g_imgdec_png_info.pngIHDR.height < 16))
	{
		ERR("PNG decoder can not support image size < 16! (this image size as %d x %d)\n",
								g_imgdec_png_info.pngIHDR.width, g_imgdec_png_info.pngIHDR.height);
		if(file_chunk_malloc)
			gp_chunk_free(file_chunk_buf);
		return -1;
	}

	if((g_imgdec_png_info.pngIHDR.width > 8192) || (g_imgdec_png_info.pngIHDR.height > 8192))
	{
		ERR("PNG decoder can not support image size > 8192! (this image size as %d x %d)\n",
								g_imgdec_png_info.pngIHDR.width, g_imgdec_png_info.pngIHDR.height);
		if(file_chunk_malloc)
			gp_chunk_free(file_chunk_buf);
		return -1;
	}

	if(g_imgdec_png_info.pngIHDR.interlace_method)
	{
		ERR("PNG decoder can not support interlaced picture!\n");
		if(file_chunk_malloc)
			gp_chunk_free(file_chunk_buf);
		return -1;
	}

	if((g_imgdec_png_info.pngIHDR.bit_depth == 16) &&
		((g_imgdec_png_info.pngIHDR.color_type == PNG_RGB_IMAGE) ||
		(g_imgdec_png_info.pngIHDR.color_type == PNG_RGB_INDEX_IMAGE) ||
		(g_imgdec_png_info.pngIHDR.color_type == PNG_RGB_ALPHA_IMAGE)))
	{
		ERR("PNG decoder can not support 16-bit depth and RGB color picture!\n");
		if(file_chunk_malloc)
			gp_chunk_free(file_chunk_buf);
		return -1;
	}

	/* // Xilinx 7045 has not CBUS, CPU can't write PNG HW palette table into DMA_BUF
	if(g_imgdec_png_info.pngPLTE.has_PLTE)
	{
		if(imgdec_hal_data_ram_mmap() < 0)
		{
			if(file_chunk_malloc)
				gp_chunk_free(file_chunk_buf);
			return -1;
		}
	}
	*/


	// ------------ allocate bitstream buffer ------------ //

	size = (g_imgdec_png_info.pngDecode.bitstream_end - g_imgdec_png_info.pngDecode.bitstream_start) + 64; // 32-byte boundary
	bitstream_chunk_buf = gp_chunk_malloc_nocache(CHUNK_POOL_INDEX, 0, size);
	if(bitstream_chunk_buf == NULL)
	{
		ERR("gp_chunk_malloc_nocache (bitstream_chunk_buf) fail!\n");
		if(file_chunk_malloc)
			gp_chunk_free(file_chunk_buf);
		return -1;
	}
	else
	{
		//DEBUG("bitstream_chunk_buf.addr=0x%08x, bitstream_chunk_buf.phy_addr=0x%08x, size=%d\n",
		//	(UINT32)bitstream_chunk_buf, gp_chunk_pa(bitstream_chunk_buf), size);
		//memset(bitstream_chunk_buf, 0, size); // mark to debug flash in letb123
	}
	if(gp_chunk_pa(bitstream_chunk_buf) & 0x1f)
		bs_start_offset = 32 - (gp_chunk_pa(bitstream_chunk_buf) & 0x1f);
	else
		bs_start_offset = 0;
	pBsBufPtr = bitstream_chunk_buf + bs_start_offset;
	bsbuf_phyaddr_start = gp_chunk_pa(bitstream_chunk_buf) + bs_start_offset;
	size -= 64;  // from IDAT chunk type to IEND chunk CRC
	memcpy((void *)pBsBufPtr, (void *)((UINT32)file_chunk_buf + g_imgdec_png_info.pngDecode.bitstream_start), size);
	if(file_chunk_malloc)
		gp_chunk_free(file_chunk_buf);


	// ------------ allocate LZ_backward & scanline buffer ------------ //

	size = 32 << 10; // 32KB
	LZ_backward_chunk_buf = gp_chunk_malloc_nocache(CHUNK_POOL_INDEX, 0, size);
	if(LZ_backward_chunk_buf == NULL)
	{
		ERR("gp_chunk_malloc_nocache (LZ_backward_chunk_buf) fail!\n");
		gp_chunk_free(bitstream_chunk_buf);
		return -1;
	}
	else
	{
		//DEBUG("LZ_backward_chunk_buf.addr=0x%08x, LZ_backward_chunk_buf.phy_addr=0x%08x, size=%d\n",
		//	(UINT32)LZ_backward_chunk_buf, gp_chunk_pa(LZ_backward_chunk_buf), size);
		//memset(LZ_backward_chunk_buf, 0, size); // mark to debug flash in letb123
	}

	size = 256 << 10; // 256KB
	scanline_chunk_buf = gp_chunk_malloc_nocache(CHUNK_POOL_INDEX, 0, size);
	if(scanline_chunk_buf == NULL)
	{
		ERR("gp_chunk_malloc_nocache (scanline_chunk_buf) fail!\n");
		gp_chunk_free(bitstream_chunk_buf);
		gp_chunk_free(LZ_backward_chunk_buf);
		return -1;
	}
	else
	{
		//DEBUG("scanline_chunk_buf.addr=0x%08x, scanline_chunk_buf.phy_addr=0x%08x, size=%d\n",
		//	(UINT32)scanline_chunk_buf, gp_chunk_pa(scanline_chunk_buf), size);
		//memset(scanline_chunk_buf, 0, size); // mark to debug flash in letb123
	}


	// ------------ allocate working buffer ------------ //

	horizontal_MCU_num = (g_imgdec_png_info.pngIHDR.width + 7) >> 3;
	wb_render_size = ((horizontal_MCU_num * 304 + 1023) >> 10) << 10; // wb_render_size  is 1024-bytes align, 304 for g_jr_reg_info.alpha_scaling = 1
	size = wb_render_size + 1024; // 1024 for wb_start_offset
	working_chunk_buf = gp_chunk_malloc_nocache(CHUNK_POOL_INDEX, 0, size);
	if(working_chunk_buf == NULL)
	{
		ERR("gp_chunk_malloc_nocache (working_chunk_buf) fail!\n");
		gp_chunk_free(bitstream_chunk_buf);
		gp_chunk_free(LZ_backward_chunk_buf);
		gp_chunk_free(scanline_chunk_buf);
		return -1;
	}
	else
	{
		//DEBUG("working_chunk_buf.addr=0x%08x, working_chunk_buf.phy_addr=0x%08x, size=%d\n",
		//	(UINT32)working_chunk_buf, gp_chunk_pa(working_chunk_buf), size);
		//memset(working_chunk_buf, 0, size); // mark to debug flash in letb123
	}
	if(gp_chunk_pa(working_chunk_buf) & 0x3ff)
		wb_start_offset = 1024 - (gp_chunk_pa(working_chunk_buf) & 0x3ff);
	else
		wb_start_offset = 0;
	wb_phyaddr_start = gp_chunk_pa(working_chunk_buf) + wb_start_offset; // wb_phyaddr_start is 1024-bytes align


	// ------------ allocate decoded buffer ------------ //

#if PNG_JR_PIPELINE_ENABLE
	size = ((g_imgdec_png_info.pngIHDR.width + 0xf) & ~0xf) * 4 * 8 + 0x10; // 16bytes-align; 4 for ARGB format; 8 for PNG MCU height; 0x10 for 16-align address
#else
	size = ((g_imgdec_png_info.pngIHDR.width + 0xf) & ~0xf) * 4 * ((g_imgdec_png_info.pngIHDR.height + 0xf) & ~0xf) + 0x10; // 16bytes-align; 4 for ARGB format; 0x10 for 16-align address
#endif
	decoded_chunk_buf = gp_chunk_malloc_nocache(CHUNK_POOL_INDEX, 0, size);
	if(decoded_chunk_buf == NULL)
	{
		ERR("gp_chunk_malloc_nocache (decoded_chunk_buf) fail!\n");
		gp_chunk_free(bitstream_chunk_buf);
		gp_chunk_free(LZ_backward_chunk_buf);
		gp_chunk_free(scanline_chunk_buf);
		gp_chunk_free(working_chunk_buf);
		return -1;
	}
	else
	{
		//DEBUG("decoded_chunk_buf.addr=0x%08x, decoded_chunk_buf.phy_addr=0x%08x, size=%d\n",
		//	(UINT32)decoded_chunk_buf, gp_chunk_pa(decoded_chunk_buf), size);
		//memset(decoded_chunk_buf, 0, size); // mark to debug flash in letb123
	}
	if(gp_chunk_pa(decoded_chunk_buf) & 0xf)
		decoded_start_offset = 16 - (gp_chunk_pa(decoded_chunk_buf) & 0xf);
	else
		decoded_start_offset = 0;
	decodedbuf_phyaddr_start = gp_chunk_pa(decoded_chunk_buf) + decoded_start_offset; // decodedbuf_phyaddr_start is 16-bytes align


	// ------------ start to decode image bitstream ------------ //

	g_imgdec_jpeg_info.jpegDecode.output_run_time = 0; // let JR working only for PNG
	g_imgdec_png_info.pngDecode.output_run_time = 0x01; // bit[3:0] = total run times
	if(decode_info_data->render_width > 4096)	g_imgdec_png_info.pngDecode.output_run_time <<= 1;
	if(decode_info_data->render_height > 4096)	g_imgdec_png_info.pngDecode.output_run_time <<= 1;
	if(decode_info_data->render_width > 4096)	g_imgdec_png_info.pngDecode.output_run_time += 0x40; // bit[6] = 1: horizontal run 2 times
	if(decode_info_data->render_height > 4096)	g_imgdec_png_info.pngDecode.output_run_time += 0x80; // bit[7] = 1: vertical run 2 times

	for(i=0; i<(g_imgdec_png_info.pngDecode.output_run_time & 0x0f); i++)
	{
		imgdec_hal_jr_setup(&g_imgdec_jpeg_info, &g_imgdec_png_info, decode_info_data, wb_phyaddr_start);

		imgdec_hal_png_hw_init(&g_imgdec_png_info,
								gp_chunk_pa(LZ_backward_chunk_buf),
								gp_chunk_pa(scanline_chunk_buf),
								decodedbuf_phyaddr_start);

		size = g_imgdec_png_info.pngDecode.bitstream_end - g_imgdec_png_info.pngDecode.bitstream_start; // from IDAT chunk type to IEND chunk CRC
		if(imgdec_hal_png_decode(&g_imgdec_png_info,
								bsbuf_phyaddr_start,
								size,
								pBsBufPtr,
								gp_chunk_pa(LZ_backward_chunk_buf),
								decodedbuf_phyaddr_start) < 0)
		{
			gp_chunk_free(decoded_chunk_buf);
			gp_chunk_free(working_chunk_buf);
			gp_chunk_free(scanline_chunk_buf);
			gp_chunk_free(LZ_backward_chunk_buf);
			gp_chunk_free(bitstream_chunk_buf);
			return -1;
		}

#if !PNG_JR_PIPELINE_ENABLE
		if(imgdec_hal_png_scale(&g_imgdec_png_info, decodedbuf_phyaddr_start, 0) < 0)
		{
			gp_chunk_free(decoded_chunk_buf);
			gp_chunk_free(working_chunk_buf);
			gp_chunk_free(scanline_chunk_buf);
			gp_chunk_free(LZ_backward_chunk_buf);
			gp_chunk_free(bitstream_chunk_buf);
			return -1;
		}
#endif

		g_imgdec_png_info.pngDecode.output_run_time += 0x10; // bit[5:4] = current run time
	}


	// ------------ end to decode image bitstream ------------ //

	/* // Xilinx 7045 has not CBUS, CPU can't write PNG HW palette table into DMA_BUF
	if(g_imgdec_png_info.pngPLTE.has_PLTE)
	{
		imgdec_hal_data_ram_unmmap();
	}
	*/

	gp_chunk_free(decoded_chunk_buf);
	gp_chunk_free(working_chunk_buf);
	gp_chunk_free(scanline_chunk_buf);
	gp_chunk_free(LZ_backward_chunk_buf);
	gp_chunk_free(bitstream_chunk_buf);

	return 0;
}

int IMGDEC_Decode_Image_Bitstream(DECODE_INFO *decode_info_data)
{
	struct file *filp;
	mm_segment_t old_fs;
	UINT8 tmpbuf[8];

	if(decode_info_data->outbuf_phyaddr == 0)
	{
		ERR("outbuf_phyaddr in DECODE_INFO is NULL!\n");
		return -1;
	}

	// 0xf for orientation
	if(((decode_info_data->render_type & 0xf) == RENDER_YUV420) && (decode_info_data->outUVbuf_phyaddr == 0))
	{
		ERR("outUVbuf_phyaddr in DECODE_INFO is NULL!\n");
		return -1;
	}

	if((decode_info_data->outbuf_width == 0) || (decode_info_data->outbuf_height == 0))
	{
		ERR("outbuf_width or  outbuf_height in DECODE_INFO is 0!\n");
		return -1;
	}

	if((decode_info_data->render_width < 16) || (decode_info_data->render_height < 16))
	{
		ERR("IMGDEC driver can not support render size < 16! (this render size as %d x %d)\n",
						decode_info_data->render_width, decode_info_data->render_height);
		return -1;
	}

	if((decode_info_data->render_width > 8192) || (decode_info_data->render_height > 8192))
	{
		ERR("IMGDEC driver can not support render size > 8192! (this render size as %d x %d)\n",
						decode_info_data->render_width, decode_info_data->render_height);
		return -1;
	}

	// 0xf for orientation
	if(((decode_info_data->render_type & 0xf) != RENDER_YUV420) && ((decode_info_data->render_type & 0xf) != RENDER_RGB565) &&
		((decode_info_data->render_type & 0xf) != RENDER_RGBA8888) && ((decode_info_data->render_type & 0xf) != RENDER_ARGB8888))
	{
		ERR("In DECODE_INFO, render_type is not in range!(render_type = %d)\n", (decode_info_data->render_type & 0xf));
		return -1;
	}

	// add for orientation
	if((((decode_info_data->render_type & 0xe000) >> 13) != NORMAL) && (((decode_info_data->render_type & 0xe000) >> 13) != RIGHT_ROTATE_90) &&
		(((decode_info_data->render_type & 0xe000) >> 13) != LEFT_ROTATE_90) && (((decode_info_data->render_type & 0xe000) >> 13) != ROTATE_180) &&
		(((decode_info_data->render_type & 0xe000) >> 13) != UP_DOWN_FLIP) && (((decode_info_data->render_type & 0xe000) >> 13) != LEFT_RIGHT_FLIP))
	{
		ERR("In SCALE_INFO, render_orientation is not in range!(render_orientation = %d)\n", ((decode_info_data->render_type & 0xe000) >> 13));
		return -1;
	}

	// 0xf for orientation
	if((((decode_info_data->render_type & 0xf) == RENDER_RGB565) || ((decode_info_data->render_type & 0xf) == RENDER_RGBA8888) ||
		((decode_info_data->render_type & 0xf) == RENDER_ARGB8888)) &&
		((decode_info_data->render_endian != OUT_BIG_ENDIAN) && (decode_info_data->render_endian != OUT_LITTLE_ENDIAN)))
	{
		ERR("In DECODE_INFO, render_endian is not in range!(render_endian = %d)\n", decode_info_data->render_endian);
		return -1;
	}

	if((decode_info_data->file_size == 0) ||(decode_info_data->filebuf_phyaddr == 0))
	{
		if(decode_info_data->filename == NULL)
		{
			ERR("decode_info_data->filename = NULL!\n");
			return -1;
		}

		filp = filp_open(decode_info_data->filename, O_RDONLY, 0);
		if(IS_ERR(filp))
		{
			ERR("filp_open(%s) error!\n", decode_info_data->filename);
			return -1;
		}
		else
		{
			old_fs = get_fs();
			set_fs(KERNEL_DS);
			filp->f_op->read(filp, tmpbuf, 8, &filp->f_pos);
			set_fs(old_fs);
		}
		filp_close(filp, NULL);
	}
	else
	{
		memcpy((void *)tmpbuf, (void *)gp_chunk_va(decode_info_data->filebuf_phyaddr), 8);
	}

	if((tmpbuf[0] == 0xFF) && (tmpbuf[1] == 0xD8)) // JPEG
	{
		return IMGDEC_Decode_JPEG_Bitstream(decode_info_data);
	}
	else if ((tmpbuf[0] == 0x89) && (tmpbuf[1] == 0x50) && (tmpbuf[2] == 0x4E) && (tmpbuf[3] == 0x47) &&
		(tmpbuf[4] == 0x0D) && (tmpbuf[5] == 0x0A) && (tmpbuf[6] == 0x1A) && (tmpbuf[7] == 0x0A)) // PNG
	{
		return IMGDEC_Decode_PNG_Bitstream(decode_info_data);
	}
	else
	{
		WARNING("Other image format: header first 8 bytes = 0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x\n",
			tmpbuf[0], tmpbuf[1], tmpbuf[2], tmpbuf[3], tmpbuf[4], tmpbuf[5], tmpbuf[6], tmpbuf[7]);
		return -1;
	}
}

int IMGDEC_Scale_Image_Data(SCALE_INFO *scale_info_data)
{
	UINT8 *working_chunk_buf = NULL;
	UINT32 horizontal_MCU_num, wb_render_size;
	UINT32 wb_start_offset, wb_phyaddr_start;


	// ------------ check scaling information data ------------ //

	if(scale_info_data->src_Ybufphyaddr == 0)
	{
		ERR("In SCALE_INFO, src_Ybufphyaddr is NULL!\n");
		return -1;
	}

	if(scale_info_data->dst_Ybufphyaddr == 0)
	{
		ERR("In SCALE_INFO, dst_Ybufphyaddr is NULL!\n");
		return -1;
	}

	if((scale_info_data->src_type == SCALE_SRC_YUV420) && (scale_info_data->src_UVbufphyaddr == 0))
	{
		ERR("In SCALE_INFO, src_type is YUV420, but src_UVbufphyaddr is NULL!\n");
		return -1;
	}

	// 0xf for orientation
	if(((scale_info_data->dst_type & 0xf) == SCALE_DST_YUV420) && (scale_info_data->dst_UVbufphyaddr == 0))
	{
		ERR("In SCALE_INFO, dst_type is YUV420, but dst_UVbufphyaddr is NULL!\n");
		return -1;
	}

	if(scale_info_data->src_bufbpl > (65535*4))
	{
		ERR("IMGDEC driver can not support auto-fetch input line pitch size > 65535*4 bytes! (this input line pitch size as %d)\n",
																				scale_info_data->src_bufbpl);
		return -1;
	}

	if((scale_info_data->src_type != SCALE_SRC_YUV420) && (scale_info_data->src_type != SCALE_SRC_YUY2) &&
		(scale_info_data->src_type != SCALE_SRC_RGBA8888) && (scale_info_data->src_type != SCALE_SRC_ARGB8888))
	{
		ERR("In SCALE_INFO, src_type is not in range!(src_type = %d)\n", scale_info_data->src_type);
		return -1;
	}

	// 0xf for orientation
	if(((scale_info_data->dst_type & 0xf) != SCALE_DST_YUV420) && ((scale_info_data->dst_type & 0xf) != SCALE_DST_YUVBW) &&
		((scale_info_data->dst_type & 0xf) != SCALE_DST_RGB565) && ((scale_info_data->dst_type & 0xf) != SCALE_DST_RGBA8888) &&
		((scale_info_data->dst_type & 0xf) != SCALE_DST_ARGB8888))
	{
		ERR("In SCALE_INFO, dst_type is not in range!(dst_type = %d)\n", (scale_info_data->dst_type & 0xf));
		return -1;
	}

	// add for orientation
	if((((scale_info_data->dst_type & 0xe000) >> 13) != NORMAL) && (((scale_info_data->dst_type & 0xe000) >> 13) != RIGHT_ROTATE_90) &&
		(((scale_info_data->dst_type & 0xe000) >> 13) != LEFT_ROTATE_90) && (((scale_info_data->dst_type & 0xe000) >> 13) != ROTATE_180) &&
		(((scale_info_data->dst_type & 0xe000) >> 13) != UP_DOWN_FLIP) && (((scale_info_data->dst_type & 0xe000) >> 13) != LEFT_RIGHT_FLIP))
	{
		ERR("In SCALE_INFO, dst_orientation is not in range!(dst_orientation = %d)\n", ((scale_info_data->dst_type & 0xe000) >> 13));
		return -1;
	}

	if( ((scale_info_data->src_type == SCALE_SRC_RGBA8888) || (scale_info_data->src_type == SCALE_SRC_ARGB8888)) &&
		((scale_info_data->src_endian != SCALE_BIG_ENDIAN) && (scale_info_data->src_endian != SCALE_LITTLE_ENDIAN)))
	{
		ERR("In SCALE_INFO, src_endian is not in range!(src_endian = %d)\n", scale_info_data->src_endian);
		return -1;
	}

	// 0xf for orientation
	if((((scale_info_data->dst_type & 0xf) == SCALE_DST_RGB565) || ((scale_info_data->dst_type & 0xf) == SCALE_DST_RGBA8888) ||
		((scale_info_data->dst_type & 0xf) == SCALE_DST_ARGB8888)) &&
		((scale_info_data->dst_endian != SCALE_BIG_ENDIAN) && (scale_info_data->dst_endian != SCALE_LITTLE_ENDIAN)))
	{
		ERR("In SCALE_INFO, dst_endian is not in range!(dst_endian = %d)\n", scale_info_data->dst_endian);
		return -1;
	}

	if(((scale_info_data->clip_x + scale_info_data->clip_width) > scale_info_data->src_width) ||
		((scale_info_data->clip_y + scale_info_data->clip_height) > scale_info_data->src_height))
	{
		ERR("In SCALE_INFO, (clip_x + clip_width) > src_width, or (clip_y + clip_height) > src_height\n");
		return -1;
	}

	if((scale_info_data->rect_width > 8191) || (scale_info_data->rect_height > 8191))
	{
		ERR("IMGDEC driver can not support scaling render size > 8191! (this scaling render size as %d x %d)\n",
										scale_info_data->rect_width, scale_info_data->rect_height);
		return -1;
	}


	// ------------ allocate working buffer ------------ //

	horizontal_MCU_num = (scale_info_data->clip_width + 7) >> 3;
	wb_render_size = (horizontal_MCU_num * 240 + 1023) & ~0x3ff; // wb_render_size is 1024-bytes align
	wb_render_size += 1024; // 1024 for wb_start_offset
	working_chunk_buf = gp_chunk_malloc_nocache(CHUNK_POOL_INDEX, 0, wb_render_size);
	if(working_chunk_buf == NULL)
	{
		ERR("gp_chunk_malloc_nocache (working_chunk_buf) fail!\n");
		return -1;
	}
	else
	{
		//DEBUG("working_chunk_buf.addr=0x%08x, working_chunk_buf.phy_addr=0x%08x, wb_render_size=%d\n",
		//	(UINT32)working_chunk_buf, gp_chunk_pa(working_chunk_buf), wb_render_size);
		//memset(working_chunk_buf, 0, wb_render_size); // mark to debug flash in letb123
	}
	if(gp_chunk_pa(working_chunk_buf) & 0x3ff)
		wb_start_offset = 1024 - (gp_chunk_pa(working_chunk_buf) & 0x3ff);
	else
		wb_start_offset = 0;
	wb_phyaddr_start = gp_chunk_pa(working_chunk_buf) + wb_start_offset; // wb_phyaddr_start is 1024-bytes align


	// ------------ start to scale image data ------------ //

	imgdec_hal_scale_setup(scale_info_data, wb_phyaddr_start);

	if(imgdec_hal_scale_start(scale_info_data) < 0)
	{
		gp_chunk_free(working_chunk_buf);
		return -1;
	}


	// ------------ end to scale image data ------------ //

	gp_chunk_free(working_chunk_buf);

	return 0;
}

void IMGDEC_Flush_Cache(void)
{
	/* flush L1 cache by all */
	flush_cache_all();
	/* flush L2 cache by all */
	outer_flush_all();
}

//---------------------------------------------------------------

static int imgdec_fops_open(struct inode *inode, struct file *pfile)
{
	int minor = iminor(inode);

	DEBUG("IMG_DEC module open\n");

	if(minor == g_imgdec_misc.minor)
	{
		return 0;
	}
	else
	{
		ERR("invalid inode\n");
		pfile->private_data = NULL;
		return -1;
	}

	return 0;
}

static int imgdec_fops_release(struct inode *inode, struct file *pfile)
{
	DEBUG("IMG_DEC module release\n");
	return 0;
}

static long imgdec_fops_ioctl(struct file *pfile, unsigned int cmd, unsigned long arg)
{
	int ret = 0;
	PARSE_INFO parse_info_data;
	DECODE_INFO decode_info_data;
	SCALE_INFO scale_info_data;

	switch (cmd)
	{
		case IMGDEC_CMD_IMAGE_PARSE:
			{
				if (wait_event_timeout(g_imgdec_done_wait, g_imgdec_done, (DONE_DEFAULT_TIMEOUT * HZ) / 1000) == 0)
				{
					ERR("wait_event_timeout !!!\n");
					return -1;
				}
				g_imgdec_done = false;

				if (copy_from_user((void *)&parse_info_data, (const void __user *)arg, sizeof(PARSE_INFO)))
				{
					g_imgdec_done = true;
					wake_up(&g_imgdec_done_wait);
					return -1;
				}
				IMGDEC_Flush_Cache();
				ret = IMGDEC_Parse_Image_Header(&parse_info_data);
				if(ret == 0)
				{
					if (copy_to_user((void __user *) arg, (const void *) &parse_info_data, sizeof(PARSE_INFO)))
					{
						g_imgdec_done = true;
						wake_up(&g_imgdec_done_wait);
						return -1;
					}
				}

				g_imgdec_done = true;
				wake_up(&g_imgdec_done_wait);
			}
			break;
		case IMGDEC_CMD_IMAGE_DECODE:
			{
				if (wait_event_timeout(g_imgdec_done_wait, g_imgdec_done, (DONE_DEFAULT_TIMEOUT * HZ) / 1000) == 0)
				{
					ERR("wait_event_timeout !!!\n");
					return -1;
				}
				g_imgdec_done = false;

				if (copy_from_user((void *)&decode_info_data, (const void __user *)arg, sizeof(DECODE_INFO)))
				{
					g_imgdec_done = true;
					wake_up(&g_imgdec_done_wait);
					return -1;
				}
				IMGDEC_Flush_Cache();
				ret = IMGDEC_Decode_Image_Bitstream(&decode_info_data);
				if(ret == 0)
				{
					if (copy_to_user((void __user *) arg, (const void *) &decode_info_data, sizeof(DECODE_INFO)))
					{
						g_imgdec_done = true;
						wake_up(&g_imgdec_done_wait);
						return -1;
					}
				}

				g_imgdec_done = true;
				wake_up(&g_imgdec_done_wait);
			}
			break;
		case IMGDEC_CMD_IMAGE_SCALE:
			{
				if (wait_event_timeout(g_imgdec_done_wait, g_imgdec_done, (DONE_DEFAULT_TIMEOUT * HZ) / 1000) == 0)
				{
					ERR("wait_event_timeout !!!\n");
					return -1;
				}
				g_imgdec_done = false;

				if (copy_from_user((void *)&scale_info_data, (const void __user *)arg, sizeof(SCALE_INFO)))
				{
					g_imgdec_done = true;
					wake_up(&g_imgdec_done_wait);
					return -1;
				}
				IMGDEC_Flush_Cache();
				ret = IMGDEC_Scale_Image_Data(&scale_info_data);
				if(ret == 0)
				{
					if (copy_to_user((void __user *) arg, (const void *) &scale_info_data, sizeof(SCALE_INFO)))
					{
						g_imgdec_done = true;
						wake_up(&g_imgdec_done_wait);
						return -1;
					}
				}

				g_imgdec_done = true;
				wake_up(&g_imgdec_done_wait);
			}
			break;
		default:
			WARNING("Unknown command\n");
			break;
	}

	return ret;
}

static int imgdec_probe(struct platform_device *pdev)
{
	UINT32 ret;

	DEBUG("IMG_DEC driver probe\n");

	// Registering misc device
	g_imgdec_misc.minor	= MISC_DYNAMIC_MINOR;
	g_imgdec_misc.name	= "imgdec";
	g_imgdec_misc.fops	= &imgdec_fops;

	ret = misc_register(&g_imgdec_misc);
	if (ret) {
		ERR("misc_register failed, ret = %d\n", ret);
		return -1;
	}

	imgdec_clock_enable();

	init_waitqueue_head(&g_imgdec_done_wait);
	g_imgdec_done = true;	// for first time non-block case

	// MJmode uses interrupt to replace polling (begin)
	imgdec_hal_jpeg_init_MJmode_waitqueue_head();
	if(request_irq(SP_IRQ_IMGDEC_PRI_INT, imgdec_hal_jpeg_decode_bs_MJmode_irq_handler, IRQF_DISABLED|IRQF_TRIGGER_RISING, "IMGDEC_IRQ", NULL) != 0)
		ERR("SP_IRQ_IMGDEC_PRI_INT can not register imgdec_hal_jpeg_decode_bs_MJmode_irq_handler!");
	// MJmode uses interrupt to replace polling (end)

	return 0;
}

static int imgdec_remove(struct platform_device *pdev)
{
	DEBUG("IMG_DEC driver remove\n");

	free_irq(SP_IRQ_IMGDEC_PRI_INT, NULL); // MJmode uses interrupt to replace polling

	misc_deregister(&g_imgdec_misc);

	imgdec_clock_disable();

	return 0;
}

static int imgdec_suspend(struct platform_device *pdev, pm_message_t state)
{
	INFO("IMG_DEC driver suspend\n");

	if (wait_event_timeout(g_imgdec_done_wait, g_imgdec_done, (DONE_DEFAULT_TIMEOUT * HZ) / 1000) == 0)
	{
		ERR("wait_event_timeout !!!\n");
		return -1;
	}

	imgdec_clock_disable();

	return 0;
}

static int imgdec_resume(struct platform_device *pdev)
{
	INFO("IMG_DEC driver resume\n");

	imgdec_clock_enable();

	return 0;
}

static int __init imgdec_init(void)
{
	int ret = 0;
	bool enable = true; // for letb8388 module driver enable/disable config

	if(sp_config_get_bool("drvIMGDEC", &enable) == SP_CFG_RET_SUCCESS) // for letb8388 module driver enable/disable config
	{
		if(enable == false)
		{
			INFO("IMG_DEC module init config check: drvIMGDEC=n\n");
			return -1;
		}
		else if(enable == true)
		{
			INFO("IMG_DEC module init config check: drvIMGDEC=y\n");
		}
		else
		{
			INFO("IMG_DEC module init config check: drvIMGDEC=?\n");
		}
	}
	else
	{
			INFO("IMG_DEC module init config check: sp_config_get_bool(drvIMGDEC) != SP_CFG_RET_SUCCES\n");
	}

	INFO("IMG_DEC module init\n");

	if(imgdec_hal_reg_mmap() < 0)
		return -1;

	//imgdec_hal_print_all_registers(); // only for test!!! (waiting to be removed as release)

	ret = platform_device_register(&imgdec_device);
	if (ret)
	{
		ERR("imgdec device register failed, ret = %d\n", ret);
		goto device_register_fail;
	}

	ret = platform_driver_register(&imgdec_driver);
	if (ret < 0)
	{
		ERR("imgdec driver register failed, ret = %d\n", ret);
		goto driver_register_fail;
	}

	return 0;

driver_register_fail:
	platform_device_unregister(&imgdec_device);
device_register_fail:

	return ret;
}

static void __exit imgdec_exit(void)
{
	INFO("IMG_DEC module exit\n");

	platform_driver_unregister(&imgdec_driver);
	platform_device_unregister(&imgdec_device);

	imgdec_hal_reg_unmmap();

	return;
}

module_init(imgdec_init);
module_exit(imgdec_exit);

/**************************************************************************
 *                  M O D U L E    D E C L A R A T I O N                      *
 **************************************************************************/

MODULE_AUTHOR("Sunplus");
MODULE_DESCRIPTION("IMG_DEC Driver");
MODULE_LICENSE("GPL");
MODULE_VERSION("1.0");