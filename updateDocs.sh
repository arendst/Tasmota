#!/bin/sh

SCRIPTPATH="$(readlink -f "$0")"
SCRIPTPATH="${SCRIPTPATH%/*}"

rm -rf "$SCRIPTPATH/html"
echo -e "\n\n\n\n"
#cd "$SCRIPTPATH/" && doxygen && git add $DOCDIR && git commit -a -m "Documentation update."
cd "$SCRIPTPATH/" && doxygen
