/*-------------------------------------------------------------------------
Arduino library to control a wide variety of WS2811- and WS2812-based RGB
LED devices such as Adafruit FLORA RGB Smart Pixels and NeoPixel strips.
Currently handles 400 and 800 KHz bitstreams on 8, 12 and 16 MHz ATmega
MCUs, with LEDs wired for various color orders.  8 MHz MCUs provide
output on PORTB and PORTD, while 16 MHz chips can handle most output pins
(possible exception with upper PORT registers on the Arduino Mega).

Written by Phil Burgess / Paint Your Dragon for Adafruit Industries,
contributions by PJRC, Michael Miller and other members of the open
source community.

Adafruit invests time and resources providing this open source code,
please support Adafruit and open-source hardware by purchasing products
from Adafruit!

-------------------------------------------------------------------------
The contents of this file were taken from the Adafruit NeoPixel library
and modified only to fit within individual calling functions.

NeoPixel is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as
published by the Free Software Foundation, either version 3 of
the License, or (at your option) any later version.

NeoPixel is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with NeoPixel.  If not, see
<http://www.gnu.org/licenses/>.
-------------------------------------------------------------------------*/

// must also check for arm due to Teensy incorrectly having ARDUINO_ARCH_AVR set
#if defined(ARDUINO_ARCH_AVR) && !defined(__arm__)

#include <Arduino.h>

// Hand-tuned assembly code issues data to the LED drivers at a specific
// rate.  There's separate code for different CPU speeds (8, 12, 16 MHz)
// for both the WS2811 (400 KHz) and WS2812 (800 KHz) drivers.  The
// datastream timing for the LED drivers allows a little wiggle room each
// way (listed in the datasheets), so the conditions for compiling each
// case are set up for a range of frequencies rather than just the exact
// 8, 12 or 16 MHz values, permitting use with some close-but-not-spot-on
// devices (e.g. 16.5 MHz DigiSpark).  The ranges were arrived at based
// on the datasheet figures and have not been extensively tested outside
// the canonical 8/12/16 MHz speeds; there's no guarantee these will work
// close to the extremes (or possibly they could be pushed further).
// Keep in mind only one CPU speed case actually gets compiled; the
// resulting program isn't as massive as it might look from source here.

#if (F_CPU >= 7400000UL) && (F_CPU <= 9500000UL)  // 8Mhz CPU

