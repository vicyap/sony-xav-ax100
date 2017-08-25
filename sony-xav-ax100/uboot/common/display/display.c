#include "disp_hal.h"
#include "reg_tnr.h"

static UINT32 gOSD_BUF_ID = 1;
static UINT16 gDMIX_V_BP = 0;

extern UINT8 get_part_updt_flag(void);

/*================================*/
/* TNR PQ def. */
static TNR_REG_T *g_tnr_reg[VPP_PATH_NUM] = {(TNR_REG_T*)((UINT32)REG_START_8388 + RGST_TNR_0_OFFSET),
											(TNR_REG_T*)((UINT32)REG_START_8388 + RGST_TNR_1_OFFSET)};

#define SUB_ABS(a, b)	((a)>(b)?(a)-(b):(b)-(a))

typedef struct
{
	UINT8 motion_lut_type; // ref. DRV_PQ_TNR_Motion_Lut_Type_e
	UINT8 y_motion_threshold[5];
	UINT8 y_blending_factor[5];
	UINT8 c_motion_threshold[5];
	UINT8 c_blending_factor[5];
} DRV_PQ_TNR_Lut_Param_t;
/*================================*/

#if 0
int LoadLogo(unsigned int bufbase, unsigned int UI_w, unsigned int UI_h, DRV_Logo_header_t* pLogoHeader)
{
	//Display reserved start 0x8B00000, 0x8B00000~0x8B00080
	//DRV_Logo_header_t* pLogoHeader = (DRV_Logo_header_t*)0x8B00100;
	unsigned int *tempbase = (unsigned int *)((UINT32)pLogoHeader + sizeof(DRV_Logo_header_t));
	unsigned int ret = 0;
	unsigned int *ptr = (UINT32 *)bufbase;
	int i;
//	int j;
	unsigned int start_x;
	unsigned int start_y;
#if 0
	// 1. Load Logo header
	if(load_app_image(BOOT_LOGO_IMG_NUM, pLogoHeader, ALIGN(sizeof(DRV_Logo_header_t), 8192)))
	{
		DBGPRINT ("Load logo header failed\r\n");
		return 1;
	}
#endif
	// 2. Logo Identifier check
	if (pLogoHeader->Identifier == 0x4c4f474f)
	{
		DBGPRINT ("Logo x = %d, y = %d, width = %d, height = %d, flag = 0x%x, UI_w %d, UI_h %d\r\n", pLogoHeader->x, pLogoHeader->y, pLogoHeader->width, pLogoHeader->height, pLogoHeader->location_flag, UI_w, UI_h);

		start_x = pLogoHeader->x;
		start_y = pLogoHeader->y;

		// 3. header error checking
		if (pLogoHeader->location_flag & HORIZONTAL_CENTERING)
		{
			if (pLogoHeader->width > UI_w)
			{
				DBGPRINT ("Incorrect Logo parameter. (pLogoHeader->width > UI_w)\r\n");
				return 1;
			}
			start_x = (UI_w - pLogoHeader->width)/2; // start x in Pixel
		}
		else if ((pLogoHeader->width + pLogoHeader->x) > UI_w)
		{
			DBGPRINT ("Incorrect Logo parameter. ((pLogoHeader->width + pLogoHeader->x) > UI_w)\r\n");
			return 1;
		}

		if (pLogoHeader->location_flag & VERTICAL_CENTERING)
		{
			if (pLogoHeader->height > UI_h)
			{
				DBGPRINT ("Incorrect Logo parameter. (pLogoHeader->height > UI_h)\r\n");
				return 1;
			}
			start_y = (UI_h -pLogoHeader->height)/2; //start y in Lines
		}
		else if ((pLogoHeader->height + pLogoHeader->y) > UI_h)
		{
			DBGPRINT ("Incorrect Logo parameter. ((pLogoHeader->height + pLogoHeader->y) > UI_h)\r\n");
			return 1;
		}
#if 0
		// 4. Load whole bin again
		if(load_app_image(BOOT_LOGO_IMG_NUM, (unsigned int *)(tempbase), ALIGN(pLogoHeader->width*pLogoHeader->height*4+sizeof(DRV_Logo_header_t), 8192)))
		{
			DBGPRINT ("Load logo data failed\r\n");
			return 1;
		}
#endif
		//tempbase += sizeof(DRV_Logo_header_t)/4;

		// 5. Copy logo data into framebuffer
		if (pLogoHeader->location_flag & LOGO_DATA_VFLIP)
		{
			for(i=0; i<pLogoHeader->height; i++)
			{
				ptr = (UINT32*)(bufbase + ((start_y+pLogoHeader->height-1-i)*UI_w + start_x)*4);
				memcpy(ptr, &tempbase[i*pLogoHeader->width], pLogoHeader->width*4);
			#if 0
				for(j=0; j < pLogoHeader->width; j++)
				{
					if(j < pLogoHeader->width/3)
						*(ptr +j) = 0xff00ffff;
					else if(j < 2 * pLogoHeader->width/3)
						*(ptr +j) = 0xffff00ff;
					else
						*(ptr +j) = 0xff0000ff;
				}
			#endif
			}
		}
		else
		{
			for(i=0; i<pLogoHeader->height; i++)
			{
				ptr = (UINT32*)(bufbase + ((start_y+i)*UI_w + start_x)*4);
				memcpy(ptr, &tempbase[i*pLogoHeader->width], pLogoHeader->width*4);
			}
		}
	}
	else
	{
		DBGPRINT ("Load logo header check failed\r\n");
		ret = 1;
	}

	return ret;
}
#endif

void SysInit(void)
{
	//unsigned int ic_stamp = (*((volatile unsigned int *)(REG_START_8388)));	// G0.0

	(*((volatile unsigned int *)(REG_START_8388+128+29*4))) |= 0x02000000;	// G1.29 , Set DVE656R Clk = 54MHz

	// VDEC patch setting
	// CHBCD clock select (G0 A4 [9:8]=3,G0 A4 [3]=1)
	// VENC (G209,A0 [3:1]=1, G209 A4 [6]=1, G209 A4 [1:0]=0 ,G211 A11 [3:2]=1,G211 A11 [1:0]=0)
	// VDAC resistor select and clk select (G1 16 [20],[3])
	// VDAC ECO item
	(*((volatile unsigned int *)(REG_START_8388+4*4))) |= 0x308;
	(*((volatile unsigned int *)(REG_START_8388+128+16*4))) = 0x2f100008;	// G1.16 , set VDAC
													//0x6f100008 -> 0x2f100008 : G1.16 b30:0=from otp, 1=from reg
	return;
}

void TGEN(UINT16 H, UINT16 V, UINT16 htotal, UINT16 vtotal)
{
	unsigned int *reg203 = (unsigned int *)(REG_START_8388 + 203*128);
	unsigned int *reg204 = (unsigned int *)(REG_START_8388 + 204*128);

	//G203
	reg203[0] = 0x001F;
	reg203[1] = 0x0000;
	reg203[2] = 0x0180;
	reg203[3] = 0x1200;
	reg203[4] = 0x0010;
	reg203[5] = 0x1006;
	reg203[6] = 0x0101;
	reg203[7] = 0x1003;
	reg203[8] = 0x035A;
	reg203[9] = 0x0311;
	reg203[10] = 0x020D;
	reg203[11] = 0x0205;
	reg203[12] = 0x0002;
	//reg203[13] = 0x01A4;
	//reg203[14] = 0x0035;
	reg203[15] = 0x0026;
	reg203[16] = 0x0002;
	reg203[17] = 0x851F;
	reg203[18] = 0x002F;
	reg203[19] = 0x035A;
	reg203[20] = 0x035A;
	reg203[21] = 0x0205;
	reg203[22] = 0x0311;
	reg203[23] = 0x211F;
	reg203[24] = 0x1D1B;
	reg203[25] = 0x1917;
	reg203[26] = 0x1207;
	//reg203[27] = 0x10DD;
	//reg203[28] = 0x011B;
	reg203[29] = 0x0029;
	reg203[30] = 0x0001;
	//reg203[31] = 0x0000;

	//G204
	reg204[0] = 0x001F;
	reg204[1] = 0x0000;
	reg204[2] = 0x0180;
	reg204[3] = 0x1200;
	reg204[4] = 0x0011;
	reg204[5] = 0x1006;
	reg204[6] = 0x0101;
	reg204[7] = 0x1003;
	reg204[8] = htotal;		// tot pixel
	reg204[9] = H + 64 + 1;	// start line
	reg204[10] = vtotal;		// tot line
	reg204[11] = 0x0205;
	reg204[12] = 0x0002;
	//reg204[13] = 0x01C9;
	//reg204[14] = 0x0248;
	reg204[15] = gDMIX_V_BP;	// act line
	reg204[16] = 0x0002;
	reg204[17] = 0x851F;
	reg204[18] = 0x002F;
	reg204[19] = 0x035A;
	reg204[20] = 0x035A;
	reg204[21] = 0x0205;
	reg204[22] = 0x0311;
	reg204[23] = 0x211F;
	reg204[24] = 0x1D1B;
	reg204[25] = 0x191B;
	reg204[26] = 0x1207;
	//reg204[27] = 0x111D;
	//reg204[28] = 0x0268;
	reg204[29] = 0x0029;
	reg204[30] = 0x0001;
	//reg204[31] = 0x0000;

	return;
}

