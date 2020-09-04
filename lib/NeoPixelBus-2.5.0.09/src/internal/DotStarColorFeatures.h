/*-------------------------------------------------------------------------
DotStarColorFeatures provides feature classes to describe color order and
color depth for NeoPixelBus template class when used with DotStars

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

class DotStar3Elements
{
public:
    static const size_t PixelSize = 4; // still requires 4 to be sent

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
            *pPixelDest++ = pPixelSrc[2];
            *pPixelDest++ = pPixelSrc[3];
        }
    }

    static void movePixelsInc(uint8_t* pPixelDest, const uint8_t* pPixelSrc, uint16_t count)
    {
        uint8_t* pEnd = pPixelDest + (count * PixelSize);
        while (pPixelDest < pEnd)
        {
            *pPixelDest++ = *pPixelSrc++;
            *pPixelDest++ = *pPixelSrc++;
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
            *--pDestBack = *--pSrcBack;
            *--pDestBack = *--pSrcBack;
        }
    }

    typedef RgbColor ColorObject;
};

class DotStar4Elements
{
public:
    static const size_t PixelSize = 4;

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
            *pPixelDest++ = pPixelSrc[2];
            *pPixelDest++ = pPixelSrc[3];
        }
    }

    static void movePixelsInc(uint8_t* pPixelDest, const uint8_t* pPixelSrc, uint16_t count)
    {
        uint8_t* pEnd = pPixelDest + (count * PixelSize);
        while (pPixelDest < pEnd)
        {
            *pPixelDest++ = *pPixelSrc++;
            *pPixelDest++ = *pPixelSrc++;
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
            *--pDestBack = *--pSrcBack;
            *--pDestBack = *--pSrcBack;
        }
    }

    typedef RgbwColor ColorObject;
};

class DotStarBgrFeature : public DotStar3Elements
{
public:
    static void applyPixelColor(uint8_t* pPixels, uint16_t indexPixel, ColorObject color)
    {
        uint8_t* p = getPixelAddress(pPixels, indexPixel);

        *p++ = 0xff; // upper three bits are always 111 and brightness at max
        *p++ = color.B;
        *p++ = color.G;
        *p = color.R;
    }

    static ColorObject retrievePixelColor(uint8_t* pPixels, uint16_t indexPixel)
    {
        ColorObject color;
        uint8_t* p = getPixelAddress(pPixels, indexPixel);

        p++; // ignore the first byte
        color.B = *p++;
        color.G = *p++;
        color.R = *p;

        return color;
    }

    static ColorObject retrievePixelColor_P(PGM_VOID_P pPixels, uint16_t indexPixel)
    {
        ColorObject color;
        const uint8_t* p = getPixelAddress((const uint8_t*)pPixels, indexPixel);

        pgm_read_byte(p++); // ignore the first byte
        color.B = pgm_read_byte(p++);
        color.G = pgm_read_byte(p++);
        color.R = pgm_read_byte(p);

        return color;
    }

};

class DotStarLbgrFeature : public DotStar4Elements
{
public:
    static void applyPixelColor(uint8_t* pPixels, uint16_t indexPixel, ColorObject color)
    {
        uint8_t* p = getPixelAddress(pPixels, indexPixel);

        *p++ = 0xE0 | (color.W < 31 ? color.W : 31); // upper three bits are always 111
        *p++ = color.B;
        *p++ = color.G;
        *p = color.R;
    }

    static ColorObject retrievePixelColor(uint8_t* pPixels, uint16_t indexPixel)
    {
        ColorObject color;
        uint8_t* p = getPixelAddress(pPixels, indexPixel);

        color.W = (*p++) & 0x1F; // mask out upper three bits
        color.B = *p++;
        color.G = *p++;
        color.R = *p;

        return color;
    }
    
    static ColorObject retrievePixelColor_P(PGM_VOID_P pPixels, uint16_t indexPixel)
    {
        ColorObject color;
        const uint8_t* p = getPixelAddress((const uint8_t*)pPixels, indexPixel);

        color.W = pgm_read_byte(p++) & 0x1F; // mask out upper three bits
        color.B = pgm_read_byte(p++);
        color.G = pgm_read_byte(p++);
        color.R = pgm_read_byte(p);

        return color;
    }
    
};

class DotStarGrbFeature : public DotStar3Elements
{
public:
    static void applyPixelColor(uint8_t* pPixels, uint16_t indexPixel, ColorObject color)
    {
        uint8_t* p = getPixelAddress(pPixels, indexPixel);

        *p++ = 0xff; // upper three bits are always 111 and brightness at max
        *p++ = color.G;
        *p++ = color.R;
        *p = color.B;
    }

    static ColorObject retrievePixelColor(uint8_t* pPixels, uint16_t indexPixel)
    {
        ColorObject color;
        uint8_t* p = getPixelAddress(pPixels, indexPixel);

        p++; // ignore the first byte
        color.G = *p++;
        color.R = *p++;
        color.B = *p;

        return color;
    }

    static ColorObject retrievePixelColor_P(PGM_VOID_P pPixels, uint16_t indexPixel)
    {
        ColorObject color;
        const uint8_t* p = getPixelAddress((const uint8_t*)pPixels, indexPixel);

        pgm_read_byte(p++); // ignore the first byte
        color.G = pgm_read_byte(p++);
        color.R = pgm_read_byte(p++);
        color.B = pgm_read_byte(p);

        return color;
    }

};

class DotStarLgrbFeature : public DotStar4Elements
{
public:
    static void applyPixelColor(uint8_t* pPixels, uint16_t indexPixel, ColorObject color)
    {
        uint8_t* p = getPixelAddress(pPixels, indexPixel);

        *p++ = 0xE0 | (color.W < 31 ? color.W : 31); // upper three bits are always 111
        *p++ = color.G;
        *p++ = color.R;
        *p = color.B;
    }

    static ColorObject retrievePixelColor(uint8_t* pPixels, uint16_t indexPixel)
    {
        ColorObject color;
        uint8_t* p = getPixelAddress(pPixels, indexPixel);

        color.W = (*p++) & 0x1F; // mask out upper three bits
        color.G = *p++;
        color.R = *p++;
        color.B = *p;

        return color;
    }

    static ColorObject retrievePixelColor_P(PGM_VOID_P pPixels, uint16_t indexPixel)
    {
        ColorObject color;
        const uint8_t* p = getPixelAddress((const uint8_t*)pPixels, indexPixel);

        color.W = pgm_read_byte(p++) & 0x1F; // mask out upper three bits
        color.G = pgm_read_byte(p++);
        color.R = pgm_read_byte(p++);
        color.B = pgm_read_byte(p);

        return color;
    }

};

/* RGB Feature -- Some APA102s ship in RGB order */
class DotStarRgbFeature : public DotStar3Elements
{
public:
    static void applyPixelColor(uint8_t* pPixels, uint16_t indexPixel, ColorObject color)
    {
        uint8_t* p = getPixelAddress(pPixels, indexPixel);

        *p++ = 0xff; // upper three bits are always 111 and brightness at max
        *p++ = color.R;
        *p++ = color.G;
        *p = color.B;
    }

