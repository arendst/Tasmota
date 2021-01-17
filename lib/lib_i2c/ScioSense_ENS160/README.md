# ScioSense ENS160
Arduino library for the ENS160 digital four channel MOX gas sensor with I2C interface from ScioSense

## Introduction
This project is an Arduino *library*. It implements a driver with examples for the ENS160.
The ENS160 chip is a digital gas sensor for TVOC and eCO2 with an I2C interface.
The driver in this Arduino library is based on the code supplied by *Sciosense*, the manufacturer of the chip.

Note that the ENS160 requires a supply voltage of 1.71V .. 1.98V.
The ENS160 also requires a IO voltage of 1.71V .. 3.6V.

## Links
The ENS160 is made by [Sciosense](http://www.sciosense.com).
 - The datasheet of the ENS160 is not yet released but can be provided on request

## Prerequisites
It is assumed that
 - The Arduino IDE has been installed.
   If not, refer to "Install the Arduino Desktop IDE" on the
   [Arduino site](https://www.arduino.cc/en/Guide/HomePage).
 - The library directory is at its default location.
   For me, that is `C:\Users\sciosense\Documents\Arduino\libraries`.

## Installation
- Visit the project page for the Arduino CCS811 library.
- Click the green button Clone or download on the right side.
- From the pop-up choose Download ZIP.
- In Arduino IDE, select Sketch > Include Library > Manage Libraries ... and browse to the just downloaded ZIP file.
- When the IDE is ready this README.md should be located at e.g. `C:\Users\sciosense\Documents\Arduino\libraries\sciosense_ens160\README.md`.

## Build an example
To build an example sketch
 - (Re)start Arduino.
 - Open File > Example > Examples from Custom Libraries > ENS160 > ENS160basic_normal
 - Make sure Tools > Board lists the correct board.
 - Select Sketch > Verify/Compile.

## Functions
> bool begin(bool debug=false, bool bootloader=false)

Required to initialize the sensor during startup. The function takes up to two arguments: 

First one enables a debug mode to provide further output during sensor operation. Default setting is false (deactivated)

Second one enables bootloader mode, which is only required to flash a new firmware. Default setting is false (deactivated)

Returns true in case of success and false in case of any issues.

> bool	available()

Provides true/false feedback whether the sensor is available or not. The information will be created during initialization.

> bool	setMode(uint8_t mode)

This function is used to set the sensor operation mode. Currently two modes are implemented and can be set by the following commands:

*ENS160_OPMODE_STD*: operates the sensor with a predefined heater scheme and provides prediction for eCO2 and TVOC

*ENS160_OPMODE_CUSTOM*: operates the sensor with a custom defined heaterand measurement scheme, only raw resistance values will be 

reported (see also initCustomMode / addCustomStep)

Returns true in case of success and false in case of any issues.

> bool	initCustomMode(uint16_t stepNum)

To be able ot use the custom mode, first it has to be initialized. The number of steps is required as input parameter. Each step has to be configured individually by calling *addCustomStep*. 

Returns true in case of success and false in case of any issues.

> bool addCustomStep(uint16_t time, bool measureHP0, bool measureHP1, bool measureHP2, bool measureHP3, uint16_t tempHP0, uint16_t tempHP1, uint16_t tempHP2, uint16_t tempHP3)

For each of the steps defined in *initCustomMode* all releavant parameters have to be defined. These are 

*time* defines the duration of the step

*measureHP<x>* defines if the resistance value for a specific hotplate should be measured 

*tempHP<x>*	sets the temperatuer for the specific hotplate 

Returns true in case of success and false in case of any issues.
	
>	bool	measure(bool waitForNew = True)

Performs the sensor measurement, which means that values from the ASIC internal registers will be stored as result in internal variables. 

If the variable *waitForNew* is set (default), the function will only terminate once new data is stored in the ASIC internal registers. Due to the measurement frequency this can take up to 1sec. 

If the variable *waitForNew* is not set, the function might terminate with pervious non-updates results stored in the internal variables.

Returns true in case of success and false in case of any issues.

>	bool	set_envdata210(uint16_t t, uint16_t h)

In case temperature and humidity compensation is reuqired for sensor operation, this function allowe the storage of  t and h (in ENS210 format) to ENV_DATA register.

Returns true in case of success and false in case of any issues.

> uint8_t getMajorRev()

Function returns the major number of the firmware release.

>	uint8_t	getMinorRev()

Function returns the minor number of the firmware release.

> uint8_t	getBuild()

Function returns the build number of the firmware release.

> uint16_t	getTVOC()

Function returns the TVOC value stored in the internal variable.

> uint16_t	geteCO2()

Function returns the equivalent CO2 value stored in the internal variable.

> uint32_t	getHP0()

Function returns the raw resistance value of hotplate 0 stored in the internal variable.

> uint32_t	getHP1()

Function returns the raw resistance value of hotplate 1 stored in the internal variable.

> uint32_t	getHP2()

Function returns the raw resistance value of hotplate 2 stored in the internal variable.

> uint32_t	getHP3()

Function returns the raw resistance value of hotplate 3 stored in the internal variable.

> uint32_t	getHP0BL()

Function returns the stored baseline of hotplate 0 (resistance).

>	uint32_t	getHP1BL()

Function returns the stored baseline of hotplate 1 (resistance).

>	uint32_t	getHP2BL() 

Function returns the stored baseline of hotplate 2 (resistance).

>	uint32_t	getHP3BL()

Function returns the stored baseline of hotplate 3 (resistance).

### ScioSense is a Joint Venture of ams AG
