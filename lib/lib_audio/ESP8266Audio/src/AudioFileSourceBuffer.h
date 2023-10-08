/*
  AudioFileSourceBuffer
  Double-buffered input file using system RAM
  
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

#ifndef _AUDIOFILESOURCEBUFFER_H
#define _AUDIOFILESOURCEBUFFER_H

#include "AudioFileSource.h"


class AudioFileSourceBuffer : public AudioFileSource
{
  public:
    AudioFileSourceBuffer(AudioFileSource *in, uint32_t bufferBytes);
    AudioFileSourceBuffer(AudioFileSource *in, void *buffer, uint32_t bufferBytes); // Pre-allocated buffer by app
    virtual ~AudioFileSourceBuffer() override;
    
    virtual uint32_t read(void *data, uint32_t len) override;
    virtual bool seek(int32_t pos, int dir) override;
    virtual bool close() override;
    virtual bool isOpen() override;
    virtual uint32_t getSize() override;
    virtual uint32_t getPos() override;
    virtual bool loop() override;

    virtual uint32_t getFillLevel();

    enum { STATUS_FILLING=2, STATUS_UNDERFLOW };

  private:
    virtual void fill();

  private:
    AudioFileSource *src;
    uint32_t buffSize;
    uint8_t *buffer;
    bool deallocateBuffer;
    uint32_t writePtr;
    uint32_t readPtr;
    uint32_t length;
    bool filled;
};


#endif

