/**********************************************************************
ModuleName:        QMagicMetaDataProvider

Description:	QMetaDatReaderControl Function Implementations

Copyright:	Copyright @	2015 Magicplus, Inc.

History:		hh.li@magicmedia.com.cn	 11/19/2015	Creation

**********************************************************************/
#include <QDebug>
#include "qmagicplayercontrol.h"

#define TAG "/MPControl "
QT_BEGIN_NAMESPACE

QMagicPlayerControl::QMagicPlayerControl(QMagicPlayerSession *session, QObject *parent)
    : QMediaPlayerControl(parent)
    , m_session(session)
    , mCurrentState(QMediaPlayer::StoppedState)
    , mMediaStatus(QMediaPlayer::NoMedia)
    , mBufferProgress(-1)
{


	connect(m_session, SIGNAL(positionChanged(qint64)),
            this, SIGNAL(positionChanged(qint64)));
    connect(m_session, SIGNAL(durationChanged(qint64)),
            this, SIGNAL(durationChanged(qint64)));
    connect(m_session, SIGNAL(mutedStateChanged(bool)),
            this, SIGNAL(mutedChanged(bool)));
    connect(m_session, SIGNAL(volumeChanged(int)),
            this, SIGNAL(volumeChanged(int)));
    connect(m_session, SIGNAL(audioAvailableChanged(bool)),
            this, SIGNAL(audioAvailableChanged(bool)));
    connect(m_session, SIGNAL(videoAvailableChanged(bool)),
            this, SIGNAL(videoAvailableChanged(bool)));
    connect(m_session, SIGNAL(seekableChanged(bool)),
            this, SIGNAL(seekableChanged(bool)));
    connect(m_session, SIGNAL(error(int,QString)),
            this, SLOT(handleError(int,QString)));
    connect(m_session, SIGNAL(playbackRateChanged(qreal)),
            this, SIGNAL(playbackRateChanged(qreal)));;
    connect(m_session,SIGNAL(bufferingProgressChanged(int)),
            this, SLOT(updateBufferProgress(int)));
	connect(m_session,SIGNAL(stateChanged(QMagicPlayerSession::PlayerState)),
            this, SLOT(updatePlayerState(QMagicPlayerSession::PlayerState)));
	connect(m_session, SIGNAL(mediaMessageChanged(QMediaPlayer::MediaMessage,QVariant)),
			this,SIGNAL(mediaMessageChanged(QMediaPlayer::MediaMessage,QVariant)));
	connect(m_session,SIGNAL(playbackComplete()),
            this, SLOT(handlePlayBackDone()));
	
   	//creat fimer for get position
	mTimer= new QTimer(this);
	connect(mTimer, SIGNAL(timeout()), this, SLOT(timeUpdate()));
	mTimer->start(500);


}

QMagicPlayerControl::~QMagicPlayerControl()
{
   MP_TRACE(TAG);
   mTimer->stop();
}

QMediaPlayerResourceSetInterface* QMagicPlayerControl::resources() const
{
    return 0;
}

qint64 QMagicPlayerControl::position() const
{
    return m_session->position();
}

qint64 QMagicPlayerControl::duration() const
{
    return m_session->duration();
}

QMediaPlayer::State QMagicPlayerControl::state() const
{
    return mCurrentState;
}

QMediaPlayer::MediaStatus QMagicPlayerControl::mediaStatus() const
{
	return mMediaStatus;
}

int QMagicPlayerControl::bufferStatus() const
{
    return mBufferProgress;
}

int QMagicPlayerControl::volume() const
{
     return m_session->volume();
}

bool QMagicPlayerControl::isMuted() const
{
    return m_session->isMuted();
}

bool QMagicPlayerControl::isSeekable() const
{
    return m_session->isSeekable();
}

qreal QMagicPlayerControl::playbackRate() const
{
    return m_session->playbackRate();
}

void QMagicPlayerControl::setPlaybackRate(qreal rate)
{
	m_session->manageActList(QMagicPlayerSession::Msg_PlayBackRate,rate);
	
}

void QMagicPlayerControl::setPosition(qint64 pos)
{
	MP_DEBUG(TAG,"play control set position %d",pos);
	m_session->manageActList(QMagicPlayerSession::Msg_Position,pos);
	
}

