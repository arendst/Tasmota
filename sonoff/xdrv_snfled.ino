/*
  xdrv_snfled.ino - PWM, WS2812 and sonoff led support for Sonoff-Tasmota

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
 * PWM, WS2812, Sonoff B1, AiLight, Sonoff Led and BN-SZ01
 *
 * light_type  Module     Color  ColorTemp
 *  1       PWM1       W      no          (Sonoff BN-SZ)
 *  2       PWM2       CW     yes         (Sonoff Led)
 *  3       PWM3       RGB    no          (H801 and MagicHome)
 *  4       PWM4       RGBW   no          (H801 and MagicHome)
 *  5       PWM5       RGBCW  yes         (H801)
 *  9       reserved          no
 * 10       reserved          yes
 * 11       +WS2812    RGB    no
 * 12       AiLight    RGBW   no
 * 13       Sonoff B1  RGBCW  yes
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

uint8_t light_entry_color[5];
uint8_t light_current_color[5];
uint8_t light_new_color[5];
uint8_t light_last_color[5];

uint8_t light_subtype = 0;
uint8_t light_power = 0;
uint8_t light_update = 1;
uint8_t light_wakeup_active = 0;
uint8_t light_wakeup_dimmer = 0;
uint16_t light_wakeup_counter = 0;

unsigned long strip_timer_counter = 0;  // Bars and Gradient

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

void LightMy92x1Init()
{
  uint8_t chips = light_type -11;       // 1 (AiLight) or 2 (Sonoff B1)

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
  uint8_t didx = light_type -12;        // 0 or 1
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
  if (light_type < 6) {                 // PWM
    for (byte i = 0; i < light_type; i++) {
      Settings.pwm_value[i] = 0;        // Disable direct PWM control
    }
    if (1 == light_type) {
      Settings.led_color[0] = 255;      // One PWM channel only supports Dimmer but needs max color
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
    Settings.led_scheme = 0;
  }
#ifdef USE_WS2812  // ************************************************************************
  else if (11 == light_type) {
    Ws2812Init();
    if (1 == Settings.led_scheme) {
      Settings.led_scheme = 0;
    }
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
    Settings.led_scheme = 0;
  }

  light_subtype = light_type &7;
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
  if (5 == light_subtype) {
    Settings.led_color[0] = 0;
    Settings.led_color[1] = 0;
    Settings.led_color[2] = 0;
    Settings.led_color[3] = (uint8_t)icold;
    Settings.led_color[4] = (uint8_t)iwarm;
  } else {
    Settings.led_color[0] = (uint8_t)icold;
    Settings.led_color[1] = (uint8_t)iwarm;
  }
}

uint16_t LightGetColorTemp()
{
  uint8_t ct_idx = 0;
  if (5 == light_subtype) {
    ct_idx = 3;
  }
  uint16_t my_ct = Settings.led_color[ct_idx +1];
  if (my_ct > 0) {
    return ((my_ct * 136) / 100) + 154;
  } else {
    my_ct = Settings.led_color[ct_idx];
    return 499 - ((my_ct * 136) / 100);
  }
}

void LightSetDimmer(uint8_t myDimmer)
{
  float temp;

  if ((SONOFF_BN == Settings.module) && (100 == myDimmer)) {
    myDimmer = 99;                      // BN-SZ01 starts flickering at dimmer = 100
  }
  float dimmer = 100 / (float)myDimmer;
  for (byte i = 0; i < light_subtype; i++) {
    temp = (float)Settings.led_color[i] / dimmer;
    light_current_color[i] = (uint8_t)temp;
  }
}

void LightSetColor()
{
  uint8_t highest = 0;
  float temp;

  for (byte i = 0; i < light_subtype; i++) {
    if (highest < light_current_color[i]) {
      highest = light_current_color[i];
    }
  }
  float mDim = (float)highest / 2.55;
  Settings.led_dimmer = (uint8_t)mDim;
  float dimmer = 100 / mDim;
  for (byte i = 0; i < light_subtype; i++) {
    temp = (float)light_current_color[i] * dimmer;
    Settings.led_color[i] = (uint8_t)temp;
  }
}

char* LightGetColor(uint8_t type, char* scolor)
{
  LightSetDimmer(Settings.led_dimmer);
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

void LightPreparePower()
{
  char scolor[25];
  char scommand[16];

  if (Settings.led_dimmer && !(light_power)) {
    ExecuteCommandPower(devices_present, 7);  // No publishPowerState
  }
  else if (!Settings.led_dimmer && light_power) {
    ExecuteCommandPower(devices_present, 6);  // No publishPowerState
  }
#ifdef USE_DOMOTICZ
//  MqttPublishDomoticzPowerState(1);
  DomoticzUpdatePowerState(devices_present);
#endif  // USE_DOMOTICZ

  GetPowerDevice(scommand, devices_present, sizeof(scommand));
  if (light_subtype > 1) {
    snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("{\"%s\":\"%s\", \"" D_CMND_DIMMER "\":%d, \"" D_CMND_COLOR "\":\"%s\"}"),
      scommand, GetStateText(light_power), Settings.led_dimmer, LightGetColor(0, scolor));
  } else {
    snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("{\"%s\":\"%s\", \"" D_CMND_DIMMER "\":%d}"),
      scommand, GetStateText(light_power), Settings.led_dimmer);
  }
}

void LightSetPower(uint8_t mpower)
{
  light_power = mpower;
  if (light_wakeup_active) {
    light_wakeup_active--;
  }
  if (light_power) {
    light_update = 1;
  }
  LightAnimate();
}

void LightAnimate()
{
  uint8_t fadeValue;
  uint8_t cur_col[5];

  strip_timer_counter++;
  if (!light_power) {                   // Power Off
    sleep = Settings.sleep;
    strip_timer_counter = 0;
    for (byte i = 0; i < light_subtype; i++) {
      light_new_color[i] = 0;
    }
  }
  else {
    sleep = 0;
    switch (Settings.led_scheme) {
      case 0:                           // Power On
        LightSetDimmer(Settings.led_dimmer);
        if (0 == Settings.led_fade) {
          for (byte i = 0; i < light_subtype; i++) {
            light_new_color[i] = light_current_color[i];
          }
        } else {
          for (byte i = 0; i < light_subtype; i++) {
            if (light_new_color[i] != light_current_color[i]) {
              if (light_new_color[i] < light_current_color[i]) {
                light_new_color[i] += ((light_current_color[i] - light_new_color[i]) >> Settings.led_speed) +1;
              }
              if (light_new_color[i] > light_current_color[i]) {
                light_new_color[i] -= ((light_new_color[i] - light_current_color[i]) >> Settings.led_speed) +1;
              }
            }
          }
        }
        break;
      case 1:                           // Power On using wake up duration
        if (2 == light_wakeup_active) {
          light_wakeup_active = 1;
          for (byte i = 0; i < light_subtype; i++) {
            light_new_color[i] = 0;
          }
          light_wakeup_counter = 0;
          light_wakeup_dimmer = 0;
        }
        light_wakeup_counter++;
        if (light_wakeup_counter > ((Settings.led_wakeup * STATES) / Settings.led_dimmer)) {
          light_wakeup_counter = 0;
          light_wakeup_dimmer++;
          if (light_wakeup_dimmer <= Settings.led_dimmer) {
            LightSetDimmer(light_wakeup_dimmer);
            for (byte i = 0; i < light_subtype; i++) {
              light_new_color[i] = light_current_color[i];
            }
          } else {
            snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("{\"" D_CMND_WAKEUP "\":\"" D_DONE "\"}"));
            MqttPublishPrefixTopic_P(2, PSTR(D_CMND_WAKEUP));
            light_wakeup_active = 0;
            Settings.led_scheme = 0;
          }
        }
        break;
#ifdef USE_WS2812  // ************************************************************************
      default:
        if (11 == light_type) {
          Ws2812ShowScheme(Settings.led_scheme -2);
        }
#endif  // USE_WS2812 ************************************************************************
    }
  }

  if ((Settings.led_scheme < 2) || !light_power) {
    for (byte i = 0; i < light_subtype; i++) {
      if (light_last_color[i] != light_new_color[i]) {
        light_update = 1;
      }
    }
    if (light_update) {
      light_update = 0;
      for (byte i = 0; i < light_subtype; i++) {
        light_last_color[i] = light_new_color[i];
        cur_col[i] = (Settings.led_table) ? ledTable[light_last_color[i]] : light_last_color[i];
        if (light_type < 6) {
          if (pin[GPIO_PWM1 +i] < 99) {
            uint16_t curcol = cur_col[i] * (Settings.pwm_range / 255);
//            snprintf_P(log_data, sizeof(log_data), PSTR(D_LOG_APPLICATION "Cur_Col%d %d, CurCol %d"), i, cur_col[i], curcol);
//            AddLog(LOG_LEVEL_DEBUG);
            analogWrite(pin[GPIO_PWM1 +i], bitRead(pwm_inverted, i) ? Settings.pwm_range - curcol : curcol);
          }
        }
      }
#ifdef USE_WS2812  // ************************************************************************
      if (11 == light_type) {
        Ws2812SetColor(0, cur_col[0], cur_col[1], cur_col[2]);
      }
#endif  // USE_ES2812 ************************************************************************
      if (light_type > 11) {
        LightMy92x1Duty(cur_col[0], cur_col[1], cur_col[2], cur_col[3], cur_col[4]);
      }
    }
  }
}

/*********************************************************************************************\
 * Hue support
\*********************************************************************************************/

