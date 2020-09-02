/*
  AudioOutputI2S
  Base class for an I2S output port
  
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

#ifndef _AUDIOOUTPUTI2S_H
#define _AUDIOOUTPUTI2S_H

#include "AudioOutput.h"

class AudioOutputI2S : public AudioOutput
{
  public:
    AudioOutputI2S(int port=0, int output_mode=EXTERNAL_I2S, int dma_buf_count = 8, int use_apll=APLL_DISABLE);
    virtual ~AudioOutputI2S() override;
    bool SetPinout(int bclkPin, int wclkPin, int doutPin);
    virtual bool SetRate(int hz) override;
    virtual bool SetBitsPerSample(int bits) override;
    virtual bool SetChannels(int channels) override;
    virtual bool begin() override;
    virtual bool ConsumeSample(int16_t sample[2]) override;
    virtual void flush() override;
    virtual bool stop() override;
    
    bool SetOutputModeMono(bool mono);  // Force mono output no matter the input

    enum : int { APLL_AUTO = -1, APLL_ENABLE = 1, APLL_DISABLE = 0 };
    enum : int { EXTERNAL_I2S = 0, INTERNAL_DAC = 1, INTERNAL_PDM = 2 };

  protected:
    virtual int AdjustI2SRate(int hz) { return hz; }
    uint8_t portNo;
    int output_mode;
    bool mono;
    bool i2sOn;
    int dma_buf_count;
    // We can restore the old values and free up these pins when in NoDAC mode
    uint32_t orig_bck;
    uint32_t orig_ws;
};

#endif

