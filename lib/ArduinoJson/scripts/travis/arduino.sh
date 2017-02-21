#!/bin/sh -eux

/sbin/start-stop-daemon --start --quiet --pidfile /tmp/custom_xvfb_1.pid --make-pidfile --background --exec /usr/bin/Xvfb -- :1 -ac -screen 0 1280x1024x16
sleep 3
export DISPLAY=:1.0

mkdir -p /tmp/arduino
curl -sS http://downloads.arduino.cc/arduino-$VERSION-linux64.tar.xz | tar xJ -C /tmp/arduino --strip 1 ||
curl -sS http://downloads.arduino.cc/arduino-$VERSION-linux64.tgz | tar xz -C /tmp/arduino --strip 1 
export PATH=$PATH:/tmp/arduino/
  
ln -s $PWD /tmp/arduino/libraries/ArduinoJson

for EXAMPLE in $PWD/examples/*/*.ino; do
	arduino --verify --board $BOARD $EXAMPLE
done
