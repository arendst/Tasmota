/*
  xdrv_04_light.ino - PWM, WS2812 and sonoff led support for Sonoff-Tasmota

  Copyright (C) 2018  Theo Arends

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

#define XDRV_04              4

#define WS2812_SCHEMES       7    // Number of additional WS2812 schemes supported by xdrv_ws2812.ino

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
#define MAX_FIXED_COLOR  12
const LRgbColor kFixedColor[MAX_FIXED_COLOR] PROGMEM =
  { 255,0,0, 0,255,0, 0,0,255, 228,32,0, 0,228,32, 0,32,228, 188,64,0, 0,160,96, 160,32,240, 255,255,0, 255,0,170, 255,255,255 };

struct LWColor {
  uint8_t W;
};
#define MAX_FIXED_WHITE  4
const LWColor kFixedWhite[MAX_FIXED_WHITE] PROGMEM = { 0, 255, 128, 32 };

struct LCwColor {
  uint8_t C, W;
};
#define MAX_FIXED_COLD_WARM  4
const LCwColor kFixedColdWarm[MAX_FIXED_COLD_WARM] PROGMEM = { 0,0, 255,0, 0,255, 128,128 };

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

uint8_t light_entry_color[5];
uint8_t light_current_color[5];
uint8_t light_new_color[5];
uint8_t light_last_color[5];
uint8_t light_signal_color[5];

uint8_t light_wheel = 0;
uint8_t light_subtype = 0;
uint8_t light_device = 0;
uint8_t light_power = 0;
uint8_t light_update = 1;
uint8_t light_wakeup_active = 0;
uint8_t light_wakeup_dimmer = 0;
uint16_t light_wakeup_counter = 0;

uint8_t light_fixed_color_index = 1;

unsigned long strip_timer_counter = 0;    // Bars and Gradient

#ifdef USE_ARILUX_RF
/*********************************************************************************************\
 * Arilux LC11 Rf support stripped from RCSwitch library
\*********************************************************************************************/

#define ARILUX_RF_TIME_AVOID_DUPLICATE  1000  // Milliseconds

#define ARILUX_RF_MAX_CHANGES           51    // Pulses (sync + 2 x 24 bits)
#define ARILUX_RF_SEPARATION_LIMIT      4300  // Microseconds
#define ARILUX_RF_RECEIVE_TOLERANCE     60    // Percentage

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

    snprintf_P(log_data, sizeof(log_data), PSTR(D_LOG_RFR D_HOST D_CODE " 0x%04X, " D_RECEIVED " 0x%06X"), stored_hostcode, arilux_rf_received_value);
    AddLog(LOG_LEVEL_DEBUG);

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
  if ((pin[GPIO_ARIRFRCV] < 99) && (pin[GPIO_LED2] < 99)) {
    if (Settings.last_module != Settings.module) {
      Settings.rf_code[1][6] = 0;
      Settings.rf_code[1][7] = 0;
      Settings.last_module = Settings.module;
    }
    arilux_rf_received_value = 0;
    digitalWrite(pin[GPIO_LED2], !bitRead(led_inverted, 1));  // Turn on RF
    attachInterrupt(pin[GPIO_ARIRFRCV], AriluxRfInterrupt, CHANGE);
  }
}

