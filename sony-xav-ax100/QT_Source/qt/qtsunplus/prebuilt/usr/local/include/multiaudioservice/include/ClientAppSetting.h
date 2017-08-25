/**
 * @file ClientAppSetting.h
 * @author xun.lu
 * @date 2016-01-13
 * @version 
 * @copyright SunMedia Technology (Chengdu) co. Ltd.
 * @brief declarates of ClientAppSetting
 */
#ifndef CLIENT_APP_SETTING_H
#define CLIENT_APP_SETTING_H

#include "AudioAppSetting.h"
class RemoteCall;

/**
 * @class ClientAppSetting
 * @brief client API of DSP's APP setting.
 * @details  bass/treble/loudness/equalizer/filter are included 
 */
class ClientAppSetting : public AudioAppSetting
{
public:
    ClientAppSetting();
    virtual ~ClientAppSetting();
    
    status_t setBassManagement(audio_bm_param_t* param);
    status_t setBassTreble(audio_bt_param_t* param);
    status_t getBassManagement(audio_bm_param_t* param);

    status_t setLoudness(audio_loudness_param_t* param);

    status_t setEqualizer(audio_eq_param_t* param);
    status_t getEqualizer(audio_eq_param_t* param);
    status_t getEqualizerQValueTable(audio_eq_qvalue_table_t* param);
    status_t getEqualizerCfTable(audio_eq_cf_table_t* param);
    status_t getEqualizerSpectrum(audio_eq_spectrum_t* param);

    status_t setFilter(audio_filter_param_t* param);
private:
    RemoteCall* mRc;
};

#endif //CLIENT_APP_SETTING_H