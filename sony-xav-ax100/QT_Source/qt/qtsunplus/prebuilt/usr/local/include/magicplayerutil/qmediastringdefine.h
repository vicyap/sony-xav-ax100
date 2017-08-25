/**********************************************************************
ModuleName:        qmagicstringdefine.h

Description:	define s+  string define

Copyright:	Copyright @	2016 Magicplus, Inc.

History:		hh.li@magicmedia.com.cn	 01/27/2016	Creation

**********************************************************************/
#ifndef QMAGICMETADATADEFINE_H
#define QMAGICMETADATADEFINE_H

#include <QtCore/qobject.h>

class QMediaStringDefine
{
	public:
		enum  VideoFrameRate
		{
			VIDEO_FRAME_RATE_FORBIDDEN = 0,
			VIDEO_FRAME_RATE_23976 	= 1,		/*!< 23.976*/
			VIDEO_FRAME_RATE_24		= 2,
			VIDEO_FRAME_RATE_25		= 3,
			VIDEO_FRAME_RATE_2997		= 4,		/*!< 29.97*/
			VIDEO_FRAME_RATE_30		= 5,
			VIDEO_FRAME_RATE_50		= 6,
			VIDEO_FRAME_RATE_5994		= 7,		/*!< 59.94*/
			VIDEO_FRAME_RATE_60		= 8
		};

		enum VideoCodecID
		{
			VIDEO_CODEC_NONE = 0,
			VIDEO_CODEC_UNKNOWN,
			VIDEO_CODEC_MPEG1,
			VIDEO_CODEC_MPEG2,
			VIDEO_CODEC_MPEG4,
			VIDEO_CODEC_H264,
			VIDEO_CODEC_VC1,
			VIDEO_CODEC_MJPEG,
			VIDEO_CODEC_RV,

		};

		enum AudioCodecID
		{
			AUDIO_CODEC_NONE = 0,
			AUDIO_CODEC_UNKNOWN,
			AUDIO_CODEC_MPEG1_LAYER1,
			AUDIO_CODEC_MPEG1_LAYER2,
			AUDIO_CODEC_MPEG1_LAYER3,
			AUDIO_CODEC_MPEG2,
			AUDIO_CODEC_AC3,
			AUDIO_CODEC_TRUEHD,
			AUDIO_CODEC_PCM,
			AUDIO_CODEC_WMA,
			AUDIO_CODEC_AAC,
			AUDIO_CODEC_MPEG2_AAC,
			AUDIO_CODEC_DTS,
			AUDIO_CODEC_OGG_VORBIS,
			AUDIO_CODEC_COOK,
			AUDIO_CODEC_FLAC,
		} MwAudioCodecID_t;

		enum MediaLanguage
		{
			MEDIA_LANGUAGE_UNKNOWN = -1,
			MEDIA_LANGUAGE_ENGLISH,
			MEDIA_LANGUAGE_SPANISH,
			MEDIA_LANGUAGE_PORTUGESE,
			MEDIA_LANGUAGE_RUSSIAN,
			MEDIA_LANGUAGE_ARABIC,
			MEDIA_LANGUAGE_GREECE,
			MEDIA_LANGUAGE_THAI,
			MEDIA_LANGUAGE_GERMAN,
			MEDIA_LANGUAGE_ITALIAN,
			MEDIA_LANGUAGE_FRENCH,
			MEDIA_LANGUAGE_VIETNAMESE,
			MEDIA_LANGUAGE_PERSIAN,
			MEDIA_LANGUAGE_SIMPLECH,
			MEDIA_LANGUAGE_TRADITIONALCH,
			MEDIA_LANGUAGE_KOREAN,
			MEDIA_LANGUAGE_JIS,
			MEDIA_LANGUAGE_DANISH,
			MEDIA_LANGUAGE_DUTCH,
			MEDIA_LANGUAGE_NORWEGIAN,
		};
		
		QMediaStringDefine();
		~QMediaStringDefine();
		static QString getVideoCodecString(quint32 codecId);
		static QString getAudioCodecString(quint32 codecId);
		static QString getLanguageString(quint32 languageId);
};

#endif
