
#include <common.h>

#ifdef USE_HOSTCC
# include <ctype.h>
#else
#include <linux/ctype.h>
#endif

#define UBOOT_TCON
#include "drivers/tcon/hal_SPHE8388_tcon.h"
#undef UBOOT_TCON
#include "drivers/gpio/sp_gpio.h"
#include <nand.h>
#include <video_font_data.h>

#define PAGEALIGN(x)	((x + 4095) >> 12 << 12)
#define WORDALIGN(x)	((x + 4) >> 2 << 2)

#define PART_NAME "pat_logo"
#define ISP_NAME "isp_logo"

typedef struct _logo_list
{
	unsigned char logo_name[24];
	void (*set_ptr)(const void *);
	UINT32 (*get_ptr)();
	UINT32 logosize;
}logo_list;

typedef struct _partition_file_header
{
	unsigned char name[24];
	unsigned int size;
	unsigned int offset;
}part_file_header;

typedef struct _bin_file_header
{
	unsigned char token[4];
	unsigned int total_size;
	unsigned int part_header_offset;
	unsigned int part_header_len;
}bin_file_header;

static UINT32 logo_ok_ADDR = 0;
static UINT32 logo_failed_ADDR = 0;
static UINT32 logo_begin_ADDR = 0;
static char *lang_name;
#define	SWAP32(x)	((((UINT32)(x))&0x000000ff)<<24 | (((UINT32)(x))&0x0000ff00)<<8 | (((UINT32)(x))&0x00ff0000)>>8 | (((UINT32)(x))&0xff000000)>>24)

extern UINT32 bingen_extract(UINT32 addr , UINT8 *name ,UINT32* logosize);

#ifdef ISP_FLOW_STATE_LED_FLASH
void isp_flow_LED_initial()
{
    GPIO_F_SET(ISP_LED_GPIO,eHW_GPIO_FIRST_GPIO);
    GPIO_M_SET(ISP_LED_GPIO,eHW_GPIO_RISC);
    GPIO_E_SET(ISP_LED_GPIO,eHW_GPIO_OUT);
}
void set_logo_ok_LED()
{
	int i,state;
	state = ISP_LED_ON;
	for(i=0;i<20;i++)
	{
		state =!state;
		GPIO_O_SET(ISP_LED_GPIO,state);
		mdelay(1000);
	}
	GPIO_O_SET(ISP_LED_GPIO,ISP_LED_ON);
}

void set_logo_failed_LED()
{
	int i,state;
	state = ISP_LED_ON;
	for(i=0;i<40;i++)
	{
		state =!state;
		GPIO_O_SET(ISP_LED_GPIO,state);
		mdelay(200);
	}
	GPIO_O_SET(ISP_LED_GPIO,ISP_LED_ON);
}

void set_logo_begin_LED()
{
	int i,state;
	state = ISP_LED_ON;
	for(i=0;i<6;i++)
	{
		GPIO_O_SET(ISP_LED_GPIO,state);
		mdelay(500);
		state =!state;
	}
	GPIO_O_SET(ISP_LED_GPIO,ISP_LED_ON);
}
#endif
void set_lang_name(UINT32 flag)
{
    lang_name = (char *)flag;
}

void set_logo_ok_addr(UINT32 addr)
{
	logo_ok_ADDR = addr;
}

void set_logo_failed_addr(UINT32 addr)
{
	logo_failed_ADDR = addr;
}

void set_logo_begin_addr(UINT32 addr)
{
	logo_begin_ADDR = addr;
}

UINT32 get_logo_ok_addr()
{
	return logo_ok_ADDR ;
}

UINT32 get_logo_failed_addr()
{
	return logo_failed_ADDR ;
}

UINT32 get_logo_begin_addr()
{
	return logo_begin_ADDR ;
}

