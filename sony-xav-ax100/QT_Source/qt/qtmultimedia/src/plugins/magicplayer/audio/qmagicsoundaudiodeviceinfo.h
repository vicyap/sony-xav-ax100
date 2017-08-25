/**********************************************************************
ModuleName:			QMagicsoundAudioDeviceInfo

Description:		The DeviceInfo Functions Declarations of Audio Service

Copyright:			Copyright @	2015 Sunplus, Inc.

History:			jie.jia@sunmedia.com.cn	 12/15/2015	Creation

**********************************************************************/

#ifndef QMAGICSOUNDAUDIODEVICEINFO_H
#define QMAGICSOUNDAUDIODEVICEINFO_H

#include <QtCore/qbytearray.h>
#include <QtCore/qstringlist.h>
#include <QtCore/qlist.h>
#include <QtCore/qdebug.h>

#include <QtMultimedia/qaudio.h>
#include <QtMultimedia/qaudiodeviceinfo.h>
#include <QtMultimedia/qaudiosystem.h>

#include "AudioTypes.h"
#include "AudioError.h"
#include "qmagicsoundcomm.h"

QT_BEGIN_NAMESPACE

const unsigned int MS_MAX_SAMPLE_RATES = 16;
const unsigned int MS_SAMPLE_RATES[] =
    { SR_8000, SR_16000, SR_32000, SR_64000, SR_128000,\
      SR_11025, SR_22050, SR_44100, SR_88200, SR_176400,\
      SR_6000, SR_12000, SR_24000, SR_48000, SR_96000, SR_192000};

class QMagicsoundAudioDeviceInfo : public QAbstractAudioDeviceInfo
{
    Q_OBJECT
public:
    QMagicsoundAudioDeviceInfo(QByteArray dev,QAudio::Mode mode);
    ~QMagicsoundAudioDeviceInfo();

	void updateLists();
	bool testSettings(const QAudioFormat& format) const;
    QAudioFormat preferredFormat() const;
    bool isFormatSupported(const QAudioFormat& format) const;
    QString deviceName() const;
    QStringList supportedCodecs();
    QList<int> supportedSampleRates();
    QList<int> supportedChannelCounts();
    QList<int> supportedSampleSizes();
    QList<QAudioFormat::Endian> supportedByteOrders();
    QList<QAudioFormat::SampleType> supportedSampleTypes();
    static QByteArray defaultInputDevice();
    static QByteArray defaultOutputDevice();
    static QList<QByteArray> availableDevices(QAudio::Mode);

private:
    QString device;
    QAudio::Mode mode;
    QAudioFormat nearest;
    QList<int> sampleRatez;
    QList<int> channelz;
    QList<int> sizez;
    QList<QAudioFormat::Endian> byteOrderz;
    QStringList codecz;
    QList<QAudioFormat::SampleType> typez;
};

QT_END_NAMESPACE


#endif // QMAGICSOUNDAUDIODEVICEINFO_H