/**********************************************************************
ModuleName:			QMagicsoundAudioInput

Description:		The Implementations of Audio Service Input Functions

Copyright:			Copyright @	2015 Sunplus, Inc.

History:			jie.jia@sunmedia.com.cn	 12/15/2015	Creation

**********************************************************************/

#include <QtCore/qcoreapplication.h>
#include <QtCore/qmath.h>

#include <QtMultimedia/private/qaudiohelpers_p.h>
#include "qmagicsoundaudioinput.h"
#include "qmagicsoundaudiodeviceinfo.h"

QT_BEGIN_NAMESPACE

//#define DEBUG_AUDIO 1

QMagicsoundAudioInput::QMagicsoundAudioInput(const QByteArray &device)
	: m_audioSource(NULL)
	, m_errorState(QAudio::NoError)
    , m_deviceState(QAudio::StoppedState)
    , m_opened(false)
    , m_pullMode(true)
    , m_bytesAvailable(0)
	, m_buffer_size(0)
	, m_period_size(0)
	, m_intervalTime(1000)
	, m_buffer_time(192)
	, m_period_time(8)
	, m_processedDataSize(0)
	, m_elapsedTimeOffset(0)
	, m_device(device)
	, m_volume(1.0f)
    , m_cliRecord(NULL)
{
    m_timer = new QTimer(this);
    connect(m_timer,SIGNAL(timeout()),SLOT(userFeed()));
}

QMagicsoundAudioInput::~QMagicsoundAudioInput()
{
    close();
    disconnect(m_timer, SIGNAL(timeout()));
    QCoreApplication::processEvents();
    delete m_timer;
}

void QMagicsoundAudioInput::setVolume(qreal v)
{
    const qreal normalizedVolume = qBound(qreal(0.0), v, qreal(1.0));
    if (m_deviceState != QAudio::ActiveState) {
        m_volume = normalizedVolume;
        return;
    }

	if(m_volume != v && m_cliRecord)
	{
		if (m_cliRecord->setVolume(qFloor(normalizedVolume*100.0f)) == AUDIO_NO_ERROR)
        	m_volume = normalizedVolume;
	}
    
}

qreal QMagicsoundAudioInput::volume() const
{
    return m_volume;
}

QAudio::Error QMagicsoundAudioInput::error() const
{
    return m_errorState;
}

QAudio::State QMagicsoundAudioInput::state() const
{
    return m_deviceState;
}

void QMagicsoundAudioInput::setFormat(const QAudioFormat& fmt)
{
    if (m_deviceState == QAudio::StoppedState)
        m_settings = fmt;
}

QAudioFormat QMagicsoundAudioInput::format() const
{
    return m_settings;
}

void QMagicsoundAudioInput::start(QIODevice* device)
{
    if(m_deviceState != QAudio::StoppedState)
        close();
	
	m_errorState = QAudio::NoError;
	m_deviceState = QAudio::StoppedState;

    m_pullMode = true;
    m_audioSource = device;

    if (open()) {
		setError(QAudio::NoError);
        setState(QAudio::ActiveState);
    } else {
    	MS_ERROR("open error,stop!");
    	close();
        setError(QAudio::OpenError);
        setState(QAudio::StoppedState);
    }
}

QIODevice* QMagicsoundAudioInput::start()
{
	if(m_deviceState != QAudio::StoppedState)
		close();
	
	m_errorState = QAudio::NoError;
	m_deviceState = QAudio::StoppedState;    

    m_audioSource = new InputPrivate(this);
    m_audioSource->open(QIODevice::ReadOnly|QIODevice::Unbuffered);
    m_pullMode = false;

    if (open()) {
		setError(QAudio::NoError);
        setState(QAudio::IdleState);
    } else {
    	MS_ERROR("open error,stop!");
    	close();
        setError(QAudio::OpenError);
        setState(QAudio::StoppedState);
    }

    return m_audioSource;
}

void QMagicsoundAudioInput::stop()
{
    if(m_deviceState == QAudio::StoppedState)
        return;

	close();
	
    setError( QAudio::NoError);
    setState( QAudio::StoppedState);
}