    static ColorObject retrievePixelColor(uint8_t* pPixels, uint16_t indexPixel)
    {
        ColorObject color;
        uint8_t* p = getPixelAddress(pPixels, indexPixel);

        p++; // ignore the first byte
        color.R = *p++;
        color.G = *p++;
        color.B = *p;

        return color;
    }

    static ColorObject retrievePixelColor_P(PGM_VOID_P pPixels, uint16_t indexPixel)
    {
        ColorObject color;
        const uint8_t* p = getPixelAddress((const uint8_t*)pPixels, indexPixel);

        pgm_read_byte(p++); // ignore the first byte
        color.R = pgm_read_byte(p++);
        color.G = pgm_read_byte(p++);
        color.B = pgm_read_byte(p);

        return color;
    }

};

class DotStarLrgbFeature : public DotStar4Elements
{
public:
    static void applyPixelColor(uint8_t* pPixels, uint16_t indexPixel, ColorObject color)
    {
        uint8_t* p = getPixelAddress(pPixels, indexPixel);

        *p++ = 0xE0 | (color.W < 31 ? color.W : 31); // upper three bits are always 111
        *p++ = color.R;
        *p++ = color.G;
        *p = color.B;
    }

    static ColorObject retrievePixelColor(uint8_t* pPixels, uint16_t indexPixel)
    {
        ColorObject color;
        uint8_t* p = getPixelAddress(pPixels, indexPixel);

        color.W = (*p++) & 0x1F; // mask out upper three bits
        color.R = *p++;
        color.G = *p++;
        color.B = *p;

        return color;
    }

