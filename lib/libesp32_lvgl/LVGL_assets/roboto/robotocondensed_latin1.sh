#!/bin/bash
set -x #echo on

rm robotocondensed_latin1.tapp
zip -j -0 robotocondensed_latin1.zip autoexec.be robotocondensed_latin1.be robotocondensed_latin1_12.bin robotocondensed_latin1_14.bin robotocondensed_latin1_16.bin robotocondensed_latin1_24.bin robotocondensed_latin1_32.bin
mv robotocondensed_latin1.zip robotocondensed_latin1.tapp