float light_hue = 0.0;
float light_saturation = 0.0;
float light_brightness = 0.0;

void LightRgbToHsb()
{
  LightSetDimmer(Settings.led_dimmer);

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

void LightHsbToRgb()
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
}

/********************************************************************************************/

void LightReplaceHsb(String *response)
{
  if (light_subtype > 2) {
    LightRgbToHsb();
    response->replace("{h}", String((uint16_t)(65535.0f * light_hue)));
    response->replace("{s}", String((uint8_t)(254.0f * light_saturation)));
    response->replace("{b}", String((uint8_t)(254.0f * light_brightness)));
  } else {
    response->replace("{h}", "0");
    response->replace("{s}", "0");
//    response->replace("{b}", String((uint8_t)(2.54f * (float)Settings.led_dimmer)));
    response->replace("{b}", String((uint8_t)(0.01f * (float)Settings.led_dimmer)));
  }
}

void LightGetHsb(float *hue, float *sat, float *bri)
{
  if (light_subtype > 2) {
    LightRgbToHsb();
    *hue = light_hue;
    *sat = light_saturation;
    *bri = light_brightness;
  } else {
    *hue = 0;
    *sat = 0;
//    *bri = (2.54f * (float)Settings.led_dimmer);
    *bri = (0.01f * (float)Settings.led_dimmer);
  }
}

