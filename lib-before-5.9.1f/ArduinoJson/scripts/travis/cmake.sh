#!/bin/sh -ex

if [ $(uname) = 'Darwin' ]; then
	URL=https://cmake.org/files/v3.4/cmake-3.4.3-Darwin-x86_64.tar.gz
	CMAKE=/tmp/CMake.app/Contents/bin/cmake
	CTEST=/tmp/CMake.app/Contents/bin/ctest
else
	URL=https://cmake.org/files/v3.4/cmake-3.4.3-Linux-x86_64.tar.gz
	CMAKE=/tmp/bin/cmake
	CTEST=/tmp/bin/ctest
fi
curl -sS $URL | tar xz -C /tmp --strip 1

if [ -n "$GCC" ]; then
	export CC="gcc-$GCC"
	export CXX="g++-$GCC"
fi

if [ -n "$CLANG" ]; then
	export CC="clang-$CLANG"
	export CXX="clang++-$CLANG"
fi

if [ -n "$SANITIZE" ]; then
	export CXXFLAGS="-fsanitize=$SANITIZE"
fi

$CMAKE .
$CMAKE --build .
$CTEST -VV .
