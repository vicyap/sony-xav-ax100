#! /usr/bin/env bash


######################################
ARMWORKHOME=${PWD}
INSBINPATHFAKE=${ARMWORKHOME}/install/bin/
MKJOBOPT=-j8
HOSTGNUPREFIX=i686-pc-linux-gnu
HOSTGNUPREFIXDASH=
HOSTGNUPATH=/usr
PRETOOLCHAINPATH=`which arm-none-linux-gnueabi-gcc` 
PRETOOLCHAINPATH=`dirname $PRETOOLCHAINPATH` 
INSBINPATHFAKE=
INCLUDEPATH=${PRETOOLCHAINPATH}/../arm-none-linux-gnueabi/libc/usr/include
######################################

echo WORKPATH=$ARMWORKHOME
echo TOOLCHAINPATH=$PRETOOLCHAINPATH

HOSTGNUBINPATH=${HOSTGNUPATH}/bin/
TARSRCS=false
DISNOTINPKG=true
DISNOTNEEDPKG=true
DISTOOLCHAIN=true
BUILD_MINGW32=false
DISLIBC12=true
DISLIBCLOC=true
DISBINUTILS=true
DISFINSTALL=true
ENNONEEDPKD=false
TMPDIS=false

mkdir -p ${ARMWORKHOME}/src
if [ ! -d "${ARMWORKHOME}/src/glibc-2.11-2010q1" ]; then
cd ${ARMWORKHOME}/src
tar xjf ${ARMWORKHOME}/glibc-2010q1-202.tar.bz2
echo "extract package <glibc-2010q1-202.tar.bz2> to ${ARMWORKHOME}/src/"
cd -
fi

if [ ! -d "${ARMWORKHOME}/src/glibc_ports-2010q1" ]; then
cd ${ARMWORKHOME}/src
tar xjf ${ARMWORKHOME}/glibc_ports-2010q1-202.tar.bz2
echo "extract package <glibc_ports-2010q1-202.tar.bz2> to ${ARMWORKHOME}/src/"
cd -
fi

set -e
inform_fd=2 
umask 022
exec < /dev/null

error_handler ()
{
    exit 1
}

check_status() {
    local status="$?"
    if [ "$status" -ne 0 ]; then
	error_handler
    fi
}

check_pipe() {
    local -a status=("${PIPESTATUS[@]}")
    local limit=$1
    local ix
    
    if [ -z "$limit" ] ; then
	limit="${#status[@]}"
    fi
    for ((ix=0; ix != $limit ; ix++)); do
	if [ "${status[$ix]}" != "0" ] ; then
	    error_handler
	fi
    done
}

error () {
    echo "$script: error: $@" >& $inform_fd
    exit 1
}

warning () {
    echo "$script: warning: $@" >& $inform_fd
}

verbose () {
    if $gnu_verbose; then
	echo "$script: $@" >& $inform_fd
    fi
}

copy_dir() {
    mkdir -p "$2"

    (cd "$1" && tar cf - .) | (cd "$2" && tar xf -)
    check_pipe
}

copy_dir_clean() {
    mkdir -p "$2"
    (cd "$1" && tar cf - \
	--exclude=CVS --exclude=.svn --exclude=.git --exclude=.pc \
	--exclude="*~" --exclude=".#*" \
	--exclude="*.orig" --exclude="*.rej" \
	.) | (cd "$2" && tar xf -)
    check_pipe
}

update_dir_clean() {
    mkdir -p "$2"


    (cd "$1" && tar cf - \
	--exclude=CVS --exclude=.svn --exclude=.git --exclude=.pc \
	--exclude="*~" --exclude=".#*" \
	--exclude="*.orig" --exclude="*.rej" \
	--after-date="$3" \
	. 2> /dev/null) | (cd "$2" && tar xf -)
    check_pipe
}

copy_dir_exclude() {
    local source="$1"
    local dest="$2"
    local excl="$3"
    shift 3
    mkdir -p "$dest"
    (cd "$source" && tar cfX - "$excl" "$@") | (cd "$dest" && tar xf -)
    check_pipe
}

copy_dir_only() {
    local source="$1"
    local dest="$2"
    shift 2
    mkdir -p "$dest"
    (cd "$source" && tar cf - "$@") | (cd "$dest" && tar xf -)
    check_pipe
}

clean_environment() {
    local env_var_list
    local var




    unset BASH_ENV CDPATH POSIXLY_CORRECT TMOUT

    env_var_list=$(export | \
	grep '^declare -x ' | \
	sed -e 's/^declare -x //' -e 's/=.*//')

    for var in $env_var_list; do
	case $var in
	    HOME|HOSTNAME|LOGNAME|PWD|SHELL|SHLVL|SSH_*|TERM|USER)


		;;
	    LD_LIBRARY_PATH|PATH| \
		FLEXLM_NO_CKOUT_INSTALL_LIC|LM_APP_DISABLE_CACHE_READ)


		;;
	    MAKEINFO)

		;;
	    *_LICENSE_FILE)












		if [ "" ]; then
		    local license_file_envvar
		    license_file_envvar=

		    if [ "$var" != "$license_file_envvar" ]; then
			export -n "$var" || true
		    fi
		else
		    export -n "$var" || true
		fi
		;;
	    *)

		export -n "$var" || true
		;;
	esac
    done


    export LANG=C
    export LC_ALL=C


    export CVS_RSH=ssh



    user_shell=$SHELL
    export SHELL=$BASH
    export CONFIG_SHELL=$BASH
}

pushenv() {
    pushenv_level=$(($pushenv_level + 1))
    eval pushenv_vars_${pushenv_level}=
}


pushenv_level=0
pushenv_vars_0=



pushenvvar() {
    local pushenv_var="$1"
    local pushenv_newval="$2"
    eval local pushenv_oldval=\"\$$pushenv_var\"
    eval local pushenv_oldset=\"\${$pushenv_var+set}\"
    local pushenv_save_var=saved_${pushenv_level}_${pushenv_var}
    local pushenv_savep_var=savedp_${pushenv_level}_${pushenv_var}
    eval local pushenv_save_set=\"\${$pushenv_savep_var+set}\"
    if [ "$pushenv_save_set" = "set" ]; then
	error "Pushing $pushenv_var more than once at level $pushenv_level"
    fi
    if [ "$pushenv_oldset" = "set" ]; then
	eval $pushenv_save_var=\"\$pushenv_oldval\"
    else
	unset $pushenv_save_var
    fi
    eval $pushenv_savep_var=1
    eval export $pushenv_var=\"\$pushenv_newval\"
    local pushenv_list_var=pushenv_vars_${pushenv_level}
    eval $pushenv_list_var=\"\$$pushenv_list_var \$pushenv_var\"
}

