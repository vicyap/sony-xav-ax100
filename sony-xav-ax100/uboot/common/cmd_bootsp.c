#include <common.h>
#include <malloc.h>
#include <command.h>
#include <asm/io.h>
#include <asm/hardware.h>
#include <u-boot/md5.h>
#include <../arch/arm/include/asm/armv7.h>
#include <sp_bootinfo.h>
#include <../arch/arm/cpu/sphe-common/iop.h>
#include <linux/clk.h>
#define UBOOT_TCON
#include "drivers/tcon/hal_SPHE8388_tcon.h"
#undef UBOOT_TCON
#include "reverse/reverse.h"
#include "sp_vendor_data.h"

#define TMP_RAM_ADDR            (CONFIG_SP_HW_BUF_ADDR)

#define FILE_NAME_KERNEL        "uImage"
#define PARTITION_NAME_KERNEL   "kernel"

#define FILE_NAME_ECOS          "ecos.img"
#define PARTITION_NAME_ECOS     "ecos"

#define PARTITION_NAME_IOP      "iop_car"

#ifdef CONFIG_SP_ISP_UPDT_CHECK
typedef struct _pack_file_header
{
	UINT8 name[24];
	UINT32 size;
	UINT32 offset;
}pack_file_header;

typedef struct __attribute__((packed)) _bin_file_header
{
	UINT8 token[6];
	UINT32 total_size;
	UINT16 bin_header_len_k;
	UINT32 pack_header_offset;
	UINT32 pack_header_len;
	UINT8 reserved [44];
	UINT8 cust_ver[64];	
	UINT8 sdk_ver[64];	
}bin_file_header;

typedef struct _user_up_ven {
    char user_up_info;
    char lan_name[24];
	char user_up_fail;
    char reserved[6];
}user_up_ven;
#endif

struct source_info {
    int media;
    void *src;      // used by MEDIA_NOR
    int what_to_load;
    int verify;     // 0: no verify, 1: verify checksum
};

enum {
	RUN_ALL = 0,
	RUN_LINUX = 1,
	RUN_ECOS = 2,
	RUN_ISP = 3,
	RUN_NONE = 4,
};

enum {
	MEDIA_NOR = 0,
	MEDIA_NAND = 1,
	MEDIA_USB    = 2,
	MEDIA_SDCARD = 3,
};

static int g_cpu1_loaded = 0;
static struct image_header g_uhdr;

extern void v7_outer_cache_disable(void);

extern int DRV_IOP_InitByClk(unsigned short wSystemClock, unsigned int iop_buffer_addr);

static int user_up_flag = 0;
extern int isp_version_info_initialized;

#ifdef CONFIG_SP_ISP_UPDT_CHECK
static UINT8 part_flag = 0;
static user_up_ven userup;
static UINT8 part_update_read_fail=0;

void set_part_updt_flag(UINT8 flag)
{
    part_flag =flag;
}

UINT8 get_part_updt_flag(void)
{
    return part_flag;
}

static char* basename(char *path)
{
	char *fname;

	fname = path + strlen(path) - 1;
	while (fname >= path) {
			if (*fname == '/') {
					fname++;
					break;
			}
			fname--;
	}
	return fname;
}

