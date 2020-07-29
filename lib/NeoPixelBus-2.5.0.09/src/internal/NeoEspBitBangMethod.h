/*-------------------------------------------------------------------------
NeoPixel library helper functions for Esp8266 and Esp32

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

#if defined(ARDUINO_ARCH_ESP8266) || defined(ARDUINO_ARCH_ESP32)

// ESP32 doesn't define ICACHE_RAM_ATTR
#ifndef ICACHE_RAM_ATTR
#define ICACHE_RAM_ATTR IRAM_ATTR
#endif

// for esp8266, due to linker overriding the ICACHE_RAM_ATTR for cpp files, these methods are
// moved into a C file so the attribute will be applied correctly
// >> this may have been fixed and is no longer a requirement <<
extern "C" void ICACHE_RAM_ATTR bitbang_send_pixels_800(uint8_t* pixels, uint8_t* end, uint8_t pin);
extern "C" void ICACHE_RAM_ATTR bitbang_send_pixels_400(uint8_t* pixels, uint8_t* end, uint8_t pin);

class NeoEspBitBangSpeedWs2812x
{
public:
    static void send_pixels(uint8_t* pixels, uint8_t* end, uint8_t pin)
    {
        bitbang_send_pixels_800(pixels, end, pin);
    }
    static const uint32_t ResetTimeUs = 300;
};

class NeoEspBitBangSpeedSk6812
{
public:
    static void send_pixels(uint8_t* pixels, uint8_t* end, uint8_t pin)
    {
        bitbang_send_pixels_800(pixels, end, pin);
    }
    static const uint32_t ResetTimeUs = 80;
};

class NeoEspBitBangSpeed800Kbps
{
public:
    static void send_pixels(uint8_t* pixels, uint8_t* end, uint8_t pin)
    {
        bitbang_send_pixels_800(pixels, end, pin);
    }
    static const uint32_t ResetTimeUs = 50; 
};

class NeoEspBitBangSpeed400Kbps
{
public:
    static void send_pixels(uint8_t* pixels, uint8_t* end, uint8_t pin)
    {
        bitbang_send_pixels_400(pixels, end, pin);
    }
    static const uint32_t ResetTimeUs = 50;
};

template<typename T_SPEED> class NeoEspBitBangMethodBase
{
public:
    NeoEspBitBangMethodBase(uint8_t pin, uint16_t pixelCount, size_t elementSize) :
        _pin(pin)
    {
        pinMode(pin, OUTPUT);

        _sizePixels = pixelCount * elementSize;
        _pixels = (uint8_t*)malloc(_sizePixels);
        memset(_pixels, 0, _sizePixels);
    }

    ~NeoEspBitBangMethodBase()
    {
        pinMode(_pin, INPUT);

        free(_pixels);
    }

    bool IsReadyToUpdate() const
    {
        uint32_t delta = micros() - _endTime;

        return (delta >= T_SPEED::ResetTimeUs);
    }

    void Initialize()
    {
        digitalWrite(_pin, LOW);

        _endTime = micros();
    }

    void Update(bool)
    {
        // Data latch = 50+ microsecond pause in the output stream.  Rather than
        // put a delay at the end of the function, the ending time is noted and
        // the function will simply hold off (if needed) on issuing the
        // subsequent round of data until the latch time has elapsed.  This
        // allows the mainline code to start generating the next frame of data
        // rather than stalling for the latch.
        while (!IsReadyToUpdate())
        {
            yield(); // allows for system yield if needed
        }

		// Need 100% focus on instruction timing
#if defined(ARDUINO_ARCH_ESP32)
		delay(1); // required
		portMUX_TYPE updateMux = portMUX_INITIALIZER_UNLOCKED;

        portENTER_CRITICAL(&updateMux);
#else
        noInterrupts(); 
#endif

        T_SPEED::send_pixels(_pixels, _pixels + _sizePixels, _pin);
		
#if defined(ARDUINO_ARCH_ESP32)
        portEXIT_CRITICAL(&updateMux);
#else
        interrupts();
#endif

        // save EOD time for latch on next call
        _endTime = micros();
    }

    uint8_t* getPixels() const
    {
        return _pixels;
    };

    size_t getPixelsSize() const
    {
        return _sizePixels;
    };

private:
    uint32_t _endTime;       // Latch timing reference
    size_t    _sizePixels;   // Size of '_pixels' buffer below
    uint8_t* _pixels;        // Holds LED color values
    uint8_t _pin;            // output pin number
};


#if defined(ARDUINO_ARCH_ESP32)

typedef NeoEspBitBangMethodBase<NeoEspBitBangSpeedWs2812x> NeoEsp32BitBangWs2812xMethod;
typedef NeoEspBitBangMethodBase<NeoEspBitBangSpeedSk6812> NeoEsp32BitBangSk6812Method;
typedef NeoEspBitBangMethodBase<NeoEspBitBangSpeed800Kbps> NeoEsp32BitBang800KbpsMethod;
typedef NeoEspBitBangMethodBase<NeoEspBitBangSpeed400Kbps> NeoEsp32BitBang400KbpsMethod;

typedef NeoEsp32BitBangWs2812xMethod NeoEsp32BitBangWs2813Method;
typedef NeoEsp32BitBang800KbpsMethod NeoEsp32BitBangWs2812Method;
typedef NeoEsp32BitBangSk6812Method NeoEsp32BitBangLc8812Method;
typedef NeoEsp32BitBang400KbpsMethod NeoEsp32BitBangApa106Method;

#else

typedef NeoEspBitBangMethodBase<NeoEspBitBangSpeedWs2812x> NeoEsp8266BitBangWs2812xMethod;
typedef NeoEspBitBangMethodBase<NeoEspBitBangSpeedSk6812> NeoEsp8266BitBangSk6812Method;
typedef NeoEspBitBangMethodBase<NeoEspBitBangSpeed800Kbps> NeoEsp8266BitBang800KbpsMethod;
typedef NeoEspBitBangMethodBase<NeoEspBitBangSpeed400Kbps> NeoEsp8266BitBang400KbpsMethod;

typedef NeoEsp8266BitBangWs2812xMethod NeoEsp8266BitBangWs2813Method;
typedef NeoEsp8266BitBang800KbpsMethod NeoEsp8266BitBangWs2812Method;
typedef NeoEsp8266BitBangSk6812Method NeoEsp8266BitBangLc8812Method;
typedef NeoEsp8266BitBang400KbpsMethod NeoEsp8266BitBangApa106Method;
#endif

// ESP bitbang doesn't have defaults and should avoided except for testing
#endif