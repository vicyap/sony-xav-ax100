/**
 * @file AudioMutex.h
 * @author mm.li
 * @date 2015-11-03
 * @version
 * @copyright SunMedia Technology (Chengdu) co. Ltd.
 * @brief declarates of AudioMutex
 */
#ifndef AUDIO_MUTEX_H
#define AUDIO_MUTEX_H

#include "AudioTypes.h"
#include <pthread.h>

class AudioMutex
{
public:
	enum
	{
		PRIVATE = 0,
		SHARED = 1
	};

	inline AudioMutex();
	inline AudioMutex(const char* name);
	inline AudioMutex(int type, const char* name = NULL);
	inline ~AudioMutex();

	// lock or unlock the mutex
	inline status_t    lock();
	inline void        unlock();

	// lock if possible; returns 0 on success, error otherwise
	inline status_t    tryLock();

	// Manages the mutex automatically. It'll be locked when Autolock is
	// constructed and released when Autolock goes out of scope.
	class AudioAutoLock
	{
	public:
		inline AudioAutoLock(AudioMutex& mutex) : mLock(mutex)
		{
			mLock.lock();
		}
		inline AudioAutoLock(AudioMutex* mutex) : mLock(*mutex)
		{
			mLock.lock();
		}
		inline ~AudioAutoLock()
		{
			mLock.unlock();
		}
	private:
		AudioMutex& mLock;
	};

private:
	friend class AudioCondition;

	// A mutex cannot be copied
	AudioMutex(const AudioMutex&);
	AudioMutex&      operator = (const AudioMutex&);

	pthread_mutex_t mAudioMutex;
};

typedef AudioMutex::AudioAutoLock AudioAutoMutex;

#if 1
inline AudioMutex::AudioMutex()
{
	pthread_mutex_init(&mAudioMutex, NULL);
}
inline AudioMutex::AudioMutex(const char* name)
{
	pthread_mutex_init(&mAudioMutex, NULL);
}
inline AudioMutex::AudioMutex(int type, const char* name)
{
	if (type == SHARED)
	{
		pthread_mutexattr_t attr;
		pthread_mutexattr_init(&attr);
		pthread_mutexattr_setpshared(&attr, PTHREAD_PROCESS_SHARED);
		pthread_mutex_init(&mAudioMutex, &attr);
		pthread_mutexattr_destroy(&attr);
	}
	else
	{
		pthread_mutex_init(&mAudioMutex, NULL);
	}
}
inline AudioMutex::~AudioMutex()
{
	pthread_mutex_destroy(&mAudioMutex);
}
inline status_t AudioMutex::lock()
{
	return -pthread_mutex_lock(&mAudioMutex);
}
inline void AudioMutex::unlock()
{
	pthread_mutex_unlock(&mAudioMutex);
}
inline status_t AudioMutex::tryLock()
{
	return -pthread_mutex_trylock(&mAudioMutex);
}
#else
#include "AudioMutex.cpp.bak"
#endif
#endif //AUDIO_MUTEX_H
