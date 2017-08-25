/*
 * (C) Copyright 2015
 * Sunplus Technology. <http://www.sunplus.com/>
 *
 */


#include <common.h>
#include "asm/arch-gemini/regmap_gemini.h"
#include "sp_config.h"
#include <sp_bootinfo.h>
#include "sp_vendor_data.h"

#undef SP_CONFIG_DEBUG


// limitation
#define CONFIG_LINE_MAX             100  //100 + \n
#define CONFIG_MAX_LEN              8192  //total length of config


#define SP_CFG_PART_HEAD            "##runtime.cfg##"
#define SP_CFG_HEAD                 "\n"
#define SP_CFG_END                  "="

#define SP_CFG_SEPARATOR            "\n"
#define SP_CFG_INITIAL_VALUE        (-1)

#define PARTITION_NAME_RTCFG        "runtime_cfg"

#ifdef SP_CONFIG_DEBUG
#define DBG(a) a
#else
#define DBG(a)
#endif

#ifdef SP_CONFIG_DEBUG
static unsigned long long g_tv1 = 0LL, g_tv2 = 0LL;
static ulong g_tm1 = 0L, g_tm2 = 0L;
#define TICK_START      sp_tick(&g_tv1)
#define TICK_STOP       sp_tick(&g_tv2)
#define TICK_ELAPSED_US (unsigned long int)((g_tv2 -g_tv1) * (1000000000/get_tbclk()))

#define TIMER_START     sp_timer(0, &g_tm1)
#define TIMER_STOP      sp_timer(g_tm1, &g_tm2)
#define TIMER_PRINT     printf("timer: %u milli-second\n", g_tm2)
#else
#define TIMER_START
#define TIMER_STOP
#define TIMER_ELAPSED_US
#endif

//pinmux table
//config name, cmdline name, No of bit
struct sp_cfg_pm pm_sft_cfg0[] =
{
    {"\0", (-1), 0, 0}       //last
};

struct sp_cfg_pm pm_sft_cfg1[] =
{
    {"pmSPI_FLASH",     0, 2, 1}, //sft_cfg1[1:0] X1, SPI_CE/SPI_D/SPI_CLK
    {"pmNAND_FLASH_HE", 4, 1, 1}, //sft_cfg1[4] X1, NAND_RDY/NAND_RE_B/NAND_CE0_B/NAND_CLE/NAND_ALE/NAND_WE_B/NAND_WP_B/NAND_D7/NAND6/NAND_D5/NAND_D4/NAND_D3/NAND_D2/NAND_D1/NAND_D0
    {"pmGPS_IN0",       7, 2, 1}, //sft_cfg1[8:7] X1, GPS_SGN/GPS_MAG
    {"pmGPS_IN1",       9, 2, 0}, //sft_cfg1[10:9] X1, BD_SGN/BD_MAG, conflict with UA3
    {"pmGPS_IN2",      11, 1, 1}, //sft_cfg1[11] X1
    {"pmUA0_TX",       12, 3, 1}, //sft_cfg1[14:12] X1
    {"pmUA0_RX",       15, 3, 1}, //sft_cfg1[17:15] X1
    {"pmUA5_TX",       18, 3, 1}, //sft_cfg1[20:18] X1
    {"pmUA5_RX",       21, 3, 1}, //sft_cfg1[23:21] X1
    {"pmUA5_CTS",      24, 3, 1}, //sft_cfg1[26:24] X1
    {"pmUA5_RTS",      27, 3, 1}, //sft_cfg1[29:27] X1
    {"\0", (-1),  0, 0}      //last
};

struct sp_cfg_pm pm_sft_cfg2[] =
{
    {"pmUA2_TX",   0, 3, 1}, //sft_cfg2[2:0] X1
    {"pmUA2_RX",   3, 3, 1}, //sft_cfg2[5:3] X1
    {"pmUA4_TX",  12, 2, 1}, //sft_cfg2[13:12] X1
    {"pmUA4_RX",  14, 2, 1}, //sft_cfg2[15:14] X1
    {"pmUA4_CTS", 16, 2, 1}, //sft_cfg2[17:16] X1
    {"pmUA4_RTS", 18, 2, 1}, //sft_cfg2[19:18] X1
    {"pmUA1_TX",  20, 3, 1}, //sft_cfg2[22:20] X1
    {"pmUA1_RX",  23, 3, 1}, //sft_cfg2[25:23] X1
    {"pmI2CM0",   26, 3, 1}, //sft_cfg2[28:26] X1, SDA0/SCL0
    {"pmI2CM1",   29, 3, 1}, //sft_cfg2[31:29] X1, SDA1/SCL1
    {"\0", (-1), 0, 0}       //last
};

