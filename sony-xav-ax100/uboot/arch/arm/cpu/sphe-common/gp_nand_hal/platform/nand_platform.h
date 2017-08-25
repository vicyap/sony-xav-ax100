#ifndef NAND_PLATFORM_H
#define NAND_PLATFORM_H

#define ALL_BITS_FULL_32BITS		((UINT32)0xFFFFFFFFUL)
#define ALL_BITS_FULL_16BITS		((UINT16)0xFFFFU)
#define ALL_BITS_FULL_8BITS			((UINT8)0xFFU)

#if defined(ASIC_NUM)
#define ASIC_8050 	1
#define ASIC_32900 	2
#define ASIC_83000 	3
#define ASIC_32900b 4
#define ASIC_GP19   5 // 32600b
#define ASIC_32700b 6

//#define ASIC_NUM	ASIC_32900b // use Makefile -D to specify
#define ASIC_letb456 10 
#endif

#if !defined(CONFIG_SP_NFTL)
#if defined(NAND_RUN_OS_IS)
#define NAND_OS_LINUX			1
#define NAND_OS_BL				2 // mean non os
#define NAND_OS_UCLINUX			3
//#define NAND_RUN_OS_IS		NAND_OS_BL // use Makefile -D to specify
#endif
#endif

#include "nand_version.h"

#define CACHE_READ_VERSION_1
#define CACHE_READ_VERSION_2

#define CACHE_WRITE_VERSION_1
#define CACHE_WRITE_VERSION_2

#if (NAND_RUN_OS_IS != NAND_OS_BL ) // Mason@2016-03-11 
#include "nand_platform_linux_includes.h"


#if (ASIC_NUM == ASIC_32700b)
#define NAND_MAX_2K_PAGE_SUPPORT	// limit nand driver support maximal 2K page size, 64 page per block only, purpose  
									// is to save buffer size to run on plaform with limited memory size
#endif

#if defined(NAND_MAX_2K_PAGE_SUPPORT)
#define NAND_DEBUG_ON				// when max 2K page siz, platform memory is small, so turn this off to reduce debug message
#else
#define NAND_DEBUG_ON
#endif

//#define	NAND_DEBUG_SUPPORT_MEM_UTIL

#if (NAND_RUN_OS_IS == NAND_OS_LINUX || NAND_RUN_OS_IS == NAND_OS_UCLINUX)
#define __ASM__		__asm__

#define NAND_CORE_PROTECT	

#if (ASIC_NUM == ASIC_83000)
#define NAND_USE_SRAM_AS_WORK_BUF			    1 // use big sram buffer to solve data hazard
#else
#define NAND_USE_SRAM_AS_WORK_BUF			    0 // use big sram buffer to solve data hazard
#endif

#if (ASIC_NUM == ASIC_letb456 )
#define NAND_USE_INTERRUPT						0  // for letb456 NAND ctrl
#define BCH_USE_INTERRUPT						  0  // for letb456 BCG
#else
#define NAND_USE_INTERRUPT						1
#endif

#if (ASIC_NUM == ASIC_83000)
#define BCH_USE_INTERRUPT						0 // since 83000 BCH executation rate more than, so use polling
#else
#define BCH_USE_INTERRUPT						0 // since waiting io complete will go to sleep which take minimal 10ms 
                                                  // but bch decode take less than this, so use polling to increase performance
#endif

#define NAND_OS_SUPPORT_MMU						1 // such as ioremap_nocache, iounmap
#define NAND_OS_SUPPORT_SEMA					1 // such as up, down
#define NAND_OS_SUPPORT_STDIO					1 // such as memset, memcpy, strcat, strcpy
#define NAND_OS_SUPPORT_MEM_UTIL			    1 // such as kmalloc, kfree
#define NAND_OS_SUPPORT_DELAY_UTIL		        1 // such as ndelay, mdelay
#define NAND_OS_SUPPORT_CACHE_UTIL		        1 // such as dma_map_single, dma_unmap_single
                                                  // dma_sync_single_for_device, dma_sync_single_for_cpu
