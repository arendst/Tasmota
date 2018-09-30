# Adafruit PCA9685 PWM Servo Driver Library [![Build Status](https://travis-ci.org/adafruit/Adafruit_PWMServoDriver.svg?branch=master)](https://travis-ci.org/adafruit/Adafruit_PWMServoDriver)

This is a library for our Adafruit 16-channel PWM & Servo driver, shield or FeatherWing

<img src="https://cdn-shop.adafruit.com/970x728/815-04.jpg" height="300"/>

Pick one up today in the adafruit shop!
  * https://www.adafruit.com/products/815
  * https://www.adafruit.com/product/1411
  * https://www.adafruit.com/product/2928

These drivers use I2C to communicate, 2 pins are required to interface.

Adafruit invests time and resources providing this open source code, please support Adafruit and open-source hardware by purchasing products from Adafruit!

Written by Limor Fried/Ladyada  for Adafruit Industries. BSD license, check license.txt for more information. 

All text above must be included in any redistribution

<!-- START COMPATIBILITY TABLE -->

## Compatibility

MCU                | Tested Works | Doesn't Work | Not Tested  | Notes
------------------ | :----------: | :----------: | :---------: | -----
Atmega328 @ 16MHz  |      X       |             |            | 
Atmega328 @ 12MHz  |      X       |             |            | 
Atmega32u4 @ 16MHz |      X       |             |            | 
Atmega32u4 @ 8MHz  |      X       |             |            | 
ESP8266            |      X       |             |            | 
Atmega2560 @ 16MHz |      X       |             |            | 
ATSAM3X8E          |      X       |             |            | Use SDA1/SCL1
ATSAM21D           |      X       |             |            | 
ATtiny85 @ 16MHz   |      X       |             |            | 
ATtiny85 @ 8MHz    |      X       |             |            | 
Intel Curie @ 32MHz |             |             |     X       | 
STM32F2            |             |             |     X       | 

  * ATmega328 @ 16MHz : Arduino UNO, Adafruit Pro Trinket 5V, Adafruit Metro 328, Adafruit Metro Mini
  * ATmega328 @ 12MHz : Adafruit Pro Trinket 3V
  * ATmega32u4 @ 16MHz : Arduino Leonardo, Arduino Micro, Arduino Yun, Teensy 2.0
  * ATmega32u4 @ 8MHz : Adafruit Flora, Bluefruit Micro
  * ESP8266 : Adafruit Huzzah
  * ATmega2560 @ 16MHz : Arduino Mega
  * ATSAM3X8E : Arduino Due
  * ATSAM21D : Arduino Zero, M0 Pro
  * ATtiny85 @ 16MHz : Adafruit Trinket 5V
  * ATtiny85 @ 8MHz : Adafruit Gemma, Arduino Gemma, Adafruit Trinket 3V

<!-- END COMPATIBILITY TABLE -->
