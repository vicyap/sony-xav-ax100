/*  How to complie this code for user-mode application:
 *     ${CROSS_COMPILE_ARM}gcc -DCODE4USERMODE -static -o path/to/somewhere/kvdb path/to/this/file
 *     ${STRIP_ARM} path/to/somewhere/kvdb
 */


/*
 * (C) Copyright 2015
 * Sunplus Technology. <http://www.sunplus.com/>
 *
 * SPDX-License-Identifier:     GPL-2.0+
 */

#ifndef CODE4USERMODE
#define CODE4UBOOT
#endif

#if defined(CODE4UBOOT)

#include <common.h>
#include <linux/mtd/mtd.h>
#include <command.h>
#include <malloc.h>
#include <asm/byteorder.h>
#include <jffs2/jffs2.h>
#include <nand.h>

#elif defined(CODE4USERMODE)

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <mtd/mtd-abi.h>

#define CMD_RET_SUCCESS     (0)
#define CMD_RET_FAILURE     (1)
#define CMD_RET_USAGE       (-1)

typedef uint64_t    u64;
typedef uint32_t    u32;
typedef uint8_t     u8;

#endif

#if defined(CODE4UBOOT)
#define ADDR_FOR_TEST   (CONFIG_SP_ISP_ADDR)
#if !defined(CONFIG_SP_BCH_REPORT)
#error "CONFIG_SP_BCH_REPORT must be defined in boot/uboot/include/configs/gemini.h"
#endif
#elif defined(CODE4USERMODE)
// CONFIG_SP_NAND_IOCTL must be defined in linux/kernel/arch/arm/mach-gemini/sp_nand.h
#define SP_NAND_IOCREAD	_IOWR('C', 0, struct sp_nand_req)
#define SP_NAND_IOCWRITE	_IOWR('C', 1, struct sp_nand_req)

struct sp_nand_req {
    int page_addr;
    unsigned int ecc_sts;
    unsigned int max_err_bits;
    unsigned int total_err_bits;
};

#endif

#define PART_NAME       "kvdb"
#define KVDB_SIGNATURE  0x6264766b      // "kvdb"

/* Note:
 *    1. The design is for data size at several pages.
 *    2. First page of each good block must be either
 *         * A valid "struct kvdb_data" or
 *         * blank (erased block/page)
 *       => easier to find the latest valid data
 *    3. Payload can't be in two blocks because Note-2.
 */
struct kvdb_header {
    u32 signature;
    u32 sequence_num;
    u32 payload_length;
    u32 crc32;
};

/* Is 32-bit sequence_num enough?
 *   Assume the endurance of erase/write cycle is ((1 << 17) = 131,072).
 *   32-bit value is enough for counting 32768 pages: (1 << 32)/((1 << 17) = (1 << 15) = 32768
 *   For a 64 pages/block NAND, it's safe as long as this partition is less than 512 (32768/64) blocks.
 */

struct kvdb_data {
    struct kvdb_header header;
    u8 payload[2<<20];          // size doesn't matter here.
};

struct kvdb_info_s {
    u32 partition_addr_start;
    u32 partition_size;
    u32 partition_addr_end;
    u32 block_size;
    u32 page_size;
    u32 block_mask;
    u32 page_mask;
    u32 last_valid_addr;
    u32 last_valid_sequence_num;
    u32 last_valid_addr_end;        // it must be an end address of a page

#if defined(CODE4UBOOT)
    nand_info_t *nand;
#elif defined(CODE4USERMODE)
    char dev_mtdX[32];
#endif
} kvdb_info;

inline u32 align2page(u32 value)
{
    return ((value + kvdb_info.page_size - 1) & (kvdb_info.page_mask));
}

inline u32 align2block(u32 value)
{
    return ((value + kvdb_info.block_size - 1) & (kvdb_info.block_mask));
}

static int nand_block_isgood(u32 address)
{
#if defined(CODE4UBOOT)
    return (nand_block_isbad(kvdb_info.nand, (loff_t)(address)) ? 0 : 1);
#elif defined(CODE4USERMODE)
    int fd;
    int ret;
    loff_t test_ofs;

    fd = open(kvdb_info.dev_mtdX, O_RDONLY);
    if (fd < 0) {
        printf("Error: %s, %s, %d\n", __func__, __FILE__, __LINE__);
        exit(1);
    }

    // printf("%s, %s, %d, 0x%08x\n", __func__, __FILE__, __LINE__, address);
    test_ofs = (loff_t)(address - kvdb_info.partition_addr_start);
    ret = ioctl(fd, MEMGETBADBLOCK, &test_ofs);
    close(fd);
    if(ret == -1)
        perror("MEMGETBADBLOCK");
    else
        return ret ? 0 : 1;
#endif
}

