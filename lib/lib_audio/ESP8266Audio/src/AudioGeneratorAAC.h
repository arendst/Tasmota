/*
  AudioGeneratorAAC
  Audio output generator using the Helix AAC decoder
  
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

#ifndef _AUDIOGENERATORAAC_H
#define _AUDIOGENERATORAAC_H

#include "AudioGenerator.h"
#include "libhelix-aac/aacdec.h"

class AudioGeneratorAAC : public AudioGenerator
{
  public:
    AudioGeneratorAAC();
    AudioGeneratorAAC(void *preallocateData, int preallocateSize);
    virtual ~AudioGeneratorAAC() override;
    virtual bool begin(AudioFileSource *source, AudioOutput *output) override;
    virtual bool loop() override;
    virtual bool stop() override;
    virtual bool isRunning() override;

  protected:
    void *preallocateSpace;
    int preallocateSize;

    // Helix AAC decoder
    HAACDecoder hAACDecoder;

    // Input buffering
    const int buffLen = 1600;
    uint8_t *buff; //[1600]; // File buffer required to store at least a whole compressed frame
    int16_t buffValid;
    int16_t lastFrameEnd;
    bool FillBufferWithValidFrame(); // Read until we get a valid syncword and min(feof, 2048) butes in the buffer

    // Output buffering
    int16_t *outSample; //[1024 * 2]; // Interleaved L/R
    int16_t validSamples;
    int16_t curSample;

    // Each frame may change this if they're very strange, I guess
    unsigned int lastRate;
    int lastChannels;

};

#endif

