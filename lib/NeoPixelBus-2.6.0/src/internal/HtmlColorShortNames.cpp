/*-------------------------------------------------------------------------
HtmlShortColorNames provides a template class for access to the short name table

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

#include "HtmlColor.h"
#include "HtmlColorNameStrings.h"

static const HtmlColorPair c_ShortColorNames[] PROGMEM = {
    { c_HtmlNameAqua, 0xffff },
    { c_HtmlNameBlack, 0x0 },
    { c_HtmlNameBlue, 0xff },
    { c_HtmlNameFuchsia, 0xff00ff },
    { c_HtmlNameGray, 0x808080 },
    { c_HtmlNameGreen, 0x8000 },
    { c_HtmlNameLime, 0xff00 },
    { c_HtmlNameMaroon, 0x800000 },
    { c_HtmlNameNavy, 0x80 },
    { c_HtmlNameOlive, 0x808000 },
    { c_HtmlNameOrange, 0xffa500 },
    { c_HtmlNamePurple, 0x800080 },
    { c_HtmlNameRed, 0xff0000 },
    { c_HtmlNameSilver, 0xc0c0c0 },
    { c_HtmlNameTeal, 0x8080 },
    { c_HtmlNameWhite, 0xffffff },
    { c_HtmlNameYellow, 0xffff00 },
};


const HtmlColorPair* HtmlShortColorNames::Pair(uint8_t index)
{
    return &c_ShortColorNames[index];
}

uint8_t HtmlShortColorNames::Count()
{
    return countof(c_ShortColorNames);
}