void LightSetHsb(float hue, float sat, float bri, uint16_t ct)
{
  if (light_type > 2) {
    if ((5 == light_subtype) && (ct > 0)) {
      LightSetColorTemp(ct);
    } else {
      light_hue = hue;
      light_saturation = sat;
      light_brightness = bri;
      LightHsbToRgb();
      LightSetColor();
    }
    LightPreparePower();
    MqttPublishPrefixTopic_P(5, PSTR(D_CMND_COLOR));
  } else {
    uint8_t tmp = (uint8_t)(bri * 100);
    Settings.led_dimmer = tmp;
    if (2 == light_subtype) {
      if (ct > 0) {
        LightSetColorTemp(ct);
      }
      LightPreparePower();
      MqttPublishPrefixTopic_P(5, PSTR(D_CMND_COLOR));
    } else {
      LightPreparePower();
      MqttPublishPrefixTopic_P(5, PSTR(D_CMND_DIMMER));
    }
  }
}

/*********************************************************************************************\
 * Commands
\*********************************************************************************************/

boolean LightColorEntry(char *buffer, uint8_t buffer_length)
{
  uint8_t entry_type = 0;                   // Decimal
  char scolor[10];
  char *p;
  char *str;

  if (buffer[0] == '#') {                   // Optional hexadecimal entry
    buffer++;
    buffer_length--;
  }
  uint8_t size = (light_subtype > sizeof(light_entry_color)) ? sizeof(light_entry_color) : light_subtype;
  if (strstr(buffer, ",")) {                // Decimal entry
    int8_t i = 0;
    for (str = strtok_r(buffer, ",", &p); str && i < size; str = strtok_r(NULL, ",", &p)) {
      light_entry_color[i++] = atoi(str);
    }
    entry_type = (size == i) ? 2 : 0;       // Decimal
  }
  else if ((2 * size) == buffer_length) {   // Hexadecimal entry
    for (byte i = 0; i < size; i++) {
      strlcpy(scolor, buffer + (i *2), 3);
      light_entry_color[i] = (uint8_t)strtol(scolor, &p, 16);
    }
    entry_type = 1;                         // Hexadecimal
  }
  if (entry_type) {
    Settings.flag.decimal_text = entry_type -1;
  }
  return (entry_type);
}

