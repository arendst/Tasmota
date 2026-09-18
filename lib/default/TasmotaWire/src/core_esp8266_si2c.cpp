/*
    si2c.c - Software I2C library for esp8266

    Copyright (c) 2015 Hristo Gochkov. All rights reserved.
    This file is part of the esp8266 core for Arduino environment.

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
*/
#include "twi.h"
#include "twi_class.h"
#include "pins_arduino.h"
#include "wiring_private.h"
#include "PolledTimeout.h"

extern "C" {
#include "ets_sys.h"
};

// Inline helpers
static inline __attribute__((always_inline)) void SDA_LOW(const int twi_sda) {
  GPES = (1 << twi_sda);
}
static inline __attribute__((always_inline)) void SDA_HIGH(const int twi_sda) {
  GPEC = (1 << twi_sda);
}
static inline __attribute__((always_inline)) bool SDA_READ(const int twi_sda) {
  return (GPI & (1 << twi_sda)) != 0;
}
static inline __attribute__((always_inline)) void SCL_LOW(const int twi_scl) {
//  GPES = (1 << twi_scl);
  (twi_scl != 16) ? GPES = (1 << twi_scl) : GP16O &= ~1;
}
static inline __attribute__((always_inline)) void SCL_HIGH(const int twi_scl) {
//  GPEC = (1 << twi_scl);
  (twi_scl != 16) ? GPEC = (1 << twi_scl) : GP16O |= 1;
}
static inline __attribute__((always_inline)) bool SCL_READ(const int twi_scl) {
//  return (GPI & (1 << twi_scl)) != 0;
  return (twi_scl != 16) ? (GPI & (1 << twi_scl)) != 0 : (GP16I & 0x01);
}

// Implement as a class to reduce code size by allowing access to many global variables with a single base pointer

#ifndef FCPU80
#define FCPU80 80000000L
#endif

// Handle the case where a slave needs to stretch the clock with a time-limited busy wait
void Twi::WAIT_CLOCK_STRETCH() {
  esp8266::polledTimeout::oneShotFastUs timeout(twi_clockStretchLimit);
  esp8266::polledTimeout::periodicFastUs yieldTimeout(5000);
  while (!timeout && !SCL_READ(twi_scl)) {  // outer loop is stretch duration up to stretch limit
    if (yieldTimeout) {  // inner loop yields every 5ms
      yield();
    }
  }
}

void Twi::setClock(unsigned int freq) {
  if (freq < 1000) {  // minimum freq 1000Hz to minimize slave timeouts and WDT resets
    freq = 1000;
  }

  preferred_si2c_clock = freq;

#if F_CPU == FCPU80

  if (freq > 400000) {
    freq = 400000;
  }
  twi_dcount = (500000000 / freq);                   // half-cycle period in ns
  twi_dcount = (1000 * (twi_dcount - 1120)) / 62500; // (half cycle - overhead) / busywait loop time

#else

  if (freq > 800000) {
    freq = 800000;
  }
  twi_dcount = (500000000 / freq);                  // half-cycle period in ns
  twi_dcount = (1000 * (twi_dcount - 560)) / 31250; // (half cycle - overhead) / busywait loop time

#endif
}

void Twi::setClockStretchLimit(uint32_t limit) {
  twi_clockStretchLimit = limit;
}

void Twi::init(unsigned char sda, unsigned char scl) {
  twi_sda = sda;
  twi_scl = scl;
  pinMode(twi_sda, INPUT_PULLUP);
//    pinMode(twi_scl, INPUT_PULLUP);
  if (16 == twi_scl) {
    pinMode(twi_scl, OUTPUT);
  } else {
    pinMode(twi_scl, INPUT_PULLUP);
  }
  twi_setClock(preferred_si2c_clock);
  twi_setClockStretchLimit(150000L); // default value is 150 mS
}

void ICACHE_RAM_ATTR Twi::busywait(unsigned int v) {
  unsigned int i;
  for (i = 0; i < v; i++) {  // loop time is 5 machine cycles: 31.25ns @ 160MHz, 62.5ns @ 80MHz
    asm("nop"); // minimum element to keep GCC from optimizing this function out.
  }
}

bool Twi::write_start(void) {
  SCL_HIGH(twi_scl);
  SDA_HIGH(twi_sda);
  if (!SDA_READ(twi_sda)) {
    return false;
  }
  busywait(twi_dcount);
  SDA_LOW(twi_sda);
  busywait(twi_dcount);
  return true;
}

bool Twi::write_stop(void) {
  SCL_LOW(twi_scl);
  SDA_LOW(twi_sda);
  busywait(twi_dcount);
  SCL_HIGH(twi_scl);
  WAIT_CLOCK_STRETCH();
  busywait(twi_dcount);
  SDA_HIGH(twi_sda);
  busywait(twi_dcount);
  return true;
}

