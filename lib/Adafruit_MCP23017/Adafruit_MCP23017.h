/***************************************************
  This is a library for the MCP23017 i2c port expander

  These displays use I2C to communicate, 2 pins are required to
  interface
  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.
  BSD license, all text above must be included in any redistribution
 ****************************************************/

#ifndef _Adafruit_MCP23017_H_
#define _Adafruit_MCP23017_H_

// Don't forget the Wire library
#ifndef ARDUINO_AVR_GEMMA
                                    //TinyWireM is now part of
                                    //   Adafruit version of Wire Library, so this
                                    // will work with Adafruit ATtiny85's
                                    //But Arduino Gemma doesn't use that library
                                    // We do NOT want to include Wire if it's an arduino Gemma
  #include <Wire.h>
#else
  #include <TinyWireM.h>
  #define Wire TinyWireM
#endif


class Adafruit_MCP23017 {
public:
  void begin(uint8_t addr);
  void begin(void);

  void pinMode(uint8_t p, uint8_t d);
  void digitalWrite(uint8_t p, uint8_t d);
  void pullUp(uint8_t p, uint8_t d);
  uint8_t digitalRead(uint8_t p);

  void writeGPIOAB(uint16_t);
  uint16_t readGPIOAB();
  uint8_t readGPIO(uint8_t b);

  void setupInterrupts(uint8_t mirroring, uint8_t open, uint8_t polarity);
  void setupInterruptPin(uint8_t p, uint8_t mode);
  uint8_t getLastInterruptPin();
  uint8_t getLastInterruptPinValue();
  uint8_t getIOCON(); // lva

 private:
  uint8_t i2caddr;

  uint8_t bitForPin(uint8_t pin);
  uint8_t regForPin(uint8_t pin, uint8_t portAaddr, uint8_t portBaddr);

  uint8_t readRegister(uint8_t addr);
  void writeRegister(uint8_t addr, uint8_t value);

  /**
   * Utility private method to update a register associated with a pin (whether port A/B)
   * reads its value, updates the particular bit, and writes its value.
   */
  void updateRegisterBit(uint8_t p, uint8_t pValue, uint8_t portAaddr, uint8_t portBaddr);

};

#define MCP23017_ADDRESS 0x20

// registers
#define MCP23017_IODIRA 0x00
#define MCP23017_IPOLA 0x02
#define MCP23017_GPINTENA 0x04
#define MCP23017_DEFVALA 0x06
#define MCP23017_INTCONA 0x08
#define MCP23017_IOCONA 0x0A
#define MCP23017_GPPUA 0x0C
#define MCP23017_INTFA 0x0E
#define MCP23017_INTCAPA 0x10
#define MCP23017_GPIOA 0x12
#define MCP23017_OLATA 0x14


#define MCP23017_IODIRB 0x01
#define MCP23017_IPOLB 0x03
#define MCP23017_GPINTENB 0x05
#define MCP23017_DEFVALB 0x07
#define MCP23017_INTCONB 0x09
#define MCP23017_IOCONB 0x0B
#define MCP23017_GPPUB 0x0D
#define MCP23017_INTFB 0x0F
#define MCP23017_INTCAPB 0x11
#define MCP23017_GPIOB 0x13
#define MCP23017_OLATB 0x15

#define MCP23017_INT_ERR 255

#endif
