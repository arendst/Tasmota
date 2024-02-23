#!/usr/bin/env python2

import time
from sys import argv
import os
import sys

APP_BASE_ADDR = '0x20000'

argv.pop(0)
if len(argv) == 0:
    print("No input files")
    sys.exit()

# read in file images
img = ""
build_dir = ""
for filename in argv:
    f = open(filename)
    img = img + f.read()
    f.close()

    if build_dir == "":
        build_dir = os.path.dirname(filename)

print("Writing %d files totaling %d bytes" % (len(argv), len(img)))

# Padding zeroes at the end
img = img + '\x00\x00\x00\x00\x00\x00\x00\x00'

# Write to output file so we can flash it
with open(build_dir+'/app_bundle.bin', 'w') as app_bin:
    app_bin.write(img)

# create jlink script
with open(build_dir+'/flash-app.jtag', 'w') as jlink_file:
    jlink_file.write('eoe 1\n');
    jlink_file.write('r\n');
    jlink_file.write('loadbin ' + build_dir + '/app_bundle.bin, ' + APP_BASE_ADDR + '\n');
    jlink_file.write('verifybin ' + build_dir + '/app_bundle.bin, ' + APP_BASE_ADDR + '\n');
    jlink_file.write('r\ng\nq\n');

# Flash apps with JLink
then = time.time()
ret = os.system('JLinkExe -device nrf51422 -if swd -speed 1200 -AutoConnect 1 ' + build_dir+'/flash-app.jtag')
if ret != 0:
    print("Error loading apps")
    sys.exit(1)

now = time.time()
print("Wrote %d bytes in %.3f seconds" %(len(img), now-then))

