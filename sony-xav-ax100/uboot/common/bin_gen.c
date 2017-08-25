#include <common.h>
#include <malloc.h>
#include <command.h>
#include <asm/io.h>
#include <asm/hardware.h>
#include <u-boot/md5.h>
#include <../arch/arm/include/asm/armv7.h>
#include <sp_bootinfo.h>

typedef unsigned int UINT32;
typedef unsigned char UINT8;

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

UINT32 bingen_extract(UINT32 addr , UINT8 *name ,UINT32* logosize)
{
	
	const char *arg;
	int ret = -1,i=0;
	UINT32 dest;
	bin_file_header *b_header = (bin_file_header *) addr;
	part_file_header * p_header = (part_file_header *)(b_header->part_header_offset+addr);

	//printf("file name %s\n",name);

	if((b_header->token[0]=='P')&&(b_header->token[1]=='A')&&(b_header->token[2]=='R')&&(b_header->token[3]=='T'))
	{
		while(&(p_header[i])<(b_header->part_header_len+b_header->part_header_offset+addr))
		{                  
			if(strncmp(p_header[i].name,name,strlen(name))==0)
			{
				dest = p_header[i].offset  ;
				*logosize = p_header[i].size  ;
				//printf("file %s addr 0x%x \n",p_header[i].name,dest);
				return dest;		
			}
			i++;		
		}
	}
	return -1;
}


