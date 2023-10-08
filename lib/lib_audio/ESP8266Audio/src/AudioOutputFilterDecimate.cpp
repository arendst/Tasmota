/*
  AudioOutputFilter
  Implements a user-defined FIR on a passthrough

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
#include "AudioOutputFilterDecimate.h"

AudioOutputFilterDecimate::AudioOutputFilterDecimate(uint8_t taps, const int16_t *tap, int num, int den, AudioOutput *sink)
{
  this->sink = sink;

  // The filter state.  Passed in TAPS must be available throughout object lifetime
  this->taps = taps;
  this->tap = (int16_t*)malloc(sizeof(int16_t) * taps);
  memcpy_P(this->tap, tap, sizeof(int16_t) * taps);
  this->hist[0] = (int16_t*)malloc(sizeof(int16_t) * taps);
  memset(this->hist[0], 0, sizeof(int16_t) * taps);
  this->hist[1] = (int16_t*)malloc(sizeof(int16_t) * taps);
  memset(this->hist[1], 0, sizeof(int16_t) * taps);
  this->idx = 0;

  // Decimator numerator and denominator with an error signal.  Not great, but fast and simple
  this->num = num;
  this->den = den;
  this->err = 0;
}

AudioOutputFilterDecimate::~AudioOutputFilterDecimate()
{
  free(hist[1]);
  free(hist[0]);
  free(tap);
}

bool AudioOutputFilterDecimate::SetRate(int hz)
{
  // Modify input frequency to account for decimation
  hz *= den;
  hz /= num;
  return sink->SetRate(hz);
}

bool AudioOutputFilterDecimate::SetBitsPerSample(int bits)
{
  return sink->SetBitsPerSample(bits);
}

bool AudioOutputFilterDecimate::SetChannels(int channels)
{
  return sink->SetChannels(channels);
}

bool AudioOutputFilterDecimate::SetGain(float gain)
{
  return sink->SetGain(gain);
}

bool AudioOutputFilterDecimate::begin()
{
  return sink->begin();
}

bool AudioOutputFilterDecimate::ConsumeSample(int16_t sample[2])
{
  // Store the data samples in history always
  hist[LEFTCHANNEL][idx] = sample[LEFTCHANNEL];
  hist[RIGHTCHANNEL][idx] = sample[RIGHTCHANNEL];
  idx++;
  if (idx == taps) idx = 0;

  // Only output if the error signal says we're ready to decimate.  This simplistic way might give some aliasing noise
  err += num;
  if (err >= den) {
    err -= den;
    // Need to output a sample, so actually calculate the filter at this point in time
    // Smarter might actually shift the history by the fractional remainder or take two filters and interpolate
    int32_t accL = 0;
    int32_t accR = 0;
    int index = idx;
    for (size_t i=0; i < taps; i++) {
      index = index != 0 ? index-1 : taps-1;
      accL += (int32_t)hist[LEFTCHANNEL][index] * tap[i];
      accR += (int32_t)hist[RIGHTCHANNEL][index] * tap[i];
    };
    int16_t out[2];
    out[LEFTCHANNEL] = accL >> 16;
    out[RIGHTCHANNEL] = accR >> 16;
    return sink->ConsumeSample(out);
  }
  return true; // Nothing to do here...
}

bool AudioOutputFilterDecimate::stop()
{
  return sink->stop();
}
