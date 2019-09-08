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

#if defined(ARDUINO_ARCH_ESP32)
    void Initialize(int8_t sck, int8_t miso, int8_t mosi, int8_t ss)
    {
        SPI.begin(sck, miso, mosi, ss);
    }
#endif

    void Initialize()
    {
        SPI.begin();
    }

    void Update(bool)
    {
        SPI.beginTransaction(SPISettings(20000000L, MSBFIRST, SPI_MODE0));
#if defined(ARDUINO_ARCH_ESP8266) || defined(ARDUINO_ARCH_ESP32)
        // ESPs have a method to write without inplace overwriting the send buffer
        // since we don't care what gets received, use it for performance
        SPI.writeBytes(_sendBuffer, _sizeSendBuffer);

#else
        // default ARDUINO transfer inplace overwrites the send buffer
        // which is bad, so we have to send one byte at a time
        uint8_t* out = _sendBuffer;
        uint8_t* end = out + _sizeSendBuffer;
        while (out < end)
        {
            SPI.transfer(*out++);
        }
#endif
        SPI.endTransaction();
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




