/*
 * (C) Copyright 2015
 * Sunplus Technology. <http://www.sunplus.com/>
 *
 */

#ifndef __SP_CONFIG_H
#define __SP_CONFIG_H

#define SP_CFG_RET_SUCCESS    0
#define SP_CFG_RET_FAILED     (-1)

#define SP_CFG_PM_LEN         25

enum{
	SP_CFG_PM_SFT_CFG_0,
	SP_CFG_PM_SFT_CFG_1,
	SP_CFG_PM_SFT_CFG_2,
	SP_CFG_PM_SFT_CFG_3,
	SP_CFG_PM_SFT_CFG_4,
	SP_CFG_PM_SFT_CFG_5,
	SP_CFG_PM_SFT_CFG_6,
	SP_CFG_PM_SFT_CFG_7,
	SP_CFG_PM_SFT_CFG_8,
	SP_CFG_PM_SFT_CFG_MAX
};

typedef struct sp_cfg_pm{
	char name[SP_CFG_PM_LEN];
	int    bit_no;
	int    bit_cnt;
	int    val;
}sp_cfg_pm_t;

int sp_config_init(void);
int sp_config_get_int(char *cfgStr, int *ret);
int sp_config_get_string(char *cfgStr, char *ret, int maxSize);
int sp_config_get_bool(char *cfgStr, bool *ret);
int sp_config_extract_cmdline(char *cfgStr, unsigned int *val);
int sp_config_parse_pm(int table, int *value);

#endif
