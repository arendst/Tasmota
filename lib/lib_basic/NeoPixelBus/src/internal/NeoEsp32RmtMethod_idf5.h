/*-------------------------------------------------------------------------
NeoPixel library helper functions for Esp32.

A BIG thanks to Andreas Merkle for the investigation and implementation of
a workaround to the GCC bug that drops method attributes from template methods 

Written by Michael C. Miller.

I invest time and resources providing this open source code,
please support me by dontating (see https://github.com/Makuna/NeoPixelBus)

-------------------------------------------------------------------------
This file is part of the Makuna/NeoPixelBus library.

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

#if defined(ARDUINO_ARCH_ESP32) && !defined(CONFIG_IDF_TARGET_ESP32C2)

/*  General Reference documentation for the APIs used in this implementation
LOW LEVEL:  (what is actually used)
DOCS: https://docs.espressif.com/projects/esp-idf/en/latest/api-reference/peripherals/rmt.html
EXAMPLE: https://github.com/espressif/esp-idf/blob/826ff7186ae07dc81e960a8ea09ebfc5304bfb3b/examples/peripherals/rmt_tx/main/rmt_tx_main.c

HIGHER LEVEL:
NO TRANSLATE SUPPORT so this was not used
NOTE: https://github.com/espressif/arduino-esp32/commit/50d142950d229b8fabca9b749dc4a5f2533bc426
Esp32-hal-rmt.h
Esp32-hal-rmt.c
*/

#include <Arduino.h>

extern void AddLog(uint32_t loglevel, PGM_P formatP, ...);

extern "C"
{
#include <rom/gpio.h>
#include "driver/rmt_tx.h"
#include "driver/rmt_encoder.h"
#include "esp_check.h"
}

#define RMT_LED_STRIP_RESOLUTION_HZ 40000000 // 40MHz resolution - setting of the "old" driver

typedef struct {
    uint32_t resolution; /*!< Encoder resolution, in Hz */
} led_strip_encoder_config_t;

typedef struct {
    rmt_encoder_t base;
    rmt_encoder_t *bytes_encoder;
    rmt_encoder_t *copy_encoder;
    int state;
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
            // static_cast<AnimalFlags>(static_cast<int>(a) | static_cast<int>(b));
            state = static_cast<rmt_encode_state_t>(static_cast<uint8_t>(state) | static_cast<uint8_t>(RMT_ENCODING_MEM_FULL));
            goto out; // yield if there's no free space for encoding artifacts
        }
    // fall-through
    case 1: // send reset code
        encoded_symbols += copy_encoder->encode(copy_encoder, channel, &led_encoder->reset_code,
                                                sizeof(led_encoder->reset_code), &session_state);
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

static esp_err_t rmt_del_led_strip_encoder(rmt_encoder_t *encoder)
{
    rmt_led_strip_encoder_t *led_encoder = __containerof(encoder, rmt_led_strip_encoder_t, base);
    rmt_del_encoder(led_encoder->bytes_encoder);
    rmt_del_encoder(led_encoder->copy_encoder);
    delete led_encoder;
    return ESP_OK;
}

static esp_err_t rmt_led_strip_encoder_reset(rmt_encoder_t *encoder)
{
    rmt_led_strip_encoder_t *led_encoder = __containerof(encoder, rmt_led_strip_encoder_t, base);
    rmt_encoder_reset(led_encoder->bytes_encoder);
    rmt_encoder_reset(led_encoder->copy_encoder);
    led_encoder->state = RMT_ENCODING_RESET;
    return ESP_OK;
}

esp_err_t rmt_new_led_strip_encoder(const led_strip_encoder_config_t *config, rmt_encoder_handle_t *ret_encoder, uint32_t bit0,  uint32_t bit1)
{
    const char* TAG = "TEST_RMT"; //TODO: Remove later
    esp_err_t ret = ESP_OK;
    rmt_led_strip_encoder_t *led_encoder = NULL;
    uint32_t reset_ticks = config->resolution / 1000000 * 50 / 2; // reset code duration defaults to 50us
    rmt_bytes_encoder_config_t bytes_encoder_config;
    rmt_copy_encoder_config_t copy_encoder_config = {};
    rmt_symbol_word_t reset_code_config;


    ESP_GOTO_ON_FALSE(config && ret_encoder, ESP_ERR_INVALID_ARG, err, TAG, "invalid argument");
    led_encoder = new rmt_led_strip_encoder_t();
    ESP_GOTO_ON_FALSE(led_encoder, ESP_ERR_NO_MEM, err, TAG, "no mem for led strip encoder");
    led_encoder->base.encode = rmt_encode_led_strip;
    led_encoder->base.del = rmt_del_led_strip_encoder;
    led_encoder->base.reset = rmt_led_strip_encoder_reset;

    bytes_encoder_config.bit0.val = bit0;
    bytes_encoder_config.bit1.val = bit1;

    bytes_encoder_config.flags.msb_first = 1; // WS2812 transfer bit order: G7...G0R7...R0B7...B0 - TODO: more checks

    ESP_GOTO_ON_ERROR(rmt_new_bytes_encoder(&bytes_encoder_config, &led_encoder->bytes_encoder), err, TAG, "create bytes encoder failed");
    ESP_GOTO_ON_ERROR(rmt_new_copy_encoder(&copy_encoder_config, &led_encoder->copy_encoder), err, TAG, "create copy encoder failed");

    reset_code_config.level0 = 0;
    reset_code_config.duration0 = reset_ticks;
    reset_code_config.level1 = 0;
    reset_code_config.duration1 = reset_ticks;
    led_encoder->reset_code = reset_code_config;
    *ret_encoder = &led_encoder->base;
    return ret;
err:
    // AddLog(2,"RMT:could not init led decoder");
    if (led_encoder) {
        if (led_encoder->bytes_encoder) {
            rmt_del_encoder(led_encoder->bytes_encoder);
        }
        if (led_encoder->copy_encoder) {
            rmt_del_encoder(led_encoder->copy_encoder);
        }
        delete led_encoder;
    }
    return ret;
}

#define NEOPIXELBUS_RMT_INT_FLAGS (ESP_INTR_FLAG_LOWMED)

class NeoEsp32RmtSpeed
{
public:
// next section is probably not needed anymore for IDF 5.1
    // ClkDiv of 2 provides for good resolution and plenty of reset resolution; but
    // a ClkDiv of 1 will provide enough space for the longest reset and does show
    // little better pulse accuracy
    const static uint8_t RmtClockDivider = 2; 

    inline constexpr static uint32_t FromNs(uint32_t ns)
    {
        return ns / NsPerRmtTick;
    }

protected:
    const static uint32_t RmtCpu = 80000000L; // 80 mhz RMT clock
    const static uint32_t NsPerSecond = 1000000000L;
    const static uint32_t RmtTicksPerSecond = (RmtCpu / RmtClockDivider);
    const static uint32_t NsPerRmtTick = (NsPerSecond / RmtTicksPerSecond); // about 25 
// end of deprecated section

};

class NeoEsp32RmtSpeedBase : public NeoEsp32RmtSpeed
{
public:
    // this is used rather than the rmt_symbol_word_t as you can't correctly initialize
    // it as a static constexpr within the template
    inline constexpr static uint32_t Item32Val(uint16_t nsHigh, uint16_t nsLow)
    {
        return (FromNs(nsLow) << 16) | (1 << 15) | (FromNs(nsHigh));
    }
};

class NeoEsp32RmtInvertedSpeedBase : public NeoEsp32RmtSpeed
{
public:
    // this is used rather than the rmt_symbol_word_t as you can't correctly initialize
    // it as a static constexpr within the template
    inline constexpr static uint32_t Item32Val(uint16_t nsHigh, uint16_t nsLow)
    {
        return (FromNs(nsLow) << 16) | (1 << 31) | (FromNs(nsHigh));
    }
};

class NeoEsp32RmtSpeedWs2811 : public NeoEsp32RmtSpeedBase
{
public:
    const static DRAM_ATTR uint32_t RmtBit0 = Item32Val(300, 950);  // TODO: DRAM_ATTR debatable everywhere
    const static DRAM_ATTR uint32_t RmtBit1 = Item32Val(900, 350); 
    const static DRAM_ATTR uint16_t RmtDurationReset = FromNs(300000); // 300us
};

