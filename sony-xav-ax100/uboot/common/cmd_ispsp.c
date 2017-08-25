/*
 * (C) Copyright 2015
 * Sunplus Technology. <http://www.sunplus.com/>
 *
 * SPDX-License-Identifier:     GPL-2.0+
 */

#include <common.h>
#include <linux/mtd/mtd.h>
#include <command.h>
#include <malloc.h>
#include <asm/byteorder.h>
#include <jffs2/jffs2.h>
#include <nand.h>

#if defined(CONFIG_AES)
#include <aes.h>
#elif defined(CONFIG_AES_FAST)
// #include <aes_fast.h>
#define AES_KEY_LENGTH          (128 / 8)
#else
#define AES_KEY_LENGTH          (128 / 8)
#endif

#include <../arch/arm/cpu/sphe-common/iop.h>
#define UBOOT_TCON
#include "drivers/tcon/hal_SPHE8388_tcon.h"
#undef UBOOT_TCON
#include "sp_config.h"

#define ISP_RAM_ADDR                (CONFIG_SP_ISP_ADDR) // which equal to (CONFIG_SP_HW_BUF_ADDR), size: CONFIG_SP_ISP_SIZE

#define ISP_CMD_ALL             0
#define ISP_CMD_UPDATE          1
#define ISP_CMD_CHK4BADBLOCK    2
#define ISP_CMD_PROGRESS        3
#define ISP_CMD_FIND_AREA       4
#define ISP_CMD_MARKBAD4BCHERR  5

#define SIZE_INIT_SCRIPT        (2048)  // must be the same as defined in build/tools/isp/isp.c

#define MAX_ISP_TRY_CNT         (5)

extern u32 ispsp_chk4badblock_max_err_bit_in_page;
// extern u32 ispsp_chk4badblock_err_bit_in_page;
extern u32 ispsp_chk4badblock_ecc_size;
extern u32 ispsp_chk4badblock_ecc_strength;
extern void write_regs(unsigned int regGroupNum, unsigned int regIndex, unsigned int value);

#ifdef CONFIG_SP_ISP_LOGO
int isp_version_info_initialized = 0;
static int isp_logo_initialized = 0;
static int part_logo_initialized = 0;
static UINT32 isp_total_time;
static UINT32 isp_now_time;
#endif

static UINT32 part_image_offset;

void set_part_image_offset(UINT32 offset)
{
	part_image_offset = offset;
}

static int do_ispsp_find_area(int argc, char *const argv[])
{
    static u32 block_size = 0;
    u32 offset;
    u32 size_without_badblock, size_with_badblock, size;
    u32 test_addr;
    nand_info_t *nand;

    if(block_size == 0) {
        run_command("nand info && nand bad", 0);

        block_size = (u32)(getenv_hex("nand_erasesize", 0));
        if(block_size == 0) {
            printf("%s, %s, %d\n", __func__, __FILE__, __LINE__);
            return -1;
        }
    }

    nand = &nand_info[0];
    if(nand->erasesize != block_size) {
        printf("error: %s, %d\n", __FILE__, __LINE__);
        return -1;
    }

    offset  = (u32)(simple_strtoul(argv[2], NULL, 16));
    if((offset & (block_size - 1)) != 0)  {
        printf("%s: offset must be block aligned\n", __func__);
        return -1;
    }

    size_without_badblock = (u32)(simple_strtoul(argv[3], NULL, 16));
    if((size_without_badblock & (block_size - 1)) != 0)  {
        printf("%s: size must be block aligned\n", __func__);
        return -1;
    }

    // printf("find 0x%x bytes (bad blocks excluded) start from 0x%x\n", size_without_badblock, offset);

    size_with_badblock = 0;
    for(size=0; size<size_without_badblock; ) {
        test_addr = offset + size_with_badblock;
        if(nand_block_isbad(nand, (loff_t)(test_addr))) {
            printf("bad block at 0x%x\n", test_addr);
        } else {
            size += block_size;
        }

        size_with_badblock += block_size;
    }

    // printf("size_without_badblock = 0x%x, size_with_badblock = 0x%x\n", size_without_badblock, size_with_badblock);
    setenv_hex("isp_find_area_start", offset);
    setenv_hex("isp_find_area_new_size", size_with_badblock);
    setenv_hex("isp_find_area_end", (offset + size_with_badblock));

    run_command("printenv isp_find_area_start", 0);
    run_command("printenv isp_find_area_new_size", 0);
    run_command("printenv isp_find_area_end", 0);

    return 0;
}

