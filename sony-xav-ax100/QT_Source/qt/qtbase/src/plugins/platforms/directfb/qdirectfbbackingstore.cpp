/****************************************************************************
**
** Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the plugins of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia.  For licensing terms and
** conditions see http://qt.digia.com/licensing.  For further information
** use the contact form at http://qt.digia.com/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Digia gives you certain additional
** rights.  These rights are described in the Digia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "qdirectfbbackingstore.h"
#include "qdirectfbintegration.h"
#include "qdirectfbblitter.h"
#include "qdirectfbconvenience.h"
#include "qdirectfbwindow.h"
#include <private/qpixmap_blitter_p.h>

#include <QtCore/qdebug.h>

#define QTDFB_FRAME_RATE    (0)
#define region_to_frame
// #define dennis_printf printf
#define dennis_printf

#define open_swap
#define TIME_STAMP

#ifdef TIME_STAMP
#define RGST_OFFSET     0x9c000000
#define COLOR_RED    "\033[0;1;31;40m"
#define COLOR_GREEN  "\033[0;1;32;40m"
#define COLOR_YELLOW "\033[0;1;33;40m"
#define COLOR_ORIGIN "\033[0m"
int timeFirst = 0;

void Read_Reg(int group, int offset, unsigned int* val)
{
        int i = 0;
        int fd = 0;
        void *map_base;
        unsigned int mapped_size;
        //void *virt_addr;
        void *virt_addr;
        unsigned int address = 0;
        unsigned int page_size, offset_in_page, len_byte;
        unsigned int data_32;

        /*Calculate maping base & size*/
        address = RGST_OFFSET + group*32*4 + offset*4;
        len_byte = 4;
        //mapped_size = page_size = getpagesize();      
        mapped_size = page_size = getpagesize();
        offset_in_page = (unsigned int)address & (page_size - 1);
        i = (offset_in_page + len_byte)/page_size;
        mapped_size = mapped_size*(i+1);

        fd = open("/dev/mem", (O_RDWR | O_SYNC));
        if(fd < 0){
                printf("Can not open /dev/mem !\n");
                exit(1);
        }

        map_base = mmap(NULL,
                        mapped_size,
                        (PROT_READ | PROT_WRITE),
                        MAP_SHARED,
                        fd,
                        address & ~(off_t)(page_size - 1));

        if (map_base == MAP_FAILED){
                printf("Mapping fail!\n");
                close(fd);
                exit(1);
        }

        //printf("Memory mapped at address %p, mapped size = %d .\n", map_base, mapped_size);

        virt_addr = (char*)map_base + offset_in_page;
        *val = *((volatile unsigned int *)virt_addr);
#if 0
        if(argc == 3)
        {
                printf("Read G%d.%d = 0x%08x (%d)\n",group,offset,*((volatile unsigned int *)virt_addr),*((volatile unsigned int *)virt_addr));
        }
        else
        {
                for(i=0; i<32; i++)
                {
                        printf("Read G%d.%d = 0x%08x (%d)\n",group,i,*((volatile unsigned int *)(virt_addr+i)),*((volatile unsigned int *)(virt_addr+i)));
                }
        }
#endif

        munmap(map_base, mapped_size);
        close(fd);
}

#define TIMESTAMP(x, th) do { \
                                unsigned int val = AV1_GetStc32(); \
                                if ( th < val ) \
                                  printf(COLOR_RED); \
				else \
                                  printf(COLOR_GREEN); \
                                printf("[TIMESTAMP] --> "); \
                                printf(x); \
                                printf("%d",val/90); \
                                printf(" ms ("); \
                                printf("%d",val); \
                                printf(" > "); \
                                printf("%d",th); \
                                printf(")"); \
                                printf(COLOR_ORIGIN); \
                                printf("\n"); \
                        } while(0)

static unsigned AV1_GetStc32()
{
    unsigned int stc_value = 0;
	unsigned int tmp=0;
	
        /**
         * strictly read sequence, should not change this order.
         */
        // smart latch
	Read_Reg( 97, 0, &tmp );

        // read
	Read_Reg( 97, 1, &tmp );
        stc_value |= (unsigned)tmp << 16;
	Read_Reg( 97, 0, &tmp );
        stc_value |= (unsigned)tmp;

        return stc_value;
}
#endif

#if QTDFB_FRAME_RATE
static int _flush = 0;
static time_t _sec = 0;
static time_t _getNowSec() {
    timeval _tv;
    gettimeofday(&_tv, NULL);
    return _tv.tv_sec;
}
#endif


#include <sys/time.h>
static int64_t getNowUs()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (int64_t)tv.tv_usec + tv.tv_sec * 1000000;
}


QT_BEGIN_NAMESPACE

QDirectFbBackingStore::QDirectFbBackingStore(QWindow *window)
    : QPlatformBackingStore(window), m_pixmap(0), m_pmdata(0)
{
    IDirectFBWindow *dfbWindow = static_cast<QDirectFbWindow *>(window->handle())->dfbWindow();
    dfbWindow->GetSurface(dfbWindow, m_dfbSurface.outPtr());

//WRONGSIZE
    QDirectFbBlitter *blitter = new QDirectFbBlitter(window->size(), m_dfbSurface.data());
    m_pmdata = new QDirectFbBlitterPlatformPixmap;
    m_pmdata->setBlittable(blitter);
    m_pixmap.reset(new QPixmap(m_pmdata));
}

