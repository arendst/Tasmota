# ScioSense ENS210 
Arduino library for the ENS210 temperature & humidity sensor with I2C interface from ScioSense
It is based on a library written by Maarten Pennings. Thanks for all the work.
https://github.com/maarten-pennings/ENS210

## Introduction
This project is an Arduino *library*. It implements a driver with examples for the ENS210. The ENS210 chip is a digital temperature & humidity sensor with an I2C interface.

The driver in this Arduino library is based on the code supplied by *Sciosense*, the manufacturer of the chip.

Note that the ENS210 requires a supply voltage of 1.71V .. 1.98V.

## Links
The ENS210 is made by [Sciosense](http://www.sciosense.com).
 - The datasheet and further documents regading the ENS210 can be downloaded here
   https://www.sciosense.com/products/relative-humidity-and-temperature-sensors/ens210-relative-humidity-and-temperature-sensor/
   
## Prerequisites
It is assumed that
 - The Arduino IDE has been installed.
   If not, refer to "Install the Arduino Desktop IDE" on the
   [Arduino site](https://www.arduino.cc/en/Guide/HomePage).
 - The library directory is at its default location.
   For me, that is `C:\Users\sciosense\Documents\Arduino\libraries`.

## Installation
Installation steps
- Visit the project page for the Arduino CCS811 library.
- Click the green button Clone or download on the right side.
- From the pop-up choose Download ZIP.
- In Arduino IDE, select Sketch > Include Library > Manage Libraries ... and browse to the just downloaded ZIP file.
- When the IDE is ready this README.md should be located at e.g. `C:\Users\sciosense\Documents\Arduino\libraries\sciosense_ens210\README.md`.

## Build an example
To build an example sketch
 - (Re)start Arduino.
 - Open File > Example > Examples from Custom Libraries > ENS210 > ENS210basic.
 - Make sure Tools > Board lists the correct board.
 - Select Sketch > Verify/Compile.
 
### ScioSense is a Joint Venture of ams AG 
