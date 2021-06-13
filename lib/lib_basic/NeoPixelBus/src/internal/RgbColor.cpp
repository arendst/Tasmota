/*-------------------------------------------------------------------------
RgbColor provides a color object that can be directly consumed by NeoPixelBus

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

#include "RgbColor.h"
#include "Rgb16Color.h"
#include "Rgb48Color.h"
#include "HslColor.h"
#include "HsbColor.h"
#include "HtmlColor.h"
#include "RgbwColor.h"

RgbColor::RgbColor(const RgbwColor& color) :
    R(color.R),
    G(color.G),
    B(color.B)
{
};

RgbColor::RgbColor(const Rgb16Color& color)
{
    R = color.getR();
    G = color.getG();
    B = color.getB();
}

RgbColor::RgbColor(const HtmlColor& color)
{
    uint32_t temp = color.Color;

    B = (temp & 0xff);
    temp = temp >> 8;
    G = (temp & 0xff);
    temp = temp >> 8;
    R = (temp & 0xff);
};

RgbColor::RgbColor(const HslColor& color)
{
    float r;
    float g;
    float b;

    _HslToRgb(color, &r, &g, &b);

    R = static_cast<uint8_t>(r * Max);
    G = static_cast<uint8_t>(g * Max);
    B = static_cast<uint8_t>(b * Max);
}

RgbColor::RgbColor(const HsbColor& color)
{
    float r;
    float g;
    float b;

    _HsbToRgb(color, &r, &g, &b);

    R = static_cast<uint8_t>(r * Max);
    G = static_cast<uint8_t>(g * Max);
    B = static_cast<uint8_t>(b * Max);
}

uint8_t RgbColor::CalculateBrightness() const
{
    return static_cast<uint8_t>((static_cast<uint16_t>(R) + static_cast<uint16_t>(G) + static_cast<uint16_t>(B)) / 3);
}

RgbColor RgbColor::Dim(uint8_t ratio) const
{
    // specifically avoids float math
    return RgbColor(_elementDim(R, ratio), _elementDim(G, ratio), _elementDim(B, ratio));
}

RgbColor RgbColor::Brighten(uint8_t ratio) const
{
    // specifically avoids float math
    return RgbColor(_elementBrighten(R, ratio), _elementBrighten(G, ratio), _elementBrighten(B, ratio));
}

void RgbColor::Darken(uint8_t delta)
{
    if (R > delta)
    {
        R -= delta;
    }
    else
    {
        R = 0;
    }

    if (G > delta)
    {
        G -= delta;
    }
    else
    {
        G = 0;
    }

    if (B > delta)
    {
        B -= delta;
    }
    else
    {
        B = 0;
    }
}

void RgbColor::Lighten(uint8_t delta)
{
    if (R < Max - delta)
    {
        R += delta;
    }
    else
    {
        R = Max;
    }

    if (G < Max - delta)
    {
        G += delta;
    }
    else
    {
        G = Max;
    }

    if (B < Max - delta)
    {
        B += delta;
    }
    else
    {
        B = Max;
    }
}

RgbColor RgbColor::LinearBlend(const RgbColor& left, const RgbColor& right, float progress)
{
    return RgbColor( left.R + ((right.R - left.R) * progress),
        left.G + ((right.G - left.G) * progress),
        left.B + ((right.B - left.B) * progress));
}

RgbColor RgbColor::BilinearBlend(const RgbColor& c00, 
    const RgbColor& c01, 
    const RgbColor& c10, 
    const RgbColor& c11, 
    float x, 
    float y)
{
    float v00 = (1.0f - x) * (1.0f - y);
    float v10 = x * (1.0f - y);
    float v01 = (1.0f - x) * y;
    float v11 = x * y;

    return RgbColor(
        c00.R * v00 + c10.R * v10 + c01.R * v01 + c11.R * v11,
        c00.G * v00 + c10.G * v10 + c01.G * v01 + c11.G * v11,
        c00.B * v00 + c10.B * v10 + c01.B * v01 + c11.B * v11);
}