QPaintDevice *QDirectFbBackingStore::paintDevice()
{
    return m_pixmap.data();
}

//int64_t begin2;
void QDirectFbBackingStore::flush(QWindow *, const QRegion &region, const QPoint &offset)
{
    // dennis_printf("[QT] picture prepare time_spent=%lld\n",getNowUs()-begin2);
    m_pmdata->blittable()->unlock();

    //int64_t begin;
    int x1,x2,y1,y2;

#if QTDFB_FRAME_RATE
    if( (_getNowSec() - _sec) >= 1 ) {
        _sec = _getNowSec();
        dennis_printf("[QDirectFB] %d FPS\n", _flush);
        _flush = 0;
    }
    else
    {
        _flush++;
    }
#endif


    QVector<QRect> rects = region.rects();
    
#ifdef region_to_frame
    for (int i = 0 ; i < rects.size(); i++) {
        const QRect rect = rects.at(i);
        DFBRegion dfbReg = { rect.x() + offset.x(),rect.y() + offset.y(),rect.right() + offset.x(),rect.bottom() + offset.y()};

        if(i==0)
        {
            x1 = dfbReg.x1;
            x2 = dfbReg.x2;
            y1 = dfbReg.y1;
            y2 = dfbReg.y2;
        }
        else
        {
            if(dfbReg.x1<x1)
                x1=dfbReg.x1;
            if(dfbReg.y1<y1)
                y1=dfbReg.y1;
            if(dfbReg.x2>x2)
                x2=dfbReg.x2;
            if(dfbReg.y2>y2)
                y2=dfbReg.y2;
        }
    }

    DFBRegion dfbRegTmp = { x1, y1, x2, y2};
    // begin = getNowUs();
    m_dfbSurface->Flip(m_dfbSurface.data(), &dfbRegTmp, DFBSurfaceFlipFlags(DSFLIP_FLUSH));
    // dennis_printf("[QT] m_dfbSurface->Flip(DSFLIP_FLUSH) x1=%d, y1=%d, x2=%d, y2=%d, time_spent=%lld\n"
    //     ,dfbRegTmp.x1,dfbRegTmp.y1,dfbRegTmp.x2,dfbRegTmp.y2,getNowUs()-begin);


#else
    for (int i = 0 ; i < rects.size(); i++) {
        const QRect rect = rects.at(i);
        DFBRegion dfbReg = { rect.x() + offset.x(),rect.y() + offset.y(),rect.right() + offset.x(),rect.bottom() + offset.y()};
    #ifndef open_swap
        m_dfbSurface->Flip(m_dfbSurface.data(), &dfbReg, DFBSurfaceFlipFlags(DSFLIP_BLIT|DSFLIP_ONSYNC));
    #else
            //begin = getNowUs();
            m_dfbSurface->Flip(m_dfbSurface.data(), &dfbReg, DFBSurfaceFlipFlags(DSFLIP_WAIT | DSFLIP_ONSYNC));
            // dennis_printf("[QT] m_dfbSurface->Flip x1=%d, y1=%d, x2=%d, y2=%d, time_spent=%lld\n"
            //     ,dfbReg.x1,dfbReg.y1,dfbReg.x2,dfbReg.y2,getNowUs()-begin);
    #endif
    }
#endif

#ifdef TIME_STAMP
	if(timeFirst==0)
	{
		timeFirst++;
		TIMESTAMP("APP 1st Frame:",0);
	}
#endif

    // begin2 = getNowUs();
}

void QDirectFbBackingStore::resize(const QSize &size, const QRegion& reg)
{
    Q_UNUSED(reg);

    if ((m_pmdata->width() == size.width()) &&
        (m_pmdata->height() == size.height()))
        return;

    QDirectFbBlitter *blitter = new QDirectFbBlitter(size, m_dfbSurface.data());
    m_pmdata->setBlittable(blitter);
}

static inline void scrollSurface(IDirectFBSurface *surface, const QRect &r, int dx, int dy)
{
    const DFBRectangle rect = { r.x(), r.y(), r.width(), r.height() };
    surface->Blit(surface, surface, &rect, r.x() + dx, r.y() + dy);
    const DFBRegion region = { rect.x + dx, rect.y + dy, r.right() + dx, r.bottom() + dy };
    surface->Flip(surface, &region, DFBSurfaceFlipFlags(DSFLIP_BLIT));
}

bool QDirectFbBackingStore::scroll(const QRegion &area, int dx, int dy)
{
    m_pmdata->blittable()->unlock();

    if (!m_dfbSurface || area.isEmpty())
        return false;
    m_dfbSurface->SetBlittingFlags(m_dfbSurface.data(), DSBLIT_NOFX);
    if (area.rectCount() == 1) {
        scrollSurface(m_dfbSurface.data(), area.boundingRect(), dx, dy);
    } else {
        const QVector<QRect> rects = area.rects();
        const int n = rects.size();
        for (int i=0; i<n; ++i) {
            scrollSurface(m_dfbSurface.data(), rects.at(i), dx, dy);
        }
    }
    return true;
}

QT_END_NAMESPACE
