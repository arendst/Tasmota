/*
  AudioOutputSTDIO
  Writes a WAV file to the STDIO filesystem
  Only for host-based testing

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

#ifndef _AUDIOOUTPUTSTDIO_H
#define _AUDIOOUTPUTSTDIO_H

#include <Arduino.h>
#ifndef ARDUINO

#include "AudioOutput.h"

class AudioOutputSTDIO : public AudioOutput
{
  public:
    AudioOutputSTDIO() { filename = NULL; f = NULL; };
    ~AudioOutputSTDIO() { free(filename); };
    virtual bool begin() override;
    virtual bool ConsumeSample(int16_t sample[2]) override;
    virtual bool stop() override;
    void SetFilename(const char *name);

  private:
    FILE *f;
    char *filename;
};

#endif

#endif
