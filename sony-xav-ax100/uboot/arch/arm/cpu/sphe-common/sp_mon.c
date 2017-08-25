/*
 * (C) Copyright 2015
 * Sunplus Technology. <http://www.sunplus.com/>
 * sp_mon.c - Sunplus monitor
 *
 * This file is released under the GPL v2
 *
 * code shared with kernel's sp_mon.c
 */
/* Linux : IS_LINUX == 1, uboot : IS_LINUX == 0 */
#ifndef IS_LINUX
#define IS_LINUX 0
#endif

#if IS_LINUX
#include <asm/memory.h> /* phys_to_virt */
#include <mach/hardware.h>
#include <mach/sp_mon.h>
#else /* u-boot */
#include <asm/arch/hardware.h>
#include <common.h>
#include "sp_mon.h"
#endif

#if IS_LINUX
extern void dbg_putc(char);
extern int dbg_printf(const char *, ...);
#define RGST_OFFSET		IO0_BASE
#else /* u-boot */
#define dbg_printf	printf
#define RGST_OFFSET		SPHE_DEVICE_BASE

#endif

#if 0
#define DBG_PRN	dbg_printf
#else
#define	DBG_PRN(...)
#endif

#define DUMP_CHARS_PER_LINE 16
/* Dump / Fill format */
#define MEM_FORMAT_BYTE		(sizeof(unsigned char))
#define MEM_FORMAT_WORD		(sizeof(unsigned short))
#define MEM_FORMAT_DWORD	(sizeof(unsigned int))

/* ============================= Functions related to executing command =================== */
/* getMemAddr is for compatibiliy with u-boot code */
/* Since linux memory addresses are remapped, convert the physical to logical address*/
static unsigned int getMemAddr(unsigned int physAddr)
{
#if IS_LINUX
	return (unsigned int) phys_to_virt(physAddr);
#else /* u-boot */
    return physAddr;
#endif
}

/* Reading memory data safely (Deal with 8, 16, 32 bit cases separately) */
static unsigned int safe8bitRead(unsigned int addr, unsigned char *data)
{
	*data = *((volatile unsigned char *)addr);
	return 1;
}

static unsigned int safe16bitRead(unsigned int addr, unsigned short *data)
{
	if ( addr & 0x1 ) {
		return 0;
	}
	
	*data = *((volatile unsigned short *)addr);
	return 1;
}

static unsigned int safe32bitRead(unsigned int addr, unsigned int *data)
{
	if ( addr & 0x3 ) {
		return 0;
	}
	
	*data = *((volatile unsigned int *)addr);
	return 1;
}

/* Write 4 bytes to a specific register in a certain group */
void write_regs(unsigned int regGroupNum, unsigned int regIndex, unsigned int value)
{
	unsigned int targetAddr = 0;

	targetAddr = RGST_OFFSET + (regGroupNum * 32 * 4) + regIndex * 4; /* unit: bytes */
	*((volatile unsigned int *) targetAddr) = value;

	dbg_printf("Write G%u.%2u = 0x%08x (%u)\n", regGroupNum, regIndex, value, value);
	return;
}

/* Print specific group of registers */
void prn_regs(unsigned int regGroupNum)
{
	unsigned int i = 0;
	unsigned int regBaseAddr = 0;

 	regBaseAddr = RGST_OFFSET + regGroupNum * 32 * 4; /* unit: bytes */
	for(i = 0; i < 32; i++)
		dbg_printf("Read G%u.%2u = 0x%08x (%u)\n", regGroupNum, i, *((volatile unsigned int *)(regBaseAddr)+i), *((volatile unsigned int *)(regBaseAddr)+i));
	return;
}

/* Dumps requested physical memory data */
int dumpPhysMem(unsigned int physAddr, unsigned int dumpSize)
{
	unsigned int dumpFmt = MEM_FORMAT_DWORD;
	unsigned int baseAddr = 0;
	unsigned char readMem[DUMP_CHARS_PER_LINE];
	/* Used to store read data from memory */
	unsigned char data_8;
	unsigned short data_16;
	unsigned int data_32;
	unsigned int offset = 0;

	baseAddr = getMemAddr(physAddr);

	DBG_PRN("phyAddr, baseAddr, dumpSize = 0x%08x, 0x%08x, %u", physAddr, baseAddr, dumpSize);
	while(offset < dumpSize) {

		/* Print the line offset label */
		if((offset % DUMP_CHARS_PER_LINE) == 0)
			dbg_printf("\n%08x:  ", physAddr + offset);

		switch ( dumpFmt ) {
		case MEM_FORMAT_BYTE:
			if (safe8bitRead(baseAddr + offset, &data_8)) {
				dbg_printf("%02x ", data_8);
				readMem[(offset % DUMP_CHARS_PER_LINE) + 0] = data_8;
			}
			else 
				return 0;
			break;

		case MEM_FORMAT_WORD:
				if (safe16bitRead(baseAddr + offset, &data_16)) {
					dbg_printf("%04x ", data_16);
					readMem[(offset % DUMP_CHARS_PER_LINE) + 0] = data_16;
				}
				else 
					return 0;
			break;

		case MEM_FORMAT_DWORD:
				if (safe32bitRead(baseAddr + offset, &data_32)) {
					dbg_printf("%08x ", data_32);
					readMem[(offset % DUMP_CHARS_PER_LINE) + 0] = data_32;
				}
				else 
					return 0;
			break;
		}
		offset = offset + dumpFmt;
	}
	dbg_printf("\n");
	return 1;
}

/* Write 4 bytes to a specific memory address */
int writeToMem(unsigned int physAddr, unsigned int value)
{
	unsigned int baseAddr = 0;
	baseAddr = getMemAddr(physAddr);

	*(volatile unsigned int *)(baseAddr & 0xFFFFFFFC) = value;
	return 1;
}
