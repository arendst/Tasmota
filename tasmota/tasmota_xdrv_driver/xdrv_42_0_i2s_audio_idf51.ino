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
#if defined(USE_I2S_AUDIO) 

#define XDRV_42           42

#define USE_I2S_SAY
#define USE_I2S_SAY_TIME
#define USE_I2S_RTTTL
#define USE_I2S_WEBRADIO

// Macros used in audio sub-functions
#undef AUDIO_PWR_ON
#undef AUDIO_PWR_OFF
#define AUDIO_PWR_ON    I2SAudioPower(true);
#define AUDIO_PWR_OFF   I2SAudioPower(false);

extern FS *ufsp;
extern FS *ffsp;

const int preallocateBufferSize = 16*1024;
const int preallocateCodecSize = 29192; // MP3 codec max mem needed
//const int preallocateCodecSize = 85332; // AAC+SBR codec max mem needed

void sayTime(int hour, int minutes);
void Cmndwav2mp3(void);
void Cmd_Time(void);

void Rtttl(char *buffer);
void CmndI2SRtttl(void);
void I2sWebRadioStopPlaying(void);

/*********************************************************************************************\
 * Commands definitions
\*********************************************************************************************/

const char kI2SAudio_Commands[] PROGMEM = "I2S|"
  "Gain|Play|Rec|MGain|Stop"
#ifdef USE_I2S_WEBRADIO
  "|WR"
#endif // USE_I2S_WEBRADIO
#ifdef USE_I2S_SAY
  "|Say"
#endif // USE_I2S_SAY
#ifdef USE_I2S_SAY_TIME
  "|Time"
#endif // USE_I2S_SAY_TIME
#ifdef USE_I2S_RTTTL
  "|Rtttl"
#endif
#if defined(USE_SHINE) && defined(MP3_MIC_STREAM)
  "|Stream"
#endif // MP3_MIC_STREAM
#ifdef I2S_BRIDGE
  "|Bridge"
#endif // I2S_BRIDGE
;

void (* const I2SAudio_Command[])(void) PROGMEM = {
  &CmndI2SGain, &CmndI2SPlay, &CmndI2SMicRec, &CmndI2SMicGain, &CmndI2SStop,
#ifdef USE_I2S_WEBRADIO
  &CmndI2SWebRadio,
#endif // USE_I2S_WEBRADIO
#ifdef USE_I2S_SAY
  &CmndI2SSay,
#endif // USE_I2S_SAY
#ifdef USE_I2S_SAY_TIME
  &Cmd_Time,
#endif // USE_I2S_SAY_TIME
#ifdef USE_I2S_RTTTL
  &CmndI2SI2SRtttl,
#endif
#if defined(USE_SHINE) && defined(MP3_MIC_STREAM)
  &CmndI2SMP3Stream,
#endif // MP3_MIC_STREAM
#ifdef I2S_BRIDGE
  &CmndI2SI2SBridge,
#endif // I2S_BRIDGE
};

/*********************************************************************************************\
 * microphone related functions
\*********************************************************************************************/

