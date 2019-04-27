/*
  xdrv_04_light.ino - PWM, WS2812 and sonoff led support for Sonoff-Tasmota

  Copyright (C) 2019  Theo Arends

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
 * PWM, WS2812, Sonoff B1, AiLight, Sonoff Led and BN-SZ01, H801, MagicHome and Arilux
 *
 * light_type  Module     Color  ColorTemp  Modules
 * ----------  ---------  -----  ---------  ----------------------------
 *  1          PWM1       W      no         (Sonoff BN-SZ)
 *  2          PWM2       CW     yes        (Sonoff Led)
 *  3          PWM3       RGB    no         (H801, MagicHome and Arilux LC01)
 *  4          PWM4       RGBW   no         (H801, MagicHome and Arilux)
 *  5          PWM5       RGBCW  yes        (H801, Arilux LC11)
 *  9          reserved          no
 * 10          reserved          yes
 * 11          +WS2812    RGB(W) no         (One WS2812 RGB or RGBW ledstrip)
 * 12          AiLight    RGBW   no
 * 13          Sonoff B1  RGBCW  yes
 * 19          SM16716    RGB    no
 * 20          SM16716+W  RGBW   no
 * 21          SM16716+CW RGBCW  yes
 *
 * light_scheme  WS2812  3+ Colors  1+2 Colors  Effect
 * ------------  ------  ---------  ----------  -----------------
 *  0            yes     yes        yes         Color On/Off
 *  1            yes     yes        yes         Wakeup light
 *  2            yes     yes        no          Color cycle RGB
 *  3            yes     yes        no          Color cycle RBG
 *  4            yes     yes        no          Random RGB colors
 *  5            yes     no         no          Clock
 *  6            yes     no         no          Incandescent
 *  7            yes     no         no          RGB
 *  8            yes     no         no          Christmas
 *  9            yes     no         no          Hanukkah
 * 10            yes     no         no          Kwanzaa
 * 11            yes     no         no          Rainbow
 * 12            yes     no         no          Fire
 *
\*********************************************************************************************/


/*********************************************************************************************\
 *
 * Light management has been refactored to provide a cleaner class-based interface.
 * Also, now all values are stored as integer, no more floats that could generate
 * rounding errors.
 *
 * Two singletons are now used to control the state of the light.
 *  - light_state (LightStateClass) stores the color / white temperature and
 *    brightness. Use this object to READ only.
 *  - light_controller (LightControllerClass) is used to change light state
 *    and adjust all Settings and levels accordingly.
 *    Always use this object to change light status.
\*********************************************************************************************/

#define XDRV_04              4
//#define DEBUG_LIGHT

const uint8_t WS2812_SCHEMES = 7;    // Number of additional WS2812 schemes supported by xdrv_ws2812.ino

enum LightCommands {
  CMND_COLOR, CMND_COLORTEMPERATURE, CMND_DIMMER, CMND_LED, CMND_LEDTABLE, CMND_FADE,
  CMND_PIXELS, CMND_RGBWWTABLE, CMND_ROTATION, CMND_SCHEME, CMND_SPEED, CMND_WAKEUP, CMND_WAKEUPDURATION,
  CMND_WHITE, CMND_WIDTH, CMND_CHANNEL, CMND_HSBCOLOR, CMND_UNDOCA };
const char kLightCommands[] PROGMEM =
  D_CMND_COLOR "|" D_CMND_COLORTEMPERATURE "|" D_CMND_DIMMER "|" D_CMND_LED "|" D_CMND_LEDTABLE "|" D_CMND_FADE "|"
  D_CMND_PIXELS "|" D_CMND_RGBWWTABLE "|" D_CMND_ROTATION "|" D_CMND_SCHEME "|" D_CMND_SPEED "|" D_CMND_WAKEUP "|" D_CMND_WAKEUPDURATION "|"
  D_CMND_WHITE "|" D_CMND_WIDTH "|" D_CMND_CHANNEL "|" D_CMND_HSBCOLOR "|UNDOCA" ;

struct LRgbColor {
  uint8_t R, G, B;
};
const uint8_t MAX_FIXED_COLOR = 12;
const LRgbColor kFixedColor[MAX_FIXED_COLOR] PROGMEM =
  { 255,0,0, 0,255,0, 0,0,255, 228,32,0, 0,228,32, 0,32,228, 188,64,0, 0,160,96, 160,32,240, 255,255,0, 255,0,170, 255,255,255 };

struct LWColor {
  uint8_t W;
};
const uint8_t MAX_FIXED_WHITE = 4;
const LWColor kFixedWhite[MAX_FIXED_WHITE] PROGMEM = { 0, 255, 128, 32 };

struct LCwColor {
  uint8_t C, W;
};
const uint8_t MAX_FIXED_COLD_WARM = 4;
const LCwColor kFixedColdWarm[MAX_FIXED_COLD_WARM] PROGMEM = { 0,0, 255,0, 0,255, 128,128 };

