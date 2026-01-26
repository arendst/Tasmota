# RadioLib ![Build Status](https://github.com/jgromes/RadioLib/workflows/CI/badge.svg) [![PlatformIO Registry](https://badges.registry.platformio.org/packages/jgromes/library/RadioLib.svg)](https://registry.platformio.org/libraries/jgromes/RadioLib) [![Component Registry](https://components.espressif.com/components/jgromes/radiolib/badge.svg)](https://components.espressif.com/components/jgromes/radiolib)

### _One radio library to rule them all!_

## Universal wireless communication library for embedded devices

## See the [Wiki](https://github.com/jgromes/RadioLib/wiki) and [FAQ](https://github.com/jgromes/RadioLib/wiki/Frequently-Asked-Questions) for further information. See the [GitHub Pages](https://jgromes.github.io/RadioLib) for detailed and up-to-date API reference.

RadioLib allows its users to integrate all sorts of different wireless communication modules, protocols and even digital modes into a single consistent system.
Want to add a Bluetooth interface to your LoRa network? Sure thing! Do you just want to go really old-school and play around with radio teletype, slow-scan TV, or even Hellschreiber using nothing but a cheap radio module? Why not!

RadioLib natively supports Arduino, but can run in non-Arduino environments as well! See [this Wiki page](https://github.com/jgromes/RadioLib/wiki/Porting-to-non-Arduino-Platforms) and [examples/NonArduino](https://github.com/jgromes/RadioLib/tree/master/examples/NonArduino).

RadioLib was originally created as a driver for [__RadioShield__](https://github.com/jgromes/RadioShield), but it can be used to control as many different wireless modules as you like - or at least as many as your microcontroller can handle!

### Supported modules:
* __CC1101__ FSK radio module
* __LLCC68__ LoRa module
* __nRF24L01__ 2.4 GHz module
* __RF69__ FSK/OOK radio module
* __RFM2x__ series FSK modules (RFM22, RM23)
* __RFM9x__ series LoRa modules (RFM95, RM96, RFM97, RFM98)
* __Si443x__ series FSK modules (Si4430, Si4431, Si4432)
* __STM32WL__ integrated microcontroller/LoRa module
* __SX126x__ series LoRa modules (SX1261, SX1262, SX1268)
* __SX127x__ series LoRa modules (SX1272, SX1273, SX1276, SX1277, SX1278, SX1279)
* __SX128x__ series LoRa/GFSK/BLE/FLRC modules (SX1280, SX1281, SX1282)
* __SX123x__ FSK/OOK radio modules (SX1231, SX1233)

### Supported protocols and digital modes:
* [__AX.25__](https://www.sigidwiki.com/wiki/PACKET) using 2-FSK or AFSK for modules:  
SX127x, RFM9x, SX126x, RF69, SX1231, CC1101, RFM2x and Si443x
* [__RTTY__](https://www.sigidwiki.com/wiki/RTTY) using 2-FSK or AFSK for modules:  
SX127x, RFM9x, SX126x, RF69, SX1231, CC1101, nRF24L01, RFM2x, Si443x and SX128x
* [__Morse Code__](https://www.sigidwiki.com/wiki/Morse_Code_(CW)) using 2-FSK or AFSK for modules:  
SX127x, RFM9x, SX126x, RF69, SX1231, CC1101, nRF24L01, RFM2x, Si443x and SX128x
* [__SSTV__](https://www.sigidwiki.com/wiki/SSTV) using 2-FSK or AFSK for modules:  
SX127x, RFM9x, SX126x, RF69, SX1231, CC1101, RFM2x and Si443x
* [__Hellschreiber__](https://www.sigidwiki.com/wiki/Hellschreiber) using 2-FSK or AFSK for modules:  
SX127x, RFM9x, SX126x, RF69, SX1231, CC1101, nRF24L01, RFM2x, Si443x and SX128x
* [__APRS__](https://www.sigidwiki.com/wiki/APRS) using AFSK for modules:  
SX127x, RFM9x, SX126x, RF69, SX1231, CC1101, nRF24L01, RFM2x, Si443x and SX128x
* [__POCSAG__](https://www.sigidwiki.com/wiki/POCSAG) using 2-FSK for modules:  
SX127x, RFM9x, RF69, SX1231, CC1101, nRF24L01, RFM2x and Si443x
* [__LoRaWAN__](https://lora-alliance.org/) using LoRa for modules:  
SX127x, RFM9x, SX126x and SX128x  
  * NOTE: LoRaWAN support is currently in beta, feedback via [Issues](https://github.com/jgromes/RadioLib/issues) and [Discussions](https://github.com/jgromes/RadioLib/discussions) is appreciated!

### Supported Arduino platforms:
* __Arduino__  
  * [__AVR__](https://github.com/arduino/ArduinoCore-avr) - Arduino Uno, Mega, Leonardo, Pro Mini, Nano etc.
  * [__mbed__](https://github.com/arduino/ArduinoCore-mbed) - Arduino Nano 33 BLE and Arduino Portenta H7
  * [__megaAVR__](https://github.com/arduino/ArduinoCore-megaavr) - Arduino Uno WiFi Rev.2 and Nano Every
  * [__SAM__](https://github.com/arduino/ArduinoCore-sam) - Arduino Due
  * [__SAMD__](https://github.com/arduino/ArduinoCore-samd) - Arduino Zero, MKR boards, M0 Pro etc.
  * [__Renesas__](https://github.com/arduino/ArduinoCore-renesas) - Arduino Uno R4

* __Adafruit__
  * [__SAMD__](https://github.com/adafruit/ArduinoCore-samd) - Adafruit Feather M0 and M4 boards (Feather, Metro, Gemma, Trinket etc.)
  * [__nRF52__](https://github.com/adafruit/Adafruit_nRF52_Arduino) - Adafruit Feather nRF528x, Bluefruit and CLUE

* __Espressif__
  * [__ESP32__](https://github.com/espressif/arduino-esp32) - ESP32-based boards
  * [__ESP8266__](https://github.com/esp8266/Arduino) - ESP8266-based boards

* __Intel__
  * [__Curie__](https://github.com/arduino/ArduinoCore-arc32) - Arduino 101

* __SparkFun__
  * [__Apollo3__](https://github.com/sparkfun/Arduino_Apollo3) - Sparkfun Artemis Redboard

* __ST Microelectronics__
  * [__STM32__ (official core)](https://github.com/stm32duino/Arduino_Core_STM32) - STM32 Nucleo, Discovery, Maple, BluePill, BlackPill etc.
  * [__STM32__ (unofficial core)](https://github.com/rogerclarkmelbourne/Arduino_STM32) - STM32F1 and STM32F4-based boards

* __MCUdude__
  * [__MegaCoreX__](https://github.com/MCUdude/MegaCoreX) - megaAVR-0 series (ATmega4809, ATmega3209 etc.)
  * [__MegaCore__](https://github.com/MCUdude/MegaCore) - AVR (ATmega1281, ATmega640 etc.)

* __Raspberry Pi__
  * [__RP2040__ (official core)](https://github.com/arduino/ArduinoCore-mbed) - Raspberry Pi Pico and Arduino Nano RP2040 Connect
  * [__RP2040__ (unofficial core)](https://github.com/earlephilhower/arduino-pico) - Raspberry Pi Pico/RP2040-based boards
  * [__Raspberry Pi__](https://github.com/me-no-dev/RasPiArduino) - Arduino framework for RaspberryPI

* __Heltec__
  * [__CubeCell__](https://github.com/HelTecAutomation/CubeCell-Arduino) - ASR650X series (CubeCell-Board, CubeCell-Capsule, CubeCell-Module etc.)

* __PJRC__
  * [__Teensy__](https://github.com/PaulStoffregen/cores) - Teensy 2.x, 3.x and 4.x boards

The list above is by no means exhaustive - RadioLib code is independent of the used platform! Compilation of all examples is tested for all platforms officially supported prior to releasing new version. In addition, RadioLib includes an internal hardware abstraction layer, which allows it to be easily ported even to non-Arduino environments.
