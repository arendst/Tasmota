/*
  xdrv_04_light.ino - PWM, WS2812 and sonoff led support for Tasmota

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

#ifdef USE_LIGHT
/*********************************************************************************************\
 * PWM, WS2812, Sonoff B1, AiLight, Sonoff Led and BN-SZ01, H801, MagicHome and Arilux
 *
 * light_type  Module     Color  ColorTemp  Modules
 * ----------  ---------  -----  ---------  ----------------------------
 *  0          -                 no         (Sonoff Basic)
 *  1          PWM1       W      no         (Sonoff BN-SZ)
 *  2          PWM2       CW     yes        (Sonoff Led)
 *  3          PWM3       RGB    no         (H801, MagicHome and Arilux LC01)
 *  4          PWM4       RGBW   no         (H801, MagicHome and Arilux)
 *  5          PWM5       RGBCW  yes        (H801, Arilux LC11)
 *  9          reserved          no
 * 10          reserved          yes
 * 11          +WS2812    RGB    no         (One WS2812 RGB or RGBW ledstrip)
 * 12          AiLight    RGBW   no
 * 13          Sonoff B1  RGBCW  yes
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
 *
 * As there have been lots of changes in light control, here is a summary out
 * the whole flow from setting colors to drving the PMW pins.
 *
 * 1.  To change colors, always use 'light_controller' object.
 *     'light_state' is only to be used to read current state.
 *  .a For color bulbs, set color via changeRGB() or changeHS() for Hue/Sat.
 *     Set the overall brightness changeBri(0..255) or changeDimmer(0..100%)
 *     RGB and Hue/Sat are always kept in sync. Internally, RGB are stored at
 *     full range (max brightness) so that when you reduce brightness and
 *     raise it back again, colors don't change due to rounding errors.
 *  .b For white bulbs with Cold/Warm colortone, use changeCW() or changeCT()
 *     to change color-tone. Set overall brightness separately.
 *     Color-tone temperature can range from 153 (Cold) to 500 (Warm).
 *     CW channels are stored at full brightness to avoid rounding errors.
 *  .c Alternatively, you can set all 5 channels at once with changeChannels(),
 *     in this case it will also set the corresponding brightness.
 *
 * 2.a After any change, the Settings object is updated so that changes
 *     survive a reboot and can be stored in flash - in saveSettings()
 *  .b Actual channel values are computed from RGB or CT combined with brightness.
 *     Range is still 0..255 (8 bits) - in getActualRGBCW()
 *  .c The 5 internal channels RGBWC are mapped to the actual channels supported
 *     by the light_type: in calcLevels()
 *     1 channel  - 0:Brightness
 *     2 channels - 0:Coldwhite 1:Warmwhite
 *     3 channels - 0:Red 1:Green 2:Blue
 *     4 chennels - 0:Red 1:Green 2:Blue 3:White
 *     5 chennels - 0:Red 1:Green 2:Blue 3:ColdWhite 4:Warmwhite
 *
 * 3.  In LightAnimate(), final PWM values are computed at next tick.
 *  .a If color did not change since last tick - ignore.
 *  .b Extend resolution from 8 bits to 10 bits, which makes a significant
 *     difference when applying gamma correction at low brightness.
 *  .c Apply Gamma Correction if LedTable==1 (by default).
 *     Gamma Correction uses an adaptative resolution table from 11 to 8 bits.
 *  .d For Warm/Cold-white channels, Gamma correction is calculated in combined mode.
 *     Ie. total white brightness (C+W) is used for Gamma correction and gives
 *     the overall light power required. Then this light power is split among
 *     Wamr/Cold channels.
 *  .e Gamma correction is still applied to 8 bits channels for compatibility
 *     with other non-PMW modules.
 *  .f Apply color balance correction from rgbwwTable[].
 *     Note: correction is done after Gamma correction, it is meant
 *     to adjust leds with different power
 *  .g If rgbwwTable[4] is zero, blend RGB with White and adjust the level of
 *     White channel according to rgbwwTable[3]
 *  .h Scale ranges from 10 bits to 0..PWMRange (by default 1023) so no change
 *     by default.
 *  .i Apply port remapping from Option37
 *  .j Invert PWM value if port is of type PMWxi instead of PMWx
 *  .k Apply PWM value with analogWrite() - if pin is configured
 *
\*********************************************************************************************/

#define XDRV_04              4
// #define DEBUG_LIGHT

enum LightSchemes { LS_POWER, LS_WAKEUP, LS_CYCLEUP, LS_CYCLEDN, LS_RANDOM, LS_MAX };

const uint8_t LIGHT_COLOR_SIZE = 25;   // Char array scolor size

const char kLightCommands[] PROGMEM = "|"  // No prefix
  D_CMND_COLOR "|" D_CMND_COLORTEMPERATURE "|" D_CMND_DIMMER "|" D_CMND_DIMMER_RANGE "|" D_CMND_LEDTABLE "|" D_CMND_FADE "|"
  D_CMND_RGBWWTABLE "|" D_CMND_SCHEME "|" D_CMND_SPEED "|" D_CMND_WAKEUP "|" D_CMND_WAKEUPDURATION "|"
  D_CMND_WHITE "|" D_CMND_CHANNEL "|" D_CMND_HSBCOLOR "|UNDOCA" ;

void (* const LightCommand[])(void) PROGMEM = {
  &CmndColor, &CmndColorTemperature, &CmndDimmer, &CmndDimmerRange, &CmndLedTable, &CmndFade,
  &CmndRgbwwTable, &CmndScheme, &CmndSpeed, &CmndWakeup, &CmndWakeupDuration,
  &CmndWhite, &CmndChannel, &CmndHsbColor, &CmndUndocA };

// Light color mode, either RGB alone, or white-CT alone, or both only available if ct_rgb_linked is false
enum LightColorModes {
  LCM_RGB = 1, LCM_CT = 2, LCM_BOTH = 3 };

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

// New version of Gamma correction compute
// Instead of a table, we do a multi-linear approximation, which is close enough
// At low levels, the slope is a bit higher than actual gamma, to make changes smoother
// Internal resolution is 10 bits.

typedef struct gamma_table_t {
  uint16_t to_src;
  uint16_t to_gamma;
} gamma_table_t;

const gamma_table_t gamma_table[] = {   // don't put in PROGMEM for performance reasons
  {    1,      1 },
  {    4,      1 },
  {  209,     13 },
  {  312,     41 },
  {  457,    106 },
  {  626,    261 },
  {  762,    450 },
  {  895,    703 },
  { 1023,   1023 },
  { 0xFFFF, 0xFFFF }          // fail-safe if out of range
};

// simplified Gamma table for Fade, cheating a little at low brightness
const gamma_table_t gamma_table_fast[] = {
  {     0,      0 },
  {   384,     67 },
  {   768,    467 },
  {  1023,   1023 },
  { 0xFFFF, 0xFFFF }          // fail-safe if out of range
};

// For reference, below are the computed gamma tables, via ledGamma()
// for 8 bits output:
//   0,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
//   1,  1,  1,  1,  1,  1,  1,  1,  1,  2,  2,  2,  2,  2,  2,  2,
//   2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  3,  3,  3,  3,  3,  3,
//   3,  3,  3,  3,  3,  3,  4,  4,  4,  4,  5,  5,  5,  6,  6,  6,
//   6,  7,  7,  7,  7,  8,  8,  8,  9,  9,  9,  9, 10, 10, 10, 11,
//  11, 12, 12, 12, 13, 13, 14, 14, 15, 15, 16, 16, 17, 17, 17, 18,
//  18, 19, 19, 20, 20, 21, 21, 21, 22, 22, 23, 23, 24, 24, 25, 25,
//  25, 26, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 36, 37, 38,
//  39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 50, 51, 52, 53,
//  54, 55, 56, 57, 58, 59, 60, 61, 61, 62, 63, 64, 65, 67, 68, 69,
//  71, 72, 73, 75, 76, 78, 79, 80, 82, 83, 85, 86, 87, 89, 90, 91,
//  93, 94, 95, 97, 98,100,101,102,104,105,107,108,109,111,112,114,
// 116,118,120,122,124,125,127,129,131,133,135,137,139,141,143,144,
// 146,148,150,152,154,156,158,160,162,164,166,168,170,171,173,175,
// 178,180,183,185,188,190,193,195,198,200,203,205,208,210,213,215,
// 218,220,223,225,228,230,233,235,238,240,243,245,248,250,253,255
//
// and for 10 bits output:
//    0,   1,   1,   1,   2,   2,   2,   2,   3,   3,   3,   3,   4,   4,   4,   4,
//    5,   5,   5,   5,   5,   6,   6,   6,   6,   7,   7,   7,   7,   8,   8,   8,
//    8,   8,   9,   9,   9,   9,  10,  10,  10,  10,  11,  11,  11,  11,  12,  12,
//   12,  12,  13,  13,  13,  14,  15,  16,  17,  18,  20,  21,  22,  23,  24,  25,
//   26,  27,  28,  29,  30,  31,  33,  34,  35,  36,  37,  38,  39,  40,  41,  43,
//   45,  47,  49,  50,  52,  54,  56,  58,  59,  61,  63,  65,  67,  68,  70,  72,
//   74,  76,  77,  79,  81,  83,  84,  86,  88,  90,  92,  93,  95,  97,  99, 101,
//  102, 104, 106, 110, 113, 117, 121, 124, 128, 132, 135, 139, 143, 146, 150, 154,
//  158, 162, 166, 169, 173, 177, 180, 184, 188, 191, 195, 199, 202, 206, 210, 213,
//  217, 221, 224, 228, 232, 235, 239, 243, 246, 250, 254, 257, 261, 267, 272, 278,
//  283, 289, 294, 300, 305, 311, 317, 322, 328, 333, 339, 344, 350, 356, 361, 367,
//  372, 378, 383, 389, 394, 400, 406, 411, 417, 422, 428, 433, 439, 444, 450, 458,
//  465, 473, 480, 488, 496, 503, 511, 518, 526, 534, 541, 549, 557, 564, 572, 579,
//  587, 595, 602, 610, 617, 627, 635, 642, 650, 657, 665, 673, 680, 688, 695, 703,
//  713, 723, 733, 743, 753, 763, 773, 783, 793, 803, 813, 823, 833, 843, 853, 863,
//  873, 883, 893, 903, 913, 923, 933, 943, 953, 963, 973, 983, 993,1003,1013,1023
//
// Output for Dimmer 0..100 values
//   0,  1,  2,  3,  3,  4,  4,  5,  5,  6,  7,  7,  8,  8,  9,
//  10, 10, 11, 12, 12, 13, 15, 17, 21, 23, 26, 28, 31, 34, 37,
//  40, 43, 49, 52, 58, 61, 67, 70, 76, 79, 84, 90, 93, 99,102,
// 110,117,128,135,146,158,166,177,184,195,202,213,221,232,239,
// 250,261,272,289,300,317,328,344,356,372,389,400,417,428,444,
// 458,480,496,518,534,557,579,595,617,635,657,673,695,713,743,
// 773,793,823,843,873,893,923,943,973,993,1023

struct LIGHT {
  uint32_t strip_timer_counter = 0;  // Bars and Gradient
  power_t power = 0;                      // Power<x> for each channel if SetOption68, or boolean if single light

  uint16_t wakeup_counter = 0;

  uint8_t entry_color[LST_MAX];
  uint8_t current_color[LST_MAX];
  uint8_t new_color[LST_MAX];
  uint8_t last_color[LST_MAX];
  uint8_t color_remap[LST_MAX];

  uint8_t wheel = 0;
  uint8_t random = 0;
  uint8_t subtype = 0;                    // LST_ subtype
  uint8_t device = 0;
  uint8_t old_power = 1;
  uint8_t wakeup_active = 0;
  uint8_t wakeup_dimmer = 0;
  uint8_t fixed_color_index = 1;
  uint8_t pwm_offset = 0;                 // Offset in color buffer
  uint8_t max_scheme = LS_MAX -1;

