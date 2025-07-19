/*
  AudioOutputFilterBiquad
  Implements a Biquad filter

  Copyright (C) 2012  Nigel Redmon
  Copyright (C) 2021  William Bérubé

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
#include "AudioOutputFilterBiquad.h"

AudioOutputFilterBiquad::AudioOutputFilterBiquad(AudioOutput *sink)
{
  this->sink = sink;
  
  type = bq_type_lowpass;
  a0 = 1.0;
  a1 = a2 = b1 = b2 = 0.0;
  Fc = 0.50;
  Q = 0.707;
  peakGain = 0.0;
  z1 = z2 = 0.0;
}

AudioOutputFilterBiquad::AudioOutputFilterBiquad(int type, float Fc, float Q, float peakGain, AudioOutput *sink)
{
  this->sink = sink;
  
  SetBiquad(type, Fc, Q, peakGain);
  z1 = z2 = 0.0;
}

AudioOutputFilterBiquad::~AudioOutputFilterBiquad() {}

bool AudioOutputFilterBiquad::SetRate(int hz)
{
  return sink->SetRate(hz);
}

bool AudioOutputFilterBiquad::SetBitsPerSample(int bits)
{
  return sink->SetBitsPerSample(bits);
}

bool AudioOutputFilterBiquad::SetChannels(int channels)
{
  return sink->SetChannels(channels);
}

bool AudioOutputFilterBiquad::SetGain(float gain)
{
  return sink->SetGain(gain);
}

void AudioOutputFilterBiquad::SetType(int type)
{
  this->type = type;
  CalcBiquad();
}

void AudioOutputFilterBiquad::SetFc(float Fc)
{
  this->Fc = Fc;
  CalcBiquad();
}

void AudioOutputFilterBiquad::SetQ(float Q)
{
  this->Q = Q;
  CalcBiquad();
}

void AudioOutputFilterBiquad::SetPeakGain(float peakGain)
{
  this->peakGain = peakGain;
  CalcBiquad();
}

void AudioOutputFilterBiquad::SetBiquad(int type, float Fc, float Q, float peakGain)
{
  this->type = type;
  this->Fc = Fc;
  this->Q = Q;
  this->peakGain = peakGain;
  CalcBiquad();
}

void AudioOutputFilterBiquad::CalcBiquad()
{
  float norm;
  float V = pow(10, fabs(peakGain) / 20.0);
  float K = tan(M_PI * Fc);

  switch (this->type) {
    case bq_type_lowpass:
      norm = 1 / (1 + K / Q + K * K);
      a0 = K * K * norm;
      a1 = 2 * a0;
      a2 = a0;
      b1 = 2 * (K * K - 1) * norm;
      b2 = (1 - K / Q + K * K) * norm;
      break;
      
    case bq_type_highpass:
      norm = 1 / (1 + K / Q + K * K);
      a0 = 1 * norm;
      a1 = -2 * a0;
      a2 = a0;
      b1 = 2 * (K * K - 1) * norm;
      b2 = (1 - K / Q + K * K) * norm;
      break;
      
    case bq_type_bandpass:
      norm = 1 / (1 + K / Q + K * K);
      a0 = K / Q * norm;
      a1 = 0;
      a2 = -a0;
      b1 = 2 * (K * K - 1) * norm;
      b2 = (1 - K / Q + K * K) * norm;
      break;
      
    case bq_type_notch:
      norm = 1 / (1 + K / Q + K * K);
      a0 = (1 + K * K) * norm;
      a1 = 2 * (K * K - 1) * norm;
      a2 = a0;
      b1 = a1;
      b2 = (1 - K / Q + K * K) * norm;
      break;
      
    case bq_type_peak:
      if (peakGain >= 0) {    // boost
        norm = 1 / (1 + 1/Q * K + K * K);
        a0 = (1 + V/Q * K + K * K) * norm;
        a1 = 2 * (K * K - 1) * norm;
        a2 = (1 - V/Q * K + K * K) * norm;
        b1 = a1;
        b2 = (1 - 1/Q * K + K * K) * norm;
      } else {    // cut
        norm = 1 / (1 + V/Q * K + K * K);
        a0 = (1 + 1/Q * K + K * K) * norm;
        a1 = 2 * (K * K - 1) * norm;
        a2 = (1 - 1/Q * K + K * K) * norm;
        b1 = a1;
        b2 = (1 - V/Q * K + K * K) * norm;
      }
      break;

    case bq_type_lowshelf:
      if (peakGain >= 0) {    // boost
        norm = 1 / (1 + sqrt(2) * K + K * K);
        a0 = (1 + sqrt(2*V) * K + V * K * K) * norm;
        a1 = 2 * (V * K * K - 1) * norm;
        a2 = (1 - sqrt(2*V) * K + V * K * K) * norm;
        b1 = 2 * (K * K - 1) * norm;
        b2 = (1 - sqrt(2) * K + K * K) * norm;
      }
      else {    // cut
        norm = 1 / (1 + sqrt(2*V) * K + V * K * K);
        a0 = (1 + sqrt(2) * K + K * K) * norm;
        a1 = 2 * (K * K - 1) * norm;
        a2 = (1 - sqrt(2) * K + K * K) * norm;
        b1 = 2 * (V * K * K - 1) * norm;
        b2 = (1 - sqrt(2*V) * K + V * K * K) * norm;
      }
      break;

    case bq_type_highshelf:
      if (peakGain >= 0) {    // boost
        norm = 1 / (1 + sqrt(2) * K + K * K);
        a0 = (V + sqrt(2*V) * K + K * K) * norm;
        a1 = 2 * (K * K - V) * norm;
        a2 = (V - sqrt(2*V) * K + K * K) * norm;
        b1 = 2 * (K * K - 1) * norm;
        b2 = (1 - sqrt(2) * K + K * K) * norm;
      }
      else {    // cut
        norm = 1 / (V + sqrt(2*V) * K + K * K);
        a0 = (1 + sqrt(2) * K + K * K) * norm;
        a1 = 2 * (K * K - 1) * norm;
        a2 = (1 - sqrt(2) * K + K * K) * norm;
        b1 = 2 * (K * K - V) * norm;
        b2 = (V - sqrt(2*V) * K + K * K) * norm;
      }
      break;
  }

    i_a0 = a0 * BQ_DECAL;
    i_a1 = a1 * BQ_DECAL;
    i_a2 = a2 * BQ_DECAL;

    i_b1 = b1 * BQ_DECAL;
    i_b2 = b2 * BQ_DECAL;

    i_lz1 = i_rz1 = z1 * BQ_DECAL;
    i_lz2 = i_rz2 = z2 * BQ_DECAL;

    i_Fc = Fc * BQ_DECAL;
    i_Q = Q * BQ_DECAL;
    i_peakGain = peakGain * BQ_DECAL;
}

bool AudioOutputFilterBiquad::begin()
{
  return sink->begin();
}

bool AudioOutputFilterBiquad::ConsumeSample(int16_t sample[2])
{

  int32_t leftSample = (sample[LEFTCHANNEL] << BQ_SHIFT) / 2;
  int32_t rightSample = (sample[RIGHTCHANNEL] << BQ_SHIFT) / 2;

  int64_t leftOutput = ((leftSample * i_a0) >> BQ_SHIFT) + i_lz1;
  i_lz1 = ((leftSample * i_a1) >> BQ_SHIFT) + i_lz2 - ((i_b1 * leftOutput) >> BQ_SHIFT);
  i_lz2 = ((leftSample * i_a2) >> BQ_SHIFT) - ((i_b2 * leftOutput) >> BQ_SHIFT);

  int64_t rightOutput = ((rightSample * i_a0) >> BQ_SHIFT) + i_rz1;
  i_rz1 = ((rightSample * i_a1) >> BQ_SHIFT) + i_rz2 - ((i_b1 * rightOutput) >> BQ_SHIFT);
  i_rz2 = ((rightSample * i_a2) >> BQ_SHIFT) - ((i_b2 * rightOutput) >> BQ_SHIFT);
  
  int16_t out[2];
  out[LEFTCHANNEL] = (int16_t)(leftOutput >> BQ_SHIFT);
  out[RIGHTCHANNEL] = (int16_t)(rightOutput >> BQ_SHIFT);

  return sink->ConsumeSample(out);
}

bool AudioOutputFilterBiquad::stop()
{
  return sink->stop();
}
