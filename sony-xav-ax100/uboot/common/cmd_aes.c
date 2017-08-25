/*
 * Copyright (C) 2014 Marek Vasut <marex@denx.de>
 *
 * Command for en/de-crypting block of memory with AES-128-CBC cipher.
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#if defined(CONFIG_AES) && defined(CONFIG_AES_FAST)
#error only one of CONFIG_AES or CONFIG_AES_FAST can be defined
#endif

#include <common.h>
#include <command.h>
#include <environment.h>

#if defined(CONFIG_AES)
#include <aes.h>
#elif defined(CONFIG_AES_FAST)
#include <aes_fast.h>
#endif

#include <malloc.h>
#include <asm/byteorder.h>
#include <linux/compiler.h>

DECLARE_GLOBAL_DATA_PTR;

/**
 * do_aes() - Handle the "aes" command-line command
 * @cmdtp:	Command data struct pointer
 * @flag:	Command flag
 * @argc:	Command-line argument count
 * @argv:	Array of command-line arguments
 *
 * Returns zero on success, CMD_RET_USAGE in case of misuse and negative
 * on error.
 */
static int do_aes(cmd_tbl_t *cmdtp, int flag, int argc, char *const argv[])
{
	uint32_t key_addr, src_addr, dst_addr, len;
	uint8_t *key_ptr, *src_ptr, *dst_ptr;
#if defined(CONFIG_AES)
	uint8_t key_exp[AES_EXPAND_KEY_LENGTH];
	uint32_t aes_blocks;
#elif defined(CONFIG_AES_FAST)
	uint8_t iv[BLOCKSZ];
	AesCtx ctx;
#endif
	int enc;

	if (argc != 6)
		return CMD_RET_USAGE;

	if (!strncmp(argv[1], "enc", 3))
		enc = 1;
	else if (!strncmp(argv[1], "dec", 3))
		enc = 0;
	else
		return CMD_RET_USAGE;

	key_addr = simple_strtoul(argv[2], NULL, 16);
	src_addr = simple_strtoul(argv[3], NULL, 16);
	dst_addr = simple_strtoul(argv[4], NULL, 16);
	len = simple_strtoul(argv[5], NULL, 16);

	key_ptr = (uint8_t *)key_addr;
	src_ptr = (uint8_t *)src_addr;
	dst_ptr = (uint8_t *)dst_addr;

#if defined(CONFIG_AES)
	/* First we expand the key. */
	aes_expand_key(key_ptr, key_exp);

	/* Calculate the number of AES blocks to encrypt. */
	aes_blocks = DIV_ROUND_UP(len, AES_KEY_LENGTH);

	if (enc)
		aes_cbc_encrypt_blocks(key_exp, src_ptr, dst_ptr, aes_blocks);
	else
		aes_cbc_decrypt_blocks(key_exp, src_ptr, dst_ptr, aes_blocks);
#elif defined(CONFIG_AES_FAST)
	memset(iv, 0x00, sizeof(iv));
	if( AesCtxIni(&ctx, iv, key_ptr, KEY128, CBC) < 0)
		printf("Error: %s, %d\n", __FILE__, __LINE__);

	if (enc) {
		if(AesEncrypt(&ctx, src_ptr, dst_ptr, len) < 0)
			printf("Error: %s, %d\n", __FILE__, __LINE__);
	} else {
		if(AesDecrypt(&ctx, src_ptr, dst_ptr, len) < 0)
			printf("Error: %s, %d\n", __FILE__, __LINE__);
	}
#endif
	return 0;
}

/***************************************************/
#ifdef CONFIG_SYS_LONGHELP
static char aes_help_text[] =
	"enc key src dst len - Encrypt block of data $len bytes long\n"
	"                          at address $src using a key at address\n"
	"                          $key and store the result at address\n"
	"                          $dst. The $len size must be multiple of\n"
	"                          16 bytes and $key must be 16 bytes long.\n"
	"aes dec key src dst len - Decrypt block of data $len bytes long\n"
	"                          at address $src using a key at address\n"
	"                          $key and store the result at address\n"
	"                          $dst. The $len size must be multiple of\n"
	"                          16 bytes and $key must be 16 bytes long.";
#endif

U_BOOT_CMD(
	aes, 6, 1, do_aes,
	"AES 128 CBC encryption",
	aes_help_text
);
