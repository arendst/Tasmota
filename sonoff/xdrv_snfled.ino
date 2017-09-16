/*
  xdrv_snfled.ino - WS2812 and sonoff led support for Sonoff-Tasmota

  Copyright (C) 2017  Theo Arends

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

/*********************************************************************************************\
 * WS2812, Sonoff B1, AiLight, Sonoff Led and BN-SZ01
 *
 * sfl_flg  Module        Color  ColorTemp
 * 1        Sonoff BN-SZ  W      no
 * 2        Sonoff Led    CW     yes
 * 3        +WS2812       RGB    no
 * 4        AiLight       RGBW   no
 * 5        Sonoff B1     RGBCW  yes
 *
 * led_scheme  WS2812  Others  Effect
 * 0           yes     yes     Color On/Off
 * 1           yes     yes     Wakeup light
 * 2           yes     no      Clock
 * 3           yes     no      Incandescent
 * 4           yes     no      RGB
 * 5           yes     no      Christmas
 * 6           yes     no      Hanukkah
 * 7           yes     no      Kwanzaa
 * 8           yes     no      Rainbow
 * 9           yes     no      Fire
 *
\*********************************************************************************************/

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

uint8_t sl_dcolor[5];
uint8_t sl_tcolor[5];
uint8_t sl_lcolor[5];

uint8_t sl_power = 0;
uint8_t sl_any = 0;
uint8_t sl_wakeupActive = 0;
uint8_t sl_wakeupDimmer = 0;
uint16_t sl_wakeupCntr = 0;

unsigned long stripTimerCntr = 0;  // Bars and Gradient

#ifdef USE_WS2812
/*********************************************************************************************\
 * WS2812 Leds using NeopixelBus library
\*********************************************************************************************/

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

uint8_t sl_ledcolor[3];

/********************************************************************************************/

void ws2812_pixels()
{
  strip->ClearTo(0);
  strip->Show();
  sl_any = 1;
}

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
  ws2812_pixels();
}

void ws2812_setLedColor(uint16_t led)
{
  RgbColor lcolor;
  lcolor.R = sl_ledcolor[0];
  lcolor.G = sl_ledcolor[1];
  lcolor.B = sl_ledcolor[2];
  strip->SetPixelColor(led -1, lcolor);  // Led 1 is strip Led 0 -> substract offset 1
  strip->Show();
}

char* ws2812_getLedColor(uint16_t led, char* scolor)
{
  RgbColor lcolor = strip->GetPixelColor(led -1);
  sl_ledcolor[0] = lcolor.R;
  sl_ledcolor[1] = lcolor.G;
  sl_ledcolor[2] = lcolor.B;
  scolor[0] = '\0';
  for (byte i = 0; i < sfl_flg; i++) {
    snprintf_P(scolor, 11, PSTR("%s%02X"), scolor, sl_ledcolor[i]);
  }
  return scolor;
}

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