static int nand_read_kvdb(void *ram_ptr, u32 nand_addr, u32 size)
{
#if defined(CODE4UBOOT)
    char cmd[256];
    snprintf(cmd, sizeof(cmd), "nand read 0x%x 0x%x 0x%x", (u32)(ram_ptr), nand_addr, size);
    run_command(cmd, 0);
    return 0;
#elif defined(CODE4USERMODE)
    int fd, ret;
    off_t seek;
    int rd;

    printf("%s: addr: 0x%x, size: 0x%x\n", __func__, nand_addr, size);

    fd = open(kvdb_info.dev_mtdX, O_RDONLY);
    if (fd < 0) {
        printf("Error: %s, %s, %d\n", __func__, __FILE__, __LINE__);
        return -1;
    }

    seek = (off_t)(nand_addr - kvdb_info.partition_addr_start);
    if (lseek(fd, seek, SEEK_SET) != seek) {
        close(fd);
        printf("Error: %s, %s, %d\n", __func__, __FILE__, __LINE__);
        return -1;
    }

    rd = 0;
    while (rd < size) {
        ret = read(fd, (void *)(ram_ptr + rd), (size - rd));
        if (ret < 0) {
            printf("Error: %s, %s, %d\n", __func__, __FILE__, __LINE__);
            close(fd);
            return -1;
        }
        rd += ret;
    }

    close(fd);
    return 0;
#endif
}

#if defined(CODE4UBOOT)
    extern u32 bch_status_for_kvdb;
#elif defined(CODE4USERMODE)
    // Nothing here.
#endif

static int nand_block_still_good(u32 nand_addr, u32 size)
{
    u32 page_num;
    u32 page_cnt;
    u32 i;
    u32 bch_err_detected, bch_capability;
    u32 bch_err_threshold;

#if defined(CODE4UBOOT)
    char cmd[256];
#elif defined(CODE4USERMODE)
    int fd_nand;
    struct sp_nand_req spnand;
#endif

    page_num = nand_addr / kvdb_info.page_size;
    page_cnt = size / kvdb_info.page_size;

    for(i=0; i<page_cnt; i++) {
        // printf("%s: Checking Page %u\n", __func__, (page_num+i));

#if defined(CODE4UBOOT)
        sprintf(cmd, "bch_sts 0x%x", (page_num+i));
        run_command(cmd, 0);
        // printf("bch_status_for_kvdb: 0x%08x\n", bch_status_for_kvdb);

        bch_capability   = bch_status_for_kvdb & 0x0000ffff;
        bch_err_detected = bch_status_for_kvdb >> 16;
#elif defined(CODE4USERMODE)
        fd_nand = open("/dev/sp_nand", O_RDWR);
        if(fd_nand < 0) {
            printf("Error: %s, %s, %d\n", __func__, __FILE__, __LINE__);
            exit(1);
        }

        spnand.page_addr = (int)(page_num+i);
        spnand.ecc_sts = 0xffffffff;
        spnand.max_err_bits = 0xffffffff;
        spnand.total_err_bits = 0xffffffff;
        ioctl(fd_nand, SP_NAND_IOCREAD, &spnand);
        // printf("UserSpace page #:%d\n", spnand.page_addr);
        // printf("UserSpace ecc_sts:0x%x, max_err=%d, total_err=%d\n", spnand.ecc_sts, spnand.max_err_bits, spnand.total_err_bits);
        bch_capability   = spnand.ecc_sts & 0x0000ffff;
        bch_err_detected = spnand.ecc_sts >> 16;
        close(fd_nand);
#endif

        bch_err_threshold = bch_capability - (bch_capability >> 2); // threshold: 75%
        if(bch_err_detected >= bch_err_threshold) {
            printf("%s: Block which includes Page %u will be marked as bad block.\n", __func__, (page_num+i));
            return 0;
        }
    }

#if defined(CODE4UBOOT)
    return 1;
#elif defined(CODE4USERMODE)
    return 1;
#endif
}

