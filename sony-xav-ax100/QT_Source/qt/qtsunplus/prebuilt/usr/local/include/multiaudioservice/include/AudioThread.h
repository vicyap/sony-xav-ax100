/**
 * @file AudioThread.h
 * @authors mm.li xun.lu
 * @date 2015-11-13
 * @version
 * @copyright SunMedia Technology (Chengdu) co. Ltd.
 * @brief declarates of AudioThread
 */
#ifndef AUDIO_THREAD_H
#define AUDIO_THREAD_H

#include "AudioTypes.h"
#include "AudioCondition.h"
#include "pthread.h"

typedef enum
{
	THREAD_STOP = 0,
	THREAD_STOPING,
	THREAD_STOPED,
	THREAD_RUNING,
} multi_thread_state;

class AudioThread
{
public:
	/* Create a AudioThread
	* Parameters:
	* service:          point to MultiAudioService
	*/
	AudioThread(void* data = NULL);

	virtual ~AudioThread();

	// Ask this object's thread to exit. This function is asynchronous, when the
	// function returns the thread might still be running. Of course, this
	// function can be called from a different thread.
	virtual status_t requestExit();

	// Call requestExit() and wait until this object's thread exits.
	// BE VERY CAREFUL of deadlocks. In particular, it would be silly to call
	// this function from this object's thread. Will return WOULD_BLOCK in
	// that case.
	status_t requestExitAndWait();

	// exitPending() returns true if requestExit() has been called.
	bool exitPending();

	// Indicates whether this thread is running or not.
	bool isRunning() const;

	// Start the thread in threadLoop() which needs to be implemented.
	virtual status_t run(const char* name = 0, int priority = -1, int policy = -10);

	// Wait until this object's thread exits. Returns immediately if not yet running.
	// Do not call from this object's thread; will return WOULD_BLOCK in that case.
	status_t join();

	/* return name of this thread */
	//string getName();

private:
	// Derived class must implement threadLoop(). The thread starts its life
	// here. There are two ways of using the Thread object:
	// 1) loop: if threadLoop() returns true, it will be called again if
	//          requestExit() wasn't called.
	// 2) once: if threadLoop() returns false, the thread will exit upon return.
	virtual bool threadLoop() = 0;
	static void* _threadLoop(void* user);
	int setPriority();

	// Good place to do one-time initializations
	virtual status_t readyToRun();

#if HAVE_PRCTL
	char* mName;
#endif
	volatile multi_thread_state mActive;
	mutable AudioMutex mLock;
	pthread_t mThread;
	AudioCondition mCondition;
	status_t mStatus;
	int mPolicy;
	int mPriority;
	void* mData;
};
#endif //AUDIO_THREAD_H