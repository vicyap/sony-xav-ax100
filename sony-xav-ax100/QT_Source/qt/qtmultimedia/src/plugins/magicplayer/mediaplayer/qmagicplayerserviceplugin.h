/**********************************************************************
ModuleName:        QMagicPlayerServicePlugin

Description:	QMediaServiceProviderPlugin Function Implementations

Copyright:	Copyright @	2015 Magicplus, Inc.

History:		hh.li@magicmedia.com.cn	 11/19/2015	Creation

**********************************************************************/


#ifndef QMAGICPLAYERSERVICEPLUGIN_H
#define QMAGICPLAYERSERVICEPLUGIN_H

#include "qmagicplayerservice.h"
#include <qmediaserviceproviderplugin.h>
#include <QStringList>

#define MPLIB_TRACE_MSG
#define MPLIB_ERROR_MSG
#define MPLIB_DEBUG_MSG

#ifdef MP_TRACE_MSG
#define MP_TRACE(TAG) qDebug("V"TAG"%s(%d) called",__FUNCTION__,__LINE__)
#else
#define MP_TRACE(TAG) do{}while(0)
#endif

#ifdef MP_ERROR_MSG
#define MP_ERROR(TAG,fmt,arg...) qDebug("E"TAG"%s(%d):"fmt,__FUNCTION__,__LINE__,##arg)
#else
#define MP_ERROR(TAG,fmt,arg...) do{}while(0)
#endif

#ifdef MPLIB_DEBUG_MSG
#define MP_DEBUG(TAG,fmt,arg...) qDebug("D"TAG"%s(%d):"fmt,__FUNCTION__,__LINE__,##arg)
#else
#define MP_DEBUG(TAG,fmt,arg...) do{}while(0)
#endif

QT_BEGIN_NAMESPACE


class QMagicPlayerServicePlugin
    : public QMediaServiceProviderPlugin
    , public QMediaServiceFeaturesInterface
    , public QMediaServiceSupportedFormatsInterface
{
    Q_OBJECT
    Q_INTERFACES(QMediaServiceFeaturesInterface)
    Q_INTERFACES(QMediaServiceSupportedFormatsInterface)
    Q_PLUGIN_METADATA(IID "org.qt-project.qt.mediaserviceproviderfactory/5.0" FILE "mediaplayer.json")
public:
    QMediaService* create(QString const& key);
    void release(QMediaService *service);

    QMediaServiceProviderHint::Features supportedFeatures(const QByteArray &service) const;

    QMultimedia::SupportEstimate hasSupport(const QString &mimeType, const QStringList& codecs) const;
    QStringList supportedMimeTypes() const;

private:
    void updateSupportedMimeTypes();

    QStringList mSupportTypes; 
};

QT_END_NAMESPACE

#endif // QMAGICPLAYERSERVICEPLUGIN_H

