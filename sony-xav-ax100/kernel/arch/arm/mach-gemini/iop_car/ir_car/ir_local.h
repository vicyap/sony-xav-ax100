#ifndef	__IR_LOCAL_H
#define __IR_LOCAL_H


#include <linux/types.h>
#include <linux/module.h>
#include <mach/common.h>


#if 0 //Kaney mark
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
	DRV_ERR_MAX					/*!< Max error number*/
} DRV_Status_t;

typedef enum
{
	DRV_IOP_IrType_NEC,
	DRV_IOP_IrType_PHILIPS,
	DRV_IOP_IrType_MAX
} DRV_IOPIrType_t;

#define IR_INVALID_VALUE		0x100
#define IR_FIRST_KEY_INTERVAL	3
#define IR_RDY					(1<<15)
#endif

#define IR_REP					(1<<14) // for key down/up mode

// for key down/up mode:
#define KEY_MODE_NORMAL		0
#define KEY_MODE_REPEAT		1
#define KEY_MODE_LONGPRESS	2
#define IR_REPEAT_MS			500 // depend on IOP repeat time

#endif
