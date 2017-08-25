/**
 * @file ClientTrack.h
 * @authors mm.li xun.lu
 * @date 2015-11-13
 * @version
 * @copyright SunMedia Technology (Chengdu) co. Ltd.
 * @brief declarates of ClientTrack
 */
#ifndef AUDIO_CLIENT_TRACK_H
#define AUDIO_CLIENT_TRACK_H

#include "AudioTrack.h"
#include "AudioCondition.h"
#include "AudioMutex.h"
#include "AudioSemaphore.h"

class RemoteCall;
class AudioRingBuffer;

class ClientTrack : public AudioTrack
{
public:

	/* Creates a ClientTrack object and registers it with AudioService.*/
	ClientTrack(int tag, int rate, int fmt, int chl, int share_duration_ms = MIN_PCM_MS_PER_LOOP * 6);

	/* Terminates the ClientTrack and unregisters it from AudioService.
	 * Also destroys all resources associated with the ClientTrack.
	 */
	~ClientTrack();

	status_t initCheck();

	/* clear a track. just clear the shareBuffer*/
	status_t clear();

	/* After it's created the track is not active.Call start() to make it active.*/
	status_t start();

	/* Stop a track.the track is stopped immediately */
	status_t stop();

	/* set volume of track */
	status_t setVolume(int volume, int volumeDuration = MIN_PCM_MS_PER_LOOP);

	/* get volume of track */
	int getVolume();

	/* Return size of buffer to process perLoop */
	int getPerLoopSize();

	/* Return size of buffer */
	int getTotalBufferSize();


	/* retusn Avail Share Buffer Size in frames.*/
	int framesFree();

	/* return output sample number*/
	status_t getOutPutSamples(unsigned long long* samples, long long* host = NULL);

	/* retusn Avail Share Buffer Size in bytes.*/
	int bytesFree();

	/* Set Auto RampDuration*/
	status_t setAutoRampDuration(int rampDuration);

	/* duck track
	     * Parameters:
	 *
	 * duckVolume:      volume of ducking.
	 * duckDuration:    time used of ducking
	 */
	status_t duck(int duckVolume, int duckDuration);

	/* unduck track
	     * Parameters:
	 *
	 * unDuckDuration:    time used of unducking
	 */
	status_t unDuck (int unDuckDuration);

	/* get buffer to write
	* Parameters:
	* writeBuffer:          size of buffer need and real size of get
	* wait:                 waitType
	*/
	int getWriteBuffer (audio_cbuffer_t& writeBuffer, waitType wait = WAIT_NOTHING);

	/* update server offset
	* Parameters:
	* writeSize:          size of buffer to update, instore time of underFlow
	*/
	status_t updateWriter (int& writeSize);


	/* write audio data to shareBuffer,so MultiAUdioService can process them.
	   * Returns actual number of bytes written >= 0 or errcode .
	   * if err happen, this track is invalid, delete it and then new one
	   *
	   * Parameters:
	   *
	   * buffer:      data addr which to write.
	   * bytes:            size of data in byte units, instore time of underFlow.
	   */
	int write(const unsigned char* buffer, unsigned int& bytes, bool isBlock = true);

private:
	status_t init(int tag, int rate, int fmt, int chl, int frameCount);
	status_t getRdPtr(unsigned int* rdPtr);
	status_t setWtPtr(unsigned int wtPtr);
	static void notifierCallbackFunc(void* opaque, unsigned int msg, unsigned int value);

	RemoteCall*             mRc;
	int                     mSampleRate;
	int                     mFormat;
	int                     mFrameSize;
	int                     mChannel;
	int                     mChNB;
	int                     mVolume;

	int                     mShareDurationMs;

	unsigned char*          mShareBufferAddr;
	unsigned int            mShareBufferSize;
	AudioRingBuffer*        mRingBuffer;
#if 0
	AudioSharedBuffer*      mBuffer;
	audio_config_t*           mConfig;
#endif

	int                     mPerLoopSize;
	int                     mPerLoopTime;
	int                     mPerLoopSamples;
	audio_state_t           mState;
	//int             mFrameCount;
	//int             mFrameSize;

	bool                    mIsBlocked;
	AudioCondition          mBlockedCond;
	AudioMutex              mBlockedMutex;
	AudioSemaphore*         mBlockedExitSem;

	status_t mInitStatus;
};
#endif //AUDIO_CLIENT_TRACK_H
