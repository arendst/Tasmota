/*-------------------------------------------------------------------------
NeoPixel library helper functions for Esp32.

Adaption of Espressif's component library code by Christian Baars

Written by Michael C. Miller.

I invest time and resources providing this open source code,
please support me by dontating (see https://github.com/Makuna/NeoPixelBus)

-------------------------------------------------------------------------
This file is not yet part of the Makuna/NeoPixelBus library.

NeoPixelBus is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as
published by the Free Software Foundation, either version 3 of
the License, or (at your option) any later version.

NeoPixelBus is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with NeoPixel.  If not, see
<http://www.gnu.org/licenses/>.
-------------------------------------------------------------------------*/

#pragma once

#if defined(ARDUINO_ARCH_ESP32)

#if (defined(CONFIG_IDF_TARGET_ESP32C3) || defined(CONFIG_IDF_TARGET_ESP32C2) || defined(CONFIG_IDF_TARGET_ESP32C5) || defined(CONFIG_IDF_TARGET_ESP32C6)) && !defined(HSPI_HOST)
// HSPI_HOST depreciated in C3
#define HSPI_HOST   SPI2_HOST
#endif

#include <Arduino.h>

extern void AddLog(uint32_t loglevel, PGM_P formatP, ...); // TODO: Remove all Addlogs

extern "C"
{
#include <rom/gpio.h>
#include "esp_rom_gpio.h"
#include "driver/spi_master.h"
#include "esp_check.h"
}

#define LED_STRIP_SPI_DEFAULT_RESOLUTION (25 * 100 * 1000) // 2.5MHz resolution
#define LED_STRIP_SPI_DEFAULT_TRANS_QUEUE_SIZE 4

#define SPI_BYTES_PER_COLOR_BYTE 3
#define SPI_BITS_PER_COLOR_BYTE (SPI_BYTES_PER_COLOR_BYTE * 8)

static const char *TAG = "led_strip_spi"; // TODO: Remove all TAG log stuff

typedef enum {
    LED_MODEL_WS2812, /*!< LED strip model: WS2812 */
    LED_MODEL_SK6812, /*!< LED strip model: SK6812 */
    LED_MODEL_INVALID /*!< Invalid LED strip model */
} led_model_t;

typedef enum {
    LED_PIXEL_FORMAT_GRB,    /*!< Pixel format: GRB */
    LED_PIXEL_FORMAT_GRBW,   /*!< Pixel format: GRBW */
    LED_PIXEL_FORMAT_INVALID /*!< Invalid pixel format */
} led_pixel_format_t;

typedef struct {
    spi_host_device_t spi_host;
    spi_device_handle_t spi_device;
    uint32_t strip_len;
    uint8_t bytes_per_pixel;
    uint8_t pixel_buf[];
} led_strip_spi_obj;

/**
 * @brief LED Strip Configuration
 */
typedef struct {
    int strip_gpio_num;      /*!< GPIO number that used by LED strip */
    uint32_t max_leds;       /*!< Maximum LEDs in a single strip */
    led_pixel_format_t led_pixel_format; /*!< LED pixel format */
    led_model_t led_model;   /*!< LED model */

    struct {
        uint32_t invert_out: 1; /*!< Invert output signal */
    } flags;
} led_strip_config_t;

typedef struct {
    spi_clock_source_t clk_src; /*!< SPI clock source */
    spi_host_device_t spi_bus;  /*!< SPI bus ID. Which buses are available depends on the specific chip */
    struct {
        uint32_t with_dma: 1;   /*!< Use DMA to transmit data */
    } flags;
} led_strip_spi_config_t;


class NeoEsp32SpiSpeed
{
public:


protected:

// end of deprecated section

};

class NeoEsp32SpiSpeedBase : public NeoEsp32SpiSpeed
{
public:

};

class NeoEsp32SpiInvertedSpeedBase : public NeoEsp32SpiSpeed
{
public:

};

class NeoEsp32SpiSpeedWs2811 : public NeoEsp32SpiSpeedBase
{
public:

};

