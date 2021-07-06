/*
  xdrv_04_light.ino - PWM, WS2812 and sonoff led support for Tasmota

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
 *     SetOption82 can expand the rendering from 200-380 due to Alexa reduced range.
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
 *     4 channels - 0:Red 1:Green 2:Blue 3:White
 *     5 channels - 0:Red 1:Green 2:Blue 3:ColdWhite 4:Warmwhite
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
  // SetOptions synonyms
  D_SO_CHANNELREMAP "|" D_SO_MULTIPWM "|" D_SO_ALEXACTRANGE "|" D_SO_POWERONFADE "|" D_SO_PWMCT "|"
  D_SO_WHITEBLEND "|"
  // Other commands
  D_CMND_COLOR "|" D_CMND_COLORTEMPERATURE "|" D_CMND_DIMMER "|" D_CMND_DIMMER_RANGE "|" D_CMND_DIMMER_STEP "|" D_CMND_LEDTABLE "|" D_CMND_FADE "|"
  D_CMND_RGBWWTABLE "|" D_CMND_SCHEME "|" D_CMND_SPEED "|" D_CMND_WAKEUP "|" D_CMND_WAKEUPDURATION "|"
  D_CMND_WHITE "|" D_CMND_CHANNEL "|" D_CMND_HSBCOLOR
  "|" D_CMND_CTRANGE
#ifdef USE_LIGHT_VIRTUAL_CT
  "|" D_CMND_VIRTUALCT
#endif // USE_LIGHT_VIRTUAL_CT
#ifdef USE_LIGHT_PALETTE
  "|" D_CMND_PALETTE
#endif  // USE_LIGHT_PALETTE
#ifdef USE_DGR_LIGHT_SEQUENCE
  "|" D_CMND_SEQUENCE_OFFSET
#endif  // USE_DGR_LIGHT_SEQUENCE
   "|UNDOCA" ;

SO_SYNONYMS(kLightSynonyms,
  37, 68, 82, 91, 92,
  105,
);

void (* const LightCommand[])(void) PROGMEM = {
  &CmndColor, &CmndColorTemperature, &CmndDimmer, &CmndDimmerRange, &CmndDimmerStep, &CmndLedTable, &CmndFade,
  &CmndRgbwwTable, &CmndScheme, &CmndSpeed, &CmndWakeup, &CmndWakeupDuration,
  &CmndWhite, &CmndChannel, &CmndHsbColor,
  &CmndCTRange,
#ifdef USE_LIGHT_VIRTUAL_CT
  &CmndVirtualCT,
#endif // USE_LIGHT_VIRTUAL_CT
#ifdef USE_LIGHT_PALETTE
  &CmndPalette,
#endif  // USE_LIGHT_PALETTE
#ifdef USE_DGR_LIGHT_SEQUENCE
  &CmndSequenceOffset,
#endif  // USE_DGR_LIGHT_SEQUENCE
  &CmndUndocA };

// Light color mode, either RGB alone, or white-CT alone, or both only available if ct_rgb_linked is false
enum LightColorModes {
  LCM_RGB = 1, LCM_CT = 2, LCM_BOTH = 3
};

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

// CT min and max
const uint16_t CT_MIN = 153;          // 6500K
const uint16_t CT_MAX = 500;          // 2000K
// Ranges used for Alexa
const uint16_t CT_MIN_ALEXA = 200;    // also 5000K
const uint16_t CT_MAX_ALEXA = 380;    // also 2600K
// Virtual CT default values
typedef uint8_t  vct_pivot_t[LST_MAX];
const size_t CT_PIVOTS = LIGHT_VIRTUAL_CT_POINTS;
const vct_pivot_t CT_PIVOTS_RGB PROGMEM = { 255, 255, 255,   0,   0 };
const vct_pivot_t CT_PIVOTS_CWW PROGMEM = {   0,   0,   0, 255,   0 };
const vct_pivot_t CT_PIVOTS_WWW PROGMEM = {   0,   0,   0,   0, 255 };

struct LIGHT {
  uint32_t strip_timer_counter = 0;  // Bars and Gradient
  power_t power = 0;                      // Power<x> for each channel if SetOption68, or boolean if single light

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
  uint8_t wakeup_active = 0;             // 0=inctive, 1=on-going, 2=about to start, 3=will be triggered next cycle
  uint8_t fixed_color_index = 1;
  uint8_t pwm_offset = 0;                 // Offset in color buffer
  uint8_t max_scheme = LS_MAX -1;

  uint32_t wakeup_start_time = 0;

  bool update = true;
  bool pwm_multi_channels = false;        // SetOption68, treat each PWM channel as an independant dimmer
  bool virtual_ct = false;                // SetOption106, add a 5th virtual channel, only if SO106 = 1, SO68 = 0, Light is RGBW (4 channels), SO37 < 128

  bool     fade_initialized = false;      // dont't fade at startup
  bool     fade_running = false;
#ifdef USE_DEVICE_GROUPS
  uint8_t  last_scheme = 0;
  bool     devgrp_no_channels_out = false; // don't share channels with device group (e.g. if scheme set by other device)
#ifdef USE_DGR_LIGHT_SEQUENCE
  uint8_t  sequence_offset = 0;            // number of channel changes this light is behind the master
  uint8_t * channels_fifo;
#endif  // USE_DGR_LIGHT_SEQUENCE
#endif  // USE_DEVICE_GROUPS
#ifdef USE_LIGHT_PALETTE
  uint8_t  palette_count = 0;            // palette entry count
  uint8_t * palette;                     // dynamically allocated palette color array
#endif  // USE_LIGHT_PALETTE
  uint16_t fade_start_10[LST_MAX] = {0,0,0,0,0};
  uint16_t fade_cur_10[LST_MAX];
  uint16_t fade_end_10[LST_MAX];         // 10 bits resolution target channel values
  uint16_t fade_duration = 0;            // duration of fade in milliseconds
  uint32_t fade_start = 0;               // fade start time in milliseconds, compared to millis()
  bool     fade_once_enabled = false;    // override fade a single time
  bool     fade_once_value = false;      // override fade a single time
  bool     speed_once_enabled = false;   // override speed a single time
  uint8_t  speed_once_value = 0;         // override speed a single time

  uint16_t pwm_min = 0;                  // minimum value for PWM, from DimmerRange, 0..1023
  uint16_t pwm_max = 1023;               // maxumum value for PWM, from DimmerRange, 0..1023

  // Virtual CT
  uint16_t vct_ct[CT_PIVOTS];            // CT value for each segment
#ifdef USE_LIGHT_VIRTUAL_CT
  vct_pivot_t vct_color[CT_PIVOTS];      // array of 3 colors each with 5 values
#endif
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
// Note: you can query values from this singleton. But to change values,
//   use the LightController - changing this object will have no effect on actual light.
//
class LightStateClass {
  private:
    uint16_t _hue = 0;  // 0..359
    uint8_t  _sat = 255;  // 0..255
    uint8_t  _briRGB = 255;  // 0..255
    uint8_t  _briRGB_orig = 255;  // 0..255
    // dimmer is same as _bri but with a range of 0%-100%
    uint8_t  _r = 255;  // 0..255
    uint8_t  _g = 255;  // 0..255
    uint8_t  _b = 255;  // 0..255

    uint8_t  _subtype = 0;  // local copy of Light.subtype, if we need multiple lights
    uint16_t _ct = CT_MIN;  // 153..500, default to 153 (cold white)
    uint8_t  _wc = 255;  // white cold channel
    uint8_t  _ww = 0;    // white warm channel
    uint8_t  _briCT = 255;
    uint8_t  _briCT_orig = 255;

    uint8_t  _color_mode = LCM_RGB; // RGB by default

  public:
    LightStateClass() {
      //AddLog(LOG_LEVEL_DEBUG_MORE, "LightStateClass::Constructor RGB raw (%d %d %d) HS (%d %d) bri (%d)", _r, _g, _b, _hue, _sat, _bri);
    }

    void setSubType(uint8_t sub_type) {
      _subtype = sub_type;    // set sub_type at initialization, shoudln't be changed afterwards
    }

    // This function is a bit hairy, it will try to match the required
    // colormode with the features of the device:
    //   LST_NONE:      LCM_RGB
    //   LST_SINGLE:    LCM_RGB
    //   LST_COLDWARM:  LCM_CT
    //   LST_RGB:       LCM_RGB
    //   LST_RGBW:      LCM_RGB, LCM_CT or LCM_BOTH
    //   LST_RGBCW:     LCM_RGB, LCM_CT or LCM_BOTH
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
        case LST_RGBCW:
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
      AddLog(LOG_LEVEL_DEBUG_MORE, "LightStateClass::setColorMode prev_cm (%d) req_cm (%d) new_cm (%d)", prev_cm, cm, _color_mode);
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

    void getChannels(uint8_t *channels) {
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

    inline uint8_t getBriCTOrig() {
      return _briCT_orig;
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

    inline uint16_t getCT() const {
      return _ct; // 153..500, or CT_MIN..CT_MAX
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
      AddLog(LOG_LEVEL_DEBUG_MORE, "LightStateClass::setBri RGB raw (%d %d %d) HS (%d %d) bri (%d)", _r, _g, _b, _hue, _sat, _briRGB);
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

    inline uint8_t getBriRGBOrig() {
      return _briRGB_orig;
    }

    void setDimmer(uint8_t dimmer) {
      setBri(DimmerToBri(dimmer));
    }

    void setCT(uint16_t ct) {
      if (0 == ct) {
        // disable ct mode
        setColorMode(LCM_RGB);  // try deactivating CT mode, setColorMode() will check which is legal
      } else {
        ct = (ct < CT_MIN ? CT_MIN : (ct > CT_MAX ? CT_MAX : ct));
        _ww = changeUIntScale(ct, Light.vct_ct[0], Light.vct_ct[CT_PIVOTS-1], 0, 255);
        _wc = 255 - _ww;
        _ct = ct;
        addCTMode();
      }
#ifdef DEBUG_LIGHT
      AddLog(LOG_LEVEL_DEBUG_MORE, "LightStateClass::setCT RGB raw (%d %d %d) HS (%d %d) briRGB (%d) briCT (%d) CT (%d)", _r, _g, _b, _hue, _sat, _briRGB, _briCT, _ct);
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
      if (sum <= 257) { free_range = false; }    // if we don't allow free range or if sum is below 255 (with tolerance of 2)

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
        _ct = changeUIntScale(w, 0, sum, CT_MIN, CT_MAX);
        addCTMode();   // activate CT mode if needed
        if (_color_mode & LCM_CT) { _briCT = free_range ? max : (sum > 255 ? 255 : sum); }
      }
#ifdef DEBUG_LIGHT
      AddLog(LOG_LEVEL_DEBUG_MORE, "LightStateClass::setCW CW (%d %d) CT (%d) briCT (%d)", c, w, _ct, _briCT);
#endif
    }

    // sets RGB and returns the Brightness. Bri is updated unless keep_bri is true
    uint8_t setRGB(uint8_t r, uint8_t g, uint8_t b, bool keep_bri = false) {
      uint16_t hue;
      uint8_t  sat;
#ifdef DEBUG_LIGHT
      AddLog(LOG_LEVEL_DEBUG_MORE, "LightStateClass::setRGB RGB input (%d %d %d)", r, g, b);
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
      AddLog(LOG_LEVEL_DEBUG_MORE, "LightStateClass::setRGB RGB raw (%d %d %d) HS (%d %d) bri (%d)", _r, _g, _b, _hue, _sat, _briRGB);
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
      AddLog(LOG_LEVEL_DEBUG_MORE, "LightStateClass::setHS HS (%d %d) rgb (%d %d %d)", hue, sat, r, g, b);
      AddLog(LOG_LEVEL_DEBUG_MORE, "LightStateClass::setHS RGB raw (%d %d %d) HS (%d %d) bri (%d)", _r, _g, _b, _hue, _sat, _briRGB);
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
    uint8_t r = channels[0];
    uint8_t g = channels[1];
    uint8_t b = channels[2];
    uint8_t cw = channels[3];
    uint8_t ww = channels[4];
    _briRGB_orig = (r > g && r > b) ? r : (g > b) ? g : b;
    _briCT_orig = (cw > ww) ? cw : ww;
#ifdef DEBUG_LIGHT
    AddLog(LOG_LEVEL_DEBUG_MORE, "LightStateClass::setChannels (%d %d %d %d %d)",
      channels[0], channels[1], channels[2], channels[3], channels[4]);
    AddLog(LOG_LEVEL_DEBUG_MORE, "LightStateClass::setChannels CT (%d) briRGB (%d) briCT (%d)", _ct, _briRGB, _briCT);
    AddLog(LOG_LEVEL_DEBUG_MORE, "LightStateClass::setChannels Actuals (%d %d %d %d %d)",
      _r, _g, _b, _wc, _ww);
#endif
  }
};


/*********************************************************************************************\
 * LightStateClass implementation
\*********************************************************************************************/

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
    AddLog(LOG_LEVEL_DEBUG_MORE, "LightControllerClass::debugLogs rgb (%d %d %d) cw (%d %d)",
      r, g, b, c, w);
    AddLog(LOG_LEVEL_DEBUG_MORE, "LightControllerClass::debugLogs lightCurrent (%d %d %d %d %d)",
      Light.current_color[0], Light.current_color[1], Light.current_color[2],
      Light.current_color[3], Light.current_color[4]);
  }
