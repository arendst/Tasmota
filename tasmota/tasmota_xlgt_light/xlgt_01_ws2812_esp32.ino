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

#ifdef ESP32
#ifdef USE_LIGHT
#if defined(USE_WS2812) && !defined(USE_WS2812_FORCE_NEOPIXELBUS)

/*********************************************************************************************\
 * WS2812 RGB / RGBW Leds using NeopixelBus library
 *
 * light_scheme  WS2812  3+ Colors  1+2 Colors  Effect
 * ------------  ------  ---------  ----------  -----------------
 *  0 (5)        yes     no         no          Clock
 *  1 (6)        yes     no         no          Incandescent
 *  2 (7)        yes     no         no          RGB
 *  3 (8)        yes     no         no          Christmas
 *  4 (9)        yes     no         no          Hanukkah
 *  5 (10)       yes     no         no          Kwanzaa
 *  6 (11)       yes     no         no          Rainbow
 *  7 (12)       yes     no         no          Fire
 *  8 (13)       yes     no         no          Stairs
 *  9 (14)       yes     no         no          Clear (= Berry)
 * 10 (15)       yes     no         no          Optional DDP
\*********************************************************************************************/

#define XLGT_01             1

const uint8_t WS2812_SCHEMES = 10;      // Number of WS2812 schemes

const char kWs2812Commands[] PROGMEM = "|"  // No prefix
  D_CMND_LED "|" D_CMND_PIXELS "|" D_CMND_ROTATION "|" D_CMND_WIDTH "|" D_CMND_STEPPIXELS "|" D_CMND_PIXELTYPE ;

void (* const Ws2812Command[])(void) PROGMEM = {
  &CmndLed, &CmndPixels, &CmndRotation, &CmndWidth, &CmndStepPixels, &CmndPixelType };

#include <TasmotaLED.h>

const uint16_t kLedType = 0;
// select the right pixel size
#if (USE_WS2812_CTYPE > NEO_3LED)
  const uint16_t kTasLed_PixelSize = TasmotaLed_4_WRGB;
#else
  const uint16_t kTasLed_PixelSize = TasmotaLed_3_RGB;
#endif

// select the right pixel order
#if (USE_WS2812_CTYPE == NEO_GRB)
  const uint16_t kTasLed_PixelOrder = TasmotaLed_GRB;
#elif (USE_WS2812_CTYPE == NEO_BRG)
  const uint16_t kTasLed_PixelOrder = TasmotaLed_BRG;
#elif (USE_WS2812_CTYPE == NEO_RBG)
  const uint16_t kTasLed_PixelOrder = TasmotaLed_RBG;
  #elif (USE_WS2812_CTYPE == NEO_RGBW)
  const uint16_t kTasLed_PixelOrder = TasmotaLed_RGB;
#elif (USE_WS2812_CTYPE == NEO_GRBW)
  const uint16_t kTasLed_PixelOrder = TasmotaLed_GRB;
#else
  const uint16_t kTasLed_PixelOrder = TasmotaLed_RGB;
#endif

// all leds have always W at the end
const uint16_t kTasLed_PixelWhite = TasmotaLed_xxxW;

// We drop support for NEO_HW_P9813, as it is too hard to support with hardwarre
#if (USE_WS2812_HARDWARE == NEO_HW_P9813)
  #error "P9813 is not supported by this library"
#endif  // USE_WS2812_CTYPE

// select timing
#if (USE_WS2812_HARDWARE == NEO_HW_WS2812X)
  const uint16_t kTasLed_Timing = TasmotaLed_WS2812;
#elif (USE_WS2812_HARDWARE == NEO_HW_SK6812)
  const uint16_t kTasLed_Timing = TasmotaLed_SK6812;
#elif (USE_WS2812_HARDWARE == NEO_HW_APA106)
  #error "APA106 is not supported by this library"
#else   // USE_WS2812_HARDWARE
  const uint16_t kTasLed_Timing = TasmotaLed_WS2812;
#endif  // USE_WS2812_HARDWARE

const uint16_t kTasLed_Type = kTasLed_PixelSize | kTasLed_PixelOrder | kTasLed_PixelWhite | kTasLed_Timing;

