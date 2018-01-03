/*-------------------------------------------------------------------------
NeoPixel library helper functions for DotStars using general Pins (APA102).

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

class DotStarGenericMethod
{
public:
    DotStarGenericMethod(uint8_t pinClock, uint8_t pinData, uint16_t pixelCount, size_t elementSize) :
        _pinClock(pinClock),
        _pinData(pinData),
        _sizePixels(pixelCount * elementSize)
    {
        pinMode(pinClock, OUTPUT);
        pinMode(pinData, OUTPUT);

        _pixels = (uint8_t*)malloc(_sizePixels);
        memset(_pixels, 0, _sizePixels);
    }

    ~DotStarGenericMethod()
    {
        pinMode(_pinClock, INPUT);
        pinMode(_pinData, INPUT);

        free(_pixels);
    }

    bool IsReadyToUpdate() const
    {
        return true; // dot stars don't have a required delay
    }

    void Initialize()
    {
        digitalWrite(_pinClock, LOW);
        digitalWrite(_pinData, LOW);
    }

    void Update()
    {
        // start frame
        for (int startFrameByte = 0; startFrameByte < 4; startFrameByte++)
        {
            _transmitByte(0x00);
        }
        
        // data
        uint8_t* data = _pixels;
        const uint8_t* endData = _pixels + _sizePixels;
        while (data < endData)
        {
            _transmitByte(*data++);
        }
        
        // end frame 
        // one bit for every two pixels with no less than 1 byte
        const uint16_t countEndFrameBytes = ((_sizePixels / 4) + 15) / 16;
        for (uint16_t endFrameByte = 0; endFrameByte < countEndFrameBytes; endFrameByte++)
        {
           _transmitByte(0xff);
        }

        // set clock and data back to low between updates
        digitalWrite(_pinData, LOW);
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
    const uint8_t  _pinClock;     // output pin number for clock line
    const uint8_t  _pinData;      // output pin number for data line
    const size_t   _sizePixels;   // Size of '_pixels' buffer below

    uint8_t* _pixels;       // Holds LED color values

    void _transmitByte(uint8_t data)
    {
        for (int bit = 7; bit >= 0; bit--)
        {
            // set data bit on pin
            digitalWrite(_pinData, (data & 0x80) == 0x80 ? HIGH : LOW);

            // set clock high as data is ready
            digitalWrite(_pinClock, HIGH);

            data <<= 1;

            // set clock low as data pin is changed
            digitalWrite(_pinClock, LOW);
        }
    }
};

typedef DotStarGenericMethod DotStarMethod;


