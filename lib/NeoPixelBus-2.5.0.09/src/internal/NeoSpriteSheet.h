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


template<typename T_BUFFER_METHOD> class NeoVerticalSpriteSheet
{
public:
    NeoVerticalSpriteSheet(uint16_t width,
        uint16_t height,
        uint16_t spriteHeight,
        PGM_VOID_P pixels) :
        _method(width, height, pixels),
        _spriteHeight(spriteHeight),
        _spriteCount(height / spriteHeight)
    {
    }

    operator NeoBufferContext<typename T_BUFFER_METHOD::ColorFeature>()
    {
        return _method;
    }

    uint16_t SpriteWidth() const
    {
        return _method.Width();
    };

    uint16_t SpriteHeight() const
    {
        return _spriteHeight;
    };

    uint16_t SpriteCount() const
    {
        return _spriteCount;
    }

    void SetPixelColor(uint16_t indexSprite,
        int16_t x,
        int16_t y,
        typename T_BUFFER_METHOD::ColorObject color)
    {
        _method.SetPixelColor(pixelIndex(indexSprite, x, y), color);
    };

    typename T_BUFFER_METHOD::ColorObject GetPixelColor(uint16_t indexSprite,
        int16_t x,
        int16_t y) const
    {
        return _method.GetPixelColor(pixelIndex(indexSprite, x, y));
    };

    void ClearTo(typename T_BUFFER_METHOD::ColorObject color)
    {
        _method.ClearTo(color);
    };

    void Blt(NeoBufferContext<typename T_BUFFER_METHOD::ColorFeature> destBuffer,
        uint16_t indexPixel,
        uint16_t indexSprite)
    {
        uint16_t destPixelCount = destBuffer.PixelCount();
        // validate indexPixel
        if (indexPixel >= destPixelCount)
        {
            return;
        }

        // validate indexSprite
        if (indexSprite >= _spriteCount)
        {
            return;
        }
        // calc how many we can copy
        uint16_t copyCount = destPixelCount - indexPixel;

        if (copyCount > SpriteWidth())
        {
            copyCount = SpriteWidth();
        }

        uint8_t* pDest = T_BUFFER_METHOD::ColorFeature::getPixelAddress(destBuffer.Pixels, indexPixel);
        const uint8_t* pSrc = T_BUFFER_METHOD::ColorFeature::getPixelAddress(_method.Pixels(), pixelIndex(indexSprite, 0, 0));
        _method.CopyPixels(pDest, pSrc, copyCount);
    }

    void Blt(NeoBufferContext<typename T_BUFFER_METHOD::ColorFeature> destBuffer,
        int16_t x,
        int16_t y,
        uint16_t indexSprite,
        LayoutMapCallback layoutMap)
    {
        if (indexSprite >= _spriteCount)
        {
            return;
        }
        uint16_t destPixelCount = destBuffer.PixelCount();

        for (int16_t srcY = 0; srcY < SpriteHeight(); srcY++)
        {
            for (int16_t srcX = 0; srcX < SpriteWidth(); srcX++)
            {
                uint16_t indexDest = layoutMap(srcX + x, srcY + y);
 
                if (indexDest < destPixelCount)
                {
                    const uint8_t* pSrc = T_BUFFER_METHOD::ColorFeature::getPixelAddress(_method.Pixels(), pixelIndex(indexSprite, srcX, srcY));
                    uint8_t* pDest = T_BUFFER_METHOD::ColorFeature::getPixelAddress(destBuffer.Pixels, indexDest);

                    _method.CopyPixels(pDest, pSrc, 1);
                }
            }
        }

    }

private:
    T_BUFFER_METHOD _method;

    const uint16_t _spriteHeight;
    const uint16_t _spriteCount;

    uint16_t pixelIndex(uint16_t indexSprite,
        int16_t x,
        int16_t y) const
    {
        uint16_t result = PixelIndex_OutOfBounds;

        if (indexSprite < _spriteCount &&
            x >= 0 &&
            (uint16_t)x < SpriteWidth() &&
            y >= 0 &&
            (uint16_t)y < SpriteHeight())
        {
            result = x + y * SpriteWidth() + indexSprite * _spriteHeight * SpriteWidth();
        }
        return result;
    }
};