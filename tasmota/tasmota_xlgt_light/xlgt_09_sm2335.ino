/*
  xlgt_09_sm2335.ino - sm2335 five channel led support for Tasmota

  Copyright (C) 2021  Theo Arends and Cossid

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
#ifdef USE_SM2335
/*********************************************************************************************\
 * SM2335 RGBCW Led bulbs like the SwitchBot Color Bulb
 *
 * SwitchBot Color Bulb
 * {"NAME":"SwitchBot Color Bulb","GPIO":[0,0,0,0,9129,9088,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],"FLAG":0,"BASE":1}
\*********************************************************************************************/

#define XLGT_09                   9

// 11 = identification | 0 = reserved | 00 = Standby | 000 = start at OUT1/5
#define SM2335_ADDR_STANDBY       0xC0 // 11000000 0xC0
// 11 = identification | 0 = reserved | 01 = 3 channels (RGB) | 000 = start at OUT1/5
#define SM2335_ADDR_START_3CH     0xC8 // 11001000 0xC8
// 11 = identification | 0 = reserved | 10 = 2 channels (CW) | 000 = start at OUT1/5
#define SM2335_ADDR_START_2CH     0xD0 // 11010000 0xD0
// 11 = identification | 0 = reserved | 11 = 5 channels (RGB+CW) | 000 = start at OUT1/5
#define SM2335_ADDR_START_5CH     0xD8 // 11011000 0xD8

// Current values
#define SM2335_RGB_10MA_CW_5MA    0x0 // 0000 RGB 10mA  | CW 5mA
#define SM2335_RGB_20MA_CW_10MA   0x1 // 0001 RGB 20mA  | CW 10mA
#define SM2335_RGB_30MA_CW_15MA   0x2 // 0010 RGB 30mA  | CW 15mA
#define SM2335_RGB_40MA_CW_20MA   0x3 // 0011 RGB 40mA  | CW 20mA
#define SM2335_RGB_50MA_CW_25MA   0x4 // 0100 RGB 50mA  | CW 25mA
#define SM2335_RGB_60MA_CW_30MA   0x5 // 0101 RGB 60mA  | CW 30mA
#define SM2335_RGB_70MA_CW_35MA   0x6 // 0110 RGB 70mA  | CW 35mA
#define SM2335_RGB_80MA_CW_40MA   0x7 // 0111 RGB 80mA  | CW 40mA
#define SM2335_RGB_90MA_CW_45MA   0x8 // 1000 RGB 90mA  | CW 45mA
#define SM2335_RGB_100MA_CW_50MA  0x9 // 1001 RGB 100mA | CW 50mA
#define SM2335_RGB_110MA_CW_55MA  0xA // 1010 RGB 110mA | CW 55mA
#define SM2335_RGB_120MA_CW_60MA  0xB // 1011 RGB 120mA | CW 60mA
#define SM2335_RGB_130MA_CW_65MA  0xC // 1100 RGB 130mA | CW 65mA
#define SM2335_RGB_140MA_CW_70MA  0xD // 1101 RGB 140mA | CW 70mA
#define SM2335_RGB_150MA_CW_75MA  0xE // 1110 RGB 150mA | CW 75mA
#define SM2335_RGB_160MA_CW_80MA  0xF // 1111 RGB 160mA | CW 80mA

enum Sm2335Current { SM2335_10_5, SM2335_20_10, SM2335_30_15, SM2335_40_20, SM2335_50_25,
  SM2335_60_30, SM2335_70_35, SM2335_80_40, SM2335_90_45, SM2335_100_50, SM2335_110_55,
  SM2335_120_60, SM2335_130_65, SM2335_140_70, SM2335_150_75, SM2335_160_80 };

struct SM2335 {
  uint8_t clk = 0;
  uint8_t data = 0;
  uint8_t currentValue;
  uint8_t currentEnum = SM2335_50_25;
} Sm2335;

/*********************************************************************************************\
 * SM2335 code - inspired by Bp5758d/SM2135
\*********************************************************************************************/
const uint8_t SM2335_DELAY = 2;

