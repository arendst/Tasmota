/*
  AudioGeneratorTalkie
  Audio output generator that speaks using the LPC code in old TI speech chips
  Output is locked at 8khz as that's that the hardcoded LPC coefficients are built around

  Based on the Talkie Arduino library by Peter Knight, https://github.com/going-digital/Talkie
    
  Copyright (C) 2020 Earle F. Philhower, III

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

#ifndef _AUDIOGENERATORTALKIE_H
#define _AUDIOGENERATORTALKIE_H

#include "AudioGenerator.h"

class AudioGeneratorTalkie : public AudioGenerator
{
  public:
    AudioGeneratorTalkie();
    virtual ~AudioGeneratorTalkie() override;
    virtual bool begin(AudioFileSource *source, AudioOutput *output) override;
    virtual bool loop() override;
    virtual bool stop() override;
    virtual bool isRunning() override;
    bool say(const uint8_t *data, size_t len, bool async = false);
    
  protected:
    // The data stream we're playing
    uint8_t *buff;
    
    // Codeword stream handlers
    uint8_t *ptrAddr;
    uint8_t ptrBit;

    bool    lastFrame;
    bool    genOneFrame(); // Fill up one frame's worth of data, returns if this is the last frame
    int16_t genOneSample(); // Generate one sample of a frame
    
    // Utilities
    uint8_t rev(uint8_t a);
    uint8_t getBits(uint8_t bits);

    // Synthesizer state
    uint8_t  synthPeriod;
    uint16_t synthEnergy;
    int16_t  synthK1, synthK2;
    int8_t   synthK3, synthK4, synthK5, synthK6, synthK7, synthK8, synthK9, synthK10;
    
    int frameLeft;
};

#endif

