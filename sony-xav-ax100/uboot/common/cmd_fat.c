/*
 * (C) Copyright 2002
 * Richard Jones, rjones@nexus-tech.net
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

/*
 * Boot support
 */
#include <common.h>
#include <command.h>
#include <s_record.h>
#include <net.h>
#include <ata.h>
#include <part.h>
#include <fat.h>
#include <fs.h>

int do_fat_fsload (cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	return do_load(cmdtp, flag, argc, argv, FS_TYPE_FAT);
}


U_BOOT_CMD(
	fatload,	7,	0,	do_fat_fsload,
	"load binary file from a dos filesystem",
	"<interface> [<dev[:part]>]  <addr> <filename> [bytes [pos]]\n"
	"    - Load binary file 'filename' from 'dev' on 'interface'\n"
	"      to address 'addr' from dos filesystem.\n"
	"      'pos' gives the file position to start loading from.\n"
	"      If 'pos' is omitted, 0 is used. 'pos' requires 'bytes'.\n"
	"      'bytes' gives the size to load. If 'bytes' is 0 or omitted,\n"
	"      the load stops on end of file.\n"
	"      If either 'pos' or 'bytes' are not aligned to\n"
	"      ARCH_DMA_MINALIGN then a misaligned buffer warning will\n"
	"      be printed and performance will suffer for the load."
);

static int do_fat_ls(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	return do_ls(cmdtp, flag, argc, argv, FS_TYPE_FAT);
}

U_BOOT_CMD(
	fatls,	4,	1,	do_fat_ls,
	"list files in a directory (default /)",
	"<interface> [<dev[:part]>] [directory]\n"
	"    - list files from 'dev' on 'interface' in a 'directory'"
);

static int do_fat_fsinfo(cmd_tbl_t *cmdtp, int flag, int argc,
			 char * const argv[])
{
	int dev, part;
	block_dev_desc_t *dev_desc;
	disk_partition_t info;

	if (argc < 2) {
		printf("usage: fatinfo <interface> [<dev[:part]>]\n");
		return 0;
	}

	part = get_device_and_partition(argv[1], argv[2], &dev_desc, &info, 1);
	if (part < 0)
		return 1;

	dev = dev_desc->dev;
	if (fat_set_blk_dev(dev_desc, &info) != 0) {
		printf("\n** Unable to use %s %d:%d for fatinfo **\n",
			argv[1], dev, part);
		return 1;
	}
	return file_fat_detectfs();
}

U_BOOT_CMD(
	fatinfo,	3,	1,	do_fat_fsinfo,
	"print information about filesystem",
	"<interface> [<dev[:part]>]\n"
	"    - print information about filesystem from 'dev' on 'interface'"
);

#ifdef CONFIG_FAT_WRITE
static int do_fat_fswrite_fsappend(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[], int mode_append)
{
	long size;
	unsigned long addr;
	unsigned long count;
	block_dev_desc_t *dev_desc = NULL;
	disk_partition_t info;
	int dev = 0;
	int part = 1;

	if (argc < 5)
		return cmd_usage(cmdtp);

	part = get_device_and_partition(argv[1], argv[2], &dev_desc, &info, 1);
	if (part < 0)
		return 1;

	dev = dev_desc->dev;

	if (fat_set_blk_dev(dev_desc, &info) != 0) {
		printf("\n** Unable to use %s %d:%d for fatwrite **\n",
			argv[1], dev, part);
		return 1;
	}
	addr = simple_strtoul(argv[3], NULL, 16);
	count = simple_strtoul(argv[5], NULL, 16);

	if(mode_append)
		size = file_fat_append(argv[4], (void *)addr, count);
	else
		size = file_fat_write(argv[4], (void *)addr, count);

	if (size == -1) {
		printf("\n** Unable to write \"%s\" from %s %d:%d **\n",
			argv[4], argv[1], dev, part);
		return 1;
	}

	printf("%ld bytes written\n", size);

	return 0;
}

static int do_fat_fswrite(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	if (argc < 5)
		return cmd_usage(cmdtp);

	return do_fat_fswrite_fsappend(cmdtp, flag, argc, argv, 0);
}

