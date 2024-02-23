#!/usr/bin/env bash

set -e
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

bold=$(tput bold)
normal=$(tput sgr0)

$SCRIPT_DIR/../tools/check_unstaged.sh || exit
export TOCK_NO_CHECK_UNSTAGED=1

function opt_rebuild {
	if [ "$CI" == "true" ]; then
		echo "${bold}Rebuilding Verbose: $1${normal}"
		make format V=1
	fi
}

echo ""
echo "${bold}Formatting examples${normal}"

for mkfile in `find . -maxdepth 4 -name Makefile`; do
	dir=`dirname $mkfile`
	if [ $dir == "." ]; then continue; fi
	# Skip directories with leading _'s, useful for leaving test apps around
	if [[ $(basename $dir) == _* ]]; then continue; fi

	pushd $dir > /dev/null
	echo ""
	echo "Formatting $dir"
	make format || (echo "${bold} ⤤ Failure formatting $dir${normal}" ; opt_rebuild $dir; exit 1)
	popd > /dev/null
done

echo ""
echo "${bold}All formatted.${normal}"
