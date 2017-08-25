TARGET = magicmediaplayer

QT += multimedia-private
QT += multimediawidgets

include(../version.pri)

DEFINES += VERSION_INFO=\\\"$$MP_VER_INFO\\\"
DEFINES += VERSION_NUM=\\\"$$MP_VER_NUMBER\\\"

INCLUDEPATH += $$QT_SYSROOT/include/magicplayerutil/

LIBS +=  -lmagicutil
 
PLUGIN_TYPE = mediaservice
PLUGIN_CLASS_NAME = QMagicPlayerServicePlugin
load(qt_plugin)

HEADERS += \
		qmagicplayercontrol.h \
		qmagicplayerservice.h \
		qmagicplayerserviceplugin.h \
		qmagicmetadataprovider.h \
		qmagicvideowindow.h\
		autodetect.h

SOURCES += \
		qmagicplayercontrol.cpp \
		qmagicplayerservice.cpp \
		qmagicmetadataprovider.cpp \
		qmagicplayerserviceplugin.cpp\
		qmagicvideowindow.cpp\
		autodetect.cpp
		

OTHER_FILES += \
    mediaplayer.json

