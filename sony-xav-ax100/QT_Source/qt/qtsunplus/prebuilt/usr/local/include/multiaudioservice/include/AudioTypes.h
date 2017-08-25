/**
 * @file AudioTypes.h
 * @authors mm.li xun.lu
 * @date 2015-12-12
 * @version
 * @copyright SunMedia Technology (Chengdu) co. Ltd.
 * @brief eclarates some types
 */
#ifndef AUDIO_TYPES_H
#define AUDIO_TYPES_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

#include "AudioDef.h"

typedef int status_t;

typedef struct audio_config_s
{
	int rate;
	int fmt;
	int chl;
	unsigned int frameSize;
	unsigned int chs;
	int tag;
	int bufSize;
	unsigned int buf_phy_addr;
	int volume;
	int overFlow;
	int underFlow;
} audio_config_t;

typedef enum _audio_channel_idx_e
{
	AUDIO_CHANNEL_LF      = (0x1<<0),     /*!< Front Left */
	AUDIO_CHANNEL_RF      = (0x1<<1),     /*!< Front Right */
	AUDIO_CHANNEL_LS      = (0x1<<2),     /*!< Surround Left */
	AUDIO_CHANNEL_RS      = (0x1<<3),     /*!< Surround Right */
	AUDIO_CHANNEL_CENTER  = (0x1<<4),     /*!< Center */
	AUDIO_CHANNEL_LEF     = (0x1<<5),     /*!< Subwoofer */
	AUDIO_CHANNEL_LINK    = 0xffff,       /*!< 5.1ch link */
} audio_channel_idx_e;

typedef enum
{
	AUDIO_SEAT_ID_FRONT = 1,       /*!<  front seat id: 1*/
	AUDIO_SEAT_ID_REAR = 2,        /*!<  rear seat id: 2*/
} audio_seat_id_e;

/*! Audio source id definition, used to select a source for front/rear seat */
typedef enum
{
	AUDIO_SOURCES_ID_STREAM1 = 1,       /*!<  audio decoder 1 */
	AUDIO_SOURCES_ID_STREAM2 = 2,       /*!<  audio decoder 2 */
	AUDIO_SOURCES_ID_MIC = 3,           /*!<  mic-in data */
	AUDIO_SOURCES_ID_AUXIN = 4,     /*!<  aux-in data */
	AUDIO_SOURCES_ID_I2SIN= 5,          /*!<  i2s-in data*/
	AUDIO_SOURCES_ID_HF  = 6,           /*!<  handsfree application */
	AUDIO_SOURCES_ID_ID_MAX,
} audio_source_id_e;

typedef enum
{
	AUDIO_SEAT_STATE_INIT = 0,       /*!< init state of audio seat */
	AUDIO_SEAT_STATE_STOP = 1,       /*!< stop state of audio seat */
	AUDIO_SEAT_STATE_PLAY = 2,       /*!< play state of audio seat */
} audio_seat_state_e;

typedef enum
{
	AUDIO_FIFO_TAG_A0,
	AUDIO_FIFO_TAG_A1,
	AUDIO_FIFO_TAG_A2,
	AUDIO_FIFO_TAG_A3,
	AUDIO_FIFO_TAG_A4,
	AUDIO_FIFO_TAG_A5,
	AUDIO_FIFO_TAG_A6,
	AUDIO_FIFO_TAG_A7,
}audio_fifo_tag_e;

typedef enum  // if modify this ,please modeif ClientFormat.java at the same time
{
	CHL_NONE = -1,
	CHL_MONO,
	CHL_STEREO   ,
	CHL_2POINT1,
	CHL_2_1 ,
	CHL_SURROUND,
	CHL_3POINT1 ,
	CHL_4POINT0 ,
	CHL_4POINT1 ,
	CHL_2_2 ,
	CHL_QUAD,
	CHL_5POINT0,
	CHL_5POINT1 ,
	CHL_5POINT0_BACK,
	CHL_5POINT1_BACK,
	CHL_6POINT0 ,
	CHL_6POINT0_FRONT,
	CHL_HEXAGONAL,
	CHL_6POINT1 ,
	CHL_6POINT1_BACK,
	CHL_6POINT1_FRONT    ,
	CHL_7POINT0 ,
	CHL_7POINT0_FRONT,
	CHL_7POINT1 ,
	CHL_7POINT1_WIDE,
	CHL_7POINT1_WIDE_BACK,
	CHL_OCTAGONAL    ,
	CHL_STEREO_DOWNMIX,
	CHL_MONO_LEFT,
	CHL_MONO_RIGHT,
	CHL_CNT,
	CHL_DEFAULT = CHL_STEREO,
} multi_audio_chl;

