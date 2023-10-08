/*
  AudioOutputFilterDecimate
  Implements a user-defined FIR on a passthrough w/rational decimation
  
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

#ifndef _AUDIOOUTPUTFILTERDECIMATE_H
#define _AUDIOOUTPUTFILTERDECIMATE_H

#include "AudioOutput.h"

class AudioOutputFilterDecimate : public AudioOutput
{
  public:
    AudioOutputFilterDecimate(uint8_t taps, const int16_t *tap, int num, int den, AudioOutput *sink);
    virtual ~AudioOutputFilterDecimate() override;
    virtual bool SetRate(int hz) override;
    virtual bool SetBitsPerSample(int bits) override;
    virtual bool SetChannels(int chan) override;
    virtual bool SetGain(float f) override;
    virtual bool begin() override;
    virtual bool ConsumeSample(int16_t sample[2]) override;
    virtual bool stop() override;

  protected:
    AudioOutput *sink;
    uint8_t taps;
    int16_t *tap;
    int16_t *hist[2];
    int idx;
    int num;
    int den;
    int err;
};

#endif

