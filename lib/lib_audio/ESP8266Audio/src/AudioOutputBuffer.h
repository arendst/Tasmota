/*
  AudioOutputBuffer
  Adds additional bufferspace to the output chain
  
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

#ifndef _AUDIOOUTPUTBUFFER_H
#define _AUDIOOUTPUTBUFFER_H

#include "AudioOutput.h"

class AudioOutputBuffer : public AudioOutput
{
  public:
    AudioOutputBuffer(int bufferSizeSamples, AudioOutput *dest);
    virtual ~AudioOutputBuffer() override;
    virtual bool SetRate(int hz) override;
    virtual bool SetBitsPerSample(int bits) override;
    virtual bool SetChannels(int channels) override;
    virtual bool begin() override;
    virtual bool ConsumeSample(int16_t sample[2]) override;
    virtual bool stop() override;
    
  protected:
    AudioOutput *sink;
    int buffSize;
    int16_t *leftSample;
    int16_t *rightSample;
    int writePtr;
    int readPtr;
    bool filled;
};

#endif