const uint8_t ledTable[] = {
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

uint8_t light_entry_color[5];
uint8_t light_current_color[5];
uint8_t light_new_color[5];
uint8_t light_last_color[5];
uint8_t light_color_remap[5];

uint8_t light_wheel = 0;
uint8_t light_subtype = 0;    // LST_ subtype
uint8_t light_device = 0;
uint8_t light_power = 0;
uint8_t light_old_power = 1;
uint8_t light_update = 1;
uint8_t light_wakeup_active = 0;
uint8_t light_wakeup_dimmer = 0;
uint16_t light_wakeup_counter = 0;

uint8_t light_fixed_color_index = 1;

unsigned long strip_timer_counter = 0;    // Bars and Gradient

//
// changeUIntScale
// Change a value for range a..b to c..d, using only unsigned int math
//
// PRE-CONDITIONS (if not satisfied, you may 'halt and catch fire')
//    from_min < from_max  (not checked)
//    to_min   < to_max    (not checked)
//    from_min <= num <= from-max  (chacked)
// POST-CONDITIONS
//    to_min <= result <= to_max
//
uint16_t changeUIntScale(uint16_t inum, uint16_t ifrom_min, uint16_t ifrom_max,
                                       uint16_t ito_min, uint16_t ito_max) {
  // convert to uint31, it's more verbose but code is more compact
  uint32_t num = inum;
  uint32_t from_min = ifrom_min;
  uint32_t from_max = ifrom_max;
  uint32_t to_min = ito_min;
  uint32_t to_max = ito_max;

  // check source range
  num = (num > from_max ? from_max : (num < from_min ? from_min : num));
  uint32_t numerator = (num - from_min) * (to_max - to_min);
  uint32_t result;
  if (numerator >= 0x80000000L) {
    // don't do rounding as it would create an overflow
    result = numerator / (from_max - from_min) + to_min;
  } else {
    result = (((numerator * 2) / (from_max - from_min)) + 1) / 2 + to_min;
  }
  return (uint32_t) (result > to_max ? to_max : (result < to_min ? to_min : result));
}

//
// LightStateClass
// This class is an abstraction of the current light state.
// It allows for b/w, full colors, or white colortone
//
// This class has 3 independant slots
// 1/ Brightness 0.255, dimmer controls both RGB and WC (warm-cold)
//    If Brightness is 0, it is equivalent to Off (for compatibility)
//    Dimmer is Brightness converted to range 0..100
// 2/ RGB and Hue/Sat - always kept in sync and stored at full brightness,
//    i.e. R G or B are 255
// 3/ White with colortone - or WC (Warm / Cold)
//    ct is either 0: no white colortone control, revert to RGB
//    ct is 153..500 temperature
//    Optional whiteBri to contraol separately the brightness of white channel
//
// RGB and Hue/Sat are always kept in sync
// Brightness is stored in full range 0..255
// Dimmer (0.100) is autoamtically derived from brightness
//
// Light has two states: either color (HS) when ct==0, or white with
//   colortone if ct > 0.
//
//
// Note: RGB is internally stored always at full brightness (ie. one of R,G,B is 255)
//    If you want the actual RGB, you need to multiply with Bri,
//    or use getActualRGBCW()
// Note: all values are stored as unsigned integer, no floats.
// Note: you can query vaules from this singleton. But to change values,
//   use the LightController - changing this object will have no effect on actual light.
//
class LightStateClass {
  private:
    uint16_t _ct = 0;  // 0 or 153..500
    uint16_t _hue = 0;  // 0..359
    uint8_t  _sat = 255;  // 0..255
    uint8_t  _bri = 255;  // 0..255
    // dimmer is same as _bri but with a range of 0%-100%
    uint8_t  _r = 255;  // 0..255
    uint8_t  _g = 255;  // 0..255
    uint8_t  _b = 255;  // 0..255
    // are RGB and CT linked, i.e. if we set CT then RGB channels are off
    bool     _ct_rgb_linked = true;
    uint8_t  _whiteBri = 255;

  public:
    LightStateClass() {
      //AddLog_P2(LOG_LEVEL_DEBUG_MORE, "LightStateClass::Constructor RGB raw (%d %d %d) HS (%d %d) bri (%d)", _r, _g, _b, _hue, _sat, _bri);
    }

    bool setCTRGBLinked(bool ct_rgb_linked) {
      bool prev = _ct_rgb_linked;
      _ct_rgb_linked = ct_rgb_linked;
      return prev;
    }

    bool isCTRGBLinked() {
      return _ct_rgb_linked;
    }

    void setWhite() {
      _r = _g = _b = 255;
      _hue = 0;
      _sat = 0;
      //AddLog_P2(LOG_LEVEL_DEBUG_MORE, "LightStateClass::setWhite RGB raw (%d %d %d) HS (%d %d) bri (%d)", _r, _g, _b, _hue, _sat, _bri);
    }

    // Get RGB color, always at full brightness (ie. one of the components is 255)
    void getRGB(uint8_t *r, uint8_t *g, uint8_t *b) {
      if (r)  *r = _r;
      if (g)  *g = _g;
      if (b)  *b = _b;
    }

    // get full brightness values for wamr and cold channels.
    // either w=c=0 (off) or w+c=255
    void getCW(uint8_t *rc, uint8_t *rw) {
      uint16_t ct = _ct;
      uint16_t w = changeUIntScale(ct, 153, 500, 0, 255);
      if (rw) { *rw = (ct ? w       : 0); }
      if (rc) { *rc = (ct ? 255 - w : 0); }
    }

    // Get the actual RGB corrected with Brightness, ready to drive leds
    // return Bri
    uint8_t getActualRGBCW(uint8_t *r, uint8_t *g, uint8_t *b, uint8_t *c, uint8_t *w) {
      uint16_t bri = _bri;
      uint16_t wBri = _whiteBri;
      bool rgb_channels_off = _ct && _ct_rgb_linked;

      if (r) { *r = rgb_channels_off ? 0 : changeUIntScale(_r, 0, 255, 0, bri); }
      if (g) { *g = rgb_channels_off ? 0 : changeUIntScale(_g, 0, 255, 0, bri); }
      if (b) { *b = rgb_channels_off ? 0 : changeUIntScale(_b, 0, 255, 0, bri); }

      if (_ct) {
        // change range from 153..500 to 0..255
        uint8_t iwarm, icold;
        getCW(&icold, &iwarm);
        if (c) { *w = changeUIntScale(icold, 0, 255, 0, wBri); }
        if (w) { *c = changeUIntScale(iwarm, 0, 255, 0, wBri); }
      } else {
        if (w) { *w = 0; }
        if (c) { *c = 0; }
      }
      return _bri;
    }

    uint8_t getChannels(uint8_t *channels) {
      return getActualRGBCW(&channels[0], &channels[1], &channels[2], &channels[3], &channels[4]);
    }

    void getHSB(uint16_t *hue, uint8_t *sat, uint8_t *bri) {
      if (hue)  *hue = _hue;
      if (sat)  *sat = _sat;
      if (bri)  *bri = _bri;
    }

    // getBri() is guaranteed to give the same result as setBri() - no rounding errors.
    uint8_t getBri() {
      return _bri;  // 0..255
    }

    // get the Optional white Brightness
    uint8_t getWhiteBri() {
      return _whiteBri;
    }

    uint8_t getDimmer() {
      uint8_t dimmer = changeUIntScale(_bri, 0, 255, 0, 100);  // 0.100
      // if brightness is non zero, force dimmer to be non-zero too
      if ((dimmer == 0) && (_bri > 0)) { dimmer = 1; }
      return dimmer;
    }

    uint16_t getCT() {
      return _ct; // 0 or 153..500
    }

    // get current color in XY format
    void getXY(float *x, float *y) {
      RgbToXy(_r, _g, _b, x, y);
    }

    // setters -- do not use directly, use the light_controller instead
    // sets both master Bri and whiteBri
    void setBri(uint8_t bri, bool syncWhiteBri = true) {
      _bri = bri;  // 0..255
      if (syncWhiteBri) { _whiteBri = bri; }
#ifdef DEBUG_LIGHT
      AddLog_P2(LOG_LEVEL_DEBUG_MORE, "LightStateClass::setBri RGB raw (%d %d %d) HS (%d %d) bri (%d)", _r, _g, _b, _hue, _sat, _bri);
#endif
    }

    // whanges the white brightness, leaving master Bri untouched
    void setWhiteBri(uint8_t wBri) {
      _whiteBri = wBri;
    }

    void setDimmer(uint8_t dimmer) {
      _bri = changeUIntScale(dimmer, 0, 100, 0, 255);  // 0..255
    }

    void setCT(uint16_t ct) {
      if (0 == ct) {
        // disable ct mode
        _ct = 0;
      } else {
        _ct = (ct < 153 ? 153 : (ct > 500 ? 500 : ct));
      }
#ifdef DEBUG_LIGHT
      AddLog_P2(LOG_LEVEL_DEBUG_MORE, "LightStateClass::setCT RGB raw (%d %d %d) HS (%d %d) bri (%d) CT (%d)", _r, _g, _b, _hue, _sat, _bri, _ct);
#endif
    }

    // recalibrate W and C, in case a channel was changed independently
    // w+c must be 255, recalculate ct temperature accordingly
    // returns brightness
    uint8_t setCW(uint8_t w, uint8_t c) {
      uint16_t wc = w + c;
      if (wc > 0) {
        uint16_t ct = changeUIntScale(w, 0, wc, 153, 500);
        setCT(ct);
      } else {
        setCT(0);
      }
#ifdef DEBUG_LIGHT
      AddLog_P2(LOG_LEVEL_DEBUG_MORE, "LightStateClass::setCW CW (%d %d) CT (%d)", c, w, _ct);
#endif
      return (wc > 255 ? 255 : wc);
    }

    // sets RGB and returns the Brightness. Bri is unchanged here.
    uint8_t setRGB(uint8_t r, uint8_t g, uint8_t b) {
      uint16_t hue;
      uint8_t  sat;

      uint32_t max = (r > g && r > b) ? r : (g > b) ? g : b;   // 0..255

      if (0 == max) {
        r = g = b = 255;
      } else if (255 > max) {
        // we need to normalize rgb
        r = changeUIntScale(r, 0, max, 0, 255);
        g = changeUIntScale(g, 0, max, 0, 255);
        b = changeUIntScale(b, 0, max, 0, 255);
      }

      RgbToHsb(r, g, b, &hue, &sat, nullptr);
      _r = r;
      _g = g;
      _b = b;
      _hue = hue;
      _sat = sat;
      _ct = 0;    // no ct mode
#ifdef DEBUG_LIGHT
      AddLog_P2(LOG_LEVEL_DEBUG_MORE, "LightStateClass::setRGB RGB raw (%d %d %d) HS (%d %d) bri (%d)", _r, _g, _b, _hue, _sat, _bri);
#endif
      return max;
    }

    void setHS(uint16_t hue, uint8_t sat) {
      uint8_t r, g, b;
      HsToRgb(hue, sat, &r, &g, &b);
      _r = r;
      _g = g;
      _b = b;
      _hue = hue;
      _sat = sat;
      _ct = 0;    // no ct mode
#ifdef DEBUG_LIGHT
      AddLog_P2(LOG_LEVEL_DEBUG_MORE, "LightStateClass::setHS HS (%d %d) rgb (%d %d %d)", hue, sat, r, g, b);
      AddLog_P2(LOG_LEVEL_DEBUG_MORE, "LightStateClass::setHS RGB raw (%d %d %d) HS (%d %d) bri (%d)", _r, _g, _b, _hue, _sat, _bri);
#endif
  }

  // set all 5 channels at once.
  // Channels are: R G B CW WW
  // Brightness is automatically recalculated to adjust channels to the desired values
  void setChannels(uint8_t *channels) {
    uint8_t briRGB = setRGB(channels[0], channels[1], channels[2]);
    uint8_t briCW = setCW(channels[3], channels[4]);
#ifdef DEBUG_LIGHT
    AddLog_P2(LOG_LEVEL_DEBUG_MORE, "LightStateClass::setChannels (%d %d %d %d %d)",
      channels[0], channels[1], channels[2], channels[3], channels[4]);
    AddLog_P2(LOG_LEVEL_DEBUG_MORE, "LightStateClass::setChannels CT (%d) briRGB (%d) briCW (%d) linked (%d)",
      _ct, briRGB, briCW, _ct_rgb_linked);
#endif
    if (_ct_rgb_linked){
      // if RGB and CT are linked, we set Brightness to either CT or RGB
      if (_ct) {
        setBri(briCW);
      } else {
        setBri(briRGB);
      }
    } else {
      // we need to store the two brightnesses separately
      setBri(briRGB);
      setWhiteBri(briCW);
    }
  }

    // new version of RGB to HSB with only integer calculation
    static void RgbToHsb(uint8_t r, uint8_t g, uint8_t b, uint16_t *r_hue, uint8_t *r_sat, uint8_t *r_bri);
    static void HsToRgb(uint16_t hue, uint8_t sat, uint8_t *r_r, uint8_t *r_g, uint8_t *r_b);
    static void RgbToXy(uint8_t i_r, uint8_t i_g, uint8_t i_b, float *r_x, float *r_y);
    static void XyToRgb(float x, float y, uint8_t *rr, uint8_t *rg, uint8_t *rb);

};


/*********************************************************************************************\
 * LightStateClass implementation
\*********************************************************************************************/

// new version with only integer computing
// brightness is not needed, it is controlled via Dimmer
void LightStateClass::RgbToHsb(uint8_t ir, uint8_t ig, uint8_t ib, uint16_t *r_hue, uint8_t *r_sat, uint8_t *r_bri) {
  uint32_t r = ir;
  uint32_t g = ig;
  uint32_t b = ib;
  uint32_t max = (r > g && r > b) ? r : (g > b) ? g : b;   // 0..255
  uint32_t min = (r < g && r < b) ? r : (g < b) ? g : b;   // 0..255
  uint32_t d = max - min;   // 0..255

  uint16_t hue = 0;   // hue value in degrees ranges from 0 to 359
  uint8_t sat = 0;    // 0..255
  uint8_t bri = max;  // 0..255

  if (d != 0) {
    sat = changeUIntScale(d, 0, max, 0, 255);
    if (r == max) {
      hue = (g > b) ?       changeUIntScale(g-b,0,d,0,60) : 360 - changeUIntScale(b-g,0,d,0,60);
    } else if (g == max) {
      hue = (b > r) ? 120 + changeUIntScale(b-r,0,d,0,60) : 120 - changeUIntScale(r-b,0,d,0,60);
    } else {
      hue = (r > g) ? 240 + changeUIntScale(r-g,0,d,0,60) : 240 - changeUIntScale(g-r,0,d,0,60);
    }
    hue = hue % 360;    // 0..359
  }

  if (r_hue) *r_hue = hue;
  if (r_sat) *r_sat = sat;
  if (r_bri) *r_bri = bri;
  //AddLog_P2(LOG_LEVEL_DEBUG_MORE, "RgbToHsb rgb (%d %d %d) hsb (%d %d %d)", r, g, b, hue, sat, bri);
}

void LightStateClass::HsToRgb(uint16_t hue, uint8_t sat, uint8_t *r_r, uint8_t *r_g, uint8_t *r_b) {
  uint32_t r = 255;  // default to white
  uint32_t g = 255;
  uint32_t b = 255;
  // we take brightness at 100%, brightness should be set separately
  hue = hue % 360;  // normalize to 0..359

  if (sat > 0) {
    uint32_t i = hue / 60;   // quadrant 0..5
    uint32_t f = hue % 60;   // 0..59
    uint32_t q = 255 - changeUIntScale(f, 0, 60, 0, sat);  // 0..59
    uint32_t p = 255 - sat;
    uint32_t t = 255 - changeUIntScale(60 - f, 0, 60, 0, sat);

    switch (i) {
      case 0:
        //r = 255;
        g = t;
        b = p;
        break;
      case 1:
        r = q;
        //g = 255;
        b = p;
        break;
      case 2:
        r = p;
        //g = 255;
        b = t;
        break;
      case 3:
        r = p;
        g = q;
        //b = 255;
        break;
      case 4:
        r = t;
        g = p;
        //b = 255;
        break;
      default:
        //r = 255;
        g = p;
        b = q;
        break;
      }
    }
  if (r_r)  *r_r = r;
  if (r_g)  *r_g = g;
  if (r_b)  *r_b = b;
}

void LightStateClass::RgbToXy(uint8_t i_r, uint8_t i_g, uint8_t i_b, float *r_x, float *r_y) {
  float x = 0.31271f;   // default medium white
  float y = 0.32902f;

  if (i_r + i_b + i_g > 0) {
    float r = (float)i_r / 255.0f;
    float g = (float)i_g / 255.0f;
    float b = (float)i_b / 255.0f;
    // https://gist.github.com/popcorn245/30afa0f98eea1c2fd34d
    // Gamma correction
    r = (r > 0.04045f) ? powf((r + 0.055f) / (1.0f + 0.055f), 2.4f) : (r / 12.92f);
    g = (g > 0.04045f) ? powf((g + 0.055f) / (1.0f + 0.055f), 2.4f) : (g / 12.92f);
    b = (b > 0.04045f) ? powf((b + 0.055f) / (1.0f + 0.055f), 2.4f) : (b / 12.92f);

    // conversion to X, Y, Z
    // Y is also the Luminance
    float X = r * 0.649926f + g * 0.103455f + b * 0.197109f;
    float Y = r * 0.234327f + g * 0.743075f + b * 0.022598f;
    float Z = r * 0.000000f + g * 0.053077f + b * 1.035763f;

    x = X / (X + Y + Z);
    y = Y / (X + Y + Z);
    // we keep the raw gamut, one nice thing could be to convert to a narrower gamut
  }
  if (r_x)  *r_x = x;
  if (r_y)  *r_y = y;
}

void LightStateClass::XyToRgb(float x, float y, uint8_t *rr, uint8_t *rg, uint8_t *rb)
{
  x = (x > 0.99f ? 0.99f : (x < 0.01f ? 0.01f : x));
  y = (y > 0.99f ? 0.99f : (y < 0.01f ? 0.01f : y));
  float z = 1.0f - x - y;
  //float Y = 1.0f;
  float X = x / y;
  float Z = z / y;
  // float r =  X * 1.4628067f - 0.1840623f - Z * 0.2743606f;
  // float g = -X * 0.5217933f + 1.4472381f + Z * 0.0677227f;
  // float b =  X * 0.0349342f - 0.0968930f + Z * 1.2884099f;
  float r =  X * 3.2406f - 1.5372f - Z * 0.4986f;
  float g = -X * 0.9689f + 1.8758f + Z * 0.0415f;
  float b =  X * 0.0557f - 0.2040f + Z * 1.0570f;
  float max = (r > g && r > b) ? r : (g > b) ? g : b;
  r = r / max;    // normalize to max == 1.0
  g = g / max;
  b = b / max;
  r = (r <= 0.0031308f) ? 12.92f * r : 1.055f * powf(r, (1.0f / 2.4f)) - 0.055f;
  g = (g <= 0.0031308f) ? 12.92f * g : 1.055f * powf(g, (1.0f / 2.4f)) - 0.055f;
  b = (b <= 0.0031308f) ? 12.92f * b : 1.055f * powf(b, (1.0f / 2.4f)) - 0.055f;
  //
  // AddLog_P2(LOG_LEVEL_DEBUG_MORE, "XyToRgb XZ (%s %s) rgb (%s %s %s)",
  //   String(X,5).c_str(), String(Z,5).c_str(),
  //   String(r,5).c_str(), String(g,5).c_str(),String(b,5).c_str());

  int32_t ir = r * 255.0f + 0.5f;
  int32_t ig = g * 255.0f + 0.5f;
  int32_t ib = b * 255.0f + 0.5f;
  if (rr) { *rr = (ir > 255 ? 255: (ir < 0 ? 0 : ir)); }
  if (rg) { *rg = (ig > 255 ? 255: (ig < 0 ? 0 : ig)); }
  if (rb) { *rb = (ib > 255 ? 255: (ib < 0 ? 0 : ib)); }
}

class LightControllerClass {
  LightStateClass *_state;

public:
  LightControllerClass(LightStateClass& state) {
    _state = &state;
  }

#ifdef DEBUG_LIGHT
  void debugLogs() {
    uint8_t r,g,b,c,w;
    _state->getActualRGBCW(&r,&g,&b,&c,&w);
    AddLog_P2(LOG_LEVEL_DEBUG_MORE, "LightControllerClass::debugLogs rgb (%d %d %d) cw (%d %d)",
      r, g, b, c, w);
    AddLog_P2(LOG_LEVEL_DEBUG_MORE, "LightControllerClass::debugLogs lightCurrent (%d %d %d %d %d)",
      light_current_color[0], light_current_color[1], light_current_color[2],
      light_current_color[3], light_current_color[4]);
  }
#endif

  void loadSettings() {
#ifdef DEBUG_LIGHT
    AddLog_P2(LOG_LEVEL_DEBUG_MORE, "LightControllerClass::loadSettings Settings.light_color (%d %d %d %d %d - %d)",
      Settings.light_color[0], Settings.light_color[1], Settings.light_color[2],
      Settings.light_color[3], Settings.light_color[4], Settings.light_dimmer);
    AddLog_P2(LOG_LEVEL_DEBUG_MORE, "LightControllerClass::loadSettings light_type/sub (%d %d)",
      light_type, light_subtype);
#endif

    // set the RGB from settings
    _state->setRGB(Settings.light_color[0], Settings.light_color[1], Settings.light_color[2]);

    // get CT only for lights that support it
    if ((LST_COLDWARM == light_subtype) || (LST_RGBW <= light_subtype)) {
      // calculate whether we have CT set
      uint32_t c = Settings.light_color[3];
      uint32_t w = Settings.light_color[4];
      uint32_t ct = ((c > 0) || (w > 0)) ? changeUIntScale(w, 0, 255, 153, 500) : 0;
      _state->setCT(ct);
    }

    // set Dimmer
    _state->setDimmer(Settings.light_dimmer);
  }

  void changeCT(uint16_t new_ct) {
    /* Color Temperature (https://developers.meethue.com/documentation/core-concepts)
     *
     * ct = 153 = 2000K = Warm = CCWW = 00FF
     * ct = 500 = 6500K = Cold = CCWW = FF00
     */
    // don't set CT if not supported
    if ((LST_COLDWARM != light_subtype) && (LST_RGBW > light_subtype)) {
      return;
    }
    _state->setCT(new_ct);
    saveSettings();
    calcLevels();
    //debugLogs();
  }

  void changeDimmer(uint8_t dimmer) {
    uint8_t bri = changeUIntScale(dimmer, 0, 100, 0, 255);
    changeBri(bri);
  }

  void changeBri(uint8_t bri) {
    _state->setBri(bri);
    saveSettings();
    calcLevels();
  }

  void changeRGB(uint8_t r, uint8_t g, uint8_t b) {
    _state->setRGB(r, g, b);
    saveSettings();
    calcLevels();
  }

  // calculate the levels for each channel
  void calcLevels() {
    uint8_t r,g,b,w,c,bri;
    bri = _state->getActualRGBCW(&r,&g,&b,&w,&c);
    uint8_t wBri = _state->getWhiteBri();

    light_current_color[0] = light_current_color[1] = light_current_color[2] = 0;
    light_current_color[3] = light_current_color[4] = 0;
    if (PHILIPS == my_module_type) {
      // Xiaomi Philips bulbs follow a different scheme:
      // channel 0=intensity, channel2=temperature
      light_current_color[0] = bri;  // set brightness from r (white)
      light_current_color[1] = c;
    } else {
      switch (light_subtype) {
        case LST_NONE:
          light_current_color[0] = 255;
          break;
        case LST_SINGLE:
          light_current_color[0] = bri;
          break;
        case LST_COLDWARM:
          light_current_color[0] = w;
          light_current_color[1] = c;
          break;
        case LST_RGB:
        case LST_RGBW:
        case LST_RGBWC:
          light_current_color[0] = r;
          light_current_color[1] = g;
          light_current_color[2] = b;
          if (c || w) {   // if we have CT set
            if (LST_RGBWC == light_subtype) {
              light_current_color[3] = w;
              light_current_color[4] = c;
            } else if (LST_RGBW == light_subtype) {
              light_current_color[3] = wBri;
            }
          }
          break;
      }
    }
  }

  void changeHS(uint16_t hue, uint8_t sat) {
    _state->setHS(hue, sat);
    saveSettings();
  }

  // save the current light state to Settings.
  void saveSettings() {
    _state->getRGB(&Settings.light_color[0], &Settings.light_color[1], &Settings.light_color[2]);
    _state->getCW(&Settings.light_color[3], &Settings.light_color[4]);
    Settings.light_dimmer = _state->getDimmer();
#ifdef DEBUG_LIGHT
    AddLog_P2(LOG_LEVEL_DEBUG_MORE, "LightControllerClass::saveSettings Settings.light_color (%d %d %d %d %d - %d)",
      Settings.light_color[0], Settings.light_color[1], Settings.light_color[2],
      Settings.light_color[3], Settings.light_color[4], Settings.light_dimmer);
#endif
  }

  // set all 5 channels at once.
  // Channels are: R G B CW WW
  // Brightness is automatically recalculated to adjust channels to the desired values
  void changeChannels(uint8_t *channels) {
    _state->setChannels(channels);
    saveSettings();
    calcLevels();
  }
};


// the singletons for light state and Light Controller
LightStateClass light_state = LightStateClass();
LightControllerClass light_controller = LightControllerClass(light_state);


#ifdef USE_ARILUX_RF
/*********************************************************************************************\
 * Arilux LC11 Rf support stripped from RCSwitch library
\*********************************************************************************************/

const uint32_t ARILUX_RF_TIME_AVOID_DUPLICATE = 1000;  // Milliseconds

const uint8_t ARILUX_RF_MAX_CHANGES = 51;              // Pulses (sync + 2 x 24 bits)
const uint32_t ARILUX_RF_SEPARATION_LIMIT = 4300;      // Microseconds
const uint32_t ARILUX_RF_RECEIVE_TOLERANCE = 60;       // Percentage

unsigned int arilux_rf_timings[ARILUX_RF_MAX_CHANGES];

unsigned long arilux_rf_received_value = 0;
unsigned long arilux_rf_last_received_value = 0;
unsigned long arilux_rf_last_time = 0;
unsigned long arilux_rf_lasttime = 0;

unsigned int arilux_rf_change_count = 0;
unsigned int arilux_rf_repeat_count = 0;

uint8_t arilux_rf_toggle = 0;


#ifndef ARDUINO_ESP8266_RELEASE_2_3_0
#ifndef USE_WS2812_DMA  // Collides with Neopixelbus but solves RF misses
void AriluxRfInterrupt(void) ICACHE_RAM_ATTR;  // As iram is tight and it works this way too
#endif  // USE_WS2812_DMA
#endif  // ARDUINO_ESP8266_RELEASE_2_3_0

void AriluxRfInterrupt(void)
{
  unsigned long time = micros();
  unsigned int duration = time - arilux_rf_lasttime;

  if (duration > ARILUX_RF_SEPARATION_LIMIT) {
    if (abs(duration - arilux_rf_timings[0]) < 200) {
      arilux_rf_repeat_count++;
      if (arilux_rf_repeat_count == 2) {
        unsigned long code = 0;
        const unsigned int delay = arilux_rf_timings[0] / 31;
        const unsigned int delayTolerance = delay * ARILUX_RF_RECEIVE_TOLERANCE / 100;
        for (unsigned int i = 1; i < arilux_rf_change_count -1; i += 2) {
          code <<= 1;
          if (abs(arilux_rf_timings[i] - (delay *3)) < delayTolerance && abs(arilux_rf_timings[i +1] - delay) < delayTolerance) {
            code |= 1;
          }
        }
        if (arilux_rf_change_count > 49) {  // Need 1 sync bit and 24 data bits
          arilux_rf_received_value = code;
        }
        arilux_rf_repeat_count = 0;
      }
    }
    arilux_rf_change_count = 0;
  }
  if (arilux_rf_change_count >= ARILUX_RF_MAX_CHANGES) {
    arilux_rf_change_count = 0;
    arilux_rf_repeat_count = 0;
  }
  arilux_rf_timings[arilux_rf_change_count++] = duration;
  arilux_rf_lasttime = time;
}

void AriluxRfHandler(void)
{
  unsigned long now = millis();
  if (arilux_rf_received_value && !((arilux_rf_received_value == arilux_rf_last_received_value) && (now - arilux_rf_last_time < ARILUX_RF_TIME_AVOID_DUPLICATE))) {
    arilux_rf_last_received_value = arilux_rf_received_value;
    arilux_rf_last_time = now;

    uint16_t hostcode = arilux_rf_received_value >> 8 & 0xFFFF;
    if (Settings.rf_code[1][6] == Settings.rf_code[1][7]) {
      Settings.rf_code[1][6] = hostcode >> 8 & 0xFF;
      Settings.rf_code[1][7] = hostcode & 0xFF;
    }
    uint16_t stored_hostcode = Settings.rf_code[1][6] << 8 | Settings.rf_code[1][7];

    AddLog_P2(LOG_LEVEL_DEBUG, PSTR(D_LOG_RFR D_HOST D_CODE " 0x%04X, " D_RECEIVED " 0x%06X"), stored_hostcode, arilux_rf_received_value);

    if (hostcode == stored_hostcode) {
      char command[33];
      char value = '-';
      command[0] = '\0';
      uint8_t  keycode = arilux_rf_received_value & 0xFF;
      switch (keycode) {
        case 1:  // Power On
        case 3:  // Power Off
          snprintf_P(command, sizeof(command), PSTR(D_CMND_POWER " %d"), (1 == keycode) ? 1 : 0);
          break;
        case 2:  // Toggle
          arilux_rf_toggle++;
          arilux_rf_toggle &= 0x3;
          snprintf_P(command, sizeof(command), PSTR(D_CMND_COLOR " %d"), 200 + arilux_rf_toggle);
          break;
        case 4:  // Speed +
          value = '+';
        case 7:  // Speed -
          snprintf_P(command, sizeof(command), PSTR(D_CMND_SPEED " %c"), value);
          break;
        case 5:  // Scheme +
          value = '+';
        case 8:  // Scheme -
          snprintf_P(command, sizeof(command), PSTR(D_CMND_SCHEME " %c"), value);
          break;
        case 6:  // Dimmer +
          value = '+';
        case 9:  // Dimmer -
          snprintf_P(command, sizeof(command), PSTR(D_CMND_DIMMER " %c"), value);
          break;
        default: {
          if ((keycode >= 10) && (keycode <= 21)) {
            snprintf_P(command, sizeof(command), PSTR(D_CMND_COLOR " %d"), keycode -9);
          }
        }
      }
      if (strlen(command)) {
        ExecuteCommand(command, SRC_LIGHT);
      }
    }
  }
  arilux_rf_received_value = 0;
}

void AriluxRfInit(void)
{
  if ((pin[GPIO_ARIRFRCV] < 99) && (pin[GPIO_LED4] < 99)) {
    if (Settings.last_module != Settings.module) {
      Settings.rf_code[1][6] = 0;
      Settings.rf_code[1][7] = 0;
      Settings.last_module = Settings.module;
    }
    arilux_rf_received_value = 0;
    digitalWrite(pin[GPIO_LED4], !bitRead(led_inverted, 3));  // Turn on RF
    attachInterrupt(pin[GPIO_ARIRFRCV], AriluxRfInterrupt, CHANGE);
  }
}

void AriluxRfDisable(void)
{
  if ((pin[GPIO_ARIRFRCV] < 99) && (pin[GPIO_LED4] < 99)) {
    detachInterrupt(pin[GPIO_ARIRFRCV]);
    digitalWrite(pin[GPIO_LED4], bitRead(led_inverted, 3));  // Turn off RF
  }
}
#endif  // USE_ARILUX_RF

/*********************************************************************************************\
 * Sonoff B1 and AiLight inspired by OpenLight https://github.com/icamgo/noduino-sdk
\*********************************************************************************************/

extern "C" {
  void os_delay_us(unsigned int);
}

uint8_t light_pdi_pin;
uint8_t light_pdcki_pin;

void LightDiPulse(uint8_t times)
{
  for (uint8_t i = 0; i < times; i++) {
    digitalWrite(light_pdi_pin, HIGH);
    digitalWrite(light_pdi_pin, LOW);
  }
}

void LightDckiPulse(uint8_t times)
{
  for (uint8_t i = 0; i < times; i++) {
    digitalWrite(light_pdcki_pin, HIGH);
    digitalWrite(light_pdcki_pin, LOW);
  }
}

void LightMy92x1Write(uint8_t data)
{
  for (uint8_t i = 0; i < 4; i++) {     // Send 8bit Data
    digitalWrite(light_pdcki_pin, LOW);
    digitalWrite(light_pdi_pin, (data & 0x80));
    digitalWrite(light_pdcki_pin, HIGH);
    data = data << 1;
    digitalWrite(light_pdi_pin, (data & 0x80));
    digitalWrite(light_pdcki_pin, LOW);
    digitalWrite(light_pdi_pin, LOW);
    data = data << 1;
  }
}

void LightMy92x1Init(void)
{
  uint8_t chips = 1;                    // 1 (AiLight)
  if (LT_RGBWC == light_type) {
    chips = 2;                          // 2 (Sonoff B1)
  }

  LightDckiPulse(chips * 32);           // Clear all duty register
  os_delay_us(12);                      // TStop > 12us.
  // Send 12 DI pulse, after 6 pulse's falling edge store duty data, and 12
  // pulse's rising edge convert to command mode.
  LightDiPulse(12);
  os_delay_us(12);                      // Delay >12us, begin send CMD data
  for (uint8_t n = 0; n < chips; n++) { // Send CMD data
    LightMy92x1Write(0x18);             // ONE_SHOT_DISABLE, REACTION_FAST, BIT_WIDTH_8, FREQUENCY_DIVIDE_1, SCATTER_APDM
  }
  os_delay_us(12);                      // TStart > 12us. Delay 12 us.
  // Send 16 DI pulse, at 14 pulse's falling edge store CMD data, and
  // at 16 pulse's falling edge convert to duty mode.
  LightDiPulse(16);
  os_delay_us(12);                      // TStop > 12us.
}

void LightMy92x1Duty(uint8_t duty_r, uint8_t duty_g, uint8_t duty_b, uint8_t duty_w, uint8_t duty_c)
{
  uint8_t channels[2] = { 4, 6 };

  uint8_t didx = 0;                     // 0 (AiLight)
  if (LT_RGBWC == light_type) {
    didx = 1;                           // 1 (Sonoff B1)
  }

  uint8_t duty[2][6] = {{ duty_r, duty_g, duty_b, duty_w, 0, 0 },        // Definition for RGBW channels
                        { duty_w, duty_c, 0, duty_g, duty_r, duty_b }};  // Definition for RGBWC channels

  os_delay_us(12);                      // TStop > 12us.
  for (uint8_t channel = 0; channel < channels[didx]; channel++) {
    LightMy92x1Write(duty[didx][channel]);  // Send 8bit Data
  }
  os_delay_us(12);                      // TStart > 12us. Ready for send DI pulse.
  LightDiPulse(8);                      // Send 8 DI pulse. After 8 pulse falling edge, store old data.
  os_delay_us(12);                      // TStop > 12us.
}

#ifdef USE_SM16716
/*********************************************************************************************\
 * SM16716 - Controlling RGB over a synchronous serial line
 * Copyright (C) 2019  Gabor Simon
 *
 * Source: https://community.home-assistant.io/t/cheap-uk-wifi-bulbs-with-tasmota-teardown-help-tywe3s/40508/27
 *
\*********************************************************************************************/

// Enable this for debug logging
//#define D_LOG_SM16716       "SM16716: "

uint8_t sm16716_pin_clk     = 100;
uint8_t sm16716_pin_dat     = 100;
uint8_t sm16716_pin_sel     = 100;
uint8_t sm16716_enabled     = 0;

void SM16716_SendBit(uint8_t v)
{
  /* NOTE:
   * According to the spec sheet, max freq is 30 MHz, that is 16.6 ns per high/low half of the
   * clk square wave. That is less than the overhead of 'digitalWrite' at this clock rate,
   * so no additional delays are needed yet. */

  digitalWrite(sm16716_pin_dat, (v != 0) ? HIGH : LOW);
  //delayMicroseconds(1);
  digitalWrite(sm16716_pin_clk, HIGH);
  //delayMicroseconds(1);
  digitalWrite(sm16716_pin_clk, LOW);
}

void SM16716_SendByte(uint8_t v)
{
  uint8_t mask;

  for (mask = 0x80; mask; mask >>= 1) {
    SM16716_SendBit(v & mask);
  }
}

void SM16716_Update(uint8_t duty_r, uint8_t duty_g, uint8_t duty_b)
{
  if (sm16716_pin_sel < 99) {
    uint8_t sm16716_should_enable = (duty_r | duty_g | duty_b);
    if (!sm16716_enabled && sm16716_should_enable) {
#ifdef D_LOG_SM16716
      AddLog_P2(LOG_LEVEL_DEBUG, PSTR(D_LOG_SM16716 "turning color on"));
#endif // D_LOG_SM16716
      sm16716_enabled = 1;
      digitalWrite(sm16716_pin_sel, HIGH);
      // in testing I found it takes a minimum of ~380us to wake up the chip
      // tested on a Merkury RGBW with an SM726EB
      delayMicroseconds(1000);
      SM16716_Init();
    }
    else if (sm16716_enabled && !sm16716_should_enable) {
#ifdef D_LOG_SM16716
      AddLog_P2(LOG_LEVEL_DEBUG, PSTR(D_LOG_SM16716 "turning color off"));
#endif // D_LOG_SM16716
      sm16716_enabled = 0;
      digitalWrite(sm16716_pin_sel, LOW);
    }
  }
#ifdef D_LOG_SM16716
  AddLog_P2(LOG_LEVEL_DEBUG, PSTR(D_LOG_SM16716 "Update; rgb=%02x%02x%02x"), duty_r, duty_g, duty_b);
#endif // D_LOG_SM16716

  // send start bit
  SM16716_SendBit(1);
  SM16716_SendByte(duty_r);
  SM16716_SendByte(duty_g);
  SM16716_SendByte(duty_b);

  // send a 'do it' pulse
  // (if multiple chips are chained, each one processes the 1st '1rgb' 25-bit block and
  // passes on the rest, right until the one starting with 0)
  //SM16716_Init();
  SM16716_SendBit(0);
  SM16716_SendByte(0);
  SM16716_SendByte(0);
  SM16716_SendByte(0);
}

bool SM16716_ModuleSelected(void)
{
  sm16716_pin_clk = pin[GPIO_SM16716_CLK];
  sm16716_pin_dat = pin[GPIO_SM16716_DAT];
  sm16716_pin_sel = pin[GPIO_SM16716_SEL];
#ifdef D_LOG_SM16716
  AddLog_P2(LOG_LEVEL_DEBUG, PSTR(D_LOG_SM16716 "ModuleSelected; clk_pin=%d, dat_pin=%d)"), sm16716_pin_clk, sm16716_pin_dat);
#endif // D_LOG_SM16716
  return (sm16716_pin_clk < 99) && (sm16716_pin_dat < 99);
}

void SM16716_Init(void)
{
  for (uint8_t t_init = 0; t_init < 50; ++t_init) {
    SM16716_SendBit(0);
  }
}

#endif  // ifdef USE_SM16716

/********************************************************************************************/

void LightInit(void)
{
  uint8_t max_scheme = LS_MAX -1;

  light_device = devices_present;
  light_subtype = light_type &7;        // Always 0 - 7

  light_controller.loadSettings();

  if (LST_SINGLE == light_subtype) {
    Settings.light_color[0] = 255;      // One channel only supports Dimmer but needs max color
  }
  if (light_type < LT_PWM6) {           // PWM
    for (uint8_t i = 0; i < light_type; i++) {
      Settings.pwm_value[i] = 0;        // Disable direct PWM control
      if (pin[GPIO_PWM1 +i] < 99) {
        pinMode(pin[GPIO_PWM1 +i], OUTPUT);
      }
    }
    if (SONOFF_LED == my_module_type) { // Fix Sonoff Led instabilities
      if (!my_module.io[4]) {
        pinMode(4, OUTPUT);             // Stop floating outputs
        digitalWrite(4, LOW);
      }
      if (!my_module.io[5]) {
        pinMode(5, OUTPUT);             // Stop floating outputs
        digitalWrite(5, LOW);
      }
      if (!my_module.io[14]) {
        pinMode(14, OUTPUT);            // Stop floating outputs
        digitalWrite(14, LOW);
      }
    }
    if (pin[GPIO_ARIRFRCV] < 99) {
      if (pin[GPIO_LED4] < 99) {
        digitalWrite(pin[GPIO_LED4], bitRead(led_inverted, 3));  // Turn off RF
      }
    }
  }
#ifdef USE_WS2812  // ************************************************************************
  else if (LT_WS2812 == light_type) {
#if (USE_WS2812_CTYPE > NEO_3LED)
    light_subtype++;  // from RGB to RGBW
#endif
    Ws2812Init();
    max_scheme = LS_MAX + WS2812_SCHEMES;
  }
#endif  // USE_WS2812 ************************************************************************
#ifdef USE_SM16716
  else if (LT_SM16716 == light_type - light_subtype) {
    // init PWM
    for (uint8_t i = 0; i < light_subtype; i++) {
      Settings.pwm_value[i] = 0;        // Disable direct PWM control
      if (pin[GPIO_PWM1 +i] < 99) {
        pinMode(pin[GPIO_PWM1 +i], OUTPUT);
      }
    }
    // init sm16716
    pinMode(sm16716_pin_clk, OUTPUT);
    digitalWrite(sm16716_pin_clk, LOW);

    pinMode(sm16716_pin_dat, OUTPUT);
    digitalWrite(sm16716_pin_dat, LOW);

    if (sm16716_pin_sel < 99) {
      pinMode(sm16716_pin_sel, OUTPUT);
      digitalWrite(sm16716_pin_sel, LOW);
      // no need to call SM16716_Init here, it will be called after sel goes HIGH
    } else {
      // no sel pin means you have an 'always on' chip, so init right away
      SM16716_Init();
    }
  }
#endif  // ifdef USE_SM16716
  else {
    light_pdi_pin = pin[GPIO_DI];
    light_pdcki_pin = pin[GPIO_DCKI];

    pinMode(light_pdi_pin, OUTPUT);
    pinMode(light_pdcki_pin, OUTPUT);
    digitalWrite(light_pdi_pin, LOW);
    digitalWrite(light_pdcki_pin, LOW);

    LightMy92x1Init();
  }

  if (light_subtype < LST_RGB) {
    max_scheme = LS_POWER;
  }
  if ((LS_WAKEUP == Settings.light_scheme) || (Settings.light_scheme > max_scheme)) {
    Settings.light_scheme = LS_POWER;
  }
  light_power = 0;
  light_update = 1;
  light_wakeup_active = 0;

  LightUpdateColorMapping();
}

void LightUpdateColorMapping(void)
{
  uint8_t param = Settings.param[P_RGB_REMAP] & 127;
  if (param > 119){ param = 0; }

  uint8_t tmp[] = {0,1,2,3,4};
  light_color_remap[0] = tmp[param / 24];
  for (uint8_t i = param / 24; i<4; ++i){
    tmp[i] = tmp[i+1];
  }
  param = param % 24;
  light_color_remap[1] = tmp[(param / 6)];
  for (uint8_t i = param / 6; i<3; ++i){
    tmp[i] = tmp[i+1];
  }
  param = param % 6;
  light_color_remap[2] = tmp[(param / 2)];
  for (uint8_t i = param / 2; i<2; ++i){
    tmp[i] = tmp[i+1];
  }
  param = param % 2;
  light_color_remap[3] = tmp[param];
  light_color_remap[4] = tmp[1-param];

  // do not allow independant RGV and WC colors
  bool ct_rgb_linked = !(Settings.param[P_RGB_REMAP] & 128);
  light_state.setCTRGBLinked(ct_rgb_linked);

  light_update = 1;
  //AddLog_P2(LOG_LEVEL_DEBUG, PSTR("%d colors: %d %d %d %d %d") ,Settings.param[P_RGB_REMAP], light_color_remap[0],light_color_remap[1],light_color_remap[2],light_color_remap[3],light_color_remap[4]);
}

void LightSetColorTemp(uint16_t ct)
{
/* Color Temperature (https://developers.meethue.com/documentation/core-concepts)
 *
 * ct = 153 = 2000K = Warm = CCWW = 00FF
 * ct = 500 = 6500K = Cold = CCWW = FF00
 */
  // don't set CT if not supported
  if ((LST_COLDWARM != light_subtype) && (LST_RGBWC != light_subtype)) {
    return;
  }
  light_controller.changeCT(ct);
}

uint16_t LightGetColorTemp(void)
{
  // don't calculate CT for unsupported devices
  if ((LST_COLDWARM != light_subtype) && (LST_RGBWC != light_subtype)) {
    return 0;
  }
  return light_state.getCT();
}

void LightSetSignal(uint16_t lo, uint16_t hi, uint16_t value)
{
/* lo - below lo is green
   hi - above hi is red
*/
  if (Settings.flag.light_signal) {
    uint16_t signal = changeUIntScale(value, lo, hi, 0, 255);  // 0..255
//    AddLog_P2(LOG_LEVEL_DEBUG, PSTR(D_LOG_DEBUG "Light signal %d"), signal);
    light_controller.changeRGB(signal, 255 - signal, 0);
    Settings.light_scheme = 0;
    if (0 == light_state.getBri()) {
      light_controller.changeBri(50);
    }
  }
}

// convert channels to string, use Option 17 to foce decimal, unless force_hex
char* LightGetColor(char* scolor, boolean force_hex = false)
{
  light_controller.calcLevels();
  scolor[0] = '\0';
  for (uint8_t i = 0; i < light_subtype; i++) {
    if (!force_hex && Settings.flag.decimal_text) {
      snprintf_P(scolor, 25, PSTR("%s%s%d"), scolor, (i > 0) ? "," : "", light_current_color[i]);
    } else {
      snprintf_P(scolor, 25, PSTR("%s%02X"), scolor, light_current_color[i]);
    }
  }
  return scolor;
}

void LightPowerOn(void)
{
  if (light_state.getBri() && !(light_power)) {
    ExecuteCommandPower(light_device, POWER_ON, SRC_LIGHT);
  }
}

void LightState(uint8_t append)
{
  char scolor[25];
  char scommand[33];

  if (append) {
    ResponseAppend_P(PSTR(","));
  } else {
    Response_P(PSTR("{"));
  }
  GetPowerDevice(scommand, light_device, sizeof(scommand), Settings.flag.device_index_enable);
  ResponseAppend_P(PSTR("\"%s\":\"%s\",\"" D_CMND_DIMMER "\":%d"), scommand, GetStateText(light_power), light_state.getDimmer());
  if (light_subtype > LST_SINGLE) {
    ResponseAppend_P(PSTR(",\"" D_CMND_COLOR "\":\"%s\""), LightGetColor(scolor));
    uint16_t hue;
    uint8_t  sat, bri;
    light_state.getHSB(&hue, &sat, &bri);
    sat = changeUIntScale(sat, 0, 255, 0, 100);
    bri = changeUIntScale(bri, 0, 255, 0, 100);

    ResponseAppend_P(PSTR(",\"" D_CMND_HSBCOLOR "\":\"%d,%d,%d\""), hue,sat,bri);
    // Add status for each channel
    ResponseAppend_P(PSTR(",\"" D_CMND_CHANNEL "\":[" ));
    for (uint8_t i = 0; i < light_subtype; i++) {
      uint8_t channel_raw = light_current_color[i];
      uint8_t channel = changeUIntScale(channel_raw,0,255,0,100);
      // if non null, force to be at least 1
      if ((0 == channel) && (channel_raw > 0)) { channel = 1; }
      ResponseAppend_P(PSTR("%s%d" ), (i > 0 ? "," : ""), channel);
    }
    ResponseAppend_P(PSTR("]"));
  }
  if ((LST_COLDWARM == light_subtype) || (LST_RGBWC == light_subtype)) {
    ResponseAppend_P(PSTR(",\"" D_CMND_COLORTEMPERATURE "\":%d"), light_state.getCT());
  }
  if (append) {
    if (light_subtype >= LST_RGB) {
      ResponseAppend_P(PSTR(",\"" D_CMND_SCHEME "\":%d"), Settings.light_scheme);
    }
    if (LT_WS2812 == light_type) {
      ResponseAppend_P(PSTR(",\"" D_CMND_WIDTH "\":%d"), Settings.light_width);
    }
    ResponseAppend_P(PSTR(",\"" D_CMND_FADE "\":\"%s\",\"" D_CMND_SPEED "\":%d,\"" D_CMND_LEDTABLE "\":\"%s\""),
      GetStateText(Settings.light_fade), Settings.light_speed, GetStateText(Settings.light_correction));
  } else {
    ResponseAppend_P(PSTR("}"));
  }
}

void LightPreparePower(void)
{
  if (light_state.getBri() && !(light_power)) {
    if (!Settings.flag.not_power_linked) {
      ExecuteCommandPower(light_device, POWER_ON_NO_STATE, SRC_LIGHT);
    }
  }
  else if (!light_state.getBri() && light_power) {
    ExecuteCommandPower(light_device, POWER_OFF_NO_STATE, SRC_LIGHT);
  }
#ifdef USE_DOMOTICZ
  DomoticzUpdatePowerState(light_device);
#endif  // USE_DOMOTICZ
  if (Settings.flag3.hass_tele_on_power) { MqttPublishTeleState(); }

  LightState(0);
}

void LightFade(void)
{
  if (0 == Settings.light_fade) {
    for (uint8_t i = 0; i < light_subtype; i++) {
      light_new_color[i] = light_current_color[i];
    }
  } else {
    uint8_t shift = Settings.light_speed;
    if (Settings.light_speed > 6) {
      shift = (strip_timer_counter % (Settings.light_speed -6)) ? 0 : 8;
    }
    if (shift) {
      for (uint8_t i = 0; i < light_subtype; i++) {
        if (light_new_color[i] != light_current_color[i]) {
          if (light_new_color[i] < light_current_color[i]) {
            light_new_color[i] += ((light_current_color[i] - light_new_color[i]) >> shift) +1;
          }
          if (light_new_color[i] > light_current_color[i]) {
            light_new_color[i] -= ((light_new_color[i] - light_current_color[i]) >> shift) +1;
          }
        }
      }
    }
  }
}

void LightWheel(uint8_t wheel_pos)
{
  wheel_pos = 255 - wheel_pos;
  if (wheel_pos < 85) {
    light_entry_color[0] = 255 - wheel_pos * 3;
    light_entry_color[1] = 0;
    light_entry_color[2] = wheel_pos * 3;
  } else if (wheel_pos < 170) {
    wheel_pos -= 85;
    light_entry_color[0] = 0;
    light_entry_color[1] = wheel_pos * 3;
    light_entry_color[2] = 255 - wheel_pos * 3;
  } else {
    wheel_pos -= 170;
    light_entry_color[0] = wheel_pos * 3;
    light_entry_color[1] = 255 - wheel_pos * 3;
    light_entry_color[2] = 0;
  }
  light_entry_color[3] = 0;
  light_entry_color[4] = 0;
  float dimmer = 100 / (float)Settings.light_dimmer;
  for (uint8_t i = 0; i < LST_RGB; i++) {
    float temp = (float)light_entry_color[i] / dimmer + 0.5f;
    light_entry_color[i] = (uint8_t)temp;
  }
}

void LightCycleColor(int8_t direction)
{
  if (strip_timer_counter % (Settings.light_speed * 2)) {
    return;
  }
  light_wheel += direction;
  LightWheel(light_wheel);
  memcpy(light_new_color, light_entry_color, sizeof(light_new_color));
}

void LightRandomColor(void)
{
  uint8_t light_update = 0;
  for (uint8_t i = 0; i < LST_RGB; i++) {
    if (light_new_color[i] != light_current_color[i]) {
      light_update = 1;
    }
  }
  if (!light_update) {
    light_wheel = random(255);
    LightWheel(light_wheel);
    memcpy(light_current_color, light_entry_color, sizeof(light_current_color));
    light_controller.changeChannels(light_current_color);
  }
  LightFade();
}

void LightSetPower(void)
{
//  light_power = XdrvMailbox.index;
  light_old_power = light_power;
  light_power = bitRead(XdrvMailbox.index, light_device -1);
  if (light_wakeup_active) {
    light_wakeup_active--;
  }
  if (light_power && !light_old_power) {
    light_update = 1;
  }
  LightAnimate();
}

void LightAnimate(void)
{
  uint8_t cur_col[5];
  uint16_t light_still_on = 0;

  strip_timer_counter++;
  if (!light_power) {                   // Power Off
    sleep = Settings.sleep;
    strip_timer_counter = 0;
    for (uint8_t i = 0; i < light_subtype; i++) {
      light_still_on += light_new_color[i];
    }
    if (light_still_on && Settings.light_fade && (Settings.light_scheme < LS_MAX)) {
      uint8_t speed = Settings.light_speed;
      if (speed > 6) {
        speed = 6;
      }
      for (uint8_t i = 0; i < light_subtype; i++) {
        if (light_new_color[i] > 0) {
          light_new_color[i] -= (light_new_color[i] >> speed) +1;
        }
      }
    } else {
      for (uint8_t i = 0; i < light_subtype; i++) {
        light_new_color[i] = 0;
      }
    }
  }
  else {
#ifdef PWM_LIGHTSCHEME0_IGNORE_SLEEP
    sleep = (LS_POWER == Settings.light_scheme) ? Settings.sleep : 0;  // If no animation then use sleep as is
#else
    sleep = 0;
#endif // PWM_LIGHTSCHEME0_IGNORE_SLEEP
    switch (Settings.light_scheme) {
      case LS_POWER:
        light_controller.calcLevels();
        LightFade();
        break;
      case LS_WAKEUP:
        if (2 == light_wakeup_active) {
          light_wakeup_active = 1;
          for (uint8_t i = 0; i < light_subtype; i++) {
            light_new_color[i] = 0;
          }
          light_wakeup_counter = 0;
          light_wakeup_dimmer = 0;
        }
        light_wakeup_counter++;
        if (light_wakeup_counter > ((Settings.light_wakeup * STATES) / Settings.light_dimmer)) {
          light_wakeup_counter = 0;
          light_wakeup_dimmer++;
          if (light_wakeup_dimmer <= Settings.light_dimmer) {
            light_state.setDimmer(light_wakeup_dimmer);
            light_controller.calcLevels();
            for (uint8_t i = 0; i < light_subtype; i++) {
              light_new_color[i] = light_current_color[i];
            }
          } else {
            Response_P(PSTR("{\"" D_CMND_WAKEUP "\":\"" D_JSON_DONE "\"}"));
            MqttPublishPrefixTopic_P(TELE, PSTR(D_CMND_WAKEUP));
            light_wakeup_active = 0;
            Settings.light_scheme = LS_POWER;
          }
        }
        break;
      case LS_CYCLEUP:
        LightCycleColor(1);
        break;
      case LS_CYCLEDN:
        LightCycleColor(-1);
        break;
      case LS_RANDOM:
        LightRandomColor();
        break;
#ifdef USE_WS2812  // ************************************************************************
      default:
        if (LT_WS2812 == light_type) {
          Ws2812ShowScheme(Settings.light_scheme -LS_MAX);
        }
#endif  // USE_WS2812 ************************************************************************
    }
  }

  if ((Settings.light_scheme < LS_MAX) || !light_power) {
    if (memcmp(light_last_color, light_new_color, light_subtype)) {
        light_update = 1;
    }
    if (light_update) {
      light_update = 0;
      for (uint8_t i = 0; i < light_subtype; i++) {
        light_last_color[i] = light_new_color[i];
        cur_col[i] = light_last_color[i]*Settings.rgbwwTable[i]/255;
        cur_col[i] = (Settings.light_correction) ? ledTable[cur_col[i]] : cur_col[i];
      }

      // color remapping
      uint8_t orig_col[5];
      memcpy(orig_col, cur_col, sizeof(orig_col));
      for (uint8_t i = 0; i < 5; i++) {
        cur_col[i] = orig_col[light_color_remap[i]];
      }

      for (uint8_t i = 0; i < light_subtype; i++) {
        if (light_type < LT_PWM6) {
          if (pin[GPIO_PWM1 +i] < 99) {
            if (cur_col[i] > 0xFC) {
              cur_col[i] = 0xFC;   // Fix unwanted blinking and PWM watchdog errors for values close to pwm_range (H801, Arilux and BN-SZ01)
            }
            uint16_t curcol = cur_col[i] * (Settings.pwm_range / 255);
//            AddLog_P2(LOG_LEVEL_DEBUG, PSTR(D_LOG_APPLICATION "Cur_Col%d %d, CurCol %d"), i, cur_col[i], curcol);
            analogWrite(pin[GPIO_PWM1 +i], bitRead(pwm_inverted, i) ? Settings.pwm_range - curcol : curcol);
          }
        }
      }

      char *tmp_data = XdrvMailbox.data;
      uint16_t tmp_data_len = XdrvMailbox.data_len;

      XdrvMailbox.data = (char*)cur_col;
      XdrvMailbox.data_len = sizeof(cur_col);
      if (XdrvCall(FUNC_SET_CHANNELS)) {
        // Serviced
      }
#ifdef USE_WS2812  // ************************************************************************
      else if (LT_WS2812 == light_type) {
        Ws2812SetColor(0, cur_col[0], cur_col[1], cur_col[2], cur_col[3]);
      }
#endif  // USE_ES2812 ************************************************************************
#ifdef USE_SM16716
      else if (LT_SM16716 == light_type - light_subtype) {
        // handle any PWM pins, skipping the first 3 values for sm16716
        for (uint8_t i = 3; i < light_subtype; i++) {
          if (pin[GPIO_PWM1 +i-3] < 99) {
            if (cur_col[i] > 0xFC) {
              cur_col[i] = 0xFC;   // Fix unwanted blinking and PWM watchdog errors for values close to pwm_range (H801, Arilux and BN-SZ01)
            }
            uint16_t curcol = cur_col[i] * (Settings.pwm_range / 255);
//            AddLog_P2(LOG_LEVEL_DEBUG, PSTR(D_LOG_APPLICATION "Cur_Col%d %d, CurCol %d"), i, cur_col[i], curcol);
            analogWrite(pin[GPIO_PWM1 +i-3], bitRead(pwm_inverted, i-3) ? Settings.pwm_range - curcol : curcol);
          }
        }
        // handle sm16716 update
        SM16716_Update(cur_col[0], cur_col[1], cur_col[2]);
      }
#endif  // ifdef USE_SM16716
      else if (light_type > LT_WS2812) {
        LightMy92x1Duty(cur_col[0], cur_col[1], cur_col[2], cur_col[3], cur_col[4]);
      }
      XdrvMailbox.data = tmp_data;
      XdrvMailbox.data_len = tmp_data_len;
    }
  }
}

/*********************************************************************************************\
 * Commands
\*********************************************************************************************/

bool LightColorEntry(char *buffer, uint8_t buffer_length)
{
  char scolor[10];
  char *p;
  char *str;
  uint8_t entry_type = 0;                           // Invalid
  uint8_t value = light_fixed_color_index;

  if (buffer[0] == '#') {                           // Optional hexadecimal entry
    buffer++;
    buffer_length--;
  }

  if (light_subtype >= LST_RGB) {
    char option = (1 == buffer_length) ? buffer[0] : '\0';
    if (('+' == option) && (light_fixed_color_index < MAX_FIXED_COLOR)) {
      value++;
    }
    else if (('-' == option) && (light_fixed_color_index > 1)) {
      value--;
    } else {
      value = atoi(buffer);
    }
  }

  memset(&light_entry_color, 0x00, sizeof(light_entry_color));
  if (strstr(buffer, ",") != nullptr) {             // Decimal entry
    int8_t i = 0;
    for (str = strtok_r(buffer, ",", &p); str && i < 6; str = strtok_r(nullptr, ",", &p)) {
      if (i < 5) {
        light_entry_color[i++] = atoi(str);
      }
    }
    entry_type = 2;                                 // Decimal
  }
  else if (((2 * light_subtype) == buffer_length) || (buffer_length > 3)) {  // Hexadecimal entry
    for (uint8_t i = 0; i < tmin((uint)(buffer_length / 2), sizeof(light_entry_color)); i++) {
      strlcpy(scolor, buffer + (i *2), 3);
      light_entry_color[i] = (uint8_t)strtol(scolor, &p, 16);
    }
    entry_type = 1;                                 // Hexadecimal
  }
  else if ((light_subtype >= LST_RGB) && (value > 0) && (value <= MAX_FIXED_COLOR)) {
    light_fixed_color_index = value;
    memcpy_P(&light_entry_color, &kFixedColor[value -1], 3);
    entry_type = 1;                                 // Hexadecimal
  }
  else if ((value > 199) && (value <= 199 + MAX_FIXED_COLD_WARM)) {
    if (LST_RGBW == light_subtype) {
      memcpy_P(&light_entry_color[3], &kFixedWhite[value -200], 1);
      entry_type = 1;                                 // Hexadecimal
    }
    else if (LST_COLDWARM == light_subtype) {
      memcpy_P(&light_entry_color, &kFixedColdWarm[value -200], 2);
      entry_type = 1;                                 // Hexadecimal
    }
    else if (LST_RGBWC == light_subtype) {
      memcpy_P(&light_entry_color[3], &kFixedColdWarm[value -200], 2);
      entry_type = 1;                                 // Hexadecimal
    }
  }
  if (entry_type) {
    Settings.flag.decimal_text = entry_type -1;
  }
  return (entry_type);
}

/********************************************************************************************/

bool LightCommand(void)
{
  char command [CMDSZ];
  bool serviced = true;
  bool coldim = false;
  bool valid_entry = false;
  char scolor[25];
  char option = (1 == XdrvMailbox.data_len) ? XdrvMailbox.data[0] : '\0';

  int command_code = GetCommandCode(command, sizeof(command), XdrvMailbox.topic, kLightCommands);
  if (-1 == command_code) {
    serviced = false;  // Unknown command
  }
  else if (((CMND_COLOR == command_code) && (light_subtype > LST_SINGLE) && (XdrvMailbox.index > 0) && (XdrvMailbox.index <= 6)) ||
           ((CMND_WHITE == command_code) && (light_subtype == LST_RGBW) && (XdrvMailbox.index == 1))) {
    if (CMND_WHITE == command_code) {
      if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 100)) {
        uint8_t whiteBri = changeUIntScale(XdrvMailbox.payload,0,100,0,255);
        snprintf_P(scolor, sizeof(scolor), PSTR("0,0,0,%d"), whiteBri);
        light_state.setBri(whiteBri); // save target Bri, will be confirmed below
        XdrvMailbox.data = scolor;
        XdrvMailbox.data_len = strlen(scolor);
      } else {
        XdrvMailbox.data_len = 0;
      }
    }
    if (XdrvMailbox.data_len > 0) {
      valid_entry = LightColorEntry(XdrvMailbox.data, XdrvMailbox.data_len);
      if (valid_entry) {
        if (XdrvMailbox.index <= 2) {    // Color(1), 2
          uint8_t old_bri = light_state.getBri();
          // change all channels to specified values
          light_controller.changeChannels(light_entry_color);
          if ((CMND_COLOR == command_code) && (1 == XdrvMailbox.index)) {
            // If Color(1) then reset brightness to maximum;
            light_controller.changeBri(255);
          } else {
            // else set back initial brightness
            light_controller.changeBri(old_bri);
          }

          Settings.light_scheme = 0;
          coldim = true;
        } else {             // Color3, 4, 5 and 6
          for (uint8_t i = 0; i < LST_RGB; i++) {
            Settings.ws_color[XdrvMailbox.index -3][i] = light_entry_color[i];
          }
        }
      }
    }
    if (!valid_entry && (XdrvMailbox.index <= 2)) {
      Response_P(S_JSON_COMMAND_SVALUE, command, LightGetColor(scolor));
    }
    if (XdrvMailbox.index >= 3) {
      scolor[0] = '\0';
      for (uint8_t i = 0; i < LST_RGB; i++) {
        if (Settings.flag.decimal_text) {
          snprintf_P(scolor, 25, PSTR("%s%s%d"), scolor, (i > 0) ? "," : "", Settings.ws_color[XdrvMailbox.index -3][i]);
        } else {
          snprintf_P(scolor, 25, PSTR("%s%02X"), scolor, Settings.ws_color[XdrvMailbox.index -3][i]);
        }
      }
      Response_P(S_JSON_COMMAND_INDEX_SVALUE, command, XdrvMailbox.index, scolor);
    }
  }
  else if ((CMND_CHANNEL == command_code) && (XdrvMailbox.index > 0) && (XdrvMailbox.index <= light_subtype ) ) {
    //  Set "Channel" directly - this allows Color and Direct PWM control to coexist
    if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 100)) {
      light_current_color[XdrvMailbox.index-1] = changeUIntScale(XdrvMailbox.payload,0,100,0,255);
      // if we change channels 1,2,3 then turn off CT mode (unless non-linked)
      if ((XdrvMailbox.index <= 3) && (light_state.isCTRGBLinked())) {
        light_current_color[3] = light_current_color[4] = 0;
      }
      light_controller.changeChannels(light_current_color);
      coldim = true;
    }
    Response_P(S_JSON_COMMAND_INDEX_NVALUE, command, XdrvMailbox.index, light_current_color[XdrvMailbox.index -1] * 100 / 255);
  }
  else if ((CMND_HSBCOLOR == command_code) && (light_subtype >= LST_RGB)) {
    bool validHSB = (XdrvMailbox.data_len > 0);
    if (validHSB) {
      uint16_t HSB[3];
      if (strstr(XdrvMailbox.data, ",") != nullptr) {  // Command with 3 comma separated parameters, Hue (0<H<360), Saturation (0<S<100) AND Brightness (0<B<100)
        for (int i = 0; i < 3; i++) {
          char *substr;

          if (0 == i) {
            substr = strtok(XdrvMailbox.data, ",");
          } else {
            substr = strtok(nullptr, ",");
          }
          if (substr != nullptr) {
            HSB[i] = atoi(substr);
            if (0 < i) {
              HSB[i] = changeUIntScale(HSB[i], 0, 100, 0, 255); // change sat and bri to 0..255
            }
          } else {
            validHSB = false;
          }
        }
      } else {  // Command with only 1 parameter, Hue (0<H<360), Saturation (0<S<100) OR Brightness (0<B<100)
        uint16_t c_hue;
        uint8_t  c_sat, c_bri;
        light_state.getHSB(&c_hue, &c_sat, &c_bri);
        HSB[0] = c_hue;
        HSB[1] = c_sat;
        HSB[2] = c_bri;

        if (1 == XdrvMailbox.index) {
          HSB[0] = XdrvMailbox.payload;
        } else if ((XdrvMailbox.index > 1) && (XdrvMailbox.index < 4)) {
          HSB[XdrvMailbox.index-1] = changeUIntScale(XdrvMailbox.payload,0,100,0,255);
        } else {
          validHSB = false;
        }
      }
      if (validHSB) {
        light_controller.changeHS(HSB[0], HSB[1]);
        light_controller.changeBri(HSB[2]);
        LightPreparePower();
        MqttPublishPrefixTopic_P(RESULT_OR_STAT, PSTR(D_CMND_COLOR));
      }
    } else {
      LightState(0);
    }
  }
