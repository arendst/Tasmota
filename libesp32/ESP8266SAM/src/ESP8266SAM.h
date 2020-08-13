/*
  ESP8266SAM
  Port of SAM to the ESP8266

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

#ifndef _ESP8266SAM_H
#define _ESP8266SAM_H

#include <Arduino.h>
#include <AudioOutput.h>

class ESP8266SAM {

public:
  ESP8266SAM()
  {
    singmode = false;
    phonetic = false;
    pitch = 0;
    mouth = 0;
    throat = 0;
    speed = 0;
    output = NULL;
  };

  ~ESP8266SAM()
  {
  }

  enum SAMVoice { VOICE_SAM, VOICE_ELF, VOICE_ROBOT, VOICE_STUFFY, VOICE_OLDLADY, VOICE_ET };
  void SetVoice(enum SAMVoice voice);

  void SetSingMode(bool val) { singmode = val; }
  void SetPhonetic(bool val) { phonetic = val; }
  void SetPitch(uint8_t val) { pitch = val; }
  void SetMouth(uint8_t val) { mouth = val; }
  void SetThroat(uint8_t val) { throat = val; }
  void SetSpeed(uint8_t val) { speed = val; }

  bool Say(AudioOutput *out, const char *str);
  bool Say_P(AudioOutput *out, const char *str) {
    char ram[256];
    strncpy_P(ram, str, 256);
    ram[255] = 0;
    return Say(out, ram);
  };

private:
  static void OutputByteCallback(void *cbdata, unsigned char b);
  void OutputByte(unsigned char b);
  bool singmode;
  bool phonetic;
  int pitch;
  int speed;
  int mouth;
  int throat;
  AudioOutput *output;
};

#endif