class NeoEsp32SpiSpeedWs2812x : public NeoEsp32SpiSpeedBase
{
public:
    static const uint8_t bytes_per_pixel = 3;

};

class NeoEsp32SpiSpeedSk6812 : public NeoEsp32SpiSpeedBase
{
public:
    static const uint8_t bytes_per_pixel = 4;

};

class NeoEsp32SpiSpeed400Kbps : public NeoEsp32SpiSpeedBase
{
public:

};

class NeoEsp32SpiSpeedApa106 : public NeoEsp32SpiSpeedBase
{
public:

};

class NeoEsp32SpiSpeedTx1812 : public NeoEsp32SpiSpeedBase
{
public:

};

class NeoEsp32SpiInvertedSpeedWs2811 : public NeoEsp32SpiInvertedSpeedBase
{
public:

};

class NeoEsp32SpiInvertedSpeedWs2812x : public NeoEsp32SpiInvertedSpeedBase
{
public:

};

class NeoEsp32SpiInvertedSpeedSk6812 : public NeoEsp32SpiInvertedSpeedBase
{
public:

};

class NeoEsp32SpiInvertedSpeed800Kbps : public NeoEsp32SpiInvertedSpeedBase
{
public:

};

class NeoEsp32SpiInvertedSpeed400Kbps : public NeoEsp32SpiInvertedSpeedBase
{
public:

};

class NeoEsp32SpiInvertedSpeedApa106 : public NeoEsp32SpiInvertedSpeedBase
{
public:

};

class NeoEsp32SpiInvertedSpeedTx1812 : public NeoEsp32SpiInvertedSpeedBase
{
public:

};

class NeoEsp32SpiChannel
{
public:

};


