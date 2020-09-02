/*
  SinglePinI2SDriver  
  ESP8266Audio I2S Minimal driver

  Copyright (C) 2020 Ivan Kostoski

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

#pragma once
#if defined(ESP8266)

#include <Arduino.h>
#include "osapi.h"
#include "ets_sys.h"
#include "i2s_reg.h"

struct SLCDecriptor {
  uint32_t          blocksize : 12;
  uint32_t          datalen   : 12;
  uint32_t          unused    : 5;
  uint32_t          sub_sof   : 1;
  uint32_t          eof       : 1;
  volatile uint32_t owner     : 1;
  uint32_t*         buf_ptr;
  SLCDecriptor* next_link_ptr;
};

class SinglePinI2SDriver
{
  public:
    enum : uint8_t { I2SO_DATA = 3 }; // GPIO3/RX0

    SinglePinI2SDriver(): descriptors(NULL), bufCount(0), bufSize(0) {};
    ~SinglePinI2SDriver();

    bool begin(uint8_t bufCount, uint16_t bufSize);
    void stop();
    void setRate(const uint32_t rateHz);
    float getActualRate();
    bool write(uint32_t sample);
    bool writeInterleaved(uint32_t samples[4]);
    int getUnderflowCount();

  protected:
    bool allocateBuffers();
    void freeBuffers(int allocated);
    void zeroBuffers();
    void advanceHead(const SLCDecriptor *finished);
    void configureSLC();
    void stopSLC();
    void setDividers(uint8_t div1, uint8_t div2);
    void startI2S();
    void stopI2S();
    SLCDecriptor *descriptors;
    uint16_t bufCount;
    uint16_t bufSize;
    SLCDecriptor *head;
    uint32_t headPos;
    uint32_t currentRate;
    int underflowCount;

    // (Re)Start transmission ("TX" DMA always needed to be enabled)
    static inline void startSLC() { SLCTXL |= SLCTXLS; SLCRXL |= SLCRXLS; }
    static inline uint32_t isSLCRunning() { return (SLCRXS & SLCRXF); };  
    static inline uint32_t isSLCStopped() { return (SLCRXS & SLCRXE); }; 
    static inline SLCDecriptor *lastSentDescriptor() { return (SLCDecriptor*)SLCRXEDA; }
};

// Global instance
extern SinglePinI2SDriver I2SDriver;

#endif // defined(ESP8266)
