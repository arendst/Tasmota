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
#ifdef USE_I2S_AAC
#include "AudioGeneratorAAC.h"
#endif // USE_I2S_AAC
#ifdef USE_I2S_OPUS
#include "AudioGeneratorOpus.h"
#endif // USE_I2S_OPUS

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

constexpr int preallocateBufferSize = 16*1024;
constexpr int preallocateCodecSize  = 29192; // MP3 codec max mem needed
constexpr int preallocateCodecSizeAAC = 85332; // AAC+SBR codec max mem needed

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
  AudioGenerator *decoder = nullptr;
  AudioFileSource *file = nullptr;
  AudioFileSource *id3 = nullptr;
  AudioFileSource *buff = NULL;

  void *preallocateBuffer = NULL;
  void *preallocateCodec = NULL;
#endif // USE_I2S_MP3

#if defined(USE_I2S_MP3) || defined(USE_I2S_WEBRADIO) || defined(USE_SHINE) || defined(MP3_MIC_STREAM)
  TaskHandle_t mp3_task_handle;
  TaskHandle_t mic_task_handle;
  char audio_title[64];
#endif // defined(USE_I2S_MP3) || defined(USE_I2S_WEBRADIO)

  char mic_path[32];
  int8_t mic_error;
  bool mic_stop = false;
  bool use_stream = false;
  bool task_running = false;
  bool file_play_pausing = false;
  bool task_has_ended = false;
  bool file_has_paused = false;
  bool task_loop_mode = false;
  uint8_t current_file_type = 0; // for resumed playback
  uint32_t paused_position = 0; // position in the file for paused audio file

// RECORD/STREAM/ENCODING
  uint32_t recdur;
  uint8_t encoder_type;
  bool  stream_active;
  bool  stream_enable;
  WiFiClient client;
  ESP8266WebServer *StreamServer;

// I2S_BRIDGE
  BRIDGE_MODE bridge_mode;
  WiFiUDP i2s_bridge_udp;
  WiFiUDP i2s_bridgec_udp;
  IPAddress i2s_bridge_ip;
  TaskHandle_t i2s_bridge_h;
  int8_t ptt_pin = -1;

} audio_i2s_mp3;

struct AUDIO_I2S_WEBRADIO_t {
  AudioFileSourceICYStream *ifile = NULL;
} Audio_webradio;

#define I2S_AUDIO_MODE_MIC 1
#define I2S_AUDIO_MODE_SPK 2

/*********************************************************************************************\
 * Presentation
\*********************************************************************************************/

#ifdef USE_WEBSERVER
const char HTTP_I2SAUDIO[] PROGMEM =
   "{s}" "Audio:" "{m}%s{e}";

void I2sWrShow(bool json) {
    if (audio_i2s_mp3.decoder) {
      if (json) {
        ResponseAppend_P(PSTR(",\"Audio\":{\"Title\":\"%s\"}"), audio_i2s_mp3.audio_title);
      } else {
        WSContentSend_PD(HTTP_I2SAUDIO,audio_i2s_mp3.audio_title);
      }
    }
}
#endif  // USE_WEBSERVER

/*********************************************************************************************\
 * Commands definitions
\*********************************************************************************************/

const char kI2SAudio_Commands[] PROGMEM = "I2S|"
  "Gain|Rec|Stop|Config"
#ifdef USE_I2S_MP3
  "|Play|Loop|Pause"
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
  &CmndI2SLoop,
  &CmndI2SPause,
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
 * Driver Settings load and save using filesystem
