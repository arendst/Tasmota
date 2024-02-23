#!/usr/bin/env bash

set -e
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

# The version we are currently using
UNCRUSTIFY_VERSION=0.75.1

if [ -x $SCRIPT_DIR/uncrustify-uncrustify-$UNCRUSTIFY_VERSION/build/uncrustify ]; then
  PATH="$SCRIPT_DIR/uncrustify-uncrustify-$UNCRUSTIFY_VERSION/build:$PATH"
fi

# Check if the right version is already installed
do_install=false
if ! command -v uncrustify >/dev/null; then
  do_install=true
else
  # Validate uncrustify version
  VERSION=$(uncrustify --version | egrep -o '0.[0-9]+[.0-9]*')
  if [[ "$VERSION" != $UNCRUSTIFY_VERSION ]]; then
    do_install=true
  fi
fi

# install uncrustify if it's missing
if $do_install; then
  echo "$(tput bold)"
  echo "INFO: uncrustify version $UNCRUSTIFY_VERSION not installed. Installing."
  echo "$(tput sgr0)(This will take a moment)"
  echo ""

  # Check that we can, cmake is probably the only thing missing
  if ! command -v cmake >/dev/null; then
    echo "$(tput bold) ERR: cmake not installed, required to build uncrustify$(tput sgr0)"
    echo ""
    echo "Please install either uncrustify version $UNCRUSTIFY_VERSION or cmake"
    exit 1
  fi

  pushd "$SCRIPT_DIR" > /dev/null

  echo " * Downloading sources..."
  wget -q https://github.com/uncrustify/uncrustify/archive/uncrustify-$UNCRUSTIFY_VERSION.tar.gz
  tar -xzf uncrustify-$UNCRUSTIFY_VERSION.tar.gz
  mkdir "uncrustify-uncrustify-$UNCRUSTIFY_VERSION/build"
  pushd "uncrustify-uncrustify-$UNCRUSTIFY_VERSION/build" > /dev/null

  echo " * Building..."
  cmake .. > /dev/null
  cmake --build . > /dev/null

  echo " * Done"
  popd > /dev/null
  popd > /dev/null

  PATH="$SCRIPT_DIR/uncrustify-uncrustify-$UNCRUSTIFY_VERSION/build:$PATH"
  echo ""
fi

set +e

COMMON_FLAGS="-c $SCRIPT_DIR/uncrustify.cfg"
exec uncrustify $COMMON_FLAGS "$@"
