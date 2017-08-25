/**
 * @file AudioNormMem.h
 * @author mm.li
 * @date 2015-11-13
 * @version 
 * @copyright SunMedia Technology (Chengdu) co. Ltd.
 * @brief declarates interface of normal memory
 */
#ifndef AUDIO_NORM_MEM_H
#define AUDIO_NORM_MEM_H

#include "AudioTypes.h"

/* call malloc */
void* AudioNormalMalloc(size_t size);

/* call free */
void AudioNormalFree(void* bufAddr);

/* call realloc */
void* AudioNormalRealloc(void* bufAddr, size_t size);

#endif //AUDIO_NORM_MEM_H