struct sp_cfg_pm pm_sft_cfg3[] =
{
    {"\0", (-1), 0, 0}       //last
};

struct sp_cfg_pm pm_sft_cfg4[] =
{
    {"pmSDIO_CTL_SD",  9, 2, 1}, //sft_cfg4[10:9] X1, SDIO_D0/SDIO_CLK/SDIO_CMD, CARD4
    {"pmSDIO_CTL_HB", 11, 2, 1}, //sft_cfg4[12:11] X1, SDIO_D1/SDIO_D2/SDIO_D3, CARD4
    {"pmCARD0_SD",  13, 2, 1}, //sft_cfg4[14:13] X1, SD0_D0/SD0_CLK/SD0_CMD, CARD0
    {"pmCARD0_HB",  15, 2, 1}, //sft_cfg4[16:15] X1, SD0_D1/SD0_D2/SD0_D3, CARD0
    {"pmCARD1_SD",  23, 2, 1}, //sft_cfg4[24:23] X1, SD1_D0/SD1_CLK/SD1_CMD, CARD1
    {"pmCARD1_SD_HB",  25, 2, 1}, //sft_cfg4[26:25] X1, SD1_D1/SD1_D2/SD1_D3, CARD1
    {"\0", (-1), 0, 0}       //last
};

struct sp_cfg_pm pm_sft_cfg5[] =
{
    {"pmCCIR656IA_CTL",	 4, 3, 1}, //sft_cfg5[6:4] X1 656_D0/656_D1/656_D2/656_D3/656_D4/656_D5/656_D6/656_D7/656_CLK
    {"pmTCON_PWM1",	    12, 3, 1}, //sft_cfg5[14:12] X1
    {"pmTCON_PWM2",    15, 3, 1}, //sft_cfg5[17:15] X1
    {"pmTCON_PWM3",    18, 3, 1}, //sft_cfg5[20:18] X1
    {"pmTCON_PWM4",    21, 3, 1}, //sft_cfg5[23:21] X1
    {"pmTCON_PWM5",    24, 3, 4}, //sft_cfg5[26:24] X4
    {"\0", (-1), 0, 0}       //last
};

struct sp_cfg_pm pm_sft_cfg6[] =
{
    {"pmTCON_PCLK",     6, 2, 1}, //sft_cfg6[7:6] X1
    {"pmTCON_8_8_8",   8, 2, 1}, //sft_cfg6[9:8] X1, R0/R1/G0/G1/B0/B1, TTLRGB888
    {"pmTCON_6_6_6",    10, 1, 1}, //sft_cfg6[10]	X1, R2-7/G2-7/B2-7/LVDS
    {"pmTCON_TTL",      11, 2, 1}, //sft_cfg6[12:11] X1, or DE/LVDS_TXAB3 & HS/LVDS_TXA3
    {"pmAUD_BT_IFX",  17, 2, 0}, //sft_cfg6[18:17] X1, BT_PCM is disable because of unsure RDA & 8388 master/slave mode.
    {"pmAUD_EXT_ADC_IFX0",    19, 2, 1}, //sft_cfg6[20:19] X1, I2SI_D0/I2SI_MCK/I2SI_LRCK/I2SI_BCK
    {"pmAUD_EXT_DAC_IFX", 25, 3, 1}, //sft_cfg6[27:25] X1, I2SO_MCLK/I2SO_LCRLK/I2SO_BCK/I2SO_D0
    {"pmAUD_EXT_DAC_IFX1", 28, 3, 1}, //sft_cfg6[30:28] X1
    {"\0", (-1), 0, 0}       //last
};

struct sp_cfg_pm pm_sft_cfg7[] =
{
    {"pmAUD_EXT_DAC_IFX2",  0, 3, 1}, //sft_cfg7[2:0] X1
    {"pmAUD_EXT_DAC_IFX3",  3, 3, 1}, //sft_cfg7[5:3] X1
    {"pmGPIO_INT0",    16, 3, 1}, //sft_cfg7[18:16] X1
    {"pmGPIO_INT1",    19, 2, 1}, //sft_cfg7[20:19] X1
    {"pmGPIO_INT2",    21, 2, 1}, //sft_cfg7[22:21]
    {"pmGPIO_INT3",    23, 2, 1}, //sft_cfg7[24:23] X1
    {"pmGPIO_INT4",    25, 2, 2}, //sft_cfg7[26:25] X2
    {"\0", (-1), 0, 0}      //last
};

