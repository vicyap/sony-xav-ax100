
/********************************************************************************
*                                                            Include File                                                                   *
*********************************************************************************/
#include <common.h>
#define UBOOT_TCON
#include "drivers/tcon/hal_SPHE8388_tcon.h"
#undef UBOOT_TCON
#include "reverse.h"
#include "sp_vendor_data.h"
#include "sp_config.h"

/********************************************************************************
*                                                            Static variables                                                              *
*********************************************************************************/
static UINT32 UI_width = 0;
static UINT32 UI_height = 0;


/********************************************************************************
*                                                            Global variables                                                              *
*********************************************************************************/


/********************************************************************************
*                                                            Macro Function                                                              *
*********************************************************************************/
#define _MAKE_CULT(R, G, B) (0xFF | (R)<<8) | ((G)<<16) | ((B)<<24)
#define _MAKE_CULT_ARGB(A, R, G, B) (A | (R)<<8) | ((G)<<16) | ((B)<<24)

#define GET_X(Y, M, B) ((Y-B)/M)


/********************************************************************************
*                                                            Data Types                                                                   *
*********************************************************************************/


/********************************************************************************
*                                                            Extern Reference                                                           *
*********************************************************************************/


/********************************************************************************
*                                                            Function Declarations                                                     *     
*********************************************************************************/
static void reverse_get_rightcord(UINT32* pRevCord, UINT32 dStPos)
{	
	int i = 0;
	UINT32 dRevCord[4]={550, 300, 700, 450};
	//printf("##### reverse_drawline rightcord dStPos=%d\n", dStPos);

	for(i=0; i<4; i++)
	{
		if(sp_vd_get_int((int)(dStPos+REV_CORD_LEN*i), REV_CORD_LEN, (int*)(pRevCord+i)) == SP_VD_RET_FAILED)
		{
			*(pRevCord+i)=dRevCord[i];
			printf("##### reverse_drawline rightcord get vender data fail, init default value\n");
		}
	}
}

static void reverse_get_leftcord(UINT32* pRevCord, UINT32 dStPos)
{	
	int i = 0;
	UINT32 dRevCord[4]={250, 300, 100, 450};
	//printf("##### reverse_drawline leftcord dStPos=%d\n", dStPos);

	for(i=0; i<4; i++)
	{
		if(sp_vd_get_int((int)(dStPos+REV_CORD_LEN*i), REV_CORD_LEN, (int*)(pRevCord+i)) == SP_VD_RET_FAILED)
		{
			*(pRevCord+i)=dRevCord[i];
			printf("##### reverse_drawline leftcord get vender data fail, init default value\n");
		}
	}
}

static Rev_Result_e reverse_check_cord(Rev_Coordinate_t* pRevCord)
{
	printf("##### reverse_drawline top_x=%d,  top_y=%d,  bottom_x=%d,  bottom_y=%d\n", pRevCord->top_x, pRevCord->top_y, pRevCord->bottom_x, pRevCord->bottom_y);
	
	if(pRevCord->top_x > UI_width)
		return REV_FAILED;
	if(pRevCord->top_y > UI_height)
		return REV_FAILED;
	if(pRevCord->bottom_x > UI_width)
		return REV_FAILED;
	if(pRevCord->bottom_y > UI_height)
		return REV_FAILED;
	
	return REV_SUCCESS;
}

static void reverse_init_palette(UINT32* pPalette)
{	
	pPalette[PC_TRANSPARENT] = _MAKE_CULT_ARGB(0, 0, 0, 0);
	pPalette[PC_RED] = _MAKE_CULT(0xFF, 0, 0);
	pPalette[PC_YELLOW] = _MAKE_CULT(0xFF, 0xFF, 0);
	pPalette[PC_GREEN] = _MAKE_CULT(0, 0xFF, 0);
}