#ifdef USE_WS2812  //  ***********************************************************************
  else if ((CMND_LED == command_code) && (LT_WS2812 == light_type) && (XdrvMailbox.index > 0) && (XdrvMailbox.index <= Settings.light_pixels)) {
    if (XdrvMailbox.data_len > 0) {
      char *p;
      uint16_t idx = XdrvMailbox.index;
      Ws2812ForceSuspend();
      for (char *color = strtok_r(XdrvMailbox.data, " ", &p); color; color = strtok_r(nullptr, " ", &p)) {
        if (LightColorEntry(color, strlen(color))) {
          Ws2812SetColor(idx, light_entry_color[0], light_entry_color[1], light_entry_color[2], light_entry_color[3]);
          idx++;
          if (idx > Settings.light_pixels) break;
        } else {
          break;
        }
      }

      Ws2812ForceUpdate();
    }
    Response_P(S_JSON_COMMAND_INDEX_SVALUE, command, XdrvMailbox.index, Ws2812GetColor(XdrvMailbox.index, scolor));
  }
  else if ((CMND_PIXELS == command_code) && (LT_WS2812 == light_type)) {
    if ((XdrvMailbox.payload > 0) && (XdrvMailbox.payload <= WS2812_MAX_LEDS)) {
      Settings.light_pixels = XdrvMailbox.payload;
      Settings.light_rotation = 0;
      Ws2812Clear();
      light_update = 1;
    }
    Response_P(S_JSON_COMMAND_NVALUE, command, Settings.light_pixels);
  }
  else if ((CMND_ROTATION == command_code) && (LT_WS2812 == light_type)) {
    if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload < Settings.light_pixels)) {
      Settings.light_rotation = XdrvMailbox.payload;
    }
    Response_P(S_JSON_COMMAND_NVALUE, command, Settings.light_rotation);
  }
  else if ((CMND_WIDTH == command_code) && (LT_WS2812 == light_type) && (XdrvMailbox.index > 0) && (XdrvMailbox.index <= 4)) {
    if (1 == XdrvMailbox.index) {
      if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 4)) {
        Settings.light_width = XdrvMailbox.payload;
      }
      Response_P(S_JSON_COMMAND_NVALUE, command, Settings.light_width);
    } else {
      if ((XdrvMailbox.payload > 0) && (XdrvMailbox.payload < 32)) {
        Settings.ws_width[XdrvMailbox.index -2] = XdrvMailbox.payload;
      }
      Response_P(S_JSON_COMMAND_INDEX_NVALUE, command, XdrvMailbox.index, Settings.ws_width[XdrvMailbox.index -2]);
    }
  }
