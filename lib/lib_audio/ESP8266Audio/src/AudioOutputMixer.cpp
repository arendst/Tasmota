/*
  AudioOutputMixer
  Simple mixer which can combine multiple inputs to a single output stream
  
  Copyright (C) 2018  Earle F. Philhower, III

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
#include "AudioOutputMixer.h"

AudioOutputMixerStub::AudioOutputMixerStub(AudioOutputMixer *sink, int id) : AudioOutput()
{
  this->id = id;
  this->parent = sink;
  SetGain(1.0);
}

AudioOutputMixerStub::~AudioOutputMixerStub()
{
  parent->RemoveInput(id);
}

bool AudioOutputMixerStub::SetRate(int hz)
{
  return parent->SetRate(hz, id);
}

bool AudioOutputMixerStub::SetBitsPerSample(int bits)
{
  return parent->SetBitsPerSample(bits, id);
}

bool AudioOutputMixerStub::SetChannels(int channels)
{
  return parent->SetChannels(channels, id);
}

bool AudioOutputMixerStub::begin()
{
  return parent->begin(id);
}

bool AudioOutputMixerStub::ConsumeSample(int16_t sample[2])
{
  int16_t amp[2];
  amp[LEFTCHANNEL] = Amplify(sample[LEFTCHANNEL]);
  amp[RIGHTCHANNEL] = Amplify(sample[RIGHTCHANNEL]);
  return parent->ConsumeSample(amp, id);
}

bool AudioOutputMixerStub::stop()
{
  return parent->stop(id);
}



AudioOutputMixer::AudioOutputMixer(int buffSizeSamples, AudioOutput *dest) : AudioOutput()
{
  buffSize = buffSizeSamples;
  leftAccum = (int32_t*)calloc(sizeof(int32_t), buffSize);
  rightAccum = (int32_t*)calloc(sizeof(int32_t), buffSize);
  for (int i=0; i<maxStubs; i++) {
    stubAllocated[i] = false;
    stubRunning[i] = false;
    writePtr[i] = 0;
  }
  readPtr = 0;
  sink = dest;
  sinkStarted = false;
}

AudioOutputMixer::~AudioOutputMixer()
{
  free(leftAccum);
  free(rightAccum);
}


// Most "standard" interfaces should fail, only MixerStub should be able to talk to us
bool AudioOutputMixer::SetRate(int hz)
{
  (void) hz;
  return false;
}

bool AudioOutputMixer::SetBitsPerSample(int bits)
{
  (void) bits;
  return false;
}

bool AudioOutputMixer::SetChannels(int channels)
{
  (void) channels;
  return false;
}

bool AudioOutputMixer::ConsumeSample(int16_t sample[2])
{
  (void) sample;
  return false;
}

bool AudioOutputMixer::begin()
{
  return false;
}

bool AudioOutputMixer::stop()
{
  return false;
}


// TODO - actually ensure all samples are same speed, size, channels, rate
bool AudioOutputMixer::SetRate(int hz, int id)
{
  (void) id;
  return sink->SetRate(hz);
}

bool AudioOutputMixer::SetBitsPerSample(int bits, int id)
{
  (void) id;
  return sink->SetBitsPerSample(bits);
}

bool AudioOutputMixer::SetChannels(int channels, int id)
{
  (void) id;
  return sink->SetChannels(channels);
}

bool AudioOutputMixer::begin(int id)
{
  stubRunning[id] = true;

  if (!sinkStarted) {
    sinkStarted = true;
    return sink->begin();
  } else {
    return true;
  }
}
  
AudioOutputMixerStub *AudioOutputMixer::NewInput()
{
  for (int i=0; i<maxStubs; i++) {
    if (!stubAllocated[i]) {
      stubAllocated[i] = true;
      stubRunning[i] = false;
      writePtr[i] = readPtr; // TODO - should it be 1 before readPtr?
      AudioOutputMixerStub *stub = new AudioOutputMixerStub(this, i);
      return stub;
    }
  }
  return nullptr;
}

void AudioOutputMixer::RemoveInput(int id)
{
  stubAllocated[id] = false;
  stubRunning[id] = false;
}

bool AudioOutputMixer::loop()
{
  // First, try and fill I2S...
  // This is not optimal, but algorithmically should work fine
  bool avail;
  do {
    avail = true;
    for (int i=0; i<maxStubs && avail; i++) {
      if (stubRunning[i] && writePtr[i] == readPtr) {
        avail = false;  // The read pointer is touching an active writer, can't advance
      }
    }
    if (avail) {
      int16_t s[2] = {0};
      if (leftAccum[readPtr] > 32767) {
        s[LEFTCHANNEL] = 32767;
      } else if (leftAccum[readPtr] < -32767) {
        s[LEFTCHANNEL] = -32767;
      } else {
        s[LEFTCHANNEL] = leftAccum[readPtr];
      }
      if (rightAccum[readPtr] > 32767) {
        s[RIGHTCHANNEL] = 32767;
      } else if (rightAccum[readPtr] < -32767) {
        s[RIGHTCHANNEL] = -32767;
      } else {
        s[RIGHTCHANNEL] = rightAccum[readPtr];
      }
//      s[LEFTCHANNEL] = Amplify(s[LEFTCHANNEL]);
//      s[RIGHTCHANNEL] = Amplify(s[RIGHTCHANNEL]);
      if (!sink->ConsumeSample(s)) {
        break; // Can't stuff any more in I2S...
      }
      // Clear the accums and advance the pointer to next potential sample
      leftAccum[readPtr] = 0;
      rightAccum[readPtr] = 0;
      readPtr = (readPtr + 1) % buffSize;
    }
  } while (avail);
  return true;
}

bool AudioOutputMixer::ConsumeSample(int16_t sample[2], int id)
{
  loop(); // Send any pre-existing, completed I2S data we can fit

  // Now, do we have space for a new sample?
  int nextWritePtr = (writePtr[id] + 1) % buffSize;
  if (nextWritePtr == readPtr) {
    return false;
  }

  leftAccum[writePtr[id]] += sample[LEFTCHANNEL];
  rightAccum[writePtr[id]] += sample[RIGHTCHANNEL];
  writePtr[id] = nextWritePtr;
  return true;
}

bool AudioOutputMixer::stop(int id)
{
  stubRunning[id] = false;
  return true;
}


