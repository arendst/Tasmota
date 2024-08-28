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

#ifdef ESP32
#if defined(USE_I2S_AUDIO) && ESP_IDF_VERSION_MAJOR >= 5

#include <ESP8266SAM.h>

#include "AudioFileSourcePROGMEM.h"
#include "AudioFileSourceID3.h"
#include "AudioGeneratorMP3.h"

#include "AudioFileSourceFS.h"
#include "AudioGeneratorTalkie.h"
#include "AudioFileSourceICYStream.h"
#include "AudioFileSourceBuffer.h"
#include "AudioGeneratorAAC.h"

#include <layer3.h>

#define XDRV_42           42

#define USE_I2S_SAY
#define USE_I2S_SAY_TIME
#define USE_I2S_RTTTL
#define USE_I2S_WEBRADIO
#define USE_I2S_MP3
#define USE_I2S_DEBUG       // remove before flight

// Macros used in audio sub-functions
#undef AUDIO_PWR_ON
#undef AUDIO_PWR_OFF
#define AUDIO_PWR_ON    I2SAudioPower(true);
#define AUDIO_PWR_OFF   I2SAudioPower(false);

#define AUDIO_CONFIG_FILENAME "/.drvset042"

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
void CmndI2SMP3Stream(void);

/*********************************************************************************************\
 * More structures
\*********************************************************************************************/

struct AUDIO_I2S_MP3_t {
#ifdef USE_I2S_MP3
  AudioGeneratorMP3 *mp3 = nullptr;
  AudioFileSourceFS *file = nullptr;
  AudioFileSourceID3 *id3 = nullptr;

  void *mp3ram = NULL;
#endif // USE_I2S_MP3

#if defined(USE_I2S_MP3) || defined(USE_I2S_WEBRADIO) || defined(USE_SHINE) || defined(MP3_MIC_STREAM)
  AudioGeneratorMP3 *decoder = NULL;
  TaskHandle_t mp3_task_handle;
  TaskHandle_t mic_task_handle;
#endif // defined(USE_I2S_MP3) || defined(USE_I2S_WEBRADIO)

  char mic_path[32];
  uint8_t mic_stop;
  int8_t mic_error;
  bool use_stream = false;
  bool task_running = false;
  bool task_has_ended = false;

// SHINE
  uint32_t recdur;
  uint8_t  stream_active;
  uint8_t  stream_enable;
  WiFiClient client;
  ESP8266WebServer *MP3Server;

// I2S_BRIDGE
  BRIDGE_MODE bridge_mode;
  WiFiUDP i2s_bridge_udp;
  WiFiUDP i2s_bridgec_udp;
  IPAddress i2s_bridge_ip;
  TaskHandle_t i2s_bridge_h;
  int8_t ptt_pin = -1;

} audio_i2s_mp3;

#define I2S_AUDIO_MODE_MIC 1
#define I2S_AUDIO_MODE_SPK 2

/*********************************************************************************************\
 * Commands definitions
\*********************************************************************************************/

const char kI2SAudio_Commands[] PROGMEM = "I2S|"
  "Gain|Rec|Stop|Config"
#ifdef USE_I2S_MP3
  "|Play"
#endif
#ifdef USE_I2S_DEBUG
  "|Mic"      // debug only
#endif // USE_I2S_DEBUG
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
#ifdef USE_I2S_BRIDGE
  "|Bridge"
#endif // USE_I2S_BRIDGE
;

void (* const I2SAudio_Command[])(void) PROGMEM = {
  &CmndI2SGain, &CmndI2SMicRec, &CmndI2SStop, &CmndI2SConfig,
#ifdef USE_I2S_MP3
  &CmndI2SPlay,
#endif
#ifdef USE_I2S_DEBUG
  &CmndI2SMic,
#endif // USE_I2S_DEBUG
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
#ifdef USE_I2S_BRIDGE
  &CmndI2SBridge,
#endif // I2S_BRIDGE
};


/*********************************************************************************************\
 * I2S configuration
\*********************************************************************************************/