static void parsing_version_pack(char *device,UINT8 device_num)
{
	bin_file_header* bin_head;
	pack_file_header* pack_head;
	UINT32 count;
	UINT32 offset;
	int i ,ret;
	char cmd[255];
	
	snprintf(cmd, sizeof(cmd), "%s start",device);
    printf("%s\n", cmd); 
    ret = run_command(cmd, 0);
	
	bin_head = (bin_file_header *)malloc(sizeof(bin_file_header));
	snprintf(cmd, sizeof(cmd), "fatload %s %d 0x%x %s %x",device,device_num,bin_head,PACK_PATH,sizeof(bin_file_header));
    printf("%s\n", cmd); 
    ret = run_command(cmd, 0);	
	
	count = bin_head->pack_header_len/sizeof(pack_file_header);

	pack_head = (pack_file_header *)malloc(sizeof(pack_file_header)*(count));
	snprintf(cmd, sizeof(cmd), "fatload %s %d 0x%x %s %x %x",device,device_num,pack_head,PACK_PATH,bin_head->pack_header_len,sizeof(bin_file_header));
    printf("%s\n", cmd); 
    ret = run_command(cmd, 0);
	
	snprintf(cmd, sizeof(cmd), "md %x",pack_head);
    printf("%s\n", cmd); 
    ret = run_command(cmd, 0);

	for(i=0;i<count;i++)
	{		
		if(strncmp(pack_head[i].name,basename(UPDT_PATH),strlen(basename(UPDT_PATH)))==0)
		{			
			printf("compare success\n");
			offset = pack_head[i].offset;
		}
		
	}
	printf("offset %x\n",offset);
	set_part_image_offset(offset);
	//snprintf(cmd, sizeof(cmd), "setenv isp_image_header_offset %x",	offset);
	//printf("%s\n", cmd); run_command(cmd, 0);
}

static void user_update_autodetect(char *device,UINT8 *device_num)
{
    char cmd[255];
    int ret;
    UINT8 *fat_temp;
    UINT16 size=0x400;
    fat_temp = malloc(size);
    strcpy(device,"non");
    
    snprintf(cmd, sizeof(cmd), "usb start");
    printf("%s\n", cmd); 
    ret = run_command(cmd, 0);
    
    snprintf(cmd, sizeof(cmd), "fatload usb 0 0x%x %s %x",fat_temp,PACK_PATH,size);
    printf("%s\n", cmd); 
    ret = run_command(cmd, 0);
    //printf("ret %d\n",ret);
    if(ret==0)
    {
        strcpy(device,"usb");
        *device_num = 0;
		part_update_read_fail=0;
        goto end;
    }else
	{		
		part_update_read_fail=1;
    }

    snprintf(cmd, sizeof(cmd), "fatload usb 1 0x%x %s %x",fat_temp,PACK_PATH,size);
    printf("%s\n", cmd); 
    ret = run_command(cmd, 0);
    //printf("ret %d\n",ret);
    if(ret==0)
    {
        strcpy(device,"usb");
        *device_num = 1;
		part_update_read_fail=0;
        goto end;
    }else
	{
		part_update_read_fail=1;
    }

    snprintf(cmd, sizeof(cmd), "mmc dev 0");
    printf("%s\n", cmd); run_command(cmd, 0);
    ret = run_command(cmd, 0);
    
    snprintf(cmd, sizeof(cmd), "mmc start");
    printf("%s\n", cmd); 
    ret = run_command(cmd, 0);
    
    snprintf(cmd, sizeof(cmd), "fatload mmc 0 0x%x %s %x",fat_temp,PACK_PATH,size);
    printf("%s\n", cmd); 
    ret = run_command(cmd, 0);
    //printf("ret %d\n",ret);
    if(ret==0)
    {
        strcpy(device,"mmc");
        *device_num = 0;
		part_update_read_fail=0;
        goto end;
    }else
	{
		part_update_read_fail=1;
    }

    snprintf(cmd, sizeof(cmd), "mmc dev 1");
    printf("%s\n", cmd); run_command(cmd, 0);
    ret = run_command(cmd, 0);
    
    snprintf(cmd, sizeof(cmd), "mmc start");
    printf("%s\n", cmd); 
    ret = run_command(cmd, 0);
    
    snprintf(cmd, sizeof(cmd), "fatload mmc 1 0x%x %s %x",fat_temp,PACK_PATH,size);
    printf("%s\n", cmd); 
    ret = run_command(cmd, 0);
    //printf("ret %d\n",ret);
    if(ret==0)
    {
        strcpy(device,"mmc");
        *device_num = 1;
		part_update_read_fail=0;
        goto end;
    }else
	{
		part_update_read_fail=1;
    }
end:
    free(fat_temp);
    BUILD_BUG_ON(sizeof(UPDT_PATH) > REMAIN_STR_LEN);
	BUILD_BUG_ON(sizeof(PACK_PATH) > REMAIN_STR_LEN);
	if(part_update_read_fail==1)
	{
		printf("user update read file failed\n");
		show_update_failed_logo();		
		show_read_failed_string();
		mdelay(5000);
		snprintf(cmd, sizeof(cmd), "reset");
		printf("%s\n", cmd); run_command(cmd, 0);
	}

}

