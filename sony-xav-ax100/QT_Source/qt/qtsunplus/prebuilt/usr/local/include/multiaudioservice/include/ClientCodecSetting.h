/**
 * @file ClientCodecSetting.h
 * @author xun.lu
 * @date 2016-01-13
 * @version 
 * @copyright SunMedia Technology (Chengdu) co. Ltd.
 * @brief declarates of ClientCodecSetting
 */
#ifndef CLIENT_CODEC_SETTING_H
#define CLIENT_CODEC_SETTING_H

#include "AudioCodecSetting.h"

class RemoteCall;


// FIXME part of driver ? cant be accessed by user?
class ClientCodecSetting : public AudioCodecSetting
{
public:
    ClientCodecSetting();
    virtual ~ClientCodecSetting();
    
    status_t setDacPowerDown();
    status_t setDacPowerOn();
    status_t setAdcPowerDown();
    status_t setAdcPowerOn();
    status_t setInstanBootPowerDown();
    status_t setInstanBootPowerOn();
    status_t setI2SPinOut();
private:
    RemoteCall* mRc;
};

#endif //CLIENT_CODEC_SETTING_H