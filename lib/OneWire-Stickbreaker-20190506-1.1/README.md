# OneWire library
  A modification of the Arduino OneWire library maintained by @PaulStoffregen.  This modifications supports the ESP32 under the Arduino-esp32 Environment.
  
  No changes are required for compatibility with Arduino coding.

Original Source is Paul's 2.3 version.  Forked 28DEC2017

@stickbreaker
V2.3.1 30APR2018 add IRAM_ATTR to read_bit() write_bit() to solve ICache miss timing failure. 
 thanks @everslick re:  https://github.com/espressif/arduino-esp32/issues/1335
V2.3   28DEC2017 original mods to support ESP32  
