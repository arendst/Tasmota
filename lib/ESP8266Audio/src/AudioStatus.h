/*
  AudioStatus
  Base class for Audio* status/metadata reporting
  
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

#ifndef _AUDIOSTATUS_H
#define _AUDIOSTATUS_H

#include <Arduino.h>

#include "AudioLogger.h"

class AudioStatus
{
  public:
    AudioStatus() { ClearCBs(); };
    virtual ~AudioStatus() {};

    void ClearCBs() { mdFn = NULL; stFn = NULL; };

    typedef void (*metadataCBFn)(void *cbData, const char *type, bool isUnicode, const char *str);
    bool RegisterMetadataCB(metadataCBFn f, void *cbData) { mdFn = f; mdData = cbData; return true; }

    // Returns a unique warning/error code, varying by the object.  The string may be a PSTR, use _P functions!
    typedef void (*statusCBFn)(void *cbData, int code, const char *string);
    bool RegisterStatusCB(statusCBFn f, void *cbData) { stFn = f; stData = cbData; return true; }

    // Safely call the md function, if defined
    inline void md(const char *type, bool isUnicode, const char *string) { if (mdFn) mdFn(mdData, type, isUnicode, string); }

    // Safely call the st function, if defined
    inline void st(int code, const char *string) { if (stFn) stFn(stData, code, string); }

  private:
    metadataCBFn mdFn;
    void *mdData;
    statusCBFn stFn;
    void *stData;
};

#endif