// #define CHK4BADBLOCK
#ifdef CHK4BADBLOCK
static int do_ispsp_chk4badblock(int argc, char *const argv[])
{
    u32 block_size, page_size, off, size, test_addr;
    char cmd[256];
    u32 isp_ram_addr = ISP_RAM_ADDR;
    nand_info_t *nand;
    const u8 pattern[] = {0x00}; // {0x55, 0xaa};
    u32 i, j, repeat_count;
    u32 threshold_err_bit_cnt_per_page;

    run_command("nand info && nand bad", 0);
    run_command("mtdparts", 0); // without running it, mtd info is not available.

    block_size = (u32)(getenv_hex("nand_erasesize", 0));
    if(block_size == 0) {
        printf("%s, %s, %d\n", __func__, __FILE__, __LINE__);
        return -1;
    }

    nand = &nand_info[0];
    if(nand->erasesize != block_size) {
        printf("error: %s, %d\n", __FILE__, __LINE__);
        return -1;
    }
    else
        page_size = nand->writesize;

    off  = (u32)(simple_strtoul(argv[2], NULL, 16));
    if((off & (block_size - 1)) != 0)  {
        printf("%s: off must be block aligned\n", __func__);
        return -1;
    }

    size = (u32)(simple_strtoul(argv[3], NULL, 16));
    if((size & (block_size - 1)) != 0)  {
        printf("%s: size must be block aligned\n", __func__);
        return -1;
    }

    if(argc > 4)
        repeat_count = (u32)(simple_strtoul(argv[4], NULL, 16));
    else
        repeat_count = 1;

    threshold_err_bit_cnt_per_page = ispsp_chk4badblock_ecc_strength * page_size / ispsp_chk4badblock_ecc_size;
    threshold_err_bit_cnt_per_page *= 31;
    threshold_err_bit_cnt_per_page /= 40;   // 31/40 = 0.775
    printf("BCH strength: allow %u bits error per %u bytes\n", ispsp_chk4badblock_ecc_strength, ispsp_chk4badblock_ecc_size);
    printf("threshold_err_bit_cnt_per_page = %u\n", threshold_err_bit_cnt_per_page);

    for(j=0; j<repeat_count; j++) {
        if(repeat_count > 1) {
            printf("\n\nLoop 0x%08x:\n\n", j);
        }

        for(test_addr=off; test_addr<(off+size); test_addr+=block_size) {
            if(nand_block_isbad(nand, (loff_t)(test_addr))) {
                printf("bad block @ 0x%08x\n", test_addr);
                continue;
            }

            for(i=0; i<(sizeof(pattern)/sizeof(pattern[0])); i++) {
                snprintf(cmd, sizeof(cmd), "nand erase 0x%x 0x%x", test_addr, block_size);
                printf("%s\n", cmd); run_command(cmd, 0);

                memset((void *)(isp_ram_addr), pattern[i], block_size);

                snprintf(cmd, sizeof(cmd), "nand write 0x%x 0x%x 0x%x", isp_ram_addr, test_addr, block_size);
                printf("%s\n", cmd); run_command(cmd, 0);

                ispsp_chk4badblock_max_err_bit_in_page = 0;
                memset((void *)(isp_ram_addr+block_size), (pattern[i]^0xff), block_size);
                snprintf(cmd, sizeof(cmd), "nand read 0x%x 0x%x 0x%x", isp_ram_addr+block_size, test_addr, block_size);
                printf("%s\n", cmd); run_command(cmd, 0);

                if( (ispsp_chk4badblock_max_err_bit_in_page > threshold_err_bit_cnt_per_page) ||
                    (memcmp((void *)(isp_ram_addr), (void *)(isp_ram_addr+block_size), block_size) != 0) ) {
                    printf("\n\nMark address 0x%x as bad block\n", test_addr);

                    snprintf(cmd, sizeof(cmd), "nand markbad 0x%x", test_addr);
                    printf("%s\n", cmd); run_command(cmd, 0);

                    snprintf(cmd, sizeof(cmd), "nand bad");
                    printf("%s\n", cmd); run_command(cmd, 0);

                    break;
                }
            }
        }
    }

    snprintf(cmd, sizeof(cmd), "nand bad");
    printf("%s\n", cmd); run_command(cmd, 0);

    return 0;
}
#endif /* CHK4BADBLOCK */

