/**********************************************************************
ModuleName:        QMagicMetaDataProvider

Description:	QMetaDatReaderControl Function Implementations

Copyright:	Copyright @	2015 Magicplus, Inc.

History:		hh.li@magicmedia.com.cn	 11/19/2015	Creation

**********************************************************************/

#ifndef QMAGICMETADATAPROVIDER_H
#define QMAGICMETADATAPROVIDER_H

#include <qmetadatareadercontrol.h>
#include "qmagicplayerserviceplugin.h"
#include "qmagicplayersession.h"

QT_BEGIN_NAMESPACE

class QMagicPlayerSession;

class QMagicMetaDataProvider : public QMetaDataReaderControl
{
    Q_OBJECT
public:
    QMagicMetaDataProvider(QMagicPlayerSession *session, QObject *parent = 0);
    virtual ~QMagicMetaDataProvider();

    bool isMetaDataAvailable() const;
    bool isWritable() const;

    QVariant metaData(const QString &key) const;
    QStringList availableMetaData() const;
private slots:
    void updateData(QVariantMap metadata);

private:
    QVariantMap m_metadata;
   QMagicPlayerSession *m_session;

};

QT_END_NAMESPACE

#endif // QMAGICMETADATAPROVIDER_H
