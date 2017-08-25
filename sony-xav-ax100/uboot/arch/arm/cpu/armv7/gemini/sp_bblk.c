#include <common.h>
#include <malloc.h>
#include <sp_bblk.h>
#include <asm/arch/regmap.h>
#include "tcpsum.h"
#include "../../sphe-common/sp_bch.h"
#include "../../sphe-common/sp_nand.h"
#ifdef CONFIG_SP_SPINAND
#include "../../sphe-common/sp_spinand.h"
#endif
#include <jffs2/load_kernel.h>    /* struct part_info */

extern loff_t g_nand_last_wr_offs; // defined in drivers/mtd/nand/nand_util.c

/*
 * Lookup sect_sz used by bootcode
 */
uint32_t sp_nand_lookup_bdata_sect_sz(nand_info_t *nand)
{
	uint32_t sz_sect = 0;

	switch (nand->writesize) {
		case 2048:
			sz_sect = 1 << 10;  /* 1024 + 128 ECC */
			break;
		case 4096:
			sz_sect = 3 << 10;  /* 3072 + 384 ECC */
			break;
		case 8192:
			sz_sect = 7 << 10;  /* 7168 + 896 ECC */
			break;
		case 16384:
		default:
			sz_sect = 14 << 10; /* 14336 + 1792 ECC */
			break;
	}

	return sz_sect;
}

static uint8_t sp_get_addr_cycle(struct nand_chip *nand)
{
	struct sp_nand_info *info = (struct sp_nand_info *)
		container_of(nand, struct sp_nand_info, nand);
#ifdef CONFIG_SP_SPINAND
	if (info->regs == CONFIG_SP_SPINAND_BASE) {
		struct sp_spinand_info *sinfo = (struct sp_spinand_info *)info;
		return (sinfo->cac + sinfo->rac);
	}
#endif
	return info->cac + info->rac;
}

static u64 get_mtd_env_off(void)
{
#ifdef CONFIG_CMD_MTDPARTS
	struct mtd_device *dev;
	struct part_info *part;
	u8 pnum;
	u64 off;
	if (find_dev_and_part(ENV_PART_NAME, &dev, &pnum, &part)) {
		printf("err: no env part!\n");
		return 0;
	}
	off = part->offset;
#ifdef ENV_PART_NAME_REDUND
	if (find_dev_and_part(ENV_PART_NAME_REDUND, &dev, &pnum, &part)) {
		printf("err: no env_redund part!\n");
		return 0;
	}
#endif
	return (off <= part->offset) ? off : part->offset;
#else
	return 0;
#endif
}

/*
 * sp_nand_compose_bhdr
 * mtd      - nand_info_t
 * hdr      - destination buffer (hdr)
 * xboot_sz - xboot image size (if 0, assume its size is 32KB)
 */
