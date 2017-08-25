/*
 * (C) Copyright 2015
 * Sunplus Technology. <http://www.sunplus.com/>
 *
 */

#ifndef __SP_VENDOR_DATA_H
#define __SP_VENDOR_DATA_H

#define SP_VD_RET_SUCCESS     0
#define SP_VD_RET_FAILED     (-1)


int sp_vd_get_int(int pos, int len, int *value);
int sp_vd_get_string(int pos, int len, char* value);
int sp_vd_get_bool(int pos, bool *value);
int sp_vd_write_raw(int pos, int len, unsigned char *value);
int sp_vd_write_flush(int pos, int len, unsigned char *value);

#define SP_VDATA_START          0
#define SP_VDATA_LANG_POS       SP_VDATA_START
#define SP_VDATA_LANG_LEN       32
#define SP_VDATA_REVERSE_POS    (SP_VDATA_LANG_POS + SP_VDATA_LANG_LEN)
#define SP_VDATA_REVERSE_LEN    64
#define SP_VDATA_REVMIRROR_POS	(SP_VDATA_REVERSE_POS + SP_VDATA_REVERSE_LEN)
#define SP_VDATA_REVMIRROR_LEN 	4
#define SP_VDATA_VINMIRROR_POS	(SP_VDATA_REVMIRROR_POS + SP_VDATA_REVMIRROR_LEN)
#define SP_VDATA_VINMIRROR_LEN 	4

#define SP_VDATA_LOCALE_POS                 (SP_VDATA_VINMIRROR_POS + SP_VDATA_VINMIRROR_LEN)
#define SP_VDATA_LOCALE_LEN                 20
#define SP_VDATA_RESTORETAG_POS             (SP_VDATA_LOCALE_POS + SP_VDATA_LOCALE_POS)
#define SP_VDATA_RESTORETAG_LEN             4  //124-127, 0x7F

#define SP_VENDOR_DATA_SDK_MAX              255

#define SP_VENDOR_DATA_CUSTOMER_START       (SP_VENDOR_DATA_SDK_MAX + 1)

#endif
