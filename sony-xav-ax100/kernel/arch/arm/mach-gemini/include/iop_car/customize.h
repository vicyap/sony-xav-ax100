#ifndef __CUSTOMIZE_H__
#define __CUSTOMIZE_H__

#include "auto_config.h"
//#include "dvb_debug.h" //Kaney mark

#ifdef RELEASE_WITHOUT_DEBUG_INFO
#undef diag_printf
#define diag_printf(fmt, arg...)
#undef diag_printk
#define diag_printk(fmt, arg...)
#endif
//=================================================================
// for burn customerID to OTP
//=================================================================
//#define SUPPORT_SERVER_CUSTOMER_ID_WRITE //zf.wang@20130906 add for mantic 0218775 
#ifdef SUPPORT_SERVER_CUSTOMER_ID_WRITE
#define SUPPORT_SERVER_CUSTOMER_ID_WRITE_LOG
#endif
//#define SUPPORT_CLIENT_CUSTOMER_ID_WRITE //huagui.yang@20130918 add for mantis 0219599
#ifdef SUPPORT_CLIENT_CUSTOMER_ID_WRITE
//#define SUPPORT_CLIENT_CUSTOMER_ID_WRITE_LOG // support write log to file
#endif
//=================================================================
#define SUPPORT_PREVIEW_CHANNEL //guoliang.yao add 20110802 for mantis 0145117
#define SUPPORT_FIND_TP_EXISTED	//added by duping for mantis:132567 @2011-04-27.
#define SUPPORT_DEL_ALL_CHANNEL
#define SUPPORT_IR_SPEEDUP //speedup ir when continuing press mantis:0200688 
#ifdef SUPPORT_PARTIAL_TS_STREAM_CONTROL
#define SUPPORT_DYNAMIC_TURNONOFF_TSBUFFER
#define SUPPORT_DYNAMIC_SWITCH_STREAM_INPUT //liying add for switch stream input in dual dvbs
#endif

//===================================================================
//                           System Config
//			 These settings are supposed not to be modified
//===================================================================
#define SUPPORT_DVB
#define SUPPORT_HW_DEMUX
//#define SUPPORT_SERVICE_SETTING
#ifdef SUPPORT_SERVICE_SETTING
#define SUPPORT_SERVICE_VOLUME
#define SUPPORT_SERVICE_SUBTITLE
#define SUPPORT_SERVICE_AUDIO
#endif
/* auto compare with current playing file */
//#define COMPARE_DECODER_CHECKSUM
//#define IPC_SOLUTION
#define JPEG_LOGO
#define SUPPORT_EXTEND_CODEPAGE
//#define SPPORT_FAVLISTEDIT   //zhangwei add 20110324 for mantis 128211
#ifndef USE_ROMTER //to load auto configuration, glenn@20100602
#define EEPROM_FLASH_STORE_INFO //for flash storage
#ifdef EEPROM_FLASH_STORE_INFO
//#define SUPPORT_CAPTURE_LOGO
#endif
#endif

#define SUPPORT_NVM_DRV_VER1  //for new flash driver nvm_flash, robert.chang, 2009-03-23
#ifdef EEPROM_FLASH_STORE_INFO
	#define DEFAULT_DB_IN_BIN
	//#define SAVE_DB_IMMEDIATELY	// remove for mantis 172553
	#define MAINMENU_SAVE_DB
#endif

#ifdef USE_ROMTER
	#define DEFAULT_DB_IN_BIN
#endif

#ifdef NET_TV_SUPPORT   //sha.zong
	//#define IPTV_NET_TEST
#endif
#define PSI_LISTENER_SPEC //yukai 110104 added for mantis 0120215
//zhaorui 100526 added for mantis 0089371 +
#define SUPPORT_SUPER_PASSWARD
#ifdef SUPPORT_SUPER_PASSWARD
#define CONST_PASSWARD	{8,8,8,8}
#endif
//zhaorui 100526 added for mantis 0089371 -
//#define  SUPPORT_NEW_WAKEUPTIMER_CHANGE_STRTETIME //rlchen add 20110915

