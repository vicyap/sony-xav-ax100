/**
 * @file AudioRecord.h
 * @author xun.lu
 * @date 2015-12-12
 * @version 
 * @copyright SunMedia Technology (Chengdu) co. Ltd.
 * @brief declaration of AudioRecord
 */
#ifndef AUDIO_RECORD_H
#define AUDIO_RECORD_H

#include "AudioTypes.h"
#include "AudioError.h"

typedef enum
{
    AUDIO_RECORD_MSG_MIN,
    AUDIO_RECORD_MSG_UPDATE_WRITE_POINTER,
    AUDIO_RECORD_MSG_DELETED,
    AUDIO_RECORD_MSG_MAX
}audio_record_msg_e;

/**
 * @class AudioRecord
 * @brief API of Mic's Input PCM 
 * @details  get pcm data from Mic driver and multi instance is allow.
 */
class AudioRecord
{
public:

    /* Creates a ClientRecord object and registers it with AudioService.*/
    AudioRecord(){}

    /* Terminates the ClientRecord and unregisters it from AudioService.
     * Also destroys all resources associated with the ClientRecord.
     */
    virtual ~AudioRecord(){}

    virtual status_t init(int tag, int rate, int fmt, int chl, int frameCount) = 0;
    virtual status_t getWtPtr(unsigned int*wtPtr) = 0;
    virtual status_t setRdPtr(unsigned int rdPtr) = 0;

    /* clear a track. just clear the shareBuffer*/
    virtual status_t clear() = 0;

    /* After it's created the track is not active.Call start() to make it active.*/
    virtual status_t start() = 0;

    /* Stop a track.the track is stopped immediately */
    virtual status_t stop() = 0;

    /* set volume of track */
    virtual status_t setVolume(int volume) = 0;

    /* get volume of track */
    virtual int getVolume() = 0;

    #if 0
    /* Set source sample rate for this track in Hz. */
    virtual status_t setSampleRate(int rate) = 0;

    /* Return current source sample rate in Hz. */
    virtual int getSampleRate() = 0;

    /* Set source ChannelMask for this track.Parameters Descriptions see construct function. */
    virtual status_t setChlMask(int chl) = 0;

    /* Return current source ChannelMask. */
    virtual int getChlMask() = 0;

    /* Set source Format for this track.Parameters Descriptions see construct function. */
    virtual status_t setFormat(int fmt) = 0;

    /* Return current source Format. */
    virtual int getFormat() = 0;
    #endif
    
    /* Result of constructing the ClientBase. This must be checked for successful initialization
    * before using any ClientBase API (except for getMinCount()), because using
    * an uninitialized ClientBase produces undefined results.
    */
    virtual status_t initCheck() = 0;

    /* Return size of buffer to process perLoop */
    virtual int getPerLoopSize() = 0;

    /* Return size of buffer */
    virtual int getTotalBufferSize() = 0;

    /* retusn Avail Share Buffer Size in frames.*/
    virtual int framesUsed() = 0;

    /* retusn Avail Share Buffer Size in bytes.*/
    virtual int bytesUsed() = 0;

    /* get buffer to read
    * Parameters:
    * readBuffer:          size of buffer need and real size of get
    * wait:                 waitType
    */
    virtual int getReadBuffer (audio_cbuffer_t& readBuffer,waitType wait = WAIT_NOTHING) = 0;

    /* update user offset
    * Parameters:
    * readSize:          size of buffer to update, instore time of overFlow
    */
    virtual status_t updateReader (int& readSize) = 0;


};

#endif //AUDIO_RECORD_H