void CmndI2SConfig(void) {
  if(!audio_i2s.Settings){
    ResponseCmndChar_P(PSTR("no valid settings"));
    return;
  }
  tI2SSettings * cfg = audio_i2s.Settings;

  // if (zigbee.init_phase) { ResponseCmndChar_P(PSTR(D_ZIGBEE_NOT_STARTED)); return; }
  TrimSpace(XdrvMailbox.data);
  if (strlen(XdrvMailbox.data) > 0) {
    JsonParser parser(XdrvMailbox.data);
    JsonParserObject root = parser.getRootObject();
    if (!root) { ResponseCmndChar_P(D_JSON_INVALID_JSON); return; }

    // remove "I2SConfig" primary key if present
    JsonParserToken config_tk = root[D_PRFX_I2S D_JSON_I2S_CONFIG];
    if ((bool)config_tk) {
      root = config_tk.getObject();
    }

    JsonParserToken sys_tk = root["Sys"];
    if (sys_tk.isObject()) {
      JsonParserObject sys = sys_tk.getObject();
      cfg->sys.mclk_inv[0] = sys.getUInt(PSTR("MclkInv0"), cfg->sys.mclk_inv[0]);
      cfg->sys.mclk_inv[1] = sys.getUInt(PSTR("MclkInv1"), cfg->sys.mclk_inv[1]);
      cfg->sys.bclk_inv[0] = sys.getUInt(PSTR("BclkInv0"), cfg->sys.bclk_inv[0]);
      cfg->sys.bclk_inv[1] = sys.getUInt(PSTR("BclkInv1"), cfg->sys.bclk_inv[1]);
      cfg->sys.ws_inv[0] = sys.getUInt(PSTR("WsInv0"), cfg->sys.ws_inv[0]);
      cfg->sys.ws_inv[1] = sys.getUInt(PSTR("WsInv1"), cfg->sys.ws_inv[1]);
      cfg->sys.mp3_preallocate = sys.getUInt(PSTR("Mp3Preallocate"), cfg->sys.mp3_preallocate);
    }

    JsonParserToken tx_tk = root["Tx"];
    if (tx_tk.isObject()) {
      JsonParserObject tx = tx_tk.getObject();
      cfg->tx.sample_rate = tx.getUInt(PSTR("SampleRate"), cfg->tx.sample_rate);
      cfg->tx.gain = tx.getUInt(PSTR("Gain"), cfg->tx.gain);
      cfg->tx.mode = tx.getUInt(PSTR("Mode"), cfg->tx.mode);
      cfg->tx.slot_mask = tx.getUInt(PSTR("SlotMask"), cfg->tx.slot_mask);
      cfg->tx.slot_config = tx.getUInt(PSTR("SlotConfig"), cfg->tx.slot_config);
      cfg->tx.channels = tx.getUInt(PSTR("Channels"), cfg->tx.channels);
      cfg->tx.apll = tx.getUInt(PSTR("APLL"), cfg->tx.apll);
    }

    JsonParserToken rx_tk = root["Rx"];
    if (rx_tk.isObject()) {
      JsonParserObject rx = rx_tk.getObject();
      cfg->rx.sample_rate = rx.getUInt(PSTR("SampleRate"), cfg->rx.sample_rate);
      float rx_gain = ((float)cfg->rx.gain) / 16;
      rx_gain = rx.getFloat(PSTR("Gain"), rx_gain);
      cfg->rx.gain = (uint16_t)(rx_gain * 16);
      // cfg->rx.gain = rx.getUInt(PSTR("Gain"), cfg->rx.gain);
      cfg->rx.mode = rx.getUInt(PSTR("Mode"), cfg->rx.mode);
      cfg->rx.slot_mask = rx.getUInt(PSTR("SlotMask"), cfg->rx.slot_mask);
      cfg->rx.slot_bit_width = rx.getUInt(PSTR("SlotWidth"), cfg->rx.slot_bit_width);
      cfg->rx.channels = rx.getUInt(PSTR("Channels"), cfg->rx.channels);
      cfg->rx.dc_filter_alpha = rx.getUInt(PSTR("DCFilterAlpha"), cfg->rx.dc_filter_alpha);
      cfg->rx.lowpass_alpha = rx.getUInt(PSTR("LowpassAlpha"), cfg->rx.lowpass_alpha);
      cfg->rx.apll = rx.getUInt(PSTR("APLL"), cfg->rx.apll);
      cfg->rx.ws_width = rx.getUInt(PSTR("WsWidth"), cfg->rx.ws_width);
      cfg->rx.ws_pol = rx.getUInt(PSTR("WsPol"), cfg->rx.ws_pol);
      cfg->rx.bit_shift = rx.getUInt(PSTR("BitShift"), cfg->rx.bit_shift);
      cfg->rx.left_align = rx.getUInt(PSTR("LeftAlign"), cfg->rx.left_align);
      cfg->rx.big_endian = rx.getUInt(PSTR("BigEndian"), cfg->rx.big_endian);
      cfg->rx.bit_order_lsb = rx.getUInt(PSTR("LsbOrder"), cfg->rx.bit_order_lsb);
      cfg->rx.dma_frame_num = rx.getUInt(PSTR("DMAFrame"), cfg->rx.dma_frame_num);
      cfg->rx.dma_desc_num = rx.getUInt(PSTR("DMADesc"), cfg->rx.dma_desc_num);
    }
    I2SSettingsSave(AUDIO_CONFIG_FILENAME);
  }

  float mic_gain = ((float)cfg->rx.gain) / 16;
  Response_P("{\"" D_PRFX_I2S D_JSON_I2S_CONFIG "\":{"
                  // Sys
                  "\"Sys\":{"
                    "\"Version\":%d,"
                    "\"Duplex\":%d,"
                    "\"Tx\":%d,"
                    "\"Rx\":%d,"
                    "\"Exclusive\":%d,"
                    "\"MclkInv0\":%d,"
                    "\"MclkInv1\":%d,"
                    "\"BclkInv0\":%d,"
                    "\"BclkInv1\":%d,"
                    "\"WsInv0\":%d,"
                    "\"WsInv1\":%d,"
                    "\"Mp3Preallocate\":%d"
                  "},"
                  "\"Tx\":{"
                    "\"SampleRate\":%d,"
                    "\"Gain\":%d,"
                    "\"Mode\":%d,"
                    "\"SlotMask\":%d,"
                    "\"SlotConfig\":%d,"
                    "\"Channels\":%d,"
                    "\"APLL\":%d"
                  "},"
                  "\"Rx\":{"
                    "\"SampleRate\":%d,"
                    "\"Gain\":%_f,"
                    // "\"Gain\":%d,"
                    "\"Mode\":%d,"
                    "\"SlotMask\":%d,"
                    "\"SlotWidth\":%d,"
                    "\"Channels\":%d,"
                    "\"DCFilterAlpha\":%d,"
                    "\"LowpassAlpha\":%d,"
                    "\"APLL\":%d,"
                    "\"WsWidth\":%d,"
                    "\"WsPol\":%d,"
                    "\"BitShift\":%d,"
                    "\"LeftAlign\":%d,"
                    "\"BigEndian\":%d,"
                    "\"LsbOrder\":%d,"
                    "\"DMAFrame\":%d,"
                    "\"DMADesc\":%d"
                  "}}}",
                  cfg->sys.version,
                  cfg->sys.duplex,
                  cfg->sys.tx,
                  cfg->sys.rx,
                  cfg->sys.exclusive,
                  cfg->sys.mclk_inv[0],
                  cfg->sys.mclk_inv[1],
                  cfg->sys.bclk_inv[0],
                  cfg->sys.bclk_inv[1],
                  cfg->sys.ws_inv[0],
                  cfg->sys.ws_inv[1],
                  cfg->sys.mp3_preallocate,
                  //
                  cfg->tx.sample_rate,
                  cfg->tx.gain,
                  cfg->tx.mode,
                  cfg->tx.slot_mask,
                  cfg->tx.slot_config,
                  cfg->tx.channels,
                  cfg->tx.apll,
                  //
                  cfg->rx.sample_rate,
                  &mic_gain,
                  // cfg->rx.gain,
                  cfg->rx.mode,
                  cfg->rx.slot_mask,
                  cfg->rx.slot_bit_width,
                  cfg->rx.channels,
                  cfg->rx.dc_filter_alpha,
                  cfg->rx.lowpass_alpha,
                  cfg->rx.apll,
                  cfg->rx.ws_width,
                  cfg->rx.ws_pol,
                  cfg->rx.bit_shift,
                  cfg->rx.left_align,
                  cfg->rx.big_endian,
                  cfg->rx.bit_order_lsb,
                  cfg->rx.dma_frame_num,
                  cfg->rx.dma_desc_num
                  );
}
/*********************************************************************************************\
 * microphone related functions
\*********************************************************************************************/

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
  uint32_t timeForOneRead;

  if (!audio_i2s_mp3.use_stream) {
    mp3_out = ufsp->open(audio_i2s_mp3.mic_path, "w");
    if (!mp3_out) {
      error = 1;
      goto exit;
    }
  } else {
    if (!audio_i2s_mp3.stream_active) {
      error = 2;
      audio_i2s_mp3.use_stream = 0;
      goto exit;
    }
    audio_i2s_mp3.client.flush();
    audio_i2s_mp3.client.setTimeout(3);
    audio_i2s_mp3.client.print("HTTP/1.1 200 OK\r\n"
    "Content-Type: audio/mpeg;\r\n\r\n");

   //  Webserver->send(200, "application/octet-stream", "");
    //"Content-Type: audio/mp3;\r\n\r\n");
  }

  shine_set_config_mpeg_defaults(&config.mpeg);

  if (audio_i2s.Settings->rx.channels == 1) {
    config.mpeg.mode = MONO;
  } else {
    config.mpeg.mode = STEREO;
  }
  // config.mpeg.bitr = 128; - this is default anyway, but maybe we want to make it a variable in the future
  config.wave.samplerate = audio_i2s.Settings->rx.sample_rate;
  config.wave.channels = (channels)(audio_i2s.Settings->rx.channels);

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
  bytesize = samples_per_pass * 2 * (audio_i2s.Settings->rx.channels);

  buffer = (int16_t*)malloc(bytesize);
  if (!buffer) {
    error = 5;
    goto exit;
  }

  ctime = TasmotaGlobal.uptime;
  timeForOneRead = 1000 / ((audio_i2s.Settings->rx.sample_rate / (samples_per_pass * audio_i2s.Settings->rx.channels )));
  timeForOneRead -= 1; // be very in time

  AddLog(LOG_LEVEL_DEBUG, PSTR("I2S: samples %u, bytesize %u, time: %u"),samples_per_pass, bytesize, timeForOneRead);

  while (!audio_i2s_mp3.mic_stop) {
      TickType_t xLastWakeTime = xTaskGetTickCount();
      size_t bytes_read;

      // bytes_read = audio_i2s.in->readMic((uint8_t*)buffer, bytesize, true /*dc_block*/, false /*apply_gain*/, true /*lowpass*/, nullptr /*peak_ptr*/);
      i2s_channel_read(audio_i2s.in->getRxHandle(), (void*)buffer, bytesize, &bytes_read, pdMS_TO_TICKS(3));

      if(bytes_read < bytesize) AddLog(LOG_LEVEL_DEBUG, PSTR("!! %u, %u"), bytes_read, bytesize);

      if (gain > 1) {
        // set gain the "old way"
        int16_t _gain = gain / 16;
        for (uint32_t cnt = 0; cnt < bytes_read / 2; cnt++) {
          buffer[cnt] *= _gain;
        }
      }
      ucp = shine_encode_buffer_interleaved(s, buffer, &written);

      if (!audio_i2s_mp3.use_stream) {
        bwritten = mp3_out.write(ucp, written);
        if (bwritten != written) {
          break;
        }
      } else {
        audio_i2s_mp3.client.write((const char*)ucp, written);

        if (!audio_i2s_mp3.client.connected()) {
          break;
        }
      }
      audio_i2s_mp3.recdur = TasmotaGlobal.uptime - ctime;
      
      vTaskDelayUntil( &xLastWakeTime, pdMS_TO_TICKS(timeForOneRead));
  }

  ucp = shine_flush(s, &written);

  if (!audio_i2s_mp3.use_stream) {
    mp3_out.write(ucp, written);
  } else {
    audio_i2s_mp3.client.write((const char*)ucp, written);
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

  if (audio_i2s_mp3.use_stream) {
    audio_i2s_mp3.client.stop();
  }

  audio_i2s.in->stopRx();
  audio_i2s_mp3.mic_stop = 0;
  audio_i2s_mp3.mic_error = error;
  AddLog(LOG_LEVEL_INFO, PSTR("I2S: mp3task result code: %d"), error);
  audio_i2s_mp3.mic_task_handle = 0;
  audio_i2s_mp3.recdur = 0;
  audio_i2s_mp3.stream_active = 0;
  vTaskDelete(NULL);

}

