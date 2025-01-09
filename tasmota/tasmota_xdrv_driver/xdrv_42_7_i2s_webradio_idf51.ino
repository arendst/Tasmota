/*
  xdrv_42_i2s_audio.ino - Audio dac support for Tasmota

  Copyright (C) 2021  Gerhard Mutz and Theo Arends

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
#if defined(USE_I2S_AUDIO) && defined(USE_I2S_WEBRADIO)

struct AUDIO_I2S_WEBRADIO_t {
  // Webradio
  AudioFileSourceICYStream *ifile = NULL;
  AudioFileSourceBuffer *buff = NULL;
  char wr_title[64];
  void *preallocateBuffer = NULL;
} Audio_webradio;

void I2sMDCallback(void *cbData, const char *type, bool isUnicode, const char *str) {
  const char *ptr = reinterpret_cast<const char *>(cbData);
  (void) isUnicode; // Punt this ball for now
  (void) ptr;
  if (strstr_P(type, PSTR("Title"))) {
    strncpy(Audio_webradio.wr_title, str, sizeof(Audio_webradio.wr_title));
    Audio_webradio.wr_title[sizeof(Audio_webradio.wr_title)-1] = 0;
    //AddLog(LOG_LEVEL_INFO,PSTR("WR-Title: %s"),wr_title);
  } else {
    // Who knows what to do?  Not me!
  }
}

void I2SWrStatusCB(void *cbData, int code, const char *str){
  AddLog(LOG_LEVEL_INFO, "I2S: status: %s",str);
}

bool I2SWebradio(const char *url, uint32_t decoder_type) {

  size_t wr_tasksize = 8000; // suitable for ACC and MP3
  if(decoder_type == 2){ // opus needs a ton of stack
    wr_tasksize = 26000;
  }

  // allocate buffers if not already done
  if (Audio_webradio.preallocateBuffer == NULL) {
    Audio_webradio.preallocateBuffer = special_malloc(preallocateBufferSize);
  }
  if (audio_i2s_mp3.preallocateCodec == NULL) {
    audio_i2s_mp3.preallocateCodec = special_malloc(preallocateCodecSize);
  }
  // check if we have buffers
  if (Audio_webradio.preallocateBuffer == NULL || audio_i2s_mp3.preallocateCodec == NULL) {
    AddLog(LOG_LEVEL_INFO, "I2S: cannot allocate buffers");
    if (Audio_webradio.preallocateBuffer != NULL) {
      free(Audio_webradio.preallocateBuffer);
      Audio_webradio.preallocateBuffer = NULL;
    }
    if (audio_i2s_mp3.preallocateCodec != NULL) {
      free(audio_i2s_mp3.preallocateCodec);
      audio_i2s_mp3.preallocateCodec = NULL;
    }
    return false;
  }

  Audio_webradio.ifile = new AudioFileSourceICYStream();
  Audio_webradio.ifile->RegisterMetadataCB(I2sMDCallback, NULL);
  Audio_webradio.ifile->RegisterStatusCB(I2SWrStatusCB, NULL);
  if(!Audio_webradio.ifile->open(url)){
    goto i2swr_fail;
  }
  AddLog(LOG_LEVEL_INFO, "I2S: did connect to %s",url);

  I2SAudioPower(true);
  Audio_webradio.buff = new AudioFileSourceBuffer(Audio_webradio.ifile, Audio_webradio.preallocateBuffer, preallocateBufferSize);
  if(Audio_webradio.buff == nullptr){
    goto i2swr_fail;
  }
  Audio_webradio.buff->RegisterStatusCB(I2sStatusCallback, NULL);

  if(I2SinitDecoder(decoder_type) == false){
    AddLog(LOG_LEVEL_DEBUG, "I2S: decoder init failed");
    goto i2swr_fail;
  }

  audio_i2s_mp3.decoder->RegisterStatusCB(I2sStatusCallback, NULL);
  if(audio_i2s_mp3.decoder->begin(Audio_webradio.buff, audio_i2s.out)){
    AddLog(LOG_LEVEL_DEBUG, "I2S: decoder started");
  } else {
    goto i2swr_fail;
  }

  AddLog(LOG_LEVEL_DEBUG,PSTR("I2S: will launch webradio task with decoder type %u"), decoder_type);
  xTaskCreatePinnedToCore(I2sMp3WrTask, "MP3-WR", wr_tasksize, NULL, 3, &audio_i2s_mp3.mp3_task_handle, 1);
  return true;

i2swr_fail:
    I2sStopPlaying();
    I2sWebRadioStopPlaying();
    return false;
}

#ifdef USE_WEBSERVER
const char HTTP_WEBRADIO[] PROGMEM =
   "{s}" "Webradio:" "{m}%s{e}";

void I2sWrShow(bool json) {
    if (audio_i2s_mp3.decoder) {
      if (json) {
        ResponseAppend_P(PSTR(",\"WebRadio\":{\"Title\":\"%s\"}"), Audio_webradio.wr_title);
      } else {
        WSContentSend_PD(HTTP_WEBRADIO,Audio_webradio.wr_title);
      }
    }
}
#endif  // USE_WEBSERVER

void CmndI2SWebRadio(void) {
  if (I2SPrepareTx() != I2S_OK) return;

  if (XdrvMailbox.data_len > 0) {
    if(I2SWebradio(XdrvMailbox.data, XdrvMailbox.index)){
      ResponseCmndChar(XdrvMailbox.data);
    } else {
      ResponseCmndFailed();
    }
  } else {
    ResponseCmndChar_P(PSTR("Stopped"));
  }
}


void I2sWebRadioStopPlaying() {
  if(audio_i2s_mp3.decoder) {
    audio_i2s_mp3.decoder->stop();
    delete audio_i2s_mp3.decoder;
    audio_i2s_mp3.decoder = nullptr;
  }
  if (Audio_webradio.buff) {
    Audio_webradio.buff->close();
    delete Audio_webradio.buff;
    Audio_webradio.buff = NULL;
  }
  if (Audio_webradio.ifile) {
    Audio_webradio.ifile->close();
    delete Audio_webradio.ifile;
    Audio_webradio.ifile = NULL;
  }
}


#endif // defined(USE_I2S_AUDIO) && defined(USE_I2S_WEBRADIO)
#endif // defined(ESP32) && ESP_IDF_VERSION_MAJOR >= 5
