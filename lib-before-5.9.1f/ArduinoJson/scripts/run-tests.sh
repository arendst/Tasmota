#!/bin/bash

FILE=../bin/ArduinoJsonTests.exe
MD5=""

file_changed() {
	[[ ! -f  "$FILE" ]] && return 1
	NEW_MD5=$(md5sum $FILE)
	[[ "$MD5" == "$NEW_MD5" ]] && return 1
	MD5=$NEW_MD5
	return 0
}

test_succeed() {
	echo -en "\007"{,}
}

test_failed() {
	echo -en "\007"{,,,,,,,,,,,}
}

run_tests() {
	$FILE
	case $? in
	0)
		test_succeed
		;;
	1)	
		test_failed
		;;
	esac
}

while true
do
	if file_changed
	then	
		run_tests
	else
		sleep 2
	fi
done


