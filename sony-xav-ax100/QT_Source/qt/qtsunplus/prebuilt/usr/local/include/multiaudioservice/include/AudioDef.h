/**
 * @file AudioDef.h
 * @authors mm.li xun.lu
 * @date 2015-11-12
 * @version
 * @copyright SunMedia Technology (Chengdu) co. Ltd.
 * @brief declaration of some global setting
 */
#ifndef AUDIO_DEF_H
#define AUDIO_DEF_H

//#define USE_CHUNK_MEM (0)

//#define WINDOWS_SHARE_MEM (1)
//#define HAVE_PTHREAD_COND_TIMEDWAIT_RELATIVE (0)

#define HAVE_POSIX_CLOCKS (1)

#define HAVE_PRCTL (0)

//#define LOG_SHOW_DEBUG_MSG_ON_SCREEN (1)

//#define USE_STD_LOG (1)

//#define USE_TINY_ALSA (0)

//#define TEST_AUXIN (0)

#define HAS_SNPRINTF (0)

#ifdef WIN32
	#define TEST_FILE_PATH "D:\\audio_service\\wave\\"
	#define OUTPUT_FILE_PATH "D:\\audio_service\\output\\"
	#define LOG_FILE_PATH "D:\\audio_service\\"
#else
	#if 0
		#define TEST_FILE_PATH "/media/sda1/"
		#define OUTPUT_FILE_PATH "/media/sda1/"
		#define LOG_FILE_PATH "/media/sda1/"
		//#define LOG_FILE_PATH ""
	#else
		#define TEST_FILE_PATH "/mnt/mmcblk0/"
		#define OUTPUT_FILE_PATH "/mnt/mmcblk0/"
		#define LOG_FILE_PATH "/mnt/mmcblk0/"
	#endif
#endif

#define CONNECTIONS_PORT 4003
#ifdef WIN32
#define CONNECTIONS_NAME "127.0.0.1"
#else
#define CONNECTIONS_NAME "/tmp/as_internal_file"
#endif

#define HAS_SHARED_WRITE_READ_POINTER (0)

#define HAS_PERFORMACE_DEBUG_FLOW (0)

#define HAS_VOICE_DELAY_DEBUG (0)

#define HAS_PERFORMANCE_LIMIT_CHECK (0)

#define HAS_INIT_TIME_CHECK (1)

#define HAS_AUDIO_FILE_SERVER_DEBUG (1)

#define HAS_DRVIVER_LATENCY_DEBUG (0)

#ifdef WIN32
	#define HAS_DYNAMIC_AUDIO_DRIVER_LIB (0)
#else
	#define HAS_DYNAMIC_AUDIO_DRIVER_LIB (1)
#endif

#if HAS_DYNAMIC_AUDIO_DRIVER_LIB
	//#define AUDIO_DRIVER_DLIB_FULL_NAME "/app/lib/libadriver.so"
	#define AUDIO_DRIVER_DLIB_FULL_NAME "/usr/local/lib/libadriver.so"
#endif

#ifdef WIN32
	#define HAS_WINDOWS_VS_MEM_LEAK_DEBUG (1)
	#define HAS_BACK_TRACK_DEBUG (0)
	#define HAS_NO_SOCKET_SIGPIPE (0)
#else
	#define HAS_WINDOWS_VS_MEM_LEAK_DEBUG (0)
	#define HAS_BACK_TRACK_DEBUG (1)
	#define HAS_NO_SOCKET_SIGPIPE (1)
#endif

#if HAS_WINDOWS_VS_MEM_LEAK_DEBUG //for mem leak check
#define DEBUG_CLIENTBLOCK  new( _CLIENT_BLOCK, __FILE__, __LINE__)
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#define new DEBUG_CLIENTBLOCK
inline void EnableMemLeakCheck()
{
	_CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_LEAK_CHECK_DF);
}
#endif

#define LOG_LEVEL          (2) // 4:DEBUG 3:LOG 2:INFO 1:WARNING 0:ERROR
#define HAS_LOG_TO_CONSOLE (1)
#define HAS_LOG_TO_FILES   (1)

#define HAS_APP_SETTING    (1)
#define HAS_CODEC_SETTING  (1)
#define HAS_MIXER_SETTING  (1)
#define HAS_SOURCE_SETTING (1)
#define HAS_VOLUME_SETTING (1)

#define VER_MAJOR  0
#define VER_MINOR  4


#endif //AUDIO_DEF_H
