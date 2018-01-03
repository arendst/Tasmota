/*-------------------------------------------------------------------------
HtmlColorNames provides a template class for access to the full name table

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

static const HtmlColorPair c_ColorNames[] PROGMEM = {
    { c_HtmlNameAliceBlue, 0xf0f8ff },
    { c_HtmlNameAntiqueWhite, 0xfaebd7 },
    { c_HtmlNameAqua, 0xffff },
    { c_HtmlNameAquamarine, 0x7fffd4 },
    { c_HtmlNameAzure, 0xf0ffff },
    { c_HtmlNameBeige, 0xf5f5dc },
    { c_HtmlNameBisque, 0xffe4c4 },
    { c_HtmlNameBlack, 0x0 },
    { c_HtmlNameBlanchedAlmond, 0xffebcd },
    { c_HtmlNameBlue, 0xff },
    { c_HtmlNameBlueViolet, 0x8a2be2 },
    { c_HtmlNameBrown, 0xa52a2a },
    { c_HtmlNameBurlyWood, 0xdeb887 },
    { c_HtmlNameCadetBlue, 0x5f9ea0 },
    { c_HtmlNameChartreuse, 0x7fff00 },
    { c_HtmlNameChocolate, 0xd2691e },
    { c_HtmlNameCoral, 0xff7f50 },
    { c_HtmlNameCornflowerBlue, 0x6495ed },
    { c_HtmlNameCornsilk, 0xfff8dc },
    { c_HtmlNameCrimson, 0xdc143c },
    { c_HtmlNameCyan, 0xffff },
    { c_HtmlNameDarkBlue, 0x8b },
    { c_HtmlNameDarkCyan, 0x8b8b },
    { c_HtmlNameDarkGoldenrod, 0xb8860b },
    { c_HtmlNameDarkGray, 0xa9a9a9 },
    { c_HtmlNameDarkGreen, 0x6400 },
    { c_HtmlNameDarkGrey, 0xa9a9a9 },
    { c_HtmlNameDarkKhaki, 0xbdb76b },
    { c_HtmlNameDarkMagenta, 0x8b008b },
    { c_HtmlNameDarkOliveGreen, 0x556b2f },
    { c_HtmlNameDarkOrange, 0xff8c00 },
    { c_HtmlNameDarkOrchid, 0x9932cc },
    { c_HtmlNameDarkRed, 0x8b0000 },
    { c_HtmlNameDarkSalmon, 0xe9967a },
    { c_HtmlNameDarkSeaGreen, 0x8fbc8f },
    { c_HtmlNameDarkSlateBlue, 0x483d8b },
    { c_HtmlNameDarkSlateGray, 0x2f4f4f },
    { c_HtmlNameDarkSlateGrey, 0x2f4f4f },
    { c_HtmlNameDarkTurquoise, 0xced1 },
    { c_HtmlNameDarkViolet, 0x9400d3 },
    { c_HtmlNameDeepPink, 0xff1493 },
    { c_HtmlNameDeepSkyBlue, 0xbfff },
    { c_HtmlNameDimGray, 0x696969 },
    { c_HtmlNameDimGrey, 0x696969 },
    { c_HtmlNameDodgerBlue, 0x1e90ff },
    { c_HtmlNameFirebrick, 0xb22222 },
    { c_HtmlNameFloralWhite, 0xfffaf0 },
    { c_HtmlNameForestGreen, 0x228b22 },
    { c_HtmlNameFuchsia, 0xff00ff },
    { c_HtmlNameGainsboro, 0xdcdcdc },
    { c_HtmlNameGhostWhite, 0xf8f8ff },
    { c_HtmlNameGold, 0xffd700 },
    { c_HtmlNameGoldenrod, 0xdaa520 },
    { c_HtmlNameGray, 0x808080 },
    { c_HtmlNameGreen, 0x8000 },
    { c_HtmlNameGreenYellow, 0xadff2f },
    { c_HtmlNameGrey, 0x808080 },
    { c_HtmlNameHoneydew, 0xf0fff0 },
    { c_HtmlNameHotPink, 0xff69b4 },
    { c_HtmlNameIndianRed, 0xcd5c5c },
    { c_HtmlNameIndigo, 0x4b0082 },
    { c_HtmlNameIvory, 0xfffff0 },
    { c_HtmlNameKhaki, 0xf0e68c },
    { c_HtmlNameLavender, 0xe6e6fa },
    { c_HtmlNameLavenderBlush, 0xfff0f5 },
    { c_HtmlNameLawnGreen, 0x7cfc00 },
    { c_HtmlNameLemonChiffon, 0xfffacd },
    { c_HtmlNameLightBlue, 0xadd8e6 },
    { c_HtmlNameLightCoral, 0xf08080 },
    { c_HtmlNameLightCyan, 0xe0ffff },
    { c_HtmlNameLightGoldenrodYellow, 0xfafad2 },
    { c_HtmlNameLightGray, 0xd3d3d3 },
    { c_HtmlNameLightGreen, 0x90ee90 },
    { c_HtmlNameLightGrey, 0xd3d3d3 },
    { c_HtmlNameLightPink, 0xffb6c1 },
    { c_HtmlNameLightSalmon, 0xffa07a },
    { c_HtmlNameLightSeaGreen, 0x20b2aa },
    { c_HtmlNameLightSkyBlue, 0x87cefa },
    { c_HtmlNameLightSlateGray, 0x778899 },
    { c_HtmlNameLightSlateGrey, 0x778899 },
    { c_HtmlNameLightSteelBlue, 0xb0c4de },
    { c_HtmlNameLightYellow, 0xffffe0 },
    { c_HtmlNameLime, 0xff00 },
    { c_HtmlNameLimeGreen, 0x32cd32 },
    { c_HtmlNameLinen, 0xfaf0e6 },
    { c_HtmlNameMagenta, 0xff00ff },
    { c_HtmlNameMaroon, 0x800000 },
    { c_HtmlNameMediumAquamarine, 0x66cdaa },
    { c_HtmlNameMediumBlue, 0xcd },
    { c_HtmlNameMediumOrchid, 0xba55d3 },
    { c_HtmlNameMediumPurple, 0x9370d8 },
    { c_HtmlNameMediumSeagreen, 0x3cb371 },
    { c_HtmlNameMediumSlateBlue, 0x7b68ee },
    { c_HtmlNameMediumSpringGreen, 0xfa9a },
    { c_HtmlNameMediumTurquoise, 0x48d1cc },
    { c_HtmlNameMediumVioletRed, 0xc71585 },
    { c_HtmlNameMidnightBlue, 0x191970 },
    { c_HtmlNameMintCream, 0xf5fffa },
    { c_HtmlNameMistyRose, 0xffe4e1 },
    { c_HtmlNameMoccasin, 0xffe4b5 },
    { c_HtmlNameNavajoWhite, 0xffdead },
    { c_HtmlNameNavy, 0x80 },
    { c_HtmlNameOldLace, 0xfdf5e6 },
    { c_HtmlNameOlive, 0x808000 },
    { c_HtmlNameOliveDrab, 0x6b8e23 },
    { c_HtmlNameOrange, 0xffa500 },
    { c_HtmlNameOrangeRed, 0xff4500 },
    { c_HtmlNameOrchid, 0xda70d6 },
    { c_HtmlNamePaleGoldenrod, 0xeee8aa },
    { c_HtmlNamePaleGreen, 0x98fb98 },
    { c_HtmlNamePaleTurquoise, 0xafeeee },
    { c_HtmlNamePaleVioletRed, 0xd87093 },
    { c_HtmlNamePapayaWhip, 0xffefd5 },
    { c_HtmlNamePeachPuff, 0xffdab9 },
    { c_HtmlNamePeru, 0xcd853f },
    { c_HtmlNamePink, 0xffc0cb },
    { c_HtmlNamePlum, 0xdda0dd },
    { c_HtmlNamePowderBlue, 0xb0e0e6 },
    { c_HtmlNamePurple, 0x800080 },
    { c_HtmlNameRed, 0xff0000 },
    { c_HtmlNameRosyBrown, 0xbc8f8f },
    { c_HtmlNameRoyalBlue, 0x4169e1 },
    { c_HtmlNameSaddleBrown, 0x8b4513 },
    { c_HtmlNameSalmon, 0xfa8072 },
    { c_HtmlNameSandyBrown, 0xf4a460 },
    { c_HtmlNameSeaGreen, 0x2e8b57 },
    { c_HtmlNameSeaShell, 0xfff5ee },
    { c_HtmlNameSienna, 0xa0522d },
    { c_HtmlNameSilver, 0xc0c0c0 },
    { c_HtmlNameSkyBlue, 0x87ceeb },
    { c_HtmlNameSlateBlue, 0x6a5acd },
    { c_HtmlNameSlateGray, 0x708090 },
    { c_HtmlNameSlateGrey, 0x708090 },
    { c_HtmlNameSnow, 0xfffafa },
    { c_HtmlNameSpringGreen, 0xff7f },
    { c_HtmlNameSteelBlue, 0x4682b4 },
    { c_HtmlNameTan, 0xd2b48c },
    { c_HtmlNameTeal, 0x8080 },
    { c_HtmlNameThistle, 0xd8bfd8 },
    { c_HtmlNameTomato, 0xff6347 },
    { c_HtmlNameTurquoise, 0x40e0d0 },
    { c_HtmlNameViolet, 0xee82ee },
    { c_HtmlNameWheat, 0xf5deb3 },
    { c_HtmlNameWhite, 0xffffff },
    { c_HtmlNameWhiteSmoke, 0xf5f5f5 },
    { c_HtmlNameYellow, 0xffff00 },
    { c_HtmlNameYellowGreen, 0x9acd32 },
};

const HtmlColorPair* HtmlColorNames::Pair(uint8_t index)
{
    return &c_ColorNames[index];
}

uint8_t HtmlColorNames::Count()
{
    return countof(c_ColorNames);
}
