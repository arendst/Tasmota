/*-------------------------------------------------------------------------
NeoPixel library helper functions for ARM MCUs.
Teensy 3.0, 3.1, LC, Arduino Due

Written by Michael C. Miller.
Some work taken from the Adafruit NeoPixel library.

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

#if defined(__arm__) && !defined(ARDUINO_ARCH_NRF52840)

template<typename T_SPEED> class NeoArmMethodBase
{
public:
    typedef NeoNoSettings SettingsObject;

    NeoArmMethodBase(uint8_t pin, uint16_t pixelCount, size_t elementSize, size_t settingsSize) :
        _sizeData(pixelCount * elementSize + settingsSize),
        _pin(pin)
    {
        pinMode(pin, OUTPUT);

        _data = static_cast<uint8_t*>(malloc(_sizeData));
        // data cleared later in Begin()
    }

    ~NeoArmMethodBase()
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
            yield(); // allows for system yield if needed
        }

        noInterrupts(); // Need 100% focus on instruction timing

        T_SPEED::send_pixels(_data, _sizeData, _pin);

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

    void applySettings(const SettingsObject& settings)
    {
    }

private:
    const  size_t    _sizeData;   // Size of '_data' buffer below

    uint32_t _endTime;       // Latch timing reference
    uint8_t* _data;        // Holds LED color values
    uint8_t _pin;            // output pin number
};

// Teensy 3.0 or 3.1 (3.2) or 3.5 or 3.6
#if defined(__MK20DX128__) || defined(__MK20DX256__) || defined(__MK64FX512__) || defined(__MK66FX1M0__) 

class NeoArmMk20dxSpeedProps800KbpsBase
{
public:
    static const uint32_t CyclesT0h = (F_CPU / 4000000);
    static const uint32_t CyclesT1h = (F_CPU / 1250000);
    static const uint32_t Cycles = (F_CPU / 800000);
};

class NeoArmMk20dxSpeedPropsWs2812x : public NeoArmMk20dxSpeedProps800KbpsBase
{
public:
    static const uint32_t ResetTimeUs = 300;
};

class NeoArmMk20dxSpeedPropsSk6812 : public NeoArmMk20dxSpeedProps800KbpsBase
{
public:
    static const uint32_t ResetTimeUs = 80;
};

class NeoArmMk20dxSpeedPropsTm1814 : public NeoArmMk20dxSpeedProps800KbpsBase
{
public:
    static const uint32_t ResetTimeUs = 200;
};

class NeoArmMk20dxSpeedPropsTm1829 : public NeoArmMk20dxSpeedProps800KbpsBase
{
public:
    static const uint32_t ResetTimeUs = 200;
};

class NeoArmMk20dxSpeedProps800Kbps : public NeoArmMk20dxSpeedProps800KbpsBase
{
public:
    static const uint32_t ResetTimeUs = 50;
};

class NeoArmMk20dxSpeedProps400Kbps
{
public:
    static const uint32_t CyclesT0h = (F_CPU / 2000000);
    static const uint32_t CyclesT1h = (F_CPU / 833333);
    static const uint32_t Cycles = (F_CPU / 400000);
    static const uint32_t ResetTimeUs = 50;
};

class NeoArmMk20dxSpeedPropsApa106
{
public:
    static const uint32_t CyclesT0h = (F_CPU / 4000000);
    static const uint32_t CyclesT1h = (F_CPU / 913750);
    static const uint32_t Cycles = (F_CPU / 584800);
    static const uint32_t ResetTimeUs = 50;
};

template<typename T_SPEEDPROPS> class NeoArmMk20dxSpeedBase
{
public:
    static const uint32_t ResetTimeUs = T_SPEEDPROPS::ResetTimeUs;

    static void send_pixels(uint8_t* pixels, size_t sizePixels, uint8_t pin)
    {
        uint8_t* p = pixels;
        uint8_t* end = p + sizePixels;
        uint8_t pix;
        uint8_t mask;

        volatile uint8_t* set = portSetRegister(pin);
        volatile uint8_t* clr = portClearRegister(pin);

        uint32_t cyc;

        ARM_DEMCR |= ARM_DEMCR_TRCENA;
        ARM_DWT_CTRL |= ARM_DWT_CTRL_CYCCNTENA;

        cyc = ARM_DWT_CYCCNT + T_SPEEDPROPS::Cycles;
        while (p < end)
        {
            pix = *p++;
            for (mask = 0x80; mask; mask >>= 1)
            {
                while (ARM_DWT_CYCCNT - cyc < T_SPEEDPROPS::Cycles);

                cyc = ARM_DWT_CYCCNT;
                *set = 1;
                if (pix & mask)
                {
                    while (ARM_DWT_CYCCNT - cyc < T_SPEEDPROPS::CyclesT1h);
                }
                else
                {
                    while (ARM_DWT_CYCCNT - cyc < T_SPEEDPROPS::CyclesT0h);
                }
                *clr = 1;
            }
        }
    }
};

typedef NeoArmMethodBase<NeoArmMk20dxSpeedBase<NeoArmMk20dxSpeedPropsWs2812x>> NeoArmWs2812xMethod;
typedef NeoArmMethodBase<NeoArmMk20dxSpeedBase<NeoArmMk20dxSpeedPropsSk6812>> NeoArmSk6812Method;
typedef NeoArmMethodBase<NeoArmMk20dxSpeedBase<NeoArmMk20dxSpeedPropsTm1814>> NeoArmTm1814InvertedMethod;
typedef NeoArmMethodBase<NeoArmMk20dxSpeedBase<NeoArmMk20dxSpeedPropsTm1829>> NeoArmTm1829InvertedMethod;
typedef NeoArmMethodBase<NeoArmMk20dxSpeedBase<NeoArmMk20dxSpeedPropsApa106>> NeoArmApa106Method;
typedef NeoArmMethodBase<NeoArmMk20dxSpeedBase<NeoArmMk20dxSpeedProps800Kbps>> NeoArm800KbpsMethod;
typedef NeoArmMethodBase<NeoArmMk20dxSpeedBase<NeoArmMk20dxSpeedProps400Kbps>> NeoArm400KbpsMethod;


#elif defined(__MKL26Z64__) // Teensy-LC

#if F_CPU == 48000000

class NeoArmMk26z64Speed800KbpsBase
{
public:
    static void send_pixels(uint8_t* pixels, size_t sizePixels, uint8_t pin)
    {
        uint8_t* p = pixels;
        uint8_t pix;
        uint8_t count;
        uint8_t dly;
        uint8_t bitmask = digitalPinToBitMask(pin);
        volatile uint8_t* reg = portSetRegister(pin);
        uint32_t num = sizePixels;

        asm volatile(
            "L%=_begin:"				"\n\t"
            "ldrb	%[pix], [%[p], #0]"		"\n\t"
            "lsl	%[pix], #24"			"\n\t"
            "movs	%[count], #7"			"\n\t"
            "L%=_loop:"				"\n\t"
            "lsl	%[pix], #1"			"\n\t"
            "bcs	L%=_loop_one"			"\n\t"
            "L%=_loop_zero:"
            "strb	%[bitmask], [%[reg], #0]"	"\n\t"
            "movs	%[dly], #4"			"\n\t"
            "L%=_loop_delay_T0H:"			"\n\t"
            "sub	%[dly], #1"			"\n\t"
            "bne	L%=_loop_delay_T0H"		"\n\t"
            "strb	%[bitmask], [%[reg], #4]"	"\n\t"
            "movs	%[dly], #13"			"\n\t"
            "L%=_loop_delay_T0L:"			"\n\t"
            "sub	%[dly], #1"			"\n\t"
            "bne	L%=_loop_delay_T0L"		"\n\t"
            "b	L%=_next"			"\n\t"
            "L%=_loop_one:"
            "strb	%[bitmask], [%[reg], #0]"	"\n\t"
            "movs	%[dly], #13"			"\n\t"
            "L%=_loop_delay_T1H:"			"\n\t"
            "sub	%[dly], #1"			"\n\t"
            "bne	L%=_loop_delay_T1H"		"\n\t"
            "strb	%[bitmask], [%[reg], #4]"	"\n\t"
            "movs	%[dly], #4"			"\n\t"
            "L%=_loop_delay_T1L:"			"\n\t"
            "sub	%[dly], #1"			"\n\t"
            "bne	L%=_loop_delay_T1L"		"\n\t"
            "nop"					"\n\t"
            "L%=_next:"				"\n\t"
            "sub	%[count], #1"			"\n\t"
            "bne	L%=_loop"			"\n\t"
            "lsl	%[pix], #1"			"\n\t"
            "bcs	L%=_last_one"			"\n\t"
            "L%=_last_zero:"
            "strb	%[bitmask], [%[reg], #0]"	"\n\t"
            "movs	%[dly], #4"			"\n\t"
            "L%=_last_delay_T0H:"			"\n\t"
            "sub	%[dly], #1"			"\n\t"
            "bne	L%=_last_delay_T0H"		"\n\t"
            "strb	%[bitmask], [%[reg], #4]"	"\n\t"
            "movs	%[dly], #10"			"\n\t"
            "L%=_last_delay_T0L:"			"\n\t"
            "sub	%[dly], #1"			"\n\t"
            "bne	L%=_last_delay_T0L"		"\n\t"
            "b	L%=_repeat"			"\n\t"
            "L%=_last_one:"
            "strb	%[bitmask], [%[reg], #0]"	"\n\t"
            "movs	%[dly], #13"			"\n\t"
            "L%=_last_delay_T1H:"			"\n\t"
            "sub	%[dly], #1"			"\n\t"
            "bne	L%=_last_delay_T1H"		"\n\t"
            "strb	%[bitmask], [%[reg], #4]"	"\n\t"
            "movs	%[dly], #1"			"\n\t"
            "L%=_last_delay_T1L:"			"\n\t"
            "sub	%[dly], #1"			"\n\t"
            "bne	L%=_last_delay_T1L"		"\n\t"
            "nop"					"\n\t"
            "L%=_repeat:"				"\n\t"
            "add	%[p], #1"			"\n\t"
            "sub	%[num], #1"			"\n\t"
            "bne	L%=_begin"			"\n\t"
            "L%=_done:"				"\n\t"
            : [p] "+r" (p),
            [pix] "=&r" (pix),
            [count] "=&r" (count),
            [dly] "=&r" (dly),
            [num] "+r" (num)
            : [bitmask] "r" (bitmask),
            [reg] "r" (reg)
            );
    }
};

class NeoArmMk26z64SpeedWs2812x : public NeoArmMk26z64Speed800KbpsBase
{
public:
    const static uint32_t ResetTimeUs = 300;
};

class NeoArmMk26z64SpeedSk6812 : public NeoArmMk26z64Speed800KbpsBase
{
public:
    const static uint32_t ResetTimeUs = 80;
};

class NeoArmMk26z64SpeedTm1814 : public NeoArmMk26z64Speed800KbpsBase
{
public:
    const static uint32_t ResetTimeUs = 200;
};

class NeoArmMk26z64SpeedTm1829 : public NeoArmMk26z64Speed800KbpsBase
{
public:
    const static uint32_t ResetTimeUs = 200;
};

class NeoArmMk26z64Speed800Kbps : public NeoArmMk26z64Speed800KbpsBase
{
public:
    const static uint32_t ResetTimeUs = 50;
};

typedef NeoArmMethodBase<NeoArmMk26z64SpeedWs2812x> NeoArmWs2812xMethod;
typedef NeoArmMethodBase<NeoArmMk26z64SpeedSk6812> NeoArmSk6812Method;
typedef NeoArmMethodBase<NeoArmMk26z64SpeedTm1814> NeoArmTm1814InvertedMethod;
typedef NeoArmMethodBase<NeoArmMk26z64SpeedTm1829> NeoArmTm1829InvertedMethod;
typedef NeoArmMethodBase<NeoArmMk26z64Speed800Kbps> NeoArm800KbpsMethod;
typedef NeoArm800KbpsMethod NeoArmApa106Method;

#else
#error "Teensy-LC: Sorry, only 48 MHz is supported, please set Tools > CPU Speed to 48 MHz"
#endif // F_CPU == 48000000

#elif defined(__SAMD21G18A__) // Arduino Zero


class NeoArmSamd21g18aSpeedProps800KbpsBase
{
public:
    static void BitPreWait()
    {
        asm("nop; nop; nop; nop; nop; nop; nop; nop;");
    }
    static void BitT1hWait()
    {
        asm("nop; nop; nop; nop; nop; nop; nop; nop;"
            "nop; nop; nop; nop; nop; nop; nop; nop;"
            "nop; nop; nop; nop;");
    }
    static void BitT0lWait()
    {
        asm("nop; nop; nop; nop; nop; nop; nop; nop;"
            "nop; nop; nop; nop; nop; nop; nop; nop;"
            "nop; nop; nop; nop;");
    }
    static void BitPostWait()
    {
        asm("nop; nop; nop; nop; nop; nop; nop; nop; nop;");
    }
};

class NeoArmSamd21g18aSpeedPropsWs2812x : public NeoArmSamd21g18aSpeedProps800KbpsBase
{
public:
    static const uint32_t ResetTimeUs = 300;
};

class NeoArmSamd21g18aSpeedPropsSk6812 : public NeoArmSamd21g18aSpeedProps800KbpsBase
{
public:
    static const uint32_t ResetTimeUs = 80;
};

class NeoArmSamd21g18aSpeedPropsTm1814 : public NeoArmSamd21g18aSpeedProps800KbpsBase
{
public:
    static const uint32_t ResetTimeUs = 200;
};

class NeoArmSamd21g18aSpeedPropsTm1829 : public NeoArmSamd21g18aSpeedProps800KbpsBase
{
public:
    static const uint32_t ResetTimeUs = 200;
};

class NeoArmSamd21g18aSpeedProps800Kbps : public NeoArmSamd21g18aSpeedProps800KbpsBase
{
public:
    static const uint32_t ResetTimeUs = 50;
};


class NeoArmSamd21g18aSpeedProps400Kbps
{
public:
    static void BitPreWait()
    {
        asm("nop; nop; nop; nop; nop; nop; nop; nop; nop; nop; nop;");
    }
    static void BitT1hWait()
    {
        asm("nop; nop; nop; nop; nop; nop; nop; nop;"
            "nop; nop; nop; nop; nop; nop; nop; nop;"
            "nop; nop; nop; nop; nop; nop; nop; nop;"
            "nop; nop; nop;");
    }
    static void BitT0lWait()
    {
        asm("nop; nop; nop; nop; nop; nop; nop; nop;"
            "nop; nop; nop; nop; nop; nop; nop; nop;"
            "nop; nop; nop; nop; nop; nop; nop; nop;"
            "nop; nop; nop;");
    }
    static void BitPostWait()
    {
        asm("nop; nop; nop; nop; nop; nop; nop;");
    }
    static const uint32_t ResetTimeUs = 50;
};

template<typename T_SPEEDPROPS> class NeoArmSamd21g18aSpeedBase
{
public:
    static const uint32_t ResetTimeUs = T_SPEEDPROPS::ResetTimeUs;

    static void send_pixels(uint8_t* pixels, size_t sizePixels, uint8_t pin)
    {
        // Tried this with a timer/counter, couldn't quite get adequate
        // resolution.  So yay, you get a load of goofball NOPs...
        uint8_t* ptr = pixels;
        uint8_t* end = ptr + sizePixels;;
        uint8_t p = *ptr++;
        uint8_t bitMask = 0x80;
        uint8_t portNum = g_APinDescription[pin].ulPort;
        uint32_t pinMask = 1ul << g_APinDescription[pin].ulPin;

        volatile uint32_t* set = &(PORT->Group[portNum].OUTSET.reg);
        volatile uint32_t* clr = &(PORT->Group[portNum].OUTCLR.reg);

        for (;;)
        {
            *set = pinMask;
            T_SPEEDPROPS::BitPreWait();

            if (p & bitMask)
            {
                T_SPEEDPROPS::BitT1hWait();
                *clr = pinMask;
            }
            else
            {
                *clr = pinMask;
                T_SPEEDPROPS::BitT0lWait();
            }
            if (bitMask >>= 1)
            {
                T_SPEEDPROPS::BitPostWait();
            }
            else
            {
                if (ptr >= end)
                {
                    break;
                }
                p = *ptr++;
                bitMask = 0x80;
            }
        }
    }
};

typedef NeoArmMethodBase<NeoArmSamd21g18aSpeedBase<NeoArmSamd21g18aSpeedPropsWs2812x>> NeoArmWs2812xMethod;
typedef NeoArmMethodBase<NeoArmSamd21g18aSpeedBase<NeoArmSamd21g18aSpeedPropsSk6812>> NeoArmSk6812Method;
typedef NeoArmMethodBase<NeoArmSamd21g18aSpeedBase<NeoArmSamd21g18aSpeedPropsTm1814>> NeoArmTm1814InvertedMethod;
typedef NeoArmMethodBase<NeoArmSamd21g18aSpeedBase<NeoArmSamd21g18aSpeedPropsTm1829>> NeoArmTm1829InvertedMethod;
typedef NeoArmMethodBase<NeoArmSamd21g18aSpeedBase<NeoArmSamd21g18aSpeedProps800Kbps>> NeoArm800KbpsMethod;
typedef NeoArmMethodBase<NeoArmSamd21g18aSpeedBase<NeoArmSamd21g18aSpeedProps400Kbps>> NeoArm400KbpsMethod;
typedef NeoArm400KbpsMethod NeoArmApa106Method;

#elif defined(ARDUINO_STM32_FEATHER) || defined(ARDUINO_ARCH_STM32L4) || defined(ARDUINO_ARCH_STM32F4) || defined(ARDUINO_ARCH_STM32F1)// FEATHER WICED (120MHz)

class NeoArmStm32SpeedProps800KbpsBase
{
public:
    static void BitT1hWait()
    {
        asm("nop; nop; nop; nop; nop; nop; nop; nop;"
            "nop; nop; nop; nop; nop; nop; nop; nop;"
            "nop; nop; nop; nop; nop; nop; nop; nop;"
            "nop; nop; nop; nop; nop; nop; nop; nop;"
            "nop; nop; nop; nop; nop; nop; nop; nop;"
            "nop; nop; nop; nop; nop; nop; nop; nop;"
            "nop; nop; nop; nop; nop; nop; nop; nop;"
            "nop; nop; nop; nop; nop; nop; nop; nop;"
            "nop; nop; nop; nop; nop; nop; nop; nop;"
            "nop; nop; nop; nop; nop; nop; nop; nop;"
            "nop; nop; nop; nop; nop; nop; nop; nop;"
            "nop; nop; nop; nop; nop; nop;");
    }
    static void BitT1lWait()
    {
        asm("nop; nop; nop; nop; nop; nop; nop; nop;"
            "nop; nop; nop; nop; nop; nop; nop; nop;"
            "nop; nop; nop; nop; nop; nop; nop; nop;"
            "nop; nop; nop; nop; nop; nop; nop; nop;"
            "nop; nop; nop; nop; nop; nop;");
    }
    static void BitT0hWait()
    {
        asm("nop; nop; nop; nop; nop; nop; nop; nop;"
            "nop; nop; nop; nop; nop; nop; nop; nop;"
            "nop; nop; nop; nop; nop; nop; nop; nop;"
            "nop; nop; nop; nop; nop; nop; nop; nop;"
            "nop; nop; nop; nop; nop; nop; nop; nop;"
            "nop;");
    }
    static void BitT0lWait()
    {
        asm("nop; nop; nop; nop; nop; nop; nop; nop;"
            "nop; nop; nop; nop; nop; nop; nop; nop;"
            "nop; nop; nop; nop; nop; nop; nop; nop;"
            "nop; nop; nop; nop; nop; nop; nop; nop;"
            "nop; nop; nop; nop; nop; nop; nop; nop;"
            "nop; nop; nop; nop; nop; nop; nop; nop;"
            "nop; nop; nop; nop; nop; nop; nop; nop;"
            "nop; nop; nop; nop; nop; nop; nop; nop;"
            "nop; nop; nop; nop; nop; nop; nop; nop;"
            "nop; nop; nop; nop; nop; nop; nop; nop;"
            "nop; nop; nop; nop; nop; nop; nop; nop;"
            "nop; nop; nop; nop;");
    }
};

class NeoArmStm32SpeedPropsWs2812x : public NeoArmStm32SpeedProps800KbpsBase
{
public:
    static const uint32_t ResetTimeUs = 300;
};

class NeoArmStm32SpeedPropsSk6812 : public NeoArmStm32SpeedProps800KbpsBase
{
public:
    static const uint32_t ResetTimeUs = 80;
};

class NeoArmStm32SpeedPropsTm1814 : public NeoArmStm32SpeedProps800KbpsBase
{
public:
    static const uint32_t ResetTimeUs = 200;
};

class NeoArmStm32SpeedPropsTm1829 : public NeoArmStm32SpeedProps800KbpsBase
{
public:
    static const uint32_t ResetTimeUs = 200;
};

class NeoArmStm32SpeedProps800Kbps : public NeoArmStm32SpeedProps800KbpsBase
{
public:
    static const uint32_t ResetTimeUs = 50;
};

/* TODO - not found in Adafruit library
class NeoArmStm32SpeedProps400Kbps
{
static void BitT1hWait()
{
}
static void BitT1lWait()
{
}
static void BitT0hWait()
{
}
static void BitT0lWait()
{
}
};
*/