static void check_user_update()
{
    if (sp_vd_get_string( SP_VDATA_LANG_POS, SP_VDATA_LANG_LEN, &userup) ==SP_VD_RET_FAILED ){
        userup.user_up_info = 0;
    }
    printf("+++++++++++++user_up_info [0] 0x%x\n",userup.user_up_info);
    if((userup.user_up_info >>6)&0x1)
    {        
		set_part_updt_flag(1);
    }else
    {        
		set_part_updt_flag(0);
    }
}

static void user_update()
{
    char cmd[256];
    char device[4];
    UINT8 device_num;
    int ret =-1;
	#ifdef CONFIG_SP_ISP_LOGO    
	set_lang_name(&userup.lan_name);     
	#endif
	if(((userup.user_up_info>>4)&0x3)==0x3)
	{
		user_update_autodetect(&device,&device_num);
		parsing_version_pack(device,device_num);
        snprintf(cmd, sizeof(cmd), "ispsp update %s %d",device,device_num);
        printf("%s\n", cmd); 
     }else
     {	
		parsing_version_pack(((userup.user_up_info>>4)&0x3)?"mmc":"usb",userup.user_up_info&0xf);
        snprintf(cmd, sizeof(cmd), "ispsp update %s %d",((userup.user_up_info>>4)&0x3)?"mmc":"usb",userup.user_up_info&0xf);
        printf("%s\n", cmd); 
     }
	userup.user_up_fail++;
	sp_vd_write_raw(SP_VDATA_LANG_POS,SP_VDATA_LANG_LEN,&userup);
	if(userup.user_up_fail>UPDATE_MAX_TIME)
	{
		userup.user_up_fail = 0;	
		sp_vd_write_raw(SP_VDATA_LANG_POS,SP_VDATA_LANG_LEN,&userup);
		show_update_retry_logo();
		parse_file_outer();
	}
    ret = run_command(cmd, 0);
    if(ret == 0)
    {
		userup.user_up_info = 0;
		userup.user_up_fail = 0;	
		sp_vd_write_flush(SP_VDATA_LANG_POS,SP_VDATA_LANG_LEN,&userup);
		#ifdef CONFIG_SP_ISP_LOGO
		show_update_ok_logo();		
		#endif
    }
    else 
    {        
		#ifdef CONFIG_SP_ISP_LOGO
		show_update_failed_logo();		
		#endif
    }
    
     
     //snprintf(cmd, sizeof(cmd), "nand read 0x%x %s 0x%x",&userup,"vendordata",SP_VDATA_LANG_LEN);
     //printf("%s\n", cmd); run_command(cmd, 0);
     
     //printf("clean %x \n",userup.user_up_info);
     
     mdelay(3000);
     snprintf(cmd, sizeof(cmd), "reset");
     printf("%s\n", cmd); run_command(cmd, 0);
    
}
#endif /* CONFIG_SP_ISP_UPDT_CHECK */

void car_iop_start(void)
{
	printf("car iop start..\n");
	printf("iop_buffer_addr = 0x%08x\n", CONFIG_SP_IOP_ADDR);
	/* load iop image */
	char cmd[256];
	sprintf(cmd, "nand read 0x%x %s 0x%x", CONFIG_SP_IOP_ADDR, PARTITION_NAME_IOP, IOP_CODE_SIZE);
	printf("%s\n", cmd); run_command(cmd, 0);

	/* fixed me when system clock in changed */
	u32 clk;
	clk = (u32)clk_get_rate(clk_get_sys(NULL, "SYS"));
	clk = clk/1000000;
	//printf("system clock = %d MHz\n", clk);
	DRV_IOP_InitByClk(clk, CONFIG_SP_IOP_ADDR);
}

