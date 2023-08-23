/*-------------------------------------------------------------------------
NeoPixel library helper functions for Esp32.

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

#if ESP_IDF_VERSION_MAJOR <= 4
// ESP32C3 I2S is not supported yet due to significant changes to interface
#if defined(ARDUINO_ARCH_ESP32) && !defined(CONFIG_IDF_TARGET_ESP32C3)

extern "C"
{
#include <Arduino.h>
#if ESP_IDF_VERSION_MAJOR >= 5
#include <rom/gpio.h>
#endif
#include "Esp32_i2s.h"
}

const uint16_t c_dmaBytesPerPixelBytes = 4;

class NeoEsp32I2sSpeedWs2812x
{
public:
    const static uint32_t I2sSampleRate = 100000;
    const static uint16_t ByteSendTimeUs = 10;
    const static uint16_t ResetTimeUs = 300;
};

class NeoEsp32I2sSpeedSk6812
{
public:
    const static uint32_t I2sSampleRate = 100000;
    const static uint16_t ByteSendTimeUs = 10;
    const static uint16_t ResetTimeUs = 80;
};

class NeoEsp32I2sSpeedTm1814
{
public:
    const static uint32_t I2sSampleRate = 100000;
    const static uint16_t ByteSendTimeUs = 10;
    const static uint16_t ResetTimeUs = 200;
};

class NeoEsp32I2sSpeedTm1914
{
public:
    const static uint32_t I2sSampleRate = 100000;
    const static uint16_t ByteSendTimeUs = 10;
    const static uint16_t ResetTimeUs = 200;
};

class NeoEsp32I2sSpeedTm1829
{
public:
    const static uint32_t I2sSampleRate = 100000;
    const static uint16_t ByteSendTimeUs = 10;
    const static uint16_t ResetTimeUs = 200;
};

class NeoEsp32I2sSpeed800Kbps
{
public:
    const static uint32_t I2sSampleRate = 100000;
    const static uint16_t ByteSendTimeUs = 10;
    const static uint16_t ResetTimeUs = 50;
};

class NeoEsp32I2sSpeed400Kbps
{
public:
    const static uint32_t I2sSampleRate = 50000;
    const static uint16_t ByteSendTimeUs = 20;
    const static uint16_t ResetTimeUs = 50;
};

class NeoEsp32I2sSpeedApa106
{
public:
    const static uint32_t I2sSampleRate = 76000;
    const static uint16_t ByteSendTimeUs = 14;
    const static uint16_t ResetTimeUs = 50;
};

class NeoEsp32I2sBusZero
{
public:
    NeoEsp32I2sBusZero() {};

    const static uint8_t I2sBusNumber = 0;
};

class NeoEsp32I2sBusOne
{
public:
    NeoEsp32I2sBusOne() {};

    const static uint8_t I2sBusNumber = 1;
};

// dynamic channel support
class NeoEsp32I2sBusN
{
public:
    NeoEsp32I2sBusN(NeoBusChannel channel) :
        I2sBusNumber(static_cast<uint8_t>(channel))
    {
    }
    NeoEsp32I2sBusN() = delete; // no default constructor

    const uint8_t I2sBusNumber;
};

class NeoEsp32I2sNotInverted
{
public:
    const static bool Inverted = false;
};

class NeoEsp32I2sInverted
{
public:
    const static bool Inverted = true;
};

template<typename T_SPEED, typename T_BUS, typename T_INVERT> class NeoEsp32I2sMethodBase
{
public:
    typedef NeoNoSettings SettingsObject;

    NeoEsp32I2sMethodBase(uint8_t pin, uint16_t pixelCount, size_t elementSize, size_t settingsSize)  :
        _sizeData(pixelCount * elementSize + settingsSize),
        _pin(pin)
    {
        construct(pixelCount, elementSize, settingsSize);
    }

    NeoEsp32I2sMethodBase(uint8_t pin, uint16_t pixelCount, size_t elementSize, size_t settingsSize, NeoBusChannel channel) :
        _sizeData(pixelCount * elementSize + settingsSize),
        _pin(pin),
        _bus(channel)
    {
        construct(pixelCount, elementSize, settingsSize);
    }

    ~NeoEsp32I2sMethodBase()
    {
        while (!IsReadyToUpdate())
        {
            yield();
        }

        gpio_matrix_out(_pin, 0x100, false, false);
        pinMode(_pin, INPUT);

        free(_data);
        free(_i2sBuffer);
    }

    bool IsReadyToUpdate() const
    {
        return (i2sWriteDone(_bus.I2sBusNumber));
    }

    void Initialize()
    {
        size_t dmaBlockCount = (_i2sBufferSize + I2S_DMA_MAX_DATA_LEN - 1) / I2S_DMA_MAX_DATA_LEN;

        i2sInit(_bus.I2sBusNumber, 
            16, 
            T_SPEED::I2sSampleRate, 
            I2S_CHAN_STEREO, 
            I2S_FIFO_16BIT_DUAL, 
            dmaBlockCount,
            0);
        i2sSetPins(_bus.I2sBusNumber, _pin, T_INVERT::Inverted);
    }

    void Update(bool)
    {
        // wait for not actively sending data
        while (!IsReadyToUpdate())
        {
            yield();
        }

        FillBuffers();

        i2sWrite(_bus.I2sBusNumber, _i2sBuffer, _i2sBufferSize, false, false);
    }

    uint8_t* getData() const
    {
        return _data;
    };

    size_t getDataSize() const
    {
        return _sizeData;
    }

    void applySettings(const SettingsObject& settings)
    {
    }

private:
    const size_t  _sizeData;    // Size of '_data' buffer 
    const uint8_t _pin;            // output pin number
    const T_BUS _bus; // holds instance for multi bus support

    uint8_t*  _data;        // Holds LED color values

    uint32_t _i2sBufferSize; // total size of _i2sBuffer
    uint8_t* _i2sBuffer;  // holds the DMA buffer that is referenced by _i2sBufDesc

    void construct(uint16_t pixelCount, size_t elementSize, size_t settingsSize) 
    {
        // DMA is too fast to support a single pixel and maintain consistency
        if (pixelCount < 2)
        {
            pixelCount = 2;
        }

        uint16_t dmaSettingsSize = c_dmaBytesPerPixelBytes * settingsSize;
        uint16_t dmaPixelSize = c_dmaBytesPerPixelBytes * elementSize;
        uint16_t resetSize = c_dmaBytesPerPixelBytes * T_SPEED::ResetTimeUs / T_SPEED::ByteSendTimeUs;

        _i2sBufferSize = pixelCount * dmaPixelSize + dmaSettingsSize + resetSize;

        // must have a 4 byte aligned buffer for i2s
        uint32_t alignment = _i2sBufferSize % 4;
        if (alignment)
        {
            _i2sBufferSize += 4 - alignment;
        }

        _data = static_cast<uint8_t*>(malloc(_sizeData));
        // data cleared later in Begin()

        _i2sBuffer = static_cast<uint8_t*>(heap_caps_malloc(_i2sBufferSize, MALLOC_CAP_DMA));
        // no need to initialize all of it, but since it contains
        // "reset" bits that don't latter get overwritten we just clear it all
        memset(_i2sBuffer, 0x00, _i2sBufferSize);
    }

    void FillBuffers()
    {
        const uint16_t bitpatterns[16] =
        {
            0b1000100010001000, 0b1000100010001110, 0b1000100011101000, 0b1000100011101110,
            0b1000111010001000, 0b1000111010001110, 0b1000111011101000, 0b1000111011101110,
            0b1110100010001000, 0b1110100010001110, 0b1110100011101000, 0b1110100011101110,
            0b1110111010001000, 0b1110111010001110, 0b1110111011101000, 0b1110111011101110,
        };

        uint16_t* pDma = reinterpret_cast<uint16_t*>(_i2sBuffer);
        uint8_t* pEnd = _data + _sizeData;
        for (uint8_t* pPixel = _data; pPixel < pEnd; pPixel++)
        {
            *(pDma++) = bitpatterns[((*pPixel) & 0x0f)];
            *(pDma++) = bitpatterns[((*pPixel) >> 4) & 0x0f];
        }
    }
};

typedef NeoEsp32I2sMethodBase<NeoEsp32I2sSpeedWs2812x, NeoEsp32I2sBusZero, NeoEsp32I2sNotInverted> NeoEsp32I2s0Ws2812xMethod;
typedef NeoEsp32I2sMethodBase<NeoEsp32I2sSpeedSk6812, NeoEsp32I2sBusZero, NeoEsp32I2sNotInverted> NeoEsp32I2s0Sk6812Method;
typedef NeoEsp32I2sMethodBase<NeoEsp32I2sSpeedTm1814, NeoEsp32I2sBusZero, NeoEsp32I2sInverted> NeoEsp32I2s0Tm1814Method;
typedef NeoEsp32I2sMethodBase<NeoEsp32I2sSpeedTm1829, NeoEsp32I2sBusZero, NeoEsp32I2sInverted> NeoEsp32I2s0Tm1829Method;
typedef NeoEsp32I2sMethodBase<NeoEsp32I2sSpeedTm1914, NeoEsp32I2sBusZero, NeoEsp32I2sInverted> NeoEsp32I2s0Tm1914Method;
typedef NeoEsp32I2sMethodBase<NeoEsp32I2sSpeed800Kbps, NeoEsp32I2sBusZero, NeoEsp32I2sNotInverted> NeoEsp32I2s0800KbpsMethod;
typedef NeoEsp32I2sMethodBase<NeoEsp32I2sSpeed400Kbps, NeoEsp32I2sBusZero, NeoEsp32I2sNotInverted> NeoEsp32I2s0400KbpsMethod;
typedef NeoEsp32I2sMethodBase<NeoEsp32I2sSpeedApa106, NeoEsp32I2sBusZero, NeoEsp32I2sNotInverted> NeoEsp32I2s0Apa106Method;


typedef NeoEsp32I2sMethodBase<NeoEsp32I2sSpeedWs2812x, NeoEsp32I2sBusZero, NeoEsp32I2sInverted> NeoEsp32I2s0Ws2812xInvertedMethod;
typedef NeoEsp32I2sMethodBase<NeoEsp32I2sSpeedSk6812, NeoEsp32I2sBusZero, NeoEsp32I2sInverted> NeoEsp32I2s0Sk6812InvertedMethod;
typedef NeoEsp32I2sMethodBase<NeoEsp32I2sSpeedTm1814, NeoEsp32I2sBusZero, NeoEsp32I2sNotInverted> NeoEsp32I2s0Tm1814InvertedMethod;
typedef NeoEsp32I2sMethodBase<NeoEsp32I2sSpeedTm1914, NeoEsp32I2sBusZero, NeoEsp32I2sNotInverted> NeoEsp32I2s0Tm1914InvertedMethod;
typedef NeoEsp32I2sMethodBase<NeoEsp32I2sSpeedTm1829, NeoEsp32I2sBusZero, NeoEsp32I2sNotInverted> NeoEsp32I2s0Tm1829InvertedMethod;
typedef NeoEsp32I2sMethodBase<NeoEsp32I2sSpeed800Kbps, NeoEsp32I2sBusZero, NeoEsp32I2sInverted> NeoEsp32I2s0800KbpsInvertedMethod;
typedef NeoEsp32I2sMethodBase<NeoEsp32I2sSpeed400Kbps, NeoEsp32I2sBusZero, NeoEsp32I2sInverted> NeoEsp32I2s0400KbpsInvertedMethod;
typedef NeoEsp32I2sMethodBase<NeoEsp32I2sSpeedApa106, NeoEsp32I2sBusZero, NeoEsp32I2sInverted> NeoEsp32I2s0Apa106InvertedMethod;

#if !defined(CONFIG_IDF_TARGET_ESP32S2) && !defined(CONFIG_IDF_TARGET_ESP32C3)
// (I2S_NUM_MAX == 2)

typedef NeoEsp32I2sMethodBase<NeoEsp32I2sSpeedWs2812x, NeoEsp32I2sBusOne, NeoEsp32I2sNotInverted> NeoEsp32I2s1Ws2812xMethod;
typedef NeoEsp32I2sMethodBase<NeoEsp32I2sSpeedSk6812, NeoEsp32I2sBusOne, NeoEsp32I2sNotInverted> NeoEsp32I2s1Sk6812Method;
typedef NeoEsp32I2sMethodBase<NeoEsp32I2sSpeedTm1814, NeoEsp32I2sBusOne, NeoEsp32I2sInverted> NeoEsp32I2s1Tm1814Method;
typedef NeoEsp32I2sMethodBase<NeoEsp32I2sSpeedTm1829, NeoEsp32I2sBusOne, NeoEsp32I2sInverted> NeoEsp32I2s1Tm1829Method;
typedef NeoEsp32I2sMethodBase<NeoEsp32I2sSpeedTm1914, NeoEsp32I2sBusOne, NeoEsp32I2sInverted> NeoEsp32I2s1Tm1914Method;
typedef NeoEsp32I2sMethodBase<NeoEsp32I2sSpeed800Kbps, NeoEsp32I2sBusOne, NeoEsp32I2sNotInverted> NeoEsp32I2s1800KbpsMethod;
typedef NeoEsp32I2sMethodBase<NeoEsp32I2sSpeed400Kbps, NeoEsp32I2sBusOne, NeoEsp32I2sNotInverted> NeoEsp32I2s1400KbpsMethod;
typedef NeoEsp32I2sMethodBase<NeoEsp32I2sSpeedApa106, NeoEsp32I2sBusOne, NeoEsp32I2sNotInverted> NeoEsp32I2s1Apa106Method;

typedef NeoEsp32I2sMethodBase<NeoEsp32I2sSpeedWs2812x, NeoEsp32I2sBusOne, NeoEsp32I2sInverted> NeoEsp32I2s1Ws2812xInvertedMethod;
typedef NeoEsp32I2sMethodBase<NeoEsp32I2sSpeedSk6812, NeoEsp32I2sBusOne, NeoEsp32I2sInverted> NeoEsp32I2s1Sk6812InvertedMethod;
typedef NeoEsp32I2sMethodBase<NeoEsp32I2sSpeedTm1814, NeoEsp32I2sBusOne, NeoEsp32I2sNotInverted> NeoEsp32I2s1Tm1814InvertedMethod;
typedef NeoEsp32I2sMethodBase<NeoEsp32I2sSpeedTm1829, NeoEsp32I2sBusOne, NeoEsp32I2sNotInverted> NeoEsp32I2s1Tm1829InvertedMethod;
typedef NeoEsp32I2sMethodBase<NeoEsp32I2sSpeedTm1914, NeoEsp32I2sBusOne, NeoEsp32I2sNotInverted> NeoEsp32I2s1Tm1914InvertedMethod;
typedef NeoEsp32I2sMethodBase<NeoEsp32I2sSpeed800Kbps, NeoEsp32I2sBusOne, NeoEsp32I2sInverted> NeoEsp32I2s1800KbpsInvertedMethod;
typedef NeoEsp32I2sMethodBase<NeoEsp32I2sSpeed400Kbps, NeoEsp32I2sBusOne, NeoEsp32I2sInverted> NeoEsp32I2s1400KbpsInvertedMethod;
typedef NeoEsp32I2sMethodBase<NeoEsp32I2sSpeedApa106, NeoEsp32I2sBusOne, NeoEsp32I2sInverted> NeoEsp32I2s1Apa106InvertedMethod;


typedef NeoEsp32I2sMethodBase<NeoEsp32I2sSpeedWs2812x, NeoEsp32I2sBusN, NeoEsp32I2sNotInverted> NeoEsp32I2sNWs2812xMethod;
typedef NeoEsp32I2sMethodBase<NeoEsp32I2sSpeedSk6812, NeoEsp32I2sBusN, NeoEsp32I2sNotInverted> NeoEsp32I2sNSk6812Method;
typedef NeoEsp32I2sMethodBase<NeoEsp32I2sSpeedTm1814, NeoEsp32I2sBusN, NeoEsp32I2sInverted> NeoEsp32I2sNTm1814Method;
typedef NeoEsp32I2sMethodBase<NeoEsp32I2sSpeedTm1829, NeoEsp32I2sBusN, NeoEsp32I2sInverted> NeoEsp32I2sNTm1829Method;
typedef NeoEsp32I2sMethodBase<NeoEsp32I2sSpeedTm1914, NeoEsp32I2sBusN, NeoEsp32I2sInverted> NeoEsp32I2sNTm1914Method;
typedef NeoEsp32I2sMethodBase<NeoEsp32I2sSpeed800Kbps, NeoEsp32I2sBusN, NeoEsp32I2sNotInverted> NeoEsp32I2sN800KbpsMethod;
typedef NeoEsp32I2sMethodBase<NeoEsp32I2sSpeed400Kbps, NeoEsp32I2sBusN, NeoEsp32I2sNotInverted> NeoEsp32I2sN400KbpsMethod;
typedef NeoEsp32I2sMethodBase<NeoEsp32I2sSpeedApa106, NeoEsp32I2sBusN, NeoEsp32I2sNotInverted> NeoEsp32I2sNApa106Method;

typedef NeoEsp32I2sMethodBase<NeoEsp32I2sSpeedWs2812x, NeoEsp32I2sBusN, NeoEsp32I2sInverted> NeoEsp32I2sNWs2812xInvertedMethod;
typedef NeoEsp32I2sMethodBase<NeoEsp32I2sSpeedSk6812, NeoEsp32I2sBusN, NeoEsp32I2sInverted> NeoEsp32I2sNSk6812InvertedMethod;
typedef NeoEsp32I2sMethodBase<NeoEsp32I2sSpeedTm1814, NeoEsp32I2sBusN, NeoEsp32I2sNotInverted> NeoEsp32I2sNTm1814InvertedMethod;
typedef NeoEsp32I2sMethodBase<NeoEsp32I2sSpeedTm1829, NeoEsp32I2sBusN, NeoEsp32I2sNotInverted> NeoEsp32I2sNTm1829InvertedMethod;
typedef NeoEsp32I2sMethodBase<NeoEsp32I2sSpeedTm1914, NeoEsp32I2sBusN, NeoEsp32I2sNotInverted> NeoEsp32I2sNTm1914InvertedMethod;
typedef NeoEsp32I2sMethodBase<NeoEsp32I2sSpeed800Kbps, NeoEsp32I2sBusN, NeoEsp32I2sInverted> NeoEsp32I2sN800KbpsInvertedMethod;
typedef NeoEsp32I2sMethodBase<NeoEsp32I2sSpeed400Kbps, NeoEsp32I2sBusN, NeoEsp32I2sInverted> NeoEsp32I2sN400KbpsInvertedMethod;
typedef NeoEsp32I2sMethodBase<NeoEsp32I2sSpeedApa106, NeoEsp32I2sBusN, NeoEsp32I2sInverted> NeoEsp32I2sNApa106InvertedMethod;

#endif

#if !defined(NEOPIXEL_ESP32_RMT_DEFAULT) && !defined(CONFIG_IDF_TARGET_ESP32S2) && !defined(CONFIG_IDF_TARGET_ESP32C3)

// I2s Bus 1 method is the default method for Esp32
// Esp32S2 & Esp32C3 will use RMT as the default allways
typedef NeoEsp32I2s1Ws2812xMethod NeoWs2813Method;
typedef NeoEsp32I2s1Ws2812xMethod NeoWs2812xMethod;
typedef NeoEsp32I2s1800KbpsMethod NeoWs2812Method;
typedef NeoEsp32I2s1Ws2812xMethod NeoWs2811Method;
typedef NeoEsp32I2s1Sk6812Method NeoSk6812Method;
typedef NeoEsp32I2s1Tm1814Method NeoTm1814Method;
typedef NeoEsp32I2s1Tm1829Method NeoTm1829Method;
typedef NeoEsp32I2s1Tm1914Method NeoTm1914Method;
typedef NeoEsp32I2s1Sk6812Method NeoLc8812Method;
typedef NeoEsp32I2s1Apa106Method NeoApa106Method;

typedef NeoEsp32I2s1Ws2812xMethod Neo800KbpsMethod;
typedef NeoEsp32I2s1400KbpsMethod Neo400KbpsMethod;

typedef NeoEsp32I2s1Ws2812xInvertedMethod NeoWs2813InvertedMethod;
typedef NeoEsp32I2s1Ws2812xInvertedMethod NeoWs2812xInvertedMethod;
typedef NeoEsp32I2s1Ws2812xInvertedMethod NeoWs2811InvertedMethod;
typedef NeoEsp32I2s1800KbpsInvertedMethod NeoWs2812InvertedMethod;
typedef NeoEsp32I2s1Sk6812InvertedMethod NeoSk6812InvertedMethod;
typedef NeoEsp32I2s1Tm1814InvertedMethod NeoTm1814InvertedMethod;
typedef NeoEsp32I2s1Tm1829InvertedMethod NeoTm1829InvertedMethod;
typedef NeoEsp32I2s1Tm1914InvertedMethod NeoTm1914InvertedMethod;
typedef NeoEsp32I2s1Sk6812InvertedMethod NeoLc8812InvertedMethod;
typedef NeoEsp32I2s1Apa106InvertedMethod NeoApa106InvertedMethod;

typedef NeoEsp32I2s1Ws2812xInvertedMethod Neo800KbpsInvertedMethod;
typedef NeoEsp32I2s1400KbpsInvertedMethod Neo400KbpsInvertedMethod;

#endif // !defined(NEOPIXEL_ESP32_RMT_DEFAULT) && !defined(CONFIG_IDF_TARGET_ESP32S2) && !defined(CONFIG_IDF_TARGET_ESP32C3)
#endif
#endif // ESP_IDF_VERSION_MAJOR < 5
