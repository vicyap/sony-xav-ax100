/**
 * @file AudioSemaphore.h
 * @author mm.li
 * @date 2015-11-03
 * @version
 * @copyright SunMedia Technology (Chengdu) co. Ltd.
 * @brief declarates of AudioSemaphore
 */
#ifndef AUDIO_SEMAPHORE_H
#define AUDIO_SEMAPHORE_H

#include "AudioDef.h"
#include "AudioTypes.h"
#include "AudioLog.h"
#include "AudioTime.h"

#include "pthread.h"
#include "semaphore.h"

class AudioSemaphore
{
public:
	enum
	{
		PRIVATE = 0,
		SHARED = 1
	};

	enum WakeUpType
	{
		WAKE_UP_ONE = 0,
		WAKE_UP_ALL = 1
	};

	inline AudioSemaphore();
	inline AudioSemaphore(int type);
	inline ~AudioSemaphore();
	// Wait on the semaphore.
	inline status_t wait();
	// same with relative timeout
	inline status_t waitRelative(long waitMilliseconds = 500L);
	// Signal the semaphore, allowing one thread to continue.
	inline void signal();
	// Signal the semaphore, allowing one or all threads to continue.
	inline void signal(WakeUpType type)
	{
		if (type == WAKE_UP_ONE)
		{
			signal();
		}
		else
		{
			broadcast();
		}
	}
	// Signal the semaphore, allowing all threads to continue.
	inline void broadcast();

private:
	sem_t mSem;
};

#if 1
#include <errno.h>

inline AudioSemaphore::AudioSemaphore()
{
	if (sem_init((sem_t*)&mSem, 0, 0) == -1)
	{
		AUD_LOGE("init sem failed\n");
	}
}

inline AudioSemaphore::AudioSemaphore(int type)
{
	if (type == SHARED)
	{
		if (sem_init((sem_t*)&mSem, 1, 0) == -1)
		{
			AUD_LOGE("init sem failed\n");
		}
	}
	else
	{
		if (sem_init((sem_t*)&mSem, 0, 0) == -1)
		{
			AUD_LOGE("init sem failed\n");
		}
	}
}

inline AudioSemaphore::~AudioSemaphore()
{
	sem_destroy((sem_t*)&mSem);
}

inline status_t AudioSemaphore::wait()
{
	return -sem_wait((sem_t*)&mSem);
}

inline status_t AudioSemaphore::waitRelative(long waitMilliseconds)
{
	int ret = 0;

	const long BILLION = 1000000000;
	const long MILLION = 1000000;

	long reltime = waitMilliseconds * MILLION;
	struct timespec abstime;
	audio_timespec_t time;

	AudioGetTime(&time);

	abstime.tv_nsec = time.tv_nsec;
	abstime.tv_sec = time.tv_sec;

	abstime.tv_sec += reltime / BILLION;
	abstime.tv_nsec+= reltime % BILLION;

	if (abstime.tv_nsec >= BILLION)
	{
		abstime.tv_nsec -= BILLION;
		abstime.tv_sec  += 1;
	}

	if (sem_timedwait((sem_t*)&mSem, &abstime) == -1)
	{
		if (errno == ETIMEDOUT)
		{
			ret = -ETIMEDOUT;
		}
	}
	return ret;
}

inline void AudioSemaphore::signal()
{
	int val;
	if (sem_getvalue((sem_t*)&mSem, &val) == 0)
	{
		if (val > 0)
		{
			//AUD_LOGD("sem not zero(%d)",val);
		}
		else
		{
			if (sem_post((sem_t*)&mSem) == -1)
			{
				AUD_LOGE("sem_post failed errno=%d\n", errno);
			}
		}
	}
}

inline void AudioSemaphore::broadcast()
{
	int val;
	if (sem_getvalue((sem_t*)&mSem, &val) == 0)
	{
		if (val > 0)
		{
			//AUD_LOGD("sem not zero(%d)",val);
		}
		else
		{
			if (sem_post((sem_t*)&mSem) == -1)
			{
				AUD_LOGE("sem_post failed errno=%d\n", errno);
			}
		}
	}
}
#else
#include "AudioSemaphore.cpp.bak"
#endif

#endif //AUDIO_SEMAPHORE_H