void ws2812_gradientColor(struct wsColor* mColor, uint16_t range, uint16_t gradRange, uint16_t i)
{
/*
 * Compute the color of a pixel at position i using a gradient of the color scheme.
 * This function is used internally by the gradient function.
 */
  ColorScheme scheme = schemes[sysCfg.led_scheme -3];
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

void ws2812_gradient()
{
/*
 * This routine courtesy Tony DiCola (Adafruit)
 * Display a gradient of colors for the current color scheme.
 *  Repeat is the number of repetitions of the gradient (pick a multiple of 2 for smooth looping of the gradient).
 */
  RgbColor c;

  ColorScheme scheme = schemes[sysCfg.led_scheme -3];
  if (scheme.count < 2) {
    return;
  }

  uint8_t repeat = repeatValues[sysCfg.led_width];  // number of scheme.count per ledcount
  uint16_t range = (uint16_t)ceil((float)sysCfg.led_pixels / (float)repeat);
  uint16_t gradRange = (uint16_t)ceil((float)range / (float)(scheme.count - 1));
  uint16_t offset = speedValues[sysCfg.led_speed] > 0 ? stripTimerCntr / speedValues[sysCfg.led_speed] : 0;

  wsColor oldColor, currentColor;
  ws2812_gradientColor(&oldColor, range, gradRange, offset);
  currentColor = oldColor;
  for (uint16_t i = 0; i < sysCfg.led_pixels; i++) {
    if (repeatValues[sysCfg.led_width] > 1) {
      ws2812_gradientColor(&currentColor, range, gradRange, i +offset);
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

void ws2812_bars()
{
/*
 * This routine courtesy Tony DiCola (Adafruit)
 * Display solid bars of color for the current color scheme.
 * Width is the width of each bar in pixels/lights.
 */
  RgbColor c;
  uint16_t i;

  ColorScheme scheme = schemes[sysCfg.led_scheme -3];

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

#endif  // USE_WS2812

/*********************************************************************************************\
 * Sonoff B1 and AiLight inspired by OpenLight https://github.com/icamgo/noduino-sdk
\*********************************************************************************************/

extern "C" {
  void os_delay_us(unsigned int);
}

uint8_t sl_pdi;
uint8_t sl_pdcki;

void sl_di_pulse(uint8_t times)
{
  for (uint8_t i = 0; i < times; i++) {
    digitalWrite(sl_pdi, HIGH);
    digitalWrite(sl_pdi, LOW);
  }
}

void sl_dcki_pulse(uint8_t times)
{
  for (uint8_t i = 0; i < times; i++) {
    digitalWrite(sl_pdcki, HIGH);
    digitalWrite(sl_pdcki, LOW);
  }
}

void sl_my92x1_write(uint8_t data)
{
  for (uint8_t i = 0; i < 4; i++) {    // Send 8bit Data
    digitalWrite(sl_pdcki, LOW);
    digitalWrite(sl_pdi, (data & 0x80));
    digitalWrite(sl_pdcki, HIGH);
    data = data << 1;
    digitalWrite(sl_pdi, (data & 0x80));
    digitalWrite(sl_pdcki, LOW);
    digitalWrite(sl_pdi, LOW);
    data = data << 1;
  }
}

void sl_my92x1_init()
{
  uint8_t chips = sfl_flg -3;  // 1 (AiLight) or 2 (Sonoff B1)

  sl_dcki_pulse(chips * 32);   // Clear all duty register
  os_delay_us(12);             // TStop > 12us.
  // Send 12 DI pulse, after 6 pulse's falling edge store duty data, and 12
  // pulse's rising edge convert to command mode.
  sl_di_pulse(12);
  os_delay_us(12);             // Delay >12us, begin send CMD data
  for (uint8_t n = 0; n < chips; n++) {    // Send CMD data
    sl_my92x1_write(0x18);     // ONE_SHOT_DISABLE, REACTION_FAST, BIT_WIDTH_8, FREQUENCY_DIVIDE_1, SCATTER_APDM
  }
  os_delay_us(12);             // TStart > 12us. Delay 12 us.
  // Send 16 DI pulse, at 14 pulse's falling edge store CMD data, and
  // at 16 pulse's falling edge convert to duty mode.
  sl_di_pulse(16);
  os_delay_us(12);             // TStop > 12us.
}

void sl_my92x1_duty(uint8_t duty_r, uint8_t duty_g, uint8_t duty_b, uint8_t duty_w, uint8_t duty_c)
{
  uint8_t channels[2] = { 4, 6 };

  uint8_t didx = sfl_flg -4;   // 0 or 1

  uint8_t duty[2][6] = {{ duty_r, duty_g, duty_b, duty_w, 0, 0 },        // Definition for RGBW channels
                        { duty_w, duty_c, 0, duty_g, duty_r, duty_b }};  // Definition for RGBWC channels

  os_delay_us(12);             // TStop > 12us.
  for (uint8_t channel = 0; channel < channels[didx]; channel++) {
    sl_my92x1_write(duty[didx][channel]);  // Send 8bit Data
  }
  os_delay_us(12);             // TStart > 12us. Ready for send DI pulse.
  sl_di_pulse(8);              // Send 8 DI pulse. After 8 pulse falling edge, store old data.
  os_delay_us(12);             // TStop > 12us.
}

/********************************************************************************************/

void sl_init(void)
{
  if (sfl_flg < 3) {
    if (!my_module.gp.io[4]) {
      pinMode(4, OUTPUT);     // Stop floating outputs
      digitalWrite(4, LOW);
    }
    if (!my_module.gp.io[5]) {
      pinMode(5, OUTPUT);     // Stop floating outputs
      digitalWrite(5, LOW);
    }
    if (!my_module.gp.io[14]) {
      pinMode(14, OUTPUT);    // Stop floating outputs
      digitalWrite(14, LOW);
    }
    sysCfg.pwmvalue[0] = 0;    // We use dimmer / led_color
    if (2 == sfl_flg) {
      sysCfg.pwmvalue[1] = 0;  // We use led_color
    }
    sysCfg.led_scheme = 0;
  }
#ifdef USE_WS2812  // ************************************************************************
  else if (3 == sfl_flg) {
    ws2812_init();
    if (1 == sysCfg.led_scheme) {
      sysCfg.led_scheme = 0;
    }
  }
#endif  // USE_WS2812 ************************************************************************
  else {
    sl_pdi = pin[GPIO_DI];
    sl_pdcki = pin[GPIO_DCKI];

    pinMode(sl_pdi, OUTPUT);
    pinMode(sl_pdcki, OUTPUT);
    digitalWrite(sl_pdi, LOW);
    digitalWrite(sl_pdcki, LOW);

    sl_my92x1_init();
    sysCfg.led_scheme = 0;
  }

  sl_power = 0;
  sl_any = 0;
  sl_wakeupActive = 0;
}

void sl_setColorTemp(uint16_t ct)
{
/* Color Temperature (https://developers.meethue.com/documentation/core-concepts)
 *
 * ct = 153 = 2000K = Warm = CCWW = 00FF
 * ct = 500 = 6500K = Cold = CCWW = FF00
 */
  uint16_t my_ct = ct - 153;
  if (my_ct > 347) {
    my_ct = 347;
  }
  uint16_t icold = (100 * (347 - my_ct)) / 136;
  uint16_t iwarm = (100 * my_ct) / 136;
  if (5 == sfl_flg) {
    sysCfg.led_color[0] = 0;
    sysCfg.led_color[1] = 0;
    sysCfg.led_color[2] = 0;
    sysCfg.led_color[3] = (uint8_t)icold;
    sysCfg.led_color[4] = (uint8_t)iwarm;
  } else {
    sysCfg.led_color[0] = (uint8_t)icold;
    sysCfg.led_color[1] = (uint8_t)iwarm;
  }
}

uint16_t sl_getColorTemp()
{
  uint8_t ct_idx = 0;
  if (5 == sfl_flg) {
    ct_idx = 3;
  }
  uint16_t my_ct = sysCfg.led_color[ct_idx +1];
  if (my_ct > 0) {
    return ((my_ct * 136) / 100) + 154;
  } else {
    my_ct = sysCfg.led_color[ct_idx];
    return 499 - ((my_ct * 136) / 100);
  }
}

void sl_setDim(uint8_t myDimmer)
{
  float temp;

  if ((1 == sfl_flg) && (100 == myDimmer)) {
    myDimmer = 99;  // BN-SZ01 starts flickering at dimmer = 100
  }
  float newDim = 100 / (float)myDimmer;
  for (byte i = 0; i < sfl_flg; i++) {
    temp = (float)sysCfg.led_color[i] / newDim;
    sl_dcolor[i] = (uint8_t)temp;
  }
}

void sl_setColor()
{
  uint8_t highest = 0;
  float temp;

  for (byte i = 0; i < sfl_flg; i++) {
    if (highest < sl_dcolor[i]) {
      highest = sl_dcolor[i];
    }
  }
  float mDim = (float)highest / 2.55;
  sysCfg.led_dimmer[0] = (uint8_t)mDim;
  float newDim = 100 / mDim;
  for (byte i = 0; i < sfl_flg; i++) {
    temp = (float)sl_dcolor[i] * newDim;
    sysCfg.led_color[i] = (uint8_t)temp;
  }
}

char* sl_getColor(char* scolor)
{
  sl_setDim(sysCfg.led_dimmer[0]);
  scolor[0] = '\0';
  for (byte i = 0; i < sfl_flg; i++) {
    snprintf_P(scolor, 11, PSTR("%s%02X"), scolor, sl_dcolor[i]);
  }
  return scolor;
}

void sl_prepPower()
{
  char scolor[11];

//  do_cmnd_power(index, (sysCfg.led_dimmer[0]>0));
  if (sysCfg.led_dimmer[0] && !(sl_power)) {
    do_cmnd_power(Maxdevice, 7);  // No publishPowerState
  }
  else if (!sysCfg.led_dimmer[0] && sl_power) {
    do_cmnd_power(Maxdevice, 6);  // No publishPowerState
  }
#ifdef USE_DOMOTICZ
//  mqtt_publishDomoticzPowerState(1);
  domoticz_updatePowerState(Maxdevice);
#endif  // USE_DOMOTICZ
  if (sfl_flg > 1) {
    snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("{\"" D_RSLT_POWER "\":\"%s\", \"" D_CMND_DIMMER "\":%d, \"" D_CMND_COLOR "\":\"%s\"}"),
      getStateText(sl_power), sysCfg.led_dimmer[0], sl_getColor(scolor));
  } else {
    snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("{\"" D_RSLT_POWER "\":\"%s\", \"" D_CMND_DIMMER "\":%d}"),
      getStateText(sl_power), sysCfg.led_dimmer[0]);
  }
}

void sl_setPower(uint8_t mpower)
{
  sl_power = ((mpower & (0x01 << (Maxdevice -1))) != 0);
  if (sl_wakeupActive) {
    sl_wakeupActive--;
  }
  sl_animate();
}

void sl_animate()
{
// {"Wakeup":"Done"}
  uint8_t fadeValue;
  uint8_t cur_col[5];

  stripTimerCntr++;
  if (!sl_power) {  // Power Off
    sleep = sysCfg.sleep;
    stripTimerCntr = 0;
    for (byte i = 0; i < sfl_flg; i++) {
      sl_tcolor[i] = 0;
    }
  }
  else {
    sleep = 0;
    switch (sysCfg.led_scheme) {
      case 0:  // Power On
        sl_setDim(sysCfg.led_dimmer[0]);  // Power On
        if (0 == sysCfg.led_fade) {
          for (byte i = 0; i < sfl_flg; i++) {
            sl_tcolor[i] = sl_dcolor[i];
          }
        } else {
          for (byte i = 0; i < sfl_flg; i++) {
            if (sl_tcolor[i] != sl_dcolor[i]) {
              if (sl_tcolor[i] < sl_dcolor[i]) {
                sl_tcolor[i] += ((sl_dcolor[i] - sl_tcolor[i]) >> sysCfg.led_speed) +1;
              }
              if (sl_tcolor[i] > sl_dcolor[i]) {
                sl_tcolor[i] -= ((sl_tcolor[i] - sl_dcolor[i]) >> sysCfg.led_speed) +1;
              }
            }
          }
        }
        break;
      case 1:  // Power On using wake up duration
        if (2 == sl_wakeupActive) {
          sl_wakeupActive = 1;
          for (byte i = 0; i < sfl_flg; i++) {
            sl_tcolor[i] = 0;
          }
          sl_wakeupCntr = 0;
          sl_wakeupDimmer = 0;
        }
        sl_wakeupCntr++;
        if (sl_wakeupCntr > ((sysCfg.led_wakeup * STATES) / sysCfg.led_dimmer[0])) {
          sl_wakeupCntr = 0;
          sl_wakeupDimmer++;
          if (sl_wakeupDimmer <= sysCfg.led_dimmer[0]) {
            sl_setDim(sl_wakeupDimmer);
            for (byte i = 0; i < sfl_flg; i++) {
              sl_tcolor[i] = sl_dcolor[i];
            }
          } else {
            snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("{\"" D_CMND_WAKEUP "\":\"" D_DONE "\"}"));
            mqtt_publish_topic_P(2, PSTR(D_CMND_WAKEUP));
            sl_wakeupActive = 0;
            sysCfg.led_scheme = 0;
          }
        }
        break;
#ifdef USE_WS2812  // ************************************************************************
      case 2:  // Clock
        if (((STATES/10)*2 == state) || (sl_any != 2)) {
          ws2812_clock();
        }
        sl_any = 2;
        break;
      default:
        if (1 == sysCfg.led_fade) {
          ws2812_gradient();
        } else {
          ws2812_bars();
        }
        sl_any = 1;
        break;
#endif  // USE_WS2812 ************************************************************************
    }
  }

  if ((sysCfg.led_scheme < 2) || !sl_power) {
    for (byte i = 0; i < sfl_flg; i++) {
      if (sl_lcolor[i] != sl_tcolor[i]) {
        sl_any = 1;
      }
    }
    if (sl_any) {
      sl_any = 0;
      for (byte i = 0; i < sfl_flg; i++) {
        sl_lcolor[i] = sl_tcolor[i];
        cur_col[i] = (sysCfg.led_table) ? ledTable[sl_lcolor[i]] : sl_lcolor[i];
        if (sfl_flg < 3) {
          if (pin[GPIO_PWM1 +i] < 99) {
            analogWrite(pin[GPIO_PWM1 +i], cur_col[i] * (PWM_RANGE / 255));
          }
        }
      }
#ifdef USE_WS2812  // ************************************************************************
      if (3 == sfl_flg) {
        RgbColor lcolor;
        lcolor.R = cur_col[0];
        lcolor.G = cur_col[1];
        lcolor.B = cur_col[2];
        for (uint16_t i = 0; i < sysCfg.led_pixels; i++) {
          strip->SetPixelColor(i, lcolor);
        }
        strip->Show();
      }
#endif  // USE_ES2812 ************************************************************************
      if (sfl_flg > 3) {
        sl_my92x1_duty(cur_col[0], cur_col[1], cur_col[2], cur_col[3], cur_col[4]);
      }
    }
  }
}

/*********************************************************************************************\
 * Hue support
\*********************************************************************************************/

void sl_rgb2hsb(float *hue, float *sat, float *bri)
{
  RgbColor dcolor;

  sl_setDim(sysCfg.led_dimmer[0]);
  dcolor.R = sl_dcolor[0];
  dcolor.G = sl_dcolor[1];
  dcolor.B = sl_dcolor[2];
  HsbColor hsb = HsbColor(dcolor);
  *hue = hsb.H;
  *sat = hsb.S;
  *bri = hsb.B;
}

/********************************************************************************************/

void sl_replaceHSB(String *response)
{
  float hue;
  float sat;
  float bri;

  if (sfl_flg > 2) {
    sl_rgb2hsb(&hue, &sat, &bri);
    response->replace("{h}", String((uint16_t)(65535.0f * hue)));
    response->replace("{s}", String((uint8_t)(254.0f * sat)));
    response->replace("{b}", String((uint8_t)(254.0f * bri)));
  } else {
    response->replace("{h}", "0");
    response->replace("{s}", "0");
//    response->replace("{b}", String((uint8_t)(2.54f * (float)sysCfg.led_dimmer[0])));
    response->replace("{b}", String((uint8_t)(0.01f * (float)sysCfg.led_dimmer[0])));
  }
}

void sl_getHSB(float *hue, float *sat, float *bri)
{
  if (sfl_flg > 2) {
    sl_rgb2hsb(hue, sat, bri);
  } else {
    *hue = 0;
    *sat = 0;
//    *bri = (2.54f * (float)sysCfg.led_dimmer[0]);
    *bri = (0.01f * (float)sysCfg.led_dimmer[0]);
  }
}

void sl_setHSB(float hue, float sat, float bri, uint16_t ct)
{
  HsbColor hsb;

/*
  char stemp1[10];
  char stemp2[10];
  char stemp3[10];
  dtostrfi(hue, 3, stemp1);
  dtostrfi(sat, 3, stemp2);
  dtostrfi(bri, 3, stemp3);
  snprintf_P(log_data, sizeof(log_data), PSTR("HUE: Set Hue %s, Sat %s, Bri %s, Ct %d"), stemp1, stemp2, stemp3, ct);
  addLog(LOG_LEVEL_DEBUG);
*/

  if (sfl_flg > 2) {
    if ((5 == sfl_flg) && (ct > 0)) {
      sl_setColorTemp(ct);
    } else {
      hsb.H = hue;
      hsb.S = sat;
      hsb.B = bri;
      RgbColor tmp = RgbColor(hsb);
      sl_dcolor[0] = tmp.R;
      sl_dcolor[1] = tmp.G;
      sl_dcolor[2] = tmp.B;
      sl_setColor();
    }
    sl_prepPower();
    mqtt_publish_topic_P(5, PSTR(D_CMND_COLOR));
  } else {
    uint8_t tmp = (uint8_t)(bri * 100);
    sysCfg.led_dimmer[0] = tmp;
    if (2 == sfl_flg) {
      if (ct > 0) {
        sl_setColorTemp(ct);
      }
      sl_prepPower();
      mqtt_publish_topic_P(5, PSTR(D_CMND_COLOR));
    } else {
      sl_prepPower();
      mqtt_publish_topic_P(5, PSTR(D_CMND_DIMMER));
    }
  }
}

/*********************************************************************************************\
 * Commands
\*********************************************************************************************/

boolean sl_command(char *type, uint16_t index, char *dataBuf, uint16_t data_len, int16_t payload)
{
  boolean serviced = true;
  boolean coldim = false;
  char scolor[11];
  char *p;

  if ((sfl_flg > 1) && !strcasecmp_P(type, PSTR(D_CMND_COLOR))) {
    if (dataBuf[0] == '#') {
      dataBuf++;
      data_len--;
    }
    if ((2 * sfl_flg) == data_len) {
      for (byte i = 0; i < sfl_flg; i++) {
        strlcpy(scolor, dataBuf + (i *2), 3);
        sl_dcolor[i] = (uint8_t)strtol(scolor, &p, 16);
      }
      sl_setColor();
      coldim = true;
    } else {
      snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("{\"" D_CMND_COLOR "\":\"%s\"}"), sl_getColor(scolor));
    }
  }
#ifdef USE_WS2812  //  ***********************************************************************
  else if ((3 == sfl_flg) && !strcasecmp_P(type, PSTR(D_CMND_LED)) && (index > 0) && (index <= sysCfg.led_pixels)) {
    if (dataBuf[0] == '#') {
      dataBuf++;
      data_len--;
    }
    if ((2 * sfl_flg) == data_len) {
      for (byte i = 0; i < sfl_flg; i++) {
        strlcpy(scolor, dataBuf + (i *2), 3);
        sl_ledcolor[i] = (uint8_t)strtol(scolor, &p, 16);
      }
      ws2812_setLedColor(index);
    }
    snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("{\"" D_CMND_LED "%d\":\"%s\"}"), index, ws2812_getLedColor(index, scolor));
  }
  else if ((3 == sfl_flg) && !strcasecmp_P(type, PSTR(D_CMND_PIXELS))) {
    if ((payload > 0) && (payload <= WS2812_MAX_LEDS)) {
      sysCfg.led_pixels = payload;
      ws2812_pixels();
    }
    snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("{\"" D_CMND_PIXELS "\":%d}"), sysCfg.led_pixels);
  }
  else if ((3 == sfl_flg) && !strcasecmp_P(type, PSTR(D_CMND_WIDTH))) {
    if ((payload >= 0) && (payload <= 4)) {
      sysCfg.led_width = payload;
    }
    snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("{\"" D_CMND_WIDTH "\":%d}"), sysCfg.led_width);
  }
  else if ((3 == sfl_flg) && !strcasecmp_P(type, PSTR(D_CMND_SCHEME))) {
    if ((payload >= 0) && (payload <= 9)) {
      sysCfg.led_scheme = payload;
      if (1 == sysCfg.led_scheme) {
        sl_wakeupActive = 3;
      }
      do_cmnd_power(Maxdevice, 1);
      stripTimerCntr = 0;
    }
    snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("{\"" D_CMND_SCHEME "\":%d}"), sysCfg.led_scheme);
  }
