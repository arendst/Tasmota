/*
  AudioOutputSerialWAV
  Writes a mostly correct WAV file to the serial port
  
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

#include "AudioOutputSerialWAV.h"

static const uint8_t wavHeaderTemplate[] PROGMEM = { // Hardcoded simple WAV header with 0xffffffff lengths all around
    0x52, 0x49, 0x46, 0x46, 0xff, 0xff, 0xff, 0xff, 0x57, 0x41, 0x56, 0x45,
    0x66, 0x6d, 0x74, 0x20, 0x10, 0x00, 0x00, 0x00, 0x01, 0x00, 0x02, 0x00, 0x22, 0x56, 0x00, 0x00, 0x88, 0x58, 0x01, 0x00, 0x04, 0x00, 0x10, 0x00,
    0x64, 0x61, 0x74, 0x61, 0xff, 0xff, 0xff, 0xff };

bool AudioOutputSerialWAV::begin()
{
  uint8_t wavHeader[sizeof(wavHeaderTemplate)];
  memcpy_P(wavHeader, wavHeaderTemplate, sizeof(wavHeaderTemplate));
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
  Serial.write(wavHeader, sizeof(wavHeader));
  count = 0;
  return true;
}

bool AudioOutputSerialWAV::ConsumeSample(int16_t sample[2])
{
  if (++count == 200) {
    count = 0;
    return false;
  }
  for (int i=0; i<channels; i++) {
    if (bps == 8) {
      uint8_t l = sample[i] & 0xff;
      Serial.write(l);
    } else {
      uint8_t l = sample[i] & 0xff;
      uint8_t h = (sample[i] >> 8) & 0xff;
      Serial.write(l);
      Serial.write(h);
    }
  }
  return true;
}


bool AudioOutputSerialWAV::stop()
{
  audioLogger->printf_P(PSTR("\n\n\nEOF\n\n\n"));
  return true;
}
 