  bool update = true;
  bool pwm_multi_channels = false;        // SetOption68, treat each PWM channel as an independant dimmer

  bool     fade_running = false;
  uint16_t fade_start_10[LST_MAX] = {0,0,0,0,0};
  uint16_t fade_cur_10[LST_MAX];
  uint16_t fade_end_10[LST_MAX];         // 10 bits resolution target channel values
  uint16_t fade_duration = 0;            // duration of fade in milliseconds
  uint32_t fade_start = 0;               // fade start time in milliseconds, compared to millis()
} Light;

power_t LightPower(void)
{
  return Light.power;                     // Make external
}

uint8_t LightDevice(void)
{
  return Light.device;                    // Make external
}

static uint32_t min3(uint32_t a, uint32_t b, uint32_t c) {
  return (a < b && a < c) ? a : (b < c) ? b : c;
}

//
// LightStateClass
// This class is an abstraction of the current light state.
// It allows for b/w, full colors, or white colortone
//
// This class has 2 independant slots
// 1/ Brightness 0.255, dimmer controls both RGB and WC (warm-cold)
// 1/ RGB and Hue/Sat - always kept in sync and stored at full brightness,
//    i.e. R G or B are 255
//    briRGB specifies the brightness for the RGB slot.
//    If Brightness is 0, it is equivalent to Off (for compatibility)
//    Dimmer is Brightness converted to range 0..100
// 2/ White with colortone - or CW (Cold / Warm)
//    ct is 153..500 temperature (153=cold, 500=warm)
//    briCT specifies the brightness for white channel
//
// Dimmer (0.100) is automatically derived from brightness
//
// INVARIANTS:
// 1.  RGB components are always stored at full brightness and modulated with briRGB
//     ((R == 255) || (G == 255) || (B == 255))
// 2.  RGB and Hue/Sat are always kept in sync whether you use setRGB() or setHS()
// 3.  Warm/Cold white channels are always stored at full brightness
//     ((WW == 255) || (WC == 255))
// 4.  WC/WW and CT are always kept in sync.
//     Note: if you use setCT() then WC+WW == 255 (both channels are linked)
//     but if you use setCW() both channels can be set independantly
// 5.  If RGB or CT channels are deactivated, then corresponding brightness is zero
//     if (colot_tone == LCM_RGB) then briCT = 0
//     if (color_tone == LCM_CT)  then briRGB = 0
//     if (colot_tone == LCM_BOTH) then briRGB and briCT can have any values
//
// Note:  If you want the actual RGB, you need to multiply with Bri, or use getActualRGBCW()
// Note: all values are stored as unsigned integer, no floats.
// Note: you can query vaules from this singleton. But to change values,
//   use the LightController - changing this object will have no effect on actual light.
//
class LightStateClass {
  private:
    uint16_t _hue = 0;  // 0..359
    uint8_t  _sat = 255;  // 0..255
    uint8_t  _briRGB = 255;  // 0..255
    // dimmer is same as _bri but with a range of 0%-100%
    uint8_t  _r = 255;  // 0..255
    uint8_t  _g = 255;  // 0..255
    uint8_t  _b = 255;  // 0..255

    uint8_t  _subtype = 0;  // local copy of Light.subtype, if we need multiple lights
    uint16_t _ct = 153;  // 153..500, default to 153 (cold white)
    uint8_t  _wc = 255;  // white cold channel
    uint8_t  _ww = 0;    // white warm channel
    uint8_t  _briCT = 255;

    uint8_t  _color_mode = LCM_RGB; // RGB by default

  public:
    LightStateClass() {
      //AddLog_P2(LOG_LEVEL_DEBUG_MORE, "LightStateClass::Constructor RGB raw (%d %d %d) HS (%d %d) bri (%d)", _r, _g, _b, _hue, _sat, _bri);
    }

    void setSubType(uint8_t sub_type) {
      _subtype = sub_type;    // set sub_type at initialization, shoudln't be changed afterwards
    }

    // This function is a bit hairy, it will try to match the rerquired
    // colormode with the features of the device:
    //   LST_NONE:      LCM_RGB
    //   LST_SINGLE:    LCM_RGB
    //   LST_COLDWARM:  LCM_CT
    //   LST_RGB:       LCM_RGB
    //   LST_RGBW:      LCM_RGB, LCM_CT or LCM_BOTH
    //   LST_RGBWC:     LCM_RGB, LCM_CT or LCM_BOTH
    uint8_t setColorMode(uint8_t cm) {
      uint8_t prev_cm = _color_mode;
      if (cm < LCM_RGB) { cm = LCM_RGB; }
      if (cm > LCM_BOTH) { cm = LCM_BOTH; }
      uint8_t maxbri = (_briRGB >= _briCT) ? _briRGB : _briCT;

      switch (_subtype) {
        case LST_COLDWARM:
          _color_mode = LCM_CT;
          break;

        case LST_NONE:
        case LST_SINGLE:
        case LST_RGB:
        default:
          _color_mode = LCM_RGB;
          break;

        case LST_RGBW:
        case LST_RGBWC:
          _color_mode = cm;
          break;
      }
      if (LCM_RGB == _color_mode) {
        _briCT = 0;
        if (0 == _briRGB) { _briRGB = maxbri; }
      }
      if (LCM_CT == _color_mode) {
        _briRGB = 0;
        if (0 == _briCT) { _briCT = maxbri; }
      }
#ifdef DEBUG_LIGHT
      AddLog_P2(LOG_LEVEL_DEBUG_MORE, "LightStateClass::setColorMode prev_cm (%d) req_cm (%d) new_cm (%d)", prev_cm, cm, _color_mode);
#endif
      return prev_cm;
    }

    inline uint8_t getColorMode() {
      return _color_mode;
    }

    void addRGBMode() {
      setColorMode(_color_mode | LCM_RGB);
    }
    void addCTMode() {
      setColorMode(_color_mode | LCM_CT);
    }

    // Get RGB color, always at full brightness (ie. one of the components is 255)
    void getRGB(uint8_t *r, uint8_t *g, uint8_t *b) {
      if (r) { *r = _r; }
      if (g) { *g = _g; }
      if (b) { *b = _b; }
    }

    // get full brightness values for warm and cold channels.
    // either w=c=0 (off) or w+c >= 255
    void getCW(uint8_t *rc, uint8_t *rw) {
      if (rc) { *rc = _wc; }
      if (rw) { *rw = _ww; }
    }

    // Get the actual values for each channel, ie multiply with brightness
    void getActualRGBCW(uint8_t *r, uint8_t *g, uint8_t *b, uint8_t *c, uint8_t *w) {
      bool rgb_channels_on = _color_mode & LCM_RGB;
      bool ct_channels_on = _color_mode & LCM_CT;

      if (r) { *r = rgb_channels_on ? changeUIntScale(_r, 0, 255, 0, _briRGB) : 0; }
      if (g) { *g = rgb_channels_on ? changeUIntScale(_g, 0, 255, 0, _briRGB) : 0; }
      if (b) { *b = rgb_channels_on ? changeUIntScale(_b, 0, 255, 0, _briRGB) : 0; }

      if (c) { *c = ct_channels_on  ? changeUIntScale(_wc, 0, 255, 0, _briCT) : 0; }
      if (w) { *w = ct_channels_on  ? changeUIntScale(_ww, 0, 255, 0, _briCT) : 0; }
    }

    uint8_t getChannels(uint8_t *channels) {
      getActualRGBCW(&channels[0], &channels[1], &channels[2], &channels[3], &channels[4]);
    }

    void getChannelsRaw(uint8_t *channels) {
      channels[0] = _r;
      channels[1] = _g;
      channels[2] = _b;
      channels[3] = _wc;
      channels[4] = _ww;
    }

    void getHSB(uint16_t *hue, uint8_t *sat, uint8_t *bri) {
      if (hue) { *hue = _hue; }
      if (sat) { *sat = _sat; }
      if (bri) { *bri = _briRGB; }
    }

    // getBri() is guaranteed to give the same result as setBri() - no rounding errors.
    uint8_t getBri(void) {
      // return the max of _briCT and _briRGB
      return (_briRGB >= _briCT) ? _briRGB : _briCT;
    }

    // get the white Brightness
    inline uint8_t getBriCT() {
      return _briCT;
    }

    static inline uint8_t DimmerToBri(uint8_t dimmer) {
      return changeUIntScale(dimmer, 0, 100, 0, 255);  // 0..255
    }
    static uint8_t BriToDimmer(uint8_t bri) {
      uint8_t dimmer = changeUIntScale(bri, 0, 255, 0, 100);
      // if brightness is non zero, force dimmer to be non-zero too
      if ((dimmer == 0) && (bri > 0)) { dimmer = 1; }
      return dimmer;
    }

    uint8_t getDimmer(uint32_t mode = 0) {
      uint8_t bri;
      switch (mode) {
        case 1:
          bri = getBriRGB();
          break;
        case 2:
          bri = getBriCT();
          break;
        default:
          bri = getBri();
          break;
      }
      return BriToDimmer(bri);
    }

    inline uint16_t getCT() {
      return _ct; // 153..500
    }

    // get current color in XY format
    void getXY(float *x, float *y) {
      RgbToXy(_r, _g, _b, x, y);
    }

    // setters -- do not use directly, use the light_controller instead
    // sets both master Bri and whiteBri
    void setBri(uint8_t bri) {
      setBriRGB(_color_mode & LCM_RGB ? bri : 0);
      setBriCT(_color_mode & LCM_CT ? bri : 0);
#ifdef DEBUG_LIGHT
      AddLog_P2(LOG_LEVEL_DEBUG_MORE, "LightStateClass::setBri RGB raw (%d %d %d) HS (%d %d) bri (%d)", _r, _g, _b, _hue, _sat, _briRGB);
#endif
    }

    // changes the RGB brightness alone
    uint8_t setBriRGB(uint8_t bri_rgb) {
      uint8_t prev_bri = _briRGB;
      _briRGB = bri_rgb;
      if (bri_rgb > 0) { addRGBMode(); }
      return prev_bri;
    }

    // changes the white brightness alone
    uint8_t setBriCT(uint8_t bri_ct) {
      uint8_t prev_bri = _briCT;
      _briCT = bri_ct;
      if (bri_ct > 0) { addCTMode(); }
      return prev_bri;
    }

    inline uint8_t getBriRGB() {
      return _briRGB;
    }

    void setDimmer(uint8_t dimmer) {
      setBri(DimmerToBri(dimmer));
    }

    void setCT(uint16_t ct) {
      if (0 == ct) {
        // disable ct mode
        setColorMode(LCM_RGB);  // try deactivating CT mode, setColorMode() will check which is legal
      } else {
        ct = (ct < 153 ? 153 : (ct > 500 ? 500 : ct));
        _ww = changeUIntScale(ct, 153, 500, 0, 255);
        _wc = 255 - _ww;
        _ct = ct;
        addCTMode();
      }
#ifdef DEBUG_LIGHT
      AddLog_P2(LOG_LEVEL_DEBUG_MORE, "LightStateClass::setCT RGB raw (%d %d %d) HS (%d %d) briRGB (%d) briCT (%d) CT (%d)", _r, _g, _b, _hue, _sat, _briRGB, _briCT, _ct);
#endif
    }

