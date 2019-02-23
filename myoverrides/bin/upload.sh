#!/bin/bash

_esptool=/home/pusky/arduino-1.8.8/portable/packages/esp8266/tools/esptool/2.5.0-3-20ed2b9/esptool

$_esptool -vv -ce -bz 4M -cd nodemcu -cb 115200 -cp /dev/arduino -ca 0x00000 -cf $1

