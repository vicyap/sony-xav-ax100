/**********************************************************************
ModuleName:			QMagicsoundAudioOutput

Description:		The Implementations of Audio Service Output Functions

Copyright:			Copyright @	2015 Sunplus, Inc.

History:			jie.jia@sunmedia.com.cn	 12/15/2015	Creation

**********************************************************************/

#include <QtCore/qcoreapplication.h>
#include <QtCore/qmath.h>
#include <QtMultimedia/private/qaudiohelpers_p.h>
#include "qmagicsoundaudiooutput.h"
#include "qmagicsoundaudiodeviceinfo.h"

QT_BEGIN_NAMESPACE

//#define DEBUG_AUDIO 1

QMagicsoundAudioOutput::QMagicsoundAudioOutput(const QByteArray &device)
	: m_audioSource(NULL)	
	, m_errorState(QAudio::NoError)
	, m_deviceState(QAudio::StoppedState)
	, m_audioBuffer(NULL)
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
	, m_cliTrack(NULL)
{
	MS_TRACE("in");     

    m_timer = new QTimer(this);
    connect(m_timer,SIGNAL(timeout()),SLOT(userFeed()));
}

QMagicsoundAudioOutput::~QMagicsoundAudioOutput()
{
    close();
    disconnect(m_timer, SIGNAL(timeout()));
    QCoreApplication::processEvents();
    delete m_timer;
}
void QMagicsoundAudioOutput::setVolume(qreal v)
{
    const qreal normalizedVolume = qBound(qreal(0.0), v, qreal(1.0));
    if (m_deviceState != QAudio::ActiveState) {
        m_volume = normalizedVolume;
        return;
    }

	if(m_volume != v && m_cliTrack)
	{
		if (m_cliTrack->setVolume(qFloor(normalizedVolume*100.0f)) == AUDIO_NO_ERROR)
        	m_volume = normalizedVolume;
	}
    
}
qreal QMagicsoundAudioOutput::volume() const
{
    return m_volume;
}
QAudio::Error QMagicsoundAudioOutput::error() const
{
    return m_errorState;
}
QAudio::State QMagicsoundAudioOutput::state() const
{
    return m_deviceState;
}
void QMagicsoundAudioOutput::start(QIODevice* device)
{
	MS_TRACE("in");
	MS_DEBUG("PULL mode\n");

	if(m_deviceState != QAudio::StoppedState)
		close();
	
	m_errorState = QAudio::NoError;
	m_deviceState = QAudio::StoppedState;

    m_pullMode = true;
    m_audioSource = device;

	if (open()) {
		//setup timer.
	//	m_timer->start(m_period_time);
		setError(QAudio::NoError);
        setState(QAudio::ActiveState);
    } else {
    	close();
        setError(QAudio::OpenError);
        setState(QAudio::StoppedState);
    }
}

QIODevice* QMagicsoundAudioOutput::start()
{
	MS_TRACE("in");
	MS_DEBUG("PUSH mode\n");
	if(m_deviceState != QAudio::StoppedState)
		close();
	
	m_errorState = QAudio::NoError;
	m_deviceState = QAudio::StoppedState;    

    m_audioSource = new OutputPrivate(this);
    m_audioSource->open(QIODevice::WriteOnly|QIODevice::Unbuffered);
    m_pullMode = false;

    if (open()) {
		setError(QAudio::NoError);
        setState(QAudio::IdleState);
    } else {
    	close();
        setError(QAudio::OpenError);
        setState(QAudio::StoppedState);
    }

    return m_audioSource;
}

void QMagicsoundAudioOutput::stop()
{
	MS_TRACE("in");
	MS_DEBUG("device state : %d\n",m_deviceState);
	
    if(m_deviceState == QAudio::StoppedState)
        return;

	close();
	
    setError( QAudio::NoError);
    setState( QAudio::StoppedState);
}

