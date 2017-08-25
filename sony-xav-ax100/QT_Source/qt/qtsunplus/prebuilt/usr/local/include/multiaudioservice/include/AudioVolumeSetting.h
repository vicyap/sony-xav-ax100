/**
 * @file AudioVolumeSetting.h
 * @author xun.lu
 * @date 2016-01-08
 * @version
 * @copyright SunMedia Technology (Chengdu) co. Ltd.
 * @brief declaration of AudioVolumeSetting
 */
#ifndef AUDIO_VOLUME_SETTING_H
#define AUDIO_VOLUME_SETTING_H

#include "AudioTypes.h"
#include "AudioError.h"

typedef struct
{
	audio_channel_idx_e nChannel;
	unsigned char volume;/*! < [0, 100]*/
} audio_channel_gain_t;

typedef struct
{
	audio_source_id_e source;
	unsigned char volume;
} audio_stream_volume_param_t;


/**
 * @class AudioVolumeSetting
 * @brief API of DSP's volume setting
 * @details  dunk/undunk/master volume/channel gain/stream volume
 */
class AudioVolumeSetting
{
public:
	AudioVolumeSetting() {}
	virtual ~AudioVolumeSetting() {}

	/**
	    * @fn status_t setMainAudioDunkingMode(unsigned int mode)
	    * @brief dunk/undunk
	    * @param mode 0:undunk 1:dunk
	    * @return error number which defined in AudioError.h.
	    */
	virtual status_t setMainAudioDunkingMode(unsigned int mode) = 0;

	/**
	    * @fn status_t setMainAudioDunkingTargetGain(float dbLevel)
	    * @brief gain of dunk
	    * @param volume  [0, 100]
	    * @return error number which defined in AudioError.h.
	    */
	virtual status_t setMainAudioDunkingTargetGain(unsigned char volume) = 0;

	/**
	    * @fn status_t setMasterVolume(float volume)
	    * @brief set master volume
	    * @param volume  [0, 100]
	    * @return error number which defined in AudioError.h.
	    */
	virtual status_t setMasterVolume(unsigned char volume) = 0; //AudioService/GPS/Effect/DSP Out

	/**
	    * @fn status_t getMasterVolume(float volume)
	    * @brief get master volume
	    * @param volume a unsigned char pointer
	    * @return error number which defined in AudioError.h.
	    */
	virtual status_t getMasterVolume(unsigned char* volume) = 0;//AudioService/GPS/Effect/DSP Out

	/**
	   * @fn status_t setChannelGain(audio_channel_gain_t* gain)
	   * @brief set channel gain volume
	   * @param gain a audio_channel_gain_t pointer
	   * @return error number which defined in AudioError.h.
	   */
	virtual status_t setChannelGain(audio_channel_gain_t* gain) = 0; // AudioService/DSP Out
	/**
	    * @fn status_t getChannelGain(audio_channel_gain_t* gain)
	    * @brief get channel gain volume
	    * @param gain a audio_channel_gain_t pointer
	    * @return error number which defined in AudioError.h.
	    */
	virtual status_t getChannelGain(audio_channel_gain_t* gain) = 0; // AudioService/DSP Out

	/**
	    * @fn status_t setStreamVolume(audio_source_id_e source, float volume)
	    * @brief set stream volume
	    * @param audio_source_id_e source's id
	    * @param volume  [0, 100]
	    * @return error number which defined in AudioError.h.
	    */
	virtual status_t setStreamVolume(audio_source_id_e source, unsigned char volume) = 0;

	/**
	    * @fn status_t getStreamVolume(audio_source_id_e source, float* volume)
	    * @brief get stream volume
	    * @param audio_source_id_e source's id
	    * @param volume a unsigned char pointer 
	    * @return error number which defined in AudioError.h.
	    */
	virtual status_t getStreamVolume(audio_source_id_e source, unsigned char* volume) = 0;

};

#endif //AUDIO_VOLUME_SETTING_H
