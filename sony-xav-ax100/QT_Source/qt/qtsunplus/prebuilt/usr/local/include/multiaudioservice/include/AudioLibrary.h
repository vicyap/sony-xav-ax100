/**
 * @file AudioLibrary.h
 * @author xun.lu
 * @date 2015-12-12
 * @version 
 * @copyright SunMedia Technology (Chengdu) co. Ltd.
 * @brief ddeclarates of library's function
 */
#ifndef AUDIO_LIBRARY_H
#define AUDIO_LIBRARY_H

void* AudioLibraryLoad(const char* path);
void* AudioLibraryGetSymbol(void* lib, const char* symb_name);
void  AudioLibraryClose(void* lib);

#endif //AUDIO_LIBRARY_H