#!/usr/bin/env bash

NUM_JOBS=$(nproc 2>/dev/null || sysctl -n hw.ncpu 2>/dev/null || 4)

set -e
set -u
set -o pipefail

bold=$(tput bold)
normal=$(tput sgr0)
red=$(tput setaf 1)

# Try to build everything and collect failures at the end
declare -a failures

function opt_rebuild {
	if [ "${CI-}" == "true" ]; then
		echo "${bold}Rebuilding Verbose: $1${normal}"
		make CFLAGS=-Werror V=1
	fi
}

for mkfile in `find . -maxdepth 6 -name Makefile`; do
	dir=`dirname $mkfile`
	if [ $dir == "." ]; then continue; fi
	# Skip directories with leading _'s, useful for leaving test apps around
	if [[ $(basename $dir) == _* ]]; then continue; fi

	pushd $dir > /dev/null
	echo ""
	echo "Building $dir"
	make CFLAGS=-Werror -j $NUM_JOBS || { echo "${bold} ⤤ Failure building $dir${normal}" ; opt_rebuild $dir; failures+=("$dir"); }
	popd > /dev/null
done

# https://stackoverflow.com/questions/7577052/bash-empty-array-expansion-with-set-u
if [[ ${failures[@]+"${failures[@]}"} ]]; then
	echo ""
	echo "${bold}${red}Build Failures:${normal}"
	for fail in ${failures[@]}; do
		echo $fail
	done
	exit 1
fi

echo ""
echo "${bold}All Built.${normal}"