static int nand_markbad_kvdb(u32 nand_addr)
{
#if defined(CODE4UBOOT)
    char cmd[256];
    snprintf(cmd, sizeof(cmd), "nand markbad 0x%x", nand_addr);
    run_command(cmd, 0);
    return 0;
#elif defined(CODE4USERMODE)
    int fd, ret;
    loff_t ofs;

    printf("%s: addr: 0x%x\n", __func__, nand_addr);
    fd = open(kvdb_info.dev_mtdX, O_RDWR);
    if (fd < 0) {
        printf("Error: %s, %s, %d\n", __func__, __FILE__, __LINE__);
        return -1;
    }

    ofs  = nand_addr - kvdb_info.partition_addr_start;
    ret = ioctl(fd, MEMSETBADBLOCK, &ofs);

    if(ret < 0) {
        printf("Error: %s, %s, %d, errno = %d\n", __func__, __FILE__, __LINE__, errno);
    }

    return ret;
#endif
}

static int nand_erase_kvdb(u32 nand_addr, u32 size)
{
#if defined(CODE4UBOOT)
    char cmd[256];
    snprintf(cmd, sizeof(cmd), "nand erase 0x%x 0x%x", nand_addr, size);
    run_command(cmd, 0);
    return 0;
#elif defined(CODE4USERMODE)
    int fd, ret;
    struct erase_info_user erase_info;

    printf("%s: addr: 0x%x, size: 0x%x\n", __func__, nand_addr, size);
    fd = open(kvdb_info.dev_mtdX, O_RDWR);
    if (fd < 0) {
        printf("Error: %s, %s, %d\n", __func__, __FILE__, __LINE__);
        return -1;
    }

    erase_info.start  = nand_addr - kvdb_info.partition_addr_start;
    erase_info.length = size;

    ret = ioctl(fd, MEMERASE, &erase_info);

    if(ret < 0) {
        printf("Error: %s, %s, %d, errno = %d\n", __func__, __FILE__, __LINE__, errno);
    }

    return ret;
#endif
}

static int nand_write_kvdb(void *ram_ptr, u32 nand_addr, u32 size)
{
#if defined(CODE4UBOOT)
    char cmd[256];
    snprintf(cmd, sizeof(cmd), "nand write 0x%x 0x%x 0x%x", (u32)(ram_ptr), nand_addr, size);
    run_command(cmd, 0);
    return 0;
#elif defined(CODE4USERMODE)
    int fd;
    off_t seek;
    ssize_t ret;
    size_t write_size;

    printf("%s: addr: 0x%x, size: 0x%x\n", __func__, nand_addr, size);

    fd = open(kvdb_info.dev_mtdX, O_RDWR);
    if (fd < 0) {
        printf("Error: %s, %s, %d\n", __func__, __FILE__, __LINE__);
        return -1;
    }

    seek = (off_t)(nand_addr - kvdb_info.partition_addr_start);
    if (lseek(fd, seek, SEEK_SET) != seek) {
        close(fd);
        printf("Error: %s, %s, %d\n", __func__, __FILE__, __LINE__);
        return -1;
    }

    write_size = align2page(size);
    ret = write(fd, ram_ptr, write_size);
    if(ret < 0) {
        printf("Error: %s, %s, %d, ret = %d, errno = %d\n", __func__, __FILE__, __LINE__, ret, errno);
        close(fd);
        return -1;
    }
    else if((size_t)(ret) != write_size) {
        printf("Error: %s, %s, %d\n", __func__, __FILE__, __LINE__);
        close(fd);
        return -1;
    }

    close(fd);

    return 0;
#endif
}

static int mem_dump_kvdb(void *ram_ptr, u32 size)
{
#if defined(CODE4UBOOT)
    char cmd[256];
    snprintf(cmd, sizeof(cmd), "md.b 0x%x 0x%x", (u32)(ram_ptr), size);
    run_command(cmd, 0);
    return 0;
#elif defined(CODE4USERMODE)
    int i;
    u8 *ptr;

    printf("data dump:");
    ptr = (u8 *)(ram_ptr);
    for(i=0; i<size; i++) {
        if((i & 0xf) == 0)
            printf("\n");

        printf("%02x ", *ptr);
        ptr++;
    }
    printf("\n");
    return 0;
#endif
}

