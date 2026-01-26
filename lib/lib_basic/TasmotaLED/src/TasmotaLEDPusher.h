/*
  TasmotaLEDPusher.h - Abstract class for Leds pusher (RMT, SPI, I2S...)

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

#ifndef __TASMOTALEDPUSHER_H__
#define __TASMOTALEDPUSHER_H__

#include <Arduino.h>

// Below are flags to enable of disable each hardware support: RMT, I2S, SPI
// By default, only enable RMT support, and SPI is used as fallback if no protocol works
//
// Use de defines below:
//    #define TASMOTALED_HARDWARE_RMT 0/1
//    #define TASMOTALED_HARDWARE_I2S 0/1
//    #define TASMOTALED_HARDWARE_SPI 0/1
//
#ifndef TASMOTALED_HARDWARE_RMT
  #define TASMOTALED_HARDWARE_RMT 1
#endif

#ifndef TASMOTALED_HARDWARE_I2S
  #define TASMOTALED_HARDWARE_I2S 0
#endif

#ifndef TASMOTALED_HARDWARE_SPI
  #define TASMOTALED_HARDWARE_SPI 0
#endif

// Disable any hardware if not supported by the SOC
#if TASMOTALED_HARDWARE_RMT && !defined(SOC_RMT_SUPPORTED)
  #undef TASMOTALED_HARDWARE_RMT
  #define TASMOTALED_HARDWARE_RMT 0
#endif

#if TASMOTALED_HARDWARE_I2S && !defined(SOC_I2S_SUPPORTED)
  #undef TASMOTALED_HARDWARE_I2S
  #define TASMOTALED_HARDWARE_I2S 0
#endif

#if TASMOTALED_HARDWARE_SPI && !defined(SOC_GPSPI_SUPPORTED)
  #undef TASMOTALED_HARDWARE_SPI
  #define TASMOTALED_HARDWARE_SPI 0
#endif

// if no protocol is defined, use SPI as fallback
#if !TASMOTALED_HARDWARE_RMT && !TASMOTALED_HARDWARE_I2S && !TASMOTALED_HARDWARE_SPI
  #undef TASMOTALED_HARDWARE_SPI
  #define TASMOTALED_HARDWARE_SPI 1
#endif

// Timing structure for LEDS - in nanoseconds
// It is passed by TasmotaLed to the pushers
typedef struct TasmotaLED_Timing {
  uint16_t    T0H, T0L, T1H, T1L;
  uint32_t    Reset;
} TasmotaLED_Timing;

/*******************************************************************************************\
 * class TasmotaLEDPusher
 * 
 * This is an virtual abstract class for Leds pusher (RMT, SPI, I2S...)
 *
 * Below are interfaces for current implementations
\*******************************************************************************************/
class TasmotaLEDPusher {
public:
  TasmotaLEDPusher() : _initialized(false), _err(ESP_OK), _pixel_count(0), _pixel_size(0), _led_timing(nullptr) {};
  virtual ~TasmotaLEDPusher() {};

  bool Initialized(void) const { return _initialized; }
  esp_err_t Error(void) const { return _err; }
  virtual bool Begin(uint16_t pixel_count, uint16_t pixel_size, const TasmotaLED_Timing * led_timing) {
    _pixel_count = pixel_count;
    _pixel_size = pixel_size;
    _led_timing = led_timing;
    return true;
  }
  virtual bool Push(uint8_t *buf) = 0;
  virtual bool CanShow(void) = 0;
  virtual bool SetPixelCount(uint16_t pixel_count) = 0;

  static uint32_t ResolveHardware(uint32_t hw);    // convert to the appropriate hardware acceleration based on capacities of the SOC
  static TasmotaLEDPusher * Create(uint32_t hw, int8_t gpio);   // create instance for the provided type, or nullptr if failed
  
protected:
  bool _initialized;                              // did the hardware got correctly initialized
  esp_err_t _err;
  uint16_t _pixel_count;
  uint16_t _pixel_size;
  const TasmotaLED_Timing * _led_timing;
};

/*******************************************************************************************\
 * class TasmotaLEDPusherRMT
 * 
 * Implementation based on RMT driver
\*******************************************************************************************/
#if TASMOTALED_HARDWARE_RMT
#include "driver/rmt_tx.h"
class TasmotaLEDPusherRMT : public TasmotaLEDPusher {
public:
  TasmotaLEDPusherRMT(int8_t pin);
  ~TasmotaLEDPusherRMT();

  bool Begin(uint16_t pixel_count, uint16_t pixel_size, const TasmotaLED_Timing * led_timing) override;
  bool SetPixelCount(uint16_t pixel_count) override;

  bool Push(uint8_t *buf) override;
  bool CanShow(void) override;
protected:
  int8_t _pin;
  rmt_transmit_config_t _tx_config = {};
  rmt_channel_handle_t _channel = nullptr;;
  rmt_encoder_handle_t _led_encoder = nullptr;
};
#endif  // TASMOTALED_HARDWARE_RMT

/*******************************************************************************************\
 * class TasmotaLEDPusherSPI
 * 
 * Implementation based on SPI driver, mandatory for C2
\*******************************************************************************************/
#if TASMOTALED_HARDWARE_SPI
#include <driver/spi_master.h>

typedef struct led_strip_spi_obj_t {
    uint8_t * pixel_buf;
    uint16_t strip_len;
    uint8_t bytes_per_pixel;
    spi_host_device_t spi_host;
    spi_device_handle_t spi_device;
    spi_transaction_t tx_conf;              // transaction in process if any
} led_strip_spi_obj;

class TasmotaLEDPusherSPI : public TasmotaLEDPusher {
public:
  TasmotaLEDPusherSPI(int8_t pin);
  ~TasmotaLEDPusherSPI();

  bool Begin(uint16_t pixel_count, uint16_t pixel_size, const TasmotaLED_Timing * led_timing) override;
  bool SetPixelCount(uint16_t pixel_count) override;

  bool Push(uint8_t *buf) override;
  bool CanShow(void) override;
  
protected:
  int8_t _pin;
  struct led_strip_spi_obj_t _spi_strip = {};;
  const bool _with_dma = true;
};
#endif  // TASMOTALED_HARDWARE_SPI

#endif  // __TASMOTALEDPUSHER_H__