#ifdef PORTD // PORTD isn't present on ATtiny85, etc.
void send_pixels_8mhz_800_PortD(uint8_t* pixels, size_t sizePixels, uint8_t pinMask)
{
    volatile size_t i = sizePixels; // Loop counter
    volatile uint8_t* ptr = pixels; // Pointer to next byte
    volatile uint8_t b = *ptr++;    // Current byte value
    volatile uint8_t hi;            // PORT w/output bit set high
    volatile uint8_t lo;            // PORT w/output bit set low

    volatile uint8_t n1;
    volatile n2 = 0;  // First, next bits out

    // Squeezing an 800 KHz stream out of an 8 MHz chip requires code
    // specific to each PORT register.  At present this is only written
    // to work with pins on PORTD or PORTB, the most likely use case --
    // this covers all the pins on the Adafruit Flora and the bulk of
    // digital pins on the Arduino Pro 8 MHz (keep in mind, this code
    // doesn't even get compiled for 16 MHz boards like the Uno, Mega,
    // Leonardo, etc., so don't bother extending this out of hand).
    // Additional PORTs could be added if you really need them, just
    // duplicate the else and loop and change the PORT.  Each add'l
    // PORT will require about 150(ish) bytes of program space.

    // 10 instruction clocks per bit: HHxxxxxLLL
    // OUT instructions:              ^ ^    ^   (T=0,2,7)

    hi = PORTD | pinMask;
    lo = PORTD & ~pinMask;
    n1 = lo;

    if (b & 0x80)
    {
        n1 = hi;
    }

    // Dirty trick: RJMPs proceeding to the next instruction are used
    // to delay two clock cycles in one instruction word (rather than
    // using two NOPs).  This was necessary in order to squeeze the
    // loop down to exactly 64 words -- the maximum possible for a
    // relative branch.

    asm volatile(
        "headD:"                   "\n\t" // Clk  Pseudocode
                                            // Bit 7:
        "out  %[port] , %[hi]"    "\n\t" // 1    PORT = hi
        "mov  %[n2]   , %[lo]"    "\n\t" // 1    n2   = lo
        "out  %[port] , %[n1]"    "\n\t" // 1    PORT = n1
        "rjmp .+0"                "\n\t" // 2    nop nop
        "sbrc %[byte] , 6"        "\n\t" // 1-2  if(b & 0x40)
        "mov %[n2]   , %[hi]"    "\n\t" // 0-1   n2 = hi
        "out  %[port] , %[lo]"    "\n\t" // 1    PORT = lo
        "rjmp .+0"                "\n\t" // 2    nop nop
                                            // Bit 6:
        "out  %[port] , %[hi]"    "\n\t" // 1    PORT = hi
        "mov  %[n1]   , %[lo]"    "\n\t" // 1    n1   = lo
        "out  %[port] , %[n2]"    "\n\t" // 1    PORT = n2
        "rjmp .+0"                "\n\t" // 2    nop nop
        "sbrc %[byte] , 5"        "\n\t" // 1-2  if(b & 0x20)
        "mov %[n1]   , %[hi]"    "\n\t" // 0-1   n1 = hi
        "out  %[port] , %[lo]"    "\n\t" // 1    PORT = lo
        "rjmp .+0"                "\n\t" // 2    nop nop
                                            // Bit 5:
        "out  %[port] , %[hi]"    "\n\t" // 1    PORT = hi
        "mov  %[n2]   , %[lo]"    "\n\t" // 1    n2   = lo
        "out  %[port] , %[n1]"    "\n\t" // 1    PORT = n1
        "rjmp .+0"                "\n\t" // 2    nop nop
        "sbrc %[byte] , 4"        "\n\t" // 1-2  if(b & 0x10)
        "mov %[n2]   , %[hi]"    "\n\t" // 0-1   n2 = hi
        "out  %[port] , %[lo]"    "\n\t" // 1    PORT = lo
        "rjmp .+0"                "\n\t" // 2    nop nop
                                            // Bit 4:
        "out  %[port] , %[hi]"    "\n\t" // 1    PORT = hi
        "mov  %[n1]   , %[lo]"    "\n\t" // 1    n1   = lo
        "out  %[port] , %[n2]"    "\n\t" // 1    PORT = n2
        "rjmp .+0"                "\n\t" // 2    nop nop
        "sbrc %[byte] , 3"        "\n\t" // 1-2  if(b & 0x08)
        "mov %[n1]   , %[hi]"    "\n\t" // 0-1   n1 = hi
        "out  %[port] , %[lo]"    "\n\t" // 1    PORT = lo
        "rjmp .+0"                "\n\t" // 2    nop nop
                                            // Bit 3:
        "out  %[port] , %[hi]"    "\n\t" // 1    PORT = hi
        "mov  %[n2]   , %[lo]"    "\n\t" // 1    n2   = lo
        "out  %[port] , %[n1]"    "\n\t" // 1    PORT = n1
        "rjmp .+0"                "\n\t" // 2    nop nop
        "sbrc %[byte] , 2"        "\n\t" // 1-2  if(b & 0x04)
        "mov %[n2]   , %[hi]"    "\n\t" // 0-1   n2 = hi
        "out  %[port] , %[lo]"    "\n\t" // 1    PORT = lo
        "rjmp .+0"                "\n\t" // 2    nop nop
                                            // Bit 2:
        "out  %[port] , %[hi]"    "\n\t" // 1    PORT = hi
        "mov  %[n1]   , %[lo]"    "\n\t" // 1    n1   = lo
        "out  %[port] , %[n2]"    "\n\t" // 1    PORT = n2
        "rjmp .+0"                "\n\t" // 2    nop nop
        "sbrc %[byte] , 1"        "\n\t" // 1-2  if(b & 0x02)
        "mov %[n1]   , %[hi]"    "\n\t" // 0-1   n1 = hi
        "out  %[port] , %[lo]"    "\n\t" // 1    PORT = lo
        "rjmp .+0"                "\n\t" // 2    nop nop
                                            // Bit 1:
        "out  %[port] , %[hi]"    "\n\t" // 1    PORT = hi
        "mov  %[n2]   , %[lo]"    "\n\t" // 1    n2   = lo
        "out  %[port] , %[n1]"    "\n\t" // 1    PORT = n1
        "rjmp .+0"                "\n\t" // 2    nop nop
        "sbrc %[byte] , 0"        "\n\t" // 1-2  if(b & 0x01)
        "mov %[n2]   , %[hi]"    "\n\t" // 0-1   n2 = hi
        "out  %[port] , %[lo]"    "\n\t" // 1    PORT = lo
        "sbiw %[count], 1"        "\n\t" // 2    i-- (don't act on Z flag yet)
                                            // Bit 0:
        "out  %[port] , %[hi]"    "\n\t" // 1    PORT = hi
        "mov  %[n1]   , %[lo]"    "\n\t" // 1    n1   = lo
        "out  %[port] , %[n2]"    "\n\t" // 1    PORT = n2
        "ld   %[byte] , %a[ptr]+" "\n\t" // 2    b = *ptr++
        "sbrc %[byte] , 7"        "\n\t" // 1-2  if(b & 0x80)
        "mov %[n1]   , %[hi]"    "\n\t" // 0-1   n1 = hi
        "out  %[port] , %[lo]"    "\n\t" // 1    PORT = lo
        "brne headD"              "\n"   // 2    while(i) (Z flag set above)
        : [byte]  "+r" (b),
        [n1]    "+r" (n1),
        [n2]    "+r" (n2),
        [count] "+w" (i)
        : [port]   "I" (_SFR_IO_ADDR(PORTD)),
        [ptr]    "e" (ptr),
        [hi]     "r" (hi),
        [lo]     "r" (lo) );
}
#endif