static int kvdb_init(void)
{
#if !defined(CONFIG_CMD_MTDPARTS) && defined(CODE4UBOOT)
    #error kvdb_init(), expect CONFIG_CMD_MTDPARTS defined
#else
    static int kvdb_init_done =0;

#if defined(CODE4UBOOT)
    struct mtd_device *dev;
    struct part_info *part;
    u8 pnum;
    int ret;
#elif defined(CODE4USERMODE)
    u32 i;
    FILE *fd;
    struct stat stat_file;
    char cmd[256], name[32];
    u32 partition_size;
#endif

    u32 addr, addr_begin, addr_end;
    struct kvdb_header *kvdb_header_ptr;

    if(kvdb_init_done)
        return 0;

    memset(&kvdb_info, 0, sizeof(kvdb_info));

#if defined(CODE4UBOOT)
    run_command("mtdparts", 0); // without running it, mtd info is not available.

    ret = find_dev_and_part(PART_NAME, &dev, &pnum, &part); // Ref: uboot/common/cmd_nand.c: get_part()
    if (ret) {
        printf("Can't find the partition: %s\n", PART_NAME);
        return ret;
    }

    kvdb_info.partition_addr_start  = (u32)(part->offset);
    kvdb_info.partition_size        = (u32)(part->size);
#elif defined(CODE4USERMODE)
    kvdb_info.partition_addr_start = 0;
    for(i=0; ;i++) {
        sprintf(cmd, "/sys/class/mtd/mtd%d", i);
        if(stat(cmd, &stat_file) == 0) {
            sprintf(cmd, "cat /sys/class/mtd/mtd%d/name", i);
            fd = popen(cmd, "r"); fscanf(fd, "%s", name); pclose(fd); printf("mtd%d, %s ", i, name);
            sprintf(cmd, "cat /sys/class/mtd/mtd%d/size", i);
            fd = popen(cmd, "r"); fscanf(fd, "%ul", &partition_size); pclose(fd); printf("0x%08x\n", partition_size);

            if(strncmp(name, PART_NAME, sizeof(name)) == 0) {
                sprintf(cmd, "cat /sys/class/mtd/mtd%d/erasesize", i);
                fd = popen(cmd, "r"); fscanf(fd, "%ul", &(kvdb_info.block_size)); pclose(fd);
                sprintf(cmd, "cat /sys/class/mtd/mtd%d/writesize", i);
                fd = popen(cmd, "r"); fscanf(fd, "%ul", &(kvdb_info.page_size)); pclose(fd);

                kvdb_info.partition_size = partition_size;
                // snprintf(kvdb_info.dev_mtdX, sizeof(kvdb_info.dev_mtdX), "/dev/mtd%d", i);
                snprintf(kvdb_info.dev_mtdX, sizeof(kvdb_info.dev_mtdX), "/dev/mtd%d", i);
                break;
            }
            kvdb_info.partition_addr_start += partition_size;
        }
        else {
            printf("Can't find the partition: %s\n", PART_NAME);
            break;
        }
    }
#endif
    kvdb_info.partition_addr_end    = kvdb_info.partition_addr_start + kvdb_info.partition_size -1;

    printf("Partition: 0x%08x - 0x%08x, Size: 0x%08x\n",
        kvdb_info.partition_addr_start,
        kvdb_info.partition_addr_end,
        kvdb_info.partition_size);

#if defined(CODE4UBOOT)
    kvdb_info.nand = &nand_info[dev->id->num];
    if(!kvdb_info.nand) {
        printf("error: %s, %d\n", __FILE__, __LINE__);
        return -1;
    }

    kvdb_info.block_size = kvdb_info.nand->erasesize;
    kvdb_info.page_size  = kvdb_info.nand->writesize;
#elif defined(CODE4USERMODE)
    // Already set kvdb_info.block_size and kvdb_info.page_size earlier.
#endif

    kvdb_info.block_mask = ~0 - (kvdb_info.block_size - 1);
    kvdb_info.page_mask  = ~0 - (kvdb_info.page_size - 1);
    printf("Block size: 0x%x, Page size: 0x%x\n", kvdb_info.block_size, kvdb_info.page_size);
    printf("Block mask: 0x%x, Page mask: 0x%x\n", kvdb_info.block_mask, kvdb_info.page_mask);

    kvdb_header_ptr = (struct kvdb_header *)malloc(kvdb_info.page_size);
    if(!kvdb_header_ptr) {
        printf("error: %s, %d\n", __FILE__, __LINE__);
        return -1;
    }

    while(kvdb_info.last_valid_addr == 0) {
        for(addr=kvdb_info.partition_addr_start; addr<=kvdb_info.partition_addr_end; addr+=kvdb_info.block_size) {
            if(!nand_block_isgood(addr)) {
                printf("Bad block at 0x%x\n", addr);
                continue;
            }

            nand_read_kvdb((void *)(kvdb_header_ptr), addr, sizeof(struct kvdb_header));
            if(kvdb_header_ptr->signature == KVDB_SIGNATURE) {
                printf("Find sequence_num: 0x%08x\n", kvdb_header_ptr->sequence_num);

                if( (kvdb_info.last_valid_addr == 0) ||
                   ((kvdb_info.last_valid_addr != 0) && (kvdb_info.last_valid_sequence_num < kvdb_header_ptr->sequence_num)) ) {
                    kvdb_info.last_valid_addr           = addr;
                    kvdb_info.last_valid_sequence_num   = kvdb_header_ptr->sequence_num;
                    kvdb_info.last_valid_addr_end       = align2page(kvdb_info.last_valid_addr + sizeof(struct kvdb_header) + kvdb_header_ptr->payload_length) - 1;
                }
            }
        }

        if(kvdb_info.last_valid_addr == 0) {
            printf("No valid key-value database found, init a blank one.\n");

            for(addr=kvdb_info.partition_addr_start; addr<=kvdb_info.partition_addr_end; addr+=kvdb_info.block_size) {
                if(nand_block_isgood(addr)) {
                    kvdb_header_ptr->signature      = KVDB_SIGNATURE;
                    kvdb_header_ptr->sequence_num   = 0;
                    kvdb_header_ptr->payload_length = 0;
                    kvdb_header_ptr->crc32          = 0;

                    nand_erase_kvdb(addr, kvdb_info.block_size);
                    nand_write_kvdb((void *)(kvdb_header_ptr), addr, sizeof(struct kvdb_header));

                    kvdb_info.last_valid_addr           = addr;
                    kvdb_info.last_valid_sequence_num   = 0;
                    kvdb_info.last_valid_addr_end       = align2page(kvdb_info.last_valid_addr + sizeof(struct kvdb_header) + kvdb_header_ptr->payload_length) - 1;
                    break;
                }
            }
        }
    }

    // find the block contains latest sequence_num already, need to find the page contains latest sequence_num:
    addr_begin = kvdb_info.last_valid_addr;
    addr_end   = kvdb_info.last_valid_addr + kvdb_info.block_size - 1;
    for(addr=addr_begin; addr<=addr_end; ) {
        nand_read_kvdb((void *)(kvdb_header_ptr), addr, sizeof(struct kvdb_header));
        if(kvdb_header_ptr->signature == KVDB_SIGNATURE) {
            printf("Find sequence_num: 0x%08x\n", kvdb_header_ptr->sequence_num);

            if(kvdb_info.last_valid_sequence_num < kvdb_header_ptr->sequence_num) {
                kvdb_info.last_valid_addr           = addr;
                kvdb_info.last_valid_sequence_num   = kvdb_header_ptr->sequence_num;
                kvdb_info.last_valid_addr_end       = align2page(kvdb_info.last_valid_addr + sizeof(struct kvdb_header) + kvdb_header_ptr->payload_length) - 1;
            }

            addr += align2page(kvdb_header_ptr->payload_length + sizeof(struct kvdb_header));
        }
        else {      /* (kvdb_header_ptr->signature != KVDB_SIGNATURE) */
            if( (kvdb_header_ptr->signature      != ~0) ||
                (kvdb_header_ptr->sequence_num   != ~0) ||
                (kvdb_header_ptr->payload_length != ~0) ||
                (kvdb_header_ptr->crc32          != ~0) ) {
                printf("%s, %d: Error, expect blank (ones) here \n", __FILE__, __LINE__);
                while(1);
            }

            break;
        }
    }
    printf("last_valid_addr: 0x%08x, last_valid_sequence_num: 0x%08x, last_valid_addr_end: 0x%08x\n", kvdb_info.last_valid_addr, kvdb_info.last_valid_sequence_num, kvdb_info.last_valid_addr_end);

    free(kvdb_header_ptr);
    kvdb_init_done = 1;
    return 0;
#endif
}

