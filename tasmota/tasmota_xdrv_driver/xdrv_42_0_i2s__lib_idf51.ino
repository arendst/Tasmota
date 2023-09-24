/*
  xdrv_42_0_i2s__lib_idf51.ino - Simplified Audio library

  Copyright (C) 2021  Gerhard Mutz, Theo Arends, Staars, Stephan Hadinger

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
#ifdef USE_I2S_AUDIO

#include "driver/i2s_std.h"
#include "driver/i2s_pdm.h"
#include "driver/gpio.h"

#include "AudioFileSourcePROGMEM.h"
#include "AudioFileSourceID3.h"
#include "AudioGeneratorMP3.h"

#include <ESP8266SAM.h>
#include "AudioFileSourceFS.h"
#include "AudioGeneratorTalkie.h"
#include "AudioFileSourceICYStream.h"
#include "AudioFileSourceBuffer.h"
#include "AudioGeneratorAAC.h"

#include <layer3.h>
#include <types.h>

/*********************************************************************************************\
 * Driver Settings in memory
\*********************************************************************************************/

typedef struct{
  struct{
    uint8_t   version = 0;    // B00

    // runtime options, will be saved but ignored on setting read
    bool      duplex = 0;     // B01 - depends on GPIO setting and SOC caps, DIN and DOUT on same port in GPIO means -> try to use duplex if possible
    bool      tx = 0;         // B02 - depends on GPIO setting
    bool      rx = 0;         // B03 - depends on GPIO setting
    uint32_t  spare01;        // B04-07
  } sys;
  struct {
    uint8_t   mode = 0;       // B00 STD = 0, PDM = 1, TDM = 2
    bool     apll = 1;        // B01 - will be ignored on unsupported SOC's
    bool     channels = 0;    // B02 - 1 = mono, 2 = stereo
    uint8_t  codec = 0;       // B03 - S3 box only, unused for now
    uint8_t  slot_config = 0; // B04 - slot configuration MSB = 0, PCM = 1, PHILIPS = 2
    uint8_t  volume = 10;     // B05
    bool     mclk_inv = 0;    // B06 - invert mclk
    bool     bclk_inv = 0;    // B07 - invert bclk
    bool     ws_inv = 0;      // B08 - invert ws
    uint8_t  spare[7];        // B09-0F
  } tx;
  struct {
    uint32_t  sample_rate = 32000;  // B00-03
    uint8_t   gain = 30;            // B04
    uint8_t   mode = 0;             // B05 - STD = 0, PDM = 1, TDM = 2

    uint8_t   slot_mask = 1;        // B06 - left = 1 /right = 2 /both = 3
    uint8_t   slot_mode = 0;        // B07 - mono/stereo - 1 is added for both
    uint8_t   codec = 0;            // B08 - unused for now
    uint8_t   mp3_encoder = 1;      // B09 - will be ignored without PS-RAM
    uint8_t  spare[6];              // B0A-0F
  } rx;
} tI2SSettings;

typedef union {
  uint8_t data;
  struct {
    uint8_t master : 1;
    uint8_t enabled : 1;
    uint8_t swap_mic : 1;
    uint8_t mode : 2;
  };
} BRIDGE_MODE;

class TasmotaAudioOutputI2S;

struct AUDIO_I2S_t {
  tI2SSettings *Settings;

  i2s_chan_handle_t rx_handle = nullptr;

  AudioGeneratorMP3 *mp3 = nullptr;
  AudioFileSourceFS *file = nullptr;

  TasmotaAudioOutputI2S *out = nullptr;

  AudioFileSourceID3 *id3 = nullptr;
  AudioGeneratorMP3 *decoder = NULL;
  void *mp3ram = NULL;

  TaskHandle_t mp3_task_handle;
  TaskHandle_t mic_task_handle;

  uint32_t mic_size;
  uint8_t *mic_buff;
  char mic_path[32];
  File fwp;
  uint8_t mic_stop;
  int8_t mic_error;
  bool use_stream = false;


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

} audio_i2s;

enum : int { EXTERNAL_I2S = 0, INTERNAL_DAC = 1, INTERNAL_PDM = 2 };