prependenvvar() {
    local pushenv_var="$1"
    local pushenv_newval="$2"
    eval local pushenv_oldval=\"\$$pushenv_var\"
    pushenvvar "$pushenv_var" "$pushenv_newval$pushenv_oldval"
}

popenv() {
    local pushenv_var=
    eval local pushenv_vars=\"\$pushenv_vars_${pushenv_level}\"
    for pushenv_var in $pushenv_vars; do
	local pushenv_save_var=saved_${pushenv_level}_${pushenv_var}
	local pushenv_savep_var=savedp_${pushenv_level}_${pushenv_var}
	eval local pushenv_save_val=\"\$$pushenv_save_var\"
	eval local pushenv_save_set=\"\${$pushenv_save_var+set}\"
	unset $pushenv_save_var
	unset $pushenv_savep_var
	if [ "$pushenv_save_set" = "set" ]; then
	    eval export $pushenv_var=\"\$pushenv_save_val\"
	else
	    unset $pushenv_var
	fi
    done
    unset pushenv_vars_${pushenv_level}
    if [ "$pushenv_level" = "0" ]; then
	error "Popping environment level 0"
    else
	pushenv_level=$(($pushenv_level - 1))
    fi
}

prepend_path() {
    if $(eval "test -n \"\$$1\""); then
	prependenvvar "$1" "$2:"
    else
	prependenvvar "$1" "$2"
    fi
}
pushenvvar CSL_SCRIPTDIR ${ARMWORKHOME}/src/scripts-trunk
pushenvvar PATH $HOSTGNUPATH/bin:/bin:$PRETOOLCHAINPATH
pushenvvar LD_LIBRARY_PATH $HOSTGNUPATH/$HOSTGNUPREFIX/lib:$HOSTGNUPATH/lib64:$HOSTGNUPATH/lib
pushenvvar MAKEINFO 'makeinfo --css-ref=../cs.css'
clean_environment

if [ $TMPDIS != true ]; then
# task [001/206] /init/dirs
pushenv
pushenvvar CC_FOR_BUILD ${HOSTGNUPREFIXDASH}gcc
rm -rf ${ARMWORKHOME}/obj
rm -rf ${ARMWORKHOME}/install
rm -rf ${ARMWORKHOME}/pkg
rm -rf ${ARMWORKHOME}/logs
mkdir -p ${ARMWORKHOME}/obj
mkdir -p ${ARMWORKHOME}/install
mkdir -p ${ARMWORKHOME}/pkg
mkdir -p ${ARMWORKHOME}/logs/data
mkdir -p ${ARMWORKHOME}/install/share/doc/arm-arm-none-linux-gnueabi/html
mkdir -p ${ARMWORKHOME}/install/share/doc/arm-arm-none-linux-gnueabi/pdf
popenv

# task [002/206] /init/cleanup
pushenv
pushenvvar CC_FOR_BUILD ${HOSTGNUPREFIXDASH}gcc
rm -f ${ARMWORKHOME}/pkg/arm-2010q1-202-arm-none-linux-gnueabi.src.tar.bz2 ${ARMWORKHOME}/pkg/arm-2010q1-202-arm-none-linux-gnueabi.backup.tar.bz2
popenv


# task [033/206] /$HOSTGNUPREFIX/host_cleanup
pushenv
pushenvvar CC_FOR_BUILD ${HOSTGNUPREFIXDASH}gcc
pushenvvar CC ${HOSTGNUPREFIXDASH}gcc
pushenvvar CXX ${HOSTGNUPREFIXDASH}g++
pushenvvar AR ${HOSTGNUPREFIXDASH}ar
pushenvvar RANLIB ${HOSTGNUPREFIXDASH}ranlib
prepend_path PATH ${ARMWORKHOME}/install/bin
popenv

if [ $ENNONEEDPKD = true ]; then
# task [058/206] /$HOSTGNUPREFIX/toolchain/install_kernel_headers_glibc
pushenv
pushenvvar CC_FOR_BUILD ${HOSTGNUPREFIXDASH}gcc
pushenvvar CC ${HOSTGNUPREFIXDASH}gcc
pushenvvar CXX ${HOSTGNUPREFIXDASH}g++
pushenvvar AR ${HOSTGNUPREFIXDASH}ar
pushenvvar RANLIB ${HOSTGNUPREFIXDASH}ranlib
prepend_path PATH ${ARMWORKHOME}/install/bin
mkdir -p ${ARMWORKHOME}/install/arm-none-linux-gnueabi/libc/./usr/include ${ARMWORKHOME}/install/arm-none-linux-gnueabi/libc/./usr/include/bits ${ARMWORKHOME}/install/arm-none-linux-gnueabi/libc/./usr/include/gnu
rm -rf ${ARMWORKHOME}/obj/linux-2010q1-202-arm-none-linux-gnueabi-$HOSTGNUPREFIX
copy_dir_clean ${ARMWORKHOME}/src/linux-2010q1 ${ARMWORKHOME}/obj/linux-2010q1-202-arm-none-linux-gnueabi-$HOSTGNUPREFIX
chmod -R u+w ${ARMWORKHOME}/obj/linux-2010q1-202-arm-none-linux-gnueabi-$HOSTGNUPREFIX
pushd ${ARMWORKHOME}/obj/linux-2010q1-202-arm-none-linux-gnueabi-$HOSTGNUPREFIX
make ARCH=arm CROSS_COMPILE=${INSBINPATHFAKE}arm-none-linux-gnueabi- INSTALL_HDR_PATH=${ARMWORKHOME}/obj/linux-2010q1-202-arm-none-linux-gnueabi-$HOSTGNUPREFIX/tmp-install headers_install
pushd ${ARMWORKHOME}/obj/linux-2010q1-202-arm-none-linux-gnueabi-$HOSTGNUPREFIX/tmp-install/include
copy_dir linux ${ARMWORKHOME}/install/arm-none-linux-gnueabi/libc/./usr/include/linux
copy_dir asm ${ARMWORKHOME}/install/arm-none-linux-gnueabi/libc/./usr/include/asm
copy_dir asm-generic ${ARMWORKHOME}/install/arm-none-linux-gnueabi/libc/./usr/include/asm-generic
copy_dir drm ${ARMWORKHOME}/install/arm-none-linux-gnueabi/libc/./usr/include/drm
copy_dir mtd ${ARMWORKHOME}/install/arm-none-linux-gnueabi/libc/./usr/include/mtd
copy_dir rdma ${ARMWORKHOME}/install/arm-none-linux-gnueabi/libc/./usr/include/rdma
copy_dir sound ${ARMWORKHOME}/install/arm-none-linux-gnueabi/libc/./usr/include/sound
copy_dir video ${ARMWORKHOME}/install/arm-none-linux-gnueabi/libc/./usr/include/video
popd
popd
popenv
#if [ $ENNONEEDPKD = true ]; then
fi

