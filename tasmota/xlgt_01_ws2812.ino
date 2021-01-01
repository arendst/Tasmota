/*
  xlgt_01_ws2812.ino - led string support for Tasmota

  Copyright (C) 2021  Theo Arends

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

#ifdef USE_LIGHT
#ifdef USE_WS2812
/*********************************************************************************************\
 * WS2812 RGB / RGBW Leds using NeopixelBus library
 *
 * light_scheme  WS2812  3+ Colors  1+2 Colors  Effect
 * ------------  ------  ---------  ----------  -----------------
 *  0            yes     no         no          Clock
 *  1            yes     no         no          Incandescent
 *  2            yes     no         no          RGB
 *  3            yes     no         no          Christmas
 *  4            yes     no         no          Hanukkah
 *  5            yes     no         no          Kwanzaa
 *  6            yes     no         no          Rainbow
 *  7            yes     no         no          Fire
 *
\*********************************************************************************************/

#define XLGT_01             1

const uint8_t WS2812_SCHEMES = 8;      // Number of WS2812 schemes

const char kWs2812Commands[] PROGMEM = "|"  // No prefix
  D_CMND_LED "|" D_CMND_PIXELS "|" D_CMND_ROTATION "|" D_CMND_WIDTH ;

void (* const Ws2812Command[])(void) PROGMEM = {
  &CmndLed, &CmndPixels, &CmndRotation, &CmndWidth };

#include <NeoPixelBus.h>

#if (USE_WS2812_HARDWARE == NEO_HW_P9813)
  typedef P9813BgrFeature selectedNeoFeatureType;
  #undef USE_WS2812_DMA
  #undef USE_WS2812_INVERTED
#elif (USE_WS2812_CTYPE == NEO_GRB)
  typedef NeoGrbFeature selectedNeoFeatureType;
#elif (USE_WS2812_CTYPE == NEO_BRG)
  typedef NeoBrgFeature selectedNeoFeatureType;
#elif (USE_WS2812_CTYPE == NEO_RBG)
  typedef NeoRbgFeature selectedNeoFeatureType;
#elif (USE_WS2812_CTYPE == NEO_RGBW)
  typedef NeoRgbwFeature selectedNeoFeatureType;
#elif (USE_WS2812_CTYPE == NEO_GRBW)
  typedef NeoGrbwFeature selectedNeoFeatureType;
#else   // USE_WS2812_CTYPE
  typedef NeoRgbFeature selectedNeoFeatureType;
#endif  // USE_WS2812_CTYPE

#ifdef USE_WS2812_DMA

#ifdef USE_WS2812_INVERTED
// See NeoEspDmaMethod.h for available options

#if (USE_WS2812_HARDWARE == NEO_HW_WS2812X)
  typedef NeoEsp8266DmaInvertedWs2812xMethod selectedNeoSpeedType;
#elif (USE_WS2812_HARDWARE == NEO_HW_SK6812)
  typedef NeoEsp8266DmaInvertedSk6812Method selectedNeoSpeedType;
#elif (USE_WS2812_HARDWARE == NEO_HW_APA106)
  typedef NeoEsp8266DmaInvertedApa106Method selectedNeoSpeedType;
#else   // USE_WS2812_HARDWARE
  typedef NeoEsp8266DmaInverted800KbpsMethod selectedNeoSpeedType;
#endif  // USE_WS2812_HARDWARE

#else  // No USE_WS2812_INVERTED

#if (USE_WS2812_HARDWARE == NEO_HW_WS2812X)
  typedef NeoEsp8266DmaWs2812xMethod selectedNeoSpeedType;
#elif (USE_WS2812_HARDWARE == NEO_HW_SK6812)
  typedef NeoEsp8266DmaSk6812Method selectedNeoSpeedType;
#elif (USE_WS2812_HARDWARE == NEO_HW_APA106)
  typedef NeoEsp8266DmaApa106Method selectedNeoSpeedType;