void send_pixels_8mhz_800_PortB(uint8_t* pixels, size_t sizePixels, uint8_t pinMask)
{
    volatile size_t i = sizePixels; // Loop counter
    volatile uint8_t* ptr = pixels; // Pointer to next byte
    volatile uint8_t b = *ptr++;    // Current byte value
    volatile uint8_t hi;            // PORT w/output bit set high
    volatile uint8_t lo;            // PORT w/output bit set low

    volatile uint8_t n1;
    volatile n2 = 0;  // First, next bits out

    // Same as above, just switched to PORTB and stripped of comments.
    hi = PORTB | pinMask;
    lo = PORTB & ~pinMask;
    n1 = lo;
    if (b & 0x80)
    {
        n1 = hi;
    }

    asm volatile(
        "headB:"                   "\n\t"
        "out  %[port] , %[hi]"    "\n\t"
        "mov  %[n2]   , %[lo]"    "\n\t"
        "out  %[port] , %[n1]"    "\n\t"
        "rjmp .+0"                "\n\t"
        "sbrc %[byte] , 6"        "\n\t"
        "mov %[n2]   , %[hi]"    "\n\t"
        "out  %[port] , %[lo]"    "\n\t"
        "rjmp .+0"                "\n\t"
        "out  %[port] , %[hi]"    "\n\t"
        "mov  %[n1]   , %[lo]"    "\n\t"
        "out  %[port] , %[n2]"    "\n\t"
        "rjmp .+0"                "\n\t"
        "sbrc %[byte] , 5"        "\n\t"
        "mov %[n1]   , %[hi]"    "\n\t"
        "out  %[port] , %[lo]"    "\n\t"
        "rjmp .+0"                "\n\t"
        "out  %[port] , %[hi]"    "\n\t"
        "mov  %[n2]   , %[lo]"    "\n\t"
        "out  %[port] , %[n1]"    "\n\t"
        "rjmp .+0"                "\n\t"
        "sbrc %[byte] , 4"        "\n\t"
        "mov %[n2]   , %[hi]"    "\n\t"
        "out  %[port] , %[lo]"    "\n\t"
        "rjmp .+0"                "\n\t"
        "out  %[port] , %[hi]"    "\n\t"
        "mov  %[n1]   , %[lo]"    "\n\t"
        "out  %[port] , %[n2]"    "\n\t"
        "rjmp .+0"                "\n\t"
        "sbrc %[byte] , 3"        "\n\t"
        "mov %[n1]   , %[hi]"    "\n\t"
        "out  %[port] , %[lo]"    "\n\t"
        "rjmp .+0"                "\n\t"
        "out  %[port] , %[hi]"    "\n\t"
        "mov  %[n2]   , %[lo]"    "\n\t"
        "out  %[port] , %[n1]"    "\n\t"
        "rjmp .+0"                "\n\t"
        "sbrc %[byte] , 2"        "\n\t"
        "mov %[n2]   , %[hi]"    "\n\t"
        "out  %[port] , %[lo]"    "\n\t"
        "rjmp .+0"                "\n\t"
        "out  %[port] , %[hi]"    "\n\t"
        "mov  %[n1]   , %[lo]"    "\n\t"
        "out  %[port] , %[n2]"    "\n\t"
        "rjmp .+0"                "\n\t"
        "sbrc %[byte] , 1"        "\n\t"
        "mov %[n1]   , %[hi]"    "\n\t"
        "out  %[port] , %[lo]"    "\n\t"
        "rjmp .+0"                "\n\t"
        "out  %[port] , %[hi]"    "\n\t"
        "mov  %[n2]   , %[lo]"    "\n\t"
        "out  %[port] , %[n1]"    "\n\t"
        "rjmp .+0"                "\n\t"
        "sbrc %[byte] , 0"        "\n\t"
        "mov %[n2]   , %[hi]"    "\n\t"
        "out  %[port] , %[lo]"    "\n\t"
        "sbiw %[count], 1"        "\n\t"
        "out  %[port] , %[hi]"    "\n\t"
        "mov  %[n1]   , %[lo]"    "\n\t"
        "out  %[port] , %[n2]"    "\n\t"
        "ld   %[byte] , %a[ptr]+" "\n\t"
        "sbrc %[byte] , 7"        "\n\t"
        "mov %[n1]   , %[hi]"    "\n\t"
        "out  %[port] , %[lo]"    "\n\t"
        "brne headB"              "\n"
        : [byte] "+r" (b), [n1] "+r" (n1), [n2] "+r" (n2), [count] "+w" (i)
        : [port] "I" (_SFR_IO_ADDR(PORTB)), [ptr] "e" (ptr), [hi] "r" (hi),
        [lo] "r" (lo));
}