static logo_list isp_logo_list[] =
{
   {"isp_ok.bin",set_logo_ok_addr,get_logo_ok_addr,0},
   {"isp_failed.bin",set_logo_failed_addr,get_logo_failed_addr,0},
   {"isp_begin.bin",set_logo_begin_addr,get_logo_begin_addr,0},
};
enum
{
	ISP_OK=0,
	ISP_FAILED,
	ISP_BEGIN,
};
static logo_list pat_logo_list[] =
{
   {"pat_ok.bin",NULL,NULL,NULL},
   {"pat_failed.bin",NULL,NULL,NULL},
   {"pat_begin.bin",NULL,NULL,NULL},
   {"pat_retry.bin",NULL,NULL,NULL},
};
enum
{
	PAT_OK=0,
	PAT_FAILED,
	PAT_BEGIN,
	PAT_RETRY,
};
int set_update_logo_addr(UINT32 mAddr,UINT32 logo_num)
{
    int i,ret=-1;
    UINT32 tAddr;
    UINT32 pAddr;
    UINT32 temp;
    UINT32 sub;
    UINT32 pagesize;
    UINT32 count;
    UINT32 logosize;
    UINT32 *tp;
    UINT8 fMalloc=0;
    UINT8 name[24];
    char cmd[256];
    bin_file_header *bfh;
    part_file_header *bpfh;
    part_file_header *pfh;
    nand_info_t *nand;
    int dev;

    dev = nand_curr_device;
    nand = &nand_info[dev];
    pagesize = nand->writesize;

    if(SP_IS_ISPBOOT())
    {
        for (i = 0;i<sizeof(isp_logo_list)/sizeof(logo_list);i++)
        {
                tAddr= bingen_extract(mAddr,isp_logo_list[i].logo_name,&(isp_logo_list[i].logosize));
                if(tAddr!=-1)
                    isp_logo_list[i].set_ptr(tAddr+mAddr);
        }
        #ifdef ISP_FLOW_STATE_LED_FLASH
        isp_flow_LED_initial();
        #endif
		ret = 0;
    }else
    {

		bfh = malloc(pagesize);

		sprintf(cmd, "nand getaddr %s 0x%x", PART_NAME, &pAddr);
		//printf("%s\n", cmd);
		run_command(cmd, 0);

        sprintf(cmd, "nand read 0x%x 0x%x 0x%x", (u32)(bfh), pAddr, pagesize);
		//printf("%s\n", cmd);
        run_command(cmd, 0);

        if((bfh->token[0]=='P')&&(bfh->token[1]=='A')&&(bfh->token[2]=='R')&&(bfh->token[3]=='T'))
        {
			if((sizeof(bin_file_header)+bfh->part_header_len)<=pagesize)
			{
				bpfh = bfh;
				pfh = (UINT32)bfh+bfh->part_header_offset;
			}else
			{
				bpfh = malloc(sizeof(bin_file_header)+bfh->part_header_len);
				fMalloc = 1;
				sprintf(cmd, "nand read 0x%x 0x%x 0x%x", (u32)(bpfh), pAddr, bfh->part_header_len+sizeof(bin_file_header));
				//printf("%s\n", cmd);
				run_command(cmd, 0);

				temp =(UINT32)bpfh;
				temp = temp+sizeof(bin_file_header);
				pfh = (part_file_header *)temp;
			}

			//printf("bpfh 0x%x\n",bpfh);
			//printf("pfh 0x%x\n",pfh);

			tp = mAddr;

			memset(&name,0,24);

			strncpy(&name,lang_name,strlen(lang_name));
			strcat(&name,"_");
			strcat(&name,&pat_logo_list[logo_num].logo_name);

			if(strlen(name)>23)
			{
				printf("Logo name too long\n");
				ret = -1;
			}else
			{
				tAddr= bingen_extract(bpfh,&name,&logosize);
				if(tAddr !=-1)
				{
					tAddr = tAddr + pAddr;
					temp = tAddr % pagesize ;

					sprintf(cmd, "nand_offread 0x%x 0x%x 0x%x 0x%x", (u32)(tp), tAddr-temp, logosize,temp);
					//printf("%s\n", cmd);
					run_command(cmd, 0);
					ret = 0;
				}
				else
				{
					printf("Can not find %s in %s partition\n",name,PART_NAME);
					ret = -1;
				}
			}
        }
        else
        {
            printf("Not isp binary generate format!!\n");
            ret = -1;
        }
    }
	if(fMalloc ==1)
		free(bpfh);
	free(bfh);
    return ret;
}
void show_update_ok_logo(void)
{
	if(SP_IS_ISPBOOT())
	{
		show_update_logo(ISP_OK);
	}else if(get_part_updt_flag())
	{
		show_update_logo(PAT_OK);
	}
}
void show_update_failed_logo(void)
{
	if(SP_IS_ISPBOOT())
	{
		show_update_logo(ISP_FAILED);
	}else if(get_part_updt_flag())
	{
		show_update_logo(PAT_FAILED);
	}
}
void show_update_begin_logo(void)
{
	if(SP_IS_ISPBOOT())
	{
		show_update_logo(ISP_BEGIN);
	}else if(get_part_updt_flag())
	{
		show_update_logo(PAT_BEGIN);
	}
}
void show_update_retry_logo(void)
{
	show_update_logo(PAT_RETRY);
}
void show_update_logo(UINT32 logo_num)
{
	UINT32 numBuf;
	UINT32 addrBuf;
	UINT32 temp;
	int ret ;
	
	if(SP_IS_ISPBOOT())
	{
		OSD_Get_Free_BufAddr(&numBuf,&addrBuf);
		memcpy(addrBuf,isp_logo_list[logo_num].get_ptr(),isp_logo_list[logo_num].logosize);
		#ifndef CONFIG_SYS_ARM_CACHE_WRITETHROUGH
		if((isp_logo_list[logo_num].logosize%32)!=0)
		{
			temp = isp_logo_list[logo_num].logosize%32;
			temp = 32-temp ;
		}else
		{
			temp = 0;
		}		
		flush_dcache_range(addrBuf, addrBuf+isp_logo_list[logo_num].logosize+temp);	
		#endif
		OSD_Set_VisableBuf(numBuf);
	}else
	{
		OSD_Get_Free_BufAddr(&numBuf,&addrBuf);
		ret = set_update_logo_addr(addrBuf,logo_num);
		if(ret == -1)
		{
			memset(addrBuf,0x00,32);
		}
		OSD_Set_VisableBuf(numBuf);
	}
}

