/*
  xdrv_ws2812.ino - ws2812 led string support for Sonoff-Tasmota

  Copyright (C) 2017  Heiko Krupp and Theo Arends

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

#ifdef USE_WS2812
/*********************************************************************************************\
 * WS2812 Leds using NeopixelBus library
\*********************************************************************************************/

#include <NeoPixelBus.h>

#ifdef USE_WS2812_DMA
#if (USE_WS2812_CTYPE == 1)
  NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> *strip = NULL;
#else  // USE_WS2812_CTYPE
  NeoPixelBus<NeoRgbFeature, Neo800KbpsMethod> *strip = NULL;
#endif  // USE_WS2812_CTYPE
#else  // USE_WS2812_DMA
#if (USE_WS2812_CTYPE == 1)
  NeoPixelBus<NeoGrbFeature, NeoEsp8266BitBang800KbpsMethod> *strip = NULL;
#else  // USE_WS2812_CTYPE
  NeoPixelBus<NeoRgbFeature, NeoEsp8266BitBang800KbpsMethod> *strip = NULL;
#endif  // USE_WS2812_CTYPE
#endif  // USE_WS2812_DMA

struct wsColor {
  uint8_t red, green, blue;
};

struct ColorScheme {
  wsColor* colors;
  uint8_t count;
};

wsColor incandescent[2] = { 255, 140, 20, 0, 0, 0 };
wsColor rgb[3] = { 255, 0, 0, 0, 255, 0, 0, 0, 255 };
wsColor christmas[2] = { 255, 0, 0, 0, 255, 0 };
wsColor hanukkah[2] = { 0, 0, 255, 255, 255, 255 };
wsColor kwanzaa[3] = { 255, 0, 0, 0, 0, 0, 0, 255, 0 };
wsColor rainbow[7] = { 255, 0, 0, 255, 128, 0, 255, 255, 0, 0, 255, 0, 0, 0, 255, 128, 0, 255, 255, 0, 255 };
wsColor fire[3] = { 255, 0, 0, 255, 102, 0, 255, 192, 0 };
ColorScheme schemes[7] = {
  incandescent, 2,
  rgb, 3,
  christmas, 2,
  hanukkah, 2,
  kwanzaa, 3,
  rainbow, 7,
  fire, 3 };

uint8_t widthValues[5] = {
    1,     // Small
    2,     // Medium
    4,     // Large
    8,     // Largest
  255 };   // All
uint8_t repeatValues[5] = {
    8,     // Small
    6,     // Medium
    4,     // Large
    2,     // Largest
    1 };   // All

uint8_t speedValues[9] = {
    0,                     // None
    1 * (STATES / 10),     // Fastest
    3 * (STATES / 10),
    5 * (STATES / 10),     // Fast
    7 * (STATES / 10),
    9 * (STATES / 10),
   11 * (STATES / 10),     // Slow
   13 * (STATES / 10),
   15 * (STATES / 10) };   // Slowest

uint8_t ws2812_showNext = 1;

/********************************************************************************************/

void ws2812_stripShow()
{
  RgbColor c;

  if (sysCfg.led_table) {
    for (uint16_t i = 0; i < sysCfg.led_pixels; i++) {
      c = strip->GetPixelColor(i);
      strip->SetPixelColor(i, RgbColor(ledTable[c.R], ledTable[c.G], ledTable[c.B]));
    }
  }
  strip->Show();
}

int mod(int a, int b)
{
   int ret = a % b;
   if (ret < 0) {
    ret += b;
   }
   return ret;
}

void ws2812_clock()
{
  RgbColor c;

  strip->ClearTo(0);   // Reset strip
  float newDim = 100 / (float)sysCfg.led_dimmer[0];
  float f1 = 255 / newDim;
  uint8_t i1 = (uint8_t)f1;
  float f2 = 127 / newDim;
  uint8_t i2 = (uint8_t)f2;
  float f3 = 63 / newDim;
  uint8_t i3 = (uint8_t)f3;

  int j = sysCfg.led_pixels;
  int clksize = 600 / j;
  int i = (rtcTime.Second * 10) / clksize;

  c = strip->GetPixelColor(mod(i,    j)); c.B = i1; strip->SetPixelColor(mod(i,    j), c);
  i = (rtcTime.Minute * 10) / clksize;
  c = strip->GetPixelColor(mod(i -1, j)); c.G = i3; strip->SetPixelColor(mod(i -1, j), c);
  c = strip->GetPixelColor(mod(i,    j)); c.G = i1; strip->SetPixelColor(mod(i,    j), c);
  c = strip->GetPixelColor(mod(i +1, j)); c.G = i3; strip->SetPixelColor(mod(i +1, j), c);
  i = (rtcTime.Hour % 12) * (50 / clksize);
  c = strip->GetPixelColor(mod(i -2, j)); c.R = i3; strip->SetPixelColor(mod(i -2, j), c);
  c = strip->GetPixelColor(mod(i -1, j)); c.R = i2; strip->SetPixelColor(mod(i -1, j), c);
  c = strip->GetPixelColor(mod(i,    j)); c.R = i1; strip->SetPixelColor(mod(i,    j), c);
  c = strip->GetPixelColor(mod(i +1, j)); c.R = i2; strip->SetPixelColor(mod(i +1, j), c);
  c = strip->GetPixelColor(mod(i +2, j)); c.R = i3; strip->SetPixelColor(mod(i +2, j), c);
  ws2812_stripShow();
}