#define NAND_OS_SUPPORT_PRINT_UTIL		        1
#define NAND_OS_SUPPORT_TIMER					1

#if (ASIC_NUM == ASIC_83000)
#define NAND_OS_SUPPORT_SDMA_MEMCPY		        1
#else
#define NAND_OS_SUPPORT_SDMA_MEMCPY		        0
#endif

extern void gp_invalidate_dcache_range(unsigned int start,unsigned int size); // Mason@2015-12-08
extern void gp_clean_dcache_range(unsigned int start, unsigned int size); // Mason@2015-12-08

#else

#define __ASM__		__asm

extern void *virt_to_phys(void *in_addr);
extern void gp_invalidate_dcache_range_AA(unsigned int start,unsigned int size);
extern void gp_clean_dcache_range(unsigned int start, unsigned int size);

#if (ASIC_NUM == ASIC_83000)
#define NAND_USE_SRAM_AS_WORK_BUF			1	// use big sram buffer to solve data hazard
#else
#define NAND_USE_SRAM_AS_WORK_BUF			0	// use big sram buffer to solve data hazard
#endif

#define NAND_USE_INTERRUPT                  0
#define BCH_USE_INTERRUPT                   0
#define NAND_OS_SUPPORT_MMU                 0 // 1
#if defined(NAND_OS_uC)
#define NAND_OS_SUPPORT_SEMA                1 // 1
#else
#define NAND_OS_SUPPORT_SEMA                0
#endif
#define NAND_OS_SUPPORT_STDIO               0
#if defined(NAND_OS_uC)
#define NAND_OS_SUPPORT_MEM_UTIL			1
#else
#define NAND_OS_SUPPORT_MEM_UTIL			0
#endif
#define NAND_OS_SUPPORT_DELAY_UTIL          0
#define NAND_OS_SUPPORT_CACHE_UTIL          0
#define NAND_OS_SUPPORT_PRINT_UTIL          0 // 1
#define NAND_OS_SUPPORT_TIMER               0

#if (ASIC_NUM == ASIC_83000)
#define NAND_OS_SUPPORT_SDMA_MEMCPY		    1
#else
#define NAND_OS_SUPPORT_SDMA_MEMCPY		    0
#endif

#define USE_REG_TO_SWITCH_PIN_GROUP

#endif // end else NAND_RUN_OS_IS == NAND_OS_LINUX || NAND_RUN_OS_IS == NAND_OS_UCLINUX

#if (NAND_OS_SUPPORT_MEM_UTIL == 1)
#if (NAND_RUN_OS_IS == NAND_OS_UCLINUX)
#define NAND_USE_GP_CHUNK_MEM               1
#else
#define NAND_USE_GP_CHUNK_MEM               0
#endif
#else
#define NAND_USE_GP_CHUNK_MEM               0
#endif

#define _BUDDY_MGR_OS_ALIGNED_SIZE	32

#if (NAND_RUN_OS_IS != NAND_OS_LINUX && NAND_RUN_OS_IS != NAND_OS_UCLINUX)
#include "typedef.h"

typedef unsigned char SP_BOOL;

#if 0
#ifndef true
#define true		1
#endif

#ifndef false
#define false		0
#endif

#ifndef NULL
#define NULL		0
#endif
#endif

#include "std_errno.h"

#if (ASIC_NUM == ASIC_83000)
#include "./hal/regmap/hardware.h"
#include "./hal/regmap/reg_scu.h"
#include "./hal/regmap/reg_gpio.h"
#include "./hal/hal_gpio.h"
#include "./hal/gp_pin_grp.h"
#include "./hal/wdt.h"
#include "./app/gp_thread.h"
#include "util.h"
#include "lib_gpfb.h"
#endif

#endif // (NAND_RUN_OS_IS != NAND_OS_LINUX && NAND_RUN_OS_IS != NAND_OS_UCLINUX)

#if (NAND_OS_SUPPORT_PRINT_UTIL == 0)
#define KERN_EMERG
#define KERN_ALERT
#define KERN_CRIT
#define KERN_ERR
#define KERN_WARNING
#define KERN_NOTICE
#define KERN_INFO