int kvdb_write(void *buf, u32 size)  // first sizeof(struct kvdb_header) should be reserved for this function
{
    struct kvdb_data *kvdb_data_ptr;

    kvdb_data_ptr = (struct kvdb_data *)(buf);
    kvdb_data_ptr->header.signature      = KVDB_SIGNATURE;
    kvdb_data_ptr->header.sequence_num   = ++kvdb_info.last_valid_sequence_num;
    kvdb_data_ptr->header.payload_length = size;
    kvdb_data_ptr->header.crc32          = 0x435243;    // not yet.

    if( ((kvdb_info.last_valid_addr_end + 1 + sizeof(struct kvdb_header) + kvdb_data_ptr->header.payload_length) & kvdb_info.block_mask) ==
        (kvdb_info.last_valid_addr & kvdb_info.block_mask) ) {  // data can fit into this block
        kvdb_info.last_valid_addr       = kvdb_info.last_valid_addr_end + 1;
        kvdb_info.last_valid_addr_end   = align2page(kvdb_info.last_valid_addr + sizeof(struct kvdb_header) + kvdb_data_ptr->header.payload_length) - 1;
    }
    else { /* Find next good block */
        kvdb_info.last_valid_addr       = align2block(kvdb_info.last_valid_addr_end + 1);   // this address must be start of a block.
        while(1) {
            while(1) {
                if( (nand_block_isgood(kvdb_info.last_valid_addr)) &&
                    (kvdb_info.last_valid_addr < kvdb_info.partition_addr_end) ) {
                    break;
                }

                kvdb_info.last_valid_addr      += kvdb_info.block_size;
                if(kvdb_info.last_valid_addr > kvdb_info.partition_addr_end) {
                    kvdb_info.last_valid_addr   = kvdb_info.partition_addr_start;
                    printf("%s, %d: Wrap around to the start of the partition\n", __FILE__, __LINE__);
                }
            }

            kvdb_info.last_valid_addr_end   = align2page(kvdb_info.last_valid_addr + sizeof(struct kvdb_header) + kvdb_data_ptr->header.payload_length) - 1;
            if(!nand_block_still_good(kvdb_info.last_valid_addr, kvdb_info.block_size)) {
                nand_markbad_kvdb(kvdb_info.last_valid_addr);
            } else {
                printf("Using new block: at 0x%x\n", kvdb_info.last_valid_addr);
                nand_erase_kvdb(kvdb_info.last_valid_addr, kvdb_info.block_size);
            }
            // Check again if this block is still a good block
            if(nand_block_isgood(kvdb_info.last_valid_addr))
                break;
            else
                printf("%s, %d: Block is bad\n", __FILE__, __LINE__);
        }
    }

    mem_dump_kvdb((void *)(kvdb_data_ptr), 0x80);
    nand_write_kvdb((void *)(kvdb_data_ptr), kvdb_info.last_valid_addr, (sizeof(struct kvdb_header) + kvdb_data_ptr->header.payload_length));

    return 0;
}

