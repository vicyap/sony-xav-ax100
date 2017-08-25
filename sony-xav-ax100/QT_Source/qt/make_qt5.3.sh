#!/bin/bash


export TOP="$PWD"

export OUTPATH=$TOP/out
export BASEPATH=$TOP/qtbase
export ENVPATH=$TOP/qtsunplus/prebuilt/usr/local
export BUILDPATH=$OUTPATH/build
export HOSTPATH=$OUTPATH/host
export TARGETPATH=$OUTPATH/target

export INSTALLPATH=/usr/local/qt

export PKG_CONFIG_PATH=$ENVPATH/lib/pkgconfig
export LD_LIBRARY_PATH=$ENVPATH/lib
export PKG_CONFIG_SYSROOT=$ENVPATH
export QT_SYSROOT=$TOP/qtsunplus/prebuilt/usr/local
export QMAKE_CFLAGS_DIRECTFB="-D_REENTRANT -I$ENVPTH/include/directfb -I$ENVPATH/include/freetype2"
export QMAKE_LIBS_DIRECTFB="-L$ENVPATH/lib -lfreetype -ldirect -ldirectfb -lfusion -lturbojpeg -ljpeg"
#export QMAKE_CFLAGS_FONTCONFIG="-I$ENVPATH/include -I$ENVPATH/include/freetype2 -I$ENVPATH/include/fontconfig"
#export QMAKE_LIBS_FONTCONFIG="-lfontconfig -lfreetype"

JOBS=`getconf _NPROCESSORS_ONLN`

function configure_qtbase(){
	mkdir -p $BUILDPATH/qtbase
	cd $BUILDPATH/qtbase && \
	($TOP/qtbase/configure -opensource -confirm-license\
	-prefix $TARGETPATH \
	-hostprefix $HOSTPATH \
	-device-option CROSS_COMPILE=arm-none-linux-gnueabi- \
	-device-option QT_SYSROOT="$TOP/qtsunplus/prebuilt/usr/local" \
	-device linux-arm-sunplus-8388 \
	-shared -no-static -directfb \
	-no-qml-debug -no-openssl \
	-qreal float -force-pkg-config -v \
	-gui -no-rpath -optimized-qmake -release \
	-dbus-linked -icu -qt-libpng -qt-libjpeg \
	-system-freetype -no-fontconfig \
	-widgets -system-zlib -no-mtdev \
	-no-sse2 -no-sse3 -no-ssse3 -no-sse4.1 -no-sse4.2 \
	-no-sql-db2 -no-sql-ibase -no-sql-mysql -no-sql-oci -no-sql-psql \
	-qt-sql-sqlite -no-sql-sqlite2 -no-sql-tds  -no-sql-ibase  -no-sql-odbc \
	-no-cups -no-glib -no-alsa -no-pulseaudio -no-xcb \
	-no-opengl -no-eglfs \
	-I$QT_SYSROOT/include/dbus-1.0 -I$QT_SYSROOT/lib/dbus-1.0/include)

}

function build_qtbase(){
	mkdir -p $BUILDPATH/qtbase
	cd $BUILDPATH/qtbase && \
	($TOP/qtbase/configure -opensource -confirm-license\
	-prefix $TARGETPATH \
	-hostprefix $HOSTPATH \
	-device-option CROSS_COMPILE=arm-none-linux-gnueabi- \
	-device-option QT_SYSROOT="$TOP/qtsunplus/prebuilt/usr/local" \
	-device linux-arm-sunplus-8388 \
	-shared -no-static -directfb \
	-no-qml-debug -no-openssl \
	-qreal float -force-pkg-config -v \
	-gui -no-rpath -optimized-qmake -release \
	-dbus-linked -icu -qt-libpng -qt-libjpeg \
	-system-freetype -no-fontconfig \
	-widgets -system-zlib -no-mtdev \
	-no-sse2 -no-sse3 -no-ssse3 -no-sse4.1 -no-sse4.2 \
	-no-sql-db2 -no-sql-ibase -no-sql-mysql -no-sql-oci -no-sql-psql \
	-qt-sql-sqlite -no-sql-sqlite2 -no-sql-tds  -no-sql-ibase  -no-sql-odbc \
	-no-cups -no-glib -no-alsa -no-pulseaudio -no-xcb \
	-no-opengl -no-eglfs \
	-I$QT_SYSROOT/include/dbus-1.0 -I$QT_SYSROOT/lib/dbus-1.0/include)	
	make -j$JOBS || exit "$?"
	make install
	echo "clean up useless files"
}

function build_qtmultimedia(){
	mkdir -p $BUILDPATH/qtmultimedia
	cd $BUILDPATH/qtmultimedia && \
    $HOSTPATH/bin/qmake $TOP/qtmultimedia/qtmultimedia.pro
	make -j$JOBS || exit "$?"
	make install
}

function cleanup_useless(){
	find $TARGETPATH -name *.la -exec rm -rf {} \;
	find $TARGETPATH -name *.prl -exec rm -rf {} \;
}

if [[ $1 == "a" ]];
then
	build_qtbase
	build_qtmultimedia
	cleanup_useless
	exit $?
elif [[ $1 == "c" ]];
then
	rm -rf out
	exit $?
elif [[ $1 == "t" ]];
then
	configure_qtbase
	exit $?
elif [[ $1 == "q" ]];
then
	exit 0
else
	echo "==============================================="
	echo "=    Build QT Package,please input parameter  ="
	echo "==============================================="
	echo "  a ==> build QT all                           "
	echo "  c ==> clean all (8+9)                        "
	echo "  q ==> exit                                   "
	echo "example: ./make_qt5.3.sh a"
fi