#endif

  void loadSettings() {
#ifdef DEBUG_LIGHT
    AddLog(LOG_LEVEL_DEBUG_MORE, "LightControllerClass::loadSettings Settings->light_color (%d %d %d %d %d - %d)",
      Settings->light_color[0], Settings->light_color[1], Settings->light_color[2],
      Settings->light_color[3], Settings->light_color[4], Settings->light_dimmer);
    AddLog(LOG_LEVEL_DEBUG_MORE, "LightControllerClass::loadSettings light_type/sub (%d %d)",
      TasmotaGlobal.light_type, Light.subtype);
#endif
    if (_pwm_multi_channels) {
      _state->setChannelsRaw(Settings->light_color);
    } else {
      // first try setting CW, if zero, it select RGB mode
      _state->setCW(Settings->light_color[3], Settings->light_color[4], true);
      _state->setRGB(Settings->light_color[0], Settings->light_color[1], Settings->light_color[2]);

      // only if non-multi channel
      // We apply dimmer in priority to RGB
      uint8_t bri = _state->DimmerToBri(Settings->light_dimmer);

      // The default values are #FFFFFFFFFF, in this case we avoid setting all channels
      // at the same time, see #6534 and #8120
      if ((DEFAULT_LIGHT_COMPONENT == Settings->light_color[0]) &&
          (DEFAULT_LIGHT_COMPONENT == Settings->light_color[1]) &&
          (DEFAULT_LIGHT_COMPONENT == Settings->light_color[2]) &&
          (DEFAULT_LIGHT_COMPONENT == Settings->light_color[3]) &&
          (DEFAULT_LIGHT_COMPONENT == Settings->light_color[4]) &&
          (DEFAULT_LIGHT_DIMMER    == Settings->light_dimmer) ) {
        if ((LST_COLDWARM == Light.subtype) || (LST_RGBCW == Light.subtype)) {
          _state->setCW(255, 0);       // avoid having both white channels at 100%, zero second channel (#see 8120)
        }
        _state->setBriCT(bri);
        _state->setBriRGB(bri);
        _state->setColorMode(LCM_RGB);
      }

      if (Settings->light_color[0] + Settings->light_color[1] + Settings->light_color[2] > 0) {
        _state->setBriRGB(bri);
      } else {
        _state->setBriCT(bri);
      }
    }
  }

  void changeCTB(uint16_t new_ct, uint8_t briCT) {
    /* Color Temperature (https://developers.meethue.com/documentation/core-concepts)
     *
     * ct = 153 = 6500K = Cold = CCWW = FF00
     * ct = 500 = 2000K = Warm = CCWW = 00FF
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
      case LST_RGBCW:
        if (LST_RGBCW == Light.subtype) {
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

  // save the current light state to Settings
  void saveSettings() {
    if (Light.pwm_multi_channels) {
      // simply save each channel
      _state->getChannelsRaw(Settings->light_color);
      Settings->light_dimmer = 100;    // arbitrary value, unused in this mode
    } else {
      uint8_t cm = _state->getColorMode();

      memset(&Settings->light_color[0], 0, sizeof(Settings->light_color));    // clear all channels
      if (LCM_RGB & cm) {   // can be either LCM_RGB or LCM_BOTH
        _state->getRGB(&Settings->light_color[0], &Settings->light_color[1], &Settings->light_color[2]);
        Settings->light_dimmer = _state->BriToDimmer(_state->getBriRGB());
        // anyways we always store RGB with BrightnessRGB
        if (LCM_BOTH == cm) {
          // then store at actual brightness CW/WW if dual mode
          _state->getActualRGBCW(nullptr, nullptr, nullptr, &Settings->light_color[3], &Settings->light_color[4]);
        }
      } else if (LCM_CT == cm) {    // cm can only be LCM_CT
        _state->getCW(&Settings->light_color[3], &Settings->light_color[4]);
        Settings->light_dimmer = _state->BriToDimmer(_state->getBriCT());
      }
    }
#ifdef DEBUG_LIGHT
    AddLog(LOG_LEVEL_DEBUG_MORE, "LightControllerClass::saveSettings Settings->light_color (%d %d %d %d %d - %d)",
      Settings->light_color[0], Settings->light_color[1], Settings->light_color[2],
      Settings->light_color[3], Settings->light_color[4], Settings->light_dimmer);
#endif
  }

  // set all 5 channels at once.
  // Channels are: R G B CW WW
  // Brightness is automatically recalculated to adjust channels to the desired values
  // if channelsmapped is true, CCT/RGB mapping has already been done
  void changeChannels(uint8_t *channels, bool channelsmapped=false) {
    if (Light.pwm_multi_channels) {
      _state->setChannelsRaw(channels);
    } else if (LST_COLDWARM == Light.subtype && ! channelsmapped) {
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
 * CT (White Color Temperature)
\*********************************************************************************************/
//
// Ensure that invariants for Virtual CT are good:
// - CT_MIN <= ct[0] <= ct[1] <= ct[2] <= CT_MAX

#ifdef USE_LIGHT_VIRTUAL_CT
void checkVirtualCT(void) {
  if (Light.vct_ct[0] < CT_MIN)       { Light.vct_ct[0] = CT_MIN; }
  if (Light.vct_ct[CT_PIVOTS-1] > CT_MAX)       { Light.vct_ct[CT_PIVOTS-1] = CT_MAX; }
  for (uint32_t i = 0; i < CT_PIVOTS-1; i++) {
    if (Light.vct_ct[i+1] < Light.vct_ct[i])    { Light.vct_ct[i+1] = Light.vct_ct[i]; }
  }
}
#endif // USE_LIGHT_VIRTUAL_CT

#ifdef USE_LIGHT_VIRTUAL_CT
// Init default values for virtual CT, depending on the number of channels
void initCTRange(uint32_t channels) {
  if (channels == 4) {
    if (Settings->flag4.virtual_ct_cw) {       // Hardware White is Cold White
      memcpy_P(Light.vct_color[0], CT_PIVOTS_CWW, sizeof(Light.vct_color[0]));      // Cold white
      memcpy_P(Light.vct_color[1], CT_PIVOTS_RGB, sizeof(Light.vct_color[1]));      // Warm white
    } else {       // Hardware White is Warm White
      memcpy_P(Light.vct_color[0], CT_PIVOTS_RGB, sizeof(Light.vct_color[0]));      // Cold white
      memcpy_P(Light.vct_color[1], CT_PIVOTS_CWW, sizeof(Light.vct_color[1]));      // Warm white
    }
  } else if (channels == 5) {
    memcpy_P(Light.vct_color[0], CT_PIVOTS_CWW, sizeof(Light.vct_color[0]));      // Cold white
    memcpy_P(Light.vct_color[1], CT_PIVOTS_WWW, sizeof(Light.vct_color[1]));      // Warm white
  } else {
    memcpy_P(Light.vct_color[0], CT_PIVOTS_RGB, sizeof(Light.vct_color[0]));      // Cold white
    memcpy_P(Light.vct_color[1], CT_PIVOTS_RGB, sizeof(Light.vct_color[1]));      // Warm white
  }
  for (uint32_t i = 1; i < CT_PIVOTS-1; i++) {
    memcpy_P(Light.vct_color[i+1], Light.vct_color[i], sizeof(Light.vct_color[0]));      // Copy slot 1 into slot 2 (slot 2 in unused)
  }
  checkVirtualCT();
}
#endif // USE_LIGHT_VIRTUAL_CT

void getCTRange(uint16_t * min_ct, uint16_t * max_ct) {
  if (min_ct != nullptr) { *min_ct = Light.vct_ct[0]; }
  if (max_ct != nullptr) { *max_ct = Light.vct_ct[CT_PIVOTS-1]; }
}

void setCTRange(uint16_t ct_min, uint16_t ct_max) {
  Light.vct_ct[0] = ct_min;
  for (uint32_t i = 1; i < CT_PIVOTS; i++) {
    Light.vct_ct[i] = ct_max;     // all slots above [1] are not used
  }
}

void setAlexaCTRange(void) {    // depending on SetOption82, full or limited CT range
  if (Settings->flag4.alexa_ct_range) {
    setCTRange(CT_MIN_ALEXA, CT_MAX_ALEXA);
  } else {
    setCTRange(CT_MIN, CT_MAX);
  }
}

/********************************************************************************************/

void LightPwmOffset(uint32_t offset)
{
  Light.pwm_offset = offset;
}

bool LightModuleInit(void)
{
  TasmotaGlobal.light_type = LT_BASIC;                 // Use basic PWM control if SetOption15 = 0

  if (Settings->flag.pwm_control) {                     // SetOption15 - Switch between commands PWM or COLOR/DIMMER/CT/CHANNEL
    for (uint32_t i = 0; i < MAX_PWMS; i++) {
      if (PinUsed(GPIO_PWM1, i)) { TasmotaGlobal.light_type++; }  // Use Dimmer/Color control for all PWM as SetOption15 = 1
    }
  }

  TasmotaGlobal.light_driver = 0;
  if (XlgtCall(FUNC_MODULE_INIT)) {
    // serviced
  }
#ifdef ESP8266
  else if (SONOFF_BN == TasmotaGlobal.module_type) {   // PWM Single color led (White)
    TasmotaGlobal.light_type = LT_PWM1;
  }
  else if (SONOFF_LED == TasmotaGlobal.module_type) {  // PWM Dual color led (White warm and cold)
    if (!TasmotaGlobal.my_module.io[4]) {              // Fix Sonoff Led instabilities
      pinMode(4, OUTPUT);                              // Stop floating outputs
      digitalWrite(4, LOW);
    }
    if (!TasmotaGlobal.my_module.io[5]) {
      pinMode(5, OUTPUT);                              // Stop floating outputs
      digitalWrite(5, LOW);
    }
    if (!TasmotaGlobal.my_module.io[14]) {
      pinMode(14, OUTPUT);                             // Stop floating outputs
      digitalWrite(14, LOW);
    }
    TasmotaGlobal.light_type = LT_PWM2;
  }
#endif  // ESP8266
#ifdef USE_PWM_DIMMER
#ifdef USE_DEVICE_GROUPS
  else if (PWM_DIMMER == TasmotaGlobal.module_type) {
    TasmotaGlobal.light_type = Settings->pwm_dimmer_cfg.pwm_count + 1;
  }
#endif  // USE_DEVICE_GROUPS
#endif  // USE_PWM_DIMMER

  if (TasmotaGlobal.light_type > LT_BASIC) {
    TasmotaGlobal.devices_present++;
  }

  // post-process for lights
  uint32_t pwm_channels = (TasmotaGlobal.light_type & 7) > LST_MAX ? LST_MAX : (TasmotaGlobal.light_type & 7);
  if (Settings->flag3.pwm_multi_channels) {  // SetOption68 - Enable multi-channels PWM instead of Color PWM
    if (0 == pwm_channels) { pwm_channels = 1; }
    TasmotaGlobal.devices_present += pwm_channels - 1;    // add the pwm channels controls at the end
  } else if ((Settings->param[P_RGB_REMAP] & 128) && (LST_RGBW <= pwm_channels)) {  // SetOption37
    // if RGBW or RGBCW, and SetOption37 >= 128, we manage RGB and W separately, hence adding a device
    TasmotaGlobal.devices_present++;
  } else {
#ifdef USE_LIGHT_VIRTUAL_CT
    initCTRange(pwm_channels);
    if ((Settings->flag4.virtual_ct) && (LST_RGB <= pwm_channels)) {
      Light.virtual_ct = true;    // enabled
      TasmotaGlobal.light_type += 5 - pwm_channels;               // pretend it is a 5 channels bulb
    }
#endif // USE_LIGHT_VIRTUAL_CT
  }

  return (TasmotaGlobal.light_type > LT_BASIC);
}

// compute actual PWM min/max values from DimmerRange
// must be called when DimmerRange is changed or LedTable
void LightCalcPWMRange(void) {
  uint16_t pwm_min, pwm_max;

  pwm_min = change8to10(LightStateClass::DimmerToBri(Settings->dimmer_hw_min));   // default 0
  pwm_max = change8to10(LightStateClass::DimmerToBri(Settings->dimmer_hw_max));   // default 100
  if (Settings->light_correction) {
    pwm_min = ledGamma10_10(pwm_min);       // apply gamma correction
    pwm_max = ledGamma10_10(pwm_max);       // 0..1023
  }
  pwm_min = pwm_min > 0 ? changeUIntScale(pwm_min, 1, 1023, 1, Settings->pwm_range) : 0;  // adapt range but keep zero and non-zero values
  pwm_max = changeUIntScale(pwm_max, 1, 1023, 1, Settings->pwm_range);  // pwm_max cannot be zero

  Light.pwm_min = pwm_min;
  Light.pwm_max = pwm_max;
  //AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("LightCalcPWMRange %d %d - %d %d"), Settings->dimmer_hw_min, Settings->dimmer_hw_max, Light.pwm_min, Light.pwm_max);
}

void LightSetScheme(uint32_t scheme) {
  if (!scheme && Settings->light_scheme) {
    Light.update = true;
  }
  Settings->light_scheme = scheme;
}

void LightInit(void)
{
  // move white blend mode from deprecated `RGBWWTable` to `SetOption105`
  if (0 == Settings->rgbwwTable[4]) {
    Settings->flag4.white_blend_mode = true;
    Settings->rgbwwTable[4] = 255;       // set RGBWWTable value to its default
  }

  Light.device = TasmotaGlobal.devices_present;
  Light.subtype = (TasmotaGlobal.light_type & 7) > LST_MAX ? LST_MAX : (TasmotaGlobal.light_type & 7); // Always 0 - LST_MAX (5)
  Light.pwm_multi_channels = Settings->flag3.pwm_multi_channels;  // SetOption68 - Enable multi-channels PWM instead of Color PWM

  if (LST_RGBW <= Light.subtype) {
    // only change if RGBW or RGBCW
    // do not allow independant RGB and WC colors
    bool ct_rgb_linked = !(Settings->param[P_RGB_REMAP] & 128);  // SetOption37
    light_controller.setCTRGBLinked(ct_rgb_linked);
  }

  if ((LST_SINGLE <= Light.subtype) && Light.pwm_multi_channels) {
    // we treat each PWM channel as an independant one, hence we switch to
    light_controller.setPWMMultiChannel(true);
    Light.device = TasmotaGlobal.devices_present - Light.subtype + 1; // adjust if we also have relays
  } else if (!light_controller.isCTRGBLinked()) {
    // if RGBW or RGBCW, and SetOption37 >= 128, we manage RGB and W separately
    Light.device--;   // we take the last two devices as lights
  }
  LightCalcPWMRange();
#ifdef DEBUG_LIGHT
  AddLog(LOG_LEVEL_DEBUG_MORE, "LightInit Light.pwm_multi_channels=%d Light.subtype=%d Light.device=%d TasmotaGlobal.devices_present=%d",
    Light.pwm_multi_channels, Light.subtype, Light.device, TasmotaGlobal.devices_present);
#endif

  light_controller.setSubType(Light.subtype);
  light_controller.loadSettings();
  setAlexaCTRange();
  light_controller.calcLevels();    // calculate the initial values (#8058)

  if (LST_SINGLE == Light.subtype) {
    Settings->light_color[0] = 255;      // One channel only supports Dimmer but needs max color
  }
  if (TasmotaGlobal.light_type < LT_PWM6) {           // PWM
    for (uint32_t i = 0; i < TasmotaGlobal.light_type; i++) {
      Settings->pwm_value[i] = 0;        // Disable direct PWM control
      if (PinUsed(GPIO_PWM1, i)) {
#ifdef ESP8266
        pinMode(Pin(GPIO_PWM1, i), OUTPUT);
#endif  // ESP8266
#ifdef ESP32
        analogAttach(Pin(GPIO_PWM1, i), i);
#endif  // ESP32
      }
    }
    if (PinUsed(GPIO_ARIRFRCV)) {
      if (PinUsed(GPIO_ARIRFSEL)) {
        pinMode(Pin(GPIO_ARIRFSEL), OUTPUT);
        digitalWrite(Pin(GPIO_ARIRFSEL), 1);  // Turn off RF
      }
    }
  }

  uint32_t max_scheme = Light.max_scheme;
  if (Light.subtype < LST_RGB) {
    max_scheme = LS_POWER;
  }
  if ((LS_WAKEUP == Settings->light_scheme) || (Settings->light_scheme > max_scheme)) {
    LightSetScheme(LS_POWER);
  }
  Light.power = 0;
  Light.update = true;
  Light.wakeup_active = 0;
  if (0 == Settings->light_wakeup) {
    Settings->light_wakeup = 60;         // Fix divide by zero exception 0 in Animate
  }
  if (Settings->flag4.fade_at_startup) {
    Light.fade_initialized = true;      // consider fade intialized starting from black
  }

  LightUpdateColorMapping();
}

void LightUpdateColorMapping(void)
{
  uint8_t param = Settings->param[P_RGB_REMAP] & 127;  // SetOption37
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
  //AddLog(LOG_LEVEL_DEBUG, PSTR("%d colors: %d %d %d %d %d") ,Settings->param[P_RGB_REMAP], Light.color_remap[0],Light.color_remap[1],Light.color_remap[2],Light.color_remap[3],Light.color_remap[4]);
}

uint8_t LightGetDimmer(uint8_t dimmer) {
  return light_state.getDimmer(dimmer);
}

void LightSetDimmer(uint8_t dimmer) {
  light_controller.changeDimmer(dimmer);
}

void LightGetHSB(uint16_t *hue, uint8_t *sat, uint8_t *bri) {
  light_state.getHSB(hue, sat, bri);
}

void LightGetXY(float *X, float *Y) {
  light_state.getXY(X, Y);
}

// If SetOption68 is set, get the brightness for a specific device
uint8_t LightGetBri(uint8_t device) {
  uint8_t bri = 254;   // default value if relay
  if (Light.pwm_multi_channels) {
    if ((device >= Light.device) && (device < Light.device + LST_MAX) && (device <= TasmotaGlobal.devices_present)) {
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
    if ((device >= Light.device) && (device < Light.device + LST_MAX) && (device <= TasmotaGlobal.devices_present)) {
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

void LightSetBriScaled(uint8_t bri) {
  // change both dimmers, retain ratio between white and color channels
  uint32_t bri_rgb = light_state.getBriRGBOrig();
  uint32_t bri_ct = light_state.getBriCTOrig();
#ifdef DEBUG_LIGHT
  AddLog(LOG_LEVEL_DEBUG, "LightSetBri bri:%d, bri_rgb:%d, bri_ct: %d", bri, bri_rgb, bri_ct);
#endif
  uint32_t max_bri = bri_rgb > bri_ct ? bri_rgb : bri_ct;
  if (max_bri == 0) {
    bri_rgb = bri;
    bri_ct = bri;
  } else {
    bri_rgb = changeUIntScale(bri_rgb, 0, max_bri, 0, bri);
    bri_ct = changeUIntScale(bri_ct, 0, max_bri, 0, bri);
  }
#ifdef DEBUG_LIGHT
  AddLog(LOG_LEVEL_DEBUG, "LightSetBri new bri_rgb:%d, new bri_ct: %d", bri_rgb, bri_ct);
#endif
  light_controller.changeBriRGB(bri_rgb);
  light_controller.changeBriCT(bri_ct);
}

void LightColorOffset(int32_t offset) {
  uint16_t hue;
  uint8_t sat;
  light_state.getHSB(&hue, &sat, nullptr);  // Allow user control over Saturation
  hue += offset;
  if (hue < 0) { hue += 359; }
  if (hue > 359) { hue -= 359; }
  if (!Light.pwm_multi_channels) {
    light_state.setHS(hue, sat);
  } else {
    light_state.setHS(hue, 255);
    light_state.setBri(255);        // If multi-channel, force bri to max, it will be later dimmed to correct value
  }
  light_controller.calcLevels(Light.new_color);
}

bool LightColorTempOffset(int32_t offset) {
  int32_t ct = LightGetColorTemp();
  if (0 == ct) { return false; }  // CT not supported
  ct += offset;
  if (ct < CT_MIN) { ct = CT_MIN; }
  else if (ct > CT_MAX) { ct = CT_MAX; }

  LightSetColorTemp(ct);
  return true;
}

void LightSetColorTemp(uint16_t ct)
{
/* Color Temperature (https://developers.meethue.com/documentation/core-concepts)
 *
 * ct = 153 mirek = 6500K = Cold = CCWW = FF00
 * ct = 500 mirek = 2000K = Warm = CCWW = 00FF
 */
  // don't set CT if not supported
  if ((LST_COLDWARM != Light.subtype) && (LST_RGBCW != Light.subtype)) {
    return;
  }
  light_controller.changeCTB(ct, light_state.getBriCT());
}

uint16_t LightGetColorTemp(void)
{
  // don't calculate CT for unsupported devices
  if ((LST_COLDWARM != Light.subtype) && (LST_RGBCW != Light.subtype)) {
    return 0;
  }
  return (light_state.getColorMode() & LCM_CT) ? light_state.getCT() : 0;
}

void LightSetSignal(uint16_t lo, uint16_t hi, uint16_t value)
{
/* lo - below lo is green
   hi - above hi is red
*/
  if (Settings->flag.light_signal) {  // SetOption18 - Pair light signal with CO2 sensor
    uint16_t signal = changeUIntScale(value, lo, hi, 0, 255);  // 0..255
//    AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_DEBUG "Light signal %d"), signal);
    light_controller.changeRGB(signal, 255 - signal, 0, true);  // keep bri
    LightSetScheme(LS_POWER);
    if (0 == light_state.getBri()) {
      light_controller.changeBri(50);
    }
  }
}

// convert channels to string, use Option 17 to foce decimal, unless force_hex
char* LightGetColor(char* scolor, boolean force_hex = false)
{
  if ((0 == Settings->light_scheme) || (!Light.pwm_multi_channels)) {
    light_controller.calcLevels();      // recalculate levels only if Scheme 0, otherwise we mess up levels
  }
  scolor[0] = '\0';
  for (uint32_t i = 0; i < Light.subtype; i++) {
    if (!force_hex && Settings->flag.decimal_text) {  // SetOption17 - Switch between decimal or hexadecimal output
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

void ResponseLightState(uint8_t append)
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
      ResponseAppend_P(PSTR("\"" D_RSLT_POWER "%d\":\"%s\",\"" D_CMND_DIMMER "1\":%d"
                           ",\"" D_RSLT_POWER "%d\":\"%s\",\"" D_CMND_DIMMER "2\":%d"),
                            Light.device, GetStateText(Light.power & 1), light_state.getDimmer(1),
                            Light.device + 1, GetStateText(Light.power & 2 ? 1 : 0), light_state.getDimmer(2));
    } else {
      GetPowerDevice(scommand, Light.device, sizeof(scommand), Settings->flag.device_index_enable);  // SetOption26 - Switch between POWER or POWER1
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
      if ((LST_COLDWARM == Light.subtype) || (LST_RGBCW == Light.subtype)) {
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
        ResponseAppend_P(PSTR(",\"" D_CMND_SCHEME "\":%d"), Settings->light_scheme);
      }
      if (Light.max_scheme > LS_MAX) {
        ResponseAppend_P(PSTR(",\"" D_CMND_WIDTH "\":%d"), Settings->light_width);
      }
      ResponseAppend_P(PSTR(",\"" D_CMND_FADE "\":\"%s\",\"" D_CMND_SPEED "\":%d,\"" D_CMND_LEDTABLE "\":\"%s\""),
        GetStateText(Settings->light_fade), Settings->light_speed, GetStateText(Settings->light_correction));
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
  AddLog(LOG_LEVEL_DEBUG, "LightPreparePower power=%d Light.power=%d", TasmotaGlobal.power, Light.power);
#endif
  // If multi-channels, then we only switch off channels with a value of zero
  if (Light.pwm_multi_channels) {
    for (uint32_t i = 0; i < Light.subtype; i++) {
      if (bitRead(channels, i)) {
        // if channel is non-null, channel is supposed to be on, but it is off, do Power On
        if ((Light.current_color[i]) && (!bitRead(Light.power, i))) {
          if (!Settings->flag.not_power_linked) {  // SetOption20 - Control power in relation to Dimmer/Color/Ct changes
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
        if (!Settings->flag.not_power_linked) {  // SetOption20 - Control power in relation to Dimmer/Color/Ct changes
          ExecuteCommandPower(Light.device, POWER_ON_NO_STATE, SRC_LIGHT);
        }
      } else if (!light_state.getBri() && Light.power) {
        ExecuteCommandPower(Light.device, POWER_OFF_NO_STATE, SRC_LIGHT);
      }
    } else {
      // RGB
      if (channels & 1) {
        if (light_state.getBriRGB() && !(Light.power & 1)) {
          if (!Settings->flag.not_power_linked) {  // SetOption20 - Control power in relation to Dimmer/Color/Ct changes
            ExecuteCommandPower(Light.device, POWER_ON_NO_STATE, SRC_LIGHT);
          }
        } else if (!light_state.getBriRGB() && (Light.power & 1)) {
          ExecuteCommandPower(Light.device, POWER_OFF_NO_STATE, SRC_LIGHT);
        }
      }
      // White CT
      if (channels & 2) {
        if (light_state.getBriCT() && !(Light.power & 2)) {
          if (!Settings->flag.not_power_linked) {  // SetOption20 - Control power in relation to Dimmer/Color/Ct changes
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

  if (Settings->flag3.hass_tele_on_power) {  // SetOption59 - Send tele/%topic%/STATE in addition to stat/%topic%/RESULT
    MqttPublishTeleState();
  }

#ifdef DEBUG_LIGHT
  AddLog(LOG_LEVEL_DEBUG, "LightPreparePower End power=%d Light.power=%d", TasmotaGlobal.power, Light.power);
#endif
  Light.power = TasmotaGlobal.power >> (Light.device - 1);  // reset next state, works also with unlinked RGB/CT
  ResponseLightState(0);
}

#ifdef USE_LIGHT_PALETTE
void LightSetPaletteEntry(void)
{
  uint8_t bri = light_state.getBri();
  uint8_t * palette_entry = &Light.palette[Light.wheel * Light.subtype];
  for (int i = 0; i < Light.subtype; i++) {
    Light.new_color[i] = changeUIntScale(palette_entry[i], 0, 255, 0, bri);
  }
  light_state.setChannelsRaw(Light.new_color);
  if (!Light.pwm_multi_channels) {
    light_state.setCW(Light.new_color[3], Light.new_color[4], true);
    if (Light.new_color[0] || Light.new_color[1] || Light.new_color[2]) light_state.addRGBMode();
  }
}
#endif  // USE_LIGHT_PALETTE

void LightCycleColor(int8_t direction)
{
//  if (Light.strip_timer_counter % (Settings->light_speed * 2)) { return; }  // Speed 1: 24sec, 2: 48sec, 3: 72sec, etc
  if (Settings->light_speed > 3) {
    if (Light.strip_timer_counter % (Settings->light_speed - 2)) { return; }  // Speed 4: 24sec, 5: 36sec, 6: 48sec, etc
  }

#ifdef USE_LIGHT_PALETTE
  if (Light.palette_count) {
    if (!Light.fade_running) {
      if (0 == direction) {
        Light.wheel = random(Light.palette_count);
      }
      else {
        Light.wheel += direction;
        if (Light.wheel >= Light.palette_count) {
          Light.wheel = 0;
          if (direction < 0) Light.wheel = Light.palette_count - 1;
        }
      }
      LightSetPaletteEntry();
    }
    return;
  }
#endif  // USE_LIGHT_PALETTE

  if (0 == direction) {
    if (Light.random == Light.wheel) {
      Light.random = random(255);

      uint8_t my_dir = (Light.random < Light.wheel -128) ? 1 :
                       (Light.random < Light.wheel     ) ? 0 :
                       (Light.random > Light.wheel +128) ? 0 : 1;  // Increment or Decrement and roll-over
      Light.random = (Light.random & 0xFE) | my_dir;

//      AddLog(LOG_LEVEL_DEBUG, PSTR("LGT: random %d"), Light.random);
    }
//    direction = (Light.random < Light.wheel) ? -1 : 1;
    direction = (Light.random &0x01) ? 1 : -1;
  }

//  if (Settings->light_speed < 3) { direction <<= (3 - Settings->light_speed); }  // Speed 1: 12/4=3sec, 2: 12/2=6sec, 3: 12sec
  if (Settings->light_speed < 3) { direction *= (4 - Settings->light_speed); }  // Speed 1: 12/3=4sec, 2: 12/2=6sec, 3: 12sec
  Light.wheel += direction;
  uint16_t hue = changeUIntScale(Light.wheel, 0, 255, 0, 359);  // Scale to hue to keep amount of steps low (max 255 instead of 359)

//  AddLog(LOG_LEVEL_DEBUG, PSTR("LGT: random %d, wheel %d, hue %d"), Light.random, Light.wheel, hue);

  if (!Light.pwm_multi_channels) {
    uint8_t sat;
    light_state.getHSB(nullptr, &sat, nullptr);  // Allow user control over Saturation
    light_state.setHS(hue, sat);
  } else {
    light_state.setHS(hue, 255);
    light_state.setBri(255);        // If multi-channel, force bri to max, it will be later dimmed to correct value
  }
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
  // Ex: 3 Relays and 4 PWM - TasmotaGlobal.devices_present = 7, Light.device = 4, Light.subtype = 4
  // Result: mask = 0b00001111 = 0x0F, shift = 3.
  // Power bits we consider are: 0b01111000 = 0x78
  // If regular situation: TasmotaGlobal.devices_present == Light.subtype
  Light.power = (XdrvMailbox.index & (mask << shift)) >> shift;
  if (Light.wakeup_active) {
    Light.wakeup_active--;
  }
#ifdef DEBUG_LIGHT
  AddLog(LOG_LEVEL_DEBUG_MORE, "LightSetPower XdrvMailbox.index=%d Light.old_power=%d Light.power=%d mask=%d shift=%d",
    XdrvMailbox.index, Light.old_power, Light.power, mask, shift);
#endif
  if (Light.power != Light.old_power) {
    Light.update = true;
  }
  LightAnimate();
}

bool LightGetFadeSetting(void) {
  if (Light.fade_once_enabled) return Light.fade_once_value;
  return Settings->light_fade;
}

uint8_t LightGetSpeedSetting(void) {
  if (Light.speed_once_enabled) return Light.speed_once_value;
  return Settings->light_speed;
}

// On entry Light.new_color[5] contains the color to be displayed
// and Light.last_color[5] the color currently displayed
// Light.power tells which lights or channels (SetOption68) are on/off
void LightAnimate(void)
{
  bool power_off = false;

  // make sure we update CT range in case SetOption82 was changed
  Light.strip_timer_counter++;

  // set sleep parameter: either settings,
  // or set a maximum of PWM_MAX_SLEEP if light is on or Fade is running
  if (Light.power || Light.fade_running) {
    if (Settings->sleep > PWM_MAX_SLEEP) {
      TasmotaGlobal.sleep = PWM_MAX_SLEEP;      // set a maximum value (in milliseconds) to sleep to ensure that animations are smooth
    } else {
      TasmotaGlobal.sleep = Settings->sleep;     // or keep the current sleep if it's low enough
    }
  } else {
    TasmotaGlobal.sleep = Settings->sleep;
  }

  if (!Light.power) {                   // All channels powered off
    Light.strip_timer_counter = 0;
    if (Settings->light_scheme >= LS_MAX) {
      power_off = true;
    }
  } else {
    switch (Settings->light_scheme) {
      case LS_POWER:
        light_controller.calcLevels(Light.new_color);
        break;
      case LS_WAKEUP:
        {
          if (2 == Light.wakeup_active) {
            Light.wakeup_active = 1;
            for (uint32_t i = 0; i < Light.subtype; i++) {
              Light.new_color[i] = 0;
            }
            Light.wakeup_start_time = millis();
          }
          // which step are we in a range 0..1023
          uint32_t step_10 = ((millis() - Light.wakeup_start_time) * 1023) / (Settings->light_wakeup * 1000);
          if (step_10 > 1023) { step_10 = 1023; }   // sanity check
          uint8_t wakeup_bri = changeUIntScale(step_10, 0, 1023, 0, LightStateClass::DimmerToBri(Settings->light_dimmer));

          if (wakeup_bri != light_state.getBri()) {
            light_state.setBri(wakeup_bri);
            light_controller.calcLevels();
            for (uint32_t i = 0; i < Light.subtype; i++) {
              Light.new_color[i] = Light.current_color[i];
            }
          }
          if (1023 == step_10) {
            Response_P(PSTR("{\"" D_CMND_WAKEUP "\":\"" D_JSON_DONE "\""));
            ResponseLightState(1);
            ResponseJsonEnd();
            MqttPublishPrefixTopicRulesProcess_P(RESULT_OR_STAT, PSTR(D_CMND_WAKEUP));

            Light.wakeup_active = 0;
            LightSetScheme(LS_POWER);
          }
        }
        break;
      case LS_CYCLEUP:
      case LS_CYCLEDN:
      case LS_RANDOM:
        if (LS_CYCLEUP == Settings->light_scheme) {
          LightCycleColor(1);
        } else if (LS_CYCLEDN == Settings->light_scheme) {
          LightCycleColor(-1);
        } else {
          LightCycleColor(0);
        }
        if (Light.pwm_multi_channels) {     // See #8058
          Light.new_color[0] = changeUIntScale(Light.new_color[0], 0, 255, 0, Settings->light_color[0]);
          Light.new_color[1] = changeUIntScale(Light.new_color[1], 0, 255, 0, Settings->light_color[1]);
          Light.new_color[2] = changeUIntScale(Light.new_color[2], 0, 255, 0, Settings->light_color[2]);
        }
        break;
      default:
        XlgtCall(FUNC_SET_SCHEME);
    }

#ifdef USE_DEVICE_GROUPS
    if (Settings->light_scheme != Light.last_scheme) {
      Light.last_scheme = Settings->light_scheme;
      SendDeviceGroupMessage(Light.device, DGR_MSGTYP_UPDATE, DGR_ITEM_LIGHT_SCHEME, Settings->light_scheme);
      Light.devgrp_no_channels_out = false;
    }
#endif  // USE_DEVICE_GROUPS
  }

  if ((Settings->light_scheme < LS_MAX) || power_off) {  // exclude WS281X Neopixel schemes

    // Apply power modifiers to Light.new_color
    LightApplyPower(Light.new_color, Light.power);

    // AddLog(LOG_LEVEL_INFO, PSTR("last_color (%02X%02X%02X%02X%02X) new_color (%02X%02X%02X%02X%02X) power %d"),
    // Light.last_color[0], Light.last_color[1], Light.last_color[2], Light.last_color[3], Light.last_color[4],
    // Light.new_color[0], Light.new_color[1], Light.new_color[2], Light.new_color[3], Light.new_color[4],
    // Light.power
    // );

    if (memcmp(Light.last_color, Light.new_color, Light.subtype)) {
      Light.update = true;
    }
    if (Light.update) {
#ifdef USE_DEVICE_GROUPS
      if (Light.power && !Light.devgrp_no_channels_out) LightSendDeviceGroupStatus();
#endif  // USE_DEVICE_GROUPS

      uint16_t cur_col_10[LST_MAX];   // 10 bits resolution
      Light.update = false;

      // first set 8 and 10 bits channels
      for (uint32_t i = 0; i < LST_MAX; i++) {
        Light.last_color[i] = Light.new_color[i];
        // Extend from 8 to 10 bits if no correction (in case no gamma correction is required)
        cur_col_10[i] = change8to10(Light.new_color[i]);
      }

      bool rgbwwtable_applied_white = false;      // did we already applied RGBWWTable to white channels (ex: in white_blend_mode or virtual_ct)
      if (Light.pwm_multi_channels) {
        calcGammaMultiChannels(cur_col_10);
      } else {
        // AddLog(LOG_LEVEL_INFO, PSTR(">>> calcGammaBulbs In  %03X,%03X,%03X,%03X,%03X"), cur_col_10[0], cur_col_10[1], cur_col_10[2], cur_col_10[3], cur_col_10[4]);
        rgbwwtable_applied_white = calcGammaBulbs(cur_col_10);     // true means that one PWM channel is used for CT
        // AddLog(LOG_LEVEL_INFO, PSTR(">>> calcGammaBulbs Out %03X,%03X,%03X,%03X,%03X"), cur_col_10[0], cur_col_10[1], cur_col_10[2], cur_col_10[3], cur_col_10[4]);
      }

      // Apply RGBWWTable only if not Settings->flag4.white_blend_mode
      for (uint32_t i = 0; i < (rgbwwtable_applied_white ? 3 : Light.subtype); i++) {
        uint32_t adjust = change8to10(Settings->rgbwwTable[i]);
        cur_col_10[i] = changeUIntScale(cur_col_10[i], 0, 1023, 0, adjust);
      }

      // final adjusments for PMW ranges, post-gamma correction
      for (uint32_t i = 0; i < LST_MAX; i++) {
        // scale from 0..1023 to 0..pwm_range, but keep any non-zero value to at least 1
        cur_col_10[i] = (cur_col_10[i] > 0) ? changeUIntScale(cur_col_10[i], 1, 1023, 1, Settings->pwm_range) : 0;
      }

      // apply port remapping on both 8 bits and 10 bits versions
      uint16_t orig_col_10bits[LST_MAX];
      memcpy(orig_col_10bits, cur_col_10, sizeof(orig_col_10bits));
      for (uint32_t i = 0; i < LST_MAX; i++) {
        cur_col_10[i] = orig_col_10bits[Light.color_remap[i]];
      }

      if (!LightGetFadeSetting() || TasmotaGlobal.skip_light_fade || power_off || (!Light.fade_initialized)) { // no fade
        // record the current value for a future Fade
        memcpy(Light.fade_start_10, cur_col_10, sizeof(Light.fade_start_10));
        // push the final values at 8 and 10 bits resolution to the PWMs
        LightSetOutputs(cur_col_10);
        Light.fade_initialized = true;      // it is now ok to fade
        Light.fade_once_enabled = false;    // light has been set, reset fade_once_enabled
        Light.speed_once_enabled = false;   // light has been set, reset speed_once_enabled
      } else {  // fade on
        if (Light.fade_running) {
          // if fade is running, we take the curring value as the start for the next fade
          memcpy(Light.fade_start_10, Light.fade_cur_10, sizeof(Light.fade_start_10));
        }
        memcpy(Light.fade_end_10, cur_col_10, sizeof(Light.fade_start_10));
        Light.fade_running = true;
        Light.fade_duration = 0;    // set the value to zero to force a recompute
        Light.fade_start = 0;
        Light.fade_once_enabled = false;    // fade will be applied, reset fade_once_enabled
        // Fade will applied immediately below
      }
    }
    if (Light.fade_running) {
      if (LightApplyFade()) {
        // AddLog(LOG_LEVEL_INFO, PSTR("LightApplyFade %d %d %d %d %d"),
        //   Light.fade_cur_10[0], Light.fade_cur_10[1], Light.fade_cur_10[2], Light.fade_cur_10[3], Light.fade_cur_10[4]);

        LightSetOutputs(Light.fade_cur_10);
      }
    }
#ifdef USE_PWM_DIMMER
    // If the power is off and the fade is done, turn the relay off.
    if (PWM_DIMMER == TasmotaGlobal.module_type && !Light.power && !Light.fade_running) PWMDimmerSetPower();
#endif  // USE_PWM_DIMMER
    // For WYZE bulbs we must set the CT pin (PWM2) to INPUT to fully turn it off
    if (TasmotaGlobal.gpio_optiona.pwm1_input && !Light.power && !Light.fade_running) {  // GPIO Option_A1
      if (PinUsed(GPIO_PWM1, 1)) { pinMode(Pin(GPIO_PWM1, 1), INPUT); }
    }
  }
}

bool isChannelGammaCorrected(uint32_t channel) {
  if (!Settings->light_correction) { return false; }   // Gamma correction not activated
  if (channel >= Light.subtype) { return false; }     // Out of range
#ifdef ESP8266
  if ((PHILIPS == TasmotaGlobal.module_type) || (Settings->flag4.pwm_ct_mode)) {
    if ((LST_COLDWARM == Light.subtype) && (1 == channel)) { return false; }   // PMW reserved for CT
    if ((LST_RGBCW == Light.subtype) && (4 == channel)) { return false; }   // PMW reserved for CT
  }
#endif  // ESP8266
  return true;
}

// is the channel a regular PWM or ColorTemp control
bool isChannelCT(uint32_t channel) {
#ifdef ESP8266
  if ((PHILIPS == TasmotaGlobal.module_type) || (Settings->flag4.pwm_ct_mode)) {
    if ((LST_COLDWARM == Light.subtype) && (1 == channel)) { return true; }   // PMW reserved for CT
    if ((LST_RGBCW == Light.subtype) && (4 == channel)) { return true; }   // PMW reserved for CT
  }
#endif  // ESP8266
  return false;
}

// Calculate the Gamma correction, if any, for fading, using the fast Gamma curve (10 bits in+out)
uint16_t fadeGamma(uint32_t channel, uint16_t v) {
  if (isChannelGammaCorrected(channel)) {
    return ledGammaFast(v);
  } else {
    return v;
  }
}
uint16_t fadeGammaReverse(uint32_t channel, uint16_t vg) {
  if (isChannelGammaCorrected(channel)) {
    return leddGammaReverseFast(vg);
  } else {
    return vg;
  }
}

uint8_t LightGetCurFadeBri(void) {
  uint8_t max_bri = 0;
  uint8_t bri_i = 0;
  for (uint8_t i = 0; i < LST_MAX; i++) {
    bri_i = changeUIntScale(fadeGammaReverse(i, Light.fade_cur_10[i]), 4, 1023, 1, 100);
    if (bri_i > max_bri) max_bri = bri_i ;
  }
  return max_bri;
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
      int32_t channel_distance = fadeGammaReverse(i, Light.fade_end_10[i]) - fadeGammaReverse(i, Light.fade_start_10[i]);
      if (channel_distance < 0) { channel_distance = - channel_distance; }
      if (channel_distance > distance) { distance = channel_distance; }
    }
    if (distance > 0) {
      // compute the duration of the animation
      // Note: Settings->light_speed is the number of half-seconds for a 100% fade,
      // i.e. light_speed=1 means 1024 steps in 500ms
      Light.fade_duration = LightGetSpeedSetting() * 500;
      Light.speed_once_enabled = false; // The once off speed value has been read, reset it
      if (!Settings->flag5.fade_fixed_duration) {
        Light.fade_duration = (distance * Light.fade_duration) / 1023;    // time is proportional to distance, except with SO117
      }
      if (Settings->save_data) {
        // Also postpone the save_data for the duration of the Fade (in seconds)
        uint32_t delay_seconds = 1 + (Light.fade_duration + 999) / 1000;   // add one more second
        // AddLog(LOG_LEVEL_INFO, PSTR("delay_seconds %d, save_data_counter %d"), delay_seconds, TasmotaGlobal.save_data_counter);
        if (TasmotaGlobal.save_data_counter < delay_seconds) {
          TasmotaGlobal.save_data_counter = delay_seconds;      // pospone
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
      Light.fade_cur_10[i] = fadeGamma(i,
                                changeUIntScale(fadeGammaReverse(i, fade_current),
                                             0, Light.fade_duration,
                                             fadeGammaReverse(i, Light.fade_start_10[i]),
                                             fadeGammaReverse(i, Light.fade_end_10[i])));
      // Light.fade_cur_10[i] = changeUIntScale(fade_current,
      //                                        0, Light.fade_duration,
      //                                        Light.fade_start_10[i], Light.fade_end_10[i]);
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
    // AddLog(LOG_LEVEL_DEBUG_MORE, "Animate>> Light.power=%d Light.new_color=[%d,%d,%d,%d,%d]",
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
  if (TasmotaGlobal.light_type < LT_PWM6) {   // only for direct PWM lights, not for Tuya, Armtronix...
    for (uint32_t i = 0; i < (Light.subtype - Light.pwm_offset); i++) {
      if (PinUsed(GPIO_PWM1, i)) {
        //AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_APPLICATION "Cur_Col%d 10 bits %d"), i, cur_col_10[i]);
        uint16_t cur_col = cur_col_10[i + Light.pwm_offset];
        if (!isChannelCT(i)) {   // if CT don't use pwm_min and pwm_max
          cur_col = cur_col > 0 ? changeUIntScale(cur_col, 0, Settings->pwm_range, Light.pwm_min, Light.pwm_max) : 0;   // shrink to the range of pwm_min..pwm_max
        }
        if (!Settings->flag4.zerocross_dimmer) {
          analogWrite(Pin(GPIO_PWM1, i), bitRead(TasmotaGlobal.pwm_inverted, i) ? Settings->pwm_range - cur_col : cur_col);
        }
#ifdef USE_PWM_DIMMER
        // Animate brightness LEDs to follow PWM dimmer brightness
        if (PWM_DIMMER == TasmotaGlobal.module_type) PWMDimmerSetBrightnessLeds(change10to8(cur_col));
#endif  // USE_PWM_DIMMER
      }
    }
  }
//  char msg[24];
//  AddLog(LOG_LEVEL_DEBUG, PSTR("LGT: Channels %s"), ToHex_P((const unsigned char *)cur_col_10, 10, msg, sizeof(msg)));

  // Some devices need scaled RGB like Sonoff L1
  uint32_t max = (cur_col_10[0] > cur_col_10[1] && cur_col_10[0] > cur_col_10[2]) ? cur_col_10[0] : (cur_col_10[1] > cur_col_10[2]) ? cur_col_10[1] : cur_col_10[2];   // 0..1023
  uint8_t scale_col[3];
  for (uint32_t i = 0; i < 3; i++) {
    scale_col[i] = (0 == max) ? 255 : changeUIntScale(cur_col_10[i], 0, max, 0, 255);
  }
//  AddLog(LOG_LEVEL_DEBUG, PSTR("LGT: CurCol %03X %03X %03X, ScaleCol %02X %02X %02X, Max %02X"),
//    cur_col_10[0], cur_col_10[1], cur_col_10[2], scale_col[0], scale_col[1], scale_col[2], max);

  uint8_t cur_col[LST_MAX];
  for (uint32_t i = 0; i < LST_MAX; i++) {
    cur_col[i] = change10to8(cur_col_10[i]);
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

// Just apply basic Gamma to each channel
void calcGammaMultiChannels(uint16_t cur_col_10[5]) {
  // Apply gamma correction for 8 and 10 bits resolutions, if needed
  if (Settings->light_correction) {
    for (uint32_t i = 0; i < LST_MAX; i++) {
      cur_col_10[i] = ledGamma10_10(cur_col_10[i]);
    }
  }
}

//
// Compute the Gamma correction for CW/WW
// Can be used for 2-channels (channels 0,1) or 5 channels (channels 3,4)
//
// It is implicitly called by calcGammaBulb5Channels()
//
// In:
// - 2 channels CW/WW in 10 bits format (0..1023)
// Out:
// - 2 channels CW/WW in 10 bits format, with Gamma corretion (if enabled), replaced in place
// - white_bri10: global brightness of white channel, split over CW/WW (basically the sum of CW+WW, but it's easier to compute on this basis)
// - white_free_cw: signals that CW/WW are free mode, and not linked via CT. This is used when channels are manually set on a channel per channel basis. CT is ignored
//
void calcGammaBulbCW(uint16_t cw10[2], uint16_t *white_bri10_out, bool *white_free_cw_out) {
  uint16_t white_bri10 = cw10[0] + cw10[1];            // cumulated brightness
  bool white_free_cw = (white_bri10 > 1031);      // take a margin of 8 above 1023 to account for rounding errors
  white_bri10 = (white_bri10 > 1023) ? 1023 : white_bri10;    // max 1023

  if (Settings->light_correction) {
    if (white_free_cw) {
      cw10[0] = ledGamma10_10(cw10[0]);
      cw10[1] = ledGamma10_10(cw10[1]);
    } else {
      uint16_t white_bri10_gamma = ledGamma10_10(white_bri10);   // gamma corrected white
      // now distributed among both channels
      cw10[0] = changeUIntScale(cw10[0], 0, white_bri10, 0, white_bri10_gamma);
      cw10[1] = changeUIntScale(cw10[1], 0, white_bri10, 0, white_bri10_gamma);
      // now use white_bri10_gamma as a reference
      white_bri10 = white_bri10_gamma;
    }
  }
  if (white_bri10_out != nullptr) { *white_bri10_out = white_bri10; }
  if (white_free_cw_out != nullptr) { *white_free_cw_out = white_free_cw; }
}

//
// Calculate the gamma correction for all 5 channels RGBCW
// Computation is valid for 1,3,4,5 channels
// 2-channels bulbs must be handled separately
//
// In:
// - 5 channels RGBCW in 10 bits format (0..1023)
// Out:
// - 5 channels RGBCW in 10 bits format, with Gamma corretion (if enabled), replaced in place
// - white_bri10: global brightness of white channel, split over CW/WW (basically the sum of CW+WW, but it's easier to compute on this basis)
// - white_free_cw: signals that CW/WW are free mode, and not linked via CT. This is used when channels are manually set on a channel per channel basis. CT is ignored
//
void calcGammaBulb5Channels(uint16_t col10[LST_MAX], uint16_t *white_bri10_out, bool *white_free_cw) {
  for (uint32_t i = 0; i < 3; i++) {
    if (Settings->light_correction) {
      col10[i] = ledGamma10_10(col10[i]);
    }
  }
  calcGammaBulbCW(&col10[3], white_bri10_out, white_free_cw);
}

// sale but converts from 8 bits to 10 bits first
void calcGammaBulb5Channels_8(uint8_t in8[LST_MAX], uint16_t col10[LST_MAX]) {
  for (uint32_t i = 0; i < LST_MAX; i++) {
    col10[i] = change8to10(in8[i]);
  }
  calcGammaBulb5Channels(col10, nullptr, nullptr);
}

bool calcGammaBulbs(uint16_t cur_col_10[5]) {
  bool rgbwwtable_applied_white = false;
  bool pwm_ct = false;
  bool white_free_cw = false;         // true if White channels are uncorrelated. Happens when CW+WW>255, i.e. manually setting white channels to exceed to total power of a single channel (may harm the power supply)
  // Various values needed for accurate White calculation
  // CT value streteched to 0..1023 (from within CT range, so not necessarily from 153 to 500). 0=Cold, 1023=Warm
  uint16_t ct = light_state.getCT();
  uint16_t ct_10 = changeUIntScale(ct, Light.vct_ct[0], Light.vct_ct[CT_PIVOTS-1], 0, 1023);

  uint16_t white_bri10 = 0;           // White total brightness normalized to 0..1023
  // uint32_t cw1 = Light.subtype - 1;       // address for the ColorTone PWM
  uint32_t cw0 = Light.subtype - 2;       // address for the White Brightness PWM

  // calc basic gamma correction for all types
  if ((LST_SINGLE == Light.subtype) || (LST_RGB <= Light.subtype)) {
    calcGammaBulb5Channels(cur_col_10, &white_bri10, &white_free_cw);
  } else if (LST_COLDWARM == Light.subtype) {
    calcGammaBulbCW(cur_col_10, &white_bri10, &white_free_cw);
  }

  // Now we know ct_10 and white_bri10 (gamma corrected if needed)

#ifdef ESP8266
  if ((LST_COLDWARM == Light.subtype) || (LST_RGBCW == Light.subtype)) {
    if ((PHILIPS == TasmotaGlobal.module_type) || (Settings->flag4.pwm_ct_mode)) {   // channel 1 is the color tone, mapped to cold channel (0..255)
      pwm_ct = true;
      // Xiaomi Philips bulbs follow a different scheme:
      // channel 0=intensity, channel1=temperature
      cur_col_10[cw0] = white_bri10;
      cur_col_10[cw0+1] = ct_10;
      return false;     // avoid any interference
    }
  }
#endif  // ESP8266

  // Now see if we need to mix RGB and  White
  // Valid only for LST_RGBW, LST_RGBCW, SetOption105 1, and white is zero (see doc)
  if ((LST_RGBW <= Light.subtype) && (Settings->flag4.white_blend_mode) && (0 == cur_col_10[3]+cur_col_10[4])) {
    uint32_t min_rgb_10 = min3(cur_col_10[0], cur_col_10[1], cur_col_10[2]);
    cur_col_10[0] -= min_rgb_10;
    cur_col_10[1] -= min_rgb_10;
    cur_col_10[2] -= min_rgb_10;

    // Add to white level
    uint32_t adjust_w_10 = change8to10(Settings->rgbwwTable[3]);   // take the correction factor, bought back to 10 bits
    white_bri10 += changeUIntScale(min_rgb_10, 0, 1023, 0, adjust_w_10);  // set white power down corrected with rgbwwTable[3]
    white_bri10 = (white_bri10 > 1023) ? 1023 : white_bri10;    // max 1023
    rgbwwtable_applied_white = true;
  }

#ifdef USE_LIGHT_VIRTUAL_CT
  // compute virtual CT, which is suppsed to be compatible with white_blend_mode
  if (Light.virtual_ct && (!white_free_cw) && (LST_RGBW <= Light.subtype)) {        // any light with a white channel
    vct_pivot_t   *pivot = &Light.vct_color[0];
    uint16_t      *from_ct = &Light.vct_ct[0];

    for (uint32_t i = 1; i < CT_PIVOTS-1; i++) {
      if (ct > Light.vct_ct[i]) {     // if above mid-point, take range [1]..[2] instead of [0]..[1]
        pivot++;
        from_ct++;
      }
    }
    uint16_t from10[LST_MAX];
    uint16_t to10[LST_MAX];
    calcGammaBulb5Channels_8(*pivot, from10);
    calcGammaBulb5Channels_8(*(pivot+1), to10);

    vct_pivot_t   *pivot1 = pivot + 1;
    // AddLog(LOG_LEVEL_INFO, PSTR("+++ from_ct %d, to_ct %d [%03X,%03X,%03X,%03X,%03X] - [%03X,%03X,%03X,%03X,%03X]"),
    //           *from_ct, *(from_ct+1), (*pivot)[0], (*pivot)[1], (*pivot)[2], (*pivot)[3], (*pivot)[4],
    //           (*pivot1)[0], (*pivot1)[1], (*pivot1)[2], (*pivot1)[3], (*pivot1)[4]);
    // AddLog(LOG_LEVEL_INFO, PSTR("+++ from10 [%03X,%03X,%03X,%03X,%03X] - to 10 [%03X,%03X,%03X,%03X,%03X]"),
    //           from10[0],from10[0],from10[0],from10[0],from10[4],
    //           to10[0],to10[0],to10[0],to10[0],to10[4]);

    // set both CW/WW to zero since their previous value don't count anymore
    cur_col_10[3] = 0;
    cur_col_10[4] = 0;

    // Add the interpolated point to each component
    for (uint32_t i = 0; i < LST_MAX; i++) {
      cur_col_10[i] += changeUIntScale(changeUIntScale(ct, *from_ct, *(from_ct+1), from10[i], to10[i]), 0, 1023, 0, white_bri10);
      if (cur_col_10[i] > 1023) { cur_col_10[i] = 1023; }
    }
  } else
#endif // USE_LIGHT_VIRTUAL_CT
  // compute the actual levels for CW/WW
  // We know ct_10 and white_bri_10 (which may be Gamma corrected)
  // cur_col_10[cw0] and cur_col_10[cw1] were unmodified up to now
  if (LST_RGBW == Light.subtype) {
    cur_col_10[3] = white_bri10;       // simple case, we set the White level to the required brightness
  } else if ((LST_COLDWARM == Light.subtype) || (LST_RGBCW == Light.subtype)) {
    // if sum of both channels is > 255, then channels are probably uncorrelated
    if (!white_free_cw) {
      // then we split the total energy among the cold and warm leds
      cur_col_10[cw0+1] = changeUIntScale(ct_10, 0, 1023, 0, white_bri10);
      cur_col_10[cw0] = white_bri10 - cur_col_10[cw0+1];
    }
  }
  return rgbwwtable_applied_white;
}

#ifdef USE_DEVICE_GROUPS
void LightSendDeviceGroupStatus()
{
  static uint8_t last_bri;
  uint8_t bri = light_state.getBri();
  bool send_bri_update = (building_status_message || bri != last_bri);
  if (Light.subtype > LST_SINGLE) {
    static uint8_t last_channels[LST_MAX + 1] = { 0, 0, 0, 0, 0, 0 };
    uint8_t channels[LST_MAX];

    light_state.getChannelsRaw(channels);
    uint8_t color_mode = light_state.getColorMode();
    if (!(color_mode & LCM_RGB)) channels[0] = channels[1] = channels[2] = 0;
    if (!(color_mode & LCM_CT)) channels[3] = channels[4] = 0;
    if (building_status_message || memcmp(channels, last_channels, LST_MAX)) {
      memcpy(last_channels, channels, LST_MAX);
      last_channels[LST_MAX]++;
      SendDeviceGroupMessage(Light.device, (send_bri_update ? DGR_MSGTYP_PARTIAL_UPDATE : DGR_MSGTYP_UPDATE), DGR_ITEM_LIGHT_CHANNELS, last_channels);
    }
  }
  if (send_bri_update) {
    last_bri = bri;
    SendDeviceGroupMessage(Light.device, DGR_MSGTYP_UPDATE, DGR_ITEM_LIGHT_BRI, light_state.getBri());
  }
}

void LightHandleDevGroupItem(void)
{
  static bool send_state = false;
  static bool restore_power = false;

  if (Settings->flag4.multiple_device_groups ? Settings->device_group_tie[*XdrvMailbox.topic] != Light.device : !(XdrvMailbox.index & DGR_FLAG_LOCAL)) return;
  bool more_to_come;
  uint32_t value = XdrvMailbox.payload;
  switch (XdrvMailbox.command_code) {
    case DGR_ITEM_EOL:
      more_to_come = (XdrvMailbox.index & DGR_FLAG_MORE_TO_COME);
      if (more_to_come) {
        TasmotaGlobal.skip_light_fade = true;
      }
      else if (restore_power) {
        restore_power = false;
        Light.power = Light.old_power;
      }

      LightAnimate();

      TasmotaGlobal.skip_light_fade = false;
      if (send_state && !more_to_come) {
        light_controller.saveSettings();
        if (Settings->flag3.hass_tele_on_power) {  // SetOption59 - Send tele/%topic%/STATE in addition to stat/%topic%/RESULT
          MqttPublishTeleState();
        }
        send_state = false;
      }
      break;
    case DGR_ITEM_LIGHT_BRI:
      if (light_state.getBri() != value) {
        light_state.setBri(value);
        Settings->light_dimmer = light_state.BriToDimmer(value);
        send_state = true;
      }
      break;
    case DGR_ITEM_LIGHT_SCHEME:
      if (Settings->light_scheme != value) {
        Light.last_scheme = Settings->light_scheme = value;
        Light.devgrp_no_channels_out = (value != 0);
        send_state = true;
      }
      break;
    case DGR_ITEM_LIGHT_CHANNELS:
      {
        uint8_t bri = light_state.getBri();
#ifdef USE_DGR_LIGHT_SEQUENCE
        static uint8_t last_sequence = 0;

        // If a sequence offset is set, set the channels to the ones we received <SequenceOffset>
        // changes ago.
        if (Light.sequence_offset) {
          light_controller.changeChannels(Light.channels_fifo, true);

          // Shift the fifo down and load the newly received channels at the end for this update and
          // any updates we missed.
          int last_entry = (Light.sequence_offset - 1) * LST_MAX;
          for (uint8_t sequence = (uint8_t)XdrvMailbox.data[LST_MAX]; (uint8_t)(sequence - last_sequence) > 0; last_sequence++) {
            memmove(Light.channels_fifo, &Light.channels_fifo[LST_MAX], last_entry);
            memcpy(&Light.channels_fifo[last_entry], XdrvMailbox.data, LST_MAX);
          }
        }
        else
#endif  // USE_DGR_LIGHT_SEQUENCE
          light_controller.changeChannels((uint8_t *)XdrvMailbox.data, true);
        light_controller.changeBri(bri);
      }
      send_state = true;
      break;
    case DGR_ITEM_LIGHT_FIXED_COLOR:
      if (Light.subtype >= LST_COLDWARM) {
        send_state = true;
#ifdef USE_LIGHT_PALETTE
        if (Light.palette_count) {
          Light.wheel = value % Light.palette_count;
          LightSetPaletteEntry();
          break;
        }
#endif  // !USE_LIGHT_PALETTE
        if (Light.subtype <= LST_COLDWARM) {
          value = value % (MAX_FIXED_COLD_WARM - 1) + 201;
        }
        else {
          uint32_t max = MAX_FIXED_COLOR;
          if (Light.subtype >= LST_RGB) {
            max++;
            if (Light.subtype >= LST_RGBCW) max += (MAX_FIXED_COLD_WARM - 2);
          }
          value = value % max + 1;
          if (value > MAX_FIXED_COLOR) value += 200 - MAX_FIXED_COLOR;
        }
        Light.fixed_color_index = value;
        bool save_decimal_text = Settings->flag.decimal_text;
        char str[16];
        LightColorEntry(str, sprintf_P(str, PSTR("%u"), value));
        Settings->flag.decimal_text = save_decimal_text;
        uint32_t old_bri = light_state.getBri();
        light_controller.changeChannels(Light.entry_color);
        light_controller.changeBri(old_bri);
        LightSetScheme(LS_POWER);
        if (!restore_power && !Light.power) {
          Light.old_power = Light.power;
          Light.power = 0xff;
          restore_power = true;
        }
      }
      break;
    case DGR_ITEM_LIGHT_FADE:
      if (Settings->light_fade != value) {
        Settings->light_fade = value;
        send_state = true;
      }
      break;
    case DGR_ITEM_LIGHT_SPEED:
      if (Settings->light_speed != value && value > 0 && value <= 40) {
        Settings->light_speed = value;
        send_state = true;
      }
      break;
    case DGR_ITEM_STATUS:
      SendDeviceGroupMessage(Light.device, DGR_MSGTYP_PARTIAL_UPDATE, DGR_ITEM_LIGHT_FADE, Settings->light_fade,
        DGR_ITEM_LIGHT_SPEED, Settings->light_speed, DGR_ITEM_LIGHT_SCHEME, Settings->light_scheme);
      LightSendDeviceGroupStatus();
      break;
  }
}
#endif  // USE_DEVICE_GROUPS

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
#ifdef USE_LIGHT_PALETTE
  if (Light.palette_count) value = Light.wheel;
#endif  // USE_LIGHT_PALETTE

  if (buffer[0] == '#') {                           // Optional hexadecimal entry
    buffer++;
    buffer_length--;
  }

  if (Light.subtype >= LST_RGB) {
    char option = (1 == buffer_length) ? buffer[0] : '\0';
    if ('+' == option) {
#ifdef USE_LIGHT_PALETTE
      if (Light.palette_count || Light.fixed_color_index < MAX_FIXED_COLOR) {
#else // USE_LIGHT_PALETTE
      if (Light.fixed_color_index < MAX_FIXED_COLOR) {
#endif  // !USE_LIGHT_PALETTE
        value++;
      }
    }
    else if ('-' == option) {
#ifdef USE_LIGHT_PALETTE
      if (Light.palette_count || Light.fixed_color_index > 1) {
#else // USE_LIGHT_PALETTE
      if (Light.fixed_color_index > 1) {
#endif  // !USE_LIGHT_PALETTE
        value--;
      }
    } else {
      value = atoi(buffer);
    }
#ifdef USE_LIGHT_PALETTE
    if (Light.palette_count) value = value % Light.palette_count;
#endif  // USE_LIGHT_PALETTE
  }

  memset(&Light.entry_color, 0x00, sizeof(Light.entry_color));
  // erase all channels except if the last character is '=', #6799
  while ((buffer_length > 0) && ('=' == buffer[buffer_length - 1])) {
    buffer_length--;  // remove all trailing '='
    memcpy(&Light.entry_color, &Light.current_color, sizeof(Light.entry_color));
  }
  if (strchr(buffer, ',') != nullptr) {             // Decimal entry
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
#ifdef USE_LIGHT_PALETTE
  else if (Light.palette_count) {
    Light.wheel = value;
    memcpy_P(&Light.entry_color, &Light.palette[value * Light.subtype], Light.subtype);
    entry_type = 1;                                 // Hexadecimal
  }
#endif  // USE_LIGHT_PALETTE
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
    else if (LST_RGBCW == Light.subtype) {
      memcpy_P(&Light.entry_color[3], &kFixedColdWarm[value -200], 2);
      entry_type = 1;                               // Hexadecimal
    }
  }

//  Too much magic so removed since 9.0.0.3
//  if (entry_type) {
//    Settings->flag.decimal_text = entry_type -1;     // SetOption17 - Switch between decimal or hexadecimal output
//  }

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
#ifdef USE_LIGHT_PALETTE
        if (Light.palette_count && XdrvMailbox.index == 2) {
          LightSetPaletteEntry();
        }
        else {
#endif  // USE_LIGHT_PALETTE
          uint32_t old_bri = light_state.getBri();
          uint32_t old_bri_rgb = light_state.getBriRGB();
          // change all channels to specified values
          light_controller.changeChannels(Light.entry_color);
          if (2 == XdrvMailbox.index) {
            // If Color2, set back old brightness
            if (light_controller.isCTRGBLinked()) {
              // RGB and white are linked, adjust brightness of all channels
              LightSetBriScaled(old_bri);
            } else {
              // RGB and white are unlinked, adjust brightness only of RGB channels
              LightSetBri(Light.device, old_bri_rgb);
            }
          }
#ifdef USE_LIGHT_PALETTE
        }
#endif  // USE_LIGHT_PALETTE
        LightSetScheme(LS_POWER);
        coldim = true;
      } else {             // Color3, 4, 5 and 6
        for (uint32_t i = 0; i < LST_RGB; i++) {
          Settings->ws_color[XdrvMailbox.index -3][i] = Light.entry_color[i];
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
      if (Settings->flag.decimal_text) {  // SetOption17 - Switch between decimal or hexadecimal output
        snprintf_P(scolor, sizeof(scolor), PSTR("%s%s%d"), scolor, (i > 0) ? "," : "", Settings->ws_color[XdrvMailbox.index -3][i]);
      } else {
        snprintf_P(scolor, sizeof(scolor), PSTR("%s%02X"), scolor, Settings->ws_color[XdrvMailbox.index -3][i]);
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
  // Color  - Show current RGBWW color state
  // Color1 - Change color to RGBWW
  // Color2 - Change color to RGBWW but retain brightness (=dimmer)
  // Color3 - Change color to RGB of WS2812 Clock Second
  // Color4 - Change color to RGB of WS2812 Clock Minute
  // Color5 - Change color to RGB of WS2812 Clock Hour
  // Color6 - Change color to RGB of WS2812 Clock Marker
  if ((Light.subtype > LST_SINGLE) && (XdrvMailbox.index > 0) && (XdrvMailbox.index <= 6)) {
    CmndSupportColor();
  }
}

void CmndWhite(void)
{
  // White        - Show current White (=Dimmer2) state
  // White 0..100 - Set White colors dimmer state
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
  // Channel<x>        - Show current Channel state
  // Channel<x> 0..100 - Set Channel dimmer state
  // Channel<x> +      - Incerement Channel in steps of 10
  // Channel<x> -      - Decrement Channel in steps of 10
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
  // HsbColor             - Show current HSB
  // HsbColor 360,100,100 - Set Hue, Saturation and Brighthness
  // HsbColor 360,100     - Set Hue and Saturation
  // HsbColor 360         - Set Hue
  // HsbColor1 360        - Set Hue
  // HsbColor2 100        - Set Saturation
  // HsbColor3 100        - Set Brightness
  if (Light.subtype >= LST_RGB) {
    if (XdrvMailbox.data_len > 0) {
      uint16_t c_hue;
      uint8_t  c_sat;
      light_state.getHSB(&c_hue, &c_sat, nullptr);
      uint32_t HSB[3];
      HSB[0] = c_hue;
      HSB[1] = c_sat;
      HSB[2] = light_state.getBriRGB();
      if ((2 == XdrvMailbox.index) || (3 == XdrvMailbox.index)) {
        if ((uint32_t)XdrvMailbox.payload > 100) { XdrvMailbox.payload = 100; }
        HSB[XdrvMailbox.index-1] = changeUIntScale(XdrvMailbox.payload, 0, 100, 0, 255);
      } else {
        uint32_t paramcount = ParseParameters(3, HSB);
        if (HSB[0] > 360) { HSB[0] = 360; }
        for (uint32_t i = 1; i < paramcount; i++) {
          if (HSB[i] > 100) { HSB[i] = 100; }
          HSB[i] = changeUIntScale(HSB[i], 0, 100, 0, 255); // change sat and bri to 0..255
        }
      }
      light_controller.changeHSB(HSB[0], HSB[1], HSB[2]);
      LightPreparePower(1);
    } else {
      ResponseLightState(0);
    }
  }
}

void CmndScheme(void)
{
  // Scheme 0..12  - Select one of schemes 0 to 12
  // Scheme 2      - Select scheme 2
  // Scheme 2,0    - Select scheme 2 with color wheel set to 0 (HSB Red)
  // Scheme +      - Select next scheme
  // Scheme -      - Select previous scheme
  if (Light.subtype >= LST_RGB) {
    uint32_t max_scheme = Light.max_scheme;

    if (1 == XdrvMailbox.data_len) {
      if (('+' == XdrvMailbox.data[0]) && (Settings->light_scheme < max_scheme)) {
        XdrvMailbox.payload = Settings->light_scheme + ((0 == Settings->light_scheme) ? 2 : 1);  // Skip wakeup
      }
      else if (('-' == XdrvMailbox.data[0]) && (Settings->light_scheme > 0)) {
        XdrvMailbox.payload = Settings->light_scheme - ((2 == Settings->light_scheme) ? 2 : 1);  // Skip wakeup
      }
    }
    if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= max_scheme)) {
      uint32_t parm[2];
      if (ParseParameters(2, parm) > 1) {
        Light.wheel = parm[1];
#ifdef USE_LIGHT_PALETTE
        Light.wheel--;
#endif  // USE_LIGHT_PALETTE
      }
      LightSetScheme(XdrvMailbox.payload);
      if (LS_WAKEUP == Settings->light_scheme) {
        Light.wakeup_active = 3;
      }
      LightPowerOn();
      Light.strip_timer_counter = 0;
      // Publish state message for Hass
      if (Settings->flag3.hass_tele_on_power) {  // SetOption59 - Send tele/%topic%/STATE in addition to stat/%topic%/RESULT
        MqttPublishTeleState();
      }
    }
    ResponseCmndNumber(Settings->light_scheme);
  }
}

void CmndWakeup(void)
{
  // Wakeup        - Start wakeup light
  // Wakeup 0..100 - Start wakeup light to dimmer value 0..100
  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 100)) {
    light_controller.changeDimmer(XdrvMailbox.payload);
  }
  Light.wakeup_active = 3;
  LightSetScheme(LS_WAKEUP);
  LightPowerOn();
  ResponseCmndChar(PSTR(D_JSON_STARTED));
}

void CmndColorTemperature(void)
{
  // CT          - Show current color temperature
  // CT 153..500 - Set color temperature
  // CT +        - Incerement color temperature in steps of 34
  // CT -        - Decrement color temperature in steps of 34
  if (Light.pwm_multi_channels) { return; }
  if ((LST_COLDWARM == Light.subtype) || (LST_RGBCW == Light.subtype)) { // ColorTemp
    uint32_t ct = light_state.getCT();
    if (1 == XdrvMailbox.data_len) {
      if ('+' == XdrvMailbox.data[0]) {
        XdrvMailbox.payload = (ct > (CT_MAX-34)) ? CT_MAX : ct + 34;
      }
      else if ('-' == XdrvMailbox.data[0]) {
        XdrvMailbox.payload = (ct < (CT_MIN+34)) ? CT_MIN : ct - 34;
      }
    }
    if ((XdrvMailbox.payload >= CT_MIN) && (XdrvMailbox.payload <= CT_MAX)) {  // https://developers.meethue.com/documentation/core-concepts
      light_controller.changeCTB(XdrvMailbox.payload, light_state.getBriCT());
      LightPreparePower(2);
    } else {
      ResponseCmndNumber(ct);
    }
  }
}

void LightDimmerOffset(uint32_t index, int32_t offset) {
  int32_t dimmer = light_state.getDimmer(index) + offset;
  if (dimmer < 1) { dimmer = Settings->flag3.slider_dimmer_stay_on; }  // SetOption77 - Do not power off if slider moved to far left
  if (dimmer > 100) { dimmer = 100; }

  XdrvMailbox.index = index;
  XdrvMailbox.payload = dimmer;
  CmndDimmer();
}

void CmndDimmer(void)
{
  // Dimmer         - Show current Dimmer state
  // Dimmer0 0..100 - Change both RGB and W(W) Dimmers
  // Dimmer1 0..100 - Change RGB Dimmer
  // Dimmer2 0..100 - Change W(W) Dimmer
  // Dimmer3 0..100 - Change both RGB and W(W) Dimmers with no fading
  // Dimmer<x> +    - Incerement Dimmer in steps of DimmerStep
  // Dimmer<x> -    - Decrement Dimmer in steps of DimmerStep
  uint32_t dimmer;
  if (XdrvMailbox.index == 3) {
    TasmotaGlobal.skip_light_fade = true;
    XdrvMailbox.index = 0;
  }
  else if (XdrvMailbox.index > 4) {
    XdrvMailbox.index = 1;
  }

  if ((light_controller.isCTRGBLinked()) || (0 == XdrvMailbox.index)) {
    dimmer = light_state.getDimmer();
  } else {
    dimmer = light_state.getDimmer(XdrvMailbox.index);
  }
  // Handle +/-/!/</> special commands
  if (1 == XdrvMailbox.data_len) {
    if ('+' == XdrvMailbox.data[0]) {
      XdrvMailbox.payload = (dimmer > (100 - Settings->dimmer_step - 1)) ? 100 : dimmer + Settings->dimmer_step;
    } else if ('-' == XdrvMailbox.data[0]) {
      XdrvMailbox.payload = (dimmer < (Settings->dimmer_step + 1)) ? 1 : dimmer - Settings->dimmer_step;
    } else if ('!' == XdrvMailbox.data[0] && Light.fade_running) {
      XdrvMailbox.payload = LightGetCurFadeBri();
    } else if ('<' == XdrvMailbox.data[0] ) {
      XdrvMailbox.payload = 1;
    } else if ('>' == XdrvMailbox.data[0] ) {
      XdrvMailbox.payload = 100;
    }
  }
  // If value is ok, change it, otherwise report old value
  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 100)) {
    if (light_controller.isCTRGBLinked()) {
      // normal state, linked RGB and CW
      if (4 == XdrvMailbox.index) {
        // change both dimmers, retain ratio between white and color channels
        uint32_t new_bri = changeUIntScale(XdrvMailbox.payload, 0, 100, 0, 255);
        LightSetBriScaled(new_bri);
      } else {
        // change both dimmers
        light_controller.changeDimmer(XdrvMailbox.payload);
      }
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
#if defined(USE_PWM_DIMMER) && defined(USE_DEVICE_GROUPS)
    uint8_t bri = light_state.getBri();
    if (bri != Settings->bri_power_on) {
      Settings->bri_power_on = bri;
      SendDeviceGroupMessage(Light.device, DGR_MSGTYP_PARTIAL_UPDATE, DGR_ITEM_BRI_POWER_ON, Settings->bri_power_on);
    }
#endif  // USE_PWM_DIMMER && USE_DEVICE_GROUPS
    Light.update = true;
    if (TasmotaGlobal.skip_light_fade) LightAnimate();
  } else {
    ResponseCmndNumber(dimmer);
  }
  TasmotaGlobal.skip_light_fade = false;
}

void CmndDimmerRange(void)
{
  // DimmerRange       - Show current dimmer range as used by Tuya and PS16DZ Dimmers
  // DimmerRange 0,100 - Set dimmer hardware range from 0 to 100 and restart
  if (XdrvMailbox.data_len > 0) {
    uint32_t parm[2];
    parm[0] = Settings->dimmer_hw_min;
    parm[1] = Settings->dimmer_hw_max;
    ParseParameters(2, parm);
    if (parm[0] < parm[1]) {
      Settings->dimmer_hw_min = parm[0];
      Settings->dimmer_hw_max = parm[1];
    } else {
      Settings->dimmer_hw_min = parm[1];
      Settings->dimmer_hw_max = parm[0];
    }
    LightCalcPWMRange();
    Light.update = true;
  }
  Response_P(PSTR("{\"" D_CMND_DIMMER_RANGE "\":{\"Min\":%d,\"Max\":%d}}"), Settings->dimmer_hw_min, Settings->dimmer_hw_max);
}

void CmndDimmerStep(void)
{
  // DimmerStep       - Show current dimmer step as used by Dimmer +/-
  // DimmerStep 1..50 - Set dimmer step
  if (XdrvMailbox.data_len > 0) {
    if (XdrvMailbox.payload < 1) {
       Settings->dimmer_step = 1;
    } else if (XdrvMailbox.payload > 50) {
      Settings->dimmer_step = 50;
    } else {
      Settings->dimmer_step = XdrvMailbox.payload;
    }
  }
  ResponseCmndNumber(Settings->dimmer_step);
}

void CmndLedTable(void)
{
  // LedTable        - Show current LedTable state
  // LedTable 0      - Turn LedTable Off
  // LedTable On     - Turn LedTable On
  // LedTable Toggle - Toggle LedTable state
  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 2)) {
    switch (XdrvMailbox.payload) {
    case 0: // Off
    case 1: // On
      Settings->light_correction = XdrvMailbox.payload;
      break;
    case 2: // Toggle
      Settings->light_correction ^= 1;
      break;
    }
    LightCalcPWMRange();
    Light.update = true;
  }
  ResponseCmndStateText(Settings->light_correction);
}

void CmndRgbwwTable(void)
{
  // RgbWwTable                     - Show current RGBWW State
  // RgbWwTable 255,255,255,255,255 - Set RGBWW state to maximum
  if ((XdrvMailbox.data_len > 0)) {
    uint32_t parm[LST_RGBCW -1];
    uint32_t parmcount = ParseParameters(LST_RGBCW, parm);
    for (uint32_t i = 0; i < parmcount; i++) {
      Settings->rgbwwTable[i] = parm[i];
    }
    Light.update = true;
  }
  char scolor[LIGHT_COLOR_SIZE];
  scolor[0] = '\0';
  for (uint32_t i = 0; i < LST_RGBCW; i++) {
    snprintf_P(scolor, sizeof(scolor), PSTR("%s%s%d"), scolor, (i > 0) ? "," : "", Settings->rgbwwTable[i]);
  }
  ResponseCmndChar(scolor);
}

void CmndFade(void)
{
  if (2 == XdrvMailbox.index) {
    // Home Assistant backwards compatibility, can be removed mid 2021
  } else {
    // Fade        - Show current Fade state
    // Fade 0      - Turn Fade Off
    // Fade On     - Turn Fade On
    // Fade Toggle - Toggle Fade state
    switch (XdrvMailbox.payload) {
    case 0: // Off
    case 1: // On
      Settings->light_fade = XdrvMailbox.payload;
      break;
    case 2: // Toggle
      Settings->light_fade ^= 1;
      break;
    }
  #ifdef USE_DEVICE_GROUPS
    if (XdrvMailbox.payload >= 0 && XdrvMailbox.payload <= 2) SendDeviceGroupMessage(Light.device, DGR_MSGTYP_UPDATE, DGR_ITEM_LIGHT_FADE, Settings->light_fade);
  #endif  // USE_DEVICE_GROUPS
    if (!Settings->light_fade) { Light.fade_running = false; }
  }
  ResponseCmndStateText(Settings->light_fade);
}

void CmndSpeed(void)
{
  if (2 == XdrvMailbox.index) {
    // Speed2 setting will be used only once, then revert to fade/speed
    if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 40)) {
      Light.fade_once_enabled = true;
      Light.fade_once_value = (XdrvMailbox.payload > 0);
      Light.speed_once_enabled = true;
      Light.speed_once_value = XdrvMailbox.payload;
      if (!Light.fade_once_value) { Light.fade_running = false; }
    }
    ResponseCmndIdxNumber(Light.speed_once_value);
  } else {
    // Speed 1  - Fast
    // Speed 40 - Very slow
    // Speed +  - Increment Speed
    // Speed -  - Decrement Speed
    if (1 == XdrvMailbox.data_len) {
      if (('+' == XdrvMailbox.data[0]) && (Settings->light_speed > 1)) {
        XdrvMailbox.payload = Settings->light_speed - 1;
      }
      else if (('-' == XdrvMailbox.data[0]) && (Settings->light_speed < 40)) {
        XdrvMailbox.payload = Settings->light_speed + 1;
      }
    }
    if ((XdrvMailbox.payload > 0) && (XdrvMailbox.payload <= 40)) {
      Settings->light_speed = XdrvMailbox.payload;
#ifdef USE_DEVICE_GROUPS
      SendDeviceGroupMessage(Light.device, DGR_MSGTYP_UPDATE, DGR_ITEM_LIGHT_SPEED, Settings->light_speed);
#endif  // USE_DEVICE_GROUPS
    }
    ResponseCmndNumber(Settings->light_speed);
  }
}

void CmndWakeupDuration(void)
{
  // WakeUpDuration    - Show current Wake Up duration in seconds
  // WakeUpDuration 60 - Set Wake Up duration to 60 seconds
  if ((XdrvMailbox.payload > 0) && (XdrvMailbox.payload < 3001)) {
    Settings->light_wakeup = XdrvMailbox.payload;
    Light.wakeup_active = 0;
  }
  ResponseCmndNumber(Settings->light_wakeup);
}

void CmndCTRange(void)
{
  // Format is "CTRange ctmin,ctmax"
  // Ex:
  // CTRange 153,500
  // CTRange
  // CTRange 200,350
  char *p;
  strtok_r(XdrvMailbox.data, ",", &p);
  if (p != nullptr) {
    int32_t ct_min = strtol(XdrvMailbox.data, nullptr, 0);
    int32_t ct_max = strtol(p, nullptr, 0);
    if ( (ct_min >= CT_MIN) && (ct_min <= CT_MAX) &&
         (ct_max >= CT_MIN) && (ct_max <= CT_MAX) &&
         (ct_min <= ct_max)
        ) {
      setCTRange(ct_min, ct_max);
    } else {
      return;     // error
    }
  }
  Response_P(PSTR("{\"%s\":\"%d,%d\"}"), XdrvMailbox.command, Light.vct_ct[0], Light.vct_ct[CT_PIVOTS-1]);
}

#ifdef USE_LIGHT_VIRTUAL_CT
void CmndVirtualCT(void)
{
  if (!Settings->flag4.virtual_ct) {
    ResponseCmndChar_P(PSTR("You need to enable `SetOption106 1`"));
    return;
  }
  if (XdrvMailbox.data[0] == ('{')) {
    // parse JSON
    JsonParser parser(XdrvMailbox.data);
    JsonParserObject root = parser.getRootObject();
    if (!root) { return; }

    uint32_t idx = 0;
    for (auto key : root) {
      if (idx >= CT_PIVOTS) { ResponseCmndChar_P(PSTR("Too many points")); return; }

      int32_t ct_val = strtol(key.getStr(), nullptr, 0);
      if ((ct_val < CT_MIN) || (ct_val > CT_MAX)) { ResponseCmndChar_P(PSTR("CT out of range")); return; }
      char * color = (char*) key.getValue().getStr();
      // call color parser
      Light.vct_ct[idx] = ct_val;
      if (LightColorEntry(color, strlen(color))) {
        memcpy(&Light.vct_color[idx], Light.entry_color, sizeof(Light.vct_color[idx]));
      }
      idx++;
    }
    for (uint32_t i = idx-1; i < CT_PIVOTS-1; i++) {
      Light.vct_ct[i+1] = Light.vct_ct[i];
      memcpy(&Light.vct_color[i+1], &Light.vct_color[i], sizeof(Light.vct_color[0]));
    }
  }
  checkVirtualCT();

  Response_P(PSTR("{\"%s\":{"), XdrvMailbox.command);
  uint32_t pivot_len = CT_PIVOTS;
  vct_pivot_t * pivot = &Light.vct_color[0];
  if (Light.vct_ct[1] >= Light.vct_ct[2]) { pivot_len = 2; }    // only 2 points are valid

  bool end = false;
  for (uint32_t i = 0; (i < CT_PIVOTS) && !end; i++) {
    if ((i >= CT_PIVOTS-1) || (Light.vct_ct[i] >= Light.vct_ct[i+1])) {
      end = true;
    }
    ResponseAppend_P(PSTR("\"%d\":\"%02X%02X%02X%02X%02X\"%c"), Light.vct_ct[i],
          (*pivot)[0], (*pivot)[1], (*pivot)[2], (*pivot)[3], (*pivot)[4],
          end ? '}' : ',');
    pivot++;
  }
  ResponseJsonEnd();
}
#endif // USE_LIGHT_VIRTUAL_CT

#ifdef USE_LIGHT_PALETTE
void CmndPalette(void)
{
  uint8_t * palette_entry;
  char * p;

  // Palette Color[ ...]
  if (XdrvMailbox.data_len) {
    Light.wheel = 0;
    Light.palette_count = 0;
    if (Light.palette) {
      free(Light.palette);
      Light.palette = nullptr;
    }
    if (XdrvMailbox.data_len > 1 || XdrvMailbox.data[0] != '0') {
      uint8_t palette_count = 0;
      char * color = XdrvMailbox.data;
      if (!(Light.palette = (uint8_t *)malloc(255 * Light.subtype))) return;
      palette_entry = Light.palette;
      for (;;) {
        p = strchr(color, ' ');
        if (p) *p = 0;
        color = Trim(color);
        if (*color && LightColorEntry(color, strlen(color))) {
          memcpy(palette_entry, Light.entry_color, Light.subtype);
          palette_entry += Light.subtype;
          palette_count++;
        }
        if (!p) break;
        color = p + 1;
      }
      if (!(Light.palette = (uint8_t *)realloc(Light.palette, palette_count * Light.subtype))) return;
      Light.palette_count = palette_count;
    }
  }

  char palette_str[5 * Light.subtype * Light.palette_count + 3];
  p = palette_str;
  *p++ = '[';
  if (Light.palette_count) {
    palette_entry = Light.palette;
    for (int entry = 0; entry < Light.palette_count; entry++) {
      if (Settings->flag.decimal_text) {  // SetOption17 - Switch between decimal or hexadecimal output
        *p++ = '"';
        for (uint32_t i = 0; i < Light.subtype; i++) {
          p += sprintf_P(p, PSTR("%d,"), *palette_entry++);
        }
        *(p - 1) = '"';
      }
      else {
        for (uint32_t i = 0; i < Light.subtype; i++) {
          p += sprintf_P(p, PSTR("%02X"), *palette_entry++);
        }
      }
      *p++ = ',';
    }
    p--;
  }
  *p++ = ']';
  *p = 0;
  ResponseCmndChar(palette_str);
}
#endif  // USE_LIGHT_PALETTE

#ifdef USE_DGR_LIGHT_SEQUENCE
void CmndSequenceOffset(void)
{
  // SequenceOffset<x> <offset>, x: 0=offset, 1=Friendly name 1 ending digits + offset [-1]
  // 2=MQTT topic ending digits + offset [-1]
  int32_t offset = XdrvMailbox.payload;
  if (XdrvMailbox.usridx && XdrvMailbox.index > 0) {
    uint32_t index = SET_FRIENDLYNAME1;
    if (XdrvMailbox.index == 2) index = SET_MQTT_TOPIC;
    char * name = SettingsText(index);
    char * ptr = name + strlen(name);
    while (--ptr >= name && isdigit(*ptr));
    if (!XdrvMailbox.data_len) offset = -1;
    offset += atoi(ptr + 1);
  }
  if (offset >= 0 && offset <= 255) {
    if (offset != Light.sequence_offset) {
      if (Light.sequence_offset) free(Light.channels_fifo);
      Light.sequence_offset = offset;
      if (Light.sequence_offset) Light.channels_fifo = (uint8_t *)calloc(Light.sequence_offset, LST_MAX);
    }
  }
  ResponseCmndNumber(Light.sequence_offset);
}
#endif  // USE_DGR_LIGHT_SEQUENCE

void CmndUndocA(void)
{
  // Theos legacy status
  char scolor[LIGHT_COLOR_SIZE];
  LightGetColor(scolor, true);  // force hex whatever Option 17
  scolor[6] = '\0';  // RGB only
  Response_P(PSTR("%s,%d,%d,%d,%d,%d"), scolor, Settings->light_fade, Settings->light_correction, Settings->light_scheme, Settings->light_speed, Settings->light_width);
  MqttPublishPrefixTopicRulesProcess_P(STAT, XdrvMailbox.topic);
  ResponseClear();
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
  else if (TasmotaGlobal.light_type) {
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
#ifdef USE_DEVICE_GROUPS
      case FUNC_DEVICE_GROUP_ITEM:
        LightHandleDevGroupItem();
        break;
#endif  // USE_DEVICE_GROUPS
      case FUNC_SET_POWER:
        LightSetPower();
        break;
      case FUNC_COMMAND:
        result = DecodeCommand(kLightCommands, LightCommand, kLightSynonyms);
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