static void draw_assist_line(unsigned int *region_addr, unsigned int buffer_pitch)
{
	unsigned char *px8 = (unsigned char*)region_addr;
	unsigned int x = 0, y = 0, i, j, thick = 10;
	unsigned char color = PC_GREEN;

	// TODO: (230,250)->(5,475) and (490,250)->(715,475)
	for( x = 200, y=350 ; y <= 475 ; x--, y++ ){
		//if( y % 10 == 0 )
		//	thick ++;

		if( y >= 434 )
			color = PC_RED;
		else if( y >= 392 )
			color = PC_YELLOW;

		for( i = 0 ; i < thick ; i++ )
			px8[y*buffer_pitch+x+i] = color; // 0xBBGGRRAA

		if( y == 350 || y == 351){
			for( j = 0 ; j < 50 ; j++ )
				px8[y*buffer_pitch+x+i+j] = color;
		}

		if( y == 392 || y == 393){
			for( j = 0 ; j < 70 ; j++ )
				px8[y*buffer_pitch+x+i+j] = color;
		}

		if( y == 434 || y == 435){
			for( j = 0 ; j < 90 ; j++ )
				px8[y*buffer_pitch+x+i+j] = color;
		}
	}

	color = PC_GREEN;
	thick = 10;
	for( x = 600, y=350 ; y <= 475 ; x++, y++ ){

		//if( y % 10 == 0 )
		//	thick ++;

		if( y >= 434 )
			color = PC_RED;
		else if( y >= 392 )
			color = PC_YELLOW;

		for( i = 0 ; i < thick ; i++ )
			px8[y*buffer_pitch+x-i] = color; // 0xBBGGRRAA

		if( y == 350 || y == 351){
			for( j = 0 ; j < 50 ; j++ )
				px8[y*buffer_pitch+x-i-j] = color;
		}

		if( y == 392 || y == 393){
			for( j = 0 ; j < 70 ; j++ )
				px8[y*buffer_pitch+x-i-j] = color;
		}

		if( y == 434 || y == 435){
			for( j = 0 ; j < 90 ; j++ )
				px8[y*buffer_pitch+x-i-j] = color;
		}

	}
	
	
}