bool QMagicsoundAudioInput::open()
{
	if(m_opened)
        return true;
	
	MS_TRACE("in");
	int err = -1;

	//check format.
	if (!m_settings.isValid()) {
        MS_ERROR("open error, invalid format.");
    } else if (m_settings.channelCount() <= 0) {
        MS_ERROR("open error, invalid number of channels (%d).",
                 m_settings.channelCount());
    } else if (m_settings.sampleSize() <= 0) {
        MS_ERROR("open error, invalid sample size (%d).",
                 m_settings.sampleSize());
    } else if (m_settings.sampleRate() < SR_8000 || m_settings.sampleRate() > SR_192000) {
        MS_ERROR("open error, sample rate out of range (%d).", 
				m_settings.sampleRate());
	} else {
		err = 0;
    }

	if(err == -1)
	{
		MS_ERROR("check format fail!\n");
        return false;
	}

	//check device.
	QString dev = QString(QLatin1String(m_device.constData()));

	if((dev.compare(QLatin1String("default")) != 0) && (dev.compare(QLatin1String("magicsound")) != 0)) {
		MS_ERROR("device:%s error!\n",(const char *)dev.data());
        return false;
	}
	
	//int framecnt = ClientRecord::getMinCount(m_settings.sampleRate(), SAMPLESIZE2MASK(m_settings.sampleSize()), SAMPLECOUNT2MASK(m_settings.channelCount()), TAG_OUT_DEFAULT);
	MS_DEBUG("sampleRate : %d",m_settings.sampleRate());
	MS_DEBUG("sampleSize : %d",m_settings.sampleSize());
	MS_DEBUG("channelCount : %d",m_settings.channelCount());
	//MS_DEBUG("framecnt : %d",framecnt);
	//new ClientTrack and set format.
	ClientRecord* cliRecord = new ClientRecord(TAG_IN_DEFAULT, 
										m_settings.sampleRate(), 
										SAMPLESIZE2MASK(m_settings.sampleSize()),
										SAMPLECOUNT2MASK(m_settings.channelCount()));

	if ( m_cliRecord) {
        delete m_cliRecord;
		m_cliRecord = NULL;
    }
	m_cliRecord = cliRecord;
	if(!m_cliRecord)
	{
		MS_ERROR("new ClientRecord fail!\n");
        return false;
	}
	
	if (m_cliRecord->initCheck() != AUDIO_NO_ERROR) {
		
        MS_ERROR("Init ClientTrack fail!\n");
        return false;
    }

	//get buffer size and minimum size from audio service.
	m_buffer_size = m_cliRecord->getTotalBufferSize();
	m_period_size = m_cliRecord->getPerLoopSize();
	
	MS_DEBUG("m_buffer_size:%d",m_buffer_size);
	MS_DEBUG("m_period_size:%d",m_period_size);

	if (m_buffer_size == 0 || m_period_size == 0) {
		MS_ERROR("get buffer size or period size fail!\n");
        return false;
    }

	//new ring buffer.
	m_ringBuffer.resize(m_buffer_size);

	//start m_cliRecord thread.
	m_cliRecord->start();

	m_bytesAvailable = checkBytesReady();

	MS_DEBUG("m_bytesAvailable:%d",m_bytesAvailable);
	if(m_pullMode)
        connect(m_audioSource,SIGNAL(readyRead()),this,SLOT(userFeed()));

	//setup timer.
	m_timer->start(m_period_time);
	
	//start audio processing.
	m_timeStampOpened.restart();
	m_timeStamp.restart();
	
    m_elapsedTimeOffset = 0;
	m_processedDataSize = 0;
	m_opened = true;
	
	return true;
	
}

void QMagicsoundAudioInput::close()
{
    m_timer->stop();

    if ( m_cliRecord) {
        delete m_cliRecord;
        m_cliRecord = NULL;
    }
	if(!m_pullMode && m_audioSource) {
        delete m_audioSource;
        m_audioSource = NULL;
    }

	m_opened = false;
}

int QMagicsoundAudioInput::checkBytesReady()
{
    if(m_deviceState != QAudio::ActiveState && m_deviceState != QAudio::IdleState)
        return 0;

    int bytes = m_cliRecord->bytesUsed();

	return bytes;
}

int QMagicsoundAudioInput::bytesReady() const
{
    return qMax(m_bytesAvailable, 0);
}

