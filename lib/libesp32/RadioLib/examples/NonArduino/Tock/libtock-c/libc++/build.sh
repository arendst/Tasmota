#!/usr/bin/env bash

GCC_SRC_DIR=$1

NEWLIB_VERSION=4.1.0

SCRIPTPATH=$( cd $(dirname $0) ; pwd -P )
NEWLIB_INCLUDE_PATH=$SCRIPTPATH/../newlib/newlib-$NEWLIB_VERSION/newlib/libc/include

if [[ ! -e $NEWLIB_INCLUDE_PATH ]]; then
  echo "ERROR: Missing NEWLIB_INCLUDE_PATH, expected"
  echo "       $NEWLIB_INCLUDE_PATH"
  echo ""
  echo "Ensure that appropriate newlib version has been built before building libc++"
  exit 1
fi

export CFLAGS_FOR_TARGET="-g -Os -ffunction-sections -fdata-sections -fPIC -msingle-pic-base -mno-pic-data-is-text-relative -mthumb -mcpu=cortex-m0 -isystem $NEWLIB_INCLUDE_PATH"
export CXXFLAGS_FOR_TARGET="-g -Os -ffunction-sections -fdata-sections -fPIC -msingle-pic-base -mno-pic-data-is-text-relative -mthumb -mcpu=cortex-m0 -isystem $NEWLIB_INCLUDE_PATH"

if gcc --version | grep -q clang; then
  echo "$(tput bold)System gcc is clang. Overriding with gcc-11"
  echo "$(tput sgr0)You may need to brew install gcc@11 if you haven't."
  echo ""
  sleep 2
  export CC=gcc-11
  export CXX=g++-11

  gmp=$($CC -v 2>&1 | tr " " "\n" | grep gmp)
  mpfr=$($CC -v 2>&1 | tr " " "\n" | grep mpfr)
  mpc=$($CC -v 2>&1 | tr " " "\n" | grep mpc)
  isl=$($CC -v 2>&1 | tr " " "\n" | grep isl)
  extra_with="$gmp $mpfr $mpc $isl"
else
  extra_with=""
fi

$GCC_SRC_DIR/configure \
  --build=x86_64-linux-gnu \
  --host=x86_64-linux-gnu \
  --target=arm-none-eabi \
  --with-cpu=cortex-m0 \
  --with-mode=thumb \
  --disable-fpu \
  --with-newlib $extra_with \
  --with-headers=$NEWLIB_INCLUDE_PATH \
  --enable-languages="c c++" \

make -j$(nproc)