void show_logo_progress_time(UINT32 total_time,UINT32 now_time)
{
	UINT32 h ,w,per,j;
	static UINT32 old_per =0;
	UINT32 temp;
	UINT32 width = OSD_Get_Buf_width();
	UINT32 numBuf;
	UINT32 dst;
	OSD_Get_Disp_BufAddr(&dst);

	UINT32 * ptr = dst;
	UINT32 pr_top = 235;
	UINT32 pr_down = 270;
	UINT32 pr_left = 130;
	UINT32 pr_right = 670;

	if(old_per==0)
	{
		for(h = pr_top-1;h<=pr_down+1;h++)
		{
			for(w=pr_left-1;w<=pr_right+1;w++)
			{
				*(ptr+(width*h)+w)=0x00000000;		
				#ifndef CONFIG_SYS_ARM_CACHE_WRITETHROUGH				
				flush_dcache_range(ptr+(width*h)+w, ptr+(width*h)+w+8);		
				#endif
			}	
		}	
	}
	temp = ((double)now_time/total_time)*100;
	per = (temp*(pr_right-pr_left))/100;
	for(h= pr_top ; h<=pr_down;h++)
	{
		for(j=old_per;j<=per;j++)
		{
			*(ptr+(width*h)+pr_left+j) = 0xffff0000;	
			#ifndef CONFIG_SYS_ARM_CACHE_WRITETHROUGH
			flush_dcache_range(ptr+(width*h)+pr_left+j, ptr+(width*h)+pr_left+j+8);	
			#endif
		}
	}	
	old_per = per ;
}

int ParseVersionInfo(const char *lineBuffer, const size_t lineBufferLenght, const char *key, char *keyValue)
{
	const size_t size = lineBufferLenght;
	const char sep = '\r';
	char *data,*sp, *dp, *name, *value;
	int ret = 0;

	/* we allocate new space to make sure we can write to the array */
	if ((data = malloc(size)) == NULL) {
		return 0;
	}

	memcpy(data, lineBuffer, size);
	dp = data;
	/* ParseVersionInfo; allow for '\0' and 'sep' as separators */
	do {
		/* skip leading white space */
		while (isblank(*dp) || *dp == '\n' )
			++dp;

		/* skip comment lines */
		if (*dp == '#') {
			while (*dp && (*dp != sep))
				++dp;
			++dp;
			continue;
		}

		/* parse name */
		for (name = dp; *dp != '=' && *dp && *dp != sep; ++dp)
			;

		/* deal with "name" and "name=" entries (delete var) */
		if (*dp == '\0' || *(dp + 1) == '\0' ||
			*dp == sep || *(dp + 1) == sep) {
			if (*dp == '=')
				*dp++ = '\0';
			*dp++ = '\0';	/* terminate name */

			continue;
		}
		*dp++ = '\0';	/* terminate name */

		/* parse value; deal with escapes */
		for (value = sp = dp; *dp && (*dp != sep); ++dp) {
			if ((*dp == '\\') && *(dp + 1))
				++dp;
			*sp++ = *dp;
		}
		*sp++ = '\0';	/* terminate value */
		++dp;

		if (*name == 0) {
			return 0;
		}

		/* copy the key value for return */
		if(strcmp(key,name)==0)
		{
			printf("ParseVersionInfo, find the destination !\n");
			strcpy(keyValue,value);
			ret = 1;
			break;
		}
	} while ((dp < data + size) && *dp);	/* size check needed for text */

	free(data);

	return ret;
}

