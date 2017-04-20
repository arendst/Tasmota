/* PCF8574_ESP -- library for using the I2C-driven 8-pin GPIO-expander
   ORIGINAL AUTHOR: Rob Tillaart
   Library modified by WereCatf */

#ifndef _PCF8574_ESP_H
#define _PCF8574_ESP_H

#if defined(ARDUINO) && ARDUINO >= 100
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

#if defined (ARDUINO_AVR_DIGISPARK) || defined (ARDUINO_AVR_ATTINYX5)
#include <TinyWireM.h>
#elif defined (__STM32F1__)
#include <HardWire.h>
#else
#include <Wire.h>
#endif

#define PCF857x_OK          0x00
#define PCF857x_PIN_ERROR   0x81
#define PCF857x_I2C_ERROR   0x82

class PCF857x
{
  public:
    // Defaults to 8574, set is8575 to true if you have a 8575 instead.
    #if defined (ARDUINO_AVR_DIGISPARK) || defined (ARDUINO_AVR_ATTINYX5)
    PCF857x(uint8_t address, bool is8575 = false);
    #elif defined (__STM32F1__)
    PCF857x(uint8_t address, HardWire *Wire, bool is8575 = false);
    #else
    PCF857x(uint8_t address, TwoWire *UseWire, bool is8575 = false);
    #endif

    void begin(uint16_t defaultValues=0xffff);
    uint8_t read8();
    uint16_t read16();
    uint8_t read(uint8_t pin);

    void write8(uint8_t value);
    void write16(uint16_t value);
    void write(uint8_t pin, uint8_t value);

    void toggle(uint8_t pin);
    void toggleAll();
    void shiftRight(uint8_t n=1);
    void shiftLeft(uint8_t n=1);
    void rotateRight(uint8_t n=1);
    void rotateLeft(uint8_t n=1);
    void resetInterruptPin();

    int lastError();

  private:
    #if defined (ARDUINO_AVR_DIGISPARK) || defined (ARDUINO_AVR_ATTINYX5)
    USI_TWI *_Wire;
    #elif defined (__STM32F1__)
    HardWire *_Wire;
    #else
    TwoWire *_Wire;
    #endif
    uint8_t _address;
    uint16_t _data;
    uint16_t _pinModeMask;
    int _error;
    bool _is8575;
};

#endif