class NeoEsp32RmtSpeedWs2812x : public NeoEsp32RmtSpeedBase
{
public:
    const static DRAM_ATTR uint32_t RmtBit0 = Item32Val(400, 850);
    const static DRAM_ATTR uint32_t RmtBit1 = Item32Val(800, 450);
    const static DRAM_ATTR uint16_t RmtDurationReset = FromNs(300000); // 300us
};

class NeoEsp32RmtSpeedSk6812 : public NeoEsp32RmtSpeedBase
{
public:
    const static DRAM_ATTR uint32_t RmtBit0 = Item32Val(400, 850); 
    const static DRAM_ATTR uint32_t RmtBit1 = Item32Val(800, 450); 
    const static DRAM_ATTR uint16_t RmtDurationReset = FromNs(80000); // 80us
};

// normal is inverted signal
class NeoEsp32RmtSpeedTm1814 : public NeoEsp32RmtInvertedSpeedBase
{
public:
    const static DRAM_ATTR uint32_t RmtBit0 = Item32Val(360, 890);
    const static DRAM_ATTR uint32_t RmtBit1 = Item32Val(720, 530);
    const static DRAM_ATTR uint16_t RmtDurationReset = FromNs(200000); // 200us
};

// normal is inverted signal
class NeoEsp32RmtSpeedTm1829 : public NeoEsp32RmtInvertedSpeedBase
{
public:
    const static DRAM_ATTR uint32_t RmtBit0 = Item32Val(300, 900);
    const static DRAM_ATTR uint32_t RmtBit1 = Item32Val(800, 400);
    const static DRAM_ATTR uint16_t RmtDurationReset = FromNs(200000); // 200us
};

// normal is inverted signal
class NeoEsp32RmtSpeedTm1914 : public NeoEsp32RmtInvertedSpeedBase
{
public:
    const static DRAM_ATTR uint32_t RmtBit0 = Item32Val(360, 890);
    const static DRAM_ATTR uint32_t RmtBit1 = Item32Val(720, 530);
    const static DRAM_ATTR uint16_t RmtDurationReset = FromNs(200000); // 200us
};

class NeoEsp32RmtSpeed800Kbps : public NeoEsp32RmtSpeedBase
{
public:
    const static DRAM_ATTR uint32_t RmtBit0 = Item32Val(400, 850); 
    const static DRAM_ATTR uint32_t RmtBit1 = Item32Val(800, 450); 
    const static DRAM_ATTR uint16_t RmtDurationReset = FromNs(50000); // 50us
};

class NeoEsp32RmtSpeed400Kbps : public NeoEsp32RmtSpeedBase
{
public:
    const static DRAM_ATTR uint32_t RmtBit0 = Item32Val(800, 1700); 
    const static DRAM_ATTR uint32_t RmtBit1 = Item32Val(1600, 900); 
    const static DRAM_ATTR uint16_t RmtDurationReset = FromNs(50000); // 50us
};

class NeoEsp32RmtSpeedApa106 : public NeoEsp32RmtSpeedBase
{
public:
    const static DRAM_ATTR uint32_t RmtBit0 = Item32Val(350, 1350);
    const static DRAM_ATTR uint32_t RmtBit1 = Item32Val(1350, 350);
    const static DRAM_ATTR uint16_t RmtDurationReset = FromNs(50000); // 50us
};

class NeoEsp32RmtSpeedTx1812 : public NeoEsp32RmtSpeedBase
{
public:
    const static DRAM_ATTR uint32_t RmtBit0 = Item32Val(300, 600); 
    const static DRAM_ATTR uint32_t RmtBit1 = Item32Val(600, 300); 
    const static DRAM_ATTR uint16_t RmtDurationReset = FromNs(80000); // 80us
};

class NeoEsp32RmtInvertedSpeedWs2811 : public NeoEsp32RmtInvertedSpeedBase
{
public:
    const static DRAM_ATTR uint32_t RmtBit0 = Item32Val(300, 950);
    const static DRAM_ATTR uint32_t RmtBit1 = Item32Val(900, 350);
    const static DRAM_ATTR uint16_t RmtDurationReset = FromNs(300000); // 300us
};

class NeoEsp32RmtInvertedSpeedWs2812x : public NeoEsp32RmtInvertedSpeedBase
{
public:
    const static DRAM_ATTR uint32_t RmtBit0 = Item32Val(400, 850);
    const static DRAM_ATTR uint32_t RmtBit1 = Item32Val(800, 450);
    const static DRAM_ATTR uint16_t RmtDurationReset = FromNs(300000); // 300us
};

class NeoEsp32RmtInvertedSpeedSk6812 : public NeoEsp32RmtInvertedSpeedBase
{
public:
    const static DRAM_ATTR uint32_t RmtBit0 = Item32Val(400, 850);
    const static DRAM_ATTR uint32_t RmtBit1 = Item32Val(800, 450);
    const static DRAM_ATTR uint16_t RmtDurationReset = FromNs(80000); // 80us
};

// normal is inverted signal
class NeoEsp32RmtInvertedSpeedTm1814 : public NeoEsp32RmtSpeedBase
{
public:
    const static DRAM_ATTR uint32_t RmtBit0 = Item32Val(360, 890);
    const static DRAM_ATTR uint32_t RmtBit1 = Item32Val(720, 530);
    const static DRAM_ATTR uint16_t RmtDurationReset = FromNs(200000); // 200us
};

// normal is inverted signal
class NeoEsp32RmtInvertedSpeedTm1829 : public NeoEsp32RmtSpeedBase
{
public:
    const static DRAM_ATTR uint32_t RmtBit0 = Item32Val(300, 900);
    const static DRAM_ATTR uint32_t RmtBit1 = Item32Val(800, 400);
    const static DRAM_ATTR uint16_t RmtDurationReset = FromNs(200000); // 200us
};

// normal is inverted signal
class NeoEsp32RmtInvertedSpeedTm1914 : public NeoEsp32RmtSpeedBase
{
public:
    const static DRAM_ATTR uint32_t RmtBit0 = Item32Val(360, 890);
    const static DRAM_ATTR uint32_t RmtBit1 = Item32Val(720, 530);
    const static DRAM_ATTR uint16_t RmtDurationReset = FromNs(200000); // 200us
};

class NeoEsp32RmtInvertedSpeed800Kbps : public NeoEsp32RmtInvertedSpeedBase
{
public:
    const static DRAM_ATTR uint32_t RmtBit0 = Item32Val(400, 850);
    const static DRAM_ATTR uint32_t RmtBit1 = Item32Val(800, 450);
    const static DRAM_ATTR uint16_t RmtDurationReset = FromNs(50000); // 50us
};

class NeoEsp32RmtInvertedSpeed400Kbps : public NeoEsp32RmtInvertedSpeedBase
{
public:
    const static DRAM_ATTR uint32_t RmtBit0 = Item32Val(800, 1700);
    const static DRAM_ATTR uint32_t RmtBit1 = Item32Val(1600, 900);
    const static DRAM_ATTR uint16_t RmtDurationReset = FromNs(50000); // 50us
};

class NeoEsp32RmtInvertedSpeedApa106 : public NeoEsp32RmtInvertedSpeedBase
{
public:
    const static DRAM_ATTR uint32_t RmtBit0 = Item32Val(350, 1350);
    const static DRAM_ATTR uint32_t RmtBit1 = Item32Val(1350, 350);
    const static DRAM_ATTR uint16_t RmtDurationReset = FromNs(50000); // 50us
};

class NeoEsp32RmtInvertedSpeedTx1812 : public NeoEsp32RmtInvertedSpeedBase
{
public:
    const static DRAM_ATTR uint32_t RmtBit0 = Item32Val(300, 600); 
    const static DRAM_ATTR uint32_t RmtBit1 = Item32Val(600, 300); 
    const static DRAM_ATTR uint16_t RmtDurationReset = FromNs(80000); // 80us
};

class NeoEsp32RmtChannel0
{
public:
    NeoEsp32RmtChannel0() {};
    rmt_channel_handle_t RmtChannelNumber = NULL;
};

class NeoEsp32RmtChannel1
{
public:
    NeoEsp32RmtChannel1() {};