template<typename T_SPEEDPROPS> class NeoArmStm32SpeedBase
{
public:
    static const uint32_t ResetTimeUs = T_SPEEDPROPS::ResetTimeUs;

    static void send_pixels(uint8_t* pixels, size_t sizePixels, uint8_t pin)
    {
        // Tried this with a timer/counter, couldn't quite get adequate
        // resolution.  So yay, you get a load of goofball NOPs...

        uint8_t* ptr = pixels;
        uint8_t* end = ptr + sizePixels;
        uint8_t p = *ptr++;
        uint8_t bitMask = 0x80;

#if defined(ARDUINO_STM32_FEATHER)
        uint32_t  pinMask = BIT(PIN_MAP[pin].gpio_bit);

        volatile uint16_t* set = &(PIN_MAP[pin].gpio_device->regs->BSRRL);
        volatile uint16_t* clr = &(PIN_MAP[pin].gpio_device->regs->BSRRH);

#elif defined(ARDUINO_ARCH_STM32F4)
        uint32_t  pinMask = BIT(pin & 0x0f);

        volatile uint16_t* set = &(PIN_MAP[pin].gpio_device->regs->BSRRL);
        volatile uint16_t* clr = &(PIN_MAP[pin].gpio_device->regs->BSRRH);

#elif defined(ARDUINO_ARCH_STM32F1)

        uint32_t  pinMask = BIT(PIN_MAP[pin].gpio_bit);

        volatile uint32_t* set = &(PIN_MAP[pin].gpio_device->regs->BRR);
        volatile uint32_t* clr = &(PIN_MAP[pin].gpio_device->regs->BSRR);

#elif defined(ARDUINO_ARCH_STM32L4)

        uint32_t pinMask = g_APinDescription[pin].bit;

        GPIO_TypeDef* GPIO = static_cast<GPIO_TypeDef*>(g_APinDescription[pin].GPIO);

        volatile uint32_t* set = &(GPIO->BRR);
        volatile uint32_t* clr = &(GPIO->BSRR);

#endif
        for (;;)
        {
            if (p & bitMask)
            {
                // ONE
                // High 800ns
                *set = pinMask;
                T_SPEEDPROPS::BitT1hWait();
                // Low 450ns
                *clr = pinMask;
                T_SPEEDPROPS::BitT1lWait();
            }
            else
            {
                // ZERO
                // High 400ns
                *set = pinMask;
                T_SPEEDPROPS::BitT0hWait();
                // Low 850ns
                *clr = pinMask;
                T_SPEEDPROPS::BitT0lWait();
            }
            if (bitMask >>= 1)
            {
                // Move on to the next pixel
                asm("nop;");
            }
            else
            {
                if (ptr >= end)
                {
                    break;
                }

                p = *ptr++;
                bitMask = 0x80;
            }
        }
    }
};

