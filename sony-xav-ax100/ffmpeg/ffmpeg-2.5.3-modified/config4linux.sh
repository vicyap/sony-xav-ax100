#!/bin/bash

#writed by jh.ai 2013-7-8 10:11:32

INSTALL_PREFIX=./out/noGPLlinux/

mkdir -p $INSTALL_PREFIX;
./configure \
	--disable-all \
	--prefix=$INSTALL_PREFIX \
	--enable-version3 \
	--enable-nonfree \
	--enable-static \
	--enable-shared \
	--enable-pic \
	--disable-doc \
	--disable-htmlpages \
	--disable-manpages \
	--disable-podpages \
	--disable-txtpages \
	--enable-cross-compile \
	--cross-prefix=arm-none-linux-gnueabi- \
	--target-os=linux \
	--disable-runtime-cpudetect \
	--arch=arm \
	--cpu=cortex-a8 \
	--enable-neon \
	--enable-safe-bitstream-reader \
	--enable-memalign-hack \
	--enable-asm \
	--enable-vfp \
	--disable-debug \
	--disable-zlib \
	--enable-stripping \
	--enable-network \
	--enable-pthreads \
	--enable-decoders \
	--enable-parsers \
	--enable-demuxers \
	--enable-protocols \
	--enable-filters \
	--enable-avformat \
	--enable-avcodec \
	--enable-avutil \
	--enable-avfilter \
	--enable-swresample \
	--enable-swscale \
	--enable-ffmpeg \
	--enable-ffprobe \
	--disable-symver \
	--enable-openssl \
	--extra-cflags="-mfloat-abi=softfp -mfpu=neon -I$HOME/WorkSpace/android-ffmpeg/openssl-linux/include" \
	--extra-ldflags="-march=armv7-a -Wl,--fix-cortex-a8 -L$HOME/WorkSpace/android-ffmpeg/openssl-linux/" \
	--extra-libs="$HOME/WorkSpace/android-ffmpeg/openssl-linux/libssl.a"

