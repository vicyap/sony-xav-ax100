/****************************************************************************
**
** Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia.  For licensing terms and
** conditions see http://qt.digia.com/licensing.  For further information
** use the contact form at http://qt.digia.com/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Digia gives you certain additional
** rights.  These rights are described in the Digia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef QMEDIAPLAYER_H
#define QMEDIAPLAYER_H

#include <QtMultimedia/qmediaobject.h>
#include <QtMultimedia/qmediacontent.h>
#include <QtMultimedia/qmediaenumdebug.h>

#include <QtNetwork/qnetworkconfiguration.h>

#define SUPPORT_SUNPLUS_EXTEND
#ifdef SUPPORT_SUNPLUS_EXTEND
#define RESUMEINFODATA_LEN 4224
#endif

QT_BEGIN_NAMESPACE


class QAbstractVideoSurface;
class QMediaPlaylist;
class QVideoWidget;
class QGraphicsVideoItem;

class QMediaPlayerPrivate;
class Q_MULTIMEDIA_EXPORT QMediaPlayer : public QMediaObject
{
    Q_OBJECT
    Q_PROPERTY(QMediaContent media READ media WRITE setMedia NOTIFY mediaChanged)
    Q_PROPERTY(QMediaContent currentMedia READ currentMedia NOTIFY currentMediaChanged)
    Q_PROPERTY(QMediaPlaylist * playlist READ playlist WRITE setPlaylist)
    Q_PROPERTY(qint64 duration READ duration NOTIFY durationChanged)
    Q_PROPERTY(qint64 position READ position WRITE setPosition NOTIFY positionChanged)
    Q_PROPERTY(int volume READ volume WRITE setVolume NOTIFY volumeChanged)
    Q_PROPERTY(bool muted READ isMuted WRITE setMuted NOTIFY mutedChanged)
    Q_PROPERTY(int bufferStatus READ bufferStatus NOTIFY bufferStatusChanged)
    Q_PROPERTY(bool audioAvailable READ isAudioAvailable NOTIFY audioAvailableChanged)
    Q_PROPERTY(bool videoAvailable READ isVideoAvailable NOTIFY videoAvailableChanged)
    Q_PROPERTY(bool seekable READ isSeekable NOTIFY seekableChanged)
    Q_PROPERTY(qreal playbackRate READ playbackRate WRITE setPlaybackRate NOTIFY playbackRateChanged)
    Q_PROPERTY(State state READ state NOTIFY stateChanged)
    Q_PROPERTY(MediaStatus mediaStatus READ mediaStatus NOTIFY mediaStatusChanged)
    Q_PROPERTY(QString error READ errorString)
    Q_ENUMS(State)
    Q_ENUMS(MediaStatus)
    Q_ENUMS(Error)
    #ifdef SUPPORT_SUNPLUS_EXTEND
	Q_ENUMS(MediaInvoke)
	Q_ENUMS(MediaMessage)
	Q_ENUMS(ContainerType)
	Q_ENUMS(AudioCodecType)
	Q_ENUMS(VideoCodecType)
	#endif

public:
    enum State
    {
        StoppedState,
        PlayingState,
        PausedState
    };

    enum MediaStatus
    {
        UnknownMediaStatus,
        NoMedia,
        LoadingMedia,
        LoadedMedia,
        StalledMedia,
        BufferingMedia,
        BufferedMedia,
        EndOfMedia,
        InvalidMedia
    };

    enum Flag
    {
        LowLatency = 0x01,
        StreamPlayback = 0x02,
        VideoSurface = 0x04,
        OnlyMetaData = 0x08,
        VideoThumbnail = 0x16
    };
 
    Q_DECLARE_FLAGS(Flags, Flag)

    enum Error
    {
        NoError,
        ResourceError,
        FormatError,
        NetworkError,
        AccessDeniedError,
        ServiceMissingError,
        MediaIsPlaylist
    };
	
#ifdef SUPPORT_SUNPLUS_EXTEND
	enum ContainerType{
		//video container
	    FILE_TYPE_UNKNOWN = 0,
	    FILE_TYPE_ASF,
	    FILE_TYPE_AVI,
	    FILE_TYPE_FLV,
	    FILE_TYPE_MKV,
	    FILE_TYPE_MPG,
	    FILE_TYPE_QT,
	    FILE_TYPE_RMF,
	    FILE_TYPE_SWF,
	    FILE_TYPE_TS,
	    FILE_TYPE_TSPL_PSA,
	    FILE_TYPE_TSPL_PSV,
	    FILE_TYPE_TSPL_TSA,
	    FILE_TYPE_TSPL_TSV,

		//audio container
	    FILE_TYPE_AAC,
	    FILE_TYPE_AC3,
	    FILE_TYPE_APE,
	    FILE_TYPE_DTS,
	    FILE_TYPE_FLAC,
	    FILE_TYPE_MP3,
	    FILE_TYPE_OGG,
	    FILE_TYPE_WAVE,
		//disc
	    FILE_TYPE_CDXA,
	    FILE_TYPE_DVD_VIDEO,
	    FILE_TYPE_DVD_PLUS_VR,
	    FILE_TYPE_DVD_MINUS_VR,
	    FILE_TYPE_SVCD,
	    FILE_TYPE_VCD20,
	    FILE_TYPE_VCD11,
	    FILE_TYPE_CDDA,

	    FILE_TYPE_MAX,
	};

	enum VideoCodecType{
	    FILE_VIDEO_CODEC_NONE,
	    FILE_CODEC_UNKNOWN,
	    FILE_VIDEO_CODEC_MPEG1,
	    FILE_VIDEO_CODEC_MPEG2,
	    FILE_VIDEO_CODEC_MPEG4,
	    FILE_VIDEO_CODEC_H264,
	    FILE_VIDEO_CODEC_VC1,
	    FILE_VIDEO_CODEC_MJPEG,
	    FILE_VIDEO_CODEC_RV,
	    FILE_VIDEO_CODEC_MAX,
	};

	enum AudioCodecType{
	    FILE_AUDIO_CODEC_NONE,
	    FILE_AUDIO_CODEC_UNKNOWN,
	    FILE_AUDIO_CODEC_MPEG1_LAYER1,
	    FILE_AUDIO_CODEC_MPEG1_LAYER2,
	    FILE_AUDIO_CODEC_MPEG1_LAYER3,
	    FILE_AUDIO_CODEC_MPEG2,
	    FILE_AUDIO_CODEC_AC3,
	    FILE_AUDIO_CODEC_TRUEHD,
	    FILE_AUDIO_CODEC_PCM,
	    FILE_AUDIO_CODEC_WMA,
	    FILE_AUDIO_CODEC_AAC,
	    FILE_AUDIO_CODEC_MPEG2_AAC,
	    FILE_AUDIO_CODEC_AAC_LATM,
	    FILE_AUDIO_CODEC_DTS,
	    FILE_AUDIO_CODEC_OGG_VORBIS,
	    FILE_AUDIO_CODEC_COOK,
	    FILE_AUDIO_CODEC_FLAC,
	    FILE_AUDIO_CODEC_APE,
	    FILE_AUDIO_CODEC_DTSLBR,
	    FILE_AUDIO_CODEC_LPCM_DTS,
	    FILE_AUDIO_CODEC_ALAC,
	    FILE_AUDIO_CODEC_MAX,
	};

	struct ResumeData
	{	
		quint8 resumeData[RESUMEINFODATA_LEN];
	};
	
	enum MediaInvoke
	{
		Invoke_changeAudio,
		Invoke_changeSubtitle,
		Invoke_enableSubtitle,
		Invoke_setResumeData,
		Invoke_audCodecSupList,
		Invoke_vidCodecSupList,
		Invoke_containerSupList
	};

	enum MediaMessage
    {
        Msg_PrepareProgress,
		Msg_AudioUnsupport,
		Msg_TrickPlayUnsupport,
		Msg_ResumeDataChanged
    };
#endif


    QMediaPlayer(QObject *parent = 0, Flags flags = 0);
    ~QMediaPlayer();

    static QMultimedia::SupportEstimate hasSupport(const QString &mimeType,
                                            const QStringList& codecs = QStringList(),
                                            Flags flags = 0);
    static QStringList supportedMimeTypes(Flags flags = 0);

    void setVideoOutput(QVideoWidget *);
    void setVideoOutput(QGraphicsVideoItem *);
    void setVideoOutput(QAbstractVideoSurface *surface);

    QMediaContent media() const;
    const QIODevice *mediaStream() const;
    QMediaPlaylist *playlist() const;
    QMediaContent currentMedia() const;

    State state() const;
    MediaStatus mediaStatus() const;

    qint64 duration() const;
    qint64 position() const;

    int volume() const;
    bool isMuted() const;
    bool isAudioAvailable() const;
    bool isVideoAvailable() const;

    int bufferStatus() const;

    bool isSeekable() const;
    qreal playbackRate() const;

    Error error() const;
    QString errorString() const;

    QNetworkConfiguration currentNetworkConfiguration() const;

    QMultimedia::AvailabilityStatus availability() const;	

public Q_SLOTS:
    void play();
    void pause();
    void stop();

    void setPosition(qint64 position);
    void setVolume(int volume);
    void setMuted(bool muted);

    void setPlaybackRate(qreal rate);

    void setMedia(const QMediaContent &media, QIODevice *stream = 0);
    void setPlaylist(QMediaPlaylist *playlist);

    void setNetworkConfigurations(const QList<QNetworkConfiguration> &configurations);

#ifdef SUPPORT_SUNPLUS_EXTEND
	void invoke(QMediaPlayer::MediaInvoke invokeid,QVariant invokeInfo);	
#endif

Q_SIGNALS:
    void mediaChanged(const QMediaContent &media);
    void currentMediaChanged(const QMediaContent &media);

    void stateChanged(QMediaPlayer::State newState);
    void mediaStatusChanged(QMediaPlayer::MediaStatus status);

    void durationChanged(qint64 duration);
    void positionChanged(qint64 position);

    void volumeChanged(int volume);
    void mutedChanged(bool muted);
    void audioAvailableChanged(bool available);
    void videoAvailableChanged(bool videoAvailable);

    void bufferStatusChanged(int percentFilled);

    void seekableChanged(bool seekable);
    void playbackRateChanged(qreal rate);

    void error(QMediaPlayer::Error error);

    void networkConfigurationChanged(const QNetworkConfiguration &configuration);

#ifdef SUPPORT_SUNPLUS_EXTEND
	void mediaMessageChanged(QMediaPlayer::MediaMessage messageid,QVariant msgInfo);
#endif

public:
    virtual bool bind(QObject *);
    virtual void unbind(QObject *);

private:
    Q_DISABLE_COPY(QMediaPlayer)
    Q_DECLARE_PRIVATE(QMediaPlayer)
    Q_PRIVATE_SLOT(d_func(), void _q_stateChanged(QMediaPlayer::State))
    Q_PRIVATE_SLOT(d_func(), void _q_mediaStatusChanged(QMediaPlayer::MediaStatus))
    Q_PRIVATE_SLOT(d_func(), void _q_error(int, const QString &))
    Q_PRIVATE_SLOT(d_func(), void _q_updateMedia(const QMediaContent&))
    Q_PRIVATE_SLOT(d_func(), void _q_playlistDestroyed())
    Q_PRIVATE_SLOT(d_func(), void _q_handlePlaylistLoaded())
    Q_PRIVATE_SLOT(d_func(), void _q_handlePlaylistLoadFailed())
};

QT_END_NAMESPACE

Q_DECLARE_METATYPE(QMediaPlayer::State)
Q_DECLARE_METATYPE(QMediaPlayer::MediaStatus)
Q_DECLARE_METATYPE(QMediaPlayer::Error)

Q_MEDIA_ENUM_DEBUG(QMediaPlayer, State)
Q_MEDIA_ENUM_DEBUG(QMediaPlayer, MediaStatus)
Q_MEDIA_ENUM_DEBUG(QMediaPlayer, Error)

#ifdef SUPPORT_SUNPLUS_EXTEND
Q_DECLARE_METATYPE(QMediaPlayer::ContainerType)
Q_DECLARE_METATYPE(QMediaPlayer::AudioCodecType)
Q_DECLARE_METATYPE(QMediaPlayer::VideoCodecType)
Q_DECLARE_METATYPE(QMediaPlayer::ResumeData)
Q_DECLARE_METATYPE(QMediaPlayer::MediaInvoke)
Q_DECLARE_METATYPE(QMediaPlayer::MediaMessage)
Q_MEDIA_ENUM_DEBUG(QMediaPlayer, ContainerType)
Q_MEDIA_ENUM_DEBUG(QMediaPlayer, AudioCodecType)
Q_MEDIA_ENUM_DEBUG(QMediaPlayer, VideoCodecType)
Q_MEDIA_ENUM_DEBUG(QMediaPlayer, MediaInvoke)
Q_MEDIA_ENUM_DEBUG(QMediaPlayer, MediaMessage)
#endif

#endif  // QMEDIAPLAYER_H