#else   // USE_WS2812_HARDWARE
  typedef NeoEsp8266Dma800KbpsMethod selectedNeoSpeedType;
#endif  // USE_WS2812_HARDWARE

#endif  // No USE_WS2812_INVERTED

#else   // No USE_WS2812_DMA

#ifdef USE_WS2812_INVERTED
// See NeoEspBitBangMethod.h for available options

#if (USE_WS2812_HARDWARE == NEO_HW_WS2812X)
  typedef NeoEsp8266BitBangWs2812xInvertedMethod selectedNeoSpeedType;
#elif (USE_WS2812_HARDWARE == NEO_HW_SK6812)
  typedef NeoEsp8266BitBangSk6812InvertedMethod selectedNeoSpeedType;
#else   // USE_WS2812_HARDWARE
  typedef NeoEsp8266BitBang400KbpsInvertedMethod selectedNeoSpeedType;
#endif  // USE_WS2812_HARDWARE

#else  // No USE_WS2812_INVERTED

#if (USE_WS2812_HARDWARE == NEO_HW_P9813)
  typedef P9813Method selectedNeoSpeedType;
#elif (USE_WS2812_HARDWARE == NEO_HW_WS2812X)
  typedef NeoEsp8266BitBangWs2812xMethod selectedNeoSpeedType;
#elif (USE_WS2812_HARDWARE == NEO_HW_SK6812)
  typedef NeoEsp8266BitBangSk6812Method selectedNeoSpeedType;
#else   // USE_WS2812_HARDWARE
  typedef NeoEsp8266BitBang800KbpsMethod selectedNeoSpeedType;
#endif  // USE_WS2812_HARDWARE

#endif  // No USE_WS2812_INVERTED

#endif  // No USE_WS2812_DMA

NeoPixelBus<selectedNeoFeatureType, selectedNeoSpeedType> *strip = nullptr;

struct WsColor {
  uint8_t red, green, blue;
};

struct ColorScheme {
  WsColor* colors;
  uint8_t count;
};

WsColor kIncandescent[2] = { 255,140,20, 0,0,0 };
WsColor kRgb[3] = { 255,0,0, 0,255,0, 0,0,255 };
WsColor kChristmas[2] = { 255,0,0, 0,255,0 };
WsColor kHanukkah[2] = { 0,0,255, 255,255,255 };
WsColor kwanzaa[3] = { 255,0,0, 0,0,0, 0,255,0 };
WsColor kRainbow[7] = { 255,0,0, 255,128,0, 255,255,0, 0,255,0, 0,0,255, 128,0,255, 255,0,255 };
WsColor kFire[3] = { 255,0,0, 255,102,0, 255,192,0 };
ColorScheme kSchemes[WS2812_SCHEMES -1] = {  // Skip clock scheme
  kIncandescent, 2,
  kRgb, 3,
  kChristmas, 2,
  kHanukkah, 2,
  kwanzaa, 3,
  kRainbow, 7,
  kFire, 3 };

uint8_t kWidth[5] = {
    1,     // Small
    2,     // Medium
    4,     // Large
    8,     // Largest
  255 };   // All
uint8_t kWsRepeat[5] = {
    8,     // Small
    6,     // Medium
    4,     // Large
    2,     // Largest
    1 };   // All

struct WS2812 {
  uint8_t show_next = 1;
  uint8_t scheme_offset = 0;
  bool suspend_update = false;
} Ws2812;

/********************************************************************************************/

void Ws2812StripShow(void)
{
#if (USE_WS2812_CTYPE > NEO_3LED)
  RgbwColor c;
#else
  RgbColor c;
#endif

  if (Settings.light_correction) {
    for (uint32_t i = 0; i < Settings.light_pixels; i++) {
      c = strip->GetPixelColor(i);
      c.R = ledGamma(c.R);
      c.G = ledGamma(c.G);
      c.B = ledGamma(c.B);
#if (USE_WS2812_CTYPE > NEO_3LED)
      c.W = ledGamma(c.W);
#endif
      strip->SetPixelColor(i, c);
    }
  }
  strip->Show();
}