#if (NAND_RUN_OS_IS != NAND_OS_LINUX && NAND_RUN_OS_IS != NAND_OS_UCLINUX)
#if (ASIC_NUM == ASIC_83000)
extern void bufprint(const char *pbFmt,...);
extern gpDebugPrint_t dbgprint;

#define printk(args...)		dbgprint(args)
//#define printk(args...)	bufprint(args)
#define DIAG_INFO		    dbgprint
#else

#if (NAND_RUN_OS_IS == NAND_OS_BL) && !defined(NAND_OS_BL2)
#include "project.h"
#endif

#if (NAND_RUN_OS_IS == NAND_OS_BL) && defined(NAND_OS_BL2)
#if (ASIC_NUM == ASIC_GP19 || ASIC_NUM == ASIC_32700b)
#if defined(NAND_OS_uC)
#if 0
extern BOOLEAN OSRunning;    
extern void *OSSemCreate(INT16U cnt);
extern void OSSemPend(void *pevent, INT16U timeout, INT8U *err);
extern INT8U OSSemPost(void *pevent);
extern void print_string(CHAR *fmt, ...);
extern INT32S cache_drain_range(INT32U addr, INT32U size);
extern INT32S cache_invalid_range(INT32U addr, INT32U size);
extern void * gp_malloc_align(INT32U size, INT32U align);
#define DBG_PRINT print_string
#define _DRV_L1_CACHE 1
#define GPIO_OUTPUT             1   
extern BOOLEAN gpio_read_io(INT32U port);
extern BOOLEAN gpio_write_io(INT32U port, BOOLEAN data);
extern BOOLEAN gpio_init_io(INT32U port, BOOLEAN direction);
#else
#include "project.h"
#include "gplib_print_string.h"
#include "drv_l1_dma.h"
#include "drv_l1_gpio.h"
#include "drv_l1_cache.h"
#ifndef _DRV_L1_CACHE
#error "no _DRV_L1_CACHE"
#endif
#endif
#else
#include "project.h"
#include "print.h"
//#include "drv_l1_cache.h"
#ifndef _DRV_L1_CACHE
#error "no _DRV_L1_CACHE"
#endif
extern INT32S cache_drain_range(INT32U addr, INT32U size);
extern INT32S cache_invalid_range(INT32U addr, INT32U size);
#ifndef GPIO_OUTPUT
#define GPIO_OUTPUT             1 
extern BOOLEAN gpio_read_io(INT32U port);
extern BOOLEAN gpio_write_io(INT32U port, BOOLEAN data);
extern BOOLEAN gpio_init_io(INT32U port, BOOLEAN direction);
#endif  
#endif
#else
#include "sio.h"
#define DBG_PRINT  sio_printf
#endif
#endif

#define printk				DBG_PRINT
//#define printk(args...)		DBG_PRINT(args)

#define DIAG_INFO		    DBG_PRINT

#endif
#endif

#else

#if (NAND_RUN_OS_IS == NAND_OS_UCLINUX)
#define DIAG_INFO(...)		printk(__VA_ARGS__)
#endif

#endif // end else if NAND_OS_SUPPORT_PRINT_UTIL == 0

#if (NAND_OS_SUPPORT_CACHE_UTIL == 0)

#if (NAND_RUN_OS_IS != NAND_OS_LINUX && NAND_RUN_OS_IS != NAND_OS_UCLINUX)
#define GFP_DMA							1
#define GFP_KERNEL					    2

#define DMA_TO_DEVICE				    1
#define DMA_FROM_DEVICE			        2
#define DMA_BIDIRECTIONAL		        3

typedef void 		*dma_addr_t;

#endif

#endif // end if NAND_OS_SUPPORT_CACHE_UTIL == 0

#if (NAND_OS_SUPPORT_DELAY_UTIL == 0)
#if (NAND_RUN_OS_IS != NAND_OS_LINUX && NAND_RUN_OS_IS != NAND_OS_UCLINUX)
extern unsigned long jiffies;

#endif

#endif

#if (NAND_OS_SUPPORT_STDIO == 0)

