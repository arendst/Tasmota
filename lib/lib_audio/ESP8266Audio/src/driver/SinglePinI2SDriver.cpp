/*
  SinglePinI2SDriver  
  ESP8266Audio I2S Minimal driver

  Most of this code is taken and reworked from ESP8266 Arduino core,
  which itself is reworked from Espessif's I2S examples.
  Original code is licensed under LGPL 2.1 or above
 
  Reasons for rewrite:
  - Non-configurable size of DMA buffers
  - We only need GPIO3 connected to I2SO_DATA
  - No API to queue data from ISR. Callbacks are unusable 
    as i2s_write_* functions are not in IRAM and may re-enable 
    SLC interrupt before returning
  - ISR overhead is not needed

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

#if defined(ESP8266)

#include "SinglePinI2SDriver.h"

SinglePinI2SDriver::~SinglePinI2SDriver()
{
  stopI2S();
  freeBuffers(bufCount);
}

bool SinglePinI2SDriver::begin(uint8_t bufCount, uint16_t bufSize)
{
  if (bufCount < 4) return false;
  if ((bufSize < 32) || (bufSize > 1020) || ((bufSize % 4) != 0)) return false;

  stopSLC();

  if ((bufCount != this->bufCount) || (bufSize != this->bufSize)) freeBuffers(this->bufCount);
  this->bufCount = bufCount;
  this->bufSize = bufSize;
  if (!allocateBuffers()) return false;

  zeroBuffers();
  configureSLC();
  startI2S();

  // Send out at least one buffer, so we have valid address of 
  // finished descriptor in SLCRXEDA
  head = &descriptors[0];
  head->owner = 1;
  startSLC();
  while (!lastSentDescriptor()) delayMicroseconds(1);
  // By here, SLC should be stalled (next descriptor owner is 0)
  head->owner = 0;
  head = head->next_link_ptr;
  headPos = 0;
  underflowCount = -1;
  return true;
};

void SinglePinI2SDriver::stop()
{
  stopI2S();
}

bool SinglePinI2SDriver::allocateBuffers() 
{
  // Allocate output (RXLINK) descriptors and bufferss
  if (descriptors) return true;
  descriptors = (SLCDecriptor*) calloc(bufCount, sizeof(SLCDecriptor));
  if (!descriptors) return false;
  int allocated;
  for (allocated = 0; allocated < bufCount; allocated++) {
    uint32_t* buffer = (uint32_t*)malloc(bufSize * sizeof(uint32_t));
    if (!buffer) break;
    auto descriptor = &descriptors[allocated];
    descriptor->eof = 1; // Needed for SLC to update SLCRXEDA
    descriptor->datalen = bufSize * sizeof(uint32_t);
    descriptor->blocksize = bufSize * sizeof(uint32_t);
    descriptor->buf_ptr = buffer;
    descriptor->next_link_ptr = &descriptors[(allocated+1) % bufCount];
  }
  // Release memory if not all buffers were allocated
  if (allocated < bufCount) {
    freeBuffers(allocated);
    return false;
  }
  zeroBuffers();
  return true;
}

void SinglePinI2SDriver::freeBuffers(int allocated)
{
  if (!descriptors) return;
  while (--allocated >= 0) {
    if (descriptors[allocated].buf_ptr) {
      free(descriptors[allocated].buf_ptr);
      descriptors[allocated].buf_ptr = NULL;
    }
  }
  free(descriptors);
  descriptors = NULL;
}

void SinglePinI2SDriver::zeroBuffers()
{
  for(int i = 0; i < bufCount; i++) {    
    auto descriptor = &descriptors[i];
    ets_memset((void *)descriptor->buf_ptr, 0, bufSize * sizeof(uint32_t));
  }
}

inline void SinglePinI2SDriver::advanceHead(const SLCDecriptor *lastSent) 
{
  if (headPos >= bufSize) {
    head->owner = 1; // Owned by SLC
    head = head->next_link_ptr;
    head->owner = 0; // Owned by CPU
    headPos = 0;
    // If SLC stopped, fill-up the buffers before (re)starting
    if (isSLCStopped() && (head->next_link_ptr == lastSent)) { 
      underflowCount++;
      startSLC();
    }
  }
}

bool SinglePinI2SDriver::write(uint32_t sample) 
{
  auto lastSent = lastSentDescriptor();
  if (isSLCRunning() && (lastSent->next_link_ptr == head)) return false;
  head->buf_ptr[headPos++] = sample;
  advanceHead(lastSent);
  return true;
};

bool SinglePinI2SDriver::writeInterleaved(uint32_t samples[4]) 
{
  auto lastSent = lastSentDescriptor();
  if (isSLCRunning() && (lastSent->next_link_ptr == head)) return false;
  uint32_t* buf = head->buf_ptr;
  buf[headPos++] = samples[1];
  buf[headPos++] = samples[0];
  buf[headPos++] = samples[3];
  buf[headPos++] = samples[2];
  advanceHead(lastSent);
  return true;
};

void SinglePinI2SDriver::configureSLC()
{
  ETS_SLC_INTR_DISABLE();
  // Reset SLC, clear interrupts
  SLCC0 |= SLCRXLR | SLCTXLR;
  SLCC0 &= ~(SLCRXLR | SLCTXLR);
  SLCIC = 0xFFFFFFFF;
  // Configure SLC DMA in mode 1
  SLCC0 &= ~(SLCMM << SLCM);
  SLCC0 |= (1 << SLCM);
  SLCRXDC |= SLCBINR | SLCBTNR;
  SLCRXDC &= ~(SLCBRXFE | SLCBRXEM | SLCBRXFM);
  // RXLINK is output DMA, but TXLINK must be configured with valid descriptr
  SLCTXL &= ~(SLCTXLAM << SLCTXLA);
  SLCRXL &= ~(SLCRXLAM << SLCRXLA);
  SLCTXL |= (uint32)&descriptors[0] << SLCTXLA;
  SLCRXL |= (uint32)&descriptors[0] << SLCRXLA;
  // Not setting up interrupts. Not starting DMA here
}

void SinglePinI2SDriver::stopSLC()
{
  // Stop SLC, reset descriptors and clear interrupts
  ETS_SLC_INTR_DISABLE();
  SLCTXL |= SLCTXLE;
  SLCRXL |= SLCRXLE;
  SLCTXL &= ~(SLCTXLAM << SLCTXLA);
  SLCRXL &= ~(SLCRXLAM << SLCRXLA);
  SLCIC = 0xFFFFFFFF;
}

void SinglePinI2SDriver::setDividers(uint8_t div1, uint8_t div2)
{
  // Ensure dividers fit in bit fields
  div1 &= I2SBDM;
  div2 &= I2SCDM;
  // trans master(active low), recv master(active_low), !bits mod(==16 bits/channel), clear clock dividers
  I2SC &= ~(I2STSM | I2SRSM | (I2SBMM << I2SBM) | (I2SBDM << I2SBD) | (I2SCDM << I2SCD));
  // I2SRF = Send/recv right channel first
  // I2SMR = MSB recv/xmit first
  // I2SRMS, I2STMS = We don't really care about WS delay here
  // div1, div2 = Set I2S WS clock frequency. BCLK seems to be generated from 32x this
  I2SC |= I2SRF | I2SMR | I2SRMS | I2STMS | (div1 << I2SBD) | (div2 << I2SCD);
}

void SinglePinI2SDriver::setRate(const uint32_t rateHz)
{
  if (rateHz == currentRate) return;
  currentRate = rateHz;

  uint32_t scaledBaseFreq = I2SBASEFREQ / 32;
  float bestDelta = scaledBaseFreq;
  uint8_t sbdDivBest=1;
  uint8_t scdDivBest=1;
  for (uint8_t i = 1; i < 64; i++) {
    for (uint8_t j = i; j < 64; j++) {
      float delta = fabs(((float)scaledBaseFreq/i/j) - rateHz);
      if (delta < bestDelta){
      	bestDelta = delta;
        sbdDivBest = i;
        scdDivBest = j;
      }
    }
  }

  setDividers(sbdDivBest, scdDivBest);
}

float SinglePinI2SDriver::getActualRate()
{
  return (float)I2SBASEFREQ/32/((I2SC>>I2SBD) & I2SBDM)/((I2SC >> I2SCD) & I2SCDM);
}

void SinglePinI2SDriver::startI2S()
{
  // Setup only I2S SD pin for output
  pinMode(I2SO_DATA, FUNCTION_1);
  I2S_CLK_ENABLE();
  // Reset I2S
  I2SIC = 0x3F;
  I2SIE = 0;
  I2SC &= ~(I2SRST);
  I2SC |= I2SRST;
  I2SC &= ~(I2SRST);  
  // I2STXFMM, I2SRXFMM=0 => 16-bit, dual channel data 
  I2SFC &= ~(I2SDE | (I2STXFMM << I2STXFM) | (I2SRXFMM << I2SRXFM)); 
  I2SFC |= I2SDE; // Enable DMA
  // I2STXCMM, I2SRXCMM=0 => Dual channel mode, RX/TX CHAN_MOD=0
  I2SCC &= ~((I2STXCMM << I2STXCM) | (I2SRXCMM << I2SRXCM));
  // Set dividers to something reasonable
  currentRate = 0;
  setRate(44100);
  // Start I2S peripheral
  I2SC |= I2STXS; 
}

void SinglePinI2SDriver::stopI2S()
{
  // Disable any I2S send or receive
  I2SC &= ~(I2STXS | I2SRXS);
  // Reset I2S
  I2SC &= ~(I2SRST);
  I2SC |= I2SRST;
  I2SC &= ~(I2SRST);

  stopSLC();
  // Reconnect RX0 ?
  //pinMode(I2SO_DATA, SPECIAL);
}

int SinglePinI2SDriver::getUnderflowCount()
{
  int count = underflowCount;
  underflowCount = 0;
  return count;
}

// Global instance
SinglePinI2SDriver I2SDriver;

#endif // defined(ESP8266)
