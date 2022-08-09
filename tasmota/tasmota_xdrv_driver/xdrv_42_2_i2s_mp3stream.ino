
#ifdef ESP32
#if defined(USE_SHINE) && ( (defined(USE_I2S_AUDIO) && defined(USE_I2S_MIC)) || defined(USE_M5STACK_CORE2) || defined(ESP32S3_BOX) )

#ifdef MP3_MIC_STREAM

#ifndef MP3_STREAM_PORT
#define MP3_STREAM_PORT 81
#endif


void Stream_mp3(void) {
  if (!audio_i2s.stream_enable) {
    return;
  }

  if (audio_i2s.stream_active) {
    return;
  }
  AddLog(LOG_LEVEL_INFO, PSTR("I2S: Handle mp3server"));
  audio_i2s.stream_active = 1;
  audio_i2s.client = audio_i2s.MP3Server->client();
  AddLog(LOG_LEVEL_INFO, PSTR("I2S: Create client"));
  i2s_record_shine((char*)"stream.mp3");
}

void i2s_mp3_loop(void) {
  if (audio_i2s.MP3Server) {
    audio_i2s.MP3Server->handleClient();
  }
}

void i2s_mp3_init(uint32_t on) {
  if (on) {
    if (!audio_i2s.MP3Server) {
      audio_i2s.MP3Server = new ESP8266WebServer(MP3_STREAM_PORT);
      audio_i2s.MP3Server->on(PSTR("/stream.mp3"), Stream_mp3);
      audio_i2s.MP3Server->on(PSTR("/stream.m3a"), Stream_mp3);
      audio_i2s.MP3Server->begin();
      AddLog(LOG_LEVEL_INFO, PSTR("MP3: server created on port: %d "), MP3_STREAM_PORT);
    }
  } else {
    if (audio_i2s.MP3Server) {
      audio_i2s.MP3Server->stop();
      delete audio_i2s.MP3Server;
      audio_i2s.MP3Server = nullptr;
      AddLog(LOG_LEVEL_INFO, PSTR("MP3: server deleted"));
    }
  }
}


void Cmd_MP3Stream(void) {
  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 1)) {
    audio_i2s.stream_enable = XdrvMailbox.payload;
  }
  i2s_mp3_init(audio_i2s.stream_enable);
  ResponseCmndNumber(audio_i2s.stream_enable);
}
#endif // MP3_MIC_STREAM


#endif // USE_SHINE
#endif // ESP32