template<typename T_SPEED, typename T_CHANNEL> class NeoEsp32SpiMethodBase
{
public:
    typedef NeoNoSettings SettingsObject;

    NeoEsp32SpiMethodBase(uint8_t pin, uint16_t pixelCount, size_t elementSize, size_t settingsSize)  :
        _sizeData(pixelCount * elementSize + settingsSize),
        // _pixelCount(pixelCount),
        _pin(pin)
    {
        _pixelCount = pixelCount;
        construct();
    }

    NeoEsp32SpiMethodBase(uint8_t pin, uint16_t pixelCount, size_t elementSize, size_t settingsSize, NeoBusChannel channel) :
        _sizeData(pixelCount* elementSize + settingsSize),
        // _pixelCount(pixelCount),
        _pin(pin)
        // _channel(channel) // TODO: Refactor this somehow
    {
        _pixelCount = pixelCount;
        construct();
    }

    ~NeoEsp32SpiMethodBase()
    {
        // wait until the last send finishes before destructing everything
        // arbitrary time out of 10 seconds

        gpio_matrix_out(_pin, 0x100, false, false);
        pinMode(_pin, INPUT);

        spi_bus_remove_device(_spi_strip->spi_device);
        spi_bus_free(_spi_strip->spi_host);
        free(_spi_strip);

        free(_dataEditing);
        free(_dataSending);
    }


    bool IsReadyToUpdate() const
    {
        return true; // TODO
    }

    void Initialize()
    {  
        esp_err_t ret = ESP_OK;
        uint8_t bytes_per_pixel = T_SPEED::bytes_per_pixel;
        uint32_t mem_caps = MALLOC_CAP_DEFAULT;
        spi_clock_source_t clk_src = SPI_CLK_SRC_DEFAULT;
        spi_bus_config_t spi_bus_cfg;
        spi_device_interface_config_t spi_dev_cfg;
        bool with_dma = true; /// TODO: pass value or compute based on pixelcount
        int clock_resolution_khz = 0;

        // ESP_GOTO_ON_FALSE(led_config && spi_config && ret_strip, ESP_ERR_INVALID_ARG, err, TAG, "invalid argument");
        // ESP_GOTO_ON_FALSE(led_config->led_pixel_format < LED_PIXEL_FORMAT_INVALID, ESP_ERR_INVALID_ARG, err, TAG, "invalid led_pixel_format");

        // if ( LED_PIXEL_FORMAT_GRB == LED_PIXEL_FORMAT_GRBW) { // TODO
        //     bytes_per_pixel = 4;
        // } else if (LED_PIXEL_FORMAT_GRB == LED_PIXEL_FORMAT_GRB) {
        //     bytes_per_pixel = 3;
        // } else {
        //     assert(false);
        // }

        if (with_dma) {  // TODO
            // DMA buffer must be placed in internal SRAM
            mem_caps |= MALLOC_CAP_INTERNAL | MALLOC_CAP_DMA;
        }
        _spi_strip = (led_strip_spi_obj *)heap_caps_calloc(1, sizeof(led_strip_spi_obj) + _pixelCount * bytes_per_pixel * SPI_BYTES_PER_COLOR_BYTE, mem_caps);

        ESP_GOTO_ON_FALSE(_spi_strip, ESP_ERR_NO_MEM, err, TAG, "no mem for spi strip");

        _spi_strip->spi_host = SPI2_HOST;

        // for backward compatibility, if the user does not set the clk_src, use the default value
        // if (clk_src) {
        //     clk_src = spi_config->clk_src;
        // }

        spi_bus_cfg = {
            .mosi_io_num = _pin,
            //Only use MOSI to generate the signal, set -1 when other pins are not used.
            .miso_io_num = -1,
            .sclk_io_num = -1,
            .quadwp_io_num = -1,
            .quadhd_io_num = -1,
            .max_transfer_sz = _pixelCount * bytes_per_pixel * SPI_BYTES_PER_COLOR_BYTE,
        };
        ESP_GOTO_ON_ERROR(spi_bus_initialize(_spi_strip->spi_host, &spi_bus_cfg, with_dma ? SPI_DMA_CH_AUTO : SPI_DMA_DISABLED), err, TAG, "create SPI bus failed");

        // if (led_config->flags.invert_out == true) {
        //     esp_rom_gpio_connect_out_signal(_pin, spi_periph_signal[_spi_strip->spi_host].spid_out, true, false);
        // }

        spi_dev_cfg = {
            .command_bits = 0,
            .address_bits = 0,
            .dummy_bits = 0,
            .mode = 0,
            //set -1 when CS is not used
            .clock_source = clk_src,
            .clock_speed_hz = LED_STRIP_SPI_DEFAULT_RESOLUTION,
            .spics_io_num = -1,
            .queue_size = LED_STRIP_SPI_DEFAULT_TRANS_QUEUE_SIZE,
        };

        ESP_GOTO_ON_ERROR(spi_bus_add_device(_spi_strip->spi_host, &spi_dev_cfg, &_spi_strip->spi_device), err, TAG, "Failed to add spi device");

        spi_device_get_actual_freq(_spi_strip->spi_device, &clock_resolution_khz);
        // TODO: ideally we should decide the SPI_BYTES_PER_COLOR_BYTE by the real clock resolution
        // But now, let's fixed the resolution, the downside is, we don't support a clock source whose frequency is not multiple of LED_STRIP_SPI_DEFAULT_RESOLUTION
        ESP_GOTO_ON_FALSE(clock_resolution_khz == LED_STRIP_SPI_DEFAULT_RESOLUTION / 1000, ESP_ERR_NOT_SUPPORTED, err,
                        TAG, "unsupported clock resolution:%dKHz", clock_resolution_khz);

        _spi_strip->bytes_per_pixel = bytes_per_pixel;
        _spi_strip->strip_len = _pixelCount;
        // if (ret) {
        //     AddLog(2,"SPI: initialized with error code: %u on pin: %u",ret, _pin);
        // }
        return;
    err:
        if (_spi_strip) {
            if (_spi_strip->spi_device) {
                spi_bus_remove_device(_spi_strip->spi_device);
            }
            if (_spi_strip->spi_host) {
                spi_bus_free(_spi_strip->spi_host);
            }
            free(_spi_strip);
        }
        // if (ret) {
        //     AddLog(2,"SPI-Error:initialized with error code: %u on pin: %u",ret, _pin);
        // }
        return;
    }

    void Update(bool maintainBufferConsistency)
    {
        // AddLog(2,"..");
        // wait for not actively sending data
        // this will time out at 10 seconds, an arbitrarily long period of time
        // and do nothing if this happens

        // if READY
        {
            // AddLog(2,"__ %u", _sizeData);
            // now start the SPI transmit with the editing buffer before we swap
            led_strip_transmit_RGB_buffer(_dataEditing, _sizeData,  T_SPEED::bytes_per_pixel);

            if (maintainBufferConsistency)
            {
                // copy editing to sending,
                // this maintains the contract that "colors present before will
                // be the same after", otherwise GetPixelColor will be inconsistent
                memcpy(_dataSending, _dataEditing, _sizeData);
            }

            // swap so the user can modify without affecting the async operation
            std::swap(_dataSending, _dataEditing);
        }
    }

    uint8_t* getData() const
    {
        return _dataEditing;
    };

    size_t getDataSize() const
    {
        return _sizeData;
    }

    void applySettings(const SettingsObject& settings)
    {
    }

private:
    const size_t _sizeData;      // Size of '_data*' buffers
    int16_t _pixelCount;
    const uint8_t _pin;           // output pin number
    T_CHANNEL _channel;             // not really used here

    led_strip_spi_obj *_spi_strip;


    // Holds data stream which include LED color values and other settings as needed
    uint8_t*  _dataEditing;   // exposed for get and set
    uint8_t*  _dataSending;   // used for async send using RMT - TODO: Check if this useful for SPI


    void construct()
    {
        _dataEditing = static_cast<uint8_t*>(malloc(_sizeData));
        // data cleared later in Begin()

        _dataSending = static_cast<uint8_t*>(malloc(_sizeData));
        // no need to initialize it, it gets overwritten on every send
    }

    // please make sure to zero-initialize the buf before calling this function

    void __led_strip_spi_bit(uint8_t data, uint8_t *buf)
    {
        // Each color of 1 bit is represented by 3 bits of SPI, low_level:100 ,high_level:110
        // So a color byte occupies 3 bytes of SPI.
        *(buf + 2) |= data & BIT(0) ? BIT(2) | BIT(1) : BIT(2);
        *(buf + 2) |= data & BIT(1) ? BIT(5) | BIT(4) : BIT(5);
        *(buf + 2) |= data & BIT(2) ? BIT(7) : 0x00;
        *(buf + 1) |= BIT(0);
        *(buf + 1) |= data & BIT(3) ? BIT(3) | BIT(2) : BIT(3);
        *(buf + 1) |= data & BIT(4) ? BIT(6) | BIT(5) : BIT(6);
        *(buf + 0) |= data & BIT(5) ? BIT(1) | BIT(0) : BIT(1);
        *(buf + 0) |= data & BIT(6) ? BIT(4) | BIT(3) : BIT(4);
        *(buf + 0) |= data & BIT(7) ? BIT(7) | BIT(6) : BIT(7);
    }

    esp_err_t led_strip_spi_set_pixel(uint32_t index, uint32_t red, uint32_t green, uint32_t blue)
    {
        if(index >= _spi_strip->strip_len) return ESP_FAIL;
        // LED_PIXEL_FORMAT_GRB takes 72bits(9bytes)
        uint32_t start = index * _spi_strip->bytes_per_pixel * SPI_BYTES_PER_COLOR_BYTE;
        memset(_spi_strip->pixel_buf + start, 0, _spi_strip->bytes_per_pixel * SPI_BYTES_PER_COLOR_BYTE);
        __led_strip_spi_bit(green, &_spi_strip->pixel_buf[start]);
        __led_strip_spi_bit(red, &_spi_strip->pixel_buf[start + SPI_BYTES_PER_COLOR_BYTE]);
        __led_strip_spi_bit(blue, &_spi_strip->pixel_buf[start + SPI_BYTES_PER_COLOR_BYTE * 2]);
        if (_spi_strip->bytes_per_pixel > 3) {
            __led_strip_spi_bit(0, &_spi_strip->pixel_buf[start + SPI_BYTES_PER_COLOR_BYTE * 3]);
        }
        return ESP_OK;
    }

    esp_err_t led_strip_spi_set_pixel_rgbw(uint32_t index, uint32_t red, uint32_t green, uint32_t blue, uint32_t white)
    {
        // LED_PIXEL_FORMAT_GRBW takes 96bits(12bytes)
        uint32_t start = index * _spi_strip->bytes_per_pixel * SPI_BYTES_PER_COLOR_BYTE;
        // SK6812 component order is GRBW
        memset(_spi_strip->pixel_buf + start, 0, _spi_strip->bytes_per_pixel * SPI_BYTES_PER_COLOR_BYTE);
        __led_strip_spi_bit(green, &_spi_strip->pixel_buf[start]);
        __led_strip_spi_bit(red, &_spi_strip->pixel_buf[start + SPI_BYTES_PER_COLOR_BYTE]);
        __led_strip_spi_bit(blue, &_spi_strip->pixel_buf[start + SPI_BYTES_PER_COLOR_BYTE * 2]);
        __led_strip_spi_bit(white, &_spi_strip->pixel_buf[start + SPI_BYTES_PER_COLOR_BYTE * 3]);

        return ESP_OK;
    }

    esp_err_t led_strip_spi_clear()
    {
        //Write zero to turn off all leds
        memset(_spi_strip->pixel_buf, 0, _spi_strip->strip_len * _spi_strip->bytes_per_pixel * SPI_BYTES_PER_COLOR_BYTE);
        uint8_t *buf = _spi_strip->pixel_buf;
        for (int index = 0; index < _spi_strip->strip_len * _spi_strip->bytes_per_pixel; index++) {
            __led_strip_spi_bit(0, buf);
            buf += SPI_BYTES_PER_COLOR_BYTE;
        }
        return led_strip_spi_refresh();
    }

    esp_err_t led_strip_spi_refresh()
    {
        spi_transaction_t tx_conf;
        memset(&tx_conf, 0, sizeof(tx_conf));

        tx_conf.length = _spi_strip->strip_len * _spi_strip->bytes_per_pixel * SPI_BITS_PER_COLOR_BYTE;
        tx_conf.tx_buffer = _spi_strip->pixel_buf;
        tx_conf.rx_buffer = NULL;
        spi_device_transmit(_spi_strip->spi_device, &tx_conf); // TODO -check
        return ESP_OK;
    }

    void led_strip_transmit_RGB_buffer(uint8_t * buffer, size_t size, uint8_t bytes_per_pixel)
    {
        for (int i = 0; i < size; i+=bytes_per_pixel) {
            led_strip_spi_set_pixel(i/bytes_per_pixel, buffer[i+1], buffer[i], buffer[i+2]); //GRB -> RGB
        }
        /* Refresh the strip to send data */
        led_strip_spi_refresh();
    }
};

// normal
typedef NeoEsp32SpiMethodBase<NeoEsp32SpiSpeedWs2812x, NeoEsp32SpiChannel> NeoEsp32SpiN800KbpsMethod;
typedef NeoEsp32SpiMethodBase<NeoEsp32SpiSpeedSk6812, NeoEsp32SpiChannel> NeoEsp32SpiNSk6812Method;


// SPI channel method is the default method for Esp32C2
#ifdef CONFIG_IDF_TARGET_ESP32C2
typedef NeoEsp32SpiSpeedWs2812x NeoWs2813Method;
typedef NeoEsp32SpiSpeedWs2812x NeoWs2812xMethod;
typedef NeoEsp32SpiSpeedSk6812 NeoSk6812Method;

#endif //CONFIG_IDF_TARGET_ESP32C2

#endif
