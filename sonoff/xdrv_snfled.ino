/*
  xdrv_snfled.ino - sonoff led support for Sonoff-Tasmota

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
 * Sonoff B1, AiLight, Sonoff Led and BN-SZ01
 * 
 * sfl_flg  Module        Color  ColorTemp
 * 1        Sonoff BN-SZ  W      no
 * 2        Sonoff Led    CW     yes
 * 3        not used
 * 4        AiLight       RGBW   no
 * 5        Sonoff B1     RGBCW  yes
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

uint8_t sl_power;
uint8_t sl_any;
uint8_t sl_wakeupActive = 0;
uint8_t sl_wakeupDimmer = 0;
uint16_t sl_wakeupCntr = 0;

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
  pin[GPIO_WS2812] = 99;    // I do not allow both Sonoff Led AND WS2812 led
  if (sfl_flg < 4) {
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
  } else {
    sl_pdi = pin[GPIO_DI];
    sl_pdcki = pin[GPIO_DCKI];
    
    pinMode(sl_pdi, OUTPUT);
    pinMode(sl_pdcki, OUTPUT);
    digitalWrite(sl_pdi, LOW);
    digitalWrite(sl_pdcki, LOW);

    sl_my92x1_init();
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

void sl_prepPower(char *svalue, uint16_t ssvalue)
{
  char scolor[11];
  
//  do_cmnd_power(index, (sysCfg.led_dimmer[0]>0));
  if (sysCfg.led_dimmer[0] && !(power&1)) {
    do_cmnd_power(1, 7);  // No publishPowerState
  }
  else if (!sysCfg.led_dimmer[0] && (power&1)) {
    do_cmnd_power(1, 6);  // No publishPowerState
  }
#ifdef USE_DOMOTICZ
  mqtt_publishDomoticzPowerState(1);
#endif  // USE_DOMOTICZ
  if (sfl_flg > 1) {
    snprintf_P(svalue, ssvalue, PSTR("{\"POWER\":\"%s\", \"Dimmer\":%d, \"Color\":\"%s\"}"),
      getStateText(power &1), sysCfg.led_dimmer[0], sl_getColor(scolor));
  } else {
    snprintf_P(svalue, ssvalue, PSTR("{\"POWER\":\"%s\", \"Dimmer\":%d}"),
      getStateText(power &1), sysCfg.led_dimmer[0]);
  }
}

void sl_setPower(uint8_t power)
{
  sl_power = power &1;
  if (sl_wakeupActive) {
    sl_wakeupActive--;
  }
  sl_animate();
}

void sl_animate()
{
// {"Wakeup":"Done"}
  char svalue[32];  // was MESSZ
  uint8_t fadeValue;
  uint8_t cur_col[5];
  
  if (0 == sl_power) {  // Power Off
    for (byte i = 0; i < sfl_flg; i++) {
      sl_tcolor[i] = 0;
    }
  }
  else {
    if (!sl_wakeupActive) {  // Power On
      sl_setDim(sysCfg.led_dimmer[0]);
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
    } else {   // Power On using wake up duration
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
          snprintf_P(svalue, sizeof(svalue), PSTR("{\"Wakeup\":\"Done\"}"));
          mqtt_publish_topic_P(2, PSTR("WAKEUP"), svalue);
          sl_wakeupActive = 0;
        }
      }
    }
  }
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
      if (sfl_flg < 4) {
        if (pin[GPIO_PWM1 +i] < 99) {
          analogWrite(pin[GPIO_PWM1 +i], cur_col[i] * (PWM_RANGE / 255));
        }
      }
    }
    if (sfl_flg > 3) {
      sl_my92x1_duty(cur_col[0], cur_col[1], cur_col[2], cur_col[3], cur_col[4]);
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
  char svalue[MESSZ];
  HsbColor hsb;
  
/*
  char log[LOGSZ];
  char stemp1[10];
  char stemp2[10];
  char stemp3[10];
  dtostrf(hue, 1, 3, stemp1);
  dtostrf(sat, 1, 3, stemp2);
  dtostrf(bri, 1, 3, stemp3);
  snprintf_P(log, sizeof(log), PSTR("HUE: Set Hue %s, Sat %s, Bri %s, Ct %d"), stemp1, stemp2, stemp3, ct);
  addLog(LOG_LEVEL_DEBUG, log);
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
    sl_prepPower(svalue, sizeof(svalue));
    mqtt_publish_topic_P(5, "COLOR", svalue);
  } else {
    uint8_t tmp = (uint8_t)(bri * 100);
    sysCfg.led_dimmer[0] = tmp;
    if (2 == sfl_flg) {
      if (ct > 0) {
        sl_setColorTemp(ct);
      }
      sl_prepPower(svalue, sizeof(svalue));
      mqtt_publish_topic_P(5, "COLOR", svalue);
    } else {
      sl_prepPower(svalue, sizeof(svalue));
      mqtt_publish_topic_P(5, "DIMMER", svalue);
    }
  }
}

/*********************************************************************************************\
 * Commands
\*********************************************************************************************/