uint32_t I2sMicInit(void) {
  esp_err_t err = ESP_OK;
  gpio_num_t clk_gpio;

  i2s_slot_mode_t slot_mode = (audio_i2s.Settings->rx.slot_mode == 0) ? I2S_SLOT_MODE_MONO : I2S_SLOT_MODE_STEREO;
  AddLog(LOG_LEVEL_DEBUG, "I2S: mic init rx_channels=%i", slot_mode);

  if (audio_i2s.Settings->sys.duplex == 1 && audio_i2s.rx_handle){
    return 0; // no need to en- or disable when in full duplex mode and already initialized
  }

  if (audio_i2s.rx_handle == nullptr){
    i2s_chan_config_t chan_cfg = I2S_CHANNEL_DEFAULT_CONFIG(I2S_NUM_AUTO, I2S_ROLE_MASTER);

    err = i2s_new_channel(&chan_cfg, NULL, &audio_i2s.rx_handle);
    AddLog(LOG_LEVEL_DEBUG, "I2S: mic init i2s_new_channel err=%i", err);
    switch (audio_i2s.Settings->rx.mode){
      case I2S_MODE_PDM:
        {
          clk_gpio = (gpio_num_t)Pin(GPIO_I2S_WS,1); //legacy setting for Core2, might be wrong
          if (clk_gpio == -1){
            clk_gpio =  (gpio_num_t)Pin(GPIO_I2S_WS); //fallback to other port, might be wrong
          }
          i2s_pdm_rx_config_t pdm_rx_cfg = {
            .clk_cfg = I2S_PDM_RX_CLK_DEFAULT_CONFIG(audio_i2s.Settings->rx.sample_rate),
            /* The default mono slot is the left slot (whose 'select pin' of the PDM microphone is pulled down) */
            .slot_cfg = I2S_PDM_RX_SLOT_DEFAULT_CONFIG(I2S_DATA_BIT_WIDTH_16BIT, slot_mode),
            .gpio_cfg = {
              .clk = clk_gpio,
              .din = (gpio_num_t)Pin(GPIO_I2S_DIN),
              .invert_flags = {
                .clk_inv = false,
              },
            },
          };
          pdm_rx_cfg.slot_cfg.slot_mask = (i2s_pdm_slot_mask_t)audio_i2s.Settings->rx.slot_mask;
          err = i2s_channel_init_pdm_rx_mode(audio_i2s.rx_handle, &pdm_rx_cfg);
          AddLog(LOG_LEVEL_DEBUG, PSTR("I2S: RX channel in PDM mode, CLK: %i, DIN: %i, 16 bit width, %i channel(s), err code: %u"),clk_gpio, Pin(GPIO_I2S_DIN), slot_mode, err);
        }
        break;
      case I2S_MODE_STD:
        {        
          i2s_std_config_t rx_std_cfg = {
            .clk_cfg  = I2S_STD_CLK_DEFAULT_CONFIG(audio_i2s.Settings->rx.sample_rate),
            .slot_cfg = I2S_STD_MSB_SLOT_DEFAULT_CONFIG(I2S_DATA_BIT_WIDTH_16BIT, slot_mode),
            .gpio_cfg = {
              .mclk = (gpio_num_t)Pin(GPIO_I2S_MCLK),
              .bclk = (gpio_num_t)Pin(GPIO_I2S_BCLK),
              .ws   = (gpio_num_t)Pin(GPIO_I2S_WS),
              .dout = I2S_GPIO_UNUSED,
              .din  = (gpio_num_t)Pin(GPIO_I2S_DIN),
              .invert_flags = {
                .mclk_inv = false,
                .bclk_inv = false,
                .ws_inv   = false,
              },
            },
          };
          rx_std_cfg.slot_cfg.slot_mask = (i2s_std_slot_mask_t)audio_i2s.Settings->rx.slot_mask;
          i2s_channel_init_std_mode(audio_i2s.rx_handle, &rx_std_cfg);
          AddLog(LOG_LEVEL_DEBUG, PSTR("I2S: RX channel in standard mode with 16 bit width on %i channel(s) initialized"),slot_mode);
        }
        break;
      default:
        AddLog(LOG_LEVEL_INFO, "I2S: invalid rx mode=%i", audio_i2s.Settings->rx.mode);
    }
  }

  err = i2s_channel_enable(audio_i2s.rx_handle);
  AddLog(LOG_LEVEL_DEBUG, PSTR("I2S: RX channel enable: %i"),err);
  return err;
}


void I2sMicDeinit(void) {
  esp_err_t err = ESP_OK;
  gpio_num_t clk_gpio;

  if (!audio_i2s.rx_handle) { return; }

  if (!audio_i2s.Settings->sys.duplex) {      // if duplex mode, there is no mic channel - TODO check this
    int err = i2s_channel_disable(audio_i2s.rx_handle);
    i2s_del_channel(audio_i2s.rx_handle);
    audio_i2s.rx_handle = nullptr;
    AddLog(LOG_LEVEL_DEBUG, "I2S: RX channel disable: %i", err);
  }
}

