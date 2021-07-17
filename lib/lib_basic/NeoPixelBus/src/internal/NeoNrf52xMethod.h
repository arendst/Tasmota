/*-------------------------------------------------------------------------
NeoPixel library helper classes for Nrf52* MCUs.
Nano 33 BLE

Written by Michael C. Miller.

I invest time and resources providing this open source code,
please support me by dontating (see https://github.com/Makuna/NeoPixelBus)

-------------------------------------------------------------------------
This file is part of the Makuna/NeoPixelBus library.
The contents of this file were taken from the Adafruit NeoPixel library
and modified only to fit within individual calling functions.

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

#if defined(ARDUINO_ARCH_NRF52840)

const uint16_t c_dmaBytesPerDataByte = 8 * sizeof(nrf_pwm_values_common_t); // bits * bytes to represent pulse

// for Bit* variables
// count 1 = 0.0625us, so max count (32768) is 2048us

class NeoNrf52xPwmSpeedWs2811
{
public:
    const static uint32_t CountTop = 20UL; // 1.25us
    const static nrf_pwm_values_common_t Bit0 = 5 | 0x8000; // ~0.3us
    const static nrf_pwm_values_common_t Bit1 = 14 | 0x8000; // ~0.9us
    const static nrf_pwm_values_common_t BitReset = 0x8000; // LOW
    const static uint16_t CountReset = 240; // 300 us / 1.25us
    const static PinStatus IdleLevel = LOW;
};

class NeoNrf52xPwmSpeedWs2812x
{
public:
    const static uint32_t CountTop = 20UL; // 1.25us
    const static nrf_pwm_values_common_t Bit0 = 6 | 0x8000; // ~0.4us
    const static nrf_pwm_values_common_t Bit1 = 13 | 0x8000; // ~0.8us
    const static nrf_pwm_values_common_t BitReset = 0x8000; // LOW
    const static uint32_t CountReset = 240 ; // 300us / 1.25us pulse width
    const static PinStatus IdleLevel = LOW;
};

class NeoNrf52xPwmSpeedSk6812
{
public:
    const static uint32_t CountTop = 20UL; // 1.25us
    const static nrf_pwm_values_common_t Bit0 = 6 | 0x8000; // ~0.4us
    const static nrf_pwm_values_common_t Bit1 = 13 | 0x8000; // ~0.8us
    const static nrf_pwm_values_common_t BitReset = 0x8000; // LOW
    const static uint32_t CountReset = 64; // 80us / 1.25us pulse width
    const static PinStatus IdleLevel = LOW;
};

class NeoNrf52xPwmSpeedTm1814
{
public:
    const static uint32_t CountTop = 20UL; // 1.25us
    const static nrf_pwm_values_common_t Bit0 = 5; // ~0.3us
    const static nrf_pwm_values_common_t Bit1 = 12; // ~0.7us
    const static nrf_pwm_values_common_t BitReset = 0x0000; // HIGH
    const static uint32_t CountReset = 160; // 200us / 1.25us pulse width
    const static PinStatus IdleLevel = HIGH;
};

class NeoNrf52xPwmSpeedTm1829
{
public:
    const static uint32_t CountTop = 20UL; // 1.25us
    const static nrf_pwm_values_common_t Bit0 = 5; // ~0.3us
    const static nrf_pwm_values_common_t Bit1 = 13 | 0x8000; // ~0.8us
    const static nrf_pwm_values_common_t BitReset = 0x0000; // HIGH
    const static uint32_t CountReset = 160; // 200us / 1.25us pulse width
    const static PinStatus IdleLevel = HIGH;
};

class NeoNrf52xPwmSpeed800Kbps
{
public:
    const static uint32_t CountTop = 20UL; // 1.25us
    const static nrf_pwm_values_common_t Bit0 = 6 | 0x8000; // ~0.4us
    const static nrf_pwm_values_common_t Bit1 = 13 | 0x8000; // ~0.8us
    const static nrf_pwm_values_common_t BitReset = 0x8000; // LOW
    const static uint32_t CountReset = 40; // 50us / 1.25us pulse width
    const static PinStatus IdleLevel = LOW;
};

class NeoNrf52xPwmSpeed400Kbps
{
public:
    const static uint32_t CountTop = 40UL; // 2.5us
    const static nrf_pwm_values_common_t Bit0 = 13 | 0x8000; // ~0.8us
    const static nrf_pwm_values_common_t Bit1 = 26 | 0x8000; // ~1.6us
    const static nrf_pwm_values_common_t BitReset = 0x8000; // LOW
    const static uint16_t CountReset = 20; // 50 us / 2.5us
    const static PinStatus IdleLevel = LOW;
};

// count 1 = 0.0625us, so max count (32768) is 2048us
class NeoNrf52xPwmSpeedApa106
{
public:
    const static uint32_t CountTop = 26UL; // ~1.525us (target is 1.65us)
    const static nrf_pwm_values_common_t Bit0 = 6 | 0x8000; // ~0.375us (target is 0.35)
    const static nrf_pwm_values_common_t Bit1 = 21 | 0x8000; // ~1.3125us (target is 1.350)
    const static nrf_pwm_values_common_t BitReset = 0x8000; // LOW
    const static uint32_t CountReset = 40; // 50us / 1.25us pulse width
    const static PinStatus IdleLevel = LOW;
};

class NeoNrf52xPwmSpeedTx1812
{
public:
    const static uint32_t CountTop = 14UL; // ~0.875us (target is 0.9us)
    const static nrf_pwm_values_common_t Bit0 = 5 | 0x8000; // ~0.3125us (target is 0.3)
    const static nrf_pwm_values_common_t Bit1 = 10 | 0x8000; // ~0.625us (target is 0.6)
    const static nrf_pwm_values_common_t BitReset = 0x8000; // LOW
    const static uint32_t CountReset = 228; // 200us / 0.875us pulse width
    const static PinStatus IdleLevel = LOW;
};

class NeoNrf52xPwmInvertedSpeedWs2811
{
public:
    const static uint32_t CountTop = 20UL; // 1.25us
    const static nrf_pwm_values_common_t Bit0 = 5; // ~0.3us
    const static nrf_pwm_values_common_t Bit1 = 14; // ~0.9us
    const static nrf_pwm_values_common_t BitReset = 0x0000; // HIGH
    const static uint16_t CountReset = 240; // 300 us / 1.25us
    const static PinStatus IdleLevel = HIGH;
};

class NeoNrf52xPwmInvertedSpeedWs2812x
{
public:
    const static uint32_t CountTop = 20UL; // 1.25us
    const static nrf_pwm_values_common_t Bit0 = 6; // ~0.4us
    const static nrf_pwm_values_common_t Bit1 = 13; // ~0.8us
    const static nrf_pwm_values_common_t BitReset = 0x0000; // HIGH
    const static uint32_t CountReset = 240; // 300us / 1.25us pulse width
    const static PinStatus IdleLevel = HIGH;
};

class NeoNrf52xPwmInvertedSpeedSk6812
{
public:
    const static uint32_t CountTop = 20UL; // 1.25us
    const static nrf_pwm_values_common_t Bit0 = 6; // ~0.4us
    const static nrf_pwm_values_common_t Bit1 = 13; // ~0.8us
    const static nrf_pwm_values_common_t BitReset = 0x0000; // HIGH
    const static uint32_t CountReset = 64; // 80us / 1.25us pulse width
    const static PinStatus IdleLevel = HIGH;
};

class NeoNrf52xPwmInvertedSpeedTm1814
{
public:
    const static uint32_t CountTop = 20UL; // 1.25us
    const static nrf_pwm_values_common_t Bit0 = 5 | 0x8000; // ~0.3us
    const static nrf_pwm_values_common_t Bit1 = 12 | 0x8000; // ~0.7us
    const static nrf_pwm_values_common_t BitReset = 0x8000; // LOW
    const static uint32_t CountReset = 160; // 200us / 1.25us pulse width
    const static PinStatus IdleLevel = LOW;
};

class NeoNrf52xPwmInvertedSpeedTm1829
{
public:
    const static uint32_t CountTop = 20UL; // 1.25us
    const static nrf_pwm_values_common_t Bit0 = 5 | 0x8000; // ~0.3us
    const static nrf_pwm_values_common_t Bit1 = 13; // ~0.8us
    const static nrf_pwm_values_common_t BitReset = 0x8000; // LOW
    const static uint32_t CountReset = 160; // 200us / 1.25us pulse width
    const static PinStatus IdleLevel = LOW;
};

class NeoNrf52xPwmInvertedSpeed800Kbps
{
public:
    const static uint32_t CountTop = 20UL; // 1.25us
    const static nrf_pwm_values_common_t Bit0 = 6; // ~0.4us
    const static nrf_pwm_values_common_t Bit1 = 13; // ~0.8us
    const static nrf_pwm_values_common_t BitReset = 0x0000; // HIGH
    const static uint32_t CountReset = 40; // 50us / 1.25us pulse width
    const static PinStatus IdleLevel = HIGH;
};

class NeoNrf52xPwmInvertedSpeed400Kbps
{
public:
    const static uint32_t CountTop = 40UL; // 2.5us
    const static nrf_pwm_values_common_t Bit0 = 13; // ~0.8us
    const static nrf_pwm_values_common_t Bit1 = 26; // ~1.6us
    const static nrf_pwm_values_common_t BitReset = 0x0000; // HIGH
    const static uint16_t CountReset = 20; // 50 us / 2.5us
    const static PinStatus IdleLevel = HIGH;
};

class NeoNrf52xPwmInvertedSpeedApa106
{
public:
    const static uint32_t CountTop = 26UL; // ~1.525us (target is 1.65us)
    const static nrf_pwm_values_common_t Bit0 = 6; // ~0.375us (target is 0.35)
    const static nrf_pwm_values_common_t Bit1 = 21; // ~1.3125us (target is 1.350)
    const static nrf_pwm_values_common_t BitReset = 0x0000; // HIGH
    const static uint32_t CountReset = 40; // 50us / 1.25us pulse width
    const static PinStatus IdleLevel = HIGH;
};

class NeoNrf52xPwmInvertedSpeedTx1812
{
public:
    const static uint32_t CountTop = 14UL; // ~0.875us (target is 0.9us)
    const static nrf_pwm_values_common_t Bit0 = 5; // ~0.3125us (target is 0.3)
    const static nrf_pwm_values_common_t Bit1 = 10; // ~0.625us (target is 0.6)
    const static nrf_pwm_values_common_t BitReset = 0x0000; // HIGH
    const static uint32_t CountReset = 228; // 200us / 0.875us pulse width
    const static PinStatus IdleLevel = HIGH;
};

class NeoNrf52xPwm0
{
public:
    NeoNrf52xPwm0() {};

    inline static NRF_PWM_Type* Pwm()
    {
        return NRF_PWM0;
    }
};

class NeoNrf52xPwm1
{
public:
    NeoNrf52xPwm1() {};

    inline static NRF_PWM_Type* Pwm()
    {
        return NRF_PWM1;
    }
};

class NeoNrf52xPwm2
{
public:
    NeoNrf52xPwm2() {};

    inline static NRF_PWM_Type* Pwm()
    {
        return NRF_PWM2;
    }
};

#if defined(NRF_PWM3)
class NeoNrf52xPwm3
{
public:
    NeoNrf52xPwm3() {};

    inline static NRF_PWM_Type* Pwm()
    {
        return NRF_PWM3;
    }
};
#endif

// dynamic channel support
class NeoNrf52xPwmN
{
public:
    NeoNrf52xPwmN(NeoBusChannel channel) 
    {
        NRF_PWM_Type* PWM[] = {
    NRF_PWM0,
    NRF_PWM1,
    NRF_PWM2
#ifdef NRF_PWM3
            ,NRF_PWM3
#endif
        };
        _pwm = PWM[channel];
    }

    inline NRF_PWM_Type* Pwm() const
    {
        return _pwm;
    }

protected:
    NRF_PWM_Type* _pwm;

    NeoNrf52xPwmN() {};
};

template<typename T_SPEED, typename T_BUS> class NeoNrf52xMethodBase
{
public:
    typedef NeoNoSettings SettingsObject;

    NeoNrf52xMethodBase(uint8_t pin, uint16_t pixelCount, size_t elementSize, size_t settingsSize) :
        _sizeData(pixelCount * elementSize + settingsSize),
        _pin(pin)
    {
        construct();
    }

    NeoNrf52xMethodBase(uint8_t pin, uint16_t pixelCount, size_t elementSize, size_t settingsSize, NeoBusChannel channel) :
        _sizeData(pixelCount* elementSize + settingsSize),
        _pin(pin),
        _bus(channel)
    {
        construct();
    }

    ~NeoNrf52xMethodBase()
    {
        while (!IsReadyToUpdate())
        {
            yield();
        }

        dmaDeinit();

        pinMode(_pin, INPUT);

        free(_data);
        free(_dmaBuffer);
    }

    bool IsReadyToUpdate() const
    {
        return (_bus.Pwm()->EVENTS_STOPPED);
    }

    void Initialize()
    {
        digitalWrite(_pin, T_SPEED::IdleLevel);

        dmaInit();

        // must force a first update so the EVENTS_SEQEND gets set as
        // you can't set it manually
        FillBuffer();
        dmaStart();
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
            yield(); // allows for system yield if needed
        }

        FillBuffer();
        dmaStart();
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
    }

private:
    const size_t   _sizeData;    // Size of '_data' buffer below
    const uint8_t _pin;      // output pin number
    const T_BUS _bus; // holds instance for multi channel support

    uint8_t* _data;        // Holds LED color values
    size_t   _dmaBufferSize; // total size of _dmaBuffer
    nrf_pwm_values_common_t* _dmaBuffer;     // Holds pixel data in native format for PWM hardware

    void construct()
    {
        pinMode(_pin, OUTPUT);

        _data = static_cast<uint8_t*>(malloc(_sizeData));
        // data cleared later in Begin()

        _dmaBufferSize = c_dmaBytesPerDataByte * _sizeData + sizeof(nrf_pwm_values_common_t);
        _dmaBuffer = static_cast<nrf_pwm_values_common_t*>(malloc(_dmaBufferSize));
    }

    void dmaInit()
    {
        // only use channel zero
        _bus.Pwm()->PSEL.OUT[0] = digitalPinToPinName(_pin);
        _bus.Pwm()->PSEL.OUT[1] = NC;
        _bus.Pwm()->PSEL.OUT[2] = NC;
        _bus.Pwm()->PSEL.OUT[3] = NC;

        _bus.Pwm()->ENABLE = 1;
        _bus.Pwm()->MODE = NRF_PWM_MODE_UP;
        _bus.Pwm()->PRESCALER = NRF_PWM_CLK_16MHz;
        _bus.Pwm()->COUNTERTOP = T_SPEED::CountTop;
        _bus.Pwm()->LOOP = 1; // single fire so events get set
        _bus.Pwm()->DECODER = NRF_PWM_LOAD_COMMON;

        // sequence zero is the primary data with a BitReset entry on the end for
        // the delay repeating
        _bus.Pwm()->SEQ[0].PTR = reinterpret_cast<uint32_t>(_dmaBuffer);
        _bus.Pwm()->SEQ[0].CNT = _dmaBufferSize / sizeof(nrf_pwm_values_common_t);
        _bus.Pwm()->SEQ[0].REFRESH = 0; // ignored
        _bus.Pwm()->SEQ[0].ENDDELAY = T_SPEED::CountReset; // ignored still?

        // sequence one is pointing to the BitReset entry at the end of the primary data
        _bus.Pwm()->SEQ[1].PTR = reinterpret_cast<uint32_t>(_dmaBuffer + (_bus.Pwm()->SEQ[0].CNT - 1));
        _bus.Pwm()->SEQ[1].CNT = 1;
        _bus.Pwm()->SEQ[1].REFRESH = 0; // ignored
        _bus.Pwm()->SEQ[1].ENDDELAY = 0; // ignored

        // stop when the loop finishes
        _bus.Pwm()->SHORTS = PWM_SHORTS_LOOPSDONE_STOP_Msk;
        _bus.Pwm()->INTEN = 0;

        dmaResetEvents();
    }

    void dmaDeinit()
    {
        _bus.Pwm()->ENABLE = 0;
        _bus.Pwm()->PSEL.OUT[0] = NC;
    }

    void FillBuffer()
    {
        nrf_pwm_values_common_t* pDma = _dmaBuffer;
        nrf_pwm_values_common_t* pDmaEnd = _dmaBuffer + (_dmaBufferSize / sizeof(nrf_pwm_values_common_t));
        uint8_t* pEnd = _data + _sizeData;

        for (uint8_t* pData = _data; pData < pEnd; pData++)
        {
            uint8_t data = *pData;

            for (uint8_t bit = 0; bit < 8; bit++)
            {
                *(pDma++) = (data & 0x80) ? T_SPEED::Bit1 : T_SPEED::Bit0;
                data <<= 1;
            }
        }

        // fill the rest with BitReset as it will get repeated when delaying or
        // at the end before being stopped
        while (pDma < pDmaEnd)
        {
            *(pDma++) = T_SPEED::BitReset;
        }
    }

    void dmaResetEvents()
    {
        _bus.Pwm()->EVENTS_LOOPSDONE = 0;
        _bus.Pwm()->EVENTS_SEQEND[0] = 0;
        _bus.Pwm()->EVENTS_SEQEND[1] = 0;
        _bus.Pwm()->EVENTS_STOPPED = 0;
    }

    void dmaStart()
    {
        dmaResetEvents();
        _bus.Pwm()->TASKS_SEQSTART[0] = 1;
    }
};

// normal
typedef NeoNrf52xMethodBase<NeoNrf52xPwmSpeedWs2811, NeoNrf52xPwmN> NeoNrf52xPwmNWs2811Method;
typedef NeoNrf52xMethodBase<NeoNrf52xPwmSpeedWs2812x, NeoNrf52xPwmN> NeoNrf52xPwmNWs2812xMethod;
typedef NeoNrf52xMethodBase<NeoNrf52xPwmSpeedSk6812, NeoNrf52xPwmN> NeoNrf52xPwmNSk6812Method;
typedef NeoNrf52xMethodBase<NeoNrf52xPwmSpeedTm1814, NeoNrf52xPwmN> NeoNrf52xPwmNTm1814Method;
typedef NeoNrf52xMethodBase<NeoNrf52xPwmSpeedTm1829, NeoNrf52xPwmN> NeoNrf52xPwmNTm1829Method;
typedef NeoNrf52xMethodBase<NeoNrf52xPwmSpeedTx1812, NeoNrf52xPwmN> NeoNrf52xPwmNTx1812Method;
typedef NeoNrf52xMethodBase<NeoNrf52xPwmSpeedApa106, NeoNrf52xPwmN> NeoNrf52xPwmNApa106Method;
typedef NeoNrf52xMethodBase<NeoNrf52xPwmSpeed800Kbps, NeoNrf52xPwmN> NeoNrf52xPwmN800KbpsMethod;
typedef NeoNrf52xMethodBase<NeoNrf52xPwmSpeed400Kbps, NeoNrf52xPwmN> NeoNrf52xPwmN400KbpsMethod;

typedef NeoNrf52xMethodBase<NeoNrf52xPwmSpeedWs2811, NeoNrf52xPwm0> NeoNrf52xPwm0Ws2811Method;
typedef NeoNrf52xMethodBase<NeoNrf52xPwmSpeedWs2812x, NeoNrf52xPwm0> NeoNrf52xPwm0Ws2812xMethod;
typedef NeoNrf52xMethodBase<NeoNrf52xPwmSpeedSk6812, NeoNrf52xPwm0> NeoNrf52xPwm0Sk6812Method;
typedef NeoNrf52xMethodBase<NeoNrf52xPwmSpeedTm1814, NeoNrf52xPwm0> NeoNrf52xPwm0Tm1814Method;
typedef NeoNrf52xMethodBase<NeoNrf52xPwmSpeedTm1829, NeoNrf52xPwm0> NeoNrf52xPwm0Tm1829Method;
typedef NeoNrf52xMethodBase<NeoNrf52xPwmSpeedTx1812, NeoNrf52xPwm0> NeoNrf52xPwm0Tx1812Method;
typedef NeoNrf52xMethodBase<NeoNrf52xPwmSpeedApa106, NeoNrf52xPwm0> NeoNrf52xPwm0Apa106Method;
typedef NeoNrf52xMethodBase<NeoNrf52xPwmSpeed800Kbps, NeoNrf52xPwm0> NeoNrf52xPwm0800KbpsMethod;
typedef NeoNrf52xMethodBase<NeoNrf52xPwmSpeed400Kbps, NeoNrf52xPwm0> NeoNrf52xPwm0400KbpsMethod;

typedef NeoNrf52xMethodBase<NeoNrf52xPwmSpeedWs2811, NeoNrf52xPwm1> NeoNrf52xPwm1Ws2811Method;
typedef NeoNrf52xMethodBase<NeoNrf52xPwmSpeedWs2812x, NeoNrf52xPwm1> NeoNrf52xPwm1Ws2812xMethod;
typedef NeoNrf52xMethodBase<NeoNrf52xPwmSpeedSk6812, NeoNrf52xPwm1> NeoNrf52xPwm1Sk6812Method;
typedef NeoNrf52xMethodBase<NeoNrf52xPwmSpeedTm1814, NeoNrf52xPwm1> NeoNrf52xPwm1Tm1814Method;
typedef NeoNrf52xMethodBase<NeoNrf52xPwmSpeedTm1829, NeoNrf52xPwm1> NeoNrf52xPwm1Tm1829Method;
typedef NeoNrf52xMethodBase<NeoNrf52xPwmSpeedTx1812, NeoNrf52xPwm1> NeoNrf52xPwm1Tx1812Method;
typedef NeoNrf52xMethodBase<NeoNrf52xPwmSpeedApa106, NeoNrf52xPwm1> NeoNrf52xPwm1Apa106Method;
typedef NeoNrf52xMethodBase<NeoNrf52xPwmSpeed800Kbps, NeoNrf52xPwm1> NeoNrf52xPwm1800KbpsMethod;
typedef NeoNrf52xMethodBase<NeoNrf52xPwmSpeed400Kbps, NeoNrf52xPwm1> NeoNrf52xPwm1400KbpsMethod;

typedef NeoNrf52xMethodBase<NeoNrf52xPwmSpeedWs2811, NeoNrf52xPwm2> NeoNrf52xPwm2Ws2811Method;
typedef NeoNrf52xMethodBase<NeoNrf52xPwmSpeedWs2812x, NeoNrf52xPwm2> NeoNrf52xPwm2Ws2812xMethod;
typedef NeoNrf52xMethodBase<NeoNrf52xPwmSpeedSk6812, NeoNrf52xPwm2> NeoNrf52xPwm2Sk6812Method;
typedef NeoNrf52xMethodBase<NeoNrf52xPwmSpeedTm1814, NeoNrf52xPwm2> NeoNrf52xPwm2Tm1814Method;
typedef NeoNrf52xMethodBase<NeoNrf52xPwmSpeedTm1829, NeoNrf52xPwm2> NeoNrf52xPwm2Tm1829Method;
typedef NeoNrf52xMethodBase<NeoNrf52xPwmSpeedTx1812, NeoNrf52xPwm2> NeoNrf52xPwm2Tx1812Method;
typedef NeoNrf52xMethodBase<NeoNrf52xPwmSpeedApa106, NeoNrf52xPwm2> NeoNrf52xPwm2Apa106Method;
typedef NeoNrf52xMethodBase<NeoNrf52xPwmSpeed800Kbps, NeoNrf52xPwm2> NeoNrf52xPwm2800KbpsMethod;
typedef NeoNrf52xMethodBase<NeoNrf52xPwmSpeed400Kbps, NeoNrf52xPwm2> NeoNrf52xPwm2400KbpsMethod;

#if defined(NRF_PWM3)
typedef NeoNrf52xMethodBase<NeoNrf52xPwmSpeedWs2811, NeoNrf52xPwm3> NeoNrf52xPwm3Ws2811Method;
typedef NeoNrf52xMethodBase<NeoNrf52xPwmSpeedWs2812x, NeoNrf52xPwm3> NeoNrf52xPwm3Ws2812xMethod;
typedef NeoNrf52xMethodBase<NeoNrf52xPwmSpeedSk6812, NeoNrf52xPwm3> NeoNrf52xPwm3Sk6812Method;
typedef NeoNrf52xMethodBase<NeoNrf52xPwmSpeedTm1814, NeoNrf52xPwm3> NeoNrf52xPwm3Tm1814Method;
typedef NeoNrf52xMethodBase<NeoNrf52xPwmSpeedTm1829, NeoNrf52xPwm3> NeoNrf52xPwm3Tm1829Method;
typedef NeoNrf52xMethodBase<NeoNrf52xPwmSpeedTx1812, NeoNrf52xPwm3> NeoNrf52xPwm3Tx1812Method;
typedef NeoNrf52xMethodBase<NeoNrf52xPwmSpeedApa106, NeoNrf52xPwm3> NeoNrf52xPwm3Apa106Method;
typedef NeoNrf52xMethodBase<NeoNrf52xPwmSpeed800Kbps, NeoNrf52xPwm3> NeoNrf52xPwm3800KbpsMethod;
typedef NeoNrf52xMethodBase<NeoNrf52xPwmSpeed400Kbps, NeoNrf52xPwm3> NeoNrf52xPwm3400KbpsMethod;
#endif

// inverted
typedef NeoNrf52xMethodBase<NeoNrf52xPwmInvertedSpeedWs2811, NeoNrf52xPwmN> NeoNrf52xPwmNWs2811InvertedMethod;
typedef NeoNrf52xMethodBase<NeoNrf52xPwmInvertedSpeedWs2812x, NeoNrf52xPwmN> NeoNrf52xPwmNWs2812xInvertedMethod;
typedef NeoNrf52xMethodBase<NeoNrf52xPwmInvertedSpeedSk6812, NeoNrf52xPwmN> NeoNrf52xPwmNSk6812InvertedMethod;
typedef NeoNrf52xMethodBase<NeoNrf52xPwmInvertedSpeedTm1814, NeoNrf52xPwmN> NeoNrf52xPwmNTm1814InvertedMethod;
typedef NeoNrf52xMethodBase<NeoNrf52xPwmInvertedSpeedTm1829, NeoNrf52xPwmN> NeoNrf52xPwmNTm1829InvertedMethod;
typedef NeoNrf52xMethodBase<NeoNrf52xPwmInvertedSpeedTx1812, NeoNrf52xPwmN> NeoNrf52xPwmNTx1812InvertedMethod;
typedef NeoNrf52xMethodBase<NeoNrf52xPwmInvertedSpeedApa106, NeoNrf52xPwmN> NeoNrf52xPwmNApa106InvertedMethod;
typedef NeoNrf52xMethodBase<NeoNrf52xPwmInvertedSpeed800Kbps, NeoNrf52xPwmN> NeoNrf52xPwmN800KbpsInvertedMethod;
typedef NeoNrf52xMethodBase<NeoNrf52xPwmInvertedSpeed400Kbps, NeoNrf52xPwmN> NeoNrf52xPwmN400KbpsInvertedMethod;

typedef NeoNrf52xMethodBase<NeoNrf52xPwmInvertedSpeedWs2811, NeoNrf52xPwm0> NeoNrf52xPwm0Ws2811InvertedMethod;
typedef NeoNrf52xMethodBase<NeoNrf52xPwmInvertedSpeedWs2812x, NeoNrf52xPwm0> NeoNrf52xPwm0Ws2812xInvertedMethod;
typedef NeoNrf52xMethodBase<NeoNrf52xPwmInvertedSpeedSk6812, NeoNrf52xPwm0> NeoNrf52xPwm0Sk6812InvertedMethod;
typedef NeoNrf52xMethodBase<NeoNrf52xPwmInvertedSpeedTm1814, NeoNrf52xPwm0> NeoNrf52xPwm0Tm1814InvertedMethod;
typedef NeoNrf52xMethodBase<NeoNrf52xPwmInvertedSpeedTm1829, NeoNrf52xPwm0> NeoNrf52xPwm0Tm1829InvertedMethod;
typedef NeoNrf52xMethodBase<NeoNrf52xPwmInvertedSpeedTx1812, NeoNrf52xPwm0> NeoNrf52xPwm0Tx1812InvertedMethod;
typedef NeoNrf52xMethodBase<NeoNrf52xPwmInvertedSpeedApa106, NeoNrf52xPwm0> NeoNrf52xPwm0Apa106InvertedMethod;
typedef NeoNrf52xMethodBase<NeoNrf52xPwmInvertedSpeed800Kbps, NeoNrf52xPwm0> NeoNrf52xPwm0800KbpsInvertedMethod;
typedef NeoNrf52xMethodBase<NeoNrf52xPwmInvertedSpeed400Kbps, NeoNrf52xPwm0> NeoNrf52xPwm0400KbpsInvertedMethod;

typedef NeoNrf52xMethodBase<NeoNrf52xPwmInvertedSpeedWs2811, NeoNrf52xPwm1> NeoNrf52xPwm1Ws2811InvertedMethod;
typedef NeoNrf52xMethodBase<NeoNrf52xPwmInvertedSpeedWs2812x, NeoNrf52xPwm1> NeoNrf52xPwm1Ws2812xInvertedMethod;
typedef NeoNrf52xMethodBase<NeoNrf52xPwmInvertedSpeedSk6812, NeoNrf52xPwm1> NeoNrf52xPwm1Sk6812InvertedMethod;
typedef NeoNrf52xMethodBase<NeoNrf52xPwmInvertedSpeedTm1814, NeoNrf52xPwm1> NeoNrf52xPwm1Tm1814InvertedMethod;
typedef NeoNrf52xMethodBase<NeoNrf52xPwmInvertedSpeedTm1829, NeoNrf52xPwm1> NeoNrf52xPwm1Tm1829InvertedMethod;
typedef NeoNrf52xMethodBase<NeoNrf52xPwmInvertedSpeedTx1812, NeoNrf52xPwm1> NeoNrf52xPwm1Tx1812InvertedMethod;
typedef NeoNrf52xMethodBase<NeoNrf52xPwmInvertedSpeedApa106, NeoNrf52xPwm1> NeoNrf52xPwm1Apa106InvertedMethod;
typedef NeoNrf52xMethodBase<NeoNrf52xPwmInvertedSpeed800Kbps, NeoNrf52xPwm1> NeoNrf52xPwm1800KbpsInvertedMethod;
typedef NeoNrf52xMethodBase<NeoNrf52xPwmInvertedSpeed400Kbps, NeoNrf52xPwm1> NeoNrf52xPwm1400KbpsInvertedMethod;

typedef NeoNrf52xMethodBase<NeoNrf52xPwmInvertedSpeedWs2811, NeoNrf52xPwm2> NeoNrf52xPwm2Ws2811InvertedMethod;
typedef NeoNrf52xMethodBase<NeoNrf52xPwmInvertedSpeedWs2812x, NeoNrf52xPwm2> NeoNrf52xPwm2Ws2812xInvertedMethod;
typedef NeoNrf52xMethodBase<NeoNrf52xPwmInvertedSpeedSk6812, NeoNrf52xPwm2> NeoNrf52xPwm2Sk6812InvertedMethod;
typedef NeoNrf52xMethodBase<NeoNrf52xPwmInvertedSpeedTm1814, NeoNrf52xPwm2> NeoNrf52xPwm2Tm1814InvertedMethod;
typedef NeoNrf52xMethodBase<NeoNrf52xPwmInvertedSpeedTm1829, NeoNrf52xPwm2> NeoNrf52xPwm2Tm1829InvertedMethod;
typedef NeoNrf52xMethodBase<NeoNrf52xPwmInvertedSpeedTx1812, NeoNrf52xPwm2> NeoNrf52xPwm2Tx1812InvertedMethod;
typedef NeoNrf52xMethodBase<NeoNrf52xPwmInvertedSpeedApa106, NeoNrf52xPwm2> NeoNrf52xPwm2Apa106InvertedMethod;
typedef NeoNrf52xMethodBase<NeoNrf52xPwmInvertedSpeed800Kbps, NeoNrf52xPwm2> NeoNrf52xPwm2800KbpsInvertedMethod;
typedef NeoNrf52xMethodBase<NeoNrf52xPwmInvertedSpeed400Kbps, NeoNrf52xPwm2> NeoNrf52xPwm2400KbpsInvertedMethod;

#if defined(NRF_PWM3)
typedef NeoNrf52xMethodBase<NeoNrf52xPwmInvertedSpeedWs2811, NeoNrf52xPwm3> NeoNrf52xPwm3Ws2811InvertedMethod;
typedef NeoNrf52xMethodBase<NeoNrf52xPwmInvertedSpeedWs2812x, NeoNrf52xPwm3> NeoNrf52xPwm3Ws2812xInvertedMethod;
typedef NeoNrf52xMethodBase<NeoNrf52xPwmInvertedSpeedSk6812, NeoNrf52xPwm3> NeoNrf52xPwm3Sk6812InvertedMethod;
typedef NeoNrf52xMethodBase<NeoNrf52xPwmInvertedSpeedTm1814, NeoNrf52xPwm3> NeoNrf52xPwm3Tm1814InvertedMethod;
typedef NeoNrf52xMethodBase<NeoNrf52xPwmInvertedSpeedTm1829, NeoNrf52xPwm3> NeoNrf52xPwm3Tm1829InvertedMethod;
typedef NeoNrf52xMethodBase<NeoNrf52xPwmInvertedSpeedTx1812, NeoNrf52xPwm3> NeoNrf52xPwm3Tx1812InvertedMethod;
typedef NeoNrf52xMethodBase<NeoNrf52xPwmInvertedSpeedApa106, NeoNrf52xPwm3> NeoNrf52xPwm3Apa106InvertedMethod;
typedef NeoNrf52xMethodBase<NeoNrf52xPwmInvertedSpeed800Kbps, NeoNrf52xPwm3> NeoNrf52xPwm3800KbpsInvertedMethod;
typedef NeoNrf52xMethodBase<NeoNrf52xPwmInvertedSpeed400Kbps, NeoNrf52xPwm3> NeoNrf52xPwm3400KbpsInvertedMethod;
#endif

// defaults
typedef NeoNrf52xPwm2Ws2812xMethod NeoWs2813Method;
typedef NeoNrf52xPwm2Ws2812xMethod NeoWs2812xMethod;
typedef NeoNrf52xPwm2800KbpsMethod NeoWs2812Method;
typedef NeoNrf52xPwm2Ws2812xMethod NeoWs2811Method;
typedef NeoNrf52xPwm2Sk6812Method NeoSk6812Method;
typedef NeoNrf52xPwm2Tm1814Method NeoTm1814Method;
typedef NeoNrf52xPwm2Tm1829Method NeoTm1829Method;
typedef NeoNrf52xPwm2Tx1812Method NeoTx1812Method;
typedef NeoNrf52xPwm2Sk6812Method NeoLc8812Method;
typedef NeoNrf52xPwm2Apa106Method NeoApa106Method;

typedef NeoNrf52xPwm2Ws2812xMethod Neo800KbpsMethod;
typedef NeoNrf52xPwm2400KbpsMethod Neo400KbpsMethod;

typedef NeoNrf52xPwm2Ws2812xInvertedMethod NeoWs2813InvertedMethod;
typedef NeoNrf52xPwm2Ws2812xInvertedMethod NeoWs2812xInvertedMethod;
typedef NeoNrf52xPwm2Ws2812xInvertedMethod NeoWs2811InvertedMethod;
typedef NeoNrf52xPwm2800KbpsInvertedMethod NeoWs2812InvertedMethod;
typedef NeoNrf52xPwm2Sk6812InvertedMethod NeoSk6812InvertedMethod;
typedef NeoNrf52xPwm2Tm1814InvertedMethod NeoTm1814InvertedMethod;
typedef NeoNrf52xPwm2Tm1829InvertedMethod NeoTm1829InvertedMethod;
typedef NeoNrf52xPwm2Tx1812InvertedMethod NeoTx1812InvertedMethod;
typedef NeoNrf52xPwm2Sk6812InvertedMethod NeoLc8812InvertedMethod;
typedef NeoNrf52xPwm2Apa106InvertedMethod NeoApa106InvertedMethod;

typedef NeoNrf52xPwm2Ws2812xInvertedMethod Neo800KbpsInvertedMethod;
typedef NeoNrf52xPwm2400KbpsInvertedMethod Neo400KbpsInvertedMethod;

#endif
