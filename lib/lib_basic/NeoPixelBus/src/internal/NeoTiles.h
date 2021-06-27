#pragma once

/*-------------------------------------------------------------------------
NeoTiles provides a mapping feature of a 2d cordinate to linear 1d cordinate
It is used to map tiles of matricies of NeoPixels to a index on the NeoPixelBus
where the the matricies use one topology and the tiles of those matricies can
use another

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
// class NeoTiles
// Simple template Tile layout class
// T_MATRIX_LAYOUT = the layout used on the pixel matrix panel (a tile)
// T_TILE_LAYOUT = the layout used for the tiles.
//
//-----------------------------------------------------------------------------
template <typename T_MATRIX_LAYOUT, typename T_TILE_LAYOUT> class NeoTiles
{
public:
    NeoTiles(uint16_t topoWidth, uint16_t topoHeight,
        uint16_t tilesWidth, uint16_t tilesHeight) :
        _topo(topoWidth, topoHeight),
        _width(tilesWidth),
        _height(tilesHeight)
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
        else if (localIndex == (_topo.getWidth() * _topo.getHeight() - 1))
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
        return _width * _topo.getWidth();
    }

    uint16_t getHeight() const
    {
        return _height * _topo.getHeight();
    }

private:
    const NeoTopology<T_MATRIX_LAYOUT> _topo;
    const uint16_t _width;
    const uint16_t _height;

    void calculate(uint16_t x, uint16_t y, uint16_t* pLocalIndex, uint16_t* pTileOffset) const
    {
        uint16_t tileX = x / _topo.getWidth();
        uint16_t topoX = x % _topo.getWidth();

        uint16_t tileY = y / _topo.getHeight();
        uint16_t topoY = y % _topo.getHeight();

        *pTileOffset = T_TILE_LAYOUT::Map(_width, _height, tileX, tileY) * _topo.getWidth() * _topo.getHeight();
        *pLocalIndex = _topo.Map(topoX, topoY);
    }
};

