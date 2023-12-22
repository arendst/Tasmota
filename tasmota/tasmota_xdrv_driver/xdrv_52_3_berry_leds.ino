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

#include <NeoPixelBus.h>

enum {
  ws2812_grb = 1,
  sk6812_grbw = 2,

  neopixel_type_end
};

#ifdef CONFIG_IDF_TARGET_ESP32C2
typedef NeoPixelBus<NeoGrbFeature, NeoEsp32SpiN800KbpsMethod> neopixel_ws2812_grb_t;
typedef NeoPixelBus<NeoGrbwFeature, NeoEsp32SpiNSk6812Method> neopixel_sk6812_grbw_t;
#else
typedef NeoPixelBus<NeoGrbFeature, NeoEsp32RmtN800KbpsMethod> neopixel_ws2812_grb_t;
typedef NeoPixelBus<NeoGrbwFeature, NeoEsp32RmtNSk6812Method> neopixel_sk6812_grbw_t;
#endif


/*********************************************************************************************\
 * Native functions mapped to Berry functions
 *
 * import unishox
 *
 *
\*********************************************************************************************/
extern "C" {

  // # Native commands
  // # 00 : ctor
  // # 01 : begin        void -> void
  // # 02 : show         void -> void
  // # 03 : CanShow      void -> bool
  // # 04 : IsDirty      void -> bool
  // # 05 : Dirty        void -> void
  // # 06 : Pixels       void -> bytes() (mapped to the buffer)
  // # 07 : PixelSize    void -> int
  // # 08 : PixelCount   void -> int
  // # 09 : ClearTo      (color:??) -> void
  // # 10 : SetPixelColor (idx:int, color:??) -> void
  // # 11 : GetPixelColor (idx:int) -> color:??
  // # 20 : RotateLeft   (rot:int [, first:int, last:int]) -> void
  // # 21 : RotateRight  (rot:int [, first:int, last:int]) -> void
  // # 22 : ShiftLeft    (rot:int [, first:int, last:int]) -> void
  // # 23 : ShiftRight   (rot:int [, first:int, last:int]) -> void

  void * be_get_neopixelbus(bvm *vm) {
    be_getmember(vm, 1, "_p");
    void * strip = (void*) be_tocomptr(vm, -1);
    be_pop(vm, 1);
    if (strip == nullptr) {
      be_raise(vm, "internal_error", "neopixelbus object not initialized");
    }
    return strip;
  }
  int32_t be_get_leds_type(bvm *vm) {
    be_getmember(vm, 1, "_t");
    int32_t type = be_toint(vm, -1);
    be_pop(vm, 1);
    if (type <= 0 || type >= neopixel_type_end) {
      be_raise(vm, "internal_error", "invalid leds type");
    }
    return type;
  }

  int be_neopixelbus_call_native(bvm *vm);
  int be_neopixelbus_call_native(bvm *vm) {
    int32_t argc = be_top(vm); // Get the number of arguments
    if (argc >= 2 && be_isint(vm, 2)) {
      int32_t cmd = be_toint(vm, 2);

      if (0 == cmd) { // 00 : ctor         (leds:int, gpio:int) -> void
        if (!(argc >= 6 && be_isint(vm, 3) && be_isint(vm, 4) && be_isint(vm, 5) && be_isint(vm, 6))) {
          be_raise(vm, "value_error", "bad arguments for neopixelbus:ctor");
        }
        int32_t leds = be_toint(vm, 3);
        int32_t gpio = be_toint(vm, 4);
        int32_t neopixel_type = be_toint(vm, 5);
        int32_t rmt = be_toint(vm, 6);
        if (neopixel_type < 1) { neopixel_type = 1; }
        if (neopixel_type >= neopixel_type_end) { neopixel_type = neopixel_type_end - 1; }
        if (rmt < 0) { rmt = 0; }
        if (rmt >= MAX_RMT) { rmt = MAX_RMT - 1; }

        // store type in attribute `_t`
        be_pushint(vm, neopixel_type);
        be_setmember(vm, 1, "_t");
        be_pop(vm, 1);

        void * strip = nullptr;
        switch (neopixel_type) {
          case ws2812_grb:    strip = new neopixel_ws2812_grb_t(leds, gpio, (NeoBusChannel) rmt);
            break;
          case sk6812_grbw:   strip = new neopixel_sk6812_grbw_t(leds, gpio, (NeoBusChannel) rmt);
           break;
        }
        be_pushcomptr(vm, (void*) strip);
        be_setmember(vm, 1, "_p");
        be_pop(vm, 1);
        be_pushnil(vm);
      } else {
        // all other commands need a valid neopixelbus pointer
        int32_t leds_type = be_get_leds_type(vm);
        const void * s = be_get_neopixelbus(vm);    // raises an exception if pointer is invalid
        // initialize all possible variants
        neopixel_ws2812_grb_t * s_ws2812_grb = (leds_type == ws2812_grb) ? (neopixel_ws2812_grb_t*) s : nullptr;
        neopixel_sk6812_grbw_t * s_sk6812_grbw = (leds_type == sk6812_grbw) ? (neopixel_sk6812_grbw_t*) s : nullptr;

        be_pushnil(vm);     // push a default `nil` return value

        switch (cmd) {
          case 1: // # 01 : begin        void -> void
            if (s_ws2812_grb)       s_ws2812_grb->Begin();
            if (s_sk6812_grbw)      s_sk6812_grbw->Begin();
            break;
          case 2: // # 02 : show         void -> void
            {
            if (Settings->flag6.berry_light_scheme &&
                (1 == TasmotaGlobal.light_driver)) {  // XLGT_01
              // TODO: Need to add test for RMT0
#ifdef USE_NETWORK_LIGHT_SCHEMES
              bool scheme_berry = ((Light.max_scheme -1) == Settings->light_scheme);
#else
              bool scheme_berry = (Light.max_scheme == Settings->light_scheme);
#endif
              if (scheme_berry) {
                if (!Light.power) {
                  // Skip berry Show() as WS2812 driver Show() has powered off leds
                  break;
                }
              } else {
                // Skip berry Show() but use WS2812 driver Show() instead
                break;
              }
            }
            uint32_t pixels_size;       // number of bytes to push
            if (s_ws2812_grb)     { s_ws2812_grb->Show();   pixels_size = s_ws2812_grb->PixelsSize(); }
            if (s_sk6812_grbw)    { s_sk6812_grbw->Show();  pixels_size = s_sk6812_grbw->PixelsSize(); }

            // Wait for RMT/I2S to complete fixes distortion due to analogRead
            // 1ms is needed for 96 bytes
            SystemBusyDelay((pixels_size + 95) / 96);
            }
            break;
          case 3: // # 03 : CanShow      void -> bool
            if (s_ws2812_grb)       be_pushbool(vm, s_ws2812_grb->CanShow());
            if (s_sk6812_grbw)      be_pushbool(vm, s_sk6812_grbw->CanShow());
            break;
          case 4: // # 04 : IsDirty      void -> bool
            if (s_ws2812_grb)       be_pushbool(vm, s_ws2812_grb->IsDirty());
            if (s_sk6812_grbw)      be_pushbool(vm, s_sk6812_grbw->IsDirty());
            break;
          case 5: // # 05 : Dirty        void -> void
            if (s_ws2812_grb)       s_ws2812_grb->Dirty();
            if (s_sk6812_grbw)      s_sk6812_grbw->Dirty();
            break;
          case 6: // # 06 : Pixels       void -> bytes() (mapped to the buffer)
            {
            uint8_t * pixels;
            if (s_ws2812_grb)       pixels = s_ws2812_grb->Pixels();
            if (s_sk6812_grbw)      pixels = s_sk6812_grbw->Pixels();

            be_pushcomptr(vm, pixels);
            }
            break;
          case 7: // # 07 : PixelSize    void -> int
            if (s_ws2812_grb)       be_pushint(vm, s_ws2812_grb->PixelSize());
            if (s_sk6812_grbw)      be_pushint(vm, s_sk6812_grbw->PixelSize());
            break;
          case 8: // # 08 : PixelCount   void -> int
            if (s_ws2812_grb)       be_pushint(vm, s_ws2812_grb->PixelCount());
            if (s_sk6812_grbw)      be_pushint(vm, s_sk6812_grbw->PixelCount());
            break;
          case 9: // # 09 : ClearTo      (color:??) -> void
            {
            uint32_t rgbw = be_toint(vm, 3);
            uint8_t w = (rgbw >> 24) & 0xFF;
            uint8_t r = (rgbw >> 16) & 0xFF;
            uint8_t g = (rgbw >>  8) & 0xFF;
            uint8_t b = (rgbw      ) & 0xFF;
            if (argc >= 5 && be_isint(vm, 4) && be_isint(vm, 5)) {
              uint32_t from = be_toint(vm, 4);
              uint32_t len = be_toint(vm, 5);
              if (s_ws2812_grb)       s_ws2812_grb->ClearTo(RgbColor(r, g, b), from, from + len - 1);
              if (s_sk6812_grbw)      s_sk6812_grbw->ClearTo(RgbwColor(r, g, b, w), from, from + len - 1);
            } else {
              if (s_ws2812_grb)       s_ws2812_grb->ClearTo(RgbColor(r, g, b));
              if (s_sk6812_grbw)      s_sk6812_grbw->ClearTo(RgbwColor(r, g, b, w));
            }
            }
            break;
          case 10: // # 10 : SetPixelColor (idx:int, color:??) -> void
            {
            int32_t idx = be_toint(vm, 3);
            uint32_t rgbw = be_toint(vm, 4);
            uint8_t w = (rgbw >> 24) & 0xFF;
            uint8_t r = (rgbw >> 16) & 0xFF;
            uint8_t g = (rgbw >>  8) & 0xFF;
            uint8_t b = (rgbw      ) & 0xFF;
            if (s_ws2812_grb)       s_ws2812_grb->SetPixelColor(idx, RgbColor(r, g, b));
            if (s_sk6812_grbw)      s_sk6812_grbw->SetPixelColor(idx, RgbwColor(r, g, b, w));
            }
            break;
          case 11: // # 11 : GetPixelColor (idx:int) -> color:??
            {
            int32_t idx = be_toint(vm, 3);
            RgbColor rgb;

            if (s_ws2812_grb) {
              RgbColor rgb = s_ws2812_grb->GetPixelColor(idx);
              be_pushint(vm, (rgb.R << 16) | (rgb.G << 8) | rgb.B);
            }
            if (s_sk6812_grbw) {
              RgbwColor rgbw = s_sk6812_grbw->GetPixelColor(idx);
              be_pushint(vm, (rgbw.W << 24) | (rgb.R << 16) | (rgb.G << 8) | rgb.B);
            }
            }
            break;
          default:
            break;
        }
      }
      be_return(vm);
    }
    be_raise(vm, kTypeError, nullptr);
  }
}