    static ColorObject retrievePixelColor_P(PGM_VOID_P pPixels, uint16_t indexPixel)
    {
        ColorObject color;
        const uint8_t* p = getPixelAddress((const uint8_t*)pPixels, indexPixel);

        color.W = pgm_read_byte(p++) & 0x1F; // mask out upper three bits
        color.R = pgm_read_byte(p++);
        color.G = pgm_read_byte(p++);
        color.B = pgm_read_byte(p);

        return color;
    }

};
/* RBG Feature -- Some APA102s ship in RBG order */
class DotStarRbgFeature : public DotStar3Elements
{
public:
    static void applyPixelColor(uint8_t* pPixels, uint16_t indexPixel, ColorObject color)
    {
        uint8_t* p = getPixelAddress(pPixels, indexPixel);

        *p++ = 0xff; // upper three bits are always 111 and brightness at max
        *p++ = color.R;
        *p++ = color.B;
        *p = color.G;
    }

    static ColorObject retrievePixelColor(uint8_t* pPixels, uint16_t indexPixel)
    {
        ColorObject color;
        uint8_t* p = getPixelAddress(pPixels, indexPixel);

        p++; // ignore the first byte
        color.R = *p++;
        color.B = *p++;
        color.G = *p;

        return color;
    }

    static ColorObject retrievePixelColor_P(PGM_VOID_P pPixels, uint16_t indexPixel)
    {
        ColorObject color;
        const uint8_t* p = getPixelAddress((const uint8_t*)pPixels, indexPixel);

        pgm_read_byte(p++); // ignore the first byte
        color.R = pgm_read_byte(p++);
        color.B = pgm_read_byte(p++);
        color.G = pgm_read_byte(p);

        return color;
    }

};

class DotStarLrbgFeature : public DotStar4Elements
{
public:
    static void applyPixelColor(uint8_t* pPixels, uint16_t indexPixel, ColorObject color)
    {
        uint8_t* p = getPixelAddress(pPixels, indexPixel);

        *p++ = 0xE0 | (color.W < 31 ? color.W : 31); // upper three bits are always 111
        *p++ = color.R;
        *p++ = color.B;
        *p = color.G;
    }

    static ColorObject retrievePixelColor(uint8_t* pPixels, uint16_t indexPixel)
    {
        ColorObject color;
        uint8_t* p = getPixelAddress(pPixels, indexPixel);

        color.W = (*p++) & 0x1F; // mask out upper three bits
        color.R = *p++;
        color.B = *p++;
        color.G = *p;

        return color;
    }

    static ColorObject retrievePixelColor_P(PGM_VOID_P pPixels, uint16_t indexPixel)
    {
        ColorObject color;
        const uint8_t* p = getPixelAddress((const uint8_t*)pPixels, indexPixel);

        color.W = pgm_read_byte(p++) & 0x1F; // mask out upper three bits
        color.R = pgm_read_byte(p++);
        color.B = pgm_read_byte(p++);
        color.G = pgm_read_byte(p);

        return color;
    }

};

/* GBR Feature -- Some APA102s ship in GBR order */
class DotStarGbrFeature : public DotStar3Elements
{
public:
    static void applyPixelColor(uint8_t* pPixels, uint16_t indexPixel, ColorObject color)
    {
        uint8_t* p = getPixelAddress(pPixels, indexPixel);

        *p++ = 0xff; // upper three bits are always 111 and brightness at max
        *p++ = color.G;
        *p++ = color.B;
        *p = color.R;
    }

    static ColorObject retrievePixelColor(uint8_t* pPixels, uint16_t indexPixel)
    {
        ColorObject color;
        uint8_t* p = getPixelAddress(pPixels, indexPixel);

        p++; // ignore the first byte
        color.G = *p++;
        color.B = *p++;
        color.R = *p;

        return color;
    }

