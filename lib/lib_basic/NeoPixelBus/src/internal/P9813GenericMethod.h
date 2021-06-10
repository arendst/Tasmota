/*-------------------------------------------------------------------------
NeoPixel library helper functions for P9813s using general Pins (APA102).

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


template<typename T_TWOWIRE> class P9813MethodBase
{
public:
    typedef typename T_TWOWIRE::SettingsObject SettingsObject;

    P9813MethodBase(uint8_t pinClock, uint8_t pinData, uint16_t pixelCount, size_t elementSize, size_t settingsSize) :
        _sizeData(pixelCount * elementSize + settingsSize),
        _sizeEndFrame((pixelCount + 15) / 16), // 16 = div 2 (bit for every two pixels) div 8 (bits to bytes)
        _wire(pinClock, pinData)
    {
        _data = static_cast<uint8_t*>(malloc(_sizeData));
        // data cleared later in Begin()
    }

#if !defined(__AVR_ATtiny85__) && !defined(ARDUINO_attiny)
    P9813MethodBase(uint16_t pixelCount, size_t elementSize, size_t settingsSize) :
        P9813MethodBase(SCK, MOSI, pixelCount, elementSize, settingsSize)
    {
    }
#endif

    ~P9813MethodBase()
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
        const uint8_t endFrame[4] = { 0x00 };
        
        _wire.beginTransaction();

        // start frame
        _wire.transmitBytes(startFrame, sizeof(startFrame));
        
        // data
        _wire.transmitBytes(_data, _sizeData);
        
        // end frame 
        _wire.transmitBytes(endFrame, sizeof(endFrame));        
        
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
    const size_t   _sizeEndFrame;

    T_TWOWIRE _wire;
    uint8_t* _data;       // Holds LED color values
};

typedef P9813MethodBase<TwoWireBitBangImple> P9813Method;

#if !defined(__AVR_ATtiny85__) && !defined(ARDUINO_attiny)
#include "TwoWireSpiImple.h"
typedef P9813MethodBase<TwoWireSpiImple<SpiSpeed20Mhz>> P9813Spi20MhzMethod;
typedef P9813MethodBase<TwoWireSpiImple<SpiSpeed10Mhz>> P9813Spi10MhzMethod;
typedef P9813MethodBase<TwoWireSpiImple<SpiSpeed5Mhz>> P9813Spi5MhzMethod;
typedef P9813MethodBase<TwoWireSpiImple<SpiSpeed2Mhz>> P9813Spi2MhzMethod;
typedef P9813MethodBase<TwoWireSpiImple<SpiSpeed1Mhz>> P9813Spi1MhzMethod;
typedef P9813MethodBase<TwoWireSpiImple<SpiSpeed500Khz>> P9813Spi500KhzMethod;

typedef P9813MethodBase<TwoWireSpiImple<SpiSpeedHz>> P9813SpiHzMethod;

typedef P9813Spi10MhzMethod P9813SpiMethod;
#endif



