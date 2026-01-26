/*-------------------------------------------------------------------------
NeoPixel library helper functions for Esp32.

A BIG thanks to Andreas Merkle for the investigation and implementation of
a workaround to the GCC bug that drops method attributes from template methods

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

#include <Arduino.h>
#include "NeoSettings.h"
#include "NeoBusChannel.h"
#include "NeoEsp32RmtMethod.h"

#if defined(ARDUINO_ARCH_ESP32) && !defined(CONFIG_IDF_TARGET_ESP32C6) && !defined(CONFIG_IDF_TARGET_ESP32C2) && !defined(CONFIG_IDF_TARGET_ESP32C5)


// translate NeoPixelBuffer into RMT buffer
// this is done on the fly so we don't require a send buffer in raw RMT format
// which would be 32x larger than the primary buffer
//
// NOTE:  This was moved from the template below to here to workaround a GCC bug
//  That bug is that the IRAM_ATTR attribute (any attributes) is lost on template classes.  
//
//  Further, it was removed from the header to support current Esp32 release
//  which will need to be removed when the latest GitHub branchis released
//  due to this method will not get inlined this way
//
void NeoEsp32RmtSpeed::_translate(const void* src,
    rmt_item32_t* dest,
    size_t src_size,
    size_t wanted_num,
    size_t* translated_size,
    size_t* item_num,
    const uint32_t rmtBit0,
    const uint32_t rmtBit1,
    const uint16_t rmtDurationReset)
{
    if (src == NULL || dest == NULL)
    {
        *translated_size = 0;
        *item_num = 0;
        return;
    }

    size_t size = 0;
    size_t num = 0;
    const uint8_t* psrc = static_cast<const uint8_t*>(src);
    rmt_item32_t* pdest = dest;

    for (;;)
    {
        uint8_t data = *psrc;

        for (uint8_t bit = 0; bit < 8; bit++)
        {
            pdest->val = (data & 0x80) ? rmtBit1 : rmtBit0;
            pdest++;
            data <<= 1;
        }
        num += 8;
        size++;

        // if this is the last byte we need to adjust the length of the last pulse
        if (size >= src_size)
        {
            // extend the last bits LOW value to include the full reset signal length
            pdest--;
            pdest->duration1 = rmtDurationReset;
            // and stop updating data to send
            break;
        }

        if (num >= wanted_num)
        {
            // stop updating data to send
            break;
        }

        psrc++;
    }

    *translated_size = size;
    *item_num = num;
}


// these are required due to the linker error with ISRs
// dangerous relocation: l32r: literal placed after use
// https://stackoverflow.com/questions/19532826/what-does-a-dangerous-relocation-error-mean
//
void NeoEsp32RmtSpeedWs2811::Translate(const void* src,
    rmt_item32_t* dest,
    size_t src_size,
    size_t wanted_num,
    size_t* translated_size,
    size_t* item_num)
{
    _translate(src, dest, src_size, wanted_num, translated_size, item_num,
        RmtBit0, RmtBit1, RmtDurationReset);
}

void NeoEsp32RmtSpeedWs2812x::Translate(const void* src,
    rmt_item32_t* dest,
    size_t src_size,
    size_t wanted_num,
    size_t* translated_size,
    size_t* item_num)
{
    _translate(src, dest, src_size, wanted_num, translated_size, item_num,
        RmtBit0, RmtBit1, RmtDurationReset);
}

void NeoEsp32RmtSpeedSk6812::Translate(const void* src,
    rmt_item32_t* dest,
    size_t src_size,
    size_t wanted_num,
    size_t* translated_size,
    size_t* item_num)
{
    _translate(src, dest, src_size, wanted_num, translated_size, item_num,
        RmtBit0, RmtBit1, RmtDurationReset);
}

void NeoEsp32RmtSpeedTm1814::Translate(const void* src,
    rmt_item32_t* dest,
    size_t src_size,
    size_t wanted_num,
    size_t* translated_size,
    size_t* item_num)
{
    _translate(src, dest, src_size, wanted_num, translated_size, item_num,
        RmtBit0, RmtBit1, RmtDurationReset);
}

void NeoEsp32RmtSpeedTm1829::Translate(const void* src,
    rmt_item32_t* dest,
    size_t src_size,
    size_t wanted_num,
    size_t* translated_size,
    size_t* item_num)
{
    _translate(src, dest, src_size, wanted_num, translated_size, item_num,
        RmtBit0, RmtBit1, RmtDurationReset);
}

void NeoEsp32RmtSpeedTm1914::Translate(const void* src,
    rmt_item32_t* dest,
    size_t src_size,
    size_t wanted_num,
    size_t* translated_size,
    size_t* item_num)
{
    _translate(src, dest, src_size, wanted_num, translated_size, item_num,
        RmtBit0, RmtBit1, RmtDurationReset);
}

void NeoEsp32RmtSpeed800Kbps::Translate(const void* src,
    rmt_item32_t* dest,
    size_t src_size,
    size_t wanted_num,
    size_t* translated_size,
    size_t* item_num)
{
    _translate(src, dest, src_size, wanted_num, translated_size, item_num,
        RmtBit0, RmtBit1, RmtDurationReset);
}

void NeoEsp32RmtSpeed400Kbps::Translate(const void* src,
    rmt_item32_t* dest,
    size_t src_size,
    size_t wanted_num,
    size_t* translated_size,
    size_t* item_num)
{
    _translate(src, dest, src_size, wanted_num, translated_size, item_num,
        RmtBit0, RmtBit1, RmtDurationReset);
}

void NeoEsp32RmtSpeedApa106::Translate(const void* src,
    rmt_item32_t* dest,
    size_t src_size,
    size_t wanted_num,
    size_t* translated_size,
    size_t* item_num)
{
    _translate(src, dest, src_size, wanted_num, translated_size, item_num,
        RmtBit0, RmtBit1, RmtDurationReset);
}

void NeoEsp32RmtSpeedTx1812::Translate(const void* src,
    rmt_item32_t* dest,
    size_t src_size,
    size_t wanted_num,
    size_t* translated_size,
    size_t* item_num)
{
    _translate(src, dest, src_size, wanted_num, translated_size, item_num,
        RmtBit0, RmtBit1, RmtDurationReset);
}

void NeoEsp32RmtInvertedSpeedWs2811::Translate(const void* src,
    rmt_item32_t* dest,
    size_t src_size,
    size_t wanted_num,
    size_t* translated_size,
    size_t* item_num)
{
    _translate(src, dest, src_size, wanted_num, translated_size, item_num,
        RmtBit0, RmtBit1, RmtDurationReset);
}

void NeoEsp32RmtInvertedSpeedWs2812x::Translate(const void* src,
    rmt_item32_t* dest,
    size_t src_size,
    size_t wanted_num,
    size_t* translated_size,
    size_t* item_num)
{
    _translate(src, dest, src_size, wanted_num, translated_size, item_num,
        RmtBit0, RmtBit1, RmtDurationReset);
}

void NeoEsp32RmtInvertedSpeedSk6812::Translate(const void* src,
    rmt_item32_t* dest,
    size_t src_size,
    size_t wanted_num,
    size_t* translated_size,
    size_t* item_num)
{
    _translate(src, dest, src_size, wanted_num, translated_size, item_num,
        RmtBit0, RmtBit1, RmtDurationReset);
}

void NeoEsp32RmtInvertedSpeedTm1814::Translate(const void* src,
    rmt_item32_t* dest,
    size_t src_size,
    size_t wanted_num,
    size_t* translated_size,
    size_t* item_num)
{
    _translate(src, dest, src_size, wanted_num, translated_size, item_num,
        RmtBit0, RmtBit1, RmtDurationReset);
}

void NeoEsp32RmtInvertedSpeedTm1829::Translate(const void* src,
    rmt_item32_t* dest,
    size_t src_size,
    size_t wanted_num,
    size_t* translated_size,
    size_t* item_num)
{
    _translate(src, dest, src_size, wanted_num, translated_size, item_num,
        RmtBit0, RmtBit1, RmtDurationReset);
}

void NeoEsp32RmtInvertedSpeedTm1914::Translate(const void* src,
    rmt_item32_t* dest,
    size_t src_size,
    size_t wanted_num,
    size_t* translated_size,
    size_t* item_num)
{
    _translate(src, dest, src_size, wanted_num, translated_size, item_num,
        RmtBit0, RmtBit1, RmtDurationReset);
}

void NeoEsp32RmtInvertedSpeed800Kbps::Translate(const void* src,
    rmt_item32_t* dest,
    size_t src_size,
    size_t wanted_num,
    size_t* translated_size,
    size_t* item_num)
{
    _translate(src, dest, src_size, wanted_num, translated_size, item_num,
        RmtBit0, RmtBit1, RmtDurationReset);
}

void NeoEsp32RmtInvertedSpeed400Kbps::Translate(const void* src,
    rmt_item32_t* dest,
    size_t src_size,
    size_t wanted_num,
    size_t* translated_size,
    size_t* item_num)
{
    _translate(src, dest, src_size, wanted_num, translated_size, item_num,
        RmtBit0, RmtBit1, RmtDurationReset);
}

void NeoEsp32RmtInvertedSpeedApa106::Translate(const void* src,
    rmt_item32_t* dest,
    size_t src_size,
    size_t wanted_num,
    size_t* translated_size,
    size_t* item_num)
{
    _translate(src, dest, src_size, wanted_num, translated_size, item_num,
        RmtBit0, RmtBit1, RmtDurationReset);
}

void NeoEsp32RmtInvertedSpeedTx1812::Translate(const void* src,
    rmt_item32_t* dest,
    size_t src_size,
    size_t wanted_num,
    size_t* translated_size,
    size_t* item_num)
{
    _translate(src, dest, src_size, wanted_num, translated_size, item_num,
        RmtBit0, RmtBit1, RmtDurationReset);
}
#endif