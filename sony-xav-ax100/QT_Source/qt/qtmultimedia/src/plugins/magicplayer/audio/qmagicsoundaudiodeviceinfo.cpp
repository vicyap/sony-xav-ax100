/**********************************************************************
ModuleName:			QMagicsoundAudioDeviceInfo

Description:		The DeviceInfo Functions Implementations of Audio Service

Copyright:			Copyright @	2015 Sunplus, Inc.

History:			jie.jia@sunmedia.com.cn	 12/15/2015	Creation

**********************************************************************/
#include "qmagicsoundaudiodeviceinfo.h"

//#include <alsa/version.h>//jie.jia

QT_BEGIN_NAMESPACE

QMagicsoundAudioDeviceInfo::QMagicsoundAudioDeviceInfo(QByteArray dev, QAudio::Mode mode)
{
	MS_TRACE("in");
    device = QLatin1String(dev);
    this->mode = mode;
	updateLists();
}

QMagicsoundAudioDeviceInfo::~QMagicsoundAudioDeviceInfo()
{
	MS_TRACE("in");
    //close();
}

bool QMagicsoundAudioDeviceInfo::isFormatSupported(const QAudioFormat& format) const
{
	MS_TRACE("in");
    return testSettings(format);
}

QAudioFormat QMagicsoundAudioDeviceInfo::preferredFormat() const
{
	MS_TRACE("in");
    QAudioFormat nearest;
    if(mode == QAudio::AudioOutput) {
        nearest.setSampleRate(44100);
        nearest.setChannelCount(2);
        nearest.setByteOrder(QAudioFormat::LittleEndian);
        nearest.setSampleType(QAudioFormat::SignedInt);
        nearest.setSampleSize(16);
        nearest.setCodec(QLatin1String("audio/pcm"));
    } else {
        nearest.setSampleRate(8000);
        nearest.setChannelCount(1);
        nearest.setSampleType(QAudioFormat::UnSignedInt);
        nearest.setSampleSize(8);
        nearest.setCodec(QLatin1String("audio/pcm"));
        if(!testSettings(nearest)) {
            nearest.setChannelCount(2);
            nearest.setSampleSize(16);
            nearest.setSampleType(QAudioFormat::SignedInt);
        }
    }
    return nearest;
}

QString QMagicsoundAudioDeviceInfo::deviceName() const
{
	MS_TRACE("in");
	
    return device;
}

QStringList QMagicsoundAudioDeviceInfo::supportedCodecs()
{
	MS_TRACE("in");
    updateLists();
    return codecz;
}

QList<int> QMagicsoundAudioDeviceInfo::supportedSampleRates()
{
	MS_TRACE("in");
	updateLists();
    return sampleRatez;
}

QList<int> QMagicsoundAudioDeviceInfo::supportedChannelCounts()
{
	MS_TRACE("in");
	updateLists();
    return channelz;
}

QList<int> QMagicsoundAudioDeviceInfo::supportedSampleSizes()
{
	MS_TRACE("in");
   	updateLists();
    return sizez;
}

QList<QAudioFormat::Endian> QMagicsoundAudioDeviceInfo::supportedByteOrders()
{
	MS_TRACE("in");
   	updateLists();
    return byteOrderz;
}

QList<QAudioFormat::SampleType> QMagicsoundAudioDeviceInfo::supportedSampleTypes()
{
	MS_TRACE("in");
   	updateLists();
    return typez;
}
void QMagicsoundAudioDeviceInfo::updateLists()
{
    // redo all lists based on current settings
    sampleRatez.clear();
    channelz.clear();
    sizez.clear();
    byteOrderz.clear();
    typez.clear();
    codecz.clear();

    for(int i=0; i<(int)MS_MAX_SAMPLE_RATES; i++) {
        //if(snd_pcm_hw_params_test_rate(handle, params, SAMPLE_RATES[i], dir) == 0)
        sampleRatez.append(MS_SAMPLE_RATES[i]);
    }
	
    channelz.append(av_getChs(CHL_MONO));//mono
    channelz.append(av_getChs(CHL_STEREO));//stereo
	channelz.append(av_getChs(CHL_5POINT1));//5.1
	channelz.append(av_getChs(CHL_7POINT1));//7.1
	
    sizez.append(8);//8 bits
    sizez.append(16);//16 bits
    sizez.append(32);//32 bits
	
    byteOrderz.append(QAudioFormat::LittleEndian);
    //byteOrderz.append(QAudioFormat::BigEndian);
	
    typez.append(QAudioFormat::SignedInt);
    typez.append(QAudioFormat::UnSignedInt);
    typez.append(QAudioFormat::Float);
	
    codecz.append(QLatin1String("audio/pcm"));
}
bool QMagicsoundAudioDeviceInfo::testSettings(const QAudioFormat& format) const
{
    // Set nearest to closest settings that do work.
    // See if what is in settings will work (return value).

    bool failed = false;
    bool match = false;

    // check codec
    for( int i = 0; i < codecz.count(); i++) {
        if (format.codec() == codecz.at(i))
            match = true;
    }
    if (!match) failed = true;

    // check channel
    match = false;
    if (!failed) {
        for (int i = 0; i < channelz.count(); i++) {
            if (format.channelCount() == channelz.at(i)) {
                match = true;
                break;
            }
        }
        if (!match)
            failed = true;
    }

    // check sampleRate
    match = false;
    if (!failed) {
        for (int i = 0; i < sampleRatez.count(); i++) {
            if (format.sampleRate() == sampleRatez.at(i)) {
                match = true;
                break;
            }
        }
        if (!match)
            failed = true;
    }

    // check sample size
    match = false;
    if (!failed) {
        for( int i = 0; i < sizez.count(); i++) {
            if (format.sampleSize() == sizez.at(i)) {
                match = true;
                break;
            }
        }
        if (!match)
            failed = true;
    }

    // check byte order
    match = false;
    if (!failed) {
        for( int i = 0; i < byteOrderz.count(); i++) {
            if (format.byteOrder() == byteOrderz.at(i)) {
                match = true;
                break;
            }
        }
        if (!match)
            failed = true;
    }

    // check sample type
    match = false;
    if (!failed) {
        for( int i = 0; i < typez.count(); i++) {
            if (format.sampleType() == typez.at(i)) {
                match = true;
                break;
            }
        }
        if (!match)
            failed = true;
    }

    if(!failed) {
        // settings work
        return true;
    }
    return false;
}
QList<QByteArray> QMagicsoundAudioDeviceInfo::availableDevices(QAudio::Mode mode)
{
	MS_TRACE("in");
    QList<QByteArray> devices;

    devices.append("magicsound");	

	//PRINT_BACKTRACE();
    return devices;
}

QByteArray QMagicsoundAudioDeviceInfo::defaultInputDevice()
{
	MS_TRACE("in");
    QList<QByteArray> devices = availableDevices(QAudio::AudioInput);
    if(devices.size() == 0)
        return QByteArray();

    return devices.first();
}

QByteArray QMagicsoundAudioDeviceInfo::defaultOutputDevice()
{
	MS_TRACE("in");
    QList<QByteArray> devices = availableDevices(QAudio::AudioOutput);
    if(devices.size() == 0)
        return QByteArray();

    return devices.first();
}

QT_END_NAMESPACE
