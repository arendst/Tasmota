/*
  TasmotaLEDPusherRMT.cpp - Implementation to push Leds via SPI channel

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

#if TASMOTALED_HARDWARE_SPI
#include <rom/gpio.h>

//**************************************************************************************************************
// enable AddLog support within a C++ library
extern void AddLog(uint32_t loglevel, PGM_P formatP, ...);
enum LoggingLevels {LOG_LEVEL_NONE, LOG_LEVEL_ERROR, LOG_LEVEL_INFO, LOG_LEVEL_DEBUG, LOG_LEVEL_DEBUG_MORE};
//**************************************************************************************************************

/*******************************************************************************************\
 * Implementation for TasmotaLEDPusherSPI
 * 
\*******************************************************************************************/

#define LED_STRIP_SPI_DEFAULT_RESOLUTION (25 * 100 * 1000) // 2.5MHz resolution
#define LED_STRIP_SPI_DEFAULT_TRANS_QUEUE_SIZE 4

#define SPI_BYTES_PER_COLOR_BYTE 3
#define SPI_BITS_PER_COLOR_BYTE (SPI_BYTES_PER_COLOR_BYTE * 8)

static void __led_strip_spi_bit(uint8_t data, uint8_t *buf)
{
  // Each color of 1 bit is represented by 3 bits of SPI, low_level:100 ,high_level:110
  // So a color byte occupies 3 bytes of SPI.
  buf[0] = (data & BIT(5) ? BIT(1) | BIT(0) : BIT(1)) | (data & BIT(6) ? BIT(4) | BIT(3) : BIT(4)) | (data & BIT(7) ? BIT(7) | BIT(6) : BIT(7));
  buf[1] = (BIT(0)) | (data & BIT(3) ? BIT(3) | BIT(2) : BIT(3)) | (data & BIT(4) ? BIT(6) | BIT(5) : BIT(6));
  buf[2] = (data & BIT(0) ? BIT(2) | BIT(1) : BIT(2)) | (data & BIT(1) ? BIT(5) | BIT(4) : BIT(5)) | (data & BIT(2) ? BIT(7) : 0x00);
}

esp_err_t led_strip_spi_refresh(led_strip_spi_obj * spi_strip)
{
  spi_strip->tx_conf.length = spi_strip->strip_len * spi_strip->bytes_per_pixel * SPI_BITS_PER_COLOR_BYTE;
  spi_strip->tx_conf.tx_buffer = spi_strip->pixel_buf;
  spi_strip->tx_conf.rx_buffer = NULL;
  spi_device_transmit(spi_strip->spi_device, &spi_strip->tx_conf);
  return ESP_OK;
}

void led_strip_transmit_buffer(led_strip_spi_obj * spi_strip, uint8_t * buffer_rgbw) {
  // Timing for 512 pixels (extreme test)
  // Copying to buffer: 418 us
  // sending pixels: 16.2 ms
  uint8_t * buf = buffer_rgbw;
  uint8_t * pix_buf = spi_strip->pixel_buf;
  for (int i = 0; i < spi_strip->strip_len; i++) {
    // LED_PIXEL_FORMAT_GRB takes 72bits(9bytes)
    __led_strip_spi_bit(*buf++, pix_buf);     pix_buf += SPI_BYTES_PER_COLOR_BYTE;
    __led_strip_spi_bit(*buf++, pix_buf);     pix_buf += SPI_BYTES_PER_COLOR_BYTE;
    __led_strip_spi_bit(*buf++, pix_buf);     pix_buf += SPI_BYTES_PER_COLOR_BYTE;
    if (spi_strip->bytes_per_pixel > 3) {
      __led_strip_spi_bit(*buf++, pix_buf); pix_buf += SPI_BYTES_PER_COLOR_BYTE;
    }
  }
  /* Refresh the strip to send data */
  led_strip_spi_refresh(spi_strip);
}


TasmotaLEDPusherSPI::~TasmotaLEDPusherSPI() {
  if (_spi_strip.spi_device) {
    spi_bus_remove_device(_spi_strip.spi_device);
  }
  if (_spi_strip.spi_host) {
    spi_bus_free(_spi_strip.spi_host);
  }

  if (_pin >= 0) {
    gpio_matrix_out(_pin, 0x100, false, false);
    pinMode(_pin, INPUT);
    _pin = -1;
  }
}