static Rev_Result_e reverse_draw_line(Rev_Coordinate_t stRevCord, UINT32* pdBufAddr, UINT32 buffer_pitch, Rev_Line_e dRevLine)
{ 
	//y=mx+b;
	//m=(y2-y1)/(x2-x1);

	double m = 0, b = 0;
	int w = 0, h = 0;
	int i = 0, j = 0, k = 0, len=0;
	int x = 0;
	int y = 0, temp_y = 0;
	int v_thick = 10;
	int h_thick = 5;
	int firstlinelen = 60;
	int seclinelen = 80;
	int thirdlinelen = 100;
	int headlinelen = 7;
	int solidlinelen = 10;
	int solidlinenum = 6;
	UINT8 color[6]={PC_GREEN, PC_GREEN, PC_YELLOW, PC_YELLOW, PC_RED, PC_RED};
	UINT8* px8 = (UINT8*)pdBufAddr;

	//check coordinate is correct
	if(reverse_check_cord(&stRevCord) < 0)
		return REV_FAILED;
	
	if(stRevCord.bottom_x == stRevCord.top_x)
		stRevCord.bottom_x++;
	if(stRevCord.bottom_y == stRevCord.top_y)
		stRevCord.bottom_y++;

	if(dRevLine == REV_LEFT)
	{
		stRevCord.top_x = stRevCord.top_x-v_thick/2;
		if(stRevCord.top_x < 0)
			stRevCord.top_x = 0;
		stRevCord.top_y = stRevCord.top_y-h_thick/2;
		if(stRevCord.top_y < 0)
			stRevCord.top_y = 0;
		stRevCord.bottom_x = stRevCord.bottom_x-v_thick/2;
		if(stRevCord.bottom_x < 0)
			stRevCord.bottom_x = 0;
	}
	else
	{
		stRevCord.top_x = stRevCord.top_x+v_thick/2;
		if(stRevCord.top_x > buffer_pitch)
			stRevCord.top_x = buffer_pitch;
		stRevCord.top_y = stRevCord.top_y-h_thick/2;
		if(stRevCord.top_y < 0)
			stRevCord.top_y = 0;
		stRevCord.bottom_x = stRevCord.bottom_x+v_thick/2;
		if(stRevCord.bottom_x > buffer_pitch)
			stRevCord.bottom_x = buffer_pitch;
	}
	//printf("##### top_x=%d, top_y=%d, bottom_x=%d, bottom_y=%d\n", stRevCord.top_x, stRevCord.top_y, stRevCord.bottom_x, stRevCord.bottom_y);

	w = stRevCord.bottom_x - stRevCord.top_x;
	h = stRevCord.bottom_y - stRevCord.top_y;
	m = (double)h/(double)w;
	b = (double)stRevCord.top_y - m*(double)stRevCord.top_x;
	solidlinelen = (h-(solidlinenum-1)*headlinelen)/solidlinenum;
	if(solidlinelen < 0)
		solidlinelen = 0;
	//printf("##### w=%d, h=%d, m=%d, b=%d, solidlinelen=%d\n", w, h, m, b, solidlinelen);
	x = stRevCord.top_x;
	y = stRevCord.top_y;
	for(i=0; i<solidlinenum; i++)
	{
		if(i>0)
		y+=(headlinelen-1);

		temp_y=y;
		
		if((i%2==0) && (y==temp_y))
		{
			if(i/2 == 0)
				len = firstlinelen;
			else if(i/2 == 1)
				len = seclinelen;
			else if(i/2 == 2)
				len = thirdlinelen;
		}		
		//printf("##### i=%d, y=%d, temp_y=%d, len=%d\n", i, y, temp_y, len);		
		for(; y<=(stRevCord.top_y+solidlinelen*(i+1)+headlinelen*i); y++)
		{
			if(dRevLine == REV_LEFT)
			{
				x=((double)y-b)/(m);
				//printf("##### x=%d, y=%d\n", x, y);
				
				for(j=0; j<v_thick; j++)
				{
					px8[y*buffer_pitch+x+j] = color[i];
					if((i%2==0) && (y>=temp_y) && (y<=(temp_y+h_thick)))
					{
						for(k=0; k<len; k++)
							px8[y*buffer_pitch+x+j+k] = color[i];
					}
				}

				if(y == (stRevCord.top_y+solidlinelen*((solidlinenum-1)+1)+headlinelen*(solidlinenum-1)))
				{
					y++;
					while(y <= stRevCord.bottom_y)
					{
						x=((double)y-b)/(m);
						//printf("##### x=%d, y=%d\n", x, y);

						for(j=0; j<v_thick; j++)
							px8[y*buffer_pitch+x+j] = color[i];
						y++;
					}
				}
			}
			else
			{
				x=((double)y-b)/(m);
				//printf("##### x=%d, y=%d\n", x, y);
				
				for(j=0; j<v_thick; j++)
				{
					px8[y*buffer_pitch+x-j] = color[i];
					if((i%2==0) && (y>=temp_y) && (y<=(temp_y+h_thick)))
					{
						for(k=0; k<len; k++)
							px8[y*buffer_pitch+x-j-k] = color[i];
					}
				}

				if(y == (stRevCord.top_y+solidlinelen*((solidlinenum-1)+1)+headlinelen*(solidlinenum-1)))
				{
					y++;
					while(y <= stRevCord.bottom_y)
					{
						x=((double)y-b)/(m);
						//printf("##### x=%d, y=%d\n", x, y);

						for(j=0; j<v_thick; j++)
							px8[y*buffer_pitch+x-j] = color[i];
						y++;
					}
				}
			}		
		}
	}

	return REV_SUCCESS;
}

static void reverse_init_ns_palette(UINT32* pPalette)
{
    pPalette[PC_TRANSPARENT] = _MAKE_CULT_ARGB(0, 0, 0, 0);
    pPalette[PC_RED] = _MAKE_CULT(0xFF, 0, 0);
}

