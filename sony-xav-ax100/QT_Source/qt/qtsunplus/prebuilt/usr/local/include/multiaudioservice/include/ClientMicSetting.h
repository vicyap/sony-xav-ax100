/**
 * @file ClientMicSetting.h
 * @author xun.lu
 * @date 2016-05-30
 * @version
 * @copyright SunMedia Technology (Chengdu) co. Ltd.
 * @brief declaration of ClientMicSetting
 */
#ifndef CLIENT_MIC_SETTING_H
#define CLIENT_MIC_SETTING_H

#include "AudioMicSetting.h"

class RemoteCall;

class ClientMicSetting : public AudioMicSetting
{
public:
	ClientMicSetting();
	virtual ~ClientMicSetting();

	status_t setUpLinkGain(float gain);
private:
	RemoteCall* mRc;
};
#endif //CLIENT_MIC_SETTING_H