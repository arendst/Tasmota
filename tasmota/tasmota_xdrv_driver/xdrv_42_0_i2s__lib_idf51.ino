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

#undef AUDIO_PWR_ON
#undef AUDIO_PWR_OFF
#define AUDIO_PWR_ON    I2SAudioPower(true);
#define AUDIO_PWR_OFF   I2SAudioPower(false);

#define USE_I2S_SAY
#define USE_I2S_SAY_TIME
#define USE_I2S_RTTTL

/*********************************************************************************************\
 * Driver Settings in memory
\*********************************************************************************************/

typedef struct{
  struct{
    uint32_t version : 8 = 0;

    // runtime options, will be saved but ignored on setting read
    uint32_t duplex : 1 = 0; // depends on GPIO setting and SOC caps, DIN and DOUT on same port in GPIO means -> try to use duplex if possible
    uint32_t tx : 1 = 0; // depends on GPIO setting
    uint32_t rx : 1 = 0; // depends on GPIO setting
  } sys;
  struct {
    uint32_t mode : 2 = 0;        // bit 0+1 STD = 0, PDM = 1, TDM = 2
    uint32_t apll : 1 = 1;        // bit 2 - will be ignored on unsupported SOC's
    uint32_t mono : 1 = 0;        // bit 3  0 = stereo, 1 = mono
    uint32_t codec : 1 = 0;       // bit 4 - S3 box only, unused for now
    uint32_t webradio : 1 = 1;    // bit 5 - allocate buffer for webradio
    uint32_t spare01 : 1 = 1;      // bit 6 - request duplex, means RX and TX on 1 slot
    uint32_t lsbJustified : 1;    // bit 7 - allow supporting LSBJ chips, e.g. TM8211/PT8211
    uint32_t volume : 8 = 10;     // bit 8-15
    uint32_t spare02 : 16;        // bit 16-31
  } tx;
  struct {
    struct{
    uint16_t sample_rate = 32000;
    uint8_t gain = 30;
    uint8_t mode = 0;   //STD = 0, PDM = 1, TDM = 2

    uint8_t slot_mask : 2 = 1; // left = 1 /right = 2 /both = 3
    uint8_t slot_mode : 1 = 0; // mono/stereo - 1 is added for both
    uint8_t codec : 1 = 0; // unused for now
    uint8_t mp3_encoder : 1 = 1; // will be ignored without PS-RAM
    };
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
  AudioFileSourceFS *file;

  TasmotaAudioOutputI2S *out;

  AudioFileSourceID3 *id3;
  AudioGeneratorMP3 *decoder = NULL;
  void *mp3ram = NULL;

  // Webradio
  AudioFileSourceICYStream *ifile = NULL;
  AudioFileSourceBuffer *buff = NULL;
  char wr_title[64];
  void *preallocateBuffer = NULL;
  void *preallocateCodec = NULL;
  uint32_t retryms = 0;


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

extern FS *ufsp;
extern FS *ffsp;

const int preallocateBufferSize = 16*1024;
const int preallocateCodecSize = 29192; // MP3 codec max mem needed
//const int preallocateCodecSize = 85332; // AAC+SBR codec max mem needed

enum : int { EXTERNAL_I2S = 0, INTERNAL_DAC = 1, INTERNAL_PDM = 2 };

void sayTime(int hour, int minutes);
void Cmndwav2mp3(void);
void Cmd_Time(void);

void Rtttl(char *buffer);
void CmndI2SRtttl(void);

/*********************************************************************************************\
 * Class for outputting sound as endpoint for ESP8266Audio library
\*********************************************************************************************/

class TasmotaAudioOutputI2S : public AudioOutput
{
public:

  // Constructor takes no parameter, everything is configured from template and config file
  TasmotaAudioOutputI2S() {
    hertz = 16000;
    i2sOn = false;
    bps = I2S_DATA_BIT_WIDTH_16BIT;
    mono = audio_i2s.Settings->tx.mono;
    lsbJustified = audio_i2s.Settings->tx.lsbJustified;
    channels = mono ? I2S_SLOT_MODE_MONO : I2S_SLOT_MODE_STEREO;
    output_mode = EXTERNAL_I2S;
    tx_is_enabled = false;
  }

  ~TasmotaAudioOutputI2S() {
    if(i2sOn){
        this->stop();
        i2s_del_channel(tx_chan);
    }
  }

  // ------------------------------------------------------------------------------------------
  // Setters for configuration parameters
  // ------------------------------------------------------------------------------------------
  bool SetBitsPerSample(int bits) {
    if ( (bits != 16) && (bits != 8) ) { return false; }
    this->bps = bits;
    return true;
  }

  bool SetChannels(int channels) {
    if ((channels < 1) || (channels > 2)) { return false; }
    if (channels == (int)this->channels) { return true; }
    this->channels = channels;
    return true;
  }

  bool SetRate(int hz) {
    if (hz == (int)this->hertz) { return true; }
    this->hertz = hz;
    if(i2sOn){
      int result = updateClockConfig();
      AddLog(LOG_LEVEL_DEBUG,PSTR("I2S: SetRate: %i - %i"),hz, result);
    }
    return true;
  }

  bool SetPinout() {    // TODO is this method still useful?
    return this->startI2SChannel();
  }

  bool begin(void);
  bool stop(void);
  bool ConsumeSample(int16_t sample[2]);
  bool startI2SChannel(void);
  int updateClockConfig(void);

protected:
  enum : int { EXTERNAL_I2S = 0, INTERNAL_DAC = 1, INTERNAL_PDM = 2 };
  int output_mode;
  bool i2sOn;
  bool mono;
  bool lsbJustified;

  i2s_chan_handle_t tx_chan;
  bool tx_is_enabled;

};


// ------------------------------------------------------------------------------------------
// Methods
// ------------------------------------------------------------------------------------------
bool TasmotaAudioOutputI2S::begin() {
  if (tx_is_enabled) { return true; }
  if (!i2sOn) {
    if (audio_i2s.Settings->sys.duplex == 0 && audio_i2s.Settings->sys.rx == 1) {
      this->startI2SChannel();
    }
  }
  int result = i2s_channel_enable(tx_chan);
  if (result != 0){
      AddLog(LOG_LEVEL_INFO, "I2S: Could not enable i2s_channel: %i", result);
      return false;
  }
  tx_is_enabled = true;
  return true;
}

bool TasmotaAudioOutputI2S::stop() {
  i2s_channel_disable(tx_chan);
  if(audio_i2s.Settings->sys.duplex == 0 && audio_i2s.Settings->sys.rx == 1){
    i2s_del_channel(tx_chan);
    i2sOn = false;
  }
  tx_is_enabled = false;
  return true;
}

bool TasmotaAudioOutputI2S::ConsumeSample(int16_t sample[2]) {
  if (!tx_is_enabled) { return false; }

  int16_t ms[2];
  ms[0] = sample[0];
  ms[1] = sample[1];
  MakeSampleStereo16(ms);

  if (this->mono) {
    // Average the two samples and overwrite
    int32_t ttl = ms[LEFTCHANNEL] + ms[RIGHTCHANNEL];
    ms[LEFTCHANNEL] = ms[RIGHTCHANNEL] = (ttl>>1) & 0xffff;
  }
  uint32_t s32;
  if (output_mode == INTERNAL_DAC) {
    int16_t l = Amplify(ms[LEFTCHANNEL]) + 0x8000;
    int16_t r = Amplify(ms[RIGHTCHANNEL]) + 0x8000;
    s32 = (r << 16) | (l & 0xffff);
  } else {
    s32 = ((Amplify(ms[RIGHTCHANNEL])) << 16) | (Amplify(ms[LEFTCHANNEL]) & 0xffff);
  }

  size_t i2s_bytes_written;
  i2s_channel_write(tx_chan, (const void*)&s32, sizeof(uint32_t), &i2s_bytes_written, 0);
  return i2s_bytes_written;
}

bool TasmotaAudioOutputI2S::startI2SChannel(void) {
  gpio_num_t _DIN = I2S_GPIO_UNUSED;

  i2s_chan_config_t tx_chan_cfg = I2S_CHANNEL_DEFAULT_CONFIG(I2S_NUM_AUTO, I2S_ROLE_MASTER);
  if (audio_i2s.Settings->sys.duplex == 1) {
    _DIN = (gpio_num_t)Pin(GPIO_I2S_DIN);
    i2s_new_channel(&tx_chan_cfg, &tx_chan, &audio_i2s.rx_handle);
  } else{
    i2s_new_channel(&tx_chan_cfg, &tx_chan, NULL);
  }

  i2s_std_config_t tx_std_cfg = {
      .clk_cfg  = I2S_STD_CLK_DEFAULT_CONFIG(hertz),
      .slot_cfg = I2S_STD_MSB_SLOT_DEFAULT_CONFIG((i2s_data_bit_width_t)bps, (i2s_slot_mode_t)channels),
      .gpio_cfg = {
          .mclk = (gpio_num_t)Pin(GPIO_I2S_MCLK),
          .bclk = (gpio_num_t)Pin(GPIO_I2S_BCLK),
          .ws   = (gpio_num_t)Pin(GPIO_I2S_WS),
          .dout = (gpio_num_t)Pin(GPIO_I2S_DOUT),
          .din  =  _DIN,
          .invert_flags = {
              .mclk_inv = false,
              .bclk_inv = false,
              .ws_inv   = false,
          },
      },
    };
  i2sOn = (i2s_channel_init_std_mode(tx_chan, &tx_std_cfg) == 0);
  AddLog(LOG_LEVEL_DEBUG, "I2S: TX channel with %i bit width on %i channels initialized", bps, channels);
  if (audio_i2s.Settings->sys.duplex == 1) {
    i2s_channel_init_std_mode(audio_i2s.rx_handle, &tx_std_cfg);
    AddLog(LOG_LEVEL_DEBUG, "I2S: RX channel added in full duplex mode");
  }
  return i2sOn;
}

int TasmotaAudioOutputI2S::updateClockConfig(void) {
  i2s_channel_disable(tx_chan);
  i2s_std_clk_config_t clk_cfg = I2S_STD_CLK_DEFAULT_CONFIG(hertz);
#ifdef SOC_I2S_SUPPORTS_APLL
  if(audio_i2s.Settings->tx.apll == 1){
    clk_cfg.clk_src = I2S_CLK_SRC_APLL;
  }
#endif
  int result = i2s_channel_reconfig_std_clock(tx_chan, &clk_cfg );
  if(tx_is_enabled) i2s_channel_enable(tx_chan);
  return result;
}

#endif  // USE_I2S_AUDIO
#endif //ESP_IDF_VERSION_MAJOR >= 5
