/**********************************************************************
ModuleName:			QMagicsoundPlugin

Description:		The Implementation of Audio Service Plugin

Copyright:			Copyright @	2015 Sunplus, Inc.

History:			jie.jia@sunmedia.com.cn	 12/15/2015	Creation

**********************************************************************/

#include "qmagicsoundplugin.h"
#include "qmagicsoundaudiodeviceinfo.h"
#include "qmagicsoundaudioinput.h"
#include "qmagicsoundaudiooutput.h"

QT_BEGIN_NAMESPACE

QMagicsoundPlugin::QMagicsoundPlugin(QObject *parent)
    : QAudioSystemPlugin(parent)
{
}

QList<QByteArray> QMagicsoundPlugin::availableDevices(QAudio::Mode mode) const
{
    return QMagicsoundAudioDeviceInfo::availableDevices(mode);
}

QAbstractAudioInput *QMagicsoundPlugin::createInput(const QByteArray &device)
{
    return new QMagicsoundAudioInput(device);
}

QAbstractAudioOutput *QMagicsoundPlugin::createOutput(const QByteArray &device)
{
    return new QMagicsoundAudioOutput(device);
}

QAbstractAudioDeviceInfo *QMagicsoundPlugin::createDeviceInfo(const QByteArray &device, QAudio::Mode mode)
{
    return new QMagicsoundAudioDeviceInfo(device, mode);
}

QT_END_NAMESPACE
