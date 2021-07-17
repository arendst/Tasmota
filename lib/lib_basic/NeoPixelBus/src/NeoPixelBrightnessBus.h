/*-------------------------------------------------------------------------
NeoPixelBus library wrapper template class that provides overall brightness control

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

template<typename T_COLOR_FEATURE, typename T_METHOD> class NeoPixelBrightnessBus : 
    public NeoPixelBus<T_COLOR_FEATURE, T_METHOD>
{
private:

    void ScaleColor(uint16_t scale, typename T_COLOR_FEATURE::ColorObject* color)
    {
        uint8_t* ptr = (uint8_t*)color;
        uint8_t* ptrEnd = ptr + sizeof(typename T_COLOR_FEATURE::ColorObject);

        while (ptr != ptrEnd)
        {
            uint16_t value = *ptr;
            *ptr++ = (value * scale) >> 8;
        }
    }

    void ConvertColor(typename T_COLOR_FEATURE::ColorObject* color)
    {
        uint16_t scale = _brightness + 1;
        ScaleColor(scale, color);
    }

    void RecoverColor(typename T_COLOR_FEATURE::ColorObject* color) const
    {
        uint8_t* ptr = (uint8_t*)color;
        uint8_t* ptrEnd = ptr + sizeof(typename T_COLOR_FEATURE::ColorObject);
        uint16_t scale = _brightness + 1;

        while (ptr != ptrEnd)
        {
            uint16_t value = *ptr;
            *ptr++ = (value << 8) / scale;
        }
    }

public:
    NeoPixelBrightnessBus(uint16_t countPixels, uint8_t pin) :
        NeoPixelBus<T_COLOR_FEATURE, T_METHOD>(countPixels, pin),
        _brightness(255)
    {
    }

    NeoPixelBrightnessBus(uint16_t countPixels, uint8_t pinClock, uint8_t pinData) :
        NeoPixelBus<T_COLOR_FEATURE, T_METHOD>(countPixels, pinClock, pinData),
        _brightness(255)
    {
    }

    NeoPixelBrightnessBus(uint16_t countPixels) :
        NeoPixelBus<T_COLOR_FEATURE, T_METHOD>(countPixels),
        _brightness(255)
    {
    }

    void SetBrightness(uint8_t brightness)
    {
        // Only update if there is a change
        if (brightness != _brightness)
        { 
            uint16_t scale = ((static_cast<uint16_t>(brightness) + 1) << 8) / (static_cast<uint16_t>(_brightness) + 1);

            // scale existing pixels
            //
            for (uint16_t indexPixel = 0; indexPixel < NeoPixelBus<T_COLOR_FEATURE, T_METHOD>::PixelCount(); indexPixel++)
            {
                typename T_COLOR_FEATURE::ColorObject color = NeoPixelBus<T_COLOR_FEATURE, T_METHOD>::GetPixelColor(indexPixel);
                ScaleColor(scale, &color);
                NeoPixelBus<T_COLOR_FEATURE, T_METHOD>::SetPixelColor(indexPixel, color);
            }
 
            _brightness = brightness;
            this->Dirty();
        }
    }

    uint8_t GetBrightness() const
    {
        return _brightness;
    }

    void SetPixelColor(uint16_t indexPixel, typename T_COLOR_FEATURE::ColorObject color)
    {
        ConvertColor(&color);
        NeoPixelBus<T_COLOR_FEATURE, T_METHOD>::SetPixelColor(indexPixel, color);
    }

    typename T_COLOR_FEATURE::ColorObject GetPixelColor(uint16_t indexPixel) const
    {
        typename T_COLOR_FEATURE::ColorObject color = NeoPixelBus<T_COLOR_FEATURE, T_METHOD>::GetPixelColor(indexPixel);
        RecoverColor(&color);
        return color;
    }

    void ClearTo(typename T_COLOR_FEATURE::ColorObject color)
    {
        ConvertColor(&color);
        NeoPixelBus<T_COLOR_FEATURE, T_METHOD>::ClearTo(color);
    };

    void ClearTo(typename T_COLOR_FEATURE::ColorObject color, uint16_t first, uint16_t last)
    {
        ConvertColor(&color);
        NeoPixelBus<T_COLOR_FEATURE, T_METHOD>::ClearTo(color, first, last);
    }


protected:
    uint8_t _brightness;
};


