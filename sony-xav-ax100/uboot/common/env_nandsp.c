/*
 * Modified from env_nand.c
 *
 * uboot env for sp_nand is designed so that
 * 1. uboot env stored on nand can be scanned dynamically upon bootup
 * 2. can saveenv by specifying nand offset (for ISP)
 * 3. A redundant copy can be declared if it is needed
 * (must have mtd partition(s) pre-defined in u-boot env for saveenv to work)
 * (Ex: setenv mtdparts mtdparts=sp_nand.0:2m(nand_header),3m(env),3m(env_redund))
 *  meanings of related defined variables
 *   CONFIG_ENV_IS_IN_SP_NAND - define this and the below items to use this function
 * ENV_MAGICNUM & ENV_MAGICNUM_SIZE -  required when scanning nand for uboot env
 * ENV_BLK_SCAN_START - Specify the starting block # to start and search for uboot env
 * ENV_BLK_SCAN_END   - Specify the ending block # to end searching for uboot env
 *                      !!!The longer range to search for u-boot env on NAND, the longer
 *                         it may take to bootup system. For optimization, start & end blocks
 *                         should be tweaked to a range sufficient for searching uboot env partitions!!!
 * CONFIG_SP_ENV_SIZE - necessary because now S+'s u-boot env format is different from uboot's generic format
 * ENV_PART_NAME - mtd partition name declared for u-boot env partition
 * ENV_PART_NAME_REDUND - mtd partition name declared u-boot env partition (redundant copy)
 *                         (comment ENV_PART_NAME_REDUND to disable saving redundant copy of u-boot env)
 * !!!ENV_PARTITIONS MUST BE WITHIN THE ENV_BLK_SCAN_START & ENV_BLK_SCAN_END range!!!!
 *  a example definition for board configs is as below (should be placed in board config file)
 */
#if 0
#define CONFIG_ENV_IS_IN_SP_NAND
#if defined(CONFIG_ENV_IS_IN_SP_NAND) && defined(CONFIG_ENV_IS_NOWHERE)
# error "CONFIG_ENV_IS_IN_SP_NAND & CONFIG_ENV_IS_NOWHERE can't be defined at the same time"
#endif
#if defined(CONFIG_ENV_IS_IN_SP_NAND) && defined(CONFIG_SP_NAND)
# define CONFIG_CMD_SAVEENV	/* saveenv */
# define ENV_MAGICNUM		0xFACEFEED	/* magic number for checking if read data is uboot env */
# define ENV_MAGICNUM_SIZE	0x4		/* magic number size */
# define ENV_BLK_SCAN_START     0               /* starting block # to start and scan for uboot env */
# define ENV_BLK_SCAN_END	100		/* ending block # to scan for uboot env */
# define ENV_PART_NAME		"env"		/* mtd partition name for uboot env */
# define ENV_PART_NAME_REDUND	"env_redund"	/* mtd partition name for uboot env (redundant copy) */
# define CONFIG_SP_ENV_SIZE	(CONFIG_ENV_SIZE + ENV_MAGICNUM_SIZE)
#endif
#endif
/*
 * (C) Copyright 2000-2010
 * Wolfgang Denk, DENX Software Engineering, wd@denx.de.
 *
 * (C) Copyright 2008
 * Stuart Wood, Lab X Technologies <stuart.wood@labxtechnologies.com>
 *
 * (C) Copyright 2004
 * Jian Zhang, Texas Instruments, jzhang@ti.com.
 *
 * (C) Copyright 2001 Sysgo Real-Time Solutions, GmbH <www.elinos.com>
 * Andreas Heppel <aheppel@sysgo.de>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <command.h>
#include <environment.h>
#include <linux/stddef.h>
#include <malloc.h>
#include <nand.h>
#include <search.h>
#include <errno.h>
#ifdef CONFIG_SP_ENV_LOAD_BY_HINT
#include <sp_bblk.h>
#endif

#ifdef USE_HOSTCC
# include <ctype.h>
#else
#include <linux/ctype.h>
#endif

#if defined(CONFIG_CMD_SAVEENV) && defined(CONFIG_CMD_NAND)
#define CMD_SAVEENV
#endif

char *env_name_spec = "NAND";

env_t *env_ptr;

DECLARE_GLOBAL_DATA_PTR;

/*
 * This is called before nand_init() so we can't read NAND to
 * validate env data.
 *
 * Mark it OK for now. env_relocate() in env_common.c will call our
 * relocate function which does the real validation.
 *
 * When using a NAND boot image (like sequoia_nand), the environment
 * can be embedded or attached to the U-Boot image in NAND flash.
 * This way the SPL loads not only the U-Boot image from NAND but
 * also the environment.
 */