void QMagicPlayerControl::play()
{	
	m_session->manageActList(QMagicPlayerSession::Msg_Play,0);
    setState(QMediaPlayer::PlayingState);
}

void QMagicPlayerControl::pause()
{
	m_session->manageActList(QMagicPlayerSession::Msg_Pause,0);
	setState(QMediaPlayer::PausedState);
}

void QMagicPlayerControl::stop()
{
	m_session->stop();
	setState(QMediaPlayer::StoppedState);
}

void QMagicPlayerControl::setVolume(int volume)
{
	
	m_session->manageActList(QMagicPlayerSession::Msg_Volume,volume);
}

void QMagicPlayerControl::setMuted(bool muted)
{
  
	m_session->manageActList(QMagicPlayerSession::Msg_Muted,muted);
}

QMediaContent QMagicPlayerControl::media() const
{
   return m_currentResource;
}

const QIODevice *QMagicPlayerControl::mediaStream() const
{
 
}

void QMagicPlayerControl::setMedia(const QMediaContent &content, QIODevice *stream)
{
	QMediaContent oldMedia = m_currentResource;
	m_currentResource = content;

	QString request;

    if (!content.isNull()) {
		request = content.canonicalUrl().path();
	}else
	{
		setMediaStatus(QMediaPlayer::NoMedia);
		MP_DEBUG(TAG,"media content is null");
		setState(QMediaPlayer::StoppedState);
		return;
	}
	
	if (!content.canonicalUrl().isEmpty()) {
		setMediaStatus(QMediaPlayer::UnknownMediaStatus);
		m_session->manageActList(QMagicPlayerSession::Msg_SetMedia,request);
	} else {
		setMediaStatus(QMediaPlayer::NoMedia);
		MP_DEBUG(TAG,"media content url is null");
		setState(QMediaPlayer::StoppedState);
		return;
	}

	if (m_currentResource != oldMedia)
		Q_EMIT mediaChanged(m_currentResource);

	Q_EMIT positionChanged(position());
}


void QMagicPlayerControl::setVideoOutput(QObject *output)
{
    m_session->setVideoOutput(output);
}

bool QMagicPlayerControl::isAudioAvailable() const
{
   return m_session->isAudioAvailable();
}

bool QMagicPlayerControl::isVideoAvailable() const
{
    return m_session->isVideoAvailable();
}

QMediaTimeRange QMagicPlayerControl::availablePlaybackRanges() const
{
    return m_session->availablePlaybackRanges();
}

void QMagicPlayerControl::updateBufferProgress(int progress)
{
	if(mBufferProgress != progress)
	{
		mBufferProgress = progress;
    	Q_EMIT bufferStatusChanged(progress);
	}
}

void QMagicPlayerControl::updatePlayerState(QMagicPlayerSession::PlayerState state)
{
   	mPlayerState = state;
    switch (mPlayerState) {
    case QMagicPlayerSession::Player_Idle:
        break;
    case QMagicPlayerSession::Player_Initialized:
        break;
    case QMagicPlayerSession::Player_Preparing:
        setMediaStatus(QMediaPlayer::LoadingMedia);
        break;
    case QMagicPlayerSession::Player_Prepared:
        setMediaStatus(QMediaPlayer::LoadedMedia);
        break;
    case QMagicPlayerSession::Player_Pause:
		MP_DEBUG(TAG,"set state pause");
        setState(QMediaPlayer::PausedState);
        break;
	case QMagicPlayerSession::Player_Play:
		MP_DEBUG(TAG,"set state playing");
        setState(QMediaPlayer::PlayingState);
		//setMediaStatus(QMediaPlayer::BufferedMedia);
        break;
    case QMagicPlayerSession::Player_Stop:
		MP_DEBUG(TAG,"set state stop");
        setState(QMediaPlayer::StoppedState);
        setMediaStatus(QMediaPlayer::NoMedia);
        Q_EMIT positionChanged(0);
        break;
	case QMagicPlayerSession::Player_FB:
	case QMagicPlayerSession::Player_FF:
	case QMagicPlayerSession::Player_SB:
	case QMagicPlayerSession::Player_SF:
		MP_DEBUG(TAG,"set state fb/ff/sb/sf");
        setState(QMediaPlayer::PlayingState);
		break;
    default:
        break;
    }

}