boolean sl_command(char *type, uint16_t index, char *dataBufUc, uint16_t data_len, int16_t payload, char *svalue, uint16_t ssvalue)
{
  boolean serviced = true;
  boolean coldim = false;
  char scolor[11];
  char *p;

  if ((sfl_flg > 1) && !strcmp_P(type,PSTR("COLOR"))) {
    if ((2 * sfl_flg) == data_len) {
      for (byte i = 0; i < sfl_flg; i++) {
        strlcpy(scolor, dataBufUc + (i *2), 3);
        sl_dcolor[i] = (uint8_t)strtol(scolor, &p, 16);
      }
      sl_setColor();
      coldim = true;
    } else {
      snprintf_P(svalue, ssvalue, PSTR("{\"Color\":\"%s\"}"), sl_getColor(scolor));
    }
  }
  else if (!strcmp_P(type,PSTR("CT")) && ((2 == sfl_flg) || (5 == sfl_flg))) { // ColorTemp
    if ((payload >= 153) && (payload <= 500)) {  // https://developers.meethue.com/documentation/core-concepts
      sl_setColorTemp(payload);
      coldim = true;
    } else {
      snprintf_P(svalue, ssvalue, PSTR("{\"CT\":%d}"), sl_getColorTemp());
    }
  }
  else if (!strcmp_P(type,PSTR("DIMMER"))) {
    if ((payload >= 0) && (payload <= 100)) {
      sysCfg.led_dimmer[0] = payload;
      coldim = true;
    } else {
      snprintf_P(svalue, ssvalue, PSTR("{\"Dimmer\":%d}"), sysCfg.led_dimmer[0]);
    }
  }
  else if (!strcmp_P(type,PSTR("LEDTABLE"))) {
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
    snprintf_P(svalue, ssvalue, PSTR("{\"LedTable\":\"%s\"}"), getStateText(sysCfg.led_table));
  }
  else if (!strcmp_P(type,PSTR("FADE"))) {
    switch (payload) {
    case 0: // Off
    case 1: // On
      sysCfg.led_fade = payload;
      break;
    case 2: // Toggle
      sysCfg.led_fade ^= 1;
      break;
    }
    snprintf_P(svalue, ssvalue, PSTR("{\"Fade\":\"%s\"}"), getStateText(sysCfg.led_fade));
  }
  else if (!strcmp_P(type,PSTR("SPEED"))) {  // 1 - fast, 8 - slow
    if ((payload > 0) && (payload <= 8)) {
      sysCfg.led_speed = payload;
    }
    snprintf_P(svalue, ssvalue, PSTR("{\"Speed\":%d}"), sysCfg.led_speed);
  }
  else if (!strcmp_P(type,PSTR("WAKEUPDURATION"))) {
    if ((payload > 0) && (payload < 3001)) {
      sysCfg.led_wakeup = payload;
      sl_wakeupActive = 0;
    }
    snprintf_P(svalue, ssvalue, PSTR("{\"WakeUpDuration\":%d}"), sysCfg.led_wakeup);
  }
  else if (!strcmp_P(type,PSTR("WAKEUP"))) {
    sl_wakeupActive = 3;
    do_cmnd_power(1, 1);
    snprintf_P(svalue, ssvalue, PSTR("{\"Wakeup\":\"Started\"}"));
  }
  else {
    serviced = false;  // Unknown command
  }
  if (coldim) {
    sl_prepPower(svalue, ssvalue);
  }
  return serviced;
}

