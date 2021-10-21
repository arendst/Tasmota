/*
  AudioOutputULP
  Outputs to ESP32 DAC through the ULP, freeing I2S for other uses
  
  Copyright (C) 2020  Martin Laclaustra, based on bitluni's code

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

// Instructions:
// AudioOutputULP out = new AudioOutputULP(); // stereo
// Connect left channel on pin 25
// Connect right channel on pin 26
// OR
// Connect mono channel on either of them (stereo samples are downmixed)
// AudioOutputULP out = new AudioOutputULP(1); // mono, only DAC 1
// OR
// AudioOutputULP out = new AudioOutputULP(2); // mono, only DAC 2


#ifndef _AUDIOOUTPUTULP_H
#define _AUDIOOUTPUTULP_H

#include "AudioOutput.h"

#ifdef ESP32

class AudioOutputULP : public AudioOutput
{
  public:
    AudioOutputULP(int argActiveDACs=3) {if(argActiveDACs<1||argActiveDACs>2)argActiveDACs=3;activeDACs=argActiveDACs;stereoOutput=activeDACs==3;};
    ~AudioOutputULP() {};
    virtual bool begin() override;
    virtual bool ConsumeSample(int16_t sample[2]) override;
    virtual bool stop() override;
    enum : int { DAC1 = 1, DAC2 = 2 };
  private:
    int lastFilledWord = 0;
    uint8_t bufferedOddSample = 128;
    bool waitingOddSample = true; // must be set to false for mono output
    int activeDACs = 3; // 1:DAC1; 2:DAC2; 3:both;
    bool stereoOutput = true;
    const int opcodeCount = 20;
    const uint32_t dacTableStart1 = 2048 - 512;
    const uint32_t dacTableStart2 = dacTableStart1 - 512;
    uint32_t totalSampleWords = 2048 - 512 - 512 - (opcodeCount + 1); // add 512 for mono
    const int totalSamples = totalSampleWords * 2;
    const uint32_t indexAddress = opcodeCount;
    const uint32_t bufferStart = indexAddress + 1;
};

#else

#error Only the ESP32 supports ULP audio output

#endif

#endif