void QMagicPlayerControl::timeUpdate()
{
	if(mCurrentState == QMediaPlayer::PlayingState)
	{
		qint64 pos = position();
		Q_EMIT positionChanged(pos);
	}
}

void QMagicPlayerControl::handleError(int errorid, const QString &errorString)
{
	MP_DEBUG(TAG,"handle error %d errorstring %s",errorid,qPrintable(errorString));

	Q_EMIT error(errorid,errorString);
	updatePlayerState(m_session->state());
}

void QMagicPlayerControl::handlePlayBackDone()
{
	qint64 pos = position();
	Q_EMIT positionChanged(pos);
	MP_DEBUG(TAG,"handle play done set state stop %d",pos);
    setMediaStatus(QMediaPlayer::EndOfMedia);
	setState(QMediaPlayer::StoppedState);
    Q_EMIT positionChanged(0);
}

void QMagicPlayerControl::setState(QMediaPlayer::State state)
{
    if (mCurrentState == state)
    {
    	MP_DEBUG(TAG,"not emit the same state %d",mCurrentState);
        return;
    }

    if (mCurrentState == QMediaPlayer::StoppedState && state == QMediaPlayer::PausedState)
    {
    	 
		 MP_DEBUG(TAG,"not emit cur state %d state %d",mCurrentState,state);
    	 
		 return;
    }

    mCurrentState = state;
	
	MP_DEBUG(TAG,"emit the state %d",mCurrentState);
    Q_EMIT stateChanged(mCurrentState);
}

void QMagicPlayerControl::setMediaStatus(QMediaPlayer::MediaStatus status)
{
    if (mMediaStatus== status)
        return;

    if (status == QMediaPlayer::NoMedia || status == QMediaPlayer::InvalidMedia)
        Q_EMIT durationChanged(0);

    if (status == QMediaPlayer::EndOfMedia)
        Q_EMIT durationChanged(duration());

    mMediaStatus= status;
    Q_EMIT mediaStatusChanged(mMediaStatus);
}

#ifdef SUPPORT_SUNPLUS_EXTEND

void QMagicPlayerControl::invoke(QMediaPlayer::MediaInvoke invokeid,QVariant invokeInfo)
{
	MP_DEBUG(TAG,"invoke id  %d",invokeid);

	switch(invokeid)
	{
		case QMediaPlayer::Invoke_changeAudio:
			{
				int audioNum = invokeInfo.toInt();
				MP_DEBUG(TAG,"change audio num %d",audioNum);
				
				m_session->manageActList(QMagicPlayerSession::Msg_AudioStream,audioNum);
				break;
			}
		case QMediaPlayer::Invoke_changeSubtitle:
			{
				int subtitleNum = invokeInfo.toInt();
				
				m_session->manageActList(QMagicPlayerSession::Msg_SubtitleNum,subtitleNum);
				break;
			}
		case QMediaPlayer::Invoke_enableSubtitle:
			{
				bool onoff = invokeInfo.toBool();
				
				m_session->manageActList(QMagicPlayerSession::Msg_SubtitleOnOff,onoff);
				break;
			}
		case QMediaPlayer::Invoke_setResumeData:
			{
				qDebug()<<"control setresumedata";
				m_session->manageActList(QMagicPlayerSession::Msg_setResumeData,invokeInfo);
				break;
			}
		case QMediaPlayer::Invoke_audCodecSupList:
			{
				
				m_session->manageActList(QMagicPlayerSession::Msg_AudioCodecList,invokeInfo);
				break;
			}
		case QMediaPlayer::Invoke_vidCodecSupList:
			{
				
				m_session->manageActList(QMagicPlayerSession::Msg_VideoCodecList,invokeInfo);
				break;
			}
		case QMediaPlayer::Invoke_containerSupList:
			{
				
				m_session->manageActList(QMagicPlayerSession::Msg_ContainerList,invokeInfo);
				break;
			}
		default:
			break;
		}
}
#endif
QT_END_NAMESPACE