// micro to mp3 file or stream
void I2sMicTask(void *arg){
  int8_t error = 0;
  uint8_t *ucp;
  int written;
  shine_config_t  config;
  shine_t s = nullptr;
  uint16_t samples_per_pass;
  File mp3_out = (File)nullptr;
  int16_t *buffer = nullptr;
  uint16_t bytesize;
  uint16_t bwritten;
  uint32_t ctime;
  uint32_t gain = audio_i2s.Settings->rx.gain;

  if (!audio_i2s.use_stream) {
    mp3_out = ufsp->open(audio_i2s.mic_path, "w");
    if (!mp3_out) {
      error = 1;
      goto exit;
    }
  } else {
    if (!audio_i2s.stream_active) {
      error = 2;
      audio_i2s.use_stream = 0;
      goto exit;
    }
    audio_i2s.client.flush();
    audio_i2s.client.setTimeout(3);
    audio_i2s.client.print("HTTP/1.1 200 OK\r\n"
    "Content-Type: audio/mpeg;\r\n\r\n");

   //  Webserver->send(200, "application/octet-stream", "");
    //"Content-Type: audio/mp3;\r\n\r\n");
  }

  shine_set_config_mpeg_defaults(&config.mpeg);

  if (audio_i2s.Settings->rx.slot_mode == 0) {
    config.mpeg.mode = MONO;
  } else {
    config.mpeg.mode = STEREO;
  }
  config.mpeg.bitr = 128;
  config.wave.samplerate = audio_i2s.Settings->rx.sample_rate;
  config.wave.channels = (channels)(audio_i2s.Settings->rx.slot_mode + 1);

  if (shine_check_config(config.wave.samplerate, config.mpeg.bitr) < 0) {
    error = 3;
    goto exit;
  }

  s = shine_initialise(&config);
  if (!s) {
    error = 4;
    goto exit;
  }

  samples_per_pass = shine_samples_per_pass(s);
  bytesize = samples_per_pass * 2 * (audio_i2s.Settings->rx.slot_mode + 1);

  buffer = (int16_t*)malloc(bytesize);
  if (!buffer) {
    error = 5;
    goto exit;
  }

  ctime = TasmotaGlobal.uptime;


  while (!audio_i2s.mic_stop) {
      size_t bytes_read;
      i2s_channel_read(audio_i2s.rx_handle, (void*)buffer, bytesize, &bytes_read, (100 / portTICK_PERIOD_MS));

      if (gain > 1) {
        // set gain
        for (uint32_t cnt = 0; cnt < bytes_read / 2; cnt++) {
          buffer[cnt] *= gain;
        }
      }
      ucp = shine_encode_buffer_interleaved(s, buffer, &written);

      if (!audio_i2s.use_stream) {
        bwritten = mp3_out.write(ucp, written);
        if (bwritten != written) {
          break;
        }
      } else {
        audio_i2s.client.write((const char*)ucp, written);

        if (!audio_i2s.client.connected()) {
          break;
        }
      }
      audio_i2s.recdur = TasmotaGlobal.uptime - ctime;
  }

  ucp = shine_flush(s, &written);

  if (!audio_i2s.use_stream) {
    mp3_out.write(ucp, written);
  } else {
    audio_i2s.client.write((const char*)ucp, written);
  }


exit:
  if (s) {
    shine_close(s);
  }
  if (mp3_out) {
    mp3_out.close();
    AddLog(LOG_LEVEL_INFO, PSTR("I2S: MP3 file closed"));
  }
  if (buffer) {
    free(buffer);
  }

  if (audio_i2s.use_stream) {
    audio_i2s.client.stop();
  }

  I2sMicDeinit();
  audio_i2s.mic_stop = 0;
  audio_i2s.mic_error = error;
  AddLog(LOG_LEVEL_INFO, PSTR("mp3task result code: %d"), error);
  audio_i2s.mic_task_handle = 0;
  audio_i2s.recdur = 0;
  audio_i2s.stream_active = 0;
  vTaskDelete(NULL);

}

