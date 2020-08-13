/*
  AudioFileSourcePROGMEM
  Store a "file" as a PROGMEM array and use it as audio source data
  
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

#include "AudioFileSourcePROGMEM.h"

AudioFileSourcePROGMEM::AudioFileSourcePROGMEM()
{
  opened = false;
  progmemData = NULL;
  progmemLen = 0;
  filePointer = 0;
}

AudioFileSourcePROGMEM::AudioFileSourcePROGMEM(const void *data, uint32_t len)
{
  open(data, len);
}

AudioFileSourcePROGMEM::~AudioFileSourcePROGMEM()
{
}

bool AudioFileSourcePROGMEM::open(const void *data, uint32_t len)
{
  if (!data || !len) return false;

  opened = true;
  progmemData = data;
  progmemLen = len;
  filePointer = 0;
  return true;
}

uint32_t AudioFileSourcePROGMEM::getSize()
{
  if (!opened) return 0;
  return progmemLen;
}

bool AudioFileSourcePROGMEM::isOpen()
{
  return opened;
}

bool AudioFileSourcePROGMEM::close()
{
  opened = false;
  progmemData = NULL;
  progmemLen = 0;
  filePointer = 0;
  return true;
}  

bool AudioFileSourcePROGMEM::seek(int32_t pos, int dir)
{
  if (!opened) return false;
  uint32_t newPtr;
  switch (dir) {
    case SEEK_SET: newPtr = pos; break;
    case SEEK_CUR: newPtr = filePointer + pos; break;
    case SEEK_END: newPtr = progmemLen - pos; break;
    default: return false;
  }
  if (newPtr > progmemLen) return false;
  filePointer = newPtr;
  return true;
}

uint32_t AudioFileSourcePROGMEM::read(void *data, uint32_t len)
{
  if (!opened) return 0;
  if (filePointer >= progmemLen) return 0;

  uint32_t toRead = progmemLen - filePointer;
  if (toRead > len) toRead = len;

  memcpy_P(data, reinterpret_cast<const uint8_t*>(progmemData)+filePointer, toRead);
  filePointer += toRead;
  return toRead;
}