typedef enum  // if modify this ,please modeif ClientFormat.java at the same time
{
	FMT_NONE = -1,
	FMT_U8,          ///< unsigned 8 bits
	FMT_S16,         ///< signed 16 bits
	FMT_S32,         ///< signed 32 bits
	FMT_FLT,         ///< float
	//FMT_S64,         ///< signed 64 bits,ffmpeg not support
	FMT_DBL,         ///< double

	FMT_U8P,         ///< unsigned 8 bits, planar
	FMT_S16P,        ///< signed 16 bits, planar
	FMT_S32P,        ///< signed 32 bits, planar
	FMT_FLTP,        ///< float, planar
	//FMT_S64P,         ///< signed 64 bits
	FMT_DBLP,        ///< double, planar
	FMT_RAW,         ///< RawData for Spdif/Hdmi
	FMT_CNT,
	FMT_DEFAULT = FMT_S16,

	HW_FMT_NONE = FMT_CNT + 1,
	HW_FMT_16, ///<  big endian
	HW_FMT_32, ///<  big endian
	HW_FMT_24, ///<  big endian
	HW_FMT_RAW, ///< useless on 8388
	HW_FMT_CNT, ///< useless on 8388
	HW_FMT_24_LITTLE, ///< little endian
	HW_FMT_DEFAULT = HW_FMT_24,


} multi_audio_fmt;

enum  // if modify this ,please modeif ClientFormat.java at the same time
{

	TAG_OUT     = 0x20000000,                      //:
	OUT_NONE    = 0x00000000,                      //:
	OUT_PCM0    = 0x00000001,      //PCM0
	OUT_PCM1    = 0x00000002,      //PCM1
	OUT_PCM2    = 0x00000004,      //PCM2
	OUT_PCM3    = 0x00000008,      //PCM3
	OUT_PCM4    = 0x00000010,      //PCM4
	OUT_IEC0    = 0x00000020,    //Spdif
	OUT_IEC1    = 0x00000040,    //Hdmi
	OUT_RISC    = 0x00000080,      //button?
	OUT_HFP     = 0x00000100,     //HFP out
	OUT_A2DP    = 0x00000200,   //A2DP out
	OUT_DEFAULT = 0x00000400,
	OUT_MAX     = 0x00000800,

	OUT_FRONT   = OUT_PCM0,     //: Front = Fifo[0]
	OUT_CENTER  = OUT_PCM1,       //: Center = Fifo[1]
	OUT_WIDE    = OUT_PCM2,       //: Wide = FIfo[2]
	OUT_SIDE    = OUT_PCM2,       //: Side = FIfo[2]
	OUT_BACK    = OUT_PCM3,          //:Back = Fifo[3]
	OUT_DOWNMIX = OUT_PCM4,          //:Back = Fifo[3]
	OUT_SPDIF   = OUT_IEC0,    //: Spdif = Fifo[IEC0]
	OUT_HDMI    = OUT_IEC1,     //: Hdmi = Fifo[IEC1]

	OUT_4POINT0             = OUT_FRONT | OUT_CENTER,
	OUT_QUAD                = OUT_FRONT | OUT_BACK,
	OUT_2_2                 = OUT_FRONT | OUT_SIDE,
	OUT_5POINT1             = OUT_FRONT | OUT_CENTER | OUT_SIDE,
	OUT_5POINT1_BACK        = OUT_FRONT | OUT_CENTER | OUT_BACK,
	OUT_7POINT1             = OUT_FRONT | OUT_CENTER | OUT_SIDE | OUT_BACK,
	OUT_7POINT1_WIDE        = OUT_FRONT | OUT_CENTER | OUT_SIDE | OUT_BACK,
	OUT_7POINT1_WIDE_BACK   = OUT_FRONT | OUT_CENTER | OUT_WIDE | OUT_BACK,
	OUT_ALL                 = OUT_PCM0 | OUT_PCM1 | OUT_PCM2 | OUT_PCM3
							  | OUT_PCM4 | OUT_IEC0 | OUT_IEC1 | OUT_RISC
							  | OUT_HFP | OUT_A2DP | OUT_DEFAULT,   //all out