void show_logo_string(UINT32 dst, int start_x, int start_y, UINT32 font_color, char *str)
{
	//font_color format: A.R.G.B.
	const int font_zoom = 2;

	UINT32 *ptr = (UINT32 *)dst;
	int i = 0;
	int font_height_temp = 0, font_width_temp = 0;
	int x_zoom_temp = 0, y_zoom_temp = 0;
	UINT8 font_dot = 0;
	UINT32 width = OSD_Get_Buf_width();

	for(i = 0; str[i] != 0; i++)
	{
		for(font_height_temp = 0; font_height_temp < VIDEO_FONT_HEIGHT; font_height_temp++)
		{
			font_dot = video_fontdata[str[i]*VIDEO_FONT_HEIGHT + font_height_temp];
			for(font_width_temp = 0; font_width_temp < VIDEO_FONT_WIDTH; font_width_temp++)
			{
				if((font_dot<<font_width_temp)&0x80)
				{
					for(y_zoom_temp = 0; y_zoom_temp < font_zoom; y_zoom_temp++)
					{
						for(x_zoom_temp = 0; x_zoom_temp < font_zoom; x_zoom_temp++)
						{
							*(ptr+(start_y+font_height_temp*font_zoom+y_zoom_temp)*width+start_x+font_width_temp*font_zoom+x_zoom_temp) = font_color;
							#ifndef CONFIG_SYS_ARM_CACHE_WRITETHROUGH
							flush_dcache_range(ptr+(start_y+font_height_temp*font_zoom+y_zoom_temp)*width+start_x+font_width_temp*font_zoom+x_zoom_temp, ptr+(start_y+font_height_temp*font_zoom+y_zoom_temp)*width+start_x+font_width_temp*font_zoom+x_zoom_temp+8);			
							#endif
						}
					}
				}
			}
		}
		start_x += VIDEO_FONT_WIDTH*font_zoom;
	}
}

void show_logo_version()
{
	char strTemp[64] = {0};
	UINT32 numBuf;
	UINT32 dst;
	OSD_Get_Disp_BufAddr(&dst);
	if(!ParseVersionInfo((char *)CONFIG_SP_OLD_VERSION_ADDR,CONFIG_SP_OLD_VERSION_SIZE,"CustomerVersion",strTemp))
	{
		strcpy(strTemp,"<Unkown>");
	}
	show_logo_string(dst, 56,64,0xff7faf00,"Old CustomerVersion:");
	show_logo_string(dst, 56+16*21,64,0xff00af00,strTemp);
	if(!ParseVersionInfo((char *)CONFIG_SP_OLD_VERSION_ADDR,CONFIG_SP_OLD_VERSION_SIZE,"SdkPublishVersion",strTemp))
	{
		strcpy(strTemp,"<Unkown>");
	}
	show_logo_string(dst, 56,128,0xff7faf00,"Old SdkPublishVersion:");
	show_logo_string(dst, 56+16*23,128,0xff00af00,strTemp);

	if(!ParseVersionInfo((char *)CONFIG_SP_NEW_VERSION_ADDR,CONFIG_SP_NEW_VERSION_SIZE,"CustomerVersion",strTemp))
	{
		strcpy(strTemp,"<Unkown>");
	}
	show_logo_string(dst, 56,320,0xffaf7f00,"New CustomerVersion:");
	show_logo_string(dst, 56+16*21,320,0xff0000ff,strTemp);
	if(!ParseVersionInfo((char *)CONFIG_SP_NEW_VERSION_ADDR,CONFIG_SP_NEW_VERSION_SIZE,"SdkPublishVersion",strTemp))
	{
		strcpy(strTemp,"<Unkown>");
	}
	show_logo_string(dst, 56,384,0xffaf7f00,"New SdkPublishVersion:");
	show_logo_string(dst, 56+16*23,384,0xff0000ff,strTemp);
}
void show_read_failed_string()
{
	UINT32 dst;
	OSD_Get_Disp_BufAddr(&dst);
	show_logo_string(dst, 56,50,0xff7faf00,"Read file failed");
}

