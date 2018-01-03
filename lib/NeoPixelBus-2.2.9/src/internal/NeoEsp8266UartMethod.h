/*-------------------------------------------------------------------------
NeoPixel library helper functions for Esp8266 UART hardware

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
#include <Arduino.h>

// NeoEsp8266Uart contains all the low level details that doesn't
// depend on the transmission speed, and therefore, it isn't a template
class NeoEsp8266Uart
{
protected:
    NeoEsp8266Uart(uint16_t pixelCount, size_t elementSize);

    ~NeoEsp8266Uart();

    void InitializeUart(uint32_t uartBaud);

    void UpdateUart();

    static const uint8_t* ICACHE_RAM_ATTR FillUartFifo(const uint8_t* pixels, const uint8_t* end);

    size_t    _sizePixels;   // Size of '_pixels' buffer below
    uint8_t* _pixels;        // Holds LED color values
    uint32_t _startTime;     // Microsecond count when last update started
};

// NeoEsp8266AsyncUart handles all transmission asynchronously using interrupts
//
// This UART controller uses two buffers that are swapped in every call to
// NeoPixelBus.Show(). One buffer contains the data that is being sent
// asynchronosly and another buffer contains the data that will be send
// in the next call to NeoPixelBus.Show().
//
// Therefore, the result of NeoPixelBus.Pixels() is invalidated after
// every call to NeoPixelBus.Show() and must not be cached.
class NeoEsp8266AsyncUart: public NeoEsp8266Uart
{
protected:
    NeoEsp8266AsyncUart(uint16_t pixelCount, size_t elementSize);

    ~NeoEsp8266AsyncUart();

    void InitializeUart(uint32_t uartBaud);

    void UpdateUart();

private:
    static void ICACHE_RAM_ATTR IntrHandler(void* param);

    uint8_t* _asyncPixels;  // Holds a copy of LED color values taken when UpdateUart began
};

// NeoEsp8266UartSpeedWs2813 contains the timing constants used to get NeoPixelBus running with the Ws2813
class NeoEsp8266UartSpeedWs2813
{
public:
    static const uint32_t ByteSendTimeUs = 10; // us it takes to send a single pixel element at 800khz speed
    static const uint32_t UartBaud = 3200000; // 800mhz, 4 serial bytes per NeoByte
    static const uint32_t ResetTimeUs = 250; // us between data send bursts to reset for next update
};

// NeoEsp8266UartSpeed800Kbps contains the timing constant used to get NeoPixelBus running at 800Khz
class NeoEsp8266UartSpeed800Kbps
{
public:
    static const uint32_t ByteSendTimeUs =  10; // us it takes to send a single pixel element at 800khz speed
    static const uint32_t UartBaud = 3200000; // 800mhz, 4 serial bytes per NeoByte
    static const uint32_t ResetTimeUs = 50; // us between data send bursts to reset for next update
};

// NeoEsp8266UartSpeed800Kbps contains the timing constant used to get NeoPixelBus running at 400Khz
class NeoEsp8266UartSpeed400Kbps
{
public:
    static const uint32_t ByteSendTimeUs = 20; // us it takes to send a single pixel element at 400khz speed
    static const uint32_t UartBaud = 1600000; // 400mhz, 4 serial bytes per NeoByte
    static const uint32_t ResetTimeUs = 50; // us between data send bursts to reset for next update
};

// NeoEsp8266UartMethodBase is a light shell arround NeoEsp8266Uart or NeoEsp8266AsyncUart that
// implements the methods needed to operate as a NeoPixelBus method.
template<typename T_SPEED, typename T_BASE>
class NeoEsp8266UartMethodBase: public T_BASE
{
public:
    NeoEsp8266UartMethodBase(uint16_t pixelCount, size_t elementSize)
        : T_BASE(pixelCount, elementSize)
    {
    }
    NeoEsp8266UartMethodBase(uint8_t pin, uint16_t pixelCount, size_t elementSize)
        : T_BASE(pixelCount, elementSize)
    {
    }

    bool IsReadyToUpdate() const
    {
        uint32_t delta = micros() - this->_startTime;
        return delta >= getPixelTime() + T_SPEED::ResetTimeUs;
    }

    void Initialize()
    {
        this->InitializeUart(T_SPEED::UartBaud);

        // Inverting logic levels can generate a phantom bit in the led strip bus
        // We need to delay 50+ microseconds the output stream to force a data
        // latch and discard this bit. Otherwise, that bit would be prepended to
        // the first frame corrupting it.
        this->_startTime = micros() - getPixelTime();
    }

    void Update()
    {
        // Data latch = 50+ microsecond pause in the output stream.  Rather than
        // put a delay at the end of the function, the ending time is noted and
        // the function will simply hold off (if needed) on issuing the
        // subsequent round of data until the latch time has elapsed.  This
        // allows the mainline code to start generating the next frame of data
        // rather than stalling for the latch.
        while (!this->IsReadyToUpdate())
        {
            yield();
        }
        this->UpdateUart();
    }

    uint8_t* getPixels() const
    {
        return this->_pixels;
    };

    size_t getPixelsSize() const
    {
        return this->_sizePixels;
    };

private:
    uint32_t getPixelTime() const
    {
        return (T_SPEED::ByteSendTimeUs * this->_sizePixels);
    };
};

typedef NeoEsp8266UartMethodBase<NeoEsp8266UartSpeedWs2813, NeoEsp8266Uart> NeoEsp8266UartWs2813Method;
typedef NeoEsp8266UartMethodBase<NeoEsp8266UartSpeed800Kbps, NeoEsp8266Uart> NeoEsp8266Uart800KbpsMethod;
typedef NeoEsp8266UartMethodBase<NeoEsp8266UartSpeed400Kbps, NeoEsp8266Uart> NeoEsp8266Uart400KbpsMethod;

typedef NeoEsp8266UartMethodBase<NeoEsp8266UartSpeedWs2813, NeoEsp8266AsyncUart> NeoEsp8266AsyncUartWs2813Method;
typedef NeoEsp8266UartMethodBase<NeoEsp8266UartSpeed800Kbps, NeoEsp8266AsyncUart> NeoEsp8266AsyncUart800KbpsMethod;
typedef NeoEsp8266UartMethodBase<NeoEsp8266UartSpeed400Kbps, NeoEsp8266AsyncUart> NeoEsp8266AsyncUart400KbpsMethod;

#endif