qint64 QMagicsoundAudioInput::read(char* data, qint64 len)
{
    // Read in some audio data and write it to QIODevice, pull mode
    if ( !m_cliRecord )
        return 0;

    int bytesRead = 0;
    int bytesInRingbufferBeforeRead = m_ringBuffer.bytesOfDataInBuffer();

    if (m_ringBuffer.bytesOfDataInBuffer() < len) {

        // bytesAvaiable is saved as a side effect of checkBytesReady().
        int bytesToRead = checkBytesReady();

        if (bytesToRead < 0) {
            // bytesAvailable as negative is error code.
            MS_ERROR("underrun!");
            close();
            setError( QAudio::IOError);
			setState( QAudio::StoppedState);
            return 0;
        }

        bytesToRead = qMin<qint64>(len, bytesToRead);
        bytesToRead = qMin<qint64>(m_ringBuffer.freeBytes(), bytesToRead);
        bytesToRead -= bytesToRead % m_period_size;

        while(bytesToRead > 0) {
            char buffer[bytesToRead];
            unsigned int out = bytesToRead > m_buffer_size ? m_buffer_size : bytesToRead;

			bytesRead = m_cliRecord->read(buffer, out);
            if (m_volume < 1.0f)
                QAudioHelperInternal::qMultiplySamples(m_volume, m_settings, buffer, buffer, bytesRead);

            if (bytesRead >= 0) {
                m_ringBuffer.write(buffer, bytesRead);
                break;
            } else {
            	MS_ERROR("read error %d!\n",bytesRead);
		        close();
		        setError( QAudio::FatalError);
		        setState( QAudio::StoppedState);

				return  bytesRead;
            }
        }

    }

    bytesRead += bytesInRingbufferBeforeRead;

    if (bytesRead > 0) {
        // got some send it onward
        if (m_deviceState != QAudio::ActiveState && m_deviceState != QAudio::IdleState)
            return 0;

        if (m_pullMode) {
            qint64 l = 0;
            qint64 bytesWritten = 0;
            while (m_ringBuffer.bytesOfDataInBuffer() > 0) {
                l = m_audioSource->write(m_ringBuffer.availableData(), m_ringBuffer.availableDataBlockSize());
                if (l > 0) {
                    m_ringBuffer.readBytes(l);
                    bytesWritten += l;
                } else {
                    break;
                }
            }

            if (l < 0) {
				MS_ERROR("IO write fail!");
				close();
		        setError( QAudio::IOError);
		        setState( QAudio::StoppedState);
            } else if (l == 0 && bytesWritten == 0) {
                if (m_deviceState != QAudio::IdleState) {
					setError( QAudio::NoError);
		        	setState( QAudio::IdleState);
                }
            } else {
                m_bytesAvailable -= bytesWritten;
                m_processedDataSize += bytesWritten;
                if (m_deviceState != QAudio::ActiveState) {
					setError( QAudio::NoError);
		        	setState( QAudio::ActiveState);
                }
            }

            return bytesWritten;
        } else {
            while (m_ringBuffer.bytesOfDataInBuffer() > 0) {
                int size = m_ringBuffer.availableDataBlockSize();
                memcpy(data, m_ringBuffer.availableData(), size);
                data += size;
                m_ringBuffer.readBytes(size);
            }

            m_bytesAvailable -= bytesRead;
            m_processedDataSize += bytesRead;
            if (m_deviceState != QAudio::ActiveState) {
                setError( QAudio::NoError);
		        setState( QAudio::ActiveState);
            }

            return bytesRead;
        }
    }

    return 0;
}

void QMagicsoundAudioInput::resume()
{
    if(m_deviceState == QAudio::SuspendedState) {

        if(m_cliRecord) {
			
            if(m_cliRecord->start() < 0)
        	{
        		MS_ERROR("Start fail!");
		        close();
		        setError( QAudio::FatalError);
		        setState( QAudio::StoppedState);
        	}
        }

		setError( QAudio::NoError);
		setState( QAudio::ActiveState);
		
		m_timer->start(m_period_time);
    }
}

void QMagicsoundAudioInput::setBufferSize(int value)
{
    m_buffer_size = value;
}

int QMagicsoundAudioInput::bufferSize() const
{
    return m_buffer_size;
}

int QMagicsoundAudioInput::periodSize() const
{
    return m_period_size;
}

void QMagicsoundAudioInput::setNotifyInterval(int ms)
{
    m_intervalTime = qMax(0, ms);
}

int QMagicsoundAudioInput::notifyInterval() const
{
    return m_intervalTime;
}

qint64 QMagicsoundAudioInput::processedUSecs() const
{
    return qint64(1000000) * m_processedDataSize / (m_settings.channelCount()*(m_settings.sampleSize()/8)) / m_settings.sampleRate();
}

