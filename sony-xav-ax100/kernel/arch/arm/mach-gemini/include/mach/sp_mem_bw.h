#ifndef __SP_MEM_BW_H__
#define __SP_MEM_BW_H__

// ----------------------------------------------
#define PRIORITY_LL             (6 << 0)
#define PRIORITY_RT             (5 << 0)
#define PRIORITY_NORMAL         (4 << 0)
#define PRIORITY_DEFAULT        (0 << 0)
#define PRIORITY_ABOVE_NORMAL   (1 << 7)

#define PRIORITY_FORCED         (1 << 2)
// ----------------------------------------------
#define MEM_BW_C2MBG             (1)
#define MEM_BW_DSPD              (2)
#define MEM_BW_PERI              (3)
#define MEM_BW_CBDMA0            (4)
#define MEM_BW_GC520             (5)
#define MEM_BW_USBC0             (6)
#define MEM_BW_USBC1             (7)
#define MEM_BW_USBH              (8)
#define MEM_BW_M2S_SDIF          (9)
#define MEM_BW_DDFCH0           (10)
#define MEM_BW_DDFCH1           (11)
#define MEM_BW_OSD0             (12)
#define MEM_BW_OSD1             (13)
#define MEM_BW_OSD2             (14)
#define MEM_BW_AFRC             (15)
#define MEM_BW_TNRIW0           (16)
#define MEM_BW_TNRIW1           (17)
#define MEM_BW_V656IN0          (18)
#define MEM_BW_V656IN1          (19)
#define MEM_BW_HOST             (20)
#define MEM_BW_MR_MIX           (21)
#define MEM_BW_TVDEC            (22)
#define MEM_BW_IMG_DEC          (23)
#define MEM_BW_LF_MIX           (24)
#define MEM_BW_MR_CHR_MIX       (25)
#define MEM_BW_LF_CHR_MIX       (26)
#define MEM_BW_MPRG_MIX         (27)
#define MEM_BW_DMAG_MIX         (28)
#define MEM_BW_HS               (29)
#define MEM_BW_DUMMY            (30)
#define MEM_BW_HS_CHR           (31)
// ----------------------------------------------
extern int set_mbar_n_channel_x(u32 mbar_n, u32 channel, u32 priority, u32 bandwidth_in_MB);
extern int mem_bw_setup(u32 type);
// ----------------------------------------------
#endif /* __SP_MEM_BW_H__ */