void DMIX(UINT16 H, UINT16 V)
{
	unsigned int *reg205 = (unsigned int *)(REG_START_8388 + 205*128);
	unsigned int *reg206 = (unsigned int *)(REG_START_8388 + 206*128);
	unsigned int *reg207 = (unsigned int *)(REG_START_8388 + 207*128);
	unsigned int *reg208 = (unsigned int *)(REG_START_8388 + 208*128);

	//G205
	reg205[0] = 0x2000;
	reg205[1] = 0x8545;
	reg205[2] = 0x041C;
	reg205[3] = 0x0000;
	reg205[4] = 0x0000;
	reg205[5] = 0x0000;
	reg205[6] = 0x0000;
	reg205[7] = 0x0000;
	reg205[8] = 0x0000;
	reg205[9] = 0x0000;
	reg205[10] = 0x0000;
	reg205[11] = 0x0000;
	reg205[12] = 0x0000;
	reg205[13] = 0x0000;
	reg205[14] = 0x0010;
	reg205[15] = 0x8080;
	reg205[16] = 0x0000;
	reg205[17] = 0x0240;
	reg205[18] = 0x0000;
	reg205[19] = 0x0000;
	reg205[20] = 0x0010;
	reg205[21] = 0x0080;
	reg205[22] = 0x0080;
	reg205[23] = 0x0000;
	reg205[24] = 0x0000;
	reg205[25] = 0x0000;
	reg205[26] = 0x0000;
	reg205[27] = 0x0000;
	reg205[28] = 0x0000;
	reg205[29] = 0x0000;
	reg205[30] = 0x0000;
	reg205[31] = 0x0000;

	//G206
	reg206[0] = 0x2000;
	reg206[1] = 0x8515;
	reg206[2] = 0x441C;
	reg206[3] = 0x0000;
	reg206[4] = 0x0000;
	reg206[5] = 0x0000;
	reg206[6] = 0x0000;
	reg206[7] = 0x0000;
	reg206[8] = 0x0000;
	reg206[9] = 0x0000;
	reg206[10] = 0x0000;
	reg206[11] = 0x0000;
	reg206[12] = 0x0000;
	reg206[13] = 0x0000;
	reg206[14] = 0x0010;
	reg206[15] = 0x8080;
	reg206[16] = 0x0000;
	reg206[17] = 8 | (gDMIX_V_BP << 4);
	reg206[18] = 0x0000;
	reg206[19] = 0x0000;
	reg206[20] = 0x0010;
	reg206[21] = 0x0080;
	reg206[22] = 0x0080;
	reg206[23] = 0x0000;
	reg206[24] = 0x0000;
	reg206[25] = 0x0000;
	reg206[26] = 0x0000;
	reg206[27] = 0x0000;
	reg206[28] = 0x0000;
	reg206[29] = 0x0000;
	reg206[30] = 0x0000;
	reg206[31] = 0x0000;

	//G207
	reg207[0] = 0x8046;
	reg207[1] = 0x0000;
	reg207[2] = 0x0000;
	reg207[3] = 0x0000;
	reg207[4] = 0x0000;
	reg207[5] = 0x0000;
	reg207[6] = 0x5060;
	reg207[7] = 0xB0A0;
	reg207[8] = 0x0100;
	reg207[9] = 0x00D5;
	reg207[10] = 0x0180;
	reg207[11] = 0x00D5;
	reg207[12] = 0x0200;
	reg207[13] = 0x0000;
	reg207[14] = 0x0001;
	reg207[15] = 0x01E0;
	reg207[16] = 0x02D0;
	reg207[17] = 0x0024;
	reg207[18] = 0x0000;
	reg207[19] = 0x0000;
	reg207[20] = 0x0010;
	reg207[21] = 0x8080;
	reg207[22] = 0x0000;
	reg207[23] = 0x0000;
	reg207[24] = 0x0000;
	reg207[25] = 0x0000;
	reg207[26] = 0x0000;
	reg207[27] = 0x0000;
	reg207[28] = 0x0000;
	reg207[29] = 0x0000;
	reg207[30] = 0x0000;
	reg207[31] = 0x0000;

	//G208
	reg208[0] = 0x8046;
	reg208[1] = 0x0000;
	reg208[2] = 0x0000;
	reg208[3] = 0x0000;
	reg208[4] = 0x0000;
	reg208[5] = 0x0000;
	reg208[6] = 0x5060;
	reg208[7] = 0xB0A0;
	reg208[8] = 0x0100;
	reg208[9] = 0x00D5;
	reg208[10] = 0x0180;
	reg208[11] = 0x00D5;
	reg208[12] = 0x0200;
	reg208[13] = 0x0000;
	reg208[14] = 0x0003;
	reg208[15] = V;
	reg208[16] = H;
	reg208[17] = gDMIX_V_BP;
	reg208[18] = 0x0000;
	reg208[19] = 0x0000;
	reg208[20] = 0x0010;
	reg208[21] = 0x8080;
	reg208[22] = 0x0002;
	reg208[23] = 0x0001;
	reg208[24] = 0x0000;
	reg208[25] = 0x0000;
	reg208[26] = 0x0000;
	reg208[27] = 0x0000;
	reg208[28] = 0x0000;
	reg208[29] = 0x0000;
	reg208[30] = 0x0000;
	reg208[31] = 0x0000;

	return;
}