typedef NeoArmMethodBase<NeoArmStm32SpeedBase<NeoArmStm32SpeedPropsWs2812x>> NeoArmWs2812xMethod;
typedef NeoArmMethodBase<NeoArmStm32SpeedBase<NeoArmStm32SpeedPropsSk6812>> NeoArmSk6812Method;
typedef NeoArmMethodBase<NeoArmStm32SpeedBase<NeoArmStm32SpeedPropstm1814>> NeoArmTm1814InvertedMethod;
typedef NeoArmMethodBase<NeoArmStm32SpeedBase<NeoArmStm32SpeedPropstm1829>> NeoArmTm1829InvertedMethod;
typedef NeoArmMethodBase<NeoArmStm32SpeedBase<NeoArmStm32SpeedProps800Kbps>> NeoArm800KbpsMethod;
typedef NeoArm800KbpsMethod NeoArmApa106Method;

#else // Other ARM architecture -- Presumed Arduino Due


#define ARM_OTHER_SCALE  VARIANT_MCK / 2UL / 1000000UL
#define ARM_OTHER_INST   (2UL * F_CPU / VARIANT_MCK)

class NeoArmOtherSpeedProps800KbpsBase
{
public:
    static const uint32_t CyclesT0h = static_cast<uint32_t>((0.40 * ARM_OTHER_SCALE + 0.5) - (5 * ARM_OTHER_INST));
    static const uint32_t CyclesT1h = static_cast<uint32_t>((0.80 * ARM_OTHER_SCALE + 0.5) - (5 * ARM_OTHER_INST));
    static const uint32_t Cycles = static_cast<uint32_t>((1.25 * ARM_OTHER_SCALE + 0.5) - (5 * ARM_OTHER_INST));
};

