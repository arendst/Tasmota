/*-------------------------------------------------------------------------
NeoPixel library helper functions for LPD6803 using general Pins 

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

// must also check for arm due to Teensy incorrectly having ARDUINO_ARCH_AVR set
#if defined(ARDUINO_ARCH_AVR) && !defined(__arm__)
#include "TwoWireBitBangImpleAvr.h"
#else
#include "TwoWireBitBangImple.h"
#endif


template<typename T_TWOWIRE> class Lpd6803MethodBase
{
public:
    typedef typename T_TWOWIRE::SettingsObject SettingsObject;

    Lpd6803MethodBase(uint8_t pinClock, uint8_t pinData, uint16_t pixelCount, size_t elementSize, size_t settingsSize) :
        _sizeData(pixelCount * elementSize + settingsSize),
		_sizeFrame((pixelCount + 7) / 8), // bit for every pixel at least
		_wire(pinClock, pinData)
    {
        _data = static_cast<uint8_t*>(malloc(_sizeData));
        // data cleared later in Begin()
    }

#if !defined(__AVR_ATtiny85__) && !defined(ARDUINO_attiny)
    Lpd6803MethodBase(uint16_t pixelCount, size_t elementSize, size_t settingsSize) :
        Lpd6803MethodBase(SCK, MOSI, pixelCount, elementSize, settingsSize)
    {
    }
#endif


    ~Lpd6803MethodBase()
    {
        free(_data);
    }

    bool IsReadyToUpdate() const
    {
        return true; // dot stars don't have a required delay
    }

#if defined(ARDUINO_ARCH_ESP32)
    void Initialize(int8_t sck, int8_t miso, int8_t mosi, int8_t ss)
    {
        _wire.begin(sck, miso, mosi, ss);
    }
#endif

    void Initialize()
    {
        _wire.begin();
    }

    void Update(bool)
    {
        const uint8_t startFrame[4] = { 0x00 };

        _wire.beginTransaction();

        // start frame
        _wire.transmitBytes(startFrame, sizeof(startFrame));
        
        // data
        _wire.transmitBytes(_data, _sizeData);
        
        // end frame 
        // one bit for every pixel with no less than 1 byte
        for (size_t frameByte = 0; frameByte < _sizeFrame; frameByte++)
        {
            _wire.transmitByte(0x00);
        }
    
        _wire.endTransaction();
    }

    uint8_t* getData() const
    {
        return _data;
    };

    size_t getDataSize() const
    {
        return _sizeData;
    };

    void applySettings(const SettingsObject& settings)
    {
        _wire.applySettings(settings);
    }

private:
    const size_t   _sizeData;   // Size of '_data' buffer below
    const size_t   _sizeFrame;

    T_TWOWIRE _wire;
    uint8_t* _data;       // Holds LED color values
};

typedef Lpd6803MethodBase<TwoWireBitBangImple> Lpd6803Method;

#if !defined(__AVR_ATtiny85__) && !defined(ARDUINO_attiny)
#include "TwoWireSpiImple.h"
typedef Lpd6803MethodBase<TwoWireSpiImple<SpiSpeed20Mhz>> Lpd6803Spi20MhzMethod;
typedef Lpd6803MethodBase<TwoWireSpiImple<SpiSpeed10Mhz>> Lpd6803Spi10MhzMethod;
typedef Lpd6803MethodBase<TwoWireSpiImple<SpiSpeed5Mhz>> Lpd6803Spi5MhzMethod;
typedef Lpd6803MethodBase<TwoWireSpiImple<SpiSpeed2Mhz>> Lpd6803Spi2MhzMethod;
typedef Lpd6803MethodBase<TwoWireSpiImple<SpiSpeed1Mhz>> Lpd6803Spi1MhzMethod;
typedef Lpd6803MethodBase<TwoWireSpiImple<SpiSpeed500Khz>> Lpd6803Spi500KhzMethod;

typedef Lpd6803MethodBase<TwoWireSpiImple<SpiSpeedHz>> Lpd6803SpiHzMethod;

typedef Lpd6803Spi10MhzMethod Lpd6803SpiMethod;
#endif