    rmt_channel_handle_t RmtChannelNumber = NULL;
};

#if !defined(CONFIG_IDF_TARGET_ESP32C5) &&  !defined(CONFIG_IDF_TARGET_ESP32C6) // C5 & C6 only 2 RMT channels
class NeoEsp32RmtChannel2
{
public:
    NeoEsp32RmtChannel2() {};

    rmt_channel_handle_t RmtChannelNumber = NULL;
};

class NeoEsp32RmtChannel3
{
public:
    NeoEsp32RmtChannel3() {};

protected:
    rmt_channel_handle_t RmtChannelNumber = NULL;
};
#endif // !defined(CONFIG_IDF_TARGET_ESP32C6)
#if !defined(CONFIG_IDF_TARGET_ESP32S2) && !defined(CONFIG_IDF_TARGET_ESP32C3) &&  !defined(CONFIG_IDF_TARGET_ESP32C5) &&  !defined(CONFIG_IDF_TARGET_ESP32C6)

class NeoEsp32RmtChannel4
{
public:
    NeoEsp32RmtChannel4() {};

    rmt_channel_handle_t RmtChannelNumber = NULL;
};

class NeoEsp32RmtChannel5
{
public:
    NeoEsp32RmtChannel5() {};

    rmt_channel_handle_t RmtChannelNumber = NULL;
};

class NeoEsp32RmtChannel6
{
public:
    NeoEsp32RmtChannel6() {};

    rmt_channel_handle_t RmtChannelNumber = NULL;
};

class NeoEsp32RmtChannel7
{
public:
    NeoEsp32RmtChannel7() {};

    rmt_channel_handle_t RmtChannelNumber = NULL;
};

#endif

// dynamic channel support
class NeoEsp32RmtChannelN
{
public:
    NeoEsp32RmtChannelN(NeoBusChannel channel) :
        RmtChannelNumber(RmtChannelNumber)
    {
        RmtChannelNumber = NULL;
    };
    NeoEsp32RmtChannelN() = delete; // no default constructor
    rmt_channel_handle_t RmtChannelNumber = NULL;
};

