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


void I2SMP3StreamInit(){
  audio_i2s_mp3.encoder_type = MP3_ENCODER;
  I2SstreamInit ();
}

void I2SOpusStreamInit(){
  audio_i2s_mp3.encoder_type = OPUS_ENCODER;
  I2SstreamInit ();
}

void I2SstreamInit (void) {
  if (audio_i2s_mp3.stream_active) {
    AddLog(LOG_LEVEL_INFO, PSTR("I2S: can not handle client - other stream task active"));
    return;
  }
  audio_i2s_mp3.stream_active = true;
  audio_i2s_mp3.client = audio_i2s_mp3.StreamServer->client();
  AddLog(LOG_LEVEL_INFO, PSTR("I2S: Create client"));

  audio_i2s_mp3.use_stream = true;
  if(I2sRecord((char*)"", audio_i2s_mp3.encoder_type) != pdTRUE){
    AddLog(LOG_LEVEL_INFO, PSTR("I2S: Stop client"));
    audio_i2s_mp3.stream_active = false;
    audio_i2s_mp3.client.stop();
  }
}

void I2sStreamLoop(void) {
  if (audio_i2s_mp3.StreamServer) {
    audio_i2s_mp3.StreamServer->handleClient();
  }
}

void I2sServerInit(uint32_t on) {
  if (on) {
    if (!audio_i2s_mp3.StreamServer) {
      audio_i2s_mp3.StreamServer = new ESP8266WebServer(MP3_STREAM_PORT);
      audio_i2s_mp3.StreamServer->on(PSTR("/stream.mp3"), I2SMP3StreamInit);
      audio_i2s_mp3.StreamServer->on(PSTR("/stream.m3a"), I2SMP3StreamInit);
      audio_i2s_mp3.StreamServer->on(PSTR("/stream.webm"), I2SOpusStreamInit);
      audio_i2s_mp3.StreamServer->begin();
      AddLog(LOG_LEVEL_INFO, PSTR("I2S: server created on port: %d "), MP3_STREAM_PORT);
    }
  } else {
    if (audio_i2s_mp3.StreamServer) {
      audio_i2s_mp3.StreamServer->stop();
      delete audio_i2s_mp3.StreamServer;
      audio_i2s_mp3.StreamServer = nullptr;
      audio_i2s_mp3.mic_stop = 1;
      audio_i2s_mp3.stream_active = false;
      AddLog(LOG_LEVEL_INFO, PSTR("I2S: server deleted"));
    }
  }
}


void CmndI2SMP3Stream(void) {
  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 1)) {
    audio_i2s_mp3.stream_enable = XdrvMailbox.payload;
  }
  I2sServerInit(audio_i2s_mp3.stream_enable);
  ResponseCmndNumber(audio_i2s_mp3.stream_enable);
}

#endif // defined(USE_I2S_AUDIO) && defined(USE_SHINE) && defined(MP3_MIC_STREAM)
#endif // defined(ESP32) && ESP_IDF_VERSION_MAJOR >= 5
