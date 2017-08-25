/**
 * @file AudioFifoSetting.h
 * @author xun.lu
 * @date 2016-03-15
 * @version
 * @copyright SunMedia Technology (Chengdu) co. Ltd.
 * @brief declaration of AudioFifoSetting
 */
#ifndef AUDIO_FIFO_SETTING_H
#define AUDIO_FIFO_SETTING_H

#include "AudioTypes.h"
#include "AudioError.h"



/**
 * @class AudioFifoSetting
 * @brief API of DSP's FIFO setting.
 */
class AudioFifoSetting
{
public:
	AudioFifoSetting() {}
	virtual ~AudioFifoSetting() {}
	virtual status_t setInputSampleRate(unsigned int rate) = 0;
	virtual status_t getFifoOutPutSampleCounter(unsigned int* counter, long long* host) = 0;
};

#endif //AUDIO_FIFO_SETTING_H