	TAG_OUT_NONE    = TAG_OUT | OUT_NONE,                      //:
	TAG_OUT_PCM0    = TAG_OUT | OUT_PCM0,      //PCM0
	TAG_OUT_PCM1    = TAG_OUT | OUT_PCM1,      //PCM1
	TAG_OUT_PCM2    = TAG_OUT | OUT_PCM2,      //PCM2
	TAG_OUT_PCM3    = TAG_OUT | OUT_PCM3,      //PCM3
	TAG_OUT_PCM4    = TAG_OUT | OUT_PCM4,      //PCM4
	TAG_OUT_IEC0    = TAG_OUT | OUT_IEC0,    //Spdif
	TAG_OUT_IEC1    = TAG_OUT | OUT_IEC1,    //Hdmi
	TAG_OUT_RISC    = TAG_OUT | OUT_RISC,      //button?
	TAG_OUT_HFP     = TAG_OUT | OUT_HFP,     //HFP out
	TAG_OUT_A2DP    = TAG_OUT | OUT_A2DP,   //A2DP out
	TAG_OUT_MAX     = TAG_OUT | OUT_MAX,

	TAG_OUT_FRONT   = TAG_OUT | OUT_FRONT,     //: Front = Fifo[0]
	TAG_OUT_CENTER  = TAG_OUT | OUT_CENTER,       //: Center = Fifo[1]
	TAG_OUT_WIDE    = TAG_OUT | OUT_WIDE,         //: Wide = FIfo[2]
	TAG_OUT_SIDE    = TAG_OUT | OUT_SIDE,         //: Side = FIfo[2]
	TAG_OUT_BACK    = TAG_OUT | OUT_BACK,          //:Back = Fifo[3]
	TAG_OUT_DOWNMIX = TAG_OUT | OUT_DOWNMIX,          //:Back = Fifo[3]
	TAG_OUT_SPDIF   = TAG_OUT | OUT_SPDIF,    //: Spdif = Fifo[IEC0]
	TAG_OUT_HDMI    = TAG_OUT | OUT_HDMI,     //: Hdmi = Fifo[IEC1]
	TAG_OUT_DEFAULT = TAG_OUT | OUT_DEFAULT,

	TAG_OUT_4POINT0             = TAG_OUT | OUT_4POINT0,
	TAG_OUT_QUAD                = TAG_OUT | OUT_QUAD,
	TAG_OUT_2_2                 = TAG_OUT | OUT_2_2,
	TAG_OUT_5POINT1             = TAG_OUT | OUT_5POINT1,
	TAG_OUT_5POINT1_BACK        = TAG_OUT | OUT_5POINT1_BACK,
	TAG_OUT_7POINT1             = TAG_OUT | OUT_7POINT1,
	TAG_OUT_7POINT1_WIDE        = TAG_OUT | OUT_7POINT1_WIDE,
	TAG_OUT_7POINT1_WIDE_BACK   = TAG_OUT | OUT_7POINT1_WIDE_BACK,
	TAG_OUT_ALL                 = TAG_OUT | OUT_ALL,    //all out

	TAG_IN      = 0x40000000,
	IN_NONE     = 0x00000000,
	IN_ECHO     = 0x00000001,      //used by Mic
	IN_MIC      = 0x00000002,       //Mic
	IN_IEC2rx   = 0x00000004,    //=11
	IN_IEC0rx   = 0x00000008,    //=12
	IN_IEC1rx   = 0x00000010,    //=13
	IN_HFP      = 0x00000020,     //HFP in
	IN_i2s0rx   = 0x00000040,    //8288
	IN_i2s1rx   = 0x00000080,    //8288
	IN_i2s2rx   = 0x00000100,    //8288
	IN_A2DP     = 0x00000200,   //A2DP in
	IN_LINEIN   = 0x00000400, //ADC0/ADC1
	IN_MUXIN1   = 0x00000800,
	IN_MUXIN2   = 0x00001000,
	IN_MUXIN3   = 0x00002000,
	IN_MUXIN4   = 0x00004000,
	IN_EDR      = 0x00008000,    //ADC1:EDR
	IN_DEFAULT  = 0x00010000,
	IN_MAX      = 0x00020000,

	IN_MUXIN0   = IN_LINEIN,
	IN_FM       = IN_MUXIN0, //ADC0:FM
	IN_DTV      = IN_MUXIN1,    //ADC0:DTV
	IN_BT       = IN_MUXIN2,    //ADC0:BT
	IN_AUXIN    = IN_MUXIN3,    //ADC0:AUXIN
	IN_GPS      = IN_MUXIN4,    //ADC0:GPS
	IN_ALL      = IN_ECHO | IN_MIC | IN_IEC2rx | IN_IEC0rx | IN_IEC1rx
				  | IN_HFP | IN_i2s0rx | IN_i2s1rx | IN_i2s2rx | IN_A2DP
				  | IN_LINEIN | IN_MUXIN1 | IN_MUXIN2 | IN_MUXIN3
				  | IN_MUXIN4 | IN_EDR | IN_DEFAULT,

