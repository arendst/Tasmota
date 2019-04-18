#!/bin/bash
#
# Create platformio.ini templates for all examples, if missing.
# Start this script from within the examples directory.

for d in *; do
  if [ -d "$d" -a -f "$d/$d.ino" -a ! -e "$d/platformio.ini" ]
  then
    cp -av platformio.ini "$d/"
  fi
done

