/*
  xdrv_42_i2s_audio.ino - audio dac support for Tasmota

  Copyright (C) 2020  Gerhard Mutz and Theo Arends

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

#if  defined(ESP32) && (defined(USE_I2S_AUDIO) || defined(USE_TTGO_WATCH))

#include "AudioFileSourcePROGMEM.h"
#include "AudioFileSourceID3.h"
#include "AudioGeneratorMP3.h"
#include "AudioOutputI2S.h"
#include <ESP8266SAM.h>
#include "AudioFileSourceFS.h"

// unfortunately tasks do not help very much,
// mp3 is extremely sensitive to interruptions
// also mp3 needs 240 Mhz CPU clock
#define MP3_TASK


#define EXTERNAL_DAC_PLAY   1

#define XDRV_42           42

AudioGeneratorMP3 *mp3 = nullptr;
AudioFileSourceFS *file;
AudioOutputI2S *out;
AudioFileSourceID3 *id3;


//! MAX98357A + INMP441 DOUBLE I2S BOARD
#ifdef ESP32
#undef TWATCH_DAC_IIS_BCK
#undef TWATCH_DAC_IIS_WS
#undef TWATCH_DAC_IIS_DOUT
#define TWATCH_DAC_IIS_BCK       26
#define TWATCH_DAC_IIS_WS        25
#define TWATCH_DAC_IIS_DOUT      33
#else
#undef TWATCH_DAC_IIS_BCK
#undef TWATCH_DAC_IIS_WS
#undef TWATCH_DAC_IIS_DOUT
#define TWATCH_DAC_IIS_BCK       15
#define TWATCH_DAC_IIS_WS        2
#define TWATCH_DAC_IIS_DOUT      3
#endif


uint8_t volume;

void I2S_Init(void) {

#if EXTERNAL_DAC_PLAY
    out = new AudioOutputI2S();
    out->SetPinout(TWATCH_DAC_IIS_BCK, TWATCH_DAC_IIS_WS, TWATCH_DAC_IIS_DOUT);
#else
    out = new AudioOutputI2S(0, 1);
#endif

  volume=50;
  out->SetGain(((float)volume/100.0)*4.0);
  out->stop();
}


#ifdef MP3_TASK
uint16_t mp3_task_timer;
TaskHandle_t mp3_task_h;

void mp3_task(void *arg) {
  while (1) {
    while (mp3->isRunning()) {
      if (!mp3->loop()) {
        mp3->stop();
        mp3_delete();
        vTaskDelete(mp3_task_h);
        //mp3_task_h=nullptr;
      }
    }
  }
}
#endif // MP3_TASK


void Play_mp3(const char *path) {
#if defined(USE_SCRIPT) && defined(USE_SCRIPT_FATFS)
  if (mp3) return;

  #ifdef USE_TTGO_WATCH
    TTGO_audio_power(true);
  #endif

  file = new AudioFileSourceFS(*fsp,path);
  id3 = new AudioFileSourceID3(file);
  mp3 = new AudioGeneratorMP3();
  mp3->begin(id3, out);

#ifdef MP3_TASK
  // esp32 use mp3 task
  xTaskCreatePinnedToCore(mp3_task, "MP3", 8192, NULL, 3, &mp3_task_h, 1);
#else
  // esp8266 must wait in loop
  while (mp3->isRunning()) {
    if (!mp3->loop()) {
      mp3->stop();
      mp3_delete();
      break;
    }
    OsWatchLoop();
  }
#endif // MP3_TASK

#endif // USE_SCRIPT
}

void mp3_delete(void) {
  delete file;
  delete id3;
  delete mp3;
  mp3=nullptr;
#ifdef USE_TTGO_WATCH
    TTGO_audio_power(false);
#endif
}

void Say(char *text) {

#ifdef USE_TTGO_WATCH
  TTGO_audio_power(true);
#endif

  out->begin();
  ESP8266SAM *sam = new ESP8266SAM;
  sam->Say(out, text);
  delete sam;
  out->stop();

#ifdef USE_TTGO_WATCH
  TTGO_audio_power(false);
#endif
}

const char kI2SAudio_Commands[] PROGMEM = "I2S|"
  "Say|Play|Gain";

void (* const I2SAudio_Command[])(void) PROGMEM = {
  &Cmd_Say,&Cmd_Play,&Cmd_Gain};

void Cmd_Play(void) {
  if (XdrvMailbox.data_len > 0) {
    Play_mp3(XdrvMailbox.data);
  }
  ResponseCmndChar(XdrvMailbox.data);
}

void Cmd_Gain(void) {
  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 100)) {
    if (out) {
      volume=XdrvMailbox.payload;
      out->SetGain(((float)(volume-2)/100.0)*4.0);
    }
  }
  ResponseCmndNumber(volume);
}

void Cmd_Say(void) {
  if (XdrvMailbox.data_len > 0) {
    Say(XdrvMailbox.data);
  }
  ResponseCmndChar(XdrvMailbox.data);
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xdrv42(uint8_t function) {
  bool result = false;

  switch (function) {
    case FUNC_COMMAND:
      result = DecodeCommand(kI2SAudio_Commands, I2SAudio_Command);
      break;
    case FUNC_INIT:
      I2S_Init();
      break;

  }
  return result;
}

#endif  // USE_I2S_AUDIO