boolean LightCommand(char *type, uint16_t index, char *dataBuf, uint16_t data_len, int16_t payload)
{
  boolean serviced = true;
  boolean coldim = false;
  boolean valid_entry = false;
  char scolor[25];

  if ((light_subtype > 1) && !strcasecmp_P(type, PSTR(D_CMND_COLOR)) && (index > 0) && (index <= 5)) {
    if (data_len > 0) {
      valid_entry = LightColorEntry(dataBuf, data_len);
      if (valid_entry) {
        if (1 == index) {
          memcpy(light_current_color, light_entry_color, sizeof(light_current_color));
          LightSetColor();
          coldim = true;
        } else {
          memcpy(Settings.ws_color + 3*(index -2), light_entry_color, 3);
        }
      }
    }
    if (!valid_entry && (1 == index)) {
      snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("{\"" D_CMND_COLOR "\":\"%s\"}"), LightGetColor(0, scolor));
    }
    if (index > 1) {
      scolor[0] = '\0';
      for (byte i = 0; i < 3; i++) {
        if (Settings.flag.decimal_text) {
          snprintf_P(scolor, 25, PSTR("%s%s%d"), scolor, (i > 0) ? "," : "", Settings.ws_color[index -2][i]);
        } else {
          snprintf_P(scolor, 25, PSTR("%s%02X"), scolor, Settings.ws_color[index -2][i]);
        }
      }
      snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("{\"" D_CMND_COLOR "%d\":\"%s\"}"), index, scolor);
    }
  }
#ifdef USE_WS2812  //  ***********************************************************************
  else if ((11 == light_type) && !strcasecmp_P(type, PSTR(D_CMND_LED)) && (index > 0) && (index <= Settings.led_pixels)) {
    if (data_len > 0) {
      if (LightColorEntry(dataBuf, data_len)) {
        Ws2812SetColor(index, light_entry_color[0], light_entry_color[1], light_entry_color[2]);
      }
    }
    snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("{\"" D_CMND_LED "%d\":\"%s\"}"), index, Ws2812GetColor(index, scolor));
  }
  else if ((11 == light_type) && !strcasecmp_P(type, PSTR(D_CMND_PIXELS))) {
    if ((payload > 0) && (payload <= WS2812_MAX_LEDS)) {
      Settings.led_pixels = payload;
      Ws2812Clear();
      light_update = 1;
    }
    snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("{\"" D_CMND_PIXELS "\":%d}"), Settings.led_pixels);
  }
  else if ((11 == light_type) && !strcasecmp_P(type, PSTR(D_CMND_WIDTH)) && (index > 0) && (index <= 4)) {
    if (1 == index) {
      if ((payload >= 0) && (payload <= 4)) {
        Settings.led_width = payload;
      }
      snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("{\"" D_CMND_WIDTH "\":%d}"), Settings.led_width);
    } else {
      if ((payload > 0) && (payload < 32)) {
        Settings.ws_width[index -2] = payload;
      }
      snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("{\"" D_CMND_WIDTH "%d\":%d}"), index, Settings.ws_width[index -2]);
    }
  }
  else if ((11 == light_type) && !strcasecmp_P(type, PSTR(D_CMND_SCHEME))) {
    if ((payload >= 0) && (payload <= 9)) {
      Settings.led_scheme = payload;
      if (1 == Settings.led_scheme) {
        light_wakeup_active = 3;
      }
      ExecuteCommandPower(devices_present, 1);
      strip_timer_counter = 0;
    }
    snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("{\"" D_CMND_SCHEME "\":%d}"), Settings.led_scheme);
  }
