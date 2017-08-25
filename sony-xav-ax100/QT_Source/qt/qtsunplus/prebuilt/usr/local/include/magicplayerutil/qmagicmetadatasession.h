/**********************************************************************
ModuleName:        QMagicMetadataSession

Description:	convert pe datastuct to magic metadata define

Copyright:	Copyright @	2015 Magicplus, Inc.

History:		hh.li@magicmedia.com.cn	 2/16/2016	Creation

**********************************************************************/

#ifndef QMAGICMETADATASESSION_H
#define QMAGICMETADATASESSION_H

#include <QObject>
#include <QtGlobal>
#include <QImage>

#include "qmagicplayersession.h"
#include "types.h"
#include "pe/pe_respondmessage.h"
#include "platform/file_def.h"
#include "SPFilePlayer.h"

using namespace spfileplayer;

QT_BEGIN_NAMESPACE
class QMagicMetaDataSession : public QObject
{
Q_OBJECT
public:
	QMagicMetaDataSession(SPFilePlayer* filePlayer,PE_Handle peHandle,QObject *parent=0);
    ~QMagicMetaDataSession();

	void updateAVMetadata(QMagicPlayerSession::PlayerMetaData& metadata);
	void updateAudioMetadata(PE_Handle peHandle,QMagicPlayerSession::PlayerMetaData& metadata);
	void updateVideoMetadata(PE_Handle peHandle,QMagicPlayerSession::PlayerMetaData& metadata);
	void updateAVCodecId(PE_Handle peHandle,QMagicPlayerSession::PlayerMetaData& metadata);
	void updateTrickPlayFlag(PE_Handle peHandle,QMagicPlayerSession::PlayerMetaData& metadata);
	void updateTotalTime(PE_Handle peHandle,QMagicPlayerSession::PlayerMetaData& metadata);
	void updateCurrentAudioStream(PE_Handle peHandle,QMagicPlayerSession::PlayerMetaData& metadata);	
	QImage updateID3Pic(QString path,quint32 picsize,quint32 picOffset);
	QImage updateID3Pic(PE_Handle peHandle,quint32 picsize);
	void fillMetadataMap(QVariantMap &map,QMagicPlayerSession::PlayerMetaData metadata);
	QString fromCharToQstring(QMagicPlayerSession::StringCodecType type,const char *str);
private:
	PE_Handle mPeHandle;
	SPFilePlayer* mFilePlayer;
};

QT_END_NAMESPACE
#endif//QMAGICMETADATASESSION_H