static Rev_Result_e reverse_no_signal(Rev_Coordinate_t stRevCord, UINT32* pdBufAddr, UINT32 buffer_pitch, int thick)
{
    //y=mx+b;
    //m=(y2-y1)/(x2-x1);

    double m = 0, b = 0;
    int w = 0, h = 0;
    int i = 0, j = 0;
    int x = 0;
    int y = 0;
    UINT8* px8 = (UINT8*)pdBufAddr;

    w = stRevCord.bottom_x - stRevCord.top_x;
    h = stRevCord.bottom_y - stRevCord.top_y;
    m = (double)h/(double)w;
    b = (double)stRevCord.top_y - m*(double)stRevCord.top_x;

    //printf("##### top_x=%d, top_y=%d, bottom_x=%d, bottom_y=%d\n", stRevCord.top_x, stRevCord.top_y, stRevCord.bottom_x, stRevCord.bottom_y);

    for(i=0; i<h; i++)
    {
        y=stRevCord.top_y+i;
        x=((double)y-b)/(m);
        //printf("##### x=%d, y=%d\n", x, y);

        for(j=0; j<thick; j++)
        {
            px8[y*buffer_pitch+x+j] = PC_RED;
        }
    }

    return REV_SUCCESS;
}

Rev_Result_e reverse_drawline(void)
{
	UINT32 tmp;
	UINT32* pdRevBufBase = (UINT32*)CONFIG_SP_OSD2_ADDR;
	UINT32* pdRevPaletteBufBase = (UINT32*)CONFIG_SP_OSD2_PALETTE_ADDR;
	Rev_Coordinate_t  stRevCord_left = {250, 300, 100, 450};
	Rev_Coordinate_t  stRevCord_right = {550, 300, 700, 450};

	if (sp_config_get_int("UI_width", &tmp))
		UI_width = 0;
	else
		UI_width = tmp;

	if (sp_config_get_int("UI_height", &tmp))
		UI_height = 0;
	else
		UI_height = tmp;

	printf("##### reverse_drawline width %d height %d\n", UI_width, UI_height);

	memset((UINT8*)pdRevBufBase, 0x0, CONFIG_SP_OSD2_SIZE);
	memset((UINT8*)pdRevPaletteBufBase, 0x0, CONFIG_SP_OSD2_PALETTE_SIZE);

	//draw_assist_line(pdRevBufBase, (UINT32)UI_width);
	reverse_get_leftcord((UINT32*)&stRevCord_left, SP_VDATA_REVERSE_POS);
	reverse_get_rightcord((UINT32*)&stRevCord_right, SP_VDATA_REVERSE_POS+REV_CORD_LEN*4);
	if(reverse_draw_line(stRevCord_left, pdRevBufBase, UI_width, REV_LEFT) < 0)
		return REV_FAILED;
	
	if(reverse_draw_line(stRevCord_right, pdRevBufBase, UI_width, REV_RIGHT) < 0)
		return REV_FAILED;

	//printf("##### reverse_drawline pdRevBufBase:0x%x, pdRevPaletteBufBase:0x%x\n", (UINT32)pdRevBufBase, (UINT32)pdRevPaletteBufBase);
	reverse_init_palette(pdRevPaletteBufBase);

    UINT32* pdRevNSBufBase = (UINT32*)((UINT8*)CONFIG_SP_OSD2_ADDR+CONFIG_SP_OSD2_SIZE/2);
    UINT32* pdRevNSPaletteBufBase = (UINT32*)((UINT8*)CONFIG_SP_OSD2_PALETTE_ADDR+CONFIG_SP_OSD2_PALETTE_SIZE/2);
    int thick = 30;
    Rev_Coordinate_t stRevCord1 = {0, 0, UI_width-thick, UI_height};
    Rev_Coordinate_t stRevCord2 = {UI_width-thick, 0, 0, UI_height};

    reverse_no_signal(stRevCord1, pdRevNSBufBase, UI_width, thick);
    reverse_no_signal(stRevCord2, pdRevNSBufBase, UI_width, thick);
    //printf("##### reverse_drawline pdRevNSBufBase:0x%x, pdRevNSPaletteBufBase:0x%x\n", (UINT32)pdRevNSBufBase, (UINT32)pdRevNSPaletteBufBase);
    reverse_init_ns_palette(pdRevNSPaletteBufBase);
#ifndef CONFIG_SYS_ARM_CACHE_WRITETHROUGH
    flush_dcache_range((unsigned long)CONFIG_SP_OSD2_ADDR, (unsigned long)(CONFIG_SP_OSD2_ADDR + CONFIG_SP_OSD2_SIZE + CONFIG_SP_OSD2_PALETTE_SIZE));
#endif
	return REV_SUCCESS;
}