bool QMagicsoundAudioOutput::open()
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
	
	//int framecnt = ClientTrack::getMinCount(m_settings.sampleRate(), SAMPLESIZE2MASK(m_settings.sampleSize()), SAMPLECOUNT2MASK(m_settings.channelCount()), TAG_OUT_DEFAULT);
	MS_DEBUG("sampleRate : %d",m_settings.sampleRate());
	MS_DEBUG("sampleSize : %d",m_settings.sampleSize());
	MS_DEBUG("channelCount : %d",m_settings.channelCount());
	//MS_DEBUG("framecnt : %d",framecnt);
	//new ClientTrack and set format.
	ClientTrack* cliTrack = new ClientTrack(TAG_OUT_DEFAULT, 
										m_settings.sampleRate(), 
										SAMPLESIZE2MASK(m_settings.sampleSize()),
										SAMPLECOUNT2MASK(m_settings.channelCount()));
    if ( m_cliTrack) {
        delete m_cliTrack;
		m_cliTrack = NULL;
    }
	m_cliTrack = cliTrack;

	if(!m_cliTrack)
	{
		MS_ERROR("new ClientTrack fail!\n");
        return false;
	}
	
	if (m_cliTrack->initCheck() != AUDIO_NO_ERROR) {
		
        MS_ERROR("Init ClientTrack fail!\n");
        return false;
    }

	//get buffer size and minimum size from audio service.
	m_buffer_size = m_cliTrack->getTotalBufferSize();
	m_period_size = m_cliTrack->getPerLoopSize();
	
	MS_DEBUG("m_buffer_size:%d",m_buffer_size);
	MS_DEBUG("m_period_size:%d",m_period_size);

	if (m_buffer_size == 0 || m_period_size == 0) {
		MS_ERROR("get buffer size or period size fail!\n");
        return false;
    }

	//new buffer.
	if(m_audioBuffer == NULL)
        m_audioBuffer = new char[m_buffer_size];

	//start ClientTrack thread.
	m_cliTrack->start();

	m_bytesAvailable = bytesFree();	
	m_timer->start(m_period_time);
	//start audio processing.
	m_timeStampOpened.restart();
	m_timeStamp.restart();
	
    m_elapsedTimeOffset = 0;
	m_processedDataSize = 0;
	m_opened = true;
	
	return true;
	
}

void QMagicsoundAudioOutput::close()
{
	MS_TRACE("in");
    m_timer->stop();

    if ( m_cliTrack) {
        delete m_cliTrack;
        m_cliTrack = NULL;
    }
	if (m_audioBuffer) {
        delete[] m_audioBuffer;
        m_audioBuffer = NULL;
    }
    if(!m_pullMode && m_audioSource) {
        delete m_audioSource;
        m_audioSource = NULL;
    }
    m_opened = false;
}


int QMagicsoundAudioOutput::bytesFree() const
{
    if(m_deviceState != QAudio::ActiveState && m_deviceState != QAudio::IdleState)
        return 0;

    int bytes = m_cliTrack->bytesFree();

	return bytes;
}

qint64 QMagicsoundAudioOutput::write( const char *data, qint64 len )
{
    // Write out some audio data
    if ( !m_cliTrack)
        return 0;
	
    int err;
    unsigned int space = bytesFree();

    if (!space)
        return 0;

    if (len < space)
        space = len;

    if (m_volume < 1.0f) {
        unsigned char out[space];
        QAudioHelperInternal::qMultiplySamples(m_volume, m_settings, data, out, space);
        err = m_cliTrack->write(out, space);
    } else {
        err = m_cliTrack->write((unsigned char*)data, space);
    }

    if(err > 0) {
        m_processedDataSize+= err;
        setError( QAudio::NoError);
        setState( QAudio::ActiveState);
		
    } else {
    	MS_ERROR("write error %d!\n",err);
        close();
        setError( QAudio::FatalError);
        setState( QAudio::StoppedState);
    }

	return err;
}

int QMagicsoundAudioOutput::periodSize() const
{
    return m_period_size;
}

void QMagicsoundAudioOutput::setBufferSize(int value)
{
    if(m_deviceState == QAudio::StoppedState)
        m_buffer_size = value;
}

int QMagicsoundAudioOutput::bufferSize() const
{
    return m_buffer_size;
}

void QMagicsoundAudioOutput::setNotifyInterval(int ms)
{
    m_intervalTime = qMax(0, ms);
}

int QMagicsoundAudioOutput::notifyInterval() const
{
    return m_intervalTime;
}

qint64 QMagicsoundAudioOutput::processedUSecs() const
{
    return qint64(1000000) * m_processedDataSize / (m_settings.channelCount()*(m_settings.sampleSize()/8)) / m_settings.sampleRate();
}

void QMagicsoundAudioOutput::resume()
{
    if(m_deviceState == QAudio::SuspendedState) {

        if(m_cliTrack) {
			
            if(m_cliTrack->start() < 0)
        	{
		        close();
		        setError( QAudio::FatalError);
		        setState( QAudio::ActiveState);
        	}
        }

		setError( QAudio::NoError);
		setState( QAudio::ActiveState);
		//if(m_pullMode)
			m_timer->start(m_period_time);
    }
}

