/*-------------------------------------------------------------------------
NeoPixel library helper functions for Esp8266.

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

#ifdef ARDUINO_ARCH_ESP8266

// due to linker overriding the ICACHE_RAM_ATTR for cpp files, these methods are
// moved into a C file so the attribute will be applied correctly
extern "C" void ICACHE_RAM_ATTR bitbang_send_pixels_800(uint8_t* pixels, uint8_t* end, uint8_t pin);
extern "C" void ICACHE_RAM_ATTR bitbang_send_pixels_400(uint8_t* pixels, uint8_t* end, uint8_t pin);

class NeoEsp8266BitBangSpeed800Kbps
{
public:
    static void send_pixels(uint8_t* pixels, uint8_t* end, uint8_t pin)
    {
        bitbang_send_pixels_800(pixels, end, pin);
    }
};

class NeoEsp8266BitBangSpeed400Kbps
{
public:
    static void send_pixels(uint8_t* pixels, uint8_t* end, uint8_t pin)
    {
        bitbang_send_pixels_400(pixels, end, pin);
    }
};

template<typename T_SPEED> class NeoEsp8266BitBangMethodBase
{
public:
    NeoEsp8266BitBangMethodBase(uint8_t pin, uint16_t pixelCount, size_t elementSize) :
        _pin(pin)
    {
        pinMode(pin, OUTPUT);

        _sizePixels = pixelCount * elementSize;
        _pixels = (uint8_t*)malloc(_sizePixels);
        memset(_pixels, 0, _sizePixels);
    }

    ~NeoEsp8266BitBangMethodBase()
    {
        pinMode(_pin, INPUT);

        free(_pixels);
    }

    bool IsReadyToUpdate() const
    {
        uint32_t delta = micros() - _endTime;

        return (delta >= 50L);
    }

    void Initialize()
    {
        digitalWrite(_pin, LOW);

        _endTime = micros();
    }

    void Update()
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

        noInterrupts(); // Need 100% focus on instruction timing

        T_SPEED::send_pixels(_pixels, _pixels + _sizePixels, _pin);

        interrupts();

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

typedef NeoEsp8266BitBangMethodBase<NeoEsp8266BitBangSpeed800Kbps> NeoEsp8266BitBang800KbpsMethod;
typedef NeoEsp8266BitBangMethodBase<NeoEsp8266BitBangSpeed400Kbps> NeoEsp8266BitBang400KbpsMethod;

#endif