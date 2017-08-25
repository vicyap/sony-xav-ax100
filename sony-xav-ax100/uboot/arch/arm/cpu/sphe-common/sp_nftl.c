/*
 * (C) Copyright 2016
 * Sunplus Technology
 * Mason Yang <mason.yang@sunplus.com>
 *
 * SPDX-License-Identifier:     GPL-2.0+
 */


#include "gp_nand_hal/platform/nand_platform.h"
#include <part.h>

#include "sp_bch.h"
#include "sp_nand.h"
#include "sp_cbdma.h"


DECLARE_GLOBAL_DATA_PTR;

struct sp_nftl {
	uint version;
	void *priv;		
	block_dev_desc_t block_dev;	
};

static struct sp_nftl sp_nftl_dev;

#ifdef CONFIG_PARTITIONS
block_dev_desc_t *spnftl_get_dev(int dev)
{
	struct sp_nftl *sp_nftl = &sp_nftl_dev;	
	return &sp_nftl->block_dev;
}

extern UINT32 GetNoFSAreaSectorSize(void);
extern void DrvNand_Sync_flush_all(void);
extern int hal_nand_init(void);
extern unsigned short DrvNand_initial(void);
extern unsigned int DrvNand_get_Size(void);
extern void spare_buf_init(void);

unsigned long spnftl_stor_read(int device, lbaint_t blknr,
			    lbaint_t blkcnt, void *buffer)
{
	unsigned long start = (unsigned long)buffer;
	unsigned long stop = start + (blkcnt<<9);
	
	invalidate_dcache_range(start, stop);		
		
	DrvNand_read_sector_NoSem((blknr+GetNoFSAreaSectorSize()), blkcnt, (UINT32)buffer,0);
	// DBG 	printf("%s,lba=%x,cnt=%d,buf[0]=0x%x\n",__FUNCTION__,blknr,blkcnt,*(unsigned int*)start);	
	return blkcnt;
}			    

unsigned long spnftl_stor_write(int device, lbaint_t blknr,
			    lbaint_t blkcnt, void *buffer)
{
	unsigned long start = (unsigned long)buffer;
	unsigned long stop = start + (blkcnt<<9);

	flush_dcache_range(start, stop);
	invalidate_dcache_range(start, stop);	
	
	// DBG 	printf("%s,lba=0x%x,cnt=%d,buf[0]=0x%x\n",__FUNCTION__,blknr,blkcnt,*(unsigned int*)start);
	DrvNand_write_sector_NoSem((blknr+GetNoFSAreaSectorSize()), blkcnt, (UINT32)buffer,0);
	DrvNand_Sync_flush_all( );
	return blkcnt;
}			    

#endif



static int sp_nand_ftl(cmd_tbl_t *cmdtp, int flag, int argc,
			  char * const argv[])
{
  struct sp_nftl *sp_nftl = &sp_nftl_dev;
  static int flag_init=0;		
	
	
	if( (strcmp(argv[1],"sync") == 0) && (flag_init == 1) ){
		printf("Sync data ...");
		DrvNand_Sync_flush_all( );
		printf(".... Done\n");
		return 0;
	}	
	
	hal_nand_init();	
	DrvNand_initial();
	flag_init=1;
	
#ifdef CONFIG_PARTITIONS
   
   sp_nftl->block_dev.if_type = IF_TYPE_SPNFTL;
   sp_nftl->block_dev.dev = 0;
   sp_nftl->block_dev.removable = 0;
   sp_nftl->block_dev.block_read = spnftl_stor_read;
   sp_nftl->block_dev.block_write =spnftl_stor_write;
   sp_nftl->block_dev.block_erase =0;
   sp_nftl->block_dev.lun = 0;
   sp_nftl->block_dev.part_type = PART_TYPE_UNKNOWN;
   sp_nftl->block_dev.type = 0;
   sp_nftl->block_dev.blksz = 512;
   sp_nftl->block_dev.log2blksz = LOG2(512);
   sp_nftl->block_dev.lba = DrvNand_get_Size()- GetNoFSAreaSectorSize();
   strcpy(sp_nftl->block_dev.vendor,"Sunplus");
   strcpy(sp_nftl->block_dev.product,"NAND-FTL");
   strcpy(sp_nftl->block_dev.revision,"ver-01");
#endif	
   // DBG printf("NAND FTL blk write addr:0x%p,DevSize(Max LBA):0x%x\n",spnftl_stor_write,sp_nftl->block_dev.lba);
  
  
	return 0;
}

U_BOOT_CMD(
	sp_ftl, 2, 1, sp_nand_ftl,
	"Sunplus NAND Data FTL Utility",
	"S+ NAND Data FTL\n"
);


extern SINT32 NandAppFlush(void);
extern UINT32 NandAppFormat(UINT16 format_type);
extern SINT32 NandAppInit(void);

static int app_init_flag=0;

static int sp_app_nftl(cmd_tbl_t *cmdtp, int flag, int argc,
			  char * const argv[])
{
  char *cmd;  
  ulong off,size,ram_addr;
  
   		
	extern int hal_nand_init(void);
	
	if (argc < 2) {
		printf("usage: sp_appftl <function> [argument ... ] {comment}\n");
		printf("i.e. sp_appftl format\n");		
		printf("     sp_appftl init\n");
		printf("     sp_appftl write ram_addr lba_addr sector-size {sector-size=1 is 512 Byte}\n");
		printf("     sp_appftl read  ram_addr lba_addr sector-size {sector-size=1 is 512 Byte}\n");
		printf(" All number is in Hex\n");
		return 0;
	}
	
	cmd = argv[1];
	
	if( app_init_flag == 0){
		hal_nand_init();
		spare_buf_init();		
	}
	
	if( strcmp(cmd,"format") == 0 ){
		
		NandAppFormat(0x10);
		printf("S+ NAND App FTL format done\n");
	}else{
		if( app_init_flag == 0)
		  printf("ByPass S+ NAND App FTL format !!! \n");
		app_init_flag = 1;
	}
	
	
		NandAppInit();
		//printf("S+ NAND App FTL init done\n");
		
	
  if( strcmp(cmd,"write") == 0 ){
	  ram_addr = (ulong)simple_strtoul(argv[2], NULL, 16);
		off = (ulong)simple_strtoul(argv[3], NULL, 16);
		size = (ulong)simple_strtoul(argv[4], NULL, 16);
	  printf("S+ NAND App FTL write %ld bytes from 0x%lx to LBA:0x%lx\n",size<<9,ram_addr,off);
	  NandAppWriteSector(off,size,ram_addr); //	1 sectors is 512 Byte 
	  NandAppFlush();
	}
	
	if( strcmp(cmd,"read") == 0 ){// && (argc > 2) && (argc < 4) ){
	  ram_addr = (ulong)simple_strtoul(argv[2], NULL, 16);
		off = (ulong)simple_strtoul(argv[3], NULL, 16);
		size = (ulong)simple_strtoul(argv[4], NULL, 16);
	  printf("S+ NAND App FTL read %ld bytes from LBA:0x%lx to 0x%lx\n",size<<9,off,ram_addr);
	  NandAppReadSector(off,size,ram_addr);	 // 1 sector is 512 Byte  
	}

	return 0;
}

U_BOOT_CMD(
	sp_appftl, 5, 1, sp_app_nftl,
	"Sunplus NAND APP FTL Utility",
	"S+ NAND APP FTL\n"
);

