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

#ifndef _AUDIOOUTPUTI2SNODAC_H
#define _AUDIOOUTPUTI2SNODAC_H

#include "AudioOutputI2S.h"

class AudioOutputI2SNoDAC : public AudioOutputI2S
{
  public:
    AudioOutputI2SNoDAC(int port = 0);
    virtual ~AudioOutputI2SNoDAC() override;
    virtual bool ConsumeSample(int16_t sample[2]) override;
    
    bool SetOversampling(int os);
    
  protected:
    virtual int AdjustI2SRate(int hz) override { return hz * oversample/32; }
    uint8_t oversample;
    void DeltaSigma(int16_t sample[2], uint32_t dsBuff[4]);
    typedef int32_t fixed24p8_t;
    enum {fixedPosValue=0x007fff00}; /* 24.8 of max-signed-int */
    fixed24p8_t lastSamp; // Last sample value
    fixed24p8_t cumErr;   // Running cumulative error since time began
};

#endif