    // Manually set Cold/Warm channels.
    // There are two modes:
    // 1. (free_range == false, default)
    //    In this mode there is only one virtual white channel with color temperature
    //    As a side effect, WC+WW = 255. It means also that the sum of light power
    //    from white LEDs is always equal to briCT. It is not possible here
    //    to set both white LEDs at full power, hence protecting power supplies
    //    from overlaoding.
    // 2. (free_range == true)
    //    In this mode, values of WC and WW are free -- both channels can be set
    //    at full power.
    //    In this mode, we always scale both channels so that one at least is 255.
    //
    // We automatically adjust briCT to have the right values of channels
    void setCW(uint8_t c, uint8_t w, bool free_range = false) {
      uint16_t max = (w > c) ? w : c;   // 0..255
      uint16_t sum = c + w;

      if (0 == max) {
        _briCT = 0;       // brightness set to null
        setColorMode(LCM_RGB);  // try deactivating CT mode, setColorMode() will check which is legal
      } else {
        if (!free_range) {
          // we need to normalize to sum = 255
          _ww = changeUIntScale(w, 0, sum, 0, 255);
          _wc = 255 - _ww;
        } else {  // we normalize to max = 255
          _ww = changeUIntScale(w, 0, max, 0, 255);
          _wc = changeUIntScale(c, 0, max, 0, 255);
        }
        _ct = changeUIntScale(w, 0, sum, 153, 500);
        addCTMode();   // activate CT mode if needed
        if (_color_mode & LCM_CT) { _briCT = free_range ? max : (sum > 255 ? 255 : sum); }
      }
#ifdef DEBUG_LIGHT
      AddLog_P2(LOG_LEVEL_DEBUG_MORE, "LightStateClass::setCW CW (%d %d) CT (%d) briCT (%d)", c, w, _ct, _briCT);
#endif
    }

    // sets RGB and returns the Brightness. Bri is updated unless keep_bri is true
    uint8_t setRGB(uint8_t r, uint8_t g, uint8_t b, bool keep_bri = false) {
      uint16_t hue;
      uint8_t  sat;
#ifdef DEBUG_LIGHT
      AddLog_P2(LOG_LEVEL_DEBUG_MORE, "LightStateClass::setRGB RGB input (%d %d %d)", r, g, b);
#endif

      uint32_t max = (r > g && r > b) ? r : (g > b) ? g : b;   // 0..255

      if (0 == max) {
        r = g = b = 255;
        setColorMode(LCM_CT);   // try deactivating RGB, setColorMode() will check if this is legal
      } else {
        if (255 > max) {
          // we need to normalize rgb
          r = changeUIntScale(r, 0, max, 0, 255);
          g = changeUIntScale(g, 0, max, 0, 255);
          b = changeUIntScale(b, 0, max, 0, 255);
        }
        addRGBMode();
      }
      if (!keep_bri) {
        _briRGB = (_color_mode & LCM_RGB) ? max : 0;
      }

      RgbToHsb(r, g, b, &hue, &sat, nullptr);
      _r = r;
      _g = g;
      _b = b;
      _hue = hue;
      _sat = sat;
#ifdef DEBUG_LIGHT
      AddLog_P2(LOG_LEVEL_DEBUG_MORE, "LightStateClass::setRGB RGB raw (%d %d %d) HS (%d %d) bri (%d)", _r, _g, _b, _hue, _sat, _briRGB);
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
      addRGBMode();
#ifdef DEBUG_LIGHT
      AddLog_P2(LOG_LEVEL_DEBUG_MORE, "LightStateClass::setHS HS (%d %d) rgb (%d %d %d)", hue, sat, r, g, b);
      AddLog_P2(LOG_LEVEL_DEBUG_MORE, "LightStateClass::setHS RGB raw (%d %d %d) HS (%d %d) bri (%d)", _r, _g, _b, _hue, _sat, _briRGB);
#endif
  }

  // set all 5 channels at once, don't modify the values in ANY way
  // Channels are: R G B CW WW
  void setChannelsRaw(uint8_t *channels) {
    _r = channels[0];
    _g = channels[1];
    _b = channels[2];
    _wc = channels[3];
    _ww = channels[4];
}

