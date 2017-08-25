/**********************************************************************
ModuleName:        QMagicPlayerServicePlugin

Description:	QMediaServiceProviderPlugin Function Implementations

Copyright:	Copyright @	2015 Magicplus, Inc.

History:		hh.li@magicmedia.com.cn	 11/19/2015	Creation

**********************************************************************/
#include <QDebug>
#include "qmagicplayerserviceplugin.h"

#define TAG "/MPPlugin "

#ifdef VERSION_NUM
    #define MP_VERSION_NUM(s) "MagicPlayer Version Number: " s
    #define MP_VERSION_INFO(s) "MagicPlayer Version Info: " s
	
	#pragma message(MP_VERSION_NUM(VERSION_NUM))
	#pragma message(MP_VERSION_INFO(VERSION_INFO))
#endif

QMediaService* QMagicPlayerServicePlugin::create(const QString &key)
{
#ifdef VERSION_NUM
	MP_DEBUG("","MagicPlayer version num:%s",VERSION_NUM);
	MP_DEBUG("","MagicPlayer version info:%s",VERSION_INFO);
#endif
	MP_TRACE(TAG);
	MP_DEBUG(TAG,"creat key:%s",qPrintable(key));

	QMediaPlayer::Flags flag = 0;
    if (key == (QLatin1String(Q_MEDIASERVICE_MEDIAPLAYER)+QLatin1String("QMediaServiceProviderHint::OnlyMetaData")))
    {
    	flag = QMediaPlayer::OnlyMetaData;
		return new QMagicPlayerService(flag);
    }else if(key == (QLatin1String(Q_MEDIASERVICE_MEDIAPLAYER)+QLatin1String("QMediaServiceProviderHint::VideoThumbnail")))
    {
    	flag = QMediaPlayer::VideoThumbnail;
		return new QMagicPlayerService(flag);
    }
	
    if (key == QLatin1String(Q_MEDIASERVICE_MEDIAPLAYER))
        return new QMagicPlayerService(flag);
	
	MP_ERROR(TAG,"MagicPlayer service plugin: unsupported key:%s",qPrintable(key));
    return NULL;
}

void QMagicPlayerServicePlugin::release(QMediaService *service)
{
     delete service;
}

QMediaServiceProviderHint::Features QMagicPlayerServicePlugin::supportedFeatures(
        const QByteArray &service) const
{
    return QMediaServiceProviderHint::Features();
}

QMultimedia::SupportEstimate QMagicPlayerServicePlugin::hasSupport(const QString &mimeType,
                                                                     const QStringList &codecs) const
{
    return QMultimedia::MaybeSupported;
}

void QMagicPlayerServicePlugin::updateSupportedMimeTypes()
{
	mSupportTypes.clear();
	mSupportTypes.append("video/avi");
	mSupportTypes.append("video/x-matroska");
	mSupportTypes.append("video/mpeg");
	mSupportTypes.append("video/quicktime");
	mSupportTypes.append("video/mp4");
	mSupportTypes.append("video/3gpp");
	mSupportTypes.append("video/x-ms-asf");
	mSupportTypes.append("video/x-ms-wmv");
	mSupportTypes.append("video/m2ts");
	mSupportTypes.append("video/flv");
	mSupportTypes.append("video/rmvb");
	mSupportTypes.append("video/x-ms-vob");
	mSupportTypes.append("audio/x-pn-realaudio");
	mSupportTypes.append("audio/mp3");
	mSupportTypes.append("audio/ac3");
	mSupportTypes.append("audio/dts");
	mSupportTypes.append("audio/x-ms-wma");
	mSupportTypes.append("audio/pcm");
	mSupportTypes.append("audio/adpcm");
	mSupportTypes.append("audio/x-aac");
	mSupportTypes.append("audio/ogg");
    mSupportTypes.append("audio/cook");	
}

QStringList QMagicPlayerServicePlugin::supportedMimeTypes() const
{
    return mSupportTypes;
}