int mod(int a, int b)
{
   int ret = a % b;
   if (ret < 0) ret += b;
   return ret;
}

void Ws2812UpdatePixelColor(int position, struct WsColor hand_color, float offset)
{
#if (USE_WS2812_CTYPE > NEO_3LED)
  RgbwColor color;
#else
  RgbColor color;
#endif

  uint32_t mod_position = mod(position, (int)Settings.light_pixels);

  color = strip->GetPixelColor(mod_position);
  float dimmer = 100 / (float)Settings.light_dimmer;
  color.R = tmin(color.R + ((hand_color.red / dimmer) * offset), 255);
  color.G = tmin(color.G + ((hand_color.green / dimmer) * offset), 255);
  color.B = tmin(color.B + ((hand_color.blue / dimmer) * offset), 255);
  strip->SetPixelColor(mod_position, color);
}

void Ws2812UpdateHand(int position, uint32_t index)
{
  uint32_t width = Settings.light_width;
  if (index < WS_MARKER) { width = Settings.ws_width[index]; }
  if (!width) { return; }  // Skip

  position = (position + Settings.light_rotation) % Settings.light_pixels;

  if (Settings.flag.ws_clock_reverse) {  // SetOption16 - Switch between clockwise or counter-clockwise
    position = Settings.light_pixels -position;
  }
  WsColor hand_color = { Settings.ws_color[index][WS_RED], Settings.ws_color[index][WS_GREEN], Settings.ws_color[index][WS_BLUE] };

  Ws2812UpdatePixelColor(position, hand_color, 1);

  uint32_t range = ((width -1) / 2) +1;
  for (uint32_t h = 1; h < range; h++) {
    float offset = (float)(range - h) / (float)range;
    Ws2812UpdatePixelColor(position -h, hand_color, offset);
    Ws2812UpdatePixelColor(position +h, hand_color, offset);
  }
}

void Ws2812Clock(void)
{
  strip->ClearTo(0); // Reset strip
  int clksize = 60000 / (int)Settings.light_pixels;

  Ws2812UpdateHand((RtcTime.second * 1000) / clksize, WS_SECOND);
  Ws2812UpdateHand((RtcTime.minute * 1000) / clksize, WS_MINUTE);
  Ws2812UpdateHand((((RtcTime.hour % 12) * 5000) + ((RtcTime.minute * 1000) / 12 )) / clksize, WS_HOUR);
  if (Settings.ws_color[WS_MARKER][WS_RED] + Settings.ws_color[WS_MARKER][WS_GREEN] + Settings.ws_color[WS_MARKER][WS_BLUE]) {
    for (uint32_t i = 0; i < 12; i++) {
      Ws2812UpdateHand((i * 5000) / clksize, WS_MARKER);
    }
  }

  Ws2812StripShow();
}

void Ws2812GradientColor(uint32_t schemenr, struct WsColor* mColor, uint32_t range, uint32_t gradRange, uint32_t i)
{
/*
 * Compute the color of a pixel at position i using a gradient of the color scheme.
 * This function is used internally by the gradient function.
 */
  ColorScheme scheme = kSchemes[schemenr];
  uint32_t curRange = i / range;
  uint32_t rangeIndex = i % range;
  uint32_t colorIndex = rangeIndex / gradRange;
  uint32_t start = colorIndex;
  uint32_t end = colorIndex +1;
  if (curRange % 2 != 0) {
    start = (scheme.count -1) - start;
    end = (scheme.count -1) - end;
  }
  float dimmer = 100 / (float)Settings.light_dimmer;
  float fmyRed = (float)map(rangeIndex % gradRange, 0, gradRange, scheme.colors[start].red, scheme.colors[end].red) / dimmer;
  float fmyGrn = (float)map(rangeIndex % gradRange, 0, gradRange, scheme.colors[start].green, scheme.colors[end].green) / dimmer;
  float fmyBlu = (float)map(rangeIndex % gradRange, 0, gradRange, scheme.colors[start].blue, scheme.colors[end].blue) / dimmer;
  mColor->red = (uint8_t)fmyRed;
  mColor->green = (uint8_t)fmyGrn;
  mColor->blue = (uint8_t)fmyBlu;
}

