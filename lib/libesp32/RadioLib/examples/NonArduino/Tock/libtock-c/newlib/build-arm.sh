#!/usr/bin/env bash

NEWLIB_SRC_DIR=$1

$NEWLIB_SRC_DIR/configure --target=arm-none-eabi \
  --disable-newlib-supplied-syscalls \
  --disable-nls \
  --enable-newlib-reent-small \
  --disable-newlib-fvwrite-in-streamio \
  --disable-newlib-fseek-optimization \
  --disable-newlib-wide-orient \
  --enable-newlib-nano-malloc \
  --disable-newlib-unbuf-stream-opt \
  --enable-lite-exit \
  --enable-newlib-global-atexit \
  --enable-newlib-nano-formatted-io

make -j$(nproc) CFLAGS_FOR_TARGET='-g -Os -ffunction-sections -fdata-sections -fPIC -msingle-pic-base -mno-pic-data-is-text-relative'
