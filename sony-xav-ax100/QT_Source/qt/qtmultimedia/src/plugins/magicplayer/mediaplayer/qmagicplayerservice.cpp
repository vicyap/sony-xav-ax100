/**********************************************************************
ModuleName:        QMagicPlayerService

Description:	QMediaService Function Implementations

Copyright:	Copyright @	2015 Magicplus, Inc.

History:		hh.li@magicmedia.com.cn	 11/19/2015	Creation

**********************************************************************/

#include <QDebug>
#if defined(HAVE_WIDGETS)
#include <QtWidgets/qwidget.h>
#endif
#include "qmagicplayerservice.h"


QT_BEGIN_NAMESPACE

QMagicPlayerService::QMagicPlayerService(QMediaPlayer::Flags features,QObject *parent):
     QMediaService(parent)
{
	//m_winSession = new QMagicWindowSession(0);
	m_session = new QMagicPlayerSession(features,0);
	m_control = new QMagicPlayerControl(m_session, this);
	m_metaData = new QMagicMetaDataProvider(m_session, this);
	//m_videoWindow = new QMagicVideoWindow(m_winSession,this);

	mChildThread.start(QThread::NormalPriority);
	m_session->moveToThread(&mChildThread);
}

QMagicPlayerService::~QMagicPlayerService()
{
	if(mChildThread.isRunning())
	{
		mChildThread.exit();
		while(!mChildThread.isFinished()){
			MP_TRACE(TAG);
			;
		}
	}
	delete m_session;
}

QMediaControl *QMagicPlayerService::requestControl(const char *name)
{
	if (qstrcmp(name,QMediaPlayerControl_iid) == 0)
		return m_control;

	if (qstrcmp(name,QMetaDataReaderControl_iid) == 0)
	{
		return m_metaData;
	}

	return 0;

}

void QMagicPlayerService::releaseControl(QMediaControl *control)
{

}

QT_END_NAMESPACE
