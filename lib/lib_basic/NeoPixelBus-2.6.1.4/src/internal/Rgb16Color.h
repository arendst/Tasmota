/*-------------------------------------------------------------------------
Rgb16Color provides a color object that stores in only 16 bits, aka 565 format

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

#include <Arduino.h>
#include "NeoSettings.h"
#include "RgbColorBase.h"


// ------------------------------------------------------------------------
// Rgb16Color represents a color object that is represented by Red, Green, Blue
// component values stored in a single 16 bit value using 565 model.  
// It contains helpful color routines to manipulate the color.
// ------------------------------------------------------------------------
struct Rgb16Color : RgbColorBase
{
    typedef NeoRgbCurrentSettings SettingsObject;

    // ------------------------------------------------------------------------
    // Construct a Rgb16Color using R, G, B values (0-255)
    // ------------------------------------------------------------------------
    Rgb16Color(uint8_t r, uint8_t g, uint8_t b) 
    {
        setR(r);
        setG(g);
        setB(b);
    };

    // ------------------------------------------------------------------------
    // Construct a Rgb16Color using a single brightness value (0-255)
    // This works well for creating gray tone colors
    // (0) = black, (255) = white, (128) = gray
    // ------------------------------------------------------------------------
    Rgb16Color(uint8_t brightness) 
    {
        setR(brightness);
        setG(brightness);
        setB(brightness);
    };

    // ------------------------------------------------------------------------
    // Construct a Rgb16Color using HtmlColor
    // ------------------------------------------------------------------------
    Rgb16Color(const HtmlColor& color)
    {
        RgbColor converted = color;

        setR(converted.R);
        setG(converted.G);
        setB(converted.B);
    };

    // ------------------------------------------------------------------------
    // Construct a Rgb16Color using HslColor
    // ------------------------------------------------------------------------
    Rgb16Color(const HslColor& color)
    {
        RgbColor converted = color;

        setR(converted.R);
        setG(converted.G);
        setB(converted.B);
    };

    // ------------------------------------------------------------------------
    // Construct a Rgb16Color using HsbColor
    // ------------------------------------------------------------------------
    Rgb16Color(const HsbColor& color)
    {
        RgbColor converted = color;

        setR(converted.R);
        setG(converted.G);
        setB(converted.B);
    };

    // ------------------------------------------------------------------------
    // Construct a Rgb16Color that will have its values set in latter operations
    // CAUTION:  The _c members are not initialized and may not be consistent
    // ------------------------------------------------------------------------
    Rgb16Color()
    {
    };

    // ------------------------------------------------------------------------
    // properties
    // ------------------------------------------------------------------------
    void setR(uint8_t r)
    {
        _c &= 0x07ff;
        _c |= ((r & 0xf8) << 8);
    };
    uint8_t getR() const
    {
        return (_c & 0xf800) >> 8;
    };

    void setG(uint8_t g)
    {
        _c &= 0xf81f;
        _c |= ((g & 0xfe) << 3);
    };
    uint8_t getG() const
    {
        return (_c & 0x07e0) >> 3;
    };

    void setB(uint8_t b)
    {
        _c &= 0xffe0;
        _c |= ((b & 0xf8) >> 3);
    };
    uint8_t getB() const
    {
        return (_c & 0x001f) << 3;
    };


    // ------------------------------------------------------------------------
    // Comparison operators
    // ------------------------------------------------------------------------
    bool operator==(const Rgb16Color& other) const
    {
        return (_c == other._c);
    };

    bool operator!=(const Rgb16Color& other) const
    {
        return !(*this == other);
    };

    // ------------------------------------------------------------------------
    // CalculateBrightness will calculate the overall brightness
    // NOTE: This is a simple linear brightness
    // ------------------------------------------------------------------------
    uint8_t CalculateBrightness() const
    {
        RgbColor converted = *this;
        return converted.CalculateBrightness();
    };

    // ------------------------------------------------------------------------
    // Dim will return a new color that is blended to black with the given ratio
    // ratio - (0-255) where 255 will return the original color and 0 will return black
    // 
    // NOTE: This is a simple linear blend
    // ------------------------------------------------------------------------
    Rgb16Color Dim(uint8_t ratio) const
    {
        RgbColor converted = *this;
        RgbColor result = converted.Dim(ratio);

        return Rgb16Color(result.R, result.G, result.B);
    };

    // ------------------------------------------------------------------------
    // Brighten will return a new color that is blended to white with the given ratio
    // ratio - (0-255) where 255 will return the original color and 0 will return white
    // 
    // NOTE: This is a simple linear blend
    // ------------------------------------------------------------------------
    Rgb16Color Brighten(uint8_t ratio) const
    {
        RgbColor converted = *this;
        RgbColor result = converted.Brighten(ratio);

        return Rgb16Color(result.R, result.G, result.B);
    };

    // ------------------------------------------------------------------------
    // Darken will adjust the color by the given delta toward black
    // NOTE: This is a simple linear change
    // delta - (0-255) the amount to dim the color
    // ------------------------------------------------------------------------
    void Darken(uint8_t delta)
    {
        RgbColor converted = *this;

        converted.Darken(delta);
        setR(converted.R);
        setG(converted.G);
        setB(converted.B);
    };

    // ------------------------------------------------------------------------
    // Lighten will adjust the color by the given delta toward white
    // NOTE: This is a simple linear change
    // delta - (0-255) the amount to lighten the color
    // ------------------------------------------------------------------------
    void Lighten(uint8_t delta)
    {
        RgbColor converted = *this;

        converted.Lighten(delta);
        setR(converted.R);
        setG(converted.G);
        setB(converted.B);
    };

    // ------------------------------------------------------------------------
    // LinearBlend between two colors by the amount defined by progress variable
    // left - the color to start the blend at
    // right - the color to end the blend at
    // progress - (0.0 - 1.0) value where 0 will return left and 1.0 will return right
    //     and a value between will blend the color weighted linearly between them
    // ------------------------------------------------------------------------
    static Rgb16Color LinearBlend(const Rgb16Color& left, const Rgb16Color& right, float progress)
    {
        RgbColor result = RgbColor::LinearBlend(left, right, progress);

        return Rgb16Color(result.R, result.G, result.B);
    };
    
    // ------------------------------------------------------------------------
    // BilinearBlend between four colors by the amount defined by 2d variable
    // c00 - upper left quadrant color
    // c01 - upper right quadrant color
    // c10 - lower left quadrant color
    // c11 - lower right quadrant color
    // x - unit value (0.0 - 1.0) that defines the blend progress in horizontal space
    // y - unit value (0.0 - 1.0) that defines the blend progress in vertical space
    // ------------------------------------------------------------------------
    static Rgb16Color BilinearBlend(const Rgb16Color& c00,
        const Rgb16Color& c01,
        const Rgb16Color& c10,
        const Rgb16Color& c11,
        float x,
        float y)
    {
        RgbColor result = RgbColor::BilinearBlend(c00, c01, c10, c11, x, y);

        return Rgb16Color(result.R, result.G, result.B);
    };

    uint32_t CalcTotalTenthMilliAmpere(const SettingsObject& settings)
    {
        auto total = 0;

        total += getR() * settings.RedTenthMilliAmpere / 255;
        total += getG() * settings.GreenTenthMilliAmpere / 255;
        total += getB() * settings.BlueTenthMilliAmpere / 255;

        return total;
    };

protected:
    uint16_t _c;
};

