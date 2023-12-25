/*
  be_animate_module.c - implements the animate module for Leds

  Copyright (C) 2023  Stephan Hadinger & Theo Arends

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/*******************************************************************\
 * `animate` global module
\*******************************************************************/

#ifdef USE_BERRY
#include "be_constobj.h"
#include "be_mapping.h"

#include "solidify/solidified_animate_1_core.h"
#include "solidify/solidified_animate_2_animate_effects.h"
#include "solidify/solidified_animate_9_module.h"


/*******************************************************************\
 * standard palettes
\*******************************************************************/
static const uint8_t PALETTE_RAINBOW_WHITE[] = {
  0x50,     0xFF, 0x00, 0x00,     // red
  0x30,     0xFF, 0x00, 0x00,     // red
  0x50,     0xFF, 0xA5, 0x00,     // orange
  0x30,     0xFF, 0xA5, 0x00,     // orange
  0x50,     0xFF, 0xFF, 0x00,     // yellow
  0x30,     0xFF, 0xFF, 0x00,     // yellow
  0x50,     0x00, 0xFF, 0x00,     // green
  0x30,     0x00, 0xFF, 0x00,     // green
  0x50,     0x00, 0x00, 0xFF,     // blue
  0x30,     0x00, 0x00, 0xFF,     // blue
  0x50,     0xFF, 0x00, 0xFF,     // indigo
  0x30,     0xFF, 0x00, 0xFF,     // indigo
  0x50,     0xFF, 0xFF, 0xFF,     // white
  0x30,     0xFF, 0xFF, 0xFF,     // white
  0x00,     0xFF, 0x00, 0x00,     // red
};

static const uint8_t PALETTE_STANDARD_TAG[] = {
  0x40,     0xFF, 0x00, 0x00,     // red
  0x40,     0xFF, 0xA5, 0x00,     // orange
  0x40,     0xFF, 0xFF, 0x00,     // yellow
  0x40,     0x00, 0xFF, 0x00,     // green
  0x40,     0x00, 0x00, 0xFF,     // blue
  0x40,     0xFF, 0x00, 0xFF,     // indigo
  0x40,     0xEE, 0x44, 0xA5,     // violet
  0x00,     0xFF, 0x00, 0x00,     // red
};

// Gradient palette "ib_jul01_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/ing/xmas/tn/ib_jul01.png.index.html
static const uint8_t PALETTE_ib_jul01_gp[] = {
  0x00,     0xE6, 0x06, 0x11,     // rgb(230,  6, 17)   0.000%,
  0x5E,     0x25, 0x60, 0x5A,     // rgb( 37, 96, 90)  37.010%,
  0x85,     0x90, 0xBD, 0x6A,     // rgb(144,189,106)  52.000%,
  0xFF,     0xBB, 0x03, 0x0D,     // rgb(187,  3, 13) 100.000%
};

// Gradient palette "ib44"
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/ing/general/tn/ib44.png.index.html
static const uint8_t PALETTE_ib_44[] = {
    0,     214,  24,  16,
   64,     227, 115,  78,
  255,     239, 206, 140,
};

// Gradient palette "Fire-1"
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/ds/icons/Fire-1.c3g
static const uint8_t PALETTE_Fire_1[] = {
    0,     255,   0,   0,
  128,     255, 128,   0,
  255,     255, 255,   0,
};

// Gradient palette "bhw1_sunconure"
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/bhw/bhw1/tn/bhw1_sunconure.png.index.html
static const uint8_t PALETTE_bhw1_sunconure[] = {
    0,      97, 240,  78,
  161,     246, 137,  30,
  255,     246,  45,  30,
};

// Gradient palette "bhw3_41"
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/bhw/bhw3/tn/bhw3_41.png.index.html
static const uint8_t PALETTE_bhw3_41[] = {
    0,       0,   0, 128,
   71,      64,  64, 255,
   75,     160, 160, 255,
   76,     255, 255, 255,
  178,     255, 255, 255,
  179,     255, 128, 128,
  196,     255,   0,   0,
  255,     128,   0,   0,
};

// Gradient palette "bhw4_089"
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/bhw/bhw4/tn/bhw4_089.png.index.html
static const uint8_t PALETTE_bhw4_089[] = {
    0,     174,  52,  28,
   28,     224, 154, 133,
   53,     235, 208, 206,
   79,     249, 208, 118,
  109,     228,  95,  50,
  132,     227, 165, 116,
  163,     226, 131,  67,
  184,     252, 213, 118,
  201,     252, 169, 125,
  224,     255, 194, 101,
  255,     215,  80,  35,
};

static const uint8_t PALETTE_STANDARD_VAL[] = {
  0x00,     0xFF, 0x00, 0x00,     // red
  0x2A,     0xFF, 0xA5, 0x00,     // orange
  0x55,     0xFF, 0xFF, 0x00,     // yellow
  0x7F,     0x00, 0xFF, 0x00,     // green
  0xAA,     0x00, 0x00, 0xFF,     // blue
  0xD4,     0xFF, 0x00, 0xFF,     // indigo
  0xFF,     0xFF, 0x00, 0x00,     // red
};

