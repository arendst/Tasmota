/*
Copyright (c) 2017 Theo Arends.  All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

- Redistributions of source code must retain the above copyright notice,
  this list of conditions and the following disclaimer.
- Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation
  and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.
*/

#ifdef USE_WS2812
/*********************************************************************************************\
 * WS2812 Leds using NeopixelBus library
\*********************************************************************************************/

#include <NeoPixelBus.h>

#ifdef USE_WS2812_DMA
  NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> *strip = NULL;
#else
  NeoPixelBus<NeoGrbFeature, NeoEsp8266BitBang800KbpsMethod> *strip = NULL;
#endif  // USE_WS2812_DMA

uint8_t ledTable[] = {
    0,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
    1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
    1,  2,  2,  2,  2,  2,  2,  2,  2,  3,  3,  3,  3,  3,  4,  4,
    4,  4,  4,  5,  5,  5,  5,  6,  6,  6,  6,  7,  7,  7,  7,  8,
    8,  8,  9,  9,  9, 10, 10, 10, 11, 11, 12, 12, 12, 13, 13, 14,
   14, 15, 15, 15, 16, 16, 17, 17, 18, 18, 19, 19, 20, 20, 21, 22,
   22, 23, 23, 24, 25, 25, 26, 26, 27, 28, 28, 29, 30, 30, 31, 32,
   33, 33, 34, 35, 36, 36, 37, 38, 39, 40, 40, 41, 42, 43, 44, 45,
   46, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60,
   61, 62, 63, 64, 65, 67, 68, 69, 70, 71, 72, 73, 75, 76, 77, 78,
   80, 81, 82, 83, 85, 86, 87, 89, 90, 91, 93, 94, 95, 97, 98, 99,
  101,102,104,105,107,108,110,111,113,114,116,117,119,121,122,124,
  125,127,129,130,132,134,135,137,139,141,142,144,146,148,150,151,
  153,155,157,159,161,163,165,166,168,170,172,174,176,178,180,182,
  184,186,189,191,193,195,197,199,201,204,206,208,210,212,215,217,
  219,221,224,226,228,231,233,235,238,240,243,245,248,250,253,255 };

uint8_t fadeValues[5] = { 
  0,      // None
  2,      // Fastest
  6,      // Fast
  12,     // Slow
  18  };  // Slowest

uint8_t lany = 0;
RgbColor dcolor, tcolor, lcolor;

void do_cmnd_led(uint16_t led, byte *colstr)
{
  int i = 0;
  HtmlColor color;
  
  uint8_t result = color.Parse<HtmlColorNames>((char *)colstr, 7);
  if(result) {
    if(0xFFFF == led) {
      for (i = 0; i < sysCfg.ws_pixels; i++)
        strip->SetPixelColor(i, RgbColor(color));
    }
    else {
      strip->SetPixelColor(led-1, RgbColor(color)); // Led 1 is strip Led 0 -> substract offset 1
    }
    strip->Show();
  }
}

void ws2812_setDim(uint8_t myDimmer)
{
  float newDim = 100 / (float)myDimmer;
  float fmyRed = (float)sysCfg.ws_red / newDim;
  float fmyGrn = (float)sysCfg.ws_green / newDim;
  float fmyBlu = (float)sysCfg.ws_blue / newDim;
  dcolor.R = (uint8_t)fmyRed;
  dcolor.G = (uint8_t)fmyGrn;
  dcolor.B = (uint8_t)fmyBlu;
}

void ws2812_setColor(char* dataBufUc)
{
  HtmlColor hcolor;
  char log[LOGSZ], colstr[8]; 
  
  snprintf_P(colstr, sizeof(colstr), PSTR("#%s"), dataBufUc);
  uint8_t result = hcolor.Parse<HtmlColorNames>((char *)colstr, 7);
  dcolor = RgbColor(hcolor);

//  snprintf_P(log, sizeof(log), PSTR("DBG: Red %02X, Green %02X, Blue %02X"), dcolor.R, dcolor.G, dcolor.B);
//  addLog(LOG_LEVEL_DEBUG, log);
  
  uint16_t temp = dcolor.R;
  if (temp < dcolor.G) temp = dcolor.G;
  if (temp < dcolor.B) temp = dcolor.B;
  float mDim = (float)temp / 2.55;
  sysCfg.ws_dimmer = (uint8_t)mDim;
  
  float newDim = 100 / mDim;
  float fmyRed = (float)dcolor.R * newDim;
  float fmyGrn = (float)dcolor.G * newDim;
  float fmyBlu = (float)dcolor.B * newDim;
  sysCfg.ws_red = (uint8_t)fmyRed;
  sysCfg.ws_green = (uint8_t)fmyGrn;
  sysCfg.ws_blue = (uint8_t)fmyBlu;
  
  lany = 1;
}