#if 0
void GSCL(unsigned int src_w, unsigned int src_h, unsigned int w, unsigned int h, UINT32 IsCVBSPal)
{
	unsigned int *reg = (UINT32 *)0x9c006c00;	// GSCL0
	UINT32 tmp_factor;

	// G216
	reg[0] = 0x00000001;
#ifdef GSCL_CENTER
	reg[1] = 0x00000000 | 0x30 | 1<<1;
#else
	reg[1] = 0x00000000 | 0x30 | ((src_w!=w)<<2) | ((src_h!=h)<<3);
#endif
	reg[2] = 0x00000000;
	reg[3] = 0x00000000;
	reg[4] = src_w;
	reg[5] = src_h;
	reg[6] = 0x00000000;
	reg[7] = 0x00000000;

	reg[8] = src_w;
	reg[9] = src_h;
	reg[10] = w;
	reg[11] = h;
#ifdef GSCL_CENTER
	reg[12] = (w-src_w)>>1;
	reg[13] = (h-src_h)>>1;
	reg[14] = src_w;
	reg[15] = src_h;
	w = src_w;
	h = src_h;
#else
	reg[12] = 0x00000000;
	reg[13] = 0x00000000;
	reg[14] = w;
	reg[15] = h;
#endif

	reg[16] = 0x00008080;
	reg[17] = 0x00000010;
	reg[18] = 0x00000000;

	{
		tmp_factor = (((src_w << 21)/w)<<1) & 0x01ffffff;

		if(tmp_factor <= (1<<22))// scaling up
			reg[19] = (reg[19] & 0xff00) | 0x34;
		else if (tmp_factor <= 4971089)
			reg[19] = (reg[19] & 0xff00) | 0x54;
		else if (tmp_factor <= 5242880)
			reg[19] = (reg[19] & 0xff00) | 0x64;
		else if (tmp_factor <= 5592265)
			reg[19] = (reg[19] & 0xff00) | 0x74;
		else if (tmp_factor <= 5965139)
			reg[19] = (reg[19] & 0xff00) | 0x84;
		else if (tmp_factor <= 6291456)
			reg[19] = (reg[19] & 0xff00) | 0x94;
		else if (tmp_factor <= 6878658)
			reg[19] = (reg[19] & 0xff00) | 0xA4;
		else if (tmp_factor <= 6991904)
			reg[19] = (reg[19] & 0xff00) | 0x44;
		else if (tmp_factor <= 7456633)
			reg[19] = (reg[19] & 0xff00) | 0xB4;
		else if (tmp_factor <= 8388608)
			reg[19] = (reg[19] & 0xff00) | 0xC4;
		else if (tmp_factor <= 9646899)
			reg[19] = (reg[19] & 0xff00) | 0xD4;
		else if (tmp_factor <= 11184950)
			reg[19] = (reg[19] & 0xff00) | 0xE4;
		else
			reg[19] = (reg[19] & 0xff00) | 0xF4;

		reg[20] = tmp_factor & 0xffff;
		reg[21] = (tmp_factor & 0x01ff0000) >> 16;
	}
	reg[22] = 0x00000000;
	reg[23] = 0x00000000;

	reg[24] = 0x00000000;
	reg[25] = 0x00000000;

	{
		tmp_factor = (((src_h << 21)/h)<<1) & 0x01ffffff;

		if(tmp_factor == (1<<22))// scaling up
			reg[26] = (reg[26] & 0xff00) | 0x10;
		else if(tmp_factor < (1<<22))// scaling up
			reg[26] = (reg[26] & 0xff00) | 0x30;
		else if (tmp_factor <= 4660291)
			reg[26] = (reg[26] & 0xff00) | 0x40;
		else if (tmp_factor <= 5033164)
			reg[26] = (reg[26] & 0xff00) | 0x50;
		else if (tmp_factor <= 5242880)
			reg[26] = (reg[26] & 0xff00) | 0x60;
		else if (tmp_factor <= 5592265)
			reg[26] = (reg[26] & 0xff00) | 0x70;
		else if (tmp_factor <= 6291456)
			reg[26] = (reg[26] & 0xff00) | 0x80;
		else if (tmp_factor <= 6710886)
			reg[26] = (reg[26] & 0xff00) | 0x90;
		else if (tmp_factor <= 6990520)
			reg[26] = (reg[26] & 0xff00) | 0xA0;
		else if (tmp_factor <= 7864320)
			reg[26] = (reg[26] & 0xff00) | 0xB0;
		else if (tmp_factor <= 8388608)
			reg[26] = (reg[26] & 0xff00) | 0xC0;
		else if (tmp_factor <= 9437184)
			reg[26] = (reg[26] & 0xff00) | 0xD0;
		else if (tmp_factor <= 10485760)
			reg[26] = (reg[26] & 0xff00) | 0xE0;
		else
			reg[26] = (reg[26] & 0xff00) | 0xF0;
	}
	reg[27] = tmp_factor & 0xffff;
	reg[28] = (tmp_factor & 0x01ff0000) >> 16;

	reg[29] = 0x00000000;
	reg[30] = 0x00000000;
	reg[31] = 0x00000000;

	reg = (UINT32 *)0x9c006c80;	// G217
	reg[0] = 0x00000000;
	reg[1] = 0x00000000;
	reg[2] = 0x00000000;
	reg[3] = 0x00000929;
	reg[4] = 0x000087b6;
	reg[5] = 0x00000e0e;
	reg[6] = 0x00000e3c;
	reg[7] = 0x00003200;

	reg[8] = 0x0000a7f0;
	reg[9] = 0x00000101;
	reg[10] = 0x00000000;
	reg[11] = 0x00000000;
	reg[12] = 0x00000000;
	reg[13] = 0x00000000;
	reg[14] = 0x00000008;
	reg[15] = 0x00000000;

	reg[16] = 0x00001f16;
	reg[17] = 0x00000023;
	reg[18] = 0x00001f14;
	reg[19] = 0x00000000;
	reg[20] = 0x00000000;
	reg[21] = 0x00000000;
	reg[22] = 0x00000000;
	reg[23] = 0x00000000;

	reg[24] = 0x00000000;
	reg[25] = 0x00000000;
	reg[26] = 0x00000000;
	reg[27] = 0x00000000;
	reg[28] = 0x00000000;
	reg[29] = 0x00000000;
	reg[30] = 0x00000000;
	reg[31] = 0x00000000;


	{
		//GSCL setting for second screen
		{
			unsigned int w1 = 720;
			unsigned int h1;

			if (IsCVBSPal)
				h1 = 576;
			else
				h1 = 480;

			reg = (UINT32 *)0x9c006d00;	// GSCL1

			// G218
			reg[0] = 0x00000001;
			reg[1] = 0x00000000 | 0x30 | ((src_w!=w1)<<2) | ((src_h!=h1)<<3);
			reg[2] = 0x00000000;
			reg[3] = 0x00000000;
			reg[4] = src_w;
			reg[5] = src_h;
			reg[6] = 0x00000000;
			reg[7] = 0x00000000;

			reg[8] = src_w;
			reg[9] = src_h;


			reg[10] = w1;
			reg[11] = h1;
			reg[12] = 0x00000000;
			reg[13] = 0x00000000;
			reg[14] = w1;
			reg[15] = h1;

			reg[16] = 0x00008080;
			reg[17] = 0x00000010;
			reg[18] = 0x00000000;

			{
				UINT32 tmp_factor;

				tmp_factor = (((src_w << 21)/w1)<<1) & 0x01ffffff;

				if(tmp_factor <= (1<<22))
				{	// scaling up
					reg[19] = (reg[19] & 0xff00) | 0x34;
				}
				else if (tmp_factor <= 4971089)
				{
					reg[19] = (reg[19] & 0xff00) | 0x54;
				}
				else if (tmp_factor <= 5242880)
				{
					reg[19] = (reg[19] & 0xff00) | 0x64;
				}
				else if (tmp_factor <= 5592265)
				{
					reg[19] = (reg[19] & 0xff00) | 0x74;
				}
				else if (tmp_factor <= 5965139)
				{
					reg[19] = (reg[19] & 0xff00) | 0x84;
				}
				else if (tmp_factor <= 6291456)
				{
					reg[19] = (reg[19] & 0xff00) | 0x94;
				}
				else if (tmp_factor <= 6878658)
				{
					reg[19] = (reg[19] & 0xff00) | 0xA4;
				}
				else if (tmp_factor <= 6991904)
				{
					reg[19] = (reg[19] & 0xff00) | 0x44;
				}
				else if (tmp_factor <= 7456633)
				{
					reg[19] = (reg[19] & 0xff00) | 0xB4;
				}
				else if (tmp_factor <= 8388608)
				{
					reg[19] = (reg[19] & 0xff00) | 0xC4;
				}
				else if (tmp_factor <= 9646899)
				{
					reg[19] = (reg[19] & 0xff00) | 0xD4;
				}
				else if (tmp_factor <= 11184950)
				{
					reg[19] = (reg[19] & 0xff00) | 0xE4;
				}
				else
				{
					reg[19] = (reg[19] & 0xff00) | 0xF4;
				}

				reg[20]	= tmp_factor & 0xffff;
				reg[21]	= (tmp_factor & 0x01ff0000) >> 16;
			}

			reg[22] = 0x00000000;
			reg[23] = 0x00000000;

			reg[24] = 0x00000000;
			reg[25] = 0x00000000;

			{
				UINT32 tmp_factor;

				tmp_factor = (((src_h << 21)/h1)<<1) & 0x01ffffff;

				if(tmp_factor == (1<<22))
				{	// scaling up
					reg[26] = (reg[26] & 0xff00) | 0x10;
				}
				else if(tmp_factor < (1<<22))
				{	// scaling up
					reg[26] = (reg[26] & 0xff00) | 0x30;
				}
				else if (tmp_factor <= 4660291)
				{
					reg[26] = (reg[26] & 0xff00) | 0x40;
				}
				else if (tmp_factor <= 5033164)
				{
					reg[26] = (reg[26] & 0xff00) | 0x50;
				}
				else if (tmp_factor <= 5242880)
				{
					reg[26] = (reg[26] & 0xff00) | 0x60;
				}
				else if (tmp_factor <= 5592265)
				{
					reg[26] = (reg[26] & 0xff00) | 0x70;
				}
				else if (tmp_factor <= 6291456)
				{
					reg[26] = (reg[26] & 0xff00) | 0x80;
				}
				else if (tmp_factor <= 6710886)
				{
					reg[26] = (reg[26] & 0xff00) | 0x90;
				}
				else if (tmp_factor <= 6990520)
				{
					reg[26] = (reg[26] & 0xff00) | 0xA0;
				}
				else if (tmp_factor <= 7864320)
				{
					reg[26] = (reg[26] & 0xff00) | 0xB0;
				}
				else if (tmp_factor <= 8388608)
				{
					reg[26] = (reg[26] & 0xff00) | 0xC0;
				}
				else if (tmp_factor <= 9437184)
				{
					reg[26] = (reg[26] & 0xff00) | 0xD0;
				}
				else if (tmp_factor <= 10485760)
				{
					reg[26] = (reg[26] & 0xff00) | 0xE0;
				}
				else
				{
					reg[26] = (reg[26] & 0xff00) | 0xF0;
				}

				reg[27] = tmp_factor & 0xffff;
				reg[28]	= (tmp_factor & 0x01ff0000) >> 16;
			}

			reg[29] = 0x00000000;
			reg[30] = 0x00000000;
			reg[31] = 0x00000000;

			reg = (UINT32 *)0x9c006d80;	// G219
			reg[0] = 0x00000000;
			reg[1] = 0x00000000;
			reg[2] = 0x00000000;
			reg[3] = 0x00000929;
			reg[4] = 0x000087b6;
			reg[5] = 0x00000e0e;
			reg[6] = 0x00000e3c;
			reg[7] = 0x00003200;

			reg[8] = 0x0000a7f0;
			reg[9] = 0x00000101;
			reg[10] = 0x00000000;
			reg[11] = 0x00000000;
			reg[12] = 0x00000000;
			reg[13] = 0x00000000;
			reg[14] = 0x00000008;
			reg[15] = 0x00000000;

			reg[16] = 0x00001f16;
			reg[17] = 0x00000023;
			reg[18] = 0x00001f14;
			reg[19] = 0x00000000;
			reg[20] = 0x00000000;
			reg[21] = 0x00000000;
			reg[22] = 0x00000000;
			reg[23] = 0x00000000;

			reg[24] = 0x00000000;
			reg[25] = 0x00000000;
			reg[26] = 0x00000000;
			reg[27] = 0x00000000;
			reg[28] = 0x00000000;
			reg[29] = 0x00000000;
			reg[30] = 0x00000000;
			reg[31] = 0x00000000;
		}
	}
}
#else

