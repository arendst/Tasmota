/*
  TasmotaLED.h - Lightweight implementation for adressable leds.

  Copyright (C) 2024  Stephan Hadinger

  This library is free software: you can redistribute it and/or modify
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

#ifndef __TASMOTALED_H__
#define __TASMOTALED_H__

enum TasmotaLEDTypesEncoding : uint16_t {
  // bits 0..3 encode for number of bytes per pixel
  TasmotaLed_1_Def  = 0x0,      // Default value - identical to TasmotaLed_3_RGB
  TasmotaLed_3_RGB  = 0x1,      // 3 bytes per pixel
  TasmotaLed_4_WRGB = 0x2,      // 4 bytes per pixel
  // bits 4..6 encode for pixel order
  TasmotaLed_Def = 0b000 << 4,  // Default value - identical to TasmotaLed_GRB
  TasmotaLed_GRB = 0b001 << 4,
  TasmotaLed_RGB = 0b010 << 4,
  TasmotaLed_RBG = 0b011 << 4,
  TasmotaLed_BRG = 0b100 << 4,
  TasmotaLed_BGR = 0b101 << 4,
  TasmotaLed_GBR = 0b110 << 4,
  // bit 7 sets the position for W channel
  TasmotaLed_xxxW = 0b0 << 7,   // W channel after color
  TasmotaLed_Wxxx = 0b1 << 7,   // W channel before color
  // bits 8..15 encode for timing specifics
  TasmotaLed_WS2812 = 0 << 8,
  TasmotaLed_SK6812 = 1 << 8,
  TasmotaLed_TimingEnd = 2 << 8,
};

enum TasmotaLEDHardware : uint32_t {
  // low-order bits are reserved for channels numbers and hardware flags - currenlty not useds
  // bits 16..23
  TasmotaLed_HW_Default = 0x000000,
  TasmotaLed_RMT  = (1 << 0) << 16,
  TasmotaLed_SPI  = (1 << 1) << 16,
  TasmotaLed_I2S  = (1 << 2) << 16,
  TasmotaLed_HW_None = 0xFF << 16,    // indicates that the specified HW is not supported
};

// Below is the encoding for full strips
// We need to keep backwards compatibility so:
// 0 = WS2812 (GRB)
// 1 = SK6812 with White (GRBW)
enum TasmotaLEDTypes : uint16_t {
  ws2812_grb  = TasmotaLed_3_RGB  | TasmotaLed_GRB | TasmotaLed_WS2812,                     // 1 for backwards compatibility
  sk6812_grbw = TasmotaLed_4_WRGB | TasmotaLed_GRB | TasmotaLed_xxxW | TasmotaLed_SK6812,   // 2 for backwards compatibility
  sk6812_grb  = TasmotaLed_3_RGB  | TasmotaLed_GRB | TasmotaLed_SK6812,
};

#ifdef __cplusplus

/*******************************************************************************************\
 * class TasmotaLED
 * 
 * This class is a lightweight replacement for NeoPixelBus library with a smaller
 * implementation focusing only on pushing a buffer to the leds.
 * 
 * It supports:
 *   - RMT and I2S hardware support
 *     Possible enhancements could be considered with SPI and Serial
 *   - Led size of 3 bytes (GRB) and 4 bytes (GRBW)
 *     APIs take 0xRRGGBB or 0xRRGGBBWW as input
 *     but Internal buffers use GGRRBB and GGRRBBWW
 *   - Led type of WS2812 and SK6812
 *   - There is no buffer swapping, the working buffer is copied to an internal
 *     buffer just before display, so you can keep a reference to the buffer
 *     and modify it without having to worry about the display
 *   - buffer is cleared at start
 *   - "Dirty" is kept for API compatibility with NeoPixelBus but is glbally ignored
 *     so any call to `Show()` pushes the pixels even if they haven't changed.
 *     Control for dirty pixels should be done by the caller if required.
 *   - We tried to keep as close as possible to NeoPixelBus method names to ease transition
\*******************************************************************************************/
class TasmotaLEDPusher;   // forward definition
class TasmotaLED {
public:
  TasmotaLED(uint16_t type, uint16_t num_leds);
  ~TasmotaLED();

  void SetPixelCount(uint16_t num_leds);
  void SetPixelSubType(uint8_t type);         // change only Pixel order and pixel size
  void _adjustSubType(void);

  bool Begin(void);
  void SetPusher(TasmotaLEDPusher *pusher);   // needs to be called before `Begin()`, sets the hardware implementation
  void Show(void);                            // pushes the pixels to the LED strip
  inline void SetRawFormat(bool raw_format) { _raw_format = raw_format; }

  void ClearTo(uint32_t rgbw, int32_t first = 0, int32_t last = -1);
  void SetPixelColor(int32_t index, uint32_t wrgb);
  uint32_t GetPixelColor(int32_t index);

  uint8_t GetType(void) const { return _type; }
  uint16_t PixelCount(void) const { return _pixel_count; }
  uint8_t PixelSize(void) const { return _pixel_size; }
  inline uint8_t * Pixels(void) const { return _buf_work; }
  inline bool IsDirty(void) const { return _dirty; }
  inline void Dirty(void) { _dirty = true; }
  
  bool CanShow(void) const;

protected:
  uint16_t _type;                     // the composite type
  uint8_t _pixel_order;               // permutation between RGB and position of W
  bool _w_before;                     // true if W channel comes first (4 channels only)
  uint8_t _timing;                    // timing code for strip, 0=WS2812, 1=SK6812...
  bool _started;                      // true if the hardware implementation is configured
  bool _dirty;                        // for NeoPixelBus compatibility, but ignored by `Push()`
  bool _raw_format;                   // if true, copy raw to leds, if false, convert from RGB to GRB or LED format
  uint16_t _pixel_count;              // how many pixels in the strip
  uint8_t _pixel_size;                // how many bytes per pixels, only 3 and 4 are supported
  uint8_t *_buf_work;                 // buffer used to draw into, can be modified directly by the caller
  uint8_t *_buf_show;                 // copy of the buffer used to push to leds, private to this class
  const uint8_t (*_pixel_matrix)[3];  // pointer to the pixer_order_matrix
  TasmotaLEDPusher *_pusher;          // pixels pusher implementation based on hardware (RMT, I2S...)
};

#endif  // __cplusplus
#endif  // __TASMOTALED_H__