void *nand_platform_memcpy(void *dest, const void *src, UINT32 count);
void *nand_platform_memset(void *p, int c, UINT32 count);
int nand_platform_memcmp(const void *p1, const void *p2, UINT32 n);
char *nand_platform_strcpy(char *dst, const char *src);
int nand_platform_strcmp (const char *p1, const char *p2);
char *nand_platform_strcat(char *dest, const char *src);
UINT32 nand_platform_strlen(const char *s);

#define NAND_MEMCPY(dest, src, count)					nand_platform_memcpy((dest), (src), (count))
#define NAND_MEMSET(p, c, n)						    nand_platform_memset((p), (c), (n))
#define NAND_MEMCMP(p1, p2, n)							nand_platform_memcmp((p1), (p2), (n))
#define NAND_STRCPY(dst, src)							nand_platform_strcpy((dst), (src))
#define NAND_STRCMP(p1, p2)							    nand_platform_strcmp((p1), (p2))
#define NAND_STRCAT(dest, src)							nand_platform_strcat((dest), (src))
#define NAND_STRLEN(s)									nand_platform_strlen((s))
#else
#define NAND_MEMCPY(dest, src, count)					memcpy((dest), (src), (count))
#define NAND_MEMSET(p, c, n)							memset((p), (c), (n))
#define NAND_MEMCMP(p1, p2, n)						    memcmp((p1), (p2), (n))
#define NAND_STRCPY(dst, src)							strcpy((dst), (src))
#define NAND_STRCMP(p1, p2)								strcmp((p1), (p2))
#define NAND_STRCAT(dest, src)						    strcat((dest), (src))
#define NAND_STRLEN(s)								    strlen((s))

#endif

#if (NAND_OS_SUPPORT_SDMA_MEMCPY == 0)
#define NAND_SDMA_MEMCPY_DRAM_TO_DRAM(dest, src, count)		NAND_MEMCPY((dest), (src), (count))
#define NAND_SDMA_MEMCPY_SRAM_TO_DRAM(dest, src, count)		NAND_MEMCPY((dest), (src), (count))
#define NAND_SDMA_MEMCPY_DRAM_TO_SRAM(dest, src, count)		NAND_MEMCPY((dest), (src), (count))											
#else
void *nand_platform_sdma_memcpy_dram_to_dram(void *dest, const void *src, UINT32 count);
#if 1
#define NAND_SDMA_MEMCPY_DRAM_TO_DRAM(dest, src, count)		nand_platform_sdma_memcpy_dram_to_dram((dest), (src), (count))
#else
#define NAND_SDMA_MEMCPY_DRAM_TO_DRAM(dest, src, count)		NAND_MEMCPY((dest), (src), (count))
#endif

void *nand_platform_sdma_memcpy_sram_to_dram(void *dest, const void *src, UINT32 count);
#if 1
#define NAND_SDMA_MEMCPY_SRAM_TO_DRAM(dest, src, count)		nand_platform_sdma_memcpy_sram_to_dram((dest), (src), (count))
#else
#define NAND_SDMA_MEMCPY_SRAM_TO_DRAM(dest, src, count)		NAND_MEMCPY((dest), (src), (count))
#endif

void *nand_platform_sdma_memcpy_dram_to_sram(void *dest, const void *src, UINT32 count);
#if 1
#define NAND_SDMA_MEMCPY_DRAM_TO_SRAM(dest, src, count)		nand_platform_sdma_memcpy_dram_to_sram((dest), (src), (count))
#else
#define NAND_SDMA_MEMCPY_DRAM_TO_SRAM(dest, src, count)		NAND_MEMCPY((dest), (src), (count))
#endif

#endif

#if (NAND_OS_SUPPORT_MMU == 0)
#define __user

void *ioremap_nocache(unsigned long phys_addr, unsigned long size);
void iounmap(void * addr);
unsigned long copy_to_user(void __user *to, const void *from, unsigned long count);
unsigned long copy_from_user(void *to, const void __user *from, unsigned long count);
#endif

#define KERNEL_3_0_8  // Mason@2015-11-17