void send_pixels_8mhz_400(uint8_t* pixels, size_t sizePixels, volatile uint8_t* port, uint8_t pinMask)
{
    volatile size_t i = sizePixels; // Loop counter
    volatile uint8_t* ptr = pixels; // Pointer to next byte
    volatile uint8_t b = *ptr++;    // Current byte value
    volatile uint8_t hi;            // PORT w/output bit set high
    volatile uint8_t lo;            // PORT w/output bit set low

    // Timing is more relaxed; unrolling the inner loop for each bit is
    // not necessary.  Still using the peculiar RJMPs as 2X NOPs, not out
    // of need but just to trim the code size down a little.
    // This 400-KHz-datastream-on-8-MHz-CPU code is not quite identical
    // to the 800-on-16 code later -- the hi/lo timing between WS2811 and
    // WS2812 is not simply a 2:1 scale!

    // 20 inst. clocks per bit: HHHHxxxxxxLLLLLLLLLL
    // ST instructions:         ^   ^     ^          (T=0,4,10)

    volatile uint8_t next, bit;

    hi = *port | pinMask;
    lo = *port & ~pinMask;
    next = lo;
    bit = 8;

    asm volatile(
        "head20:"                  "\n\t" // Clk  Pseudocode    (T =  0)
        "st   %a[port], %[hi]"    "\n\t" // 2    PORT = hi     (T =  2)
        "sbrc %[byte] , 7"        "\n\t" // 1-2  if(b & 128)
        "mov  %[next], %[hi]"    "\n\t" // 0-1   next = hi    (T =  4)
        "st   %a[port], %[next]"  "\n\t" // 2    PORT = next   (T =  6)
        "mov  %[next] , %[lo]"    "\n\t" // 1    next = lo     (T =  7)
        "dec  %[bit]"             "\n\t" // 1    bit--         (T =  8)
        "breq nextbyte20"         "\n\t" // 1-2  if(bit == 0)
        "rol  %[byte]"            "\n\t" // 1    b <<= 1       (T = 10)
        "st   %a[port], %[lo]"    "\n\t" // 2    PORT = lo     (T = 12)
        "rjmp .+0"                "\n\t" // 2    nop nop       (T = 14)
        "rjmp .+0"                "\n\t" // 2    nop nop       (T = 16)
        "rjmp .+0"                "\n\t" // 2    nop nop       (T = 18)
        "rjmp head20"             "\n\t" // 2    -> head20 (next bit out)
        "nextbyte20:"              "\n\t" //                    (T = 10)
        "st   %a[port], %[lo]"    "\n\t" // 2    PORT = lo     (T = 12)
        "nop"                     "\n\t" // 1    nop           (T = 13)
        "ldi  %[bit]  , 8"        "\n\t" // 1    bit = 8       (T = 14)
        "ld   %[byte] , %a[ptr]+" "\n\t" // 2    b = *ptr++    (T = 16)
        "sbiw %[count], 1"        "\n\t" // 2    i--           (T = 18)
        "brne head20"             "\n"   // 2    if(i != 0) -> (next byte)
        : [port]  "+e" (port),
        [byte]  "+r" (b),
        [bit]   "+r" (bit),
        [next]  "+r" (next),
        [count] "+w" (i)
        : [hi]    "r" (hi),
        [lo]    "r" (lo),
        [ptr]   "e" (ptr));
}

