/*-------------------------------------------------------------------------
RgbColorBase provides a RGB color object common support

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

#include "RgbColorBase.h"
#include "RgbColor.h"
#include "Rgb48Color.h"
#include "HslColor.h"
#include "HsbColor.h"
#include "HtmlColor.h"

float RgbColorBase::_CalcColor(float p, float q, float t)
{
    if (t < 0.0f)
        t += 1.0f;
    if (t > 1.0f)
        t -= 1.0f;

    if (t < 1.0f / 6.0f)
        return p + (q - p) * 6.0f * t;

    if (t < 0.5f)
        return q;

    if (t < 2.0f / 3.0f)
        return p + ((q - p) * (2.0f / 3.0f - t) * 6.0f);

    return p;
}

void RgbColorBase::_HslToRgb(const HslColor& color, float* r, float* g, float* b)
{
    float h = color.H;
    float s = color.S;
    float l = color.L;


    if (color.S == 0.0f || color.L == 0.0f)
    {
        *r = *g = *b = l; // achromatic or black
    }
    else
    {
        float q = l < 0.5f ? l * (1.0f + s) : l + s - (l * s);
        float p = 2.0f * l - q;
        *r = _CalcColor(p, q, h + 1.0f / 3.0f);
        *g = _CalcColor(p, q, h);
        *b = _CalcColor(p, q, h - 1.0f / 3.0f);
    }
}

void RgbColorBase::_HsbToRgb(const HsbColor& color, float* r, float* g, float* b)
{
    float h = color.H;
    float s = color.S;
    float v = color.B;

    if (color.S == 0.0f)
    {
        *r = *g = *b = v; // achromatic or black
    }
    else
    {
        if (h < 0.0f)
        {
            h += 1.0f;
        }
        else if (h >= 1.0f)
        {
            h -= 1.0f;
        }
        h *= 6.0f;
        int i = (int)h;
        float f = h - i;
        float q = v * (1.0f - s * f);
        float p = v * (1.0f - s);
        float t = v * (1.0f - s * (1.0f - f));
        switch (i)
        {
        case 0:
            *r = v;
            *g = t;
            *b = p;
            break;
        case 1:
            *r = q;
            *g = v;
            *b = p;
            break;
        case 2:
            *r = p;
            *g = v;
            *b = t;
            break;
        case 3:
            *r = p;
            *g = q;
            *b = v;
            break;
        case 4:
            *r = t;
            *g = p;
            *b = v;
            break;
        default:
            *r = v;
            *g = p;
            *b = q;
            break;
        }
    }
}