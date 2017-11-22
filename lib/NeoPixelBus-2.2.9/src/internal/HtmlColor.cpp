/*-------------------------------------------------------------------------
This file contains the HtmlColor implementation

Written by Unai Uribarri

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
#include "HtmlColor.h"

static inline char hexdigit(uint8_t v)
{
    return v + (v < 10 ? '0' : 'a' - 10);
}


size_t HtmlColor::ToNumericalString(char* buf, size_t bufSize) const
{
    size_t bufLen = bufSize - 1;

    if (bufLen-- > 0)
    {
        if (bufLen > 0)
        {
            buf[0] = '#';
        }

        uint32_t color = Color;
        for (uint8_t indexDigit = 6; indexDigit > 0; indexDigit--)
        {
            if (bufLen > indexDigit)
            {
                buf[indexDigit] = hexdigit(color & 0x0000000f);
            }
            color >>= 4;
        }

        buf[(bufLen < 7 ? bufLen : 7)] = 0;
    }
    return 7;
}