#define ISP_RETRY_CNT_STRING        "isp_retry_cnt="
static u32 isp_retry_cnt_rw(u32 rw) // (rw == 0): read, (rw != 0): write, value be be increased
{
    char cmd[256];
    u32 isp_retry_cnt_ram_addr = ISP_RAM_ADDR + (8 << 20);
    static u32 isp_retry_cnt = ~0;

    if(isp_retry_cnt == ~0) {
        run_command("nand info && nand bad", 0);
        snprintf(cmd, sizeof(cmd), "nand read 0x%x 0x0 0x100 && md.b 0x%x 0x100", isp_retry_cnt_ram_addr, isp_retry_cnt_ram_addr);
        printf("%s\n", cmd); run_command(cmd, 0);
        if(memcmp((const void *)(isp_retry_cnt_ram_addr), ISP_RETRY_CNT_STRING, (sizeof(ISP_RETRY_CNT_STRING) - 1)) == 0) {
            isp_retry_cnt = (u32)(simple_strtoul((const char *)(isp_retry_cnt_ram_addr + sizeof(ISP_RETRY_CNT_STRING) - 1), NULL, 10));
        } else
            isp_retry_cnt = 0;

        printf("init: isp_retry_cnt = %u\n", isp_retry_cnt);
    }

    if(rw) {
        isp_retry_cnt += rw;
        printf("write isp_retry_cnt = %u to NAND\n", isp_retry_cnt);
        run_command("nand erase 0x0 0x100", 0);
        memset((void *)(isp_retry_cnt_ram_addr), 0, 0x100);
        snprintf((char *)(isp_retry_cnt_ram_addr), 0x100, "%s%u ", ISP_RETRY_CNT_STRING, isp_retry_cnt);
        snprintf(cmd, sizeof(cmd), "nand write 0x%x 0x0 0x100", isp_retry_cnt_ram_addr);
        printf("%s\n", cmd); run_command(cmd, 0);
    }

    // test
    // snprintf(cmd, sizeof(cmd), "nand read 0x%x 0x0 0x100 && md.b 0x%x 0x100", isp_retry_cnt_ram_addr, isp_retry_cnt_ram_addr);
    // printf("%s\n", cmd); run_command(cmd, 0);

    return isp_retry_cnt;
}

