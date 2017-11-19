/*-------------------------------------------------------------------------
NeoPixel library helper functions for Atmel AVR.

Written by Michael C. Miller.
Some work taken from the Adafruit NeoPixel library.

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

#ifdef ARDUINO_ARCH_AVR

extern "C"
{
    void send_pixels_8mhz_800_PortD(uint8_t* pixels, size_t sizePixels, uint8_t pinMask);
    void send_pixels_8mhz_800_PortB(uint8_t* pixels, size_t sizePixels, uint8_t pinMask);
    void send_pixels_8mhz_400(uint8_t* pixels, size_t sizePixels, volatile uint8_t* port, uint8_t pinMask);
    void send_pixels_12mhz_800_PortD(uint8_t* pixels, size_t sizePixels, uint8_t pinMask);
    void send_pixels_12mhz_800_PortB(uint8_t* pixels, size_t sizePixels, uint8_t pinMask);
    void send_pixels_12mhz_400(uint8_t* pixels, size_t sizePixels, volatile uint8_t* port, uint8_t pinMask);
    void send_pixels_16mhz_800(uint8_t* pixels, size_t sizePixels, volatile uint8_t* port, uint8_t pinMask);
    void send_pixels_16mhz_400(uint8_t* pixels, size_t sizePixels, volatile uint8_t* port, uint8_t pinMask);
}

class NeoAvrSpeed800Kbps
{
public:
    static void send_pixels(uint8_t* pixels, size_t sizePixels, volatile uint8_t* port, uint8_t pinMask)
    {
#if (F_CPU >= 7400000UL) && (F_CPU <= 9500000UL)  // 8Mhz CPU
#ifdef PORTD // PORTD isn't present on ATtiny85, etc.
        if (port == &PORTD)
            send_pixels_8mhz_800_PortD(pixels, sizePixels, pinMask);
        else if (port == &PORTB)
#endif // PORTD
            send_pixels_8mhz_800_PortB(pixels, sizePixels, pinMask);

#elif (F_CPU >= 11100000UL) && (F_CPU <= 14300000UL)  // 12Mhz CPU
#ifdef PORTD // PORTD 
        if (port == &PORTD)
            send_pixels_12mhz_800_PortD(pixels, sizePixels, pinMask);
        else if (port == &PORTB)
#endif // PORTD
            send_pixels_12mhz_800_PortB(pixels, sizePixels, pinMask);

#elif (F_CPU >= 15400000UL) && (F_CPU <= 19000000L)  // 16Mhz CPU
        send_pixels_16mhz_800(pixels, sizePixels, port, pinMask);
#else
#error "CPU SPEED NOT SUPPORTED"
#endif
    }
};

class NeoAvrSpeed400Kbps
{
public:
    static void send_pixels(uint8_t* pixels, size_t sizePixels, volatile uint8_t* port, uint8_t pinMask)
    {
#if (F_CPU >= 7400000UL) && (F_CPU <= 9500000UL)  // 8Mhz CPU
        send_pixels_8mhz_400(pixels, sizePixels, port, pinMask);

#elif (F_CPU >= 11100000UL) && (F_CPU <= 14300000UL)  // 12Mhz CPU
        send_pixels_12mhz_400(pixels, sizePixels, port, pinMask);

#elif (F_CPU >= 15400000UL) && (F_CPU <= 19000000L)  // 16Mhz CPU
        send_pixels_16mhz_400(pixels, sizePixels, port, pinMask);
#else
#error "CPU SPEED NOT SUPPORTED"
#endif
    }
};

template<typename T_SPEED> class NeoAvrMethodBase
{
public:
    NeoAvrMethodBase(uint8_t pin, uint16_t pixelCount, size_t elementSize) :
        _pin(pin),
        _port(NULL),
        _pinMask(0)
    {
        pinMode(pin, OUTPUT);

        _sizePixels = pixelCount * elementSize;
        _pixels = (uint8_t*)malloc(_sizePixels);
        memset(_pixels, 0, _sizePixels);

        _port = portOutputRegister(digitalPinToPort(pin));
        _pinMask = digitalPinToBitMask(pin);
    }

    ~NeoAvrMethodBase()
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
#if !defined(ARDUINO_TEEONARDU_LEO) && !defined(ARDUINO_TEEONARDU_FLORA)
            yield(); // allows for system yield if needed
#endif
        }

        noInterrupts(); // Need 100% focus on instruction timing

        T_SPEED::send_pixels(_pixels, _sizePixels, _port, _pinMask);

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

    volatile uint8_t* _port;         // Output PORT register
    uint8_t  _pinMask;      // Output PORT bitmask
};

typedef NeoAvrMethodBase<NeoAvrSpeed800Kbps> NeoAvr800KbpsMethod;
typedef NeoAvrMethodBase<NeoAvrSpeed400Kbps> NeoAvr400KbpsMethod;

// AVR doesn't have alternatives yet, so there is just the default
typedef NeoAvr800KbpsMethod Neo800KbpsMethod;
typedef NeoAvr400KbpsMethod Neo400KbpsMethod;

#endif

