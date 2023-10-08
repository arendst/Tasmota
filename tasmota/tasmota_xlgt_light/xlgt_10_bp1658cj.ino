/*
  xlgt_10_bp1658cj.ino - bp1658cj five channel led support for Tasmota

  Copyright (C) 2022  Theo Arends and Cossid

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
#ifdef USE_BP1658CJ
/*********************************************************************************************\
 * BP1658CJ RGBCW Led bulbs like the Orein OS0100411267 Bulb
 *
 * Orein OS0100411267 Bulb
 * 
 * 
 * FIXME
 * {"NAME":"Orein OS0100411267 Bulb","GPIO":[0,0,0,0,9129,9088,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],"FLAG":0,"BASE":1}
\*********************************************************************************************/

#define XLGT_10                     10

// 10 = identification | 00 = Standby | 0000 = start at OUT1/5
#define BP1658CJ_ADDR_STANDBY       0x80 // 10000000 0x80
// 10 = identification | 01 = 3 channels (RGB) | 0000 = start at OUT1/5
//#define BP1658CJ_ADDR_START_3CH     0x90 // 10010000 0xC8
// 10 = identification | 10 = 2 channels (CW) | 0000 = start at OUT1/5
//#define BP1658CJ_ADDR_START_2CH     0xA0 // 10100000 0xA0
// 10 = identification | 11 = 5 channels (RGB+CW) | 0000 = start at OUT1/5
#define BP1658CJ_ADDR_START_5CH     0xB0 // 10110000 0xB0

// Current values
// 0x0 // 0000 RGB 0mA  | CW 0mA
// 0x1 // 0001 RGB 10mA  | CW 5mA
// 0x2 // 0010 RGB 20mA  | CW 10mA
// 0x3 // 0011 RGB 30mA  | CW 15mA
// 0x4 // 0100 RGB 40mA  | CW 20mA
// 0x5 // 0101 RGB 50mA  | CW 25mA
// 0x6 // 0110 RGB 60mA  | CW 30mA
// 0x7 // 0111 RGB 70mA  | CW 35mA
// 0x8 // 1000 RGB 80mA  | CW 40mA
// 0x9 // 1001 RGB 900mA | CW 45mA
// 0xA // 1010 RGB 100mA | CW 50mA
// 0xB // 1011 RGB 110mA | CW 55mA
// 0xC // 1100 RGB 120mA | CW 60mA
// 0xD // 1101 RGB 130mA | CW 65mA
// 0xE // 1110 RGB 140mA | CW 70mA
// 0xF // 1111 RGB 150mA | CW 75mA

struct BP1658CJ {
  uint8_t clk = 0;
  uint8_t data = 0;
  uint8_t current;
} Bp1658cj;

/*********************************************************************************************\
 * BP1658CJ code - inspired by Bp5758d/SM2335
\*********************************************************************************************/
const uint8_t BP1658CJ_DELAY = 2;

void BP1658CJInit(void) {
  pinMode(Bp1658cj.data, OUTPUT);
  pinMode(Bp1658cj.clk, OUTPUT);
  BP1658CJStop();
}

void BP1658CJWrite(uint8_t value) {
  for (int bit_idx = 7; bit_idx >= 0; bit_idx--) {
        bool bit = bitRead(value, bit_idx);
        digitalWrite(Bp1658cj.data, bit);
        delayMicroseconds(BP1658CJ_DELAY);
        digitalWrite(Bp1658cj.clk, HIGH);
        delayMicroseconds(BP1658CJ_DELAY);
        digitalWrite(Bp1658cj.clk, LOW);
        delayMicroseconds(BP1658CJ_DELAY);
      }
  // Wait for ACK
  pinMode(Bp1658cj.data, INPUT);
  digitalWrite(Bp1658cj.clk, HIGH);
  delayMicroseconds(BP1658CJ_DELAY);
  digitalWrite(Bp1658cj.clk, LOW);
  delayMicroseconds(BP1658CJ_DELAY);
  pinMode(Bp1658cj.data, OUTPUT);
}

void BP1658CJStart(uint8_t addr) {
  digitalWrite(Bp1658cj.data, LOW);
  delayMicroseconds(BP1658CJ_DELAY);
  digitalWrite(Bp1658cj.clk, LOW);
  delayMicroseconds(BP1658CJ_DELAY);
  BP1658CJWrite(addr);
}

void BP1658CJStop(void) {
  digitalWrite(Bp1658cj.clk, HIGH);
  delayMicroseconds(BP1658CJ_DELAY);
  digitalWrite(Bp1658cj.data, HIGH);
  delayMicroseconds(BP1658CJ_DELAY);
}

/********************************************************************************************/

bool BP1658CJSetChannels(void) {  
  uint16_t *cur_col_10 = (uint16_t*)XdrvMailbox.command;
    
  // If we receive 0 for all channels, we'll assume that the lightbulb is off, and activate BP1658CJ's standby mode.
  if (cur_col_10[0] == 0 && cur_col_10[1] == 0 && cur_col_10[2] == 0 && cur_col_10[3] == 0 && cur_col_10[4] == 0) {
    BP1658CJStart(BP1658CJ_ADDR_STANDBY);
    // Clear all remaining data.  This clears out Current, Red, Green, Blue, Cold White, Warm White.
    for (int i = 0; i < 11; i++) {
      BP1658CJWrite(0);
    }
    BP1658CJStop();
    return true;
  }
    
  // Write the header activating all 5 channels
  BP1658CJStart(BP1658CJ_ADDR_START_5CH);
  // Set the current defined in ModuleSelected.
  BP1658CJWrite(Bp1658cj.current);
  // Set RGB and CW grayscale.
  for (int i = 0; i < 5; i++) {
    BP1658CJWrite((uint8_t)(cur_col_10[i] & 0x1F));
    BP1658CJWrite((uint8_t)(cur_col_10[i] >> 5));
  }
  BP1658CJStop();
  return true;
}

void BP1658CJModuleSelected(void)
{
  if (PinUsed(GPIO_BP1658CJ_CLK) && PinUsed(GPIO_BP1658CJ_DAT, GPIO_ANY)) {
    Bp1658cj.clk = Pin(GPIO_BP1658CJ_CLK);
    Bp1658cj.data = Pin(GPIO_BP1658CJ_DAT, GPIO_ANY);
    // See #define MAX_BP1658CJ_DAT 16 in tasmota_template.h
    int currentDat = GetPin(Bp1658cj.data) - AGPIO(GPIO_BP1658CJ_DAT);  // 0 .. 15
    // Split RGB and CW current.
    Bp1658cj.current = (currentDat << 4) | currentDat;

    BP1658CJInit();

    TasmotaGlobal.light_type = LT_RGBWC;
    TasmotaGlobal.light_driver = XLGT_10;
    AddLog(LOG_LEVEL_DEBUG, PSTR("LGT: BP1658CJ Found"));
  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xlgt10(uint32_t function)
{
  bool result = false;

  switch (function) {
    case FUNC_SET_CHANNELS:
      result = BP1658CJSetChannels();
      break;
    case FUNC_MODULE_INIT:
      BP1658CJModuleSelected();
      break;
  }
  return result;
}

#endif  // USE_BP1658CJ
#endif  // USE_LIGHT