#define MAX_BCH_ERR_BLK_ALLOWED     (5)
extern unsigned int isp_ng_blk_addr;
static int do_ispsp_markbad4bcherr(int argc, char *const argv[])
{
    u32 block_size, page_size, nand_size, test_addr;
    char cmd[256];
    u32 isp_ram_addr = ISP_RAM_ADDR;
    nand_info_t *nand;
    u32 flag_all;
    u32 bch_err_blk_cnt = 0;

    if( (argc >= 3) && (strcmp(argv[2], "all") == 0) )
        flag_all = 1;
    else
        flag_all = 0;

    run_command("nand info && nand bad", 0);

    block_size = (u32)(getenv_hex("nand_erasesize", 0));
    if(block_size == 0) {
        printf("%s, %s, %d\n", __func__, __FILE__, __LINE__);
        return -1;
    }

    nand = &nand_info[0];
    if(nand->erasesize != block_size) {
        printf("error: %s, %d\n", __FILE__, __LINE__);
        return -1;
    }
    else {
        page_size = nand->writesize;
        nand_size = (u32)(nand->size);
        // printf("nand_size = 0x%x\n", nand_size);
    }

    isp_retry_cnt_rw(1);
    if(isp_retry_cnt_rw(0) > MAX_ISP_TRY_CNT)
        run_command("setenv isp_force_to_abort yes", 0);

    if(isp_ng_blk_addr) {
        bch_err_blk_cnt++;
        printf("Mark address 0x%x as bad block\n", isp_ng_blk_addr);

        snprintf(cmd, sizeof(cmd), "nand markbad 0x%x", isp_ng_blk_addr);
        printf("%s\n", cmd); run_command(cmd, 0);

        snprintf(cmd, sizeof(cmd), "nand bad");
        printf("%s\n", cmd); run_command(cmd, 0);
    }

    test_addr = isp_ng_blk_addr;
    if(flag_all) {
        for( ; test_addr<nand_size; test_addr+=block_size) {
            isp_ng_blk_addr = 0;
            snprintf(cmd, sizeof(cmd), "nand read 0x%x 0x%x 0x%x", isp_ram_addr, test_addr, block_size);
            run_command(cmd, 0);
            if(isp_ng_blk_addr) {
                bch_err_blk_cnt++;
                snprintf(cmd, sizeof(cmd), "nand markbad 0x%x", isp_ng_blk_addr);
                printf("%s\n", cmd); run_command(cmd, 0);
                snprintf(cmd, sizeof(cmd), "nand bad");
                printf("%s\n", cmd); run_command(cmd, 0);

                if(bch_err_blk_cnt > MAX_BCH_ERR_BLK_ALLOWED)
                    return -1;
            }
        }
    }

    return 0;
}

