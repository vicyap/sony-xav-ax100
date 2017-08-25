/*
 * (C) Copyright 2014
 * Sunplus Technology
 * Kuo-Jung Su <dante.su@sunplus.com>
 *
 * SPDX-License-Identifier:     GPL-2.0+
 */

#ifndef __SP_CBDMA_H
#define __SP_CBDMA_H

int sp_cbdma_memset(dma_addr_t buf, int val, size_t len);
int sp_cbdma_memcpy(dma_addr_t src, dma_addr_t dst, size_t len);
int sp_cbdma_write(dma_addr_t src, dma_addr_t dst, size_t len);
int sp_cbdma_read(dma_addr_t src, dma_addr_t dst, size_t len);

#endif /* __SP_CBDMA_H */
