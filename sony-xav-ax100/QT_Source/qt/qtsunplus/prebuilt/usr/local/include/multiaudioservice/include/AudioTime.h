/**
 * @file AudioTime.h
 * @author xun.lu
 * @date 2015-11-02
 * @version
 * @copyright SunMedia Technology (Chengdu) co. Ltd.
 * @brief declaration of time's functions
 */
#ifndef AUDIO_TIME_H
#define AUDIO_TIME_H

typedef struct audio_timespec_s
{
	long long tv_sec;
	long tv_nsec;
} audio_timespec_t;

/* get system time in ms */
long AudioGetSysTimeMs();

/* get system time in us */
long long AudioGetSysTimeUs();

void AudioGetTime(audio_timespec_t* time);


#endif //AUDIO_TIME_H