void GSCL(UINT32 id, UINT32 x, UINT32 y, UINT32 src_w, UINT32 src_h, UINT32 w, UINT32 h, UINT32 flag)
{
	UINT32 *reg_0 = (UINT32 *)(REG_START_8388 + (216+id*2)*128);
	UINT32 *reg_1 = (UINT32 *)(REG_START_8388 + (217+id*2)*128);
	UINT32 tmp_factor;

	reg_0[0] = 0x00000001;

	reg_0[1] = 0x00000020;
	if ((flag & HORIZONTAL_SCALING) && (src_w != w))
		reg_0[1] |= (1<<2);
	if ((flag & VERTICAL_SCALING) && (src_h != h))
		reg_0[1] |= (1<<3) | (1<<4);
	if (((flag & HORIZONTAL_CENTERING) && (src_w != w)) || ((flag & VERTICAL_CENTERING) && (src_h != h)))
		reg_0[1] |= (1<<1);

	reg_0[2] = 0x00000000;
	reg_0[3] = 0x00000000;
	reg_0[4] = src_w;
	reg_0[5] = src_h;
	reg_0[6] = 0x00000000;
	reg_0[7] = 0x00000000;

	reg_0[8] = src_w;
	reg_0[9] = src_h;
	reg_0[10] = w;
	reg_0[11] = h;

	if (flag & HORIZONTAL_CENTERING)
		reg_0[12] = SUB_ABS(w, src_w)>>1;
	else
		reg_0[12] = x;
	if (flag & VERTICAL_CENTERING)
		reg_0[13] = SUB_ABS(h, src_h)>>1;
	else
		reg_0[13] = y;
	if ((flag & HORIZONTAL_SCALING) && (src_w != w))
		reg_0[14] = w;
	else
	{
		reg_0[14] = src_w;
		w = src_w;
	}
	if ((flag & VERTICAL_SCALING) && (src_h != h))
		reg_0[15] = h;
	else
	{
		reg_0[15] = src_h;
		h = src_h;
	}

	reg_0[16] = 0x00008080;
	reg_0[17] = 0x00000010;
	reg_0[18] = 0x00000000;

	{
		tmp_factor = (((src_w << 21)/w)<<1) & 0x01ffffff;

		if(tmp_factor <= (1<<22))// scaling up
			reg_0[19] = (reg_0[19] & 0xff00) | 0x34;
		else if (tmp_factor <= 4971089)
			reg_0[19] = (reg_0[19] & 0xff00) | 0x54;
		else if (tmp_factor <= 5242880)
			reg_0[19] = (reg_0[19] & 0xff00) | 0x64;
		else if (tmp_factor <= 5592265)
			reg_0[19] = (reg_0[19] & 0xff00) | 0x74;
		else if (tmp_factor <= 5965139)
			reg_0[19] = (reg_0[19] & 0xff00) | 0x84;
		else if (tmp_factor <= 6291456)
			reg_0[19] = (reg_0[19] & 0xff00) | 0x94;
		else if (tmp_factor <= 6878658)
			reg_0[19] = (reg_0[19] & 0xff00) | 0xA4;
		else if (tmp_factor <= 6991904)
			reg_0[19] = (reg_0[19] & 0xff00) | 0x44;
		else if (tmp_factor <= 7456633)
			reg_0[19] = (reg_0[19] & 0xff00) | 0xB4;
		else if (tmp_factor <= 8388608)
			reg_0[19] = (reg_0[19] & 0xff00) | 0xC4;
		else if (tmp_factor <= 9646899)
			reg_0[19] = (reg_0[19] & 0xff00) | 0xD4;
		else if (tmp_factor <= 11184950)
			reg_0[19] = (reg_0[19] & 0xff00) | 0xE4;
		else
			reg_0[19] = (reg_0[19] & 0xff00) | 0xF4;

		reg_0[20] = tmp_factor & 0xffff;
		reg_0[21] = (tmp_factor & 0x01ff0000) >> 16;
	}
	reg_0[22] = 0x00000000;
	reg_0[23] = 0x00000000;

	reg_0[24] = 0x00000000;
	reg_0[25] = 0x00000000;

	{
		tmp_factor = (((src_h << 21)/h)<<1) & 0x01ffffff;

		if(tmp_factor == (1<<22))// scaling up
			reg_0[26] = (reg_0[26] & 0xff00) | 0x10;
		else if(tmp_factor < (1<<22))// scaling up
			reg_0[26] = (reg_0[26] & 0xff00) | 0x30;
		else if (tmp_factor <= 4660291)
			reg_0[26] = (reg_0[26] & 0xff00) | 0x40;
		else if (tmp_factor <= 5033164)
			reg_0[26] = (reg_0[26] & 0xff00) | 0x50;
		else if (tmp_factor <= 5242880)
			reg_0[26] = (reg_0[26] & 0xff00) | 0x60;
		else if (tmp_factor <= 5592265)
			reg_0[26] = (reg_0[26] & 0xff00) | 0x70;
		else if (tmp_factor <= 6291456)
			reg_0[26] = (reg_0[26] & 0xff00) | 0x80;
		else if (tmp_factor <= 6710886)
			reg_0[26] = (reg_0[26] & 0xff00) | 0x90;
		else if (tmp_factor <= 6990520)
			reg_0[26] = (reg_0[26] & 0xff00) | 0xA0;
		else if (tmp_factor <= 7864320)
			reg_0[26] = (reg_0[26] & 0xff00) | 0xB0;
		else if (tmp_factor <= 8388608)
			reg_0[26] = (reg_0[26] & 0xff00) | 0xC0;
		else if (tmp_factor <= 9437184)
			reg_0[26] = (reg_0[26] & 0xff00) | 0xD0;
		else if (tmp_factor <= 10485760)
			reg_0[26] = (reg_0[26] & 0xff00) | 0xE0;
		else
			reg_0[26] = (reg_0[26] & 0xff00) | 0xF0;
	}
	reg_0[27] = tmp_factor & 0xffff;
	reg_0[28] = (tmp_factor & 0x01ff0000) >> 16;

	reg_0[29] = 0x00000000;
	reg_0[30] = 0x00000000;
	reg_0[31] = 0x00000000;

	reg_1[0] = 0x00000000;
	reg_1[1] = 0x00000000;
	reg_1[2] = 0x00000000;
	reg_1[3] = 0x00000929;
	reg_1[4] = 0x000087b6;
	reg_1[5] = 0x00000e0e;
	reg_1[6] = 0x00000e3c;
	reg_1[7] = 0x00003200;

	reg_1[8] = 0x0000a7f0;
	reg_1[9] = 0x00000101;
	reg_1[10] = 0x00000000;
	reg_1[11] = 0x00000000;
	reg_1[12] = 0x00000000;
	reg_1[13] = 0x00000000;
	reg_1[14] = 0x00000008;
	reg_1[15] = 0x00000000;

	reg_1[16] = 0x00001f16;
	reg_1[17] = 0x00000023;
	reg_1[18] = 0x00001f14;
	reg_1[19] = 0x00000000;
	reg_1[20] = 0x00000000;
	reg_1[21] = 0x00000000;
	reg_1[22] = 0x00000000;
	reg_1[23] = 0x00000000;

	reg_1[24] = 0x00000000;
	reg_1[25] = 0x00000000;
	reg_1[26] = 0x00000000;
	reg_1[27] = 0x00000000;
	reg_1[28] = 0x00000000;
	reg_1[29] = 0x00000000;
	reg_1[30] = 0x00000000;
	reg_1[31] = 0x00000000;
}

#endif

#if 0
void OSD(UINT32 w, UINT32 h, int ISP, unsigned int DispAddr)
{
	//OSD0, OSD1
	unsigned int *reg213 = (unsigned int *)(REG_START_8388 + 213*128);
	unsigned int *reg214 = (unsigned int *)(REG_START_8388 + 214*128);

	UINT32 bufbase = (UINT32)DispAddr;
	unsigned int *osd0_header = (unsigned int *)((UINT32)bufbase + PAGEALIGN(w*h*4));
	DRV_Logo_header_t* pLogoTmp = (DRV_Logo_header_t* )(osd0_header + 1024);
	UINT32 TotalSizeOfOsd = 1024 + 1024 + PAGEALIGN(w*h*4)*2;
	//char disp_cmd[256];
	//UINT32 partionSize = PAGEALIGN(w*h*4)*2;

	memset((UINT8*)bufbase, 0, TotalSizeOfOsd);

	if(!ISP)
	{
		extern int set_logo_addr(UINT32 mAddr);
		extern UINT32 get_nor_logo_addr(void);
		if (set_logo_addr((UINT32)pLogoTmp) != -1)
			LoadLogo(bufbase, w, h, (DRV_Logo_header_t*)get_nor_logo_addr());
	}

	printf("Input addr=0x%x\n", DispAddr);

	osd0_header[0] = 0x10000E;
	//osd0_header[1] = 0x2C012C01;
	osd0_header[1] = SWAP32((h<<16) | w);
	osd0_header[2] = 0;
	osd0_header[3] = 0;
	osd0_header[4] = 0;
	//osd0_header[5] = 0x2C010100;
	osd0_header[5] = SWAP32(0x00010000 | w);
	osd0_header[6] = 0xE0FFFFFF;
	//osd0_header[7] = 0x10E00B;
	osd0_header[7] = SWAP32(bufbase);

	//G213
	reg213[0]  = 0x04A0;
	reg213[1]  = 0x0001;
	reg213[2]  = ((UINT32)osd0_header) & 0xFFFF;
	reg213[3]  = (((UINT32)osd0_header) & 0xFFFF0000) >> 16;
	reg213[4]  = 0x000E;
	reg213[5]  = gDMIX_V_BP + 1;
	reg213[6]  = 0x0000;
	reg213[7]  = 0x0000;
	reg213[8]  = 0x8010;
	reg213[9]  = 0x0000;
	reg213[10]  = 0x0200;
	reg213[11]  = 0x0200;
	reg213[12]  = 0x0007;
	reg213[13]  = 0x0002;
	reg213[14] = 0x7ADF;
	reg213[15] = 0x4001;
	reg213[16] = 0x0000;
	//reg213[17] = 0x012C;
	reg213[17] = w;
	reg213[18] = 0x0000;
	//reg213[19] = 0x012C;
	reg213[19] = h;
	reg213[20] = 0xFF70;
	reg213[21] = 0x0000;
	reg213[22] = 0x0000;
	reg213[23] = 0x0000;
	reg213[24] = 0x0000;
	reg213[25] = 0x0000;
	reg213[26] = 0x0000;
	reg213[27] = 0x0000;
	reg213[28] = 0x0000;
	reg213[29] = 0x0000;
	reg213[30] = 0x0012;
	reg213[31] = 0x0016;

	//G214
	reg214[0]  = 0x04A0;
	reg214[1]  = 0x0001;
	reg214[2]  = ((UINT32)osd0_header) & 0xFFFF;
	reg214[3]  = (((UINT32)osd0_header) & 0xFFFF0000) >> 16;
	reg214[4]  = 0x000E;
	reg214[5]  = 0x0025;
	reg214[6]  = 0x0000;
	reg214[7]  = 0x0000;
	reg214[8]  = 0x8010;
	reg214[9]  = 0x0000;
	reg214[10]  = 0x0200;
	reg214[11]  = 0x0200;
	reg214[12]  = 0x0001;
	reg214[13]  = 0x0002;
	reg214[14] = 0x7ADF;
	reg214[15] = 0x4001;
	reg214[16]  = 0x0000;
	//reg214[17]  = 0x012C;
	reg214[17]  = w;
	reg214[18] = 0x0000;
	//reg214[19] = 0x012C;
	reg214[19] = h;
	reg214[20] = 0xFF70;
	reg214[21] = 0x0000;
	reg214[22] = 0x0000;
	reg214[23] = 0x0000;
	reg214[24] = 0x0000;
	reg214[25] = 0x0000;
	reg214[26] = 0x0000;
	reg214[27] = 0x0000;
	reg214[28] = 0x0000;
	reg214[29] = 0x0000;
	reg214[30] = 0x000B;
	reg214[31] = 0x0017;

	return;
}

