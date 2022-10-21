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

#ifndef _AudioOutputFilterBiquad_H
#define _AudioOutputFilterBiquad_H

#include "AudioOutput.h"

#define      BQ_SHIFT 16
#define      BQ_DECAL 65536

enum {
    bq_type_lowpass = 0,
    bq_type_highpass,
    bq_type_bandpass,
    bq_type_notch,
    bq_type_peak,
    bq_type_lowshelf,
    bq_type_highshelf
};

class AudioOutputFilterBiquad : public AudioOutput
{
  public:
    AudioOutputFilterBiquad(AudioOutput *sink);
    AudioOutputFilterBiquad(int type, float Fc, float Q, float peakGain, AudioOutput *sink);
    virtual ~AudioOutputFilterBiquad() override;
    virtual bool SetRate(int hz) override;
    virtual bool SetBitsPerSample(int bits) override;
    virtual bool SetChannels(int chan) override;
    virtual bool SetGain(float f) override;
    virtual bool begin() override;
    virtual bool ConsumeSample(int16_t sample[2]) override;
    virtual bool stop() override;

  private:
    void SetType(int type);
    void SetFc(float Fc);
    void SetQ(float Q);
    void SetPeakGain(float peakGain);
    void SetBiquad(int type, float Fc, float Q, float peakGain);

  protected:
    AudioOutput *sink;
    int buffSize;
    int16_t *leftSample;
    int16_t *rightSample;
    int writePtr;
    int readPtr;
    bool filled;
    int type;
    void CalcBiquad();
    int64_t i_a0, i_a1, i_a2, i_b1, i_b2;
    int64_t i_Fc, i_Q, i_peakGain;
    int64_t i_lz1, i_lz2, i_rz1, i_rz2;
    float a0, a1, a2, b1, b2;
    float Fc, Q, peakGain;
    float z1, z2;
};

#endif