int sp_nand_compose_bhdr(nand_info_t *mtd, struct BootProfileHeader *hdr,
		uint32_t xboot_sz)
{
	struct nand_chip *nand = (struct nand_chip *)mtd->priv;
	uint32_t sz_sect = sp_nand_lookup_bdata_sect_sz(mtd);

	debug("%s xboot_sz=%u\n", __func__, xboot_sz);

	if (xboot_sz == 0) {
		xboot_sz = 32 *1024;
	}

	memset((void *)hdr, 0, sizeof(*hdr));

	// 0
	hdr->Signature     = BHDR_BEG_SIGNATURE;
	hdr->Length        = sizeof(*hdr);
	hdr->Version       = 0;
        //reserved12;

	// 16
	hdr->BchType       = 0; // 1K60
	hdr->addrCycle     = sp_get_addr_cycle(nand);
	hdr->ReduntSize    = mtd->oobsize;
	hdr->BlockNum      = mtd->size >> nand->phys_erase_shift;
	hdr->BadBlockNum   = 0; //TODO
	hdr->PagePerBlock  = mtd->erasesize / mtd->writesize;

	// 32
	hdr->PageSize      = mtd->writesize;
	//ACWriteTiming
	//ACWriteTiming
	//reserved44

	// 48
	hdr->xboot_copies  = 2;                                 // assume xboot has 2 copies
	hdr->xboot_pg_off  = hdr->PagePerBlock;                 // assume xboot is at block 1
	hdr->xboot_pg_cnt  = (xboot_sz  + sz_sect - 1) / sz_sect; // assume xboot size=32KB
	//reserved60

	//64
	hdr->uboot_env_off = get_mtd_env_off(); // u64 hint for "Scanning for env"
	//reserved72
	//reserved76

	//80
	//struct OptBootEntry16 opt_entry[10];

	// 240
	//reserved240
	//reserved244
	hdr->EndSignature  = BHDR_END_SIGNATURE;
	hdr->CheckSum      = tcpsum((u8 *)hdr, 252);

	// Print important information
	printf("BchType=%d addrCycle=%d ReduntSize=%d BlockNum=%d PagePerBlock=%d PageSize=%d\n"
		"xboot_copies=%d xboot_pg_off=%d xboot_pg_cnt=%d\n"
		"tcpsum=0x%08x\n",
		hdr->BchType, hdr->addrCycle, hdr->ReduntSize, hdr->BlockNum,
		hdr->PagePerBlock, hdr->PageSize,
		hdr->xboot_copies, hdr->xboot_pg_off, hdr->xboot_pg_cnt,
		hdr->CheckSum);

	BUILD_BUG_ON(sizeof(*hdr) != 256);

	return 0;
}

/*
 * Write a boot sect to a page
 * @nand: pointer to nand_info_t
 * @maxsize: write length limitation (depends on nand size)
 * @off: nand offset (must align to page size)
 * @sz_sect: sect size (page size - sect ecc size)
 * @buf: sect data and sect ecc to write where sect ecc is generated in this function.
 * @no_skip: don't skip bad block (for block 0 header)
 */
int sp_nand_write_bsect(nand_info_t *nand,
		loff_t maxsize, void *buf, uint32_t off, uint32_t sz_sect, int no_skip)
{
	int ret, res;
	size_t rwsize = nand->writesize;
	uint32_t i;

	//debug("%s: buf=0x%x off=0x%x sz_sect=%u\n", __func__, (uint32_t)buf, (uint32_t)off, sz_sect);

	if (off % nand->writesize) {
		printf("%s: offset must be aligend to page size\n", __func__);
		return -EINVAL;
	}

	// Append 1K60 ecc to buf. That is, buf[] = (data of sz_sect) + (its ecc).
	// Each 1K data has a 128 ecc.
	for (i = 0; i < sz_sect / 1024; i++) {
		res = sp_bch_encode_1024x60(buf + (i * 1024), buf + sz_sect + (i *128));
		if (res) {
			printf("%s: bch 1k60 failed at i=%u, res=%d\n", __func__, i, res);
		}
	}

	if (no_skip) {
		// Write buf to a page (even if it's in a bad block)
		ret = nand_write(nand, off, &rwsize, (u_char *)buf);
	} else {
		// Write buf to a page
		ret = nand_write_skip_bad(nand, off, &rwsize,
				NULL, maxsize,
				(u_char *)buf, 0);
	}

	if (ret) {
		printf("%s: sz_sect=%u off=%u err=%d\n", __func__, sz_sect, off, ret);
	}

	return ret;
}

/*
 * Read a boot sect from a page
 * @nand: pointer to nand_info_t
 * @maxsize: write length limitation (depends on nand size)
 * @off: nand offset (must align to page size)
 * @sz_sect: sect size (page size - sect ecc size)
 * @buf: sect data and sect ecc to write where sect ecc is generated in this function.
 * @no_skip: don't skip bad block (for block 0 header)
 */