if [ $ENNONEEDPKD = true ]; then
# task [059/206] /$HOSTGNUPREFIX/toolchain/create_sysroot/glibc_0
pushenv
pushenvvar CC_FOR_BUILD ${HOSTGNUPREFIXDASH}gcc
pushenvvar CC ${HOSTGNUPREFIXDASH}gcc
pushenvvar CXX ${HOSTGNUPREFIXDASH}g++
pushenvvar AR ${HOSTGNUPREFIXDASH}ar
pushenvvar RANLIB ${HOSTGNUPREFIXDASH}ranlib
prepend_path PATH ${ARMWORKHOME}/install/bin
mkdir -p ${ARMWORKHOME}/install/arm-none-linux-gnueabi/libc/./
popenv

#if [ $ENNONEEDPKD = true ]; then
fi

# task [066/206] /$HOSTGNUPREFIX/toolchain/glibc_headers/glibc_0/copy
pushenv
pushenvvar CC_FOR_BUILD ${HOSTGNUPREFIXDASH}gcc
pushenvvar CC ${HOSTGNUPREFIXDASH}gcc
pushenvvar CXX ${HOSTGNUPREFIXDASH}g++
pushenvvar AR ${HOSTGNUPREFIXDASH}ar
pushenvvar RANLIB ${HOSTGNUPREFIXDASH}ranlib
prepend_path PATH ${ARMWORKHOME}/install/bin
rm -rf ${ARMWORKHOME}/obj/glibc-src-2010q1-202-arm-none-linux-gnueabi-$HOSTGNUPREFIX/default/
copy_dir_clean ${ARMWORKHOME}/src/glibc-2.11-2010q1 ${ARMWORKHOME}/obj/glibc-src-2010q1-202-arm-none-linux-gnueabi-$HOSTGNUPREFIX/default/
chmod -R u+w ${ARMWORKHOME}/obj/glibc-src-2010q1-202-arm-none-linux-gnueabi-$HOSTGNUPREFIX/default/
copy_dir_clean ${ARMWORKHOME}/src/glibc-ports-2.11-2010q1 ${ARMWORKHOME}/obj/glibc-src-2010q1-202-arm-none-linux-gnueabi-$HOSTGNUPREFIX/default//ports
chmod -R u+w ${ARMWORKHOME}/obj/glibc-src-2010q1-202-arm-none-linux-gnueabi-$HOSTGNUPREFIX/default//ports
touch ${ARMWORKHOME}/obj/glibc-src-2010q1-202-arm-none-linux-gnueabi-$HOSTGNUPREFIX/default//.gnu-stamp
popenv

if [ $ENNONEEDPKD = true ]; then
# task [067/206] /$HOSTGNUPREFIX/toolchain/glibc_headers/glibc_0/configure
pushenv
pushenvvar CC_FOR_BUILD ${HOSTGNUPREFIXDASH}gcc
pushenvvar CC ${HOSTGNUPREFIXDASH}gcc
pushenvvar CXX ${HOSTGNUPREFIXDASH}g++
pushenvvar AR ${HOSTGNUPREFIXDASH}ar
pushenvvar RANLIB ${HOSTGNUPREFIXDASH}ranlib
prepend_path PATH ${ARMWORKHOME}/install/bin
mkdir -p ${ARMWORKHOME}/install/arm-none-linux-gnueabi/libc/./lib/./ ${ARMWORKHOME}/install/arm-none-linux-gnueabi/libc/./usr/lib/./
rm -rf ${ARMWORKHOME}/obj/glibc-headers-2010q1-202-arm-none-linux-gnueabi-$HOSTGNUPREFIX/default/
mkdir -p ${ARMWORKHOME}/obj/glibc-headers-2010q1-202-arm-none-linux-gnueabi-$HOSTGNUPREFIX/default/
pushd ${ARMWORKHOME}/obj/glibc-headers-2010q1-202-arm-none-linux-gnueabi-$HOSTGNUPREFIX/default/
pushenv
pushenvvar BUILD_CC gcc
pushenvvar CC "${INSBINPATHFAKE}arm-none-linux-gnueabi-gcc "
pushenvvar CXX "${INSBINPATHFAKE}arm-none-linux-gnueabi-g++ "
pushenvvar CFLAGS '-g -O2'
pushenvvar AR ${INSBINPATHFAKE}arm-none-linux-gnueabi-ar
pushenvvar RANLIB ${INSBINPATHFAKE}arm-none-linux-gnueabi-ranlib
pushenvvar PERL /usr/bin/perl
pushenvvar BASH_SHELL /bin/bash
${ARMWORKHOME}/obj/glibc-src-2010q1-202-arm-none-linux-gnueabi-$HOSTGNUPREFIX/default//configure --prefix=/usr --with-headers=${INCLUDEPATH} --build=$HOSTGNUPREFIX --host=arm-none-linux-gnueabi --disable-profile --without-gd --without-cvs --enable-add-ons --enable-kernel=2.6.16 '--with-pkgversion=Sourcery G++ Lite 2010q1-202' --with-bugurl=https://support.codesourcery.com/GNUToolchain/
popenv
popd
popenv