// select hardware acceleration - bitbanging is not supported on ESP32 due to interference of interrupts - use default
const uint32_t kTasLed_Hardware = TasmotaLed_HW_Default;   // use whatever is available

#if (USE_WS2812_HARDWARE == NEO_HW_P9813)
  #error "P9813 is not supported by this library"
#endif


/*******************************************************************************************\
 * Support for TasmotaLED
 * 
 * From here we have defined:
 *    kTasLed_Type: encodes pixel size, pixel order, pixel white, timing
 *    kTasLed_Hardware: encodes the harware support to push to Leds: RMT, SPI, I2S
 *******************************************************************************************/
TasmotaLED *strip = nullptr;

typedef union LedColor {
    uint32_t C;               // encoded as 0xWWRRGGBB
    struct {
      uint8_t B, G, R, W;     // WRGB in little endian
    };
} LedColor;

struct ColorScheme {
  const LedColor* colors;
  uint8_t count;
};

const LedColor kIncandescent[2] = { 0xFF8C14, 0x000000 };
const LedColor kRgb[3] = { 0xFF0000, 0x00FF00, 0x0000FF };
const LedColor kChristmas[2] = { 0xFF0000, 0x00FF00 };
const LedColor kHanukkah[2] = { 0x0000FF, 0xFFFFFF };
const LedColor kwanzaa[3] = { 0xFF0000, 0x000000, 0x00FF00 };
const LedColor kRainbow[7] = { 0xFF0000, 0xFF8000, 0xFFFF00, 0x00FF00, 0x0000FF, 0x8000FF, 0xFF00FF };
const LedColor kFire[3] = { 0xFF0000, 0xFF6600, 0xFFC000 };
const LedColor kStairs[2] = { 0x000000, 0xFFFFFF0 };

const ColorScheme kSchemes[WS2812_SCHEMES -2] = {  // Skip clock and clear scheme
  kIncandescent, 2,
  kRgb, 3,
  kChristmas, 2,
  kHanukkah, 2,
  kwanzaa, 3,
  kRainbow, 7,
  kFire, 3,
  kStairs, 2
};

const uint8_t kWidth[5] = {
    1,     // Small
    2,     // Medium
    4,     // Large
    8,     // Largest
  255     // All
};
const uint8_t kWsRepeat[5] = {
    8,     // Small
    6,     // Medium
    4,     // Large
    2,     // Largest
    1      // All
};

struct WS2812 {
  uint8_t show_next = 1;
  uint8_t scheme_offset = 0;
  bool suspend_update = false;
} Ws2812;

/********************************************************************************************/

