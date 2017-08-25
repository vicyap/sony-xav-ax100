TARGET = qtaudio_magicsound
QT += multimedia-private

PLUGIN_TYPE = audio
PLUGIN_CLASS_NAME = QMagicsoundPlugin
load(qt_plugin)

LIBS += -lasclient
INCPATH += $$QT_SYSROOT/include/multiaudioservice/include

HEADERS += \
		qmagicsoundcomm.h \
    qmagicsoundplugin.h \
    qmagicsoundaudiodeviceinfo.h \
    qmagicsoundaudiooutput.h \
    qmagicsoundaudioinput.h

SOURCES += \
    qmagicsoundplugin.cpp \
    qmagicsoundaudiodeviceinfo.cpp \
    qmagicsoundaudiooutput.cpp \
    qmagicsoundaudioinput.cpp

OTHER_FILES += \
    audio.json