# task [068/206] /$HOSTGNUPREFIX/toolchain/glibc_headers/glibc_0/install
pushenv
pushenvvar CC_FOR_BUILD ${HOSTGNUPREFIXDASH}gcc
pushenvvar CC ${HOSTGNUPREFIXDASH}gcc
pushenvvar CXX ${HOSTGNUPREFIXDASH}g++
pushenvvar AR ${HOSTGNUPREFIXDASH}ar
pushenvvar RANLIB ${HOSTGNUPREFIXDASH}ranlib
prepend_path PATH ${ARMWORKHOME}/install/bin
pushd ${ARMWORKHOME}/obj/glibc-headers-2010q1-202-arm-none-linux-gnueabi-$HOSTGNUPREFIX/default/
make install_root=${ARMWORKHOME}/install/arm-none-linux-gnueabi/libc/./ install-bootstrap-headers=yes install-headers
make csu/subdir_lib
cp csu/crt1.o csu/crti.o csu/crtn.o ${ARMWORKHOME}/install/arm-none-linux-gnueabi/libc/./usr/lib/./
popd
${INSBINPATHFAKE}arm-none-linux-gnueabi-gcc -o ${ARMWORKHOME}/install/arm-none-linux-gnueabi/libc/./usr/lib/.//libc.so -nostdlib -nostartfiles -shared -x c /dev/null
popenv

#if [ $ENNONEEDPKD = true ]; then
fi

# task [079/206] /$HOSTGNUPREFIX/toolchain/glibc/glibc_0/configure
pushenv
pushenvvar CC_FOR_BUILD ${HOSTGNUPREFIXDASH}gcc
pushenvvar CC ${HOSTGNUPREFIXDASH}gcc
pushenvvar CXX ${HOSTGNUPREFIXDASH}g++
pushenvvar AR ${HOSTGNUPREFIXDASH}ar
pushenvvar RANLIB ${HOSTGNUPREFIXDASH}ranlib
prepend_path PATH ${ARMWORKHOME}/install/bin
mkdir -p ${ARMWORKHOME}/install/arm-none-linux-gnueabi/libc/./lib/./ ${ARMWORKHOME}/install/arm-none-linux-gnueabi/libc/./usr/lib/./
rm -rf ${ARMWORKHOME}/obj/glibc-2010q1-202-arm-none-linux-gnueabi-$HOSTGNUPREFIX/default/
mkdir -p ${ARMWORKHOME}/obj/glibc-2010q1-202-arm-none-linux-gnueabi-$HOSTGNUPREFIX/default/
pushd ${ARMWORKHOME}/obj/glibc-2010q1-202-arm-none-linux-gnueabi-$HOSTGNUPREFIX/default/
pushenv
pushenvvar BUILD_CC gcc
pushenvvar CC "${INSBINPATHFAKE}arm-none-linux-gnueabi-gcc "
pushenvvar CXX "${INSBINPATHFAKE}arm-none-linux-gnueabi-g++ "
pushenvvar CFLAGS '-g -O2'
pushenvvar AR ${INSBINPATHFAKE}arm-none-linux-gnueabi-ar
pushenvvar RANLIB ${INSBINPATHFAKE}arm-none-linux-gnueabi-ranlib
pushenvvar PERL /usr/bin/perl
pushenvvar BASH_SHELL /bin/bash
${ARMWORKHOME}/obj/glibc-src-2010q1-202-arm-none-linux-gnueabi-$HOSTGNUPREFIX/default//configure --prefix=/usr --with-headers=${INCLUDEPATH}  --build=$HOSTGNUPREFIX --host=arm-none-linux-gnueabi --disable-profile --without-gd --without-cvs --enable-add-ons --enable-kernel=2.6.16 '--with-pkgversion=Sourcery G++ Lite 2010q1-202' --with-bugurl=https://support.codesourcery.com/GNUToolchain/
popenv
popd
popenv

# task [080/206] /$HOSTGNUPREFIX/toolchain/glibc/glibc_0/build
pushenv
pushenvvar CC_FOR_BUILD ${HOSTGNUPREFIXDASH}gcc
pushenvvar CC ${HOSTGNUPREFIXDASH}gcc
pushenvvar CXX ${HOSTGNUPREFIXDASH}g++
pushenvvar AR ${HOSTGNUPREFIXDASH}ar
pushenvvar RANLIB ${HOSTGNUPREFIXDASH}ranlib
prepend_path PATH ${ARMWORKHOME}/install/bin
pushd ${ARMWORKHOME}/obj/glibc-2010q1-202-arm-none-linux-gnueabi-$HOSTGNUPREFIX/default/
pushenv
pushenvvar PARALLELMFLAGS ${MKJOBOPT}
make
popenv
popd
popenv

# task [081/206] /$HOSTGNUPREFIX/toolchain/glibc/glibc_0/install
pushenv
pushenvvar CC_FOR_BUILD ${HOSTGNUPREFIXDASH}gcc
pushenvvar CC ${HOSTGNUPREFIXDASH}gcc
pushenvvar CXX ${HOSTGNUPREFIXDASH}g++
pushenvvar AR ${HOSTGNUPREFIXDASH}ar
pushenvvar RANLIB ${HOSTGNUPREFIXDASH}ranlib
prepend_path PATH ${ARMWORKHOME}/install/bin
rm -f ${ARMWORKHOME}/install/arm-none-linux-gnueabi/libc/.//usr/info/dir ${ARMWORKHOME}/install/arm-none-linux-gnueabi/libc/.//usr/share/info/dir
pushd ${ARMWORKHOME}/obj/glibc-2010q1-202-arm-none-linux-gnueabi-$HOSTGNUPREFIX/default/
make install_root=${ARMWORKHOME}/install/arm-none-linux-gnueabi/libc/./ install
popd
popenv
#if [ $TMPDIS != true ]; then
fi