/*********************************************************************************************\
 * Class for outputting sound as endpoint for ESP8266Audio library
\*********************************************************************************************/

class TasmotaAudioOutputI2S : public AudioOutput
{
public:

  // Constructor takes no parameter, everything is configured from template and config file
  TasmotaAudioOutputI2S() {
    loadSettings();
  }

  ~TasmotaAudioOutputI2S() {
    this->stop();
  }

  // ------------------------------------------------------------------------------------------
  // Setters for configuration parameters
  //
  // TODO: not sure we still need them since all this should be set at initialiation
  // ------------------------------------------------------------------------------------------
  virtual bool SetBitsPerSample(int bits) {
    AddLog(LOG_LEVEL_DEBUG,PSTR("I2S: SetBitsPerSample: %i"), bits);
    if ( (bits != 16) && (bits != 8) ) { return false; }
    this->bps = bits;
    return true;
  }

  virtual bool SetChannels(int channels) {
    AddLog(LOG_LEVEL_DEBUG,PSTR("I2S: SetChannels: %i"), channels);
    if ((channels < 1) || (channels > 2)) { return false; }
    if (channels == (int)this->channels) { return true; }
    this->channels = channels;
    return true;
  }

  virtual bool SetRate(int hz) {
    AddLog(LOG_LEVEL_DEBUG,PSTR("I2S: SetRate: %i"), hz);
    if (hz == (int) this->hertz) { return true; }
    this->hertz = hz;
    if (_i2s_on) {
      int result = updateClockConfig();
    }
    return true;
  }

  virtual bool SetGain(float f) {
    AddLog(LOG_LEVEL_DEBUG,PSTR("I2S: SetGain: %_f"), &f);
    return AudioOutput::SetGain(f);
  }

  // ------------------------------------------------------------------------------------------
  // Getters
  inline uint8_t getTxMode(void) const { return _tx_mode; }
  inline uint8_t getTxChannels(void) const { return _tx_channels; }
  inline bool getTxEnabled(void) const { return _tx_enabled; }

  // ------------------------------------------------------------------------------------------
  // Setters
  inline void setTxMode(uint8_t mode) { _tx_mode = mode; }
  inline void setTxChannels(uint8_t channels) { _tx_channels = channels; }
  inline void setTxEnabled(bool enabled) { _tx_enabled = enabled; }

  // ------------------------------------------------------------------------------------------
  // AudioOutput has inconsistent case for methods, provide consistent setters for super class
  inline void setRate(int hz) { SetRate(hz); } 
  inline void setBitsPerSample(int bits) { SetBitsPerSample(bits); }
  inline void setChannels(int chan) { SetChannels(chan); }
  inline void setGain(float f) { SetGain(f); }

  bool begin(void);
  bool stop(void);
  bool ConsumeSample(int16_t sample[2]);
  bool startI2SChannel(void);
  int updateClockConfig(void);

  // The following is now the preferred function
  // and allows to send multiple samples at once
  //
  // Max 128 samples, it is clipped otherwise
  // Returns: the number of samples actually consumed
  //   or -1 if an error occured
  //
  // The call is non blocking and does not wait
  int32_t consumeSamples(int16_t *samples, size_t count);

protected:
  void loadSettings(void);            // load all settings from Settings file and template - the goal is to have zero touch

protected:

  bool    _i2s_on = false;            // is I2S audio active
  // local copy of useful settings for audio
  // TX
  uint8_t _tx_mode = EXTERNAL_I2S;    // EXTERNAL_I2S = 0, INTERNAL_DAC = 1, INTERNAL_PDM = 2
  bool    _tx_enabled = false;        // true = enabled, false = disabled
  uint8_t    _tx_channels = 2;        // true = mono, false = stereo
  i2s_chan_handle_t _tx_chan;         // I2S channel handle, automatically computed

  // GPIOs for I2S
  gpio_num_t  _gpio_mclk = GPIO_NUM_NC;            // GPIO for master clock
  gpio_num_t  _gpio_bclk = GPIO_NUM_NC;            // GPIO for bit clock
  gpio_num_t  _gpio_ws = GPIO_NUM_NC;              // GPIO for word select
  gpio_num_t  _gpio_dout = GPIO_NUM_NC;            // GPIO for data out
  gpio_num_t  _gpio_din = GPIO_NUM_NC;             // GPIO for data in
  bool    _gpio_mclk_inv = false;     // invert master clock
  bool    _gpio_bclk_inv = false;     // invert bit clock
  bool    _gpio_ws_inv = false;       // invert word select

};


