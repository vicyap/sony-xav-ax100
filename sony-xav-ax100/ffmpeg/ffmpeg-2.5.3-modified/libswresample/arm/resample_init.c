/*
 * Copyright (c) 2015 Sunplus Technology Ltd
 * Author: smchiang <smchiang@sunplus.com>
 *
 */

#include "libavutil/arm/cpu.h"
#include "libswresample/resample.h"

int ff_resample_common_int16_neon(ResampleContext *c, void *dst, const void *src, int sz, int upd);
int ff_resample_common_int32_neon(ResampleContext *c, void *dst, const void *src, int sz, int upd);
int ff_resample_common_float_neon(ResampleContext *c, void *dst, const void *src, int sz, int upd);

void swri_resample_dsp_arm_init(ResampleContext *c)
{
	int cpu_flags = av_get_cpu_flags();		
	
	if (have_neon(cpu_flags)) {
		switch(c->format){
    	case AV_SAMPLE_FMT_S16P:
            if (!c->linear)                 	    
       	    	c->dsp.resample = ff_resample_common_int16_neon;
    	    break;
    	case AV_SAMPLE_FMT_S32P: 
            if (!c->linear)   	    
    	    	c->dsp.resample = ff_resample_common_int32_neon;
    	    break; 
    	case AV_SAMPLE_FMT_FLTP: 
	    if (!c->linear)	       	
        	c->dsp.resample = ff_resample_common_float_neon;
        	break;      	
    	}
	}    
}