// show 1st video I frame immediately, mantis 99140
#define SUPPORT_ENABLE_1STVIDEO
#define SUPPORT_NONBLOCK_CONNECT_SIGNAL
//#define SUPPORT_SERVICE_REPLACEMENT		//glenn 20090813
//#define SUPPORT_DOLBY_CERTIFICATION_CERT
#ifdef SUPPORT_DOLBY_CERTIFICATION_CERT //support dolby certification , glenn@20120322
#define SUPPORT_DOLBY_CERTIFICATION_LOGO //runqian add 101202 for mantis bug 0112747
#endif
#ifdef CIPLUS_SUPPORT
//#define CIPLUS_TEST
//#define CIPLUS_HDCP_TEST
#endif
#define CHECK_SUM	//zhangjie add for check sys
//#define TEST_ZAPPING_TIME

/********************************************************************
Function: Video output mode
Description:
********************************************************************/
//#define SUPPORT_CVBS_RGB_ONLY
//#define SUPPORT_CVBS_YUV_ONLY
//#define SUPPORT_CVBS_RGB_YUV_ONLY
//#define SUPPORT_4_3_ASPECT_RATIO_UNDER_HIGH_RESOLUTION//zhaorui 110706 add for mantis 0139183
/********************************************************************
Function: TS Demux Setting
Description:
********************************************************************/
#define TS_SERIAL_MODE 1
#define TS_PARALLEL_MODE 0
#define MPEG_TS_SERIAL_MODE 0
#define MPEG_TS_PARALLEL_MODE 1

#define MPEG_TS_OUT  MPEG_TS_SERIAL_MODE
//#define MPEG_TS_OUT  MPEG_TS_PARALLEL_MODE
#if (MPEG_TS_OUT == MPEG_TS_SERIAL_MODE)
#define TS_SERIAL_PIN		1
#else
#define TS_SERIAL_PIN		0
#endif
#define TS_INTERFACE_MODE	TS_SERIAL_MODE
//#define TS_INTERFACE_MODE	TS_PARALLEL_MODE


//#define SUPPORT_DVB_CC
//===================================================================
//                           General SPEC
//===================================================================
/********************************************************************
Function: DTG
Description:
********************************************************************/
//#define SUPPORT_DTG_CERTIFICATION
#ifdef SUPPORT_DTG_CERTIFICATION
#define SUPPORT_MHEG5
//#define SUPPORT_SKIP_SPECIFIED_CHARS	//yhong 2011.04.15 for 0131273, filter 0-0x1f char.del by all use.//skip 0x01 ~ 0x1f when DTG certification
#define SUPPORT_SHOW_0X00_DRAW_STR		  //replace 0x00 in the string
//#define CHECK_HIDDEN_SERVICE // BRAD 090803 : used to replace Other Service Type
//#define SUPPORT_DYNAMIC_MULTIPLEXES //add to update other TS' services dynamically -glenn 20090826
#define SUPPORT_SKIP_NEWLINE_IN_SERVICE_NAME
//#define SUPPORT_PROGNAME_WELGLA
//#define SUPPORT_PREFERRED_NAME //BRAD 090903 //To parsing preferred name list and preferred name identifier
#define SUPPORT_DTG_CMG //BRAD to parse FTA content management descriptor
#endif

/********************************************************************
Function: Z-Book
Description:
********************************************************************/

/********************************************************************
Function: Nordig
Description:
********************************************************************/
//#define SUPPORT_NORDIG

/********************************************************************
Function: for LCN conflict
Description:  Target Region Descriptor support
********************************************************************/
//#define SUPPORT_LCN
#ifdef SUPPORT_LCN
//#define SUPPORT_LCN_ONOFF	//JennyLee 20090911 added
#endif /*SUPPORT_LCN*/



//===================================================================
//                           Customer SPEC
//===================================================================
/********************************************************************
Function: UI
Description:
********************************************************************/
//#define SUPPORT_OSD_FIXED_REGION_SIZE //curtis add for OSD region size fixed without scaling at any situation
//#define SUPPORT_OSD_AUTO_SEL_MIXER // curtis add for OSD mixing layer auto selection
#define GUI_RENAME_V2 //2009-09-28 jilin fix channel rename new style UI.
#define SUPPORT_INITINSTALL //2009-10-13 jilin fix add initinstall page.
//#define SHOW_MSG_ON_ISOMPEG4_VIDEO//BRAD MOD for mantis 68587
#define SUPPORT_REMIND_DB_FULL //add by ztli 091110 for mantis bug 69879
#define SUPPORT_SERVICEPROVD_NAME  // StanShih 2009/11/26
#define SUPPORT_NETWORK_NAME  // StanShih 2009/11/26
#ifdef SUPPORT_NETWORK_NAME
#define SUPPORT_MULTILINGUAL_NETWORK_NAME //runqian add 100408 for mantis bug 83021
#endif

