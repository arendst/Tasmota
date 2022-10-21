/*
  AudioFileSource
  Base class of an input "file" to be used by AudioGenerator
  
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

#ifndef _AUDIOFILESOURCE_H
#define _AUDIOFILESOURCE_H

#include <Arduino.h>
#include "AudioStatus.h"

class AudioFileSource
{
  public:
    AudioFileSource() {};
    virtual ~AudioFileSource() {};
    virtual bool open(const char *filename) { (void)filename; return false; };
    virtual uint32_t read(void *data, uint32_t len) { (void)data; (void)len; return 0; };
    virtual uint32_t readNonBlock(void *data, uint32_t len) { return read(data, len); }; 
    virtual bool seek(int32_t pos, int dir) { (void)pos; (void)dir; return false; };
    virtual bool close() { return false; };
    virtual bool isOpen() { return false; };
    virtual uint32_t getSize() { return 0; };
    virtual uint32_t getPos() { return 0; };
    virtual bool loop() { return true; };

  public:
    virtual bool RegisterMetadataCB(AudioStatus::metadataCBFn fn, void *data) { return cb.RegisterMetadataCB(fn, data); }
    virtual bool RegisterStatusCB(AudioStatus::statusCBFn fn, void *data) { return cb.RegisterStatusCB(fn, data); }

  protected:
    AudioStatus cb;
};

#endif