#else

void OSD(UINT32 id, UINT32 w, UINT32 h, UINT32 osd_header)
{
	UINT32 *reg = (UINT32 *)(REG_START_8388 + (213+id)*128);

	//G213
	reg[0] = 0x04A0;
	reg[2] = ((UINT32)osd_header) & 0xFFFF;
	reg[3] = (((UINT32)osd_header) & 0xFFFF0000) >> 16;
	reg[4] = 0x000E;
	reg[5] = gDMIX_V_BP + 1;
	reg[6] = 0x0000;
	reg[7] = 0x0000;
	reg[8] = 0x8040;
	reg[9] = 0x0000;
	reg[10] = 0x0200;
	reg[11] = 0x0200;
	reg[12] = 0x0007;
	reg[13] = 0x0002;
	reg[14] = 0x7ADF;
	reg[15] = 0x4001;
	reg[16] = 0x0000;
	reg[17] = w;
	reg[18] = 0x0000;
	reg[19] = h;
	reg[20] = 0xFF70;
	reg[21] = 0x0000;
	reg[22] = 0x0000;
	reg[23] = 0x0000;
	reg[24] = 0x0000;
	reg[25] = 0x0000;
	reg[26] = 0x0000;
	reg[27] = 0x0000;
	reg[28] = 0x0000;
	reg[29] = 0x0000;
	reg[30] = 0x0012;
	reg[31] = 0x0016;

	reg[1] = 0x0001;	//last enable it.

	return;
}

#endif
void VENC(UINT32 IsCVBSPal)
{
	//VENC
	unsigned int *reg209 = (unsigned int *)(REG_START_8388 + 209*128);
	unsigned int *reg210 = (unsigned int *)(REG_START_8388 + 210*128);
	unsigned int *reg211 = (unsigned int *)(REG_START_8388 + 211*128);
	//G209
	if (IsCVBSPal)
	{
		reg209[0] = 0xB083;
		reg209[1] = 0xD000;
		reg209[2] = 0x1080;
	}
	else
	{
		reg209[0] = 0x1083;
		reg209[1] = 0xAC00;
		reg209[2] = 0x10F0;
	}
	reg209[3] = 0x2600;
	reg209[4] = 0x0040;//0x103f;
	reg209[5] = 0x000C;
	reg209[6] = 0x8ACB;
	reg209[7] = 0x2A09;
	reg209[8] = 0x0000;
	reg209[9] = 0x0000;
	reg209[10] = 0x0000;
	reg209[11] = 0x0000;
	reg209[12] = 0x0000;
	reg209[13] = 0x0000;
	reg209[14] = 0x0000;

	if (IsCVBSPal)
		reg209[15] = 0x0007;
	else
		reg209[15] = 0x0001;

	reg209[16] = 0x0018;
	reg209[17] = 0x1710;
	reg209[18] = 0x0000;
	reg209[19] = 0x0000;
	reg209[20] = 0x0000;
	reg209[21] = 0x4512;
	reg209[22] = 0x2A87;
	reg209[23] = 0x3D50;
	reg209[24] = 0x1FF0;
	reg209[25] = 0x154F;
	reg209[26] = 0xE7E6;
	reg209[27] = 0x0402;
	reg209[28] = 0x7260;
	reg209[29] = 0x0000;
	reg209[30] = 0x0000;
	reg209[31] = 0x0000;

	//G210
	reg210[0] = 0x0000;
	reg210[1] = 0x0000;
	reg210[2] = 0x0000;
	reg210[3] = 0x0000;
	reg210[4] = 0x0000;
	reg210[5] = 0x0018;
	reg210[6] = 0x005C;
	reg210[7] = 0x0018;
	reg210[8] = 0x005C;
	reg210[9] = 0x0018;
	reg210[10] = 0x005C;
	reg210[11] = 0x0001;
	reg210[12] = 0x0004;
	reg210[13] = 0x0000;
	reg210[14] = 0x0000;
	reg210[15] = 0x0274;
	reg210[16] = 0x0274;
	reg210[17] = 0x001B;
	reg210[18] = 0x0420;
	reg210[19] = 0x0320;
	reg210[20] = 0x0095;
	reg210[21] = 0x005A;
	reg210[22] = 0x0080;
	reg210[23] = 0x0001;
	reg210[24] = 0x0000;
	reg210[25] = 0x0000;
	//reg210[26] = 0x0E60;
	reg210[27] = 0x00BF;
	reg210[28] = 0x0000;
	reg210[29] = 0x0000;
	reg210[30] = 0x0000;
	reg210[31] = 0x0000;

	//G211
	if (IsCVBSPal)
		reg211[0] = 0x06C8;
	else
		reg211[0] = 0x0608;

	reg211[1] = 0x0000;
	reg211[2] = 0x0000;
	reg211[3] = 0x5060;
	reg211[4] = 0xB0A0;
	reg211[5] = 0x0000;
	reg211[6] = 0x0000;
	if (IsCVBSPal)
	{
		reg211[7] = 0x0017;
		reg211[8] = 0x0150;
		reg211[9] = 0x002B;
	}
	else
	{
		reg211[7] = 0x0011;
		reg211[8] = 0x0118;
		reg211[9] = 0x0023;
	}
	reg211[10] = 0x2A28;

	if (IsCVBSPal)
		reg211[11] = 0xC0E8; //8388 , set by Emil.Yeh, 0xC0E4->0xC0E8
	else
		reg211[11] = 0x80E8;//8388 , set by Emil.Yeh, 0x80E4->0x80E8

	reg211[12] = 0x8100;
	reg211[13] = 0xEC00;
	reg211[14] = 0xF010;
	reg211[15] = 0x0000;
	reg211[16] = 0x0000;
	reg211[17] = 0x0000;
	reg211[18] = 0x0000;
	reg211[19] = 0x0056;
	reg211[20] = 0x0080;
	reg211[21] = 0x0800;
	reg211[22] = 0x026D;
	reg211[23] = 0x0001;
	reg211[24] = 0x0001;
	reg211[25] = 0x0F17;
	reg211[26] = 0x0000;
	reg211[27] = 0x0000;
	reg211[28] = 0x0100;
	reg211[29] = 0x00D5;
	reg211[30] = 0x0180;
	reg211[31] = 0x00D5;

	return;
}

int DRV_Uboot_DMIX_Luma_Adjust_Set(DRV_DMIX_Luma_Adj_u* LumaAdjInfo)
{
	// Y = (X - 127) * A + 127 + B
	// => Y = A * X + C
	// => C = B - 127 * A + 127
	// => A is contrast, B is brightness
	int i = 0;
	int x[5] = {0, 50, 100, 200, 255};
	int y[5] = {0, 50, 100, 200, 255};
	UINT16 slope[4] = {256, 256, 256, 256};
	int brightness_indx = 0, contrast_indx = 0;
	int brightness = 0;
	double contrast = 0.00;
	int brightness_min = DIMIX_LUMA_OFFSET_MIN, brightness_max = DIMIX_LUMA_OFFSET_MAX;
	double contrast_min = DIMIX_LUMA_SLOPE_MIN, contrast_max = DIMIX_LUMA_SLOPE_MAX;
	unsigned int *reg208 = (UINT32 *)0x9c006800;

	// calculate A and C
	brightness_indx = (LumaAdjInfo->brightness > 24) ? 24 : LumaAdjInfo->brightness;
	contrast_indx = (LumaAdjInfo->contrast > 24) ? 24 : LumaAdjInfo->contrast;

	if (brightness_indx <= 12)
	{
		brightness = (double)(0 - brightness_min) * (double)brightness_indx / 12 + brightness_min;
	}
	else
	{
		brightness = (double)(brightness_max - 0) * (double)(brightness_indx - 12) / 12;
	}

	if (contrast_indx <= 12)
	{
		contrast = (double)(1 - contrast_min) * (double)contrast_indx / 12 + contrast_min;
	}
	else
	{
		contrast = (double)(contrast_max - 1.00) * (double)(contrast_indx - 12) / 12 + 1.00;
	}

	brightness += (127 - (double)127 * contrast);

	// calculate point
	y[0] = contrast * (double) x[0] + brightness;
	y[4] = contrast * (double) x[4] + brightness;

	if (y[0] > 0)
	{
		x[1] = x[0];
		y[1] = y[0];
	}
	else if (y[0] == 0)
	{
		x[1] = 50;
		y[1] = contrast * (double) x[1] + brightness;
	}
	else
	{
		x[1] = (double)((-1) * brightness) / contrast;
		y[1] = 0;
	}

	if (y[4] > 255)
	{
		y[3] = 255;
		x[3] = (double)(y[3] - brightness) / contrast;
	}
	else
	{
		x[3] = 200;
		y[3] = contrast * (double) x[3] + brightness;
	}

	x[2] = (x[1] + x[3]) / 2;
	y[2] = contrast * (double) x[2] + brightness;

	for (i = 0; i < 5; i++)
	{
		if (y[i] < 0)
			y[i] = 0;
		else if (y[i] > 255)
			y[i] = 255;
	}

	// calculate slope
	for (i = 0; i < 4; i++)
	{
		if (x[i+1] == x[i])
			slope[i] = 256;
		else
			slope[i] = ((double)(y[i+1] - y[i]) / (double)(x[i+1] - x[i])) * 256;
	}

	// config reg.
	reg208[4] = (reg208[4] & (~0x0002)) | ((LumaAdjInfo->enable & 0x1) << 1);
	reg208[5] = ((y[1] & 0xFF) << 8) | (x[1] & 0xFF);
	reg208[6] = ((y[2] & 0xFF) << 8) | (x[2] & 0xFF);
	reg208[7] = ((y[3] & 0xFF) << 8) | (x[3] & 0xFF);
	reg208[8] = (slope[0] & 0x07FF);
	reg208[9] = (slope[1] & 0x07FF);
	reg208[10] = (slope[2] & 0x07FF);
	reg208[11] = (slope[3] & 0x07FF);

	return 0;
}

