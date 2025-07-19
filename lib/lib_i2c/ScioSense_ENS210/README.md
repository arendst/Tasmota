# ENS210
Arduino library for the ENS210 temperature & humidity sensor with I2C interface from ScioSense

## Introduction
This project is an Arduino *library*. It implements a driver with examples for the ENS210.
The ENS210 chip is a digital temperature & humidity sensor with an I2C interface.
The driver in this Arduino library is based on the code supplied by *Sciosense*, the manufacturer of the chip.

Note that the ENS210 requires a supply voltage of 1.71V .. 1.98V.

## Links
The ENS210 is made by [Sciosense](http://www.sciosense.com).
 - The datasheet of the ENS210 is not yet released

## Prerequisites
It is assumed that
 - The Arduino IDE has been installed.
   If not, refer to "Install the Arduino Desktop IDE" on the
   [Arduino site](https://www.arduino.cc/en/Guide/HomePage).
 - The library directory is at its default location.
   For me, Christoph, that is `C:\Users\christoph\Documents\Arduino\libraries`.

## Build an example
To build an example sketch
 - (Re)start Arduino.
 - Open File > Example > Examples from Custom Libraries > ENS210 > ENS210basic.
 - Make sure Tools > Board lists the correct board.
 - Select Sketch > Verify/Compile.