int sp_nand_read_bsect(nand_info_t *nand,
		loff_t maxsize, void *buf, uint32_t off, uint32_t sz_sect, int no_skip, int *skipped)
{
	int ret, res;
	size_t rwsize = nand->writesize;
	uint32_t i;
	int max_skip_blk = 20;
	uint32_t org_off = off;

	*skipped = 0;

	//debug("%s: buf=0x%x off=0x%x sz_sect=%u\n", __func__, (uint32_t)buf, (uint32_t)off, sz_sect);

	if (off % nand->writesize) {
		printf("%s: offset must be aligend to page size\n", __func__);
		return -EINVAL;
	}

	if (no_skip) { // for block 0 header
		// Read to buf from a page (no skip bad)
		ret = nand_read(nand, off, &rwsize,(u_char *)buf);
	} else {
		// Skip bad blocks before read.
		// Otherwise, always read next good block's page 0
		while (max_skip_blk-- && 
			nand_block_isbad(nand, off & ~(nand->erasesize - 1))) {
			off += nand->erasesize; // next block
			(*skipped)++;
		}

		if (!max_skip_blk) {
			printf("too many bad block since off=0x%x\n", org_off);
			return -1;
		}

		// Read to buf from a page
		ret = nand_read_skip_bad(nand, off, &rwsize,
				NULL, maxsize, (u_char *)buf);
	}

	if (ret) {
		printf("%s: sz_sect=%u off=%u err=%d\n", __func__, sz_sect, off, ret);
	}

	// Decode 1K60 ecc in buf[] = (data of sz_sect) + (its ecc).
	// Each 1K data has a 128 ecc.
	for (i = 0; i < sz_sect / 1024; i++) {
		res = sp_bch_decode_1024x60(buf + (i * 1024), buf + sz_sect + (i *128));
		if (res) {
			printf("%s: bch decode 1k60 failed at i=%u, res=%d\n", __func__, i, res);
		}
	}

	return ret;
}


/*
 * Write boot blocks (1K60 ecc in data area)
 * @nand: pointer to nand_info_t
 * @off: nand offset (must align to block size)
 * @length: write length
 * @maxsize: write length limitation (depends on nand size)
 * @data: data to write
 * @is_hdr: is data boot header?
 */
int sp_nand_write_bblk(nand_info_t *nand, loff_t off, size_t *length,
		loff_t maxsize, u_char *data, int is_hdr)
{
	uint32_t pgnr = nand->erasesize / nand->writesize;
	uint32_t sect_sz;
	uint32_t nsect, copies, i;
	int res, ret = 0;
	u_char *buf;

	if (off % nand->erasesize) {
		printf("%s: offset must be aligned to block\n", __func__);
		return -EINVAL;
	}

	buf = malloc(nand->writesize);
	if (!buf) {
		printf("%s: can't malloc page\n", __func__);
		return -ENOMEM;
	}

	if (is_hdr) {
		// boot header block = pages of (1K + ecc)
		sect_sz = 1024;

		if (*length > sect_sz) {
			printf("%s: max bhdr length is %u\n", __func__, sect_sz);
			free(buf);
			return -EINVAL;
		}

		// buf = data + 0xff .... 0xff
		memcpy(buf, data, *length);
		memset(buf + *length, 0xff, nand->writesize - *length);

		printf("%s: write bhdr off=0x%x\n", __func__, (uint32_t)off);

		// write bhdr every 4 pages
		for (i = 0; i < pgnr; i += 4) {

			//TODO: write block 0 w/o skip bad

			// write a nand page
			//res = sp_nand_write_bsect(nand, maxsize, buf, off, sect_sz);
			res = sp_nand_write_bsect(nand, maxsize, buf, off, sect_sz, 1);
			if (res)
				ret = res;

			off += nand->writesize * 4;
		}
	} else {

		sect_sz = sp_nand_lookup_bdata_sect_sz(nand);
		if (!sect_sz) {
			free(buf);
			return -EINVAL;
		}

		// note: we allow bdata length > block size
		// that is, nsect can be > pgnr

		nsect = (*length + sect_sz - 1) / sect_sz;

		copies = (pgnr / nsect);
		if (copies == 0)
			copies = 1;

		printf("%s: write bblk off=0x%x nsect=%u copies=%u\n",
				__func__, (uint32_t)off, nsect, copies);

		while (copies--) {
			for (i = 0; i < nsect; i++) {
				// page = data[sect_sz] + 0xff-padding
				if (i == nsect - 1 && (*length % sect_sz)) { // last page?
					memcpy(buf, data + (i * sect_sz), (*length % sect_sz));
					memset(buf + (*length % sect_sz), 0xff, nand->writesize - (*length % sect_sz));
				} else  {
					memcpy(buf, data + (i * sect_sz), sect_sz);
					memset(buf + sect_sz, 0xff, nand->writesize - sect_sz);
				}

				g_nand_last_wr_offs = 0; // clear

				// write a nand page
				res = sp_nand_write_bsect(nand, maxsize, buf, off, sect_sz, 0);
				if (res)
					ret = res;

				if (!g_nand_last_wr_offs) {
					printf("warn: g_nand_last_wr_offs is unset! off=0x%llx\n", off);
				} else {
					off = g_nand_last_wr_offs;
				}

				off += nand->writesize;
				//debug("moved off=0x%llx\n", off);
			}
		}
	}

	//
	// Calculate next block offset for ISP
	//
	uint32_t noffs = off;
	if (noffs & (nand->erasesize - 1)) {
		// move to next block
		noffs = (off + nand->erasesize) & ~(nand->erasesize - 1);
	}
	debug("isp_addr_next=0x%x\n", noffs);
	setenv_hex("isp_addr_next", noffs);

	free(buf);

	return ret;
}

