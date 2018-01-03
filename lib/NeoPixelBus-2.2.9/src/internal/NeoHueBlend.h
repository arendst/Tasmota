/*-------------------------------------------------------------------------
NeoHueBlend provides method objects that can be directly consumed by
blend template functions in HslColor and HsbColor

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

class NeoHueBlendBase
{
protected:
    static float FixWrap(float value)
    {
        if (value < 0.0f)
        {
            value += 1.0f;
        }
        else if (value > 1.0f)
        {
            value -= 1.0f;
        }
        return value;
    }
};

class NeoHueBlendShortestDistance : NeoHueBlendBase
{
public:
    static float HueBlend(float left, float right, float progress)
    {
        float delta = right - left;
        float base = left;
        if (delta > 0.5f)
        {
            base = right;
            delta = 1.0f - delta;
            progress = 1.0f - progress;
        }
        else if (delta < -0.5f)
        {
            delta = 1.0f + delta;
        }
        return FixWrap(base + (delta) * progress);
    };
};

class NeoHueBlendLongestDistance : NeoHueBlendBase
{
public:
    static float HueBlend(float left, float right, float progress)
    {
        float delta = right - left;
        float base = left;
        if (delta < 0.5f && delta >= 0.0f)
        {
            base = right;
            delta = 1.0f - delta;
            progress = 1.0f - progress;
        }
        else if (delta > -0.5f && delta < 0.0f)
        {
            delta = 1.0f + delta;
        }
        return FixWrap(base + delta * progress);
    };
};

class NeoHueBlendClockwiseDirection : NeoHueBlendBase
{
public:
    static float HueBlend(float left, float right, float progress)
    {
        float delta = right - left;
        float base = left;
        if (delta < 0.0f)
        {
            delta = 1.0f + delta;
        }

        return FixWrap(base + delta * progress);
    };
};

class NeoHueBlendCounterClockwiseDirection : NeoHueBlendBase
{
public:
    static float HueBlend(float left, float right, float progress)
    {
        float delta = right - left;
        float base = left;
        if (delta > 0.0f)
        {
            delta = delta - 1.0f;
        }

        return FixWrap(base + delta * progress);
    };
};