#elif (F_CPU >= 11100000UL) && (F_CPU <= 14300000UL)  // 12Mhz CPU

#ifdef PORTD // PORTD isn't present on ATtiny85, etc.
void send_pixels_12mhz_800_PortD(uint8_t* pixels, size_t sizePixels, uint8_t pinMask)
{
    volatile size_t i = sizePixels; // Loop counter
    volatile uint8_t* ptr = pixels; // Pointer to next byte
    volatile uint8_t b = *ptr++;    // Current byte value
    volatile uint8_t hi;            // PORT w/output bit set high
    volatile uint8_t lo;            // PORT w/output bit set low

    // In the 12 MHz case, an optimized 800 KHz datastream (no dead time
    // between bytes) requires a PORT-specific loop similar to the 8 MHz
    // code (but a little more relaxed in this case).

    // 15 instruction clocks per bit: HHHHxxxxxxLLLLL
    // OUT instructions:              ^   ^     ^     (T=0,4,10)

    volatile uint8_t next;

    hi = PORTD | pinMask;
    lo = PORTD & ~pinMask;
    next = lo;
    if (b & 0x80) next = hi;

    // Don't "optimize" the OUT calls into the bitTime subroutine;
    // we're exploiting the RCALL and RET as 3- and 4-cycle NOPs!
    asm volatile(
        "headD:"                   "\n\t" //        (T =  0)
        "out   %[port], %[hi]"    "\n\t" //        (T =  1)
        "rcall bitTimeD"          "\n\t" // Bit 7  (T = 15)
        "out   %[port], %[hi]"    "\n\t"
        "rcall bitTimeD"          "\n\t" // Bit 6
        "out   %[port], %[hi]"    "\n\t"
        "rcall bitTimeD"          "\n\t" // Bit 5
        "out   %[port], %[hi]"    "\n\t"
        "rcall bitTimeD"          "\n\t" // Bit 4
        "out   %[port], %[hi]"    "\n\t"
        "rcall bitTimeD"          "\n\t" // Bit 3
        "out   %[port], %[hi]"    "\n\t"
        "rcall bitTimeD"          "\n\t" // Bit 2
        "out   %[port], %[hi]"    "\n\t"
        "rcall bitTimeD"          "\n\t" // Bit 1
                                         // Bit 0:
        "out  %[port] , %[hi]"    "\n\t" // 1    PORT = hi    (T =  1)
        "rjmp .+0"                "\n\t" // 2    nop nop      (T =  3)
        "ld   %[byte] , %a[ptr]+" "\n\t" // 2    b = *ptr++   (T =  5)
        "out  %[port] , %[next]"  "\n\t" // 1    PORT = next  (T =  6)
        "mov  %[next] , %[lo]"    "\n\t" // 1    next = lo    (T =  7)
        "sbrc %[byte] , 7"        "\n\t" // 1-2  if(b & 0x80) (T =  8)
        "mov %[next] , %[hi]"    "\n\t" // 0-1    next = hi  (T =  9)
        "nop"                     "\n\t" // 1                 (T = 10)
        "out  %[port] , %[lo]"    "\n\t" // 1    PORT = lo    (T = 11)
        "sbiw %[count], 1"        "\n\t" // 2    i--          (T = 13)
        "brne headD"              "\n\t" // 2    if(i != 0) -> (next byte)
        "rjmp doneD"             "\n\t"
        "bitTimeD:"               "\n\t" //      nop nop nop     (T =  4)
        "out  %[port], %[next]"  "\n\t" // 1    PORT = next     (T =  5)
        "mov  %[next], %[lo]"    "\n\t" // 1    next = lo       (T =  6)
        "rol  %[byte]"           "\n\t" // 1    b <<= 1         (T =  7)
        "sbrc %[byte], 7"        "\n\t" // 1-2  if(b & 0x80)    (T =  8)
        "mov %[next], %[hi]"    "\n\t" // 0-1   next = hi      (T =  9)
        "nop"                    "\n\t" // 1                    (T = 10)
        "out  %[port], %[lo]"    "\n\t" // 1    PORT = lo       (T = 11)
        "ret"                    "\n\t" // 4    nop nop nop nop (T = 15)
        "doneD:"                 "\n"
        : [byte]  "+r" (b),
        [next]  "+r" (next),
        [count] "+w" (i)
        : [port]   "I" (_SFR_IO_ADDR(PORTD)),
        [ptr]    "e" (ptr),
        [hi]     "r" (hi),
        [lo]     "r" (lo));
}
#endif

