#!/bin/bash

TAG=$(git describe)
OUTPUT="ArduinoJson-$TAG.zip"

cd $(dirname $0)/../..

# remove existing file
rm -f $OUTPUT

# create zip
7z a $OUTPUT \
	ArduinoJson/CHANGELOG.md \
	ArduinoJson/examples \
	ArduinoJson/include \
	ArduinoJson/keywords.txt \
	ArduinoJson/library.properties \
	ArduinoJson/LICENSE.md \
	ArduinoJson/README.md \
	ArduinoJson/ArduinoJson.h