static int do_ispsp_progress(int argc, char *const argv[])
{
    u32 isp_size_total;
    u32 isp_RTCFG_offset_in_ISPBOOOT, isp_RTCFG_size_in_ISPBOOOT;
    u32 isp_tcon_offset_in_ISPBOOOT, isp_tcon_size_in_ISPBOOOT;
	#ifdef CONFIG_SP_ISP_LOGO
    u32 isp_logo_offset_in_ISPBOOOT, isp_logo_size_in_ISPBOOOT;
    u32 version_info_offset_in_ISPBOOOT, version_info_size_in_ISPBOOOT;
	#endif 
	
    u32 size_write, size_verify;
    static u32 size_written  = ~0;
    static u32 size_verified = ~0;
    static u32 RTCFG_initialized = 0;
    static u32 tcon_initialized = 0;
    char cmd[256];
    u8 *ptr, *ptr2;

    size_write  = (u32)(simple_strtoul(argv[2], NULL, 16));
    size_verify = (u32)(simple_strtoul(argv[3], NULL, 16));
    if((size_write == 0) && (size_verify == 0)) {
        size_written  = 0;
        size_verified = 0;
    }

    size_written  += size_write;
    size_verified += size_verify;

    isp_size_total = getenv_hex("isp_size_total", 0);

    isp_RTCFG_offset_in_ISPBOOOT = getenv_hex("isp_RTCFG_offset_in_ISPBOOOT", 0); // it would be 0 if partial update
    isp_RTCFG_size_in_ISPBOOOT   = getenv_hex("isp_RTCFG_size_in_ISPBOOOT", 0);   // it would be 0 if partial update

    isp_tcon_offset_in_ISPBOOOT = getenv_hex("isp_tcon_offset_in_ISPBOOOT", 0); // it would be 0 if partial update
    isp_tcon_size_in_ISPBOOOT   = getenv_hex("isp_tcon_size_in_ISPBOOOT", 0);   // it would be 0 if partial update

    if(isp_RTCFG_offset_in_ISPBOOOT && !RTCFG_initialized) {
        ptr = malloc(isp_RTCFG_size_in_ISPBOOOT + ARCH_DMA_MINALIGN -1);
        if(ptr) {
            ptr2 = (u8 *)(((u32)(ptr) + (ARCH_DMA_MINALIGN -1)) & (~(ARCH_DMA_MINALIGN -1)));
            sprintf(cmd, "fatload $isp_if $isp_dev 0x%x /ISPBOOOT.BIN 0x%x 0x%x\n", (u32)(ptr2), isp_RTCFG_size_in_ISPBOOOT, isp_RTCFG_offset_in_ISPBOOOT);
            run_command(cmd, 0);
            sp_config_isp_init(ptr2);
            //free(ptr);
            RTCFG_initialized = 1;
        }
        else {
            printf("Error: %s, %d\n", __FILE__, __LINE__);
            while(1);
        }
    }

    if(isp_tcon_offset_in_ISPBOOOT && !tcon_initialized) {
        ptr = malloc(isp_tcon_size_in_ISPBOOOT + ARCH_DMA_MINALIGN -1);
        if(ptr) {
            ptr2 = (u8 *)(((u32)(ptr) + (ARCH_DMA_MINALIGN -1)) & (~(ARCH_DMA_MINALIGN -1)));
            sprintf(cmd, "fatload $isp_if $isp_dev 0x%x /ISPBOOOT.BIN 0x%x 0x%x\n", (u32)(ptr2), isp_tcon_size_in_ISPBOOOT, isp_tcon_offset_in_ISPBOOOT);
            run_command(cmd, 0);

            sprintf(cmd, "md.b 0x%x 0x100\n", (u32)(ptr2));
            run_command(cmd, 0);

            panel_Header_addr((u32)ptr2);
            panel_init();

          extern int Display_init(int ISP);
          Display_init(1);
          
            free(ptr);
            tcon_initialized = 1;
        }
        else {
            printf("Error: %s, %d\n", __FILE__, __LINE__);
            while(1);
        }
    }
	#ifdef CONFIG_SP_ISP_LOGO
	isp_logo_offset_in_ISPBOOOT = getenv_hex("isp_logo_offset_in_ISPBOOOT", 0); // it would be 0 if partial update
    isp_logo_size_in_ISPBOOOT   = getenv_hex("isp_logo_size_in_ISPBOOOT", 0);   // it would be 0 if partial update
	
	if(isp_logo_offset_in_ISPBOOOT && !isp_logo_initialized) {
        
		ptr2 = CONFIG_SP_ISP_LOGO_ADDR;
		#ifndef CONFIG_SYS_ARM_CACHE_WRITETHROUGH
		flush_dcache_range(CONFIG_SP_ISP_LOGO_ADDR, CONFIG_SP_ISP_LOGO_ADDR+CONFIG_SP_ISP_LOGO_SIZE);	
		#endif
		printf("isp logo offset 0x%x Dram Addr 0x%x\n",isp_logo_offset_in_ISPBOOOT,ptr2);
		sprintf(cmd, "fatload $isp_if $isp_dev 0x%x /ISPBOOOT.BIN 0x%x 0x%x\n", (u32)(ptr2), isp_logo_size_in_ISPBOOOT, isp_logo_offset_in_ISPBOOOT);
		run_command(cmd, 0);

		sprintf(cmd, "md.b 0x%x 0x100\n", (u32)(ptr2));
		run_command(cmd, 0);    
		set_update_logo_addr((u32)ptr2,NULL);	
		//printf("set_isp_logo_addr done\n");
		isp_logo_initialized = 1;
 
    }

	if(isp_version_info_initialized == 0)
	{
		int ret = -1;
		ptr2 = CONFIG_SP_OLD_VERSION_ADDR;
		char mtdparts[1024] = {0};
		if(sp_get_nand_env("mtdparts",mtdparts)>0)
		{
			setenv("mtdparts",mtdparts);
			// Read version_info partition from nand to ram
			sprintf(cmd, "nand read 0x%x %s 0x%x", ptr2, "version_info", CONFIG_SP_OLD_VERSION_SIZE);
			printf("%s\n", cmd);
			ret = run_command(cmd, 0);
		}
		if(ret != 0) //fail.
		{
			strcpy(ptr2,"<Unkown Version>");
		}
		isp_version_info_initialized = 1;
	}

    version_info_offset_in_ISPBOOOT = getenv_hex("version_info_offset_in_ISPBOOOT", 0); // it would be 0 if partial update
    version_info_size_in_ISPBOOOT   = getenv_hex("version_info_size_in_ISPBOOOT", 0);   // it would be 0 if partial update

	if(version_info_offset_in_ISPBOOOT && isp_version_info_initialized < 2)
	{
		int ret = -1;
		ptr2 = CONFIG_SP_NEW_VERSION_ADDR;
		#ifndef CONFIG_SYS_ARM_CACHE_WRITETHROUGH
		flush_dcache_range(CONFIG_SP_NEW_VERSION_ADDR, CONFIG_SP_NEW_VERSION_ADDR+CONFIG_SP_NEW_VERSION_SIZE);	
		#endif
		printf("version info offset 0x%x Dram Addr 0x%x\n",version_info_offset_in_ISPBOOOT,ptr2);
		sprintf(cmd, "fatload $isp_if $isp_dev 0x%x /ISPBOOOT.BIN 0x%x 0x%x\n", (u32)(ptr2), version_info_size_in_ISPBOOOT, version_info_offset_in_ISPBOOOT);
		ret = run_command(cmd, 0);
		if(ret != 0) //failed.
		{
			strcpy(ptr2,"<Unkown Version>");
		}
		isp_version_info_initialized += 2;
	}
	#endif //CONFIG_SP_ISP_LOGO
    printf("\n------------------------------\n");
    printf("%s: total: 0x%x, written: 0x%x, verified: 0x%x\n", __func__, isp_size_total, size_written, size_verified);
    printf("------------------------------\n\n");

	#ifdef CONFIG_SP_ISP_LOGO
	isp_total_time = isp_size_total*2;
	isp_now_time = size_written + size_verified;
	#endif
	
    return 0;
}


