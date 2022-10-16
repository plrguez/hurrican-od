#! /bin/sh

BASEDIR=$(cd $(dirname "$0") && pwd);

[ -z "$TOOLCHAIN" ] && TOOLCHAIN=/opt/gcw0-toolchain/usr/bin
LIBMODPLUG_BASEDIR="$BASEDIR"/libmodplug
LIBMODPLUG_REVISION=d1b97ed

[ -d "$LIBMODPLUG_BASEDIR" ] || git clone https://github.com/Konstanty/libmodplug.git "$LIBMODPLUG_BASEDIR"

cd "$LIBMODPLUG_BASEDIR"
git checkout $LIBMODPLUG_REVISION
aclocal
autoconf
libtoolize
automake --add-missing

export PATH="$TOOLCHAIN:$PATH"
./configure CC=mipsel-gcw0-linux-uclibc-gcc --host=mipsel-gcw0-linux-uclibc CXXFLAGS='-O2 -mips32r2' --prefix='/tmp/libmodplug-install'
make clean && make

[ -f "$BASEDIR"/data/lib/libmodplug.so.1 ] && rm "$BASEDIR"/data/lib/libmodplug.so.1
cp "$LIBMODPLUG_BASEDIR"/src/.libs/libmodplug.so.1.0.0 "$BASEDIR"/data/lib/libmodplug.so.1