bool Twi::write_bit(bool bit) {
  SCL_LOW(twi_scl);
  if (bit) {
    SDA_HIGH(twi_sda);
  } else {
    SDA_LOW(twi_sda);
  }
  busywait(twi_dcount + 1);
  SCL_HIGH(twi_scl);
  WAIT_CLOCK_STRETCH();
  busywait(twi_dcount);
  return true;
}

bool Twi::read_bit(void) {
  SCL_LOW(twi_scl);
  SDA_HIGH(twi_sda);
  busywait(twi_dcount + 2);
  SCL_HIGH(twi_scl);
  WAIT_CLOCK_STRETCH();
  bool bit = SDA_READ(twi_sda);
  busywait(twi_dcount);
  return bit;
}

bool Twi::write_byte(unsigned char byte) {
  unsigned char bit;
  for (bit = 0; bit < 8; bit++) {
    write_bit(byte & 0x80);
    byte <<= 1;
  }
  return !read_bit(); //NACK/ACK
}

unsigned char Twi::read_byte(bool nack) {
  unsigned char byte = 0;
  unsigned char bit;
  for (bit = 0; bit < 8; bit++) {
    byte = (byte << 1) | read_bit();
  }
  write_bit(nack);
  return byte;
}

unsigned char Twi::writeTo(unsigned char address, unsigned char *buf, unsigned int len, unsigned char sendStop) {
  unsigned int i;
  if (!write_start()) {
    return 4; //line busy
  }
  if (!write_byte(((address << 1) | 0) & 0xFF)) {
    if (sendStop) {
      write_stop();
    }
    return 2; //received NACK on transmit of address
  }
  for (i = 0; i < len; i++) {
    if (!write_byte(buf[i])) {
      if (sendStop) {
        write_stop();
      }
      return 3; //received NACK on transmit of data
    }
  }
  if (sendStop) {
    write_stop();
  } else {
    twi_scl_valley();
    // TD-er: Also busywait(twi_dcount) here?
    // busywait(twi_dcount);
  }
  i = 0;
  while (!SDA_READ(twi_sda) && (i++) < 10) {
    twi_scl_valley();
    busywait(twi_dcount);
  }
  return 0;
}

unsigned char Twi::readFrom(unsigned char address, unsigned char *buf, unsigned int len, unsigned char sendStop) {
  unsigned int i;
  if (!write_start()) {
    return 4; //line busy
  }
  if (!write_byte(((address << 1) | 1) & 0xFF)) {
    if (sendStop) {
      write_stop();
    }
    return 2; //received NACK on transmit of address
  }
  for (i = 0; i < (len - 1); i++) {
    buf[i] = read_byte(false);
  }
  buf[len - 1] = read_byte(true);
  if (sendStop) {
      write_stop();
  } else {
    twi_scl_valley();
    // TD-er: Also busywait(twi_dcount) here?
    // busywait(twi_dcount);
  }
  i = 0;
  while (!SDA_READ(twi_sda) && (i++) < 10) {
    twi_scl_valley();
    busywait(twi_dcount);
  }
  return 0;
}

void Twi::twi_scl_valley(void) {
  SCL_LOW(twi_scl);
  busywait(twi_dcount);
  SCL_HIGH(twi_scl);
  WAIT_CLOCK_STRETCH();
}

uint8_t Twi::status() {
  WAIT_CLOCK_STRETCH(); // wait for a slow slave to finish
  if (!SCL_READ(twi_scl)) {
    return I2C_SCL_HELD_LOW; // SCL held low by another device, no procedure available to recover
  }

  int clockCount = 20;
  while (!SDA_READ(twi_sda) && clockCount-- > 0) { // if SDA low, read the bits slaves have to sent to a max
    read_bit();
    if (!SCL_READ(twi_scl)) {
      return I2C_SCL_HELD_LOW_AFTER_READ; // I2C bus error. SCL held low beyond slave clock stretch time
    }
  }
  if (!SDA_READ(twi_sda)) {
    return I2C_SDA_HELD_LOW; // I2C bus error. SDA line held low by slave/another_master after n bits.
  }

  return I2C_OK;
}

static Twi twi;

// C wrappers for the object, since API is exposed only as C
extern "C" {
  void twi_init(unsigned char sda, unsigned char scl) {
    return twi.init(sda, scl);
  }

  void twi_setClock(unsigned int freq) {
    twi.setClock(freq);
  }

  void twi_setClockStretchLimit(uint32_t limit) {
    twi.setClockStretchLimit(limit);
  }

  uint8_t twi_writeTo(unsigned char address, unsigned char *buf, unsigned int len, unsigned char sendStop) {
    return twi.writeTo(address, buf, len, sendStop);
  }

  uint8_t twi_readFrom(unsigned char address, unsigned char *buf, unsigned int len, unsigned char sendStop) {
    return twi.readFrom(address, buf, len, sendStop);
  }

  uint8_t twi_status() {
    return twi.status();
  }
};
