#!/bin/sh
if [ ! -n "$1" ] ;then  
    echo "please input your Libdatrie source code path"  
    exit 0
else  
    echo "your Libdatrie source code path is $1"  
fi  
export PATH=$PATH:$1/x86/out/usr/local/bin
export DESTDIR=$1
echo $PATH
./autogen.sh
./configure --host=arm-none-linux-gnueabi --enable-static=no --disable-doxygen-doc CFLAGS='-mthumb-interwork -mthumb -L${DESTDIR}/arm/out/usr/local/lib' CPPFLAGS='-mthumb-interwork -mthumb' DATRIE_CFLAGS="-I${DESTDIR}/arm/out/usr/local/include" DATRIE_LIBS="-L${DESTDIR}/arm/out/usr/local/lib -ldatrie"
make -j8
make install DESTDIR=$PWD/out
