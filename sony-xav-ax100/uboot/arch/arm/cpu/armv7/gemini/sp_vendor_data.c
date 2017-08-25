/*
 * (C) Copyright 2015
 * Sunplus Technology. <http://www.sunplus.com/>
 *
 */

#include <common.h>
#include <exports.h>
#include "sp_vendor_data.h"

#define SP_VENDOR_DATA_PART_NAME          "vendordata"
#define SP_VENDOR_DATA_RESTORE_PART_NAME  "vd_restore"
#define SP_VENDOR_DATA_PART_SIZE          2048 //because alignment, user space must base page, uboot doesnot

#define SP_VENDOR_DATA_RESTORETAG_POS     0x7F //(SP_VDATA_RESTORETAG_POS + SP_VDATA_RESTORETAG_LEN - 1)
#define SP_VENDOR_DATA_RESTORETAG_DATA    0x7F

#define SP_VENDOR_DATA_LOAD_ADDR          CONFIG_SP_VD_ADDR
#define SP_VENDOR_DATA_RESTORE_LOAD_ADDR  (CONFIG_SP_VD_ADDR + SP_VENDOR_DATA_PART_SIZE)

#undef VD_TEST
#ifdef VD_TEST
void sp_vd_test();
#endif
//#define SP_VENDOR_DATA_INDEX_POS        0
//#define SP_VENDOR_DATA_INDEX_EMPTY      0x00
//#define SP_VENDOR_DATA_INDEX_INIT       0x01
//#define SP_VENDOR_DATA_INDEX_SETUP      0x02

static unsigned char *gVDPart = NULL;


static int sp_vd_write_partition(char* part_name);
static unsigned char* sp_vd_extract_raw(int pos, int len);

int sp_vd_check(unsigned char *data, int ret_nand){
    int ret = SP_VD_RET_FAILED;

    if(ret_nand != 0){
        printf("%s %s  %d, check data = 0x%x ret = %d \n", __FILE__, __FUNCTION__,__LINE__, data, ret_nand);
        return SP_VD_RET_FAILED;
    }

    if(!data){
        printf("%s %s  %d, null pointer data = 0x%x\n", __FILE__, __FUNCTION__,__LINE__, data);
        return SP_VD_RET_FAILED;
    }else{
        ret = data[SP_VENDOR_DATA_RESTORETAG_POS] == SP_VENDOR_DATA_RESTORETAG_DATA? SP_VD_RET_SUCCESS : SP_VD_RET_FAILED;
        if(ret == SP_VD_RET_FAILED)
            printf("%s %s  %d, pos = 0x%x data = 0x%x\n", __FILE__, __FUNCTION__,__LINE__, data[SP_VENDOR_DATA_RESTORETAG_POS], data);        	
        return ret;
    }
}

static int sp_vd_load_partition(void){
    char cmd[256] = {0};
    unsigned char *part = NULL, *backup = NULL;
    int ret = SP_VD_RET_FAILED;
    int ret_vd = 0, ret_restore = 0;

    if(gVDPart){
        return SP_VD_RET_SUCCESS;
    }

    part = SP_VENDOR_DATA_LOAD_ADDR;
    backup = SP_VENDOR_DATA_RESTORE_LOAD_ADDR;

    // Read vendordata partition from nand to ram
    sprintf(cmd, "nand read 0x%x %s 0x%x", part, SP_VENDOR_DATA_PART_NAME, SP_VENDOR_DATA_PART_SIZE);
    printf("%s\n", cmd);
    ret_vd = run_command(cmd, 0);

    sprintf(cmd, "nand read 0x%x %s 0x%x", backup, SP_VENDOR_DATA_RESTORE_PART_NAME, SP_VENDOR_DATA_PART_SIZE);
    printf("%s\n", cmd);
    ret_restore = run_command(cmd, 0);

    if(sp_vd_check(part, ret_vd) == SP_VD_RET_SUCCESS){
        gVDPart = part;
        if(sp_vd_check(backup, ret_restore) == SP_VD_RET_FAILED){
            //if there is only one can be read, vendordata still works
            sp_vd_write_partition(SP_VENDOR_DATA_RESTORE_PART_NAME);
        }
        ret = SP_VD_RET_SUCCESS;
    }else{
        if(sp_vd_check(backup, ret_restore) == SP_VD_RET_SUCCESS){
            gVDPart = backup;
            //if there is only one can be read, vendordata still works
            sp_vd_write_partition(SP_VENDOR_DATA_PART_NAME);
            ret = SP_VD_RET_SUCCESS;
        }else{
            ret = SP_VD_RET_FAILED;
        }
    }


    return ret;

}