int32_t I2sRecordShine(char *path) {
  esp_err_t err = ESP_OK;

  switch(audio_i2s.Settings->rx.sample_rate){
    case 32000: case 48000: case 44100: 
      break; // supported
    default:
    AddLog(LOG_LEVEL_INFO, PSTR("I2S: unsupported sample rate for MP3 encoding: %d"), audio_i2s.Settings->rx.sample_rate);
    return -1;
  }
   AddLog(LOG_LEVEL_INFO, PSTR("I2S: accepted sample rate for MP3 encoding: %d"), audio_i2s.Settings->rx.sample_rate);

#ifdef USE_I2S_MP3
  if (audio_i2s_mp3.decoder || audio_i2s_mp3.mp3) return 0;
#endif

  strlcpy(audio_i2s_mp3.mic_path, path, sizeof(audio_i2s_mp3.mic_path));
  audio_i2s_mp3.mic_stop = 0;
  uint32_t stack = 8000;
  audio_i2s_mp3.use_stream = !strcmp(audio_i2s_mp3.mic_path, "stream.mp3");

  audio_i2s.in->startRx();

  err = xTaskCreatePinnedToCore(I2sMicTask, "MIC", stack, NULL, 3, &audio_i2s_mp3.mic_task_handle, 1);

  return err;
}

