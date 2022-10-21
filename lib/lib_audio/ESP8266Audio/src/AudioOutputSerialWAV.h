/*
  AudioOutputSerialWAV
  Writes a mostly correct WAV file to the serial port
  
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

#ifndef _AUDIOOUTPUTSERIALWAV_H
#define _AUDIOOUTPUTSERIALWAV_H

#include "AudioOutput.h"

class AudioOutputSerialWAV : public AudioOutput
{
  public:
    AudioOutputSerialWAV() {};
    ~AudioOutputSerialWAV() {};
    virtual bool begin() override;
    virtual bool ConsumeSample(int16_t sample[2]) override;
    virtual bool stop() override;
  private:
    int count;
};

#endif

