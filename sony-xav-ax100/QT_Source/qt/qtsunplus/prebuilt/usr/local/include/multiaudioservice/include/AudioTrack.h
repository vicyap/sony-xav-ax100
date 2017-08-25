/**
 * @file AudioTrack.h
 * @author xun.lu
 * @date 2015-12-12
 * @version
 * @copyright SunMedia Technology (Chengdu) co. Ltd.
 * @brief declaration of AudioTrack
 */
#ifndef AUDIO_TRACK_H
#define AUDIO_TRACK_H

#include "AudioTypes.h"
#include "AudioError.h"

typedef enum
{
	AUDIO_TRACK_MSG_MIN,
	AUDIO_TRACK_MSG_UPDATE_READ_POINTER,
	AUDIO_TRACK_MSG_DELETED,
	AUDIO_TRACK_MSG_MAX
} audio_track_msg_e;

/**
 * @class AudioTrack
 * @brief API of put pcm to speaker
 * @details  put pcm data to speaker driver and multi instance is allow.
 */
class AudioTrack
{
public:
	AudioTrack() {}

	/* Terminates the ClientTrack and unregisters it from AudioService.
	 * Also destroys all resources associated with the ClientTrack.
	 */
	virtual ~AudioTrack() {}

	virtual status_t init(int tag, int rate, int fmt, int chl, int frameCount) = 0;
	virtual status_t getRdPtr(unsigned int* rdPtr) = 0;
	virtual status_t setWtPtr(unsigned int wtPtr) = 0;

	/* clear a track. just clear the shareBuffer*/
	virtual status_t clear() = 0;

	/* After it's created the track is not active.Call start() to make it active.*/
	virtual status_t start() = 0;

	/* Stop a track.the track is stopped immediately */
	virtual status_t stop() = 0;

	/* set volume of track */
	virtual status_t setVolume(int volume, int volumeDuration = -1) = 0;

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
	virtual int framesFree() = 0;

	/* retusn Avail Share Buffer Size in bytes.*/
	virtual int bytesFree() = 0;

	/* return output samples number */
	virtual status_t getOutPutSamples(unsigned long long* samples, long long* host) = 0;

	/* Set Auto RampDuration*/
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
	* writeSize:          size of buffer to update, instore time of underFlow
	*/
	virtual status_t updateWriter (int& writeSize) = 0;

};

#endif //AUDIO_TRACK_H