#if (NAND_RUN_OS_IS == NAND_OS_LINUX || NAND_RUN_OS_IS == NAND_OS_UCLINUX)

// Mason@2015-12-07 adding testing
//#if (KERNEL_VERSION(3,4,5) == LINUX_VERSION_CODE)
//#define KERNEL_3_4_5
//#endif

#if (KERNEL_VERSION(3,10,0) == LINUX_VERSION_CODE)
#define KERNEL_3_10_0
#endif

#if (KERNEL_VERSION(3,0,8) == LINUX_VERSION_CODE)
#define KERNEL_3_0_8
#endif

#if (KERNEL_VERSION(2,6,38) == LINUX_VERSION_CODE)
#define KERNEL_2_6_38
#endif

#if (KERNEL_VERSION(2,6,32) == LINUX_VERSION_CODE)
#define KERNEL_2_6_32
#endif

#if (KERNEL_VERSION(2,6,35) == LINUX_VERSION_CODE)
#define KERNEL_2_6_35
#endif
#else
#define KERNEL_2_6_35
#endif

#if defined(KERNEL_3_10_0)
#if !defined(KERNEL_2_6_35)
#define KERNEL_2_6_35
#endif
#endif

#if defined(KERNEL_3_0_8)
#if !defined(KERNEL_2_6_35)
#define KERNEL_2_6_35
#endif
#endif

#if defined(KERNEL_2_6_38)
#if !defined(KERNEL_2_6_35)
#define KERNEL_2_6_35
#endif
#endif

#if defined(KERNEL_2_6_35)
#define blk_queue_max_sectors 						blk_queue_max_hw_sectors
#define blk_queue_max_phys_segments					blk_queue_max_segments
#define blk_queue_max_hw_segments(a, b)			
#endif

#if defined(KERNEL_2_6_38) || defined(KERNEL_3_0_8) || defined(KERNEL_3_10_0)
//#if defined(KERNEL_2_6_38) || defined(KERNEL_3_0_8) || defined(KERNEL_3_10_0) || defined(KERNEL_3_4_5)
#define REQ_RW_SYNC									REQ_SYNC
#define init_MUTEX(sem)								sema_init((sem), 1)
#define blk_queue_ordered(a, b, c)					blk_queue_flush((a), REQ_FLUSH)
#endif

#if (ASIC_NUM == ASIC_83000)
#define NAND_CHIP_NUM		4
#else
#define NAND_CHIP_NUM		2
#endif

extern void DrvNand_WP_Initial(void);
extern void DrvNand_WP_Disable(void);
extern void DrvNand_WP_Enable(void);
extern int DrvNand_WP_Get(void);

/////////////////////////////////////////////////////////////////////////////////
#if (NAND_OS_SUPPORT_MEM_UTIL == 0)

#define NAND_PLATFORM_MEM_MGR	0	// 0 use nand_byte_mem_mgr
									// 1 use buddy mem mgr
									
void *nand_platform_malloc(UINT32 size);
void nand_platform_free(void *ptr);

#define NAND_MEM_ALLOC_NATIVE(siz, flags)					nand_platform_malloc((siz))	
#define NAND_MEM_FREE_NATIVE(ptr)							nand_platform_free((ptr))

#else

#if (NAND_RUN_OS_IS == NAND_OS_LINUX || NAND_RUN_OS_IS == NAND_OS_UCLINUX)

#if (NAND_USE_GP_CHUNK_MEM == 1)
#include <mach/gp_chunkmem.h>
#define NAND_MEM_ALLOC_NATIVE(siz, flags)					gp_chunk_malloc((siz))	
#define NAND_MEM_FREE_NATIVE(ptr)							gp_chunk_free((ptr))
#else
// Mason@2016-01-14 GFP_DMA to GFP_KERNEL 
#define NAND_MEM_ALLOC_NATIVE(siz, flags)					kmalloc((siz), (flags))	
// #define NAND_MEM_ALLOC_NATIVE(siz, flags)					gp_chunk_malloc(0,1,(unsigned int)(siz))
// extern unsigned int* Get_SP_NAND_FTL_DISK(void);
//#define NAND_MEM_ALLOC_NATIVE(siz, flags)					dmam_alloc_coherent(struct device* Get_SP_NAND_FTL_DISK(),(siz),NULL,(flags))	
// #define NAND_MEM_ALLOC_NATIVE(siz, flags)					kmalloc((siz), GFP_KERNEL)	


