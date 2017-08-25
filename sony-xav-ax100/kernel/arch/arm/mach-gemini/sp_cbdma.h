/*
 * (C) Copyright 2014
 * Sunplus Technology
 * Kuo-Jung Su <dante.su@sunplus.com>
 *
 * SPDX-License-Identifier:     GPL-2.0+
 */

#ifndef __SP_CBDMA_H
#define __SP_CBDMA_H

int sp_cbdma_memset(struct device *dev,
		dma_addr_t s, int c, size_t n);

int sp_cbdma_write(struct device *dev,
		dma_addr_t src, dma_addr_t dst, size_t len);

int sp_cbdma_read(struct device *dev,
		dma_addr_t src, dma_addr_t dst, size_t len);

int sp_cbdma_memcpy(struct device *dev,
		dma_addr_t src, dma_addr_t dst, size_t len);

struct device *sp_cbdma_getbyname(const char *name);

#endif /* __SP_CBDMA_H */
