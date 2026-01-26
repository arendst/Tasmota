# ScioSense ENS16x
Arduino library for the ENS160 and ENS161 digital four channel MOX gas sensor with I2C interface from ScioSense

## Introduction
This project is an Arduino *library*. It implements a driver with examples for the ENS160 and ENS161.
The ENS16x chip is a digital gas sensor for TVOC and eCO2 with an I2C interface.
The driver in this Arduino library is based on the code supplied by *Sciosense*, the manufacturer of the chip.

Note that the ENS16x requires a supply voltage of 1.71V .. 1.98V.
The ENS16x also requires a IO voltage of 1.71V .. 3.6V.

## Links
The ENS16x is made by [Sciosense](http://www.sciosense.com).
 - The datasheet of the ENS160 is available through the website. The datasheet of ENS161 is not yet released but can be provided on request

## Prerequisites
It is assumed that
 - The Arduino IDE has been installed.
   If not, refer to "Install the Arduino Desktop IDE" on the
   [Arduino site](https://www.arduino.cc/en/Guide/HomePage).
 - The library directory is at its default location.
   For me, that is `C:\Users\sciosense\Documents\Arduino\libraries`.

## Installation
- Visit the project page for the Arduino ENS16x library.
- Click the green button Clone or download on the right side.
- From the pop-up choose Download ZIP.
- In Arduino IDE, select Sketch > Include Library > Manage Libraries ... and browse to the just downloaded ZIP file.
- When the IDE is ready this README.md should be located at e.g. `C:\Users\sciosense\Documents\Arduino\libraries\ScioSense_ENS16x\README.md`.

## Build an example
To build an example sketch
 - (Re)start Arduino.
 - Open File > Example > Examples from Custom Libraries > ENS16x > ENS16xbasic_normal
 - Make sure Tools > Board lists the correct board.
 - Select Sketch > Verify/Compile.

### ScioSense is a Joint Venture of ams AG
