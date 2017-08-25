#!/bin/sh
./configure --arch=armv7 --cross-prefix=arm-none-linux-gnueabi- --enable-cross-compile --target-os=linux --enable-shared --disable-debug --extra-cflags='-DLINUX -O2 -mfloat-abi=softfp -mfpu=neon' --disable-avdevice   --disable-avdevice --disable-avcodec --disable-avformat  --disable-swscale  --disable-postproc --disable-avfilter
make all -j 8
make install DESTDIR=$PWD/output