#define NAND_MEM_FREE_NATIVE(ptr)							kfree((ptr))
//#define NAND_MEM_FREE_NATIVE(ptr)	            gp_chunk_free((ptr))

#endif

#endif

#if defined(NAND_OS_uC)
#define NAND_MEM_ALLOC_NATIVE(siz, flags)					gp_malloc_align((siz), 32)	
#define NAND_MEM_FREE_NATIVE(ptr)							gp_free((ptr))
#endif

#endif // else if NAND_OS_SUPPORT_MEM_UTIL == 0

#if defined(NAND_DEBUG_SUPPORT_MEM_UTIL)
void *nand_platform_malloc_debug(UINT32 size, UINT32 flags, const char *file_name, UINT32 line, const char *func_name);
void nand_platform_free_debug(void *ptr, const char *file_name, UINT32 line, const char *func_name);

#ifdef __FUNCTION__								
#define NAND_MEM_ALLOC(siz, flags)							nand_platform_malloc_debug((siz), (flags), (__FILE__), (__LINE__), (__FUNCTION__))											
#define NAND_MEM_FREE(ptr)									nand_platform_free_debug((ptr), (__FILE__), (__LINE__), (__FUNCTION__)) 
#else
#define NAND_MEM_ALLOC(siz, flags)							nand_platform_malloc_debug((siz), (flags), (__FILE__), (__LINE__), ("nofunc"))											
#define NAND_MEM_FREE(ptr)									nand_platform_free_debug((ptr), (__FILE__), (__LINE__), ("nofunc")) 
#endif
#else
#define NAND_MEM_ALLOC(siz, flags)							NAND_MEM_ALLOC_NATIVE((siz), (flags))
#define NAND_MEM_FREE(ptr)									NAND_MEM_FREE_NATIVE((ptr))
#endif

#if (NAND_USE_SRAM_AS_WORK_BUF == 0)
#define NAND_MEM_ALLOC_SRAM_NATIVE(siz, flags)		        NAND_MEM_ALLOC_NATIVE((siz), (flags))	
#define NAND_MEM_FREE_SRAM_NATIVE(ptr)						NAND_MEM_FREE_NATIVE((ptr))
#define NAND_MEM_ALLOC_SRAM(siz, flags)						NAND_MEM_ALLOC((siz), (flags))
#define NAND_MEM_FREE_SRAM(ptr)								NAND_MEM_FREE((ptr))
#else
void *nand_platform_init_sram_zone(void);
void nand_platform_uninit_sram_zone(void);
void *nand_platform_get_sram_start_addr(void **phy_addr);
void *kmalloc_sram(UINT32 size, UINT32 flags);
void kfree_sram(void *ptr);
void *nand_platform_get_sram_phy_addr(void *vir_addr);
UINT8 nand_platform_is_sram_addr(void *vir_addr);
#define NAND_MEM_ALLOC_SRAM_NATIVE(siz, flags)		        kmalloc_sram((siz), (flags))	
#define NAND_MEM_FREE_SRAM_NATIVE(ptr)						kfree_sram((ptr))
#define NAND_MEM_ALLOC_SRAM(siz, flags)						NAND_MEM_ALLOC_SRAM_NATIVE((siz), (flags))
#define NAND_MEM_FREE_SRAM(ptr)								NAND_MEM_FREE_SRAM_NATIVE((ptr))
#endif

//////////////////////////////////////////////////////////////////////////////////
#if (NAND_OS_SUPPORT_CACHE_UTIL == 0)
void nand_platform_invalidate_dcache_range(void *start, UINT32 siz, UINT8 is_desc);
void nand_platform_clean_dcache_range(void *start, UINT32 siz, UINT8 is_desc);
void *nand_platform_dma_map_single(void *dev, void *buffer, UINT32 size, UINT8 direction);
void nand_platform_dma_unmap_single(void *dev, void *buffer, UINT32 size, UINT8 direction);
void nand_platform_dma_sync_single_for_device(void *dev, void *bus_addr, UINT32 size, UINT8 direction);
void nand_platform_dma_sync_single_for_cpu(void *dev, void *bus_addr, UINT32 size, UINT8 direction);

