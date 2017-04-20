# PCF8574_ESP

This is a simple library for using the PCF8574/PCF8574A/PCF8575 I/O-expanders over I2C. I took the code by Rob Tillaart from http://playground.arduino.cc/Main/PCF8574Class and modified it. Rob's original code is designed to work only with the first I2C-bus, but many devices now have multiple I2C-buses and on the ESP8266 the I2C is software-based, so you can have as many buses as you have free I/O-pins for and you can use whatever free pin you like for it. I also added support for the PCF8575 16-pin I/O-expander.

This library does not supply any special functionality for using the interrupt-pin on the PCF8574/PCF8575, you have to do that part yourself. Don't forget to configure the pin on the MCU's end as INPUT_PULLUP! You also have to set up the I2C-bus yourself before calling any library-functions.

Despite the library's name, it supports the ESP8266, Attiny85, STM32F1 and should work with any standard Arduino-device at the moment, though, out of the official Arduino-boards, I have only tested it with Arduino Micro as I don't yet own any other ones.

# Usage
```
What arguments the class-constructor accepts depends on the board you use:
Attiny85: PCF857x(uint8_t address, is8575 = false)
STM32F1: PCF857x(uint8_t address, HardWire *Wire, is8575 = false)
ESP8266 and others: PCF857x(uint8_t address, TwoWire *UseWire, is8575 = false)

void begin()
uint8_t read8() -- Read all 8 pins' status at once as a bitmask with a pin being HIGH if the corresponding bit is set, and vice versa.
uint8_t read(uint8_t pin) -- Returns a single pin's status.
void write8(uint8_t value) -- Set all 8 pins' status at once.
void write(uint8_t pin, uint8_t value) -- Set a single pin's status.
void toggle(uint8_t pin) -- Reverses the corresponding pin's status, HIGH to LOW or vice versa.
void toggleAll() -- Reverses all the pins' statuses, from HIGH to LOW and vice versa.
void shiftLeft(uint8_t n=1) -- Shift the pins' states left, with pin 1's state going into pin 2 and so on.
void shiftRight(uint8_t n=1) -- Like above, but to the right instead.
void rotateLeft(uint8_t n=1) -- Rotate the pins' status instead of just shifting them, with pin 7's status going to pin 0, ie. wrapping around.
void rotateRight(uint8_t n=1) -- Like above.
int lastError()
uint16_t read16() -- Returns all the 16 pins' status at once if you set is8575 as true, otherwise doesn't do anything and returns zero.
void write16() -- Sets all the 16 pins' statuses at once if you set is8575 as true, otherwise doesn't do anything.
```

# Mixing INPUT- and OUTPUT-pins and write8

Due to the way the PCF8574 works you cannot just use read8() to read the pin-states, then change one pin and write8() the new states out because if you are using some pins as INPUT and the pin is being pulled low the moment you read8() its state you'll then be pulling the pin LOW when issuing write8() and it'll stop working as an INPUT. For this reason the library caches written values instead of relying on reading the pin-states when using write() or toggle(), and if you use write8() in your own code you need to remember to pull HIGH any pin you want to use as INPUT regardless of their current state.