void send_pixels_12mhz_800_PortB(uint8_t* pixels, size_t sizePixels, uint8_t pinMask)
{
    volatile uint16_t i = (uint16_t)sizePixels; // Loop counter
    volatile uint8_t* ptr = pixels; // Pointer to next byte
    volatile uint8_t b = *ptr++;    // Current byte value
    volatile uint8_t hi;            // PORT w/output bit set high
    volatile uint8_t lo;            // PORT w/output bit set low

    volatile uint8_t next;

    hi = PORTB | pinMask;
    lo = PORTB & ~pinMask;
    next = lo;
    if (b & 0x80)
    {
        next = hi;
    }

    // Same as above, just set for PORTB & stripped of comments
    asm volatile(
        "headB:"                   "\n\t"
        "out   %[port], %[hi]"    "\n\t"
        "rcall bitTimeB"          "\n\t"
        "out   %[port], %[hi]"    "\n\t"
        "rcall bitTimeB"          "\n\t"
        "out   %[port], %[hi]"    "\n\t"
        "rcall bitTimeB"          "\n\t"
        "out   %[port], %[hi]"    "\n\t"
        "rcall bitTimeB"          "\n\t"
        "out   %[port], %[hi]"    "\n\t"
        "rcall bitTimeB"          "\n\t"
        "out   %[port], %[hi]"    "\n\t"
        "rcall bitTimeB"          "\n\t"
        "out   %[port], %[hi]"    "\n\t"
        "rcall bitTimeB"          "\n\t"
        "out  %[port] , %[hi]"    "\n\t"
        "rjmp .+0"                "\n\t"
        "ld   %[byte] , %a[ptr]+" "\n\t"
        "out  %[port] , %[next]"  "\n\t"
        "mov  %[next] , %[lo]"    "\n\t"
        "sbrc %[byte] , 7"        "\n\t"
        "mov %[next] , %[hi]"    "\n\t"
        "nop"                     "\n\t"
        "out  %[port] , %[lo]"    "\n\t"
        "sbiw %[count], 1"        "\n\t"
        "brne headB"              "\n\t"
        "rjmp doneB"             "\n\t"
        "bitTimeB:"               "\n\t"
        "out  %[port], %[next]"  "\n\t"
        "mov  %[next], %[lo]"    "\n\t"
        "rol  %[byte]"           "\n\t"
        "sbrc %[byte], 7"        "\n\t"
        "mov %[next], %[hi]"    "\n\t"
        "nop"                    "\n\t"
        "out  %[port], %[lo]"    "\n\t"
        "ret"                    "\n\t"
        "doneB:"                 "\n"
        : [byte] "+r" (b), [next] "+r" (next), [count] "+w" (i)
        : [port] "I" (_SFR_IO_ADDR(PORTB)), [ptr] "e" (ptr), [hi] "r" (hi),
        [lo] "r" (lo));
}

void send_pixels_12mhz_400(uint8_t* pixels, size_t sizePixels, volatile uint8_t* port, uint8_t pinMask)
{
    volatile uint16_t i = (uint16_t)sizePixels; // Loop counter
    volatile uint8_t* ptr = pixels; // Pointer to next byte
    volatile uint8_t b = *ptr++;    // Current byte value
    volatile uint8_t hi;            // PORT w/output bit set high
    volatile uint8_t lo;            // PORT w/output bit set low

    // 30 instruction clocks per bit: HHHHHHxxxxxxxxxLLLLLLLLLLLLLLL
    // ST instructions:               ^     ^        ^    (T=0,6,15)

    volatile uint8_t next, bit;

    hi = *port | pinMask;
    lo = *port & ~pinMask;
    next = lo;
    bit = 8;

    asm volatile(
        "head30:"                  "\n\t" // Clk  Pseudocode    (T =  0)
        "st   %a[port], %[hi]"    "\n\t" // 2    PORT = hi     (T =  2)
        "sbrc %[byte] , 7"        "\n\t" // 1-2  if(b & 128)
        "mov  %[next], %[hi]"    "\n\t" // 0-1   next = hi    (T =  4)
        "rjmp .+0"                "\n\t" // 2    nop nop       (T =  6)
        "st   %a[port], %[next]"  "\n\t" // 2    PORT = next   (T =  8)
        "rjmp .+0"                "\n\t" // 2    nop nop       (T = 10)
        "rjmp .+0"                "\n\t" // 2    nop nop       (T = 12)
        "rjmp .+0"                "\n\t" // 2    nop nop       (T = 14)
        "nop"                     "\n\t" // 1    nop           (T = 15)
        "st   %a[port], %[lo]"    "\n\t" // 2    PORT = lo     (T = 17)
        "rjmp .+0"                "\n\t" // 2    nop nop       (T = 19)
        "dec  %[bit]"             "\n\t" // 1    bit--         (T = 20)
        "breq nextbyte30"         "\n\t" // 1-2  if(bit == 0)
        "rol  %[byte]"            "\n\t" // 1    b <<= 1       (T = 22)
        "rjmp .+0"                "\n\t" // 2    nop nop       (T = 24)
        "rjmp .+0"                "\n\t" // 2    nop nop       (T = 26)
        "rjmp .+0"                "\n\t" // 2    nop nop       (T = 28)
        "rjmp head30"             "\n\t" // 2    -> head30 (next bit out)
        "nextbyte30:"              "\n\t" //                    (T = 22)
        "nop"                     "\n\t" // 1    nop           (T = 23)
        "ldi  %[bit]  , 8"        "\n\t" // 1    bit = 8       (T = 24)
        "ld   %[byte] , %a[ptr]+" "\n\t" // 2    b = *ptr++    (T = 26)
        "sbiw %[count], 1"        "\n\t" // 2    i--           (T = 28)
        "brne head30"             "\n"   // 1-2  if(i != 0) -> (next byte)
        : [port]  "+e" (port),
        [byte]  "+r" (b),
        [bit]   "+r" (bit),
        [next]  "+r" (next),
        [count] "+w" (i)
        : [hi]     "r" (hi),
        [lo]     "r" (lo),
        [ptr]    "e" (ptr));
}

