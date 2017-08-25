/*
 * (C) Copyright 2015
 * Sunplus Technology. <http://www.sunplus.com/>
 *
 */
#include <mach/kernel.h>
#include <mach/sp_reservemem.h>
#include <mach/sp_config.h>
#include <mach/module.h>
#undef SP_CONFIG_DEBUG

//DECLARE_GLOBAL_DATA_PTR;

//#define CFG_ATAG_BASE               0x00800100

// limitation
#define CONFIG_LINE_MAX             100  //100 + \n
#define CONFIG_MAX_LEN              10240  //total length of config

//#define SP_CFG_PREFIX_LEN           16
//#define SP_CFG_SUFFIX_LEN           16

//#define SP_CFG_TAG_MAX              2
//#define SP_CFG_TAG_PREFIX_LEN       5  //strlen(uboot_tag[0])

//#define SP_CFG_PREFIX_BEGIN         " config_begin"
//#define SP_CFG_SUFFIX_END           " config_end"
#define SP_CFG_HEAD                 "\n"
#define SP_CFG_END                  "="

#define SP_CFG_CHAR_SEPARATOR       0x0A
#define SP_CFG_SEPARATOR            "\n"
#define SP_CFG_INITIAL_VALUE        (-1)

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

/*
sample config string => ucfg.panel_num=1 ucfg.panel_resol=2 kcfg.display_dual_screen=n
*/
char *g_cfgContent = NULL;

/*
sample input = ucfg.panel_num=1 ucfg.panel_resol=2 kcfg.display_dual_screen=n
*/

#ifdef SP_CONFIG_DEBUG
static void sp_tick(unsigned long long* t){  //tick = 10 ^ -7
    *t = get_ticks();
}

static void sp_timer(ulong b, ulong* t){  //tick = 10 ^ -7
    *t = get_timer(b);
}
#endif

static int _atoi(char *s)
{
	int i = 0;
	int sign = 1;

	while ( *s != NULL &&
		    (((*s) >= '0' && (*s) <= '9') || (*s == '-')) ) {
		if (*s == '-') {
			sign = -1;
		} else {
			i = i * 10 + (*s - '0');
		}
		s++;
	}

	i = sign * i;

	//DBG(printf("i = %d\n"));
	return i;
}

int sp_config_get_int(char *cfgStr, int *ret)
{
	char *s = NULL;
	char cond[CONFIG_LINE_MAX] = SP_CFG_HEAD;
	int sign = 0;
	int base = 10;

	if (g_cfgContent == NULL || strlen(g_cfgContent) == 0) {
		return SP_CFG_RET_FAILED;
	}

	strcat(cond, cfgStr);
	strcat(cond, SP_CFG_END);

	s = strstr(g_cfgContent, cond);
	if (s != NULL) {
		s = strstr(s, SP_CFG_END);
		if (s != NULL) {
			++s;
			if (s[0] == '-') {
				sign = 1;
				s++;
			}
			if (s[0] == '0' && s[1] == 'x') {
				base = 16;
				s += 2;
			}
			if (s[0] == '0' && s[1] == 'o') {
				base = 8;
				s += 2;
			}
			if (s[0] == '0' && s[1] == 'b') {
				base = 2;
				s += 2;
			}

			*ret = simple_strtol(s, NULL, base);
			if (sign)
				*ret = - *ret;
			return SP_CFG_RET_SUCCESS;
		}
	} else {
		*ret = (-1);
	}

	return SP_CFG_RET_FAILED;
}
EXPORT_SYMBOL(sp_config_get_int);
int sp_config_get_string(char *cfgStr, char *ret, int maxSize)
{
	char *s = NULL;
	char cond[CONFIG_LINE_MAX] = SP_CFG_HEAD;

	if (g_cfgContent == NULL || strlen(g_cfgContent) == 0) {
		return SP_CFG_RET_FAILED;
	}

	strcat(cond, cfgStr);
	strcat(cond, SP_CFG_END);

	s = strstr(g_cfgContent, cond);
	if (s != NULL) {
		char* e = NULL;
		int len;
		s = strstr(s, SP_CFG_END);
		e = strstr(s, SP_CFG_SEPARATOR);
		if (s != NULL && e != NULL) {
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
	if (g_cfgContent == NULL || strlen(g_cfgContent) == 0) {
		return SP_CFG_RET_FAILED;
	}

	strcat(cond, cfgStr);
	strcat(cond, SP_CFG_END);

	s = strstr(g_cfgContent, cond);
	if (s != NULL) {
		s = strstr(s, SP_CFG_END);
		if (s != NULL) {
			if (*(s+1) == 'y') {
				*ret = true;
				return SP_CFG_RET_SUCCESS;
			} else if (*(s+1) == 'n') {
				*ret = false;
				return SP_CFG_RET_SUCCESS;
			}
		}
	}
	return SP_CFG_RET_FAILED;
}

int sp_config_init(void)
{
	volatile u8 *runtime_cfg_addr;
	runtime_cfg_addr = (u8*)ioremap( (unsigned long)res_rtcfg_addr, res_rtcfg_size);
	g_cfgContent = (char*)runtime_cfg_addr;

	printk("sp_config: init \n");

	return SP_CFG_RET_SUCCESS;

error:

	return SP_CFG_RET_FAILED;

}
MODULE_LICENSE("GPL v2");