int32_t I2sRecordShine(char *path) {
  esp_err_t err = ESP_OK;

  if (audio_i2s.decoder || audio_i2s.mp3) return 0;

  strlcpy(audio_i2s.mic_path, path, sizeof(audio_i2s.mic_path));
  audio_i2s.mic_stop = 0;
  uint32_t stack = 4096;
  audio_i2s.use_stream = !strcmp(audio_i2s.mic_path, "stream.mp3");

  if (audio_i2s.use_stream) {
    stack = 8000;
  }
  I2sMicInit();

  err = xTaskCreatePinnedToCore(I2sMicTask, "MIC", stack, NULL, 3, &audio_i2s.mic_task_handle, 1);

  return err;
}

/*********************************************************************************************\
 * Driver Settings load and save using filesystem
\*********************************************************************************************/
// error codes
enum {
  I2S_OK = 0,
  I2S_ERR_OUTPUT_NOT_CONFIGURE,
  I2S_ERR_DECODER_IN_USE,
  I2S_ERR_FILE_NOT_FOUND,
};

// signal to an external Berry driver that we turn audio power on or off
void I2SAudioPower(bool power) {
  callBerryEventDispatcher(PSTR("audio"), PSTR("power"), power, nullptr, 0);
}

//
// I2SSettingsLoad(erase:bool)
//
// Load settings from file system.
// File is `/.drvset042`
void I2SSettingsLoad(const char * config_filename, bool erase) {
  // allocate memory for settings
  audio_i2s.Settings = new tI2SSettings();
  if (!audio_i2s.Settings) {
    AddLog(LOG_LEVEL_ERROR, "I2S: ERROR memory allocation failed");
    return;
  }

  if (!config_filename) { return; }     // if no filename, use defaults

#ifndef USE_UFILESYS
  AddLog(LOG_LEVEL_INFO, "I2S: use defaults as file system not enabled");
#else
  if (erase) {
    TfsDeleteFile(config_filename);  // Use defaults
  }
  else if (TfsLoadFile(config_filename, (uint8_t*)audio_i2s.Settings, sizeof(tI2SSettings))) {
    AddLog(LOG_LEVEL_INFO, "I2S: config loaded from file '%s'", config_filename);
  }
  else {
    // File system not ready: No flash space reserved for file system
    AddLog(LOG_LEVEL_DEBUG, "I2S: use defaults as file system not ready or file not found");
    I2SSettingsSave(config_filename);
  }
#endif  // USE_UFILESYS
}

void I2SSettingsSave(const char * config_filename) {
#ifdef USE_UFILESYS
  if (!config_filename) { return; }     // if no filename, use defaults

  if (TfsSaveFile(config_filename, (const uint8_t*)audio_i2s.Settings, sizeof(tI2SSettings))) {
    AddLog(LOG_LEVEL_DEBUG, "I2S: config saved to file '%s'", config_filename);
  } else {
    // File system not ready: No flash space reserved for file system
    AddLog(LOG_LEVEL_DEBUG, "I2S: ERROR file system not ready or unable to save file");
  }
#endif  // USE_UFILESYS
}

/*********************************************************************************************\
 * Driver init
\*********************************************************************************************/