void ws2812_gradientColor(uint8_t schemenr, struct wsColor* mColor, uint16_t range, uint16_t gradRange, uint16_t i)
{
/*
 * Compute the color of a pixel at position i using a gradient of the color scheme.
 * This function is used internally by the gradient function.
 */
  ColorScheme scheme = schemes[schemenr];
  uint16_t curRange = i / range;
  uint16_t rangeIndex = i % range;
  uint16_t colorIndex = rangeIndex / gradRange;
  uint16_t start = colorIndex;
  uint16_t end = colorIndex +1;
  if (curRange % 2 != 0) {
    start = (scheme.count -1) - start;
    end = (scheme.count -1) - end;
  }
  float newDim = 100 / (float)sysCfg.led_dimmer[0];
  float fmyRed = (float)map(rangeIndex % gradRange, 0, gradRange, scheme.colors[start].red, scheme.colors[end].red) / newDim;
  float fmyGrn = (float)map(rangeIndex % gradRange, 0, gradRange, scheme.colors[start].green, scheme.colors[end].green) / newDim;
  float fmyBlu = (float)map(rangeIndex % gradRange, 0, gradRange, scheme.colors[start].blue, scheme.colors[end].blue) / newDim;
  mColor->red = (uint8_t)fmyRed;
  mColor->green = (uint8_t)fmyGrn;
  mColor->blue = (uint8_t)fmyBlu;
}

void ws2812_gradient(uint8_t schemenr)
{
/*
 * This routine courtesy Tony DiCola (Adafruit)
 * Display a gradient of colors for the current color scheme.
 *  Repeat is the number of repetitions of the gradient (pick a multiple of 2 for smooth looping of the gradient).
 */
  RgbColor c;

  ColorScheme scheme = schemes[schemenr];
  if (scheme.count < 2) {
    return;
  }

  uint8_t repeat = repeatValues[sysCfg.led_width];  // number of scheme.count per ledcount
  uint16_t range = (uint16_t)ceil((float)sysCfg.led_pixels / (float)repeat);
  uint16_t gradRange = (uint16_t)ceil((float)range / (float)(scheme.count - 1));
  uint16_t offset = speedValues[sysCfg.led_speed] > 0 ? stripTimerCntr / speedValues[sysCfg.led_speed] : 0;

  wsColor oldColor, currentColor;
  ws2812_gradientColor(schemenr, &oldColor, range, gradRange, offset);
  currentColor = oldColor;
  for (uint16_t i = 0; i < sysCfg.led_pixels; i++) {
    if (repeatValues[sysCfg.led_width] > 1) {
      ws2812_gradientColor(schemenr, &currentColor, range, gradRange, i +offset);
    }
    if (sysCfg.led_speed > 0) {
      // Blend old and current color based on time for smooth movement.
      c.R = map(stripTimerCntr % speedValues[sysCfg.led_speed], 0, speedValues[sysCfg.led_speed], oldColor.red, currentColor.red);
      c.G = map(stripTimerCntr % speedValues[sysCfg.led_speed], 0, speedValues[sysCfg.led_speed], oldColor.green, currentColor.green);
      c.B = map(stripTimerCntr % speedValues[sysCfg.led_speed], 0, speedValues[sysCfg.led_speed], oldColor.blue, currentColor.blue);
    }
    else {
      // No animation, just use the current color.
      c.R = currentColor.red;
      c.G = currentColor.green;
      c.B = currentColor.blue;
    }
    strip->SetPixelColor(i, c);
    oldColor = currentColor;
  }
  ws2812_stripShow();
}

