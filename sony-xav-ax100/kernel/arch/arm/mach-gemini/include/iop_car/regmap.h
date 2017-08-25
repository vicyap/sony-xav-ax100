#ifndef __REGMAP_H__
#define __REGMAP_H__

#include <mach/hardware.h>
#include "platform_define.h" //Kaney add
#include "customize.h" //Kaney add

typedef struct
{
#if (IC_VER == QAF536)
	#include "regmap_1503.inc"
	#define 	sft_cfg_5 mbus_mx_ab_sel
	#define 	mbar0_page_setting mbar0_m_setting[0]
#elif (IC_VER == QAF561)
	#include "regmap_1506.inc"
	#define 	sft_cfg_5 mbus_mx_ab_sel
	#define 	mbar0_page_setting mbar0_m_setting[0]
#elif (IC_VER == QCE491)
	#include "regmap_1502.inc"
	#define 	sft_cfg_5 mbus_mx_ab_sel
	#define 	mbar0_page_setting mbar0_m_setting[0]
#elif (IC_VER == QCE483)
	#include "regmap_8602.inc"
	#define  	g1_reserved_0 sft_cfg_0
	#define 	sft_cfg_5 mbus_mx_ab_sel
	#define 	mbar0_page_setting mbar0_m_setting[0]
	#define 	mbar1_page_setting mbar1_m_setting[0]
#else
	#include "regmap.inc"
#endif
} RegisterFile;

#define RGST_OFFSET IO0_BASE

#define regs0 ((volatile RegisterFile *)(RGST_OFFSET))

#define HWREG_W(reg, value)	\
	do {					\
		regs0->reg = value; \
                            \
	} while (0)
#define HWREG_R(reg) (regs0->reg)

/* Get the address of a HWREG.  */
#define HWREG_ADDR(reg)	(&(regs0->reg))

/* Set HWREG bits.  */
#define HWREG_S(reg, mask, pattern);	\
	do {								\
		unsigned int x;					\
		x = HWREG_R(reg);				\
		x = x & (mask);					\
		x = x | (pattern);				\
		HWREG_W(reg, x);				\
	} while (0)

#define HWREG_1(reg, pattern)	\
	do {						\
		unsigned int x;			\
		x = HWREG_R(reg);		\
		x = x | (pattern);		\
		HWREG_W(reg, x);		\
	} while (0)

#define HWREG_0(reg, pattern)	\
	do {						\
		unsigned int x;			\
		x = HWREG_R(reg);		\
		x = x & (pattern);		\
		HWREG_W(reg, x);		\
	} while (0)

#endif /*__REGMAP_H__*/