//#define SUPPORT_TIME_ON_OSD	//zhaorui 100612 added for mantis 0090346
#define SHOW_LOGO_BOOTUPTIME	// Show logo first at bootup time
#ifndef SUPPORT_4MB_FTA
#define SUPPORT_USER_DEFINE_PID	//allow user modify pid, only work on dvbs
#endif
//#define SUPPORT_AUS_RATING_SET //zhaorui 100823 added for mantis 0101295
#ifndef SUPPORT_4MB_FTA
#define SUPPORT_FAVTYPE_EDITOR
#endif
/********************************************************************
Function: Scan
Description:
********************************************************************/
//#define SUPPORT_SCRAMBLE_CHANNEL_RESPECT_SDT //Rex_100824: respect free_ca_mode in SDT if enable
#define SUPPORT_UPDATE_LAST_PROG_NAME //evo add mantis 0171573
/********************************************************************
Function: EPG
Description:
********************************************************************/
#define EPG_NOW_NEXT_MODE
#define EPG_DAILY_MODE
#define EPG_WEEKLY_MODE
#define SUPPORT_DVB_PARENTAL_RATING
//#define SUPPORT_CHECK_LOCK_EVERYTIME		//to check parental lock every time, glenn@20100603
#define SUPPORT_EPG_DYNAMIC_REFRESH
#define SUPPORT_EPG_CONTENT_DESCRIPTOR
#define SUPPORT_EPG_EXTEND_DES_ITEMS
//#define SUPPORT_EPG_BY_FAV_GROUP	//Mika 090818 added
#define SUPPORT_EPG_QUICK_ZAPPING	//Mika_100406: quick zapping with osd update until threshold is made

/********************************************************************
Function: Subtitle
Description:
********************************************************************/
#define SUPPORT_SUBTITLE
#ifdef SUPPORT_SUBTITLE// yhong 2012.02.21 for 0169265, review TSF Mode and subtitle error.
#define	SUPPORT_DVB_SUBTITLE
//#define	SUPPORT_SBTVD_SUBTITLE
//#define NOT_SUPPORT_REGIONNUM_CLEAR_REGION//liying add for mantis 165369,default disable define
#define	SUPPORT_HARD_HEARING	//zhaorui 100604 added for mantis 0090740
#endif

/********************************************************************
Function: Teletext
Description:
********************************************************************/
#define SUPPORT_TTX
#ifdef SUPPORT_TTX
#ifdef SUPPORT_SUBTITLE
#define SUPPORT_TTX_SUBTITLE
#endif
#define SUPPORT_NEW_TTX_FONT_STYLE
#define SUPPORT_TVVBI_TTX
#define SUPPORT_SKIP_NO_DATA_PAGE  //hxb added for support skip the page which is not in cache. 070517
#define SUPPORT_FLOF_NAVIGATION //willis added for support 4 color key navigation 071026
#define SUPPORT_TTX_SUBPAGE //willis 080428 add for subpage viewer
#define SUPPORT_TTX_MERGE_MISSLINE
#endif
#if defined(SUPPORT_TTX)||defined(SUPPORT_SUBTITLE)
#define DVB_OSD2_MODULE_CTRL//for MH5/TTX/Sub
#endif

/********************************************************************
Function: Sysset and Database
Description:
********************************************************************/
#define SUPPORT_HIDE_NOPROG_SAT//jgliu 2011-01-04 add for mantis:119998
#define	SUPPORT_DB_EXPORT
#define SUPPORT_USB_UPLOADER
#ifdef SUPPORT_USB_UPLOADER
#define SUPPORT_UPGRADE_DB_SYSSETTING
#endif
#define USE_LARGE_FLASH

#define SUPPORT_TIMER_USING_AUTONOMY_NVM_SEGMENT
#ifdef SUPPORT_TIMER_USING_AUTONOMY_NVM_SEGMENT
//#define SUPPORT_TIMER_SAVE_EVENT_NAME
#endif