    static ColorObject retrievePixelColor_P(PGM_VOID_P pPixels, uint16_t indexPixel)
    {
        ColorObject color;
        const uint8_t* p = getPixelAddress((const uint8_t*)pPixels, indexPixel);

        pgm_read_byte(p++); // ignore the first byte
        color.G = pgm_read_byte(p++);
        color.B = pgm_read_byte(p++);
        color.R = pgm_read_byte(p);

        return color;
    }

};

class DotStarLgbrFeature : public DotStar4Elements
{
public:
    static void applyPixelColor(uint8_t* pPixels, uint16_t indexPixel, ColorObject color)
    {
        uint8_t* p = getPixelAddress(pPixels, indexPixel);

        *p++ = 0xE0 | (color.W < 31 ? color.W : 31); // upper three bits are always 111
        *p++ = color.G;
        *p++ = color.B;
        *p = color.R;
    }

    static ColorObject retrievePixelColor(uint8_t* pPixels, uint16_t indexPixel)
    {
        ColorObject color;
        uint8_t* p = getPixelAddress(pPixels, indexPixel);

        color.W = (*p++) & 0x1F; // mask out upper three bits
        color.G = *p++;
        color.B = *p++;
        color.R = *p;

        return color;
    }

    static ColorObject retrievePixelColor_P(PGM_VOID_P pPixels, uint16_t indexPixel)
    {
        ColorObject color;
        const uint8_t* p = getPixelAddress((const uint8_t*)pPixels, indexPixel);

        color.W = pgm_read_byte(p++) & 0x1F; // mask out upper three bits
        color.G = pgm_read_byte(p++);
        color.B = pgm_read_byte(p++);
        color.R = pgm_read_byte(p);

        return color;
    }

};
/* BRG Feature -- Some APA102s ship in BRG order */
class DotStarBrgFeature : public DotStar3Elements
{
public:
    static void applyPixelColor(uint8_t* pPixels, uint16_t indexPixel, ColorObject color)
    {
        uint8_t* p = getPixelAddress(pPixels, indexPixel);

        *p++ = 0xff; // upper three bits are always 111 and brightness at max
        *p++ = color.B;
        *p++ = color.R;
        *p = color.G;
    }

    static ColorObject retrievePixelColor(uint8_t* pPixels, uint16_t indexPixel)
    {
        ColorObject color;
        uint8_t* p = getPixelAddress(pPixels, indexPixel);

        p++; // ignore the first byte
        color.B = *p++;
        color.R = *p++;
        color.G = *p;

        return color;
    }

    static ColorObject retrievePixelColor_P(PGM_VOID_P pPixels, uint16_t indexPixel)
    {
        ColorObject color;
        const uint8_t* p = getPixelAddress((const uint8_t*)pPixels, indexPixel);

        pgm_read_byte(p++); // ignore the first byte
        color.B = pgm_read_byte(p++);
        color.R = pgm_read_byte(p++);
        color.G = pgm_read_byte(p);

        return color;
    }

};

class DotStarLbrgFeature : public DotStar4Elements
{
public:
    static void applyPixelColor(uint8_t* pPixels, uint16_t indexPixel, ColorObject color)
    {
        uint8_t* p = getPixelAddress(pPixels, indexPixel);

        *p++ = 0xE0 | (color.W < 31 ? color.W : 31); // upper three bits are always 111
        *p++ = color.B;
        *p++ = color.R;
        *p = color.G;
    }

    static ColorObject retrievePixelColor(uint8_t* pPixels, uint16_t indexPixel)
    {
        ColorObject color;
        uint8_t* p = getPixelAddress(pPixels, indexPixel);

        color.W = (*p++) & 0x1F; // mask out upper three bits
        color.B = *p++;
        color.R = *p++;
        color.G = *p;

        return color;
    }

    static ColorObject retrievePixelColor_P(PGM_VOID_P pPixels, uint16_t indexPixel)
    {
        ColorObject color;
        const uint8_t* p = getPixelAddress((const uint8_t*)pPixels, indexPixel);

        color.W = pgm_read_byte(p++) & 0x1F; // mask out upper three bits
        color.B = pgm_read_byte(p++);
        color.R = pgm_read_byte(p++);
        color.G = pgm_read_byte(p);

        return color;
    }

};
