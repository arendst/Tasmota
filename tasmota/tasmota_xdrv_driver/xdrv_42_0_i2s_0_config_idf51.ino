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
#define AUDIO_SETTINGS_VERSION  2

typedef struct{
  struct{
    uint8_t   version = AUDIO_SETTINGS_VERSION;    // B00

    // runtime options, will be saved but ignored on setting read
    bool      duplex = 0;           // B01 - depends on GPIO setting and SOC caps, DIN and DOUT on same port in GPIO means -> try to use duplex if possible
    bool      tx = 0;               // B02 - depends on GPIO setting
    bool      rx = 0;               // B03 - depends on GPIO setting
    bool      exclusive = 0;        // B04 - depends on GPIO setting, if WS is shared between 2 ports, drivers needs to be reinstalled before being used (Yuck... but we don't have a choice)

    bool      mclk_inv[I2S_SLOTS] = {0};  // B05-06 - invert mclk
    bool      bclk_inv[I2S_SLOTS] = {0};  // B07-08 - invert bclk
    bool      ws_inv[I2S_SLOTS] = {0};    // B09-0A - invert ws
    uint8_t   mp3_preallocate = 0;    // B0B - will be ignored without PS-RAM
    uint8_t   spare[4];              // B0C-0F
  } sys;
  struct {
    uint32_t sample_rate = 16000;   // B00-03
    uint8_t  gain = 10;             // B04 - was `volume`
    uint8_t  mode = I2S_MODE_STD;   // B05 - I2S mode standard, PDM, TDM, DAC
    uint8_t  slot_mask = BIT(0) | BIT(1);  // B06 - slot mask - both slots aka stereo by default
    uint8_t  slot_config = I2S_SLOT_MSB;// B07 - slot configuration MSB = 0, PCM = 1, PHILIPS = 2
    uint8_t  channels = 2;          // B08 - mono/stereo - 1 is added for both
    bool     apll = 1;              // B09 - will be ignored on unsupported SOC's
    bool     stream_enable = 0;     // B0A - enable streaming of MP3
    uint8_t  spare[5];              // B0B-0F
  } tx;
  struct {
    uint32_t sample_rate = 32000;  // B00-03 - 32000 is compatible with MP3 encoding
    uint16_t gain = 30 * 16;       // B04-05 - in Q12.4
#if SOC_I2S_SUPPORTS_PDM_RX
    uint8_t  mode = I2S_MODE_PDM;  // B06 - I2S mode standard, PDM, TDM, DAC
#else
    uint8_t  mode = I2S_MODE_STD;  // B06 - I2S mode standard, PDM, TDM, DAC
#endif
    uint8_t  slot_mask = BIT(0);   // B07 - slot mask  = left/right/both depended on mode, so BIT(0) maybe left or right
    uint8_t  slot_bit_width = I2S_SLOT_BIT_WIDTH_32BIT;  // B08 - auto equals data_bit_width - can differ from bits per sample e.g. INMP441
    uint8_t  channels = 1;         // B09 - mono/stereo - 1 is added for both
    // filters
    uint16_t dc_filter_alpha = 0b0111111011111111;  // B0A-B0B - DC_block filter = (1-2^(-7)) Q32:1.31, or `0` if disabled
    // low pass filter
    // See: https://en.wikipedia.org/wiki/Low-pass_filter#Simple_infinite_impulse_response_filter
    // For 3000Hz low pass filter, RC = 0.0000530786
    // dt = 1/16000 = 0.0000625
    // alpha = dt / (RC + dt) = 0.540757545
    // alpha = (b) 0.100010100110111 = 0x4537
    uint16_t lowpass_alpha = 0b0100010100110111;    // B0C-B0D - lowpass filter = 3000Hz for 16000Hz sample rate
    bool     apll = 1;              // B0E - will be ignored on unsupported SOC's
    uint8_t  ws_width = 32;         // B0F - WS signal width - can differ from bits per sample - default 32 bits for INMP441
    bool     ws_pol = false;        // B10 - WS signal polarity
    bool     bit_shift = true;      // B11 - enable bit shift in Philips mode 
    bool     left_align = true;     // B12 - left alignment
    bool     big_endian = false;    // B13 - big endian
    bool     bit_order_lsb = false; // B14 - lsb first
    uint16_t dma_frame_num = 576;   // B015/16 - DMA buffer size in samples, 576 should be okay up to ~32000 bps
    uint8_t  dma_desc_num = 3;      // B17 - number of DMA buffers, maybe increased with smaller buffers
    uint8_t  spare[3];              // B018-20 - padding
  } rx;
} tI2SSettings;

static_assert(sizeof(tI2SSettings) == 60, "tI2SSettings Size is not correct");

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

  // i2s_chan_handle_t rx_handle = nullptr;
  TasmotaI2S *out = nullptr;        // instance used for I2S output, or `nullptr` if none
  TasmotaI2S *in = nullptr;         // instance used for I2S input, or `nullptr` if none (it can be the same as `out` in case of full duplex)
} audio_i2s;

#endif // USE_I2S_AUDIO
#endif // defined(ESP32) && ESP_IDF_VERSION_MAJOR >= 5
