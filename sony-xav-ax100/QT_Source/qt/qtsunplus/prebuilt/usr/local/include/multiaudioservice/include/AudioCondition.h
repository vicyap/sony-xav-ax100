/**
 * @file AudioCondition.h
 * @authors mm.li
 * @date 2015-11-03
 * @version
 * @copyright SunMedia Technology (Chengdu) co. Ltd.
 * @brief declarates of AudioCondition
 */
#ifndef AUDIO_CONDITION_H
#define AUDIO_CONDITION_H

#include "AudioDef.h"
#include "AudioTypes.h"
#include "AudioMutex.h"

#include <pthread.h>
/*
 * Condition variable class.  The implementation is system-dependent.
 *
 * Condition variables are paired up with mutexes.  Lock the mutex,
 * call wait(), then either re-wait() if things aren't quite what you want,
 * or unlock the mutex and continue.  All threads calling wait() must
 * use the same mutex for a given Condition.
 */
class AudioCondition
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

	inline AudioCondition();
	inline AudioCondition(int type);
	inline ~AudioCondition();
	// Wait on the condition variable.  Lock the mutex before calling.
	inline status_t wait(AudioMutex& mutex);
	inline status_t wait(AudioMutex* mutex);
	// same with relative timeout
	inline status_t waitRelative(AudioMutex& mutex, long waitMilliseconds = 500L);
	// Signal the condition variable, allowing one thread to continue.
	inline void signal();
	// Signal the condition variable, allowing one or all threads to continue.
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
	// Signal the condition variable, allowing all threads to continue.
	inline void broadcast();

private:
	pthread_cond_t mCond;
};
#if 1
inline AudioCondition::AudioCondition()
{
	pthread_cond_init(&mCond, NULL);
}

inline AudioCondition::AudioCondition(int type)
{
	if (type == SHARED)
	{
		pthread_condattr_t attr;
		pthread_condattr_init(&attr);
		pthread_condattr_setpshared(&attr, PTHREAD_PROCESS_SHARED);
		pthread_cond_init(&mCond, &attr);
		pthread_condattr_destroy(&attr);
	}
	else
	{
		pthread_cond_init(&mCond, NULL);
	}
}

inline AudioCondition::~AudioCondition()
{
	pthread_cond_destroy(&mCond);
}

inline status_t AudioCondition::wait(AudioMutex& mutex)
{
	return -pthread_cond_wait(&mCond, &mutex.mAudioMutex);
}

inline status_t AudioCondition::wait(AudioMutex* mutex)
{
	return -pthread_cond_wait(&mCond, &(mutex->mAudioMutex));
}

inline status_t AudioCondition::waitRelative(AudioMutex& mutex, long waitMilliseconds)
{

	const long BILLION = 1000000000;
	const long MILLION = 1000000;

	long reltime = waitMilliseconds * MILLION;
#if defined(HAVE_PTHREAD_COND_TIMEDWAIT_RELATIVE)
	struct timespec ts;
	ts.tv_sec  = reltime/BILLION;
	ts.tv_nsec = reltime%BILLION;
	return -pthread_cond_timedwait_relative_np(&mCond, &mutex.mAudioMutex, &ts);
#else // HAVE_PTHREAD_COND_TIMEDWAIT_RELATIVE
	struct timespec ts;
#if 1
	ts.tv_sec = time(NULL);
#elif defined(HAVE_POSIX_CLOCKS)
	clock_gettime(CLOCK_REALTIME, &ts);
#else // HAVE_POSIX_CLOCKS
	// we don't support the clocks here.
	struct timeval t;
	gettimeofday(&t, NULL);
	ts.tv_sec = t.tv_sec;
	ts.tv_nsec= t.tv_usec*1000;
#endif // HAVE_POSIX_CLOCKS
	ts.tv_sec += reltime/BILLION;
	ts.tv_nsec+= reltime%BILLION;
	if (ts.tv_nsec >= BILLION)
	{
		ts.tv_nsec -= BILLION;
		ts.tv_sec  += 1;
	}
	return -pthread_cond_timedwait(&mCond, &mutex.mAudioMutex, &ts);
#endif // HAVE_PTHREAD_COND_TIMEDWAIT_RELATIVE
}

inline void AudioCondition::signal()
{
	pthread_cond_signal(&mCond);
}

inline void AudioCondition::broadcast()
{
	pthread_cond_broadcast(&mCond);
}
#else
#include "AudioCondition.cpp.bak"
#endif
#endif //AUDIO_CONDITION_H
