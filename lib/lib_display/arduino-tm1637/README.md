# SevenSegmentTM1637 Arduino Library

* master&nbsp;&nbsp;[![Build Status](https://travis-ci.org/bremme/arduino-tm1637.svg?branch=master)](https://travis-ci.org/bremme/arduino-tm1637)
* develop&nbsp;[![Build Status](https://travis-ci.org/bremme/arduino-tm1637.svg?branch=develop)](https://travis-ci.org/bremme/arduino-tm1637)


Arduino library for controlling a TM163x based 7-segment display module. These modules are sold under various names by various suppliers. For example:

* [Seed Studio: Grove 4 digit display](http://www.seeedstudio.com/depot/grove-4digit-display-p-1198.html)
* [Ebay: 4 Bits Digital Tube LED TM1637](http://www.ebay.com/sch/i.html?_odkw=4+Bits+Digital+Tube+LED&_osacat=0&_from=R40&_trksid=p2045573.m570.l1313.TR0.TRC0.H0.X4+Bits+Digital+Tube+LED+TM1637.TRS0&_nkw=4+Bits+Digital+Tube+LED+TM1637&_sacat=0)
* [DealExtreme: LED 4-Digit Display Module](http://www.dx.com/s/TM1637)

They come in different sizes, colors and there is a clock (with a colon) and decimal (with four decimal dots) display variant. But the most common one is the red 0.36" clock version, this is also the cheapest one (you can get those for about $1.50). I've written this library using the above module, if I get my hands on any of the other variants I might add some specific code, for example to print floats on the decimal version.

![TM1637](extras/img/TM1637-4-digit-colon.jpg)

# Hardware setup

| TM1637 PIN | Arduino PIN      | Description         |
|------------|------------------|---------------------|
| CLK        |  Any digital pin | Clock               |
| DIO        |  Any digital pin | Digital output      |
| VCC        |  5V              | Supply voltage      |
| GND        |  GND             | Ground              |


# Installation

Like any other Arduino library this library is installed by copying the files into a directory on the Arduino IDE search path. Most common is to put all files in a director in `your sketch folder/libraries/SevenSegmentTM1637/`. See [installing additional Arduino libraries](https://www.arduino.cc/en/Guide/Libraries) for more information.

# Usage

This library uses the [LCD API v1.0](http://playground.arduino.cc/Code/LCDAPI) so you can use the same functions/methods using this library as any lcd library which conforms to the LCD API. Furthermore this library (like described in the LCD API) inherent the Print class (See [Serial.print()](https://www.arduino.cc/en/Serial/Print) and for more details [Print.h](https://github.com/arduino/Arduino/blob/master/hardware/arduino/avr/cores/arduino/Print.h)). This means that you can use all Print class functions/methods like you're used to when you're doing `Serial.print("Something")` or `Serial.print(128, BIN)` for example.

## Basic methods

* `SevenSegmentTM1637(clkPin, dioPin)`Creates a display object
* `init()`              Initializes the display
* `print(value)`        Prints anything to the display (e.g. a string, number..)
* `clear()`             Clears the display (and resets cursor)
* `home()`              Resets cursor
* `setCursor(row, col)` Set the cursor to specified position
* `setBacklight(value)` Sets the brightness of the display
* `on()`                Turn the display on (set brightness to default)
* `off()`               Turns the display off (set brightness to zero)
* `blink()`             Blinks what ever is on the display
* `setColonOn(bool)`    Sets the colon in the middle of the display
* `setPrintDelay(time)` Sets the delay when printing more than 4 characters

## Advanced methods

* `encode()`    Encodes a single digit, single char or char array (C string)
* `printRaw()`  Prints raw byte(s) to the display
* `command()`   Sends one or more raw byte commands to the display

## Low level methods

* `comStart()`            Serial communication start command
* `comWriteByte(bytes)`   Serial communication send byte command
* `comAck()`              Serial communication get acknowledged command
* `comStop()`             Serial communication stop command

If you still want or need more functionality, I've included two super classes:

* `SevenSegmentExtended()`  Extends the base class with more usefull functions.
* `SevenSegmentFun()`       Even more extensions for fun.

If you use any of these super classes, you will also get all the basic, advanced and low level methods as well. If you use the fun class extension you will get the extended class methods as well.

## Extended class extra methods

* `SevenSegmentExtended(clkPin, dioPin)`        Creates a display object
* `printTime(hour, min, [blink], [blinkDelay])` Prints the time to the display
* `printTime(time, [blink], [blinkDelay])`      Prints the time to the display
* `printDualCounter(leftValue, rightValue)`     Prints two digits to the display

## Fun class extra methods

* `SevenSegmentFun(clkPin, dioPin)`           Creates a display object
* `printLevelVertical(level, [leftToRight])`  Print 0-100% (2 steps per digit) vertical level e.g. volume, battery
* `printLevelVertical(level, leftToRight, symbol)`  Print 0-100% (1 step per digit) vertical level using custom symbol
* `printLevelHorizontal(levels[])` Prints 4 horizontal levels e.g. equalizer
* `scrollingText()`             Prints text and (keeps) scrolling
* `snake()`                     Classic snake demo
* `nightrider()`                Nightrider Kit demo
* `bombTimer()`                 Count down a (bomb) timer
* `bouncingBall()`              Bouncing ball demo

For more extended information on what arguments all above functions accept and return see the header files of the classes (SevenSegmentTM1637.h, SevenSegmentExtended.h and SevenSegmentFun.h).

# Trouble shooting

* Some boards might not have enough power on their 5V pin. In that case try to use an external 5V power supply. People reported that the ESP8266 might be one of those boards.

# Todo

* Refactor library to make it more modular and support more chips
  + Add support for all TM16xx chips (most should already work)
  + Add support for MAX7219 displays
  + Add support for TM74HC595 displays

# Changelog

+ 09-07-2020 version 1.1.1
  + Bug fixes
    + fixed a typo in SevenSegmentFun.h file
+ 04-07-2020 version 1.1.0
  + Improved `printLevelVertical()` using [pablo-lp](https://github.com/pablo-lp) suggestions
    + The default `printLevelVertical()` can diplay twice the number of levels now (e.g. 9 levels for a defaulf 4 digit display)
  + Add configurable blink delay when using the `printTime` methods (thanks to [simoneluconi](https://github.com/simoneluconi))
  + Added a new `printNumber()` method to make it easier to print right aligned numbers
    + Thanks [dan2600](https://github.com/dan2600), [Bilick88](https://github.com/Bilick88), [jasonacox](https://github.com/jasonacox) for your suggestions.
  + Bug fixes
    + Merge PR from [berendkleinhaneveld](https://github.com/berendkleinhaneveld) to fix some compiler warnings
    + Merge PR from [facelessloser](https://github.com/facelessloser) Remove some `Serial.prinln()` calls
    + Merge PR from [RAnders00](https://github.com/RAnders00) Fix incorrect repsonse from `comReadByte()`
    + Merge PR from [per1234](https://github.com/per1234) Use correct separator in `keywords.txt`
+ 04-11-2016 version 1.0.4
  + Fixed bombTimer not counting down to zero (thanks to [foons](https://github.com/fooons) for opening an issue)
* 22-05-2016 version 1.0.3
 + add support for all AVR MCU's (thanks to [per1234](https://github.com/per1234))
* 08-05-2016 version 1.0.2
	+ fixed digitalHigh() macro for non AVR boards (thanks to [per1234](https://github.com/per1234))
* 28-09-2015 version 1.0.1
	+ fixed folder structure to comply with Arduino library 1.5 rev 2 specifications
* 28-09-2015 version 1.0.0
	+ First release


# Note

I spend quite a bit of time to build this library. I hope it will be useful to others. I decided to publish it, although there still might be small bugs. If you find one, just let me know and I will try to fix it. If you have any other suggestion, don't hesitate to contact me.

# Sources

I've looked at many sources while constructing this library, bassicly all alternative Arduino TM1637 libraries and some AVR TM1637 libraries. While doing so I've found some really nice improvements compared to other implementations. For example, most libraries use a really low clock speed (~50us), I've tested with clock speeds as low as 1us and the chip still seems to respond well. Furthermore, from the (Chinese) datasheet it seems that you always have to perform three operation when communicating with the IC; set some configuration, set the address and send 1-4 digit data and set the display brightness. I've found out that this is not the case, you can do all of those separately if you want.

Still without all these fine examples it would have taken me a lot more time to figure out the inner workings of this IC!

Sources:
* http://blog.3d-logic.com/2015/01/21/arduino-and-the-tm1637-4-digit-seven-segment-display/
* http://www.arduino.md/hardware/lcd-and-leds/0-36-led-display-4-digit-red/
* https://brainy-bits.com/tutorials/4-bits-7-segment-led-display-with-arduino/


#### Keywords

People will probably Google for keywords when looking for a driver, so here are some:

```TM1637 TM1636 library Arduino Led Driver 4 Digit Bits Digital LED Tube module LCD API Print.h Print class API display LCD AVR Atmega```