void SM2335Init(void) {
  pinMode(Sm2335.data, OUTPUT);
  pinMode(Sm2335.clk, OUTPUT);
  SM2335Stop();
}

void SM2335Write(uint8_t value) {
  for (int bit_idx = 7; bit_idx >= 0; bit_idx--) {
        bool bit = bitRead(value, bit_idx);
        digitalWrite(Sm2335.data, bit);
        delayMicroseconds(SM2335_DELAY);
        digitalWrite(Sm2335.clk, HIGH);
        delayMicroseconds(SM2335_DELAY);
        digitalWrite(Sm2335.clk, LOW);
        delayMicroseconds(SM2335_DELAY);
      }
  // Wait for ACK
  pinMode(Sm2335.data, INPUT);
  digitalWrite(Sm2335.clk, HIGH);
  delayMicroseconds(SM2335_DELAY);
  digitalWrite(Sm2335.clk, LOW);
  delayMicroseconds(SM2335_DELAY);
  pinMode(Sm2335.data, OUTPUT);
}

void SM2335Start(uint16_t addr) {
  digitalWrite(Sm2335.data, LOW);
  delayMicroseconds(SM2335_DELAY);
  digitalWrite(Sm2335.clk, LOW);
  delayMicroseconds(SM2335_DELAY);
  SM2335Write(addr);
}

void SM2335Stop(void) {
  digitalWrite(Sm2335.clk, HIGH);
  delayMicroseconds(SM2335_DELAY);
  digitalWrite(Sm2335.data, HIGH);
  delayMicroseconds(SM2335_DELAY);
}

/********************************************************************************************/

bool SM2335SetChannels(void) {  
  uint16_t *cur_col_10 = (uint16_t*)XdrvMailbox.command;
    
  // If we receive 0 for all channels, we'll assume that the lightbulb is off, and activate SM2335's standby mode.
  if (cur_col_10[0] == 0 && cur_col_10[1] == 0 && cur_col_10[2] == 0 && cur_col_10[3] == 0 && cur_col_10[4] == 0) {
    SM2335Start(SM2335_ADDR_STANDBY);
    SM2335Write(0); // Current
    SM2335Write(0); // Red 1/2
    SM2335Write(0); // Red 2/2
    SM2335Write(0); // Green 1/2
    SM2335Write(0); // Green 2/2
    SM2335Write(0); // Blue 1/2
    SM2335Write(0); // Blue 2/2
    SM2335Write(0); // Cold White 1/2
    SM2335Write(0); // Cold White 2/2
    SM2335Write(0); // Warm White 1/2
    SM2335Write(0); // WarmW hite 2/2
    SM2335Stop();
    return true;
  }
    
  // Write the header activating all 5 channels
  SM2335Start(SM2335_ADDR_START_5CH);
  // Set the current defined in ModuleSelected.
  SM2335Write(Sm2335.currentValue);
  // Set RGB and CW grayscale.
  SM2335Write((uint8_t)(cur_col_10[0] >> 8));   // Red 1/2
  SM2335Write((uint8_t)(cur_col_10[0] & 0xFF)); // Red 2/2
  SM2335Write((uint8_t)(cur_col_10[1] >> 8));   // Green 1/2
  SM2335Write((uint8_t)(cur_col_10[1] & 0xFF)); // Green 2/2
  SM2335Write((uint8_t)(cur_col_10[2] >> 8));   // Blue 1/2
  SM2335Write((uint8_t)(cur_col_10[2] & 0xFF)); // Blue 2/2
  SM2335Write((uint8_t)(cur_col_10[3] >> 8));   // Cold White 1/2
  SM2335Write((uint8_t)(cur_col_10[3] & 0xFF)); // Cold White 2/2
  SM2335Write((uint8_t)(cur_col_10[4] >> 8));   // Warm White 1/2
  SM2335Write((uint8_t)(cur_col_10[4] & 0xFF)); // Warm White 2/2

  SM2335Stop();
  return true;
}

