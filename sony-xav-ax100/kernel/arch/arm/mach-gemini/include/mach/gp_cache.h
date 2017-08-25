/**
 * @file gp_cache.h
 */

#ifndef _GP_CACHE_H_
#define _GP_CACHE_H_

#include <asm-generic/ioctl.h>
//#include <mach/typedef.h>
/**************************************************************************
 *                           C O N S T A N T S                            *
 **************************************************************************/

/* Ioctl for device node definition */
#define CACHE_IOCTL_MAGIC	'C'
#define DCACHE_FLUSH	_IOWR(CACHE_IOCTL_MAGIC, 1, unsigned int)
#define TLB_FLUSH	_IOW(CACHE_IOCTL_MAGIC,  2, unsigned int)
#define DCACHE_CLEAN_RANGE	_IOW(CACHE_IOCTL_MAGIC,  3, struct gp_cache_address_s)
#define DCACHE_INVALIDATE_RANGE	_IOW(CACHE_IOCTL_MAGIC,  4, struct gp_cache_address_s)


/**************************************************************************
 *                          D A T A    T Y P E S                          *
 **************************************************************************/

typedef struct gp_cache_address_s {
	unsigned int start;
	unsigned int size;
}gp_cache_address_t;

/**************************************************************************
 *            U S E R    S P A C E    I O C T L    U S A G E              *
 **************************************************************************/
/*
 * [DCACHE_FLUSH] : flush all L1 and L2 cache
 *   There is no any input parameter.
 *
 *   Kernel will flush cache. This is no any output parameter.
 *
 * [DCACHE_CLEAN_RANGE] : clean a range of address
 *   User input gp_cache_address_t to parameter of ioctl().
 *   Fill field @start and @size of chunk_block_t.
 *   @start means the start address which is going to be flushed, and it
 *   is also a user space virtual address. @size means how many size from
 *   @start is going to be flushed.
 *
 *   Kernel will flush address from @start to @start + @size. There is no
 *   output parameter.
 *
 * [DCACHE_INVALIDATE_RANGE] : invalidate a range of address
 *   User input gp_cache_address_t to parameter of ioctl().
 *   Fill field @start and @size of chunk_block_t.
 *   @start means the start address which is going to be invalidated, and
 *   it is also a user space virtual address. @size means how many size
 *   from @start is going to be invalidated.
 *
 *   Kernel will invalidate address from @start to @start + @size. There is
 *   no output parameter.
 */

#endif	/*_GP_CACHE_H_*/