  // set all 5 channels at once.
  // Channels are: R G B CW WW
  // Brightness is automatically recalculated to adjust channels to the desired values
  void setChannels(uint8_t *channels) {
    setRGB(channels[0], channels[1], channels[2]);
    setCW(channels[3], channels[4], true);  // free range for WC and WW
#ifdef DEBUG_LIGHT
    AddLog_P2(LOG_LEVEL_DEBUG_MORE, "LightStateClass::setChannels (%d %d %d %d %d)",
      channels[0], channels[1], channels[2], channels[3], channels[4]);
    AddLog_P2(LOG_LEVEL_DEBUG_MORE, "LightStateClass::setChannels CT (%d) briRGB (%d) briCT (%d)", _ct, _briRGB, _briCT);
#endif
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

#define POW FastPrecisePowf

void LightStateClass::RgbToXy(uint8_t i_r, uint8_t i_g, uint8_t i_b, float *r_x, float *r_y) {
  float x = 0.31271f;   // default medium white
  float y = 0.32902f;

  if (i_r + i_b + i_g > 0) {
    float r = (float)i_r / 255.0f;
    float g = (float)i_g / 255.0f;
    float b = (float)i_b / 255.0f;
    // https://gist.github.com/popcorn245/30afa0f98eea1c2fd34d
    // Gamma correction
    r = (r > 0.04045f) ? POW((r + 0.055f) / (1.0f + 0.055f), 2.4f) : (r / 12.92f);
    g = (g > 0.04045f) ? POW((g + 0.055f) / (1.0f + 0.055f), 2.4f) : (g / 12.92f);
    b = (b > 0.04045f) ? POW((b + 0.055f) / (1.0f + 0.055f), 2.4f) : (b / 12.92f);

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
  r = (r <= 0.0031308f) ? 12.92f * r : 1.055f * POW(r, (1.0f / 2.4f)) - 0.055f;
  g = (g <= 0.0031308f) ? 12.92f * g : 1.055f * POW(g, (1.0f / 2.4f)) - 0.055f;
  b = (b <= 0.0031308f) ? 12.92f * b : 1.055f * POW(b, (1.0f / 2.4f)) - 0.055f;
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
private:
  LightStateClass *_state;

  // are RGB and CT linked, i.e. if we set CT then RGB channels are off
  bool     _ct_rgb_linked = true;
  bool     _pwm_multi_channels = false;    // treat each channel as independant dimmer

public:
  LightControllerClass(LightStateClass& state) {
    _state = &state;
  }

  void setSubType(uint8_t sub_type) {
    _state->setSubType(sub_type);
  }

  inline bool setCTRGBLinked(bool ct_rgb_linked) {
    bool prev = _ct_rgb_linked;
    if (_pwm_multi_channels) {
      _ct_rgb_linked = false;   // force to false if _pwm_multi_channels is set
    } else {
      _ct_rgb_linked = ct_rgb_linked;
    }
    return prev;
  }

  inline bool isCTRGBLinked() {
    return _ct_rgb_linked;
  }

  inline bool setPWMMultiChannel(bool pwm_multi_channels) {
    bool prev = _pwm_multi_channels;
    _pwm_multi_channels = pwm_multi_channels;
    if (pwm_multi_channels)  setCTRGBLinked(false);    // if pwm multi channel, then unlink RGB and CT
    return prev;
  }

  inline bool isPWMMultiChannel(void) {
    return _pwm_multi_channels;
  }

#ifdef DEBUG_LIGHT
  void debugLogs() {
    uint8_t r,g,b,c,w;
    _state->getActualRGBCW(&r,&g,&b,&c,&w);
    AddLog_P2(LOG_LEVEL_DEBUG_MORE, "LightControllerClass::debugLogs rgb (%d %d %d) cw (%d %d)",
      r, g, b, c, w);
    AddLog_P2(LOG_LEVEL_DEBUG_MORE, "LightControllerClass::debugLogs lightCurrent (%d %d %d %d %d)",
      Light.current_color[0], Light.current_color[1], Light.current_color[2],
      Light.current_color[3], Light.current_color[4]);
  }
#endif

  void loadSettings() {
#ifdef DEBUG_LIGHT
    AddLog_P2(LOG_LEVEL_DEBUG_MORE, "LightControllerClass::loadSettings Settings.light_color (%d %d %d %d %d - %d)",
      Settings.light_color[0], Settings.light_color[1], Settings.light_color[2],
      Settings.light_color[3], Settings.light_color[4], Settings.light_dimmer);
    AddLog_P2(LOG_LEVEL_DEBUG_MORE, "LightControllerClass::loadSettings light_type/sub (%d %d)",
      light_type, Light.subtype);
#endif
    if (_pwm_multi_channels) {
      _state->setChannelsRaw(Settings.light_color);
    } else {
      // first try setting CW, if zero, it select RGB mode
      _state->setCW(Settings.light_color[3], Settings.light_color[4], true);
      _state->setRGB(Settings.light_color[0], Settings.light_color[1], Settings.light_color[2]);

      // only if non-multi channel
      // We apply dimmer in priority to RGB
      uint8_t bri = _state->DimmerToBri(Settings.light_dimmer);
      if (Settings.light_color[0] + Settings.light_color[1] + Settings.light_color[2] > 0) {
        // The default values are #FFFFFFFFFF, in this case we avoid setting all channels
        // at the same time, see #6534
        if ( (DEFAULT_LIGHT_COMPONENT == Settings.light_color[0]) &&
             (DEFAULT_LIGHT_COMPONENT == Settings.light_color[1]) &&
             (DEFAULT_LIGHT_COMPONENT == Settings.light_color[2]) &&
             (DEFAULT_LIGHT_COMPONENT == Settings.light_color[3]) &&
             (DEFAULT_LIGHT_COMPONENT == Settings.light_color[4]) &&
             (DEFAULT_LIGHT_DIMMER    == Settings.light_dimmer) ) {
          _state->setColorMode(LCM_RGB);
        }
        _state->setBriRGB(bri);
      } else {
        _state->setBriCT(bri);
      }
    }
  }

  void changeCTB(uint16_t new_ct, uint8_t briCT) {
    /* Color Temperature (https://developers.meethue.com/documentation/core-concepts)
     *
     * ct = 153 = 2000K = Warm = CCWW = 00FF
     * ct = 500 = 6500K = Cold = CCWW = FF00
     */
    // don't set CT if not supported
    if ((LST_COLDWARM != Light.subtype) && (LST_RGBW > Light.subtype)) {
      return;
    }
    _state->setCT(new_ct);
    _state->setBriCT(briCT);
    if (_ct_rgb_linked) { _state->setColorMode(LCM_CT); }   // try to force CT
    saveSettings();
    calcLevels();
    //debugLogs();
  }

  void changeDimmer(uint8_t dimmer, uint32_t mode = 0) {
    uint8_t bri = changeUIntScale(dimmer, 0, 100, 0, 255);
    switch (mode) {
      case 1:
        changeBriRGB(bri);
        if (_ct_rgb_linked) { _state->setColorMode(LCM_RGB); }   // try to force CT
        break;
      case 2:
        changeBriCT(bri);
        if (_ct_rgb_linked) { _state->setColorMode(LCM_CT); }   // try to force CT
        break;
      default:
        changeBri(bri);
        break;
    }
  }

  void changeBri(uint8_t bri) {
    _state->setBri(bri);
    saveSettings();
    calcLevels();
  }

  void changeBriRGB(uint8_t bri) {
    _state->setBriRGB(bri);
    saveSettings();
    calcLevels();
  }

  void changeBriCT(uint8_t bri) {
    _state->setBriCT(bri);
    saveSettings();
    calcLevels();
  }

  void changeRGB(uint8_t r, uint8_t g, uint8_t b, bool keep_bri = false) {
    _state->setRGB(r, g, b, keep_bri);
    if (_ct_rgb_linked) { _state->setColorMode(LCM_RGB); }   // try to force RGB
    saveSettings();
    calcLevels();
  }

  // calculate the levels for each channel
  // if no parameter, results are stored in Light.current_color
  void calcLevels(uint8_t *current_color = nullptr) {
    uint8_t r,g,b,c,w,briRGB,briCT;
    if (current_color == nullptr) { current_color = Light.current_color; }

    if (_pwm_multi_channels) { // if PWM multi channel, no more transformation required
      _state->getChannelsRaw(current_color);
      return;
    }

    _state->getActualRGBCW(&r,&g,&b,&c,&w);
    briRGB = _state->getBriRGB();
    briCT = _state->getBriCT();

    current_color[0] = current_color[1] = current_color[2] = 0;
    current_color[3] = current_color[4] = 0;
    switch (Light.subtype) {
      case LST_NONE:
        current_color[0] = 255;
        break;
      case LST_SINGLE:
        current_color[0] = briRGB;
        break;
      case LST_COLDWARM:
        current_color[0] = c;
        current_color[1] = w;
        break;
      case LST_RGBW:
      case LST_RGBWC:
        if (LST_RGBWC == Light.subtype) {
          current_color[3] = c;
          current_color[4] = w;
        } else {
          current_color[3] = briCT;
        }
        // continue
      case LST_RGB:
        current_color[0] = r;
        current_color[1] = g;
        current_color[2] = b;
        break;
    }
  }

  void changeHSB(uint16_t hue, uint8_t sat, uint8_t briRGB) {
    _state->setHS(hue, sat);
    _state->setBriRGB(briRGB);
    if (_ct_rgb_linked) { _state->setColorMode(LCM_RGB); }   // try to force RGB
    saveSettings();
    calcLevels();
  }

  // save the current light state to Settings.
  void saveSettings() {
    if (Light.pwm_multi_channels) {
      // simply save each channel
      _state->getChannelsRaw(Settings.light_color);
      Settings.light_dimmer = 100;    // arbitrary value, unused in this mode
    } else {
      uint8_t cm = _state->getColorMode();

      memset(&Settings.light_color[0], 0, sizeof(Settings.light_color));    // clear all channels
      if (LCM_RGB & cm) {   // can be either LCM_RGB or LCM_BOTH
        _state->getRGB(&Settings.light_color[0], &Settings.light_color[1], &Settings.light_color[2]);
        Settings.light_dimmer = _state->BriToDimmer(_state->getBriRGB());
        // anyways we always store RGB with BrightnessRGB
        if (LCM_BOTH == cm) {
          // then store at actual brightness CW/WW if dual mode
          _state->getActualRGBCW(nullptr, nullptr, nullptr, &Settings.light_color[3], &Settings.light_color[4]);
        }
      } else if (LCM_CT == cm) {    // cm can only be LCM_CT
        _state->getCW(&Settings.light_color[3], &Settings.light_color[4]);
        Settings.light_dimmer = _state->BriToDimmer(_state->getBriCT());
      }
    }
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
    if (Light.pwm_multi_channels) {
      _state->setChannelsRaw(channels);
    } else if (LST_COLDWARM == Light.subtype) {
      // remap channels 0-1 to 3-4 if cold/warm
      uint8_t remapped_channels[5] = {0,0,0,channels[0],channels[1]};
      _state->setChannels(remapped_channels);
    } else {
      _state->setChannels(channels);
    }

    saveSettings();
    calcLevels();
  }
};


// the singletons for light state and Light Controller
LightStateClass light_state = LightStateClass();
LightControllerClass light_controller = LightControllerClass(light_state);

/*********************************************************************************************\
 * Change scales from 8 bits to 10 bits and vice versa
\*********************************************************************************************/
// 8 to 10 to 8 is garanteed to give the same result
uint16_t change8to10(uint8_t v) {
  return changeUIntScale(v, 0, 255, 0, 1023);
}
// change from 10 bits to 8 bits, but any non-zero input will be non-zero
uint8_t change10to8(uint16_t v) {
  return (0 == v) ? 0 : changeUIntScale(v, 4, 1023, 1, 255);
}

/*********************************************************************************************\
 * Gamma correction
\*********************************************************************************************/
// Calculate the gamma corrected value for LEDS
uint16_t ledGamma_internal(uint16_t v, const struct gamma_table_t *gt_ptr) {
  uint16_t from_src = 0;
  uint16_t from_gamma = 0;

  for (const gamma_table_t *gt = gt_ptr; ; gt++) {
    uint16_t to_src = gt->to_src;
    uint16_t to_gamma = gt->to_gamma;
    if (v <= to_src) {
      return changeUIntScale(v, from_src, to_src, from_gamma, to_gamma);
    }
    from_src = to_src;
    from_gamma = to_gamma;
  }
}
// 10_10 bits, fast fade mode
uint16_t ledGamma10_10_fast(uint16_t v) {
  return ledGamma_internal(v, gamma_table_fast);
}

// 10 bits in, 10 bits out
uint16_t ledGamma10_10(uint16_t v) {
  return ledGamma_internal(v, gamma_table);
}
// 10 bits resolution, 8 bits in
uint16_t ledGamma10(uint8_t v) {
  return ledGamma10_10(change8to10(v));
}

// Legacy function
uint8_t ledGamma(uint8_t v) {
  return change10to8(ledGamma10(v));
}

/********************************************************************************************/

void LightPwmOffset(uint32_t offset)
{
  Light.pwm_offset = offset;
}

bool LightModuleInit(void)
{
  light_type = LT_BASIC;                    // Use basic PWM control if SetOption15 = 0

  if (Settings.flag.pwm_control) {          // SetOption15 - Switch between commands PWM or COLOR/DIMMER/CT/CHANNEL
    for (uint32_t i = 0; i < MAX_PWMS; i++) {
      if (pin[GPIO_PWM1 +i] < 99) { light_type++; }  // Use Dimmer/Color control for all PWM as SetOption15 = 1
    }
  }

  light_flg = 0;
  if (XlgtCall(FUNC_MODULE_INIT)) {
    // serviced
  }
  else if (SONOFF_BN == my_module_type) {   // PWM Single color led (White)
    light_type = LT_PWM1;
  }
  else if (SONOFF_LED == my_module_type) {  // PWM Dual color led (White warm and cold)
    if (!my_module.io[4]) {                 // Fix Sonoff Led instabilities
      pinMode(4, OUTPUT);                   // Stop floating outputs
      digitalWrite(4, LOW);
    }
    if (!my_module.io[5]) {
      pinMode(5, OUTPUT);                   // Stop floating outputs
      digitalWrite(5, LOW);
    }
    if (!my_module.io[14]) {
      pinMode(14, OUTPUT);                  // Stop floating outputs
      digitalWrite(14, LOW);
    }
    light_type = LT_PWM2;
  }

  if (light_type > LT_BASIC) {
    devices_present++;
  }

  // post-process for lights
  if (Settings.flag3.pwm_multi_channels) {  // SetOption68 - Enable multi-channels PWM instead of Color PWM
    uint32_t pwm_channels = (light_type & 7) > LST_MAX ? LST_MAX : (light_type & 7);
    if (0 == pwm_channels) { pwm_channels = 1; }
    devices_present += pwm_channels - 1;    // add the pwm channels controls at the end
  } else if ((Settings.param[P_RGB_REMAP] & 128) && (LST_RGBW <= (light_type & 7))) {
    // if RGBW or RGBCW, and SetOption37 >= 128, we manage RGB and W separately, hence adding a device
    devices_present++;
  }

  return (light_type > LT_BASIC);
}

void LightInit(void)
{
  Light.device = devices_present;
  Light.subtype = (light_type & 7) > LST_MAX ? LST_MAX : (light_type & 7); // Always 0 - LST_MAX (5)
  Light.pwm_multi_channels = Settings.flag3.pwm_multi_channels;  // SetOption68 - Enable multi-channels PWM instead of Color PWM

  if (LST_RGBW <= Light.subtype) {
    // only change if RGBW or RGBCW
    // do not allow independant RGB and WC colors
    bool ct_rgb_linked = !(Settings.param[P_RGB_REMAP] & 128);
    light_controller.setCTRGBLinked(ct_rgb_linked);
  }

  if ((LST_SINGLE <= Light.subtype) && Light.pwm_multi_channels) {
    // we treat each PWM channel as an independant one, hence we switch to
    light_controller.setPWMMultiChannel(true);
    Light.device = devices_present - Light.subtype + 1; // adjust if we also have relays
  } else if (!light_controller.isCTRGBLinked()) {
    // if RGBW or RGBCW, and SetOption37 >= 128, we manage RGB and W separately
    Light.device--;   // we take the last two devices as lights
  }
#ifdef DEBUG_LIGHT
  AddLog_P2(LOG_LEVEL_DEBUG_MORE, "LightInit Light.pwm_multi_channels=%d Light.subtype=%d Light.device=%d devices_present=%d",
    Light.pwm_multi_channels, Light.subtype, Light.device, devices_present);
#endif

  light_controller.setSubType(Light.subtype);
  light_controller.loadSettings();

  if (LST_SINGLE == Light.subtype) {
    Settings.light_color[0] = 255;      // One channel only supports Dimmer but needs max color
  }
  if (light_type < LT_PWM6) {           // PWM
    for (uint32_t i = 0; i < light_type; i++) {
      Settings.pwm_value[i] = 0;        // Disable direct PWM control
      if (pin[GPIO_PWM1 +i] < 99) {
        pinMode(pin[GPIO_PWM1 +i], OUTPUT);
      }
    }
    if (pin[GPIO_ARIRFRCV] < 99) {
      if (pin[GPIO_ARIRFSEL] < 99) {
        pinMode(pin[GPIO_ARIRFSEL], OUTPUT);
        digitalWrite(pin[GPIO_ARIRFSEL], 1);  // Turn off RF
      }
    }
  }

  uint32_t max_scheme = Light.max_scheme;
  if (Light.subtype < LST_RGB) {
    max_scheme = LS_POWER;
  }
  if ((LS_WAKEUP == Settings.light_scheme) || (Settings.light_scheme > max_scheme)) {
    Settings.light_scheme = LS_POWER;
  }
  Light.power = 0;
  Light.update = true;
  Light.wakeup_active = 0;

  LightUpdateColorMapping();
}

void LightUpdateColorMapping(void)
{
  uint8_t param = Settings.param[P_RGB_REMAP] & 127;
  if (param > 119){ param = 0; }

  uint8_t tmp[] = {0,1,2,3,4};
  Light.color_remap[0] = tmp[param / 24];
  for (uint32_t i = param / 24; i<4; ++i){
    tmp[i] = tmp[i+1];
  }
  param = param % 24;
  Light.color_remap[1] = tmp[(param / 6)];
  for (uint32_t i = param / 6; i<3; ++i){
    tmp[i] = tmp[i+1];
  }
  param = param % 6;
  Light.color_remap[2] = tmp[(param / 2)];
  for (uint32_t i = param / 2; i<2; ++i){
    tmp[i] = tmp[i+1];
  }
  param = param % 2;
  Light.color_remap[3] = tmp[param];
  Light.color_remap[4] = tmp[1-param];

  Light.update = true;
  //AddLog_P2(LOG_LEVEL_DEBUG, PSTR("%d colors: %d %d %d %d %d") ,Settings.param[P_RGB_REMAP], Light.color_remap[0],Light.color_remap[1],Light.color_remap[2],Light.color_remap[3],Light.color_remap[4]);
}

void LightSetDimmer(uint8_t dimmer) {
  light_controller.changeDimmer(dimmer);
}

uint32_t LightGetHSB(uint16_t *hue,uint8_t  *sat, uint8_t *bri) {
  light_state.getHSB(hue, sat, bri);
}

void LightHsToRgb(uint16_t hue, uint8_t sat, uint8_t *r_r, uint8_t *r_g, uint8_t *r_b) {
  light_state.HsToRgb(hue, sat, r_r, r_g, r_b);
}

// If SetOption68 is set, get the brightness for a specific device
uint8_t LightGetBri(uint8_t device) {
  uint8_t bri = 254;   // default value if relay
  if (Light.pwm_multi_channels) {
    if ((device >= Light.device) && (device < Light.device + LST_MAX) && (device <= devices_present)) {
      bri = Light.current_color[device - Light.device];
    }
  } else if (light_controller.isCTRGBLinked()) {   // standard behavior
    if (device == Light.device) {
      bri = light_state.getBri();
    }
  } else {    // unlinked
    if (device == Light.device) {
      bri = light_state.getBriRGB();
    } else if (device == Light.device + 1) {
      bri = light_state.getBriCT();
    }
  }
  return bri;
}

// If SetOption68 is set, set the brightness for a specific device
void LightSetBri(uint8_t device, uint8_t bri) {
  if (Light.pwm_multi_channels) {
    if ((device >= Light.device) && (device < Light.device + LST_MAX) && (device <= devices_present)) {
      Light.current_color[device - Light.device] = bri;
      light_controller.changeChannels(Light.current_color);
    }
  } else if (light_controller.isCTRGBLinked()) {  // standard
    if (device == Light.device) {
      light_controller.changeBri(bri);
    }
  } else {  // unlinked
    if (device == Light.device) {
      light_controller.changeBriRGB(bri);
    } else if (device == Light.device + 1) {
      light_controller.changeBriCT(bri);
    }
  }
}

void LightSetColorTemp(uint16_t ct)
{
/* Color Temperature (https://developers.meethue.com/documentation/core-concepts)
 *
 * ct = 153 = 2000K = Warm = CCWW = 00FF
 * ct = 500 = 6500K = Cold = CCWW = FF00
 */
  // don't set CT if not supported
  if ((LST_COLDWARM != Light.subtype) && (LST_RGBWC != Light.subtype)) {
    return;
  }
  light_controller.changeCTB(ct, light_state.getBriCT());
}

uint16_t LightGetColorTemp(void)
{
  // don't calculate CT for unsupported devices
  if ((LST_COLDWARM != Light.subtype) && (LST_RGBWC != Light.subtype)) {
    return 0;
  }
  return (light_state.getColorMode() & LCM_CT) ? light_state.getCT() : 0;
}

void LightSetSignal(uint16_t lo, uint16_t hi, uint16_t value)
{
/* lo - below lo is green
   hi - above hi is red
*/
  if (Settings.flag.light_signal) {  // SetOption18 - Pair light signal with CO2 sensor
    uint16_t signal = changeUIntScale(value, lo, hi, 0, 255);  // 0..255
//    AddLog_P2(LOG_LEVEL_DEBUG, PSTR(D_LOG_DEBUG "Light signal %d"), signal);
    light_controller.changeRGB(signal, 255 - signal, 0, true);  // keep bri
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
  for (uint32_t i = 0; i < Light.subtype; i++) {
    if (!force_hex && Settings.flag.decimal_text) {  // SetOption17 - Switch between decimal or hexadecimal output
      snprintf_P(scolor, LIGHT_COLOR_SIZE, PSTR("%s%s%d"), scolor, (i > 0) ? "," : "", Light.current_color[i]);
    } else {
      snprintf_P(scolor, LIGHT_COLOR_SIZE, PSTR("%s%02X"), scolor, Light.current_color[i]);
    }
  }
  return scolor;
}

void LightPowerOn(void)
{
  if (light_state.getBri() && !(Light.power)) {
    ExecuteCommandPower(Light.device, POWER_ON, SRC_LIGHT);
  }
}

void LightState(uint8_t append)
{
  char scolor[LIGHT_COLOR_SIZE];
  char scommand[33];
  bool unlinked = !light_controller.isCTRGBLinked() && (Light.subtype >= LST_RGBW);  // there are 2 power and dimmers for RGB and White

  if (append) {
    ResponseAppend_P(PSTR(","));
  } else {
    Response_P(PSTR("{"));
  }
  if (!Light.pwm_multi_channels) {
    if (unlinked) {
      // RGB and W are unlinked, we display the second Power/Dimmer
      ResponseAppend_P(PSTR("\"" D_RSLT_POWER "%d\":\"%s\",\"" D_CMND_DIMMER "%d\":%d"
                           ",\"" D_RSLT_POWER "%d\":\"%s\",\"" D_CMND_DIMMER "%d\":%d"),
                            Light.device, GetStateText(Light.power & 1), Light.device, light_state.getDimmer(1),
                            Light.device + 1, GetStateText(Light.power & 2 ? 1 : 0), Light.device + 1, light_state.getDimmer(2));
    } else {
      GetPowerDevice(scommand, Light.device, sizeof(scommand), Settings.flag.device_index_enable);  // SetOption26 - Switch between POWER or POWER1
      ResponseAppend_P(PSTR("\"%s\":\"%s\",\"" D_CMND_DIMMER "\":%d"), scommand, GetStateText(Light.power & 1),
                      light_state.getDimmer());
    }


    if (Light.subtype > LST_SINGLE) {
      ResponseAppend_P(PSTR(",\"" D_CMND_COLOR "\":\"%s\""), LightGetColor(scolor));
      if (LST_RGB <= Light.subtype) {
        uint16_t hue;
        uint8_t  sat, bri;
        light_state.getHSB(&hue, &sat, &bri);
        sat = changeUIntScale(sat, 0, 255, 0, 100);
        bri = changeUIntScale(bri, 0, 255, 0, 100);

        ResponseAppend_P(PSTR(",\"" D_CMND_HSBCOLOR "\":\"%d,%d,%d\""), hue,sat,bri);
      }
      // Add White level
      if ((LST_COLDWARM == Light.subtype) || (LST_RGBW <= Light.subtype)) {
        ResponseAppend_P(PSTR(",\"" D_CMND_WHITE "\":%d"), light_state.getDimmer(2));
      }
      // Add CT
      if ((LST_COLDWARM == Light.subtype) || (LST_RGBWC == Light.subtype)) {
        ResponseAppend_P(PSTR(",\"" D_CMND_COLORTEMPERATURE "\":%d"), light_state.getCT());
      }
      // Add status for each channel
      ResponseAppend_P(PSTR(",\"" D_CMND_CHANNEL "\":[" ));
      for (uint32_t i = 0; i < Light.subtype; i++) {
        uint8_t channel_raw = Light.current_color[i];
        uint8_t channel = changeUIntScale(channel_raw,0,255,0,100);
        // if non null, force to be at least 1
        if ((0 == channel) && (channel_raw > 0)) { channel = 1; }
        ResponseAppend_P(PSTR("%s%d" ), (i > 0 ? "," : ""), channel);
      }
      ResponseAppend_P(PSTR("]"));
    }

    if (append) {
      if (Light.subtype >= LST_RGB) {
        ResponseAppend_P(PSTR(",\"" D_CMND_SCHEME "\":%d"), Settings.light_scheme);
      }
      if (Light.max_scheme > LS_MAX) {
        ResponseAppend_P(PSTR(",\"" D_CMND_WIDTH "\":%d"), Settings.light_width);
      }
      ResponseAppend_P(PSTR(",\"" D_CMND_FADE "\":\"%s\",\"" D_CMND_SPEED "\":%d,\"" D_CMND_LEDTABLE "\":\"%s\""),
        GetStateText(Settings.light_fade), Settings.light_speed, GetStateText(Settings.light_correction));
    }
  } else {  // Light.pwm_multi_channels
    for (uint32_t i = 0; i < Light.subtype; i++) {
      GetPowerDevice(scommand, Light.device + i, sizeof(scommand), 1);
      uint32_t light_power_masked = Light.power & (1 << i);    // the Light.power value for this device
      light_power_masked = light_power_masked ? 1 : 0;                    // convert to on/off
      ResponseAppend_P(PSTR("\"%s\":\"%s\",\"" D_CMND_CHANNEL "%d\":%d,"), scommand, GetStateText(light_power_masked), Light.device + i,
        changeUIntScale(Light.current_color[i], 0, 255, 0, 100));
    }
    ResponseAppend_P(PSTR("\"" D_CMND_COLOR "\":\"%s\""), LightGetColor(scolor));
  }   // Light.pwm_multi_channels

  if (!append) {
    ResponseJsonEnd();
  }
}

void LightPreparePower(power_t channels = 0xFFFFFFFF) {    // 1 = only RGB, 2 = only CT, 3 = both RGB and CT
#ifdef DEBUG_LIGHT
  AddLog_P2(LOG_LEVEL_DEBUG, "LightPreparePower power=%d Light.power=%d", power, Light.power);
#endif
  // If multi-channels, then we only switch off channels with a value of zero
  if (Light.pwm_multi_channels) {
    for (uint32_t i = 0; i < Light.subtype; i++) {
      if (bitRead(channels, i)) {
        // if channel is non-null, channel is supposed to be on, but it is off, do Power On
        if ((Light.current_color[i]) && (!bitRead(Light.power, i))) {
          if (!Settings.flag.not_power_linked) {  // SetOption20 - Control power in relation to Dimmer/Color/Ct changes
            ExecuteCommandPower(Light.device + i, POWER_ON_NO_STATE, SRC_LIGHT);
          }
        } else {
          // if channel is zero and channel is on, set it off
          if ((0 == Light.current_color[i]) && bitRead(Light.power, i)) {
            ExecuteCommandPower(Light.device + i, POWER_OFF_NO_STATE, SRC_LIGHT);
          }
        }
  #ifdef USE_DOMOTICZ
        DomoticzUpdatePowerState(Light.device + i);
  #endif  // USE_DOMOTICZ
      }
    }
  } else {
    if (light_controller.isCTRGBLinked()) {   // linked, standard
      if (light_state.getBri() && !(Light.power)) {
        if (!Settings.flag.not_power_linked) {  // SetOption20 - Control power in relation to Dimmer/Color/Ct changes
          ExecuteCommandPower(Light.device, POWER_ON_NO_STATE, SRC_LIGHT);
        }
      } else if (!light_state.getBri() && Light.power) {
        ExecuteCommandPower(Light.device, POWER_OFF_NO_STATE, SRC_LIGHT);
      }
    } else {
      // RGB
      if (channels & 1) {
        if (light_state.getBriRGB() && !(Light.power & 1)) {
          if (!Settings.flag.not_power_linked) {  // SetOption20 - Control power in relation to Dimmer/Color/Ct changes
            ExecuteCommandPower(Light.device, POWER_ON_NO_STATE, SRC_LIGHT);
          }
        } else if (!light_state.getBriRGB() && (Light.power & 1)) {
          ExecuteCommandPower(Light.device, POWER_OFF_NO_STATE, SRC_LIGHT);
        }
      }
      // White CT
      if (channels & 2) {
        if (light_state.getBriCT() && !(Light.power & 2)) {
          if (!Settings.flag.not_power_linked) {  // SetOption20 - Control power in relation to Dimmer/Color/Ct changes
            ExecuteCommandPower(Light.device + 1, POWER_ON_NO_STATE, SRC_LIGHT);
          }
        } else if (!light_state.getBriCT() && (Light.power & 2)) {
          ExecuteCommandPower(Light.device + 1, POWER_OFF_NO_STATE, SRC_LIGHT);
        }
      }
    }
#ifdef USE_DOMOTICZ
    DomoticzUpdatePowerState(Light.device);
#endif  // USE_DOMOTICZ
  }

  if (Settings.flag3.hass_tele_on_power) {  // SetOption59 - Send tele/%topic%/STATE in addition to stat/%topic%/RESULT
    MqttPublishTeleState();
  }

#ifdef DEBUG_LIGHT
  AddLog_P2(LOG_LEVEL_DEBUG, "LightPreparePower End power=%d Light.power=%d", power, Light.power);
#endif
  Light.power = power >> (Light.device - 1);  // reset next state, works also with unlinked RGB/CT
  LightState(0);
}

void LightCycleColor(int8_t direction)
{
  if (Light.strip_timer_counter % (Settings.light_speed * 2)) {
    return;
  }

  if (0 == direction) {
    if (Light.random == Light.wheel) {
      Light.random = random(255);
    }
    direction = (Light.random < Light.wheel) ? -1 : 1;
  }
  Light.wheel += direction;
  uint16_t hue = changeUIntScale(Light.wheel, 0, 255, 0, 359);  // Scale to hue to keep amount of steps low (max 255 instead of 359)

//  AddLog_P2(LOG_LEVEL_DEBUG, PSTR("DBG: random %d, wheel %d, hue %d"), Light.random, Light.wheel, hue);

  uint8_t sat;
  light_state.getHSB(nullptr, &sat, nullptr);  // Allow user control over Saturation
  light_state.setHS(hue, sat);
  light_controller.calcLevels(Light.new_color);
}

void LightSetPower(void)
{
//  Light.power = XdrvMailbox.index;
  Light.old_power = Light.power;
  //Light.power = bitRead(XdrvMailbox.index, Light.device -1);
  uint32_t mask = 1;  // default mask
  if (Light.pwm_multi_channels) {
    mask = (1 << Light.subtype) - 1;   // wider mask
  } else if (!light_controller.isCTRGBLinked()) {
    mask = 3;   // we got 2 devices, for RGB and White
  }
  uint32_t shift = Light.device - 1;
  // If PWM multi_channels
  // Ex: 3 Relays and 4 PWM - devices_present = 7, Light.device = 4, Light.subtype = 4
  // Result: mask = 0b00001111 = 0x0F, shift = 3.
  // Power bits we consider are: 0b01111000 = 0x78
  // If regular situation: devices_present == Light.subtype
  Light.power = (XdrvMailbox.index & (mask << shift)) >> shift;
  if (Light.wakeup_active) {
    Light.wakeup_active--;
  }
#ifdef DEBUG_LIGHT
  AddLog_P2(LOG_LEVEL_DEBUG_MORE, "LightSetPower XdrvMailbox.index=%d Light.old_power=%d Light.power=%d mask=%d shift=%d",
    XdrvMailbox.index, Light.old_power, Light.power, mask, shift);
#endif
  if (Light.power != Light.old_power) {
    Light.update = true;
  }
  LightAnimate();
}

// On entry Light.new_color[5] contains the color to be displayed
// and Light.last_color[5] the color currently displayed
// Light.power tells which lights or channels (SetOption68) are on/off
void LightAnimate(void)
{
  uint16_t light_still_on = 0;
  bool power_off = false;

  Light.strip_timer_counter++;

  // set sleep parameter: either settings,
  // or set a maximum of PWM_MAX_SLEEP if light is on or Fade is running
  if (Light.power || Light.fade_running) {
    if (Settings.sleep > PWM_MAX_SLEEP) {
      sleep = PWM_MAX_SLEEP;      // set a maxumum value of 50 milliseconds to ensure that animations are smooth
    } else {
      sleep = Settings.sleep;     // or keep the current sleep if it's lower than 50
    }
  } else {
    sleep = Settings.sleep;
  }

  if (!Light.power) {                   // All channels powered off
    Light.strip_timer_counter = 0;
    if (Settings.light_scheme >= LS_MAX) {
      power_off = true;
    }
  } else {
    switch (Settings.light_scheme) {
      case LS_POWER:
        light_controller.calcLevels(Light.new_color);
        break;
      case LS_WAKEUP:
        if (2 == Light.wakeup_active) {
          Light.wakeup_active = 1;
          for (uint32_t i = 0; i < Light.subtype; i++) {
            Light.new_color[i] = 0;
          }
          Light.wakeup_counter = 0;
          Light.wakeup_dimmer = 0;
        }
        Light.wakeup_counter++;
        if (Light.wakeup_counter > ((Settings.light_wakeup * STATES) / Settings.light_dimmer)) {
          Light.wakeup_counter = 0;
          Light.wakeup_dimmer++;
          if (Light.wakeup_dimmer <= Settings.light_dimmer) {
            light_state.setDimmer(Light.wakeup_dimmer);
            light_controller.calcLevels();
            for (uint32_t i = 0; i < Light.subtype; i++) {
              Light.new_color[i] = Light.current_color[i];
            }
          } else {
            Response_P(PSTR("{\"" D_CMND_WAKEUP "\":\"" D_JSON_DONE "\"}"));
            MqttPublishPrefixTopic_P(TELE, PSTR(D_CMND_WAKEUP));
            Light.wakeup_active = 0;
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
        LightCycleColor(0);
        break;
      default:
        XlgtCall(FUNC_SET_SCHEME);
    }
  }

  if ((Settings.light_scheme < LS_MAX) || power_off) {  // exclude WS281X Neopixel schemes

    // Apply power modifiers to Light.new_color
    LightApplyPower(Light.new_color, Light.power);

    // AddLog_P2(LOG_LEVEL_INFO, PSTR("last_color (%02X%02X%02X%02X%02X) new_color (%02X%02X%02X%02X%02X) power %d"),
    // Light.last_color[0], Light.last_color[1], Light.last_color[2], Light.last_color[3], Light.last_color[4],
    // Light.new_color[0], Light.new_color[1], Light.new_color[2], Light.new_color[3], Light.new_color[4],
    // Light.power
    // );

    if (memcmp(Light.last_color, Light.new_color, Light.subtype)) {
      Light.update = true;
    }
    if (Light.update) {
      uint16_t cur_col_10[LST_MAX];   // 10 bits resolution
      Light.update = false;

      // first set 8 and 10 bits channels
      for (uint32_t i = 0; i < LST_MAX; i++) {
        Light.last_color[i] = Light.new_color[i];
        // Extend from 8 to 10 bits if no correction (in case no gamma correction is required)
        cur_col_10[i] = change8to10(Light.new_color[i]);
      }

      if (Light.pwm_multi_channels) {
        calcGammaMultiChannels(cur_col_10);
      } else {
        calcGammaBulbs(cur_col_10);
        if (PHILIPS == my_module_type) {
          calcGammaCTPwm(cur_col_10);
        }

        // Now see if we need to mix RGB and True White
        // Valid only for LST_RGBW, LST_RGBWC, rgbwwTable[4] is zero, and white is zero (see doc)
        if ((LST_RGBW <= Light.subtype) && (0 == Settings.rgbwwTable[4]) && (0 == cur_col_10[3]+cur_col_10[4])) {
          uint32_t min_rgb_10 = min3(cur_col_10[0], cur_col_10[1], cur_col_10[2]);
          for (uint32_t i=0; i<3; i++) {
            // substract white and adjust according to rgbwwTable
            uint32_t adjust10 = change8to10(Settings.rgbwwTable[i]);
            cur_col_10[i] = changeUIntScale(cur_col_10[i] - min_rgb_10, 0, 1023, 0, adjust10);
          }

          // compute the adjusted white levels for 10 and 8 bits
          uint32_t adjust_w_10 = changeUIntScale(Settings.rgbwwTable[3], 0, 255, 0, 1023);
          uint32_t white_10 = changeUIntScale(min_rgb_10, 0, 1023, 0, adjust_w_10);  // set white power down corrected with rgbwwTable[3]
          if (LST_RGBW == Light.subtype) {
            // we simply set the white channel
            cur_col_10[3] = white_10;
          } else {  // LST_RGBWC
            // we distribute white between cold and warm according to CT value
            uint32_t ct = light_state.getCT();
            cur_col_10[4] = changeUIntScale(ct, 153, 500, 0, white_10);
            cur_col_10[3] = white_10 - cur_col_10[4];
          }
        }
      }

      // final adjusments for PMW, post-gamma correction
      for (uint32_t i = 0; i < LST_MAX; i++) {
        // scale from 0..1023 to 0..pwm_range, but keep any non-zero value to at least 1
        cur_col_10[i] = (cur_col_10[i] > 0) ? changeUIntScale(cur_col_10[i], 1, 1023, 1, Settings.pwm_range) : 0;
      }

      // apply port remapping on both 8 bits and 10 bits versions
      uint16_t orig_col_10bits[LST_MAX];
      memcpy(orig_col_10bits, cur_col_10, sizeof(orig_col_10bits));
      for (uint32_t i = 0; i < LST_MAX; i++) {
        cur_col_10[i] = orig_col_10bits[Light.color_remap[i]];
      }

      if (!Settings.light_fade || power_off) { // no fade
        // record the current value for a future Fade
        memcpy(Light.fade_start_10, cur_col_10, sizeof(Light.fade_start_10));
        // push the final values at 8 and 10 bits resolution to the PWMs
        LightSetOutputs(cur_col_10);
      } else {  // fade on
        if (Light.fade_running) {
          // if fade is running, we take the curring value as the start for the next fade
          memcpy(Light.fade_start_10, Light.fade_cur_10, sizeof(Light.fade_start_10));
        }
        memcpy(Light.fade_end_10, cur_col_10, sizeof(Light.fade_start_10));
        Light.fade_running = true;
        Light.fade_duration = 0;    // set the value to zero to force a recompute
        Light.fade_start = 0;
        // Fade will applied immediately below
      }
    }
    if (Light.fade_running) {
      if (LightApplyFade()) {
        // AddLog_P2(LOG_LEVEL_INFO, PSTR("LightApplyFade %d %d %d %d %d"),
        //   Light.fade_cur_10[0], Light.fade_cur_10[1], Light.fade_cur_10[2], Light.fade_cur_10[3], Light.fade_cur_10[4]);

        LightSetOutputs(Light.fade_cur_10);
      }
    }
  }
}

bool LightApplyFade(void) {   // did the value chanegd and needs to be applied
  static uint32_t last_millis = 0;
  uint32_t now = millis();

  if ((now - last_millis) <= 5) {
    return false;     // the value was not changed in the last 5 milliseconds, ignore
  }
  last_millis = now;

  // Check if we need to calculate the duration
  if (0 == Light.fade_duration) {
    Light.fade_start = now;
    // compute the distance between start and and color (max of distance for each channel)
    uint32_t distance = 0;
    for (uint32_t i = 0; i < Light.subtype; i++) {
      int32_t channel_distance = Light.fade_end_10[i] - Light.fade_start_10[i];
      if (channel_distance < 0) { channel_distance = - channel_distance; }
      if (channel_distance > distance) { distance = channel_distance; }
    }
    if (distance > 0) {
      // compute the duration of the animation
      // Note: Settings.light_speed is the number of half-seconds for a 100% fade,
      // i.e. light_speed=1 means 1024 steps in 500ms
      Light.fade_duration = (distance * Settings.light_speed * 500) / 1023;
      if (Settings.save_data) {
        // Also postpone the save_data for the duration of the Fade (in seconds)
        uint32_t delay_seconds = 1 + (Light.fade_duration + 999) / 1000;   // add one more second
        // AddLog_P2(LOG_LEVEL_INFO, PSTR("delay_seconds %d, save_data_counter %d"), delay_seconds, save_data_counter);
        if (save_data_counter < delay_seconds) {
          save_data_counter = delay_seconds;      // pospone
        }
      }
    } else {
      // no fade needed, we keep the duration at zero, it will fallback directly to end of fade
      Light.fade_running = false;
    }
  }

  uint16_t fade_current = now - Light.fade_start;   // number of milliseconds since start of fade
  if (fade_current <= Light.fade_duration) {    // fade not finished
    //Serial.printf("Fade: %d / %d - ", fade_current, Light.fade_duration);
    for (uint32_t i = 0; i < Light.subtype; i++) {
      Light.fade_cur_10[i] = changeUIntScale(fade_current,
                                              0, Light.fade_duration,
                                              Light.fade_start_10[i], Light.fade_end_10[i]);
    }
  } else {
    // stop fade
//AddLop_P2(LOG_LEVEL_DEBUG, PSTR("Stop fade"));
    Light.fade_running = false;
    Light.fade_start = 0;
    Light.fade_duration = 0;
    // set light to target value
    memcpy(Light.fade_cur_10, Light.fade_end_10, sizeof(Light.fade_end_10));
    // record the last value for next start
    memcpy(Light.fade_start_10, Light.fade_end_10, sizeof(Light.fade_start_10));
  }
  return true;
}

// On entry we take the 5 channels 8 bits entry, and we apply Power modifiers
// I.e. shut down channels that are powered down
void LightApplyPower(uint8_t new_color[LST_MAX], power_t power) {
  // If SetOption68, multi_channels
  if (Light.pwm_multi_channels) {
    // if multi-channels, specifically apply the Light.power bits
    for (uint32_t i = 0; i < LST_MAX; i++) {
      if (0 == bitRead(power,i)) {  // if power down bit is zero
        new_color[i] = 0;   // shut down this channel
      }
    }
    // #ifdef DEBUG_LIGHT
    // AddLog_P2(LOG_LEVEL_DEBUG_MORE, "Animate>> Light.power=%d Light.new_color=[%d,%d,%d,%d,%d]",
    //   Light.power, Light.new_color[0], Light.new_color[1], Light.new_color[2],
    //   Light.new_color[3], Light.new_color[4]);
    // #endif
  } else {
    if (!light_controller.isCTRGBLinked()) {
      // we have 2 power bits for RGB and White
      if (0 == (power & 1)) {
        new_color[0] = new_color[1] = new_color[2] = 0;
      }
      if (0 == (power & 2)) {
        new_color[3] = new_color[4] = 0;
      }
    } else if (!power) {
      for (uint32_t i = 0; i < LST_MAX; i++) {
        new_color[i] = 0;
      }
    }
  }
}

void LightSetOutputs(const uint16_t *cur_col_10) {
  // now apply the actual PWM values, adjusted and remapped 10-bits range
  if (light_type < LT_PWM6) {   // only for direct PWM lights, not for Tuya, Armtronix...
    for (uint32_t i = 0; i < (Light.subtype - Light.pwm_offset); i++) {
      if (pin[GPIO_PWM1 +i] < 99) {
        //AddLog_P2(LOG_LEVEL_DEBUG, PSTR(D_LOG_APPLICATION "Cur_Col%d 10 bits %d"), i, cur_col_10[i]);
        analogWrite(pin[GPIO_PWM1 +i], bitRead(pwm_inverted, i) ? Settings.pwm_range - cur_col_10[(i + Light.pwm_offset)] : cur_col_10[(i + Light.pwm_offset)]);
      }
    }
  }
  char msg[24];
  AddLog_P2(LOG_LEVEL_DEBUG, PSTR("LGT: Channels %s"),
            ToHex_P((const unsigned char *)cur_col_10, 10, msg, sizeof(msg)));
  
  uint8_t cur_col[LST_MAX];
  for (uint32_t i = 0; i < LST_MAX; i++) {
    cur_col[i] = change10to8(cur_col_10[i]);
  }
  // Some devices need scaled RGB like Sonoff L1
  // TODO, should be probably moved to the Sonoff L1 support code
  uint8_t scale_col[3];
  uint32_t max = (cur_col[0] > cur_col[1] && cur_col[0] > cur_col[2]) ? cur_col[0] : (cur_col[1] > cur_col[2]) ? cur_col[1] : cur_col[2];   // 0..255
  for (uint32_t i = 0; i < 3; i++) {
    scale_col[i] = (0 == max) ? 255 : (255 > max) ? changeUIntScale(cur_col[i], 0, max, 0, 255) : cur_col[i];
  }

  char *tmp_data = XdrvMailbox.data;
  char *tmp_topic = XdrvMailbox.topic;
  XdrvMailbox.data = (char*)cur_col;
  XdrvMailbox.topic = (char*)scale_col;
  if (XlgtCall(FUNC_SET_CHANNELS)) { /* Serviced */ }
  else if (XdrvCall(FUNC_SET_CHANNELS)) { /* Serviced */ }
  XdrvMailbox.data = tmp_data;
  XdrvMailbox.topic = tmp_topic;
}

// Do specific computation is SetOption73 is on, Color Temp is a separate PWM channel
void calcGammaCTPwm(uint16_t cur_col_10[5]) {
  // Xiaomi Philips bulbs follow a different scheme:
  uint8_t cold, warm;   // channel 1 is the color tone, mapped to cold channel (0..255)
  light_state.getCW(&cold, &warm);
  // channels for white are always the last two channels
  uint32_t cw1 = Light.subtype - 1;       // address for the ColorTone PWM
  uint32_t cw0 = Light.subtype - 2;       // address for the White Brightness PWM
  // overall brightness
  uint16_t pxBri10 = cur_col_10[cw0] + cur_col_10[cw1];
  if (pxBri10 > 1023) { pxBri10 = 1023; }
  cur_col_10[cw1] = changeUIntScale(cold, 0, cold + warm, 0, 1023);   //
  // channel 0=intensity, channel1=temperature
  if (Settings.light_correction) { // gamma correction
    cur_col_10[cw0] = ledGamma10_10(pxBri10);    // 10 bits gamma correction
  } else {
    cur_col_10[cw0] = pxBri10;  // no gamma, extend to 10 bits
  }
}

// Just apply basic Gamma to each channel
void calcGammaMultiChannels(uint16_t cur_col_10[5]) {
  // Apply gamma correction for 8 and 10 bits resolutions, if needed
  if (Settings.light_correction) {
    for (uint32_t i = 0; i < LST_MAX; i++) {
      cur_col_10[i] = ledGamma10_10(cur_col_10[i]);
    }
  }
}

void calcGammaBulbs(uint16_t cur_col_10[5]) {
  // Apply gamma correction for 8 and 10 bits resolutions, if needed
  if (Settings.light_correction) {
    // First apply combined correction to the overall white power
    if ((LST_COLDWARM == Light.subtype) || (LST_RGBWC == Light.subtype)) {
      uint8_t w_idx[2] = {0, 1};        // if LST_COLDWARM, channels 0 and 1
      if (LST_RGBWC == Light.subtype) { // if LST_RGBWC, channels 3 and 4
        w_idx[0] = 3;
        w_idx[1] = 4;
      }
      uint16_t white_bri10 = cur_col_10[w_idx[0]] + cur_col_10[w_idx[1]];
      // if sum of both channels is > 255, then channels are probablu uncorrelated
      if (white_bri10 <= 1023) {
        // we calculate the gamma corrected sum of CW + WW
        uint16_t white_bri_10bits = ledGamma10_10(white_bri10);
        // then we split the total energy among the cold and warm leds
        cur_col_10[w_idx[0]] = changeUIntScale(cur_col_10[w_idx[0]], 0, white_bri10, 0, white_bri_10bits);
        cur_col_10[w_idx[1]] = changeUIntScale(cur_col_10[w_idx[1]], 0, white_bri10, 0, white_bri_10bits);
      } else {
        cur_col_10[w_idx[0]] = ledGamma10_10(cur_col_10[w_idx[0]]);
        cur_col_10[w_idx[1]] = ledGamma10_10(cur_col_10[w_idx[1]]);
      }
    }
    // then apply gamma correction to RGB channels
    if (LST_RGB <= Light.subtype) {
      for (uint32_t i = 0; i < 3; i++) {
        cur_col_10[i] = ledGamma10_10(cur_col_10[i]);
      }
    }
    // If RGBW or Single channel, also adjust White channel
    if ((LST_COLDWARM != Light.subtype) && (LST_RGBWC != Light.subtype)) {
      cur_col_10[3] = ledGamma10_10(cur_col_10[3]);
    }
  }
}

/*********************************************************************************************\
 * Commands
\*********************************************************************************************/

bool LightColorEntry(char *buffer, uint32_t buffer_length)
{
  char scolor[10];
  char *p;
  char *str;
  uint32_t entry_type = 0;                          // Invalid
  uint8_t value = Light.fixed_color_index;

  if (buffer[0] == '#') {                           // Optional hexadecimal entry
    buffer++;
    buffer_length--;
  }

  if (Light.subtype >= LST_RGB) {
    char option = (1 == buffer_length) ? buffer[0] : '\0';
    if (('+' == option) && (Light.fixed_color_index < MAX_FIXED_COLOR)) {
      value++;
    }
    else if (('-' == option) && (Light.fixed_color_index > 1)) {
      value--;
    } else {
      value = atoi(buffer);
    }
  }

  memset(&Light.entry_color, 0x00, sizeof(Light.entry_color));
  // erase all channels except if the last character is '=', #6799
  while ((buffer_length > 0) && ('=' == buffer[buffer_length - 1])) {
    buffer_length--;  // remove all trailing '='
    memcpy(&Light.entry_color, &Light.current_color, sizeof(Light.entry_color));
  }
  if (strstr(buffer, ",") != nullptr) {             // Decimal entry
    int8_t i = 0;
    for (str = strtok_r(buffer, ",", &p); str && i < 6; str = strtok_r(nullptr, ",", &p)) {
      if (i < LST_MAX) {
        Light.entry_color[i++] = atoi(str);
      }
    }
    entry_type = 2;                                 // Decimal
  }
  else if (((2 * Light.subtype) == buffer_length) || (buffer_length > 3)) {  // Hexadecimal entry
    for (uint32_t i = 0; i < tmin((uint)(buffer_length / 2), sizeof(Light.entry_color)); i++) {
      strlcpy(scolor, buffer + (i *2), 3);
      Light.entry_color[i] = (uint8_t)strtol(scolor, &p, 16);
    }
    entry_type = 1;                                 // Hexadecimal
  }
  else if ((Light.subtype >= LST_RGB) && (value > 0) && (value <= MAX_FIXED_COLOR)) {
    Light.fixed_color_index = value;
    memcpy_P(&Light.entry_color, &kFixedColor[value -1], 3);
    entry_type = 1;                                 // Hexadecimal
  }
  else if ((value > 199) && (value <= 199 + MAX_FIXED_COLD_WARM)) {
    if (LST_RGBW == Light.subtype) {
      memcpy_P(&Light.entry_color[3], &kFixedWhite[value -200], 1);
      entry_type = 1;                               // Hexadecimal
    }
    else if (LST_COLDWARM == Light.subtype) {
      memcpy_P(&Light.entry_color, &kFixedColdWarm[value -200], 2);
      entry_type = 1;                               // Hexadecimal
    }
    else if (LST_RGBWC == Light.subtype) {
      memcpy_P(&Light.entry_color[3], &kFixedColdWarm[value -200], 2);
      entry_type = 1;                               // Hexadecimal
    }
  }
  if (entry_type) {
    Settings.flag.decimal_text = entry_type -1;     // SetOption17 - Switch between decimal or hexadecimal output
  }
  return (entry_type);
}

/********************************************************************************************/

void CmndSupportColor(void)
{
  bool valid_entry = false;
  bool coldim = false;

  if (XdrvMailbox.data_len > 0) {
    valid_entry = LightColorEntry(XdrvMailbox.data, XdrvMailbox.data_len);
    if (valid_entry) {
      if (XdrvMailbox.index <= 2) {    // Color(1), 2
        uint32_t old_bri = light_state.getBri();
        // change all channels to specified values
        light_controller.changeChannels(Light.entry_color);
        if (2 == XdrvMailbox.index) {
          // If Color2, set back old brightness
          light_controller.changeBri(old_bri);
        }

        Settings.light_scheme = 0;
        coldim = true;
      } else {             // Color3, 4, 5 and 6
        for (uint32_t i = 0; i < LST_RGB; i++) {
          Settings.ws_color[XdrvMailbox.index -3][i] = Light.entry_color[i];
        }
      }
    }
  }
  char scolor[LIGHT_COLOR_SIZE];
  if (!valid_entry && (XdrvMailbox.index <= 2)) {
    ResponseCmndChar(LightGetColor(scolor));
  }
  if (XdrvMailbox.index >= 3) {
    scolor[0] = '\0';
    for (uint32_t i = 0; i < LST_RGB; i++) {
      if (Settings.flag.decimal_text) {  // SetOption17 - Switch between decimal or hexadecimal output
        snprintf_P(scolor, sizeof(scolor), PSTR("%s%s%d"), scolor, (i > 0) ? "," : "", Settings.ws_color[XdrvMailbox.index -3][i]);
      } else {
        snprintf_P(scolor, sizeof(scolor), PSTR("%s%02X"), scolor, Settings.ws_color[XdrvMailbox.index -3][i]);
      }
    }
    ResponseCmndIdxChar(scolor);
  }
  if (coldim) {
    LightPreparePower();    // no parameter, recalculate Power for all channels
  }
}

void CmndColor(void)
{
  if ((Light.subtype > LST_SINGLE) && (XdrvMailbox.index > 0) && (XdrvMailbox.index <= 6)) {
    CmndSupportColor();
  }
}

void CmndWhite(void)
{
  if (Light.pwm_multi_channels) { return; }
  if ( ((Light.subtype >= LST_RGBW) || (LST_COLDWARM == Light.subtype)) && (XdrvMailbox.index == 1)) {
    if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 100)) {
      light_controller.changeDimmer(XdrvMailbox.payload, 2);
      LightPreparePower(2);
    } else {
      ResponseCmndNumber(light_state.getDimmer(2));
    }
  }
}

void CmndChannel(void)
{
  if ((XdrvMailbox.index >= Light.device) && (XdrvMailbox.index < Light.device + Light.subtype )) {
    uint32_t light_index = XdrvMailbox.index - Light.device;
    power_t coldim = 0;   // bit flag to update

    // Handle +/- special command
    if (1 == XdrvMailbox.data_len) {
      uint8_t channel = changeUIntScale(Light.current_color[light_index],0,255,0,100);
      if ('+' == XdrvMailbox.data[0]) {
        XdrvMailbox.payload = (channel > 89) ? 100 : channel + 10;
      } else if ('-' == XdrvMailbox.data[0]) {
        XdrvMailbox.payload = (channel < 11) ? 1 : channel - 10;
      }
    }

    //  Set "Channel" directly - this allows Color and Direct PWM control to coexist
    if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 100)) {
      Light.current_color[light_index] = changeUIntScale(XdrvMailbox.payload,0,100,0,255);
      if (Light.pwm_multi_channels) {
        coldim = 1 << light_index;      // change the specified channel
      } else {
        if (light_controller.isCTRGBLinked()) {
          // if we change channels 1,2,3 then turn off CT mode (unless non-linked)
          if ((light_index < 3) && (light_controller.isCTRGBLinked())) {
            Light.current_color[3] = Light.current_color[4] = 0;
          } else {
            Light.current_color[0] = Light.current_color[1] = Light.current_color[2] = 0;
          }
          coldim = 1;
        } else {
          if (light_index < 3) { coldim = 1; }    // RGB
          else { coldim = 2; }                    // CT
        }
      }
      light_controller.changeChannels(Light.current_color);
    }
    ResponseCmndIdxNumber(changeUIntScale(Light.current_color[light_index],0,255,0,100));
    if (coldim) {
      LightPreparePower(coldim);
    }
  }
}

void CmndHsbColor(void)
{
  if (Light.subtype >= LST_RGB) {
    bool validHSB = (XdrvMailbox.data_len > 0);
    if (validHSB) {
      uint16_t HSB[3];
      if (strstr(XdrvMailbox.data, ",") != nullptr) {  // Command with 3 comma separated parameters, Hue (0<H<360), Saturation (0<S<100) AND Brightness (0<B<100)
        for (uint32_t i = 0; i < 3; i++) {
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
        uint8_t  c_sat;
        light_state.getHSB(&c_hue, &c_sat, nullptr);
        HSB[0] = c_hue;
        HSB[1] = c_sat;
        HSB[2] = light_state.getBri();

        if (1 == XdrvMailbox.index) {
          HSB[0] = XdrvMailbox.payload;
        } else if ((XdrvMailbox.index > 1) && (XdrvMailbox.index < 4)) {
          HSB[XdrvMailbox.index-1] = changeUIntScale(XdrvMailbox.payload,0,100,0,255);
        } else {
          validHSB = false;
        }
      }
      if (validHSB) {
        light_controller.changeHSB(HSB[0], HSB[1], HSB[2]);
        LightPreparePower(1);
      }
    } else {
      LightState(0);
    }
  }
}

void CmndScheme(void)
{
  if (Light.subtype >= LST_RGB) {
    uint32_t max_scheme = Light.max_scheme;

    if (1 == XdrvMailbox.data_len) {
      if (('+' == XdrvMailbox.data[0]) && (Settings.light_scheme < max_scheme)) {
        XdrvMailbox.payload = Settings.light_scheme + ((0 == Settings.light_scheme) ? 2 : 1);  // Skip wakeup
      }
      else if (('-' == XdrvMailbox.data[0]) && (Settings.light_scheme > 0)) {
        XdrvMailbox.payload = Settings.light_scheme - ((2 == Settings.light_scheme) ? 2 : 1);  // Skip wakeup
      }
    }
    if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= max_scheme)) {
      Settings.light_scheme = XdrvMailbox.payload;
      if (LS_WAKEUP == Settings.light_scheme) {
        Light.wakeup_active = 3;
      }
      LightPowerOn();
      Light.strip_timer_counter = 0;
      // Publish state message for Hass
      if (Settings.flag3.hass_tele_on_power) {  // SetOption59 - Send tele/%topic%/STATE in addition to stat/%topic%/RESULT
        MqttPublishTeleState();
      }
    }
    ResponseCmndNumber(Settings.light_scheme);
  }
}

void CmndWakeup(void)
{
  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 100)) {
    Settings.light_dimmer = XdrvMailbox.payload;
  }
  Light.wakeup_active = 3;
  Settings.light_scheme = LS_WAKEUP;
  LightPowerOn();
  ResponseCmndChar(D_JSON_STARTED);
}

