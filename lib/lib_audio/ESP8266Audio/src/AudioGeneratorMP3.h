/*
  AudioGeneratorMP3
  Wrap libmad MP3 library to play audio
    
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

#ifndef _AUDIOGENERATORMP3_H
#define _AUDIOGENERATORMP3_H

#include "AudioGenerator.h"
#include "libmad/config.h"
#include "libmad/mad.h"

class AudioGeneratorMP3 : public AudioGenerator
{
  public:
    AudioGeneratorMP3();
    AudioGeneratorMP3(void *preallocateSpace, int preallocateSize);
    AudioGeneratorMP3(void *buff, int buffSize, void *stream, int streamSize, void *frame, int frameSize, void *synth, int synthSize);
    virtual ~AudioGeneratorMP3() override;
    virtual bool begin(AudioFileSource *source, AudioOutput *output) override;
    virtual bool loop() override;
    virtual bool stop() override;
    virtual bool isRunning() override;
    virtual void desync () override;

    static constexpr int preAllocSize () { return preAllocBuffSize() + preAllocStreamSize() + preAllocFrameSize() + preAllocSynthSize(); }
    static constexpr int preAllocBuffSize () { return ((buffLen + 7) & ~7); }
    static constexpr int preAllocStreamSize () { return ((sizeof(struct mad_stream) + 7) & ~7); }
    static constexpr int preAllocFrameSize () { return (sizeof(struct mad_frame) + 7) & ~7; }
    static constexpr int preAllocSynthSize () { return (sizeof(struct mad_synth) + 7) & ~7; }

  protected:   
    void *preallocateSpace = nullptr;
    int preallocateSize = 0;
    void *preallocateStreamSpace = nullptr;
    int preallocateStreamSize = 0;
    void *preallocateFrameSpace = nullptr;
    int preallocateFrameSize = 0;
    void *preallocateSynthSpace = nullptr;
    int preallocateSynthSize = 0;

    static constexpr int buffLen = 0x600; // Slightly larger than largest MP3 frame
    unsigned char *buff;
    int lastReadPos;
    int lastBuffLen;
    unsigned int lastRate;
    int lastChannels;
    
    // Decoding bits
    bool madInitted;
    struct mad_stream *stream;
    struct mad_frame *frame;
    struct mad_synth *synth;
    int samplePtr;
    int nsCount;
    int nsCountMax;

    // The internal helpers
    enum mad_flow ErrorToFlow();
    enum mad_flow Input();
    bool DecodeNextFrame();
    bool GetOneSample(int16_t sample[2]);

  private:
    int unrecoverable = 0;
};

#endif

