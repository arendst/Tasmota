/*
  AudioOutputSTDIO
  Writes a WAV file to the STDIO filesystem
  Only for host-based testing

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

#include <Arduino.h>

#ifndef ARDUINO

#include "AudioOutputSTDIO.h"
#include <unistd.h>

static const uint8_t wavHeaderTemplate[] PROGMEM = { // Hardcoded simple WAV header with 0xffffffff lengths all around
    0x52, 0x49, 0x46, 0x46, 0xff, 0xff, 0xff, 0xff, 0x57, 0x41, 0x56, 0x45,
    0x66, 0x6d, 0x74, 0x20, 0x10, 0x00, 0x00, 0x00, 0x01, 0x00, 0x02, 0x00, 0x22, 0x56, 0x00, 0x00, 0x88, 0x58, 0x01, 0x00, 0x04, 0x00, 0x10, 0x00,
    0x64, 0x61, 0x74, 0x61, 0xff, 0xff, 0xff, 0xff };

void AudioOutputSTDIO::SetFilename(const char *name)
{
  free(filename);
  filename = strdup(name);
}

bool AudioOutputSTDIO::begin()
{
  uint8_t wavHeader[sizeof(wavHeaderTemplate)];
  memset(wavHeader, 0, sizeof(wavHeader));

  if (f) return false; // Already open!
  unlink(filename);
  f = fopen(filename, "wb+");
  if (!f) return false;
  
  // We'll fix the header up when we close the file
  fwrite(wavHeader, sizeof(wavHeader), 1, f);
  return true;
}

bool AudioOutputSTDIO::ConsumeSample(int16_t sample[2])
{
  for (int i=0; i<channels; i++) {
    if (bps == 8) {
      uint8_t l = sample[i] & 0xff;
      fwrite(&l, sizeof(l), 1, f);
    } else {
      uint8_t l = sample[i] & 0xff;
      uint8_t h = (sample[i] >> 8) & 0xff;
      fwrite(&l, sizeof(l), 1, f);
      fwrite(&h, sizeof(h), 1, f);
    }
  }
  return true;
}


bool AudioOutputSTDIO::stop()
{
  uint8_t wavHeader[sizeof(wavHeaderTemplate)];

  memcpy_P(wavHeader, wavHeaderTemplate, sizeof(wavHeaderTemplate));

  int chunksize = ftell(f) - 8;
  wavHeader[4] = chunksize & 0xff;
  wavHeader[5] = (chunksize>>8)&0xff;
  wavHeader[6] = (chunksize>>16)&0xff;
  wavHeader[7] = (chunksize>>24)&0xff;

  wavHeader[22] = channels & 0xff;
  wavHeader[23] = 0;

  wavHeader[24] = hertz & 0xff;
  wavHeader[25] = (hertz >> 8) & 0xff;
  wavHeader[26] = (hertz >> 16) & 0xff;
  wavHeader[27] = (hertz >> 24) & 0xff;
  int byteRate = hertz * bps * channels / 8;
  wavHeader[28] = byteRate & 0xff;
  wavHeader[29] = (byteRate >> 8) & 0xff;
  wavHeader[30] = (byteRate >> 16) & 0xff;
  wavHeader[31] = (byteRate >> 24) & 0xff;
  wavHeader[32] = channels * bps / 8;
  wavHeader[33] = 0;
  wavHeader[34] = bps;
  wavHeader[35] = 0;

  int datasize = ftell(f) - sizeof(wavHeader);
  wavHeader[40] = datasize & 0xff;
  wavHeader[41] = (datasize>>8)&0xff;
  wavHeader[42] = (datasize>>16)&0xff;
  wavHeader[43] = (datasize>>24)&0xff;

  // Write real header out
  fseek(f, 0, SEEK_SET);
  fwrite(wavHeader, sizeof(wavHeader), 1, f);
  fclose(f);
  return true;
}

#endif
 
