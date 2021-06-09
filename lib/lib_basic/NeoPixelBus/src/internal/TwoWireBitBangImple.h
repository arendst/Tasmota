/*-------------------------------------------------------------------------
NeoPixel library helper functions for DotStars using general Pins (APA102/LPD8806).

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


class TwoWireBitBangImple
{
public:
    typedef NeoNoSettings SettingsObject;

    TwoWireBitBangImple(uint8_t pinClock, uint8_t pinData) :
        _pinClock(pinClock),
        _pinData(pinData)
    {
        pinMode(pinClock, OUTPUT);
        pinMode(pinData, OUTPUT);
    }

    ~TwoWireBitBangImple()
    {
        pinMode(_pinClock, INPUT);
        pinMode(_pinData, INPUT);
    }

    void begin()
    {
        digitalWrite(_pinClock, LOW);
        digitalWrite(_pinData, LOW);
    }

    void beginTransaction()
    {

    }

    void endTransaction()
    {
        digitalWrite(_pinData, LOW);
    }

    void transmitByte(uint8_t data)
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

    void transmitBytes(const uint8_t* data, size_t dataSize)
    {
        const uint8_t* endData = data + dataSize;
        while (data < endData)
        {
            transmitByte(*data++);
        }
    }

    void applySettings(const SettingsObject& settings)
    {
    }

private:
    const uint8_t  _pinClock;     // output pin number for clock line
    const uint8_t  _pinData;      // output pin number for data line
};