U_BOOT_CMD(
	fatwrite,	6,	0,	do_fat_fswrite,
	"write file into a dos filesystem",
	"<interface> <dev[:part]> <addr> <filename> <bytes>\n"
	"    - write file 'filename' from the address 'addr' in RAM\n"
	"      to 'dev' on 'interface'"
);

static int do_fat_fsappend(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	if (argc < 5)
		return cmd_usage(cmdtp);

	return do_fat_fswrite_fsappend(cmdtp, flag, argc, argv, 1);
}

U_BOOT_CMD(
	fatappend,	6,	0,	do_fat_fsappend,
	"append data to a file (dos filesystem)",
	"<interface> <dev[:part]> <addr> <filename> <bytes>\n"
	"    - append file 'filename' from the address 'addr' in RAM\n"
	"      to 'dev' on 'interface'"
);


static int do_fat_mkfs(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	int dev;
	block_dev_desc_t *dev_desc = NULL;
	disk_partition_t info;
	int part = 1;

	if (argc < 3)
		return cmd_usage(cmdtp);

	part = get_device_and_partition(argv[1], argv[2], &dev_desc, &info, 1);
	if (part < 0)
		return 1;

	dev = dev_desc->dev;
	if (fat_set_blk_dev(dev_desc, &info) != 0) {
		printf("%s, fat_set_blk_dev() != 0\n", __func__);
	}

	return fat_mkfs_ff(dev_desc, &info);
}

U_BOOT_CMD(
	fatmkfs,	3,	0,	do_fat_mkfs,
	"Simplified mkfs.vfat",
	"<interface> <dev[:part]>\n"
	"    - create FAT file system."
);

static int do_fat_fdisk(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	int dev;
	block_dev_desc_t *dev_desc = NULL;
	disk_partition_t info;
	int part = 1;

	if (argc < 3)
		return cmd_usage(cmdtp);

	part = get_device_and_partition(argv[1], argv[2], &dev_desc, &info, 1);
	if (part < 0)
		return 1;

	dev = dev_desc->dev;
	if (fat_set_blk_dev(dev_desc, &info) != 0) {
		printf("%s, fat_set_blk_dev() != 0\n", __func__);
	}

#if 0	/* debug code */
	printf("if_type = %d\n",              dev_desc->if_type);
	printf("dev = %d\n",                  dev_desc->dev);
	printf("part_type = 0x%x\n",          dev_desc->part_type);
	printf("target = 0x%x\n",             dev_desc->target);
	printf("lun = 0x%x\n",                dev_desc->lun);
	printf("type = 0x%x\n",               dev_desc->type);
	printf("removable = 0x%x\n",          dev_desc->removable);
	printf("lba = 0x%x\n",          (u32)(dev_desc->lba));
	printf("blksz = 0x%x\n",        (u32)(dev_desc->blksz));
	printf("log2blksz = %d\n",            dev_desc->log2blksz);
	printf("vendor = %s\n",               dev_desc->vendor);
	printf("product = %s\n",              dev_desc->product);
	printf("revision = %s\n",             dev_desc->revision);
#endif

	return fat_fdisk_ff(dev_desc, &info);
}

U_BOOT_CMD(
	fatfdisk,	3,	0,	do_fat_fdisk,
	"Simplified fdisk",
	"<interface> <dev[:part]>\n"
	"    - create MBR with only one partition"
);

static int do_fat_mkdir(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	int dev;
	block_dev_desc_t *dev_desc = NULL;
	disk_partition_t info;
	int part = 1;

	if (argc < 4)
		return cmd_usage(cmdtp);

	part = get_device_and_partition(argv[1], argv[2], &dev_desc, &info, 1);
	if (part < 0)
		return 1;

	dev = dev_desc->dev;
	if (fat_set_blk_dev(dev_desc, &info) != 0) {
		printf("Error: %s, %d\n", __FILE__, __LINE__);
		return 1;
	}

	return fat_mkdir_ff(dev_desc, &info, argv[3]);
}

U_BOOT_CMD(
	fatmkdir,	4,	0,	do_fat_mkdir,
	"Simplified mkdir",
	"<interface> <dev[:part]> <subdir0/subdir1/...>\n"
	"    - create subdirectory"
);

#endif