int DRV_Uboot_DMIX_Chroma_Adjust_Set(DRV_DMIX_Chroma_Adj_u* ChromaAdjInfo)
{
	// Chroma Adjust work on L1-L6, not support on LF
	unsigned int *reg208 = (UINT32 *)0x9c006800;

	reg208[4] = (reg208[4] & (~0x1)) | (ChromaAdjInfo->enable & 0x1);
	reg208[12] = (ChromaAdjInfo->satcos & 0x3FF);
	reg208[13] = (ChromaAdjInfo->satsin & 0x3FF);

	return 0;
}

void IMG_Set(DRV_VPP_Fav_Bin_Package_u *package, UINT8 enable)
{
	IMG_REG *IMG1_Reg = (IMG_REG*)0x9c006300;
	BRI_REG *BRI1_Reg = (BRI_REG*)0x9c009900;
	UINT32 i;

	if (package == NULL)
	{
		PQ_DEBUG("FCA package pointer error\n");
	}
	else
	{	//Firstly we set IMG1
		IMG1_Reg->img_sram_ctrl = DRV_VPP_IMG_SRAM_WRITE;
		IMG1_Reg->img_sram_addr = 0;
		BRI1_Reg->img_bri_sram_ctrl = DRV_VPP_IMG_SRAM_WRITE;
		BRI1_Reg->img_bri_sram_addr = 0;

		for(i = 0; i < 256; i++)
		{
			IMG1_Reg->img_sram_write_data = package->ColorInfo[i].satsin;
			IMG1_Reg->img_sram_write_data = package->ColorInfo[i].satcos;
			BRI1_Reg->img_bri_sram_write_data = package->ColorInfo[i].brightness;
			PQ_DEBUG("[%d](0x%x, 0x%x, 0x%x)\n", i, package->ColorInfo[i].satsin,
														package->ColorInfo[i].satcos,
														package->ColorInfo[i].brightness);
		}
		IMG1_Reg->img_sram_ctrl = DRV_VPP_IMG_SRAM_WROFF;
		BRI1_Reg->img_bri_sram_ctrl = DRV_VPP_IMG_SRAM_WROFF;

		if(enable)
		{
			IMG1_Reg->img_config |= 0x2;	// IMG CADJ on
			BRI1_Reg->img_bri_en |= 0x1;	// IMG BRI on
		}
		else
		{
			IMG1_Reg->img_config &= (~0x2);
			BRI1_Reg->img_bri_en &= (~0x1);
		}
	}
}

void VPP_Set_PeakingEnable_PQ(void)
{
	IMG_REG *IMG1_Reg = (IMG_REG*)0x9c006300;

	unsigned int img_pkti_ctrl_temp;
	img_pkti_ctrl_temp = IMG1_Reg->img_pkti_ctrl;
	//Clear Pre_LP bit 14, HP bit 13, BP bit 11, EP bit 10, Post_LP bit 1
	//img_pkti_ctrl_temp &= 0xfffffdff;
	img_pkti_ctrl_temp |= 0x100;
	IMG1_Reg->img_pkti_ctrl = img_pkti_ctrl_temp;
	//INFO("PeakingEnable==0x%x\n", pVppReg->img[enable_info->id].img_pkti_ctrl);

	return;
}

void VPP_Set_DLTIEnable_PQ(void)
{
	IMG_REG *IMG1_Reg = (IMG_REG*)0x9c006300;
	unsigned int img_plti_ctrl_temp;

	img_plti_ctrl_temp = IMG1_Reg->img_pkti_ctrl;
	img_plti_ctrl_temp |= 0x1;
	IMG1_Reg->img_pkti_ctrl = img_plti_ctrl_temp;
}

void VPP_Set_DCTIEnable_PQ(void)
{
	DCTI_REG *DCTI1_Reg = (DCTI_REG*)0x9c009600;
	DCTI1_Reg->dcti_config1.DCTI_EN = 1;
}

void VPP_Set_Peaking(DRV_VPP_Peaking_Info_u *peaking_info)
{
	IMG_REG *IMG1_Reg = (IMG_REG*)0x9c006300;
	//Gain and Negative
	IMG1_Reg->img_pkti_lpk_config.pkti_lpk_gain_neg = peaking_info->pkti_lpk_gain_neg;
	IMG1_Reg->img_pkti_lpk_gain.pkti_lpk_gain_hp = peaking_info->pkti_lpk_gain_hp;
	IMG1_Reg->img_pkti_lpk_gain.pkti_lpk_gain_bp = peaking_info->pkti_lpk_gain_bp;
	IMG1_Reg->img_pkti_lpk_gain.pkti_lpk_gain_ep = peaking_info->pkti_lpk_gain_ep;

	//Slope information
	IMG1_Reg->img_pkti_lpk_lut_1.pkti_lpk_lut_mp1 = peaking_info->pkti_lpk_lut_mp1;
	IMG1_Reg->img_pkti_lpk_lut_2.pkti_lpk_lut_mp2 = peaking_info->pkti_lpk_lut_mp2;
	IMG1_Reg->img_pkti_lpk_lut_3.pkti_lpk_lut_xp0 = peaking_info->pkti_lpk_lut_xp0;
	IMG1_Reg->img_pkti_lpk_lut_3.pkti_lpk_lut_xp1 = peaking_info->pkti_lpk_lut_xp1;
	IMG1_Reg->img_pkti_lpk_lut_4.pkti_lpk_lut_xp2 = peaking_info->pkti_lpk_lut_xp2;
	IMG1_Reg->img_pkti_lpk_lut_4.pkti_lpk_lut_xp3 = peaking_info->pkti_lpk_lut_xp3;
	IMG1_Reg->img_pkti_lpk_lut_5.pkti_lpk_lut_yp0 = peaking_info->pkti_lpk_lut_yp0;
	IMG1_Reg->img_pkti_lpk_lut_5.pkti_lpk_lut_yp1 = peaking_info->pkti_lpk_lut_yp1;
	IMG1_Reg->img_pkti_lpk_lut_6.pkti_lpk_lut_yp2 = peaking_info->pkti_lpk_lut_yp2;

	//Filter setting. HP filter, BP filter, EP filter. Pre/Post filter
	unsigned int img_pkti_ctrl_temp;
	img_pkti_ctrl_temp = IMG1_Reg->img_pkti_ctrl;
	//Clear Pre_LP bit 14, HP bit 13, BP bit 11, EP bit 10, Post_LP bit 1
	img_pkti_ctrl_temp &= 0xffff91fd;
	//Start tp set filter
	if(peaking_info->hp_filter)
	{
		img_pkti_ctrl_temp |= 0x00002000;
	}
	if(peaking_info->bp_filter)
	{
		img_pkti_ctrl_temp |= 0x00000800;
	}
	if(peaking_info->ep_filter)
	{
		img_pkti_ctrl_temp |= 0x00000400;
	}
	if(peaking_info->pre_lp_filter)
	{
		img_pkti_ctrl_temp |= 0x00004000;
	}
	if(peaking_info->post_lp_filter)
	{
		img_pkti_ctrl_temp |= 0x200;
	}
	IMG1_Reg->img_pkti_ctrl = img_pkti_ctrl_temp;


	//Coring
	IMG1_Reg->img_pkti_lpk_lut_6.pkti_lpk_coring = peaking_info->pkti_lpk_coring;

	//Threshold
	IMG1_Reg->img_pkti_lpk_ovsht_1.pkti_lpk_hi_coring = peaking_info->pkti_lpk_threshold;

	return;
}

