/**
 * @file AudioSharedMem.h
 * @author mm.li
 * @date 2015-11-02
 * @version 
 * @copyright SunMedia Technology (Chengdu) co. Ltd.
 * @brief declarates interface of AudioSharedMem
 */
#ifndef AUDIO_SHARED_MEM_H
#define AUDIO_SHARED_MEM_H

#include <stdlib.h>

/* malloc share mem, return share addr on success or NUll of fail.
* len:      size of share mem to alloc
* shredAddr: sharedMem addr to share for other process
*/
int shrMalloc(size_t size, unsigned int& sharedKey);

/* delete share mem
* bufAddr:  share id malloced by shrMalloc
*/
int shrFree(int sharedId, void* sharedAddr = NULL, unsigned int sharedKey = 0);

/* unMap share mem
* shredId:  share id malloced by shrMalloc
*/
int shrUnShare(void* sharedAddr);

/* realloc share mem, return share addr on success or NUll of fail.
* shredId:  share Id malloced by shrMalloc
* len:      size of share mem to alloc
* shredAddr: sharedMem addr to share for other process
*/
int shrRealloc(int sharedId, size_t size, unsigned int& sharedAddr);

/* mmap a phy addr to  vitual addr
* shredId: share id to mmap
* size:     size of this mem block
*/
void* shrMmap(int sharedId, size_t size = 0);

/* mmap a phy addr to  vitual addr
* shredAddr: addr to share
*/
void* shrShare(unsigned int sharedKey, size_t size = 0);
#endif //AUDIO_SHARED_MEM_H