void ws2812_stripShow()
{
  RgbColor c;  

  if (sysCfg.ws_ledtable) {
    for (uint16_t i = 0; i < sysCfg.ws_pixels; i++) {
      c = strip->GetPixelColor(i);
      strip->SetPixelColor(i, RgbColor(ledTable[c.R], ledTable[c.G], ledTable[c.B]));
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

void ws2812_clock()
{
  RgbColor c;
  
  strip->ClearTo(0);   // Reset strip
  float newDim = 100 / (float)sysCfg.ws_dimmer;
  float f1 = 255 / newDim;
  uint8_t i1 = (uint8_t)f1;
  float f2 = 127 / newDim;
  uint8_t i2 = (uint8_t)f2;
  float f3 = 63 / newDim;
  uint8_t i3 = (uint8_t)f3;

  int j = sysCfg.ws_pixels;
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

void ws2812_getColor(char* svalue, uint16_t ssvalue)
{
  ws2812_setDim(sysCfg.ws_dimmer);
  uint32_t color = (uint32_t)dcolor.R << 16;
  color += (uint32_t)dcolor.G << 8;
  color += (uint32_t)dcolor.B;
  snprintf_P(svalue, ssvalue, PSTR("{\"Color\":\"%06X\"}"), color);
}

void ws2812_animate()
{
  char log[LOGSZ];
  uint8_t fadeValue;
  
  if (power == 0) {
    tcolor = 0;
  }
  else {
    switch (sysCfg.ws_scheme) {
      case 0:
        ws2812_setDim(sysCfg.ws_dimmer);
        if (sysCfg.ws_fade == 0) {
          tcolor = dcolor;
        } else {
          fadeValue = fadeValues[sysCfg.ws_speed] +1;
          if (tcolor != dcolor) {
            if (tcolor.R < dcolor.R) tcolor.R += (dcolor.R - tcolor.R) / fadeValue;
            if (tcolor.G < dcolor.G) tcolor.G += (dcolor.G - tcolor.G) / fadeValue;
            if (tcolor.B < dcolor.B) tcolor.B += (dcolor.B - tcolor.B) / fadeValue;
            if (tcolor.R > dcolor.R) tcolor.R -= (tcolor.R - dcolor.R) / fadeValue;
            if (tcolor.G > dcolor.G) tcolor.G -= (tcolor.G - dcolor.G) / fadeValue;
            if (tcolor.B > dcolor.B) tcolor.B -= (tcolor.B - dcolor.B) / fadeValue;
          }
        }
        break;
      case 1:
        if ((state == (STATES/10)*2) || (lany != 2)) ws2812_clock();
        lany = 2;
        break;
    }
  }

  if ((sysCfg.ws_scheme < 1) || (!(power &1))) {
    if ((lcolor != tcolor) || lany) {
      lany = 0;
      lcolor = tcolor;

//    snprintf_P(log, sizeof(log), PSTR("DBG: StripPixels %d, CfgPixels %d, Red %02X, Green %02X, Blue %02X"), strip.PixelCount(), sysCfg.ws_pixels, lcolor.R, lcolor.G, lcolor.B);
//    addLog(LOG_LEVEL_DEBUG, log);

      if (sysCfg.ws_ledtable) {
        for (uint16_t i = 0; i < sysCfg.ws_pixels; i++) strip->SetPixelColor(i, RgbColor(ledTable[lcolor.R],ledTable[lcolor.G],ledTable[lcolor.B]));      
      } else {
        for (uint16_t i = 0; i < sysCfg.ws_pixels; i++) strip->SetPixelColor(i, lcolor);
      }
      strip->Show();
    }
  }
}

void ws2812_update()
{
  lany = 1;
}

void ws2812_pixels()
{
  strip->ClearTo(0);
  strip->Show();
  tcolor = 0;
  lany = 1;
}

void ws2812_init()
{
#ifdef USE_WS2812_DMA
  strip = new NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod>(WS2812_MAX_LEDS);  // For Esp8266, the Pin is omitted and it uses GPIO3 due to DMA hardware use.
#else
  strip = new NeoPixelBus<NeoGrbFeature, NeoEsp8266BitBang800KbpsMethod>(WS2812_MAX_LEDS, pin[GPIO_WS2812]);
#endif  // USE_WS2812_DMA
  strip->Begin();
  ws2812_pixels();
}
#endif  // USE_WS2812