#elif (F_CPU >= 15400000UL) && (F_CPU <= 19000000L)  // 16Mhz CPU

void send_pixels_16mhz_800(uint8_t* pixels, size_t sizePixels, volatile uint8_t* port, uint8_t pinMask)
{
    volatile uint16_t i = (uint16_t)sizePixels; // Loop counter
    volatile uint8_t* ptr = pixels; // Pointer to next byte
    volatile uint8_t b = *ptr++;    // Current byte value
    volatile uint8_t hi;            // PORT w/output bit set high
    volatile uint8_t lo;            // PORT w/output bit set low

    // WS2811 and WS2812 have different hi/lo duty cycles; this is
    // similar but NOT an exact copy of the prior 400-on-8 code.

    // 20 inst. clocks per bit: HHHHHxxxxxxxxLLLLLLL
    // ST instructions:         ^   ^        ^       (T=0,5,13)

    volatile uint8_t next;
    volatile uint8_t bit;

    hi = *port | pinMask;
    lo = *port & ~pinMask;
    next = lo;
    bit = 8;

    asm volatile(
        "head20:"                   "\n\t" // Clk  Pseudocode    (T =  0)
        "st   %a[port],  %[hi]"    "\n\t" // 2    PORT = hi     (T =  2)
        "sbrc %[byte],  7"         "\n\t" // 1-2  if(b & 128)
        "mov  %[next], %[hi]"     "\n\t" // 0-1   next = hi    (T =  4)
        "dec  %[bit]"              "\n\t" // 1    bit--         (T =  5)
        "st   %a[port],  %[next]"  "\n\t" // 2    PORT = next   (T =  7)
        "mov  %[next] ,  %[lo]"    "\n\t" // 1    next = lo     (T =  8)
        "breq nextbyte20"          "\n\t" // 1-2  if(bit == 0) (from dec above)
        "rol  %[byte]"             "\n\t" // 1    b <<= 1       (T = 10)
        "rjmp .+0"                 "\n\t" // 2    nop nop       (T = 12)
        "nop"                      "\n\t" // 1    nop           (T = 13)
        "st   %a[port],  %[lo]"    "\n\t" // 2    PORT = lo     (T = 15)
        "nop"                      "\n\t" // 1    nop           (T = 16)
        "rjmp .+0"                 "\n\t" // 2    nop nop       (T = 18)
        "rjmp head20"              "\n\t" // 2    -> head20 (next bit out)
        "nextbyte20:"               "\n\t" //                    (T = 10)
        "ldi  %[bit]  ,  8"        "\n\t" // 1    bit = 8       (T = 11)
        "ld   %[byte] ,  %a[ptr]+" "\n\t" // 2    b = *ptr++    (T = 13)
        "st   %a[port], %[lo]"     "\n\t" // 2    PORT = lo     (T = 15)
        "nop"                      "\n\t" // 1    nop           (T = 16)
        "sbiw %[count], 1"         "\n\t" // 2    i--           (T = 18)
        "brne head20"             "\n"   // 2    if(i != 0) -> (next byte)
        : [port]  "+e" (port),
        [byte]  "+r" (b),
        [bit]   "+r" (bit),
        [next]  "+r" (next),
        [count] "+w" (i)
        : [ptr]    "e" (ptr),
        [hi]     "r" (hi),
        [lo]     "r" (lo));
}