int kvdb_read(void *buf, u32 *size_ptr)
{
    // Note:
    //   Please disable SUPPORT_DOUBLE_BUFFER_PREFETCH in arch/arm/cpu/sphe-common/sp_nand.h,
    //   if you want to do something like following operations (It's a known bug of NAND driver):
    //   Run "kvdb wr_tst size" -> "kvdb read"

    struct kvdb_data *kvdb_data_ptr;

    kvdb_data_ptr = (struct kvdb_data *)(buf);

    nand_read_kvdb((void *)(kvdb_data_ptr), kvdb_info.last_valid_addr, sizeof(struct kvdb_header));
    if(kvdb_data_ptr->header.signature != KVDB_SIGNATURE) {
        printf("error: %s, %d\n", __FILE__, __LINE__);
        return -1;
    }

    nand_read_kvdb((void *)(kvdb_data_ptr), kvdb_info.last_valid_addr, (sizeof(struct kvdb_header) + kvdb_data_ptr->header.payload_length));
    return 0;
}

static int do_kvdb_wr_tst(u32 size)
{
    struct kvdb_data *kvdb_data_ptr;
    u32 *u32_ptr;
    u32 i;

    if(kvdb_init())
        return CMD_RET_FAILURE;

    printf("Write 0x%08x bytes of data ...\n", size);

#if defined(CODE4UBOOT)
    kvdb_data_ptr = (struct kvdb_data *)(ADDR_FOR_TEST);
#elif defined(CODE4USERMODE)
    kvdb_data_ptr = (struct kvdb_data *)(malloc(kvdb_info.block_size));
    if(!kvdb_data_ptr) {
        printf("Error: %s, %s, %d\n", __func__, __FILE__, __LINE__);
        return -1;
    }
#endif

    u32_ptr = (u32 *)(kvdb_data_ptr->payload);
    for(i=0; i<(size/sizeof(u32)); i++) {
        *u32_ptr = size;
        u32_ptr++;
    }
    kvdb_write((void *)(kvdb_data_ptr), size);

#if defined(CODE4UBOOT)
    // Nothing here.
#elif defined(CODE4USERMODE)
    free(kvdb_data_ptr);
#endif
    return CMD_RET_SUCCESS;
}

