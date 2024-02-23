#!/bin/bash
set -e

rm -rf ./build

cd libtock-c/libtock
make -j4
cd ../../

mkdir -p build
cd build

cmake -G "CodeBlocks - Unix Makefiles" ..
make -j4

cd ..

elf2tab -n radio-lib --stack 4096 --app-heap 2048 --kernel-heap 2048 \
	--kernel-major 2 --kernel-minor 1 \
	-v ./build/tock-sx1261
