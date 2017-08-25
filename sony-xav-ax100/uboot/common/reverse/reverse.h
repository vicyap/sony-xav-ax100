#ifndef __REVERSE_H__
#define __REVERSE_H__

/********************************************************************************
*                                                            Include File                                                                   *
*********************************************************************************/


/********************************************************************************
*                                                            Macro Function                                                             *
*********************************************************************************/
#define MAX_COLOR_NUM    256
#define REV_CORD_LEN    8


/********************************************************************************
*                                                            Enum and Struct                                                            *
*********************************************************************************/
typedef enum Rev_Line_e_
{
	REV_LEFT = 0,
	REV_RIGHT,
}Rev_Line_e;

typedef enum Rev_Result_e_
{
	REV_SUCCESS = 0,
	REV_FAILED = -1,
}Rev_Result_e;

//Index of LUT8
typedef enum Rev_Palette_Color_e_
{
	PC_TRANSPARENT = 0,
	PC_RED,
	PC_YELLOW,
	PC_GREEN,
}Rev_Palette_Color_e;

typedef struct Rev_Coordinate_t_
{
	UINT32 top_x;
	UINT32 top_y;
	UINT32 bottom_x;
	UINT32 bottom_y;
}Rev_Coordinate_t;


/********************************************************************************
*                                                            Function Prototype                                                         *
*********************************************************************************///
extern Rev_Result_e reverse_drawline(void);

#endif	//__REVERSE_H__

