#ifndef _HAL_COMMON_H_
#define _HAL_COMMON_H_

#include <framebuffer/display/hardware.h>
#include <framebuffer/display/typedef.h>

/**************************************************************************
 *                           C O N S T A N T S                            *
 **************************************************************************/
#define DEBUG_MSG				0	// 0: disable debug message; 1: enable debug message

typedef enum
{
	/* common status values */
	DRV_SUCCESS,				/*!< successful outcome					*/
	DRV_ERR_FAILURE,			/*!< operation failed					*/
	DRV_ERR_INVALID_HANDLE,		/*!< invalid handle						*/
	DRV_ERR_INVALID_ID,			/*!< invalid identifier					*/
	DRV_ERR_INVALID_PARAM,		/*!< invalid parameter					*/
	DRV_ERR_INVALID_OP,			/*!< requested operation is invalid		*/
	DRV_ERR_MEMORY_ALLOC,		/*!< problem allocating memory			*/
	DRV_ERR_MEMORY_SIZE,		/*!< problem with the size of memory	*/
								/* < supplied							*/
	DRV_ERR_RESOURCE_UNAVAILABLE,
	DRV_ERR_TIMEOUT,			/*!< timeout							*/
	DRV_WARN_NO_ACTION,			/* < the function completed successfully,*/
	                            /* < but no action was taken            */
	DRV_WARN_PARAM_CLIPPED,		/*!< the function has completed			*/
								/*!< successfully, though a parameter was	*/
								/*!< clipped to within a valid range.		*/
	DRV_WARN_BUFFER_EMPTY,
	DRV_WARN_BUFFER_FULL,
	DRV_WARN_UNINITED,			/*!< driver has not been initialized yet */
	DRV_WARN_INITED,			/*!< driver has been initialized already */
	DRV_ERR_MODE_MISMATCH,		/*!< deinterlacer off*/
	DRV_ERR_IPCLEVEL,			/*!< IPC command error*/
	DRV_ERR_MAX					/*!< Max error number*/
} DRV_Status_e;

/**************************************************************************
 *                          D A T A    T Y P E S                          *
 **************************************************************************/
typedef struct _display_size_t
{
	UINT16 width;
	UINT16 height;
} display_size_t;

typedef struct _display_rect_t
{
	UINT16 x;
	UINT16 y;
	UINT16 width;
	UINT16 height;
} display_rect_t;

typedef struct _display_color_t
{
	UINT8	blue;
	UINT8	green;
	UINT8	red;
	UINT8	alpha;
} display_color_t;

#endif // _HAL_COMMON_H_