//
// I2sCheckCfg
//
// Multiple checks
void I2sCheckCfg(void){
  // din and dout must be configured on port 0 for full duplex
  bool useDuplexMode = ((Pin(GPIO_I2S_DIN) != -1) && (Pin(GPIO_I2S_DOUT) != -1));
  // AddLog(LOG_LEVEL_DEBUG, PSTR("I2S: DIN %i , DOUT %i"),Pin(GPIO_I2S_DIN),Pin(GPIO_I2S_DOUT) );

  if (useDuplexMode){
    if (audio_i2s.Settings->rx.mode == I2S_MODE_PDM || audio_i2s.Settings->tx.mode == I2S_MODE_PDM ){
      useDuplexMode = false;
      AddLog(LOG_LEVEL_DEBUG, PSTR("I2S: PDM forbids full duplex mode"));
    }
    audio_i2s.Settings->sys.duplex = useDuplexMode ? 1 : 0;
    if(useDuplexMode){
      AddLog(LOG_LEVEL_DEBUG, PSTR("I2S: will try to use full duplex mode"));
    }
  }
  if (Pin(GPIO_I2S_DIN) != -1 || Pin(GPIO_I2S_DIN, 1) != -1){ // micro could be port 0 or 1
    audio_i2s.Settings->sys.rx = 1;
    AddLog(LOG_LEVEL_DEBUG, PSTR("I2S: RX config = mode: %i, channels: %i, gain: %i, sample rate: %i"), audio_i2s.Settings->rx.mode, (uint8_t)(audio_i2s.Settings->rx.slot_mode + 1), audio_i2s.Settings->rx.gain, audio_i2s.Settings->rx.sample_rate);
  }
  else{
    audio_i2s.Settings->sys.rx = 0;
    audio_i2s.Settings->rx.mp3_encoder = 0; // do not allocate buffer
  }
  if (Pin(GPIO_I2S_DOUT) != -1){ // output is only supported on port 0
    audio_i2s.Settings->sys.tx = 1;
  }
  else{
    audio_i2s.Settings->sys.tx = 0;
  }

  AddLog(LOG_LEVEL_INFO, PSTR("I2S: init pins bclk=%d, ws=%d, dout=%d, mclk=%d, din=%d [tx=%i, rx=%i, duplex=%i]"),
                          Pin(GPIO_I2S_BCLK) , Pin(GPIO_I2S_WS), Pin(GPIO_I2S_DOUT), Pin(GPIO_I2S_MCLK), Pin(GPIO_I2S_DIN),
                          audio_i2s.Settings->sys.tx, audio_i2s.Settings->sys.tx, audio_i2s.Settings->sys.duplex);

}

//
// I2sInit
//
// Initialize I2S driver for input and output
void I2sInit(void) {
  // we need at least one pin configured
  if(Pin(GPIO_I2S_DIN) + Pin(GPIO_I2S_DIN,1) + Pin(GPIO_I2S_DOUT) + Pin(GPIO_I2S_DOUT,1) == -4){
    AddLog(LOG_LEVEL_DEBUG,PSTR("I2S: no pin configured"));
    return;
  }

  I2SSettingsLoad("/.drvset042", false);    // load configuration (no-erase)
  if (!audio_i2s.Settings) { return; }     // fatal error, could not allocate memory for configuration

  // check configuration is valid
  I2sCheckCfg();

  // TODO 
  int result = 0;
  if (audio_i2s.Settings->sys.rx == 1 && audio_i2s.Settings->sys.duplex == 0){
    I2sMicDeinit();
  }

  if(audio_i2s.Settings->sys.tx == 1){
    audio_i2s.out = new TasmotaAudioOutputI2S;
    int err = audio_i2s.out->startI2SChannel() ? 0 : 1;
    result += err;
  }

  if(audio_i2s.out != nullptr){
    audio_i2s.out->SetGain(((float)audio_i2s.Settings->tx.volume / 100.0) * 4.0);
    audio_i2s.out->begin();
    audio_i2s.out->stop();
  }
  audio_i2s.mp3ram = nullptr;

  if(audio_i2s.Settings->rx.mp3_encoder == 1){
    AddLog(LOG_LEVEL_DEBUG,PSTR("I2S: will allocate buffer for mp3 encoder"));
    if (UsePSRAM()) {
      audio_i2s.mp3ram = heap_caps_malloc(preallocateCodecSize, MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);
    }
    else{
      audio_i2s.Settings->rx.mp3_encoder = 0; // no PS-RAM -> no MP3 encoding
    }
  }
}

/*********************************************************************************************\
 * Driver features and commands
\*********************************************************************************************/