void SM2335ModuleSelected(void)
{
  if (PinUsed(GPIO_SM2335_CLK) && PinUsed(GPIO_SM2335_DAT, GPIO_ANY)) {
    Sm2335.clk = Pin(GPIO_SM2335_CLK);
    Sm2335.data = Pin(GPIO_SM2335_DAT, GPIO_ANY);

    // See #define MAX_SM2335_DAT 16 in tasmota_template.h
    Sm2335.currentEnum = GetPin(Sm2335.data) - AGPIO(GPIO_SM2335_DAT);  // 0 .. 15

    switch (Sm2335.currentEnum) {
      case SM2335_10_5:
        Sm2335.currentValue = (SM2335_RGB_10MA_CW_5MA << 4) | SM2335_RGB_10MA_CW_5MA;
        break;
      case SM2335_20_10:
        Sm2335.currentValue = (SM2335_RGB_20MA_CW_10MA << 4) | SM2335_RGB_20MA_CW_10MA;
        break;
      case SM2335_30_15:
        Sm2335.currentValue = (SM2335_RGB_30MA_CW_15MA << 4) | SM2335_RGB_30MA_CW_15MA;
        break;
      case SM2335_40_20:
        Sm2335.currentValue = (SM2335_RGB_30MA_CW_15MA << 4) | SM2335_RGB_30MA_CW_15MA;
        break;
      case SM2335_50_25:
        Sm2335.currentValue = (SM2335_RGB_50MA_CW_25MA << 4) | SM2335_RGB_50MA_CW_25MA;
        break;
      case SM2335_60_30:
        Sm2335.currentValue = (SM2335_RGB_60MA_CW_30MA << 4) | SM2335_RGB_60MA_CW_30MA;
        break;
      case SM2335_70_35:
        Sm2335.currentValue = (SM2335_RGB_70MA_CW_35MA << 4) | SM2335_RGB_70MA_CW_35MA;
        break;
      case SM2335_80_40:
        Sm2335.currentValue = (SM2335_RGB_80MA_CW_40MA << 4) | SM2335_RGB_80MA_CW_40MA;
        break;
      case SM2335_90_45:
        Sm2335.currentValue = (SM2335_RGB_90MA_CW_45MA << 4) | SM2335_RGB_90MA_CW_45MA;
        break;
      case SM2335_100_50:
        Sm2335.currentValue = (SM2335_RGB_100MA_CW_50MA << 4) | SM2335_RGB_100MA_CW_50MA;
        break;
      case SM2335_110_55:
        Sm2335.currentValue = (SM2335_RGB_110MA_CW_55MA << 4) | SM2335_RGB_110MA_CW_55MA;
        break;
      case SM2335_120_60:
        Sm2335.currentValue = (SM2335_RGB_120MA_CW_60MA << 4) | SM2335_RGB_120MA_CW_60MA;
        break;
      case SM2335_130_65:
        Sm2335.currentValue = (SM2335_RGB_130MA_CW_65MA << 4) | SM2335_RGB_130MA_CW_65MA;
        break;
      case SM2335_140_70:
        Sm2335.currentValue = (SM2335_RGB_140MA_CW_70MA << 4) | SM2335_RGB_140MA_CW_70MA;
        break;
      case SM2335_150_75:
        Sm2335.currentValue = (SM2335_RGB_150MA_CW_75MA << 4) | SM2335_RGB_150MA_CW_75MA;
        break;
      case SM2335_160_80:
        Sm2335.currentValue = (SM2335_RGB_160MA_CW_80MA << 4) | SM2335_RGB_160MA_CW_80MA;
        break;
    }

    SM2335Init();

    TasmotaGlobal.light_type = LT_RGBWC;
    TasmotaGlobal.light_driver = XLGT_09;
    AddLog(LOG_LEVEL_DEBUG, PSTR("LGT: SM2335 Found"));
  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xlgt09(uint8_t function)
{
  bool result = false;

  switch (function) {
    case FUNC_SET_CHANNELS:
      result = SM2335SetChannels();
      break;
    case FUNC_MODULE_INIT:
      SM2335ModuleSelected();
      break;
  }
  return result;
}

#endif  // USE_SM2335
#endif  // USE_LIGHT