int env_init(void)
{

	gd->env_addr	= (ulong)&default_environment[0];
	gd->env_valid	= 1;

	return 0;
}

#ifdef CMD_SAVEENV
/*
 * The below code is executed when command "saveenv" is entered
 * saveenv is used when trying to save uboot environment variables to storage medium
 * 1. mtdparts (nand's partition layout) must be pre-declared (or else saveenv doesn't know
      where the target partition info (size, offset)
 * 2. mtd partition ENV_PART_NAME must exist
 * 3. mtd partition ENV_PART_NAME_REDUND must be defined and exists in mtd partition layout
 *    to store it successfully
 *
 */
int saveenv(void)
{
	ALLOC_CACHE_ALIGN_BUFFER(env_t, env_new, 1);
	ssize_t	len;
	char	*res;
	char cmd[256];
	char env_part[] = ENV_PART_NAME;
#ifdef ENV_PART_NAME_REDUND
	char env_part_redund[] = ENV_PART_NAME_REDUND;
#endif
	res = (char *)&env_new->data;
	len = hexport_r(&env_htab, '\0', 0, &res, ENV_SIZE, 0, NULL);
	if (len < 0) {
		error("Cannot export environment: errno = %d\n", errno);
		return 1;
	}

	env_new->magicnum   = ENV_MAGICNUM;
	env_new->crc   = crc32(0, env_new->data, ENV_SIZE);

	printf("Erasing partition %s...\n", env_part);
	sprintf(cmd, "nand erase.part %s", env_part);
	run_command(cmd, 0);

	printf("Writing...\n");
	sprintf(cmd, "nand write 0x%08x %s 0x%08x", (unsigned int)env_new, env_part, CONFIG_SP_ENV_SIZE);
	run_command(cmd, 0);

#ifdef ENV_PART_NAME_REDUND
	printf("Erasing partition %s...\n", env_part_redund);
	sprintf(cmd, "nand erase.part %s", env_part_redund);
	run_command(cmd, 0);

	printf("Writing...\n");
	sprintf(cmd, "nand write 0x%08x %s 0x%08x", (unsigned int)env_new, env_part_redund, CONFIG_SP_ENV_SIZE);
	run_command(cmd, 0);
#endif

	return 0;
}

#endif /* CMD_SAVEENV */

int readenv(size_t offset, u_char *buf)
{
	/* At this point (u-boot bootup), we don't know the exact partition size for saving uboot env,
	 * we have to pre-define a range to limit the amount of bytes read from NAND (to deal with boundary condition)
	 * 'end' is used for this exact purpose. In regular scenarios readenv should terminate successfully before
	 * triggering the described boundary condition. 'end' is set as the total NAND size
	 */
	uint64_t end = nand_info[0].size;
	size_t amount_loaded = 0;
	size_t blocksize, len;
	u_char *char_ptr;

	blocksize = nand_info[0].erasesize;
	if (!blocksize)
		return 1;

	len = min(blocksize, CONFIG_SP_ENV_SIZE);

	while (amount_loaded < (CONFIG_SP_ENV_SIZE) && offset < end) {
		if (nand_block_isbad(&nand_info[0], offset)) {
			offset += blocksize;
		} else {
			char_ptr = &buf[amount_loaded];
			if (nand_read_skip_bad(&nand_info[0], offset,
					       &len, NULL,
					       nand_info[0].size, char_ptr))
				return 1;

			offset += blocksize;
			amount_loaded += len;
		}
	}

	if (amount_loaded != (CONFIG_SP_ENV_SIZE))
		return 1;

	return 0;
}