// For some reason map fails to compile so renamed to wsmap
long wsmap(long x, long in_min, long in_max, long out_min, long out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void Ws2812LibStripShow(void) {
  strip->Show();
}

void Ws2812StripShow(void)
{
  LedColor c;

  if (Settings->light_correction) {
    for (uint32_t i = 0; i < Settings->light_pixels; i++) {
      c.C = strip->GetPixelColor(i);
      c.R = ledGamma(c.R);
      c.G = ledGamma(c.G);
      c.B = ledGamma(c.B);
#if (USE_WS2812_CTYPE > NEO_3LED)
      c.W = ledGamma(c.W);
#endif
      strip->SetPixelColor(i, c.C);
    }
  }
  Ws2812LibStripShow();
}

int mod(int a, int b)
{
   int ret = a % b;
   if (ret < 0) ret += b;
   return ret;
}

void Ws2812UpdatePixelColor(int position, LedColor hand_color, float offset);
void Ws2812UpdatePixelColor(int position, LedColor hand_color, float offset)
{
  LedColor color;

  uint32_t mod_position = mod(position, (int)Settings->light_pixels);

  color.C = strip->GetPixelColor(mod_position);
  float dimmer = 100 / (float)Settings->light_dimmer;
  color.R = tmin(color.R + ((hand_color.R / dimmer) * offset), 255);
  color.G = tmin(color.G + ((hand_color.G / dimmer) * offset), 255);
  color.B = tmin(color.B + ((hand_color.B / dimmer) * offset), 255);
  strip->SetPixelColor(mod_position, color.C);
}

void Ws2812UpdateHand(int position, uint32_t index)
{
  uint32_t width = Settings->light_width;
  if (index < WS_MARKER) { width = Settings->ws_width[index]; }
  if (!width) { return; }  // Skip

  position = (position + Settings->light_rotation) % Settings->light_pixels;

  if (Settings->flag.ws_clock_reverse) {  // SetOption16 - Switch between clockwise or counter-clockwise
    position = Settings->light_pixels -position;
  }
  LedColor hand_color = {0};
  hand_color.R = Settings->ws_color[index][WS_RED];
  hand_color.G = Settings->ws_color[index][WS_GREEN];
  hand_color.B = Settings->ws_color[index][WS_BLUE];

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
  int clksize = 60000 / (int)Settings->light_pixels;

  Ws2812UpdateHand((RtcTime.second * 1000) / clksize, WS_SECOND);
  Ws2812UpdateHand((RtcTime.minute * 1000) / clksize, WS_MINUTE);
  Ws2812UpdateHand((((RtcTime.hour % 12) * 5000) + ((RtcTime.minute * 1000) / 12 )) / clksize, WS_HOUR);
  if (Settings->ws_color[WS_MARKER][WS_RED] + Settings->ws_color[WS_MARKER][WS_GREEN] + Settings->ws_color[WS_MARKER][WS_BLUE]) {
    for (uint32_t i = 0; i < 12; i++) {
      Ws2812UpdateHand((i * 5000) / clksize, WS_MARKER);
    }
  }

  Ws2812StripShow();
}

void Ws2812GradientColor(uint32_t schemenr, LedColor* mColor, uint32_t range, uint32_t gradRange, uint32_t i);
void Ws2812GradientColor(uint32_t schemenr, LedColor* mColor, uint32_t range, uint32_t gradRange, uint32_t i)
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
  float dimmer = 100 / (float)Settings->light_dimmer;
  float fmyRed = (float)wsmap(rangeIndex % gradRange, 0, gradRange, scheme.colors[start].R, scheme.colors[end].R) / dimmer;
  float fmyGrn = (float)wsmap(rangeIndex % gradRange, 0, gradRange, scheme.colors[start].G, scheme.colors[end].G) / dimmer;
  float fmyBlu = (float)wsmap(rangeIndex % gradRange, 0, gradRange, scheme.colors[start].B, scheme.colors[end].B) / dimmer;
  mColor->R = (uint8_t)fmyRed;
  mColor->G = (uint8_t)fmyGrn;
  mColor->B = (uint8_t)fmyBlu;
}

