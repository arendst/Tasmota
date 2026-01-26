/*
  TasmotaLEDPusherRMT.cpp - Implementation to push Leds via RMT channel

  Copyright (C) 2024  Stephan Hadinger

  This library is free software: you can redistribute it and/or modify
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

#include "TasmotaLEDPusher.h"
#include "TasmotaLED.h"

#if TASMOTALED_HARDWARE_RMT
#include <rom/gpio.h>
#include <esp_check.h>

//**************************************************************************************************************
// enable AddLog support within a C++ library
extern void AddLog(uint32_t loglevel, PGM_P formatP, ...);
enum LoggingLevels {LOG_LEVEL_NONE, LOG_LEVEL_ERROR, LOG_LEVEL_INFO, LOG_LEVEL_DEBUG, LOG_LEVEL_DEBUG_MORE};
//**************************************************************************************************************

/*******************************************************************************************\
 * Implementation for TasmotaLEDPusherRMT
 * 
 * Code mostly copied from Tasmota patch to NeoPixelBus applied to support esp-idf 5.x
 * itself inspired from esp-idf example for RMT encoder from
 * https://github.com/espressif/esp-idf/tree/v5.3.1/examples/peripherals/rmt/ir_nec_transceiver
\*******************************************************************************************/
#define RMT_LED_STRIP_RESOLUTION_HZ 40000000 // 40MHz resolution, steps of 25 nanoseconds

// structure used to pass arguments to `rmt_new_led_strip_encoder`
// currently only the encoder resolution in Hz
typedef struct {
    uint32_t resolution; /*!< Encoder resolution, in Hz */
} led_strip_encoder_config_t;

// structure used to store all the necessary information for the RMT encoder
typedef struct {
    rmt_encoder_t base;
    rmt_encoder_t *bytes_encoder;
    rmt_encoder_t *copy_encoder;
    int32_t state;
    rmt_symbol_word_t reset_code;
} rmt_led_strip_encoder_t;

static IRAM_ATTR size_t rmt_encode_led_strip(rmt_encoder_t *encoder, rmt_channel_handle_t channel, const void *primary_data, size_t data_size, rmt_encode_state_t *ret_state)
{
  rmt_led_strip_encoder_t *led_encoder = __containerof(encoder, rmt_led_strip_encoder_t, base);
  rmt_encoder_handle_t bytes_encoder = led_encoder->bytes_encoder;
  rmt_encoder_handle_t copy_encoder = led_encoder->copy_encoder;
  rmt_encode_state_t session_state = RMT_ENCODING_RESET;
  rmt_encode_state_t state = RMT_ENCODING_RESET;
  size_t encoded_symbols = 0;
  switch (led_encoder->state) {
  case 0: // send RGB data
    encoded_symbols += bytes_encoder->encode(bytes_encoder, channel, primary_data, data_size, &session_state);
    if (session_state & RMT_ENCODING_COMPLETE) {
        led_encoder->state = 1; // switch to next state when current encoding session finished
    }
    if (session_state & RMT_ENCODING_MEM_FULL) {
        state = static_cast<rmt_encode_state_t>(static_cast<uint8_t>(state) | static_cast<uint8_t>(RMT_ENCODING_MEM_FULL));
        goto out; // yield if there's no free space for encoding artifacts
    }
  // fall-through
  case 1: // send reset code
    encoded_symbols += copy_encoder->encode(copy_encoder, channel, &led_encoder->reset_code, sizeof(led_encoder->reset_code), &session_state);
    if (session_state & RMT_ENCODING_COMPLETE) {
        led_encoder->state = RMT_ENCODING_RESET; // back to the initial encoding session
        state = static_cast<rmt_encode_state_t>(static_cast<uint8_t>(state) | static_cast<uint8_t>(RMT_ENCODING_COMPLETE));
    }
    if (session_state & RMT_ENCODING_MEM_FULL) {
        state = static_cast<rmt_encode_state_t>(static_cast<uint8_t>(state) | static_cast<uint8_t>(RMT_ENCODING_MEM_FULL));
        goto out; // yield if there's no free space for encoding artifacts
    }
  }
out:
  *ret_state = state;
  return encoded_symbols;
}