#define NAND_INVALIDATE_DCACHE_RANGE_NATIVE(start, siz, is_desc)				nand_platform_invalidate_dcache_range((start), (siz), (is_desc))	
#define NAND_CLEAN_DCACHE_RANGE_NATIVE(start, siz, is_desc)						nand_platform_clean_dcache_range((start), (siz), (is_desc))

#define NAND_DMA_MAP_SINGLE(dev, buffer, size, direction)						nand_platform_dma_map_single((dev), (buffer), (size), (direction))

#define NAND_DMA_UNMAP_SINGLE(dev, buffer, size, direction)					    nand_platform_dma_unmap_single((dev), (buffer), (size), (direction))

#define NAND_DMA_SYNC_FOR_DEVICE(dev, bus_addr, size, direction)				nand_platform_dma_sync_single_for_device((dev), (bus_addr), (size), (direction))

#define NAND_DMA_SYNC_FOR_CPU(dev, bus_addr, size, direction)					nand_platform_dma_sync_single_for_cpu((dev), (bus_addr), (size), (direction))


#else

#if (NAND_RUN_OS_IS == NAND_OS_LINUX || NAND_RUN_OS_IS == NAND_OS_UCLINUX)

#define NAND_INVALIDATE_DCACHE_RANGE_NATIVE(start, siz, is_desc)		        gp_invalidate_dcache_range((start), (siz))
#define NAND_CLEAN_DCACHE_RANGE_NATIVE(start, siz, is_desc)					    gp_clean_dcache_range((start), (siz))

#define NAND_DMA_MAP_SINGLE(dev, buffer, size, direction)						dma_map_single((dev), (buffer), (size), (direction))

#define NAND_DMA_UNMAP_SINGLE(dev, buffer, size, direction)					    dma_unmap_single((dev), (buffer), (size), (direction))

#define NAND_DMA_SYNC_FOR_DEVICE(dev, bus_addr, size, direction)		        dma_sync_single_for_device((dev), (bus_addr), (size), (direction))

#define NAND_DMA_SYNC_FOR_CPU(dev, bus_addr, size, direction)				    dma_sync_single_for_cpu((dev), (bus_addr), (size), (direction))


#endif

#endif // else if NAND_OS_SUPPORT_CACHE_UTIL == 0

#if defined(NAND_DEBUG_SUPPORT_CACHE_UTIL)
void nand_platform_invalidate_dcache_range_debug(void *start, UINT32 siz, UINT8 is_desc, const char *file_name, UINT32 line, const char *func_name);
void nand_platform_clean_dcache_range_debug(void *start, UINT32 siz, UINT8 is_desc, const char *file_name, UINT32 line, const char *func_name);
								
#define NAND_INVALIDATE_DCACHE_RANGE(start, siz, is_desc)					    nand_platform_invalidate_dcache_range_debug((start), (siz), (is_desc), (__FILE__), (__LINE__), (__FUNCTION__))											
#define NAND_CLEAN_DCACHE_RANGE(start, siz, is_desc)							nand_platform_clean_dcache_range_debug((start), (siz), (is_desc), (__FILE__), (__LINE__), (__FUNCTION__)) 

#else
#define NAND_INVALIDATE_DCACHE_RANGE(start, siz, is_desc)					    NAND_INVALIDATE_DCACHE_RANGE_NATIVE(start, siz, is_desc)
#define NAND_CLEAN_DCACHE_RANGE(start, siz, is_desc)							NAND_CLEAN_DCACHE_RANGE_NATIVE(start, siz, is_desc)
#endif

//////////////////////////////////////////////////////////////////////////////////
#if (NAND_OS_SUPPORT_DELAY_UTIL == 0)
void nand_platform_ndelay(unsigned long nsecs);
void nand_platform_mdelay(unsigned long msecs);