	TAG_IN_NONE     = TAG_IN | IN_NONE,
	TAG_IN_ECHO     = TAG_IN | IN_ECHO,      //used by Mic
	TAG_IN_MIC      = TAG_IN | IN_MIC,       //Mic
	TAG_IN_IEC2rx   = TAG_IN | IN_IEC2rx,    //=11
	TAG_IN_IEC0rx   = TAG_IN | IN_IEC0rx,    //=12
	TAG_IN_IEC1rx   = TAG_IN | IN_IEC1rx,    //=13
	TAG_IN_HFP      = TAG_IN | IN_HFP,     //HFP in
	TAG_IN_i2s0rx   = TAG_IN | IN_i2s0rx,    //8288
	TAG_IN_i2s1rx   = TAG_IN | IN_i2s1rx,    //8288
	TAG_IN_i2s2rx   = TAG_IN | IN_i2s2rx,    //8288
	TAG_IN_A2DP     = TAG_IN | IN_A2DP,  //A2DP in
	TAG_IN_LINEIN   = TAG_IN | IN_LINEIN, //ADC0/ADC1
	TAG_IN_MUXIN1   = TAG_IN | IN_MUXIN1,
	TAG_IN_MUXIN2   = TAG_IN | IN_MUXIN2,
	TAG_IN_MUXIN3   = TAG_IN | IN_MUXIN3,
	TAG_IN_MUXIN4   = TAG_IN | IN_MUXIN4,
	TAG_IN_EDR      = TAG_IN | IN_EDR,    //ADC1:EDR
	TAG_IN_MAX      = TAG_IN | IN_MAX,

	TAG_IN_DEFAULT  = TAG_IN | IN_DEFAULT,
	TAG_IN_MUXIN0   = TAG_IN | IN_MUXIN0,
	TAG_IN_FM       = TAG_IN | IN_FM, //ADC0:FM
	TAG_IN_DTV      = TAG_IN | IN_DTV,    //ADC0:DTV
	TAG_IN_BT       = TAG_IN | IN_BT,    //ADC0:BT
	TAG_IN_AUXIN    = TAG_IN | IN_AUXIN,    //ADC0:AUXIN
	TAG_IN_GPS      = TAG_IN | IN_GPS,    //ADC0:GPS
	TAG_IN_ALL      = TAG_IN | IN_ALL,

	TAG_TYPE        = 0x80000000,
	TAG_TYPE_TRACK_MUSIC, /**< for dsp stream flow*/
	TAG_TYPE_TRACK_EFFECT, /**< useless*/
	TAG_TYPE_TRACK_GPS, /**< useless*/
	TAG_TYPE_TRACK_HF /**< useless*/
};

enum
{
	SR_NONE = -1,
	SR_8000     = 8000,
	SR_16000        = 16000,
	SR_32000        = 32000,
	SR_64000        = 64000,
	SR_128000   = 128000,

	SR_11025        = 11025,
	SR_22050        = 22050,
	SR_44100        = 44100,
	SR_88200        = 88200 ,
	SR_176400   = 176400,

	SR_6000        = 6000,
	SR_12000        = 12000,
	SR_24000        = 24000,
	SR_48000        = 48000,
	SR_96000        = 96000,
	SR_192000   = 192000,

	SR_DEFAULT = 48000
};

typedef enum audio_state_e
{
	NO_INIT, /**< init track/record/voice */
	IDLE, /**<  track/record/voice have not data need  to process */
	STOPPING, /** @todo track stop is need this now, should be remove later */
	ACTIVE, /**<  track/record/voice have data need  to process*/
} audio_state_t;


#define SECOND_MS (1000)
#define DEFAULT_BUFFER_COUNT (24)
#define MIN_PCM_MS_PER_LOOP (8)

#define USED_FIFO_OUT_NUM (3)

typedef unsigned int        uint32_t;
typedef unsigned char       uint8_t;
typedef unsigned short      uint16_t;
typedef long long           int64_t;

typedef unsigned char*      pBuf;

typedef struct
{
	pBuf buf; /**< start address of buffer*/
	unsigned int size; /**<  size of buffer*/
} audio_cbuffer_t;
typedef audio_cbuffer_t*            pBuffer;

typedef enum
{
	WAIT_FREE_SIZE,
	WAIT_NOT_FULL,
	WAIT_USED_SIZE = WAIT_FREE_SIZE,
	WAIT_NOT_EMPTY = WAIT_NOT_FULL,
	WAIT_NOTHING
} waitType;

#endif //AUDIO_TYPES_H