/********************************************************************
Function: Multimedia
Description:
********************************************************************/
#define SUPPORT_MULTIMEDIA
//#define SUPPORT_CUSTOMIZE_MOUNT_NAME
#ifdef SUPPORT_MULTIMEDIA
//#define SUPPORT_DIVX_DRM //rlchen 20100908 add for DIVX
#define SUPPORT_TSF_PSIMON
#define MP4_SUB_PREFER_LANG
#define EBOOK_PREFER_LANG    //20101214 add by zhangwei for mantis 0114625
//#define MP4_SUB_SETFONT_TYPE	//wujun add 2010/11/10
//#define SUPPORT_EXIT_KEY_BACK_TO_TV_MODE//fangxu 110711 added for mantis 141802
#define SUPPORT_SOURCE_KEY_TO_SWITCH_SOURCE	//dongsheng 2012.01.31 add for mantis 163784
#endif

/********************************************************************
Function: Network
Description:
********************************************************************/
#ifdef NET_SUPPORT
#define SUPPORT_ETHERNET
#ifdef	SUPPORT_ETHERNET
	#define SUPPORT_ETHERNET_CONFIG
	#ifdef SUPPORT_ETHERNET_CONFIG
		#define SUPPORT_PING_TEST
		#define NET_PROXY_SUPPORT	//0189002
	#endif

	#define SUPPORT_NETCA
//	#define SUPPORT_NET_TESTING
//	#define SUPPORT_NET_UPGRADE
#endif
#ifdef GMOTE_SUPPORT
	//#define SUPPORT_GMOTE_ENTERTAINMENT
#endif
#endif

//yi.ding 20130418 add for mantis:0211711 {{
#if defined(NET_SUPPORT)  &&  defined(NET_WIFI_SUPPORT)       
//#define SUPPORT_WIRE_WIFI_AUTO_LINK 
#endif
//yi.ding 20130418 add for mantis:0211711 }}


/********************************************************************
Function: USB/CARD DEVICE PORT define
Description: wangshengchun 20100529 add
if none of the four defination opened,means support dual usb and card
********************************************************************/
//#define SUPPORT_DUAL_USB
//#define SUPPORT_SINGLE_USB_AND_CARD
//#define SUPPORT_SINGLE_USB_ONLY
//#define SUPPORT_CARD_ONLY

/********************************************************************
Function: PVR
Description:
********************************************************************/
#ifdef SUPPORT_PVR_VER1
#define SUPPORT_PVR
#endif
#ifdef SUPPORT_PVR
	#ifdef SUPPORT_DUAL_NIM
		#define SUPPORT_DUAL_PVR	//for Timeshift A Record B
	#endif
	//#define SUPPORT_REC_PAUSE
	#define SUPPORT_PVR_CONTENT_PROTECT	//wsc 20110401 modify to default open
	#define SUPPORT_REC_A_PLAY_B
	#define AUTO_REC_FINISH_POP_STANDBY_MSG	//zhaorui 100429 added for mantis 0085688
	#define SUPPORT_REMINDER_DEVICE_SPEED  //wsc 20100512 add .if device speed not enough,a selection reminder will pop up
	#define	SUPPORT_DVB_DEVICE_LIST		//zhaorui 100601 added for mantis 0090275
	#define SUPORT_PVR_INFO_BANNER_TIMEOUT  //pinchao 20100603 add for mantis 0090560
	#define SUPPORT_SET_PREFER_STORAGE  //wsc 20100612 add for easyly open/close prefer storage item(bug id 90799)
	#if defined (SUPPORT_SINGLE_USB_ONLY) || defined (SUPPORT_CARD_ONLY) || defined(SUPPORT_DUAL_PVR) //wsc 20100623 add for 92200	//2011/4/26 wenjing.yang modified for mantis 0132693
	#undef SUPPORT_SET_PREFER_STORAGE
	#endif
	#define SUPPORT_SET_PVR_CONFIG  //wsc 20100610 add for utek(bug id 90799)
	//#define PVR_TS_GENERAL_EXT_NAME	//for using .ts for TS-PVR
	#define SUPPORT_REC_FILENAME_CAT_EVENT_NAME
	#define PREFER_PARTITION_STORAGE //haoye_20110401 added for storing the preference parrtions selected by user.(mantis 129349)