\*********************************************************************************************/
// error codes
enum {
  I2S_OK = 0,
  I2S_ERR_OUTPUT_NOT_CONFIGURED,
  I2S_ERR_INPUT_NOT_CONFIGURED,
  I2S_ERR_DECODER_IN_USE,
  I2S_ERR_DECODER_FAILED_TO_INIT,
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
    exclusive = true; //TODO: try fix full dupleyx mode
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
  audio_i2s_mp3.preallocateCodec = nullptr;
  if (audio_i2s.Settings->sys.mp3_preallocate == 1){
    // if (UsePSRAM()) {
    AddLog(LOG_LEVEL_DEBUG,PSTR("I2S: will allocate buffer for mp3 encoder"));
    audio_i2s_mp3.preallocateCodec = special_malloc(preallocateCodecSize);
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
  audio_i2s_mp3.file_play_pausing = false;
  while (audio_i2s_mp3.decoder->isRunning() && audio_i2s_mp3.task_running) {
    if (!audio_i2s_mp3.decoder->loop()) {
      audio_i2s_mp3.task_running = false;
    }
    if (audio_i2s_mp3.file_play_pausing == true) {
      audio_i2s_mp3.paused_position = audio_i2s_mp3.file->getPos();
      audio_i2s_mp3.decoder->stop();
      audio_i2s_mp3.file_has_paused = true;
    }
    vTaskDelay(pdMS_TO_TICKS(1));
  }
  audio_i2s.out->flush();
  audio_i2s_mp3.decoder->stop();
  audio_i2s_mp3.task_loop_mode = false;
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
    while(audio_i2s_mp3.decoder){
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

bool I2SinitDecoder(uint32_t decoder_type){
  switch(decoder_type){
    case MP3_DECODER:
      if (audio_i2s_mp3.preallocateCodec) {
        audio_i2s_mp3.decoder = dynamic_cast<AudioGenerator *>(new AudioGeneratorMP3(audio_i2s_mp3.preallocateCodec, preallocateCodecSize));
      } else {
        audio_i2s_mp3.decoder = dynamic_cast<AudioGenerator *>(new AudioGeneratorMP3());
      }
      break;
#ifdef USE_I2S_AAC
    case AAC_DECODER:
      audio_i2s_mp3.preallocateCodec = special_realloc(audio_i2s_mp3.preallocateCodec, preallocateCodecSizeAAC);
      if(audio_i2s_mp3.preallocateCodec == nullptr){
        AddLog(LOG_LEVEL_ERROR, "I2S: could not alloc heap for AAC");
        return false;
      }
      audio_i2s_mp3.decoder = dynamic_cast<AudioGenerator *>(new AudioGeneratorAAC(audio_i2s_mp3.preallocateCodec, preallocateCodecSizeAAC));
      break;
#endif //USE_I2S_AAC
#ifdef USE_I2S_OPUS
    case OPUS_DECODER:
      free(audio_i2s_mp3.preallocateCodec);
      audio_i2s_mp3.preallocateCodec = nullptr;
      audio_i2s_mp3.decoder = dynamic_cast<AudioGenerator *>(new AudioGeneratorOpus());
      break;
#endif //USE_I2S_OPUS
  }
  if(audio_i2s_mp3.decoder == nullptr){
    return false;
  }
  return true;
}

// Play a audio file from filesystem
//
// Returns I2S_error_t
int32_t I2SPlayFile(const char *path, uint32_t decoder_type) {
  if (audio_i2s_mp3.decoder != nullptr) return I2S_ERR_DECODER_IN_USE;

  int32_t i2s_err = I2SPrepareTx();
  if ((i2s_err) != I2S_OK) { return i2s_err; }

  // check if the filename starts with '/', if not add it
  char fname[64];
  if (path[0] != '/') {
    snprintf(fname, sizeof(fname), "/%s", path);
  } else {
    snprintf(fname, sizeof(fname), "%s", path);
  }
  if (!ufsp->exists(fname)) { return I2S_ERR_FILE_NOT_FOUND; }

  strncpy(audio_i2s_mp3.audio_title, fname, sizeof(audio_i2s_mp3.audio_title));
  audio_i2s_mp3.audio_title[sizeof(audio_i2s_mp3.audio_title)-1] = 0;
  audio_i2s_mp3.current_file_type = decoder_type; //save for i2spause

  I2SAudioPower(true);

  if (audio_i2s_mp3.task_loop_mode == true){
    File _loopFile = ufsp->open(fname);
    size_t _fsize = _loopFile.size();
    audio_i2s_mp3.preallocateBuffer = special_realloc(audio_i2s_mp3.preallocateBuffer,_fsize);
    size_t _received = _loopFile.read(reinterpret_cast<uint8_t*>(audio_i2s_mp3.preallocateBuffer),_fsize);
    _loopFile.close();

    audio_i2s_mp3.file = new AudioFileSourceLoopBuffer (audio_i2s_mp3.preallocateBuffer, _fsize); // use the id3 var to make the code shorter down the line
  } else {
    audio_i2s_mp3.file = new AudioFileSourceFS(*ufsp, fname);
  }
  audio_i2s_mp3.id3 = new AudioFileSourceID3(audio_i2s_mp3.file);

  if(I2SinitDecoder(decoder_type)){
    audio_i2s_mp3.decoder->begin(audio_i2s_mp3.id3, audio_i2s.out);
    audio_i2s_mp3.file->seek(audio_i2s_mp3.paused_position, 1); // seek to the position where we paused or have it set to 0, 1 = SEEK_CUR
  } else {
    return I2S_ERR_DECODER_FAILED_TO_INIT;
  }

  size_t play_tasksize = 8000; // suitable for ACC and MP3
  if(decoder_type == OPUS_DECODER){ // opus needs a ton of stack
    play_tasksize = 26000;
  }

  // Always use a task
  xTaskCreatePinnedToCore(I2sMp3Task, "PLAYFILE", play_tasksize, NULL, 3, &audio_i2s_mp3.mp3_task_handle, 1);
  return I2S_OK;
}

void mp3_delete(void) {
  delete audio_i2s_mp3.buff;
  delete audio_i2s_mp3.file;
  delete audio_i2s_mp3.id3;
  delete audio_i2s_mp3.decoder;
  audio_i2s_mp3.decoder = nullptr;
}

void i2s_clean_pause_data(void) {
  audio_i2s_mp3.current_file_type = MP3_DECODER;
  audio_i2s_mp3.paused_position = 0;
  audio_i2s_mp3.file_play_pausing = false;
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
  i2s_clean_pause_data();
  ResponseCmndDone();
}

#ifdef USE_I2S_MP3
void CmndI2SLoop(void) {
    audio_i2s_mp3.task_loop_mode = 1;
    CmndI2SPlay();
}

void CmndI2SPause(void) {
  if(audio_i2s_mp3.task_running 
#ifdef MP3_MIC_STREAM
    && !audio_i2s_mp3.stream_active
#endif //MP3_MIC_STREAM
#ifdef USE_I2S_WEBRADIO
    && Audio_webradio.ifile == nullptr
#endif // USE_I2S_WEBRADIO
  ){
    audio_i2s_mp3.file_play_pausing = true;
    ResponseCmndChar("Player Paused");
  } else {
    ResponseCmndChar("Player not running"); // or webradio is using decoder
  }
}

void CmndI2SPlay(void) {
  if (XdrvMailbox.data_len > 0) {
    i2s_clean_pause_data(); // clean up any previous pause data, set start to 0
    int32_t err = I2SPlayFile(XdrvMailbox.data, XdrvMailbox.index);
    // display return message
    switch (err) {
      case I2S_OK:
        ResponseCmndChar("Started");
        break;
      case I2S_ERR_OUTPUT_NOT_CONFIGURED:
        ResponseCmndChar("I2S output not configured");
        break;
      case I2S_ERR_DECODER_IN_USE:
        ResponseCmndChar("Decoder already in use");
        break;
      case I2S_ERR_DECODER_FAILED_TO_INIT:
        ResponseCmndChar("Decoder failed to init");
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
    if(audio_i2s_mp3.file_play_pausing == true){
      int32_t err = I2SPlayFile((const char *)audio_i2s_mp3.audio_title, (uint32_t)audio_i2s_mp3.current_file_type); // the line above should rule out basically any error, but we'll see ...
      AddLog(LOG_LEVEL_DEBUG, "I2S: Resume: %s, type: %i at %i , err: %i", audio_i2s_mp3.audio_title, audio_i2s_mp3.current_file_type, audio_i2s_mp3.paused_position, err);
      ResponseCmndChar("Player resumed");
    } else {
      ResponseCmndChar("Missing filename");
    }
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

  if (XdrvMailbox.data_len > 0) {
    if (!strncmp(XdrvMailbox.data, "-?", 2)) {
      Response_P("{\"I2SREC-duration\":%d}", audio_i2s_mp3.recdur);
    } else {
      audio_i2s_mp3.use_stream = false;
      int err = I2sRecord(XdrvMailbox.data, XdrvMailbox.index);
      // int err = I2sRecordShine(XdrvMailbox.data);
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

void I2sEventHandler(){
  if(audio_i2s_mp3.file_has_paused == true){
    audio_i2s_mp3.task_has_ended = false; //do not send ended event
    audio_i2s_mp3.file_has_paused = false;
    audio_i2s_mp3.task_running = false;
    MqttPublishPayloadPrefixTopicRulesProcess_P(RESULT_OR_STAT,PSTR(""),PSTR("{\"Event\":{\"I2SPlay\":\"Paused\"}}"));
    // Rule1 ON event#i2splay=paused DO <something> ENDON
    I2SAudioPower(false);
  }
  if(audio_i2s_mp3.task_has_ended == true){
    audio_i2s_mp3.task_has_ended = false;
      audio_i2s_mp3.task_running = false;
    MqttPublishPayloadPrefixTopicRulesProcess_P(RESULT_OR_STAT,PSTR(""),PSTR("{\"Event\":{\"I2SPlay\":\"Ended\"}}"));
    // Rule1 ON event#i2splay=ended DO <something> ENDON
    I2SAudioPower(false);
  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

void I2sStreamLoop(void);

bool Xdrv42(uint32_t function) {
  bool result = false;

  switch (function) {
    case FUNC_INIT:
      I2sInit();
      break;
    case FUNC_EVERY_50_MSECOND:
      I2sEventHandler();
      break;
    case FUNC_COMMAND:
      result = DecodeCommand(kI2SAudio_Commands, I2SAudio_Command);
      break;
    case FUNC_WEB_ADD_MAIN_BUTTON:
      //MP3ShowStream();
      break;
    case FUNC_LOOP:
#if defined(USE_SHINE) && defined(MP3_MIC_STREAM)
      I2sStreamLoop();
#endif
#if defined(I2S_BRIDGE)
      i2s_bridge_loop();
#endif
      break;
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