void Ws2812Gradient(uint32_t schemenr)
{
/*
 * This routine courtesy Tony DiCola (Adafruit)
 * Display a gradient of colors for the current color scheme.
 *  Repeat is the number of repetitions of the gradient (pick a multiple of 2 for smooth looping of the gradient).
 */
  LedColor c;

  ColorScheme scheme = kSchemes[schemenr];
  if (scheme.count < 2) { return; }

  uint32_t repeat = kWsRepeat[Settings->light_width];  // number of scheme.count per ledcount
  uint32_t range = (uint32_t)ceil((float)Settings->light_pixels / (float)repeat);
  uint32_t gradRange = (uint32_t)ceil((float)range / (float)(scheme.count - 1));
  uint32_t speed = ((Settings->light_speed * 2) -1) * (STATES / 10);
  uint32_t offset = speed > 0 ? Light.strip_timer_counter / speed : 0;

  LedColor oldColor, currentColor;
  Ws2812GradientColor(schemenr, &oldColor, range, gradRange, offset);
  currentColor = oldColor;
  speed = speed ? speed : 1;    // should never happen, just avoid div0
  for (uint32_t i = 0; i < Settings->light_pixels; i++) {
    if (kWsRepeat[Settings->light_width] > 1) {
      Ws2812GradientColor(schemenr, &currentColor, range, gradRange, i + offset + 1);
    }
    // Blend old and current color based on time for smooth movement.
    c.R = wsmap(Light.strip_timer_counter % speed, 0, speed, oldColor.R, currentColor.R);
    c.G = wsmap(Light.strip_timer_counter % speed, 0, speed, oldColor.G, currentColor.G);
    c.B = wsmap(Light.strip_timer_counter % speed, 0, speed, oldColor.B, currentColor.B);
    strip->SetPixelColor(i, c.C);
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
  LedColor c;

  ColorScheme scheme = kSchemes[schemenr];

  uint32_t maxSize = Settings->light_pixels / scheme.count;
  if (kWidth[Settings->light_width] > maxSize) { maxSize = 0; }

  uint32_t speed = ((Settings->light_speed * 2) -1) * (STATES / 10);
  uint32_t offset = (speed > 0) ? Light.strip_timer_counter / speed : 0;

  LedColor mcolor[scheme.count];
  memcpy(mcolor, scheme.colors, sizeof(mcolor));
  float dimmer = 100 / (float)Settings->light_dimmer;
  for (uint32_t i = 0; i < scheme.count; i++) {
    float fmyRed = (float)mcolor[i].R / dimmer;
    float fmyGrn = (float)mcolor[i].G / dimmer;
    float fmyBlu = (float)mcolor[i].B / dimmer;
    mcolor[i].R = (uint8_t)fmyRed;
    mcolor[i].G = (uint8_t)fmyGrn;
    mcolor[i].B = (uint8_t)fmyBlu;
  }
  uint32_t colorIndex = offset % scheme.count;
  for (uint32_t i = 0; i < Settings->light_pixels; i++) {
    if (maxSize) { colorIndex = ((i + offset) % (scheme.count * kWidth[Settings->light_width])) / kWidth[Settings->light_width]; }
    c.R = mcolor[colorIndex].R;
    c.G = mcolor[colorIndex].G;
    c.B = mcolor[colorIndex].B;
    strip->SetPixelColor(i, c.C);
  }
  Ws2812StripShow();
}

void Ws2812Steps(uint32_t schemenr) {
  LedColor c;

  ColorScheme scheme = kSchemes[schemenr];
	// apply main color if current sheme == kStairs
	if (scheme.colors == kStairs) {
    // we patch the colors
    static LedColor colors_stairs[2] = {
      { 0x000000 },
      { .B = Settings->light_color[2], .G = Settings->light_color[1], .R = Settings->light_color[0] }
    };
    scheme.colors = colors_stairs;
	}

	uint8_t scheme_count = scheme.count;
	if (Settings->light_fade) {
		scheme_count = Settings->ws_width[WS_HOUR];  // Width4
	}
  if (scheme_count < 2) {
    scheme_count = 2;
  }

  LedColor mcolor[scheme_count];

	uint8_t color_start = 0;
	uint8_t color_end = 1;
	if (Settings->light_rotation & 0x01) {
		color_start = 1;
		color_end = 0;
	}

	if (Settings->light_fade) {
		// generate gradient (width = Width4)
		for (uint32_t i = 1; i < scheme_count - 1; i++) {
			mcolor[i].R = (uint8_t) wsmap(i, 0, scheme_count, scheme.colors[color_start].R, scheme.colors[color_end].R);
			mcolor[i].G = (uint8_t) wsmap(i, 0, scheme_count, scheme.colors[color_start].G, scheme.colors[color_end].G);
			mcolor[i].B = (uint8_t) wsmap(i, 0, scheme_count, scheme.colors[color_start].B, scheme.colors[color_end].B);
		}
	} else {
		memcpy(mcolor, scheme.colors, sizeof(mcolor));
	}
	// Repair first & last color in gradient; apply scheme rotation if fade==0
	mcolor[0].R = scheme.colors[color_start].R;
	mcolor[0].G = scheme.colors[color_start].G;
	mcolor[0].B = scheme.colors[color_start].B;
	mcolor[scheme_count-1].R = scheme.colors[color_end].R;
	mcolor[scheme_count-1].G = scheme.colors[color_end].G;
	mcolor[scheme_count-1].B = scheme.colors[color_end].B;

	// Adjust to dimmer value
  float dimmer = 100 / (float)Settings->light_dimmer;
  for (uint32_t i = 0; i < scheme_count; i++) {
    float fmyRed = (float)mcolor[i].R / dimmer;
    float fmyGrn = (float)mcolor[i].G / dimmer;
    float fmyBlu = (float)mcolor[i].B / dimmer;
    mcolor[i].R = (uint8_t)fmyRed;
    mcolor[i].G = (uint8_t)fmyGrn;
    mcolor[i].B = (uint8_t)fmyBlu;
  }

  uint32_t speed = Settings->light_speed;
	int32_t current_position = Light.strip_timer_counter / speed;

	//all pixels are shown already | rotation change will not change current state
	if (current_position >  Settings->light_pixels / Settings->light_step_pixels + scheme_count ) {
		return;
	}

  int32_t colorIndex;
  int32_t step_nr;

  for (uint32_t i = 0; i < Settings->light_pixels; i++) {
		step_nr = i / Settings->light_step_pixels;
		colorIndex = current_position - step_nr;
	  if (colorIndex < 0) { colorIndex = 0; }
		if (colorIndex > scheme_count - 1) { colorIndex = scheme_count - 1; }
    c.R = mcolor[colorIndex].R;
    c.G = mcolor[colorIndex].G;
    c.B = mcolor[colorIndex].B;
		// Adjust the scheme rotation
		if (Settings->light_rotation & 0x02) {
			strip->SetPixelColor(Settings->light_pixels - i - 1, c.C);
		} else {
			strip->SetPixelColor(i, c.C);
		}
  }
  Ws2812StripShow();
}

#ifdef USE_NETWORK_LIGHT_SCHEMES
void Ws2812DDP(void)
{
  LedColor c = {0};

  // Can't be trying to initialize UDP too early.
  if (TasmotaGlobal.restart_flag || TasmotaGlobal.global_state.network_down) return;

  // Start DDP listener, if fail, just set last ddp_color
  if (!ddp_udp_up) {
    if (!ddp_udp.begin(4048)) return;
    ddp_udp_up = 1;
    AddLog(LOG_LEVEL_DEBUG_MORE, "DDP: UDP Listener Started: WS2812 Scheme");
  }

  // Get the DDP payload over UDP
  std::vector<uint8_t> payload;
  while (uint16_t packet_size = ddp_udp.parsePacket()) {
    payload.resize(packet_size);
    if (!ddp_udp.read(&payload[0], payload.size())) {
      continue;
    }
  }

  // No verification checks performed against packet besides length
  if (payload.size() > (9+3*Settings->light_pixels)) {
    for (uint32_t i = 0; i < Settings->light_pixels; i++) {
      c.R = payload[10+3*i];
      c.G = payload[11+3*i];
      c.B = payload[12+3*i];
      strip->SetPixelColor(i, c.C);
    }
    Ws2812StripShow();
  }
}
#endif  // USE_NETWORK_LIGHT_SCHEMES

void Ws2812Clear(bool display = true);
void Ws2812Clear(bool display)
{
  strip->ClearTo(0);
  if (display) {
    Ws2812LibStripShow();
    Ws2812.show_next = 1;
  }
}

void Ws2812SetColor(uint32_t led, uint8_t red, uint8_t green, uint8_t blue, uint8_t white)
{
  LedColor lcolor = {0};

  lcolor.R = red;
  lcolor.G = green;
  lcolor.B = blue;
  if (led) {
    strip->SetPixelColor(led -1, lcolor.C);  // Led 1 is strip Led 0 -> substract offset 1
  } else {
//    strip->ClearTo(lcolor);  // Set WS2812_MAX_LEDS pixels
    for (uint32_t i = 0; i < Settings->light_pixels; i++) {
      strip->SetPixelColor(i, lcolor.C);
    }
  }

  if (!Ws2812.suspend_update) {
    Ws2812LibStripShow();
    Ws2812.show_next = 1;
  }
}

char* Ws2812GetColor(uint32_t led, char* scolor)
{
  uint8_t sl_ledcolor[4];

  LedColor lcolor;
  lcolor.C = strip->GetPixelColor(led -1);
  sl_ledcolor[0] = lcolor.R;
  sl_ledcolor[1] = lcolor.G;
  sl_ledcolor[2] = lcolor.B;
  sl_ledcolor[3] = lcolor.W;
  scolor[0] = '\0';
  for (uint32_t i = 0; i < Light.subtype; i++) {
    if (Settings->flag.decimal_text) {  // SetOption17 - Switch between decimal or hexadecimal output (0 = hexadecimal, 1 = decimal)
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
  Ws2812LibStripShow();
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
  uint32_t scheme = Settings->light_scheme - Ws2812.scheme_offset;

#ifdef USE_NETWORK_LIGHT_SCHEMES
  if ((scheme != 10) && (ddp_udp_up)) {
    ddp_udp.stop();
    ddp_udp_up = 0;
    AddLog(LOG_LEVEL_DEBUG_MORE, "DDP: UDP Stopped: WS2812 Scheme not DDP");
  }
#endif
  switch (scheme) {
    case 0:  // Clock
      if ((1 == TasmotaGlobal.state_250mS) || (Ws2812.show_next)) {
        Ws2812Clock();
        Ws2812.show_next = 0;
      }
      break;
    case 9:  // Clear
      if (Settings->light_scheme != Light.last_scheme) {
        Ws2812Clear();
      }
      break;
#ifdef USE_NETWORK_LIGHT_SCHEMES
    case 10:
      Ws2812DDP();
      break;
#endif  // USE_NETWORK_LIGHT_SCHEMES
    default:
			if(Settings->light_step_pixels > 0){
				Ws2812Steps(scheme -1);
			} else {
				if (1 == Settings->light_fade) {
					Ws2812Gradient(scheme -1);
				} else {
					Ws2812Bars(scheme -1);
				}
      }
      Ws2812.show_next = 1;
      break;
  }
}

// convert the Settings to a Led type compatible with TasmotaLED
uint16_t Ws2812SettingsToLedType(void) {
  uint16_t led_type = kTasLed_Type;         // default value from compile options
  if (Settings->mbflag2.light_pixels_order != 0) {
    led_type = (led_type & 0xFF00) | (Settings->mbflag2.light_pixels_order << 4)
                                    | (Settings->mbflag2.light_pixels_w_first ? TasmotaLed_Wxxx : 0)
                                    | (Settings->mbflag2.light_pixels_rgbw ? TasmotaLed_4_WRGB : TasmotaLed_3_RGB);
  }
  return led_type;
}

bool Ws2812InitStrip(void)
{
  if (strip != nullptr) {
    return true;
  }

  if (PinUsed(GPIO_WS2812)) {  // RGB led
    int32_t gpio = Pin(GPIO_WS2812);
    TasmotaLEDPusher * pusher = TasmotaLEDPusher::Create(kTasLed_Hardware, gpio);
    if (pusher == nullptr) {
      AddLog(LOG_LEVEL_ERROR, "LED: No hardware supported");
      return false;
    }
    uint16_t led_type = Ws2812SettingsToLedType();
    strip = new TasmotaLED(led_type, Settings->light_pixels);
    strip->SetPusher(pusher);
    strip->Begin();

    Ws2812Clear();
    return true;
  }
  return false;
}

bool Ws2812ChangePixelCount(void)
{
  if (strip == nullptr) {
    return true;
  }
  strip->SetPixelCount(Settings->light_pixels);
  Ws2812Clear();
  return true;
}

bool Ws2812ChangePixelType(bool clear)
{
  if (strip == nullptr) {
    return true;
  }
  uint16_t led_type = Ws2812SettingsToLedType();
  strip->SetPixelSubType(led_type & 0xFF);    // just submit the lower part
  if (clear) {
    Ws2812Clear();
  } else {
    Ws2812LibStripShow();
  }
  return true;
}

void Ws2812ModuleSelected(void)
{
  if (Ws2812InitStrip()) {
    Ws2812.scheme_offset = Light.max_scheme +1;
    Light.max_scheme += WS2812_SCHEMES;

#ifdef USE_NETWORK_LIGHT_SCHEMES
    Light.max_scheme++;
#endif

#if (USE_WS2812_CTYPE > NEO_3LED)
    TasmotaGlobal.light_type = LT_RGBW;
#else
    TasmotaGlobal.light_type = LT_RGB;
#endif
    TasmotaGlobal.light_driver = XLGT_01;
  }
}

#ifdef ESP32
#ifdef USE_BERRY
/********************************************************************************************/
// Callbacks for Berry driver
//
// Since we dont' want to export all the template stuff, we need to encapsulate the calls
// in plain functions
//
void *Ws2812GetStrip(void) {
  return strip;
}

void Ws2812Begin(void) {
  if (strip) { strip->Begin(); }
}

void Ws2812Show(void) {
  if (strip) { strip->Show(); }
}

uint32_t Ws2812PixelsSize(void) {
  if (strip) { return strip->PixelCount(); }
  return 0;
}

bool Ws2812CanShow(void) {
  if (strip) { return strip->CanShow(); }
  return false;
}

void Ws2812CanShowWait(void) {
  if (strip) {
    while (!strip->CanShow()) {
      yield();
    }
  }
}

bool Ws2812IsDirty(void) {
  if (strip) { return strip->IsDirty(); }
  return false;
}

void Ws2812Dirty(void) {
  if (strip) { strip->Dirty(); }
}

uint8_t * Ws2812Pixels(void) {
  if (strip) { return strip->Pixels(); }
  return nullptr;
}

size_t Ws2812PixelSize(void) {
  if (strip) { return strip->PixelSize(); }
  return 0;
}

size_t Ws2812PixelCount(void) {
  if (strip) { return strip->PixelCount(); }
  return 0;
}

void Ws2812ClearTo(uint8_t r, uint8_t g, uint8_t b, uint8_t w, int32_t from, int32_t to) {
  LedColor lcolor;
  lcolor.W = w;
  lcolor.R = r;
  lcolor.G = g;
  lcolor.B = b;
  if (strip) {
    if (from < 0) {
      strip->ClearTo(lcolor.C);
    } else {
      strip->ClearTo(lcolor.C, from, to);
    }
  }
}

void Ws2812SetPixelColor(uint32_t idx, uint8_t r, uint8_t g, uint8_t b, uint8_t w)
{
  LedColor lcolor;
  lcolor.W = w;
  lcolor.R = r;
  lcolor.G = g;
  lcolor.B = b;
  if (strip) {
    strip->SetPixelColor(idx, lcolor.C);
  }
}

uint32_t Ws2812GetPixelColor(uint32_t idx) {
  LedColor lcolor;
  if (strip) {
    lcolor.C = strip->GetPixelColor(idx);
    return lcolor.C;      // already encoded as WWRRGGBB
    // return (lcolor.W << 24) | (lcolor.R << 16) | (lcolor.G << 8) | lcolor.B;
  }
  return 0;
}

#endif  // ESP32
#endif  // USE_BERRY

/********************************************************************************************/

void CmndLed(void)
{
  if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= Settings->light_pixels)) {
    if (XdrvMailbox.data_len > 0) {
      char *p;
      uint16_t idx = XdrvMailbox.index;
      Ws2812ForceSuspend();
      for (char *color = strtok_r(XdrvMailbox.data, " ", &p); color; color = strtok_r(nullptr, " ", &p)) {
        if (LightColorEntry(color, strlen(color))) {
          Ws2812SetColor(idx, Light.entry_color[0], Light.entry_color[1], Light.entry_color[2], Light.entry_color[3]);
          idx++;
          if (idx > Settings->light_pixels) { break; }
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
  uint32_t parm[4] = { Settings->light_pixels, 0 /* reverse = 0 */,
                       0 /* height = 1 (minus 1) */, 0 /* alternate = 0 */ };
  if (ParseParameters(4, parm) > 0) {
    if ((parm[0] > 0) && (parm[0] <= WS2812_MAX_LEDS)) {
      Ws2812Clear();                     // Clear all known pixels
      Ws2812CanShowWait();
      Settings->light_pixels = parm[0];
      Settings->light_pixels_reverse = parm[1];
      Settings->light_pixels_height_1 = (parm[2] > 0) ? parm[2] - 1 : 0;
      Settings->light_pixels_alternate = parm[3];
      Ws2812ChangePixelCount();
      Light.update = true;
    }
  }
  Response_P(PSTR("{\"Pixels\":%i,\"PixelsReverse\":%i,\"PixelsHeight\":%i,\"PixelsAlternate\":%i}"),
    Settings->light_pixels, Settings->light_pixels_reverse, Settings->light_pixels_height_1 + 1, Settings->light_pixels_alternate);
}

void CmndPixelType(void)
{
  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload < 32)) {
    // Value is:
    // 0 = use compile-time option (default value)
    // 1..6 = Pixel Order: 1=GRB 2=RGB 3=RBG 4=BRG 5=BGR 6=GBR
    // Modifiers:
    // +8 = 4 channels RGBW strip - default is 3 channels RGB
    // +16 = W channel is sent first - default W channel is sent last
    uint32_t pixels_order = XdrvMailbox.payload & 0x07;
    uint32_t pixels_w_first = (XdrvMailbox.payload & 0x08) ? 1 : 0;
    uint32_t pixels_rgbw = (XdrvMailbox.payload & 0x10) ? 1 : 0;
    // changing number of channels requires a reboot
    bool reboot = pixels_rgbw != Settings->mbflag2.light_pixels_rgbw;
    if (reboot) {
      TasmotaGlobal.restart_flag = 2;             // force restart if number of channels changed
    }

    Settings->mbflag2.light_pixels_order = pixels_order;
    Settings->mbflag2.light_pixels_w_first = pixels_w_first;
    Settings->mbflag2.light_pixels_rgbw = (XdrvMailbox.payload & 0x10) ? 1 : 0;
    Ws2812ChangePixelType(reboot);
  }
  uint32_t pixel_type = 0;
  if (Settings->mbflag2.light_pixels_order != 0) {
    pixel_type = Settings->mbflag2.light_pixels_order | (Settings->mbflag2.light_pixels_w_first ? 0x08 : 0)
                                                      | (Settings->mbflag2.light_pixels_rgbw ? 0x10 : 0);
  }
  ResponseCmndNumber(pixel_type);
}

void CmndStepPixels(void)
{
  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 255)) {
    Settings->light_step_pixels = (XdrvMailbox.payload > WS2812_MAX_LEDS) ? WS2812_MAX_LEDS :  XdrvMailbox.payload;
    // Ws2812ReinitStrip();   -- not sure it's actually needed
    Light.update = true;
  }
  ResponseCmndNumber(Settings->light_step_pixels);
}


void CmndRotation(void)
{
  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload < Settings->light_pixels)) {
    Settings->light_rotation = XdrvMailbox.payload;
  }
  ResponseCmndNumber(Settings->light_rotation);
}