#elif defined SUPPORT_PVR_VER2
	#define AUTO_REC_FINISH_POP_STANDBY_MSG
	//#define SUPPORT_DISP_REC_IN_CHANNEL_LIST//zhaorui 110726 add for mantis 0141469
	#define SUPPORT_RECORDINGLIST_CONFIRMMENU//qiuwei097add for maints 0149714
	#ifndef SUPPORT_NEW_WAKEUPTIMER_CHANGE_STRTETIME
	#define SUPPORT_TIMEOFFSET_AUTO_ADJ
	#endif
#ifdef CIPLUS_SUPPORT
	#define SUPPORT_CI_PLUS_PVR_V12	//for CI+
	#define SUPPORT_CI_PLUS_PVR_V13	//for CI+
#endif
	#define DISPLAY_RECORDING_PROG_INFO //haoye_20110919 added for mantis 149306
#define REC_ICON_STYLE1//haoye_20110923 added for mantis 153148
	#define GET_PREFER_PARTITION_USE_TASK //lizhan 20111125 added for mantis 0160455 insert large HD,set default prefer partion need long time,cause a long time system do not reponse IR operate.
#ifndef SUPPORT_4MB_FTA
#ifndef SUPPORT_DUAL_NIM
	#define SUPPORT_1TP_DUAL_RECORDER		//mika_0319: 169898
#endif
#endif
	#define SUPORT_PVR_INFO_BANNER_TIMEOUT	//mika_0406: 175025
	#define SUPPORT_REC_PAUSE
#endif // #define SUPPORT_PVR
#define SUPPORT_DISK_MANAGER  //pinchao 20091118 add for Disk Manager
#if (defined SUPPORT_DISK_MANAGER) || (defined SUPPORT_PVR)		//dongsheng 2010/06/18 add for device info in mantis 88892
	#define SUPPORT_SHOW_DEVICE_INFORMATION
#endif
#define SUPPORT_VIDEO_RESUME_PLAY	// added by duping for resuming play video in "Movie" at 2011-08-29.(mantis 150718)
#ifdef SUPPORT_VIDEO_RESUME_PLAY
#define SUPPORT_RECORD_VIDEO_RESUME_PLAY //lvfengkai 2012/12/28 add for mantis 0203743 
#endif

//wsc 20120903 add begin{{{
#ifdef SUPPORT_HID_INPUT
//#define HID_INPUT_DAEMON_TEST
#endif
//wsc 20120903 add end}}}

#ifdef SUPPORT_SAT2IP//jgliu 2013-06-27 add for SAT>IP.
#define SUPPORT_PVR_FOR_SAT2IP
#endif

/********************************************************************
Function: DSP
Description:
********************************************************************/
#define SUPPORT_AUDIO_AC3
#define SUPPORT_MP3_LRCDISPLAY	//dongsheng 2010/03/14 add for lrc display
//#define SUPPORT_AD_DESCRIPTION //open to support Audio description, glenn@20091218
//#define SUPPORT_DUAL_DSP //[kiwi 20091214]
//#define SUPPORT_AC3_ENCODE //glenn@20120112

/********************************************************************
Function: SPDIF
Description: to contorl delay output of S/PDIF interface
********************************************************************/
//#define SUPPORT_SPDIF_DELAY_CTRL
//#define SUPPORT_SDHD_SIMULCAST_HANDLING

/********************************************************************
Function: CI
Description: Common Interface supported
********************************************************************/
//According the config on product.conf to decide whether the CI definition is supported or not
#if (CI_SUPPORT == 1)
	#define SUPPORT_GENERAL_CI_SYS
#endif
#ifdef SUPPORT_GENERAL_CI_SYS
#define CI_SINGLE_SLOT
#define CI_WITH_PCMCIA
//#define CI_WITH_SPV611
#ifdef CI_WITH_SPV611
//#define CI_SPV611_16MXtal		//for DVB_T used only ,victor.hsu 090220
#define CI_SPV611_21MXtal		//for DVB_S used only ,victor.hsu 090220
#endif
#define NOT_BYPASS_STREAM	//20090331 JennyLee added, stream will always pass through CAM module
#endif

#ifdef CIPLUS_SUPPORT
#ifndef SUPPORT_MHEG5
#define SUPPORT_MHEG5
#define SUPPORT_SUBTITLE_ON_PG
#endif
#endif

/********************************************************************
Function: OTA
Description:
********************************************************************/
//#define USE_NEW_FLASH_LAYOUT