/*
 * The below code is executed upon system bootup, it will try to
 * 1. scan for u-boot env starting from ENV_BLK_SCAN_START to ENV_BLK_SCAN_END
 * 2. check if ENV_MAGICNUM is correct, if it is then it is the target u-boot env data
 * 3. checks loaded uboot env's crc32 and see if it matches
 * 4. if something wrong happens or couldn't find uboot's env in the scanning process
 *    it will try to load uboot's default env defined in board's config file
 * Note: It took ~6xxms to scan for uboot env located on block 3 for Samsung k9gbg08u0b
 *       Most of the time was spent scanning the whole chip for bad blocks for the first time (600ms)
 */

#ifdef ENV_PART_NAME_REDUND
void env_relocate_spec(void)
{
	int read1_fail = 1, read2_fail = 1;
	int crc1_ok = 0, crc2_ok = 0;
	loff_t offset_block_num = ENV_BLK_SCAN_START, limit_block_num = ENV_BLK_SCAN_END;
	env_t *ep, *tmp_env1, *tmp_env2;
	loff_t hint_blk = -1;

	tmp_env1 = (env_t *)malloc(CONFIG_SP_ENV_SIZE);
	tmp_env2 = (env_t *)malloc(CONFIG_SP_ENV_SIZE);
	if (tmp_env1 == NULL || tmp_env2 == NULL) {
		puts("Can't allocate buffers for environment\n");
		set_default_env("!malloc() failed");
		goto done;
	}

	printf("Scanning for env...");
	debug("starting offset block #:%llu, limit_block_num:%llu\n", offset_block_num, limit_block_num);

#ifdef CONFIG_SP_ENV_LOAD_BY_HINT
	/* Load env by hint */
	if (SP_BHDR_IS_VALID()) {
		hint_blk = SP_GET_BHDR_UBOOT_ENV_HINT() / nand_info[0].erasesize;
		printf("hint(%lld)..", hint_blk);
	}
	if (hint_blk >= ENV_BLK_SCAN_START && hint_blk <= ENV_BLK_SCAN_END) {
		offset_block_num = hint_blk;
	} else {
		printf("no hint..");
		hint_blk = -1;
	}

fallback_scan:
#endif

	/* Scan for the 1st copy of U-Boot env */
	/* In a special case (where env partition was erased), env_redund will be read by this code snippet */
	while (offset_block_num <= limit_block_num) {
		debug("Block 0x%llx\n", offset_block_num);
		if (0 == readenv((offset_block_num * nand_info[0].erasesize), (u_char *) tmp_env1)) {
			debug("tmp_env1->magicnum = 0x%04x\n", tmp_env1->magicnum);

			/* magicnum matches, meaning a copy of env is found */
			if (tmp_env1->magicnum == ENV_MAGICNUM) {
				debug("Found env magicnum!\n");
				offset_block_num++; /* progress to the next offset NAND block (for reading second copy of env*/
				read1_fail = 0;
				break;
			}
		}

		offset_block_num++;
	}

	/* Scan for the 2nd copy of U-Boot env */
	while (offset_block_num <= limit_block_num) {
		debug("Block 0x%llx\n", offset_block_num);
		if (0 == readenv((offset_block_num * nand_info[0].erasesize), (u_char*) tmp_env2)) {
			debug("tmp_env2->magicnum = 0x%04x\n", tmp_env2->magicnum);

			/* magicnum matches, meaning a copy of env is found */
			if (tmp_env2->magicnum == ENV_MAGICNUM) {
				debug("Found env magicnum!\n");
				read2_fail = 0;
				break;
			}
		}

		offset_block_num++;
	}

#ifdef CONFIG_SP_ENV_LOAD_BY_HINT
	/* In case that the hint is incorrect (env block < hint_blk), fallback to scan!
	 * This case can be tested by erasing either env or env_redund.
	 */
	if (hint_blk != -1 && (read1_fail || read2_fail)) {
		printf("fallback scan...");
		hint_blk = -1;
		read1_fail = read2_fail = 1;
		offset_block_num = ENV_BLK_SCAN_START;
		goto fallback_scan;
	}
#endif

	/* Check crc for env's read from NAND */
	debug("Checking env CRC...\n");
	crc1_ok = !read1_fail &&
		 (crc32(0, tmp_env1->data, ENV_SIZE) == tmp_env1->crc);
	crc2_ok = !read2_fail &&
		 (crc32(0, tmp_env2->data, ENV_SIZE) == tmp_env2->crc);

	if (!crc1_ok && !crc2_ok) {
		set_default_env("!bad CRC");
		goto done;
	} else if (crc1_ok && !crc2_ok) {
		gd->env_valid = 1;
	} else if (!crc1_ok && crc2_ok) {
		gd->env_valid = 2;
	} else {
		/* both ok, use the 1st copy as env */
		gd->env_valid = 1;
	}

	debug("Importing env...\n");
	if (gd->env_valid == 1)
		ep = tmp_env1;
	else
		ep = tmp_env2;

	env_import((char *) ep, 0);

	/* After importing env, if a copy is broken, repair it by saving env again */
	if (!crc1_ok || !crc2_ok) {
		printf("\n\nWarning!! env CRC error!\n");
		printf("Repairing env...\n");
		saveenv();
	}

done:
	free(tmp_env1);
	free(tmp_env2);

	printf("done!\n");
	return;
}

