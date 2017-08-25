/**********************************************************************
ModuleName:			QMagicsoundPlugin

Description:		The Declaration of Audio Service Plugin

Copyright:			Copyright @	2015 Sunplus, Inc.

History:			jie.jia@sunmedia.com.cn	 12/15/2015	Creation

**********************************************************************/

#ifndef QMAGICSOUNDPLUGIN_H
#define QMAGICSOUNDPLUGIN_H

#include <QtMultimedia/qaudiosystemplugin.h>

QT_BEGIN_NAMESPACE

class QMagicsoundPlugin : public QAudioSystemPlugin
{
    Q_OBJECT

    Q_PLUGIN_METADATA(IID "org.qt-project.qt.audiosystemfactory/5.0" FILE "audio.json")

public:
    QMagicsoundPlugin(QObject *parent = 0);
    ~QMagicsoundPlugin() {}

    QList<QByteArray> availableDevices(QAudio::Mode mode) const Q_DECL_OVERRIDE;
    QAbstractAudioInput *createInput(const QByteArray &device) Q_DECL_OVERRIDE;
    QAbstractAudioOutput *createOutput(const QByteArray &device) Q_DECL_OVERRIDE;
    QAbstractAudioDeviceInfo *createDeviceInfo(const QByteArray &device, QAudio::Mode mode) Q_DECL_OVERRIDE;
};

QT_END_NAMESPACE

#endif // QMAGICSOUNDPLUGIN_H