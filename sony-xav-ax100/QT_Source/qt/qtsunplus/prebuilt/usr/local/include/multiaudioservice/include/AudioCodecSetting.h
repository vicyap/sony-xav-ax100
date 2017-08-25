/**
 * @file AudioCodecSetting.h
 * @author xun.lu
 * @date 2016-01-08
 * @version 
 * @copyright SunMedia Technology (Chengdu) co. Ltd.
 * @brief declaration of AudioCodecSetting
 */
#ifndef AUDIO_CODEC_SETTING_H
#define AUDIO_CODEC_SETTING_H

#include "AudioTypes.h"
#include "AudioError.h"



/**
 * @class AudioCodecSetting
 * @brief API of DSP's Codec setting.
 * @details  power down/on & i2s output setting
 * @todo part of driver ? cant be accessed by user?
 */
class AudioCodecSetting
{
public:
    AudioCodecSetting(){}
    virtual ~AudioCodecSetting(){}
    
    virtual status_t setDacPowerDown() = 0;
    virtual status_t setDacPowerOn() = 0;
    virtual status_t setAdcPowerDown() = 0;
    virtual status_t setAdcPowerOn() = 0;
    virtual status_t setInstanBootPowerDown() = 0;
    virtual status_t setInstanBootPowerOn() = 0;
    //virtual status_t initAudioCodec() = 0; //driver
    //virtual status_t setPcmFormat() = 0;//driver
    //virtual status_t setExtDacXckFs() = 0;//driver
    virtual status_t setI2SPinOut() = 0;
};

#endif //AUDIO_CODEC_SETTING_H