int sp_get_nand_env(const char *key, char *keyValue)
{
	const size_t size = ENV_SIZE;
	const char sep = '\0';

	int ret = -1, i = 0;
	env_t *ep = NULL;
	char *data, *sp, *dp, *name, *value;

	int read1_fail = 0, read2_fail = 0;
	int crc1_ok = 0, crc2_ok = 0;
	loff_t offset_block_num = ENV_BLK_SCAN_START, limit_block_num = ENV_BLK_SCAN_END;
	env_t *tmp_env1, *tmp_env2;
	tmp_env1 = (env_t *)malloc(CONFIG_SP_ENV_SIZE);
	tmp_env2 = (env_t *)malloc(CONFIG_SP_ENV_SIZE);

	printf("sp_get_nand_mtdparts in, Scanning NAND for env...");

	/* Scan for the 1st copy of U-Boot env */
	/* In a special case (where env partition was erased), env_redund will be read by this code snippet */
	while (offset_block_num <= limit_block_num) {
		debug("Block 0x%llx\n", offset_block_num);
		read1_fail = readenv((offset_block_num * nand_info[0].erasesize), (u_char *) tmp_env1);

		if (read1_fail == 0) {
			debug("tmp_env1->magicnum = 0x%04x\n", tmp_env1->magicnum);

			/* magicnum matches, meaning a copy of env is found */
			if (tmp_env1->magicnum == ENV_MAGICNUM) {
				debug("Found env magicnum!\n");
				offset_block_num++; /* progress to the next offset NAND block (for reading second copy of env*/
				break;
			}
		}

		offset_block_num++;
	}

	/* Scan for the 2nd copy of U-Boot env */
	while (offset_block_num <= limit_block_num) {
		debug("Block 0x%llx\n", offset_block_num);
		read2_fail = readenv((offset_block_num * nand_info[0].erasesize), (u_char*)tmp_env2);

		if (read2_fail == 0) {
			debug("tmp_env2->magicnum = 0x%04x\n", tmp_env2->magicnum);

			/* magicnum matches, meaning a copy of env is found */
			if (tmp_env2->magicnum == ENV_MAGICNUM) {
				debug("Found env magicnum!\n");
				break;
			}
		}

		offset_block_num++;
	}

	/* Check crc for env's read from NAND */
	debug("Checking env CRC...\n");
	crc1_ok = !read1_fail &&
		 (crc32(0, tmp_env1->data, ENV_SIZE) == tmp_env1->crc);
	crc2_ok = !read2_fail &&
		 (crc32(0, tmp_env2->data, ENV_SIZE) == tmp_env2->crc);

	if (!crc1_ok && !crc2_ok) {
		free(tmp_env1);
		free(tmp_env2);
		return ret;
	} else if (crc1_ok && !crc2_ok) {
		ep = (env_t *)tmp_env1;
	} else if (!crc1_ok && crc2_ok) {
		ep = (env_t *)tmp_env2;
	} else {
		/* both ok, use the 1st copy as env */
		ep = (env_t *)tmp_env1;
	}

	printf("done!\n");

	/* we allocate new space to make sure we can write to the array */
	if ((data = malloc(size)) == NULL) {
		free(tmp_env1);
		free(tmp_env2);
		return 0;
	}
	memcpy(data, ep->data, size);
	dp = data;

	free(tmp_env1);
	free(tmp_env2);

	/* Parse environment; allow for '\0' and 'sep' as separators */
	do {
		ENTRY *rv;

		/* skip leading white space */
		while (isblank(*dp))
			++dp;

		/* skip comment lines */
		if (*dp == '#') {
			while (*dp && (*dp != sep))
				++dp;
			++dp;
			continue;
		}

		/* parse name */
		for (name = dp; *dp != '=' && *dp && *dp != sep; ++dp)
			;

		/* deal with "name" and "name=" entries (delete var) */
		if (*dp == '\0' || *(dp + 1) == '\0' ||
		    *dp == sep || *(dp + 1) == sep) {
			if (*dp == '=')
				*dp++ = '\0';
			*dp++ = '\0';	/* terminate name */

			continue;
		}
		*dp++ = '\0';	/* terminate name */

		/* parse value; deal with escapes */
		for (value = sp = dp; *dp && (*dp != sep); ++dp) {
			if ((*dp == '\\') && *(dp + 1))
				++dp;
			*sp++ = *dp;
		}
		*sp++ = '\0';	/* terminate value */
		++dp;

		if (*name == 0) {
			return 0;
		}

		/* enter into hash table */
		if(strcmp(key,name)==0)
		{
			printf("sp_get_nand_env, find the destination !\n");
			//printf("----> name: %s\n",name);
			//printf("===> value: %s\n",value);
			strcpy(keyValue,value);
			ret = 1;
			break;
		}
	} while ((dp < data + size) && *dp);	/* size check needed for text */

	free(data);

	return ret;
}