struct sp_cfg_pm pm_sft_cfg8[] =
{
    {"pmUSBC0_OTG_EN", 2, 1, 1}, //sft_cfg8[2] X1
    {"pmUSBC1_OTG_EN", 3, 1, 1}, //sft_cfg8[3] X1
    {"\0", (-1), 0, 0}       //last
};

char* sp_sft_cfg_name[SP_CFG_PM_SFT_CFG_MAX] =
{
    "pm_sft_cfg0",
    "pm_sft_cfg1",
    "pm_sft_cfg2",
    "pm_sft_cfg3",
    "pm_sft_cfg4",
    "pm_sft_cfg5",
    "pm_sft_cfg6",
    "pm_sft_cfg7",
    "pm_sft_cfg8",
};

char* sp_sft_rst_name[SP_CFG_PM_SFT_CFG_MAX] =
{
    "pm_sft_rst0",
    "pm_sft_rst1",
    "pm_sft_rst2",
    "pm_sft_rst3",
    "pm_sft_rst4",
    "pm_sft_rst5",
    "pm_sft_rst6",
    "pm_sft_rst7",
    "pm_sft_rst8",
};


typedef enum
{
    GPIO_FIRST_START = 0,
    SD0_SEN = 8,
    SD1_SEN = 15,
    GPIO_FIRST_0 = 31,

    GPIO_FIRST_MAX = 186,
} SP_CFG_GPIO_DEFAULT_EN;

char* gpio_enable_list[] =
{
    "gpioSD0Sen",
    "gpioSD1Sen",
    "\0"
};

//special case for fast reverse
#define MIRROR_VENDOR_DATA_SIZE    2
int mirror_vendor_data_offset[MIRROR_VENDOR_DATA_SIZE] =
{
	SP_VDATA_REVMIRROR_POS,
	SP_VDATA_VINMIRROR_POS
};

int mirror_vendor_data_len[MIRROR_VENDOR_DATA_SIZE] =
{
	SP_VDATA_REVMIRROR_LEN,
	SP_VDATA_VINMIRROR_LEN
};

char* mirror_vendor_data[MIRROR_VENDOR_DATA_SIZE] =
{
    "rev_mirror_vd=",
    "vin_mirror_vd="
};

static void sp_config_insert_mirror(char *content){
    //int total = 1; //insert "\n" first
    //int data_len = strlen(content);
    char input[255];
    int i = 0, value = 0;
    int len = strlen(content);

    memset(input, 0, 255);

    for(i = 0; i < MIRROR_VENDOR_DATA_SIZE; i++){
        if (sp_vd_get_int(mirror_vendor_data_offset[i], mirror_vendor_data_len[i], &value) == SP_VD_RET_SUCCESS)
            sprintf((input + strlen(input)), "%s%d\n", mirror_vendor_data[i], value);
    }
	printf("sp_config_insert_mirror input = %s\n", input);

	//strcat(content, input);
	strcpy((content + len), input);
    len += strlen(input);
	content[len] = '\0';
	printf("%s\n", input);

    /*
	printf("\n\n");
	for(i = 0; i <= len; i++)
		printf("0x%x", content[i]);

	printf("\n\n");
	//printf("sp_config_insert_mirror result: %s\n\n %s\n", input, content);
    */
}


/*
sample config string => ucfg.panel_num=1 ucfg.panel_resol=2 kcfg.display_dual_screen=n
*/
char* g_cfgContent = NULL;

static char* g_atagPreCfg = NULL; /* a copy of atag before config_begin */

void sp_config_pm_extra(void)
{
    int enable = 1;

    if (sp_config_get_int("gpio_LECD_en", &enable) == SP_CFG_RET_SUCCESS){
        //printf("sp_config_pm_extra gpio_LECD_en enable = %d \n", enable);
        if(!enable){
            unsigned int extra = 0x00000000;
            extra |= ((1 << 5) | (1 << 6) | (1 << 27) | (1 << 28));  // 69, 70, 91, 92
            (PAD_CTL_REG)->gpio_first[2] |= extra;
            printf("sp_config_pm_extra close, extra = %d \n", extra);
        }
    }
}