static unsigned char* sp_vd_extract_raw(int pos, int len){
    if((pos + len) > SP_VENDOR_DATA_PART_SIZE){
        printf("%s %s %d Error: extract overflow %d, %d\n", __FILE__, __FUNCTION__,__LINE__, pos, len);
        return NULL;
    }

    if(sp_vd_load_partition() != SP_VD_RET_SUCCESS){
        printf("%s %s %d Error: init failed %d, %d\n", __FILE__, __FUNCTION__,__LINE__, pos, len);
        return NULL;
    }

    unsigned char* output = (gVDPart + pos);
    if(!strlen((char *)output)){
        printf("%s %s %d Warning: no data gVDPart = 0x%x %d %d\n", __FILE__, __FUNCTION__,__LINE__, gVDPart, pos, len);
        return NULL;
    }

    return output;
}

static int sp_vd_write_partition(char* part_name){
    char cmd[256] = {0};
    int ret = 0;

    if(!strlen(part_name)){
        printf("%s %s %d Error: part_name is empty\n", __FILE__, __FUNCTION__,__LINE__);
        return SP_VD_RET_FAILED;
    }

    //Erase partition
    sprintf(cmd, "nand erase.part %s", part_name);
    printf("%s\n", cmd);
    run_command(cmd, 0);

    memset(cmd, 0, sizeof(cmd));
    // Write vendordata partition from ram to nand
    sprintf(cmd, "nand write 0x%x %s 0x%x", (unsigned int)gVDPart, part_name, SP_VENDOR_DATA_PART_SIZE);
    printf("%s\n", cmd);
    ret = run_command(cmd, 0);

    if(ret == 0){
        return SP_VD_RET_SUCCESS;
    }else{
        printf("%s %s %d Error: value is NULL\n", __FILE__, __FUNCTION__,__LINE__);
        return SP_VD_RET_FAILED;
    }

}

static unsigned char* sp_vd_update_raw(int pos, int len, unsigned char* value){
    int ret = SP_VD_RET_FAILED;
    if((pos + len) > SP_VENDOR_DATA_PART_SIZE){
        printf("%s %s %d Error: update overflow \n", __FILE__, __FUNCTION__,__LINE__);
        return NULL;
    }

    if(sp_vd_load_partition() != SP_VD_RET_SUCCESS){
        printf("%s %s %d Error: init failed\n", __FILE__, __FUNCTION__,__LINE__);
        return NULL;
    }

    memcpy((gVDPart + pos), value, len);
    //only do vendordata error handler, because it is the main storage.
    ret = sp_vd_write_partition(SP_VENDOR_DATA_PART_NAME);
    sp_vd_write_partition(SP_VENDOR_DATA_RESTORE_PART_NAME);

    return (ret == SP_VD_RET_SUCCESS)? (gVDPart + pos):NULL;
}