#define NAND_NDELAY(nsecs)													    nand_platform_ndelay((nsecs))
#define NAND_MDELAY(msecs)														nand_platform_mdelay((msecs))
																	
#else

#if (NAND_RUN_OS_IS == NAND_OS_LINUX || NAND_RUN_OS_IS == NAND_OS_UCLINUX)
#define NAND_NDELAY(nsecs)							ndelay((nsecs))
#define NAND_MDELAY(msecs)						    mdelay((msecs))
#endif

#endif  // endif NAND_OS_SUPPORT_DELAY_UTIL == 0
unsigned long nand_platform_ms_get(void);

#if ((NAND_RUN_OS_IS == NAND_OS_LINUX || NAND_RUN_OS_IS == NAND_OS_UCLINUX) && (ASIC_NUM == ASIC_83000))
#if 1
typedef int (*gpStorageReadStart_t	)(UINT8 partitionType, unsigned int indexSector, unsigned int wLen, unsigned char *buf); //sector operation
typedef int (*gpStorageWriteStart_t	)(UINT8 partitionType, unsigned int indexSector, unsigned int wLen, unsigned char *buf); //sector operation
typedef int (*gpStorageFlush_t		)(UINT8 partition_kind, UINT16 which_partition);
typedef int (*gpStorageInforGet_t	)(UINT8 partitionType, UINT16 *which_kind, UINT16 *which_partition,  unsigned int *nrSector); //sector operation

typedef struct gpStorageForFastbootApiAddr_s {
	//gpStorageInit_t		 	gpStorageInit;			// init function before fastboot
	gpStorageReadStart_t	 gpStorageReadStart;
	gpStorageWriteStart_t	 gpStorageWriteStart;
	gpStorageFlush_t		 gpStorageFlush;
	gpStorageInforGet_t 	 gpStorageInforGet;
	UINT16 gpStorageWarpKindNum;
	UINT16 gpStorageWarpPartitionNum;
	UINT16 gpStorageQuickKindNum;
	UINT16 gpStorageQuickPartitionNum;
	UINT16 gpStorageHiberKindNum;
	UINT16 gpStorageHiberPartitionNum;
}gpStorageForFastbootApiAddr_t;
#endif
extern SINT32 nand_write_user_partition_by_sector(UINT8 partitionType, unsigned int indexSector, unsigned int wLen, unsigned char *buf);
extern SINT32 nand_read_user_partition_by_sector(UINT8 partitionType, unsigned int indexSector, unsigned int wLen, unsigned char *buf);
extern SINT32 nand_flush_user_partition(UINT8 partition_kind, UINT16 which_partition);
extern SINT32 nand_search_user_partition_by_userbyte( UINT16 user_byte, UINT16* which_kind, UINT16* which_partition, UINT32* sector_size);
#endif


#else // from #if (NAND_RUN_OS_IS != NAND_OS_BL ), Mason@2016-03-11

#include <common.h>
#include <malloc.h>
#include <asm/io.h>
#include <nand.h>
#include <linux/clk.h>
#include <linux/clkdev.h>
#include <asm/arch/regmap.h>


#define NAND_MEM_ALLOC(siz, flags)					malloc((siz))	
#define NAND_MEM_FREE(ptr)							    free((ptr))
#define NAND_MEMCPY                         memcpy

#define DIAG_INFO                           printf
#define printk                              printf
#define NAND_NDELAY                         mdelay

#ifndef UINT32
#define UINT32 unsigned int
#endif

#ifndef SINT32
#define SINT32 int
#endif


#ifndef UINT16
#define UINT16 unsigned short
#endif

#ifndef SINT16
#define SINT16 short
#endif


#ifndef UINT8
#define UINT8 unsigned char
#endif

#ifndef SINT8
#define SINT8 char
#endif


#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE  1
#endif


#ifndef NULL
#define NULL  0
#endif

#endif // end of #if (NAND_RUN_OS_IS != NAND_OS_BL )

#endif  // endif NAND_PLATFORM_H
