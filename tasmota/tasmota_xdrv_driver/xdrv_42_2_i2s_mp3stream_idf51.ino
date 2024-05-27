/*
  xdrv_42_2_i2s_mp3stream_idf51.ino - Audio dac support for Tasmota

  Copyright (C) 2024  Gerhard Mutz and Theo Arends

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

#if defined(ESP32) && ESP_IDF_VERSION_MAJOR >= 5
#if defined(USE_I2S_AUDIO) && defined(USE_SHINE) && defined(MP3_MIC_STREAM)

#ifndef MP3_STREAM_PORT
#define MP3_STREAM_PORT 81
#endif


void Stream_mp3(void) {
  if (audio_i2s_mp3.stream_active) {
    AddLog(LOG_LEVEL_INFO, PSTR("I2S: can not handle client - other MP3 task active"));
    return;
  }
  AddLog(LOG_LEVEL_INFO, PSTR("I2S: Handle mp3server"));
  audio_i2s_mp3.stream_active = 1;
  audio_i2s_mp3.client = audio_i2s_mp3.MP3Server->client();
  AddLog(LOG_LEVEL_INFO, PSTR("I2S: Create client"));
  // i2s_record_shine((char*)"stream.mp3");
  I2sRecordShine((char*)"stream.mp3");
}

void I2sMp3Loop(void) {
  if (audio_i2s_mp3.MP3Server) {
    audio_i2s_mp3.MP3Server->handleClient();
  }
}

void I2sMp3Init(uint32_t on) {
  if (on) {
    if (!audio_i2s_mp3.MP3Server) {
      audio_i2s_mp3.MP3Server = new ESP8266WebServer(MP3_STREAM_PORT);
      audio_i2s_mp3.MP3Server->on(PSTR("/stream.mp3"), Stream_mp3);
      audio_i2s_mp3.MP3Server->on(PSTR("/stream.m3a"), Stream_mp3);
      audio_i2s_mp3.MP3Server->begin();
      AddLog(LOG_LEVEL_INFO, PSTR("MP3: server created on port: %d "), MP3_STREAM_PORT);
    }
  } else {
    if (audio_i2s_mp3.MP3Server) {
      audio_i2s_mp3.MP3Server->stop();
      delete audio_i2s_mp3.MP3Server;
      audio_i2s_mp3.MP3Server = nullptr;
      audio_i2s_mp3.mic_stop = 1;
      audio_i2s_mp3.stream_active = 0;
      AddLog(LOG_LEVEL_INFO, PSTR("MP3: server deleted"));
    }
  }
}


void CmndI2SMP3Stream(void) {
  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 1)) {
    audio_i2s_mp3.stream_enable = XdrvMailbox.payload;
  }
  I2sMp3Init(audio_i2s_mp3.stream_enable);
  ResponseCmndNumber(audio_i2s_mp3.stream_enable);
}

#endif // defined(USE_I2S_AUDIO) && defined(USE_SHINE) && defined(MP3_MIC_STREAM)
#endif // defined(ESP32) && ESP_IDF_VERSION_MAJOR >= 5