# task [132/206] /$HOSTGNUPREFIX/strip_target_objects
pushenv
pushenvvar CC_FOR_BUILD ${HOSTGNUPREFIXDASH}gcc
pushenvvar CC ${HOSTGNUPREFIXDASH}gcc
pushenvvar CXX ${HOSTGNUPREFIXDASH}g++
pushenvvar AR ${HOSTGNUPREFIXDASH}ar
pushenvvar RANLIB ${HOSTGNUPREFIXDASH}ranlib
prepend_path PATH ${ARMWORKHOME}/install/bin
#find ${ARMWORKHOME}/install/arm-none-linux-gnueabi/lib/ -name "*.so*" |xargs -n 1 arm-none-linux-gnueabi-objcopy -R .comment -R .note -R .debug_info -R .debug_aranges -R .debug_pubnames -R .debug_abbrev -R .debug_line -R .debug_str -R .debug_ranges -R .debug_loc
#arm-none-linux-gnueabi-objcopy -R .comment -R .note -R .debug_info -R .debug_aranges -R .debug_pubnames -R .debug_abbrev -R .debug_line -R .debug_str -R .debug_ranges -R .debug_loc ${ARMWORKHOME}/install/arm-none-linux-gnueabi/libc/armv4t/usr/lib/libpthread.a || true

