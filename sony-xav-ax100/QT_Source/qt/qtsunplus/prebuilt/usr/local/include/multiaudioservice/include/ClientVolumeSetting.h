/**
 * @file ClientVolumeSetting.h
 * @author xun.lu
 * @date 2016-01-13
 * @version
 * @copyright SunMedia Technology (Chengdu) co. Ltd.
 * @brief declarates of ClientVolumeSetting
 */
#ifndef CLIENT_VOLUME_SETTING_H
#define CLIENT_VOLUME_SETTING_H

#include "AudioVolumeSetting.h"


class RemoteCall;

class ClientVolumeSetting : public AudioVolumeSetting
{
public:
	ClientVolumeSetting();
	virtual ~ClientVolumeSetting();

	status_t setMainAudioDunkingMode(unsigned int mode);
	status_t setMainAudioDunkingTargetGain(unsigned char volume);

	status_t setMasterVolume(unsigned char volume); //AudioService/GPS/Effect/DSP Out
	status_t getMasterVolume(unsigned char* volume);//AudioService/GPS/Effect/DSP Out

	status_t setChannelGain(audio_channel_gain_t* gain); // AudioService/DSP Out
	status_t getChannelGain(audio_channel_gain_t* gain); // AudioService/DSP Out

	status_t setStreamVolume(audio_source_id_e source, unsigned char volume);
	status_t getStreamVolume(audio_source_id_e source, unsigned char* volume);
private:
	RemoteCall* mRc;
};

#endif //CLIENT_VOLUME_SETTING_H