void send_pixels_16mhz_400(uint8_t* pixels, size_t sizePixels, volatile uint8_t* port, uint8_t pinMask)
{
    volatile size_t i = sizePixels; // Loop counter
    volatile uint8_t* ptr = pixels; // Pointer to next byte
    volatile uint8_t b = *ptr++;    // Current byte value
    volatile uint8_t hi;            // PORT w/output bit set high
    volatile uint8_t lo;            // PORT w/output bit set low

    // The 400 KHz clock on 16 MHz MCU is the most 'relaxed' version.

    // 40 inst. clocks per bit: HHHHHHHHxxxxxxxxxxxxLLLLLLLLLLLLLLLLLLLL
    // ST instructions:         ^       ^           ^         (T=0,8,20)

    volatile uint8_t next, bit;

    hi = *port | pinMask;
    lo = *port & ~pinMask;
    next = lo;
    bit = 8;

    asm volatile(
        "head40:"                  "\n\t" // Clk  Pseudocode    (T =  0)
        "st   %a[port], %[hi]"    "\n\t" // 2    PORT = hi     (T =  2)
        "sbrc %[byte] , 7"        "\n\t" // 1-2  if(b & 128)
        "mov  %[next] , %[hi]"   "\n\t" // 0-1   next = hi    (T =  4)
        "rjmp .+0"                "\n\t" // 2    nop nop       (T =  6)
        "rjmp .+0"                "\n\t" // 2    nop nop       (T =  8)
        "st   %a[port], %[next]"  "\n\t" // 2    PORT = next   (T = 10)
        "rjmp .+0"                "\n\t" // 2    nop nop       (T = 12)
        "rjmp .+0"                "\n\t" // 2    nop nop       (T = 14)
        "rjmp .+0"                "\n\t" // 2    nop nop       (T = 16)
        "rjmp .+0"                "\n\t" // 2    nop nop       (T = 18)
        "rjmp .+0"                "\n\t" // 2    nop nop       (T = 20)
        "st   %a[port], %[lo]"    "\n\t" // 2    PORT = lo     (T = 22)
        "nop"                     "\n\t" // 1    nop           (T = 23)
        "mov  %[next] , %[lo]"    "\n\t" // 1    next = lo     (T = 24)
        "dec  %[bit]"             "\n\t" // 1    bit--         (T = 25)
        "breq nextbyte40"         "\n\t" // 1-2  if(bit == 0)
        "rol  %[byte]"            "\n\t" // 1    b <<= 1       (T = 27)
        "nop"                     "\n\t" // 1    nop           (T = 28)
        "rjmp .+0"                "\n\t" // 2    nop nop       (T = 30)
        "rjmp .+0"                "\n\t" // 2    nop nop       (T = 32)
        "rjmp .+0"                "\n\t" // 2    nop nop       (T = 34)
        "rjmp .+0"                "\n\t" // 2    nop nop       (T = 36)
        "rjmp .+0"                "\n\t" // 2    nop nop       (T = 38)
        "rjmp head40"             "\n\t" // 2    -> head40 (next bit out)
        "nextbyte40:"              "\n\t" //                    (T = 27)
        "ldi  %[bit]  , 8"        "\n\t" // 1    bit = 8       (T = 28)
        "ld   %[byte] , %a[ptr]+" "\n\t" // 2    b = *ptr++    (T = 30)
        "rjmp .+0"                "\n\t" // 2    nop nop       (T = 32)
        "st   %a[port], %[lo]"    "\n\t" // 2    PORT = lo     (T = 34)
        "rjmp .+0"                "\n\t" // 2    nop nop       (T = 36)
        "sbiw %[count], 1"        "\n\t" // 2    i--           (T = 38)
        "brne head40"             "\n"   // 1-2  if(i != 0) -> (next byte)
        : [port]  "+e" (port),
        [byte]  "+r" (b),
        [bit]   "+r" (bit),
        [next]  "+r" (next),
        [count] "+w" (i)
        : [ptr]    "e" (ptr),
        [hi]     "r" (hi),
        [lo]     "r" (lo));
}

#else
#error "CPU SPEED NOT SUPPORTED"
#endif

#endif