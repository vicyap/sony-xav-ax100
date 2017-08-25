
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

#define LOGO_NAME "logo"

typedef struct _logo_list
{
	unsigned char logo_name[24];
	//void (*func_ptr)(const void *);
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

#define	SWAP32(x)	((((UINT32)(x))&0x000000ff)<<24 | (((UINT32)(x))&0x0000ff00)<<8 | (((UINT32)(x))&0x00ff0000)>>8 | (((UINT32)(x))&0xff000000)>>24)

extern UINT32 bingen_extract(UINT32 addr , UINT8 *name ,UINT32* logosize);

static logo_list nor_logo_list[] =
{
	"logo.bin",
};

int set_normal_logo_addr(UINT32 mAddr,UINT32 logo_num)
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
    UINT8 fMalloc = 0;
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
    
	bfh = malloc(pagesize);   
	
	sprintf(cmd, "nand getaddr %s 0x%x", LOGO_NAME, &pAddr);
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
		
		strcpy(&name,&nor_logo_list[logo_num].logo_name);

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
				printf("Can not find %s in %s partition\n",name,LOGO_NAME);
				ret = -1;				
			}
		}

	}
	else
	{
		printf("Not isp binary generate format!!\n");
		ret =  -1;
	}    
	
	if(fMalloc == 1)
		free(bpfh);
	free(bfh);
    return ret;
}

void show_normal_logo(UINT32 logo_num)
{
	UINT32 numBuf;
	UINT32 addrBuf;
	OSD_Get_Free_BufAddr(&numBuf,&addrBuf);
	//printf("free %d 0x%x\n",numBuf,addrBuf);	
	set_normal_logo_addr(addrBuf,logo_num);
	OSD_Set_VisableBuf(0,numBuf);	
}

