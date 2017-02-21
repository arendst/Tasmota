#!/bin/bash

set -eu

OUTPUT="$(pwd)/sizes.csv"

echo "Tag;Date;Parser;Generator" > $OUTPUT

cd $(dirname $(dirname $0))

git tag | while read TAG
do 

	git checkout -q tags/$TAG

	DATE=$(git log -1 --date=short --pretty=format:%cd)
	PARSER_SIZE=$(arduino --verify examples/JsonParserExample/JsonParserExample.ino 2>/dev/null  | grep -e 'Sketch uses' | sed 's/.*uses \([0-9]*\).\([0-9]\+\).*/\1\2/')
	
	if [ -e 'examples/JsonGeneratorExample/JsonGeneratorExample.ino' ]; then
		GENERATOR_SIZE=$(arduino --verify examples/JsonGeneratorExample/JsonGeneratorExample.ino 2>/dev/null  | grep -e 'Sketch uses' | sed 's/.*uses \([0-9]*\).\([0-9]\+\).*/\1\2/')
	else
		GENERATOR_SIZE=""
	fi

	echo $TAG 
	if [ ! -z "$PARSER_SIZE" ]
	then
		echo "JsonParserExample = $PARSER_SIZE bytes"
	else
		echo "JsonParserExample compilation failed."
	fi

	if [ ! -z "$GENERATOR_SIZE" ]
	then
		echo "JsonGeneratorExample = $GENERATOR_SIZE bytes"
	else
		echo "JsonGeneratorExample compilation failed."
	fi

	echo "$TAG;$DATE;$PARSER_SIZE;$GENERATOR_SIZE" >> $OUTPUT

done
