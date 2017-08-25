#include <common.h>
#include <asm/arch/regmap.h>
//#include <types.h>
//#include <regmap.h>
//#include <config.h>
//#include "regmap_8388_all.h"

// use group 97 AV1 stc timer for measure boot cost with TIMESTAMP.
#define AVD_STC_REG_1 (STC_AV1_REG)

void AV1_SetStc(long long stc)
{
        AVD_STC_REG_1->stc_config  = 0;
        // 90KHz
        AVD_STC_REG_1->stc_divisor = (0x1 << 15) | 0x95;
        // renew
        AVD_STC_REG_1->stc_64      = 0;

        /**
         * strictly write sequence, should not change this order.
         */
        AVD_STC_REG_1->stc_31_16 = stc >> 16;
        AVD_STC_REG_1->stc_47_32 = stc >> 32;
        AVD_STC_REG_1->stc_63_48 = stc >> 48;
        AVD_STC_REG_1->stc_15_0  = stc;
}

unsigned AV1_GetStc32(void)
{
        unsigned stc_value = 0;

        /**
         * strictly read sequence, should not change this order.
         */
        // smart latch
        stc_value = AVD_STC_REG_1->stc_15_0;
		stc_value = 0;

		// read
        stc_value |= (unsigned)AVD_STC_REG_1->stc_31_16 << 16;
        stc_value |= AVD_STC_REG_1->stc_15_0;

        return stc_value;
}


long long AV1_GetStc64(void)
{
        long long stc_value = 0;

        // smart latch
        stc_value = AVD_STC_REG_1->stc_15_0;
		stc_value = 0;

		// read
        stc_value |= (long long)AVD_STC_REG_1->stc_63_48 << 48;
        stc_value |= (long long)AVD_STC_REG_1->stc_47_32 << 32;
        stc_value |= (long long)AVD_STC_REG_1->stc_31_16 << 16;
        stc_value |= AVD_STC_REG_1->stc_15_0;

        return stc_value;
}
