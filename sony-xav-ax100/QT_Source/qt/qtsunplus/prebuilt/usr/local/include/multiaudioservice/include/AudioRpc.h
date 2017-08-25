/**
 * @file AudioRpc.h
 * @authors mm.li xun.lu
 * @date 2015-11-03
 * @version
 * @copyright SunMedia Technology (Chengdu) co. Ltd.
 * @brief declarates some types
 */
#ifndef AUDIO_RPC_H
#define AUDIO_RPC_H

#include "AudioDef.h"
#include "AudioTypes.h"

enum
{
	PARAM_HEAD = 0xADDF,
};

typedef enum
{
	CONNECT_TRACK = 0x100,                      //: ClientTrack
	CONNECT_TRACK_MUSIC,

	CONNECT_RECORD = 0x200,                         //: ClientRecord

	CONNECT_VOICE = 0x400,                          //: ClientVoice

	CONNECT_CTRL = 0x800,                           //: ClientCtrl
	CONNECT_APP_SETTING,
	CONNECT_CODEC_SETTING,
	CONNECT_MIXER_SETTING,
	CONNECT_SOURCE_SETTING,
	CONNECT_VOLUME_SETTING,
	CONNECT_FIFO_SETTING,
	CONNECT_MIC_SETTING,

	#if EN_SIT_AS_TEST
	CONNECT_AS_TEST = 0x1000,
	#endif
	
	CONNECT_DEFAULT = CONNECT_TRACK,        //: default in server
} audio_connect_type;

typedef enum
{
	SOCKET_NONE                 = 0,
	SOCKET_TRACK_CONNECTINT     = 1,
	SOCKET_RECORD_CONNECTINT    = 2,
	SOCKET_VOICE_CONNECTINT     = 3,
	SOCKET_CTRL_CONNECTINT      = 4,
	SOCKET_FD_ERR               = 5,
	SOCKET_FD_READ              = 6,
	SOCKET_FD_WRITE             = 7,
	SOCKET_UNKNOWN,
} event_type;