void VPP_Set_Transient(DRV_VPP_Transient_Info_u *transient_info)
{
	IMG_REG *IMG1_Reg = (IMG_REG*)0x9c006300;
	//Gain
	IMG1_Reg->img_pkti_lti_gain.pkti_lti_gain_hp = transient_info->pkti_lti_gain_hp;
	IMG1_Reg->img_pkti_lti_gain.pkti_lti_gain_bp = transient_info->pkti_lti_gain_bp;
	IMG1_Reg->img_pkti_lti_gain.pkti_lti_gain_ep = transient_info->pkti_lti_gain_ep;

	//Slope
	IMG1_Reg->img_pkti_lti_lut_1.pkti_lti_lut_mp1 = transient_info->pkti_lti_lut_mp1;
	IMG1_Reg->img_pkti_lti_lut_2.pkti_lti_lut_mp2 = transient_info->pkti_lti_lut_mp2;
	IMG1_Reg->img_pkti_lti_lut_3.pkti_lti_lut_xp0 = transient_info->pkti_lti_lut_xp0;
	IMG1_Reg->img_pkti_lti_lut_3.pkti_lti_lut_xp1 = transient_info->pkti_lti_lut_xp1;
	IMG1_Reg->img_pkti_lti_lut_4.pkti_lti_lut_xp2 = transient_info->pkti_lti_lut_xp2;
	IMG1_Reg->img_pkti_lti_lut_4.pkti_lti_lut_xp3 = transient_info->pkti_lti_lut_xp3;
	IMG1_Reg->img_pkti_lti_lut_5.pkti_lti_lut_yp0 = transient_info->pkti_lti_lut_yp0;
	IMG1_Reg->img_pkti_lti_lut_5.pkti_lti_lut_yp1 = transient_info->pkti_lti_lut_yp1;
	IMG1_Reg->img_pkti_lti_lut_6.pkti_lti_lut_yp2 = transient_info->pkti_lti_lut_yp2;

	//Filter setting. HP filter, BP filter, EP filter. Pre/Post filter
	unsigned int img_plti_ctrl_temp;
	img_plti_ctrl_temp = IMG1_Reg->img_pkti_ctrl;
	//Clear BP bit 3, EP bit 2, Stepness filter bit 5:4
	img_plti_ctrl_temp &= 0xffffffc1;

	//Start tp set filter
	if(transient_info->bp_filter)
	{
		img_plti_ctrl_temp |= 0x8;
	}
	if(transient_info->ep_filter)
	{
		img_plti_ctrl_temp |= 0x4;
	}
	if(transient_info->stp_filter)
	{
		img_plti_ctrl_temp |= 0x10;
	}
	if(transient_info->post_lp_filter)
	{
		img_plti_ctrl_temp |= 0x2;
	}

	IMG1_Reg->img_pkti_ctrl = img_plti_ctrl_temp;

	//Coring
	IMG1_Reg->img_pkti_lti_lut_6.pkti_lti_coring = transient_info->pkti_lti_coring;

	//Threshold
	IMG1_Reg->img_pkti_lti_ovsht_1.pkti_lti_hi_coring = transient_info->pkti_lti_threshold;

	return;
}

void VPP_Set_DeOvershoot(DRV_VPP_DeOvershoot_u *deovershoot)
{
	IMG_REG *IMG1_Reg = (IMG_REG*)0x9c006300;
	//Peaking

	//Gain
	IMG1_Reg->img_pkti_lpk_ovsht_1.pkti_lpk_ovrsht_gain_max = deovershoot->pkti_lpk_ovrsht_gain_max;
	IMG1_Reg->img_pkti_lpk_ovsht_1.pkti_lpk_ovrsht_gain_min = deovershoot->pkti_lpk_ovrsht_gain_min;

	//Clip
	IMG1_Reg->img_pkti_lpk_ovsht_2.pkti_lpk_over_max = deovershoot->pkti_lpk_clip_max;
	IMG1_Reg->img_pkti_lpk_ovsht_2.pkti_lpk_over_min = deovershoot->pkti_lpk_clip_min;

	//Transient

	//Gain
	IMG1_Reg->img_pkti_lti_ovsht_1.pkti_lti_ovrsht_gain_max = deovershoot->pkti_lti_ovrsht_gain_max;
	IMG1_Reg->img_pkti_lti_ovsht_1.pkti_lti_ovrsht_gain_min = deovershoot->pkti_lti_ovrsht_gain_min;

	//Clip
	IMG1_Reg->img_pkti_lti_ovsht_2.pkti_lti_over_max = deovershoot->pkti_lti_clip_max;
	IMG1_Reg->img_pkti_lti_ovsht_2.pkti_lti_over_min = deovershoot->pkti_lti_clip_min;

	return;
}

void VPP_Set_DCTI(DRV_VPP_DCTI_Info_t *dcti)
{
	DCTI_REG *DCTI1_Reg = (DCTI_REG*)0x9c009600;
	DCTI1_Reg->dcti_gain_weight = dcti->GainWeight;
}

static UINT8 check_if_lut_right(DRV_PQ_TNR_Lut_Param_t lut)
{
	int i = 0;
	UINT8 check = TRUE;

	for (i = 0; i < 4; i++)
	{

		if (lut.y_motion_threshold[i] >= lut.y_motion_threshold[i+1])
		{
			check = FALSE;
			diag_printf("Luma motion threshold param. is invalid\n");
			break;
		}

		if (lut.c_motion_threshold[i] >= lut.c_motion_threshold[i+1])
		{
			diag_printf("Chroma motion threshold param. is invalid\n");
			check = FALSE;
			break;
		}
	}

	return check;
}

static int set_lut_param(UINT8 chl, DRV_PQ_TNR_Lut_Param_t lut)
{
	int i = 0, err = 0;
	UINT32 *p1 , *p2, *p3;
	UINT8 slope_y = 0, slope_c = 0;

	if (check_if_lut_right(lut))
	{
		p1 = (UINT32*) &g_tnr_reg[chl]->grp0_02;
		p2 = (UINT32*) &g_tnr_reg[chl]->grp0_11;
		p3 = (UINT32*) &g_tnr_reg[chl]->grp0_07;
		for (i = 0; i < 5; i++)
		{
			*(p1 + i) = (lut.y_motion_threshold[i] & 0xFF) |
				((lut.c_motion_threshold[i] & 0xFF) << 8);

			*(p2 + i) = (lut.y_blending_factor[i] & 0x1F) |
				((lut.c_blending_factor[i] & 0x1F) << 8);
		}

		if (lut.motion_lut_type == TNR_MOTION_LUT_LINEAR_TYPE)
		{
			for (i = 0; i < 4; i++)
			{
				slope_y = (UINT8)((float)256 / (float)(lut.y_motion_threshold[i+1] - lut.y_motion_threshold[i]) + 0.5);
				slope_c = (UINT8)((float)256 / (float)(lut.c_motion_threshold[i+1] - lut.c_motion_threshold[i]) + 0.5);
				*(p3 + i) = slope_y + ((UINT16)slope_c << 8);
			}

			g_tnr_reg[chl]->weight_new = 1;
		}
		else
		{
			g_tnr_reg[chl]->weight_new = 0;
		}
	}
	else
	{
		err = -1;
	}

	return err;
}

static int DRV_Uboot_TNR_Param_Set(UINT8 chl, DRV_VPP_TNR_Param_u *param)
{
	int err = 0;
	DRV_PQ_TNR_Lut_Param_t lut;

	// set param
	g_tnr_reg[chl]->clamp_diff = (param->clamp_method == TNR_TWO_THR_CLAMP) ? 1 : 0;
	g_tnr_reg[chl]->yc_motion_sep = (param->yc_motion_process == TNR_YC_MOTION_SEPERATED_PROC) ? 1 : 0;
	g_tnr_reg[chl]->diffmax_on = (param->yc_diff_cal == TNR_YC_DIFF_MAX_CAL) ? 1 : 0;
	g_tnr_reg[chl]->chroma_weight = param->chroma_weight & 0xF;
	g_tnr_reg[chl]->yclamp_value_th = param->y_output_clamp_thr[0];
	g_tnr_reg[chl]->yclamp_value_th2 = param->y_output_clamp_thr[1];
	g_tnr_reg[chl]->cclamp_value_th = param->c_output_clamp_thr[0];
	g_tnr_reg[chl]->cclamp_value_th2 = param->c_output_clamp_thr[1];

	lut.motion_lut_type = param->motion_lut_type;
	memcpy(&lut.y_motion_threshold[0], &param->y_motion_threshold[0], 5);
	memcpy(&lut.y_blending_factor[0], &param->y_blending_factor[0], 5);
	memcpy(&lut.c_motion_threshold[0], &param->c_motion_threshold[0], 5);
	memcpy(&lut.c_blending_factor[0], &param->c_blending_factor[0], 5);
	err = set_lut_param(chl, lut);
	return err;
}