void sp_init_before_backlight_on(void)
{
	UINT32 level = 0;

	//VCOM GPIO 39 ctrl for vendordata
	if (sp_vd_get_int(256+40, 8, (int*)(&level)) != SP_VD_RET_FAILED)
	{
		if ((level >= 0) && (level <= 255))
		{
			const stTCON_API_t *TCON = Hal_Get_TCON();

			TCON->Set_PWM_Duty(eTCON_PWM4, level, 0);
			TCON->Set_PWM_Mode_Ctl(eTCON_PWM4, ENABLE, DISABLE);
			panel_set_gpio(0, 39, 0);
		}
	}
}

void sp_init_before_boot(void)
{
	const stTCON_API_t *TCON = Hal_Get_TCON();
	struct sp_bootinfo *bootinfo = SP_GET_BOOTINFO();

	panel_init();

#if !defined(CONFIG_PLATFORM_letb123)
	if (!SP_IS_ISPBOOT()) { // avoid iop from using dram
		car_iop_start();
	}
#endif

#ifdef CONFIG_SP_ISP_UPDT_CHECK
	if (!SP_IS_ISPBOOT())
	{		
        check_user_update();
	}
#endif
	if (!SP_IS_ISPBOOT())
	{
		if(get_part_updt_flag())
		{			
			Display_init(1);
		}else{
			Display_init(0);
		}
		show_normal_logo(0);		
	}
	else
		Display_init(1);

	//panel vendordata function
	sp_init_before_backlight_on();

#ifdef CONFIG_SP_ISP_UPDT_CHECK
       if((get_part_updt_flag())&&(!SP_IS_ISPBOOT()))
	   {			
			if (TCON)
				TCON->SetBackLight_OnOff(ENABLE);
            user_update();
	   }
#endif
	
	if (!SP_IS_ISPBOOT())	//video in init
	{
		UINT32 cvbs_sel = 0;
		char cmd[256];

		if (sp_config_get_int("CVBS_Sel", &cvbs_sel))
			cvbs_sel = 0;

		sprintf(cmd, "nand read 0x%x %s 0x%x", CONFIG_SP_VI_ADDR, "vi", CONFIG_SP_VI_SIZE);

		if (run_command(cmd, 0))
			return 0;

		//default CVBS sel
		fTVD_CheSel((UINT8)cvbs_sel);
		dTVD_3D_CombFilter_MemAddr(CONFIG_SP_VIDEO_DECODER_ADDR);
		fTVD_init();
	}
	if (TCON)
		TCON->SetBackLight_OnOff(ENABLE);

}

/**
 * Load image from NOR/NAND/USB/SDCARD
 * @img_name     - image name in header
 * @dst          - destination address. If NULL, use the load address in header.
 * @src_info_ptr - source info
 *
 * return image data size (exclude header)
 */