// ------------------------------------------------------------------------------------------
// Methods
// ------------------------------------------------------------------------------------------
void TasmotaAudioOutputI2S::loadSettings(void) {
  hertz = 16000;
  _i2s_on = false;
  bps = I2S_DATA_BIT_WIDTH_16BIT;
  _tx_channels = audio_i2s.Settings->tx.channels;
  if (_tx_channels == 0) { _tx_channels = 1; }      // if zero channel default to mono
  if (_tx_channels > 2) { _tx_channels = 2; }       // if > 2 channels default to stereo
  channels = (_tx_channels == 1) ? I2S_SLOT_MODE_MONO : I2S_SLOT_MODE_STEREO;
  _tx_mode = EXTERNAL_I2S;
  _tx_enabled = false;

  _gpio_mclk = (gpio_num_t) Pin(GPIO_I2S_MCLK);
  _gpio_bclk = (gpio_num_t) Pin(GPIO_I2S_BCLK);
  _gpio_ws   = (gpio_num_t) Pin(GPIO_I2S_WS);
  _gpio_dout = (gpio_num_t) Pin(GPIO_I2S_DOUT);
  _gpio_din  = (gpio_num_t) Pin(GPIO_I2S_DIN);
  _gpio_mclk_inv = audio_i2s.Settings->tx.mclk_inv;
  _gpio_bclk_inv = audio_i2s.Settings->tx.bclk_inv;
  _gpio_ws_inv = audio_i2s.Settings->tx.ws_inv;
}

bool TasmotaAudioOutputI2S::begin() {
  if (_tx_enabled) { return true; }
  if (!_i2s_on) {
    if (audio_i2s.Settings->sys.duplex == 0 && audio_i2s.Settings->sys.rx == 1) {
      this->startI2SChannel();
    }
  }
  int result = i2s_channel_enable(_tx_chan);
  if (result != 0){
      AddLog(LOG_LEVEL_INFO, "I2S: Could not enable i2s_channel: %i", result);
      return false;
  }
  _tx_enabled = true;
  return true;
}

bool TasmotaAudioOutputI2S::stop() {
  i2s_channel_disable(_tx_chan);
  if (audio_i2s.Settings->sys.duplex == 0 && audio_i2s.Settings->sys.rx == 1) {
    i2s_del_channel(_tx_chan);
    _i2s_on = false;
  }
  _tx_enabled = false;
  return true;
}

bool TasmotaAudioOutputI2S::ConsumeSample(int16_t sample[2]) {
  return consumeSamples(sample, 1);
}

int32_t TasmotaAudioOutputI2S::consumeSamples(int16_t *samples, size_t count) {
  if (!_tx_enabled) { return -1; }
  if (count == 0) { return 0; }
  if (count > 128) { count = 128; }

  int16_t ms[count*2];
  for (int32_t i = 0; i < count; i++) {
    int16_t left = samples[i*2 + LEFTCHANNEL];
    int16_t right = samples[i*2 + RIGHTCHANNEL];

    if (this->_tx_channels == 1) {    // if mono, average the two samples
      // Average the two samples and overwrite
      int32_t ttl = left + right;
      left = right = (ttl>>1) & 0xffff;
    }

    if (bps == 8) {
      left = (((int16_t)(left & 0xff)) - 128) << 8;
      right = (((int16_t)(right & 0xff)) - 128) << 8;
    }

    if (_tx_mode == INTERNAL_DAC) {
      left = Amplify(left) + 0x8000;
      right = Amplify(right) + 0x8000;
    } else {
      left = Amplify(left);
      right = Amplify(right);
    }

    ms[i*2 + LEFTCHANNEL] = left;
    ms[i*2 + RIGHTCHANNEL] = right;
  }

  size_t i2s_bytes_written;
  esp_err_t err = i2s_channel_write(_tx_chan, ms, sizeof(ms), &i2s_bytes_written, 0);
  if (err && err != ESP_ERR_TIMEOUT) {
    AddLog(LOG_LEVEL_INFO, "I2S: Could not write samples (count=%i): %i", count, err);
    return -1;
  }
  return i2s_bytes_written;
}