void CmndColorTemperature(void)
{
  if (Light.pwm_multi_channels) { return; }
  if ((LST_COLDWARM == Light.subtype) || (LST_RGBWC == Light.subtype)) { // ColorTemp
    uint32_t ct = light_state.getCT();
    if (1 == XdrvMailbox.data_len) {
      if ('+' == XdrvMailbox.data[0]) {
        XdrvMailbox.payload = (ct > (500-34)) ? 500 : ct + 34;
      }
      else if ('-' == XdrvMailbox.data[0]) {
        XdrvMailbox.payload = (ct < (153+34)) ? 153 : ct - 34;
      }
    }
    if ((XdrvMailbox.payload >= 153) && (XdrvMailbox.payload <= 500)) {  // https://developers.meethue.com/documentation/core-concepts
      light_controller.changeCTB(XdrvMailbox.payload, light_state.getBri());
      LightPreparePower(2);
    } else {
      ResponseCmndNumber(ct);
    }
  }
}

void CmndDimmer(void)
{
  uint32_t dimmer;
  if (XdrvMailbox.index > 2) { XdrvMailbox.index = 1; }

  if ((light_controller.isCTRGBLinked()) || (0 == XdrvMailbox.index)) {
    dimmer = light_state.getDimmer();
  } else {
    dimmer = light_state.getDimmer(XdrvMailbox.index);
  }
  // Handle +/- special command
  if (1 == XdrvMailbox.data_len) {
    if ('+' == XdrvMailbox.data[0]) {
      XdrvMailbox.payload = (dimmer > 89) ? 100 : dimmer + 10;
    } else if ('-' == XdrvMailbox.data[0]) {
      XdrvMailbox.payload = (dimmer < 11) ? 1 : dimmer - 10;
    }
  }
  // If value is ok, change it, otherwise report old value
  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 100)) {
    if (light_controller.isCTRGBLinked()) {
      // normal state, linked RGB and CW
      light_controller.changeDimmer(XdrvMailbox.payload);
      LightPreparePower();
    } else {
      if (0 != XdrvMailbox.index) {
        light_controller.changeDimmer(XdrvMailbox.payload, XdrvMailbox.index);
        LightPreparePower(1 << (XdrvMailbox.index - 1));    // recalculate only the target dimmer
      } else {
        // change both dimmers
        light_controller.changeDimmer(XdrvMailbox.payload, 1);
        light_controller.changeDimmer(XdrvMailbox.payload, 2);
        LightPreparePower();
      }
    }
    Light.update = true;
  } else {
    ResponseCmndNumber(dimmer);
  }
}

