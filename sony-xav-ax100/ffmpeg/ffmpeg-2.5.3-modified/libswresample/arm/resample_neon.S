/*
 * Copyright (c) 2015 Sunplus Technology Ltd
 * Author: smchiang <smchiang@sunplus.com>
 *
 */

#include "libavutil/arm/asm.S"

// NOTE! The following values must be consistent with  
// struct ResampleContext in resample.h

#define	RC_FILTER_BANK				0x04
#define RC_FILTER_LENGTH			0x08
#define RC_FILTER_ALLOC				0x0c
#define RC_DST_INCR_DIV				0x18
#define RC_DST_INCR_MOD				0x1c
#define RC_INDEX					0x20
#define RC_FRAC						0x24
#define RC_SRC_INCR					0x28
#define RC_PHASE_SHIFT				0x30
#define RC_PHASE_MASK				0x34

/*static int RENAME(resample_common)(ResampleContext *c,
 *                                   void *dest, const void *source,
 *                                   int n, int update_ctx);
 */          
                         
function ff_resample_common_int16_neon, export=1								
		push    	{r4-r11,lr}	
											@ r0: *c
											@ r1: *dest
											@ r2: *source
											@ r3: n
											
											
		ldr			r4,[r0,#RC_INDEX]		@ r4: index = c->index													
		ldr			r5,[r0,#RC_PHASE_SHIFT]	@ r5: phase_shift = c->phase_shift
		ldr			r6,[r0,#RC_PHASE_MASK]	@ r6: phase_mask = c->phase_mask
		ldr			r7,[r0,#RC_FRAC]		@ r7: frac = c->frac
		mov			r8,r4,lsr r5			@ r8: sample_index = index >> phase_shift
		and			r4,r4,r6				@ index &= phase_mask
		
								
		// for (dst_index = 0; dst_index < n; dst_index++)									
10:
		ldr			r9,[r0,#RC_FILTER_ALLOC]@ load c->filter_alloc 
		ldr			r10,[r0,#RC_FILTER_BANK]@ load c->filter_bank
		ldr			r11,[r0,#RC_FILTER_LENGTH]@ load c->filter_length
		mov			r12,r9,lsl #1			@ c->filter_alloc*2
		mla			r10,r12,r4,r10			@ r10: p_filter = c->filter_bank + c->filter_alloc*2 * index
		add			r12,r11,#7				@ c->filter_length + 7
		add			r11,r2,r8,lsl #1		@ r11: p_src = source + sample_index*2
		mov			r9,r12,lsr #3			@ r9: cnt = (c->filter_length + 7)/8
		
		
		vmov.i32    q0,#0					@ val = 0
		vmov.i32    q1,#0					@ val = 0
		
		
		// for (i = 0; i < c->filter_length; i++)
20:				
		vld1.16 	{d8,d9},[r10]!			@ load p_filter
		vld1.16		{d10,d11},[r11]!		@ load p_src	
		vmlal.s16	q0,d8,d10
		vmlal.s16	q1,d9,d11			
        subs 		r9,r9,#1				@ cnt--
        bne			20b						@ if cnt != 0, skip ahead
        
        vadd.i32    q0,q0,q1
        vpadd.i32	d2,d0,d1            
        vmov.i32	d3,#0
        vpadd.i32	d0,d2,d3
        vqrshrn.s32	d2,q0,#15
        vst1.16		d2[0],[r1]!				@ save *dest

		ldr			r9,[r0,#RC_DST_INCR_DIV]@ load c->dst_incr_div
		ldr			r10,[r0,#RC_DST_INCR_MOD]@ load c->dst_incr_mod
		ldr			r11,[r0,#RC_SRC_INCR]	@ load c->src_incr
		add			r4,r4,r9				@ index += c->dst_incr_div
		add			r7,r7,r10				@ frac  += c->dst_incr_mod
		cmp			r7,r11					@ is frac >= c->src_incr ? 
        itt         ge
		subge		r7,r7,r11				@ if true, frac -= c->src_incr
		addge		r4,r4,#1				@ if true, index++
		
        mov         r12,r4,lsr r5		
		add			r8,r8,r12				@ sample_index += index >> c->phase_shift
		and			r4,r4,r6				@ index &= phase_mask
		
		subs		r3,r3,#1				@ n--
		bne 		10b						@ if n != 0, skip ahead
		
		
		 
		ldr			r3,[sp,#9*4]			@ load update_ctx
		cmp			r3,#0					@ is update_ctx != 0 ?
        itt         ne		
		strne		r4,[r0,#RC_INDEX]		@ if true, save c->index= index
		strne		r7,[r0,#RC_FRAC]		@ if true, c->frac= frac
		
        mov         r0,r8					@ return (sample_index)	
99:		pop     	{r4-r11,pc}
endfunc

function ff_resample_common_int32_neon, export=1
		push    	{r4-r11,lr}		
											@ r0: *c
											@ r1: *dest
											@ r2: *source
											@ r3: n
											
											
		ldr			r4,[r0,#RC_INDEX]		@ r4: index = c->index													
		ldr			r5,[r0,#RC_PHASE_SHIFT]	@ r5: phase_shift = c->phase_shift
		ldr			r6,[r0,#RC_PHASE_MASK]	@ r6: phase_mask = c->phase_mask
		ldr			r7,[r0,#RC_FRAC]		@ r7: frac = c->frac
		mov			r8,r4,lsr r5			@ r8: sample_index = index >> phase_shift
		and			r4,r4,r6				@ index &= phase_mask
		
								
		// for (dst_index = 0; dst_index < n; dst_index++)									
10:
		ldr			r9,[r0,#RC_FILTER_ALLOC]@ load c->filter_alloc 
		ldr			r10,[r0,#RC_FILTER_BANK]@ load c->filter_bank
		ldr			r11,[r0,#RC_FILTER_LENGTH]@ load c->filter_length
		mov			r12,r9,lsl #2			@ c->filter_alloc*4
		mla			r10,r12,r4,r10			@ r10: p_filter = c->filter_bank + c->filter_alloc*4 * index
		add			r12,r11,#7				@ c->filter_length + 7
		add			r11,r2,r8,lsl #2		@ r11: p_src = source + sample_index*4
		mov			r9,r12,lsr #3			@ r9: cnt = (c->filter_length + 7)/8
		
		
		vmov.i64    q0,#0					@ val = 0
		vmov.i64    q1,#0					@ val = 0
		vmov.i64    q2,#0					@ val = 0
		vmov.i64    q3,#0					@ val = 0
		
		// for (i = 0; i < c->filter_length; i++)
20:				
		vld1.32 	{d8,d9,d10,d11},[r10]!	@ load p_filter
		vld1.32		{d12,d13,d14,d15},[r11]!@ load p_src	
		vmlal.s32	q0,d8,d12
		vmlal.s32	q1,d9,d13
		vmlal.s32	q2,d10,d14
		vmlal.s32	q3,d11,d15		
        subs 		r9,r9,#1				@ cnt--
        bne			20b						@ if cnt != 0, skip ahead
        
        vadd.i64    q0,q0,q1
        vadd.i64    q0,q0,q2       
        vadd.i64    q0,q0,q3             
        vadd.i64	d2,d0,d1
        vqrshrn.s64	d0,q1,#30
        vst1.32		d0[0],[r1]!				@ save *dest

		ldr			r9,[r0,#RC_DST_INCR_DIV]@ load c->dst_incr_div
		ldr			r10,[r0,#RC_DST_INCR_MOD]@ load c->dst_incr_mod
		ldr			r11,[r0,#RC_SRC_INCR]	@ load c->src_incr
		add			r4,r4,r9				@ index += c->dst_incr_div
		add			r7,r7,r10				@ frac  += c->dst_incr_mod
		cmp			r7,r11					@ is frac >= c->src_incr ? 
        itt         ge
		subge		r7,r7,r11				@ if true, frac -= c->src_incr
		addge		r4,r4,#1				@ if true, index++
		
        mov         r12,r4,lsr r5		
		add			r8,r8,r12				@ sample_index += index >> c->phase_shift
		and			r4,r4,r6				@ index &= phase_mask
		
		subs		r3,r3,#1				@ n--
		bne 		10b						@ if n != 0, skip ahead
		
		
		 
		ldr			r3,[sp,#9*4]			@ load update_ctx
		cmp			r3,#0					@ is update_ctx != 0 ?
        itt         ne		
		strne		r4,[r0,#RC_INDEX]		@ if true, save c->index= index
		strne		r7,[r0,#RC_FRAC]		@ if true, c->frac= frac
		
        mov         r0,r8					@ return (sample_index)		        	
99:		pop     	{r4-r11,pc}
endfunc

function ff_resample_common_float_neon, export=1
		push    	{r4-r11,lr}		
											@ r0: *c
											@ r1: *dest
											@ r2: *source
											@ r3: n
											
											
		ldr			r4,[r0,#RC_INDEX]		@ r4: index = c->index													
		ldr			r5,[r0,#RC_PHASE_SHIFT]	@ r5: phase_shift = c->phase_shift
		ldr			r6,[r0,#RC_PHASE_MASK]	@ r6: phase_mask = c->phase_mask
		ldr			r7,[r0,#RC_FRAC]		@ r7: frac = c->frac
		mov			r8,r4,lsr r5			@ r8: sample_index = index >> phase_shift
		and			r4,r4,r6				@ index &= phase_mask
		
								
		// for (dst_index = 0; dst_index < n; dst_index++)									
10:
		ldr			r9,[r0,#RC_FILTER_ALLOC]@ load c->filter_alloc 
		ldr			r10,[r0,#RC_FILTER_BANK]@ load c->filter_bank
		ldr			r11,[r0,#RC_FILTER_LENGTH]@ load c->filter_length
		mov			r12,r9,lsl #2			@ c->filter_alloc*4
		mla			r10,r12,r4,r10			@ r10: p_filter = c->filter_bank + c->filter_alloc*4 * index
		add			r12,r11,#7				@ c->filter_length + 7
		add			r11,r2,r8,lsl #2		@ r11: p_src = source + sample_index*4
		mov			r9,r12,lsr #3			@ r9: cnt = (c->filter_length + 7)/8
		
		
		vmov.f32    q0,#0.0					@ val = 0
		vmov.f32    q1,#0.0					@ val = 0
		
		
		// for (i = 0; i < c->filter_length; i++)
20:				
		vld1.32 	{d8,d9,d10,d11},[r10]!	@ load p_filter
		vld1.32		{d12,d13,d14,d15},[r11]!@ load p_src	
		vmla.f32	q0,q4,q6
		vmla.f32	q1,q5,q7			
        subs 		r9,r9,#1				@ cnt--
        bne			20b						@ if cnt != 0, skip ahead
                
        vadd.f32    q0,q0,q1
        vpadd.f32	d2,d0,d1            
        vmov.f32	d3,#0.0
        vpadd.f32	d0,d2,d3        
        vst1.32		d0[0],[r1]!				@ save *dest

		ldr			r9,[r0,#RC_DST_INCR_DIV]@ load c->dst_incr_div
		ldr			r10,[r0,#RC_DST_INCR_MOD]@ load c->dst_incr_mod
		ldr			r11,[r0,#RC_SRC_INCR]	@ load c->src_incr
		add			r4,r4,r9				@ index += c->dst_incr_div
		add			r7,r7,r10				@ frac  += c->dst_incr_mod
		cmp			r7,r11					@ is frac >= c->src_incr ? 
        itt         ge
		subge		r7,r7,r11				@ if true, frac -= c->src_incr
		addge		r4,r4,#1				@ if true, index++
		
        mov         r12,r4,lsr r5		
		add			r8,r8,r12				@ sample_index += index >> c->phase_shift
		and			r4,r4,r6				@ index &= phase_mask
		
		subs		r3,r3,#1				@ n--
		bne 		10b						@ if n != 0, skip ahead
		
		
		 
		ldr			r3,[sp,#9*4]			@ load update_ctx
		cmp			r3,#0					@ is update_ctx != 0 ?
        itt         ne		
		strne		r4,[r0,#RC_INDEX]		@ if true, save c->index= index
		strne		r7,[r0,#RC_FRAC]		@ if true, c->frac= frac
		
        mov         r0,r8					@ return (sample_index)		        	
99:		pop     	{r4-r11,pc}
endfunc
