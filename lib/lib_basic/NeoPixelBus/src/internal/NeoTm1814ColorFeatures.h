/*-------------------------------------------------------------------------
NeoTm1814ColorFeatures provides feature classes to describe color order and
color depth for NeoPixelBus template class specific to the TM1814 chip

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

class NeoTm1814Settings : public NeoRgbwCurrentSettings
{
public:
    NeoTm1814Settings(uint16_t red, uint16_t green, uint16_t blue, uint16_t white)  :
        NeoRgbwCurrentSettings(red, green, blue, white)
    {
    }

    const static uint16_t MinCurrent = 65;
    const static uint16_t MaxCurrent = 380;

    static uint16_t LimitCurrent(uint16_t value)
    {
        if (value < MinCurrent)
        {
            value = MinCurrent;
        }
        else if (value > MaxCurrent)
        {
            value = MaxCurrent;
        }
        return value;
    }
};

class Neo4ElementsTm1814Settings : public Neo4Elements
{
public:
    typedef NeoTm1814Settings SettingsObject;
    static const size_t SettingsSize = 8;

    static void applySettings(uint8_t* pData, const SettingsObject& settings)
    {
        uint8_t* pSet = pData;

        // C1
        *pSet++ = (SettingsObject::LimitCurrent(settings.WhiteCurrent) - SettingsObject::MinCurrent) / 5;
        *pSet++ = (SettingsObject::LimitCurrent(settings.RedTenthMilliAmpere) - SettingsObject::MinCurrent) / 5;
        *pSet++ = (SettingsObject::LimitCurrent(settings.GreenTenthMilliAmpere) - SettingsObject::MinCurrent) / 5;
        *pSet++ = (SettingsObject::LimitCurrent(settings.BlueTenthMilliAmpere) - SettingsObject::MinCurrent) / 5;
        
        uint8_t* pC1 = pData;

        // C2
        for (uint8_t elem = 0; elem < 4; elem++)
        {
            *pSet++ = ~(*pC1++);
        }
    }

    static uint8_t* pixels(uint8_t* pData)
    {
        return pData + SettingsSize;
    }

    static const uint8_t* pixels(const uint8_t* pData)
    {
        return pData + SettingsSize;
    }
};


class NeoWrgbTm1814Feature : public Neo4ElementsTm1814Settings
{
public:
    static void applyPixelColor(uint8_t* pPixels, uint16_t indexPixel, ColorObject color)
    {
        uint8_t* p = getPixelAddress(pPixels, indexPixel);

        *p++ = color.W;
        *p++ = color.R;
        *p++ = color.G;
        *p = color.B;
    }

    static ColorObject retrievePixelColor(const uint8_t* pPixels, uint16_t indexPixel)
    {
        ColorObject color;
        const uint8_t* p = getPixelAddress(pPixels, indexPixel);

        color.W = *p++;
        color.R = *p++;
        color.G = *p++;
        color.B = *p;

        return color;
    }
    
    static ColorObject retrievePixelColor_P(PGM_VOID_P pPixels, uint16_t indexPixel)
    {
        ColorObject color;
        const uint8_t* p = getPixelAddress((const uint8_t*)pPixels, indexPixel);

        color.W = pgm_read_byte(p++);
        color.R = pgm_read_byte(p++);
        color.G = pgm_read_byte(p++);
        color.B = pgm_read_byte(p);

        return color;
    }
    
};

