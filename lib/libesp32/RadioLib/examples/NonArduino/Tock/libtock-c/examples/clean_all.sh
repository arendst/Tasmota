#!/usr/bin/env bash

bold=$(tput bold)
normal=$(tput sgr0)

set -e

for mkfile in `find . -maxdepth 4 -name Makefile`; do
	dir=`dirname $mkfile`
	if [ $dir == "." ]; then continue; fi
	pushd $dir > /dev/null
	make clean > /dev/null || echo "${bold} ⤤ $dir${normal}"
	popd > /dev/null
done

echo "${bold}All Clean.${normal}"
