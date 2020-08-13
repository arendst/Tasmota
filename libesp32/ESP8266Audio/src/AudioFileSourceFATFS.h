/*
  AudioFileSourceFS
  Input Arduion "file" to be used by AudioGenerator
  
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

#ifndef _AUDIOFILESOURCEFATFS_H
#define _AUDIOFILESOURCEFATFS_H

#ifdef ESP32

#include <Arduino.h>
#include <FS.h>
#include <FFat.h>

#include "AudioFileSource.h"
#include "AudioFileSourceFS.h"

/*
 AudioFileSource for FAT filesystem.
 */
class AudioFileSourceFATFS : public AudioFileSourceFS
{
  public:
    AudioFileSourceFATFS() : AudioFileSourceFS(FFat) {};
    AudioFileSourceFATFS(const char *filename) : AudioFileSourceFS(FFat) {
      // We call open() ourselves because calling AudioFileSourceFS(FFat, filename)
      // would call the parent open() and we do not want that
      open(filename);
    };

    virtual bool open(const char *filename) override {
      // make sure that the FATFS filesystem has been mounted
      if (!FFat.begin()) {
        audioLogger->printf_P(PSTR("Unable to initialize FATFS filesystem\n"));
        return false;
      } else {
        // now that the fielsystem has been mounted, we can call the regular parent open() function
        return AudioFileSourceFS::open(filename);
      }
    };

    // Others are inherited from base
};

#endif


#endif