void QMagicsoundAudioOutput::setFormat(const QAudioFormat& fmt)
{
	MS_TRACE("in");
    if (m_deviceState == QAudio::StoppedState)
        m_settings = fmt;
}

QAudioFormat QMagicsoundAudioOutput::format() const
{
    return m_settings;
}

void QMagicsoundAudioOutput::suspend()
{
    if(m_deviceState == QAudio::ActiveState || m_deviceState == QAudio::IdleState) {

		//if(m_pullMode)
        	m_timer->stop();
		
		if(m_cliTrack) {

			if(m_cliTrack->stop() < 0) {
		        close();
		        setError( QAudio::FatalError);
		        setState( QAudio::StoppedState);
        	}
		}			
		
        setError( QAudio::NoError);
		setState( QAudio::SuspendedState);
    }
}

void QMagicsoundAudioOutput::userFeed()
{
    if(m_deviceState == QAudio::StoppedState || m_deviceState == QAudio::SuspendedState)
        return;
	
    m_bytesAvailable = bytesFree();

    deviceReady();
}

bool QMagicsoundAudioOutput::deviceReady()
{
	if(m_deviceState == QAudio::StoppedState || m_deviceState == QAudio::SuspendedState)
        return false;
	
    if(m_pullMode) {
        int l = 0;
        if(m_bytesAvailable==0) {
            m_bytesAvailable = bytesFree();
            return false;
        }

        int input = m_bytesAvailable > m_buffer_size ? m_buffer_size : m_bytesAvailable;
        l = m_audioSource->read(m_audioBuffer,input);

        if (!m_cliTrack)
            return false;

        if(l > 0) {
            // Got some data to output
            if(m_deviceState != QAudio::ActiveState)
                return true;
            qint64 bytesWritten = write(m_audioBuffer,l);
            if (bytesWritten != l)
                m_audioSource->seek(m_audioSource->pos()-(l-bytesWritten));
            m_bytesAvailable = bytesFree();

        } else if(l == 0) {
            // Did not get any data to output
            m_bytesAvailable = bytesFree();
            if(m_bytesAvailable > (m_buffer_size - m_period_size)) {
                // Underrun
                if (m_deviceState != QAudio::IdleState) {
                    setError( QAudio::UnderrunError);
					setState( QAudio::IdleState);
                }
            }

        } else if(l < 0) {
            close();
            setError( QAudio::IOError);
			setState( QAudio::StoppedState);
        }
    } else {
        m_bytesAvailable = bytesFree();
        if(m_bytesAvailable > (m_buffer_size - m_period_size)) {
            // Underrun
           
            if (m_deviceState != QAudio::IdleState) {
                setError( QAudio::UnderrunError);
				setState( QAudio::IdleState);
            }
        }
    }

    if(m_deviceState != QAudio::ActiveState)
        return true;

    if(m_intervalTime && (m_timeStamp.elapsed() + m_elapsedTimeOffset) > m_intervalTime) {
        emit notify();
        m_elapsedTimeOffset = m_timeStamp.elapsed() + m_elapsedTimeOffset - m_intervalTime;
        m_timeStamp.restart();
    }
    return true;
}

qint64 QMagicsoundAudioOutput::elapsedUSecs() const
{
    if (m_deviceState == QAudio::StoppedState)
        return 0;

    return m_timeStampOpened.elapsed()*1000;
}

void QMagicsoundAudioOutput::reset()
{
	stop();
}
void QMagicsoundAudioOutput::setError(QAudio::Error error)
{
    if (m_errorState != error) {
        m_errorState = error;
        emit errorChanged(error);
    }
}

void QMagicsoundAudioOutput::setState(QAudio::State state)
{
    if (m_deviceState != state) {
        m_deviceState = state;
        emit stateChanged(state);
    }
}
OutputPrivate::OutputPrivate(QMagicsoundAudioOutput* audio)
{
    audioDevice = qobject_cast<QMagicsoundAudioOutput*>(audio);
}

OutputPrivate::~OutputPrivate() {}

qint64 OutputPrivate::readData( char* data, qint64 len)
{
    Q_UNUSED(data)
    Q_UNUSED(len)

    return 0;
}

qint64 OutputPrivate::writeData(const char* data, qint64 len)
{
    int retry = 0;
    qint64 written = 0;
    if((audioDevice->m_deviceState == QAudio::ActiveState)
            ||(audioDevice->m_deviceState == QAudio::IdleState)) {
        while(written < len) {
            int chunk = audioDevice->write(data+written,(len-written));
            if(chunk <= 0)
                retry++;
            written+=chunk;
            if(retry > 10)
                return written;
        }
    }
    return written;
}

QT_END_NAMESPACE

