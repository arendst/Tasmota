/*
  xlgt_03_sm16716.ino - sm16716 three channel led support for Tasmota

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
#ifdef USE_SM16716
/*********************************************************************************************\
 * SM16716 - Controlling RGB over a synchronous serial line
 * Copyright (C) 2021  Gabor Simon
 *
 * Source: https://community.home-assistant.io/t/cheap-uk-wifi-bulbs-with-tasmota-teardown-help-tywe3s/40508/27
\*********************************************************************************************/

#define XLGT_03             3

#define D_LOG_SM16716       "SM16716: "

struct SM16716 {
  int8_t pin_clk = 0;
  int8_t pin_dat = 0;
  int8_t pin_sel = 0;
  bool enabled = false;
} Sm16716;

void SM16716_SendBit(uint8_t v)
{
  /* NOTE:
   * According to the spec sheet, max freq is 30 MHz, that is 16.6 ns per high/low half of the
   * clk square wave. That is less than the overhead of 'digitalWrite' at this clock rate,
   * so no additional delays are needed yet. */

  digitalWrite(Sm16716.pin_dat, (v != 0) ? HIGH : LOW);
  //delayMicroseconds(1);
  digitalWrite(Sm16716.pin_clk, HIGH);
  //delayMicroseconds(1);
  digitalWrite(Sm16716.pin_clk, LOW);
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
  if (Sm16716.pin_sel >= 0) {
    bool should_enable = (duty_r | duty_g | duty_b);
    if (!Sm16716.enabled && should_enable) {
      DEBUG_DRIVER_LOG(PSTR(D_LOG_SM16716 "turning color on"));
      Sm16716.enabled = true;
      digitalWrite(Sm16716.pin_sel, HIGH);
      // in testing I found it takes a minimum of ~380us to wake up the chip
      // tested on a Merkury RGBW with an SM726EB
      delayMicroseconds(1000);
      SM16716_Init();
    }
    else if (Sm16716.enabled && !should_enable) {
      DEBUG_DRIVER_LOG(PSTR(D_LOG_SM16716 "turning color off"));
      Sm16716.enabled = false;
      digitalWrite(Sm16716.pin_sel, LOW);
    }
  }
  DEBUG_DRIVER_LOG(PSTR(D_LOG_SM16716 "Update; rgb=%02x%02x%02x"), duty_r, duty_g, duty_b);

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

/*
bool SM16716_ModuleSelected(void)
{
  Sm16716.pin_clk = Pin(GPIO_SM16716_CLK);
  Sm16716.pin_dat = Pin(GPIO_SM16716_DAT);
  Sm16716.pin_sel = Pin(GPIO_SM16716_SEL);
  DEBUG_DRIVER_LOG(PSTR(D_LOG_SM16716 "ModuleSelected; clk_pin=%d, dat_pin=%d)"), Sm16716.pin_clk, Sm16716.pin_dat);
  return (Sm16716.pin_clk >= 0) && (Sm16716.pin_dat >= 0);
}
*/

void SM16716_Init(void)
{
  for (uint32_t t_init = 0; t_init < 50; ++t_init) {
    SM16716_SendBit(0);
  }
}

/********************************************************************************************/

bool Sm16716SetChannels(void)
{
/*
  // handle any PWM pins, skipping the first 3 values for sm16716
  for (uint32_t i = 3; i < Light.subtype; i++) {
    if (PinUsed(GPIO_PWM1, i-3)) {
      //AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_APPLICATION "Cur_Col%d 10 bits %d, Pwm%d %d"), i, cur_col[i], i+1, curcol);
      AnalogWrite(Pin(GPIO_PWM1, i-3), bitRead(TasmotaGlobal.pwm_inverted, i-3) ? Settings->pwm_range - cur_col_10bits[i] : cur_col_10bits[i]);
    }
  }
*/
  // handle sm16716 update
  uint8_t *cur_col = (uint8_t*)XdrvMailbox.data;

  SM16716_Update(cur_col[0], cur_col[1], cur_col[2]);

  return true;
}

void Sm16716ModuleSelected(void)
{
  if (PinUsed(GPIO_SM16716_CLK) && PinUsed(GPIO_SM16716_DAT)) {
    Sm16716.pin_clk = Pin(GPIO_SM16716_CLK);
    Sm16716.pin_dat = Pin(GPIO_SM16716_DAT);
    Sm16716.pin_sel = Pin(GPIO_SM16716_SEL);

    // init sm16716
    pinMode(Sm16716.pin_clk, OUTPUT);
    digitalWrite(Sm16716.pin_clk, LOW);

    pinMode(Sm16716.pin_dat, OUTPUT);
    digitalWrite(Sm16716.pin_dat, LOW);

    if (Sm16716.pin_sel >= 0) {
      pinMode(Sm16716.pin_sel, OUTPUT);
      digitalWrite(Sm16716.pin_sel, LOW);
      // no need to call SM16716_Init here, it will be called after sel goes HIGH
    } else {
      // no sel pin means you have an 'always on' chip, so init right away
      SM16716_Init();
    }

    LightPwmOffset(LST_RGB);  // Handle any PWM pins, skipping the first 3 color values for sm16716
    TasmotaGlobal.light_type += LST_RGB;    // Add RGB to be controlled by sm16716
    TasmotaGlobal.light_driver = XLGT_03;
    AddLog(LOG_LEVEL_DEBUG, PSTR("DBG: SM16716 Found"));
  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xlgt03(uint32_t function)
{
  bool result = false;

  switch (function) {
    case FUNC_SET_CHANNELS:
      result = Sm16716SetChannels();
      break;
    case FUNC_MODULE_INIT:
      Sm16716ModuleSelected();
      break;
  }
  return result;
}

#endif  // USE_SM16716
#endif  // USE_LIGHT