TasmotaLEDPusherSPI::TasmotaLEDPusherSPI(int8_t pin) : _pin(pin) {
  spi_host_device_t spi_host = SPI2_HOST;
  spi_bus_config_t spi_bus_cfg = {
      .mosi_io_num = _pin,
      //Only use MOSI to generate the signal, set -1 when other pins are not used.
      .miso_io_num = -1,
      .sclk_io_num = -1,
      .quadwp_io_num = -1,
      .quadhd_io_num = -1,
      .max_transfer_sz = _pixel_count * _pixel_size * SPI_BYTES_PER_COLOR_BYTE,
  };
  _err = spi_bus_initialize(spi_host, &spi_bus_cfg, _with_dma ? SPI_DMA_CH_AUTO : SPI_DMA_DISABLED);
  if (_err == ESP_OK) {
    _spi_strip.spi_host = spi_host;     // confirmed working, so keep it's value to free it later
    _initialized = true;
  }
}

bool TasmotaLEDPusherSPI::Begin(uint16_t pixel_count, uint16_t pixel_size, const TasmotaLED_Timing * led_timing) {
  if (!_initialized) {
    return false;
  }
  TasmotaLEDPusher::Begin(pixel_count, pixel_size, led_timing);
  _spi_strip.bytes_per_pixel = _pixel_size;
  _spi_strip.strip_len = _pixel_count;

  uint32_t mem_caps = MALLOC_CAP_DEFAULT;
  // spi_clock_source_t clk_src = SPI_CLK_SRC_DEFAULT;
  spi_device_interface_config_t spi_dev_cfg;
  int clock_resolution_khz = 0;

  if (_with_dma) {  // TODO
    // DMA buffer must be placed in internal SRAM
    mem_caps |= MALLOC_CAP_INTERNAL | MALLOC_CAP_DMA;
  }
  _spi_strip.pixel_buf = (uint8_t *)heap_caps_calloc(1, _pixel_count * _pixel_size * SPI_BYTES_PER_COLOR_BYTE, mem_caps);
  if (_spi_strip.pixel_buf == nullptr) {
    AddLog(LOG_LEVEL_INFO, PSTR("LED: Error no mem for spi strip"));
    goto err;
  }

  spi_dev_cfg = {
    .command_bits = 0,
    .address_bits = 0,
    .dummy_bits = 0,
    .mode = 0,
    //set -1 when CS is not used
    .clock_source = SPI_CLK_SRC_DEFAULT,    // clk_src,
    .clock_speed_hz = LED_STRIP_SPI_DEFAULT_RESOLUTION,
    .spics_io_num = -1,
    .queue_size = LED_STRIP_SPI_DEFAULT_TRANS_QUEUE_SIZE,
  };
  _err = spi_bus_add_device(_spi_strip.spi_host, &spi_dev_cfg, &_spi_strip.spi_device);
  if (_err != ESP_OK) {
    // AddLog(LOG_LEVEL_INFO, "LED: Error failed to add spi device");
    goto err;
  }

  _err = spi_device_get_actual_freq(_spi_strip.spi_device, &clock_resolution_khz);
  if (_err != ESP_OK) {
    // AddLog(LOG_LEVEL_INFO, "LED: Error failed to get spi frequency");
    goto err;
  }
  // TODO: ideally we should decide the SPI_BYTES_PER_COLOR_BYTE by the real clock resolution
  // But now, let's fixed the resolution, the downside is, we don't support a clock source whose frequency is not multiple of LED_STRIP_SPI_DEFAULT_RESOLUTION
  if (clock_resolution_khz != LED_STRIP_SPI_DEFAULT_RESOLUTION / 1000) {
    // AddLog(LOG_LEVEL_INFO, "LED: Error unsupported clock resolution: %dKHz", clock_resolution_khz);
    goto err;
  }
  return true;
err:
  if (_spi_strip.spi_device) {
    spi_bus_remove_device(_spi_strip.spi_device);
  }
  if (_spi_strip.spi_host) {
    spi_bus_free(_spi_strip.spi_host);
  }
  _initialized = false;
  return false;
}

bool TasmotaLEDPusherSPI::CanShow(void) {
  return _initialized; // TODO
}

bool TasmotaLEDPusherSPI::Push(uint8_t *buf) {
  if (!_initialized) { return false; }
  if (CanShow()) {
    led_strip_transmit_buffer(&_spi_strip, buf);
  }
  return true;
}

#endif // TASMOTALED_HARDWARE_SPI
#endif // ESP32
