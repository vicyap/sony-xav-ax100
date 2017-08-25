#!/bin/sh
./configure --host=arm-none-linux-gnueab --disable-udev --disable-debug-log --disable-timerfd 
make -j8 
make install DESTDIR=$PWD/output
