#!/bin/sh
export DESTDIR=$PWD/prebuilt
./configure --host=arm-none-linux-gnueabi --disable-x11 --disable-voodoo --disable-mmx --disable-sse --disable-sdl --with-gfxdrivers=none --enable-fbdev=yes --enable-devmem --enable-debug=no --enable-static=no --enable-debug-support=no --enable-trace=no -with-inputdrivers=tslib --enable-silent-rules PKG_CONFIG_PATH=$DESTDIR/usr/local/lib/pkgconfig LDFLAGS="-L$DESTDIR/usr/local/lib -lturbojpeg" CFLAGS="-I$DESTDIR/usr/local/include -I$DESTDIR/usr/local/include/freetype2 -mthumb-interwork -mthumb" CPPFLAGS="-mthumb-interwork -mthumb" TSLIB_LIBS="-L$DESTDIR/usr/local/lib -lts" TSLIB_CFLAGS="-I$DESTDIR/usr/local/include" LIBPNG_CFLAGS=$CFLAGS FREETYPE_CFLAGS=$CFLAGS LIBPNG_LIBS="$LDFLAGS -lpng -lz" FREETYPE_LIBS="$LDFLAGS -lfreetype -lpng -lz" LIBJPEG="$LDFLAGS -lturbojpeg -ljpeg" 
make -j8 
make install DESTDIR=$PWD/output
