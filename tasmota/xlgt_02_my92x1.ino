/*
  xlgt_02_my92x1.ino - led support for Tasmota

  Copyright (C) 2020  Theo Arends

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
#ifdef USE_MY92X1
/*********************************************************************************************\
 * Sonoff B1 and AiLight inspired by OpenLight https://github.com/icamgo/noduino-sdk
\*********************************************************************************************/

#define XLGT_02             2

struct MY92X1 {
  uint8_t pdi_pin = 0;
  uint8_t pdcki_pin = 0;
  uint8_t model = 0;
} My92x1;

extern "C" {
  void os_delay_us(unsigned int);
}

void LightDiPulse(uint8_t times)
{
  for (uint32_t i = 0; i < times; i++) {
    digitalWrite(My92x1.pdi_pin, HIGH);
    digitalWrite(My92x1.pdi_pin, LOW);
  }
}

void LightDckiPulse(uint8_t times)
{
  for (uint32_t i = 0; i < times; i++) {
    digitalWrite(My92x1.pdcki_pin, HIGH);
    digitalWrite(My92x1.pdcki_pin, LOW);
  }
}

void LightMy92x1Write(uint8_t data)
{
  for (uint32_t i = 0; i < 4; i++) {     // Send 8bit Data
    digitalWrite(My92x1.pdcki_pin, LOW);
    digitalWrite(My92x1.pdi_pin, (data & 0x80));
    digitalWrite(My92x1.pdcki_pin, HIGH);
    data = data << 1;
    digitalWrite(My92x1.pdi_pin, (data & 0x80));
    digitalWrite(My92x1.pdcki_pin, LOW);
    digitalWrite(My92x1.pdi_pin, LOW);
    data = data << 1;
  }
}

void LightMy92x1Init(void)
{
  uint8_t chips[3] = { 1, 2, 2 };

  LightDckiPulse(chips[My92x1.model] * 32);  // Clear all duty register
  os_delay_us(12);                      // TStop > 12us.
  // Send 12 DI pulse, after 6 pulse's falling edge store duty data, and 12
  // pulse's rising edge convert to command mode.
  LightDiPulse(12);
  os_delay_us(12);                      // Delay >12us, begin send CMD data
  for (uint32_t n = 0; n < chips[My92x1.model]; n++) {  // Send CMD data
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
  uint8_t channels[3] = { 4, 6, 6 };

  uint8_t duty[3][6] = {{ duty_r, duty_g, duty_b, duty_w, 0, 0 },             // Definition for RGBW channels
                        { duty_w, duty_c, 0, duty_g, duty_r, duty_b },        // Definition for RGBWC channels
                        { duty_r, duty_g, duty_b, duty_w, duty_w, duty_w }};  // Definition for RGBWWW channels as used in Lohas which uses up to 3 CW channels

  os_delay_us(12);                      // TStop > 12us.
  for (uint32_t channel = 0; channel < channels[My92x1.model]; channel++) {
    LightMy92x1Write(duty[My92x1.model][channel]);  // Send 8bit Data
  }
  os_delay_us(12);                      // TStart > 12us. Ready for send DI pulse.
  LightDiPulse(8);                      // Send 8 DI pulse. After 8 pulse falling edge, store old data.
  os_delay_us(12);                      // TStop > 12us.
}

/********************************************************************************************/

bool My92x1SetChannels(void)
{
  uint8_t *cur_col = (uint8_t*)XdrvMailbox.data;

  LightMy92x1Duty(cur_col[0], cur_col[1], cur_col[2], cur_col[3], cur_col[4]);

  return true;
}

void My92x1ModuleSelected(void)
{
  if ((pin[GPIO_DCKI] < 99) && (pin[GPIO_DI] < 99)) {
    My92x1.pdi_pin = pin[GPIO_DI];
    My92x1.pdcki_pin = pin[GPIO_DCKI];

    pinMode(My92x1.pdi_pin, OUTPUT);
    pinMode(My92x1.pdcki_pin, OUTPUT);
    digitalWrite(My92x1.pdi_pin, LOW);
    digitalWrite(My92x1.pdcki_pin, LOW);

    My92x1.model = 2;
    light_type = LT_RGBW;                    // RGBW (2 chips) as used in Lohas
    if (AILIGHT == my_module_type) {         // RGBW (1 chip) as used in Ailight
      My92x1.model = 0;
//      light_type = LT_RGBW;
    }
    else if (SONOFF_B1 == my_module_type) {  // RGBWC (2 chips) as used in Sonoff B1
      My92x1.model = 1;
      light_type = LT_RGBWC;
    }

    LightMy92x1Init();

    light_flg = XLGT_02;
    AddLog_P2(LOG_LEVEL_DEBUG, PSTR("DBG: MY29x1 Found"));
  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xlgt02(uint8_t function)
{
  bool result = false;

  switch (function) {
    case FUNC_SET_CHANNELS:
      result = My92x1SetChannels();
      break;
    case FUNC_MODULE_INIT:
      My92x1ModuleSelected();
      break;
  }
  return result;
}

#endif  // USE_MY92X1
#endif  // USE_LIGHT