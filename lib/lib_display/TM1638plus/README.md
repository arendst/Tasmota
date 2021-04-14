![ module pics ](https://github.com/gavinlyonsrepo/TM1638plus/blob/master/extra/images/tm16383.jpg)

Table of contents
---------------------------

  * [Overview](#overview)
  * [Installation](#installation)
  * [Features](#features)
  * [Model One](#model-one)
  * [Model Two](#model-two)
  * [Model Three](#model-three)
  * [Notes](#notes)
  * [Memory](#memory)
  * [Pic Ports](#pic-port)
  

Overview
--------------------------------------------
* Name: TM1638plus
* Description: 
An Arduino library to display data on a 8-digit TM1638 seven segment module.
This library supports 3 different models, pictured above from left to right.

1. Model 1, The (8 KEY & 8 LED) variant which has 8 LED's and 8 Push buttons.
2. Model 2, The (QYF 16 KEY) variant which has 16 pushbuttons.
3. Model 3, The (LKM1638) variant which has 8 bi-colour LED's and 8 Push buttons.


* Main Author: Gavin Lyons.

* Tested on Development platforms: 

1. Arduino UNO & NANO v3 .
2. ATtiny85 .
3. ESP32.
4. STM32 STM32F103C8T6 "the blue pill".
5. ESP8266.
6. Teensy 4.0. (may not work at highest frequency see  notes section)

* History: see CHANGELOG.md in extra folder
* Contributors: [gabormay](https://github.com/gabormay)  [centic9](https://github.com/centic9) [wunderbaum](https://github.com/wunderbaum)


Installation
------------------------------

The library is included in the official Arduino library manger and the optimum way to install it 
is using the library manager which can be opened by the *manage libraries* option in Arduino IDE. 
Search "tm1638" in search bar of library manager to find it.

See link below for instruction for this and for the other methods too.

[Installing Additional Arduino Libraries guide](https://www.arduino.cc/en/Guide/Libraries)


Features
----------------------

Connections to MCU: 

1. GPIO = STB = Strobe
2. GPIO  = CLK  = Clock
3. GPIO = DIO = Data input / output
4. GND
5. VCC 5V.

This device is 5V if using 3.3V MCU, level shift.

This library supports three variants of the TM1638, which for purposes of this documentation, 
will be named Model 1 ,Model 2 and Model 3.  

| Model No | Module Name | LEDS | Push buttons | 
| ------ | ------ |  ------ | ------ |
| 1 | TM1638 LED & KEY | 8 red only | 8 |
| 2 | TM1638 KEYS, QYF  | 0 | 16 |
| 3 | TM1638 V1.3 or LKM1638  | 8 bi color,  red and green  | 8 |

There are two sets of files to support model 1 & 2 . I kept them separate as the models are wired quite different, Model 1 address by digit, while Model 2 address by segment. So the code is quite different  for most functions. Model 3 uses same code as Model 1, just different example file and different use of LED functions. Common settings, data and functions are in the TM1638plus_common.x files.

| Model | Header | Code file | TEST files | 
| ------ | ------ |  ------ | ------ |
| 1 | TM1638plus.h       | TM1638plus.cpp | TM1638plus_HELLOWORLD_Model1.ino TM1638plus_TEST_Model1.ino |
| 3 |  Same as  model 1     |  Same as  model 1 | TM1638plus_TEST_Model3.ino |
| 2 | TM1638plus_Model2.h  | TM1638plus_Model2.cpp | TM1638plus_HELLOWORLD_Model2 TM1638plus_TEST_Model2.ino |

Model One
--------------------------------------

TM1638 Module 8 Push buttons 8 LEDS (LED & KEY)

![ module ](https://github.com/gavinlyonsrepo/pic_16F18446_projects/blob/master/images/TM1638.jpg)

![ sch ](https://github.com/gavinlyonsrepo/pic_16F18446_projects/blob/master/images/TM1638_2.jpg)
 
This variant consist of an 8-digit seven segment display with decimal points,
8 Leds and 8 Push buttons. Two 4 digit 3461AS-1 (.34 inch, 4 digit ,common Cathode,  decimal point, RED) are used in this module giving a total of 8 digits. A TM1638 controller chip drives the unit.
The unit is marked (LED & KEY).

**Model 1 Library Functions**

The commented functions can be found in library header file TM1638plus.h.
The library support ASCII ,text ,Hex and allows for setting individual segments,
and the decimal point of segment.
The TM1638plus_TEST.ino contains a set of tests demonstrating library functions.
For more information see the commented headers in header file. 

1. Print an ASCII character.
2. Print an ASCII character with a dot/decimal point.
3. Print a Hexadecimal digit(0-15).
4. Print a long integer number with or without leading zeros.
5. Print two integer numbers (0-9999) to each nibble with or without leading zeros.
6. Print a text string(dots are replaced and dot is turned on preceding digit), 
"abc.def" becomes "abcdef" with c decimal point segment switched on.
7. Read buttons status. User may have to debounce buttons depending on application.
debouncing left out to minimise library size. 
See [URL LINK](https://github.com/gavinlyonsrepo/Arduino_Clock_3) 
for a practical real world example of using this library,
including a example of debouncing the key presses. It is also possible to read multiple key presses.
8. Reset and init module functions.
9. Adjust brightness of module. Support 8 degree brightness adjustment.
If user wishes to change the default brightness at start-up change,
The DEFAULT_BRIGHTNESS define in header file. 
10. Manually set segments to create custom patterns.
11. Switch the 8 LEDS on and off, both a set one LED and set all LEDS methods available.


Model Two
-----------------------------------------

TM1638 Module 16 Push buttons (16 KEY) (QYF).

![ module ](https://github.com/gavinlyonsrepo/TM1638plus/blob/master/extra/images/tm16381.jpg)

![ sch ](https://github.com/gavinlyonsrepo/TM1638plus/blob/master/extra/images/tm16382.jpg)

They consist of an 8-digit seven segment display with decimal points,
and 16 Push buttons.Two 4 digit 3461BS-1 (.34 inch, 4 digit ,common Anode,  decimal point, RED)are used in this module giving a total of 8 digits. A TM1638 controller chip drives the unit.
NB :  If your display shows "56781234" for "12345678" see Notes section. Note A.

**Model 2 Library Functions**

The commented functions can be found in library header file TM1638plus_Model2.h.
The library support Strings,decimal ,Hex ,raw ASCII data, setting individual segments,
and the decimal point. For more detailed information on functions see commented headers in header file(.h).

1. Print a Hexadecimal number with or without leading zeros
2. Print a decimal number with or without leading zeros
3. Manually set segments to create custom patterns.
4. Print two 4 digit decimal number(0-9999) to each nibble with or without leading zeros.
5. Print a text string, dot function supported. 
6. Read buttons status. User may want to debounce buttons depending on application.
See TM1638plus_ADC_TEST_Model2.ino for debounce button example. 
Two different functions to read buttons. 
7. Reset and init module functions.
8. Adjust brightness of module. Support 8 degree brightness adjustment.
If user wishes to change the default brightness at start-up change,
The "DEFAULT_BRIGHTNESS" define in header file. 
9. Print raw ASCII data without reference to font file.

Model Three
-----------------------------------------

There are different PCB's  of these modules on market, 
This library was tested on no 3 below. I think this software will work for all of them 
and the differences in PCB are related to connectors, layout and component placement. 
This module is a variant of Model 1. The differences are the LEDs are bigger and bi-color
both red and green, The seven segment display is larger and extra connectors are added for Daisy chaining. 
Two 4 digit KYX-5461AS-7.3 (.54 inch, 4 digit ,common cathode,  decimal point, RED)are used in this module
giving a total of 8 digits. 

1. LKM1638 v1.1
2. LKM1638 v1.2
3. TM1638 V1.3 

![ module ](https://github.com/gavinlyonsrepo/TM1638plus/blob/master/extra/images/tm16384.jpg)

**Model 3 Library Functions**

The code is the same as model 1 and there is one unique model 3 example file. 
setLED and setLEDs functions behaviour is the only difference in code base between 1 and 3.
SetLED: The difference is when you call the setLED function you pass the following to get LEDs to change colour. For more detailed information on functions see commented headers in header file(.h).

| Model | setLED Value | result | 
|    ----   |    ----   |    ----   |
|    1 & 3  |    0   |    LED off   |
|    3   |    1   |    Led green on  |
|    3   |    2   |    LED red on  |
|    1   |    1   |    LED on   |

SetLEDs: When you pass call the setLEDs function you can pass a word pattern where upper byte is turns LEDs green on and lower byte turns LEDs red on . Model one ignores lower byte, Set to 0x00 always. 

1. Model 3 setLEDs(word) = 0xGGRR
3. Model 1 setLEDs(word) = 0xRR00

Notes
--------------------------

1. Swapped data on Display issue on some Model 2 modules
2. Anomaly's  on High frequency micro-controllers.
3. Driving multiple displays.
4. Detecting multiple buttons pressed together.

*Note A* : Swapped display Issue: Model 2 only

For Some users using this library the nibbles in information display byte 
where swapped around. This is because there are different versions of modules on market with different wiring.  See issue #3 on github called Swapped display :: "12345678" becomes "56781234". 
If you test library and you see this issue, in order to fix this when you declare the 
Object, set the fourth parameter "swap_nibbles" to True, The default is false.

| PCB Model noted Label | Operation | Object constructor 4th parameter |
| ------ | ------ | ------ | 
| QYF-TM1638 | default operation | false | 
| QYF-TM1638 -Ver 1.0 | Swapped display Fix |  true  | 

*Note B* :  High frequency micro-controllers.

This library uses a software SPI-like protocol and may not work fully on 
micro-controllers running at a very high frequency, without some adjustments to timing.
Its a SPI-like interface with a single bidirectional data wire DIO.
The TM1638 is basically a slow SPI device (< 500kHz) in DIO mode. The clock uses the equivalent of SPI mode 3 (normally high, clocks data on the rising edge). The problem is that the native Arduino shiftIn()/shiftOut() wire functions are simply too fast for this device (technically the clock signalling for the TM1638 is inverted but as it triggers on a rising edge still it is tolerant of that).
To make this work with fast devices, the shift clocking is slowed with a small delay (on the order of a microsecond).  As of version 1.6 a new parameter *(_HIGH_FREQ)* has been introduced to constructor it is false by default. Set to true for high frequency MCU ~> 100Mhz.  This will fix the issue of HF MCU not reading buttons correctly(ESP-Xs).  The High_Freq parameter causes a custom shift-in function to be used.
The  Teensy results have been sent to me, I don't have these MCU's them at time of writing. 

| IC |  frequency | Status | 
| ------ | ------ | ------ | 
| ATtiny85 | 1Mhz internal | Working | 
| Arduino UNO  |  16 MHz  | Working | 
| Arduino Nano  | 16 MHz  | Working |
| STM32 "blue pill" STM32F103C8T6 | 72Mhz | Working |
| ESP8266 | 160Mhz | Working |
| ESP 32  |   240 MHz  | Working, with high_freq set to  true | 
| Teensy 4.0| 150Mhz | Working model 1,  no Data rest of models |
| Teensy 4.0| 396Mhz | Not working on  model1 ,  no Data rest of models |

*Note C* : Driving multiple displays.

It is possible to drive multiple modules. Share the DIO and CLK lines and use a unique
STB line for each device. see issue number 10 at github for example code.

*Note D* : Detecting multiple buttons pressed together.

Model 1 and Model 3 CAN detect multiple buttons pressed.

Model 3 has two different functions:

1. ReadKey16 returns a byte with decimal value 1-16 this function cannot 
detect multiple buttons pressed.

2. ReadKey16Two returns a 16 bit integer where each bit corresponds to the 16 switch's.
However due to the wiring  of the module, see SG-X lines  on schematic,
Pressing Certain combinations of buttons will cause the  data on Seven Segments to 
change. So the simultaneous use of multiple key presses and the seven segments display 
is problematic. See issue 12 on github for more details.

Memory
-------------------------------

Version 1.4.

1. Model 1 memory usage NANO, basic hello world sketch.

Sketch uses 1488 bytes (4%) of program storage space. 
Global variables use 22 bytes (1%) of dynamic memory.

2. Model 2 memory usage NANO, basic hello world sketch.

Sketch uses 1536 bytes (5%) of program storage space. 
Global variables use 23 bytes (1%) of dynamic memory.


Pic Port
-------------------

MicroChip PIC XC8 port.
I have ported this library to the PIC for the XC8 compiler: 
[ Link ](https://github.com/gavinlyonsrepo/pic_16F18446_projects)
