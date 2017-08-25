/**
 * @file AudioAppSetting.h
 * @author xun.lu
 * @date 2016-01-08
 * @version
 * @copyright SunMedia Technology (Chengdu) co. Ltd.
 * @brief declaration of AudioAppSetting
 */
#ifndef AUDIO_APP_SETTING_H
#define AUDIO_APP_SETTING_H

#include "AudioTypes.h"
#include "AudioError.h"


/*! the data structure of parametric equalizer curve */
typedef struct
{
	audio_channel_idx_e nChannel;
	unsigned short nEqFc[8];        /*!< Cutoff frequency, 20 Hz to 20 kHz in 1/6 octave steps. Disable this band if cf is 0 */
	float sEqGain[8];       /*!< equalizer gain ranging from -12 to 12 dB, each step is 0.5 */
	unsigned short sEqQ[8];     /*!< equalizer Q factor, range from 0.5 to 10, each step is 0.5 */
} audio_eq_param_t;

/*! the data structure of parametric equalizer curve for bass and treble */
typedef struct
{
	audio_channel_idx_e nChannel;
	unsigned short nEqFc[2];        /*!< Cutoff frequency, 20 Hz to 20 kHz in 1/6 octave steps. Disable this band if cf is 0 */
	float sEqGain[2];       /*!< equalizer gain ranging from -12 to 12 dB, each step is 0.5 */
} audio_bt_param_t;

typedef struct
{
	audio_channel_idx_e nChannel;
	unsigned short nEnable;		/*!< enable(1)/disable(0) bass management, other parameters are ignored if disable */
	unsigned short nCutFreq;		/*!< Cutoff frequency, 20 Hz to 640Hz. Disable this band if 0. Given table index instead of freq value*/
	unsigned short nFilterType;	/*!< 0 for lowpass, 1 for highpass */
	unsigned short nSlope;		/*!< 6, 12, 18, 24 (dB/oct) */
} audio_bm_param_t;

/*! the data structure of loudness */
typedef struct
{
	unsigned short nEnable;         /*!< loudness enable/disable swtich */
	unsigned short nHighBoost;      /*!< boost high frequence on/off switch */
	unsigned short nFreq;               /*!< peak frequency, only support 400, 800, 2400 */
} audio_loudness_param_t;

/*! the data structure of App filter */
typedef struct
{
	audio_channel_idx_e nChannel;
	short nLevel;       /*!< 0 ~ (-15) dB */
	unsigned short Method;  /*!< 0 for butterworth, 1 for Linkwitz-Riley */
	unsigned short nSlope;  /*!< 6, 12, 18, 24 (dB/oct) */
	unsigned short type;        /*!< 0 for lowpass, 1 for highpass */
	unsigned short phase;       /*!< 0 or 180 degree */
	unsigned short nFreq;       /*!< Cutoff frequency, 20 Hz to 640Hz. Disable this band if 0. Given table index instead of freq value*/
} audio_filter_param_t;

/** the structure of qvalue table */
typedef struct
{
	float nTable[16];
} audio_eq_qvalue_table_t;

/** the structure of cf table */
typedef struct
{
	unsigned int nTable[64];
} audio_eq_cf_table_t;

/** the structure of cf spec trum */
typedef struct
{
	unsigned short nTable[8];
} audio_eq_spectrum_t;


/**
 * @class AudioAppSetting
 * @brief API of DSP's APP setting.
 * @details  bass/treble/loudness/equalizer/filter are included
 */
class AudioAppSetting
{
public:
	AudioAppSetting() {}
	virtual ~AudioAppSetting() {}

	/**
	    * @fn status_t setBassManagement(audio_bm_param_t* param)
	    * @brief API of bass setting
	    * @param param audio_bm_param_t pointer
	    * @return error number which defined in AudioError.h.
	    */
	virtual status_t setBassManagement(audio_bm_param_t* param) = 0;

	/**
	    * @fn status_t setBassTreble(audio_bt_param_t* param)
	    * @brief API of treble setting
	    * @param param audio_bt_param_t pointer
	    * @return error number which defined in AudioError.h.
	    */
	virtual status_t setBassTreble(audio_bt_param_t* param) = 0;

	/**
	    * @fn status_t getBassManagement(audio_bm_param_t* param)
	    * @brief API of getting bass param.
	    * @param param audio_bm_param_t pointer
	    * @return error number which defined in AudioError.h.
	    */
	virtual status_t getBassManagement(audio_bm_param_t* param) = 0;

	/**
	    * @fn status_t setLoudness(audio_loudness_param_t* param)
	    * @brief API of loudness setting
	    * @param param audio_loudness_param_t pointer
	    * @return error number which defined in AudioError.h.
	    */
	virtual status_t setLoudness(audio_loudness_param_t* param) = 0;

	/**
	    * @fn status_t setEqualizer(audio_eq_param_t* param)
	    * @brief API of eq setting
	    * @param param audio_eq_param_t pointer
	    * @return error number which defined in AudioError.h.
	    */
	virtual status_t setEqualizer(audio_eq_param_t* param) = 0;

	/**
	    * @fn status_t getEqualizer(audio_eq_param_t* param)
	    * @brief API of getting eq param.
	    * @param param audio_eq_param_t pointer
	    * @return error number which defined in AudioError.h.
	    */
	virtual status_t getEqualizer(audio_eq_param_t* param) = 0;

	/**
	    * @fn status_t getEqualizerQValueTable(audio_eq_qvalue_table_t* param)
	    * @brief API of getting eq qvalue.
	    * @param param audio_eq_qvalue_table_t pointer
	    * @return error number which defined in AudioError.h.
	    */
	virtual status_t getEqualizerQValueTable(audio_eq_qvalue_table_t* param) = 0;

	/**
	    * @fn status_t getEqualizerCfTable(audio_eq_cf_table_t* param)
	    * @brief API of getting eq cf.
	    * @param param audio_eq_cf_table_t pointer
	    * @return error number which defined in AudioError.h.
	    */
	virtual status_t getEqualizerCfTable(audio_eq_cf_table_t* param) = 0;

	/**
	   * @fn status_t getEqualizerSpectrum(audio_eq_spectrum_t* param)
	   * @brief API of getting eq spectrum.
	   * @param param audio_eq_spectrum_t pointer
	   * @return error number which defined in AudioError.h.
	   */
	virtual status_t getEqualizerSpectrum(audio_eq_spectrum_t* param) = 0;

	/**
	    * @fn status_t setFilter(audio_filter_param_t* param)
	    * @brief API of filter setting
	    * @param param audio_filter_param_t pointer
	    * @return error number which defined in AudioError.h.
	    */
	virtual status_t setFilter(audio_filter_param_t* param) = 0;
};

#endif //AUDIO_APP_SETTING_H