static int load_uhdr_image(struct image_header *hdr, const char *img_name, void *dst, struct source_info *src_info_ptr)
{
    int len;
    char name[32];
    char cmd[256];
    void *mem_dst;
    ulong t1;

    mem_dst = (dst != NULL) ? dst : ((void *)(TMP_RAM_ADDR));

    switch(src_info_ptr->what_to_load) {
        case RUN_ECOS:
            sprintf(name, "%s", (src_info_ptr->media == MEDIA_NAND) ? PARTITION_NAME_ECOS : FILE_NAME_ECOS);
            break;

        case RUN_LINUX:
            sprintf(name, "%s", (src_info_ptr->media == MEDIA_NAND) ? PARTITION_NAME_KERNEL : FILE_NAME_KERNEL);
            break;

        default:
            memset(name, 0, sizeof(name));
            break;
    }

    // To measure loading time of header (page)
    //t1 = get_timer(0);

    switch(src_info_ptr->media) {
        case MEDIA_NOR:
            printf("Load %s from NOR@0x%x\n", img_name, (uint32_t)(src_info_ptr->src));
            memcpy(hdr, src_info_ptr->src, sizeof(*hdr)); // 64
            break;

        case MEDIA_NAND:
            sprintf(cmd, "nand read 0x%x %s 0x%x", (u32)(mem_dst), name, sizeof(struct image_header));
            printf("%s\n", cmd); run_command(cmd, 0);
            memcpy(hdr, mem_dst, sizeof(*hdr)); // 64
            break;

        case MEDIA_USB:
        case MEDIA_SDCARD:
            sprintf(cmd, "%s start", (src_info_ptr->media == MEDIA_USB) ? "usb" : "mmc");
            printf("%s\n", cmd); run_command(cmd, 0);
            sprintf(cmd, "fatload %s 0 0x%x /%s 0x%x", (src_info_ptr->media == MEDIA_USB) ? "usb" : "mmc", (u32)(mem_dst), name, sizeof(struct image_header));
            printf("%s\n", cmd); run_command(cmd, 0);
            memcpy(hdr, mem_dst, sizeof(*hdr)); // 64
            break;

        default:
            break;
    }

    //t1 = get_timer(t1); printf("load hdr %lu ms\n", t1); t1 = get_timer(0);

    // magic check
    if (!image_check_magic(hdr)) {
        printf("bad magic\n");
        return -1;
    }
    // header crc
    if (!image_check_hcrc(hdr)) {
            printf("bad hcrc\n");
            return -1;
    }

    // get load address
    if (!dst) {
        dst = (void *)image_get_load(hdr);
        debug("hdr load addr=%p\n", dst);
    }

    // dst = hdr + data ...
    memcpy(dst, hdr, sizeof(*hdr));

    len = image_get_size(hdr);
    printf("load data size=%u to 0x%p\n", len, dst);

    //
    // Time - begin loading
    //
    t1 = get_timer(0); // to measure load time excluding header

    // load image data
    switch(src_info_ptr->media) {
        case MEDIA_NOR:
            memcpy(dst + sizeof(*hdr), src_info_ptr->src + sizeof(*hdr), len);
            break;

        case MEDIA_NAND:
            sprintf(cmd, "nand read 0x%x %s 0x%x", (u32)(dst), name, 64 + len);
            printf("%s\n", cmd);
            t1 = get_timer(0); // exclude printf time
            run_command(cmd, 0);
            break;

        case MEDIA_USB:
        case MEDIA_SDCARD:
            sprintf(cmd, "fatload %s 0 0x%x /%s", (src_info_ptr->media == MEDIA_USB) ? "usb" : "mmc", (u32)(dst), name);
            printf("%s\n", cmd); run_command(cmd, 0);
            break;

        default:
            break;
    }

    //
    // Time - end loading
    //
    t1 = get_timer(t1);
    printf("%d bytes read in %lu ms", 64+len, t1);
    if (t1 > 0) {
        puts(" (");
        print_size((64 + len) * 1000ULL / t1, "/s");
        puts(")");
    }
    puts("\n");

    // Verify checksum of image data?
    if (!src_info_ptr->verify) {
        return len;
    }

    //
    // Time - begin verify
    //
    t1 = get_timer(0);

    if (!image_check_dcrc(dst)) {
        printf("corrupted image\n");
        return -1;
    }

    //
    // Time - end verify
    //
    t1 = get_timer(t1);
    printf("chksum takes %lu ms\n", t1);

    return len;
}

#ifdef CONFIG_UNCOMPRESS_ECOS_IMG
/*
 * Load ecos image and decompress it
 */