static int do_kvdb_read(void)
{
    u32 size;
    struct kvdb_data *kvdb_data_ptr;
    u8 *buf;
    u32 i;

    if(kvdb_init())
        return CMD_RET_FAILURE;

#if defined(CODE4UBOOT)
    buf = (u8 *)(ADDR_FOR_TEST);
#elif defined(CODE4USERMODE)
    buf = (u8 *)(malloc(kvdb_info.block_size));
    if(!buf) {
        printf("Error: %s, %s, %d\n", __func__, __FILE__, __LINE__);
        return -1;
    }
#endif

    if(kvdb_read((void *)(buf), &size) != 0) {
#if defined(CODE4UBOOT)
        // Nothing here.
#elif defined(CODE4USERMODE)
        free(buf);
#endif
        return CMD_RET_FAILURE;
    }

    kvdb_data_ptr = (struct kvdb_data *)(buf);
    printf("Seq. #: 0x%08x, payload length: 0x%x, Data: ",
        kvdb_data_ptr->header.sequence_num,
        kvdb_data_ptr->header.payload_length);
    for(i=0; i<0x10; i++) {
        if(i < kvdb_data_ptr->header.payload_length)
            printf("0x%02x ", kvdb_data_ptr->payload[i]);
    }
    printf("...\n");

#if defined(CODE4UBOOT)
    // Nothing here.
#elif defined(CODE4USERMODE)
    free(buf);
#endif
    return CMD_RET_SUCCESS;
}

static char kvdb_help_text[] =
    "sub_cmd ... - Key-Value database\n"
    "kvdb wr_tst size_in_hex\n"
    "kvdb read\n"
    "kvdb debug\n";

#if defined(CODE4UBOOT)
static int do_kvdb(cmd_tbl_t *cmdtp, int flag, int argc, char *const argv[])
#elif defined(CODE4USERMODE)
int main(int argc, char *argv[])
#endif
{
    char *cmd;
    cmd = argv[1];

#if defined(CODE4UBOOT)
    if(argc < 2)
        return CMD_RET_USAGE;
#elif defined(CODE4USERMODE)
    u32 size;

    if(argc < 2) {
        printf("%s %s", argv[0], kvdb_help_text);
        return CMD_RET_USAGE;
    }
#endif

    if(strncmp(cmd, "read", 4) == 0) {
        return do_kvdb_read();
    }

    if(strncmp(cmd, "debug", 5) == 0) {
        if(kvdb_init())
            return CMD_RET_FAILURE;

        printf("last_valid_addr: 0x%08x, last_valid_sequence_num: 0x%08x, last_valid_addr_end: 0x%08x\n",
            kvdb_info.last_valid_addr,
            kvdb_info.last_valid_sequence_num,
            kvdb_info.last_valid_addr_end);
        return CMD_RET_SUCCESS;
    }

    if(argc < 3)
        return CMD_RET_USAGE;

    if(strncmp(cmd, "wr_tst", 6) == 0) {
#if defined(CODE4UBOOT)
        return do_kvdb_wr_tst(simple_strtoul(argv[2], NULL, 16));
#elif defined(CODE4USERMODE)
        if(sscanf(argv[2], "0x%x", &size) == 1)
            return do_kvdb_wr_tst(size);
        else
            return CMD_RET_USAGE;
#endif
    }

    return CMD_RET_USAGE;
    // return CMD_RET_SUCCESS;
}


#if defined(CODE4UBOOT)
U_BOOT_CMD(
    kvdb, 4, 0, do_kvdb,
    "Sunplus Key-Value database", kvdb_help_text
);
#elif defined(CODE4USERMODE)
    // Nothing here.
#endif