int sp_config_gpio_parse(char* key, int *group, int *bit)
{
    int value = (-1);
    int gpio_max = 186;
    //printf("sp_config_gpio_parse key = %s\n", key);
    if (sp_config_get_int(key, &value) == SP_CFG_RET_SUCCESS)
    {
        if(value > gpio_max) return SP_CFG_RET_FAILED;

        *group = value/32;
        *bit = value%32;
        return SP_CFG_RET_SUCCESS;
    }
    else
    {
        return SP_CFG_RET_FAILED;
    }
}


void sp_config_gpio_init(void)
{
    unsigned int init = 0;
    int group = 0, bit = 0;
    int now = 0;

    //GPIO First #0
    init = (PAD_CTL_REG)->gpio_first[group];
    //printf("sp_config_gpio_init gpio_first#0 before = 0x%x\n", (PAD_CTL_REG)->gpio_first[group]);
    while (strlen(gpio_enable_list[now]) > 0)
    {
        if(sp_config_gpio_parse(gpio_enable_list[now], &group, &bit) == SP_CFG_RET_SUCCESS)
        {
            (PAD_CTL_REG)->gpio_first[group] |= (1 << bit);
        }
        now++;
    }
    printf("sp_config_gpio_init gpio_first#0 before = 0x%x after = 0x%x\n", init, (PAD_CTL_REG)->gpio_first[group]);

}


int sp_config_pminit(void)
{
    int table = 0;
    int init = 0, value = 0;
    int ret = 0;
    int idx= 0;
    int reset = 0;

    for(table = 0; table < SP_CFG_PM_SFT_CFG_MAX; table ++)
    {
        struct sp_cfg_pm *pos = NULL;
        init = value = (MOON1_REG)->sft_cfg[table];

        if(sp_config_get_int(sp_sft_cfg_name[table], &ret) == SP_CFG_RET_SUCCESS
                && sp_config_get_int(sp_sft_rst_name[table], &reset) == SP_CFG_RET_SUCCESS)
        {
            if(ret == 0 && reset == 0xFFFFFFFF) continue;

            value = ((value & reset) | ret);
            (MOON1_REG)->sft_cfg[table] = value;
            DBG(printf("sp_config_init cfg sft_cfg_%d before = 0x%8x, after = 0x%8x\n", table, init, value));
        }
        else
        {
            switch(table)
            {
            case SP_CFG_PM_SFT_CFG_0:
                pos = pm_sft_cfg0;
                break;
            case SP_CFG_PM_SFT_CFG_1:
                pos = pm_sft_cfg1;
                break;
            case SP_CFG_PM_SFT_CFG_2:
                pos = pm_sft_cfg2;
                break;
            case SP_CFG_PM_SFT_CFG_4:
                pos = pm_sft_cfg4;
                break;
            case SP_CFG_PM_SFT_CFG_5:
                pos = pm_sft_cfg5;
                break;
            case SP_CFG_PM_SFT_CFG_6:
                pos = pm_sft_cfg6;
                break;
            case SP_CFG_PM_SFT_CFG_7:
                pos = pm_sft_cfg7;
                break;
            case SP_CFG_PM_SFT_CFG_8:
                pos = pm_sft_cfg8;
                break;
            default:
                continue;
            }


            while(pos != NULL && strlen(pos->name) > 0)
            {
                ret = pos->val;
                idx = 0;
                while(idx < pos->bit_cnt)
                {
                    value &= ~(1<<(pos->bit_no + idx)); //reset the bit
                    idx++;
                }
                value |= (ret<<pos->bit_no);  //or the result
                pos++;
            }
            (MOON1_REG)->sft_cfg[table] = value;

            //printf("sp_config_init def sft_cfg_%d before = 0x%8x, after = 0x%8x\n", table, init, value);
        }
    }

    sp_config_pm_extra();
    return SP_CFG_RET_SUCCESS;
}

/*
sample input = ucfg.panel_num=1 ucfg.panel_resol=2 kcfg.display_dual_screen=n
*/

#ifdef SP_CONFIG_DEBUG
static void sp_tick(unsigned long long* t)   //tick = 10 ^ -7
{
    *t = get_ticks();
}

static void sp_timer(ulong b, ulong* t)   //tick = 10 ^ -7
{
    *t = get_timer(b);
}
#endif

static int _atoi(char *s)
{
    int i = 0;
    int sign = 1;

    while ( *s != NULL &&
            (((*s) >= '0' && (*s) <= '9') || (*s == '-')) )
    {
        if (*s == '-')
        {
            sign = -1;
        }
        else
        {
            i = i * 10 + (*s - '0');
        }
        s++;
    }

    i = sign * i;

    //DBG(printf("i = %d\n"));
    return i;
}

