/*-------------------------------------------------------------------------
NeoPixelGamma class is used to correct RGB colors for human eye gamma levels

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

#include <Arduino.h>
#include "RgbColor.h"
#include "RgbwColor.h"
#include "NeoEase.h"
#include "NeoGamma.h"

const uint8_t NeoGammaTableMethod::_table[] = {
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   2,   2,   2,   2,   2,   2,
    3,   3,   3,   3,   3,   3,   4,   4,   4,   4,   5,   5,   5,   5,   6,   6,
    6,   7,   7,   7,   7,   8,   8,   8,   9,   9,   9,   10,  10,  11,  11,  11,
    12,  12,  13,  13,  14,  14,  14,  15,  15,  16,  16,  17,  17,  18,  18,  19,
    19,  20,  20,  21,  22,  22,  23,  23,  24,  25,  25,  26,  26,  27,  28,  28,
    29,  30,  30,  31,  32,  33,  33,  34,  35,  35,  36,  37,  38,  39,  39,  40,
    41,  42,  43,  43,  44,  45,  46,  47,  48,  49,  50,  50,  51,  52,  53,  54,
    55,  56,  57,  58,  59,  60,  61,  62,  63,  64,  65,  66,  67,  68,  69,  71,
    72,  73,  74,  75,  76,  77,  78,  80,  81,  82,  83,  84,  86,  87,  88,  89,
    91,  92,  93,  94,  96,  97,  98,  100, 101, 102, 104, 105, 106, 108, 109, 110,
    112, 113, 115, 116, 118, 119, 121, 122, 123, 125, 126, 128, 130, 131, 133, 134,
    136, 137, 139, 140, 142, 144, 145, 147, 149, 150, 152, 154, 155, 157, 159, 160,
    162, 164, 166, 167, 169, 171, 173, 175, 176, 178, 180, 182, 184, 186, 187, 189,
    191, 193, 195, 197, 199, 201, 203, 205, 207, 209, 211, 213, 215, 217, 219, 221,
    223, 225, 227, 229, 231, 233, 235, 238, 240, 242, 244, 246, 248, 251, 253, 255
};