#ifndef __GEMINI_CORE_H
#define __GEMINI_CORE_H

#define CFG_FWUPT_DIR       "/fwisp_dir"
#define CFG_FWUPT_BPART_NR  6   /* block partition */
#define CFG_FWUPT_SPART_NR  52  /* system partition */

#define CFG_ISPFWUPD_FILENAME "/ispfwupd.bin"
#define CFG_ISPCMDLI_FILENAME	"/ispcmdli.bin"
#define CFG_ISPINITR_FILENAME	"/ispinitr.bin"
#define CFG_ISPKERNE_FILENAME	"/ispkerne.bin"

/*
 * Clock APIs
 */
void clk_init(void);

#endif