#else

void env_relocate_spec(void)
{
	int read1_fail = 1;
	int crc1_ok = 0;
	ALLOC_CACHE_ALIGN_BUFFER(char, tmp_env1, CONFIG_SP_ENV_SIZE);
	env_t *ep1 = (env_t *) tmp_env1;
	loff_t offset_block_num = ENV_BLK_SCAN_START, limit_block_num = ENV_BLK_SCAN_END;
	loff_t hint_blk = -1;

	printf("Scanning for env...");
	debug("starting offset block #:%llu, limit_block_num:%llu\n", offset_block_num, limit_block_num);

#ifdef CONFIG_SP_ENV_LOAD_BY_HINT
	/* Load env by hint first */
	if (SP_BHDR_IS_VALID()) {
		hint_blk = SP_GET_BHDR_UBOOT_ENV_HINT() / nand_info[0].erasesize;
		printf("hint(%lld)..", hint_blk);
	}
	if (hint_blk >= ENV_BLK_SCAN_START && hint_blk <= ENV_BLK_SCAN_END) {
		offset_block_num = hint_blk;
	} else {
		printf("no hint..");
		hint_blk = -1;
	}

fallback_scan:
#endif

	/* Scan for the 1st copy of U-Boot env */
	while (offset_block_num <= limit_block_num) {
		debug("Block 0x%llx\n", offset_block_num);
		if (0 == readenv((offset_block_num * nand_info[0].erasesize), (u_char *)tmp_env1)) {
			debug("ep1->magicnum = 0x%04x\n", ep1->magicnum);

			/* magicnum matches, meaning a copy of env is found */
			if (ep1->magicnum == ENV_MAGICNUM) {
				debug("Found env magicnum!\n");
				read1_fail = 0;
				break;
			}
		}

		offset_block_num++;
	}

#ifdef CONFIG_SP_ENV_LOAD_BY_HINT
	/* In case that the hint is incorrect (env block < hint_blk), fallback scan.
	 * This case can be tested by ++hin_blk initially.
	 */
	if (hint_blk != -1 && read1_fail) {
		printf("fallback scan...");
		hint_blk = -1;
		read1_fail = 1;
		offset_block_num = ENV_BLK_SCAN_START;
		goto fallback_scan;
	}
#endif

	/* Check CRC for env's read from NAND */
	debug("Checking env CRC...\n");
	crc1_ok = !read1_fail &&
		 (crc32(0, ep1->data, ENV_SIZE) == ep1->crc);

	debug("crc1_ok = %d\n", crc1_ok);

	if (!crc1_ok) {
		set_default_env("!bad CRC");
		return;
	} else
		gd->env_valid = 1;

	debug("Importing env...\n");
	env_import(tmp_env1, 0);

	printf("done!\n");

	return;
}