class NeoArmOtherSpeedPropsWs2812x : public NeoArmOtherSpeedProps800KbpsBase
{
public:
    static const uint32_t ResetTimeUs = 300;
};

class NeoArmOtherSpeedPropsSk6812 : public NeoArmOtherSpeedProps800KbpsBase
{
public:
    static const uint32_t ResetTimeUs = 80;
};

class NeoArmOtherSpeedPropsTm1814 : public NeoArmOtherSpeedProps800KbpsBase
{
public:
    static const uint32_t ResetTimeUs = 200;
};

class NeoArmOtherSpeedPropsTm1829 : public NeoArmOtherSpeedProps800KbpsBase
{
public:
    static const uint32_t ResetTimeUs = 200;
};

class NeoArmOtherSpeedProps800Kbps : public NeoArmOtherSpeedProps800KbpsBase
{
public:
    static const uint32_t ResetTimeUs = 50;
};

class NeoArmOtherSpeedProps400Kbps
{
public:
    static const uint32_t CyclesT0h = static_cast<uint32_t>((0.50 * ARM_OTHER_SCALE + 0.5) - (5 * ARM_OTHER_INST));
    static const uint32_t CyclesT1h = static_cast<uint32_t>((1.20 * ARM_OTHER_SCALE + 0.5) - (5 * ARM_OTHER_INST));
    static const uint32_t Cycles = static_cast<uint32_t>((2.50 * ARM_OTHER_SCALE + 0.5) - (5 * ARM_OTHER_INST));
    static const uint32_t ResetTimeUs = 50;
};

