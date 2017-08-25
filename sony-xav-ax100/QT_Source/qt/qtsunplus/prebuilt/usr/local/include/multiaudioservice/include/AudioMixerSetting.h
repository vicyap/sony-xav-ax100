/**
 * @file AudioMixerSetting.h
 * @author xun.lu
 * @date 2016-01-08
 * @version
 * @copyright SunMedia Technology (Chengdu) co. Ltd.
 * @brief declaration of AudioMixerSetting
 */
#ifndef AUDIO_MIXER_SETTING_H
#define AUDIO_MIXER_SETTING_H

#include "AudioTypes.h"
#include "AudioError.h"

typedef struct
{
	audio_fifo_tag_e   tag; /*! < fifo tag*/
	unsigned char l2l; /*! < left channel of source is mixed to left channel of output volume, [0, 100]*/
	unsigned char l2r; /*! < left channel of source is mixed to right channel of output volume, [0, 100]*/
	unsigned char r2l; /*! < right channel of source is mixed to left channel of output volume, [0, 100]*/
	unsigned char r2r; /*! < right channel of source is mixed to right channel of output volume, [0, 100]*/
} audio_mixer_float_gain_t;

/**
 * @class AudioMixerSetting
 * @brief API of DSP's mixer setting
 * @details  GPS  Upmix
 */
class AudioMixerSetting
{
public:
	AudioMixerSetting() {}
	virtual ~AudioMixerSetting() {}

	/**
	    * @fn status_t setUpMixMode(unsigned int mode)
	    * @brief enable/disable upmix
	    * @param mode 0:disable upmix 1:enable upmix
	    * @return error number which defined in AudioError.h.
	    */
	virtual status_t setUpMixMode(unsigned int mode) = 0;

	/**
	    * @fn status_t mixExtGps(unsigned int mode)
	    * @brief enable/disable external gps module
	    * @param mode 0:disable  1:enable
	    * @return error number which defined in AudioError.h.
	    */
	virtual status_t mixExtGps(unsigned int mode) = 0;

	/**
	    * @fn status_t setGpsMixerGain(audio_mixer_float_gain_t* gain)
	    * @brief volume of external GPS's voice
	    * @param gain
	    * @todo param will be instead by other param
	    * @return error number which defined in AudioError.h.
	    */
	virtual status_t setGpsMixerGain(audio_mixer_float_gain_t* gain) = 0;
};

#endif //AUDIO_MIXER_SETTING_H