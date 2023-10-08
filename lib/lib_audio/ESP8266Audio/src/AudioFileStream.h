/*
  AudioFileStream
  Convert an AudioFileSource* to a Stream*
  
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

#ifndef AUDIOFILESTREAM_H
#define AUDIOFILESTREAM_H

#include <Arduino.h>
#include "AudioFileSource.h"

class AudioFileStream : public Stream
{
public:
  AudioFileStream(AudioFileSource *source, int definedLen);
  virtual ~AudioFileStream();

public:
  // Stream interface - see the Arduino library documentation.
  virtual int available() override;
  virtual int read() override;
  virtual int peek() override;
  virtual void flush() override;
  virtual size_t write(uint8_t x) override { (void)x; return 0; };

private:
  AudioFileSource *src;
  int saved;
  int len;
  int ptr;
};

#endif
