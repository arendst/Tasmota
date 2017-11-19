#!/bin/bash

export PATH="$PATH:/Applications/CMake.app/Contents/bin/"

cd $(dirname $0)/..
ROOT=$(pwd)

mkdir "build"
cd build
BUILD=$(pwd)

build-env()
{
	cd $BUILD
	mkdir "$1"
	cd "$1"
	cmake "$ROOT" -G "$2"
}

if [[ $(uname) == MINGW* ]]
then
	build-env "Make" "MinGW Makefiles"
	build-env "SublimeText" "Sublime Text 2 - Ninja"
	build-env "VisualStudio" "Visual Studio 14 2015"
else
	build-env "SublimeText" "Sublime Text 2 - Ninja"
	build-env "Make" "Unix Makefiles"
	build-env "Xcode" "Xcode"
fi
