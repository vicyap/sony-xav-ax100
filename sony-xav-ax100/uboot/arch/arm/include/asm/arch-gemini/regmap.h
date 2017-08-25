#ifndef __INC_REGMAP_H
#define __INC_REGMAP_H

#include <asm/arch/hardware.h>

#ifndef CONFIG_PLATFORM_letb123
#include "regmap_gemini.h"
#else
#include "regmap_letb123.h"
#endif

#define CONFIG_SP_BCH_BASE        BCH_S338_BASE_ADDRESS
#define CONFIG_SP_NAND_BASE       NAND_S330_BASE

#endif /* __INC_REGMAP_H */
