/**
 * @file ClientVoice.h
 * @author xun.lu
 * @date 2015-11-26
 * @version
 * @copyright SunMedia Technology (Chengdu) co. Ltd.
 * @brief declarates of ClientVoice
 */
#ifndef AUDIO_CLIENT_VOICE_H
#define AUDIO_CLIENT_VOICE_H

#include "AudioVoice.h"
#include "AudioCondition.h"
#include "AudioMutex.h"
#include "AudioSemaphore.h"

class RemoteCall;
class AudioRingBuffer;


class ClientVoice : public AudioVoice
{
public:

	/* Creates a ClientVoice object and registers it with AudioService.*/
	ClientVoice(int tag, int rate, int fmt, int chl, int share_duration_ms = MIN_PCM_MS_PER_LOOP * 6);

	/* Terminates the ClientVoice and unregisters it from AudioService.
	 * Also destroys all resources associated with the ClientVoice.
	 */
	~ClientVoice();

	#if HAS_VOICE_DELAY_DEBUG
	int dbgUpdateProtocal(unsigned int isOut, unsigned int size, unsigned int ts);
	#endif

	/* Result of constructing the ClientBase. This must be checked for successful initialization
	* before using any ClientBase API (except for getMinCount()), because using
	* an uninitialized ClientBase produces undefined results.
	*/
	status_t initCheck();

	/* clear a track. just clear the shareBuffer*/
	status_t clear();

	/* After it's created the track is not active.Call start() to make it active.*/
	status_t start();

	/* Stop a track.the track is stopped immediately */
	status_t stop();

	/* set volume of track */
	status_t setOutVolume(int volume, int volumeDuration = MIN_PCM_MS_PER_LOOP);

	/* get volume of track */
	int getOutVolume();

	/* set volume of track */
	status_t setInVolume(int volume, int volumeDuration = MIN_PCM_MS_PER_LOOP);

	/* get volume of track */
	int getInVolume();

	/* write audio data to shareBuffer,so MultiAUdioService can process them.
	 * Returns actual number of bytes written >= 0 or errcode .
	 * if err happen, this track is invalid, delete it and then new one
	 *
	 * Parameters:
	 *
	 * buffer:      data addr which to write.
	 * bytes:            size of data in byte units.
	 */
	int write(const unsigned char* buffer, unsigned int bytes, bool isBlock = false);

	/* retusn Avail Share Buffer Size in frames.*/
	int framesFree();

	/* retusn Avail Share Buffer Size in bytes.*/
	int bytesFree();

	/*rampDuration: time of ramping in ms
	    *Set rampDuration of rampUp/rampDown while start/stop
	    */
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
	* writeSize:          size of buffer to update
	*/
	status_t updateWriter (int writeSize);

	/* read audio data from shareBuffer,so app can process them.
	 * Returns actual number of bytes read >= 0 or errcode .
	 * if err happen, this track is invalid, delete it and then new one
	 *
	 * Parameters:
	 *
	 * buffer:      data addr which to read.
	 * size:            size of data in byte units.
	 */
	int read(unsigned char* buffer, unsigned int bytes, bool isBlock = false);

	/* retusn Avail Share Buffer Size in frames.*/
	int framesUsed();

	/* retusn Avail Share Buffer Size in bytes.*/
	int bytesUsed();

	/* get buffer to read
	* Parameters:
	* readBuffer:          size of buffer need and real size of get
	* wait:                 waitType
	*/
	int getReadBuffer (audio_cbuffer_t& readBuffer,waitType wait = WAIT_NOTHING);

	/* update user offset
	* Parameters:
	* readSize:          size of buffer to update
	*/
	status_t updateReader (int readSize);

	int getPerLoopSize();
private:
	status_t init(int tag, int rate, int fmt, int chl, int frameCount);
	status_t getRdPtr(unsigned int* rdPtr);
	status_t setWtPtr(unsigned int wtPtr);
	status_t getWtPtr(unsigned int* wtPtr);
	status_t setRdPtr(unsigned int rdPtr);
	static void notifierCallbackFunc(void* opaque, unsigned int msg, unsigned int value);

	RemoteCall* mRc;

	int                     mSampleRate;
	int                     mFormat;
	int                     mFrameSize;
	int                     mChannel;
	int                     mChNB;

	int                     mInVolume;
	int                     mOutVolume;

	int                     mShareDurationMs;

	unsigned char*          mShareBufferAddr;
	unsigned int            mShareBufferSize;

	AudioRingBuffer*        mInRingBuffer;
	AudioRingBuffer*        mOutRingBuffer;

	int                     mPerLoopSize;
	int                     mPerLoopTime;
	int                     mPerLoopSamples;
	audio_state_t           mState;

	bool                    mIsOutBlocked;
	AudioCondition          mOutBlockedCond;
	AudioMutex              mOutBlockedMutex;

	bool                    mIsInBlocked;
	AudioCondition          mInBlockedCond;
	AudioMutex              mInBlockedMutex;
	AudioSemaphore*         mBlockedExitSem;

	#if HAS_VOICE_DELAY_DEBUG
	unsigned int mDbgProtocalSize;
	unsigned int mDbgProtocalTimeStamp;
	unsigned int mDbgClientTimeStamp;
	unsigned int mDbgIsOut;
	#endif
};
#endif //AUDIO_CLIENT_VOICE_H
