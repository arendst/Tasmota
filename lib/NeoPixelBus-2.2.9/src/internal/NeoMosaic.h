#pragma once

/*-------------------------------------------------------------------------
Mosiac provides a mapping feature of a 2d cordinate to linear 1d cordinate
It is used to map tiles of matricies of NeoPixels to a index on the NeoPixelBus
where the the matricies use a set of prefered topology and the tiles of 
those matricies use the RowMajorAlternating layout

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


//-----------------------------------------------------------------------------
// class NeoMosaic
// Complex Tile layout class that reduces distance of the interconnects between 
// the tiles by using different rotations of the layout at specific locations
//
// T_LAYOUT = the layout used for matrix panel (rotation is ignored)
//
// NOTE:  The tiles in the mosaic are always laid out using RowMajorAlternating 
//
//-----------------------------------------------------------------------------

template <typename T_LAYOUT> class NeoMosaic
{
public:
    NeoMosaic(uint16_t topoWidth, uint16_t topoHeight,
        uint16_t mosaicWidth, uint16_t mosaicHeight) :
        _topoWidth(topoWidth),
        _topoHeight(topoHeight),
        _mosaicWidth(mosaicWidth),
        _mosaicHeight(mosaicHeight)
    {
    }

    uint16_t Map(int16_t x, int16_t y) const
    {
        uint16_t totalWidth = getWidth();
        uint16_t totalHeight = getHeight();

        if (x >= totalWidth)
        {
            x = totalWidth - 1;
        }
        else if (x < 0)
        {
            x = 0;
        }

        if (y >= totalHeight)
        {
            y = totalHeight - 1;
        }
        else if (y < 0)
        {
            y = 0;
        }

        uint16_t localIndex;
        uint16_t tileOffset;

        calculate(x, y, &localIndex, &tileOffset);

        return localIndex + tileOffset;
    }

    uint16_t MapProbe(int16_t x, int16_t y) const
    {
        uint16_t totalWidth = getWidth();
        uint16_t totalHeight = getHeight();

        if (x < 0 || x >= totalWidth || y < 0 || y >= totalHeight)
        {
            return totalWidth  * totalHeight; // count, out of bounds
        }

        uint16_t localIndex;
        uint16_t tileOffset;

        calculate(x, y, &localIndex, &tileOffset);

        return localIndex + tileOffset;
    }

    NeoTopologyHint TopologyHint(int16_t x, int16_t y) const
    {
        uint16_t totalWidth = getWidth();
        uint16_t totalHeight = getHeight();

        if (x < 0 || x >= totalWidth || y < 0 || y >= totalHeight)
        {
            return NeoTopologyHint_OutOfBounds;
        }

        uint16_t localIndex;
        uint16_t tileOffset;
        NeoTopologyHint result;

        calculate(x, y, &localIndex, &tileOffset);

        if (localIndex == 0)
        {
            result = NeoTopologyHint_FirstOnPanel;
        }
        else if (localIndex == (_topoWidth * _topoHeight - 1))
        {
            result = NeoTopologyHint_LastOnPanel;
        }
        else
        {
            result = NeoTopologyHint_InPanel;
        }
        
        return result;
    }

    uint16_t getWidth() const
    {
        return _topoWidth * _mosaicWidth;
    }

    uint16_t getHeight() const
    {
        return _topoHeight * _mosaicHeight;
    }

private:
    const uint16_t _topoWidth;
    const uint16_t _topoHeight;
    const uint16_t _mosaicWidth;
    const uint16_t _mosaicHeight;

    void calculate(uint16_t x, uint16_t y, uint16_t* pLocalIndex, uint16_t* pTileOffset) const
    {
        uint16_t tileX = x / _topoWidth;
        uint16_t topoX = x % _topoWidth;

        uint16_t tileY = y / _topoHeight;
        uint16_t topoY = y % _topoHeight;

        *pTileOffset = RowMajorAlternatingLayout::Map(_mosaicWidth,
            _mosaicHeight,
            tileX,
            tileY) * _topoWidth * _topoHeight;

        if (tileX & 0x0001)
        {
            // odd columns
            if (tileY & 0x0001)
            {
                *pLocalIndex = T_LAYOUT::OddRowOddColumnLayout::Map(_topoWidth, _topoHeight, topoX, topoY);
            }
            else
            {
                *pLocalIndex = T_LAYOUT::EvenRowOddColumnLayout::Map(_topoWidth, _topoHeight, topoX, topoY);
            }
        }
        else
        {
            // even columns
            if (tileY & 0x0001)
            {
                *pLocalIndex = T_LAYOUT::OddRowEvenColumnLayout::Map(_topoWidth, _topoHeight, topoX, topoY);
            }
            else
            {
                *pLocalIndex = T_LAYOUT::EvenRowEvenColumnLayout::Map(_topoWidth, _topoHeight, topoX, topoY);
            }
        }
    }
};