static esp_err_t rmt_del_led_strip_encoder(rmt_encoder_t *encoder) {
  rmt_led_strip_encoder_t *led_encoder = __containerof(encoder, rmt_led_strip_encoder_t, base);
  rmt_del_encoder(led_encoder->bytes_encoder);
  rmt_del_encoder(led_encoder->copy_encoder);
  delete led_encoder;
  return ESP_OK;
}

static esp_err_t rmt_led_strip_encoder_reset(rmt_encoder_t *encoder) {
  rmt_led_strip_encoder_t *led_encoder = __containerof(encoder, rmt_led_strip_encoder_t, base);
  rmt_encoder_reset(led_encoder->bytes_encoder);
  rmt_encoder_reset(led_encoder->copy_encoder);
  led_encoder->state = RMT_ENCODING_RESET;
  return ESP_OK;
}

static esp_err_t rmt_new_led_strip_encoder(const led_strip_encoder_config_t *config, rmt_encoder_handle_t *ret_encoder, rmt_symbol_word_t bit0,  rmt_symbol_word_t bit1, rmt_symbol_word_t reset_code) {
  static const char* TAG = "TASMOTA_RMT";
  esp_err_t ret = ESP_OK;
  rmt_led_strip_encoder_t *led_encoder = NULL;
  rmt_bytes_encoder_config_t bytes_encoder_config;
  rmt_copy_encoder_config_t copy_encoder_config = {};

  ESP_GOTO_ON_FALSE(config && ret_encoder, ESP_ERR_INVALID_ARG, err, TAG, "invalid argument");
  led_encoder = new rmt_led_strip_encoder_t();
  ESP_GOTO_ON_FALSE(led_encoder, ESP_ERR_NO_MEM, err, TAG, "no mem for led strip encoder");
  led_encoder->base.encode = rmt_encode_led_strip;
  led_encoder->base.del = rmt_del_led_strip_encoder;
  led_encoder->base.reset = rmt_led_strip_encoder_reset;
  led_encoder->reset_code = reset_code;

  bytes_encoder_config.bit0 = bit0;
  bytes_encoder_config.bit1 = bit1;
  bytes_encoder_config.flags.msb_first = 1; // WS2812 transfer bit order: G7...G0R7...R0B7...B0 - TODO: more checks

  ESP_GOTO_ON_ERROR(rmt_new_bytes_encoder(&bytes_encoder_config, &led_encoder->bytes_encoder), err, TAG, "create bytes encoder failed");
  ESP_GOTO_ON_ERROR(rmt_new_copy_encoder(&copy_encoder_config, &led_encoder->copy_encoder), err, TAG, "create copy encoder failed");

  *ret_encoder = &led_encoder->base;
  return ret;
err:
  AddLog(LOG_LEVEL_INFO, "RMT: could not init led encoder");
  if (led_encoder) {
    if (led_encoder->bytes_encoder) { rmt_del_encoder(led_encoder->bytes_encoder); }
    if (led_encoder->copy_encoder) { rmt_del_encoder(led_encoder->copy_encoder); }
    delete led_encoder;
  }
  return ret;
}

TasmotaLEDPusherRMT::~TasmotaLEDPusherRMT() {
  if (_channel) {
    rmt_tx_wait_all_done(_channel, 10000 / portTICK_PERIOD_MS);
    rmt_del_channel(_channel);
    _channel = nullptr;
  }

  if (_pin >= 0) {
    gpio_matrix_out(_pin, 0x100, false, false);
    pinMode(_pin, INPUT);
    _pin = -1;
  }
}

TasmotaLEDPusherRMT::TasmotaLEDPusherRMT(int8_t pin) : _pin(pin) {
  esp_err_t ret = ESP_OK;
  rmt_tx_channel_config_t config = {};
  config.clk_src = RMT_CLK_SRC_DEFAULT;
  config.gpio_num = static_cast<gpio_num_t>(_pin);
  config.mem_block_symbols = 192;         // memory block size, 64 * 4 = 256 Bytes
  config.resolution_hz = RMT_LED_STRIP_RESOLUTION_HZ; // 40 MHz tick resolution, i.e., 1 tick = 0.025 µs or 25 ns
  config.trans_queue_depth = 4;           // set the number of transactions that can pend in the background
  config.flags.invert_out = false;        // do not invert output signal
  config.flags.with_dma = false;          // do not need DMA backend

  _err = rmt_new_tx_channel(&config, &_channel);
  if (_err == ESP_OK) {
    _initialized = true;
  }
}

