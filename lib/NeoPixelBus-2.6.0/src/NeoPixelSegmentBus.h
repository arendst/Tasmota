/*-------------------------------------------------------------------------
NeoPixelBus library wrapper template class that provides enhanced methods
for writing to segment based strips

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

#include "NeoPixelBus.h"

template<typename T_COLOR_FEATURE, typename T_METHOD> class NeoPixelSegmentBus : 
    public NeoPixelBus<T_COLOR_FEATURE, T_METHOD>
{
public:
    NeoPixelSegmentBus(uint16_t countPixels, uint8_t pin) :
        NeoPixelBus<T_COLOR_FEATURE, T_METHOD>(countPixels, pin)
    {
    }

    NeoPixelSegmentBus(uint16_t countPixels) :
        NeoPixelBus<T_COLOR_FEATURE, T_METHOD>(countPixels)
    {
    }

    void SetString(uint16_t indexDigit, 
        const char* str, 
        uint8_t brightness, 
        uint8_t defaultBrightness = 0)
    {
        T_COLOR_FEATURE::ColorObject::SetString(*this,
            indexDigit,
            str,
            brightness,
            defaultBrightness);
    }

    void SetString(uint16_t indexDigit, 
        const String& str, 
        uint8_t brightness, 
        uint8_t defaultBrightness = 0)
    {
        SetString(indexDigit, str.c_str(), brightness, defaultBrightness);
    }
};