/*********************************************************************************************\
 * Driver Settings load and save using filesystem
\*********************************************************************************************/
// error codes
enum {
  I2S_OK = 0,
  I2S_ERR_OUTPUT_NOT_CONFIGURED,
  I2S_ERR_INPUT_NOT_CONFIGURED,
  I2S_ERR_DECODER_IN_USE,
  I2S_ERR_FILE_NOT_FOUND,
  I2S_ERR_TX_FAILED,
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
    if ((audio_i2s.Settings->sys.version == 0) || (audio_i2s.Settings->sys.version > AUDIO_SETTINGS_VERSION)) {
      AddLog(LOG_LEVEL_DEBUG, "I2S: unsupported configuration version %u, use defaults", audio_i2s.Settings->sys.version);
      delete audio_i2s.Settings;
      audio_i2s.Settings = new tI2SSettings();
      I2SSettingsSave(config_filename);
    }
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
// I2sInit
//
// Initialize I2S driver for input and output
void I2sInit(void) {
  int32_t gpio_din_0 = Pin(GPIO_I2S_DIN, 0);
  int32_t gpio_din_1 = Pin(GPIO_I2S_DIN, 1);
  int32_t gpio_dout_0 = Pin(GPIO_I2S_DOUT, 0);
  int32_t gpio_dout_1 = Pin(GPIO_I2S_DOUT, 1);
  int32_t gpio_ws_0 = Pin(GPIO_I2S_WS, 0);
  int32_t gpio_dac_0 = Pin(GPIO_I2S_DAC, 0);    // DAC-1 needs to be comfigured if DAC-2 is needed as well
  int32_t gpio_dac_1 = Pin(GPIO_I2S_DAC, 1);    // DAC-1 needs to be comfigured if DAC-2 is needed as well

  // we need at least one pin configured
  // Note: in case of ESP32 DAC output we may have only WS_0 configured. DAC is only supported on port 0
  if ((gpio_din_0 < 0) && (gpio_din_1 < 0) && (gpio_dout_0 < 0) && (gpio_dout_1 < 0) && (gpio_ws_0 < 0) && (gpio_dac_0 < 0)) {
    AddLog(LOG_LEVEL_DEBUG,PSTR("I2S: no pin configured"));
    return;
  }

  I2SSettingsLoad(AUDIO_CONFIG_FILENAME, false);    // load configuration (no-erase)
  if (!audio_i2s.Settings) { return; }     // fatal error, could not allocate memory for configuration

  bool duplex = false;      // the same ports are used for input and output
  bool exclusive = false;   // signals that in/out have a shared GPIO and need to un/install driver before use
  bool dac_mode = (gpio_dac_0 >= 0);
  if (dac_mode) {
    audio_i2s.Settings->tx.mode = I2S_MODE_DAC;
  }

  // AddLog(LOG_LEVEL_INFO, PSTR("I2S: init pins bclk=%d, ws=%d, dout=%d, mclk=%d, din=%d"),
  //                         Pin(GPIO_I2S_BCLK, 0) , Pin(GPIO_I2S_WS, 0), Pin(GPIO_I2S_DOUT, 0), Pin(GPIO_I2S_MCLK, 0), Pin(GPIO_I2S_DIN, 0));

  audio_i2s.Settings->sys.duplex = false;
  audio_i2s.Settings->sys.tx = false;
  audio_i2s.Settings->sys.rx = false;
  audio_i2s.Settings->sys.exclusive = false;

  for (uint32_t port = 0; port < SOC_I2S_NUM; port++) {
    int32_t bclk = Pin(GPIO_I2S_BCLK, port);
    int32_t ws = Pin(GPIO_I2S_WS, port);
    int32_t dout = Pin(GPIO_I2S_DOUT, port);
    int32_t mclk = Pin(GPIO_I2S_MCLK, port);
    int32_t din = Pin(GPIO_I2S_DIN, port);
    bool tx = false;      // is Tx enabled for this port
    bool rx = false;      // is Rx enabled for this port

    AddLog(LOG_LEVEL_DEBUG, "I2S: I2S%i bclk=%i, ws=%i, dout=%i, mclk=%i, din=%i", port, bclk, ws, dout, mclk, din);

    // if neither input, nor output, nor DAC/ADC skip (WS could is only needed for DAC but supports only port 0)
    if (din < 0 && dout < 0 && (!(dac_mode && port == 0))) { continue; }

    duplex = (din >= 0) && (dout >= 0);
    if (duplex) {
      if (audio_i2s.Settings->rx.mode == I2S_MODE_PDM || audio_i2s.Settings->tx.mode == I2S_MODE_PDM ){
        exclusive = true;
      }
      AddLog(LOG_LEVEL_DEBUG, "I2S: enabling duplex mode, exclusive:%i", exclusive);
    }

    const char *err_msg = nullptr;   // to save code, we indicate an error with a message configured
    if (din >= 0 || dout >= 0) {
      // we have regular I2S configuration
      // do multiple checks
      // 1. check that WS is configured
      if (ws < 0) {
        // WS may be shared between both ports, so if it is configured on port 0, we accept it on port 1
        int32_t ws0 = Pin(GPIO_I2S_WS, 0);
        if (ws0 >= 0) {
          ws = ws0;
          AddLog(LOG_LEVEL_DEBUG, "I2S: I2S%i WS is shared, using WS from port 0 (%i)", port, ws);
          exclusive = true;
        }
        if (ws < 0) {
          err_msg = "no WS pin configured";
        }
      }
      // 2. check that DAC mode is not enabled for output(incompatible with DIN/DOUT)
      else if (dout >= 0 && audio_i2s.Settings->tx.mode == I2S_MODE_DAC) {
        err_msg = "DAC mode is not compatible with DOUT";
      }
      // 3. check that ADC mode is not enabled for output (incompatible with DIN/DOUT)
      else if (din >= 0 && audio_i2s.Settings->rx.mode == I2S_MODE_DAC) {
        err_msg = "ADC mode is not compatible with DIN";
      }
      // 4. check that output is not already configured
      else if (dout >= 0 && audio_i2s.out) {
        err_msg = "output already configured";
      }
      // 5. check that input is not already configured
      else if (din >= 0 && audio_i2s.in) {
        err_msg = "input already configured";
      }
      // 6. check that we don't try PDM on port 1
      else if (port != 0 && din >= 0 && audio_i2s.Settings->rx.mode == I2S_MODE_PDM) {
        err_msg = "PDM Rx is not supported";
      }
      // 7. check that we don't try PDM on port 1
      else if (port != 0 && dout >= 0 && audio_i2s.Settings->tx.mode == I2S_MODE_PDM) {
        err_msg = "PDM Tx is not supported";
      }
    } else {
      // no DIN/DOUT, try DAC mode
      // 1. Check that tx.mode is I2S_MODE_DAC
      if (dac_mode) {
        AddLog(LOG_LEVEL_DEBUG, "I2S: Configuraing DAC mode DAC0=%i DAC1=%i", gpio_dac_0, gpio_dac_1);
      } else {
        err_msg = "DAC mode is not enabled";
      }
    }

    // is there any error?
    if (err_msg) {
      AddLog(LOG_LEVEL_DEBUG, "I2S: Error: %s for I2S%i, skipping", err_msg, port);
      continue;   // skip this port
    }

    tx = (dout >= 0) || dac_mode;
    rx = (din >= 0);

    if (tx && audio_i2s.out) {
      AddLog(LOG_LEVEL_DEBUG, "I2S: Warning: Tx already configured, skipping superfluous Tx configuration");
      tx = false;
    }
    if (rx && audio_i2s.in) {
      AddLog(LOG_LEVEL_DEBUG, "I2S: Warning: Rx already configured, skipping superfluous Rx configuration");
      rx = false;
    }

    TasmotaI2S * i2s = new TasmotaI2S;
    i2s->setPinout(bclk, ws, dout, mclk, din,
                    audio_i2s.Settings->sys.mclk_inv[0], audio_i2s.Settings->sys.bclk_inv[0],
                    audio_i2s.Settings->sys.ws_inv[0], audio_i2s.Settings->tx.apll);
    i2s->setSlotConfig((i2s_port_t)port, audio_i2s.Settings->tx.slot_config,
                      audio_i2s.Settings->tx.slot_mask, audio_i2s.Settings->rx.slot_mask);
    if (tx) {
      i2s->setTxMode(audio_i2s.Settings->tx.mode);
      // i2s->setTxChannels(audio_i2s.Settings->tx.channels);
      // i2s->setRate(audio_i2s.Settings->tx.sample_rate);
    }
    if (rx) {
      i2s->setRxMode(audio_i2s.Settings->rx.mode);
      i2s->setRxFreq(audio_i2s.Settings->rx.sample_rate);
      i2s->setRxChannels(audio_i2s.Settings->rx.channels);
      i2s->setRxGain(audio_i2s.Settings->rx.gain);
    }

    bool init_tx_ok = false;
    bool init_rx_ok = false;
    if (tx && rx && exclusive) {
      i2s->setExclusive(true);
      audio_i2s.Settings->sys.exclusive = exclusive;
      // in exclusive mode, we need to intialize in sequence Tx and Rx
      init_tx_ok = i2s->startI2SChannel(true, false);
      init_rx_ok = i2s->startI2SChannel(false, true);
    } else if (tx && rx) {
      init_tx_ok = init_rx_ok = i2s->startI2SChannel(true, true);
    } else {
      if (tx) { init_tx_ok = i2s->startI2SChannel(true, false); }
      if (rx) { init_rx_ok = i2s->startI2SChannel(false, true); }
    }
    if (init_tx_ok) { audio_i2s.out = i2s; }
    if (init_rx_ok) { audio_i2s.in = i2s; }
    audio_i2s.Settings->sys.tx |= init_tx_ok; // Do not set to zero id already configured on another channnel
    audio_i2s.Settings->sys.rx |= init_rx_ok;
    if (init_tx_ok && init_rx_ok) { audio_i2s.Settings->sys.duplex = true; }

    // if intput and output are configured, don't proceed with other IS2 ports
    if (audio_i2s.out && audio_i2s.in) {
      break;
    }

  }

  // do we have exclusive mode?
  if (audio_i2s.out) { audio_i2s.out->setExclusive(exclusive); }
  if (audio_i2s.in) { audio_i2s.in->setExclusive(exclusive); }

  // if(audio_i2s.out != nullptr){
  //   audio_i2s.out->SetGain(((float)(audio_i2s.Settings->tx.gain + 1)/ 100.0));
  //   audio_i2s.out->beginTx();     // TODO - useful?
  //   audio_i2s.out->stopTx();
  // }
#ifdef USE_I2S_MP3
  audio_i2s_mp3.mp3ram = nullptr;
  if (audio_i2s.Settings->sys.mp3_preallocate == 1){
    // if (UsePSRAM()) {
    AddLog(LOG_LEVEL_DEBUG,PSTR("I2S: will allocate buffer for mp3 encoder"));
    audio_i2s_mp3.mp3ram = special_malloc(preallocateCodecSize);
  }
#endif // USE_I2S_MP3
  AddLog(LOG_LEVEL_DEBUG, "I2S: I2sInit done");
}

/*********************************************************************************************\
 * General functions
\*********************************************************************************************/

//
// I2SPrepareTx() -> int
//
// Prepare I2S for output, handle exclusive access if necessary
//
// Returns `I2S_OK` if ok to send to output or error code
int32_t I2SPrepareTx(void) {
  I2sStopPlaying();
  
  AddLog(LOG_LEVEL_DEBUG, "I2S: I2SPrepareTx out=%p", audio_i2s.out);
  if (!audio_i2s.out) { return I2S_ERR_OUTPUT_NOT_CONFIGURED; }

  if (!audio_i2s.out->beginTx()) { return I2S_ERR_TX_FAILED; }

  audio_i2s.out->SetGain(((float)(audio_i2s.Settings->tx.gain + 1)/ 100.0));

  return I2S_OK;
}

//
// I2SPrepareRx() -> int
//
// Prepare I2S for input, handle exclusive access if necessary
//
// Returns `I2S_OK` if ok to record input or error code
int32_t I2SPrepareRx(void) {
  if (!audio_i2s.in) return I2S_ERR_OUTPUT_NOT_CONFIGURED;

  if (audio_i2s.Settings->sys.exclusive) {
    // TODO - deconfigure input driver
  }
  return I2S_OK;
}

/*********************************************************************************************\
 * Driver features and commands
\*********************************************************************************************/

#if defined(USE_I2S_MP3) || defined(USE_I2S_WEBRADIO)
void I2sMp3Task(void *arg) {
  audio_i2s_mp3.task_running = true;
  while (audio_i2s_mp3.mp3->isRunning() && audio_i2s_mp3.task_running) {
    if (!audio_i2s_mp3.mp3->loop()) {
        audio_i2s_mp3.task_running = false;
    }
    vTaskDelay(pdMS_TO_TICKS(1));
  }
  audio_i2s.out->flush();
  audio_i2s_mp3.mp3->stop();
  mp3_delete();
  audio_i2s_mp3.mp3_task_handle = nullptr;
  audio_i2s_mp3.task_has_ended = true;
  vTaskDelete(NULL);
}
#endif // defined(USE_I2S_MP3) || defined(USE_I2S_WEBRADIO)

void I2sStatusCallback(void *cbData, int code, const char *string) {
  const char *ptr = reinterpret_cast<const char *>(cbData);
  (void) code;
  (void) ptr;
  AddLog(LOG_LEVEL_DEBUG, "I2S: -> %s", string);
}

#ifdef USE_I2S_MP3
void I2sMp3WrTask(void *arg){
  audio_i2s_mp3.task_running = true;
  audio_i2s_mp3.task_has_ended = false;
  while (audio_i2s_mp3.task_running) {
    if (audio_i2s_mp3.decoder && audio_i2s_mp3.decoder->isRunning()) {
      if (!audio_i2s_mp3.decoder->loop()) {
        audio_i2s_mp3.task_running = false;
      }
      vTaskDelay(pdMS_TO_TICKS(1));
    }
  }
  audio_i2s.out->flush();
  I2sWebRadioStopPlaying();
  audio_i2s_mp3.mp3_task_handle = nullptr;
  audio_i2s_mp3.task_has_ended = true;
  vTaskDelete(NULL);
}

#endif // USE_I2S_MP3

void I2sStopPlaying() {
  I2SAudioPower(false);

  if(audio_i2s_mp3.task_running){
    audio_i2s_mp3.task_running = false;
    while(audio_i2s_mp3.task_has_ended == false){
      delay(10);
    }
    while(audio_i2s_mp3.mp3){
      delay(10);
    }
  }
  if (audio_i2s_mp3.mic_task_handle) {
    audio_i2s_mp3.mic_stop = 1;
    while (audio_i2s_mp3.mic_stop) {
      delay(10);
    }
  }
}

#ifdef USE_I2S_MP3
// Play_mp3 - Play a MP3 file from filesystem
//
// Returns I2S_error_t
int32_t I2SPlayMp3(const char *path) {
  int32_t i2s_err = I2SPrepareTx();
  if ((i2s_err) != I2S_OK) { return i2s_err; }
  if (audio_i2s_mp3.mp3) return I2S_ERR_DECODER_IN_USE;

  // check if the filename starts with '/', if not add it
  char fname[64];
  if (path[0] != '/') {
    snprintf(fname, sizeof(fname), "/%s", path);
  } else {
    snprintf(fname, sizeof(fname), "%s", path);
  }
  if (!ufsp->exists(fname)) { return I2S_ERR_FILE_NOT_FOUND; }

  I2SAudioPower(true);

  audio_i2s_mp3.file = new AudioFileSourceFS(*ufsp, fname);

  audio_i2s_mp3.id3 = new AudioFileSourceID3(audio_i2s_mp3.file);

  if (audio_i2s_mp3.mp3ram) {
    audio_i2s_mp3.mp3 = new AudioGeneratorMP3(audio_i2s_mp3.mp3ram, preallocateCodecSize);
  } else {
    audio_i2s_mp3.mp3 = new AudioGeneratorMP3();
  }
  audio_i2s_mp3.mp3->begin(audio_i2s_mp3.id3, audio_i2s.out);

  // Always use a task
  xTaskCreatePinnedToCore(I2sMp3Task, "MP3", 8192, NULL, 3, &audio_i2s_mp3.mp3_task_handle, 1);
  return I2S_OK;
}

void mp3_delete(void) {
  delete audio_i2s_mp3.file;
  delete audio_i2s_mp3.id3;
  delete audio_i2s_mp3.mp3;
  audio_i2s_mp3.mp3 = nullptr;

  // if (audio_i2s_mp3.decoder) {
    // audio_i2s_mp3.decoder->stop();
    // delete audio_i2s_mp3.decoder;
    // audio_i2s_mp3.decoder = nullptr;
    // AddLog(LOG_LEVEL_DEBUG, "I2S: audio_i2s_mp3.decoder = nullptr");
  // }
}
#endif // USE_I2S_MP3

/*********************************************************************************************\
 * Commands
\*********************************************************************************************/

void CmndI2SMic(void) {
  if (I2SPrepareRx()) {
    ResponseCmndChar("I2S Mic not configured");
    return;
  }

  esp_err_t err = ESP_OK;
  audio_i2s.in->startRx();
  if (audio_i2s.in->getRxRunning()) {
    uint8_t buf[128];

    size_t bytes_read = 0;
    int32_t btr = audio_i2s.in->readMic(buf, sizeof(buf), true /*dc_block*/, false /*apply_gain*/, true /*lowpass*/, nullptr /*peak_ptr*/);
    if (btr < 0) {
      AddLog(LOG_LEVEL_INFO, "I2S: Mic (err:%i)", -btr);
      ResponseCmndChar("I2S Mic read error");
      return;
    }
    // esp_err_t err = i2s_channel_read(audio_i2s.in->getRxHandle(), buf, sizeof(buf), &bytes_read, 0);
    // if (err == ESP_ERR_TIMEOUT) { err = ESP_OK; }
    AddLog(LOG_LEVEL_INFO, "I2S: Mic (%i) %*_H", btr, btr, buf);
  }

  // err = xTaskCreatePinnedToCore(I2sMicTask, "MIC", stack, NULL, 3, &audio_i2s_mp3.mic_task_handle, 1);

  ResponseCmndDone();
}


void CmndI2SStop(void) {
  if (I2SPrepareTx() != I2S_OK) {
    ResponseCmndChar("I2S output not configured");
    return;
  }
  audio_i2s.out->setGain(0);
  ResponseCmndDone();
}

#ifdef USE_I2S_MP3
void CmndI2SPlay(void) {
  if (XdrvMailbox.data_len > 0) {
    int32_t err = I2SPlayMp3(XdrvMailbox.data);
    // display return message
    switch (err) {
      case I2S_OK:
        ResponseCmndDone();
        break;
      case I2S_ERR_OUTPUT_NOT_CONFIGURED:
        ResponseCmndChar("I2S output not configured");
        break;
      case I2S_ERR_DECODER_IN_USE:
        ResponseCmndChar("Decoder already in use");
        break;
      case I2S_ERR_FILE_NOT_FOUND:
        ResponseCmndChar("File not found");
        break;
      case I2S_ERR_TX_FAILED:
        ResponseCmndChar("Unable to open sound output");
        break;
      default:
        ResponseCmndChar("Unknown error");
        break;
    }
  } else {
    ResponseCmndChar("Missing filename");
  }
}
#endif // USE_I2S_MP3

void CmndI2SGain(void) {
  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 100)) {
    if (audio_i2s.out) {
      audio_i2s.Settings->tx.gain = XdrvMailbox.payload;
      audio_i2s.out->SetGain(((float)(audio_i2s.Settings->tx.gain+1)/100.0));
    }
  }
  ResponseCmndNumber(audio_i2s.Settings->tx.gain);
}

