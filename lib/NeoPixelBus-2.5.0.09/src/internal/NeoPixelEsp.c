/*-------------------------------------------------------------------------
NeoPixel library helper functions for Esp8266 and Esp32.

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
#if defined(ARDUINO_ARCH_ESP8266)
#include <eagle_soc.h>
#endif

// ESP32 doesn't define ICACHE_RAM_ATTR
#ifndef ICACHE_RAM_ATTR
#define ICACHE_RAM_ATTR IRAM_ATTR
#endif

static uint32_t _getCycleCount(void) __attribute__((always_inline));

static inline uint32_t _getCycleCount(void)
{
    uint32_t ccount;
    __asm__ __volatile__("rsr %0,ccount":"=a" (ccount));
    return ccount;
}

#define CYCLES_800_T0H  (F_CPU / 2500000) // 0.4us
#define CYCLES_800_T1H  (F_CPU / 1250000) // 0.8us
#define CYCLES_800      (F_CPU /  800000) // 1.25us per bit
#define CYCLES_400_T0H  (F_CPU / 2000000)
#define CYCLES_400_T1H  (F_CPU /  833333)
#define CYCLES_400      (F_CPU /  400000) 

void ICACHE_RAM_ATTR bitbang_send_pixels_800(uint8_t* pixels, uint8_t* end, uint8_t pin)
{
    const uint32_t pinRegister = _BV(pin);
    uint8_t mask = 0x80;
    uint8_t subpix = *pixels++;
    uint32_t cyclesStart = 0; // trigger emediately
	uint32_t cyclesNext = 0;

    for (;;)
    {
        // do the checks here while we are waiting on time to pass
		uint32_t cyclesBit = CYCLES_800_T0H;
		if (subpix & mask)
		{
			cyclesBit = CYCLES_800_T1H;
		}

        // after we have done as much work as needed for this next bit
        // now wait for the HIGH
		while (((cyclesStart = _getCycleCount()) - cyclesNext) < CYCLES_800);

        // set high
#if defined(ARDUINO_ARCH_ESP32)
        GPIO.out_w1ts = pinRegister;
#else
        GPIO_REG_WRITE(GPIO_OUT_W1TS_ADDRESS, pinRegister);
#endif
		
        // wait for the LOW
		while ((_getCycleCount() - cyclesStart) < cyclesBit);
		
        // set low
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

void ICACHE_RAM_ATTR bitbang_send_pixels_400(uint8_t* pixels, uint8_t* end, uint8_t pin)
{
	const uint32_t pinRegister = _BV(pin);
	uint8_t mask = 0x80;
	uint8_t subpix = *pixels++;
	uint32_t cyclesStart = 0; // trigger emediately
	uint32_t cyclesNext = 0;

	for (;;)
	{
		// do the checks here while we are waiting on time to pass
		uint32_t cyclesBit = CYCLES_400_T0H;
		if (subpix & mask)
		{
			cyclesBit = CYCLES_400_T1H;
		}

		// after we have done as much work as needed for this next bit
		// now wait for the HIGH
		while (((cyclesStart = _getCycleCount()) - cyclesNext) < CYCLES_400);

		// set high
#if defined(ARDUINO_ARCH_ESP32)
		GPIO.out_w1ts = pinRegister;
#else
		GPIO_REG_WRITE(GPIO_OUT_W1TS_ADDRESS, pinRegister);
#endif

		// wait for the LOW
		while ((_getCycleCount() - cyclesStart) < cyclesBit);

		// set low
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

#endif
