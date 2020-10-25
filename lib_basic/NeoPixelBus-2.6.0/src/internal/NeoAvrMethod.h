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

#if defined(ARDUINO_ARCH_AVR) || defined(ARDUINO_ARCH_MEGAAVR)

extern "C"
{
    void send_data_8mhz_800_PortD(uint8_t* data, size_t sizeData, uint8_t pinMask);
    void send_data_8mhz_800_PortB(uint8_t* data, size_t sizeData, uint8_t pinMask);
    void send_data_8mhz_400(uint8_t* data, size_t sizeData, volatile uint8_t* port, uint8_t pinMask);
    void send_data_12mhz_800_PortD(uint8_t* data, size_t sizeData, uint8_t pinMask);
    void send_data_12mhz_800_PortB(uint8_t* data, size_t sizeData, uint8_t pinMask);
    void send_data_12mhz_400(uint8_t* data, size_t sizeData, volatile uint8_t* port, uint8_t pinMask);
    void send_data_16mhz_800(uint8_t* data, size_t sizeData, volatile uint8_t* port, uint8_t pinMask);
    void send_data_16mhz_400(uint8_t* data, size_t sizeData, volatile uint8_t* port, uint8_t pinMask);
}

class NeoAvrSpeed800KbpsBase
{
public:
    static void send_data(uint8_t* data, size_t sizeData, volatile uint8_t* port, uint8_t pinMask)
    {
#if (F_CPU >= 7400000UL) && (F_CPU <= 9500000UL)  // 8Mhz CPU
#ifdef PORTD // PORTD isn't present on ATtiny85, etc.
        if (port == &PORTD)
            send_data_8mhz_800_PortD(data, sizeData, pinMask);
        else if (port == &PORTB)
#endif // PORTD
            send_data_8mhz_800_PortB(data, sizeData, pinMask);

#elif (F_CPU >= 11100000UL) && (F_CPU <= 14300000UL)  // 12Mhz CPU
#ifdef PORTD // PORTD 
        if (port == &PORTD)
            send_data_12mhz_800_PortD(data, sizeData, pinMask);
        else if (port == &PORTB)
#endif // PORTD
            send_data_12mhz_800_PortB(data, sizeData, pinMask);

#elif (F_CPU >= 15400000UL) && (F_CPU <= 19000000L)  // 16Mhz CPU
        send_data_16mhz_800(data, sizeData, port, pinMask);
#else
#error "CPU SPEED NOT SUPPORTED"
#endif
    }
    
};

class NeoAvrSpeedWs2812x :  public NeoAvrSpeed800KbpsBase
{
public:
    static const uint32_t ResetTimeUs = 300;
};

class NeoAvrSpeedSk6812 : public NeoAvrSpeed800KbpsBase
{
public:
    static const uint32_t ResetTimeUs = 80;
};

class NeoAvrSpeedTm1814 : public NeoAvrSpeed800KbpsBase
{
public:
    static const uint32_t ResetTimeUs = 200;
};

class NeoAvrSpeed800Kbps: public NeoAvrSpeed800KbpsBase
{
public:
    static const uint32_t ResetTimeUs = 50;
};

class NeoAvrSpeed400Kbps
{
public:
    static void send_data(uint8_t* data, size_t sizeData, volatile uint8_t* port, uint8_t pinMask)
    {
#if (F_CPU >= 7400000UL) && (F_CPU <= 9500000UL)  // 8Mhz CPU
        send_data_8mhz_400(data, sizeData, port, pinMask);

#elif (F_CPU >= 11100000UL) && (F_CPU <= 14300000UL)  // 12Mhz CPU
        send_data_12mhz_400(data, sizeData, port, pinMask);

#elif (F_CPU >= 15400000UL) && (F_CPU <= 19000000L)  // 16Mhz CPU
        send_data_16mhz_400(data, sizeData, port, pinMask);
#else
#error "CPU SPEED NOT SUPPORTED"
#endif
    }
    static const uint32_t ResetTimeUs = 50;
};

template<typename T_SPEED> class NeoAvrMethodBase
{
public:
    NeoAvrMethodBase(uint8_t pin, uint16_t pixelCount, size_t elementSize, size_t settingsSize) :
        _sizeData(pixelCount * elementSize + settingsSize),
        _pin(pin),
        _port(NULL),
        _pinMask(0)
    {
        pinMode(pin, OUTPUT);

        _data = static_cast<uint8_t*>(malloc(_sizeData));
        memset(_data, 0, _sizeData);

        _port = portOutputRegister(digitalPinToPort(pin));
        _pinMask = digitalPinToBitMask(pin);
    }

    ~NeoAvrMethodBase()
    {
        pinMode(_pin, INPUT);

        free(_data);
    }

    bool IsReadyToUpdate() const
    {
        uint32_t delta = micros() - _endTime;

        return (delta >= T_SPEED::ResetTimeUs);
    }

    void Initialize()
    {
        digitalWrite(_pin, LOW);

        _endTime = micros();
    }

    void Update(bool)
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

        T_SPEED::send_data(_data, _sizeData, _port, _pinMask);

        interrupts();

        // save EOD time for latch on next call
        _endTime = micros();
    }

    uint8_t* getData() const
    {
        return _data;
    };

    size_t getDataSize() const
    {
        return _sizeData;
    };

private:
    const size_t  _sizeData;     // size of _data below       
    const uint8_t _pin;         // output pin number

    uint32_t _endTime;       // Latch timing reference
    uint8_t* _data;        // Holds data stream which include LED color values and other settings as needed
    
    volatile uint8_t* _port;         // Output PORT register
    uint8_t  _pinMask;      // Output PORT bitmask
};


typedef NeoAvrMethodBase<NeoAvrSpeedWs2812x> NeoAvrWs2812xMethod;
typedef NeoAvrMethodBase<NeoAvrSpeedSk6812> NeoAvrSk6812Method;
typedef NeoAvrMethodBase<NeoAvrSpeedTm1814> NeoAvrTm1814InvertedMethod;
typedef NeoAvrMethodBase<NeoAvrSpeed800Kbps> NeoAvr800KbpsMethod;
typedef NeoAvrMethodBase<NeoAvrSpeed400Kbps> NeoAvr400KbpsMethod;


// AVR doesn't have alternatives yet, so there is just the default
typedef NeoAvrWs2812xMethod NeoWs2813Method;
typedef NeoAvrWs2812xMethod NeoWs2812xMethod;
typedef NeoAvr800KbpsMethod NeoWs2812Method;
typedef NeoAvrWs2812xMethod NeoWs2811Method;
typedef NeoAvrSk6812Method NeoSk6812Method;
typedef NeoAvrSk6812Method NeoLc8812Method;
typedef NeoAvr400KbpsMethod NeoApa106Method;
typedef NeoAvrWs2812xMethod Neo800KbpsMethod;
typedef NeoAvr400KbpsMethod Neo400KbpsMethod;

typedef NeoAvrTm1814InvertedMethod NeoTm1814InvertedMethod;
#endif