void CmndI2SSay(void) {
  if (XdrvMailbox.data_len > 0) {
    if (I2SPrepareTx() != I2S_OK) {
      ResponseCmndChar("I2S output not configured");
      return;
    } else {
      I2SAudioPower(true);
      ESP8266SAM sam;
      sam.Say(audio_i2s.out, XdrvMailbox.data);
      audio_i2s.out->stopTx();
      I2SAudioPower(false);
      // end of scope, ESP8266SAM is destroyed
    }
  }
  ResponseCmndChar(XdrvMailbox.data);
}

void CmndI2SI2SRtttl(void) {
  if (I2SPrepareTx() != I2S_OK) {
    ResponseCmndChar("I2S output not configured");
    return;
  }
  if (XdrvMailbox.data_len > 0) {
    Rtttl(XdrvMailbox.data);
  }
  ResponseCmndChar(XdrvMailbox.data);
}

void CmndI2SMicRec(void) {
  if (I2SPrepareRx()) {
    ResponseCmndChar("I2S Mic not configured");
    return;
  }
  if (audio_i2s_mp3.mp3ram == nullptr){
    AddLog(LOG_LEVEL_DEBUG,PSTR("I2S: try late buffer allocation for mp3 encoder"));
    audio_i2s_mp3.mp3ram = special_malloc(preallocateCodecSize);
  }
  if (audio_i2s_mp3.mp3ram != nullptr) {
    if (XdrvMailbox.data_len > 0) {
      if (!strncmp(XdrvMailbox.data, "-?", 2)) {
        Response_P("{\"I2SREC-duration\":%d}", audio_i2s_mp3.recdur);
      } else {
        int err = I2sRecordShine(XdrvMailbox.data);
        if(err == pdPASS){
          ResponseCmndChar(XdrvMailbox.data);
        } else {
          ResponseCmndChar_P(PSTR("Did not launch recording task"));
        }
      }
    } else {
      if (audio_i2s_mp3.mic_task_handle) {
        // stop task
        audio_i2s_mp3.mic_stop = 1;
        while (audio_i2s_mp3.mic_stop) {
          delay(1);
        }
        ResponseCmndChar_P(PSTR("Stopped"));
      }
      else {
        ResponseCmndChar_P(PSTR("No running recording"));
      }
    }
  }
  else{
    if (audio_i2s.in){
      ResponseCmndChar_P(PSTR("need PSRAM for MP3 recording"));
    }
    else{
      ResponseCmndChar_P(PSTR("no mic configured"));
    }
  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

void I2sMp3Loop(void);
void I2sMp3Init(uint32_t on);
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
      // audio_i2s.Settings->tx.stream_enable = 1;
      // I2sMp3Init(1);
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
