 /*
  * (C) Copyright 2015
  * Sunplus Technology. <http://www.sunplus.com/>
  *
  * SPDX-License-Identifier:     GPL-2.0+
  */
#include <common.h>
#include <command.h>
#include <linux/compiler.h>
#include "asm/arch-gemini/regmap.h"
#include "../arch/arm/cpu/sphe-common/sp_mon.h"

#if 0
#define DBG_PRN	dbg_printf
#else
#define	DBG_PRN(...)
#endif

#define ENTER		0x0D
#define SPACE		0x20
#define BCKSPACE	0x7F
#define	IS_DIGIT(c)		((c) >= '0' && (c) <= '9')
#define	IS_LETTER_LOW(c)	((c) >= 'a' && (c) <= 'z')
#define	IS_LETTER_UP(c)		((c) >= 'A' && (c) <= 'Z')
#define IS_HEX(c)		(((c) >= '0' && (c) <= '9') || ((c) >= 'a' && (c) <= 'f') || ((c) >= 'a' && (c) <= 'f'))
#define TO_INT(x)		((unsigned int)(x - '0'))
#define ASCII_PRINTABLE(c)	((c >= ' ') && (c <= '~'))
#define LTR_LOW_TO_INT(c)	((unsigned int)(c - 'a' + 10))
#define LTR_UP_TO_INT(c)	((unsigned int)(c - 'A' + 10))
#define DUMP_CHARS_PER_LINE 16
/* Dump / Fill format */
#define MEM_FORMAT_BYTE		(sizeof(unsigned char))
#define MEM_FORMAT_WORD		(sizeof(unsigned short))
#define MEM_FORMAT_DWORD	(sizeof(unsigned int))

/* Load Registers (by group)
 *
 * Syntax:
 *	lreg {group}
 */
static int do_reg_lreg(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	int	regGroupNum = 0;

	if (argc == 3) {

	} else if (argc == 2){
		regGroupNum = simple_strtoul(argv[1], NULL, 0);
		prn_regs(regGroupNum);
	} else
		return CMD_RET_USAGE;

	return 0;
}

/* Write to Register
 *
 * Syntax:
 *	wreg {group} {register index in group} {value}
 */
static int do_reg_wreg(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	if (argc == 4){
		unsigned int regGroupNum = 0, offset = 0, value = 0;

		regGroupNum = simple_strtoul(argv[1], NULL, 0);
		offset = simple_strtoul(argv[2], NULL, 0);
		value = simple_strtoul(argv[3], NULL, 0);

		write_regs(regGroupNum, offset, value);
	} else
		return CMD_RET_USAGE;
	return 0;
}

/* Dump 64 bytes of memory data
 *
 * Syntax:
 *	lw {physical address}
 */
 /* Reading memory data safely (Deal with 8, 16, 32 bit cases separately) */
static int do_mem_lw(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	if (argc == 2) {
		u64 physAddr = 0;

		physAddr = simple_strtoul(argv[1], NULL, 0);

		dumpPhysMem(physAddr, 64);
	} else
		return CMD_RET_USAGE;
	printf("\n");
	return 0;
}

/* Store 4 bytes (32 bits) to memory
 *
 * Syntax:
 *	sw {physical address} {4 bytes value}
 */
static int do_mem_sw(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	if (argc == 3) {
		unsigned long physAddr = 0, value = 0;

		physAddr = simple_strtoul(argv[1], NULL, 0);
		value = simple_strtoul(argv[2], NULL, 0);

		writeToMem(physAddr, value);
	} else
		return CMD_RET_USAGE;
	printf("\n");
	return 0;
}

/**************************************************/
U_BOOT_CMD(
	lreg,	2,	1,	do_reg_lreg,
	"Sunplus MON : load register (by group)",
	"[register group #]"
);

U_BOOT_CMD(
	wreg,	4,	1,	do_reg_wreg,
	"Sunplus MON : write 4 bytes to register",
	"[register group #] [register # in group] [value (4 bytes)]"
);

U_BOOT_CMD(
	lw,	2,	1,	do_mem_lw,
	"Sunplus MON : display 64 bytes from memory",
	"[physical address]"
);

U_BOOT_CMD(
	sw,	3,	1,	do_mem_sw,
	"Sunplus MON : store 4 bytes to memory",
	"[physical address] [value(4 bytes)]"
);
