GENCCODE_ASSEMBLY_TYPE=-a gcc
SO=so
SOBJ=so
A=a
LIBPREFIX=lib
LIB_EXT_ORDER=.55.1
COMPILE=arm-none-linux-gnueabi-gcc -D_REENTRANT  -DU_HAVE_ELF_H=1 -DU_ENABLE_DYLOAD=0 -DU_HAVE_ATOMIC=0  -DU_ATTRIBUTE_DEPRECATED= -DU_HAVE_GCC_ATOMICS=0   -c
LIBFLAGS=-I/usr/local/include -DPIC -fPIC
GENLIB=arm-none-linux-gnueabi-gcc -DU_HAVE_GCC_ATOMICS=0    -shared -Wl,-Bsymbolic
LDICUDTFLAGS=-nodefaultlibs -nostdlib
LD_SONAME=-Wl,-soname -Wl,
RPATH_FLAGS=
BIR_LDFLAGS=-Wl,-Bsymbolic
AR=arm-linux-ar
ARFLAGS=r
RANLIB=arm-linux-ranlib
INSTALL_CMD=/usr/bin/install -c