/*
 * Read boot block (1K60 ecc in data area)
 * @nand: pointer to nand_info_t
 * @off: nand offset (must align to block size)
 * @length: length to read
 * @maxsize: read length limitation (depends on nand size)
 * @data: destination buffer
 * @no_skip: don't skip bad block (for block 0 header)
 */
int sp_nand_read_bblk(nand_info_t *nand, loff_t off, size_t *length,
		loff_t maxsize, u_char *data, int no_skip)
{
	uint32_t sect_sz;
	uint32_t nsect, i;
	int res, ret = 0;
	u_char *buf;
	int skipped;

	if (off % nand->erasesize) {
		printf("%s: offset must be aligned to block\n", __func__);
		return -EINVAL;
	}

	buf = malloc(nand->writesize);
	if (!buf) {
		printf("%s: can't malloc page\n", __func__);
		return -ENOMEM;
	}

	sect_sz = sp_nand_lookup_bdata_sect_sz(nand);
	if (!sect_sz) {
		free(buf);
		return -EINVAL;
	}

	// note: we allow bdata length > block size
	// that is, nsect can be > pgnr

	nsect = (*length + sect_sz - 1) / sect_sz;

	printf("%s: read bblk off=0x%x nsect=%u no_skip=%d\n",
			__func__, (uint32_t)off, nsect, no_skip);

	for (i = 0; i < nsect; i++) {
		// read a nand page = Data[sect_sz] + Data[ecc]
		res = sp_nand_read_bsect(nand, maxsize, buf, off, sect_sz, no_skip, &skipped);

		// last page?
		if (i == nsect - 1 && (*length % sect_sz)) {
			memcpy(data + (i * sect_sz), buf, (*length % sect_sz));
		} else {
			memcpy(data + (i * sect_sz), buf, sect_sz);
		}

		if (res)
			ret = res;

		// skip bad block for next read
		if (off % nand->erasesize == 0 && skipped) {
			printf("skip %d\n", skipped);
			off += (skipped * nand->erasesize);
		}

		off += nand->writesize;
	}

	free(buf);

	return ret;
}

void sp_nand_info(nand_info_t *mtd)
{
	struct nand_chip *nand = (struct nand_chip *)mtd->priv;
	struct sp_nand_info *info = (struct sp_nand_info *)
		container_of(nand, struct sp_nand_info, nand);
	//debug("  mtd=%p nand=%p sp=%p\n", mtd, nand, info);
#ifdef CONFIG_SP_SPINAND
	if (info->regs == CONFIG_SP_SPINAND_BASE) {
		struct sp_spinand_info *sinfo = (struct sp_spinand_info *)info;
		printf("  cycle cac=%d rac=%d\n", sinfo->cac, sinfo->rac);
		return;
	}
#endif
	// parallel nand
	printf("  cycle cac=%d rac=%d\n", info->cac, info->rac);
}