bool TasmotaLEDPusherRMT::Begin(uint16_t pixel_count, uint16_t pixel_size, const TasmotaLED_Timing * led_timing) {
  if (!_initialized) { return false; }
  TasmotaLEDPusher::Begin(pixel_count, pixel_size, led_timing);
  led_strip_encoder_config_t encoder_config = {
    .resolution = RMT_LED_STRIP_RESOLUTION_HZ,
  };

  _tx_config.loop_count = 0;  // no loop

  rmt_symbol_word_t RmtBit0 = {
    .duration0 = (uint16_t) (led_timing->T0H * (RMT_LED_STRIP_RESOLUTION_HZ / 1000000) / 1000),
    .level0 = 1,
    .duration1 = (uint16_t) (led_timing->T0L * (RMT_LED_STRIP_RESOLUTION_HZ / 1000000) / 1000),
    .level1 = 0,
  };
  rmt_symbol_word_t RmtBit1 = {
    .duration0 = (uint16_t) (led_timing->T1H * (RMT_LED_STRIP_RESOLUTION_HZ / 1000000) / 1000),
    .level0 = 1,
    .duration1 = (uint16_t) (led_timing->T1L * (RMT_LED_STRIP_RESOLUTION_HZ / 1000000) / 1000),
    .level1 = 0,
  };
  rmt_symbol_word_t RmtReset = {
    .duration0 = (uint16_t) (led_timing->Reset * (RMT_LED_STRIP_RESOLUTION_HZ / 1000000) / 1000),
    .level0 = 0,
    .duration1 = 50 * (RMT_LED_STRIP_RESOLUTION_HZ / 1000000) / 1000,
    .level1 = 1,
  };
  // AddLog(LOG_LEVEL_INFO, "RMT: RmtBit0 0x%08X RmtBit1 0x%08X RmtReset 0x%08X", RmtBit0.val, RmtBit1.val, RmtReset.val);
  _err = rmt_new_led_strip_encoder(&encoder_config, &_led_encoder, RmtBit0, RmtBit1, RmtReset);
  if (_err != ESP_OK) {
    // AddLog(LOG_LEVEL_INFO, "RMT: cannot initialize led strip encoder err=%i", ret);
    return false;
  }
  _err = rmt_enable(_channel);
  if (_err != ESP_OK) {
    // AddLog(LOG_LEVEL_INFO, "RMT: cannot enable channel err=%i", ret);
    return false;
  }
  return true;
}

bool TasmotaLEDPusherRMT::SetPixelCount(uint16_t pixel_count) {
  if (!_initialized) { return false; }
  if (pixel_count > 0 && _pixel_count != pixel_count) {
    _pixel_count = pixel_count;
    return true;
  }
  return true;
}

bool TasmotaLEDPusherRMT::CanShow(void) {
  if (_channel && _initialized) {
    return (ESP_OK == rmt_tx_wait_all_done(_channel, 0));
  } else {
    return false;
  }
}

bool TasmotaLEDPusherRMT::Push(uint8_t *buf) {
  if (!_initialized) { return false; }

  // wait for not actively sending data
  // this will time out at 1 second, an arbitrarily long period of time
  // and do nothing if this happens
  esp_err_t ret = rmt_tx_wait_all_done(_channel, 1000 / portTICK_PERIOD_MS);
  if (ESP_OK == ret) {
    // now start the RMT transmit with the editing buffer before we swap
    ret = rmt_transmit(_channel, _led_encoder, buf, _pixel_count * _pixel_size, &_tx_config);
    if (ESP_OK != ret) {
      AddLog(LOG_LEVEL_DEBUG, "RMT: cannot transmit err=%i", ret);
      return false;
    }
  }
  return true;
}

#endif // TASMOTALED_HARDWARE_RMT
#endif // ESP32
