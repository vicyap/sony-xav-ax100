/**********************************************************************
ModuleName:        QMagicPlayerSession

Description:	connect  to pe

Copyright:	Copyright @	2015 Magicplus, Inc.

History:		hh.li@magicmedia.com.cn	 11/19/2015	Creation

**********************************************************************/

#ifndef QMAGICPLAYERSESSION_H
#define QMAGICPLAYERSESSION_H

#include <QObject>
#include <QtGlobal>
#include <QTimer>
#include <QImage>
#include <QSize>
#include <QList>


#include <qmediaplayercontrol.h>
#include <qmediaplayer.h>

#define ID3_V1     	(1<<0)
#define ID3_V2     	(1<<1)
#define ID3_LYRICS	(1<<2)
#define ID3_PIC     (1<<3)

#define MPLIB_TRACE_MSG
#define MPLIB_ERROR_MSG
#define MPLIB_DEBUG_MSG

#ifdef MPLIB_TRACE_MSG
#define MPLIB_TRACE(TAG) qDebug("V"TAG"%s(%d) called",__FUNCTION__,__LINE__)
#else
#define MPLIB_TRACE(TAG) do{}while(0)
#endif

#ifdef MPLIB_ERROR_MSG
#define MPLIB_ERROR(TAG,fmt,arg...) qDebug("E"TAG"%s(%d):"fmt,__FUNCTION__,__LINE__,##arg)
#else
#define MPLIB_ERROR(TAG,fmt,arg...) do{}while(0)
#endif

#ifdef MPLIB_DEBUG_MSG
#define MPLIB_DEBUG(TAG,fmt,arg...) qDebug("D"TAG"%s(%d):"fmt,__FUNCTION__,__LINE__,##arg)
#else
#define MPLIB_DEBUG(TAG,fmt,arg...) do{}while(0)
#endif

#define SUCCESS 1
#define FAIL 0

#define PE_INVAILD_HANDLE -1
#define MPMAX_SN_LEN 256
#define CHECK_HANDLE(h) (((qint32)h != PE_INVAILD_HANDLE && h != 0)?SUCCESS:FAIL)

