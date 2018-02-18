#!/bin/bash
#
# Create platformio.ini links for all examples, if missing.
# Start this script from within the examples directory.

for d in *; do
  if [ -d "$d" -a -f "$d/$d.ino" -a ! -e "$d/platformio.ini" ]
  then
    ln -sv ../platformio.ini "$d/"
  fi
done