arm-none-linux-gnueabi-objcopy -R .comment -R .note -R .debug_info -R .debug_aranges -R .debug_pubnames -R .debug_abbrev -R .debug_line -R .debug_str -R .debug_ranges -R .debug_loc ${ARMWORKHOME}/install/arm-none-linux-gnueabi/libc/usr/lib/libbsd-compat.a || true
arm-none-linux-gnueabi-objcopy -R .comment -R .note -R .debug_info -R .debug_aranges -R .debug_pubnames -R .debug_abbrev -R .debug_line -R .debug_str -R .debug_ranges -R .debug_loc ${ARMWORKHOME}/install/arm-none-linux-gnueabi/libc/usr/lib/libcidn.so || true
arm-none-linux-gnueabi-objcopy -R .comment -R .note -R .debug_info -R .debug_aranges -R .debug_pubnames -R .debug_abbrev -R .debug_line -R .debug_str -R .debug_ranges -R .debug_loc ${ARMWORKHOME}/install/arm-none-linux-gnueabi/libc/usr/lib/libthread_db.so || true
arm-none-linux-gnueabi-objcopy -R .comment -R .note -R .debug_info -R .debug_aranges -R .debug_pubnames -R .debug_abbrev -R .debug_line -R .debug_str -R .debug_ranges -R .debug_loc ${ARMWORKHOME}/install/arm-none-linux-gnueabi/libc/usr/lib/libpthread.a || true
arm-none-linux-gnueabi-objcopy -R .comment -R .note -R .debug_info -R .debug_aranges -R .debug_pubnames -R .debug_abbrev -R .debug_line -R .debug_str -R .debug_ranges -R .debug_loc ${ARMWORKHOME}/install/arm-none-linux-gnueabi/libc/usr/lib/libnss_compat.so || true
arm-none-linux-gnueabi-objcopy -R .comment -R .note -R .debug_info -R .debug_aranges -R .debug_pubnames -R .debug_abbrev -R .debug_line -R .debug_str -R .debug_ranges -R .debug_loc ${ARMWORKHOME}/install/arm-none-linux-gnueabi/libc/usr/lib/libutil.so || true
arm-none-linux-gnueabi-objcopy -R .comment -R .note -R .debug_info -R .debug_aranges -R .debug_pubnames -R .debug_abbrev -R .debug_line -R .debug_str -R .debug_ranges -R .debug_loc ${ARMWORKHOME}/install/arm-none-linux-gnueabi/libc/usr/lib/libresolv.a || true
arm-none-linux-gnueabi-objcopy -R .comment -R .note -R .debug_info -R .debug_aranges -R .debug_pubnames -R .debug_abbrev -R .debug_line -R .debug_str -R .debug_ranges -R .debug_loc ${ARMWORKHOME}/install/arm-none-linux-gnueabi/libc/usr/lib/libcrypt.so || true
arm-none-linux-gnueabi-objcopy -R .comment -R .note -R .debug_info -R .debug_aranges -R .debug_pubnames -R .debug_abbrev -R .debug_line -R .debug_str -R .debug_ranges -R .debug_loc ${ARMWORKHOME}/install/arm-none-linux-gnueabi/libc/usr/lib/libnss_nis.so || true
arm-none-linux-gnueabi-objcopy -R .comment -R .note -R .debug_info -R .debug_aranges -R .debug_pubnames -R .debug_abbrev -R .debug_line -R .debug_str -R .debug_ranges -R .debug_loc ${ARMWORKHOME}/install/arm-none-linux-gnueabi/libc/usr/lib/libnss_nisplus.so || true
arm-none-linux-gnueabi-objcopy -R .comment -R .note -R .debug_info -R .debug_aranges -R .debug_pubnames -R .debug_abbrev -R .debug_line -R .debug_str -R .debug_ranges -R .debug_loc ${ARMWORKHOME}/install/arm-none-linux-gnueabi/libc/usr/lib/libc_nonshared.a || true
arm-none-linux-gnueabi-objcopy -R .comment -R .note -R .debug_info -R .debug_aranges -R .debug_pubnames -R .debug_abbrev -R .debug_line -R .debug_str -R .debug_ranges -R .debug_loc ${ARMWORKHOME}/install/arm-none-linux-gnueabi/libc/usr/lib/libc.a || true
arm-none-linux-gnueabi-objcopy -R .comment -R .note -R .debug_info -R .debug_aranges -R .debug_pubnames -R .debug_abbrev -R .debug_line -R .debug_str -R .debug_ranges -R .debug_loc ${ARMWORKHOME}/install/arm-none-linux-gnueabi/libc/usr/lib/libnsl.a || true
arm-none-linux-gnueabi-objcopy -R .comment -R .note -R .debug_info -R .debug_aranges -R .debug_pubnames -R .debug_abbrev -R .debug_line -R .debug_str -R .debug_ranges -R .debug_loc ${ARMWORKHOME}/install/arm-none-linux-gnueabi/libc/usr/lib/Mcrt1.o || true
arm-none-linux-gnueabi-objcopy -R .comment -R .note -R .debug_info -R .debug_aranges -R .debug_pubnames -R .debug_abbrev -R .debug_line -R .debug_str -R .debug_ranges -R .debug_loc ${ARMWORKHOME}/install/arm-none-linux-gnueabi/libc/usr/lib/libdl.so || true
arm-none-linux-gnueabi-objcopy -R .comment -R .note -R .debug_info -R .debug_aranges -R .debug_pubnames -R .debug_abbrev -R .debug_line -R .debug_str -R .debug_ranges -R .debug_loc ${ARMWORKHOME}/install/arm-none-linux-gnueabi/libc/usr/lib/gcrt1.o || true
arm-none-linux-gnueabi-objcopy -R .comment -R .note -R .debug_info -R .debug_aranges -R .debug_pubnames -R .debug_abbrev -R .debug_line -R .debug_str -R .debug_ranges -R .debug_loc ${ARMWORKHOME}/install/arm-none-linux-gnueabi/libc/usr/lib/libg.a || true
arm-none-linux-gnueabi-objcopy -R .comment -R .note -R .debug_info -R .debug_aranges -R .debug_pubnames -R .debug_abbrev -R .debug_line -R .debug_str -R .debug_ranges -R .debug_loc ${ARMWORKHOME}/install/arm-none-linux-gnueabi/libc/usr/lib/librt.a || true
arm-none-linux-gnueabi-objcopy -R .comment -R .note -R .debug_info -R .debug_aranges -R .debug_pubnames -R .debug_abbrev -R .debug_line -R .debug_str -R .debug_ranges -R .debug_loc ${ARMWORKHOME}/install/arm-none-linux-gnueabi/libc/usr/lib/libcrypt.a || true
arm-none-linux-gnueabi-objcopy -R .comment -R .note -R .debug_info -R .debug_aranges -R .debug_pubnames -R .debug_abbrev -R .debug_line -R .debug_str -R .debug_ranges -R .debug_loc ${ARMWORKHOME}/install/arm-none-linux-gnueabi/libc/usr/lib/libanl.a || true
arm-none-linux-gnueabi-objcopy -R .comment -R .note -R .debug_info -R .debug_aranges -R .debug_pubnames -R .debug_abbrev -R .debug_line -R .debug_str -R .debug_ranges -R .debug_loc ${ARMWORKHOME}/install/arm-none-linux-gnueabi/libc/usr/lib/librt.so || true
arm-none-linux-gnueabi-objcopy -R .comment -R .note -R .debug_info -R .debug_aranges -R .debug_pubnames -R .debug_abbrev -R .debug_line -R .debug_str -R .debug_ranges -R .debug_loc ${ARMWORKHOME}/install/arm-none-linux-gnueabi/libc/usr/lib/crt1.o || true
arm-none-linux-gnueabi-objcopy -R .comment -R .note -R .debug_info -R .debug_aranges -R .debug_pubnames -R .debug_abbrev -R .debug_line -R .debug_str -R .debug_ranges -R .debug_loc ${ARMWORKHOME}/install/arm-none-linux-gnueabi/libc/usr/lib/crti.o || true
arm-none-linux-gnueabi-objcopy -R .comment -R .note -R .debug_info -R .debug_aranges -R .debug_pubnames -R .debug_abbrev -R .debug_line -R .debug_str -R .debug_ranges -R .debug_loc ${ARMWORKHOME}/install/arm-none-linux-gnueabi/libc/usr/lib/libanl.so || true
#arm-none-linux-gnueabi-objcopy -R .comment -R .note -R .debug_info -R .debug_aranges -R .debug_pubnames -R .debug_abbrev -R .debug_line -R .debug_str -R .debug_ranges -R .debug_loc ${ARMWORKHOME}/install/arm-none-linux-gnueabi/libc/usr/lib/libc.so || true
#arm-none-linux-gnueabi-objcopy -R .comment -R .note -R .debug_info -R .debug_aranges -R .debug_pubnames -R .debug_abbrev -R .debug_line -R .debug_str -R .debug_ranges -R .debug_loc ${ARMWORKHOME}/install/arm-none-linux-gnueabi/libc/usr/lib/libstdc++.so || true
arm-none-linux-gnueabi-objcopy -R .comment -R .note -R .debug_info -R .debug_aranges -R .debug_pubnames -R .debug_abbrev -R .debug_line -R .debug_str -R .debug_ranges -R .debug_loc ${ARMWORKHOME}/install/arm-none-linux-gnueabi/libc/usr/lib/libdl.a || true
arm-none-linux-gnueabi-objcopy -R .comment -R .note -R .debug_info -R .debug_aranges -R .debug_pubnames -R .debug_abbrev -R .debug_line -R .debug_str -R .debug_ranges -R .debug_loc ${ARMWORKHOME}/install/arm-none-linux-gnueabi/libc/usr/lib/libresolv.so || true
arm-none-linux-gnueabi-objcopy -R .comment -R .note -R .debug_info -R .debug_aranges -R .debug_pubnames -R .debug_abbrev -R .debug_line -R .debug_str -R .debug_ranges -R .debug_loc ${ARMWORKHOME}/install/arm-none-linux-gnueabi/libc/usr/lib/librpcsvc.a || true
#arm-none-linux-gnueabi-objcopy -R .comment -R .note -R .debug_info -R .debug_aranges -R .debug_pubnames -R .debug_abbrev -R .debug_line -R .debug_str -R .debug_ranges -R .debug_loc ${ARMWORKHOME}/install/arm-none-linux-gnueabi/libc/usr/lib/libstdc++.a || true
#arm-none-linux-gnueabi-objcopy -R .comment -R .note -R .debug_info -R .debug_aranges -R .debug_pubnames -R .debug_abbrev -R .debug_line -R .debug_str -R .debug_ranges -R .debug_loc ${ARMWORKHOME}/install/arm-none-linux-gnueabi/libc/usr/lib/libpthread.so || true
arm-none-linux-gnueabi-objcopy -R .comment -R .note -R .debug_info -R .debug_aranges -R .debug_pubnames -R .debug_abbrev -R .debug_line -R .debug_str -R .debug_ranges -R .debug_loc ${ARMWORKHOME}/install/arm-none-linux-gnueabi/libc/usr/lib/Scrt1.o || true
arm-none-linux-gnueabi-objcopy -R .comment -R .note -R .debug_info -R .debug_aranges -R .debug_pubnames -R .debug_abbrev -R .debug_line -R .debug_str -R .debug_ranges -R .debug_loc ${ARMWORKHOME}/install/arm-none-linux-gnueabi/libc/usr/lib/libnsl.so || true
arm-none-linux-gnueabi-objcopy -R .comment -R .note -R .debug_info -R .debug_aranges -R .debug_pubnames -R .debug_abbrev -R .debug_line -R .debug_str -R .debug_ranges -R .debug_loc ${ARMWORKHOME}/install/arm-none-linux-gnueabi/libc/usr/lib/libutil.a || true
arm-none-linux-gnueabi-objcopy -R .comment -R .note -R .debug_info -R .debug_aranges -R .debug_pubnames -R .debug_abbrev -R .debug_line -R .debug_str -R .debug_ranges -R .debug_loc ${ARMWORKHOME}/install/arm-none-linux-gnueabi/libc/usr/lib/libnss_dns.so || true
arm-none-linux-gnueabi-objcopy -R .comment -R .note -R .debug_info -R .debug_aranges -R .debug_pubnames -R .debug_abbrev -R .debug_line -R .debug_str -R .debug_ranges -R .debug_loc ${ARMWORKHOME}/install/arm-none-linux-gnueabi/libc/usr/lib/libnss_files.so || true
arm-none-linux-gnueabi-objcopy -R .comment -R .note -R .debug_info -R .debug_aranges -R .debug_pubnames -R .debug_abbrev -R .debug_line -R .debug_str -R .debug_ranges -R .debug_loc ${ARMWORKHOME}/install/arm-none-linux-gnueabi/libc/usr/lib/libBrokenLocale.so || true
arm-none-linux-gnueabi-objcopy -R .comment -R .note -R .debug_info -R .debug_aranges -R .debug_pubnames -R .debug_abbrev -R .debug_line -R .debug_str -R .debug_ranges -R .debug_loc ${ARMWORKHOME}/install/arm-none-linux-gnueabi/libc/usr/lib/libBrokenLocale.a || true
arm-none-linux-gnueabi-objcopy -R .comment -R .note -R .debug_info -R .debug_aranges -R .debug_pubnames -R .debug_abbrev -R .debug_line -R .debug_str -R .debug_ranges -R .debug_loc ${ARMWORKHOME}/install/arm-none-linux-gnueabi/libc/usr/lib/libpthread_nonshared.a || true
arm-none-linux-gnueabi-objcopy -R .comment -R .note -R .debug_info -R .debug_aranges -R .debug_pubnames -R .debug_abbrev -R .debug_line -R .debug_str -R .debug_ranges -R .debug_loc ${ARMWORKHOME}/install/arm-none-linux-gnueabi/libc/usr/lib/crtn.o || true
arm-none-linux-gnueabi-objcopy -R .comment -R .note -R .debug_info -R .debug_aranges -R .debug_pubnames -R .debug_abbrev -R .debug_line -R .debug_str -R .debug_ranges -R .debug_loc ${ARMWORKHOME}/install/arm-none-linux-gnueabi/libc/usr/lib/libmcheck.a || true
arm-none-linux-gnueabi-objcopy -R .comment -R .note -R .debug_info -R .debug_aranges -R .debug_pubnames -R .debug_abbrev -R .debug_line -R .debug_str -R .debug_ranges -R .debug_loc ${ARMWORKHOME}/install/arm-none-linux-gnueabi/libc/usr/lib/libnss_hesiod.so || true
arm-none-linux-gnueabi-objcopy -R .comment -R .note -R .debug_info -R .debug_aranges -R .debug_pubnames -R .debug_abbrev -R .debug_line -R .debug_str -R .debug_ranges -R .debug_loc ${ARMWORKHOME}/install/arm-none-linux-gnueabi/libc/usr/lib/libm.so || true
arm-none-linux-gnueabi-objcopy -R .comment -R .note -R .debug_info -R .debug_aranges -R .debug_pubnames -R .debug_abbrev -R .debug_line -R .debug_str -R .debug_ranges -R .debug_loc ${ARMWORKHOME}/install/arm-none-linux-gnueabi/libc/usr/lib/libieee.a || true
arm-none-linux-gnueabi-objcopy -R .comment -R .note -R .debug_info -R .debug_aranges -R .debug_pubnames -R .debug_abbrev -R .debug_line -R .debug_str -R .debug_ranges -R .debug_loc ${ARMWORKHOME}/install/arm-none-linux-gnueabi/libc/usr/lib/libm.a || true
#arm-none-linux-gnueabi-objcopy -R .comment -R .note -R .debug_info -R .debug_aranges -R .debug_pubnames -R .debug_abbrev -R .debug_line -R .debug_str -R .debug_ranges -R .debug_loc ${ARMWORKHOME}/install/arm-none-linux-gnueabi/libc/lib/libgcc_s.so || true
arm-none-linux-gnueabi-objcopy -R .comment -R .note -R .debug_info -R .debug_aranges -R .debug_pubnames -R .debug_abbrev -R .debug_line -R .debug_str -R .debug_ranges -R .debug_loc ${ARMWORKHOME}/install/arm-none-linux-gnueabi/libc/lib/libBrokenLocale-2.11.1.so || true
arm-none-linux-gnueabi-objcopy -R .comment -R .note -R .debug_info -R .debug_aranges -R .debug_pubnames -R .debug_abbrev -R .debug_line -R .debug_str -R .debug_ranges -R .debug_loc ${ARMWORKHOME}/install/arm-none-linux-gnueabi/libc/lib/libpcprofile.so || true
arm-none-linux-gnueabi-objcopy -R .comment -R .note -R .debug_info -R .debug_aranges -R .debug_pubnames -R .debug_abbrev -R .debug_line -R .debug_str -R .debug_ranges -R .debug_loc ${ARMWORKHOME}/install/arm-none-linux-gnueabi/libc/lib/libnss_hesiod-2.11.1.so || true
arm-none-linux-gnueabi-objcopy -R .comment -R .note -R .debug_info -R .debug_aranges -R .debug_pubnames -R .debug_abbrev -R .debug_line -R .debug_str -R .debug_ranges -R .debug_loc ${ARMWORKHOME}/install/arm-none-linux-gnueabi/libc/lib/libmemusage.so || true
arm-none-linux-gnueabi-objcopy -R .comment -R .note -R .debug_info -R .debug_aranges -R .debug_pubnames -R .debug_abbrev -R .debug_line -R .debug_str -R .debug_ranges -R .debug_loc ${ARMWORKHOME}/install/arm-none-linux-gnueabi/libc/lib/libnss_dns-2.11.1.so || true
arm-none-linux-gnueabi-objcopy -R .comment -R .note -R .debug_info -R .debug_aranges -R .debug_pubnames -R .debug_abbrev -R .debug_line -R .debug_str -R .debug_ranges -R .debug_loc ${ARMWORKHOME}/install/arm-none-linux-gnueabi/libc/lib/libcrypt-2.11.1.so || true
arm-none-linux-gnueabi-objcopy -R .comment -R .note -R .debug_info -R .debug_aranges -R .debug_pubnames -R .debug_abbrev -R .debug_line -R .debug_str -R .debug_ranges -R .debug_loc ${ARMWORKHOME}/install/arm-none-linux-gnueabi/libc/lib/libanl-2.11.1.so || true
arm-none-linux-gnueabi-objcopy -R .comment -R .note -R .debug_info -R .debug_aranges -R .debug_pubnames -R .debug_abbrev -R .debug_line -R .debug_str -R .debug_ranges -R .debug_loc ${ARMWORKHOME}/install/arm-none-linux-gnueabi/libc/lib/libc-2.11.1.so || true
arm-none-linux-gnueabi-objcopy -R .comment -R .note -R .debug_info -R .debug_aranges -R .debug_pubnames -R .debug_abbrev -R .debug_line -R .debug_str -R .debug_ranges -R .debug_loc ${ARMWORKHOME}/install/arm-none-linux-gnueabi/libc/lib/libnsl-2.11.1.so || true
arm-none-linux-gnueabi-objcopy -R .comment -R .note -R .debug_info -R .debug_aranges -R .debug_pubnames -R .debug_abbrev -R .debug_line -R .debug_str -R .debug_ranges -R .debug_loc ${ARMWORKHOME}/install/arm-none-linux-gnueabi/libc/lib/libnss_files-2.11.1.so || true
arm-none-linux-gnueabi-objcopy -R .comment -R .note -R .debug_info -R .debug_aranges -R .debug_pubnames -R .debug_abbrev -R .debug_line -R .debug_str -R .debug_ranges -R .debug_loc ${ARMWORKHOME}/install/arm-none-linux-gnueabi/libc/lib/libcidn-2.11.1.so || true
arm-none-linux-gnueabi-objcopy -R .comment -R .note -R .debug_info -R .debug_aranges -R .debug_pubnames -R .debug_abbrev -R .debug_line -R .debug_str -R .debug_ranges -R .debug_loc ${ARMWORKHOME}/install/arm-none-linux-gnueabi/libc/lib/librt-2.11.1.so || true
arm-none-linux-gnueabi-objcopy -R .comment -R .note -R .debug_info -R .debug_aranges -R .debug_pubnames -R .debug_abbrev -R .debug_line -R .debug_str -R .debug_ranges -R .debug_loc ${ARMWORKHOME}/install/arm-none-linux-gnueabi/libc/lib/libthread_db-1.0.so || true
arm-none-linux-gnueabi-objcopy -R .comment -R .note -R .debug_info -R .debug_aranges -R .debug_pubnames -R .debug_abbrev -R .debug_line -R .debug_str -R .debug_ranges -R .debug_loc ${ARMWORKHOME}/install/arm-none-linux-gnueabi/libc/lib/libutil-2.11.1.so || true
arm-none-linux-gnueabi-objcopy -R .comment -R .note -R .debug_info -R .debug_aranges -R .debug_pubnames -R .debug_abbrev -R .debug_line -R .debug_str -R .debug_ranges -R .debug_loc ${ARMWORKHOME}/install/arm-none-linux-gnueabi/libc/lib/libm-2.11.1.so || true
arm-none-linux-gnueabi-objcopy -R .comment -R .note -R .debug_info -R .debug_aranges -R .debug_pubnames -R .debug_abbrev -R .debug_line -R .debug_str -R .debug_ranges -R .debug_loc ${ARMWORKHOME}/install/arm-none-linux-gnueabi/libc/lib/libnss_nisplus-2.11.1.so || true
arm-none-linux-gnueabi-objcopy -R .comment -R .note -R .debug_info -R .debug_aranges -R .debug_pubnames -R .debug_abbrev -R .debug_line -R .debug_str -R .debug_ranges -R .debug_loc ${ARMWORKHOME}/install/arm-none-linux-gnueabi/libc/lib/libresolv-2.11.1.so || true
arm-none-linux-gnueabi-objcopy -R .comment -R .note -R .debug_info -R .debug_aranges -R .debug_pubnames -R .debug_abbrev -R .debug_line -R .debug_str -R .debug_ranges -R .debug_loc ${ARMWORKHOME}/install/arm-none-linux-gnueabi/libc/lib/ld-2.11.1.so || true
arm-none-linux-gnueabi-objcopy -R .comment -R .note -R .debug_info -R .debug_aranges -R .debug_pubnames -R .debug_abbrev -R .debug_line -R .debug_str -R .debug_ranges -R .debug_loc ${ARMWORKHOME}/install/arm-none-linux-gnueabi/libc/lib/libSegFault.so || true
arm-none-linux-gnueabi-objcopy -R .comment -R .note -R .debug_info -R .debug_aranges -R .debug_pubnames -R .debug_abbrev -R .debug_line -R .debug_str -R .debug_ranges -R .debug_loc ${ARMWORKHOME}/install/arm-none-linux-gnueabi/libc/lib/libnss_nis-2.11.1.so || true
arm-none-linux-gnueabi-objcopy -R .comment -R .note -R .debug_info -R .debug_aranges -R .debug_pubnames -R .debug_abbrev -R .debug_line -R .debug_str -R .debug_ranges -R .debug_loc ${ARMWORKHOME}/install/arm-none-linux-gnueabi/libc/lib/libpthread-2.11.1.so || true
arm-none-linux-gnueabi-objcopy -R .comment -R .note -R .debug_info -R .debug_aranges -R .debug_pubnames -R .debug_abbrev -R .debug_line -R .debug_str -R .debug_ranges -R .debug_loc ${ARMWORKHOME}/install/arm-none-linux-gnueabi/libc/lib/libdl-2.11.1.so || true
arm-none-linux-gnueabi-objcopy -R .comment -R .note -R .debug_info -R .debug_aranges -R .debug_pubnames -R .debug_abbrev -R .debug_line -R .debug_str -R .debug_ranges -R .debug_loc ${ARMWORKHOME}/install/arm-none-linux-gnueabi/libc/lib/libnss_compat-2.11.1.so || true
popenv