static int do_ispsp(cmd_tbl_t *cmdtp, int flag, int argc, char *const argv[])
{
    char cmd[256];
    u32 isp_key_addr = ISP_RAM_ADDR;
    u32 isp_ram_addr = ISP_RAM_ADDR + ((AES_KEY_LENGTH + ARCH_DMA_MINALIGN -1) & (~(ARCH_DMA_MINALIGN -1)));
    u32 i;
    u32 subcmd;
    u32 encryption_enable;
    u8 *u8_ptr;
    u32 isp_all_or_update_done;
	u32 isp_header_offset = 0x00100000;
    if(argc < 2)
        return CMD_RET_USAGE;

    //
    // Forbid other MCU touching DRAM
    //
    IOP_RESET(); // IOP will use HW_BUFFER
    flush_dcache_all();

    if(strcmp(argv[1], "all") == 0)
        subcmd = ISP_CMD_ALL;
    else if(strcmp(argv[1], "update") == 0)
    {
        subcmd = ISP_CMD_UPDATE;
		write_regs(315, 7,0);//turn off 3d combo filter
		#ifdef CONFIG_SP_ISP_LOGO
        printf("ispsp update \n");
        part_logo_initialized = 1;        
		show_update_begin_logo();
		#endif
    }
    else if(strcmp(argv[1], "find_area") == 0)
    {
        subcmd = ISP_CMD_FIND_AREA;
        return do_ispsp_find_area(argc, argv);
    }
#ifdef CHK4BADBLOCK
    else if(strcmp(argv[1], "chk4badblock") == 0) {
        subcmd = ISP_CMD_CHK4BADBLOCK;
        return do_ispsp_chk4badblock(argc, argv);
    }
#endif /* CHK4BADBLOCK */
    else if(strcmp(argv[1], "markbad4bcherr") == 0) {
        subcmd = ISP_CMD_MARKBAD4BCHERR;
        return do_ispsp_markbad4bcherr(argc, argv);
    }
    else if(strcmp(argv[1], "progress") == 0) {
        subcmd = ISP_CMD_PROGRESS;
		#ifdef CONFIG_SP_ISP_LOGO
		if(isp_logo_initialized==1)
		{                                   
			const stTCON_API_t *TCON = Hal_Get_TCON();

			printf("show_logo_begin\n");
			show_update_begin_logo();

			if(isp_version_info_initialized)
			{
				extern void show_logo_version();
				show_logo_version();
			}
			#ifdef ISP_FLOW_STATE_LED_FLASH
			set_logo_begin_LED();
			#endif
			printf("backlight on\n");
			TCON->Set_PWM_Mode_Ctl(0, ENABLE, DISABLE);
			//TCON->SetBackLight_OnOff(ENABLE);
			isp_logo_initialized++;
		}else if(isp_logo_initialized==2)
			show_logo_progress_time(isp_total_time,isp_now_time);

              if(part_logo_initialized==1)
              {
					#ifdef CONFIG_SP_ISP_UPDT_CHECK
                    if(get_part_updt_flag()==1)
                        show_logo_progress_time(isp_total_time,isp_now_time);
					#endif
              }
		#endif
        return do_ispsp_progress(argc, argv);
    }
    else
        return CMD_RET_USAGE;

    /* For isp from SD card to work, the user has to switch to the current in-use
     * SD card slot using command "mmc dev #" (# meaning the SD card slot number)
     */
    if(strcmp(argv[2], "mmc") == 0) {
        snprintf(cmd, sizeof(cmd), "mmc dev %s", argv[3]);
        printf("%s\n", cmd); run_command(cmd, 0);
    }

    snprintf(cmd, sizeof(cmd), "setenv isp_if %s && setenv isp_dev %s && setenv isp_ram_addr 0x%x", argv[2], argv[3], isp_ram_addr);
    printf("%s\n", cmd); run_command(cmd, 0);

    if(subcmd == ISP_CMD_UPDATE) {
        for(i=0; i<4; i++) {
            snprintf(cmd, sizeof(cmd), "setenv isp_key_addr%u 0x%x", i, (isp_key_addr + 4*i));
            printf("%s\n", cmd); run_command(cmd, 0);
        }
#ifdef CONFIG_SP_ISP_UPDT_CHECK
		if(get_part_updt_flag()==1)
		{
			snprintf(cmd, sizeof(cmd), "setenv isp_update_file_name %s", PACK_PATH);
			printf("%s\n", cmd); run_command(cmd, 0);
			isp_header_offset = part_image_offset;
		}else
#endif
		{
			snprintf(cmd, sizeof(cmd), "setenv isp_update_file_name %s", UPDT_PATH);
			printf("%s\n", cmd); run_command(cmd, 0);
			isp_header_offset = 0x00000000;
		}
    }

    snprintf(cmd, sizeof(cmd), "$isp_if start && fatls $isp_if $isp_dev /\n");
    printf("%s\n", cmd);
    if(run_command(cmd, 0) != 0) {
        snprintf(cmd, sizeof(cmd), "setenv isp_dev %s:0", argv[3]);
        printf("%s\n", cmd); run_command(cmd, 0);

        snprintf(cmd, sizeof(cmd), "fatls $isp_if $isp_dev /\n");
        printf("%s\n", cmd); run_command(cmd, 0);
    }

	snprintf(cmd, sizeof(cmd), "setenv isp_image_header_offset 0x%x",isp_header_offset);
	printf("%s\n", cmd); run_command(cmd, 0);
	
    snprintf(cmd, sizeof(cmd), "fatload $isp_if $isp_dev $isp_ram_addr %s 0x%x $isp_image_header_offset && md.b $isp_ram_addr 0x200",
        (subcmd == ISP_CMD_ALL) ? "/ISPBOOOT.BIN" : "$isp_update_file_name",
        SIZE_INIT_SCRIPT);

    printf("%s\n", cmd); run_command(cmd, 0);

    snprintf(cmd, sizeof(cmd), "setexpr script_addr $isp_ram_addr + 0x20 && setenv script_addr 0x${script_addr}");
    printf("%s\n", cmd); run_command(cmd, 0);

    // 01000020: 47 65 6d 69 6e 69 5f 49 53 50 5f 69 6d 61 67 65    Gemini_ISP_image <--- isp_ram_addr
    // 01000030: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00    ................
    // 01000040: 27 05 19 56 b1 94 9d c2 56 a8 5e dd 00 00 03 0a    '..V....V.^.....
    // 01000050: 00 00 00 00 00 00 00 00 de eb f4 a1 05 02 06 00    ................
    // 01000060: 49 6e 69 74 20 75 70 64 61 74 65 20 73 63 72 69    Init update scri
    // 01000070: 70 74 00 00 00 00 00 00 00 00 00 00 00 00 00 00    pt..............
    // 01000080: 00 00 03 02 00 00 00 00 65 63 68 6f 20 76 65 72    ........echo ver
    // 01000090: 69 66 79 69 6e 67 20 78 62 6f 6f 74 31 20 2e 2e    ifying xboot1 .. <--- Check this area for value >= 0x80
    // 010000a0: 2e 0a 65 63 68 6f 20 6e 61 6e 64 20 72 65 61 64    ..echo nand read
    // 010000b0: 2e 62 62 6c 6b 20 24 69 73 70 5f 72 61 6d 5f 61    .bblk $isp_ram_a
    // 010000c0: 64 64 72 20 78 62 6f 6f 74 31 20 30 78 38 30 30    ddr xboot1 0x800
    encryption_enable = 0;
    u8_ptr = (u8 *)(isp_ram_addr + 0x70);
    if(subcmd == ISP_CMD_UPDATE) {
        for(i=0; i<0x40; i++) {
            if(*u8_ptr >= 0x80) {
                encryption_enable = 1;
                break;
            }
            u8_ptr++;
        }
    }

    if(encryption_enable) {
        // Retrieve the key generated by set_script_key() in build/tools/isp/isp.c
        snprintf(cmd, sizeof(cmd), "md5sum ${isp_ram_addr} 0x20 *${isp_key_addr0} && aes dec ${isp_key_addr0} ${script_addr} ${script_addr} 0x%x && md.b ${isp_ram_addr} 0x200", SIZE_INIT_SCRIPT);
        printf("%s\n", cmd); run_command(cmd, 0);
    }

    if(subcmd == ISP_CMD_ALL) {
#if 0   // test code:
        isp_retry_cnt_rw(0);
        isp_retry_cnt_rw(1);   isp_retry_cnt_rw(0);
        isp_retry_cnt_rw(100); isp_retry_cnt_rw(0);
#endif
#if 0   // test code:
        printf("isp_retry_cnt_rw(0) = %u\n", isp_retry_cnt_rw(0));
        run_command("setenv isp_force_to_abort yes", 0);
        return -1;
#endif

        if(isp_retry_cnt_rw(0) > MAX_ISP_TRY_CNT)
            run_command("setenv isp_force_to_abort yes", 0);
    }

    snprintf(cmd, sizeof(cmd), "source $script_addr");
    printf("%s\n", cmd); run_command(cmd, 0);
    isp_all_or_update_done = (u32)(getenv_hex("isp_all_or_update_done", 0));
    if(isp_all_or_update_done) {
        run_command("setenv isp_all_or_update_done", 0);
        return 0;
    }
    else
        return -1;
}

static char ispsp_help_text[] =
    "sub_cmd ... - ISP utility\n"
    "ispsp all usb|mmc 0|1|2\n"
    "ispsp update usb|mmc 0|1|2\n"
    "ispsp find_area start_addr size\n"
#ifdef CHK4BADBLOCK
    "ispsp chk4badblock off size [repeat_count]\n"
#endif /* CHK4BADBLOCK */
    "ispsp markbad4bcherr [all]\n"
    "\n";

U_BOOT_CMD(
    ispsp, 5, 0, do_ispsp,
    "Sunplus ISP utility", ispsp_help_text
);