#define Q_MPASSERT(x) \
do{\
	if(CHECK_HANDLE(x) == FAIL)\
	{\
		qDebug("please check %s in %s(%d)\n",#x,__FILE__,__LINE__);\
		Q_ASSERT(true);\
	}\
}while(0)

QT_BEGIN_NAMESPACE

class QBusHelper;
class QMagicPlayerSessionPrivate;
class QMagicPlayerSession : public QObject
{
Q_OBJECT
    Q_ENUMS(PlayerState)
    Q_ENUMS(MessageId)
    Q_ENUMS(StringCodecType)
public:	
	enum PlayerState
    {
        Player_Idle,
		Player_Initialized,
		Player_Stop,
        Player_Preparing,
        Player_Prepared,
        Player_Play,
        Player_FF,
        Player_FB,
        Player_SF,
        Player_SB, 
        Player_Pause
    };

	enum MessageId
    {
        Msg_SetMedia,
		Msg_PlayBackRate,
		Msg_Position,
        Msg_Play,
        Msg_Pause,
		Msg_Stop,
        Msg_Volume,
        Msg_Muted,
		Msg_SubtitleNum,
		Msg_AudioStream,
        Msg_ResumePlay,
		Msg_SubtitleOnOff,
		Msg_setResumeData,
		Msg_AudioCodecList,
		Msg_VideoCodecList,
		Msg_ContainerList,
        Msg_Unknown
    };

	enum AudioChannel
	{
		AUDIO_CHN_1CH=0,
		AUDIO_CHN_2CH=1,// 2ch
		AUDIO_CHN_3Ch=2,// 2.1ch
		AUDIO_CHN_4CH=3,//3.1ch
		AUDIO_CHN_5CH=4,//5ch
		AUDIO_CHN_6CH=5,//5.1ch
		AUDIO_CHN_7CH=6,//6.1ch
		AUDIO_CHN_8CH=7,//7.1ch
		AUDIO_CHN_MAX,
	} ;

	enum  VideoFrameRate
	{
		VIDEO_FRAME_RATE_FORBIDDEN = 0,
		VIDEO_FRAME_RATE_23976	= 1,		/*!< 23.976*/
		VIDEO_FRAME_RATE_24 	= 2,
		VIDEO_FRAME_RATE_25 	= 3,
		VIDEO_FRAME_RATE_2997		= 4,		/*!< 29.97*/
		VIDEO_FRAME_RATE_30 	= 5,
		VIDEO_FRAME_RATE_50 	= 6,
		VIDEO_FRAME_RATE_5994		= 7,		/*!< 59.94*/
		VIDEO_FRAME_RATE_60 	= 8
	};

	
	enum  StringCodecType
	{
		STRING_LATIN1	= 1,		
		STRING_LOCAL8BIT 	= 2,
		STRING_UTF8		= 4,		
		STRING_UTF16 	= 5,
	};

	struct ID3Info
	{
		bool id3String;
		StringCodecType codecType;
		char song[MPMAX_SN_LEN + 1]; //add str end:\0
		char artist[MPMAX_SN_LEN + 1];
		char album[MPMAX_SN_LEN +1];
		char genre[MPMAX_SN_LEN +1];
		char year[4+1];
		char comment[MPMAX_SN_LEN+1];
		bool id3Pic;
		quint32 songflag;
		quint32 artistflag;
		quint32 alnumflag;
		quint32 picSize;
		quint32 picOffset;
	};
	
	struct VideoAttribute
	{
		int audioCnt;
		int subtitleCnt;
		QSize resolution;	
		qreal frameRate;		/*!<  Frame Rate of Video*/
		quint32 videoCodecID;   /*!<  Video Codec ID */
	};

	struct AudioAttribute
	{
		quint32 channelInfo;
		qint32 audioLanguage;
		quint32 audioCodecID;		/*!<  Audio Codec ID */
		ID3Info *id3Info;
	};
	
	struct PlayerMetaData
	{
		int mediaType;
		quint32 totalTime;
		bool supportTrickPlay;
		VideoAttribute* videoAttribute;
		AudioAttribute* audioAttribute;
	};
	
    QMagicPlayerSession(QMediaPlayer::Flags features,QObject *parent);
    virtual ~QMagicPlayerSession();
	
	PlayerState state() const;
	int bufferStatur()const;

    qint64 duration() const;
    qint64 position() const;

    int volume() const;
    bool isMuted() const;

    bool isAudioAvailable() const;
	
	QMediaTimeRange availablePlaybackRanges() const;

    void setVideoRenderer(QObject *renderer);
    bool isVideoAvailable() const;

    bool isSeekable() const;

    qreal playbackRate() const;

	int getCurrentAudioStream() const;

	QImage getID3Pic();
	void stop();
	void actMessage(QMagicPlayerSession::MessageId msgid,QVariant Param0,QVariant Param1);
	void manageActList(QMagicPlayerSession::MessageId msgid, QVariant Param0);


public slots:
	void setVideoOutput(QObject *output);
	void enableMessageWait();
	void waitMessageDone();
	void handleMessage(QMagicPlayerSession::MessageId msgid,QVariant param0,QVariant param1);

signals:
    void durationChanged(qint64 duration);
    void positionChanged(qint64 position);
    void stateChanged(QMagicPlayerSession::PlayerState state);
	void metaDataChanged(const QString &key, const QVariant &value);
	void metadataChanged(QVariantMap metadata);
    void volumeChanged(int volume);
    void mutedStateChanged(bool muted);
    void audioAvailableChanged(bool audioAvailable);
    void videoAvailableChanged(bool videoAvailable);
    void bufferingProgressChanged(int percentFilled);
	void playbackRateChanged(qreal);
    void playbackComplete();
    void seekableChanged(bool);
    void error(int error, const QString &errorString);	
	void playSessionMessage(QMagicPlayerSession::MessageId msgid,QVariant param0,QVariant param1);
	#ifdef SUPPORT_SUNPLUS_EXTEND
	void mediaMessageChanged(QMediaPlayer::MediaMessage messageid,QVariant msgInfo);
	#endif
	
protected:
	QMagicPlayerSessionPrivate *d_ptr;

private:
	QList<QMap<MessageId,QVariant> > *actlist;
	bool isstop;
	int actstate;
	Q_DECLARE_PRIVATE(QMagicPlayerSession)
};

QT_END_NAMESPACE
Q_DECLARE_METATYPE(QMagicPlayerSession::MessageId);
Q_DECLARE_METATYPE(QMagicPlayerSession::PlayerState);
Q_DECLARE_METATYPE(QMagicPlayerSession::PlayerMetaData);
Q_DECLARE_METATYPE(QMagicPlayerSession::AudioAttribute);
Q_DECLARE_METATYPE(QMagicPlayerSession::VideoAttribute);
Q_DECLARE_METATYPE(QMagicPlayerSession::ID3Info);
Q_DECLARE_METATYPE(QMagicPlayerSession::StringCodecType);


#endif // QMAGICPLAYERSESSION_H
