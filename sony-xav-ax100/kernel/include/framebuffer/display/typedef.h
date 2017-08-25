/**
 * @file typedef.h
 * @brief Basic datatype definition
 * @author kt.huang@sunplusmm.com
 */
#ifndef _TYPEDEF_H_
#define _TYPEDEF_H_

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __KERNEL__
#include <linux/types.h>
#else
#include <stdint.h>
#endif

/** @defgroup typedef Basic Types
 *  @{
 */

#ifndef __SP_INT_TYPES__
#define __SP_INT_TYPES__

#ifndef GSL_INCLUDE
typedef unsigned char			BYTE;
typedef unsigned short			WORD;
typedef unsigned long			DWORD;

typedef unsigned long long 		UINT64;
typedef unsigned int   			UINT32;
typedef unsigned short 			UINT16;
typedef unsigned char 			UINT8;

typedef long long 				INT64;
typedef int   					INT32;
typedef short 					INT16;
typedef char					INT8;

typedef signed long long		SINT64;
typedef signed int				SINT32;
typedef signed short			SINT16;
typedef signed char				SINT8;
typedef char 					Boolean;
#endif

#endif

#ifndef GSL_INCLUDE
/** @brief SP_BOOL is a boolean value */
typedef UINT32             SP_BOOL;
/** @brief SP_CHAR is a 8 bits character */
typedef char               SP_CHAR;
/** @brief SP_WCHAR is a 16 bits wide character (for unicode) */
typedef UINT16             SP_WCHAR;
/** @brief HANDLE is a 32 bits integer (it is a uniform HANDLE) */
typedef void*              HANDLE;
#endif

#ifndef NULL
/** @brief A value that represents NULL. */
#define NULL ((void *)0)
#endif

/** @brief A value that represents a "OK" result. */
#define SP_OK              0
/** @brief A value that represents a "FAIL" result. */
#define SP_FAIL            1
/** @brief A value that represents a "SUCCESS" result */
#define SP_SUCCESS         0
/** @brief TRUE value */
#define SP_TRUE            1
/** @brief FALSE value */
#define SP_FALSE           0

#define DO_MEMLOG	0	/* a flag to turn on the memory log ! */

/** @} */


/** @defgroup spErrCode Error Codes
 *  @{
 */

enum {
	SP_ERR = 1,                  /*!< @brief Generic error */
	SP_ERR_MEM,                  /*!< @brief Insufficient memory */
	SP_ERR_LIST_IS_EMPTY,        /*!< @brief The list is empty */
	SP_ERR_NO_NODE,              /*!< @brief Cannot insert message to global message queue */
	SP_ERR_FNT_OPEN_FAIL,        /*!< @brief Open font error */
	SP_ERR_FNT_READ_FAIL,        /*!< @brief Read font error */
	SP_ERR_FNT_SIGNATURE,        /*!< @brief Font signature error */
	SP_ERR_FNT_READ_IDX_FAIL,    /*!< @brief Font read index error */
	SP_ERR_FNT_SEEK_FAIL,        /*!< @brief Font seek error */
	SP_ERR_FNT_CODEPAGE,         /*!< @brief Font codepage error */
	SP_ERR_OPEN,                 /*!< @brief Open error */
	SP_ERR_READ,                 /*!< @brief Read error */
	SP_ERR_WRITE,                /*!< @brief Write error */
	SP_ERR_BMPFMT_MISMATCH,      /*!< @brief The bitmap format mismtach */
	SP_ERR_EXCEED_MAX_DRIVER,    /*!< @brief Exceed maxinum of drivers */
	SP_ERR_TMR_POSTPONE,         /*!< @brief Postpone the timer event. */
	SP_ERR_PARAM,                /*!< @brief Parameter to the function is incorrect. */
	SP_ERR_NOT_FOUND,            /*!< @brief Cannot find data */
	SP_ERR_ITEM_EXIST,           /*!< @brief Item is exist */
	SP_ERR_BUSY,                 /*!< @brief Resource is busy */
	SP_ERR_FORMAT_UNSUPPORTED,   /*!< @brief Format unsupported */
	SP_ERR_NOT_IMPLEMENT,        /*!< @brief The service is not implemented */
	SP_ERR_EOF,                  /*!< @brief Read EOF error */
	SP_ERR_STOP,                 /*!< @brief The service is stop error */
	SP_ERR_UNHANDLED,            /*!< @brief Message unhandled */
	SP_ERR_SETTINGS_SIGNATURE,   /*!< @brief Settings signature error */
	SP_ERR_SETTINGS_VERSION,     /*!< @brief Settings version error */
	SP_ERR_HW_LIMIT,             /*!< @brief Hardware limitation error */
	SP_ERR_RESOURCE_NOT_FOUND,   /*!< @brief Resource not found */
	SP_ERR_JPEG_DECODE,          /*!< @brief Jpeg decode error */
	SP_ERR_JPEG_FORMAT,          /*!< @brief Jpeg format error */
	SP_ERR_SPACE_NOT_ENOUGH,     /*!< @brief Storage is full */	
	SP_ERR_NOT_SUPPORTED,		 /*!< @brief Not supported function */
	SP_ERR_MAX                   /*!< @brief The last error message */
};

/** @} */


/** @defgroup bitmap Module Bitmap
 *  @{
 */

/** @brief Bitmap format: YUV422 (YYYYYYYYUUUUVVVV) */

enum {
	SP_BITMAP_RGB565 = 0,
	SP_BITMAP_RGAB5515,
	SP_BITMAP_ARGB1555,
	SP_BITMAP_ARGB4444,
	SP_BITMAP_RGB888,
	SP_BITMAP_ARGB8888,
	SP_BITMAP_BGR565,
	SP_BITMAP_RGB555,
	SP_BITMAP_BGAR5515,
	SP_BITMAP_ABGR1555,
	SP_BITMAP_ABGR4444,
	SP_BITMAP_BGR888,
	SP_BITMAP_ABGR8888,
	SP_BITMAP_1BPP,
	SP_BITMAP_2BPP,
	SP_BITMAP_4BPP,
	SP_BITMAP_8BPP,
	SP_BITMAP_YCbCr,
	SP_BITMAP_YUV,
	SP_BITMAP_YCbYCr,
	SP_BITMAP_YUYV,
	SP_BITMAP_4Y4U4Y4V,
	SP_BITMAP_4Y4Cb4Y4Cr,
	SP_BITMAP_YCbCr400,
	SP_BITMAP_YUV400,
	SP_BITMAP_YCbCr422,
	SP_BITMAP_YUV422,
	SP_BITMAP_YCbCr444,
	SP_BITMAP_YUV444,
	SP_BITMAP_YCbCr420,       /*!< @brief Ceva decoder output format */
	SP_BITMAP_YUV420,         /*!< @brief Ceva decoder output format */
	SP_BITMAP_SEMI400,
    SP_BITMAP_SEMI420,
    SP_BITMAP_SEMI422,
    SP_BITMAP_SEMI444,
    SP_BITMAP_RGBA8888,
    SP_BITMAP_BGRA8888,
	NUM_BITMAP_TYPE
};


/** @} */


#ifdef __cplusplus
};
#endif


#endif