// Initialize I2S channel
// return `true` if successful, `false` otherwise
bool TasmotaAudioOutputI2S::startI2SChannel(void) {
  gpio_num_t _DIN = I2S_GPIO_UNUSED;          // no input pin by default

  // default dma_desc_num = 6 (DMA buffers), dma_frame_num = 240 (frames per buffer)
  i2s_chan_config_t tx_chan_cfg = I2S_CHANNEL_DEFAULT_CONFIG(I2S_NUM_AUTO, I2S_ROLE_MASTER);

  if (audio_i2s.Settings->sys.duplex == 1) {
    _DIN = (gpio_num_t)_gpio_din;
    i2s_new_channel(&tx_chan_cfg, &_tx_chan, &audio_i2s.rx_handle);
  } else{
    i2s_new_channel(&tx_chan_cfg, &_tx_chan, NULL);
  }

  // by default we configure for MSB 2 slots `I2S_STD_MSB_SLOT_DEFAULT_CONFIG`
  i2s_std_config_t tx_std_cfg = {
    .clk_cfg  = I2S_STD_CLK_DEFAULT_CONFIG(hertz),
    .slot_cfg = I2S_STD_MSB_SLOT_DEFAULT_CONFIG((i2s_data_bit_width_t)bps, (i2s_slot_mode_t)channels),
    .gpio_cfg = {
      .mclk = _gpio_mclk,
      .bclk = _gpio_bclk,
      .ws   = _gpio_ws,
      .dout = _gpio_dout,
      .din  =  _DIN,
      .invert_flags = {
        .mclk_inv = _gpio_mclk_inv,
        .bclk_inv = _gpio_bclk_inv,
        .ws_inv   = _gpio_ws_inv,
      }
    }
  };

  // change configuration if we are using PCM or PHILIPS
  if (audio_i2s.Settings->tx.slot_config == 1) {    // PCM
    tx_std_cfg.slot_cfg = I2S_STD_PCM_SLOT_DEFAULT_CONFIG((i2s_data_bit_width_t)bps, (i2s_slot_mode_t)channels);
  } else if (audio_i2s.Settings->tx.slot_config == 2) { // PHILIPS
    tx_std_cfg.slot_cfg = I2S_STD_PHILIPS_SLOT_DEFAULT_CONFIG((i2s_data_bit_width_t)bps, (i2s_slot_mode_t)channels);
  }

  _i2s_on = (i2s_channel_init_std_mode(_tx_chan, &tx_std_cfg) == 0);
  AddLog(LOG_LEVEL_DEBUG, "I2S: TX channel with %i bit width on %i channels initialized ok=%i", bps, channels, _i2s_on);

  if (audio_i2s.Settings->sys.duplex == 1) {
    i2s_channel_init_std_mode(audio_i2s.rx_handle, &tx_std_cfg);
    AddLog(LOG_LEVEL_DEBUG, "I2S: RX channel added in full duplex mode");
  }
  return _i2s_on;
}

int TasmotaAudioOutputI2S::updateClockConfig(void) {
  i2s_channel_disable(_tx_chan);
  i2s_std_clk_config_t clk_cfg = I2S_STD_CLK_DEFAULT_CONFIG(hertz);
#ifdef SOC_I2S_SUPPORTS_APLL
  if (audio_i2s.Settings->tx.apll == 1) {
    clk_cfg.clk_src = I2S_CLK_SRC_APLL;
  }
#endif
  int result = i2s_channel_reconfig_std_clock(_tx_chan, &clk_cfg );
  if (_tx_enabled) { i2s_channel_enable(_tx_chan); }
  AddLog(LOG_LEVEL_DEBUG, "I2S: Updating clock config, result=%i", result);
  return result;
}

#endif // USE_I2S_AUDIO
#endif // defined(ESP32) && ESP_IDF_VERSION_MAJOR >= 5
