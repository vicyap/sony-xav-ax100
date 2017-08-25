/**********************************************************************
ModuleName:        QMagicMetaDataProvider

Description:	QMetaDatReaderControl Function Implementations

Copyright:	Copyright @	2015 Magicplus, Inc.

History:		hh.li@magicmedia.com.cn	 11/19/2015	Creation

**********************************************************************/
#include <QImage>
#include <QDebug>
#include <QThread>

#include <QMediaMetaData>
#include "qmagicmetadataprovider.h"
#include "qmediastringdefine.h"
#include "autodetect.h"
#include "charsetdet.h"
#include <QTextCodec>
#include <QTime>

#define TAG "/MPProvider "


QT_BEGIN_NAMESPACE


QMagicMetaDataProvider::QMagicMetaDataProvider(QMagicPlayerSession *session, QObject *parent)
    :QMetaDataReaderControl(parent), m_session(session)
{
	
	//connect(m_session, SIGNAL(metadataChanged(QMagicPlayerSession::PlayerMetaData)), SLOT(updateData(QMagicPlayerSession::PlayerMetaData)),Qt::QueuedConnection);
	connect(m_session, SIGNAL(metadataChanged(QVariantMap)), SLOT(updateData(QVariantMap)));
	connect(m_session, SIGNAL(metaDataChanged(const QString&,const QVariant&)), SIGNAL(metaDataChanged(const QString&,const QVariant&)));
}

QMagicMetaDataProvider::~QMagicMetaDataProvider()
{
}

bool QMagicMetaDataProvider::isMetaDataAvailable() const
{
    return !m_metadata.isEmpty();
}

bool QMagicMetaDataProvider::isWritable() const
{
    return false;
}

QVariant QMagicMetaDataProvider::metaData(const QString &key) const
{
	if((key == QMediaMetaData::ThumbnailImage)
		&&(m_metadata.value(key).toBool()==true))
	{
		return m_session->getID3Pic();
	}else if(key == QMediaMetaData::AudioStream)
	{
		return m_session->getCurrentAudioStream();
	}
    return m_metadata.value(key);
}

QStringList QMagicMetaDataProvider::availableMetaData() const
{
    return m_metadata.keys();
}



void QMagicMetaDataProvider::updateData(QVariantMap metadata)
{
	MP_DEBUG(TAG,"updateData");
	m_metadata =  metadata;
    emit metaDataChanged();
}

QT_END_NAMESPACE