static const uint8_t PALETTE_SATURATED_TAG[] = {
  0x40,     0xFF, 0x00, 0x00,     // red
  0x40,     0xFF, 0xA5, 0x00,     // orange
  0x40,     0xFF, 0xFF, 0x00,     // yellow
  0x40,     0x00, 0xFF, 0x00,     // green
  0x40,     0x00, 0x00, 0xFF,     // blue
  0x40,     0xFF, 0x00, 0xFF,     // indigo
  0x00,     0xFF, 0x00, 0x00,     // red
};

extern const bclass be_class_Leds_frame;

#include "be_fixed_animate.h"

/* @const_object_info_begin

module animate (scope: global, strings: weak) {
  SAWTOOTH, int(1)
  TRIANGLE, int(2)
  SQUARE, int(3)
  COSINE, int(4)
  LASTFORM, int(5)

  PALETTE_STANDARD_TAG, comptr(PALETTE_STANDARD_TAG)
  PALETTE_RAINBOW_WHITE, comptr(PALETTE_RAINBOW_WHITE)
  PALETTE_STANDARD_VAL, comptr(PALETTE_STANDARD_VAL)
  PALETTE_SATURATED_TAG, comptr(PALETTE_SATURATED_TAG)
  PALETTE_ib_jul01_gp, comptr(PALETTE_ib_jul01_gp)
  PALETTE_ib_44, comptr(PALETTE_ib_44)
  PALETTE_Fire_1, comptr(PALETTE_Fire_1)
  PALETTE_bhw1_sunconure, comptr(PALETTE_bhw1_sunconure)
  PALETTE_bhw4_089, comptr(PALETTE_bhw4_089)

  core, class(be_class_Animate_core)
  animator, class(be_class_Animate_animator)
  frame, class(be_class_Leds_frame)
  pulse, class(be_class_Animate_pulse)
  palette, class(be_class_Animate_palette)
  oscillator, class(be_class_Animate_oscillator)
}

@const_object_info_end */

/* Unit test for palettes

import animate
var p, gradient
p = animate.palette.ptr_to_palette(animate.PALETTE_STANDARD_TAG)
assert(p == bytes('40FF000040FFA50040FFFF004000FF00400000FF40FF00FF40EE44A500FF0000'))
gradient = animate.palette.to_css_gradient(p)
assert(gradient == 'background:linear-gradient(to right,#FF0000 0.0%,#FFA500 14.3%,#FFFF00 28.6%,#00FF00 42.9%,#0000FF 57.1%,#FF00FF 71.4%,#EE44A5 85.7%,#FF0000 100.0%);')

p = animate.palette.ptr_to_palette(animate.PALETTE_STANDARD_VAL)
assert(p == bytes('00FF00002AFFA50055FFFF007F00FF00AA0000FFD4FF00FFFFFF0000'))
gradient = animate.palette.to_css_gradient(animate.PALETTE_STANDARD_VAL)
assert(gradient == 'background:linear-gradient(to right,#FF0000 0.0%,#FFA500 16.5%,#FFFF00 33.3%,#00FF00 49.8%,#0000FF 66.7%,#FF00FF 83.1%,#FF0000 100.0%);')


# unit tests
import animate
var o = animate.oscillator(-1000, 1000, 3000)
o.start(1000)
assert(o.value == -1000)
assert(o.animate(1500) == -667)
assert(o.animate(2500) == 0)
assert(o.animate(3999) == 1000)
assert(o.animate(4000) == -1000)
assert(o.animate(4100) == -933)

o = animate.oscillator(-1000, 1000, 6000, animate.TRIANGLE)
o.start(1000)
assert(o.value == -1000)
assert(o.animate(1500) == -667)
assert(o.animate(2500) == 0)
assert(o.animate(3999) == 1000)
assert(o.animate(4000) == 1000)
assert(o.animate(4100) == 933)
assert(o.animate(6000) == -334)
assert(o.animate(7000) == -1000)
assert(o.animate(7100) == -933)

o = animate.oscillator(-1000, 1000, 6000, animate.SQUARE)
o.start(1000)
assert(o.value == -1000)
assert(o.animate(1500) == -1000)
assert(o.animate(2500) == -1000)
assert(o.animate(3999) == -1000)
assert(o.animate(4000) == 1000)
assert(o.animate(4100) == 1000)
assert(o.animate(6000) == 1000)
assert(o.animate(7000) == -1000)
assert(o.animate(7100) == -1000)

o = animate.oscillator(-1000, 1000, 6000, animate.COSINE)
o.start(1000)
assert(o.animate(1000) == -1000)
assert(o.animate(1500) == -867)
assert(o.animate(2000) == -500)
assert(o.animate(2500) == 0)
assert(o.animate(4000) == 1000)
assert(o.animate(5500) == 0)
assert(o.animate(7000) == -1000)



*/

#endif // USE_BERRY