extern "C" {
  // Leds.blend_color(color_a:int, color_b:int [, alpha:int]) -> color:int
  //
  // color_a is 0x..RRGGBB
  // color_b is 0xAARRGGBB, AA is transparency (00: opaque, FF: transparent)
  //    Note: the default is 00 considered opaque, so Transparency = 255 - alpha
  // alpha is 0..255, and optional. If present it overrides `AA` from color_b
  //    Note: alpha `00` is transparent, `FF` is opaque
  int32_t be_leds_blend_color(bvm *vm);
  int32_t be_leds_blend_color(bvm *vm) {
    int32_t top = be_top(vm); // Get the number of arguments
    if (top >= 2 && be_isint(vm, 1) && be_isint(vm, 2)) {
      uint32_t color_a = be_toint(vm, 1);
      uint32_t color_b = be_toint(vm, 2);
      uint32_t transpency = (color_b >> 24) & 0xFF;
      if (top >= 3 && be_isint(vm, 3)) {
        transpency = 255 - be_toint(vm, 3);
      }
      // remove any transparency
      color_a = color_a & 0xFFFFFF;
      color_b = color_b & 0xFFFFFF;

      if (transpency == 0) {     // color_b is opaque, return color_b
        be_pushint(vm, color_b);
        be_return(vm);
      }
      if (transpency >= 255) {  // color_b is transparent, return color_a
        be_pushint(vm, color_a);
        be_return(vm);
      }
      int32_t r = changeUIntScale(transpency, 0, 255, (color_b >> 16) & 0xFF, (color_a >> 16) & 0xFF);
      int32_t g = changeUIntScale(transpency, 0, 255, (color_b >>  8) & 0xFF, (color_a >>  8) & 0xFF);
      int32_t b = changeUIntScale(transpency, 0, 255, (color_b      ) & 0xFF, (color_a      ) & 0xFF);
      uint32_t rgb = (r << 16) | (g << 8) | b;
      be_pushint(vm, rgb);
      be_return(vm);
    }
    be_raise(vm, kTypeError, nullptr);
  }

  static uint32_t ApplyBriGamma(uint32_t color_a /* 0xRRGGBB */, uint32_t bri /* 0..255 */, bool gamma) {
    if (bri == 0) { return 0x000000; }              // if bri is zero, short-cut
    uint32_t r = (color_a >> 16) & 0xFF;
    uint32_t g = (color_a >>  8) & 0xFF;
    uint32_t b = (color_a      ) & 0xFF;

    if (bri < 255) {              // apply bri
      r = changeUIntScale(bri, 0, 255, 0, r);
      g = changeUIntScale(bri, 0, 255, 0, g);
      b = changeUIntScale(bri, 0, 255, 0, b);
    }

    if (gamma) {                  // apply gamma
      r = ledGamma(r);
      g = ledGamma(g);
      b = ledGamma(b);
    }
    uint32_t rgb = (r << 16) | (g << 8) | b;
    return rgb;
  }
  
  // Leds.apply_bri_gamma(color_rgb:int (0xRRGGBB) [bri:int (0..255), gamma:bool]) -> color:int (0xRRGGBB)
  //
  int32_t be_leds_apply_bri_gamma(bvm *vm);
  int32_t be_leds_apply_bri_gamma(bvm *vm) {
    int32_t top = be_top(vm); // Get the number of arguments
    if (top >= 1 && be_isint(vm, 1)) {
      uint32_t color_a = be_toint(vm, 1);
      uint32_t bri255 = 255;
      if (top >= 2 && be_isint(vm, 2)) {
        bri255 = be_toint(vm, 2);
      }
      bool gamma = false;
      if (top >= 3) {
        gamma = be_tobool(vm, 3);
      }

      uint32_t rgb = ApplyBriGamma(color_a, bri255, gamma);

      be_pushint(vm, rgb);
      be_return(vm);
    }
    be_raise(vm, kTypeError, nullptr);
  }

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
      uint32_t r3 = changeUIntScale(alpha, 0, 255, r2, r);
      uint32_t g3 = changeUIntScale(alpha, 0, 255, g2, g);
      uint32_t b3 = changeUIntScale(alpha, 0, 255, b2, b);
      uint32_t a3 = changeUIntScale(alpha, 0, 255, a2, a);
      uint32_t rgb = (a3 << 24) | (r3 << 16) | (g3 << 8) | b3;
      be_pushint(vm, rgb);
      be_return(vm);
    }
    be_raise(vm, kTypeError, nullptr);
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
            uint32_t dest_r_new = changeUIntScale(fore_alpha, 0, 255, fore_r, back_r);
            uint32_t dest_g_new = changeUIntScale(fore_alpha, 0, 255, fore_g, back_g);
            uint32_t dest_b_new = changeUIntScale(fore_alpha, 0, 255, fore_b, back_b);
            dest_rgb_new = (dest_r_new << 16) | (dest_g_new << 8) | dest_b_new;
          }
          dest[i] = dest_rgb_new;
        }
      }
      be_return_nil(vm);
    }
    be_raise(vm, kTypeError, nullptr);
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
    be_raise(vm, kTypeError, nullptr);
  }

  // Leds_frame.paste_pixels(neopixel:bytes(), led_buffer:bytes(), bri:int 0..100, gamma:bool)
  //
  // Copy from ARGB buffer to GRB
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
          dest_buf[i * 3 + 0] = src_g;
          dest_buf[i * 3 + 1] = src_r;
          dest_buf[i * 3 + 2] = src_b;
        }
      }
      be_return_nil(vm);
    }
    be_raise(vm, kTypeError, nullptr);
  }


}

#endif // USE_WS2812
#endif  // USE_BERRY
