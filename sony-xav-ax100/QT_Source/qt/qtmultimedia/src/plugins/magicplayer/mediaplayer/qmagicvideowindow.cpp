/**********************************************************************
ModuleName:        QMagicVideoWindow

Description:	QVideoWindowControl Function Implementations

Copyright:	Copyright @	2015 Magicplus, Inc.

History:		hh.li@magicmedia.com.cn	 11/19/2015	Creation

**********************************************************************/
#include <QDebug>
#include "qmagicvideowindow.h"
#include "qmagicplayerserviceplugin.h"

#define TAG "QMPVideoWidget "
class QMagicVideoWidget : public QWidget
{
public:
    QMagicVideoWidget(QMagicWindowSession *session,QWidget *parent = 0)
        :QWidget(parent)
        ,mSession(session)
        ,mRect(0,0,0,0)
    {
    };
    QSize sizeHint() const;
protected:
	void showEvent(QShowEvent *event);
	void moveEvent(QMoveEvent *);
	void resizeEvent(QResizeEvent *);
	void paintEvent(QPaintEvent *event);
private:
	QMagicWindowSession *mSession;
	QRect mRect;
};

QSize QMagicVideoWidget::sizeHint() const
{
	MP_DEBUG(TAG,"%d*%d",mSession->displayRect().width(),mSession->displayRect().height());
	return QSize(mSession->displayRect().width(),mSession->displayRect().height());
}

void QMagicVideoWidget::paintEvent(QPaintEvent *event)
{	
	MP_DEBUG(TAG,"( %d %d )%d*%d",this->parentWidget()->x(),this->parentWidget()->y(),this->width(),this->height());
	QRect rect(this->parentWidget()->x(),this->parentWidget()->y(),this->width(),this->height());
	if(rect!= mRect)
	{
	    mSession->setDisplayRect(rect);
		mRect = rect;
	}
}


void QMagicVideoWidget::showEvent(QShowEvent *event)
{	
	MP_DEBUG(TAG,"( %d %d )%d*%d",this->parentWidget()->x(),this->parentWidget()->y(),this->width(),this->height());
}
void QMagicVideoWidget::moveEvent(QMoveEvent *)
{
	MP_DEBUG(TAG,"( %d %d )%d*%d",this->parentWidget()->x(),this->parentWidget()->y(),this->width(),this->height());
}

void QMagicVideoWidget::resizeEvent(QResizeEvent *)
{
	MP_DEBUG(TAG,"( %d %d )%d*%d",this->parentWidget()->x(),this->parentWidget()->y(),this->width(),this->height());;
}

QMagicVideoWindow::QMagicVideoWindow(QMagicWindowSession *session,QObject *parent)
    :QVideoWidgetControl(parent)
    ,mSession(session)
    ,m_widget(new QMagicVideoWidget(session))
{
	connect(mSession,SIGNAL(fullScreenChanged(bool)),SIGNAL(fullScreenChanged(bool)));
	connect(mSession,SIGNAL(brightnessChanged(int)),SIGNAL(brightnessChanged(int)));
	connect(mSession,SIGNAL(contrastChanged(int)),SIGNAL(contrastChanged(int)));
	connect(mSession,SIGNAL(hueChanged(int)),SIGNAL(hueChanged(int)));
	connect(mSession,SIGNAL(saturationChanged(int)),SIGNAL(saturationChanged(int)));
}

QMagicVideoWindow::~QMagicVideoWindow()
{
   
}

QWidget *QMagicVideoWindow::videoWidget()
{
    return m_widget;
}


QRect QMagicVideoWindow::displayRect() const
{
    return mSession->displayRect();
}

void QMagicVideoWindow::setDisplayRect(const QRect &rect)
{
    mSession->setDisplayRect(rect);
}

Qt::AspectRatioMode QMagicVideoWindow::aspectRatioMode() const
{
    return mSession->aspectRatioMode();
}

void QMagicVideoWindow::setAspectRatioMode(Qt::AspectRatioMode mode)
{
    mSession->setAspectRatioMode(mode);
}

int QMagicVideoWindow::brightness() const
{
    return mSession->brightness();
}

void QMagicVideoWindow::setBrightness(int brightness)
{
	mSession->setBrightness(brightness);
}

int QMagicVideoWindow::contrast() const
{
    return mSession->contrast();
}

void QMagicVideoWindow::setContrast(int contrast)
{
	mSession->setContrast(contrast);
}

int QMagicVideoWindow::hue() const
{
    return mSession->hue();
}

void QMagicVideoWindow::setHue(int hue)
{
	mSession->setHue(hue);
}

int QMagicVideoWindow::saturation() const
{
    return mSession->saturation();
}

void QMagicVideoWindow::setSaturation(int saturation)
{
 	mSession->setSaturation(saturation);
}

bool QMagicVideoWindow::isFullScreen() const
{
    return mSession->isFullScreen();
}

void QMagicVideoWindow::setFullScreen(bool fullScreen)
{
    mSession->setFullScreen(fullScreen);
}

