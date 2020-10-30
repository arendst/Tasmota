/*
  AudioFileSourceSPIFFS
  Input SD card "file" to be used by AudioGenerator
  
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

#include "AudioFileSourceSD.h"

AudioFileSourceSD::AudioFileSourceSD()
{
}

AudioFileSourceSD::AudioFileSourceSD(const char *filename)
{
  open(filename);
}

bool AudioFileSourceSD::open(const char *filename)
{
  f = SD.open(filename, FILE_READ);
  return f;
}

AudioFileSourceSD::~AudioFileSourceSD()
{
  if (f) f.close();
}

uint32_t AudioFileSourceSD::read(void *data, uint32_t len)
{
  return f.read(reinterpret_cast<uint8_t*>(data), len);
}

bool AudioFileSourceSD::seek(int32_t pos, int dir)
{
  if (!f) return false;
  if (dir==SEEK_SET) return f.seek(pos);
  else if (dir==SEEK_CUR) return f.seek(f.position() + pos);
  else if (dir==SEEK_END) return f.seek(f.size() + pos);
  return false;
}

bool AudioFileSourceSD::close()
{
  f.close();
  return true;
}

bool AudioFileSourceSD::isOpen()
{
  return f?true:false;
}

uint32_t AudioFileSourceSD::getSize()
{
  if (!f) return 0;
  return f.size();
}

uint32_t AudioFileSourceSD::getPos()
{
  if (!f) return 0;
  return f.position();
}