int PQSet(UINT32 BuffAnchor, const stTCON_API_t *TCON)
{
	//Size information
	UINT32 Scenario_Size;
	UINT32 ScenNum;
	UINT32 ScenSelect;
//	UINT8 *PQBuff = (UINT8 *)0xab270e0; // availabe address
	//UINT8 *PQBuff = (UINT8 *)0x2391000;
	UINT8 *PQBuff = (UINT8 *)BuffAnchor;
	UINT8* CurrentPtr = NULL;
	PQ_Bin_BinPackageClient_u *PQPackage = NULL;
	PQ_Bin_MainHeader_u *MainHeader = NULL;
	unsigned int ic_stamp = 0;
	IMG_REG *IMG1_Reg = (IMG_REG*)0x9c006300;

	//Used to stored the variable sent to adjust function
	DRV_DMIX_Luma_Adj_u LumaAdjInfo;
	DRV_DMIX_Chroma_Adj_u ChromaAdjInfo;
	char disp_cmd[256];
	#define PQ_PART "pq"
	UINT32 partionSize = 87040;

	memset(PQBuff, 0, sizeof(PQ_Bin_BinPackageClient_u));

	sprintf(disp_cmd, "nand read 0x%x %s 0x%x", (UINT32)PQBuff, PQ_PART, partionSize);

	if(run_command(disp_cmd, 0)){
		printf("[PQ]Nand read fail\n");
		return -1;
	}

	//Assign main header to the approatiate place. We will use this pointer to grab main header information
	MainHeader = (PQ_Bin_MainHeader_u* )PQBuff;

	//check header token
	if (!((MainHeader->token[0] == 0xfe) && (MainHeader->token[1] == 'I')
		&& (MainHeader->token[2] == 'M') && (MainHeader->token[3] == 'G')))
	{
		printf("PQ header error\n");
		return -1;
	}
	else
	{
		printf("Parsing header successfully\n");
	}

	//add for pq.bin version detect

	Scenario_Size = sizeof(PQ_Bin_BinPackageClient_u);

	ScenNum = MainHeader->Len/Scenario_Size;
	ScenSelect = MainHeader->ScenarioSelect;

	PQ_DEBUG("Buffer anchor=0x%x\n", BuffAnchor);

	PQ_DEBUG("Scenario size:==%d==\n", Scenario_Size);
	PQ_DEBUG("Scenario number:==0x%x==\n", ScenNum);
	PQ_DEBUG("Scenario select:==0x%x==\n", ScenSelect);

	if(ScenSelect > ScenNum){
		printf("Error Scenario number:==%d==\n", ScenSelect);
		//Default use scenario number 1
		ScenSelect = 0;
	}

	//Skip MainHeader and Go to the scenario u want
	CurrentPtr = PQBuff;
	//Main header size:16
	CurrentPtr = CurrentPtr + 16 + Scenario_Size * ScenSelect;


	PQPackage = (PQ_Bin_BinPackageClient_u* )CurrentPtr;


	ChromaAdjInfo.OutId = 1;
	ChromaAdjInfo.enable = 1;
	ChromaAdjInfo.satsin = PQPackage->LumaHueInfo.satsin;
	ChromaAdjInfo.satcos = PQPackage->LumaHueInfo.satcos;

	DRV_Uboot_DMIX_Chroma_Adjust_Set(&ChromaAdjInfo);

	LumaAdjInfo.enable = 1;
	LumaAdjInfo.OutId = 1;
	LumaAdjInfo.brightness = PQPackage->LumaHueInfo.brightness;
	LumaAdjInfo.contrast = PQPackage->LumaHueInfo.contrast;

	DRV_Uboot_DMIX_Luma_Adjust_Set(&LumaAdjInfo);

	PQ_DEBUG("Out ID :==%d==\n", PQPackage->LumaHueInfo.OutId);
	PQ_DEBUG("Satsin :==%d==\n", PQPackage->LumaHueInfo.satsin);
	PQ_DEBUG("Satcos :==%d==\n", PQPackage->LumaHueInfo.satcos);
	PQ_DEBUG("Brightness :==%d==\n", PQPackage->LumaHueInfo.brightness);
	PQ_DEBUG("Contrast :==%d==\n", PQPackage->LumaHueInfo.contrast);
	PQ_DEBUG("Huet :==%d==\n", PQPackage->LumaHueInfo.hue);
	PQ_DEBUG("Saturation :==%d==\n", PQPackage->LumaHueInfo.saturation);

#ifdef GAMMA_OPEN
	TCON->Set_PAT_Gamma((char*)&PQPackage->GammaInfo, sizeof(TCON_PAT_GAMMA_t));
#endif

	DRV_Uboot_TNR_Param_Set(VPP_MAIN_PATH, &PQPackage->TNR_Info);

	ic_stamp = (*((volatile unsigned int *)(REG_START_8388)));	// G0.0
	if(ic_stamp == 0x631) // ic ver.B
	{
		//Connect G2D and IMG1
		IMG1_Reg->img_config |= 0x40;
		IMG_Set(&PQPackage->favoriteUI, PQPackage->FavHeaderUI.Enable);
		VPP_Set_Peaking(&PQPackage->PeakingInfo_UI);
		VPP_Set_Transient(&PQPackage->TransientInfo_UI);
		VPP_Set_DeOvershoot(&PQPackage->DeOverShootInfo_UI);
		VPP_Set_DCTI(&PQPackage->DCTI_Info_UI);

		if(PQPackage->PeakingHeader_UI.Enable)
			VPP_Set_PeakingEnable_PQ();

		if(PQPackage->TransientHeader_UI.Enable)
			VPP_Set_DLTIEnable_PQ();

		if(PQPackage->DCTI_Header_UI.Enable)
			VPP_Set_DCTIEnable_PQ();
	}

	return 0;
}

void update_OSD(UINT32 *DispAddr, UINT32 *ImgAddr)
{
	UINT32 *reg208 = (UINT32 *)0x9c006800;
	UINT16 H = reg208[16];
	UINT16 V = reg208[15];
	UINT32 *osd0_header = DispAddr;
	DRV_Logo_header_t* imgheader = 0;
	UINT32 imgbase = 0;
	UINT32 w, h;
	UINT32 IsCVBSPal = 0; // TODO get default value

	#ifndef CONFIG_SYS_ARM_CACHE_WRITETHROUGH
	invalidate_dcache_range((UINT32)ImgAddr,(UINT32)(ImgAddr+8));
	#endif
	imgheader = (DRV_Logo_header_t *)ImgAddr;
	imgbase = (UINT32)imgheader + sizeof(DRV_Logo_header_t);
	//printf("imgheader 0x%x imgbase 0x%x\n",(UINT32)imgheader,(UINT32)imgbase);

	if (imgbase & 0x1f)
		printf("\n\n\nOSD img addr must 32 align\n\n\n");

	w = imgheader->width;
	h = imgheader->height;
	//printf("\n\n\nInput w=%d h=%d imgheader=%x imgbase=%x\n\n\n", imgheader->width, imgheader->height, (UINT32)imgheader, (UINT32)imgbase);

	osd0_header[0] = 0x0010000E;
	//osd0_header[1] = 0x2C012C01;
	osd0_header[1] = SWAP32((h<<16) | w);
	osd0_header[2] = 0;
	osd0_header[3] = 0;
	osd0_header[4] = 0;
	//osd0_header[5] = 0x2C010100;
	osd0_header[5] = SWAP32(0x00010000 | w);
	osd0_header[6] = 0xE0FFFFFF;
	//osd0_header[7] = 0x10E00B;
	if ((w == 0) && (h == 0))
	{
		osd0_header[7] = 0xE0FFFFFF;
		w = H;
		h = V;
	}
	else
		osd0_header[7] = SWAP32(imgbase);
	#ifndef CONFIG_SYS_ARM_CACHE_WRITETHROUGH
	flush_dcache_range((UINT32)osd0_header, (UINT32)(osd0_header+(CONFIG_SP_OSD_HEADER_SIZE/4)));	
	#endif
	
	GSCL(0, imgheader->x, imgheader->y, w, h, H, V, imgheader->location_flag);
	H = 720;
	if (IsCVBSPal)
		V = 576;
	else
		V = 480;
	GSCL(1, imgheader->x, imgheader->y, w, h, H, V, imgheader->location_flag);

	OSD(0, w, h, (UINT32)osd0_header);
	OSD(1, w, h, (UINT32)osd0_header);
}

UINT32 OSD_Header_Addr(void)
{
	return (SP_IS_ISPBOOT()||get_part_updt_flag())?CONFIG_SP_OSD_HEADER_ADDR:CONFIG_SP_MFD_ADDR;
}

UINT32 OSD_Buf_BaseAddr(void)
{
	return (SP_IS_ISPBOOT()||get_part_updt_flag())?CONFIG_SP_OSD_LOGO_ADDR:(CONFIG_SP_MFD_ADDR+CONFIG_SP_OSD_HEADER_SIZE);
}

void OSD_Get_Free_BufAddr(UINT32 *id, UINT32 *addr)
{
	if (gOSD_BUF_ID == 0)
		*id = 1;
	else
		*id = 0;
	*addr = OSD_Buf_BaseAddr() + CONFIG_SP_OSD_LOGO_MAX_SIZE*(*id);
}

void OSD_Get_Disp_BufAddr(UINT32 *addr)
{
	*addr = OSD_Buf_BaseAddr() + CONFIG_SP_OSD_LOGO_MAX_SIZE*(gOSD_BUF_ID);
}

void OSD_Set_VisableBuf(UINT32 id)
{
	update_OSD((UINT32 *)OSD_Header_Addr(), (UINT32 *)(OSD_Buf_BaseAddr() + CONFIG_SP_OSD_LOGO_MAX_SIZE*id));
	gOSD_BUF_ID = id;
}

UINT32 OSD_Get_Buf_width(void)
{
	UINT32 *osd_header = (UINT32 *)OSD_Header_Addr();
	return SWAP32(osd_header[1]) & 0xffff;
}

int Display_init(int ISP)
{
	const stTCON_API_t *TCON = Hal_Get_TCON();
	UINT16 htotal = 0;
	UINT16 vtotal = 0;
	UINT16 H = 0;
	UINT16 V = 0;
	UINT32 IsCVBSPal = 0; // TODO get default value

	if (TCON)
	{
		TCON->GetTCON_HV_Area(&htotal, &vtotal, &H, &V);
		TCON->SetPANEL_FPS(eNTSC_59_94);
		TCON->SetTCON_PLL_cfg();
		TCON->GetTCON_NTSC_HVTotal(&htotal, &vtotal);
		TCON->GetDMIX_V_BP(&gDMIX_V_BP);
	}

	SysInit();
	TGEN(H, V, htotal, vtotal);
	DMIX(H, V);
	VENC(IsCVBSPal);
#if 0
	GSCL(gUI_width, gUI_height, H, V, IsCVBSPal);
	OSD(gUI_width, gUI_height, ISP, DispAddr);
#endif

	if (!ISP)
	{
		//We don't need pq in ISP mode
		UINT32 pq_offset = 1024 + 1024 + PAGEALIGN(H*V*4)*2 + 65536; //Because this section is used for boot logo
		UINT32 buffAnchorPQ = CONFIG_SP_MFD_ADDR + pq_offset;
		//printf("PQ buffer anchor:=0x%x\n", buffAnchorPQ);
		PQSet(buffAnchorPQ, TCON);
	}
	return 0;
}

