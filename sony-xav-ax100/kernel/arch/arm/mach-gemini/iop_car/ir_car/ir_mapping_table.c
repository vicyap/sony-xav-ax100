/********************************************************************
** File Name: ir_mapping_table.c (IR401)
**
** File Description: this file is sunplus ir 401 mapping table
**
** Copyright(c) 2009 Sunplus Technologies - All Rights Reserved
**
** History Information Description
** Date						Author					Modification
** 2009/03/16				James					Create
********************************************************************/
#ifdef BOOTLOADER_SYS
#include "platform/1500/drv_ir.h"
#else
//#include "drivers/drv_ir.h" //Kaney mark
#include <iop_car/drv_ir.h> //Kaney add
#endif
#include <iop_car/event.h> //Kaney add

const UINT32 g_u32CustomCode = 0xFE;

//Setup the serviced command in Standby IOP, plz enter the "IR Command Value". The Max Size can be checked by IR_STANDBY_SERVICED_CMD_COUNT
const DRV_IRStandbySupCmd_t g_u8IrStandbyTable[]=
{
	//{IrCmd Value, System Command} --> System Can be re-mapping to UI_EVENT
	//Command 1: (suggest the Power Key)
	{0x62,UI_EVENT_POWER},
};

const UINT8 g_u8IrMapTable[]=
{
	//0x00,				0x01,				0x02,				0x03,
	UI_EVENT_PREV,		UI_EVENT_NULL,		UI_EVENT_NULL,		UI_EVENT_NEXT,
	//0x04,				0x05,				0x06,				0x07,
	UI_EVENT_PAUSE,		UI_EVENT_MUTE,		UI_EVENT_NULL,		UI_EVENT_SLOW,
	//0x08,				0x09,				0x0a,				0x0b,
	UI_EVENT_AB,		UI_EVENT_VOL_DN,	UI_EVENT_6,			UI_EVENT_REPEAT,
	//0x0c,				0x0d,				0x0e,				0x0f,
	UI_EVENT_MIXER,		UI_EVENT_CH_DN,		UI_EVENT_AUDIO,		UI_EVENT_SOURCE,
	//0x10,				0x11,				0x12,				0x13,
	UI_EVENT_YELLOW,	UI_EVENT_RECALL,	UI_EVENT_PAGE_DN,	UI_EVENT_BLUE,
	//0x14,				0x15,				0x16,				0x17,
	UI_EVENT_ENTER,		UI_EVENT_EXIT,		UI_EVENT_NULL,		UI_EVENT_RIGHT,
	//0x18,				0x19,				0x1a,				0x1b,
	UI_EVENT_DOWN,		UI_EVENT_CH_UP,		UI_EVENT_9,			UI_EVENT_P_SCAN,
	//0x1c,				0x1d,				0x1e,				0x1f,
	UI_EVENT_UP,		UI_EVENT_VOL_UP,	UI_EVENT_3,			UI_EVENT_EPG,
	//0x20,				0x21,				0x22,				0x23,
	UI_EVENT_NULL,		UI_EVENT_NULL,		UI_EVENT_NULL,		UI_EVENT_NULL,
	//0x24,				0x25,				0x26,				0x27,
	UI_EVENT_NULL,		UI_EVENT_NULL,		UI_EVENT_NULL,		UI_EVENT_NULL,
	//0x28,				0x29,				0x2a,				0x2b,
	UI_EVENT_NULL,		UI_EVENT_NULL,		UI_EVENT_NULL,		UI_EVENT_NULL,
	//0x2c,				0x2d,				0x2e,				0x2f,
	UI_EVENT_NULL,		UI_EVENT_NULL,		UI_EVENT_NULL,		UI_EVENT_NULL,
	//0x30,				0x31,				0x32,				0x33,
	UI_EVENT_NULL,		UI_EVENT_NULL,		UI_EVENT_NULL,		UI_EVENT_NULL,
	//0x34,				0x35,				0x36,				0x37,
	UI_EVENT_NULL,		UI_EVENT_NULL,		UI_EVENT_NULL,		UI_EVENT_NULL,
	//0x38,				0x39,				0x3a,				0x3b,
	UI_EVENT_NULL,		UI_EVENT_NULL,		UI_EVENT_NULL,		UI_EVENT_NULL,
	//0x3c,				0x3d,				0x3e,				0x3f,
	UI_EVENT_NULL,		UI_EVENT_NULL,		UI_EVENT_NULL,		UI_EVENT_NULL,
	//0x40,				0x41,				0x42,				0x43,
	UI_EVENT_FF,		UI_EVENT_NULL,		UI_EVENT_NULL,		UI_EVENT_FR,
	//0x44,				0x45,				0x46,				0x47,
	UI_EVENT_PLAY,		UI_EVENT_NULL,		UI_EVENT_POWER,		UI_EVENT_STOP,
	//0x48,				0x49,				0x4a,				0x4b,
	UI_EVENT_GOTO,		UI_EVENT_5,			UI_EVENT_4,			UI_EVENT_REC,
	//0x4c,				0x4d,				0x4e,				0x4f,
	UI_EVENT_VFMT,		UI_EVENT_0,			UI_EVENT_FAV,		UI_EVENT_DEV,
	//0x50,				0x51,				0x52,				0x53,
	UI_EVENT_GREEN,		UI_EVENT_PAGE_UP,	UI_EVENT_TVRADIO,	UI_EVENT_RED,
	//0x54,				0x55,				0x56,				0x57,
	UI_EVENT_NULL,		UI_EVENT_MENU,		UI_EVENT_INFO,		UI_EVENT_LEFT,
	//0x58,				0x59,				0x5a,				0x5b,
	UI_EVENT_NULL,		UI_EVENT_8,			UI_EVENT_7,			UI_EVENT_SUBTITLE,
	//0x5c,				0x5d,				0x5e,				0x5f,
	UI_EVENT_NULL,		UI_EVENT_2,			UI_EVENT_1,			UI_EVENT_TEXT,
	//0x60,				0x61,				0x62,				0x63,
	UI_EVENT_NULL,		UI_EVENT_NULL,		UI_EVENT_NULL,		UI_EVENT_NULL,
	//0x64,				0x65,				0x66,				0x67,
	UI_EVENT_NULL,		UI_EVENT_NULL,		UI_EVENT_NULL,		UI_EVENT_NULL,
	//0x68,				0x69,				0x6a,				0x6b,
	UI_EVENT_NULL,		UI_EVENT_NULL,		UI_EVENT_NULL,		UI_EVENT_NULL,
};

UINT8 IrStandby_Get_CmdCount(void)
{
	return ( sizeof(g_u8IrStandbyTable)/sizeof(DRV_IRStandbySupCmd_t) );
}

