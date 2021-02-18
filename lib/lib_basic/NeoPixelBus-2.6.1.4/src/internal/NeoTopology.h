#pragma once

/*-------------------------------------------------------------------------
NeoTopology provides a mapping feature of a 2d cordinate to linear 1d cordinate
It is used to map a matrix of NeoPixels to a index on the NeoPixelBus

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

enum NeoTopologyHint
{
    NeoTopologyHint_FirstOnPanel,
    NeoTopologyHint_InPanel,
    NeoTopologyHint_LastOnPanel,
    NeoTopologyHint_OutOfBounds
};

template <typename T_LAYOUT> class NeoTopology
{
public:
    NeoTopology(uint16_t width, uint16_t height) :
        _width(width),
        _height(height)
    {

    }

    uint16_t Map(int16_t x, int16_t y) const
    {   
        if (x >= _width)
        {
            x = _width - 1;
        }
        else if (x < 0)
        {
            x = 0;
        }
        if (y >= _height)
        {
            y = _height - 1;
        }
        else if (y < 0)
        {
            y = 0;
        }
        return T_LAYOUT::Map(_width, _height, x, y);
    }

    uint16_t MapProbe(int16_t x, int16_t y) const
    {
        if (x < 0 || x >= _width || y < 0 || y >= _height)
        {
            return _width  * _height; // count, out of bounds
        }
        return T_LAYOUT::Map(_width, _height, x, y);
    }

    uint16_t getWidth() const 
    {
        return _width;
    }

    uint16_t getHeight() const 
    {
        return _height;
    }

private:
    const uint16_t _width;
    const uint16_t _height;
};
