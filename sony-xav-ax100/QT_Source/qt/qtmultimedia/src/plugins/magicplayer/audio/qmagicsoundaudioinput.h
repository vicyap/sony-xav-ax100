/**********************************************************************
ModuleName:			QMagicsoundAudioInput

Description:		The Declarations of Audio Service Input Functions

Copyright:			Copyright @	2015 Sunplus, Inc.

History:			jie.jia@sunmedia.com.cn	 12/15/2015	Creation

**********************************************************************/

#ifndef QMAGICSOUNDAUDIOINPUT_H
#define QMAGICSOUNDAUDIOINPUT_H

#include "ClientRecord.h"

#include <QtCore/qfile.h>
#include <QtCore/qdebug.h>
#include <QtCore/qtimer.h>
#include <QtCore/qstring.h>
#include <QtCore/qstringlist.h>
#include <QtCore/qdatetime.h>

#include <QtMultimedia/qaudio.h>
#include <QtMultimedia/qaudiodeviceinfo.h>
#include <QtMultimedia/qaudiosystem.h>

QT_BEGIN_NAMESPACE


class InputPrivate;

class RingBuffer
{
public:
    RingBuffer();

    void resize(int size);

    int bytesOfDataInBuffer() const;
    int freeBytes() const;

    const char *availableData() const;
    int availableDataBlockSize() const;
    void readBytes(int bytes);

    void write(char *data, int len);

private:
    int m_head;
    int m_tail;

    QByteArray m_data;
};

class QMagicsoundAudioInput : public QAbstractAudioInput
{
    Q_OBJECT
public:
    QMagicsoundAudioInput(const QByteArray &device);
    ~QMagicsoundAudioInput();

    qint64 read(char* data, qint64 len);

    void start(QIODevice* device);
    QIODevice* start();
    void stop();
    void reset();
    void suspend();
    void resume();
    int bytesReady() const;
    int periodSize() const;
    void setBufferSize(int value);
    int bufferSize() const;
    void setNotifyInterval(int milliSeconds);
    int notifyInterval() const;
    qint64 processedUSecs() const;
    qint64 elapsedUSecs() const;
    QAudio::Error error() const;
    QAudio::State state() const;
    void setFormat(const QAudioFormat& fmt);
    QAudioFormat format() const;
    void setVolume(qreal);
    qreal volume() const;
    
    QIODevice* m_audioSource;
    QAudioFormat m_settings;
    QAudio::Error m_errorState;
    QAudio::State m_deviceState;

private slots:
    void userFeed();
    bool deviceReady();

private:
    int checkBytesReady();
    int xrun_recovery(int err);
    int setFormat();
    bool open();
    void close();
	void setError(QAudio::Error error);
	void setState(QAudio::State state);

	RingBuffer m_ringBuffer;
	bool m_opened;
	bool m_pullMode;
	int m_bytesAvailable;
	int m_buffer_size;
    int m_period_size;
	int m_intervalTime;
	unsigned int m_buffer_time;//ms
    unsigned int m_period_time;//ms
	qint64 m_processedDataSize;
	qint64 m_elapsedTimeOffset;
	
    QTimer* m_timer;
	QByteArray m_device;
    QTime m_timeStamp;
    QTime m_timeStampOpened;
	qreal m_volume;
    
    ClientRecord* m_cliRecord;    
};

class InputPrivate : public QIODevice
{
    Q_OBJECT
public:
    InputPrivate(QMagicsoundAudioInput* audio);
    ~InputPrivate();

    qint64 readData( char* data, qint64 len);
    qint64 writeData(const char* data, qint64 len);

    void trigger();
private:
    QMagicsoundAudioInput *audioDevice;
};

QT_END_NAMESPACE


#endif //QMAGICSOUNDAUDIOINPUT_H