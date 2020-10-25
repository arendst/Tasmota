/*-------------------------------------------------------------------------
SegmentDigit provides a color object that can be directly consumed by NeoPixelBus

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

#include "SegmentDigit.h"

//
// https://en.wikichip.org/wiki/seven-segment_display/representing_letters
//
const uint8_t SevenSegDigit::DecodeNumbers[] = {
    // 0     1     2     3     4     5     6     7     8     9 
    0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F };

const uint8_t SevenSegDigit::DecodeAlphaCaps[] = {
    // A     B     C     D     E     F     G  
    0x77, 0x00, 0x39, 0x00, 0x79, 0x71, 0x3D,
    // H     I     J     K     L     M     N    
    0x76, 0x30, 0x1E, 0x00, 0x38, 0x00, 0x00,
    // O     P     Q     R     S 
    0x3F, 0x73, 0x00, 0x00, 0x6D,
    // T     U     V     W     X     Y     Z  
    0x00, 0x3E, 0x00, 0x00, 0x00, 0x00, 0x00 };

const uint8_t SevenSegDigit::DecodeAlpha[] = {
    // a     b     c     d     e     f     g  
    0x00, 0x7C, 0x58, 0x5E, 0x00, 0x00, 0x00,
    // h     i     j     k     l     m     n 
    0x74, 0x00, 0x00, 0x00, 0x00, 0x00, 0x54,
    // o     p     q     r     s     
    0x5C, 0x00, 0x67, 0x50, 0x00,
    // t     u     v     w     x     y     z 
    0x78, 0x1C, 0x00, 0x00, 0x00, 0x6E, 0x00 };

const uint8_t SevenSegDigit::DecodeSpecial[] = {
    // ,     -     .     /
    0x80, 0x40, 0x80, 0x40 };

void SevenSegDigit::init(uint8_t bitmask, uint8_t brightness, uint8_t defaultBrightness)
{
    for (uint8_t iSegment = 0; iSegment < SegmentCount; iSegment++)
    {
        Segment[iSegment] = (bitmask & 0x01) ? brightness : defaultBrightness;
        bitmask >>= 1;
    }
}

SevenSegDigit::SevenSegDigit(uint8_t bitmask, uint8_t brightness, uint8_t defaultBrightness)
{
    init(bitmask, brightness, defaultBrightness);
};

SevenSegDigit::SevenSegDigit(char letter, uint8_t brightness, uint8_t defaultBrightness)
{
    if (letter >= '0' && letter <= '9')
    {
        init(DecodeNumbers[letter - '0'], brightness, defaultBrightness);
    }
    else if (letter >= 'a' && letter <= 'z')
    {
        init(DecodeAlpha[letter - 'a'], brightness, defaultBrightness);
    }
    else if (letter >= 'A' && letter <= 'Z')
    {
        init(DecodeAlphaCaps[letter - 'A'], brightness, defaultBrightness);
    }
    else if (letter >= ',' && letter <= '/')
    {
        init(DecodeSpecial[letter - ','], brightness, defaultBrightness);
    }
    else
    {
        memset(Segment, defaultBrightness, sizeof(Segment));
    }
};

uint8_t SevenSegDigit::CalculateBrightness() const
{
    uint16_t sum = 0;

    for (uint8_t iSegment = 0; iSegment < SegmentCount; iSegment++)
    {
        sum += Segment[iSegment];
    }

    return (uint8_t)(sum / SegmentCount);
}

void SevenSegDigit::Darken(uint8_t delta)
{
    for (uint8_t iSegment = 0; iSegment < SegmentCount; iSegment++)
    {
        uint8_t element = Segment[iSegment];
        if (element > delta)
        {
            element -= delta;
        }
        else
        {
            element = 0;
        }
        Segment[iSegment] = element;
    }
}

void SevenSegDigit::Lighten(uint8_t delta)
{
    for (uint8_t iSegment = 0; iSegment < SegmentCount; iSegment++)
    {
        uint8_t element = Segment[iSegment];
        if (element < 255 - delta)
        {
            element += delta;
        }
        else
        {
            element = 255;
        }
        Segment[iSegment] = element;
    }
}

SevenSegDigit SevenSegDigit::LinearBlend(const SevenSegDigit& left, const SevenSegDigit& right, float progress)
{
    SevenSegDigit result;

    for (uint8_t iSegment = 0; iSegment < SegmentCount; iSegment++)
    {
        result.Segment[iSegment] = left.Segment[iSegment] + ((right.Segment[iSegment] - left.Segment[iSegment]) * progress);
    }
    return result;
}
