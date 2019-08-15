# VL53L0X library for Arduino

Version: 1.0.2<br>
Release date: 2017 Jun 27<br>
[![Build Status](https://travis-ci.org/pololu/vl53l0x-arduino.svg?branch=master)](https://travis-ci.org/pololu/vl53l0x-arduino)<br>
[www.pololu.com](https://www.pololu.com/)

## Summary

This is a library for the Arduino IDE that helps interface with ST's [VL53L0X time-of-flight distance sensor](https://www.pololu.com/product/2490). The library makes it simple to configure the sensor and read range data from it via I&sup2;C.

## Supported platforms

This library is designed to work with the Arduino IDE versions 1.6.x or later; we have not tested it with earlier versions.  This library should support any Arduino-compatible board, including the [Pololu A-Star 32U4 controllers](https://www.pololu.com/category/149/a-star-programmable-controllers).

## Getting started

### Hardware

A [VL53L0X carrier](https://www.pololu.com/product/2490) can be purchased from Pololu's website.  Before continuing, careful reading of the [product page](https://www.pololu.com/product/2490) as well as the VL53L0X datasheet is recommended.

Make the following connections between the Arduino and the VL53L0X board:

#### 5V Arduino boards

(including Arduino Uno, Leonardo, Mega; Pololu A-Star 32U4)

    Arduino   VL53L0X board
    -------   -------------
         5V - VIN
        GND - GND
        SDA - SDA
        SCL - SCL

#### 3.3V Arduino boards

(including Arduino Due)

    Arduino   VL53L0X board
    -------   -------------
        3V3 - VIN
        GND - GND
        SDA - SDA
        SCL - SCL

### Software

If you are using version 1.6.2 or later of the [Arduino software (IDE)](http://www.arduino.cc/en/Main/Software), you can use the Library Manager to install this library:

1. In the Arduino IDE, open the "Sketch" menu, select "Include Library", then "Manage Libraries...".
2. Search for "VL53L0X".
3. Click the VL53L0X entry in the list.
4. Click "Install".

If this does not work, you can manually install the library:

1. Download the [latest release archive from GitHub](https://github.com/pololu/vl53l0x-arduino/releases) and decompress it.
2. Rename the folder "vl53l0x-arduino-master" to "VL53L0X".
3. Move the "VL53L0X" folder into the "libraries" directory inside your Arduino sketchbook directory.  You can view your sketchbook location by opening the "File" menu and selecting "Preferences" in the Arduino IDE.  If there is not already a "libraries" folder in that location, you should make the folder yourself.
4. After installing the library, restart the Arduino IDE.

## Examples

Several example sketches are available that show how to use the library. You can access them from the Arduino IDE by opening the "File" menu, selecting "Examples", and then selecting "VL53L0X". If you cannot find these examples, the library was probably installed incorrectly and you should retry the installation instructions above.

## ST's VL53L0X API and this library

Most of the functionality of this library is based on the [VL53L0X API](http://www.st.com/content/st_com/en/products/embedded-software/proximity-sensors-software/stsw-img005.html) provided by ST (STSW-IMG005), and some of the explanatory comments in the code are quoted or paraphrased from the API source code, API user manual (UM2039), and the VL53L0X datasheet. For more explanation about the library code and how it was derived from the API, see the comments in VL53L0X.cpp.

This library is intended to provide a quicker and easier way to get started using the VL53L0X with an Arduino-compatible controller, in contrast to customizing and compiling ST's API for the Arduino. The library has a more streamlined interface, as well as smaller storage and memory footprints. However, it does not implement some of the more advanced functionality available in the API (for example, calibrating the sensor to work well under a cover glass), and it has less robust error checking. For advanced applications, especially when storage and memory are less of an issue, consider using the VL53L0X API directly.

## Library reference

* `uint8_t last_status`<br>
  The status of the last I&sup2;C write transmission. See the [`Wire.endTransmission()` documentation](http://arduino.cc/en/Reference/WireEndTransmission) for return values.

* `VL53L0X(void)`<br>
  Constructor.

* `void setAddress(uint8_t new_addr)`<br>
  Changes the I&sup2;C slave device address of the VL53L0X to the given value (7-bit).

* `uint8_t getAddress(void)`<br>
  Returns the current I&sup2;C address.

* `bool init(bool io_2v8 = true)`<br>
  Iniitializes and configures the sensor. If the optional argument `io_2v8` is true (the default if not specified), the sensor is configured for 2V8 mode (2.8 V I/O); if false, the sensor is left in 1V8 mode. The return value is a boolean indicating whether the initialization completed successfully. 

* `void writeReg(uint8_t reg, uint8_t value)`<br>
  Writes an 8-bit sensor register with the given value.

  Register address constants are defined by the regAddr enumeration type in VL53L0X.h.<br>
  Example use: `sensor.writeReg(VL53L0X::SYSRANGE_START, 0x01);`

* `void writeReg16Bit(uint8_t reg, uint16_t value)`<br>
  Writes a 16-bit sensor register with the given value.

* `void writeReg32Bit(uint8_t reg, uint32_t value)`<br>
  Writes a 32-bit sensor register with the given value.

* `uint8_t readReg(uint8_t reg)`<br>
  Reads an 8-bit sensor register and returns the value read.

* `uint16_t readReg16Bit(uint8_t reg)`<br>
  Reads a 16-bit sensor register and returns the value read.

* `uint32_t readReg32Bit(uint8_t reg)`<br>
  Reads a 32-bit sensor register and returns the value read.

* `void writeMulti(uint8_t reg, uint8_t const * src, uint8_t count)`<br>
  Writes an arbitrary number of bytes from the given array to the sensor, starting at the given register.

* `void readMulti(uint8_t reg, uint8_t * dst, uint8_t count)`<br>
  Reads an arbitrary number of bytes from the sensor, starting at the given register, into the given array.

* `bool setSignalRateLimit(float limit_Mcps)`<br>
  Sets the return signal rate limit to the given value in units of MCPS (mega counts per second). This is the minimum amplitude of the signal reflected from the target and received by the sensor necessary for it to report a valid reading. Setting a lower limit increases the potential range of the sensor but also increases the likelihood of getting an inaccurate reading because of reflections from objects other than the intended target. This limit is initialized to 0.25 MCPS by default. The return value is a boolean indicating whether the requested limit was valid.

* `float getSignalRateLimit(void)`<br>
  Returns the current return signal rate limit in MCPS.

* `bool setMeasurementTimingBudget(uint32_t budget_us)`<br>
  Sets the measurement timing budget to the given value in microseconds. This is the time allowed for one range measurement; a longer timing budget allows for more accurate measurements. The default budget is about 33000 microseconds, or 33 ms; the minimum is 20 ms. The return value is a boolean indicating whether the requested budget was valid.

* `uint32_t getMeasurementTimingBudget(void)`<br>
  Returns the current measurement timing budget in microseconds.

* `bool setVcselPulsePeriod(vcselPeriodType type, uint8_t period_pclks)`
  Sets the VCSEL (vertical cavity surface emitting laser) pulse period for the given period type (`VL53L0X::VcselPeriodPreRange` or `VL53L0X::VcselPeriodFinalRange`) to the given value (in PCLKs). Longer periods increase the potential range of the sensor. Valid values are (even numbers only):

  Pre: 12 to 18 (initialized to 14 by default)<br>
  Final: 8 to 14 (initialized to 10 by default)

  The return value is a boolean indicating whether the requested period was valid.

* `uint8_t getVcselPulsePeriod(vcselPeriodType type)`<br>
  Returns the current VCSEL pulse period for the given period type.

* `void startContinuous(uint32_t period_ms = 0)`<br>
  Starts continuous ranging measurements. If the optional argument `period_ms` is 0 (the default if not specified), continuous back-to-back mode is used (the sensor takes measurements as often as possible); if it is nonzero, continuous timed mode is used, with the specified inter-measurement period in milliseconds determining how often the sensor takes a measurement.

* `void stopContinuous(void)`<br>
  Stops continuous mode.

* `uint16_t readRangeContinuousMillimeters(void)`<br>
  Returns a range reading in millimeters when continuous mode is active.

* `uint16_t readRangeSingleMillimeters(void)`<br>
  Performs a single-shot ranging measurement and returns the reading in millimeters.

* `void setTimeout(uint16_t timeout)`<br>
  Sets a timeout period in milliseconds after which read operations will abort if the sensor is not ready. A value of 0 disables the timeout.

* `uint16_t getTimeout(void)`<br>
  Returns the current timeout period setting.

* `bool timeoutOccurred(void)`<br>
  Indicates whether a read timeout has occurred since the last call to `timeoutOccurred()`.

## Version history

* 1.0.2 (2017 Jun 27): Fixed a typo in a register modification in `getSpadInfo()` (thanks @tridge).
* 1.0.1 (2016 Dec 08): Fixed type error in `readReg32Bit()`.
* 1.0.0 (2016 Aug 12): Original release.
