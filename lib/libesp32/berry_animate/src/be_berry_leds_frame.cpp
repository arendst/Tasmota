/*
  xdrv_52_3_berry_leds.ino - Berry scripting language, native fucnctions

  Copyright (C) 2021 Stephan Hadinger, Berry language by Guan Wenliang https://github.com/Skiars/berry

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


#ifdef USE_BERRY

#include <berry.h>

#ifdef USE_WS2812

extern uint16_t changeUIntScale(uint16_t inum, uint16_t ifrom_min, uint16_t ifrom_max,uint16_t ito_min, uint16_t ito_max);
extern uint32_t ApplyBriGamma(uint32_t color_a /* 0xRRGGBB */, uint32_t bri /* 0..255 */, bool gamma);

extern "C" {
  // Leds_frame.blend(color1:int, color2:int, alpha:int) -> int
  //
  int32_t be_leds_blend(bvm *vm);
  int32_t be_leds_blend(bvm *vm) {
    int32_t top = be_top(vm); // Get the number of arguments
    if (top >= 3 && be_isint(vm, 1) && be_isint(vm, 2) && be_isint(vm, 3)) {
      uint32_t color_a = be_toint(vm, 1);
      uint32_t color_b = be_toint(vm, 2);
      uint32_t alpha = be_toint(vm, 3);
      uint32_t r = (color_a >> 16) & 0xFF;
      uint32_t g = (color_a >>  8) & 0xFF;
      uint32_t b = (color_a      ) & 0xFF;
      uint32_t a = (color_a >> 24) & 0xFF;
      uint32_t r2 = (color_b >> 16) & 0xFF;
      uint32_t g2 = (color_b >>  8) & 0xFF;
      uint32_t b2 = (color_b      ) & 0xFF;
      uint32_t a2 = (color_b >> 24) & 0xFF;
      uint8_t r3 = changeUIntScale(alpha, 0, 255, r2, r);
      uint8_t g3 = changeUIntScale(alpha, 0, 255, g2, g);
      uint8_t b3 = changeUIntScale(alpha, 0, 255, b2, b);
      uint8_t a3 = changeUIntScale(alpha, 0, 255, a2, a);
      uint32_t rgb = (a3 << 24) | (r3 << 16) | (g3 << 8) | b3;
      be_pushint(vm, rgb);
      be_return(vm);
    }
    be_raise(vm, "type_error", nullptr);
  }

  // Leds_frame.blend_pixels(dest:bytes(), foreground:bytes) -> nil
  // Destination can be the same as foreground or background
  //
  // All calculation are done in `0xAARRGGBB` format, AA=0 if opaque (i.e. ignored)
  // Background has always alpha = 0 (any other value is ignored) - for simplification
  // Size is truncated to smallest of all 3 buffers
  int32_t be_leds_blend_pixels(bvm *vm);
  int32_t be_leds_blend_pixels(bvm *vm) {
    int32_t top = be_top(vm); // Get the number of arguments
    if (top >= 2 && be_isbytes(vm, 2)) {
      size_t dest_len = 0;
      uint32_t * dest_buf = (uint32_t*) be_tobytes(vm, 1, &dest_len);
      // back = dest for now, could be changed in the future
      size_t back_len = 0;
      const uint32_t * back_buf = (const uint32_t*) be_tobytes(vm, 1, &back_len);
      size_t fore_len = 0;
      const uint32_t * fore_buf = (const uint32_t*) be_tobytes(vm, 2, &fore_len);

      if (fore_len < dest_len) { dest_len = fore_len; }
      if (back_len < dest_len) { dest_len = back_len; }
      size_t pixels_count = dest_len / 4;
      if (pixels_count > 0) {
        uint32_t * dest = (uint32_t *)dest_buf;
        uint32_t * back = (uint32_t *)back_buf;
        uint32_t * fore = (uint32_t *)fore_buf;
        for (size_t i = 0; i < pixels_count; i++) {
          uint32_t back_argb = back[i];
          uint32_t fore_argb = fore[i];
          uint32_t fore_alpha = (fore_argb >> 24) & 0xFF;
          uint32_t dest_rgb_new = back_argb;
          if (fore_alpha == 0) {        // opaque layer, copy value from fore
            dest_rgb_new = fore_argb;
          } else if (fore_alpha == 255) {   // fore is transparent, use back
            // nothing to do, dest_rgb_new = back_argb above
          } else {
            uint32_t back_r = (back_argb >> 16) & 0xFF;
            uint32_t fore_r = (fore_argb >> 16) & 0xFF;
            uint32_t back_g = (back_argb >>  8) & 0xFF;
            uint32_t fore_g = (fore_argb >>  8) & 0xFF;
            uint32_t back_b = (back_argb      ) & 0xFF;
            uint32_t fore_b = (fore_argb      ) & 0xFF;
            uint8_t dest_r_new = changeUIntScale(fore_alpha, 0, 255, fore_r, back_r);
            uint8_t dest_g_new = changeUIntScale(fore_alpha, 0, 255, fore_g, back_g);
            uint8_t dest_b_new = changeUIntScale(fore_alpha, 0, 255, fore_b, back_b);
            dest_rgb_new = (dest_r_new << 16) | (dest_g_new << 8) | dest_b_new;
          }
          dest[i] = dest_rgb_new;
        }
      }
      be_return_nil(vm);
    }
    be_raise(vm, "type_error", nullptr);
  }

  // Leds_frame.fill_pixels(dest:bytes(), color:int) -> nil
  //
  // Fill buffer with same color
  int32_t be_leds_fill_pixels(bvm *vm);
  int32_t be_leds_fill_pixels(bvm *vm) {
    int32_t top = be_top(vm); // Get the number of arguments
    if (top >= 2 && be_isint(vm, 2)) {
      size_t dest_len = 0;
      uint32_t * dest_buf = (uint32_t*) be_tobytes(vm, 1, &dest_len);
      uint32_t color = be_toint(vm, 2);

      size_t pixels_count = dest_len / 4;
      if (pixels_count > 0) {
        uint32_t * dest = (uint32_t *)dest_buf;
        for (size_t i = 0; i < pixels_count; i++) {
          dest[i] = color;
        }
      }
      be_return_nil(vm);
    }
    be_raise(vm, "type_error", nullptr);
  }

  // Leds_frame.paste_pixels(neopixel:bytes(), led_buffer:bytes(), bri:int 0..100, gamma:bool)
  //
  // Copy from ARGB buffer to RGB
  int32_t be_leds_paste_pixels(bvm *vm);
  int32_t be_leds_paste_pixels(bvm *vm) {
    int32_t top = be_top(vm); // Get the number of arguments
    if (top >= 2 && be_isbytes(vm, 2)) {
      size_t src_len = 0;
      uint32_t * src_buf = (uint32_t*) be_tobytes(vm, 1, &src_len);
      size_t dest_len = 0;
      uint8_t * dest_buf = (uint8_t*) be_tobytes(vm, 2, &dest_len);

      uint32_t bri255 = 255;
      if (top >= 3 && be_isint(vm, 3)) {
        bri255 = be_toint(vm, 3);
      }
      bool gamma = false;
      if (top >= 4 && be_isbool(vm, 4)) {
        gamma = be_tobool(vm, 4);
      }

      size_t pixels_count = src_len / 4;
      if (pixels_count > dest_len / 3) { pixels_count = dest_len / 3; }
      if (pixels_count > 0) {
        for (size_t i = 0; i < pixels_count; i++) {
          uint32_t src_argb = ApplyBriGamma(src_buf[i], bri255, gamma);
          uint32_t src_r = (src_argb >> 16) & 0xFF;
          uint32_t src_g = (src_argb >>  8) & 0xFF;
          uint32_t src_b = (src_argb      ) & 0xFF;
          dest_buf[i * 3 + 0] = src_r;
          dest_buf[i * 3 + 1] = src_g;
          dest_buf[i * 3 + 2] = src_b;
        }
      }
      be_return_nil(vm);
    }
    be_raise(vm, "type_error", nullptr);
  }


}

#endif // USE_WS2812
#endif  // USE_BERRY
