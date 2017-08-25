/*
 * (C) Copyright 2015
 * Sunplus Technology. <http://www.sunplus.com/>
 * sp_mon.h - Sunplus monitor
 *
 * This file is released under the GPL v2
 *
 * code shared with kernel's sp_mon.h
 */
#define BUF_SIZE	256

#if 0
#define DBG_PRN	dbg_printf
#else
#define	DBG_PRN(...)
#endif

/* Write 4 bytes to a specific register in a certain group */
void write_regs(unsigned int, unsigned int, unsigned int);
/* Print specific greoup of registers */
void prn_regs(unsigned int);
/* Dumps requested physical memory data */
int dumpPhysMem(unsigned int, unsigned int);
/* Write 4 bytes to a specific memory address */
int writeToMem(unsigned int, unsigned int);
