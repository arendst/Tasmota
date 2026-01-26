/*-------------------------------------------------------------------------
NeoPixel library helper functions for Esp8266 and Esp32

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

#if defined(ARDUINO_ARCH_ESP8266) || defined(ARDUINO_ARCH_ESP32)

#include <Arduino.h>

// ESP32C3 I2S is not supported yet
#if !defined(CONFIG_IDF_TARGET_ESP32C3) && !defined(CONFIG_IDF_TARGET_ESP32C2) && !defined(CONFIG_IDF_TARGET_ESP32C5) && !defined(CONFIG_IDF_TARGET_ESP32C6)
#if !defined(ARDUINO_ARCH_ESP8266)
#include "soc/gpio_periph.h"
#endif

static inline uint32_t getCycleCount(void)
{
    uint32_t ccount;
    __asm__ __volatile__("rsr %0,ccount":"=a" (ccount));
    return ccount;
}

void IRAM_ATTR NeoEspBitBangBase_send_pixels(uint8_t* pixels, uint8_t* end, uint8_t pin, uint32_t t0h, uint32_t t1h, uint32_t period)
{
    const uint32_t pinRegister = _BV(pin);
    uint8_t mask = 0x80;
    uint8_t subpix = *pixels++;
    uint32_t cyclesStart = 0; // trigger emediately
    uint32_t cyclesNext = 0;

    for (;;)
    {
        // do the checks here while we are waiting on time to pass
        uint32_t cyclesBit = t0h;
        if (subpix & mask)
        {
            cyclesBit = t1h;
        }

        // after we have done as much work as needed for this next bit
        // now wait for the HIGH
        while (((cyclesStart = getCycleCount()) - cyclesNext) < period);

        // set pin state
#if defined(ARDUINO_ARCH_ESP32)
        GPIO.out_w1ts = pinRegister;
#else
        GPIO_REG_WRITE(GPIO_OUT_W1TS_ADDRESS, pinRegister);
#endif

        // wait for the LOW
        while ((getCycleCount() - cyclesStart) < cyclesBit);

        // reset pin start
#if defined(ARDUINO_ARCH_ESP32)
        GPIO.out_w1tc = pinRegister;
#else
        GPIO_REG_WRITE(GPIO_OUT_W1TC_ADDRESS, pinRegister);
#endif

        cyclesNext = cyclesStart;

        // next bit
        mask >>= 1;
        if (mask == 0)
        {
            // no more bits to send in this byte
            // check for another byte
            if (pixels >= end)
            {
                // no more bytes to send so stop
                break;
            }
            // reset mask to first bit and get the next byte
            mask = 0x80;
            subpix = *pixels++;
        }
    }
}

void IRAM_ATTR NeoEspBitBangBase_send_pixels_inv(uint8_t* pixels, uint8_t* end, uint8_t pin, uint32_t t0h, uint32_t t1h, uint32_t period)
{
    const uint32_t pinRegister = _BV(pin);
    uint8_t mask = 0x80;
    uint8_t subpix = *pixels++;
    uint32_t cyclesStart = 0; // trigger emediately
    uint32_t cyclesNext = 0;

    for (;;)
    {
        // do the checks here while we are waiting on time to pass
        uint32_t cyclesBit = t0h;
        if (subpix & mask)
        {
            cyclesBit = t1h;
        }

        // after we have done as much work as needed for this next bit
        // now wait for the HIGH
        while (((cyclesStart = getCycleCount()) - cyclesNext) < period);

        // set pin state
#if defined(ARDUINO_ARCH_ESP32)
        GPIO.out_w1tc = pinRegister;
#else
        GPIO_REG_WRITE(GPIO_OUT_W1TC_ADDRESS, pinRegister);
#endif

        // wait for the LOW
        while ((getCycleCount() - cyclesStart) < cyclesBit);

        // reset pin start
#if defined(ARDUINO_ARCH_ESP32)
        GPIO.out_w1ts = pinRegister;
#else
        GPIO_REG_WRITE(GPIO_OUT_W1TS_ADDRESS, pinRegister);
#endif

        cyclesNext = cyclesStart;

        // next bit
        mask >>= 1;
        if (mask == 0)
        {
            // no more bits to send in this byte
            // check for another byte
            if (pixels >= end)
            {
                // no more bytes to send so stop
                break;
            }
            // reset mask to first bit and get the next byte
            mask = 0x80;
            subpix = *pixels++;
        }
    }
}

#endif // !defined(CONFIG_IDF_TARGET_ESP32C3) && !defined(CONFIG_IDF_TARGET_ESP32C2) && !defined(CONFIG_IDF_TARGET_ESP32C6)
#endif //  defined(ARDUINO_ARCH_ESP8266) || defined(ARDUINO_ARCH_ESP32)
