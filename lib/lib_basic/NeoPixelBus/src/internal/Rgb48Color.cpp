/*-------------------------------------------------------------------------
Rgb48Color provides a color object that can be directly consumed by NeoPixelBus

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

#include "Rgb48Color.h"
#include "RgbColor.h"
#include "HslColor.h"
#include "HsbColor.h"
#include "HtmlColor.h"

Rgb48Color::Rgb48Color(const HtmlColor& color)
{
    uint32_t temp = color.Color;

    B = (temp & 0xff);
    temp = temp >> 8;
    G = (temp & 0xff);
    temp = temp >> 8;
    R = (temp & 0xff);
};

Rgb48Color::Rgb48Color(const HslColor& color)
{
    float r;
    float g;
    float b;

    _HslToRgb(color, &r, &g, &b);

    R = static_cast<uint16_t>(r * Max);
    G = static_cast<uint16_t>(g * Max);
    B = static_cast<uint16_t>(b * Max);
}

Rgb48Color::Rgb48Color(const HsbColor& color)
{
    float r;
    float g;
    float b;

    _HsbToRgb(color, &r, &g, &b);

    R = static_cast<uint16_t>(r * Max);
    G = static_cast<uint16_t>(g * Max);
    B = static_cast<uint16_t>(b * Max);
}

uint16_t Rgb48Color::CalculateBrightness() const
{
    return static_cast<uint16_t>((static_cast<uint32_t>(R) + static_cast<uint32_t>(G) + static_cast<uint32_t>(B)) / 3);
}

Rgb48Color Rgb48Color::Dim(uint16_t ratio) const
{
    // specifically avoids float math
    return Rgb48Color(_elementDim(R, ratio), _elementDim(G, ratio), _elementDim(B, ratio));
}

Rgb48Color Rgb48Color::Brighten(uint16_t ratio) const
{
    // specifically avoids float math
    return Rgb48Color(_elementBrighten(R, ratio), _elementBrighten(G, ratio), _elementBrighten(B, ratio));
}

void Rgb48Color::Darken(uint16_t delta)
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

void Rgb48Color::Lighten(uint16_t delta)
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

Rgb48Color Rgb48Color::LinearBlend(const Rgb48Color& left, const Rgb48Color& right, float progress)
{
    return Rgb48Color( left.R + ((right.R - left.R) * progress),
        left.G + ((right.G - left.G) * progress),
        left.B + ((right.B - left.B) * progress));
}

Rgb48Color Rgb48Color::BilinearBlend(const Rgb48Color& c00, 
    const Rgb48Color& c01, 
    const Rgb48Color& c10, 
    const Rgb48Color& c11, 
    float x, 
    float y)
{
    float v00 = (1.0f - x) * (1.0f - y);
    float v10 = x * (1.0f - y);
    float v01 = (1.0f - x) * y;
    float v11 = x * y;

    return Rgb48Color(
        c00.R * v00 + c10.R * v10 + c01.R * v01 + c11.R * v11,
        c00.G * v00 + c10.G * v10 + c01.G * v01 + c11.G * v11,
        c00.B * v00 + c10.B * v10 + c01.B * v01 + c11.B * v11);
}