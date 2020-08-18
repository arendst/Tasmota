/*
  AudioOutputI2SNoDAC
  Audio player using SW delta-sigma to generate "analog" on I2S data
 
  Copyright (C) 2017  Earle F. Philhower, III

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <Arduino.h>
#ifdef ESP32
  #include "driver/i2s.h"
#else
  #include <i2s.h>
#endif
#include "AudioOutputI2SNoDAC.h"


AudioOutputI2SNoDAC::AudioOutputI2SNoDAC(int port) : AudioOutputI2S(port, false)
{
  SetOversampling(32);
  lastSamp = 0;
  cumErr = 0;
#ifndef ESP32
  WRITE_PERI_REG(PERIPHS_IO_MUX_MTDO_U, orig_bck);
  WRITE_PERI_REG(PERIPHS_IO_MUX_GPIO2_U, orig_ws);
#endif
}

AudioOutputI2SNoDAC::~AudioOutputI2SNoDAC()
{
}

bool AudioOutputI2SNoDAC::SetOversampling(int os) {
  if (os % 32) return false;  // Only Nx32 oversampling supported
  if (os > 256) return false; // Don't be silly now!
  if (os < 32) return false;  // Nothing under 32 allowed

  oversample = os;
  return SetRate(hertz);
}

void AudioOutputI2SNoDAC::DeltaSigma(int16_t sample[2], uint32_t dsBuff[8])
{
  // Not shift 8 because addition takes care of one mult x 2
  int32_t sum = (((int32_t)sample[0]) + ((int32_t)sample[1])) >> 1;
  fixed24p8_t newSamp = ( (int32_t)Amplify(sum) ) << 8;

  int oversample32 = oversample / 32;
  // How much the comparison signal changes each oversample step
  fixed24p8_t diffPerStep = (newSamp - lastSamp) >> (4 + oversample32);

  // Don't need lastSamp anymore, store this one for next round
  lastSamp = newSamp;

  for (int j = 0; j < oversample32; j++) {
    uint32_t bits = 0; // The bits we convert the sample into, MSB to go on the wire first
    
    for (int i = 32; i > 0; i--) {
      bits = bits << 1;
      if (cumErr < 0) {
        bits |= 1;
        cumErr += fixedPosValue - newSamp;
      } else {
        // Bits[0] = 0 handled already by left shift
        cumErr -= fixedPosValue + newSamp;
      }
      newSamp += diffPerStep; // Move the reference signal towards destination
    }
    dsBuff[j] = bits;
  }
}

bool AudioOutputI2SNoDAC::ConsumeSample(int16_t sample[2])
{
  int16_t ms[2];
  ms[0] = sample[0];
  ms[1] = sample[1];
  MakeSampleStereo16( ms );

  // Make delta-sigma filled buffer
  uint32_t dsBuff[8];
  DeltaSigma(ms, dsBuff);

  // Either send complete pulse stream or nothing
#ifdef ESP32
  if (!i2s_write_bytes((i2s_port_t)portNo, (const char *)dsBuff, sizeof(uint32_t) * (oversample/32), 0))
    return false;
#else 
  if (!i2s_write_sample_nb(dsBuff[0])) return false; // No room at the inn
  // At this point we've sent in first of possibly 8 32-bits, need to send
  // remaining ones even if they block.
  for (int i = 32; i < oversample; i+=32)
    i2s_write_sample( dsBuff[i / 32]);
#endif
  return true;
}