int sp_get_nand_env(const char *key, char *keyValue)
{
	const size_t size = ENV_SIZE;
	const char sep = '\0';

	int ret = -1, i = 0;
	env_t *ep = NULL;
	char *data, *sp, *dp, *name, *value;

	int read1_fail = 0;
	int crc1_ok = 0;
	ALLOC_CACHE_ALIGN_BUFFER(char, tmp_env1, CONFIG_SP_ENV_SIZE);
	env_t *ep1 = (env_t *)tmp_env1;
	loff_t offset_block_num = ENV_BLK_SCAN_START, limit_block_num = ENV_BLK_SCAN_END;

	printf("sp_get_nand_mtdparts in, Scanning NAND for env...");

	/* Scan for the 1st copy of U-Boot env */
	while (offset_block_num <= limit_block_num) {
		debug("Block 0x%llx\n", offset_block_num);
		read1_fail = readenv((offset_block_num * nand_info[0].erasesize), (u_char *) tmp_env1);

		if (read1_fail == 0) {
			debug("ep1->magicnum = 0x%04x\n", ep1->magicnum);

			/* magicnum matches, meaning a copy of env is found */
			if (ep1->magicnum == ENV_MAGICNUM) {
				debug("Found env magicnum!\n");
				break;
			}
		}

		offset_block_num++;
	}

	/* Check CRC for env's read from NAND */
	debug("Checking env CRC...\n");
	crc1_ok = !read1_fail &&
		 (crc32(0, ep1->data, ENV_SIZE) == ep1->crc);

	debug("crc1_ok = %d\n", crc1_ok);

	if (!crc1_ok) {
		return ret;
	}

	printf("done!\n");

	ep = (env_t *)tmp_env1;

	/* we allocate new space to make sure we can write to the array */
	if ((data = malloc(size)) == NULL) {
		return 0;
	}
	memcpy(data, ep->data, size);
	dp = data;
	/* Parse environment; allow for '\0' and 'sep' as separators */
	do {
		ENTRY *rv;

		/* skip leading white space */
		while (isblank(*dp))
			++dp;

		/* skip comment lines */
		if (*dp == '#') {
			while (*dp && (*dp != sep))
				++dp;
			++dp;
			continue;
		}

		/* parse name */
		for (name = dp; *dp != '=' && *dp && *dp != sep; ++dp)
			;

		/* deal with "name" and "name=" entries (delete var) */
		if (*dp == '\0' || *(dp + 1) == '\0' ||
		    *dp == sep || *(dp + 1) == sep) {
			if (*dp == '=')
				*dp++ = '\0';
			*dp++ = '\0';	/* terminate name */

			continue;
		}
		*dp++ = '\0';	/* terminate name */

		/* parse value; deal with escapes */
		for (value = sp = dp; *dp && (*dp != sep); ++dp) {
			if ((*dp == '\\') && *(dp + 1))
				++dp;
			*sp++ = *dp;
		}
		*sp++ = '\0';	/* terminate value */
		++dp;

		if (*name == 0) {
			return 0;
		}

		/* enter into hash table */
		if(strcmp(key,name)==0)
		{
			printf("sp_get_nand_env, find the destination !\n");
			//printf("----> name: %s\n",name);
			//printf("===> value: %s\n",value);
			strcpy(keyValue,value);
			ret = 1;
			break;
		}
	} while ((dp < data + size) && *dp);	/* size check needed for text */

	free(data);

	return ret;
}

#endif /* ENV_PART_NAME_REDUND */
