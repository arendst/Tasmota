#!/usr/bin/env bash

bad=$(find . | grep Makefile | xargs grep -E 'C.*FLAGS[\ +:]*=' | grep -v override)

if [ -n "$bad" ]; then
  echo "Found flags variable assignment without override directive."
  echo
  echo "The Tock build system uses the override directive on all flags variables."
  echo "This lets users add runtime flags without inadvertently trampling flags"
  echo "that are required to build correctly, however it means that any flags set"
  echo "without override will be ignored. To fix, simply prefix your flags assignment"
  echo "with 'override', as in 'override CFLAGS += -DMY_FLAG=1'".
  echo
  echo "For more on the override directive, see the make documentation:"
  echo "https://www.gnu.org/software/make/manual/html_node/Override-Directive.html"
  echo
  echo "The following lines set flags without override:"
  echo "$bad"
  echo
  exit 1
fi
