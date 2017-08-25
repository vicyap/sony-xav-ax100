/**********************************************************************
ModuleName:        QMagicWindowSession

Description:	dispplay control to display service

Copyright:	Copyright @	2016 Magicplus, Inc.

History:		hh.li@sunmedia.com.cn	 03/17/2016	Creation

**********************************************************************/


#ifndef QMAGICWINDOWSESSION_H
#define QMAGICWINDOWSESSION_H
#include <QObject>

QT_BEGIN_NAMESPACE
class QMagicWindowSessionPrivate;
class QMagicWindowSession : public QObject
{
    Q_OBJECT
public:
    QMagicWindowSession(QObject *parent = 0);
    ~QMagicWindowSession();
    QRect displayRect() const;
    void setDisplayRect(const QRect &rect);

    bool isFullScreen() const;
    void setFullScreen(bool fullScreen);

    QSize nativeSize() const;

    Qt::AspectRatioMode aspectRatioMode() const;
    void setAspectRatioMode(Qt::AspectRatioMode mode);

    void repaint();

    int brightness() const;
    void setBrightness(int brightness);

    int contrast() const;
    void setContrast(int contrast);

    int hue() const;
    void setHue(int hue);

    int saturation() const;
    void setSaturation(int saturation);

	bool showDisplayWinow();
	
Q_SIGNALS:
	void fullScreenChanged(bool fullScreen);
	void brightnessChanged(int brightness);
	void contrastChanged(int contrast);
	void hueChanged(int hue);
	void saturationChanged(int saturation);
	void nativeSizeChanged();//not support currently


protected:
	QMagicWindowSessionPrivate *d_ptr;
	
private:
	Q_DECLARE_PRIVATE(QMagicWindowSession)

	
};
QT_END_NAMESPACE

#endif