#define DVB_OTA_AUTO_DETECT//yc.guan 2010-04-15 add for SSU
#ifdef DVB_OTA_AUTO_DETECT
#define SUPPORT_INDEPENDENCE_DETECT_OTA
//#define DVB_OAD_AUTO_DETECT
#endif
#ifdef NET_ISP_SUPPORT
#define SUPPORT_ISP_UPGRADE   //pinchao 20100820 add for mants bug 0100718 (isp upgrade)
#ifdef SUPPORT_ISP_UPGRADE
	//#define SUPPORT_ISP_DUMMY_UPGRADE
	#define ISP_UPGRADE_TEST_FOR_DVB
#endif
#endif
//#define SUPPORT_SCAN_SSU_PROGRAM_TEST  //ycguan add for scan ssu's program ,100610

/********************************************************************
Function: IR
Description:
********************************************************************/
//#define IR_PLAYPAUE_COMBINE//default off for ir401 //qiuwei20100520add
#define IR_SWITCH_RESOLUTION //Haoye_100420 added: for switch resolution by IR key.
#define HOT_KEY_SWITCH_ASPECT  //wsc 20100529 add for hot key switch aspect ratio
//#define UP_DOWN_COMBINE_WITH_CH  //hyzhang 20100516 add for mantis bug 0089520
//#define  LEFT_RIGHT_COMBINE_WITH_VOL 	//hyzhang 20100516 add for mantis bug 0089521
#ifndef LEFT_RIGHT_COMBINE_WITH_VOL //yhong 2010.11.24 for 0102020, Volue can be changed at any time .
//#define	SUPPORT_HOT_KEY_VOL_CACHE
#endif
#define ZOOM_RED_COMBINE  //wsc 20100611 add for utek
#define SUPPORT_DVB_ZOOM_FUNC	//Mika_100618: modify dvb zoom func. flow
//#define SUPPORT_TOGGLE_ZOOM_FUNC	//Mika_100630
//pingchi 20100726 added for mantis bug 97117 ++
#ifndef SUPPORT_DTG_CERTIFICATION
#define SUPPORT_HOT_KEY_TIMER
#endif
#define IR_SET_SPEEDUP			//zhangjie add for mantis	0149901 speedup
//pingchi 20100726 added for mantis bug 97117 --

/********************************************************************
Function: GPIO/IOP/I2C
Description:
********************************************************************/
//#define SUPPORT_RISC_I2C //Kaney mark
#ifdef SUPPORT_RISC_I2C
#define SUPPORT_COMMON_I2C
#define NEW_RISC_I2C_VER //peiqin.dong 2011-06-01 for 0134684
#define SUPPORT_RISC_I2C_SPEED_FINTUNE//peiqin.dong 2011-05-26 add  0134684
#else
#define RISC_SET_IOP_I2C_GPIO	//2010/11/10 wenjing.yang added for mantis 0110106
#define SUPPORT_COMMON_I2C
#define NEW_RISC_I2C_VER //Kaney add
#endif

#define SUPPORT_GPIO_MONITOR //2009/12/09,Haoye added for gpio monitor.

//BRAD MOD+ : If you want to follow AFD spec, please choose type 2.
//#define SCART_PIN8_TYPE_1 //BRAD : control the high/low of pin8 by the result of Aspect Ratio in Menu setting
#define SCART_PIN8_TYPE_2 //BRAD : control the high/low of pin8 by the result of Decoder Format Conversion
//BRAD MOD-
//#define REC_ICON_STYLE1//haoye_20110923 added for mantis 153148

/********************************************************************
Function: HDMI settings
Description:
********************************************************************/
//#define SUPPORT_HDMI_DEEP_COLOR_SETTING  //wangshengchun 20100604 add for 90796
//#define SUPPORT_HDMI_COLOR_SPACE_SETTING  //wangshengchun 20100604 add for 90796
#define HDMI_AUTO_DETECT_PREFERMODE //kf 20101019 mantis107224
#define SUPPORT_HDMI_AUDIO_OUTPUT//Qiuwei 20101211 fix mantis ID :0111145  HDMI OUTPUT support ,MUTE,PCM,RAW
//#define SUPPORT_HDMI_HAS_16_9_UNDER_SD_RESOLUTION //Rex_100927 add for HDMI support 16:9 mode when SD output resolution
#define SUPPORT_ENABLE_JUDGE_HDMIRESOLUTION       //lijin add for judge the HDMI resolution Mode
#define AUTO_HDCP //glenn@20120823 enable HDCP ON when content protect
#define HDMI_FACTORY_TEST	//2012/10/29 wenjing.yang added for hdmi factory test, mantis 0196495 
/********************************************************************
Function: Stanby mode configuration
Description:
********************************************************************/
//pinchao 20100622 add for mantis bug 0092512+
#define SUPPORT_FAKE_STANDBY
#ifdef SUPPORT_FAKE_STANDBY
	#define RECTIMER_ENTER_FACK_STANDBY_MINUTE	(3)
