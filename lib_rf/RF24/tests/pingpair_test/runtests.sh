#!/bin/sh

# Connect u0 to receiver, u0 to sender
# WARNING: Test config 2 only works with PLUS units.

jam u0 u1 && expect test.ex 1
sleep 1
stty 57600 raw ignbrk hup < /dev/ttyUSB0
sleep 1
stty 57600 raw ignbrk hup < /dev/ttyUSB1
expect test.ex 2
sleep 1
stty 57600 raw ignbrk hup < /dev/ttyUSB0
sleep 1
stty 57600 raw ignbrk hup < /dev/ttyUSB1
expect test.ex 3
sleep 1
stty 57600 raw ignbrk hup < /dev/ttyUSB0
sleep 1
stty 57600 raw ignbrk hup < /dev/ttyUSB1
expect test.ex 4
