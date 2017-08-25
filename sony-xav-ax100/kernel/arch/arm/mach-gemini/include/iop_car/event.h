/**********************************************************************
ModuleName:	Applcation Layer

Description: Definition File of Application Global Events

Copyright:	Copyright @	2009 Sunplus, Inc.

History:		garylin@sunplus.com	 03/09/2009	Creation
**********************************************************************/

#ifndef _EVENT_H_
#define _EVENT_H_
//#include <types.h>//include this .h file for mantis bug 95090 by ztli 2010/07/09 //Kaney mark
#include "platform_define.h" //Kaney add
#include "customize.h" //Kaney add
#if 1 //def USE_IN_APP_LAYER //Kaney modify
#define APPLICATION_GLOBAL_EVENT_TYPE	(1<<31)

typedef enum AppGlobalEvent_t_ {

	//User Input Event Definitions

	//General Events
	UI_EVENT_0 = 0x0000,
	UI_EVENT_1,
	UI_EVENT_2,
	UI_EVENT_3,
	UI_EVENT_4,
	UI_EVENT_5,
	UI_EVENT_6,
	UI_EVENT_7,
	UI_EVENT_8,
	UI_EVENT_9,
	UI_EVENT_10,
	UI_EVENT_10_PLUS,
	UI_EVENT_CLEAR,
	UI_EVENT_POWER,
	UI_EVENT_UP,		//14
	UI_EVENT_DOWN,	//15
	UI_EVENT_LEFT,		//16
	UI_EVENT_RIGHT,	//17
	UI_EVENT_ENTER,		//18
	UI_EVENT_DISPLAY,	//19
	UI_EVENT_MUTE,	//20
	UI_EVENT_VOL_UP,	//21
	UI_EVENT_VOL_DN,		//22
	UI_EVENT_MENU,		//23
	UI_EVENT_GOTO,		//24
	UI_EVENT_AB,		//25
	UI_EVENT_REPEAT,	//26
	UI_EVENT_STOP,		//27
	UI_EVENT_PLAY,		//28
	UI_EVENT_PAUSE,	//29
	UI_EVENT_SLOW,	//30
	UI_EVENT_FF,		//31
	UI_EVENT_FR,		//32
	UI_EVENT_NEXT,		//33
	UI_EVENT_PREV,		//34
	UI_EVENT_RED,
	UI_EVENT_GREEN,
	UI_EVENT_YELLOW,
	UI_EVENT_BLUE,
	UI_EVENT_VIRTUAL_REMOTE,

	UI_EVENT_TITLE,
	UI_EVENT_SETUP,
	UI_EVENT_A,
	UI_EVENT_B,
	UI_EVENT_C,
	UI_EVENT_D,
	UI_EVENT_ZOOM,
	UI_EVENT_MARK,
	UI_EVENT_RANDOM,
	UI_EVENT_PIPAUDIO,
	UI_EVENT_PIPVIDEO,
	UI_EVENT_LR,
	UI_EVENT_PN,
	UI_EVENT_KARAOKE,
	UI_EVENT_RESOLUTION,
	UI_EVENT_STILL_OFF,
	UI_EVENT_SECONDARY_VIDEO_ENABLE_DISABLE,
	UI_EVENT_SECONDARY_AUDIO_ENABLE_DISABLE,

	UI_EVENT_DIRECT_PLAY,
	UI_EVENT_FILE_TYPE,
	UI_EVENT_DEFAULT,
	UI_EVENT_HOMEPAGE,
	UI_EVENT_PLAY_PAUSE,


	//Disc Player Specific Events
	UI_EVENT_EJECT = 0x0050,
	UI_EVENT_TITLE_MENU,
	UI_EVENT_RETURN,
	UI_EVENT_ANGLE,
	UI_EVENT_LANGUAGE,

	//STB Specific Events
	UI_EVENT_CH_UP = 0x0090,
	UI_EVENT_CH_DN,
	UI_EVENT_FAV,
	UI_EVENT_AUDIO,
	UI_EVENT_TVRADIO,
	UI_EVENT_PAGE_UP,
	UI_EVENT_PAGE_DN,
	UI_EVENT_RECALL,
	UI_EVENT_INFO,
	UI_EVENT_EXIT,
	UI_EVENT_TEXT,
	UI_EVENT_EPG,
	UI_EVENT_SUBTITLE,
	UI_EVENT_P_SCAN,
	UI_EVENT_DEV,
	UI_EVENT_VFMT,
	UI_EVENT_MIXER,
	UI_EVENT_SOURCE,
	UI_EVENT_REC,
	UI_EVENT_ASPECT,  //wsc 20100407 add
	UI_EVENT_TIMESHIFT,  //wsc 20100604 add for utek
	UI_EVENT_MOSAIC, //wurh add it.20110722
	//mili for NVOD_SUPPORT 20110801 {
	#if defined(NVOD_SUPPORT)
	UI_EVENT_NVOD,
	#endif
	//mili for NVOD_SUPPORT 20110801 }

	UI_EVENT_NULL,

	//All setup event should be declared between SETUP_EVENT_BASE and SETUP_EVENT_MAX
	SETUP_EVENT_BASE,
	SETUP_EVENT_AspectRatio,
	SETUP_EVENT_TVSystem,
	SETUP_EVENT_OutputComponent,
	SETUP_EVENT_YCbCrHDMIResolution,
	SETUP_EVENT_VGAResolution,
	SETUP_EVENT_HDMI1080p24Hz,
	SETUP_EVENT_HDMIColorSetting,
	SETUP_EVENT_HDMIDeepColor,
	SETUP_EVENT_MenuLanguage,
	SETUP_EVENT_DiscAudio,
	SETUP_EVENT_DiscSubtitle,
	SETUP_EVENT_DiscMenu,
	SETUP_EVENT_SPDIF,
	SETUP_EVENT_HDMIAudio,
	SETUP_EVENT_DownSampling,
	SETUP_EVENT_Surround,
	SETUP_EVENT_EQ,
	SETUP_EVENT_DRC,
	SETUP_EVENT_KaraokeEcho,
	SETUP_EVENT_KaraokeKeyShift,
	SETUP_EVENT_AudioDownMix,
	SETUP_EVENT_GM5,
	SETUP_EVENT_AudioSize,
	SETUP_EVENT_AudioDistance,
	SETUP_EVENT_AudioLevel,
	SETUP_EVENT_AudioTestTone,
	SETUP_EVENT_DVDRating,
	SETUP_EVENT_Resume,
	SETUP_EVENT_BlackLevelEnable,
	SETUP_EVENT_HDMIAutoLink,
	SETUP_EVENT_ScreenFit,
	SETUP_EVENT_MAX,

	DIAG_EVENT_REFRESH,
	ETEXIT_WITH_DAEMON_DOWNLOAD,
	N32EXIT_WITH_DAEMON_DOWNLOAD,

	//DAEMON Generated Event Definitions
	DMN_EVENT_SCREEN_SAVER_ON,
	DMN_EVENT_SCREEN_SAVER_OFF,
	DMN_EVENT_SCREEN_SAVER_UPDATE,
	DMN_EVENT_TEMPTEST_START_USB_MINUSVR, //chenzhao 2009-7-3 19:8 .
	DMN_EVENT_TEMPTEST_START_USB_VCD,
	DMN_EVENT_TEMPTEST_START_USB_DVD = 0x00FF, //SpikeYang, HDMI inserted
	//DMN_EVENT_HDMI_HPD_IN = 0x0100, //Reversed
	//DMN_EVENT_HDMI_HPD_OUT,
	//DMN_EVENT_HDMI_RSEN_IN,
	//DMN_EVENT_HDMI_RSEN_OUT,
	DMN_EVENT_HDMI_EDID_VALID,
	DMN_EVENT_HDMI_EDID_INVALID,
	//DMN_EVENT_HDMI_AVMUTE,
	//DMN_EVENT_HDMI_DEAVMUTE,
	DMN_EVENT_HDMI_REQUEST_RES = 0x0108,
	DMN_EVENT_FF_MIN, //CEC start
	DMN_EVENT_FF_MEDIUM,
	DMN_EVENT_FF_MAX,
	DMN_EVENT_FR_MIN,
	DMN_EVENT_FR_MEDIUM,
	DMN_EVENT_FR_MAX,
	DMN_EVENT_SF_MIN,
	DMN_EVENT_SF_MEDIUM,
	DMN_EVENT_SF_MAX,
	DMN_EVENT_SR_MIN,
	DMN_EVENT_SR_MEDIUM,
	DMN_EVENT_SR_MAX,
	DMN_EVENT_ROOT_MENU,
	DMN_EVENT_RESTORE_VOL,
	DMN_EVENT_SET_LANGUAGE,	//CEC end
	DMN_EVENT_ANIMATION_COMPLETED,
	DMN_EVENT_AUTO_POWER_OFF,

	// the device type
	DMN_EVENT_USB_HDD_ATTACHED =0x1000,
	DMN_EVENT_USB_HDD_DETACHED,
	DMN_EVENT_USB_LOADER_ATTACHED,
	DMN_EVENT_USB_LOADER_DETACHED,
	DMN_EVENT_CARD_DEV_ATTACHED,
	DMN_EVENT_CARD_DEV_DETACHED,
	DMN_EVENT_IDE_HDD_ATTACHED,
	DMN_EVENT_IDE_HDD_DETACHED,
	DMN_EVENT_IDE_LOADER_ATTACHED,
	DMN_EVENT_IDE_LOADER_DETACHED,
	DMN_EVENT_SERVO_ATTACHED,
	DMN_EVENT_SERVO_DETACHED,
	DMN_EVENT_NAND_ATTACHED,
	DMN_EVENT_NAND_DETACHED,
	DMN_EVENT_FILE_LOADER_ATTACHED,
	DMN_EVENT_FILE_LOADER_DETACHED,
	DMN_EVENT_WIFI_DONGLE_ATTACHED,
	DMN_EVENT_WIFI_DONGLE_DETACHED,
//#ifdef SUPPORT_HID_INPUT			//mark for 3G
	DMN_EVENT_USB_HID_DEV_ATTACHED,
	DMN_EVENT_USB_HID_DEV_DETACHED,
//#endif
	DMN_EVENT_UNKNOWN_DEVICE_ATTACHED,
	DMN_EVENT_3G_DONGLE_ATTACHED,
	DMN_EVENT_3G_DONGLE_DETACHED,
	//Raymond refine all disc content type
	DMN_EVENT_DI_UNMOUNT_SUCCESS,
	DMN_EVENT_BD_VIDEO_DISC_INSERTED,
	DMN_EVENT_AVCHD_DISC_INSERTED,
	DMN_EVENT_DVD_VIDEO_DISC_INSERTED,
	DMN_EVENT_MINUSVR_DISC_INSERTED, //chenzhao 2009-7-3 19:8 .
	DMN_EVENT_PLUSVR_DISC_INSERTED,
	DMN_EVENT_SVCD_DISC_INSERTED,
	DMN_EVENT_VCD_DISC_INSERTED,
	DMN_EVENT_DISC_INSERTED,//add by george.chang 2010.10.04
	DMN_EVENT_CDDA_DISC_INSERTED,
	DMN_EVENT_KOK_DISC_INSERED,
	DMN_EVENT_DATA_DISC_INSERED,
	DMN_EVENT_KODAKPIC_DISC_INSERED,  //lgz 2011-6-3 17:55 add for KODAK_PIC_MODE_SUPPORT
	DMN_EVENT_UNKNOW_DISC_INSERTED,
	DMN_EVENT_NO_DISC_INSERTED,
	DMN_EVENT_BEGIN_IDENTIFY_DISC,//Info App current DI start to identify Disc
	DMN_EVENT_DI_DISCEJECT_SUCCESS,
	DMN_EVENT_DI_AUTO_TRAYIN,
	DMN_EVENT_DI_AUTO_TRAYOUT,

	DMN_EVENT_APP_TIMER,
	DMN_EVENT_WKTIMER_TRIGGERED,
	DMN_EVENT_DVB_TSRECORDER_DISK_FULL,  //xqhuang 2012/05/30 add for mantis 181216
	DMN_EVENT_DVB_TSRECORDER_WRITE_EER, //xqhuang 2012/05/30 add for mantis 181216
	DMN_EVENT_DVB_RECORDER_WRITE_EER, // for DVB recorder callback
	DMN_EVENT_DVB_RECORDER_DISK_FULL, // for DVB recorder callback
	DMN_EVENT_DVB_RECORDER_GFS_OK, // for DVB recorder callback
	DMN_EVENT_DVB_RECORDER_GFS_DISKFULL, // for DVB recorder callback
	DMN_EVENT_DVB_RECORDER_GFS_ERR, // for DVB recorder callback
	DMN_EVENT_DVB_RECORDER_DISK_CHECKING, // for DVB recorder callback
	DMN_EVENT_DVB_RECORDER_NTFS_DITRY,//pinchao 20100531 add for mantis bug 0090195
	DMN_EVENT_DVB_RECORDER_RINGSIZE_UPDATE,
	DMN_EVENT_DVB_RECORDER_NTFS_LIMIT,	//ntfs limitation
	DMN_EVENT_DVB_RECORDER_GFS_PAUSE, // for DVB recorder callback
#ifdef GET_PREFER_PARTITION_USE_TASK
	DMN_EVENT_DVB_RECORDER_GET_PREFER_PARTION_OK,
	DMN_EVENT_DVB_RECORDER_GET_PREFER_PARTION_ERR,
#endif
	DMN_EVENT_DVB_TSPL_PARSE_DONE,
	DMN_EVENT_DVB_TSPL_PARSE_FAIL,
	DMN_EVENT_DVB_TSPL_PLAY_DONE,
	DMN_EVENT_DVB_TSPL_PLAY_READERR,
	DMN_EVENT_DVB_TSPL_PLAY_RESUME,
	DMN_EVENT_DVB_TSPL_PLAYINIT_FAIL,
	DMN_EVENT_DVB_TSPL_PLAYINIT_OK,
#ifdef SUPPORT_CI_PLUS_PVR_V12
	DMN_EVENT_DVB_TSPL_PLAY_RET_EXPIRED,
#endif

	DMN_EVENT_CONTENT_FRAME_RATE_CHG,
	DMN_EVENT_CONTENT_XVCOLOR_SUPPORT_CHG,
	DMN_EVENT_NETISP_CONNECT,  //zhangjihua 09-1-13
	DMN_EVENT_NETISP_NET_FAIL,
	DMN_EVENT_NETISP_DOWNLOAD_FAIL,
	DMN_EVENT_NETISP_UPGRADE,
	DMN_EVENT_DVB_TS_CHANGE,//jinfa 2012-4-11 modify for mantis  175147 
	//jgliu move to here, let them bypass to system APP DVB. Start{
	#ifdef SUPPORT_SAT2IP//If define SUPPORT_SAT2IP, DVB APP should always response the events.
	DMN_EVENT_NET_LINKER_ATTACHED,
	DMN_EVENT_NET_LINKER_DETACHED,
	DMN_EVENT_NET_LINKER_DETACHED_WITHOUT_POPUP,
	DMN_EVENT_NET_PPPOELINKER_ATTACHED,
	DMN_EVENT_NET_PPPOELINKER_DETACHED,
	DMN_EVENT_NET_LINKDOWN,
	DMN_EVENT_NET_INTI_DONE,
	#endif
	//jgliu move to here, let them bypass to system APP DVB. End}
	DMN_EVENT_MAINAPP_STARTS,
	DMN_EVENT_DVB_SOURCE_SWITCH,
	DMN_EVENT_GPIO_IPOD_IN,	//yanyuan 2010-09-08
	DMN_EVENT_GPIO_IPOD_OUT,	//yanyuan 2010-09-08
	DMN_EVENT_CLOSE_AND_OPEN_PVRSTATE,	//kf 110323 mantis 127800
	
	#ifndef SUPPORT_SAT2IP//If no define SUPPORT_SAT2IP, these event only responsed in network APPs.
	DMN_EVENT_NET_LINKER_ATTACHED,
	DMN_EVENT_NET_LINKER_DETACHED,
	DMN_EVENT_NET_LINKER_DETACHED_WITHOUT_POPUP,
	DMN_EVENT_NET_PPPOELINKER_ATTACHED,
	DMN_EVENT_NET_PPPOELINKER_DETACHED,
	DMN_EVENT_NET_LINKDOWN,
	DMN_EVENT_NET_INTI_DONE,
	#endif

   // DMN_EVENT_IPOD_PLUG_IN, //wangdeyou add for ipod detect
   // DMN_EVENT_IPOD_PLUG_OUT,

	DMN_EVENT_OTA_EVENT_START,
	DMN_EVENT_USER_OTA_EVENT_START,

	DMN_EVENT_AUTOSTANDBY_TRIGGERED,
	DMN_EVENT_AUTOSTANDBY_SYSTEM_POWER_DOWN,
	DMN_EVENT_AUTOSTANDBY_UI,
	DMN_EVENT_NET_UPGRADE_EVENT_START,
	DMN_EVENT_ISP_GET_BIN_FAIL,
	DMN_EVENT_ISP_GET_BIN_DONE,
	DMN_EVENT_ISP_FINISH,

	DMN_EVENT_USB_MOUSE_ATTACHED,
	DMN_EVENT_USB_KEYBOARD_ATTACHED,
	DMN_EVENT_USB_JOYSTICK_ATTACHED,

	DMN_EVENT_USB_MOUSE_DETACHED,
	DMN_EVENT_USB_KEYBOARD_DETACHED,
	DMN_EVENT_USB_JOYSTICK_DETACHED,

	DMN_EVENT_USB_MOUSE_KEY_IN,
	DMN_EVENT_USB_KEYBOARD_KEY_IN,
	DMN_EVENT_USB_JOYSTICK_KEY_IN,

	DMN_EVENT_DVB_SIUPDATE,
	DMN_EVENT_DVB_NETWORK_UPDATE,//xqhuang add for manits 134236
	
	DMN_EVENT_DVB_RESTORE_SERVICE,
	DMN_EVENT_DVB_OSD2_UPDATE,
	DMN_EVENT_DVB_PROG_UPDATE_SCRAMBLE_ICON,
	DMN_EVENT_DVB_UPDATE_SERVICE_LIST,
	DMN_EVENT_DVB_COPYCONTROL_UP,
#ifdef FREEVIEW_HD_SUPPORT_RCT
	DMN_EVENT_DVB_SHOW_RCT_ICON,
	DMN_EVENT_DVB_CLOSE_RCT_MENU,
#endif
#ifdef SUPPORT_DVBC_SD_AD
	DMN_EVENT_DVB_SHOW_AD,
#endif
#ifdef GMOTE_SUPPORT							//jiang.hu 2012-11-19
	RM_EVENT_KEYBOARD_UTF8STR_POST,
#endif

#ifdef NET_WEBSERVER_SUPPORT							//ff.feng 2013-6-8 for webserver keyboard
	WS_EVENT_KEYBOARD_UTF8STR_POST,
#endif


#ifdef SUPPORT_HID_INPUT
	DMN_EVENT_HIDDEV_MOUSE_REL_XY,
	DMN_EVENT_HIDDEV_MOUSE_REL_WHEEL,
	DMN_EVENT_HIDDEV_MOUSE_BTN_LEFT,
	DMN_EVENT_HIDDEV_MOUSE_BTN_RIGHT,
	DMN_EVENT_HIDDEV_MOUSE_BTN_MIDDLE,
	DMN_EVENT_HIDDEV_MOUSE_BTN_SIDE,
	DMN_EVENT_HIDDEV_MOUSE_BTN_EXTRA,
	DMN_EVENT_HIDDEV_MOUSE_BTN_FORWARD,
	DMN_EVENT_HIDDEV_MOUSE_BTN_BACK,
	DMN_EVENT_HIDDEV_MOUSE_BTN_TASK,

	DMN_EVENT_HIDDEV_KEYBOARD_KEY_ASCII,
	DMN_EVENT_HIDDEV_KEYBOARD_KEY_KPASCII,

	DMN_EVENT_HIDDEV_KEYBOARD_KEY_CAPSLOCK,
	DMN_EVENT_HIDDEV_KEYBOARD_KEY_NUMLOCK,
	DMN_EVENT_HIDDEV_KEYBOARD_KEY_SCROLLLOCK,

	DMN_EVENT_HIDDEV_KEYBOARD_KEY_ESC,
	DMN_EVENT_HIDDEV_KEYBOARD_KEY_TAB,
	DMN_EVENT_HIDDEV_KEYBOARD_KEY_ENTER,
	DMN_EVENT_HIDDEV_KEYBOARD_KEY_KPENTER,
	DMN_EVENT_HIDDEV_KEYBOARD_KEY_BACKSPACE,

	DMN_EVENT_HIDDEV_KEYBOARD_KEY_F1,
	DMN_EVENT_HIDDEV_KEYBOARD_KEY_F2,
	DMN_EVENT_HIDDEV_KEYBOARD_KEY_F3,
	DMN_EVENT_HIDDEV_KEYBOARD_KEY_F4,
	DMN_EVENT_HIDDEV_KEYBOARD_KEY_F5,
	DMN_EVENT_HIDDEV_KEYBOARD_KEY_F6,
	DMN_EVENT_HIDDEV_KEYBOARD_KEY_F7,
	DMN_EVENT_HIDDEV_KEYBOARD_KEY_F8,
	DMN_EVENT_HIDDEV_KEYBOARD_KEY_F9,
	DMN_EVENT_HIDDEV_KEYBOARD_KEY_F10,
	DMN_EVENT_HIDDEV_KEYBOARD_KEY_F11,
	DMN_EVENT_HIDDEV_KEYBOARD_KEY_F12,
	DMN_EVENT_HIDDEV_KEYBOARD_KEY_SYSRQ,
	DMN_EVENT_HIDDEV_KEYBOARD_KEY_PAUSE,

	DMN_EVENT_HIDDEV_KEYBOARD_KEY_INSERT,
	DMN_EVENT_HIDDEV_KEYBOARD_KEY_DELETE,
	DMN_EVENT_HIDDEV_KEYBOARD_KEY_HOME,
	DMN_EVENT_HIDDEV_KEYBOARD_KEY_END,
	DMN_EVENT_HIDDEV_KEYBOARD_KEY_PAGEUP,
	DMN_EVENT_HIDDEV_KEYBOARD_KEY_PAGEDOWN,
	DMN_EVENT_HIDDEV_KEYBOARD_KEY_UP,
	DMN_EVENT_HIDDEV_KEYBOARD_KEY_DOWN,
	DMN_EVENT_HIDDEV_KEYBOARD_KEY_LEFT,
	DMN_EVENT_HIDDEV_KEYBOARD_KEY_RIGHT,

	DMN_EVENT_HIDDEV_KEYBOARD_KEY_LEFTSHIFT,
	DMN_EVENT_HIDDEV_KEYBOARD_KEY_LEFTCTRL,
	DMN_EVENT_HIDDEV_KEYBOARD_KEY_LEFTMETA,
	DMN_EVENT_HIDDEV_KEYBOARD_KEY_LEFTALT,
	DMN_EVENT_HIDDEV_KEYBOARD_KEY_RIGHTALT,
	DMN_EVENT_HIDDEV_KEYBOARD_KEY_RIGHTMETA,
	DMN_EVENT_HIDDEV_KEYBOARD_KEY_COMPOSE,
	DMN_EVENT_HIDDEV_KEYBOARD_KEY_RIGHTCTRL,
	DMN_EVENT_HIDDEV_KEYBOARD_KEY_RIGHTSHIFT,

	DMN_EVENT_HIDDEV_JOYSTICK_ABS_KEY_PRESS,
	DMN_EVENT_HIDDEV_JOYSTICK_BTN_TRIGGER,
	DMN_EVENT_HIDDEV_JOYSTICK_BTN_THUMB,
	DMN_EVENT_HIDDEV_JOYSTICK_BTN_THUMB2,
	DMN_EVENT_HIDDEV_JOYSTICK_BTN_TOP,
	DMN_EVENT_HIDDEV_JOYSTICK_BTN_TOP2,
	DMN_EVENT_HIDDEV_JOYSTICK_BTN_PINKIE,
	DMN_EVENT_HIDDEV_JOYSTICK_BTN_BASE,
	DMN_EVENT_HIDDEV_JOYSTICK_BTN_BASE2,
	DMN_EVENT_HIDDEV_JOYSTICK_BTN_BASE3,
	DMN_EVENT_HIDDEV_JOYSTICK_BTN_BASE4,
	DMN_EVENT_HIDDEV_JOYSTICK_BTN_BASE5,
	DMN_EVENT_HIDDEV_JOYSTICK_BTN_BASE6,
	DMN_EVENT_HIDDEV_JOYSTICK_BTN_DEAD,

	DMN_EVENT_HIDDEV_GAMEPAD_BTN_A,
	DMN_EVENT_HIDDEV_GAMEPAD_BTN_B,
	DMN_EVENT_HIDDEV_GAMEPAD_BTN_C,
	DMN_EVENT_HIDDEV_GAMEPAD_BTN_X,
	DMN_EVENT_HIDDEV_GAMEPAD_BTN_Y,
	DMN_EVENT_HIDDEV_GAMEPAD_BTN_Z,
	DMN_EVENT_HIDDEV_GAMEPAD_BTN_TL,
	DMN_EVENT_HIDDEV_GAMEPAD_BTN_TR,
	DMN_EVENT_HIDDEV_GAMEPAD_BTN_TL2,
	DMN_EVENT_HIDDEV_GAMEPAD_BTN_TR2,
	DMN_EVENT_HIDDEV_GAMEPAD_BTN_SELECT,
	DMN_EVENT_HIDDEV_GAMEPAD_BTN_START,
	DMN_EVENT_HIDDEV_GAMEPAD_BTN_MODE,
	DMN_EVENT_HIDDEV_GAMEPAD_BTN_THUMBL,
	DMN_EVENT_HIDDEV_GAMEPAD_BTN_THUMBR,
	
	DMN_EVENT_HIDDEV_JOYSTICK_ABS_KEY_RELEASE_X,
	DMN_EVENT_HIDDEV_JOYSTICK_ABS_KEY_RELEASE_Y,
#endif

	GLOBAL_EVENT_MAX
} AppGlobalEvent_t;

#endif
#endif