#endif
//pinchao 20100622 add for mantis bug 0092512-
//#define SUPPORT_AUTOSEARCH_IN_STANDBY
//#ifndef SUPPORT_CEC
#define SUPPORT_IOP2_STANDBY_WAKEUPTIMER //lizhan 2010-3-19 add for in standby use iop2 wakeuptimer mantis id=0081196
//#endif
#ifdef SUPPORT_IOP2_STANDBY_WAKEUPTIMER
	#define IOP2_STANDBY_WAKEUP_EARLIER_MINUTE	(2)	//zhaorui 100519 added for mantis 0087891
#endif
#define SUPPORT_REC_CONFIRM_POWER		//add by ztli 2011/05/03 for 133034
#define AUTO_IDENTIFY_1500C_IC_IOP_PWR_CTRL //lizhan 2011/2/26 mantis 0126083 add for automatic identification sphe1500 ic iop power control.

#define SUPPORT_EUP_AUTO_STANDBY //added by duping at 2011-03-22 for mantis 128687:for europe green power ui spec :if user do not press ir key whithin 3 hours,the stb will enter standby.
#ifdef SUPPORT_EUP_AUTO_STANDBY
#define EUP_REMINDER_UI_TIME_OUT   (2)   // 2 minute
#endif

/********************************************************************
Function: ANTENNA
Description:
********************************************************************/
#ifdef SUPPORT_GPIO_MONITOR
    //#define SUPPORT_ACTIVE_ANTENNA
    #ifdef SUPPORT_ACTIVE_ANTENNA
	#ifdef GPIO_LNB_SHORT_DETECT
	        #define ACTIVE_ANTENNA_SHORT_PROTECT
	#endif
    #endif
#endif
//pingchi 20100819 added for mantis bug 93722 --
//#define LNB_SEL_IN_DRIVER//ycguan for bug0086338  ,100504
#define SUPPORT_USALS

/********************************************************************
Function: OTHER
Description:
********************************************************************/
#ifdef OSD_GAME_SUPPORT //define in product.conf
	#define SUPPORT_NATIVE_GAME_SAVE
#endif

#if defined(MD_GAME_SUPPORT)  ||  defined(NES_GAME_SUPPORT)        //lijin Add  for open Game pad
	//wsc 20120903 add begin{{{
	#ifdef SUPPORT_HID_INPUT
	//#define NEED_CONTROL_NES_BY_USBHID
	//#define HID_GAMEKEY_MAP2_IOPDATA4
	#endif
	
	#ifndef NEED_CONTROL_NES_BY_USBHID
	//#define RISC_CFG_IOP_USE_GPAD_GPIO									//2010/5/4 added for risc dynamic config game pad pin,do not complier iop.
	#endif
	//wsc 20120903 add end}}}
#endif

#define SUPPORT_VCR //pinchao 20100623 add for mantis bug 0092305
//#define SUPPORT_EQ_SURROUND  //ycguan for bug 0090352
#define VOLUME_MAX_VALUE	(100)	//It's Max Volume, can be 30/60/100 or user define.
//#define SUPPORT_CHN_TIMER_WITH_DUR  //add by ztli 2010/06/09 for mantis bug 90310
#define UI_SPEED_UP //20121113 jiayu mantis 197155 added for disable animation function.

//0216847: The define is always supported
#define SUPPORT_RENAME_KEYBORD_CURSOR_MOVE  //xqhuang 2011/06/29 add for mantis 140328
//#define SUPPORT_DISEQC12_STOP_WHEN_DETECTED_QUALITY	//w.wang 20110629 add for mantis 0140332 +
//#define SUPPORT_SAVE_SHD_TYPE_WHEN_SCAN	//w.wang 20110630 add for mantis 0140334 +
//#define SUPPORT_ARABIC_KEYBOARD	//w.wang 20110713 add for mantis 0140339
//lvfengkai 2012-09-07 add for keyboard+++
#ifdef SUPPORT_ARABIC_KEYBOARD
	 #define ONLY_SUPPORT_ARABIC  0  /* 0 :support any osd language have the arabic keyboard
                                                                     1 :only support arabic keyboard when the osd language is arabic language
	 								  */
