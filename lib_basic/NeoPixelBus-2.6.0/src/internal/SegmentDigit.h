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
#pragma once

#include <Arduino.h>

enum LedSegment
{
    LedSegment_A,
    LedSegment_B,
    LedSegment_C,
    LedSegment_D,
    LedSegment_E,
    LedSegment_F,
    LedSegment_G,
    LedSegment_Decimal,
    LedSegment_COUNT
};

// ------------------------------------------------------------------------
// SevenSegDigit represents a color object that is represented by the segments
// of a 7 segment LED display digit.  It contains helpful routines to manipulate 
// and set the elements.
//
// The order represents the physical LED location starting at A, through to G, then
// ending at the decimal point
// "abcdefg."
// ------------------------------------------------------------------------
struct SevenSegDigit
{
    // ------------------------------------------------------------------------
    // Construct a SevenSegDigit using 
    //   the default brightness to apply to all segments
    // ------------------------------------------------------------------------
    SevenSegDigit(uint8_t defaultBrightness)
    {
        memset(Segment, defaultBrightness, sizeof(Segment));
    }

    // ------------------------------------------------------------------------
    // Construct a SevenSegDigit using 
    //   a bitmask for the segment (bit order is  ".gfedcba")
    //   the brightness to apply to them, (0-255)
    //   the default brightness to apply to those not set in the bitmask (0-255)
    // ------------------------------------------------------------------------
    SevenSegDigit(uint8_t bitmask, uint8_t brightness, uint8_t defaultBrightness = 0);

    // ------------------------------------------------------------------------
    // Construct a SevenSegDigit using 
    //   a char that will get mapped to the segments,
    //   the brightness to apply to them, (0-255)
    //   the default brightness to apply to those not set in the bitmask (0-255)
    // ------------------------------------------------------------------------
    SevenSegDigit(char letter, uint8_t brightness, uint8_t defaultBrightness = 0);

    // ------------------------------------------------------------------------
    // Construct a SevenSegDigit that will have its values set in latter operations
    // CAUTION:  The members are not initialized and may not be consistent
    // ------------------------------------------------------------------------
    SevenSegDigit()
    {
    };

    // ------------------------------------------------------------------------
    // Comparison operators
    // ------------------------------------------------------------------------
    bool operator==(const SevenSegDigit& other) const
    {
        for (uint8_t iSegment = 0; iSegment < SegmentCount; iSegment++)
        {
            if (Segment[iSegment] != other.Segment[iSegment])
            {
                return false;
            }
        }
        return true;
    };

    bool operator!=(const SevenSegDigit& other) const
    {
        return !(*this == other);
    };

    // ------------------------------------------------------------------------
    // CalculateBrightness will calculate the overall brightness
    // NOTE: This is a simple linear brightness
    // ------------------------------------------------------------------------
    uint8_t CalculateBrightness() const;

    // ------------------------------------------------------------------------
    // Darken will adjust the color by the given delta toward black
    // NOTE: This is a simple linear change
    // delta - (0-255) the amount to dim the segment
    // ------------------------------------------------------------------------
    void Darken(uint8_t delta);

    // ------------------------------------------------------------------------
    // Lighten will adjust the color by the given delta toward white
    // NOTE: This is a simple linear change
    // delta - (0-255) the amount to lighten the segment
    // ------------------------------------------------------------------------
    void Lighten(uint8_t delta);

    // ------------------------------------------------------------------------
    // LinearBlend between two colors by the amount defined by progress variable
    // left - the segment to start the blend at
    // right - the segment to end the blend at
    // progress - (0.0 - 1.0) value where 0 will return left and 1.0 will return right
    //     and a value between will blend the brightness of each element
    //     weighted linearly between them
    // ------------------------------------------------------------------------
    static SevenSegDigit LinearBlend(const SevenSegDigit& left, const SevenSegDigit& right, float progress);

    template <typename T_SET_TARGET> 
    static void SetString(T_SET_TARGET& target, uint16_t indexDigit, const char* str, uint8_t brightness, uint8_t defaultBrightness = 0)
    {
        if (str == nullptr)
        {
            return;
        }

        const char* pFirst = str;
        const char* pIter = str;

        // digits are right to left
        // so find the end
        while (*pIter != '\0')
        {
            pIter++;
        }
        pIter--;


        while (pIter >= pFirst)
        {
            bool decimal = false;
            char value = *pIter;

            // check if merging a decimal is required
            if (pIter > pFirst && (*pIter == '.' || *pIter == ','))
            {
                // merge a decimal as long as they aren't the same
                if (*(pIter - 1) != *pIter)
                {
                    decimal = true;
                    pIter--;
                    value = *pIter; // use the next char
                }
            }

            SevenSegDigit digit(value, brightness, defaultBrightness);
            if (decimal)
            {
                digit.Segment[LedSegment_Decimal] = brightness;
            }
            target.SetPixelColor(indexDigit, digit);
            indexDigit++;
        }
    }

    // ------------------------------------------------------------------------
    // segment members (0-255) where each represents the segment location
    // and the value defines the brightnes (0) is off and (255) is full brightness
    // ------------------------------------------------------------------------
    static const uint8_t SegmentCount = 9;
    uint8_t Segment[SegmentCount];


    // segment decode maps from ascii relative first char in map to a bitmask of segments
    //
    static const uint8_t DecodeNumbers[10]; // 0-9
    static const uint8_t DecodeAlphaCaps[26]; // A-Z
    static const uint8_t DecodeAlpha[26]; // a-z
    static const uint8_t DecodeSpecial[4]; // , - . /

protected:
    void init(uint8_t bitmask, uint8_t brightness, uint8_t defaultBrightness);
};

