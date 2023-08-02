/*
  AudioFileSourceFS
  Input "file" to be used by AudioGenerator
  
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

#include "AudioFileSourceFS.h"

AudioFileSourceFS::AudioFileSourceFS(FS &fs, const char *filename)
{
  filesystem = &fs;
  open(filename);
}

bool AudioFileSourceFS::open(const char *filename)
{
#ifndef ESP32
  filesystem->begin();
#endif
  f = filesystem->open(filename, "r");
  return f;
}

AudioFileSourceFS::~AudioFileSourceFS()
{
  if (f) f.close();
}

uint32_t AudioFileSourceFS::read(void *data, uint32_t len)
{
  return f.read(reinterpret_cast<uint8_t*>(data), len);
}

bool AudioFileSourceFS::seek(int32_t pos, int dir)
{
  return f.seek(pos, (dir==SEEK_SET)?SeekSet:(dir==SEEK_CUR)?SeekCur:SeekEnd);
}

bool AudioFileSourceFS::close()
{
  f.close();
  return true;
}

bool AudioFileSourceFS::isOpen()
{
  return f?true:false;
}

uint32_t AudioFileSourceFS::getSize()
{
  if (!f) return 0;
  return f.size();
}