void CmndWidth(void)
{
  if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= 4)) {
    if (1 == XdrvMailbox.index) {
      if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 4)) {
        Settings->light_width = XdrvMailbox.payload;
      }
      ResponseCmndNumber(Settings->light_width);
    } else {
      if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload < 32)) {
        Settings->ws_width[XdrvMailbox.index -2] = XdrvMailbox.payload;
      }
      ResponseCmndIdxNumber(Settings->ws_width[XdrvMailbox.index -2]);
    }
  }
}

/*********************************************************************************************\
 * Internal calls for ArtNet
\*********************************************************************************************/
// check is the Neopixel strip is configured
bool Ws2812StripConfigured(void) {
  return strip != nullptr;
}
size_t Ws2812StripGetPixelSize(void) {
  return strip->PixelSize();
}
// return true if strip was dirty and an actual refresh was triggered
bool Ws2812StripRefresh(void) {
  if (strip->IsDirty()) {
    Ws2812LibStripShow();
    return true;
  } else {
    return false;
  }
}
void Ws2812CopyPixels(const uint8_t *buf, size_t len, size_t offset_in_matrix) {
  uint8_t *pixels = strip->Pixels();
  memmove(&pixels[offset_in_matrix], buf, len);
  strip->Dirty();
}



/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xlgt01(uint32_t function)
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
#endif  // ESP32