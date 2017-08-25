/**
 * @file ClientSourceSetting.h
 * @author xun.lu
 * @date 2016-01-13
 * @version 
 * @copyright SunMedia Technology (Chengdu) co. Ltd.
 * @brief declarates of ClientSourceSetting
 */
#ifndef CLIENT_SOURCE_SETTING_H
#define CLIENT_SOURCE_SETTING_H

#include "AudioSourceSetting.h"

class RemoteCall;

class ClientSourceSetting : public AudioSourceSetting
{
public:
    ClientSourceSetting();
    virtual ~ClientSourceSetting();
    /*
        * what's source can be selected by user? STREAM1? STREAM2? AUXIN? I2SIN?
      */
    status_t selectSource(audio_seat_id_e seat, audio_source_id_e source);
    status_t setFrontAudioState(audio_seat_state_e state);
    status_t setRearAudioState(audio_seat_state_e state);
private:
    RemoteCall* mRc;
};

#endif //CLIENT_SOURCE_SETTING_H