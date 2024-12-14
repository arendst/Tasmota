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

#include "TasmotaLED.h"
#include "TasmotaLEDPusher.h"

/*********************************************************************************************\
 * Functions from Tasmota WS2812 driver
 *
\*********************************************************************************************/
extern void *Ws2812GetStrip(void);
extern void Ws2812Begin(void);
extern void Ws2812Show(void);
extern uint32_t Ws2812PixelsSize(void);
extern bool Ws2812CanShow(void);
extern bool Ws2812IsDirty(void);
extern void Ws2812Dirty(void);
extern uint8_t * Ws2812Pixels(void);
extern size_t Ws2812PixelSize(void);
extern size_t Ws2812PixelCount(void);
extern void Ws2812ClearTo(uint8_t r, uint8_t g, uint8_t b, uint8_t w, int32_t from, int32_t to);
extern void Ws2812SetPixelColor(uint32_t idx, uint8_t r, uint8_t g, uint8_t b, uint8_t w);
extern uint32_t Ws2812GetPixelColor(uint32_t idx);

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

  void * be_get_tasmotaled(bvm *vm) {
    be_getmember(vm, 1, "_p");
    void * strip = (void*) be_tocomptr(vm, -1);
    be_pop(vm, 1);
    return strip;
  }

  int be_tasmotaled_call_native(bvm *vm);
  int be_tasmotaled_call_native(bvm *vm) {
    int32_t argc = be_top(vm); // Get the number of arguments
    if (argc >= 2 && be_isint(vm, 2)) {
      int32_t cmd = be_toint(vm, 2);

      if (0 == cmd) { // 00 : ctor         (leds:int, gpio:int) -> void
        if ((argc != 2) && !(argc >= 5 && be_isint(vm, 3) && be_isint(vm, 4) && be_isint(vm, 5))) {
          be_raise(vm, "value_error", "bad arguments for tasmotaled:ctor");
        }
        int32_t leds = -1;
        int32_t gpio = -1;
        int32_t led_type = 0;
        int32_t hardware = 0;
        if (argc >= 6 && be_isint(vm, 6)) {
          hardware = be_toint(vm, 6) & 0xFF0000;    // remove the low 16 bits to avoid any interference with legacy parameter for RMT channels
        }
        TasmotaLED * strip = nullptr;
        if (argc > 2) {
          leds = be_toint(vm, 3);
          gpio = be_toint(vm, 4);
          led_type = be_toint(vm, 5);
        }

        if (-1 == gpio) {
          // if GPIO is '-1'
          led_type = 0;
          Ws2812InitStrip();          // ensure the tasmotaled object is initialized, because Berry code runs before the driver is initialized
        } else {
          if (led_type < 1) { led_type = 1; }
          TasmotaLEDPusher * pusher = TasmotaLEDPusher::Create(hardware, gpio);
          if (pusher == nullptr) {
            be_raise(vm, "value_error", "LED interface not supported");
          }
          strip = new TasmotaLED(led_type, leds);
          strip->SetPusher(pusher);
        }

        be_pushcomptr(vm, (void*) strip);   // if native driver, it is NULL
        be_setmember(vm, 1, "_p");
        be_pop(vm, 1);
        be_pushnil(vm);
      } else {
        // all other commands need a valid tasmotaled pointer
        TasmotaLED * strip = (TasmotaLED*) be_get_tasmotaled(vm);    // raises an exception if pointer is invalid
        // detect native driver means strip == nullptr
        be_pushnil(vm);     // push a default `nil` return value

        switch (cmd) {
          case 1: // # 01 : begin        void -> void
            if (strip)                strip->Begin();
            else                      Ws2812Begin();
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
            bool update_completed = false;
            if (strip) {
              strip->Show();
              pixels_size = strip->PixelCount() * strip->PixelSize();
              update_completed = strip->CanShow();
            } else {
              Ws2812Show();
              pixels_size = Ws2812PixelsSize();
              update_completed =Ws2812CanShow();
            }
            }
            break;
          case 3: // # 03 : CanShow      void -> bool
            if (strip)              be_pushbool(vm, strip->CanShow());
            else                    be_pushbool(vm, Ws2812CanShow());
            break;
          case 4: // # 04 : IsDirty      void -> bool
            if (strip)              be_pushbool(vm, strip->IsDirty());
            else                    be_pushbool(vm, Ws2812IsDirty());
            break;
          case 5: // # 05 : Dirty        void -> void
            if (strip)              strip->Dirty();
            else                    Ws2812Dirty();
            break;
          case 6: // # 06 : Pixels       void -> bytes() (mapped to the buffer)
            if (strip)              be_pushcomptr(vm, strip->Pixels());
            else                    be_pushcomptr(vm, Ws2812Pixels());
            break;
          case 7: // # 07 : PixelSize    void -> int
            if (strip)              be_pushint(vm, strip->PixelSize());
            else                    be_pushint(vm, Ws2812PixelSize());
            break;
          case 8: // # 08 : PixelCount   void -> int
            if (strip)             be_pushint(vm, strip->PixelCount());
            else                   be_pushint(vm, Ws2812PixelCount());
            break;
          case 9: // # 09 : ClearTo      (color:??) -> void
            {
            uint32_t rgbw = be_toint(vm, 3);
            uint8_t w = (rgbw >> 24) & 0xFF;
            uint8_t r = (rgbw >> 16) & 0xFF;
            uint8_t g = (rgbw >>  8) & 0xFF;
            uint8_t b = (rgbw      ) & 0xFF;
            if (argc >= 5 && be_isint(vm, 4) && be_isint(vm, 5)) {
              int32_t from = be_toint(vm, 4);
              int32_t len = be_toint(vm, 5);
              if (from < 0)          { from = 0; }
              if (len < 0)           { len = 0; }

              if (strip)              strip->ClearTo(rgbw, from, from + len - 1);
              else                    Ws2812ClearTo(r, g, b, w, from, from + len - 1);
            } else {
              if (strip)              strip->ClearTo(rgbw);
              else                    Ws2812ClearTo(r, g, b, w, -1, -1);
            }
            }
            break;
          case 10: // # 10 : SetPixelColor (idx:int, color:int wrgb) -> void
            {
            int32_t idx = be_toint(vm, 3);
            uint32_t wrgb = be_toint(vm, 4);
            if (strip) {
              strip->SetPixelColor(idx, wrgb);
            } else {
              uint8_t w = (wrgb >> 24) & 0xFF;
              uint8_t r = (wrgb >> 16) & 0xFF;
              uint8_t g = (wrgb >>  8) & 0xFF;
              uint8_t b = (wrgb      ) & 0xFF;
              Ws2812SetPixelColor(idx, r, g, b, w);
            }
            }
            break;
          case 11: // # 11 : GetPixelColor (idx:int) -> color:int wrgb
            {
            int32_t idx = be_toint(vm, 3);
            if (strip) {
              be_pushint(vm, strip->GetPixelColor(idx));
            } else {
              be_pushint(vm, Ws2812GetPixelColor(idx));
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

uint32_t ApplyBriGamma(uint32_t color_a /* 0xRRGGBB */, uint32_t bri /* 0..255 */, bool gamma) {
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
}

#endif // USE_WS2812
#endif  // USE_BERRY