int sp_vd_get_int(int pos, int len, int *value){
    unsigned char *output = NULL;	
    unsigned char *s = NULL;
    int sign = 0;
    int base = 10;

    if(!value){
        printf("%s %s %d Error: value is NULL\n", __FILE__, __FUNCTION__,__LINE__);
        return SP_VD_RET_FAILED;
    }

    output = sp_vd_extract_raw(pos, len);
    if(!output){
        printf("%s %s %d Error: read position failed pos = %d\n", __FILE__, __FUNCTION__,__LINE__, pos);
        return SP_VD_RET_FAILED;
    }

    s = malloc(len + 1);

    if(s){
        memset(s, 0, len);
        memcpy(s, output, len);

        if (s[0] == '-'){
            sign = 1;
            s++;
        }
         if (s[0] == '0' && s[1] == 'x'){
            base = 16;
            s += 2;
         }
         if (s[0] == '0' && s[1] == 'o'){
            base = 8;
            s += 2;
         }
         if (s[0] == '0' && s[1] == 'b'){
            base = 2;
            s += 2;
         }

         *value = simple_strtol((char *)s, NULL, base);
         if (sign)
            *value = - *value;

         free(s);
         return SP_VD_RET_SUCCESS;

    }

    free(s);
    return SP_VD_RET_FAILED;
}

int sp_vd_get_string(int pos, int len, char *value){
    unsigned char *s = NULL;

    if(!value){
        printf("%s %s %d Error: value is NULL\n", __FILE__, __FUNCTION__,__LINE__);
        return SP_VD_RET_FAILED;
    }

    s = sp_vd_extract_raw(pos, len);
    if(!s){
        printf("%s %s %d Error: read position failed pos = %d\n", __FILE__, __FUNCTION__,__LINE__, pos);
        return SP_VD_RET_FAILED;
    }

    memcpy(value, s, len);

    return SP_VD_RET_SUCCESS;
}


int sp_vd_get_bool(int pos, bool *value){
    unsigned char *s = NULL;

    if(!value){
        printf("%s %s %d Error: value is NULL\n", __FILE__, __FUNCTION__,__LINE__);
        return SP_VD_RET_FAILED;
    }

    s = sp_vd_extract_raw(pos, 1);
    if(!s){
        printf("%s %s %d Error: read position failed pos = %d\n", __FILE__, __FUNCTION__,__LINE__, pos);
        return SP_VD_RET_FAILED;
    }

    if(s[0] == 'y' || s[0] == 'Y'){
        *value = true;
    }else if(s[0] == 'n' || s[0] == 'N'){
        *value = false;
    }else{
        return SP_VD_RET_FAILED;
    }

    return SP_VD_RET_SUCCESS;
}

int sp_vd_write_raw(int pos, int len, unsigned char *value){
    unsigned char *s = NULL;

    if(!value){
        printf("%s %s %d Error: value is NULL\n", __FILE__, __FUNCTION__,__LINE__);
        return SP_VD_RET_FAILED;
    }

    s = sp_vd_update_raw(pos, len, value);
    if(!s){
        printf("%s %s %d Error: update position failed\n", __FILE__, __FUNCTION__,__LINE__);
        return SP_VD_RET_FAILED;
    }

    return SP_VD_RET_SUCCESS;
}

int sp_vd_write_flush(int pos, int len, unsigned char *value){
    unsigned char *s = NULL;

    if(!value){
        printf("%s %s %d Error: value is NULL\n", __FILE__, __FUNCTION__,__LINE__);
        return SP_VD_RET_FAILED;
    }

    gVDPart = NULL;
    s = sp_vd_update_raw(pos, len, value);
    if(!s){
        printf("%s %s %d Error: update position failed\n", __FILE__, __FUNCTION__,__LINE__);
        return SP_VD_RET_FAILED;
    }

    return SP_VD_RET_SUCCESS;
}

#ifdef VD_TEST
void sp_vd_test(){
    printf("sp_vd_test: start\n");
    //test case
    //read string: 0x7F is a rect
    //read int: 0x7F
    int pos1 = 0x7F, len1 = 1;
    char v1[1], ret;
    ret = sp_vd_get_string(pos1, len1, v1);
    printf("vd test: sp_vd_get_string value (%s = %c?) ret (%d = 0?)\n", v1, 0x7F, ret);

    int v2 = 0;
    ret = sp_vd_get_int(pos1, len1, &v2);
    printf("vd test: sp_vd_get_int value (0x%x = 0x7F?) ret (%d = 0?)\n", v2, 0x7F, ret);

}
#endif