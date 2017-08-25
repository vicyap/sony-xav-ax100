#!/bin/sh
./autogen.sh
./configure --host=arm-none-linux-gnueabi --enable-static=no --disable-doxygen-doc CFLAGS="-mthumb-interwork -mthumb" CPPFLAGS="-mthumb-interwork -mthumb"
make -j8 
make install DESTDIR=$PWD/arm/out
echo ######Build x86 version to generate thai dictionary######
./configure
make clean
make -j8
make install DESTDIR=$PWD/x86/out