static int load_ecos_decompr(struct source_info *source)
{
	void *temp = (void *)CONFIG_SYS_LOAD_ADDR; // borrow Linux address to load image
	void *eload;
	size_t dstlen;
	int comp, res, len;
	ulong ts1;

	/*
	 * Source:       temp  (uhdr + uncompr. img)
	 * Destination : eload (uhdr + decompr. img)
	 */
	len = load_uhdr_image(&g_uhdr, "eCos", temp, source);
	if (len > 0) {
		eload = (void *)image_get_load(&g_uhdr);
		memcpy(eload, &g_uhdr, 64);
		dstlen = CONFIG_SP_ECOS_END_ADDR - (u32)eload - 64;
		comp = image_get_comp(&g_uhdr);

		ts1 = get_timer(0);
		if (comp == IH_COMP_NONE) {
			// none (copy to destination)
			sp_cbdma_memcpy(temp + 64, eload + 64, len);
#ifdef CONFIG_GZIP
		} else if (comp == IH_COMP_GZIP) {
			// gunzip (dst, src)
			res = gunzip(eload + 64, dstlen,
				(unsigned char *)temp + 64, (unsigned long *)&len);
			if (res){
				printf("GUNZIP: err=%d\n", res);
				return -1;
			}
#endif
#ifdef CONFIG_LZO
		} else if (comp == IH_COMP_LZO) {
			// lzop_decompress (src, dst)
			res = lzop_decompress((unsigned char *)temp + 64, len,
				eload + 64, &dstlen);
			if (res) {
				printf("LZO: err=%d\n", res);
				return -1;
			}
			len = dstlen;
#endif
#ifdef CONFIG_LZ4
		} else if (comp == IH_COMP_LZ4) {
			// ulz4fn (src, dst)
			res = ulz4fn(temp + 64, len, eload + 64, &dstlen);
			if (res) {
				printf("LZ4: err=%d\n", res);
				return -1;
			}
			len = dstlen;
#endif
		} else { // Not support
			printf("not support comp type=%d\n", comp);
			return -1;
		}

		ts1 = get_timer(ts1);
		printf("decompr[%d] takes %lu ms, sz=%d\n", comp, ts1, len);
	}

	return len;
}
#endif

// return image size > 0 if ok
static int load_ecos(struct source_info *source)
{
	int len;

	if (g_cpu1_loaded) {
		printf("cpu1 has been loaded!\n");
		return -1;
	}

#ifndef CONFIG_UNCOMPRESS_ECOS_IMG
	len = load_uhdr_image(&g_uhdr, "eCos", NULL, source);
	if (len > 0 && image_get_comp(&g_uhdr) != IH_COMP_NONE) {
		puts("warn: compressed ecos\n");
	}
#else
	len = load_ecos_decompr(source);
#endif

	return len;
}

static int run_ecos(int image_sz)
{
	u32 addr = image_get_ep(&g_uhdr);
	const char *name = image_get_name(&g_uhdr);
	const char *expect = "ecos_"CONFIG_SYS_BOARD; //  "ecos_<BOARD_NAME>"

	// Check Image Name -  To avoid mismatched memory layout
	if (strcmp(name, expect)) {
		// please build ecos with correct config
		printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
		printf("!! Wrong Image: %s\n", name);
		printf("!! Expected:    %s\n", expect);
		printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
		mdelay(5000);
	}

	printf("Run %s @0x%x (cpu1)\n", name, addr);

	// flush ecos-side dram only :
	// flush_dcache_range(addr, image_sz);
	// invalidate_dcache_range(0, addr);

	// flush all in case that modules have shared but unclean dram ranges
	flush_dcache_all(); // clean & invalidate < 1ms

	*(volatile u32 *)CPU1_START_POS = addr;
	g_cpu1_loaded = 1;

	debug("cpu0 conti\n");

	return 0;
}

// return image size > 0 if ok
static int load_linux(struct source_info *source)
{
	int ret = 0;
	ret = load_uhdr_image(&g_uhdr, "Linux", (void *)CONFIG_SYS_LOAD_ADDR, source);
	return ret;
}

