/**
 * @file AudioVoice.h
 * @author xun.lu
 * @date 2015-12-12
 * @version 
 * @copyright SunMedia Technology (Chengdu) co. Ltd.
 * @brief declaration of AudioVoice
 */
#ifndef AUDIO_VOCIE_H
#define AUDIO_VOCIE_H

#include "AudioTypes.h"
#include "AudioError.h"

typedef enum
{
    AUDIO_VOICE_MSG_MIN,
    AUDIO_VOICE_MSG_UPDATE_WRITE_POINTER,
    AUDIO_VOICE_MSG_UPDATE_READ_POINTER,
    AUDIO_VOICE_MSG_DELETED,
    AUDIO_VOICE_MSG_MAX
}audio_voice_msg_e;

/**
 * @class AudioVoice
 * @brief API of put & get phone's pcm data
 * @details  multi instance is not allowed.
 */
class AudioVoice
{
public:
    AudioVoice(){}

    /* Terminates the ClientVoice and unregisters it from AudioService.
     * Also destroys all resources associated with the ClientVoice.
     */
    virtual ~AudioVoice(){}

    virtual status_t init(int tag, int rate, int fmt, int chl, int frameCount) = 0;
    virtual status_t getRdPtr(unsigned int* rdPtr) = 0;
    virtual status_t setWtPtr(unsigned int wtPtr) = 0;
    virtual status_t getWtPtr(unsigned int* wtPtr) = 0;
    virtual status_t setRdPtr(unsigned int rdPtr) = 0;

    /* clear a track. just clear the shareBuffer*/
    virtual status_t clear() = 0;

    /* After it's created the track is not active.Call start() to make it active.*/
    virtual status_t start() = 0;

    /* Stop a track.the track is stopped immediately */
    virtual status_t stop() = 0;

    /* set volume of track */
    virtual status_t setOutVolume(int volume, int volumeDuration = -1) = 0;

    /* get volume of track */
    virtual int getOutVolume() = 0;

    /* set volume of track */
    virtual status_t setInVolume(int volume, int volumeDuration = -1) = 0;

    /* get volume of track */
    virtual int getInVolume() = 0;

    /* retusn Avail Share Buffer Size in frames.*/
    virtual int framesFree() = 0;

    /* retusn Avail Share Buffer Size in bytes.*/
    virtual int bytesFree() = 0;

    /*rampDuration: time of ramping in ms
        *Set rampDuration of rampUp/rampDown while start/stop
        */
    virtual status_t setAutoRampDuration(int rampDuration) = 0;

    /* duck track 
         * Parameters:
     *
     * duckVolume:      volume of ducking.
     * duckDuration:    time used of ducking
     */
    virtual status_t duck(int duckVolume, int duckDuration) = 0;

    /* unduck track 
         * Parameters:
     *
     * unDuckDuration:    time used of unducking
     */
    virtual status_t unDuck (int unDuckDuration) = 0;

    /* get buffer to write
    * Parameters:
    * writeBuffer:          size of buffer need and real size of get
    * wait:                 waitType
    */
    virtual int getWriteBuffer (audio_cbuffer_t& writeBuffer, waitType wait = WAIT_NOTHING) = 0;

    /* update server offset
    * Parameters:
    * writeSize:          size of buffer to update
    */
    virtual status_t updateWriter (int writeSize) = 0;

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
    * readSize:          size of buffer to update
    */
    virtual status_t updateReader (int readSize) = 0;

    virtual int getPerLoopSize() = 0;
};

#endif //AUDIO_VOCIE_H