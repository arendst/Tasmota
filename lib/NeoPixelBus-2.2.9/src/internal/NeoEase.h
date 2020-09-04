/*-------------------------------------------------------------------------
NeoEase provides animation curve equations for animation support.

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

#ifdef ARDUINO_ARCH_AVR

typedef float(*AnimEaseFunction)(float unitValue);

#else

#undef max
#undef min
#include <functional>
typedef std::function<float(float unitValue)> AnimEaseFunction;

#endif

class NeoEase
{
public:
    static float Linear(float unitValue)
    {
        return unitValue;
    }

    static float QuadraticIn(float unitValue)
    {
        return unitValue * unitValue;
    }

    static float QuadraticOut(float unitValue)
    {
        return (-unitValue * (unitValue - 2.0f));
    }

    static float QuadraticInOut(float unitValue)
    {
        unitValue *= 2.0f;
        if (unitValue < 1.0f)
        {
            return (0.5f * unitValue * unitValue);
        }
        else
        {
            unitValue -= 1.0f;
            return (-0.5f * (unitValue * (unitValue - 2.0f) - 1.0f));
        }
    }

    static float CubicIn(float unitValue)
    {
        return (unitValue * unitValue * unitValue);
    }

    static float CubicOut(float unitValue)
    {
        unitValue -= 1.0f;
        return (unitValue * unitValue * unitValue + 1);
    }

    static float CubicInOut(float unitValue)
    {
        unitValue *= 2.0f;
        if (unitValue < 1.0f)
        {
            return (0.5f * unitValue * unitValue * unitValue);
        }
        else
        {
            unitValue -= 2.0f;
            return (0.5f * (unitValue * unitValue * unitValue + 2.0f));
        }
    }

    static float QuarticIn(float unitValue)
    {
        return (unitValue * unitValue * unitValue * unitValue);
    }

    static float QuarticOut(float unitValue)
    {
        unitValue -= 1.0f;
        return -(unitValue * unitValue * unitValue * unitValue - 1);
    }

    static float QuarticInOut(float unitValue)
    {
        unitValue *= 2.0f;
        if (unitValue < 1.0f)
        {
            return (0.5f * unitValue * unitValue * unitValue * unitValue);
        }
        else
        {
            unitValue -= 2.0f;
            return (-0.5f * (unitValue * unitValue * unitValue * unitValue - 2.0f));
        }
    }

    static float QuinticIn(float unitValue)
    {
        return (unitValue * unitValue * unitValue * unitValue * unitValue);
    }

    static float QuinticOut(float unitValue)
    {
        unitValue -= 1.0f;
        return (unitValue * unitValue * unitValue * unitValue * unitValue + 1.0f);
    }

    static float QuinticInOut(float unitValue)
    {
        unitValue *= 2.0f;
        if (unitValue < 1.0f)
        {
            return (0.5f * unitValue * unitValue * unitValue * unitValue * unitValue);
        }
        else
        {
            unitValue -= 2.0f;
            return (0.5f * (unitValue * unitValue * unitValue * unitValue * unitValue + 2.0f));
        }
    }

    static float SinusoidalIn(float unitValue)
    {
        return (-cos(unitValue * HALF_PI) + 1.0f);
    }

    static float SinusoidalOut(float unitValue)
    {
        return (sin(unitValue * HALF_PI));
    }

    static float SinusoidalInOut(float unitValue)
    {
        return -0.5 * (cos(PI * unitValue) - 1.0f);
    }

    static float ExponentialIn(float unitValue)
    {
        return (pow(2, 10.0f * (unitValue - 1.0f)));
    }

    static float ExponentialOut(float unitValue)
    {
        return (-pow(2, -10.0f * unitValue) + 1.0f);
    }

    static float ExponentialInOut(float unitValue)
    {
        unitValue *= 2.0f;
        if (unitValue < 1.0f)
        {
            return (0.5f * pow(2, 10.0f * (unitValue - 1.0f)));
        }
        else
        {
            unitValue -= 1.0f;
            return (0.5f * (-pow(2, -10.0f * unitValue) + 2.0f));
        }
    }

    static float CircularIn(float unitValue)
    {
        if (unitValue == 1.0f)
        {
            return 1.0f;
        }
        else
        {
            return (-(sqrt(1.0f - unitValue * unitValue) - 1.0f));
        }
    }

    static float CircularOut(float unitValue)
    {
        unitValue -= 1.0f;
        return (sqrt(1.0f - unitValue * unitValue));
    }

    static float CircularInOut(float unitValue)
    {
        unitValue *= 2.0f;
        if (unitValue < 1.0f)
        {
            return (-0.5f * (sqrt(1.0f - unitValue * unitValue) - 1));
        }
        else
        {
            unitValue -= 2.0f;
            return (0.5f * (sqrt(1.0f - unitValue * unitValue) + 1.0f));
        }
    }

    static float Gamma(float unitValue)
    {
        return pow(unitValue, 1.0f / 0.45f);
    }
};