void AriluxRfDisable(void)
{
  if ((pin[GPIO_ARIRFRCV] < 99) && (pin[GPIO_LED2] < 99)) {
    detachInterrupt(pin[GPIO_ARIRFRCV]);
    digitalWrite(pin[GPIO_LED2], bitRead(led_inverted, 1));  // Turn off RF
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

/********************************************************************************************/

void LightInit(void)
{
  uint8_t max_scheme = LS_MAX -1;

  light_device = devices_present;
  light_subtype = light_type &7;        // Always 0 - 7

  if (LST_SINGLE == light_subtype) {
    Settings.light_color[0] = 255;      // One channel only supports Dimmer but needs max color
  }
  if (light_type < LT_PWM6) {           // PWM
    for (byte i = 0; i < light_type; i++) {
      Settings.pwm_value[i] = 0;        // Disable direct PWM control
      if (pin[GPIO_PWM1 +i] < 99) {
        pinMode(pin[GPIO_PWM1 +i], OUTPUT);
      }
    }
    if (SONOFF_LED == Settings.module) { // Fix Sonoff Led instabilities
      if (!my_module.gp.io[4]) {
        pinMode(4, OUTPUT);             // Stop floating outputs
        digitalWrite(4, LOW);
      }
      if (!my_module.gp.io[5]) {
        pinMode(5, OUTPUT);             // Stop floating outputs
        digitalWrite(5, LOW);
      }
      if (!my_module.gp.io[14]) {
        pinMode(14, OUTPUT);            // Stop floating outputs
        digitalWrite(14, LOW);
      }
    }
    if (pin[GPIO_ARIRFRCV] < 99) {
      if (pin[GPIO_LED2] < 99) {
        digitalWrite(pin[GPIO_LED2], bitRead(led_inverted, 1));  // Turn off RF
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
}

void LightSetColorTemp(uint16_t ct)
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
  if (PHILIPS == Settings.module) {
    // Xiaomi Philips bulbs follow a different scheme:
    // channel 0=intensity, channel2=temperature
    Settings.light_color[1] = (uint8_t)icold;
  } else
  if (LST_RGBWC == light_subtype) {
    Settings.light_color[0] = 0;
    Settings.light_color[1] = 0;
    Settings.light_color[2] = 0;
    Settings.light_color[3] = (uint8_t)icold;
    Settings.light_color[4] = (uint8_t)iwarm;
  } else {
    Settings.light_color[0] = (uint8_t)icold;
    Settings.light_color[1] = (uint8_t)iwarm;
  }
}

uint16_t LightGetColorTemp(void)
{
  uint8_t ct_idx = 0;
  if (LST_RGBWC == light_subtype) {
    ct_idx = 3;
  }
  uint16_t my_ct = Settings.light_color[ct_idx +1];
  if (my_ct > 0) {
    return ((my_ct * 136) / 100) + 154;
  } else {
    my_ct = Settings.light_color[ct_idx];
    return 499 - ((my_ct * 136) / 100);
  }
}

void LightSetDimmer(uint8_t myDimmer)
{
  float temp;

  if (PHILIPS == Settings.module) {
    // Xiaomi Philips bulbs use two PWM channels with a different scheme:
    float dimmer = 100 / (float)myDimmer;
    temp = (float)Settings.light_color[0] / dimmer; // channel 1 is intensity
    light_current_color[0] = (uint8_t)temp;
    temp = (float)Settings.light_color[1];          // channel 2 is temperature
    light_current_color[1] = (uint8_t)temp;
    return;
  }
  if (LT_PWM1 == light_type) {
    Settings.light_color[0] = 255;    // One PWM channel only supports Dimmer but needs max color
  }
  float dimmer = 100 / (float)myDimmer;
  for (byte i = 0; i < light_subtype; i++) {
    if (Settings.flag.light_signal) {
      temp = (float)light_signal_color[i] / dimmer;
    } else {
      temp = (float)Settings.light_color[i] / dimmer;
    }
    light_current_color[i] = (uint8_t)temp;
  }
}

void LightSetColor(void)
{
  uint8_t highest = 0;

  for (byte i = 0; i < light_subtype; i++) {
    if (highest < light_current_color[i]) {
      highest = light_current_color[i];
    }
  }
  float mDim = (float)highest / 2.55;
  Settings.light_dimmer = (uint8_t)mDim;
  float dimmer = 100 / mDim;
  for (byte i = 0; i < light_subtype; i++) {
    float temp = (float)light_current_color[i] * dimmer;
    Settings.light_color[i] = (uint8_t)temp;
  }
}

void LightSetSignal(uint16_t lo, uint16_t hi, uint16_t value)
{
/* lo - below lo is green
   hi - above hi is red
*/
  if (Settings.flag.light_signal) {
    uint16_t signal = 0;
    if (value > lo) {
      signal = (value - lo) * 10 / ((hi - lo) * 10 / 256);
      if (signal > 255) {
        signal = 255;
      }
    }
//    snprintf_P(log_data, sizeof(log_data), PSTR(D_LOG_DEBUG "Light signal %d"), signal);
//    AddLog(LOG_LEVEL_DEBUG);
    light_signal_color[0] = signal;
    light_signal_color[1] = 255 - signal;
    light_signal_color[2] = 0;
    light_signal_color[3] = 0;
    light_signal_color[4] = 0;

    Settings.light_scheme = 0;
    if (!Settings.light_dimmer) {
      Settings.light_dimmer = 20;
    }
  }
}

char* LightGetColor(uint8_t type, char* scolor)
{
  LightSetDimmer(Settings.light_dimmer);
  scolor[0] = '\0';
  for (byte i = 0; i < light_subtype; i++) {
    if (!type && Settings.flag.decimal_text) {
      snprintf_P(scolor, 25, PSTR("%s%s%d"), scolor, (i > 0) ? "," : "", light_current_color[i]);
    } else {
      snprintf_P(scolor, 25, PSTR("%s%02X"), scolor, light_current_color[i]);
    }
  }
  return scolor;
}

void LightPowerOn(void)
{
  if (Settings.light_dimmer && !(light_power)) {
    ExecuteCommandPower(light_device, POWER_ON, SRC_LIGHT);
  }
}

void LightState(uint8_t append)
{
  char scolor[25];
  char scommand[33];
  float hsb[3];
  int16_t h,s,b;

  if (append) {
    snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s,"), mqtt_data);
  } else {
    snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("{"));
  }
  GetPowerDevice(scommand, light_device, sizeof(scommand), Settings.flag.device_index_enable);
  snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s\"%s\":\"%s\",\"" D_CMND_DIMMER "\":%d"),
    mqtt_data, scommand, GetStateText(light_power), Settings.light_dimmer);
  if (light_subtype > LST_SINGLE) {
    snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s,\"" D_CMND_COLOR "\":\"%s\""), mqtt_data, LightGetColor(0, scolor));
    //  Add status for HSB
    LightGetHsb(&hsb[0],&hsb[1],&hsb[2], false);
    //  Scale these percentages up to the numbers expected by the client
    h = round(hsb[0] * 360);
    s = round(hsb[1] * 100);
    b = round(hsb[2] * 100);
    snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s,\"" D_CMND_HSBCOLOR "\":\"%d,%d,%d\""), mqtt_data, h,s,b);
    // Add status for each channel
    snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s,\"" D_CMND_CHANNEL "\":[" ), mqtt_data);
    for (byte i = 0; i < light_subtype; i++) {
      snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s%s%d" ), mqtt_data, (i > 0 ? "," : ""), light_current_color[i] * 100 / 255);
    }
    snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s]" ), mqtt_data);
  }
  if ((LST_COLDWARM == light_subtype) || (LST_RGBWC == light_subtype)) {
    snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s,\"" D_CMND_COLORTEMPERATURE "\":%d"), mqtt_data, LightGetColorTemp());
  }
  if (append) {
    if (light_subtype >= LST_RGB) {
      snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s,\"" D_CMND_SCHEME "\":%d"), mqtt_data, Settings.light_scheme);
    }
    if (LT_WS2812 == light_type) {
      snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s,\"" D_CMND_WIDTH "\":%d"), mqtt_data, Settings.light_width);
    }
    snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s,\"" D_CMND_FADE "\":\"%s\",\"" D_CMND_SPEED "\":%d,\"" D_CMND_LEDTABLE "\":\"%s\""),
      mqtt_data, GetStateText(Settings.light_fade), Settings.light_speed, GetStateText(Settings.light_correction));
  } else {
    snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s}"), mqtt_data);
  }
}

