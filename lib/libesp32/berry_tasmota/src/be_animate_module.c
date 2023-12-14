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

#include "be_constobj.h"
#include "be_mapping.h"

#include "solidify/solidified_animate_0_core.h"
#include "solidify/solidified_animate_1_animate_effects.h"
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
  0x40,     0xFF, 0xFF, 0xFF,     // violet
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

static const uint8_t PALETTE_STANDARD_VAL[] = {
  0x00,     0xFF, 0x00, 0x00,     // red
  0x24,     0xFF, 0xA5, 0x00,     // orange
  0x49,     0xFF, 0xFF, 0x00,     // yellow
  0x6E,     0x00, 0xFF, 0x00,     // green
  0x92,     0x00, 0x00, 0xFF,     // blue
  0xB7,     0xFF, 0x00, 0xFF,     // indigo
  0xDB,     0xFF, 0xFF, 0xFF,     // violet
  0xFF,     0xFF, 0x00, 0x00,     // red
};

static const uint8_t PALETTE_SATURATED_TAG[] = {
  0x40,     0xFF, 0x00, 0x00,     // red
  0x40,     0xFF, 0xA5, 0x00,     // orange
  0x40,     0xFF, 0xFF, 0x00,     // yellow
  0x40,     0x00, 0xFF, 0x00,     // green
  0x40,     0x00, 0x00, 0xFF,     // blue
  0x40,     0xFF, 0x00, 0xFF,     // indigo
  0x40,     0xFF, 0xFF, 0xFF,     // violet
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
  SINE, int(5)
  LASTFORM, int(5)

  PALETTE_STANDARD_TAG, comptr(PALETTE_STANDARD_TAG)
  PALETTE_RAINBOW_WHITE, comptr(PALETTE_RAINBOW_WHITE)
  PALETTE_STANDARD_VAL, comptr(PALETTE_STANDARD_VAL)
  PALETTE_SATURATED_TAG, comptr(PALETTE_SATURATED_TAG)

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
assert(p == bytes('40FF000040FFA50040FFFF004000FF00400000FF40FF00FF40FFFFFF00FF0000'))
gradient = animate.palette.to_css_gradient(p)
assert(gradient == 'background:linear-gradient(to right,#FF0000 0.0%,#FFA500 14.3%,#FFFF00 28.6%,#00FF00 42.9%,#0000FF 57.1%,#FF00FF 71.4%,#FFFFFF 85.7%,#FF0000 100.0%);')

p = animate.palette.ptr_to_palette(animate.PALETTE_STANDARD_VAL)
assert(p == bytes('00FF000024FFA50049FFFF006E00FF00920000FFB7FF00FFDBFFFFFFFFFF0000'))
gradient = animate.palette.to_css_gradient(animate.PALETTE_STANDARD_VAL)
assert(gradient == 'background:linear-gradient(to right,#FF0000 0.0%,#FFA500 14.1%,#FFFF00 28.6%,#00FF00 43.1%,#0000FF 57.3%,#FF00FF 71.8%,#FFFFFF 85.9%,#FF0000 100.0%);')


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

o = animate.oscillator(-1000, 1000, 6000, animate.SINE)
o.start(1000)
assert(o.animate(1000) == 0)
assert(o.animate(1500) == 500)
assert(o.animate(2000) == 867)
assert(o.animate(2500) == 1000)
assert(o.animate(4000) == 0)
assert(o.animate(5500) == -1000)
assert(o.animate(7000) == 0)

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