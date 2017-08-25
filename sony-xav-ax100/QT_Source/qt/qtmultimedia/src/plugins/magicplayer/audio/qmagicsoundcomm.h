/**********************************************************************
ModuleName:			QMagicsoundComm

Description:		Common marco and interface.

Copyright:			Copyright @	2015 Sunplus, Inc.

History:			jie.jia@sunmedia.com.cn	 12/15/2015	Creation

**********************************************************************/
#ifndef QMAGICSOUNDAUDIOCOMM_H
#define QMAGICSOUNDAUDIOCOMM_H
#include <QtCore/qdebug.h>
#include <execinfo.h>
#include "AudioFormat.h"

#define CALL_FUNC_STACKADDR(x) __builtin_return_address(x)
//#define MS_TRACE_MSG
//#define MS_ERROR_MSG
//#define MS_DEBUG_MSG
#ifdef MS_TRACE_MSG
#define MS_TRACE(x) qDebug() << __FILE__ << "->" << __FUNCTION__ << "(" << __LINE__ << ")" << x << endl
#else
#define MS_TRACE(x) do{}while(0)
#endif
#ifdef MS_ERROR_MSG
#define MS_ERROR(fmt,arg...) qDebug("%s(%d) Error:"fmt,__FUNCTION__,__LINE__,##arg)
#else
#define MS_ERROR(fmt,arg...) do{}while(0)
#endif
#ifdef MS_DEBUG_MSG
#define MS_DEBUG(fmt,arg...) qDebug("%s(%d) Debug:"fmt,__FUNCTION__,__LINE__,##arg)
#else
#define MS_DEBUG(fmt,arg...) do{}while(0)
#endif

static void _print_backtrace(void)
{
	void *stack[128];
	int i = 0;
    int stack_size = backtrace(stack, sizeof(stack) / sizeof(void *));
    char **stack_symbols = backtrace_symbols(stack, stack_size);

	qDebug() << "Obtained "<< stack_size << "stack frames" << "last call function:" << CALL_FUNC_STACKADDR(0) << endl;

	for( i = 0; i < stack_size; i++)
		qDebug() << stack_symbols[i];	
}
#define PRINT_BACKTRACE() _print_backtrace()

#define SAMPLESIZE2MASK(x) \
	(x)		== 8 	?	FMT_U8 : \
	((x)	== 16	?	FMT_S16 : FMT_S32)

#define SAMPLECOUNT2MASK(x) \
	(x)		== 1 	?	CHL_MONO : \
	((x) 	== 2	?	CHL_STEREO : \
	((x)	== 6	?	CHL_5POINT1 : CHL_7POINT1))

#define FRAMES2BYTES(frames,fmt) (frames*(fmt.sampleSize()/8)*(fmt.channelCount()))
#define BYTES2FRAMES(bytes,fmt) (bytes/(fmt.sampleSize()/8)*(fmt.channelCount()))
#endif //QMAGICSOUNDAUDIOCOMM_H