void LightPreparePower(void)
{
  if (Settings.light_dimmer && !(light_power)) {
    if (!Settings.flag.not_power_linked) {
      ExecuteCommandPower(light_device, POWER_ON_NO_STATE, SRC_LIGHT);
    }
  }
  else if (!Settings.light_dimmer && light_power) {
    ExecuteCommandPower(light_device, POWER_OFF_NO_STATE, SRC_LIGHT);
  }
#ifdef USE_DOMOTICZ
  DomoticzUpdatePowerState(light_device);
#endif  // USE_DOMOTICZ
  if (Settings.flag3.hass_tele_on_power) {
    mqtt_data[0] = '\0';
    MqttShowState();
    MqttPublishPrefixTopic_P(TELE, PSTR(D_RSLT_STATE), MQTT_TELE_RETAIN);
  }

  LightState(0);
}

void LightFade(void)
{
  if (0 == Settings.light_fade) {
    for (byte i = 0; i < light_subtype; i++) {
      light_new_color[i] = light_current_color[i];
    }
  } else {
    uint8_t shift = Settings.light_speed;
    if (Settings.light_speed > 6) {
      shift = (strip_timer_counter % (Settings.light_speed -6)) ? 0 : 8;
    }
    if (shift) {
      for (byte i = 0; i < light_subtype; i++) {
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
  for (byte i = 0; i < LST_RGB; i++) {
    float temp = (float)light_entry_color[i] / dimmer;
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
  for (byte i = 0; i < LST_RGB; i++) {
    if (light_new_color[i] != light_current_color[i]) {
      light_update = 1;
    }
  }
  if (!light_update) {
    light_wheel = random(255);
    LightWheel(light_wheel);
    memcpy(light_current_color, light_entry_color, sizeof(light_current_color));
  }
  LightFade();
}

void LightSetPower(void)
{
//  light_power = XdrvMailbox.index;
  light_power = bitRead(XdrvMailbox.index, light_device -1);
  if (light_wakeup_active) {
    light_wakeup_active--;
  }
  if (light_power) {
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
    for (byte i = 0; i < light_subtype; i++) {
      light_still_on += light_new_color[i];
    }
    if (light_still_on && Settings.light_fade && (Settings.light_scheme < LS_MAX)) {
      uint8_t speed = Settings.light_speed;
      if (speed > 6) {
        speed = 6;
      }
      for (byte i = 0; i < light_subtype; i++) {
        if (light_new_color[i] > 0) {
          light_new_color[i] -= (light_new_color[i] >> speed) +1;
        }
      }
    } else {
      for (byte i = 0; i < light_subtype; i++) {
        light_new_color[i] = 0;
      }
    }
  }
  else {
    sleep = 0;
    switch (Settings.light_scheme) {
      case LS_POWER:
        LightSetDimmer(Settings.light_dimmer);
        LightFade();
        break;
      case LS_WAKEUP:
        if (2 == light_wakeup_active) {
          light_wakeup_active = 1;
          for (byte i = 0; i < light_subtype; i++) {
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
            LightSetDimmer(light_wakeup_dimmer);
            for (byte i = 0; i < light_subtype; i++) {
              light_new_color[i] = light_current_color[i];
            }
          } else {
            snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("{\"" D_CMND_WAKEUP "\":\"" D_JSON_DONE "\"}"));
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
    for (byte i = 0; i < light_subtype; i++) {
      if (light_last_color[i] != light_new_color[i]) {
        light_update = 1;
      }
    }
    if (light_update) {
      light_update = 0;
      for (byte i = 0; i < light_subtype; i++) {
        light_last_color[i] = light_new_color[i];
        cur_col[i] = light_last_color[i]*Settings.rgbwwTable[i]/255;
        cur_col[i] = (Settings.light_correction) ? ledTable[cur_col[i]] : cur_col[i];
        if (light_type < LT_PWM6) {
          if (pin[GPIO_PWM1 +i] < 99) {
            if (cur_col[i] > 0xFC) {
              cur_col[i] = 0xFC;   // Fix unwanted blinking and PWM watchdog errors for values close to pwm_range (H801, Arilux and BN-SZ01)
            }
            uint16_t curcol = cur_col[i] * (Settings.pwm_range / 255);
//            snprintf_P(log_data, sizeof(log_data), PSTR(D_LOG_APPLICATION "Cur_Col%d %d, CurCol %d"), i, cur_col[i], curcol);
//            AddLog(LOG_LEVEL_DEBUG);
            analogWrite(pin[GPIO_PWM1 +i], bitRead(pwm_inverted, i) ? Settings.pwm_range - curcol : curcol);
          }
        }
      }
#ifdef USE_WS2812  // ************************************************************************
      if (LT_WS2812 == light_type) {
        Ws2812SetColor(0, cur_col[0], cur_col[1], cur_col[2], cur_col[3]);
      }
#endif  // USE_ES2812 ************************************************************************
      if (light_type > LT_WS2812) {
        LightMy92x1Duty(cur_col[0], cur_col[1], cur_col[2], cur_col[3], cur_col[4]);
      }
#ifdef USE_TUYA_DIMMER
      if (light_type == LT_SERIAL1 && Settings.module == TUYA_DIMMER ) {
        LightSerialDuty(cur_col[0]);
      }
#endif  // USE_TUYA_DIMMER
#ifdef USE_ARMTRONIX_DIMMERS
      if (light_type == LT_SERIAL2) {
        LightSerial2Duty(cur_col[0],cur_col[1]);
      }
#endif  // USE_ARMTRONIX_DIMMERS
#ifdef USE_PS_16_DZ
      if (light_type == LT_SERIAL1 && Settings.module == PS_16_DZ) {
        PS16DZSerialDuty(cur_col[0]);
      }
#endif  // USE_PS_16_DZ

    }
  }
}

/*********************************************************************************************\
 * Hue support
\*********************************************************************************************/

float light_hue = 0.0;
float light_saturation = 0.0;
float light_brightness = 0.0;

void LightRgbToHsb(void)
{
  LightSetDimmer(Settings.light_dimmer);

  // convert colors to float between (0.0 - 1.0)
  float r = light_current_color[0] / 255.0f;
  float g = light_current_color[1] / 255.0f;
  float b = light_current_color[2] / 255.0f;

  float max = (r > g && r > b) ? r : (g > b) ? g : b;
  float min = (r < g && r < b) ? r : (g < b) ? g : b;

  float d = max - min;

  light_hue = 0.0;
  light_brightness = max;
  light_saturation = (0.0f == light_brightness) ? 0 : (d / light_brightness);

  if (d != 0.0f)
  {
    if (r == max) {
      light_hue = (g - b) / d + (g < b ? 6.0f : 0.0f);
    } else if (g == max) {
      light_hue = (b - r) / d + 2.0f;
    } else {
      light_hue = (r - g) / d + 4.0f;
    }
    light_hue /= 6.0f;
  }
}

void LightHsbToRgb(void)
{
  float r;
  float g;
  float b;

  float h = light_hue;
  float s = light_saturation;
  float v = light_brightness;

  if (0.0f == light_saturation) {
    r = g = b = v;                      // Achromatic or black
  } else {
    if (h < 0.0f) {
      h += 1.0f;
    }
    else if (h >= 1.0f) {
      h -= 1.0f;
    }
    h *= 6.0f;
    int i = (int)h;
    float f = h - i;
    float q = v * (1.0f - s * f);
    float p = v * (1.0f - s);
    float t = v * (1.0f - s * (1.0f - f));
    switch (i) {
      case 0:
        r = v;
        g = t;
        b = p;
        break;
      case 1:
        r = q;
        g = v;
        b = p;
        break;
      case 2:
        r = p;
        g = v;
        b = t;
        break;
      case 3:
        r = p;
        g = q;
        b = v;
        break;
      case 4:
        r = t;
        g = p;
        b = v;
        break;
      default:
        r = v;
        g = p;
        b = q;
        break;
      }
  }

  light_current_color[0] = (uint8_t)(r * 255.0f);
  light_current_color[1] = (uint8_t)(g * 255.0f);
  light_current_color[2] = (uint8_t)(b * 255.0f);
  light_current_color[3] = 0;
  light_current_color[4] = 0;
}

/********************************************************************************************/

void LightGetHsb(float *hue, float *sat, float *bri, bool gotct)
{
  if (light_subtype > LST_COLDWARM && !gotct) {
    LightRgbToHsb();
    *hue = light_hue;
    *sat = light_saturation;
    *bri = light_brightness;
  } else {
    *hue = 0;
    *sat = 0;
    *bri = (0.01f * (float)Settings.light_dimmer);
  }
}

void LightSetHsb(float hue, float sat, float bri, uint16_t ct, bool gotct)
{
  if (light_subtype > LST_COLDWARM) {
    if ((LST_RGBWC == light_subtype) && (gotct)) {
      uint8_t tmp = (uint8_t)(bri * 100);
      Settings.light_dimmer = tmp;
      if (ct > 0) {
        LightSetColorTemp(ct);
      }
    } else {
      light_hue = hue;
      light_saturation = sat;
      light_brightness = bri;
      LightHsbToRgb();
      LightSetColor();
    }
    LightPreparePower();
    MqttPublishPrefixTopic_P(RESULT_OR_STAT, PSTR(D_CMND_COLOR));
  } else {
    uint8_t tmp = (uint8_t)(bri * 100);
    Settings.light_dimmer = tmp;
    if (LST_COLDWARM == light_subtype) {
      if (ct > 0) {
        LightSetColorTemp(ct);
      }
      LightPreparePower();
      MqttPublishPrefixTopic_P(RESULT_OR_STAT, PSTR(D_CMND_COLOR));
    } else {
      LightPreparePower();
      MqttPublishPrefixTopic_P(RESULT_OR_STAT, PSTR(D_CMND_DIMMER));
    }
  }
}

/*********************************************************************************************\
 * Commands
\*********************************************************************************************/

boolean LightColorEntry(char *buffer, uint8_t buffer_length)
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
  if (strstr(buffer, ",")) {                        // Decimal entry
    int8_t i = 0;
    for (str = strtok_r(buffer, ",", &p); str && i < 6; str = strtok_r(NULL, ",", &p)) {
      if (i < 5) {
        light_entry_color[i++] = atoi(str);
      }
    }
    entry_type = 2;                                 // Decimal
  }
  else if (((2 * light_subtype) == buffer_length) || (buffer_length > 3)) {  // Hexadecimal entry
    for (byte i = 0; i < buffer_length / 2; i++) {
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

boolean LightCommand(void)
{
  char command [CMDSZ];
  boolean serviced = true;
  boolean coldim = false;
  boolean valid_entry = false;
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
        snprintf_P(scolor, sizeof(scolor), PSTR("0,0,0,%d"), XdrvMailbox.payload * 255 / 100);
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
          memcpy(light_current_color, light_entry_color, sizeof(light_current_color));
          uint8_t dimmer = Settings.light_dimmer;
          LightSetColor();
          if (2 == XdrvMailbox.index) {
            Settings.light_dimmer = dimmer;
          }
          Settings.light_scheme = 0;
          coldim = true;
        } else {             // Color3, 4, 5 and 6
          for (byte i = 0; i < LST_RGB; i++) {
            Settings.ws_color[XdrvMailbox.index -3][i] = light_entry_color[i];
          }
        }
      }
    }
    if (!valid_entry && (XdrvMailbox.index <= 2)) {
      snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_COMMAND_SVALUE, command, LightGetColor(0, scolor));
    }
    if (XdrvMailbox.index >= 3) {
      scolor[0] = '\0';
      for (byte i = 0; i < LST_RGB; i++) {
        if (Settings.flag.decimal_text) {
          snprintf_P(scolor, 25, PSTR("%s%s%d"), scolor, (i > 0) ? "," : "", Settings.ws_color[XdrvMailbox.index -3][i]);
        } else {
          snprintf_P(scolor, 25, PSTR("%s%02X"), scolor, Settings.ws_color[XdrvMailbox.index -3][i]);
        }
      }
      snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_COMMAND_INDEX_SVALUE, command, XdrvMailbox.index, scolor);
    }
  }
  else if ((CMND_CHANNEL == command_code) && (XdrvMailbox.index > 0) && (XdrvMailbox.index <= light_subtype ) ) {
    //  Set "Channel" directly - this allows Color and Direct PWM control to coexist
    if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 100)) {
      light_current_color[XdrvMailbox.index-1] = XdrvMailbox.payload * 255 / 100;
      LightSetColor();
      coldim = true;
    }
    snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_COMMAND_INDEX_NVALUE, command, XdrvMailbox.index, light_current_color[XdrvMailbox.index -1] * 100 / 255);
  }
  else if ((CMND_HSBCOLOR == command_code) && ( light_subtype >= LST_RGB)) {
    bool validHSB = (XdrvMailbox.data_len > 0);
    if (validHSB) {
      uint16_t HSB[3];
      if (strstr(XdrvMailbox.data, ",")) {  // Command with 3 comma separated parameters, Hue (0<H<360), Saturation (0<S<100) AND Brightness (0<B<100)
        for (int i = 0; i < 3; i++) {
          char *substr;

          if (0 == i) {
            substr = strtok(XdrvMailbox.data, ",");
          } else {
            substr = strtok(NULL, ",");
          }
          if (substr != NULL) {
            HSB[i] = atoi(substr);
          } else {
            validHSB = false;
          }
        }
      } else {  // Command with only 1 parameter, Hue (0<H<360), Saturation (0<S<100) OR Brightness (0<B<100)
        float hsb[3];

        LightGetHsb(&hsb[0],&hsb[1],&hsb[2], false);
        HSB[0] = round(hsb[0] * 360);
        HSB[1] = round(hsb[1] * 100);
        HSB[2] = round(hsb[2] * 100);
        if ((XdrvMailbox.index > 0) && (XdrvMailbox.index < 4)) {
          HSB[XdrvMailbox.index -1] = XdrvMailbox.payload;
        } else {
          validHSB = false;
        }
      }
      if (validHSB) {
        //  Translate to fractional elements as required by LightHsbToRgb
        //  Keep the results <=1 in the event someone passes something out of range.
        LightSetHsb(( (HSB[0]>360) ? (HSB[0] % 360) : HSB[0] ) /360.0,
                    ( (HSB[1]>100) ? (HSB[1] % 100) : HSB[1] ) /100.0,
                    ( (HSB[2]>100) ? (HSB[2] % 100) : HSB[2] ) /100.0,
                    0,
                    false);
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
      for (char *color = strtok_r(XdrvMailbox.data, " ", &p); color; color = strtok_r(NULL, " ", &p)) {
        if (LightColorEntry(color, strlen(color))) {
          Ws2812SetColor(idx, light_entry_color[0], light_entry_color[1], light_entry_color[2], light_entry_color[3]);
          idx++;
          if (idx >= Settings.light_pixels) break;
        } else {
          break;
        }
      }

      Ws2812ForceUpdate();
    }
    snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_COMMAND_INDEX_SVALUE, command, XdrvMailbox.index, Ws2812GetColor(XdrvMailbox.index, scolor));
  }
  else if ((CMND_PIXELS == command_code) && (LT_WS2812 == light_type)) {
    if ((XdrvMailbox.payload > 0) && (XdrvMailbox.payload <= WS2812_MAX_LEDS)) {
      Settings.light_pixels = XdrvMailbox.payload;
      Settings.light_rotation = 0;
      Ws2812Clear();
      light_update = 1;
    }
    snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_COMMAND_NVALUE, command, Settings.light_pixels);
  }
  else if ((CMND_ROTATION == command_code) && (LT_WS2812 == light_type)) {
    if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload < Settings.light_pixels)) {
      Settings.light_rotation = XdrvMailbox.payload;
    }
    snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_COMMAND_NVALUE, command, Settings.light_rotation);
  }
  else if ((CMND_WIDTH == command_code) && (LT_WS2812 == light_type) && (XdrvMailbox.index > 0) && (XdrvMailbox.index <= 4)) {
    if (1 == XdrvMailbox.index) {
      if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 4)) {
        Settings.light_width = XdrvMailbox.payload;
      }
      snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_COMMAND_NVALUE, command, Settings.light_width);
    } else {
      if ((XdrvMailbox.payload > 0) && (XdrvMailbox.payload < 32)) {
        Settings.ws_width[XdrvMailbox.index -2] = XdrvMailbox.payload;
      }
      snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_COMMAND_INDEX_NVALUE, command, XdrvMailbox.index, Settings.ws_width[XdrvMailbox.index -2]);
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
      if (Settings.flag3.hass_tele_on_power) {
        mqtt_data[0] = '\0';
        MqttShowState();
        MqttPublishPrefixTopic_P(TELE, PSTR(D_RSLT_STATE), MQTT_TELE_RETAIN);
      }
    }
    snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_COMMAND_NVALUE, command, Settings.light_scheme);
  }
  else if (CMND_WAKEUP == command_code) {
    if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 100)) {
      Settings.light_dimmer = XdrvMailbox.payload;
    }
    light_wakeup_active = 3;
    Settings.light_scheme = LS_WAKEUP;
    LightPowerOn();
    snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_COMMAND_SVALUE, command, D_JSON_STARTED);
  }
  else if ((CMND_COLORTEMPERATURE == command_code) && ((LST_COLDWARM == light_subtype) || (LST_RGBWC == light_subtype))) { // ColorTemp
    if (option != '\0') {
      uint16_t value = LightGetColorTemp();
      if ('+' == option) {
        XdrvMailbox.payload = (value  > 466) ? 500 : value + 34;
      }
      else if ('-' == option) {
        XdrvMailbox.payload = (value  < 187) ? 153 : value - 34;
      }
    }
    if ((XdrvMailbox.payload >= 153) && (XdrvMailbox.payload <= 500)) {  // https://developers.meethue.com/documentation/core-concepts
      LightSetColorTemp(XdrvMailbox.payload);
      coldim = true;
    } else {
      snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_COMMAND_NVALUE, command, LightGetColorTemp());
    }
  }
  else if (CMND_DIMMER == command_code) {
    if ('+' == option) {
      XdrvMailbox.payload = (Settings.light_dimmer > 89) ? 100 : Settings.light_dimmer + 10;
    }
    else if ('-' == option) {
      XdrvMailbox.payload = (Settings.light_dimmer < 11) ? 1 : Settings.light_dimmer - 10;
    }
    if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 100)) {
      Settings.light_dimmer = XdrvMailbox.payload;
      light_update = 1;
      coldim = true;
    } else {
      snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_COMMAND_NVALUE, command, Settings.light_dimmer);
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
    snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_COMMAND_SVALUE, command, GetStateText(Settings.light_correction));
  }
  else if (CMND_RGBWWTABLE == command_code) {
    bool validtable = (XdrvMailbox.data_len > 0);
    char scolor[25];
    if (validtable) {
      uint16_t HSB[3];
      if (strstr(XdrvMailbox.data, ",")) {  // Command with up to 5 comma separated parameters
        for (int i = 0; i < LST_RGBWC; i++) {
          char *substr;

          if (0 == i) {
            substr = strtok(XdrvMailbox.data, ",");
          } else {
            substr = strtok(NULL, ",");
          }
          if (substr != NULL) {
            Settings.rgbwwTable[i] = atoi(substr);
          }
        }
      }
      light_update = 1;
    }
    scolor[0] = '\0';
    for (byte i = 0; i < LST_RGBWC; i++) {
      snprintf_P(scolor, 25, PSTR("%s%s%d"), scolor, (i > 0) ? "," : "", Settings.rgbwwTable[i]);
    }
    snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_COMMAND_INDEX_SVALUE, command, XdrvMailbox.index, scolor);
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
    snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_COMMAND_SVALUE, command, GetStateText(Settings.light_fade));
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
    snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_COMMAND_NVALUE, command, Settings.light_speed);
  }
  else if (CMND_WAKEUPDURATION == command_code) {
    if ((XdrvMailbox.payload > 0) && (XdrvMailbox.payload < 3001)) {
      Settings.light_wakeup = XdrvMailbox.payload;
      light_wakeup_active = 0;
    }
    snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_COMMAND_NVALUE, command, Settings.light_wakeup);
  }
  else if (CMND_UNDOCA == command_code) {  // Theos legacy status
    LightGetColor(1, scolor);
    scolor[6] = '\0';  // RGB only
    snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s,%d,%d,%d,%d,%d"),
      scolor, Settings.light_fade, Settings.light_correction, Settings.light_scheme, Settings.light_speed, Settings.light_width);
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

boolean Xdrv04(byte function)
{
  boolean result = false;

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