/** @todo remove */
typedef enum
{

	ID_CMD_IOCTL_SPDIF_SET_MODE = 0,
	ID_CMD_IOCTL_SPDIF_SET_CGMS = 1,
	ID_CMD_IOCTL_SPDIF_UPDATE = 2,
	ID_CMD_IOCTL_SPDIF_GET_MODE = 3,
	ID_CMD_IOCTL_SPDIF_SET_FS = 4,
	ID_CMD_IOCTL_SPDIF_MIN = ID_CMD_IOCTL_SPDIF_SET_MODE,
	ID_CMD_IOCTL_SPDIF_MAX = ID_CMD_IOCTL_SPDIF_SET_FS,

	ID_CMD_IOCTL_HDMI_SET_MODE = 5,
	ID_CMD_IOCTL_HDMI_UPDATE = 6,
	ID_CMD_IOCTL_HDMI_GET_MODE = 7,
	ID_CMD_IOCTL_HDMI_SET_FS = 8,
	ID_CMD_IOCTL_HDMI_MIN = ID_CMD_IOCTL_HDMI_SET_MODE,
	ID_CMD_IOCTL_HDMI_MAX = ID_CMD_IOCTL_HDMI_SET_FS,

	ID_CMD_IOCTL_PCM_SET_GAIN = 9,
	ID_CMD_IOCTL_PCM_GET_GAIN = 10,
	ID_CMD_IOCTL_PCM_MIN = ID_CMD_IOCTL_PCM_SET_GAIN,
	ID_CMD_IOCTL_PCM_MAX = ID_CMD_IOCTL_PCM_GET_GAIN,

	ID_CMD_IOCTL_BT_SET_MODE = 11,
	ID_CMD_IOCTL_BT_MIN = ID_CMD_IOCTL_BT_SET_MODE,
	ID_CMD_IOCTL_BT_MAX = ID_CMD_IOCTL_BT_SET_MODE,

	ID_CMD_IOCTL_SET_PTS_MODE = 12,
	ID_CMD_IOCTL_SET_PTS = 13,
	ID_CMD_IOCTL_GET_PTS = 14,
	ID_CMD_IOCTL_SET_MUTE = 15,
	ID_CMD_IOCTL_SET_DEMUTE = 16,
	ID_CMD_IOCTL_SET_GAIN = 17,
	ID_CMD_IOCTL_GET_GAIN = 18,
	ID_CMD_IOCTL_GET_GAIN_INFO = 19,
	ID_CMD_IOCTL_SET_BS_FORMAT = 20,
	ID_CMD_IOCTL_SET_BS_LAYOUT = 21,
	ID_CMD_IOCTL_SET_BS_FS = 22,
	ID_CMD_IOCTL_SET_FS = 23,
	ID_CMD_IOCTL_SET_I2SRX_MIXER = 24,
	ID_CMD_IOCTL_SET_OUTPUT_MODE = 25,
	ID_CMD_IOCTL_SET_LINEIN_PATH = 26,
	ID_CMD_IOCTL_GET_LINEIN_PATH = 27,
	ID_CMD_IOCTL_SET_MUXIN_PATH = 28,
	ID_CMD_IOCTL_GET_MUXIN_PATH = 29,
	ID_CMD_IOCTL_SET_I2SFMT = 30,
	ID_CMD_IOCTL_GET_I2SFMT = 31,
	ID_CMD_IOCTL_SET_AGC_MODE = 32,
	ID_CMD_IOCTL_GET_AGC_MODE = 33,
	ID_CMD_IOCTL_SET_AGC_ENERGY = 34,
	ID_CMD_IOCTL_GET_AGC_ENERGY = 35,
	ID_CMD_IOCTL_SET_ADC_SYSTEMGAIN = 36,
	ID_CMD_IOCTL_GET_ADC_SYSTEMGAIN = 37,
	ID_CMD_IOCTL_SET_DAC_AUTOSLEEP = 38,
	ID_CMD_IOCTL_GET_DAC_AUTOSLEEP = 39,
	ID_CMD_IOCTL_SET_ADC_ANALOGGAIN = 40,
	ID_CMD_IOCTL_GET_ADC_ANALOGGAIN = 41,
	ID_CMD_IOCTL_ALL_MIN = ID_CMD_IOCTL_SET_PTS_MODE,
	ID_CMD_IOCTL_ALL_MAX = ID_CMD_IOCTL_GET_ADC_ANALOGGAIN,

	ID_CMD_IOCTL_MIN = ID_CMD_IOCTL_SPDIF_MIN,
	ID_CMD_IOCTL_MAX = ID_CMD_IOCTL_ALL_MAX,

	ID_CMD_TRACK_CREATE,                    //: Create Track
	ID_CMD_TRACK_REMOVE,                    //: Remove Track
	ID_CMD_TRACK_FLUSH,                     //: Flush Track Data to Driver
	ID_CMD_TRACK_START,                     //: Start Track
	ID_CMD_TRACK_STARTRECORD,               //: Start Track
	ID_CMD_TRACK_STOP,                      //: Stop Track
	ID_CMD_TRACK_RAMPUP,                    //:set Duration of RampUp
	ID_CMD_TRACK_RAMPDOWN,                  //:set Duration of RampDown
	ID_CMD_TRACK_ATUORAMP,                  //:Auto Ramp Duration
	ID_CMD_TRACK_ATUOMUTE_DURATION,         //:Auto Mute Duration before play
	ID_CMD_TRACK_RESET,                     //: Reset Track
	ID_CMD_TRACK_PAUSE,                     //: Pause Track
	ID_CMD_TRACK_RESUME,                    //: Resume Track
	ID_CMD_TRACK_OUT_DATA_REDAY,            //: Out Track Data Ready
	ID_CMD_TRACK_IN_DATA_REDAY,             //: In Track Data Ready
	ID_CMD_TRACK_CLEAR_MIX_BUFFER,          //: Clear Mix Buffer which track or record used
	ID_CMD_TRACK_DUCK,                      //: duck this track
	ID_CMD_TRACK_UNDUCK,                    //: duck this track
	ID_CMD_TRACK_SETNOISEENABLE,            //: set noise enable or not this on track
	ID_CMD_TRACK_SETAECENABLE,              //: set AEC enable or not this on track
	ID_CMD_TRACK_SETAGCENABLE,              //: set AGC enable or not this on track

	ID_CMD_SET_SAMPLERATE,                  //: Set SampleRate
	ID_CMD_SET_FORMAT,                      //: Set Format
	ID_CMD_SET_CHANNEL,                     //: Set Channel
	ID_CMD_SET_SHRMEM_ADDR,                 //: Set ShareMem Addr
	ID_CMD_SET_STREAM_TYPE,                 //: Set StreamType
	ID_CMD_SET_TAG,                         //: Set Tag
	ID_CMD_SET_TAG_OUTPUT,                  //: Set Tag of Out
	ID_CMD_SET_TAG_INPUT,                   //: Set Tag of In
	ID_CMD_SET_PTS,                         //: Set PTS
	ID_CMD_SET_FLAG_MIX_INPUT,              //: Set if mix input to output or not
	ID_CMD_SET_POSITION,                    //: Set Position of track
	ID_CMD_SET_VOLUME,                      //: Set Volume of track
	ID_CMD_SET_OTHER_VOLUME,                //: Set Volume of track
	ID_CMD_SET_BUFFER_SIZE,                 //: Set BufferSize of ShareMem
	ID_CMD_SET_PID,                         //: Set SampleRate
	ID_CMD_TRACK_SET_MIN = ID_CMD_TRACK_CREATE,
	ID_CMD_TRACK_SET_MAX = ID_CMD_SET_PID,

	ID_CMD_SYS_SET_NONE,
	ID_CMD_SET_STREAM_VOLUME,               //: Set Volume of stream
	ID_CMD_SET_STREAM_REAL_VOLUME,          //: Set Volume of stream
	ID_CMD_SET_STREAM_MUTE,                 //: Set mute of MASTER
	ID_CMD_SET_MASTER_VOLUME,               //: Set Volume of MASTER
	ID_CMD_SET_MASTER_DEVIATION_VOLUME,     //: Set Volume of MASTER
	ID_CMD_SET_MASTER_REAL_VOLUME,          //: Set Volume of MASTER
	ID_CMD_SET_MASTER_MUTE,                 //: Set mute of stream
	ID_CMD_SET_VOLUME_CHANGE_TIME,          //: Set time of Changing Volume
	ID_CMD_SET_SYSTEM_MODE,                 //: Set System Mode
	ID_CMD_SET_GPS_PERCENT_VOLUME,
	ID_CMD_SET_LATENCY_BT_8K,
	ID_CMD_SET_LATENCY_BT_16K,
	ID_CMD_SET_LATENCY_CARPLAY_8K,
	ID_CMD_SET_LATENCY_CARPLAY_16K,
	ID_CMD_SET_LATENCY_CARPLAY_24K,
	ID_CMD_SET_LATENCY_DEFAULT,
	ID_CMD_SET_OUTPUT_FIFO,
	ID_CMD_SYS_SET_CNT,

	ID_CMD_GET_SOCKET_READY,

	ID_CMD_GET_PTS,                         //: Get PTS
	ID_CMD_GET_VOLUME,                      //: Get Volume of track
	ID_CMD_GET_POSITION,                    //: Get Position of track
	ID_CMD_GET_OTHER_VOLUME,                //: Get Volume of track
	ID_CMD_GET_LATENCY,                     //: Get Latency of track
	ID_CMD_GET_SHRMEM_ADDR,                 //: Get Addr of ShareMem
	ID_CMD_GET_EXTRA_SHRMEM_ADDR,           //: Get Addr of ShareMem
	ID_CMD_GET_BUFFER_SIZE,                 //: Get BufferSize of ShameMem
	ID_CMD_GET_WRITE_READY,                 //: Get write ready
	ID_CMD_GET_READ_READY,                  //: Get read ready
	ID_CMD_GET_PERLOOP_SAMPLES,             //: Get Samples of Processing perLoop
	ID_CMD_GET_PERLOOP_TIME,                //: Get Time of Processing perLoop
	ID_CMD_GET_PERLOOP_SIZE,                //: Get BufferSize of Processing perLoop
	ID_CMD_TRACK_GET_MIN = ID_CMD_GET_PTS,
	ID_CMD_TRACK_GET_MAX = ID_CMD_GET_PERLOOP_SIZE,

	ID_CMD_SYS_GET_NONE,
	ID_CMD_GET_STREAM_VOLUME,               //: Get Volume of stream
	ID_CMD_GET_STREAM_REAL_VOLUME,          //: Get Volume of stream
	ID_CMD_GET_STREAM_MUTE,                 //: Get mute of stream
	ID_CMD_GET_MASTER_VOLUME,               //: Get Volume of MASTER
	ID_CMD_GET_MASTER_DEVIATION_VOLUME,     //: Set Volume of MASTER
	ID_CMD_GET_MASTER_REAL_VOLUME,          //: Get Volume of MASTER
	ID_CMD_GET_MASTER_MUTE,                 //: Get mute of MASTER
	ID_CMD_GET_VOLUME_CHANGE_TIME,          //: Get time of Changing Volume
	ID_CMD_GET_LATENCY_BT_8K,
	ID_CMD_GET_LATENCY_BT_16K,
	ID_CMD_GET_LATENCY_CARPLAY_8K,
	ID_CMD_GET_LATENCY_CARPLAY_16K,
	ID_CMD_GET_LATENCY_CARPLAY_24K,
	ID_CMD_GET_CURENT_VOICE_TYPE,
	ID_CMD_SYS_GET_CNT,

	ID_CMD_BT_A2DP_NONE,                    //:
	ID_CMD_BT_A2DP_STANDBY,                 //: A2dp Standby
	ID_CMD_BT_A2DP_SET_STATE,               //: Set state of A2dp
	ID_CMD_BT_A2DP_CLOSE,                   //: Close A2dp
	ID_CMD_BT_A2DP_SUSPEND,                 //: Suspend A2dp
	ID_CMD_BT_A2DP_IN_START,                //: A2dp start
	ID_CMD_BT_A2DP_IN_STOP,                 //: A2dp stop
	ID_CMD_BT_A2DP_IN_PAUSE,                //: A2dp pase
	ID_CMD_BT_A2DP_IN_SET_RATE,             //: Set A2dp In SampleRate
	ID_CMD_BT_A2DP_IN_SET_VOLUME,           //: Set A2dp In Volume
	ID_CMD_BT_A2DP_IN_GET_VOLUME,           //: Get A2dp In Volume
	ID_CMD_BT_A2DP_IN_AUTO_RAMP,            //: Set A2dp IN Ramp Up and Down
	ID_CMD_BT_A2DP_IN_AUTO_MUTE,            //: Set A2dp IN Mute time before start
	ID_CMD_BT_A2DP_OUT_START,               //: A2dp start
	ID_CMD_BT_A2DP_OUT_STOP,                //: A2dp stop
	ID_CMD_BT_A2DP_OUT_PAUSE,               //: A2dp pase
	ID_CMD_BT_A2DP_OUT_SET_RATE,            //: Set A2dp Out SampleRate
	ID_CMD_BT_A2DP_CNT,

	ID_CMD_DISC_4POINT0_IN_START,           //:Dvd mode 4.0ch start
	ID_CMD_DISC_4POINT0_IN_STOP,            //:Dvd mode 4.0ch stop
	ID_CMD_DISC_4POINT0_IN_PAUSE,           //:Dvd mode 4.0ch pause
	ID_CMD_DISC_4POINT0_IN_SET_RATE,        //:Dvd mode 4.0ch set rate
	ID_CMD_DISC_4POINT0_IN_SET_VOLUME,      //:Dvd mode 4.0ch set Volume
	ID_CMD_DISC_4POINT0_IN_GET_VOLUME,      //:Dvd mode 4.0ch set Volume
	ID_CMD_DISC_4POINT0_IN_AUTO_RAMP,       //:Set Dvd mode 4.0ch IN Ramp Up and Down
	ID_CMD_DISC_4POINT0_IN_SET_FRONT_TAG,       //Dvd mode 4.0ch set front tag
	ID_CMD_DISC_4POINT0_IN_SET_BACK_TAG,        //Dvd mode 4.0ch set center tag
	ID_CMD_DISC_4POINT0_IN_SET_FULL_TAG,        //Dvd mode 4.0ch set full car tag

	ID_CMD_BT_HFP_NONE,
	ID_CMD_BT_HFP_STANDBY,                  //: Hfp Standby
	ID_CMD_BT_HFP_SET_STATE,                //: Set State of Hfp
	ID_CMD_BT_HFP_CLOSE,                    //: Close Hfp
	ID_CMD_BT_HFP_SUSPEND,                  //: Suspend Hfp
	ID_CMD_BT_HFP_IN_START,                 //: A2dp start
	ID_CMD_BT_HFP_IN_STOP,                  //: A2dp stop
	ID_CMD_BT_HFP_IN_PAUSE,                 //: A2dp pase
	ID_CMD_BT_HFP_IN_AUTO_RAMP,     //:Set Hfp IN Ramp Up and Down
	ID_CMD_BT_HFP_IN_AUTO_MUTE,            //: Set HFP IN Mute time before start
	ID_CMD_BT_HFP_START,                    //: Hfp start
	ID_CMD_BT_HFP_STOP,                     //: Hfp stop
	ID_CMD_BT_HFP_RING_START,               //: Hfp Ring start
	ID_CMD_BT_HFP_RING_STOP,                //: Hfp Ring stop
	ID_CMD_BT_HFP_PAUSE,                    //: Hfp pase
	ID_CMD_BT_HFP_MIC_START,                //: Hfp Mic start
	ID_CMD_BT_HFP_MIC_STOP,                 //: Hfp Mic stop
	ID_CMD_BT_HFP_MIC_PAUSE,                //: Hfp Mic pase
	ID_CMD_BT_HFP_SET_RATE,                 //: Set Hfp SampleRate
	ID_CMD_BT_HFP_SET_VOLUME,               //: Set Hfp Volume
	ID_CMD_BT_HFP_GET_VOLUME,               //: Get Hfp Volume
	ID_CMD_BT_HFP_CNT,

	ID_CMD_EFFECT_NONE,
	ID_CMD_EFFECT_CONFIG,                   //: Effect Config
	ID_CMD_EFFECT_ENABLE,                   //: Effect Enable
	ID_CMD_EFFECT_DISABLE,                  //: Effect Disable
	ID_CMD_EFFECT_COMMAND,                  //: Effect Command
	ID_CMD_EFFECT_CNT,
	ID_CMD_EFFECT_SET_PARAM,
	ID_CMD_EFFECT_GET_PARAM,

	ID_CMD_FULL_MODE_ON,
	ID_CMD_FULL_MODE_OFF,

	ID_CMD_SUBWOOFER_MODE_ON,
	ID_CMD_SUBWOOFER_MODE_OFF,

	ID_CMD_SET_OK,                          //: Set OK
	ID_CMD_GET_OK,                          //: Get OK
	ID_CMD_SET_FAIL,                        //: Set FAIL
	ID_CMD_GET_FAIL,                        //: GET FAIL
	ID_CMD_UNKNOWN,
	ID_CMD_NO_ALLOWED,

	////////////////////////////////////////////////
	ID_CMD_RPC,
	ID_CMD_RPC_CREATE,
	ID_CMD_RPC_DESTROY,
	ID_CMD_ASYNC_RPC,
	////////////////////////////////////////////////
} audio_cmd;

typedef struct
{
	unsigned int cmdId;
	unsigned int value;
	unsigned int value1;
	unsigned int value2;
	long long    timestamp;
} audio_param_t;


#define CMD_SIZE sizeof(audio_param_t)


#endif //AUDIO_TYPES_H