void CmndDimmerRange(void)
{
  if (XdrvMailbox.data_len > 0) {
    char *p;
    uint8_t i = 0;
    uint16_t parm[2];
    parm[0] = Settings.dimmer_hw_min;
    parm[1] = Settings.dimmer_hw_max;
    for (char *str = strtok_r(XdrvMailbox.data, ", ", &p); str && i < 2; str = strtok_r(nullptr, ", ", &p)) {
      parm[i] = strtoul(str, nullptr, 0);
      i++;
    }

    if (parm[0] < parm[1]) {
      Settings.dimmer_hw_min = parm[0];
      Settings.dimmer_hw_max = parm[1];
    } else {
      Settings.dimmer_hw_min = parm[1];
      Settings.dimmer_hw_max = parm[0];
    }
    restart_flag = 2;
  }
  Response_P(PSTR("{\"" D_CMND_DIMMER_RANGE "\":{\"Min\":%d,\"Max\":%d}}"), Settings.dimmer_hw_min, Settings.dimmer_hw_max);
}

void CmndLedTable(void)
{
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
    Light.update = true;
  }
  ResponseCmndStateText(Settings.light_correction);
}

void CmndRgbwwTable(void)
{
  if ((XdrvMailbox.data_len > 0)) {
    if (strstr(XdrvMailbox.data, ",") != nullptr) {  // Command with up to 5 comma separated parameters
      for (uint32_t i = 0; i < LST_RGBWC; i++) {
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
    Light.update = true;
  }
  char scolor[LIGHT_COLOR_SIZE];
  scolor[0] = '\0';
  for (uint32_t i = 0; i < LST_RGBWC; i++) {
    snprintf_P(scolor, sizeof(scolor), PSTR("%s%s%d"), scolor, (i > 0) ? "," : "", Settings.rgbwwTable[i]);
  }
  ResponseCmndIdxChar(scolor);
}

void CmndFade(void)
{
  switch (XdrvMailbox.payload) {
  case 0: // Off
  case 1: // On
    Settings.light_fade = XdrvMailbox.payload;
    break;
  case 2: // Toggle
    Settings.light_fade ^= 1;
    break;
  }
  if (!Settings.light_fade) { Light.fade_running = false; }
  ResponseCmndStateText(Settings.light_fade);
}

void CmndSpeed(void)
{  // 1 - fast, 40 - very slow
  if (1 == XdrvMailbox.data_len) {
    if (('+' == XdrvMailbox.data[0]) && (Settings.light_speed > 1)) {
      XdrvMailbox.payload = Settings.light_speed - 1;
    }
    else if (('-' == XdrvMailbox.data[0]) && (Settings.light_speed < 40)) {
      XdrvMailbox.payload = Settings.light_speed + 1;
    }
  }
  if ((XdrvMailbox.payload > 0) && (XdrvMailbox.payload <= 40)) {
    Settings.light_speed = XdrvMailbox.payload;
  }
  ResponseCmndNumber(Settings.light_speed);
}

void CmndWakeupDuration(void)
{
  if ((XdrvMailbox.payload > 0) && (XdrvMailbox.payload < 3001)) {
    Settings.light_wakeup = XdrvMailbox.payload;
    Light.wakeup_active = 0;
  }
  ResponseCmndNumber(Settings.light_wakeup);
}

void CmndUndocA(void)
{  // Theos legacy status
  char scolor[LIGHT_COLOR_SIZE];
  LightGetColor(scolor, true);  // force hex whatever Option 17
  scolor[6] = '\0';  // RGB only
  Response_P(PSTR("%s,%d,%d,%d,%d,%d"), scolor, Settings.light_fade, Settings.light_correction, Settings.light_scheme, Settings.light_speed, Settings.light_width);
  MqttPublishPrefixTopic_P(STAT, XdrvMailbox.topic);
  mqtt_data[0] = '\0';
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xdrv04(uint8_t function)
{
  bool result = false;

  if (FUNC_MODULE_INIT == function) {
    return LightModuleInit();
  }
  else if (light_type) {
    switch (function) {
      case FUNC_SERIAL:
        result = XlgtCall(FUNC_SERIAL);
        break;
      case FUNC_LOOP:
        if (Light.fade_running) {
          if (LightApplyFade()) {
            LightSetOutputs(Light.fade_cur_10);
          }
        }
        break;
      case FUNC_EVERY_50_MSECOND:
        LightAnimate();
        break;
      case FUNC_SET_POWER:
        LightSetPower();
        break;
      case FUNC_COMMAND:
        result = DecodeCommand(kLightCommands, LightCommand);
        if (!result) {
          result = XlgtCall(FUNC_COMMAND);
        }
        break;
      case FUNC_PRE_INIT:
        LightInit();
        break;
    }
  }
  return result;
}

#endif  // USE_LIGHT