void I2sMp3Task(void *arg) {
  while (1) {
    while (audio_i2s.mp3->isRunning()) {
      if (!audio_i2s.mp3->loop()) {
        audio_i2s.mp3->stop();
        mp3_delete();
        audio_i2s.out->stop();
        if (audio_i2s.mp3_task_handle) {
          vTaskDelete(audio_i2s.mp3_task_handle);
          audio_i2s.mp3_task_handle = 0;
        }
        //mp3_task_handle=nullptr;
      }
     vTaskDelay(pdMS_TO_TICKS(1));
    }
  }
}

void I2sStatusCallback(void *cbData, int code, const char *string) {
  const char *ptr = reinterpret_cast<const char *>(cbData);
  (void) code;
  (void) ptr;
  //strncpy_P(status, string, sizeof(status)-1);
  //status[sizeof(status)-1] = 0;
}

void I2sMp3Task2(void *arg){
  while (1) {
    if (audio_i2s.decoder && audio_i2s.decoder->isRunning()) {
      if (!audio_i2s.decoder->loop()) {
        I2sStopPlaying();
        //retryms = millis() + 2000;
      }
      vTaskDelay(pdMS_TO_TICKS(1));
    }
  }
}

void I2sStopPlaying() {

  if (audio_i2s.mp3_task_handle) {
    vTaskDelete(audio_i2s.mp3_task_handle);
    audio_i2s.mp3_task_handle = nullptr;
  }

  if (audio_i2s.decoder) {
    audio_i2s.decoder->stop();
    delete audio_i2s.decoder;
    audio_i2s.decoder = NULL;
  }
#ifdef USE_I2S_WEBRADIO
  I2sWebRadioStopPlaying();
#endif

  I2SAudioPower(false);
}

// Play_mp3 - Play a MP3 file from filesystem
//
// Returns I2S_error_t
int32_t I2SPlayMp3(const char *path) {
  if (!audio_i2s.out) return I2S_ERR_OUTPUT_NOT_CONFIGURE;
  if (audio_i2s.decoder || audio_i2s.mp3) return I2S_ERR_DECODER_IN_USE;

  // check if the filename starts with '/', if not add it
  char fname[64];
  if (path[0] != '/') {
    snprintf(fname, sizeof(fname), "/%s", path);
  } else {
    snprintf(fname, sizeof(fname), "%s", path);
  }
  if (!ufsp->exists(fname)) { return I2S_ERR_FILE_NOT_FOUND; }

  I2SAudioPower(true);

  audio_i2s.file = new AudioFileSourceFS(*ufsp, fname);

  audio_i2s.id3 = new AudioFileSourceID3(audio_i2s.file);

  if (audio_i2s.mp3ram) {
    audio_i2s.mp3 = new AudioGeneratorMP3(audio_i2s.mp3ram, preallocateCodecSize);
  } else {
    audio_i2s.mp3 = new AudioGeneratorMP3();
  }
  audio_i2s.mp3->begin(audio_i2s.id3, audio_i2s.out);

  // Always use a task
  xTaskCreatePinnedToCore(I2sMp3Task, "MP3", 8192, NULL, 3, &audio_i2s.mp3_task_handle, 1);
  return I2S_OK;
}

void mp3_delete(void) {
  delete audio_i2s.file;
  delete audio_i2s.id3;
  delete audio_i2s.mp3;
  audio_i2s.mp3=nullptr;
  I2SAudioPower(false);
}

void Say(char *text) {

  if (!audio_i2s.out) return;

  I2SAudioPower(true);

  ESP8266SAM *sam = new ESP8266SAM;

  sam->Say(audio_i2s.out, text);
  delete sam;
  audio_i2s.out->stop();

  I2SAudioPower(false);
}

/*********************************************************************************************\
 * Commands
\*********************************************************************************************/

void CmndI2SStop(void) {
  I2sStopPlaying();
  ResponseCmndDone();
}