static void sp_compose_bootargs(void)
{
        char *cmdline = getenv("bootargs");
        char *cmd_mtdparts = getenv("mtdparts");
        char *newcmd;
        int len, len_mtdparts;

        if(cmd_mtdparts)
            len_mtdparts = strlen(cmd_mtdparts) + 2;
        else {
            len_mtdparts = 1;
            cmd_mtdparts = "";
        }

        len = strlen(cmdline) + len_mtdparts + strlen(CONFIG_SP_CMDARGS) + 128; // %x expands to 8 bytes

        /*
         *  COMMAND_LINE_SIZE defined in U-Boot and kernel must be the same (checked by "make consistent_check").
         *  * uboot/arch/arm/include/asm/setup.h and
         *  * kernel/arch/arm/include/asm/setup.h
         */
#if (COMMAND_LINE_SIZE > 4096)
#error COMMAND_LINE_SIZE can not be larger than 1 page size. (refer to kernel code)
#endif

        if(len > COMMAND_LINE_SIZE) {
                printf("%s, %d: command line too long (%d > %d). \n", __FILE__, __LINE__, len, COMMAND_LINE_SIZE);
                while(1);
        }else if(len > (COMMAND_LINE_SIZE - 50))
                printf("\n\n\n\nWarning: command line buffer close to full (%d / %d). \n\n\n\n", len, COMMAND_LINE_SIZE);

        newcmd = malloc(len);
        if (!newcmd) {
                printf("error: malloc newcmd failed, len=%u\n", len);
                return;
        }

        sprintf(newcmd, "%s %s " CONFIG_SP_CMDARGS, cmdline, cmd_mtdparts,
                CONFIG_SP_ECOS_END_ADDR,
                CONFIG_SP_IOP_ADDR, CONFIG_SP_IOP_SIZE,
                CONFIG_SP_DISPLAY_ADDR, CONFIG_SP_DISPLAY_SIZE,
                CONFIG_SP_RTCFG_ADDR, CONFIG_SP_RTCFG_SIZE,
                CONFIG_SP_LINUX_SIZE,
                CONFIG_SP_CHUNKMEM_ADDR, CONFIG_SP_CHUNKMEM_SIZE);
        setenv("bootargs", newcmd);

        free(newcmd);

        debug("bootargs=%s\n", getenv("bootargs"));
}

static void run_linux(void)
{
	char cmd[128];
	u32 addr = (CONFIG_SYS_LOAD_ADDR + sizeof(struct image_header));

	// compose linux cmdline
	sp_compose_bootargs();

	printf("Run linux @0x%x\n", addr);

	dcache_disable(); // implies flush_dcache_all()
	v7_outer_cache_disable();
	invalidate_dcache_all();

	// use bootz so that atag is passed
	sprintf(cmd, "bootz 0x%08x", addr);

	run_command(cmd, 0);
}


/*
 * do_bootsp os media verify
 */
