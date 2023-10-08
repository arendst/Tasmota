/*-------------------------------------------------------------------------
HtmlColor provides a color object that can be directly consumed by NeoPixelBus

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
#include "RgbColor.h"

#define MAX_HTML_COLOR_NAME_LEN 21

#ifndef pgm_read_ptr
// ESP8266 doesn't define this macro
#define pgm_read_ptr(addr) (*reinterpret_cast<const void* const *>(addr))
#endif

#ifndef countof
#define countof(array) (sizeof(array)/sizeof(array[0]))
#endif

// ------------------------------------------------------------------------
// HtmlColorPair represents an association between a name and a HTML color code
// ------------------------------------------------------------------------
struct HtmlColorPair
{
    PGM_P Name;
    uint32_t Color;
};

// ------------------------------------------------------------------------
// HtmlShortColorNames is a template class used for Parse and ToString
// ------------------------------------------------------------------------
class HtmlShortColorNames
{
public:
    static const HtmlColorPair* Pair(uint8_t index);
    static uint8_t Count();
};

// ------------------------------------------------------------------------
// HtmlColorNames is a template class used for Parse and ToString
// ------------------------------------------------------------------------
class HtmlColorNames
{
public:
    static const HtmlColorPair* Pair(uint8_t index);
    static uint8_t Count();
};

// ------------------------------------------------------------------------
// HtmlColor represents a color object that is represented by a single uint32
// value.  It contains minimal routines and used primarily as a helper to
// initialize other color objects
// ------------------------------------------------------------------------
struct HtmlColor
{
    // ------------------------------------------------------------------------
    // Construct a HtmlColor using a single value (0-0xffffff)
    // This works well for hexidecimal color definitions
    // 0xff0000 = red, 0x00ff00 = green, and 0x0000ff = blue
    // ------------------------------------------------------------------------
    HtmlColor(uint32_t color) :
        Color(color)
    {
    };

    // ------------------------------------------------------------------------
    // Construct a HtmlColor using RgbColor
    // ------------------------------------------------------------------------
    HtmlColor(const RgbColor& color)
    {
        Color = static_cast<uint32_t>(color.R) << 16 | static_cast<uint32_t>(color.G) << 8 | static_cast<uint32_t>(color.B);
    }

    // ------------------------------------------------------------------------
    // Construct a HtmlColor that will have its values set in latter operations
    // CAUTION:  The Color member is not initialized and may not be consistent
    // ------------------------------------------------------------------------
    HtmlColor()
    {
    };

    // ------------------------------------------------------------------------
    // Comparison operators
    // ------------------------------------------------------------------------
    bool operator==(const HtmlColor& other) const
    {
        return (Color == other.Color);
    };

    bool operator!=(const HtmlColor& other) const
    {
        return !(*this == other);
    };

    // ------------------------------------------------------------------------
    // Parse a HTML4/CSS3 color name
    // T_HTMLCOLORNAMES - template class that defines the collection of names
    // name - the color name
    // nameSize - the max size of name to check
    //
    // returns - zero if failed, or the number of chars parsed
    //
    // It will stop parsing name when a null terminator is reached, 
    // nameSize is reached, no match is found in the name/color pair table, or
    // a non-alphanumeric is read like seperators or whitespace.
    //
    // It also accepts 3 or 6 digit hexadecimal notation (#rgb or #rrggbb),
    // but it doesn't accept RGB, RGBA nor HSL values.
    //
    // See https://www.w3.org/TR/css3-color/#SRGB
    //
    // name must point to the first non-whitespace character to be parsed
    // parsing will stop at the first non-alpha numeric
    //
    // Name MUST NOT be a PROGMEM pointer
    //
    // examples:
    //  Parse<HtmlShortColorNames>(buff, buffSize);
    //  Parse<HtmlColorNames>(buff, buffSize);
    // ------------------------------------------------------------------------

    template <typename T_HTMLCOLORNAMES> size_t Parse(const char* name, size_t nameSize)
    {
        if (nameSize > 0)
        {
            if (name[0] == '#')
            {
                // Parse an hexadecimal notation "#rrbbgg" or "#rgb"
                //
                uint8_t temp[6]; // stores preconverted chars to hex values
                uint8_t tempSize = 0;

                for (uint8_t indexChar = 1; indexChar < nameSize && indexChar < 8; ++indexChar)
                {
                    char c = name[indexChar];
                    if (c >= '0' && c <= '9')
                    {
                        c -= '0';
                    }
                    else
                    {
                        // Convert a letter to lower case (only for ASCII letters)
                        // It's faster & smaller than tolower()
                        c |= 32;
                        if (c >= 'a' && c <= 'f')
                        {
                            c = c - 'a' + 10;
                        }
                        else
                        {
                            // we found an non hexidecimal char
                            // which could be null, deliminator, or other spacing
                            break;
                        }
                    }

                    temp[tempSize] = c;
                    tempSize++;
                }

                if (tempSize != 3 && tempSize != 6)
                {
                    // invalid count of numerical chars
                    return 0;
                }
                else
                {
                    uint32_t color = 0;
                    for (uint8_t indexChar = 0; indexChar < tempSize; ++indexChar)
                    {
                        color = color * 16 + temp[indexChar];
                        if (tempSize == 3)
                        {
                            // 3 digit hexadecimal notation can be supported easily
                            // duplicating digits.
                            color = color * 16 + temp[indexChar];
                        }
                    }

                    Color = color;
                    return tempSize;
                }
            }
            else
            {
                // parse a standard name for the color
                //

                // the normal list is small enough a binary search isn't interesting,
                for (uint8_t indexName = 0; indexName < T_HTMLCOLORNAMES::Count(); ++indexName)
                {
                    const HtmlColorPair* colorPair = T_HTMLCOLORNAMES::Pair(indexName);
                    PGM_P searchName = reinterpret_cast<PGM_P>(pgm_read_ptr(&(colorPair->Name)));
                    size_t str1Size = nameSize;
                    const char* str1 = name;
                    const char* str2P = searchName;

                    uint16_t result;

                    while (str1Size > 0)
                    {
                        char ch1 = tolower(*str1++);
                        char ch2 = tolower(pgm_read_byte(str2P++));
                        result = ch1 - ch2;
                        if (result != 0 || ch2 == '\0')
                        {
                            if (ch2 == '\0' && !isalnum(ch1))
                            {
                                // the string continues but is not part of a
                                // valid color name, 
                                // ends in white space, deliminator, etc
                                result = 0;
                            }
                            break;
                        }
                        result = -1; // have not reached the end of searchName;
                        str1Size--;
                    }

                    if (result == 0)
                    {
                        Color = pgm_read_dword(&colorPair->Color);
                        return nameSize - str1Size;
                    }
                }
            }
        }

        return 0;
    }

    template <typename T_HTMLCOLORNAMES> size_t Parse(const char* name)
    { 
        return Parse<T_HTMLCOLORNAMES>(name, MAX_HTML_COLOR_NAME_LEN + 1);
    }

    template <typename T_HTMLCOLORNAMES> size_t Parse(String const &name)
    { 
        return Parse<T_HTMLCOLORNAMES>(name.c_str(), name.length() + 1);
    }

    // ------------------------------------------------------------------------
    // Converts this color code to its HTML4/CSS3 name
    // T_HTMLCOLORNAMES - template class that defines the collection of names
    // buf - buffer to write the string
    // bufSize - actual size of buf array
    //
    // It returns the number of chars required not including the NUL terminator.
    //
    // If there is not enough space in the buffer, it will write as many
    // characters as allowed and will always finish the buffer with a NUL char
    //
    // examples:
    //  ToString<HtmlShortColorNames>(buf, bufSize);
    //  ToString<htmlColorNames>(buf, bufSize);
    // ------------------------------------------------------------------------

    template <typename T_HTMLCOLORNAMES> size_t ToString(char* buf, size_t bufSize) const
    {
        // search for a color value/name pairs first
        for (uint8_t indexName = 0; indexName < T_HTMLCOLORNAMES::Count(); ++indexName)
        {
            const HtmlColorPair* colorPair = T_HTMLCOLORNAMES::Pair(indexName);
            if (pgm_read_dword(&colorPair->Color) == Color)
            {
                PGM_P name = (PGM_P)pgm_read_ptr(&colorPair->Name);
                strncpy_P(buf, name, bufSize);
                return strlen_P(name);
            }
        }

        // no color name pair match, convert using numerical format
        return ToNumericalString(buf, bufSize);
    }

    // ------------------------------------------------------------------------
    // Converts this color code to its HTML4/CSS3 numerical name
    //
    // buf - buffer to write the string
    // bufSize - actual size of buf array
    //
    // It returns the number of chars required not including the NUL terminator.
    //
    // If there is not enough space in the buffer, it will write as many
    // characters as allowed and will always finish the buffer with a NUL char
    // ------------------------------------------------------------------------

    size_t ToNumericalString(char* buf, size_t bufSize) const;

    // ------------------------------------------------------------------------
    // BilinearBlend between four colors by the amount defined by 2d variable
    // c00 - upper left quadrant color
    // c01 - upper right quadrant color
    // c10 - lower left quadrant color
    // c11 - lower right quadrant color
    // x - unit value (0.0 - 1.0) that defines the blend progress in horizontal space
    // y - unit value (0.0 - 1.0) that defines the blend progress in vertical space
    // ------------------------------------------------------------------------

    static HtmlColor BilinearBlend(const HtmlColor& c00,
        const HtmlColor& c01,
        const HtmlColor& c10,
        const HtmlColor& c11,
        float x,
        float y)
    {
        return RgbColor::BilinearBlend(c00, c01, c10, c11, x, y);
    }

    // ------------------------------------------------------------------------
    // Color member (0-0xffffff) where 
    // 0xff0000 is red
    // 0x00ff00 is green
    // 0x0000ff is blue
    // ------------------------------------------------------------------------
    uint32_t Color;
};

