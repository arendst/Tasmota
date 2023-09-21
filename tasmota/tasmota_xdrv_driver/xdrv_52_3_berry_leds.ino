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
            uint8_t w = (rgbw & 0xFF000000) >> 24;
            uint8_t r = (rgbw & 0xFF0000) >> 16;
            uint8_t g = (rgbw & 0xFF00) >> 8;
            uint8_t b = (rgbw & 0xFF);
            if (s_ws2812_grb)       s_ws2812_grb->ClearTo(RgbColor(r, g, b));
            if (s_sk6812_grbw)      s_sk6812_grbw->ClearTo(RgbwColor(r, g, b, w));
            }
            break;
          case 10: // # 10 : SetPixelColor (idx:int, color:??) -> void
            {
            int32_t idx = be_toint(vm, 3);
            uint32_t rgbw = be_toint(vm, 4);
            uint8_t w = (rgbw & 0xFF000000) >> 24;
            uint8_t r = (rgbw & 0xFF0000) >> 16;
            uint8_t g = (rgbw & 0xFF00) >> 8;
            uint8_t b = (rgbw & 0xFF);
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

#endif // USE_WS2812
#endif  // USE_BERRY