void Ws2812Gradient(uint32_t schemenr)
{
/*
 * This routine courtesy Tony DiCola (Adafruit)
 * Display a gradient of colors for the current color scheme.
 *  Repeat is the number of repetitions of the gradient (pick a multiple of 2 for smooth looping of the gradient).
 */
#if (USE_WS2812_CTYPE > NEO_3LED)
  RgbwColor c;
  c.W = 0;
#else
  RgbColor c;
#endif

  ColorScheme scheme = kSchemes[schemenr];
  if (scheme.count < 2) { return; }

  uint32_t repeat = kWsRepeat[Settings.light_width];  // number of scheme.count per ledcount
  uint32_t range = (uint32_t)ceil((float)Settings.light_pixels / (float)repeat);
  uint32_t gradRange = (uint32_t)ceil((float)range / (float)(scheme.count - 1));
  uint32_t speed = ((Settings.light_speed * 2) -1) * (STATES / 10);
  uint32_t offset = speed > 0 ? Light.strip_timer_counter / speed : 0;

  WsColor oldColor, currentColor;
  Ws2812GradientColor(schemenr, &oldColor, range, gradRange, offset);
  currentColor = oldColor;
  speed = speed ? speed : 1;    // should never happen, just avoid div0
  for (uint32_t i = 0; i < Settings.light_pixels; i++) {
    if (kWsRepeat[Settings.light_width] > 1) {
      Ws2812GradientColor(schemenr, &currentColor, range, gradRange, i + offset + 1);
    }
    // Blend old and current color based on time for smooth movement.
    c.R = map(Light.strip_timer_counter % speed, 0, speed, oldColor.red, currentColor.red);
    c.G = map(Light.strip_timer_counter % speed, 0, speed, oldColor.green, currentColor.green);
    c.B = map(Light.strip_timer_counter % speed, 0, speed, oldColor.blue, currentColor.blue);
    strip->SetPixelColor(i, c);
    oldColor = currentColor;
  }
  Ws2812StripShow();
}

void Ws2812Bars(uint32_t schemenr)
{
/*
 * This routine courtesy Tony DiCola (Adafruit)
 * Display solid bars of color for the current color scheme.
 * Width is the width of each bar in pixels/lights.
 */
#if (USE_WS2812_CTYPE > NEO_3LED)
  RgbwColor c;
  c.W = 0;
#else
  RgbColor c;
#endif

  ColorScheme scheme = kSchemes[schemenr];

  uint32_t maxSize = Settings.light_pixels / scheme.count;
  if (kWidth[Settings.light_width] > maxSize) { maxSize = 0; }

  uint32_t speed = ((Settings.light_speed * 2) -1) * (STATES / 10);
  uint32_t offset = (speed > 0) ? Light.strip_timer_counter / speed : 0;

  WsColor mcolor[scheme.count];
  memcpy(mcolor, scheme.colors, sizeof(mcolor));
  float dimmer = 100 / (float)Settings.light_dimmer;
  for (uint32_t i = 0; i < scheme.count; i++) {
    float fmyRed = (float)mcolor[i].red / dimmer;
    float fmyGrn = (float)mcolor[i].green / dimmer;
    float fmyBlu = (float)mcolor[i].blue / dimmer;
    mcolor[i].red = (uint8_t)fmyRed;
    mcolor[i].green = (uint8_t)fmyGrn;
    mcolor[i].blue = (uint8_t)fmyBlu;
  }
  uint32_t colorIndex = offset % scheme.count;
  for (uint32_t i = 0; i < Settings.light_pixels; i++) {
    if (maxSize) { colorIndex = ((i + offset) % (scheme.count * kWidth[Settings.light_width])) / kWidth[Settings.light_width]; }
    c.R = mcolor[colorIndex].red;
    c.G = mcolor[colorIndex].green;
    c.B = mcolor[colorIndex].blue;
    strip->SetPixelColor(i, c);
  }
  Ws2812StripShow();
}

