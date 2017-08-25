#!/bin/bash

#
# This script is to help print calculated config variables
#
# $1: output file name
#


if [ "$1" = "" ];then
	echo "$0: missed arg1!"
	exit 1
fi

# Use absolute path for output file
OUT_H="`pwd`/$1"

cd `dirname $0` > /dev/null

UCONF="../include/autoconf.mk"

if [ ! -f "$UCONF" ];then
	echo "not found $UCONF"
	exit
fi

# have "preprocessing" on the copy
UTMP="./autoconf.mk.tmp"
cp $UCONF $UTMP
# replace "=$" with "=" to do $((str))
sed -i 's/=\$/=/g' $UTMP
source "$UTMP" 2>/dev/null

# out_def_var - save "define xxxx xxxx" to OUT_H
# $1: variable
# $2: 0=save, 1=save and print
function out_def_var()
{
	str="$1"

	# sanity check
	if [ "${!str}" = "" ];then
		echo "Error: Not found config for $str in $UTMP"
		exit 1
	fi

	val=$((str))

	printf "#define $str    \t0x%08X // ($((val/1024/1024))MB~)\n" $val >> $OUT_H

	# tee it
	if [ "$2" = "1" ];then
		printf "#define $str    \t0x%08X // ($((val/1024/1024))MB~)\n" $val
	fi
}

echo "---------- Linux -----------"

INC_H=__INC_LINUX_MEM_H__

echo -e "// This file is generated automatically" > $OUT_H
echo -e "#ifndef $INC_H" >> $OUT_H
echo -e "#define $INC_H\n" >> $OUT_H

out_def_var CONFIG_SP_IOP_ADDR 1
out_def_var CONFIG_SP_IOP_SIZE 1
out_def_var CONFIG_SP_DISPLAY_ADDR 1
out_def_var CONFIG_SP_DISPLAY_SIZE 1
out_def_var CONFIG_SP_RTCFG_ADDR 1
out_def_var CONFIG_SP_RTCFG_SIZE 1
out_def_var CONFIG_SP_RTCFG_MAX  1
out_def_var CONFIG_SP_VD_ADDR    1
out_def_var CONFIG_SP_VD_SIZE    1
out_def_var CONFIG_BOOT_PARAM_ADDR 1
out_def_var CONFIG_SYS_LOAD_ADDR 1
out_def_var CONFIG_INITRD_ADDR 1
out_def_var CONFIG_SP_CHUNKMEM_ADDR 1
out_def_var CONFIG_SP_CHUNKMEM_SIZE 1

echo -e "\n#endif //$INC_H" >> $OUT_H

echo "Output to $OUT_H"

rm -f $UTMP