template<typename T_SPEEDPROPS> class NeoArmOtherSpeedBase
{
public:
    static const uint32_t ResetTimeUs = T_SPEEDPROPS::ResetTimeUs;

    static void send_pixels(uint8_t* pixels, size_t sizePixels, uint8_t pin)
    {
        uint32_t pinMask;
        uint32_t t;
        Pio* port;
        volatile WoReg* portSet;
        volatile WoReg* portClear;
        volatile WoReg* timeValue;
        volatile WoReg* timeReset;
        uint8_t* p;
        uint8_t* end;
        uint8_t pix;
        uint8_t mask;

        pmc_set_writeprotect(false);
        pmc_enable_periph_clk(static_cast<uint32_t>(TC3_IRQn));

        TC_Configure(TC1, 0,
            TC_CMR_WAVE | TC_CMR_WAVSEL_UP | TC_CMR_TCCLKS_TIMER_CLOCK1);
        TC_Start(TC1, 0);

        pinMask = g_APinDescription[pin].ulPin; // Don't 'optimize' these into
        port = g_APinDescription[pin].pPort; // declarations above.  Want to
        portSet = &(port->PIO_SODR);            // burn a few cycles after
        portClear = &(port->PIO_CODR);            // starting timer to minimize
        timeValue = &(TC1->TC_CHANNEL[0].TC_CV);  // the initial 'while'.
        timeReset = &(TC1->TC_CHANNEL[0].TC_CCR);
        p = pixels;
        end = p + sizePixels;
        pix = *p++;
        mask = 0x80;

        for (;;)
        {
            if (pix & mask)
            {
                t = T_SPEEDPROPS::CyclesT1h;
            }
            else
            {
                t = T_SPEEDPROPS::CyclesT0h;
            }

            // wait for the end of the previous cycle
            while (*timeValue < T_SPEEDPROPS::Cycles);

            *portSet = pinMask;
            *timeReset = TC_CCR_CLKEN | TC_CCR_SWTRG;

            while (*timeValue < t);

            *portClear = pinMask;
            if (!(mask >>= 1))
            {
                // This 'inside-out' loop logic utilizes
                if (p >= end)
                {
                    break; // idle time to minimize inter-byte delays.
                }
                pix = *p++;
                mask = 0x80;
            }
        }

        // not really needed as the wait for latch does this and
        // while (*timeValue < T_SPEEDPROPS::Cycles); // Wait for last bit

        TC_Stop(TC1, 0);
    }
};

