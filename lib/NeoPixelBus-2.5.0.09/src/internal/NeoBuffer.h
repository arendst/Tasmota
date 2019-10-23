/*-------------------------------------------------------------------------
NeoPixel library

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

template<typename T_BUFFER_METHOD> class NeoBuffer
{
public:
    NeoBuffer(uint16_t width,
        uint16_t height,
        PGM_VOID_P pixels) :
        _method(width, height, pixels)
    {
    }

    operator NeoBufferContext<typename T_BUFFER_METHOD::ColorFeature>()
    {
        return _method;
    }

    uint16_t PixelCount() const
    {
        return _method.PixelCount();
    };

    uint16_t Width() const
    {
        return _method.Width();
    };

    uint16_t Height() const
    {
        return _method.Height();
    };

    void SetPixelColor(
        int16_t x,
        int16_t y,
        typename T_BUFFER_METHOD::ColorObject color)
    {
        _method.SetPixelColor(pixelIndex(x, y), color);
    };

    typename T_BUFFER_METHOD::ColorObject GetPixelColor(
        int16_t x,
        int16_t y) const
    {
        return _method.GetPixelColor(pixelIndex(x, y));
    };

    void ClearTo(typename T_BUFFER_METHOD::ColorObject color)
    {
        _method.ClearTo(color);
    };

    void Blt(NeoBufferContext<typename T_BUFFER_METHOD::ColorFeature> destBuffer,
        uint16_t indexPixel)
    {
        uint16_t destPixelCount = destBuffer.PixelCount();
        // validate indexPixel
        if (indexPixel >= destPixelCount)
        {
            return;
        }

        // calc how many we can copy
        uint16_t copyCount = destPixelCount - indexPixel;
        uint16_t srcPixelCount = PixelCount();
        if (copyCount > srcPixelCount)
        {
            copyCount = srcPixelCount;
        }

        uint8_t* pDest = T_BUFFER_METHOD::ColorFeature::getPixelAddress(destBuffer.Pixels, indexPixel);
        _method.CopyPixels(pDest, _method.Pixels(), copyCount);
    }

    void Blt(NeoBufferContext<typename T_BUFFER_METHOD::ColorFeature> destBuffer,
        int16_t xDest,
        int16_t yDest,
        int16_t xSrc,
        int16_t ySrc,
        int16_t wSrc,
        int16_t hSrc,
        LayoutMapCallback layoutMap)
    {
        uint16_t destPixelCount = destBuffer.PixelCount();

        for (int16_t y = 0; y < hSrc; y++)
        {
            for (int16_t x = 0; x < wSrc; x++)
            {
                uint16_t indexDest = layoutMap(xDest + x, yDest + y);
 
                if (indexDest < destPixelCount)
                {
                    const uint8_t* pSrc = T_BUFFER_METHOD::ColorFeature::getPixelAddress(_method.Pixels(), pixelIndex(xSrc + x, ySrc + y));
                    uint8_t* pDest = T_BUFFER_METHOD::ColorFeature::getPixelAddress(destBuffer.Pixels, indexDest);

                    _method.CopyPixels(pDest, pSrc, 1);
                }
            }
        }
    }

    void Blt(NeoBufferContext<typename T_BUFFER_METHOD::ColorFeature> destBuffer,
        int16_t xDest,
        int16_t yDest,
        LayoutMapCallback layoutMap)
    {
        Blt(destBuffer, xDest, yDest, 0, 0, Width(), Height(), layoutMap);
    }

    template <typename T_SHADER> void Render(NeoBufferContext<typename T_BUFFER_METHOD::ColorFeature> destBuffer, T_SHADER& shader)
    {
        uint16_t countPixels = destBuffer.PixelCount();

        if (countPixels > _method.PixelCount())
        {
            countPixels = _method.PixelCount();
        }

        for (uint16_t indexPixel = 0; indexPixel < countPixels; indexPixel++)
        {
            typename T_BUFFER_METHOD::ColorObject color;
            
            shader.Apply(indexPixel, (uint8_t*)(&color), _method.Pixels() + (indexPixel * _method.PixelSize()));

            T_BUFFER_METHOD::ColorFeature::applyPixelColor(destBuffer.Pixels, indexPixel, color);
        }
    }

private:
    T_BUFFER_METHOD _method;

    uint16_t pixelIndex(
        int16_t x,
        int16_t y) const
    {
        uint16_t result = PixelIndex_OutOfBounds;

        if (x >= 0 &&
            (uint16_t)x < Width() &&
            y >= 0 &&
            (uint16_t)y < Height())
        {
            result = x + y * Width();
        }
        return result;
    }
};