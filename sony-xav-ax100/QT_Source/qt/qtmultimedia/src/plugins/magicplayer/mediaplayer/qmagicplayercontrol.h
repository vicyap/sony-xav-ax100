/**********************************************************************
ModuleName:        QMagicPlayerControl

Description:	QMediaPlayerControl Function Implementations

Copyright:	Copyright @	2015 Magicplus, Inc.

History:		hh.li@magicmedia.com.cn	 11/19/2015	Creation

**********************************************************************/

#ifndef QMAGICPLAYERCONTROL_H
#define QMAGICPLAYERCONTROL_H

#include <QTimer>
#include <qmediaplayercontrol.h>
#include "qmagicplayerserviceplugin.h"
#include <qmediaplayer.h>
#include "qmagicplayersession.h"

QT_BEGIN_NAMESPACE

class QMediaPlayerResourceSetInterface;
class QMagicPlayerSession;

class QMagicPlayerControl : public QMediaPlayerControl
{
    Q_OBJECT

public:
    QMagicPlayerControl(QMagicPlayerSession *session, QObject *parent = 0);
    ~QMagicPlayerControl();

    QMediaPlayer::State state() const;
    QMediaPlayer::MediaStatus mediaStatus() const;

    qint64 position() const;
    qint64 duration() const;

    int bufferStatus() const;

    int volume() const;
    bool isMuted() const;

    bool isAudioAvailable() const;
    bool isVideoAvailable() const;
    void setVideoOutput(QObject *output);
	
	QMediaTimeRange availablePlaybackRanges() const;
    bool isSeekable() const;

    qreal playbackRate() const;
    void setPlaybackRate(qreal rate);

    QMediaContent media() const;
    const QIODevice *mediaStream() const;
    void setMedia(const QMediaContent&, QIODevice *);

    QMediaPlayerResourceSetInterface* resources() const;

public Q_SLOTS:
    void setPosition(qint64 pos);

    void play();
    void pause();
    void stop();

    void setVolume(int volume);
    void setMuted(bool muted);
	#ifdef SUPPORT_SUNPLUS_EXTEND
	void invoke(QMediaPlayer::MediaInvoke invokeid,QVariant invokeInfo);
	#endif

signals:
	void playSessionMessage(QMagicPlayerSession::MessageId msgid,QVariant param0,QVariant param1);

private Q_SLOTS:
    void updatePlayerState(QMagicPlayerSession::PlayerState state);
	void handleError(int errorid, const QString &errorString);
	void handlePlayBackDone();
    void updateBufferProgress(int progress);
	void timeUpdate();
private:
    QMagicPlayerSession *m_session;
	QMagicPlayerSession::PlayerState mPlayerState;
    QMediaPlayer::State mCurrentState;;
    QMediaPlayer::MediaStatus mMediaStatus;
    int mBufferProgress;
    QMediaContent m_currentResource;
	QTimer *mTimer;

	void setMediaStatus(QMediaPlayer::MediaStatus status);
	void setState(QMediaPlayer::State state);
};

QT_END_NAMESPACE

#endif