void CmndI2SPlay(void) {
  if (XdrvMailbox.data_len > 0) {
    int32_t err = I2SPlayMp3(XdrvMailbox.data);
    // display return message
    switch (err) {
      case I2S_OK:
        ResponseCmndDone();
        break;
      case I2S_ERR_OUTPUT_NOT_CONFIGURE:
        ResponseCmndChar("I2S output not configured");
        break;
      case I2S_ERR_DECODER_IN_USE:
        ResponseCmndChar("Decoder already in use");
        break;
      case I2S_ERR_FILE_NOT_FOUND:
        ResponseCmndChar("File not found");
        break;
      default:
        ResponseCmndChar("Unknown error");
        break;
    }
  } else {
    ResponseCmndChar("Missing filename");
  }
}

void CmndI2SGain(void) {
  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 100)) {
    if (audio_i2s.out) {
      audio_i2s.Settings->tx.volume = XdrvMailbox.payload;
      audio_i2s.out->SetGain(((float)(audio_i2s.Settings->tx.volume-2)/100.0)*4.0);
    }
  }
  ResponseCmndNumber(audio_i2s.Settings->tx.volume);
}

void CmndI2SSay(void) {
  if (XdrvMailbox.data_len > 0) {
    Say(XdrvMailbox.data);
  }
  ResponseCmndChar(XdrvMailbox.data);
}

void CmndI2SI2SRtttl(void) {
  if (XdrvMailbox.data_len > 0) {
    Rtttl(XdrvMailbox.data);
  }
  ResponseCmndChar(XdrvMailbox.data);
}

void CmndI2SMicRec(void) {
if (audio_i2s.Settings->rx.mp3_encoder == 1) {
  if (XdrvMailbox.data_len > 0) {
    if (!strncmp(XdrvMailbox.data, "-?", 2)) {
      Response_P("{\"I2SREC-duration\":%d}", audio_i2s.recdur);
    } else {
      I2sRecordShine(XdrvMailbox.data);
      ResponseCmndChar(XdrvMailbox.data);
    }
  } else {
    if (audio_i2s.mic_task_handle) {
      // stop task
      audio_i2s.mic_stop = 1;
      while (audio_i2s.mic_stop) {
        delay(1);
      }
      ResponseCmndChar_P(PSTR("Stopped"));
    }
  }
}
else{
  if(audio_i2s.Settings->sys.rx == 1){
    ResponseCmndChar_P(PSTR("need PSRAM for MP3 recording"));
  }
  else{
    ResponseCmndChar_P(PSTR("no mic configured"));
  }
}
}

// mic gain in factor not percent
void CmndI2SMicGain(void) {
  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 256)) {
      audio_i2s.Settings->rx.gain = XdrvMailbox.payload;
  }
  ResponseCmndNumber(audio_i2s.Settings->rx.gain);
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

void I2sMp3Loop(void);
void I2sMp3Init(void);
void MP3ShowStream(void);

bool Xdrv42(uint32_t function) {
  bool result = false;

  switch (function) {
    case FUNC_INIT:
      I2sInit();
      break;
    case FUNC_COMMAND:
      result = DecodeCommand(kI2SAudio_Commands, I2SAudio_Command);
      break;
    case FUNC_WEB_ADD_MAIN_BUTTON:
      //MP3ShowStream();
      break;
    case FUNC_LOOP:
#if defined(USE_SHINE) && defined(MP3_MIC_STREAM)
      I2sMp3Loop();
#endif
#if defined(I2S_BRIDGE)
      i2s_bridge_loop();
#endif
      break;
    case FUNC_WEB_ADD_HANDLER:
#if defined(USE_SHINE) && defined(MP3_MIC_STREAM)
      audio_i2s.stream_enable = 1;
      I2sMp3Init(1);
#endif
#if defined(I2S_BRIDGE)
      I2SBridgeInit();
#endif
      break;

#ifdef USE_WEBSERVER
    case FUNC_WEB_SENSOR:
      I2sWrShow(false);
      break;
#endif  // USE_WEBSERVER
    case FUNC_JSON_APPEND:
      I2sWrShow(true);
    break;
  }
  return result;
}

#endif // USE_I2S_AUDIO
#endif // defined(ESP32) && ESP_IDF_VERSION_MAJOR >= 5
