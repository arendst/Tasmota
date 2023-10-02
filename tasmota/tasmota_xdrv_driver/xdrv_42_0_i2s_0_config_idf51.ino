/*
  xdrv_42_0_i2s_0_config_idf51.ino - Simplified Audio library, general configuration

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
#include "soc/soc_caps.h"

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

// I2S communication mode
enum : int8_t {
  I2S_MODE_STD = 0,       // I2S mode standard
  I2S_MODE_PDM = 1,       // I2S mode PDM
  I2S_MODE_TDM = 2,       // I2S mode TDM
  I2S_MODE_DAC = 3,       // Using internal DAC - only available on ESP32
};

// I2S slot mask (left, right, both)
enum : int8_t {
  I2S_SLOT_NOCHANGE = 0,  // don't change default
  I2S_SLOT_LEFT = 1,      // left
  I2S_SLOT_RIGHT = 2,     // right
  I2S_SLOT_BOTH = 3,      // both
};

// I2S slot configuration
enum : int8_t {
  I2S_SLOT_MSB = 0,       // MSB
  I2S_SLOT_PCM = 1,       // PCM
  I2S_SLOT_PHILIPS = 2,   // Philips
};

#define I2S_SLOTS   2

typedef struct{
  struct{
    uint8_t   version = 0;    // B00

    // runtime options, will be saved but ignored on setting read
    bool      duplex = 0;           // B01 - depends on GPIO setting and SOC caps, DIN and DOUT on same port in GPIO means -> try to use duplex if possible
    bool      tx = 0;               // B02 - depends on GPIO setting
    bool      rx = 0;               // B03 - depends on GPIO setting
    bool      exclusive = 0;        // B04 - depends on GPIO setting, if WS is shared between 2 ports, drivers needs to be reinstalled before being used (Yuck... but we don't have a choice)

    bool      mclk_inv[I2S_SLOTS] = {0};  // B05-06 - invert mclk
    bool      bclk_inv[I2S_SLOTS] = {0};  // B07-08 - invert bclk
    bool      ws_inv[I2S_SLOTS] = {0};    // B09-0A - invert ws
    uint8_t   spare[5];              // B0B-0F
  } sys;
  struct {
    uint32_t sample_rate = 16000;   // B00-03
    uint8_t  gain = 10;             // B04 - was `volume`
    uint8_t  mode = I2S_MODE_STD;   // B05 - I2S mode standard, PDM, TDM, DAC
    uint8_t  slot_mask = I2S_SLOT_NOCHANGE;// B06 - slot mask 
    uint8_t  slot_config = I2S_SLOT_MSB;// B07 - slot configuration MSB = 0, PCM = 1, PHILIPS = 2
    uint8_t  channels = 2;          // B08 - mono/stereo - 1 is added for both
    bool     apll = 1;              // B09 - will be ignored on unsupported SOC's
    // device specific
    uint8_t  mp3_preallocate = 0;   // B0A - preallocate MP3 buffer for mp3 playing
    uint8_t  codec = 0;             // B0B - S3 box only, unused for now
    uint8_t  spare[4];              // B0C-0F
  } tx;
  struct {
    uint32_t sample_rate = 16000;  // B00-03
    uint8_t  gain = 30;            // B04
    uint8_t  mode = I2S_MODE_PDM;  // B05 - I2S mode standard, PDM, TDM, DAC
    uint8_t  slot_mask = I2S_SLOT_NOCHANGE;// B06 - slot mask 
    uint8_t  slot_config = I2S_SLOT_MSB;// B07 - slot configuration MSB = 0, PCM = 1, PHILIPS = 2
    uint8_t  channels = 1;         // B08 - mono/stereo - 1 is added for both
    bool     apll = 1;              // B09 - will be ignored on unsupported SOC's
    // device specific
    uint8_t  codec = 0;             // B0A - S3 box only, unused for now
    uint8_t  mp3_preallocate = 0;   // B0B - will be ignored without PS-RAM
    uint8_t  spare[4];              // B0C-0F
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

class TasmotaI2S;

struct AUDIO_I2S_t {
  tI2SSettings *Settings;

  i2s_chan_handle_t rx_handle = nullptr;

  AudioGeneratorMP3 *mp3 = nullptr;
  AudioFileSourceFS *file = nullptr;

  TasmotaI2S *out = nullptr;        // instance used for I2S output, or `nullptr` if none
  TasmotaI2S *in = nullptr;         // instance used for I2S input, or `nullptr` if none (it can be the same as `out` in case of full duplex)

  AudioFileSourceID3 *id3 = nullptr;
  AudioGeneratorMP3 *decoder = NULL;
  void *mp3ram = NULL;

  TaskHandle_t mp3_task_handle;
  TaskHandle_t mic_task_handle;

  char mic_path[32];
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

#endif // USE_I2S_AUDIO
#endif // defined(ESP32) && ESP_IDF_VERSION_MAJOR >= 5