void ws2812_bars(uint8_t schemenr)
{
/*
 * This routine courtesy Tony DiCola (Adafruit)
 * Display solid bars of color for the current color scheme.
 * Width is the width of each bar in pixels/lights.
 */
  RgbColor c;
  uint16_t i;

  ColorScheme scheme = schemes[schemenr];

  uint16_t maxSize = sysCfg.led_pixels / scheme.count;
  if (widthValues[sysCfg.led_width] > maxSize) {
    maxSize = 0;
  }

  uint8_t offset = speedValues[sysCfg.led_speed] > 0 ? stripTimerCntr / speedValues[sysCfg.led_speed] : 0;

  wsColor mcolor[scheme.count];
  memcpy(mcolor, scheme.colors, sizeof(mcolor));
  float newDim = 100 / (float)sysCfg.led_dimmer[0];
  for (i = 0; i < scheme.count; i++) {
    float fmyRed = (float)mcolor[i].red / newDim;
    float fmyGrn = (float)mcolor[i].green / newDim;
    float fmyBlu = (float)mcolor[i].blue / newDim;
    mcolor[i].red = (uint8_t)fmyRed;
    mcolor[i].green = (uint8_t)fmyGrn;
    mcolor[i].blue = (uint8_t)fmyBlu;
  }
  uint8_t colorIndex = offset % scheme.count;
  for (i = 0; i < sysCfg.led_pixels; i++) {
    if (maxSize) {
      colorIndex = ((i + offset) % (scheme.count * widthValues[sysCfg.led_width])) / widthValues[sysCfg.led_width];
    }
    c.R = mcolor[colorIndex].red;
    c.G = mcolor[colorIndex].green;
    c.B = mcolor[colorIndex].blue;
    strip->SetPixelColor(i, c);
  }
  ws2812_stripShow();
}

/*********************************************************************************************\
 * Public
\*********************************************************************************************/

void ws2812_init()
{
#ifdef USE_WS2812_DMA
#if (USE_WS2812_CTYPE == 1)
  strip = new NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod>(WS2812_MAX_LEDS);  // For Esp8266, the Pin is omitted and it uses GPIO3 due to DMA hardware use.
#else  // USE_WS2812_CTYPE
  strip = new NeoPixelBus<NeoRgbFeature, Neo800KbpsMethod>(WS2812_MAX_LEDS);  // For Esp8266, the Pin is omitted and it uses GPIO3 due to DMA hardware use.
#endif  // USE_WS2812_CTYPE
#else  // USE_WS2812_DMA
#if (USE_WS2812_CTYPE == 1)
  strip = new NeoPixelBus<NeoGrbFeature, NeoEsp8266BitBang800KbpsMethod>(WS2812_MAX_LEDS, pin[GPIO_WS2812]);
#else  // USE_WS2812_CTYPE
  strip = new NeoPixelBus<NeoRgbFeature, NeoEsp8266BitBang800KbpsMethod>(WS2812_MAX_LEDS, pin[GPIO_WS2812]);
#endif  // USE_WS2812_CTYPE
#endif  // USE_WS2812_DMA
  strip->Begin();
  ws2812_clear();
}

void ws2812_clear()
{
  strip->ClearTo(0);
  strip->Show();
  ws2812_showNext = 1;
}

void ws2812_setColor(uint16_t led, uint8_t red, uint8_t green, uint8_t blue)
{
  RgbColor lcolor;
  lcolor.R = red;
  lcolor.G = green;
  lcolor.B = blue;
  if (led) {
    strip->SetPixelColor(led -1, lcolor);  // Led 1 is strip Led 0 -> substract offset 1
  } else {
//    strip->ClearTo(lcolor);  // Set WS2812_MAX_LEDS pixels
    for (uint16_t i = 0; i < sysCfg.led_pixels; i++) {
      strip->SetPixelColor(i, lcolor);
    }
  }
  strip->Show();
  ws2812_showNext = 1;
}

char* ws2812_getColor(uint16_t led, char* scolor)
{
  uint8_t sl_ledcolor[3];

  RgbColor lcolor = strip->GetPixelColor(led -1);
  sl_ledcolor[0] = lcolor.R;
  sl_ledcolor[1] = lcolor.G;
  sl_ledcolor[2] = lcolor.B;
  scolor[0] = '\0';
  for (byte i = 0; i < 3; i++) {
    snprintf_P(scolor, 11, PSTR("%s%02X"), scolor, sl_ledcolor[i]);
  }
  return scolor;
}

void ws2812_showScheme(uint8_t scheme)
{
  switch (scheme) {
    case 0:  // Clock
      if (((STATES/10)*2 == state) || (ws2812_showNext)) {
        ws2812_clock();
        ws2812_showNext = 0;
      }
      break;
    default:
      if (1 == sysCfg.led_fade) {
        ws2812_gradient(scheme -1);
      } else {
        ws2812_bars(scheme -1);
      }
      ws2812_showNext = 1;
      break;
  }
}

#endif  // USE_WS2812
