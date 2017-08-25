/**
 * @file AudioMicSetting.h
 * @author xun.lu
 * @date 2016-05-30
 * @version
 * @copyright SunMedia Technology (Chengdu) co. Ltd.
 * @brief declaration of AudioMicSetting
 */
#ifndef AUDIO_MIC_SETTING_H
#define AUDIO_MIC_SETTING_H

#include "AudioTypes.h"
#include "AudioError.h"


/**
 * @class AudioMicSetting
 * @brief API of DSP's mic setting
 * @details  
 */
class AudioMicSetting
{
public:
	AudioMicSetting() {}
	virtual ~AudioMicSetting() {}

	/**
	    * @fn status_t setUpLinkGain(float gain)
	    * @brief  setting of uplink gain
	    * @param gain [-12.0, 12.0]
	    * @return error number which defined in AudioError.h.
	    */
	virtual status_t setUpLinkGain(float gain) = 0;
};

#endif //AUDIO_MIC_SETTING_H