typedef NeoArmMethodBase<NeoArmOtherSpeedBase<NeoArmOtherSpeedPropsWs2812x>> NeoArmWs2812xMethod;
typedef NeoArmMethodBase<NeoArmOtherSpeedBase<NeoArmOtherSpeedPropsSk6812>> NeoArmSk6812Method;
typedef NeoArmMethodBase<NeoArmOtherSpeedBase<NeoArmOtherSpeedPropsTm1814>> NeoArmTm1814InvertedMethod;
typedef NeoArmMethodBase<NeoArmOtherSpeedBase<NeoArmOtherSpeedPropsTm1829>> NeoArmTm1829InvertedMethod;
typedef NeoArmMethodBase<NeoArmOtherSpeedBase<NeoArmOtherSpeedProps800Kbps>> NeoArm800KbpsMethod;
typedef NeoArmMethodBase<NeoArmOtherSpeedBase<NeoArmOtherSpeedProps400Kbps>> NeoArm400KbpsMethod;
typedef NeoArm400KbpsMethod NeoArmApa106Method;

#endif


// Arm doesn't have alternatives methods yet, so only one to make the default
typedef NeoArmWs2812xMethod NeoWs2813Method;
typedef NeoArmWs2812xMethod NeoWs2812xMethod;
typedef NeoArmWs2812xMethod NeoWs2811Method;
typedef NeoArmSk6812Method NeoSk6812Method;
typedef NeoArmSk6812Method NeoLc8812Method;
typedef NeoArm800KbpsMethod NeoWs2812Method;
typedef NeoArmApa106Method NeoApa106Method;
typedef NeoArmWs2812xMethod Neo800KbpsMethod;
#ifdef NeoArm400KbpsMethod // this is needed due to missing 400Kbps for some platforms
typedef NeoArm400KbpsMethod Neo400KbpsMethod;
#endif
// there is no invert methods for arm, but the norm for TM1814 is inverted, so
typedef NeoArmTm1814InvertedMethod NeoTm1814InvertedMethod;
typedef NeoArmTm1829InvertedMethod NeoTm1829InvertedMethod;

#endif // defined(__arm__)

