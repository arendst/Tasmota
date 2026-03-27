/*
    TwoWire.h - TWI/I2C library for Arduino & Wiring
    Copyright (c) 2006 Nicholas Zambetti.  All right reserved.

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

    Modified 2012 by Todd Krein (todd@krein.org) to implement repeated starts
    Modified December 2014 by Ivan Grokhotkov (ivan@esp8266.com) - esp8266 support
    Modified April 2015 by Hrsto Gochkov (ficeto@ficeto.com) - alternative esp8266 support
*/

#ifdef ESP8266

#ifndef TwoWire_h
#define TwoWire_h

#include "Arduino.h"
#include <inttypes.h>
#include "Stream.h"

//#warning "Using TasmotaWire library"

class Twi;

#define BUFFER_LENGTH 128

class TwoWire : public Stream {
private:
  Twi &twi;
  uint8_t rxBuffer[BUFFER_LENGTH];
  uint8_t rxBufferIndex;
  uint8_t rxBufferLength;

  uint8_t txAddress;
  uint8_t txBuffer[BUFFER_LENGTH];
  uint8_t txBufferIndex;
  uint8_t txBufferLength;

  uint8_t default_sda_pin;
  uint8_t default_scl_pin;

  uint8_t transmitting;

public:
  TwoWire();
  void begin(int sda, int scl);
  void pins(int sda, int scl) __attribute__((deprecated)); // use begin(sda, scl) in new code
  void begin();
  void setClock(uint32_t);
  void setClockStretchLimit(uint32_t);
  void beginTransmission(uint8_t);
  void beginTransmission(int);
  uint8_t endTransmission(void);
  uint8_t endTransmission(uint8_t);
  size_t requestFrom(uint8_t address, size_t size, bool sendStop);

  uint8_t requestFrom(uint8_t, uint8_t);
  uint8_t requestFrom(uint8_t, uint8_t, uint8_t);
  uint8_t requestFrom(int, int);
  uint8_t requestFrom(int, int, int);

  virtual size_t write(uint8_t);
  virtual size_t write(const uint8_t *, size_t);
  virtual int available(void);
  virtual int read(void);
  virtual int peek(void);
  virtual void flush(void);

  using Print::write;
};

extern TwoWire Wire;
extern TwoWire Wire1;

#endif  // TwoWire_h

#endif  // ESP8266
