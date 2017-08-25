/**
 * @file AudioSourceSetting.h
 * @author xun.lu
 * @date 2016-01-08
 * @version
 * @copyright SunMedia Technology (Chengdu) co. Ltd.
 * @brief declaration of AudioSourceSetting
 */
#ifndef AUDIO_SOURCE_SETTING_H
#define AUDIO_SOURCE_SETTING_H

#include "AudioTypes.h"
#include "AudioError.h"


typedef struct
{
	audio_seat_id_e seat;
	audio_source_id_e source;
} audio_select_source_param_t;


/**
 * @class AudioSourceSetting
 * @brief API of DSP's source setting
 * @details  source selector/seat state
 */
class AudioSourceSetting
{
public:
	AudioSourceSetting() {}
	virtual ~AudioSourceSetting() {}

	/**
	    * @fn status_t selectSource(audio_seat_id_e seat, audio_source_id_e source)
	    * @brief setting of seat source
	    * @param audio_seat_id_e  front or rear seat
	    * @param audio_source_id_e source(only one source can be bind to one seat at same time
	    * @return error number which defined in AudioError.h.
	    * @todo which source can be selected by user? STREAM1? STREAM2? AUXIN? I2SIN?
	  */
	virtual status_t selectSource(audio_seat_id_e seat, audio_source_id_e source) = 0;
	/**
	    * @fn status_t setFrontAudioState(audio_seat_state_e state)
	    * @brief set front speaker to play or stop
	    * @param state init/play/stop
	    * @return error number which defined in AudioError.h.
	    */
	virtual status_t setFrontAudioState(audio_seat_state_e state) = 0;

	/**
	    * @fn status_t setRearAudioState(audio_seat_state_e state)
	    * @brief set rear speaker to play or stop
	    * @param state init/play/stop
	    * @return error number which defined in AudioError.h.
	    */
	virtual status_t setRearAudioState(audio_seat_state_e state) = 0;
};

#endif //AUDIO_SOURCE_SETTING_H