void QMagicsoundAudioInput::suspend()
{
    if(m_deviceState == QAudio::ActiveState || m_deviceState == QAudio::IdleState) {

        m_timer->stop();

		if(m_cliRecord) {

			if(m_cliRecord->stop() < 0) {
				MS_ERROR("Stop fail!");
		        close();
		        setError( QAudio::FatalError);
		        setState( QAudio::StoppedState);
        	}
		}
		
    	setError( QAudio::NoError);
		setState( QAudio::SuspendedState);
    }
}

void QMagicsoundAudioInput::userFeed()
{
    if(m_deviceState == QAudio::StoppedState || m_deviceState == QAudio::SuspendedState)
        return;

	m_bytesAvailable = checkBytesReady();
	
    deviceReady();
}

bool QMagicsoundAudioInput::deviceReady()
{
	if(m_deviceState == QAudio::StoppedState || m_deviceState == QAudio::SuspendedState)
        return false;
	
    if(m_pullMode) {
        // reads some audio data and writes it to QIODevice
        read(0, m_buffer_size);
    } else {
        // emits readyRead() so user will call read() on QIODevice to get some audio data
        InputPrivate* a = qobject_cast<InputPrivate*>(m_audioSource);
        a->trigger();
    }

	m_bytesAvailable = checkBytesReady();
	

    if(m_intervalTime && (m_timeStamp.elapsed() + m_elapsedTimeOffset) > m_intervalTime) {
        emit notify();
        m_elapsedTimeOffset = m_timeStamp.elapsed() + m_elapsedTimeOffset - m_intervalTime;
        m_timeStamp.restart();
    }
    return true;
}

qint64 QMagicsoundAudioInput::elapsedUSecs() const
{
    if (m_deviceState == QAudio::StoppedState)
        return 0;

    return m_timeStampOpened.elapsed()*1000;
}

void QMagicsoundAudioInput::reset()
{
	stop();
}

void QMagicsoundAudioInput::setError(QAudio::Error error)
{
    if (m_errorState != error) {
        m_errorState = error;
        emit errorChanged(error);
    }
}

void QMagicsoundAudioInput::setState(QAudio::State state)
{
    if (m_deviceState != state) {
        m_deviceState = state;
        emit stateChanged(state);
    }
}
InputPrivate::InputPrivate(QMagicsoundAudioInput* audio)
{
    audioDevice = qobject_cast<QMagicsoundAudioInput*>(audio);
}

InputPrivate::~InputPrivate()
{
}

qint64 InputPrivate::readData( char* data, qint64 len)
{
    return audioDevice->read(data,len);
}

qint64 InputPrivate::writeData(const char* data, qint64 len)
{
    Q_UNUSED(data)
    Q_UNUSED(len)
    return 0;
}

void InputPrivate::trigger()
{
    emit readyRead();
}

RingBuffer::RingBuffer() :
        m_head(0),
        m_tail(0)
{
}

void RingBuffer::resize(int size)
{
    m_data.resize(size);
}

int RingBuffer::bytesOfDataInBuffer() const
{
    if (m_head < m_tail)
        return m_tail - m_head;
    else if (m_tail < m_head)
        return m_data.size() + m_tail - m_head;
    else
        return 0;
}

int RingBuffer::freeBytes() const
{
    if (m_head > m_tail)
        return m_head - m_tail - 1;
    else if (m_tail > m_head)
        return m_data.size() - m_tail + m_head - 1;
    else
        return m_data.size() - 1;
}

const char *RingBuffer::availableData() const
{
    return (m_data.constData() + m_head);
}

int RingBuffer::availableDataBlockSize() const
{
    if (m_head > m_tail)
        return m_data.size() - m_head;
    else if (m_tail > m_head)
        return m_tail - m_head;
    else
        return 0;
}

void RingBuffer::readBytes(int bytes)
{
    m_head = (m_head + bytes) % m_data.size();
}

void RingBuffer::write(char *data, int len)
{
    if (m_tail + len < m_data.size()) {
        memcpy(m_data.data() + m_tail, data, len);
        m_tail += len;
    } else {
        int bytesUntilEnd = m_data.size() - m_tail;
        memcpy(m_data.data() + m_tail, data, bytesUntilEnd);
        if (len - bytesUntilEnd > 0)
            memcpy(m_data.data(), data + bytesUntilEnd, len - bytesUntilEnd);
        m_tail = len - bytesUntilEnd;
    }
}

QT_END_NAMESPACE
