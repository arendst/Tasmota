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

#ifndef _AUDIOFILESOURCEPROGMEM_H
#define _AUDIOFILESOURCEPROGMEM_H

#include "AudioFileSource.h"

class AudioFileSourcePROGMEM : public AudioFileSource
{
  public:
    AudioFileSourcePROGMEM();
    AudioFileSourcePROGMEM(const void *data, uint32_t len);
    virtual ~AudioFileSourcePROGMEM() override;
    virtual uint32_t read(void *data, uint32_t len) override;
    virtual bool seek(int32_t pos, int dir) override;
    virtual bool close() override;
    virtual bool isOpen() override;
    virtual uint32_t getSize() override;
    virtual uint32_t getPos() override { if (!opened) return 0; else return filePointer; };

    bool open(const void *data, uint32_t len);

  private:
    bool opened;
    const void *progmemData;
    uint32_t progmemLen;
    uint32_t filePointer;
};

#endif

