/**
 * @file ClientRecord.h
 * @authors mm.li xun.lu
 * @date 2015-11-13
 * @version 
 * @copyright SunMedia Technology (Chengdu) co. Ltd.
 * @brief declarates of ClientRecord
 */
#ifndef AUDIO_CLIENT_RECORD_H
#define AUDIO_CLIENT_RECORD_H

#include "AudioRecord.h"
#include "AudioCondition.h"
#include "AudioMutex.h"
#include "AudioSemaphore.h"

class RemoteCall;
class AudioRingBuffer;

class ClientRecord : public AudioRecord
{
public:

    /* Creates a ClientRecord object and registers it with AudioService.*/
    ClientRecord(int tag, int rate, int fmt, int chl, int share_duration_ms = MIN_PCM_MS_PER_LOOP * 6);

    /* Terminates the ClientRecord and unregisters it from AudioService.
     * Also destroys all resources associated with the ClientRecord.
     */
    ~ClientRecord(); 

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
    status_t setVolume(int volume);

    /* get volume of track */
    int getVolume();
    

    /* Return size of buffer to process perLoop */
    int getPerLoopSize();

    /* Return size of buffer */
    int getTotalBufferSize();

    /* read audio data from shareBuffer,so app can process them.
     * Returns actual number of bytes read >= 0 or errcode .
     * if err happen, this track is invalid, delete it and then new one
     *
     * Parameters:
     *
     * buffer:      data addr which to read.
     * size:            size of data in byte units, instore time of overFlow
     */
    int read(void* buffer, unsigned int& bytes, bool isBlock = false);

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
    * readSize:          size of buffer to update, instore time of overFlow
    */
    status_t updateReader (int& readSize);
private:
    status_t init(int tag, int rate, int fmt, int chl, int frameCount);
    status_t getWtPtr(unsigned int* wtPtr);
    status_t setRdPtr(unsigned int rdPtr);
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
    
    bool                    mIsBlocked;
    AudioCondition          mBlockedCond;
    AudioMutex              mBlockedMutex;
    AudioSemaphore*         mBlockedExitSem;    

    //int             mFrameCount;
    //int             mFrameSize;
    status_t                mInitStatus;
protected:
    ClientRecord():AudioRecord(){}
};
#endif //AUDIO_CLIENT_RECORD_H
