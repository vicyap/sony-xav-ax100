#!/bin/bash

#writed by jh.ai 2013-7-4 11:07:25

INSTALL_PREFIX=$PWD/out/NoGPL_android

mkdir -p $INSTALL_PREFIX;
./configure \
	--disable-all \
	--prefix=$INSTALL_PREFIX \
	--enable-version3 \
	--enable-nonfree \
	--disable-static \
	--enable-shared \
	--enable-pic \
	--disable-doc \
	--disable-htmlpages \
	--disable-manpages \
	--disable-podpages \
	--disable-txtpages \
	--enable-cross-compile \
	--cross-prefix=arm-linux-androideabi- \
	--target-os=linux \
	--enable-runtime-cpudetect \
	--arch=arm \
	--cpu=armv7-a \
	--enable-neon \
	--enable-safe-bitstream-reader \
	--enable-memalign-hack \
	--enable-asm \
	--enable-vfp \
	--disable-debug \
	--enable-zlib \
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
	--disable-librtmp \
	--enable-openssl \
	--disable-decoder=amrnb \
	--disable-decoder=amrwb \
	--enable-libopencore-amrnb \
	--enable-libopencore-amrwb \
	--extra-cflags="-march=armv7-a -mfloat-abi=softfp -mfpu=neon -I../rtmpdump/librtmp/out/android/include -I$HOME/workspace/android-ffmpeg/openssl-android/include -I../opencore-amr-0.1.3/out/android/include" \
	--extra-ldflags="-march=armv7-a -Wl,--fix-cortex-a8 -L../rtmpdump/librtmp/out/android/lib -L$HOME/workspace/android-ffmpeg/openssl-android/libs/armeabi -L../opencore-amr-0.1.3/out/android/lib" \