#endif  // USE_WS2812 ************************************************************************
  else if ((CMND_SCHEME == command_code) && (light_subtype >= LST_RGB)) {
    uint8_t max_scheme = (LT_WS2812 == light_type) ? LS_MAX + WS2812_SCHEMES : LS_MAX -1;
    if (('+' == option) && (Settings.light_scheme < max_scheme)) {
      XdrvMailbox.payload = Settings.light_scheme + ((0 == Settings.light_scheme) ? 2 : 1);  // Skip wakeup
    }
    else if (('-' == option) && (Settings.light_scheme > 0)) {
      XdrvMailbox.payload = Settings.light_scheme - ((2 == Settings.light_scheme) ? 2 : 1);  // Skip wakeup
    }
    if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= max_scheme)) {
      Settings.light_scheme = XdrvMailbox.payload;
      if (LS_WAKEUP == Settings.light_scheme) {
        light_wakeup_active = 3;
      }
      LightPowerOn();
      strip_timer_counter = 0;
      // Publish state message for Hass
      if (Settings.flag3.hass_tele_on_power) { MqttPublishTeleState(); }
    }
    Response_P(S_JSON_COMMAND_NVALUE, command, Settings.light_scheme);
  }
  else if (CMND_WAKEUP == command_code) {
    if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 100)) {
      Settings.light_dimmer = XdrvMailbox.payload;
    }
    light_wakeup_active = 3;
    Settings.light_scheme = LS_WAKEUP;
    LightPowerOn();
    Response_P(S_JSON_COMMAND_SVALUE, command, D_JSON_STARTED);
  }
  else if ((CMND_COLORTEMPERATURE == command_code) && ((LST_COLDWARM == light_subtype) || (LST_RGBWC == light_subtype))) { // ColorTemp
    uint16_t ct = light_state.getCT();
    if (option != '\0') {
      if ('+' == option) {
        ct = (ct > (500-34)) ? 500 : ct + 34;
      }
      else if ('-' == option) {
        ct = (ct < (153+34)) ? 153 : ct - 34;
      }
    } else {
      ct = XdrvMailbox.payload;
    }
    if ((ct >= 153) && (ct <= 500)) {  // https://developers.meethue.com/documentation/core-concepts
      //LightSetColorTemp(XdrvMailbox.payload);
      light_controller.changeCT(ct);
      coldim = true;
    } else {
      Response_P(S_JSON_COMMAND_NVALUE, command, ct);
    }
  }
  else if (CMND_DIMMER == command_code) {
    uint32_t dimmer = light_state.getDimmer();
    if ('+' == option) {
      XdrvMailbox.payload = (dimmer > 89) ? 100 : dimmer + 10;
    }
    else if ('-' == option) {
      XdrvMailbox.payload = (dimmer < 11) ? 1 : dimmer - 10;
    }
    if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 100)) {
      light_controller.changeDimmer(XdrvMailbox.payload);
      light_update = 1;
      coldim = true;
    } else {
      Response_P(S_JSON_COMMAND_NVALUE, command, Settings.light_dimmer);
    }
  }
  else if (CMND_LEDTABLE == command_code) {
    if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 2)) {
      switch (XdrvMailbox.payload) {
      case 0: // Off
      case 1: // On
        Settings.light_correction = XdrvMailbox.payload;
        break;
      case 2: // Toggle
        Settings.light_correction ^= 1;
        break;
      }
      light_update = 1;
    }
    Response_P(S_JSON_COMMAND_SVALUE, command, GetStateText(Settings.light_correction));
  }
  else if (CMND_RGBWWTABLE == command_code) {
    bool validtable = (XdrvMailbox.data_len > 0);
    char scolor[25];
    if (validtable) {
      if (strstr(XdrvMailbox.data, ",") != nullptr) {  // Command with up to 5 comma separated parameters
        for (int i = 0; i < LST_RGBWC; i++) {
          char *substr;

          if (0 == i) {
            substr = strtok(XdrvMailbox.data, ",");
          } else {
            substr = strtok(nullptr, ",");
          }
          if (substr != nullptr) {
            Settings.rgbwwTable[i] = atoi(substr);
          }
        }
      }
      light_update = 1;
    }
    scolor[0] = '\0';
    for (uint8_t i = 0; i < LST_RGBWC; i++) {
      snprintf_P(scolor, 25, PSTR("%s%s%d"), scolor, (i > 0) ? "," : "", Settings.rgbwwTable[i]);
    }
    Response_P(S_JSON_COMMAND_INDEX_SVALUE, command, XdrvMailbox.index, scolor);
  }
  else if (CMND_FADE == command_code) {
    switch (XdrvMailbox.payload) {
    case 0: // Off
    case 1: // On
      Settings.light_fade = XdrvMailbox.payload;
      break;
    case 2: // Toggle
      Settings.light_fade ^= 1;
      break;
    }
    Response_P(S_JSON_COMMAND_SVALUE, command, GetStateText(Settings.light_fade));
  }
  else if (CMND_SPEED == command_code) {  // 1 - fast, 20 - very slow
    if (('+' == option) && (Settings.light_speed > 1)) {
      XdrvMailbox.payload = Settings.light_speed -1;
    }
    else if (('-' == option) && (Settings.light_speed < STATES)) {
      XdrvMailbox.payload = Settings.light_speed +1;
    }
    if ((XdrvMailbox.payload > 0) && (XdrvMailbox.payload <= STATES)) {
      Settings.light_speed = XdrvMailbox.payload;
    }
    Response_P(S_JSON_COMMAND_NVALUE, command, Settings.light_speed);
  }
  else if (CMND_WAKEUPDURATION == command_code) {
    if ((XdrvMailbox.payload > 0) && (XdrvMailbox.payload < 3001)) {
      Settings.light_wakeup = XdrvMailbox.payload;
      light_wakeup_active = 0;
    }
    Response_P(S_JSON_COMMAND_NVALUE, command, Settings.light_wakeup);
  }
  else if (CMND_UNDOCA == command_code) {  // Theos legacy status
    LightGetColor(scolor, true);  // force hex whatever Option 17
    scolor[6] = '\0';  // RGB only
    Response_P(PSTR("%s,%d,%d,%d,%d,%d"), scolor, Settings.light_fade, Settings.light_correction, Settings.light_scheme, Settings.light_speed, Settings.light_width);
    MqttPublishPrefixTopic_P(STAT, XdrvMailbox.topic);
    mqtt_data[0] = '\0';
  }
  else {
    serviced = false;  // Unknown command
  }

  if (coldim) {
    LightPreparePower();
  }

  return serviced;
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xdrv04(uint8_t function)
{
  bool result = false;

  if (light_type) {
    switch (function) {
      case FUNC_PRE_INIT:
        LightInit();
        break;
      case FUNC_EVERY_50_MSECOND:
        LightAnimate();
#ifdef USE_ARILUX_RF
        if (pin[GPIO_ARIRFRCV] < 99) AriluxRfHandler();
#endif  // USE_ARILUX_RF
        break;
#ifdef USE_ARILUX_RF
      case FUNC_EVERY_SECOND:
        if (10 == uptime) AriluxRfInit();  // Needs rest before enabling RF interrupts
        break;
#endif  // USE_ARILUX_RF
      case FUNC_COMMAND:
        result = LightCommand();
        break;
      case FUNC_SET_POWER:
        LightSetPower();
        break;
    }
  }
  return result;
}