void Ws2812Clear(void)
{
  strip->ClearTo(0);
  strip->Show();
  Ws2812.show_next = 1;
}

void Ws2812SetColor(uint32_t led, uint8_t red, uint8_t green, uint8_t blue, uint8_t white)
{
#if (USE_WS2812_CTYPE > NEO_3LED)
  RgbwColor lcolor;
  lcolor.W = white;
#else
  RgbColor lcolor;
#endif

  lcolor.R = red;
  lcolor.G = green;
  lcolor.B = blue;
  if (led) {
    strip->SetPixelColor(led -1, lcolor);  // Led 1 is strip Led 0 -> substract offset 1
  } else {
//    strip->ClearTo(lcolor);  // Set WS2812_MAX_LEDS pixels
    for (uint32_t i = 0; i < Settings.light_pixels; i++) {
      strip->SetPixelColor(i, lcolor);
    }
  }

  if (!Ws2812.suspend_update) {
    strip->Show();
    Ws2812.show_next = 1;
  }
}

char* Ws2812GetColor(uint32_t led, char* scolor)
{
  uint8_t sl_ledcolor[4];

 #if (USE_WS2812_CTYPE > NEO_3LED)
  RgbwColor lcolor = strip->GetPixelColor(led -1);
  sl_ledcolor[3] = lcolor.W;
 #else
  RgbColor lcolor = strip->GetPixelColor(led -1);
 #endif
  sl_ledcolor[0] = lcolor.R;
  sl_ledcolor[1] = lcolor.G;
  sl_ledcolor[2] = lcolor.B;
  scolor[0] = '\0';
  for (uint32_t i = 0; i < Light.subtype; i++) {
    if (Settings.flag.decimal_text) {  // SetOption17 - Switch between decimal or hexadecimal output (0 = hexadecimal, 1 = decimal)
      snprintf_P(scolor, 25, PSTR("%s%s%d"), scolor, (i > 0) ? "," : "", sl_ledcolor[i]);
    } else {
      snprintf_P(scolor, 25, PSTR("%s%02X"), scolor, sl_ledcolor[i]);
    }
  }
  return scolor;
}

/*********************************************************************************************\
 * Public - used by scripter only
\*********************************************************************************************/

void Ws2812ForceSuspend (void)
{
  Ws2812.suspend_update = true;
}

void Ws2812ForceUpdate (void)
{
  Ws2812.suspend_update = false;
  strip->Show();
  Ws2812.show_next = 1;
}

/********************************************************************************************/

bool Ws2812SetChannels(void)
{
  uint8_t *cur_col = (uint8_t*)XdrvMailbox.data;

  Ws2812SetColor(0, cur_col[0], cur_col[1], cur_col[2], cur_col[3]);

  return true;
}

void Ws2812ShowScheme(void)
{
  uint32_t scheme = Settings.light_scheme - Ws2812.scheme_offset;

  switch (scheme) {
    case 0:  // Clock
      if ((1 == TasmotaGlobal.state_250mS) || (Ws2812.show_next)) {
        Ws2812Clock();
        Ws2812.show_next = 0;
      }
      break;
    default:
      if (1 == Settings.light_fade) {
        Ws2812Gradient(scheme -1);
      } else {
        Ws2812Bars(scheme -1);
      }
      Ws2812.show_next = 1;
      break;
  }
}

