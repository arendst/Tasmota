/*
  AudioGeneratorRTTTL
  Audio output generator that plays RTTTL (Nokia ringtones)

  Based on the Rtttl Arduino library by James BM, https://github.com/spicajames/Rtttl
  Based on the gist from Daniel Hall https://gist.github.com/smarthall/1618800
    
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

#ifndef _AUDIOGENERATORRTTTL_H
#define _AUDIOGENERATORRTTTL_H

#include "AudioGenerator.h"

class AudioGeneratorRTTTL : public AudioGenerator
{
  public:
    AudioGeneratorRTTTL();
    virtual ~AudioGeneratorRTTTL() override;
    virtual bool begin(AudioFileSource *source, AudioOutput *output) override;
    virtual bool loop() override;
    virtual bool stop() override;
    virtual bool isRunning() override;
    void SetRate(uint16_t hz) { rate = hz; }

  private:
    bool SkipWhitespace();
    bool ReadInt(int *dest);
    bool ParseHeader();
    bool GetNextNote();
    
  protected:
    uint16_t rate;

    // We copy the entire tiny song to a buffer for easier access
    char *buff;
    int len;
    int ptr;

    // Song-global settings
    int defaultDuration;
    int defaultOctave;
    int wholeNoteMS;

    // The note we're currently playing
    int ttlSamplesPerWaveFP10;
    int ttlSamples;
    int samplesSent;
};

#endif

