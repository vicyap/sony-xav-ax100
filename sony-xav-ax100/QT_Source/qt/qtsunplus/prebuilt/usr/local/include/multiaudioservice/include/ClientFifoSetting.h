/**
 * @file ClientFifoSetting.h
 * @author xun.lu
 * @date 2016-03-15
 * @version
 * @copyright SunMedia Technology (Chengdu) co. Ltd.
 * @brief declarates of ClientFifoSetting
 */
#ifndef CLIENT_FIFO_SETTING_H
#define CLIENT_FIFO_SETTING_H

#include "AudioFifoSetting.h"

class RemoteCall;

class ClientFifoSetting : public AudioFifoSetting
{
public:
	ClientFifoSetting(unsigned int rate = SR_48000);
	virtual ~ClientFifoSetting();
	status_t getFifoOutPutSampleCounter(unsigned int* counter, long long* host = NULL);
private:
	status_t setInputSampleRate(unsigned int rate);
	RemoteCall* mRc;
	unsigned int mSampleRate;
};

#endif //CLIENT_FIFO_SETTING_H