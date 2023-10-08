
/*
  audio is2 RTTTL

  Copyright (C) 2023  Fernando Pena López

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
#if (defined(USE_I2S_AUDIO) || defined(USE_TTGO_WATCH) || defined(USE_M5STACK_CORE2) || defined(ESP32S3_BOX))
#ifdef USE_I2S_RTTTL
#include "AudioFileSourcePROGMEM.h"
#include "AudioGeneratorRTTTL.h"

void Rtttl(char *buffer);

void Rtttl(char *buffer) {

  if (!audio_i2s.out) return;

  AudioGeneratorRTTTL *rtttl;
  AudioFileSourcePROGMEM *file = NULL;

  file = new AudioFileSourcePROGMEM(buffer, strlen(buffer));
  rtttl = new AudioGeneratorRTTTL();

  AUDIO_PWR_ON
  rtttl->begin(file, audio_i2s.out);

  while(rtttl->isRunning()) {
    if (!rtttl->loop()) {
      rtttl->stop();
    } else {
      delay(1); // Keep the dog happy
    }
  }

  audio_i2s.out->stop();
  AUDIO_PWR_OFF
  delete rtttl;
  delete file;
}
#endif // USE_I2S_RTTTL
#endif // is2audio