#endif

#define SUPPORT_UNIFIED_KEYBOARD   /*at file_player ,only the dvbs can make the unified keyboard */
//lvfengkai 2012-09-07 add for keyboard---

// chun.liu 20130731 add
#ifdef SUPPORT_UNIFIED_KEYBOARD
//#define SUPPORT_RUSSIAN_KEYBOARD
#endif

//lvfengkai 2012-12-10 add for mantis 0202861+++
//#define SUPPORT_FARSI_KEYBOARD 

#ifdef SUPPORT_FARSI_KEYBOARD
	#define ONLY_SUPPORT_FARSI  0/* 0 :support any osd language have the persian keyboard
                                                                 1 :only support persian keyboard when the osd language is persian language
 								  */
#endif
//lvfengkai 2012-12-10 add for mantis 0202861--- 


#define RECORD_PREVIEW_SUPPORT    //haolin  add for mantis 0190357
#define SUPPORT_CVBS_RGB_CONTROL  //haolin add for mantis 0199618


/********************************************************************
Function: TS stream buffer control
Description:
********************************************************************/
// ============== Partial TS stream buffer control ===================== //
#ifdef SUPPORT_PARTIAL_TS_STREAM_CONTROL
	#ifdef IC_SUPPORT_TRIPLE_DEMUX
		#if (PRIMARY_LIVE_INPUT == TS_STRM_CTRL_INPUT)
			#error "SUPPORT_PARTIAL_TS_STREAM_CONTROL primary live input can not equal to stream control input !!!"
		#endif	// #if (PRIMARY_LIVE_INPUT == TS_STRM_CTRL_INPUT)
	#else
		#if (PRIMARY_LIVE_INPUT != SECOND_LIVE_INPUT)
			#error "SUPPORT_PARTIAL_TS_STREAM_CONTROL can not support dual live input!!!"
		#endif	// #if (PRIMARY_LIVE_INPUT != SECOND_LIVE_INPUT)
		#ifdef SUPPORT_PVR_VER2
			#if (LIVE_INPUT_UNIT0 != LIVE_INPUT_UNIT1)
				#error "SUPPORT_PARTIAL_TS_STREAM_CONTROL can not support dual live input!!!"
			#endif	// #if (LIVE_INPUT_UNIT0 != LIVE_INPUT_UNIT1)
			#if (PRIMARY_LIVE_INPUT == TS_STRM_CTRL_INPUT)
				#error "SUPPORT_PARTIAL_TS_STREAM_CONTROL primary live input can not equal to stream control input !!!"
			#endif	// #if (PRIMARY_LIVE_INPUT == TS_STRM_CTRL_INPUT)
		#endif	// #ifdef SUPPORT_PVR_VER2
	#endif	//#ifndef IC_SUPPORT_TRIPLE_DEMUX
#endif	// #ifdef SUPPORT_PARTIAL_TS_STREAM_CONTROL

/********************************************************************
Function: Upgrade USB Loader
Description:
********************************************************************/
#define SUPPORT_UPGRADE_USBLOADER // [chen.jian]mantis 206048

/********************************************************************
Function: IRD Upgrade 
Description:
********************************************************************/
//#define SUPPORT_UART_UPGRADE1 //add by xd.liao
#ifdef SUPPORT_UART_UPGRADE1
	#ifdef USE_ROMTER
		#error "IRD Upgrade only support data from flash,please change it !!!"
	#endif
	#ifndef DIAG_PRINTF_DISABLE
		#error "you must turn 'diag_printf()' off  !!!"
	#endif
	#ifdef AP_MON
		#error "you must turn 'support monitor mode' off  !!!"
	#endif
#endif


/********************************************************************
Function: Sort all program through the Provider name
********************************************************************/
#if DEFAULT_DB_IDX == 1
#define PROVIDER_METHOD_TO_SORT_ALLPROG //20130307 chen.jian add for mantis 0208892
#endif

#endif /* __CUSTOMIZE_H__ */

