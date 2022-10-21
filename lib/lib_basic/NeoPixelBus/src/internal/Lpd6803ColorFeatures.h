/*-------------------------------------------------------------------------
Lpd6803ColorFeatures provides feature classes to describe color order and
color depth for NeoPixelBus template class when used with DotStar like chips

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

class Lpd68033ElementsNoSettings
{
public:
    typedef NeoNoSettings SettingsObject;
    static const size_t SettingsSize = 0;

    static void applySettings(uint8_t*, const SettingsObject&)
    {
    }

    static uint8_t* pixels(uint8_t* pData)
    {
        return pData;
    }

    static const uint8_t* pixels(const uint8_t* pData)
    {
        return pData;
    }
};

class Lpd68033Elements : public Lpd68033ElementsNoSettings
{
public:
    static const size_t PixelSize = 2;   // 1 bit + 555 encoded elements

    static uint8_t* getPixelAddress(uint8_t* pPixels, uint16_t indexPixel)
    {
        return pPixels + indexPixel * PixelSize;
    }
    static const uint8_t* getPixelAddress(const uint8_t* pPixels, uint16_t indexPixel)
    {
        return pPixels + indexPixel * PixelSize;
    }

    static void replicatePixel(uint8_t* pPixelDest, const uint8_t* pPixelSrc, uint16_t count)
    {
        uint8_t* pEnd = pPixelDest + (count * PixelSize);
        while (pPixelDest < pEnd)
        {
            *pPixelDest++ = pPixelSrc[0];
            *pPixelDest++ = pPixelSrc[1];
        }
    }

    static void movePixelsInc(uint8_t* pPixelDest, const uint8_t* pPixelSrc, uint16_t count)
    {
        uint8_t* pEnd = pPixelDest + (count * PixelSize);
        while (pPixelDest < pEnd)
        {
            *pPixelDest++ = *pPixelSrc++;
            *pPixelDest++ = *pPixelSrc++;
        }
    }

    static void movePixelsInc_P(uint8_t* pPixelDest, PGM_VOID_P pPixelSrc, uint16_t count)
    {
        uint8_t* pEnd = pPixelDest + (count * PixelSize);
        const uint8_t* pSrc = (const uint8_t*)pPixelSrc;
        while (pPixelDest < pEnd)
        {
            *pPixelDest++ = pgm_read_byte(pSrc++);
            *pPixelDest++ = pgm_read_byte(pSrc++);
        }
    }

    static void movePixelsDec(uint8_t* pPixelDest, const uint8_t* pPixelSrc, uint16_t count)
    {
        uint8_t* pDestBack = pPixelDest + (count * PixelSize);
        const uint8_t* pSrcBack = pPixelSrc + (count * PixelSize);
        while (pDestBack > pPixelDest)
        {
            *--pDestBack = *--pSrcBack;
            *--pDestBack = *--pSrcBack;
        }
    }

    typedef RgbColor ColorObject;

protected:
    static void encodePixel(uint8_t c1, uint8_t c2, uint8_t c3, uint16_t* color555)
    {
        *color555 = (0x8000 | 
                ((c1 & 0xf8) << 7) | 
                ((c2 & 0xf8) << 2) | 
                ((c3 & 0xf8) >> 3));
    }

    static void decodePixel(uint16_t color555, uint8_t* c1, uint8_t* c2, uint8_t* c3)
    {
        *c1 = (color555 >> 7) & 0xf8;
        *c2 = (color555 >> 2) & 0xf8;
        *c3 = (color555 << 3) & 0xf8;
    }
};

class  Lpd6803BrgFeature : public Lpd68033Elements
{
public:
    static void applyPixelColor(uint8_t* pPixels, uint16_t indexPixel, ColorObject color)
    {
        uint8_t* p = getPixelAddress(pPixels, indexPixel);
        uint16_t color555;

        encodePixel(color.B, color.R, color.G, &color555);
        *p++ = color555 >> 8;
        *p = color555 & 0xff;
    }

    static ColorObject retrievePixelColor(const uint8_t* pPixels, uint16_t indexPixel)
    {
        ColorObject color;
        const uint8_t* p = getPixelAddress(pPixels, indexPixel);

        uint16_t color555;

        color555 = ((*p++) << 8);
        color555 |= (*p);

        decodePixel(color555, &color.B, &color.R, &color.G);

        return color;
    }

    static ColorObject retrievePixelColor_P(PGM_VOID_P pPixels, uint16_t indexPixel)
    {
        ColorObject color;
        const uint8_t* p = getPixelAddress((const uint8_t*)pPixels, indexPixel);

        uint16_t color555;

        color555 = (pgm_read_byte(p++) << 8);
        color555 |= pgm_read_byte(p);

        decodePixel(color555, &color.B, &color.R, &color.G);

        return color;
    }
};

class  Lpd6803GrbFeature : public Lpd68033Elements
{
public:
    static void applyPixelColor(uint8_t* pPixels, uint16_t indexPixel, ColorObject color)
    {
        uint8_t* p = getPixelAddress(pPixels, indexPixel);
        uint16_t color555;

        encodePixel(color.G, color.R, color.B, &color555);
        *p++ = color555 >> 8;
        *p = color555 & 0xff;
    }

    static ColorObject retrievePixelColor(const uint8_t* pPixels, uint16_t indexPixel)
    {
        ColorObject color;
        const uint8_t* p = getPixelAddress(pPixels, indexPixel);

        uint16_t color555;

        color555 = ((*p++) << 8);
        color555 |= (*p);

        decodePixel(color555, &color.G, &color.R, &color.B);

        return color;
    }

    static ColorObject retrievePixelColor_P(PGM_VOID_P pPixels, uint16_t indexPixel)
    {
        ColorObject color;
        const uint8_t* p = getPixelAddress((const uint8_t*)pPixels, indexPixel);

        uint16_t color555;

        color555 = (pgm_read_byte(p++) << 8);
        color555 |= pgm_read_byte(p);

        decodePixel(color555, &color.G, &color.R, &color.B);

        return color;
    }
};

class  Lpd6803GbrFeature : public Lpd68033Elements
{
public:
    static void applyPixelColor(uint8_t* pPixels, uint16_t indexPixel, ColorObject color)
    {
        uint8_t* p = getPixelAddress(pPixels, indexPixel);
        uint16_t color555;

        encodePixel(color.G, color.B, color.R, &color555);
        *p++ = color555 >> 8;
        *p = color555 & 0xff;
    }

    static ColorObject retrievePixelColor(const uint8_t* pPixels, uint16_t indexPixel)
    {
        ColorObject color;
        const uint8_t* p = getPixelAddress(pPixels, indexPixel);

        uint16_t color555;

        color555 = ((*p++) << 8);
        color555 |= (*p);

        decodePixel(color555, &color.G, &color.B, &color.R);

        return color;
    }

    static ColorObject retrievePixelColor_P(PGM_VOID_P pPixels, uint16_t indexPixel)
    {
        ColorObject color;
        const uint8_t* p = getPixelAddress((const uint8_t*)pPixels, indexPixel);

        uint16_t color555;

        color555 = (pgm_read_byte(p++) << 8);
        color555 |= pgm_read_byte(p);

        decodePixel(color555, &color.G, &color.B, &color.R);

        return color;
    }
};


class  Lpd6803RgbFeature : public Lpd68033Elements
{
public:
    static void applyPixelColor(uint8_t* pPixels, uint16_t indexPixel, ColorObject color)
    {
        uint8_t* p = getPixelAddress(pPixels, indexPixel);
        uint16_t color555;

        encodePixel(color.R, color.G, color.B, &color555);
        *p++ = color555 >> 8;
        *p = color555 & 0xff;
    }

    static ColorObject retrievePixelColor(const uint8_t* pPixels, uint16_t indexPixel)
    {
        ColorObject color;
        const uint8_t* p = getPixelAddress(pPixels, indexPixel);

        uint16_t color555;

        color555 = ((*p++) << 8);
        color555 |= (*p);

        decodePixel(color555, &color.R, &color.G, &color.B);

        return color;
    }

    static ColorObject retrievePixelColor_P(PGM_VOID_P pPixels, uint16_t indexPixel)
    {
        ColorObject color;
        const uint8_t* p = getPixelAddress((const uint8_t*)pPixels, indexPixel);

        uint16_t color555;

        color555 = (pgm_read_byte(p++) << 8);
        color555 |= pgm_read_byte(p);

        decodePixel(color555, &color.R, &color.G, &color.B);

        return color;
    }
};

