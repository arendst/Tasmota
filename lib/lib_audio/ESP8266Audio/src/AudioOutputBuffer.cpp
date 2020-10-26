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

#include <Arduino.h>
#include "AudioOutputBuffer.h"

AudioOutputBuffer::AudioOutputBuffer(int buffSizeSamples, AudioOutput *dest)
{
  buffSize = buffSizeSamples;
  leftSample = (int16_t*)malloc(sizeof(int16_t) * buffSize);
  rightSample = (int16_t*)malloc(sizeof(int16_t) * buffSize);
  writePtr = 0;
  readPtr = 0;
  sink = dest;
}

AudioOutputBuffer::~AudioOutputBuffer()
{
  free(leftSample);
  free(rightSample);
}

bool AudioOutputBuffer::SetRate(int hz)
{
  return sink->SetRate(hz);
}

bool AudioOutputBuffer::SetBitsPerSample(int bits)
{
  return sink->SetBitsPerSample(bits);
}

bool AudioOutputBuffer::SetChannels(int channels)
{
  return sink->SetChannels(channels);
}

bool AudioOutputBuffer::begin()
{
  filled = false;
  return sink->begin();
}

bool AudioOutputBuffer::ConsumeSample(int16_t sample[2])
{
  // First, try and fill I2S...
  if (filled) {
    while (readPtr != writePtr) {
      int16_t s[2] = {leftSample[readPtr], rightSample[readPtr]};
      if (!sink->ConsumeSample(s)) break; // Can't stuff any more in I2S...
      readPtr = (readPtr + 1) % buffSize;
    }
  }

  // Now, do we have space for a new sample?
  int nextWritePtr = (writePtr + 1) % buffSize;
  if (nextWritePtr == readPtr) {
    filled = true;
    return false;
  }
  leftSample[writePtr] = sample[LEFTCHANNEL];
  rightSample[writePtr] = sample[RIGHTCHANNEL];
  writePtr = nextWritePtr;
  return true;
}

bool AudioOutputBuffer::stop()
{
  return sink->stop();
}