void Ws2812ModuleSelected(void)
{
#if (USE_WS2812_HARDWARE == NEO_HW_P9813)
  if (PinUsed(GPIO_P9813_CLK) && PinUsed(GPIO_P9813_DAT)) {  // RGB led
    strip = new NeoPixelBus<selectedNeoFeatureType, selectedNeoSpeedType>(WS2812_MAX_LEDS, Pin(GPIO_P9813_CLK), Pin(GPIO_P9813_DAT));
#else
  if (PinUsed(GPIO_WS2812)) {  // RGB led
    // For DMA, the Pin is ignored as it uses GPIO3 due to DMA hardware use.
    strip = new NeoPixelBus<selectedNeoFeatureType, selectedNeoSpeedType>(WS2812_MAX_LEDS, Pin(GPIO_WS2812));
#endif  // NEO_HW_P9813
    strip->Begin();

    Ws2812Clear();

    Ws2812.scheme_offset = Light.max_scheme +1;
    Light.max_scheme += WS2812_SCHEMES;

#if (USE_WS2812_CTYPE > NEO_3LED)
    TasmotaGlobal.light_type = LT_RGBW;
#else
    TasmotaGlobal.light_type = LT_RGB;
#endif
    TasmotaGlobal.light_driver = XLGT_01;
  }
}

/********************************************************************************************/

void CmndLed(void)
{
  if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= Settings.light_pixels)) {
    if (XdrvMailbox.data_len > 0) {
      char *p;
      uint16_t idx = XdrvMailbox.index;
      Ws2812ForceSuspend();
      for (char *color = strtok_r(XdrvMailbox.data, " ", &p); color; color = strtok_r(nullptr, " ", &p)) {
        if (LightColorEntry(color, strlen(color))) {
          Ws2812SetColor(idx, Light.entry_color[0], Light.entry_color[1], Light.entry_color[2], Light.entry_color[3]);
          idx++;
          if (idx > Settings.light_pixels) { break; }
        } else {
          break;
        }
      }
      Ws2812ForceUpdate();
    }
    char scolor[LIGHT_COLOR_SIZE];
    ResponseCmndIdxChar(Ws2812GetColor(XdrvMailbox.index, scolor));
  }
}

void CmndPixels(void)
{
  if ((XdrvMailbox.payload > 0) && (XdrvMailbox.payload <= WS2812_MAX_LEDS)) {
    Settings.light_pixels = XdrvMailbox.payload;
    Settings.light_rotation = 0;
    Ws2812Clear();
    Light.update = true;
  }
  ResponseCmndNumber(Settings.light_pixels);
}

void CmndRotation(void)
{
  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload < Settings.light_pixels)) {
    Settings.light_rotation = XdrvMailbox.payload;
  }
  ResponseCmndNumber(Settings.light_rotation);
}

void CmndWidth(void)
{
  if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= 4)) {
    if (1 == XdrvMailbox.index) {
      if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 4)) {
        Settings.light_width = XdrvMailbox.payload;
      }
      ResponseCmndNumber(Settings.light_width);
    } else {
      if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload < 32)) {
        Settings.ws_width[XdrvMailbox.index -2] = XdrvMailbox.payload;
      }
      ResponseCmndIdxNumber(Settings.ws_width[XdrvMailbox.index -2]);
    }
  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xlgt01(uint8_t function)
{
  bool result = false;

  switch (function) {
    case FUNC_SET_CHANNELS:
      result = Ws2812SetChannels();
      break;
    case FUNC_SET_SCHEME:
      Ws2812ShowScheme();
      break;
    case FUNC_COMMAND:
      result = DecodeCommand(kWs2812Commands, Ws2812Command);
      break;
    case FUNC_MODULE_INIT:
      Ws2812ModuleSelected();
      break;
  }
  return result;
}

#endif  // USE_WS2812
#endif  // USE_LIGHT
