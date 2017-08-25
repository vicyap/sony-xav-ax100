/*! \addtogroup UNICODE
 * @{
 */
/*!
*
* @file		charsetdet.h
*
* @brief	This file specifies the character detection APIs
*
* @note		Copyright (c) 2009 Sunplus Technology Co., Ltd. \n
*			All rights reserved.
*
* @author
*
******************************************************************************/
#ifndef __CHARSETDET_H__
#define __CHARSETDET_H__
#if 1
typedef enum {
	 /* One byte */
	FONT_TT = -2,		/* TT font */
	FONT_CC708 = -1,	/* CC font ,refer to CEA 708-C*/
	FONT_BASIC = 0,     /* The basic fontbase */
	FONT_ENGLISH,		/* english, ISO/IEC 8859-1*/
	FONT_ISO8859_1=FONT_ENGLISH,	
	FONT_SPANISH=FONT_ISO8859_1,	/* spanish, ISO/IEC 8859-1*/
	FONT_PORTUGESE=FONT_ISO8859_1,  /* portugese, ISO/IEC 8859-1*/
	FONT_ISO8859_2,
	FONT_ISO8859_3,
	FONT_ISO8859_4,
	FONT_RUSSIAN,				    /* russian, ISO/IEC 8859-5*/
	FONT_ISO8859_5 = FONT_RUSSIAN,
	FONT_ARABIC,
	FONT_ISO8859_6 = FONT_ARABIC,
	FONT_GREECE,				    /* greece, ISO/IEC 8859-7*/
	FONT_ISO8859_7 = FONT_GREECE,
	FONT_ISO8859_8,				    /* hebrew, ISO/IEC 8859-8*/
	FONT_ISO8859_9,
	FONT_ISO8859_10,
	FONT_THAI,
	FONT_ISO8859_11 = FONT_THAI,
	FONT_ISO8859_13,
	FONT_ISO8859_14,
	FONT_ISO8859_15,
	FONT_GERMAN=FONT_ISO8859_15,	/* german, ISO/IEC 8859-15, added by junluo 090107*/
	FONT_ITALIAN=FONT_ISO8859_15,	/* italian, ISO/IEC 8859-15, added by junluo 090107*/
	FONT_FRENCH=FONT_ISO8859_15,	/* french, ISO/IEC 8859-15*/
	FONT_ISO8859_16,
	FONT_ISO6937,				    /* refer to ISO/IEC 6937*/	
	FONT_ATSC_MODE_0X27,            /* Actually used for ATSC MODE 0x27 */
	/*
	** These following micros distinguishes between signal byte 
	** code set and double bytes font code set 
	*/
	FONT_VIETNAMESE,	
	FONT_WINDOWS_1250,
	FONT_WINDOWS_1251,
	FONT_WINDOWS_1252,
	FONT_WINDOWS_1253,
	FONT_WINDOWS_1254,
	FONT_WINDOWS_1255,
	FONT_WINDOWS_1256,
	FONT_WINDOWS_1257,
	FONT_WINDOWS_1258,
	FONT_PERSIAN,	               /* Persian, added by kh.wang */
	FONT_HINDI,
	FONT_SINGLE_BYTE_LAST = FONT_HINDI,
	/* Double bytes char set */
	FONT_GB2312,
	FONT_SIMPLECH=FONT_GB2312, 			/* simple chinese, GB2312*/
	FONT_BIG5,
	FONT_TRADITIONALCH=FONT_BIG5,		/* traditional chinese, BIG5*/
	FONT_KSC5601_1987,
	FONT_KOREAN=FONT_KSC5601_1987,		/* korean, KSC 5601-1987*/
	FONT_JIS_X0201,		/* Japanese */
	FONT_DOUBLE_BYTE_LAST = FONT_JIS_X0201,
	/* Unicode char set, until now process UTF-8/16 */	
	FONT_CUSTOM_USER_DEFINED1,
	FONT_CUSTOM_USER_DEFINED2,
	FONT_CUSTOM_USER_DEFINED3,
	FONT_CUSTOM_USER_DEFINED4,
	FONT_CUSTOM_USER_DEFINED5,
	FONT_NATIVE_LAST,
	FONT_UTF8,
	FONT_UTF16,
	//qiuwei qiuwei20100609add for maints 0090280 start{{{
	#if 0//def SUPPORT_EXTEND_CODEPAGE
	//wsc 20090217 add explanation
	FONT_CP1250,	//Central and East European Latin<==>ISO8859-2
	FONT_CP1251,	//Cyrillic<==>ISO8859-5
	FONT_CP1252,	//West European Latin<==>ISO8859-1
	FONT_CP1253,	//Greek<==>ISO8859-7
	FONT_CP1254,	//Turkish<==>ISO8859-9
	FONT_CP1255,	//Hebrew<==>ISO8859-8
	FONT_CP1256,	//Arabic<==>ISO8859-6
	FONT_CP1257,	//Baltic<==>ISO8859-13
	FONT_CP1258,	//Vietnamese 
	#endif
	//qiuwei qiuwei20100609add for maints 0090280 end}}}
	FONT_LAST
} LANGName_t;
#else
typedef enum {	
    FONT_BASIC        = 0,
    FONT_ENGLISH      = (1 << 0),
    FONT_BIG5         = (1 << 1),
    FONT_GB2312       = (1 << 2),
    FONT_UTF8         = (1 << 3),
    FONT_ISO8859_5    = (1 << 4),
    FONT_ISO8859_7    = (1 << 5),
    FONT_ISO8859_8    = (1 << 6),
    FONT_ISO8859_9    = (1 << 7),
    FONT_WINDOWS_1251 = (1 << 8),
    FONT_WINDOWS_1252 = (1 << 9),
    FONT_WINDOWS_1253 = (1 << 10),
    FONT_WINDOWS_1255 = (1 << 11),

    kEncodingAll = (FONT_ENGLISH | FONT_BIG5 | FONT_GB2312 | FONT_UTF8 |  FONT_ISO8859_5 | FONT_ISO8859_7 | FONT_ISO8859_8 | FONT_ISO8859_9 |  FONT_WINDOWS_1251 | FONT_WINDOWS_1252 | FONT_WINDOWS_1253 | FONT_WINDOWS_1255),
} LANGName_t;
#endif
/******************************************************************************/
/*!
 * @brief	Guess a codepage by a raw character stream
 *
 * @param	rawchr raw character stream
 * @param	size of raw character stream
 *
 * @return	guessed code page number
 *******************************************************************************/
#ifdef __cplusplus
extern "C"{
#endif

int CS_GuessCodePage(const unsigned char *rawchr, int size);

#ifdef __cplusplus
}
#endif

/*! @} end of defgroup Unicode */

#endif // __CHARSETDET_H__

/*! @} end of addtogroup UNICODE */
