/******************************************************************
    Copyright (c) 2013 SunMedia technology (Chengdu) co. Ltd.
    All right reserved

    File    Name  :  AudioFormat.h
    Date          :  2015-11-09
    Author        :  mm.li@sunmedia.com.cn
    Descriptions  :  declarates interface of AudioFormat
    Change log    :
    Date             Mender               Cause
******************************************************************/

#ifndef AUDIO_FORMAT_H
#define AUDIO_FORMAT_H

/* return bytes per channel
* fmt:      audio format (e.g.FMT_S16)
*/
int av_getBytesFrame(unsigned int fmt);

/* return chs per frame
* chl:      audio channel (e.g.CHL_STEREO)
*/
int av_getChs(unsigned int chl);
#endif // AUDIO_FORMAT_H
