/*-------------------------------------------------------------------------
NeoPixel library helper functions for DotStars using SPI (APA102/LPD8806).

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

class SpiSpeed40Mhz
{
public:
    typedef NeoNoSettings SettingsObject;
    SpiSpeed40Mhz() {};

    static void applySettings(const SettingsObject& settings) {}

    static const uint32_t Clock = 40000000L;
};

class SpiSpeed20Mhz
{
public:
    typedef NeoNoSettings SettingsObject;
    SpiSpeed20Mhz() {};

    static void applySettings(const SettingsObject& settings) {}

    static const uint32_t Clock = 20000000L;
};

class SpiSpeed10Mhz
{
public:
    typedef NeoNoSettings SettingsObject;
    SpiSpeed10Mhz() {};

    static void applySettings(const SettingsObject& settings) {}

    static const uint32_t Clock = 10000000L;
};

class SpiSpeed5Mhz
{
public:
    typedef NeoNoSettings SettingsObject;
    SpiSpeed5Mhz() {};

    static void applySettings(const SettingsObject& settings) {}

    static const uint32_t Clock = 5000000L;
};

class SpiSpeed2Mhz
{
public:
    typedef NeoNoSettings SettingsObject;
    SpiSpeed2Mhz() {};

    static void applySettings(const SettingsObject& settings) {}

    static const uint32_t Clock = 2000000L;
};

class SpiSpeed1Mhz
{
public:
    typedef NeoNoSettings SettingsObject;
    SpiSpeed1Mhz() {};

    static void applySettings(const SettingsObject& settings) {}

    static const uint32_t Clock = 1000000L;
};

class SpiSpeed500Khz
{
public:
    typedef NeoNoSettings SettingsObject;
    SpiSpeed500Khz() {};

    static void applySettings(const SettingsObject& settings) {}

    static const uint32_t Clock = 500000L;
};

class NeoSpiSettings
{
public:
    NeoSpiSettings(uint32_t clock) :
        Clock(clock)
    {
    }
    uint32_t Clock;
};

class SpiSpeedHz
{
public:
    typedef NeoSpiSettings SettingsObject;

    SpiSpeedHz() :
        Clock(10000000)
    {};

    void applySettings(const SettingsObject& settings)
    {
        Clock = settings.Clock;
    }

    uint32_t Clock;
};

template<typename T_SPISPEED> class TwoWireSpiImple
{
public:
    typedef typename T_SPISPEED::SettingsObject SettingsObject;

    TwoWireSpiImple(uint8_t, uint8_t) // clock and data pins ignored for hardware SPI
    {
    }

    ~TwoWireSpiImple()
    {
        SPI.end();
    }

#if defined(ARDUINO_ARCH_ESP32)
    // for cases where hardware SPI can have pins changed
    void begin(int8_t sck, int8_t miso, int8_t mosi, int8_t ss)
    {
        SPI.begin(sck, miso, mosi, ss);
    }
#endif

    void begin()
    {
        SPI.begin();
    }

    void beginTransaction()
    {
        SPI.beginTransaction(SPISettings(_speed.Clock, MSBFIRST, SPI_MODE0));
    }

    void endTransaction()
    {
        SPI.endTransaction();
    }

    void transmitByte(uint8_t data)
    {
        SPI.transfer(data);
    }

    void transmitBytes(const uint8_t* data, size_t dataSize)
    {
#if defined(ARDUINO_ARCH_ESP8266) || defined(ARDUINO_ARCH_ESP32)
        // ESPs have a method to write without inplace overwriting the send buffer
        // since we don't care what gets received, use it for performance
        // FIX: but for what ever reason on Esp32, its not const
        SPI.writeBytes(const_cast<uint8_t*>(data), dataSize);

#else
        // default ARDUINO transfer inplace overwrites the send buffer
        // which is bad in this case, so we have to send one byte at a time
        const uint8_t* endData = data + dataSize;
        while (data < endData)
        {
            SPI.transfer(*data++);
        }
#endif
    }

    void applySettings(const SettingsObject& settings)
    {
        _speed.applySettings(settings);
    }

private:
    T_SPISPEED _speed;
};