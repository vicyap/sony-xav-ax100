/**********************************************************************
ModuleName:        QMagicVideoWindow

Description:	QVideoWindowControl Function Implementations

Copyright:	Copyright @	2015 Magicplus, Inc.

History:		hh.li@magicmedia.com.cn	 11/19/2015	Creation

**********************************************************************/


#ifndef QMAGICVIDEOWINDOW_H
#define QMAGICVIDEOWINDOW_H

#include <QColor>
#include <QtWidgets/qwidget.h>
#include <qvideowidgetcontrol.h>
#include "qmagicwindowsession.h"

QT_BEGIN_NAMESPACE
class QMagicVideoWidget;
class QMagicVideoWindow : public QVideoWidgetControl
{
    Q_OBJECT
public:
    QMagicVideoWindow(QMagicWindowSession *session,QObject *parent = 0);
	
    ~QMagicVideoWindow();

	QWidget *videoWidget();

    QRect displayRect() const;
    void setDisplayRect(const QRect &rect);

    bool isFullScreen() const;
    void setFullScreen(bool fullScreen);

    Qt::AspectRatioMode aspectRatioMode() const;
    void setAspectRatioMode(Qt::AspectRatioMode mode);

    int brightness() const;
    void setBrightness(int brightness);

    int contrast() const;
    void setContrast(int contrast);

    int hue() const;
    void setHue(int hue);

    int saturation() const;
    void setSaturation(int saturation);
private:
	QMagicWindowSession *mSession;
	QMagicVideoWidget *m_widget;
};

QT_END_NAMESPACE

#endif
