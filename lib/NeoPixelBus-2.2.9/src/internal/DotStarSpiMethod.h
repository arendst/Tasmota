/*-------------------------------------------------------------------------
NeoPixel library helper functions for DotStars using SPI hardware (APA102).

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

#include <SPI.h>

class DotStarSpiMethod
{
public:
    DotStarSpiMethod(uint16_t pixelCount, size_t elementSize) :
        _sizeData(pixelCount * elementSize),
        _sizeSendBuffer(calcBufferSize(pixelCount * elementSize))
    {
        _sendBuffer = (uint8_t*)malloc(_sizeSendBuffer);
        memset(_sendBuffer, 0, _sizeSendBuffer);
        setEndFrameBytes();
    }

    ~DotStarSpiMethod()
    {
        SPI.end();
        free(_sendBuffer);
    }

    bool IsReadyToUpdate() const
    {
        return true; // dot stars don't have a required delay
    }

    void Initialize()
    {
        SPI.begin();

#if defined(ARDUINO_ARCH_ESP8266)
        SPI.setFrequency(20000000L);
#elif defined(ARDUINO_ARCH_AVR) 
        SPI.setClockDivider(SPI_CLOCK_DIV2); // 8 MHz (6 MHz on Pro Trinket 3V)
#else
        SPI.setClockDivider((F_CPU + 4000000L) / 8000000L); // 8-ish MHz on Due
#endif
        SPI.setBitOrder(MSBFIRST);
        SPI.setDataMode(SPI_MODE0);
    }

    void Update()
    {
        // due to API inconsistencies need to call different methods on SPI
#if defined(ARDUINO_ARCH_ESP8266) || defined(ARDUINO_ARCH_ESP32)
        SPI.writeBytes(_sendBuffer, _sizeSendBuffer);
#else
        SPI.transfer(_sendBuffer, _sizeSendBuffer);
#endif
    }

    uint8_t* getPixels() const
    {
        return _sendBuffer + _countStartFrame;
    };

    size_t getPixelsSize() const
    {
        return _sizeData;
    };

private:
    const size_t _countStartFrame = 4;
    const size_t   _sizeData; // size of actuall pixel data within _sendBuffer
    const size_t   _sizeSendBuffer;   // Size of '_sendBuffer' buffer below

    uint8_t* _sendBuffer;       // Holds SPI send Buffer, including LED color values

    size_t calcBufferSize(size_t sizePixels) const
    {
        const size_t countEndFrameBytes = calcEndFrameSize(sizePixels);

        // start frame + data + end frame
        const size_t bufferSize = _countStartFrame + sizePixels + countEndFrameBytes;
        return bufferSize;
    }

    size_t calcEndFrameSize(size_t sizePixels) const
    {
        // end frame 
        // one bit for every two pixels with no less than 1 byte
        return ((sizePixels / 4) + 15) / 16;
    }

    void setEndFrameBytes()
    {
        uint8_t* pEndFrame = _sendBuffer + _countStartFrame + _sizeData;
        uint8_t* pEndFrameStop = pEndFrame + calcEndFrameSize(_sizeData);
        while (pEndFrame != pEndFrameStop)
        {
            *pEndFrame++ = 0xff;
        }
    }
};




