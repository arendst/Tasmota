#pragma once

/*-------------------------------------------------------------------------
NeoRingTopology provides a mapping feature of a 2d polar cordinate to a 
linear 1d cordinate.
It is used to map a series of concentric rings of NeoPixels to a index on 
the NeoPixelBus.

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

template <typename T_LAYOUT> class NeoRingTopology : protected T_LAYOUT
{
public:
    NeoRingTopology()
    {
    }

    uint16_t Map(uint8_t ring, uint16_t pixel) const
    {
        if (pixel >= getPixelCountAtRing(ring)) 
        {
            return 0; // invalid ring and/or pixel argument, always return a valid value, the first one
        }

        return _map(ring, pixel);
    }

    uint16_t MapProbe(uint8_t ring, uint16_t pixel) const
    {
        if (pixel >= getPixelCountAtRing(ring)) 
        {
            return getPixelCount(); // total count, out of bounds
        }

        return _map(ring, pixel);
    }

    uint16_t RingPixelShift(uint8_t ring, uint16_t pixel, int16_t shift)
    {
        int32_t ringPixel = pixel;
        ringPixel += shift;

        if (ringPixel < 0)
        {
            ringPixel = 0;
        }
        else 
        {
            uint16_t count = getPixelCountAtRing(ring);
            if (ringPixel >= count)
            {
                ringPixel = count - 1;
            }
        }
        return ringPixel;
    }

    uint16_t RingPixelRotate(uint8_t ring, uint16_t pixel, int16_t rotate)
    {
        int32_t ringPixel = pixel;
        ringPixel += rotate;
        return ringPixel % getPixelCountAtRing(ring);
    }

    uint8_t getCountOfRings() const 
    {
        return _ringCount() - 1; // minus one as the Rings includes the extra value
    }

    uint16_t getPixelCountAtRing(uint8_t ring) const 
    {
        if (ring >= getCountOfRings())
        {
            return 0; // invalid, no pixels
        }

        return T_LAYOUT::Rings[ring + 1] - T_LAYOUT::Rings[ring]; // using the extra value for count calc
    }

    uint16_t getPixelCount() const
    {
        return T_LAYOUT::Rings[_ringCount() - 1]; // the last entry is the total count
    }

private:
    uint16_t _map(uint8_t ring, uint16_t pixel)  const
    {
        return T_LAYOUT::Rings[ring] + pixel;
    }

    uint8_t _ringCount() const
    {
        return sizeof(T_LAYOUT::Rings) / sizeof(T_LAYOUT::Rings[0]);
    }
};
