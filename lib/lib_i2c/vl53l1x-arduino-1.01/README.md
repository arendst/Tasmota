# VL53L1X library for Arduino

Version: 1.0.1<br>
Release date: 2018-09-19<br>
[![Build Status](https://travis-ci.org/pololu/vl53l1x-arduino.svg?branch=master)](https://travis-ci.org/pololu/vl53l1x-arduino)<br>
[www.pololu.com](https://www.pololu.com/)

## Summary

This is a library for the Arduino IDE that helps interface with ST's [VL53L1X time-of-flight distance sensor](https://www.pololu.com/product/3415). The library makes it simple to configure the sensor and read range data from it via I&sup2;C.

## Supported platforms

This library is designed to work with the Arduino IDE versions 1.6.x or later; we have not tested it with earlier versions.  This library should support any Arduino-compatible board, including the [Pololu A-Star controllers](https://www.pololu.com/category/149/a-star-programmable-controllers).

## Getting started

### Hardware

A [VL53L1X carrier](https://www.pololu.com/product/3415) can be purchased from Pololu's website.  Before continuing, careful reading of the [product page](https://www.pololu.com/product/3415) as well as the VL53L1X datasheet is recommended.

Make the following connections between the Arduino and the VL53L1X board:

#### 5V Arduino boards

(including Arduino Uno, Leonardo, Mega; Pololu A-Star 32U4)

    Arduino   VL53L1X board
    -------   -------------
         5V - VIN
        GND - GND
        SDA - SDA
        SCL - SCL

#### 3.3V Arduino boards

(including Arduino Due)

    Arduino   VL53L1X board
    -------   -------------
        3V3 - VIN
        GND - GND
        SDA - SDA
        SCL - SCL

### Software

If you are using version 1.6.2 or later of the [Arduino software (IDE)](http://www.arduino.cc/en/Main/Software), you can use the Library Manager to install this library:

1. In the Arduino IDE, open the "Sketch" menu, select "Include Library", then "Manage Libraries...".
2. Search for "VL53L1X".
3. Click the VL53L1X entry in the list.
4. Click "Install".

If this does not work, you can manually install the library:

1. Download the [latest release archive from GitHub](https://github.com/pololu/vl53l1x-arduino/releases) and decompress it.
2. Rename the folder "vl53l1x-arduino-master" to "VL53L1X".
3. Move the "VL53L1X" folder into the "libraries" directory inside your Arduino sketchbook directory.  You can view your sketchbook location by opening the "File" menu and selecting "Preferences" in the Arduino IDE.  If there is not already a "libraries" folder in that location, you should make the folder yourself.
4. After installing the library, restart the Arduino IDE.

## Examples

Several example sketches are available that show how to use the library. You can access them from the Arduino IDE by opening the "File" menu, selecting "Examples", and then selecting "VL53L1X". If you cannot find these examples, the library was probably installed incorrectly and you should retry the installation instructions above.

## ST's VL53L1X API and this library

Most of the functionality of this library is based on the [VL53L1X API](http://www.st.com/content/st_com/en/products/embedded-software/proximity-sensors-software/stsw-img007.html) provided by ST (STSW-IMG007), and some of the explanatory comments in the code are quoted or paraphrased from the API source code, API user manual (UM2356), and the VL53L1X datasheet. For more explanation about the library code and how it was derived from the API, see the comments in VL53L1X.cpp.

This library is intended to provide a quicker and easier way to get started using the VL53L1X with an Arduino-compatible controller, in contrast to using ST's API on the Arduino. The library has a more streamlined interface, as well as smaller storage and memory footprints. However, it does not currently implement some of the more advanced functionality available in the API (for example, calibrating the sensor to work well under a cover glass or selecting a smaller region of interest (ROI)), and it has less robust error checking. For advanced applications, especially when storage and memory are less of an issue, consider using the VL53L1X API directly. We have an [implementation of ST's VL53L1X API for Arduino](https://github.com/pololu/vl53l1x-st-api-arduino) available.

## Library reference

* `RangingData ranging_data`<br>
  This struct contains information about the last ranging measurement. Its members are:
  * `uint16_t range_mm`<br>
    Range reading from the last measurement, in millimeters. (This reading can also be obtained as the return value of `read()`.)
  * `RangeStatus range_status`<br>
    Status of the last measurement; see the definition of the `RangeStatus` enumeration type in VL53L1X.h (or the API user manual and source code) for descriptions of the possible statuses. A status of `VL53L1X::RangeValid` means there were no problems with the measurement.
  * `float peak_signal_count_rate_MCPS`<br>
    Peak signal count rate of the last measurement, in units of mega counts per second.
  * `float ambient_count_rate_MCPS`<br>
    Ambient count rate of the last measurement, in units of mega counts per second.

* `uint8_t last_status`<br>
  The status of the last I&sup2;C write transmission. See the [`Wire.endTransmission()` documentation](http://arduino.cc/en/Reference/WireEndTransmission) for return values.

* `VL53L1X()`<br>
  Constructor.

* `void setAddress(uint8_t new_addr)`<br>
  Changes the I&sup2;C slave device address of the VL53L1X to the given value (7-bit).

* `uint8_t getAddress()`<br>
  Returns the current I&sup2;C address.

* `bool init(bool io_2v8 = true)`<br>
  Iniitializes and configures the sensor. If the optional argument `io_2v8` is true (the default if not specified), the sensor is configured for 2V8 mode (2.8 V I/O); if false, the sensor is left in 1V8 mode. The return value is a boolean indicating whether the initialization completed successfully.

* `void writeReg(uint16_t reg, uint8_t value)`<br>
  Writes an 8-bit sensor register with the given value.

  Register address constants are defined by the `regAddr` enumeration type in VL53L1X.h.<br>
  Example use: `sensor.writeReg(VL53L1X::SOFT_RESET, 0x00);`

* `void writeReg16Bit(uint16_t reg, uint16_t value)`<br>
  Writes a 16-bit sensor register with the given value.

* `void writeReg32Bit(uint16_t reg, uint32_t value)`<br>
  Writes a 32-bit sensor register with the given value.

* `uint8_t readReg(uint16_t reg)`<br>
  Reads an 8-bit sensor register and returns the value read.

* `uint16_t readReg16Bit(uint16_t reg)`<br>
  Reads a 16-bit sensor register and returns the value read.

* `uint32_t readReg32Bit(uint16_t reg)`<br>
  Reads a 32-bit sensor register and returns the value read.

* `bool setDistanceMode(DistanceMode mode)`<br>
  Sets the distance mode of the sensor (`VL53L1X::Short`, `VL53L1X::Medium`, or `VL53L1X::Long`). Shorter distance modes are less affected by ambient light but have lower maximum ranges. See the datasheet for more information. The return value is a boolean indicating whether the requested mode was valid.

* `DistanceMode getDistanceMode()`<br>
  Returns the previously set distance mode.

* `bool setMeasurementTimingBudget(uint32_t budget_us)`<br>
  Sets the measurement timing budget to the given value in microseconds. This is the time allowed for one range measurement; a longer timing budget allows for more accurate measurements. The minimum budget is 20 ms (20000 us) in short distance mode and 33 ms for medium and long distance modes. See the VL53L1X datasheet for more information on range and timing limits. The return value is a boolean indicating whether the requested budget was valid.

* `uint32_t getMeasurementTimingBudget()`<br>
  Returns the current measurement timing budget in microseconds.

* `void startContinuous(uint32_t period_ms)`<br>
  Starts continuous ranging measurements. The specified inter-measurement period in milliseconds determines how often the sensor takes a measurement; if it is shorter than the timing budget, the sensor will start a new measurement as soon as the previous one finishes.

* `void stopContinuous()`<br>
  Stops continuous mode.

* `uint16_t read(bool blocking = true)`<br>
  After continuous ranging measurements have been started, calling this function returns a range reading in millimeters and updates the `ranging_data` struct with details about the last measurement. If the optional argument `blocking` is true (the default if not specified), this function will wait until data from a new measurement is available before returning.

  If you do not want this function to block, you can use the `dataReady()` function to check if new data is available before calling `read(false)`. Calling `read(false)` before new data is available will return a reading of 0, and `ranging_data.range_status` will have the value `VL53L1X::None`, indicating that there has been no update.

* `uint16_t readRangeContinuousMillimeters(bool blocking = true)`<br>
  Alias of `read()` for convenience.

* `bool dataReady()`<br>
  Returns a boolean indicating whether data from a new measurement is available from the sensor.

* `static const char * rangeStatusToString(RangeStatus status)`<br>
  Converts a `RangeStatus` into a readable string describing that status.
  
  Note that on an AVR, the strings in this function are stored in RAM (dynamic memory), which makes working with them easier but uses up 200+ bytes of RAM (many AVR-based Arduinos only have about 2000 bytes of RAM). You can avoid this memory usage if you do not call this function in your sketch.

* `void setTimeout(uint16_t timeout)`<br>
  Sets a timeout period in milliseconds after which read operations will abort if the sensor is not ready. A value of 0 disables the timeout.

* `uint16_t getTimeout()`<br>
  Returns the current timeout period setting.

* `bool timeoutOccurred()`<br>
  Indicates whether a read timeout has occurred since the last call to `timeoutOccurred()`.

## Version history

* 1.0.1 (2018-09-19): Fix Arduino 101 hanging in init().
* 1.0.0 (2018-05-31): Original release.