static int do_bootsp(cmd_tbl_t *cmdtp, int flag, int argc, char *const argv[])
{
	const char *arg;
	int os = RUN_ALL;           // all | linux | ecos
	int media = MEDIA_NOR;      // nor | nand | usb | sdcard
	int verify = 0;             // verify
	int len;
	int devport = 0;
	struct source_info source;
	int ret = -1;
	struct sp_bootinfo *bootinfo = SP_GET_BOOTINFO();

#ifdef CONFIG_SP_BOOT_STRICT_CHECK
	verify = 1; // verify checksum but slow-down boot speed
#endif

	if (argc == 1) {
		//
		// boot info -> boot options
		//

		os = RUN_ALL;

		printf("bootmode=0x%x \n", bootinfo->gbootRom_boot_mode);

		switch (bootinfo->gbootRom_boot_mode) {
		default: /* exception */
			printf("found bad bootmode\n");
		case NAND_LARGE_BOOT:
		case SPI_NAND_BOOT:
			media = MEDIA_NAND;
			break;
		case SPI_BOOT:
			media = MEDIA_NOR;
			verify = 1; //don't trust romter
			break;
		case USB_ISP:
			media = MEDIA_USB;
			os = RUN_ISP;
#ifdef CONFIG_USB_PORT_FASTEN
			if (bootinfo->bootdev == DEVICE_USB1_ISP) {
				devport = 1; // USB0 -> dev=0, USB1 -> dev=1
			}
#endif
			break;
		case SDCARD_ISP:
			media = MEDIA_SDCARD;
			if (bootinfo->bootdev == DEVICE_SD1) {
				//devport = 1; // always 0 in u-boot
			}
			os = RUN_ISP;
			break;
		}
	}

	if (argc > 1) {
		arg = argv[1];
		if (strcmp(arg, "all") == 0) {
			os = RUN_ALL;
		} else if (strcmp(arg, "linux") == 0) {
			os = RUN_LINUX;
		} else if (strcmp(arg, "ecos") == 0) {
			os = RUN_ECOS;
		} else if (strcmp(arg, "isp") == 0) {
			os = RUN_ISP;
		} else if (strcmp(arg, "none") == 0) {
			os = RUN_NONE;
		} else
			return CMD_RET_USAGE;
	}

	if (argc > 2) {
		arg = argv[2];
		if (strcmp(arg, "nor") == 0) {
			media = MEDIA_NOR;
		} else if (strcmp(arg, "nand") == 0) {
			media = MEDIA_NAND;
		} else if (strcmp(arg, "usb") == 0) {
			media = MEDIA_USB;
		} else if (strcmp(arg, "sdcard") == 0) {
			media = MEDIA_SDCARD;
		} else
			return CMD_RET_USAGE;
	}

	if (argc > 3 && strcmp(argv[3], "verify") == 0) {
		verify = 1;
	}

	/* debug only: pinmux to SPI_NOR */
	if (media == MEDIA_NOR) {
		void SetBootDev(unsigned int bootdev, unsigned int pin_x);
		SetBootDev(DEVICE_SPI_NOR, 1 + (bootinfo->hw_bootmode == SPI_NOR_X2_BOOT));
	}

	/* Boot to ISP ? */
	if (os == RUN_ISP) {
		char cmd[128];
		if (media == MEDIA_USB) {
			sprintf(cmd, "ispsp all usb %d", devport);
		} else {
			sprintf(cmd, "ispsp all mmc %d", devport);
		}
		printf("%s\n", cmd);
		ret = run_command(cmd, 0);
		
		#ifdef CONFIG_SP_ISP_LOGO
		if(ret == 0)
		{			
			show_update_ok_logo();
			if(isp_version_info_initialized)
			{
				extern void show_logo_version();
				show_logo_version();
			}
			#ifdef ISP_FLOW_STATE_LED_FLASH
			set_logo_ok_LED();
			#endif
		}
		else 
		{			
			show_update_failed_logo();

			#ifdef ISP_FLOW_STATE_LED_FLASH
			set_logo_failed_LED();
			#endif

			/* force to program NAND again by reset in case of "ispsp all" fails */
			mdelay(1000);
			run_command("ispsp markbad4bcherr", 0);
			if(getenv("isp_force_to_abort") == NULL)
				run_command("reset", 0);
		}
		#endif
		
		return 0;
	}
	
	reverse_drawline();

	/*
	 * Phase 1: eCos
	 */
	if (os == RUN_ALL || os == RUN_ECOS) {
		source.media         = media;
		source.src           = (void *)(SPHE_SPI_FLASH_BASE + CONFIG_SPI_ECOS_OFFSET);
		source.what_to_load  = RUN_ECOS;
		source.verify        = verify;

		len = load_ecos(&source);
		if (len > 0)
			run_ecos(len);
	}

	/*
	 * Phase 2: Linux
	 */
	if (os == RUN_ALL || os == RUN_LINUX) {
		source.media         = media;
		source.src           = (void *)(SPHE_SPI_FLASH_BASE + CONFIG_SPI_KERNEL_OFFSET);
		source.what_to_load  = RUN_LINUX;
		source.verify        = verify;

		len = load_linux(&source);
		if (len > 0)
			run_linux();
	}

	return 0;
}

U_BOOT_CMD(
	bootsp, 4, 0, do_bootsp,
	"Sunplus boot utility",
	"ecos|linux|all|isp|none nor|nand|usb|sdcard [verify]\n"
);