/* speed up config searching by strcfg() instead of strstr() */
#define USE_STRCFG

#ifdef USE_STRCFG
/*
 * sp_config_strcfg - search for cfg "xxxx=yyy" within config data
 *
 * Return address pointing to string "yyy" or NULL if not found
 */
char *sp_config_strcfg(char *cfg)
{
	char *pstr;
	int plen;
	char cond[CONFIG_LINE_MAX] = SP_CFG_HEAD;
	int limit = CONFIG_SP_RTCFG_SIZE; /* limit search space */

	if (g_cfgContent == NULL)
		return NULL;

	strcat(cond, cfg);
	strcat(cond, SP_CFG_END);

	pstr = g_cfgContent;
	plen = strlen(cond);
	for (; limit > 0; pstr++, limit--) {
		while (limit > 0 && *pstr && *pstr != '\n') {
			pstr++;
			limit--;
		}

		if (!*pstr)
			break;

		if (strncmp(pstr, cond, plen) == 0) {
			return pstr + plen;
		}
	}

	return NULL;
}
#endif

int sp_config_get_int(char *cfgStr, int *ret)
{
    char *s = NULL;
    int sign = 0;
    int base = 10;

#ifndef USE_STRCFG
    char cond[CONFIG_LINE_MAX] = SP_CFG_HEAD;

    if (g_cfgContent == NULL || strlen(g_cfgContent) == 0)
    {
        return SP_CFG_RET_FAILED;
    }

    strcat(cond, cfgStr);
    strcat(cond, SP_CFG_END);

    s = strstr(g_cfgContent, cond);
#else
    s = sp_config_strcfg(cfgStr);
#endif
    if (s != NULL)
    {
        //s = strstr(s, SP_CFG_END);
#ifndef USE_STRCFG
        s += strlen(cond);
#endif
        if (s != NULL && s != '\0')
        {
            //++s;
            if (s[0] == '-')
            {
                sign = 1;
                s++;
            }
            if (s[0] == '0' && s[1] == 'x')
            {
                base = 16;
                s += 2;
            }
            if (s[0] == '0' && s[1] == 'o')
            {
                base = 8;
                s += 2;
            }
            if (s[0] == '0' && s[1] == 'b')
            {
                base = 2;
                s += 2;
            }

            *ret = simple_strtol(s, NULL, base);
            if (sign)
                *ret = - *ret;
	    DBG(printf("%s=%d(0x%x)\n", cfgStr, *ret, *ret));
            return SP_CFG_RET_SUCCESS;
        }
    }
    else
    {
	DBG(printf("%s not found\n", cfgStr));
        *ret = (-1);
    }

    return SP_CFG_RET_FAILED;
}

int sp_config_get_string(char *cfgStr, char *ret, int maxSize)
{
    char *s = NULL;
    char cond[CONFIG_LINE_MAX] = SP_CFG_HEAD;

    if (g_cfgContent == NULL || strlen(g_cfgContent) == 0)
    {
        return SP_CFG_RET_FAILED;
    }

    strcat(cond, cfgStr);
    strcat(cond, SP_CFG_END);

    s = strstr(g_cfgContent, cond);
    if (s != NULL)
    {
        char* e = NULL;
        int len;
        //s = strstr(s, SP_CFG_END);
        s += strlen(cond);
        e = strstr(s, SP_CFG_SEPARATOR);
        if (s != NULL && e != NULL)
        {
            len = (e - s - 1);
            len = (len >= maxSize)? (maxSize - 1): len;
            memcpy(ret, (s + 1), len); //use memcpy instead of strcopy for avoiding ret not init
            ret[len] = '\0';
            return len;
        }
    }
    return SP_CFG_RET_FAILED;
}

int sp_config_get_bool(char *cfgStr, bool *ret)
{
    char *s = NULL;
    char cond[CONFIG_LINE_MAX] = SP_CFG_HEAD;

    if (g_cfgContent == NULL || strlen(g_cfgContent) == 0)
    {
        return SP_CFG_RET_FAILED;
    }

    strcat(cond, cfgStr);
    strcat(cond, SP_CFG_END);

    s = strstr(g_cfgContent, cond);
    if (s != NULL)
    {
        //s = strstr(s, SP_CFG_END);
        s += strlen(cond);
        if (s != NULL)
        {
            if (*(s+1) == 'y' || *(s+1) == 'Y')
            {
                *ret = true;
                return SP_CFG_RET_SUCCESS;
            }
            else if (*(s+1) == 'n' || *(s+1) == 'N')
            {
                *ret = false;
                return SP_CFG_RET_SUCCESS;
            }
        }
    }
    return SP_CFG_RET_FAILED;
}

