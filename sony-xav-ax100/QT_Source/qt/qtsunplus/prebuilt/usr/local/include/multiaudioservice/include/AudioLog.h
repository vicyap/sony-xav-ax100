/**
 * @file AudioLog.h
 * @author xun.lu
 * @date 2015-12-12
 * @version 
 * @copyright SunMedia Technology (Chengdu) co. Ltd.
 * @brief declarates of log
 */
#ifndef AUDIO_LOG_H
#define AUDIO_LOG_H

#include "AudioDef.h"

#define LEVEL_ERROR         (0)
#define LEVEL_WARNING       (1)
#define LEVEL_INFO          (2)
#define LEVEL_LOG           (3)
#define LEVEL_DEBUG         (4)

#define WIDFILE_      __FILE__

#ifndef LOG_LEVEL
#define LOG_LEVEL LEVEL_DEBUG
#warning "LOG_LEVEL is default LEVEL_DEBUG"
#endif

#define AUDIO_DATA_NAMED_LOG(name, logBuf,len) \
                    do { \
                        audioLog((const char*)name,((char*)(logBuf)),((int)(len)),-1); \
                    } while (0)

void audioLog(const char* name, char* logBuf, int len, int loglevel);


#define ASLOG_BUG_PREFIX_STRING_ARGS "<4>[%s %s %s %d] BUG: ", "", __FILE__, __FUNCTION__, __LINE__
#define ASLOG_ERR_PREFIX_STRING_ARGS "<3>[%s %s %s %d] ERR: ", "", __FILE__, __FUNCTION__, __LINE__
#define ASLOG_WAN_PREFIX_STRING_ARGS "<2>[%s %s %s %d] WRN: ", "", WIDFILE_, __FUNCTION__, __LINE__
#define ASLOG_INF_PREFIX_STRING_ARGS "<1>[%s %s %s %d] INF: ", "", WIDFILE_, __FUNCTION__, __LINE__
#define ASLOG_DBG_PREFIX_STRING_ARGS "<0>[%s %s %s %d] ",      "", WIDFILE_, __FUNCTION__, __LINE__


#if (LOG_LEVEL >= LEVEL_DEBUG)
#define AUD_LOGD(...)  audioPrint(ASLOG_DBG_PREFIX_STRING_ARGS, __VA_ARGS__)  
#else
#define AUD_LOGD(...)
#endif

#if (LOG_LEVEL >= LEVEL_INFO)
#define AUD_LOGI(...)  audioPrint(ASLOG_INF_PREFIX_STRING_ARGS, __VA_ARGS__)
#else
#define AUD_LOGI(...)
#endif

#if (LOG_LEVEL >= LEVEL_WARNING)
#define AUD_LOGW(...)  audioPrint(ASLOG_WAN_PREFIX_STRING_ARGS, __VA_ARGS__)
#else
#define AUD_LOGW(...)
#endif

#define AUD_BUG(...)   audioPrint(ASLOG_BUG_PREFIX_STRING_ARGS, __VA_ARGS__)
#define AUD_ASSERT(x)  { if (!(x)) { AUD_BUG(#x"\n"); while(1);} }
#define AUD_LOGE(...)  audioPrint(ASLOG_ERR_PREFIX_STRING_ARGS, __VA_ARGS__)

void AudioLogInit(const char* LogFileName);
void audioPrint(const char* prefix_fmt, const char* obj, const char* file, const char* func, int line, const char* fmt, ...);
#endif //AUDIO_LOG_H