template<typename T_SPEED, typename T_CHANNEL> class NeoEsp32RmtMethodBase
{
public:
    typedef NeoNoSettings SettingsObject;

    NeoEsp32RmtMethodBase(uint8_t pin, uint16_t pixelCount, size_t elementSize, size_t settingsSize)  :
        _sizeData(pixelCount * elementSize + settingsSize),
        _pin(pin)
    {
        construct();
    }

    NeoEsp32RmtMethodBase(uint8_t pin, uint16_t pixelCount, size_t elementSize, size_t settingsSize, NeoBusChannel channel) :
        _sizeData(pixelCount* elementSize + settingsSize),
        _pin(pin),
        _channel(channel)
    {
        construct();
    }

    ~NeoEsp32RmtMethodBase()
    {
        // wait until the last send finishes before destructing everything
        // arbitrary time out of 10 seconds

        ESP_ERROR_CHECK_WITHOUT_ABORT(rmt_tx_wait_all_done(_channel.RmtChannelNumber, 10000 / portTICK_PERIOD_MS));
        ESP_ERROR_CHECK( rmt_del_channel(_channel.RmtChannelNumber));

        gpio_matrix_out(_pin, 0x100, false, false);
        pinMode(_pin, INPUT);

        free(_dataEditing);
        free(_dataSending);
    }


    bool IsReadyToUpdate() const
    {
        return (ESP_OK == rmt_tx_wait_all_done(_channel.RmtChannelNumber, 0));
    }

    void Initialize()
    {
        esp_err_t ret = ESP_OK;
        rmt_tx_channel_config_t config = {};
        config.clk_src = RMT_CLK_SRC_DEFAULT;
        config.gpio_num = static_cast<gpio_num_t>(_pin);
        config.mem_block_symbols = 192;         // memory block size, 64 * 4 = 256 Bytes
        config.resolution_hz = RMT_LED_STRIP_RESOLUTION_HZ; // 1 MHz tick resolution, i.e., 1 tick = 1 µs
        config.trans_queue_depth = 4;           // set the number of transactions that can pend in the background
        config.flags.invert_out = false;        // do not invert output signal
        config.flags.with_dma = false;          // do not need DMA backend

        ret += rmt_new_tx_channel(&config,&_channel.RmtChannelNumber);
        led_strip_encoder_config_t encoder_config = {};
        encoder_config.resolution = RMT_LED_STRIP_RESOLUTION_HZ;

        _tx_config.loop_count = 0; //no loop

        ret += rmt_new_led_strip_encoder(&encoder_config, &_led_encoder, T_SPEED::RmtBit0, T_SPEED::RmtBit1);

        // ESP_LOGI(TAG, "Enable RMT TX channel");
        ret += rmt_enable(_channel.RmtChannelNumber);
        // if (ret) {
        //     AddLog(2,"RMT: initialized with error code: %u on pin: %u",ret, _pin);
        // }
    }

    void Update(bool maintainBufferConsistency)
    {
        // AddLog(2,"..");
        // wait for not actively sending data
        // this will time out at 10 seconds, an arbitrarily long period of time
        // and do nothing if this happens

        if (ESP_OK == ESP_ERROR_CHECK_WITHOUT_ABORT(rmt_tx_wait_all_done(_channel.RmtChannelNumber, 10000 / portTICK_PERIOD_MS)))
        {
            // AddLog(2,"__ %u", _sizeData);
            // now start the RMT transmit with the editing buffer before we swap
           esp_err_t ret = rmt_transmit(_channel.RmtChannelNumber, _led_encoder, _dataEditing, _sizeData, &_tx_config); // 3 for _sizeData
            // AddLog(2,"rmt_transmit: %u", ret);
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
    const size_t  _sizeData;      // Size of '_data*' buffers 
    const uint8_t _pin;            // output pin number

    rmt_transmit_config_t _tx_config = {};
    rmt_encoder_handle_t _led_encoder = nullptr;

    T_CHANNEL _channel; // holds instance for multi channel support


    // Holds data stream which include LED color values and other settings as needed
    uint8_t*  _dataEditing;   // exposed for get and set
    uint8_t*  _dataSending;   // used for async send using RMT


    void construct()
    {
        // AddLog(2,"RMT:construct");
        _dataEditing = static_cast<uint8_t*>(malloc(_sizeData));
        // data cleared later in Begin()

        _dataSending = static_cast<uint8_t*>(malloc(_sizeData));
        // no need to initialize it, it gets overwritten on every send
    }
};

// normal
typedef NeoEsp32RmtMethodBase<NeoEsp32RmtSpeedWs2811, NeoEsp32RmtChannelN> NeoEsp32RmtNWs2811Method;
typedef NeoEsp32RmtMethodBase<NeoEsp32RmtSpeedWs2812x, NeoEsp32RmtChannelN> NeoEsp32RmtNWs2812xMethod;
typedef NeoEsp32RmtMethodBase<NeoEsp32RmtSpeedSk6812, NeoEsp32RmtChannelN> NeoEsp32RmtNSk6812Method;
typedef NeoEsp32RmtMethodBase<NeoEsp32RmtSpeedTm1814, NeoEsp32RmtChannelN> NeoEsp32RmtNTm1814Method;
typedef NeoEsp32RmtMethodBase<NeoEsp32RmtSpeedTm1829, NeoEsp32RmtChannelN> NeoEsp32RmtNTm1829Method;
typedef NeoEsp32RmtMethodBase<NeoEsp32RmtSpeedTm1914, NeoEsp32RmtChannelN> NeoEsp32RmtNTm1914Method;
typedef NeoEsp32RmtMethodBase<NeoEsp32RmtSpeedApa106, NeoEsp32RmtChannelN> NeoEsp32RmtNApa106Method;
typedef NeoEsp32RmtMethodBase<NeoEsp32RmtSpeedTx1812, NeoEsp32RmtChannelN> NeoEsp32RmtNTx1812Method;
typedef NeoEsp32RmtMethodBase<NeoEsp32RmtSpeed800Kbps, NeoEsp32RmtChannelN> NeoEsp32RmtN800KbpsMethod;
typedef NeoEsp32RmtMethodBase<NeoEsp32RmtSpeed400Kbps, NeoEsp32RmtChannelN> NeoEsp32RmtN400KbpsMethod;

typedef NeoEsp32RmtMethodBase<NeoEsp32RmtSpeedWs2811, NeoEsp32RmtChannel0> NeoEsp32Rmt0Ws2811Method;
typedef NeoEsp32RmtMethodBase<NeoEsp32RmtSpeedWs2812x, NeoEsp32RmtChannel0> NeoEsp32Rmt0Ws2812xMethod;
typedef NeoEsp32RmtMethodBase<NeoEsp32RmtSpeedSk6812, NeoEsp32RmtChannel0> NeoEsp32Rmt0Sk6812Method;
typedef NeoEsp32RmtMethodBase<NeoEsp32RmtSpeedTm1814, NeoEsp32RmtChannel0> NeoEsp32Rmt0Tm1814Method;
typedef NeoEsp32RmtMethodBase<NeoEsp32RmtSpeedTm1829, NeoEsp32RmtChannel0> NeoEsp32Rmt0Tm1829Method;
typedef NeoEsp32RmtMethodBase<NeoEsp32RmtSpeedTm1914, NeoEsp32RmtChannel0> NeoEsp32Rmt0Tm1914Method;
typedef NeoEsp32RmtMethodBase<NeoEsp32RmtSpeedApa106, NeoEsp32RmtChannel0> NeoEsp32Rmt0Apa106Method;
typedef NeoEsp32RmtMethodBase<NeoEsp32RmtSpeedTx1812, NeoEsp32RmtChannel0> NeoEsp32Rmt0Tx1812Method;
typedef NeoEsp32RmtMethodBase<NeoEsp32RmtSpeed800Kbps, NeoEsp32RmtChannel0> NeoEsp32Rmt0800KbpsMethod;
typedef NeoEsp32RmtMethodBase<NeoEsp32RmtSpeed400Kbps, NeoEsp32RmtChannel0> NeoEsp32Rmt0400KbpsMethod;

typedef NeoEsp32RmtMethodBase<NeoEsp32RmtSpeedWs2811, NeoEsp32RmtChannel1> NeoEsp32Rmt1Ws2811Method;
typedef NeoEsp32RmtMethodBase<NeoEsp32RmtSpeedWs2812x, NeoEsp32RmtChannel1> NeoEsp32Rmt1Ws2812xMethod;
typedef NeoEsp32RmtMethodBase<NeoEsp32RmtSpeedSk6812, NeoEsp32RmtChannel1> NeoEsp32Rmt1Sk6812Method;
typedef NeoEsp32RmtMethodBase<NeoEsp32RmtSpeedTm1814, NeoEsp32RmtChannel1> NeoEsp32Rmt1Tm1814Method;
typedef NeoEsp32RmtMethodBase<NeoEsp32RmtSpeedTm1829, NeoEsp32RmtChannel1> NeoEsp32Rmt1Tm1829Method;
typedef NeoEsp32RmtMethodBase<NeoEsp32RmtSpeedTm1914, NeoEsp32RmtChannel1> NeoEsp32Rmt1Tm1914Method;
typedef NeoEsp32RmtMethodBase<NeoEsp32RmtSpeedApa106, NeoEsp32RmtChannel1> NeoEsp32Rmt1Apa106Method;
typedef NeoEsp32RmtMethodBase<NeoEsp32RmtSpeedTx1812, NeoEsp32RmtChannel1> NeoEsp32Rmt1Tx1812Method;
typedef NeoEsp32RmtMethodBase<NeoEsp32RmtSpeed800Kbps, NeoEsp32RmtChannel1> NeoEsp32Rmt1800KbpsMethod;
typedef NeoEsp32RmtMethodBase<NeoEsp32RmtSpeed400Kbps, NeoEsp32RmtChannel1> NeoEsp32Rmt1400KbpsMethod;

#if !defined(CONFIG_IDF_TARGET_ESP32C3) && !defined(CONFIG_IDF_TARGET_ESP32C5) && !defined(CONFIG_IDF_TARGET_ESP32C6)

typedef NeoEsp32RmtMethodBase<NeoEsp32RmtSpeedWs2811, NeoEsp32RmtChannel2> NeoEsp32Rmt2Ws2811Method;
typedef NeoEsp32RmtMethodBase<NeoEsp32RmtSpeedWs2812x, NeoEsp32RmtChannel2> NeoEsp32Rmt2Ws2812xMethod;
typedef NeoEsp32RmtMethodBase<NeoEsp32RmtSpeedSk6812, NeoEsp32RmtChannel2>  NeoEsp32Rmt2Sk6812Method;
typedef NeoEsp32RmtMethodBase<NeoEsp32RmtSpeedTm1814, NeoEsp32RmtChannel2> NeoEsp32Rmt2Tm1814Method;
typedef NeoEsp32RmtMethodBase<NeoEsp32RmtSpeedTm1829, NeoEsp32RmtChannel2> NeoEsp32Rmt2Tm1829Method;
typedef NeoEsp32RmtMethodBase<NeoEsp32RmtSpeedTm1914, NeoEsp32RmtChannel2> NeoEsp32Rmt2Tm1914Method;
typedef NeoEsp32RmtMethodBase<NeoEsp32RmtSpeedApa106, NeoEsp32RmtChannel2> NeoEsp32Rmt2Apa106Method;
typedef NeoEsp32RmtMethodBase<NeoEsp32RmtSpeedTx1812, NeoEsp32RmtChannel2> NeoEsp32Rmt2Tx1812Method;
typedef NeoEsp32RmtMethodBase<NeoEsp32RmtSpeed800Kbps, NeoEsp32RmtChannel2> NeoEsp32Rmt2800KbpsMethod;
typedef NeoEsp32RmtMethodBase<NeoEsp32RmtSpeed400Kbps, NeoEsp32RmtChannel2> NeoEsp32Rmt2400KbpsMethod;

typedef NeoEsp32RmtMethodBase<NeoEsp32RmtSpeedWs2811, NeoEsp32RmtChannel3> NeoEsp32Rmt3Ws2811Method;
typedef NeoEsp32RmtMethodBase<NeoEsp32RmtSpeedWs2812x, NeoEsp32RmtChannel3> NeoEsp32Rmt3Ws2812xMethod;
typedef NeoEsp32RmtMethodBase<NeoEsp32RmtSpeedSk6812, NeoEsp32RmtChannel3>  NeoEsp32Rmt3Sk6812Method;
typedef NeoEsp32RmtMethodBase<NeoEsp32RmtSpeedTm1814, NeoEsp32RmtChannel3> NeoEsp32Rmt3Tm1814Method;
typedef NeoEsp32RmtMethodBase<NeoEsp32RmtSpeedTm1829, NeoEsp32RmtChannel3> NeoEsp32Rmt3Tm1829Method;
typedef NeoEsp32RmtMethodBase<NeoEsp32RmtSpeedTm1914, NeoEsp32RmtChannel3> NeoEsp32Rmt3Tm1914Method;
typedef NeoEsp32RmtMethodBase<NeoEsp32RmtSpeedApa106, NeoEsp32RmtChannel3> NeoEsp32Rmt3Apa106Method;
typedef NeoEsp32RmtMethodBase<NeoEsp32RmtSpeedTx1812, NeoEsp32RmtChannel3> NeoEsp32Rmt3Tx1812Method;
typedef NeoEsp32RmtMethodBase<NeoEsp32RmtSpeed800Kbps, NeoEsp32RmtChannel3> NeoEsp32Rmt3800KbpsMethod;
typedef NeoEsp32RmtMethodBase<NeoEsp32RmtSpeed400Kbps, NeoEsp32RmtChannel3> NeoEsp32Rmt3400KbpsMethod;

#if !defined(CONFIG_IDF_TARGET_ESP32S2) 

typedef NeoEsp32RmtMethodBase<NeoEsp32RmtSpeedWs2811, NeoEsp32RmtChannel4> NeoEsp32Rmt4Ws2811Method;
typedef NeoEsp32RmtMethodBase<NeoEsp32RmtSpeedWs2812x, NeoEsp32RmtChannel4> NeoEsp32Rmt4Ws2812xMethod;
typedef NeoEsp32RmtMethodBase<NeoEsp32RmtSpeedSk6812, NeoEsp32RmtChannel4>  NeoEsp32Rmt4Sk6812Method;
typedef NeoEsp32RmtMethodBase<NeoEsp32RmtSpeedTm1814, NeoEsp32RmtChannel4> NeoEsp32Rmt4Tm1814Method;
typedef NeoEsp32RmtMethodBase<NeoEsp32RmtSpeedTm1829, NeoEsp32RmtChannel4> NeoEsp32Rmt4Tm1829Method;
typedef NeoEsp32RmtMethodBase<NeoEsp32RmtSpeedTm1914, NeoEsp32RmtChannel4> NeoEsp32Rmt4Tm1914Method;
typedef NeoEsp32RmtMethodBase<NeoEsp32RmtSpeedApa106, NeoEsp32RmtChannel4> NeoEsp32Rmt4Apa106Method;
typedef NeoEsp32RmtMethodBase<NeoEsp32RmtSpeedTx1812, NeoEsp32RmtChannel4> NeoEsp32Rmt4Tx1812Method;
typedef NeoEsp32RmtMethodBase<NeoEsp32RmtSpeed800Kbps, NeoEsp32RmtChannel4> NeoEsp32Rmt4800KbpsMethod;
typedef NeoEsp32RmtMethodBase<NeoEsp32RmtSpeed400Kbps, NeoEsp32RmtChannel4> NeoEsp32Rmt4400KbpsMethod;

typedef NeoEsp32RmtMethodBase<NeoEsp32RmtSpeedWs2811, NeoEsp32RmtChannel5> NeoEsp32Rmt5Ws2811Method;
typedef NeoEsp32RmtMethodBase<NeoEsp32RmtSpeedWs2812x, NeoEsp32RmtChannel5> NeoEsp32Rmt5Ws2812xMethod;
typedef NeoEsp32RmtMethodBase<NeoEsp32RmtSpeedSk6812, NeoEsp32RmtChannel5>  NeoEsp32Rmt5Sk6812Method;
typedef NeoEsp32RmtMethodBase<NeoEsp32RmtSpeedTm1814, NeoEsp32RmtChannel5> NeoEsp32Rmt5Tm1814Method;
typedef NeoEsp32RmtMethodBase<NeoEsp32RmtSpeedTm1829, NeoEsp32RmtChannel5> NeoEsp32Rmt5Tm1829Method;
typedef NeoEsp32RmtMethodBase<NeoEsp32RmtSpeedTm1914, NeoEsp32RmtChannel5> NeoEsp32Rmt5Tm1914Method;
typedef NeoEsp32RmtMethodBase<NeoEsp32RmtSpeedApa106, NeoEsp32RmtChannel5> NeoEsp32Rmt5Apa106Method;
typedef NeoEsp32RmtMethodBase<NeoEsp32RmtSpeedTx1812, NeoEsp32RmtChannel5> NeoEsp32Rmt5Tx1812Method;
typedef NeoEsp32RmtMethodBase<NeoEsp32RmtSpeed800Kbps, NeoEsp32RmtChannel5> NeoEsp32Rmt5800KbpsMethod;
typedef NeoEsp32RmtMethodBase<NeoEsp32RmtSpeed400Kbps, NeoEsp32RmtChannel5> NeoEsp32Rmt5400KbpsMethod;

typedef NeoEsp32RmtMethodBase<NeoEsp32RmtSpeedWs2811, NeoEsp32RmtChannel6> NeoEsp32Rmt6Ws2811Method;
typedef NeoEsp32RmtMethodBase<NeoEsp32RmtSpeedWs2812x, NeoEsp32RmtChannel6> NeoEsp32Rmt6Ws2812xMethod;
typedef NeoEsp32RmtMethodBase<NeoEsp32RmtSpeedSk6812, NeoEsp32RmtChannel6>  NeoEsp32Rmt6Sk6812Method;
typedef NeoEsp32RmtMethodBase<NeoEsp32RmtSpeedTm1814, NeoEsp32RmtChannel6> NeoEsp32Rmt6Tm1814Method;
typedef NeoEsp32RmtMethodBase<NeoEsp32RmtSpeedTm1829, NeoEsp32RmtChannel6> NeoEsp32Rmt6Tm1829Method;
typedef NeoEsp32RmtMethodBase<NeoEsp32RmtSpeedTm1914, NeoEsp32RmtChannel6> NeoEsp32Rmt6Tm1914Method;
typedef NeoEsp32RmtMethodBase<NeoEsp32RmtSpeedApa106, NeoEsp32RmtChannel6> NeoEsp32Rmt6Apa106Method;
typedef NeoEsp32RmtMethodBase<NeoEsp32RmtSpeedTx1812, NeoEsp32RmtChannel6> NeoEsp32Rmt6Tx1812Method;
typedef NeoEsp32RmtMethodBase<NeoEsp32RmtSpeed800Kbps, NeoEsp32RmtChannel6> NeoEsp32Rmt6800KbpsMethod;
typedef NeoEsp32RmtMethodBase<NeoEsp32RmtSpeed400Kbps, NeoEsp32RmtChannel6> NeoEsp32Rmt6400KbpsMethod;

typedef NeoEsp32RmtMethodBase<NeoEsp32RmtSpeedWs2811, NeoEsp32RmtChannel7> NeoEsp32Rmt7Ws2811Method;
typedef NeoEsp32RmtMethodBase<NeoEsp32RmtSpeedWs2812x, NeoEsp32RmtChannel7> NeoEsp32Rmt7Ws2812xMethod;
typedef NeoEsp32RmtMethodBase<NeoEsp32RmtSpeedSk6812, NeoEsp32RmtChannel7>  NeoEsp32Rmt7Sk6812Method;
typedef NeoEsp32RmtMethodBase<NeoEsp32RmtSpeedTm1814, NeoEsp32RmtChannel7> NeoEsp32Rmt7Tm1814Method;
typedef NeoEsp32RmtMethodBase<NeoEsp32RmtSpeedTm1829, NeoEsp32RmtChannel7> NeoEsp32Rmt7Tm1829Method;
typedef NeoEsp32RmtMethodBase<NeoEsp32RmtSpeedTm1914, NeoEsp32RmtChannel7> NeoEsp32Rmt7Tm1914Method;
typedef NeoEsp32RmtMethodBase<NeoEsp32RmtSpeedApa106, NeoEsp32RmtChannel7> NeoEsp32Rmt7Apa106Method;
typedef NeoEsp32RmtMethodBase<NeoEsp32RmtSpeedTx1812, NeoEsp32RmtChannel7> NeoEsp32Rmt7Tx1812Method;
typedef NeoEsp32RmtMethodBase<NeoEsp32RmtSpeed800Kbps, NeoEsp32RmtChannel7> NeoEsp32Rmt7800KbpsMethod;
typedef NeoEsp32RmtMethodBase<NeoEsp32RmtSpeed400Kbps, NeoEsp32RmtChannel7> NeoEsp32Rmt7400KbpsMethod;

#endif // !defined(CONFIG_IDF_TARGET_ESP32S2) 
#endif // !defined(CONFIG_IDF_TARGET_ESP32C3) && !defined(CONFIG_IDF_TARGET_ESP32C3)

// inverted
typedef NeoEsp32RmtMethodBase<NeoEsp32RmtInvertedSpeedWs2811, NeoEsp32RmtChannelN> NeoEsp32RmtNWs2811InvertedMethod;
typedef NeoEsp32RmtMethodBase<NeoEsp32RmtInvertedSpeedWs2812x, NeoEsp32RmtChannelN> NeoEsp32RmtNWs2812xInvertedMethod;
typedef NeoEsp32RmtMethodBase<NeoEsp32RmtInvertedSpeedSk6812, NeoEsp32RmtChannelN> NeoEsp32RmtNSk6812InvertedMethod;
typedef NeoEsp32RmtMethodBase<NeoEsp32RmtInvertedSpeedTm1814, NeoEsp32RmtChannelN> NeoEsp32RmtNTm1814InvertedMethod;
typedef NeoEsp32RmtMethodBase<NeoEsp32RmtInvertedSpeedTm1829, NeoEsp32RmtChannelN> NeoEsp32RmtNTm1829InvertedMethod;
typedef NeoEsp32RmtMethodBase<NeoEsp32RmtInvertedSpeedTm1914, NeoEsp32RmtChannelN> NeoEsp32RmtNTm1914InvertedMethod;
typedef NeoEsp32RmtMethodBase<NeoEsp32RmtInvertedSpeedApa106, NeoEsp32RmtChannelN> NeoEsp32RmtNApa106InvertedMethod;
typedef NeoEsp32RmtMethodBase<NeoEsp32RmtInvertedSpeedTx1812, NeoEsp32RmtChannelN> NeoEsp32RmtNTx1812InvertedMethod;
typedef NeoEsp32RmtMethodBase<NeoEsp32RmtInvertedSpeed800Kbps, NeoEsp32RmtChannelN> NeoEsp32RmtN800KbpsInvertedMethod;
typedef NeoEsp32RmtMethodBase<NeoEsp32RmtInvertedSpeed400Kbps, NeoEsp32RmtChannelN> NeoEsp32RmtN400KbpsInvertedMethod;

typedef NeoEsp32RmtMethodBase<NeoEsp32RmtInvertedSpeedWs2811, NeoEsp32RmtChannel0> NeoEsp32Rmt0Ws2811InvertedMethod;
typedef NeoEsp32RmtMethodBase<NeoEsp32RmtInvertedSpeedWs2812x, NeoEsp32RmtChannel0> NeoEsp32Rmt0Ws2812xInvertedMethod;
typedef NeoEsp32RmtMethodBase<NeoEsp32RmtInvertedSpeedSk6812, NeoEsp32RmtChannel0> NeoEsp32Rmt0Sk6812InvertedMethod;
typedef NeoEsp32RmtMethodBase<NeoEsp32RmtInvertedSpeedTm1814, NeoEsp32RmtChannel0> NeoEsp32Rmt0Tm1814InvertedMethod;
typedef NeoEsp32RmtMethodBase<NeoEsp32RmtInvertedSpeedTm1829, NeoEsp32RmtChannel0> NeoEsp32Rmt0Tm1829InvertedMethod;
typedef NeoEsp32RmtMethodBase<NeoEsp32RmtInvertedSpeedTm1914, NeoEsp32RmtChannel0> NeoEsp32Rmt0Tm1914InvertedMethod;
typedef NeoEsp32RmtMethodBase<NeoEsp32RmtInvertedSpeedApa106, NeoEsp32RmtChannel0> NeoEsp32Rmt0Apa106InvertedMethod;
typedef NeoEsp32RmtMethodBase<NeoEsp32RmtInvertedSpeedTx1812, NeoEsp32RmtChannel0> NeoEsp32Rmt0Tx1812InvertedMethod;
typedef NeoEsp32RmtMethodBase<NeoEsp32RmtInvertedSpeed800Kbps, NeoEsp32RmtChannel0> NeoEsp32Rmt0800KbpsInvertedMethod;
typedef NeoEsp32RmtMethodBase<NeoEsp32RmtInvertedSpeed400Kbps, NeoEsp32RmtChannel0> NeoEsp32Rmt0400KbpsInvertedMethod;

typedef NeoEsp32RmtMethodBase<NeoEsp32RmtInvertedSpeedWs2811, NeoEsp32RmtChannel1> NeoEsp32Rmt1Ws2811InvertedMethod;
typedef NeoEsp32RmtMethodBase<NeoEsp32RmtInvertedSpeedWs2812x, NeoEsp32RmtChannel1> NeoEsp32Rmt1Ws2812xInvertedMethod;
typedef NeoEsp32RmtMethodBase<NeoEsp32RmtInvertedSpeedSk6812, NeoEsp32RmtChannel1> NeoEsp32Rmt1Sk6812InvertedMethod;
typedef NeoEsp32RmtMethodBase<NeoEsp32RmtInvertedSpeedTm1814, NeoEsp32RmtChannel1> NeoEsp32Rmt1Tm1814InvertedMethod;
typedef NeoEsp32RmtMethodBase<NeoEsp32RmtInvertedSpeedTm1829, NeoEsp32RmtChannel1> NeoEsp32Rmt1Tm1829InvertedMethod;
typedef NeoEsp32RmtMethodBase<NeoEsp32RmtInvertedSpeedTm1914, NeoEsp32RmtChannel1> NeoEsp32Rmt1Tm1914InvertedMethod;
typedef NeoEsp32RmtMethodBase<NeoEsp32RmtInvertedSpeedApa106, NeoEsp32RmtChannel1> NeoEsp32Rmt1Apa106InvertedMethod;
typedef NeoEsp32RmtMethodBase<NeoEsp32RmtInvertedSpeedTx1812, NeoEsp32RmtChannel1> NeoEsp32Rmt1Tx1812InvertedMethod;
typedef NeoEsp32RmtMethodBase<NeoEsp32RmtInvertedSpeed800Kbps, NeoEsp32RmtChannel1> NeoEsp32Rmt1800KbpsInvertedMethod;
typedef NeoEsp32RmtMethodBase<NeoEsp32RmtInvertedSpeed400Kbps, NeoEsp32RmtChannel1> NeoEsp32Rmt1400KbpsInvertedMethod;

#if !defined(CONFIG_IDF_TARGET_ESP32C3) && !defined(CONFIG_IDF_TARGET_ESP32C5) && !defined(CONFIG_IDF_TARGET_ESP32C6)

typedef NeoEsp32RmtMethodBase<NeoEsp32RmtInvertedSpeedWs2811, NeoEsp32RmtChannel2> NeoEsp32Rmt2Ws2811InvertedMethod;
typedef NeoEsp32RmtMethodBase<NeoEsp32RmtInvertedSpeedWs2812x, NeoEsp32RmtChannel2> NeoEsp32Rmt2Ws2812xInvertedMethod;
typedef NeoEsp32RmtMethodBase<NeoEsp32RmtInvertedSpeedSk6812, NeoEsp32RmtChannel2>  NeoEsp32Rmt2Sk6812InvertedMethod;
typedef NeoEsp32RmtMethodBase<NeoEsp32RmtInvertedSpeedTm1814, NeoEsp32RmtChannel2> NeoEsp32Rmt2Tm1814InvertedMethod;
typedef NeoEsp32RmtMethodBase<NeoEsp32RmtInvertedSpeedTm1829, NeoEsp32RmtChannel2> NeoEsp32Rmt2Tm1829InvertedMethod;
typedef NeoEsp32RmtMethodBase<NeoEsp32RmtInvertedSpeedTm1914, NeoEsp32RmtChannel2> NeoEsp32Rmt2Tm1914InvertedMethod;
typedef NeoEsp32RmtMethodBase<NeoEsp32RmtInvertedSpeedApa106, NeoEsp32RmtChannel2> NeoEsp32Rmt2Apa106InvertedMethod;
typedef NeoEsp32RmtMethodBase<NeoEsp32RmtInvertedSpeedTx1812, NeoEsp32RmtChannel2> NeoEsp32Rmt2Tx1812InvertedMethod;
typedef NeoEsp32RmtMethodBase<NeoEsp32RmtInvertedSpeed800Kbps, NeoEsp32RmtChannel2> NeoEsp32Rmt2800KbpsInvertedMethod;
typedef NeoEsp32RmtMethodBase<NeoEsp32RmtInvertedSpeed400Kbps, NeoEsp32RmtChannel2> NeoEsp32Rmt2400KbpsInvertedMethod;

typedef NeoEsp32RmtMethodBase<NeoEsp32RmtInvertedSpeedWs2811, NeoEsp32RmtChannel3> NeoEsp32Rmt3Ws2811InvertedMethod;
typedef NeoEsp32RmtMethodBase<NeoEsp32RmtInvertedSpeedWs2812x, NeoEsp32RmtChannel3> NeoEsp32Rmt3Ws2812xInvertedMethod;
typedef NeoEsp32RmtMethodBase<NeoEsp32RmtInvertedSpeedSk6812, NeoEsp32RmtChannel3>  NeoEsp32Rmt3Sk6812InvertedMethod;
typedef NeoEsp32RmtMethodBase<NeoEsp32RmtInvertedSpeedTm1814, NeoEsp32RmtChannel3> NeoEsp32Rmt3Tm1814InvertedMethod;
typedef NeoEsp32RmtMethodBase<NeoEsp32RmtInvertedSpeedTm1829, NeoEsp32RmtChannel3> NeoEsp32Rmt3Tm1829InvertedMethod;
typedef NeoEsp32RmtMethodBase<NeoEsp32RmtInvertedSpeedTm1914, NeoEsp32RmtChannel3> NeoEsp32Rmt3Tm1914InvertedMethod;
typedef NeoEsp32RmtMethodBase<NeoEsp32RmtInvertedSpeedApa106, NeoEsp32RmtChannel3> NeoEsp32Rmt3Apa106InvertedMethod;
typedef NeoEsp32RmtMethodBase<NeoEsp32RmtInvertedSpeedTx1812, NeoEsp32RmtChannel3> NeoEsp32Rmt3Tx1812InvertedMethod;
typedef NeoEsp32RmtMethodBase<NeoEsp32RmtInvertedSpeed800Kbps, NeoEsp32RmtChannel3> NeoEsp32Rmt3800KbpsInvertedMethod;
typedef NeoEsp32RmtMethodBase<NeoEsp32RmtInvertedSpeed400Kbps, NeoEsp32RmtChannel3> NeoEsp32Rmt3400KbpsInvertedMethod;

#if !defined(CONFIG_IDF_TARGET_ESP32S2)

typedef NeoEsp32RmtMethodBase<NeoEsp32RmtInvertedSpeedWs2811, NeoEsp32RmtChannel4> NeoEsp32Rmt4Ws2811InvertedMethod;
typedef NeoEsp32RmtMethodBase<NeoEsp32RmtInvertedSpeedWs2812x, NeoEsp32RmtChannel4> NeoEsp32Rmt4Ws2812xInvertedMethod;
typedef NeoEsp32RmtMethodBase<NeoEsp32RmtInvertedSpeedSk6812, NeoEsp32RmtChannel4>  NeoEsp32Rmt4Sk6812InvertedMethod;
typedef NeoEsp32RmtMethodBase<NeoEsp32RmtInvertedSpeedTm1814, NeoEsp32RmtChannel4> NeoEsp32Rmt4Tm1814InvertedMethod;
typedef NeoEsp32RmtMethodBase<NeoEsp32RmtInvertedSpeedTm1829, NeoEsp32RmtChannel4> NeoEsp32Rmt4Tm1829InvertedMethod;
typedef NeoEsp32RmtMethodBase<NeoEsp32RmtInvertedSpeedTm1914, NeoEsp32RmtChannel4> NeoEsp32Rmt4Tm1914InvertedMethod;
typedef NeoEsp32RmtMethodBase<NeoEsp32RmtInvertedSpeedApa106, NeoEsp32RmtChannel4> NeoEsp32Rmt4Apa106InvertedMethod;
typedef NeoEsp32RmtMethodBase<NeoEsp32RmtInvertedSpeedTx1812, NeoEsp32RmtChannel4> NeoEsp32Rmt4Tx1812InvertedMethod;
typedef NeoEsp32RmtMethodBase<NeoEsp32RmtInvertedSpeed800Kbps, NeoEsp32RmtChannel4> NeoEsp32Rmt4800KbpsInvertedMethod;
typedef NeoEsp32RmtMethodBase<NeoEsp32RmtInvertedSpeed400Kbps, NeoEsp32RmtChannel4> NeoEsp32Rmt4400KbpsInvertedMethod;

typedef NeoEsp32RmtMethodBase<NeoEsp32RmtInvertedSpeedWs2811, NeoEsp32RmtChannel5> NeoEsp32Rmt5Ws2811InvertedMethod;
typedef NeoEsp32RmtMethodBase<NeoEsp32RmtInvertedSpeedWs2812x, NeoEsp32RmtChannel5> NeoEsp32Rmt5Ws2812xInvertedMethod;
typedef NeoEsp32RmtMethodBase<NeoEsp32RmtInvertedSpeedSk6812, NeoEsp32RmtChannel5>  NeoEsp32Rmt5Sk6812InvertedMethod;
typedef NeoEsp32RmtMethodBase<NeoEsp32RmtInvertedSpeedTm1814, NeoEsp32RmtChannel5> NeoEsp32Rmt5Tm1814InvertedMethod;
typedef NeoEsp32RmtMethodBase<NeoEsp32RmtInvertedSpeedTm1829, NeoEsp32RmtChannel5> NeoEsp32Rmt5Tm1829InvertedMethod;
typedef NeoEsp32RmtMethodBase<NeoEsp32RmtInvertedSpeedTm1914, NeoEsp32RmtChannel5> NeoEsp32Rmt5Tm1914InvertedMethod;
typedef NeoEsp32RmtMethodBase<NeoEsp32RmtInvertedSpeedApa106, NeoEsp32RmtChannel5> NeoEsp32Rmt5Apa106InvertedMethod;
typedef NeoEsp32RmtMethodBase<NeoEsp32RmtInvertedSpeedTx1812, NeoEsp32RmtChannel5> NeoEsp32Rmt5Tx1812InvertedMethod;
typedef NeoEsp32RmtMethodBase<NeoEsp32RmtInvertedSpeed800Kbps, NeoEsp32RmtChannel5> NeoEsp32Rmt5800KbpsInvertedMethod;
typedef NeoEsp32RmtMethodBase<NeoEsp32RmtInvertedSpeed400Kbps, NeoEsp32RmtChannel5> NeoEsp32Rmt5400KbpsInvertedMethod;

typedef NeoEsp32RmtMethodBase<NeoEsp32RmtInvertedSpeedWs2811, NeoEsp32RmtChannel6> NeoEsp32Rmt6Ws2811InvertedMethod;
typedef NeoEsp32RmtMethodBase<NeoEsp32RmtInvertedSpeedWs2812x, NeoEsp32RmtChannel6> NeoEsp32Rmt6Ws2812xInvertedMethod;
typedef NeoEsp32RmtMethodBase<NeoEsp32RmtInvertedSpeedSk6812, NeoEsp32RmtChannel6>  NeoEsp32Rmt6Sk6812InvertedMethod;
typedef NeoEsp32RmtMethodBase<NeoEsp32RmtInvertedSpeedTm1814, NeoEsp32RmtChannel6> NeoEsp32Rmt6Tm1814InvertedMethod;
typedef NeoEsp32RmtMethodBase<NeoEsp32RmtInvertedSpeedTm1829, NeoEsp32RmtChannel6> NeoEsp32Rmt6Tm1829InvertedMethod;
typedef NeoEsp32RmtMethodBase<NeoEsp32RmtInvertedSpeedTm1914, NeoEsp32RmtChannel6> NeoEsp32Rmt6Tm1914InvertedMethod;
typedef NeoEsp32RmtMethodBase<NeoEsp32RmtInvertedSpeedApa106, NeoEsp32RmtChannel6> NeoEsp32Rmt6Apa106InvertedMethod;
typedef NeoEsp32RmtMethodBase<NeoEsp32RmtInvertedSpeedTx1812, NeoEsp32RmtChannel6> NeoEsp32Rmt6Tx1812InvertedMethod;
typedef NeoEsp32RmtMethodBase<NeoEsp32RmtInvertedSpeed800Kbps, NeoEsp32RmtChannel6> NeoEsp32Rmt6800KbpsInvertedMethod;
typedef NeoEsp32RmtMethodBase<NeoEsp32RmtInvertedSpeed400Kbps, NeoEsp32RmtChannel6> NeoEsp32Rmt6400KbpsInvertedMethod;

typedef NeoEsp32RmtMethodBase<NeoEsp32RmtInvertedSpeedWs2811, NeoEsp32RmtChannel7> NeoEsp32Rmt7Ws2811InvertedMethod;
typedef NeoEsp32RmtMethodBase<NeoEsp32RmtInvertedSpeedWs2812x, NeoEsp32RmtChannel7> NeoEsp32Rmt7Ws2812xInvertedMethod;
typedef NeoEsp32RmtMethodBase<NeoEsp32RmtInvertedSpeedSk6812, NeoEsp32RmtChannel7>  NeoEsp32Rmt7Sk6812InvertedMethod;
typedef NeoEsp32RmtMethodBase<NeoEsp32RmtInvertedSpeedTm1814, NeoEsp32RmtChannel7> NeoEsp32Rmt7Tm1814InvertedMethod;
typedef NeoEsp32RmtMethodBase<NeoEsp32RmtInvertedSpeedTm1829, NeoEsp32RmtChannel7> NeoEsp32Rmt7Tm1829InvertedMethod;
typedef NeoEsp32RmtMethodBase<NeoEsp32RmtInvertedSpeedTm1914, NeoEsp32RmtChannel7> NeoEsp32Rmt7Tm1914InvertedMethod;
typedef NeoEsp32RmtMethodBase<NeoEsp32RmtInvertedSpeedApa106, NeoEsp32RmtChannel7> NeoEsp32Rmt7Apa106InvertedMethod;
typedef NeoEsp32RmtMethodBase<NeoEsp32RmtInvertedSpeedTx1812, NeoEsp32RmtChannel7> NeoEsp32Rmt7Tx1812InvertedMethod;
typedef NeoEsp32RmtMethodBase<NeoEsp32RmtInvertedSpeed800Kbps, NeoEsp32RmtChannel7> NeoEsp32Rmt7800KbpsInvertedMethod;
typedef NeoEsp32RmtMethodBase<NeoEsp32RmtInvertedSpeed400Kbps, NeoEsp32RmtChannel7> NeoEsp32Rmt7400KbpsInvertedMethod;

#endif // !defined(CONFIG_IDF_TARGET_ESP32S2) 
#endif // !defined(CONFIG_IDF_TARGET_ESP32C3) && !defined(CONFIG_IDF_TARGET_ESP32C6)


#if defined(NEOPIXEL_ESP32_RMT_DEFAULT) || defined(CONFIG_IDF_TARGET_ESP32S2) || defined(CONFIG_IDF_TARGET_ESP32C3) || defined(CONFIG_IDF_TARGET_ESP32C5) || defined(CONFIG_IDF_TARGET_ESP32C6)

// Normally I2s method is the default, defining NEOPIXEL_ESP32_RMT_DEFAULT 
// will switch to use RMT as the default method
// The ESP32S2 & ESP32C3 will always defualt to RMT

#if defined(CONFIG_IDF_TARGET_ESP32S2) || defined(CONFIG_IDF_TARGET_ESP32C3) || defined(CONFIG_IDF_TARGET_ESP32C5) || defined(CONFIG_IDF_TARGET_ESP32C6)

// RMT channel 1 method is the default method for Esp32S2 & Esp32C3
typedef NeoEsp32Rmt1Ws2812xMethod NeoWs2813Method;
typedef NeoEsp32Rmt1Ws2812xMethod NeoWs2812xMethod;
typedef NeoEsp32Rmt1800KbpsMethod NeoWs2812Method;
typedef NeoEsp32Rmt1Ws2812xMethod NeoWs2811Method;
typedef NeoEsp32Rmt1Sk6812Method NeoSk6812Method;
typedef NeoEsp32Rmt1Tm1814Method NeoTm1814Method;
typedef NeoEsp32Rmt1Tm1829Method NeoTm1829Method;
typedef NeoEsp32Rmt1Tm1914Method NeoTm1914Method;
typedef NeoEsp32Rmt1Sk6812Method NeoLc8812Method;
typedef NeoEsp32Rmt1Apa106Method NeoApa106Method;
typedef NeoEsp32Rmt1Tx1812Method NeoTx1812Method;

typedef NeoEsp32Rmt1Ws2812xMethod Neo800KbpsMethod;
typedef NeoEsp32Rmt1400KbpsMethod Neo400KbpsMethod;

typedef NeoEsp32Rmt1Ws2812xInvertedMethod NeoWs2813InvertedMethod;
typedef NeoEsp32Rmt1Ws2812xInvertedMethod NeoWs2812xInvertedMethod;
typedef NeoEsp32Rmt1Ws2812xInvertedMethod NeoWs2811InvertedMethod;
typedef NeoEsp32Rmt1800KbpsInvertedMethod NeoWs2812InvertedMethod;
typedef NeoEsp32Rmt1Sk6812InvertedMethod NeoSk6812InvertedMethod;
typedef NeoEsp32Rmt1Tm1814InvertedMethod NeoTm1814InvertedMethod;
typedef NeoEsp32Rmt1Tm1829InvertedMethod NeoTm1829InvertedMethod;
typedef NeoEsp32Rmt1Tm1914InvertedMethod NeoTm1914InvertedMethod;
typedef NeoEsp32Rmt1Sk6812InvertedMethod NeoLc8812InvertedMethod;
typedef NeoEsp32Rmt1Apa106InvertedMethod NeoApa106InvertedMethod;
typedef NeoEsp32Rmt1Tx1812InvertedMethod NeoTx1812InvertedMethod;

typedef NeoEsp32Rmt1Ws2812xInvertedMethod Neo800KbpsInvertedMethod;
typedef NeoEsp32Rmt1400KbpsInvertedMethod Neo400KbpsInvertedMethod;

#else // defined(CONFIG_IDF_TARGET_ESP32S2) || defined(CONFIG_IDF_TARGET_ESP32C3)

// RMT channel 6 method is the default method for Esp32
typedef NeoEsp32Rmt6Ws2812xMethod NeoWs2813Method;
typedef NeoEsp32Rmt6Ws2812xMethod NeoWs2812xMethod;
typedef NeoEsp32Rmt6800KbpsMethod NeoWs2812Method;
typedef NeoEsp32Rmt6Ws2812xMethod NeoWs2811Method;
typedef NeoEsp32Rmt6Sk6812Method NeoSk6812Method;
typedef NeoEsp32Rmt6Tm1814Method NeoTm1814Method;
typedef NeoEsp32Rmt6Tm1829Method NeoTm1829Method;
typedef NeoEsp32Rmt6Tm1914Method NeoTm1914Method;
typedef NeoEsp32Rmt6Sk6812Method NeoLc8812Method;
typedef NeoEsp32Rmt6Apa106Method NeoApa106Method;
typedef NeoEsp32Rmt6Tx1812Method NeoTx1812Method;

typedef NeoEsp32Rmt6Ws2812xMethod Neo800KbpsMethod;
typedef NeoEsp32Rmt6400KbpsMethod Neo400KbpsMethod;

typedef NeoEsp32Rmt6Ws2812xInvertedMethod NeoWs2813InvertedMethod;
typedef NeoEsp32Rmt6Ws2812xInvertedMethod NeoWs2812xInvertedMethod;
typedef NeoEsp32Rmt6Ws2812xInvertedMethod NeoWs2811InvertedMethod;
typedef NeoEsp32Rmt6800KbpsInvertedMethod NeoWs2812InvertedMethod;
typedef NeoEsp32Rmt6Sk6812InvertedMethod NeoSk6812InvertedMethod;
typedef NeoEsp32Rmt6Tm1814InvertedMethod NeoTm1814InvertedMethod;
typedef NeoEsp32Rmt6Tm1829InvertedMethod NeoTm1829InvertedMethod;
typedef NeoEsp32Rmt6Tm1914InvertedMethod NeoTm1914InvertedMethod;
typedef NeoEsp32Rmt6Sk6812InvertedMethod NeoLc8812InvertedMethod;
typedef NeoEsp32Rmt6Apa106InvertedMethod NeoApa106InvertedMethod;
typedef NeoEsp32Rmt6Tx1812InvertedMethod NeoTx1812InvertedMethod;

typedef NeoEsp32Rmt6Ws2812xInvertedMethod Neo800KbpsInvertedMethod;
typedef NeoEsp32Rmt6400KbpsInvertedMethod Neo400KbpsInvertedMethod;

#endif // defined(CONFIG_IDF_TARGET_ESP32S2) || defined(CONFIG_IDF_TARGET_ESP32C3) || defined(CONFIG_IDF_TARGET_ESP32C6)

#endif // defined(NEOPIXEL_ESP32_RMT_DEFAULT) || defined(CONFIG_IDF_TARGET_ESP32S2) || defined(CONFIG_IDF_TARGET_ESP32C3) || defined(CONFIG_IDF_TARGET_ESP32C6)

#endif