int sp_config_setup(void){
		bool value = false;
    if(sp_config_get_bool("spCRDS", &value) == SP_CFG_RET_SUCCESS){
        if(value){
            (MOON1_REG)->sft_cfg[1] &= ~(7 << 15);
            (MOON1_REG)->sft_cfg[2] &= ~(7 << 9);
            printf("sp_config_seup: 0x%x 0x%x\n", (MOON1_REG)->sft_cfg[1], (MOON1_REG)->sft_cfg[2]);
        }
    }
    
    return SP_CFG_RET_SUCCESS;
}


/*
 * Check if sp_config is valid
 */
static int sp_config_check(void)
{
	if (strncmp(g_cfgContent, SP_CFG_PART_HEAD, strlen(SP_CFG_PART_HEAD)) == 0)
		return SP_CFG_RET_SUCCESS;
	return SP_CFG_RET_FAILED;
}

/*
 * Load config data from storage (NAND)
 */
static int sp_config_load(void)
{
	char cmd[256] = {0};
	int load_sz = CONFIG_MAX_LEN;
	int ret = 0;

	sprintf(cmd, "nand read 0x%x %s 0x%x", CONFIG_SP_RTCFG_ADDR, PARTITION_NAME_RTCFG, load_sz);
	printf("%s\n", cmd);
	ret = run_command(cmd, 0);
	if(ret != 0){
		printf("\n!!!!!!!!!!!!!!!!\n");
		printf("ERROR: runtime config nand read failed, addr = 0x%x, %d\n", CONFIG_SP_RTCFG_ADDR, ret);
		printf("\n!!!!!!!!!!!!!!!!\n");
	}
	
	return ret;
}

int sp_config_init(void)
{
    static bool bIsLoadCfgDone = false;
    struct sp_bootinfo *bootinfo = SP_GET_BOOTINFO();

    if((bootinfo->gbootRom_boot_mode == USB_ISP) || (bootinfo->gbootRom_boot_mode == SDCARD_ISP))
    {
        printf("sp_config_init: isp mode, init later, 0x%x \n", bootinfo->gbootRom_boot_mode);
        return SP_CFG_RET_SUCCESS;
    }
 
    if (bIsLoadCfgDone == true)
    {
        return SP_CFG_RET_SUCCESS;
    }
    bIsLoadCfgDone = true;

    // Read runtime_cfg partition from nand to ram
    if(sp_config_load() != 0){
        return SP_CFG_RET_FAILED;
    }

    // Store a 0x00 at the end of runtime.cfg, as the end of strstr()
    /*
    sprintf(cmd, "mw.b 0x%x 0x00 0x%x", (CONFIG_SP_RTCFG_ADDR + CONFIG_SP_RTCFG_SIZE - 1), 1);
    printf("%s\n", cmd);
    run_command(cmd, 0);
    */

//    run_command("md.b 0x90FFFF0 20", 0);
//    run_command("md.b 0x9000000 f30", 0);

    g_cfgContent = (char*)CONFIG_SP_RTCFG_ADDR;

    if (sp_config_check() != SP_CFG_RET_SUCCESS) {
        printf("\n!!!!!!!!!!!!!!!!\n");
        printf("ERROR: corrupted sp_config data\n");
        printf("!!!!!!!!!!!!!!!!\n\n");
        g_cfgContent = NULL;
        return SP_CFG_RET_FAILED;
    }

    sp_config_pminit();
    sp_config_gpio_init();
    sp_config_setup();
    //sp_config_insert_mirror(g_cfgContent);
    return SP_CFG_RET_SUCCESS;
}

int sp_config_isp_init(char *p)
{
    if(p == NULL)     return SP_CFG_RET_FAILED;

    g_cfgContent = (char*)p;

    //special case for unset pinmux for isp from SD: sft_cfg4 [14:13], [16:15]
    unsigned int isp_sd = ((MOON1_REG)->sft_cfg[4] & 0x1E000);


    sp_config_pminit();
    sp_config_gpio_init();

    //special case for unset pinmux when isp
    (MOON1_REG)->sft_cfg[4] = (((MOON1_REG)->sft_cfg[4] & 0xFFFE1FFF) | isp_sd);


    return SP_CFG_RET_SUCCESS;
}