#endif  // USE_WS2812 ************************************************************************
  else if (!strcasecmp_P(type, PSTR(D_CMND_COLORTEMPERATURE)) && ((2 == sfl_flg) || (5 == sfl_flg))) { // ColorTemp
    if ((payload >= 153) && (payload <= 500)) {  // https://developers.meethue.com/documentation/core-concepts
      sl_setColorTemp(payload);
      coldim = true;
    } else {
      snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("{\"" D_CMND_COLORTEMPERATURE "\":%d}"), sl_getColorTemp());
    }
  }
  else if (!strcasecmp_P(type, PSTR(D_CMND_DIMMER))) {
    if ((payload >= 0) && (payload <= 100)) {
      sysCfg.led_dimmer[0] = payload;
      coldim = true;
    } else {
      snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("{\"" D_CMND_DIMMER "\":%d}"), sysCfg.led_dimmer[0]);
    }
  }
  else if (!strcasecmp_P(type, PSTR(D_CMND_LEDTABLE))) {
    if ((payload >= 0) && (payload <= 2)) {
      switch (payload) {
      case 0: // Off
      case 1: // On
        sysCfg.led_table = payload;
        break;
      case 2: // Toggle
        sysCfg.led_table ^= 1;
        break;
      }
      sl_any = 1;
    }
    snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("{\"" D_CMND_LEDTABLE "\":\"%s\"}"), getStateText(sysCfg.led_table));
  }
  else if (!strcasecmp_P(type, PSTR(D_CMND_FADE))) {
    switch (payload) {
    case 0: // Off
    case 1: // On
      sysCfg.led_fade = payload;
      break;
    case 2: // Toggle
      sysCfg.led_fade ^= 1;
      break;
    }
    snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("{\"" D_CMND_FADE "\":\"%s\"}"), getStateText(sysCfg.led_fade));
  }
  else if (!strcasecmp_P(type, PSTR(D_CMND_SPEED))) {  // 1 - fast, 8 - slow
    if ((payload > 0) && (payload <= 8)) {
      sysCfg.led_speed = payload;
    }
    snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("{\"" D_CMND_SPEED "\":%d}"), sysCfg.led_speed);
  }
  else if (!strcasecmp_P(type, PSTR(D_CMND_WAKEUPDURATION))) {
    if ((payload > 0) && (payload < 3001)) {
      sysCfg.led_wakeup = payload;
      sl_wakeupActive = 0;
    }
    snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("{\"" D_CMND_WAKEUPDURATION "\":%d}"), sysCfg.led_wakeup);
  }
  else if (!strcasecmp_P(type, PSTR(D_CMND_WAKEUP))) {
    if ((payload >= 0) && (payload <= 100)) {
      sysCfg.led_dimmer[0] = payload;
    }
    sl_wakeupActive = 3;
    sysCfg.led_scheme = 1;
    do_cmnd_power(Maxdevice, 1);
    snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("{\"" D_CMND_WAKEUP "\":\"" D_STARTED "\"}"));
  }
  else if (!strcasecmp_P(type, PSTR("UNDOCA"))) {  // Theos legacy status
    sl_getColor(scolor);
    scolor[6] = '\0';  // RGB only
    snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s, %d, %d, %d, %d, %d"),
      scolor, sysCfg.led_fade, sysCfg.led_table, sysCfg.led_scheme, sysCfg.led_speed, sysCfg.led_width);
    mqtt_publish_topic_P(1, type);
    mqtt_data[0] = '\0';
  }
  else {
    serviced = false;  // Unknown command
  }
  if (coldim) {
    sl_prepPower();
  }
  return serviced;
}

