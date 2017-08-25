/**********************************************************************
ModuleName:        QMagicPlayerService

Description:	QMediaService Function Implementations

Copyright:	Copyright @	2015 Magicplus, Inc.

History:		hh.li@magicmedia.com.cn	 11/19/2015	Creation

**********************************************************************/

#ifndef QMAGICPLAYERSERVICE_H
#define QMAGICPLAYERSERVICE_H

#include <QObject>
#include <QThread>

#include <qvideowindowcontrol.h>
#include <qmediaservice.h>
#include "qmagicvideowindow.h"
#include "qmagicplayersession.h"
#include "qmagicplayercontrol.h"
#include "qmagicmetadataprovider.h"

QT_BEGIN_NAMESPACE
class QMagicPlayerControl;
class QMagicWindowSession;
class QMagicPlayerSession;
class QMagicMetaDataProvider;

class QMagicPlayerService : public QMediaService
{
    Q_OBJECT
public:
    QMagicPlayerService(QMediaPlayer::Flags flag,QObject *parent = 0);
    ~QMagicPlayerService();

    QMediaControl *requestControl(const char *name);
    void releaseControl(QMediaControl *control);

private:
	QThread mChildThread;
    QMagicPlayerControl *m_control;
    QMagicPlayerSession *m_session;
	//QMagicWindowSession *m_winSession;
    QMagicMetaDataProvider *m_metaData;

    //QMediaControl *m_videoOutput;
    //QMediaControl *m_videoRenderer;
    //QMediaControl *m_videoWindow;
    //QMediaControl *m_videoWidget;
};

QT_END_NAMESPACE

#endif
