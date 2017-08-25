/**
 * @file ClientMixerSetting.h
 * @author xun.lu
 * @date 2016-01-13
 * @version 
 * @copyright SunMedia Technology (Chengdu) co. Ltd.
 * @brief declarates of ClientMixerSetting
 */
#ifndef CLIENT_MIXER_SETTING_H
#define CLIENT_MIXER_SETTING_H

#include "AudioMixerSetting.h"

class RemoteCall;

class ClientMixerSetting : public AudioMixerSetting
{
public:
    ClientMixerSetting();
    virtual ~ClientMixerSetting();
    status_t setUpMixMode(unsigned int mode);
    status_t mixExtGps(unsigned int mode);
    status_t setGpsMixerGain(audio_mixer_float_gain_t* gain);
    //status_t setDunkingMode(unsigned int mode);
    //status_t setDunkingTargetGain(float dbLevel);
private:
    RemoteCall* mRc;
};

#endif //CLIENT_MIXER_SETTING_H