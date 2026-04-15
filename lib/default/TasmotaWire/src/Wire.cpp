/*
    TwoWire.cpp - TWI/I2C library for Arduino & Wiring
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

extern "C" {
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
}

#include "twi.h"
#include "twi_class.h"
#include "Wire.h"

// Initialize Class Variables //////////////////////////////////////////////////

// Constructors ////////////////////////////////////////////////////////////////

TwoWire::TwoWire() : twi(*new Twi()) {
  //    rxBuffer=new uint8_t[BUFFER_LENGTH];
  rxBufferIndex = 0;
  rxBufferLength = 0;

  txAddress = 0;
  //   txBuffer=new uint8_t[BUFFER_LENGTH];
  txBufferIndex = 0;
  txBufferLength = 0;

  transmitting = 0;

  default_sda_pin = SDA;
  default_scl_pin = SCL;
}

// Public Methods //////////////////////////////////////////////////////////////

void TwoWire::begin(int sda, int scl) {
  default_sda_pin = sda;
  default_scl_pin = scl;
  twi.init(sda, scl);
  flush();
}

void TwoWire::pins(int sda, int scl) {
  default_sda_pin = sda;
  default_scl_pin = scl;
}

void TwoWire::begin(void) {
  begin(default_sda_pin, default_scl_pin);
}

void TwoWire::setClock(uint32_t frequency) {
  twi.setClock(frequency);
}

void TwoWire::setClockStretchLimit(uint32_t limit) {
  twi.setClockStretchLimit(limit);
}

size_t TwoWire::requestFrom(uint8_t address, size_t size, bool sendStop) {
  if (size > BUFFER_LENGTH) {
    size = BUFFER_LENGTH;
  }
  size_t read = (twi.readFrom(address, rxBuffer, size, sendStop) == 0) ? size : 0;
  rxBufferIndex = 0;
  rxBufferLength = read;
  return read;
}

uint8_t TwoWire::requestFrom(uint8_t address, uint8_t quantity, uint8_t sendStop) {
  return requestFrom(address, static_cast<size_t>(quantity), static_cast<bool>(sendStop));
}

uint8_t TwoWire::requestFrom(uint8_t address, uint8_t quantity) {
  return requestFrom(address, static_cast<size_t>(quantity), true);
}

uint8_t TwoWire::requestFrom(int address, int quantity) {
  return requestFrom(static_cast<uint8_t>(address), static_cast<size_t>(quantity), true);
}

uint8_t TwoWire::requestFrom(int address, int quantity, int sendStop) {
  return requestFrom(static_cast<uint8_t>(address), static_cast<size_t>(quantity), static_cast<bool>(sendStop));
}

void TwoWire::beginTransmission(uint8_t address) {
  transmitting = 1;
  txAddress = address;
  txBufferIndex = 0;
  txBufferLength = 0;
}

void TwoWire::beginTransmission(int address) {
  beginTransmission((uint8_t)address);
}

uint8_t TwoWire::endTransmission(uint8_t sendStop) {
  int8_t ret = twi.writeTo(txAddress, txBuffer, txBufferLength, sendStop);
  txBufferIndex = 0;
  txBufferLength = 0;
  transmitting = 0;
  return ret;
}

uint8_t TwoWire::endTransmission(void) {
  return endTransmission(true);
}

size_t TwoWire::write(uint8_t data) {
  if (transmitting){
    if (txBufferLength >= BUFFER_LENGTH) {
      setWriteError();
      return 0;
    }
    txBuffer[txBufferIndex] = data;
    ++txBufferIndex;
    txBufferLength = txBufferIndex;
  }
  return 1;
}

size_t TwoWire::write(const uint8_t *data, size_t quantity) {
  if (transmitting) {
    for (size_t i = 0; i < quantity; ++i) {
      if (!write(data[i])) {
        return i;
      }
    }
  }
  return quantity;
}

int TwoWire::available(void) {
  int result = rxBufferLength - rxBufferIndex;

  if (!result) {
    // yielding here will not make more data "available",
    // but it will prevent the system from going into WDT reset
    optimistic_yield(1000);
  }

  return result;
}

int TwoWire::read(void) {
  int value = -1;
  if (rxBufferIndex < rxBufferLength) {
    value = rxBuffer[rxBufferIndex];
    ++rxBufferIndex;
  }
  return value;
}

int TwoWire::peek(void) {
  int value = -1;
  if (rxBufferIndex < rxBufferLength) {
    value = rxBuffer[rxBufferIndex];
  }
  return value;
}

void TwoWire::flush(void) {
  rxBufferIndex = 0;
  rxBufferLength = 0;
  txBufferIndex = 0;
  txBufferLength = 0;
}

// Preinstantiate Objects //////////////////////////////////////////////////////

TwoWire Wire;
TwoWire Wire1;