#endif  // USE_WS2812 ************************************************************************
  else if (!strcasecmp_P(type, PSTR(D_CMND_WAKEUP))) {
    if ((payload >= 0) && (payload <= 100)) {
      Settings.led_dimmer = payload;
    }
    light_wakeup_active = 3;
    Settings.led_scheme = 1;
    ExecuteCommandPower(devices_present, 1);
    snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("{\"" D_CMND_WAKEUP "\":\"" D_STARTED "\"}"));
  }
  else if (!strcasecmp_P(type, PSTR(D_CMND_COLORTEMPERATURE)) && ((2 == light_subtype) || (5 == light_subtype))) { // ColorTemp
    if ((payload >= 153) && (payload <= 500)) {  // https://developers.meethue.com/documentation/core-concepts
      LightSetColorTemp(payload);
      coldim = true;
    } else {
      snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("{\"" D_CMND_COLORTEMPERATURE "\":%d}"), LightGetColorTemp());
    }
  }
  else if (!strcasecmp_P(type, PSTR(D_CMND_DIMMER))) {
    if ((payload >= 0) && (payload <= 100)) {
      Settings.led_dimmer = payload;
      coldim = true;
    } else {
      snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("{\"" D_CMND_DIMMER "\":%d}"), Settings.led_dimmer);
    }
  }
  else if (!strcasecmp_P(type, PSTR(D_CMND_LEDTABLE))) {
    if ((payload >= 0) && (payload <= 2)) {
      switch (payload) {
      case 0: // Off
      case 1: // On
        Settings.led_table = payload;
        break;
      case 2: // Toggle
        Settings.led_table ^= 1;
        break;
      }
      light_update = 1;
    }
    snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("{\"" D_CMND_LEDTABLE "\":\"%s\"}"), GetStateText(Settings.led_table));
  }
  else if (!strcasecmp_P(type, PSTR(D_CMND_FADE))) {
    switch (payload) {
    case 0: // Off
    case 1: // On
      Settings.led_fade = payload;
      break;
    case 2: // Toggle
      Settings.led_fade ^= 1;
      break;
    }
    snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("{\"" D_CMND_FADE "\":\"%s\"}"), GetStateText(Settings.led_fade));
  }
  else if (!strcasecmp_P(type, PSTR(D_CMND_SPEED))) {  // 1 - fast, 8 - slow
    if ((payload > 0) && (payload <= 8)) {
      Settings.led_speed = payload;
    }
    snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("{\"" D_CMND_SPEED "\":%d}"), Settings.led_speed);
  }
  else if (!strcasecmp_P(type, PSTR(D_CMND_WAKEUPDURATION))) {
    if ((payload > 0) && (payload < 3001)) {
      Settings.led_wakeup = payload;
      light_wakeup_active = 0;
    }
    snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("{\"" D_CMND_WAKEUPDURATION "\":%d}"), Settings.led_wakeup);
  }
  else if (!strcasecmp_P(type, PSTR("UNDOCA"))) {  // Theos legacy status
    LightGetColor(1, scolor);
    scolor[6] = '\0';  // RGB only
    snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s, %d, %d, %d, %d, %d"),
      scolor, Settings.led_fade, Settings.led_table, Settings.led_scheme, Settings.led_speed, Settings.led_width);
    MqttPublishPrefixTopic_P(1, type);
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

