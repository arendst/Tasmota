/*
  xlgt_08_bp5758d.ino - bp5758d five channel led support for Tasmota

  Copyright (C) 2022  Theo Arends and oomg

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
#ifdef USE_BP5758D
/*********************************************************************************************\
 * BP5758D RGBCW Led bulbs commonly used by Tuya hardware
 *
 * Example configuration for a Fitop RGBCCT filament light bulb:
 * {"NAME":"Fitop Filament RGBCCT","GPIO":[1,1,0,1,1,0,0,0,8416,0,8384,0,0,0],"FLAG":0,"BASE":18,"CMND":"RGBWWTable 255,109,245,255,255"}
\*********************************************************************************************/

#define XLGT_08             8

// Layout: Bits B[7:8]=10 (address selection identification bits), B[5:6] sleep mode if set to 00, B[0:4] Address selection
#define BP5758D_ADDR_SLEEP   0x80  //10 00 xxxx: Set to sleep mode
#define BP5758D_ADDR_SETUP   0x90  //10 01 0000: OUT1-5 enable/disable setup - used during init
#define BP5758D_ADDR_OUT1_CR 0x91  //10 01 0001: OUT1 current range
#define BP5758D_ADDR_OUT2_CR 0x92  //10 01 0010: OUT2 current range
#define BP5758D_ADDR_OUT3_CR 0x93  //10 01 0011: OUT3 current range
#define BP5758D_ADDR_OUT4_CR 0x94  //10 01 0100: OUT4 current range
#define BP5758D_ADDR_OUT5_CR 0x95  //10 01 0101: OUT5 current range
#define BP5758D_ADDR_OUT1_GL 0x96  //10 01 0110: OUT1 gray-scale level
#define BP5758D_ADDR_OUT2_GL 0x98  //10 01 1000: OUT2 gray-scale level
#define BP5758D_ADDR_OUT3_GL 0x9A  //10 01 1010: OUT3 gray-scale level
#define BP5758D_ADDR_OUT4_GL 0x9C  //10 01 1100: OUT4 gray-scale level
#define BP5758D_ADDR_OUT5_GL 0x9E  //10 01 1110: OUT5 gray-scale level

// Output enabled (OUT1-5, represented by lower 5 bits)
#define BP5758D_ENABLE_OUTPUTS_ALL 0x1F
#define BP5758D_DISABLE_OUTPUTS_ALL 0x00

// Current values: Bit 6 to 0 represent 30mA, 32mA, 16mA, 8mA, 4mA, 2mA, 1mA respectively
#define BP5758D_10MA 0x0A // 0 0001010
#define BP5758D_14MA 0x0E // 0 0001110
#define BP5758D_15MA 0x0F // 0 0001111
#define BP5758D_65MA 0x63 // 0 1100011
#define BP5758D_90MA 0x7C // 0 1111100

struct BP5758D {
  uint8_t clk = 0;
  uint8_t data = 0;
  uint8_t current;
} Bp5758d;

/*********************************************************************************************\
 * Bp5758d code - inspired by SM2135 and github.com/dbuezas/esphome-bp5758
\*********************************************************************************************/
const uint8_t BP5758D_DELAY = 2;

void Bp5758dInit(void) {
  pinMode(Bp5758d.data, OUTPUT);
  pinMode(Bp5758d.clk, OUTPUT);
  Bp5758dStop();
}

void Bp5758dWrite(uint8_t value) {
  for (int bit_idx = 7; bit_idx >= 0; bit_idx--) {
        bool bit = bitRead(value, bit_idx);
        digitalWrite(Bp5758d.data, bit);
        delayMicroseconds(BP5758D_DELAY);
        digitalWrite(Bp5758d.clk, HIGH);
        delayMicroseconds(BP5758D_DELAY);
        digitalWrite(Bp5758d.clk, LOW);
        delayMicroseconds(BP5758D_DELAY);
      }
  // Wait for ACK
  pinMode(Bp5758d.data, INPUT);
  digitalWrite(Bp5758d.clk, HIGH);
  delayMicroseconds(BP5758D_DELAY);
  digitalWrite(Bp5758d.clk, LOW);
  delayMicroseconds(BP5758D_DELAY);
  pinMode(Bp5758d.data, OUTPUT);
}

void Bp5758dStart(uint8_t addr) {
  digitalWrite(Bp5758d.data, LOW);
  delayMicroseconds(BP5758D_DELAY);
  digitalWrite(Bp5758d.clk, LOW);
  delayMicroseconds(BP5758D_DELAY);
  Bp5758dWrite(addr);
}

void Bp5758dStop(void) {
  digitalWrite(Bp5758d.clk, HIGH);
  delayMicroseconds(BP5758D_DELAY);
  digitalWrite(Bp5758d.data, HIGH);
  delayMicroseconds(BP5758D_DELAY);
}

/********************************************************************************************/

bool Bp5758dSetChannels(void) {  
  static bool bIsSleeping = false; //Save sleep state of Lamp  
  uint16_t *cur_col_10 = (uint16_t*)XdrvMailbox.command;
    
  // If we receive 0 for all channels, we'll assume that the lightbulb is off, and activate BP5758d's sleep mode.
  if (cur_col_10[0]==0 && cur_col_10[1]==0 && cur_col_10[2]==0 && cur_col_10[3]==0 && cur_col_10[4]==0) {
    Bp5758dStart(BP5758D_ADDR_SETUP);
    Bp5758dWrite(BP5758D_DISABLE_OUTPUTS_ALL);
    Bp5758dStop();
    Bp5758dStart(BP5758D_ADDR_SLEEP);
    Bp5758dStop();
    bIsSleeping = true;
    return true;
  }

  if (bIsSleeping) {
    bIsSleeping = false; //No need to run it every time a val gets changed
    Bp5758dStart(BP5758D_ADDR_SETUP); //Sleep mode gets disabled too since bits 5:6 get set to 01
    Bp5758dWrite(BP5758D_ENABLE_OUTPUTS_ALL); //Set all outputs to ON
    Bp5758dStop();
  }
    
  // Even though we could address changing channels only, in practice we observed that the lightbulb always sets all channels.
  Bp5758dStart(BP5758D_ADDR_OUT1_GL);
  // Brigtness values are transmitted as two bytes. The light-bulb accepts a 10-bit integer (0-1023) as an input value.
  // The first 5bits of this input are transmitted in second byte, the second 5bits in the first byte.  
  Bp5758dWrite((uint8_t)(cur_col_10[0] & 0x1F));  //Red
  Bp5758dWrite((uint8_t)(cur_col_10[0] >> 5));
  Bp5758dWrite((uint8_t)(cur_col_10[1] & 0x1F)); //Green
  Bp5758dWrite((uint8_t)(cur_col_10[1] >> 5));
  Bp5758dWrite((uint8_t)(cur_col_10[2] & 0x1F)); //Blue
  Bp5758dWrite((uint8_t)(cur_col_10[2] >> 5));
  Bp5758dWrite((uint8_t)(cur_col_10[4] & 0x1F)); //Cold
  Bp5758dWrite((uint8_t)(cur_col_10[4] >> 5));
  Bp5758dWrite((uint8_t)(cur_col_10[3] & 0x1F)); //Warm
  Bp5758dWrite((uint8_t)(cur_col_10[3] >> 5));
  
  Bp5758dStop();
  return true;
}

void Bp5758dModuleSelected(void)
{
  if (PinUsed(GPIO_BP5758D_CLK) && PinUsed(GPIO_BP5758D_DAT)) {
    Bp5758d.clk = Pin(GPIO_BP5758D_CLK);
    Bp5758d.data = Pin(GPIO_BP5758D_DAT);

    // For it's init sequence, BP5758D just sets all fields
    Bp5758dInit();
    Bp5758dStart(BP5758D_ADDR_SETUP);
    // Output enabled: enable all outputs since we're using a RGBCW light
    Bp5758dWrite(BP5758D_ENABLE_OUTPUTS_ALL);
    // Set currents for OUT1-OUT5
    Bp5758dWrite(BP5758D_14MA);
    Bp5758dWrite(BP5758D_14MA);
    Bp5758dWrite(BP5758D_14MA);
    Bp5758dWrite(BP5758D_14MA);
    Bp5758dWrite(BP5758D_14MA);
    // Set grayscale levels ouf all outputs to 0
    Bp5758dWrite(0x00);
    Bp5758dWrite(0x00);
    Bp5758dWrite(0x00);
    Bp5758dWrite(0x00);
    Bp5758dWrite(0x00);
    Bp5758dWrite(0x00);
    Bp5758dWrite(0x00);
    Bp5758dWrite(0x00);
    Bp5758dWrite(0x00);
    Bp5758dWrite(0x00);
    Bp5758dStop();

    TasmotaGlobal.light_type = LT_RGBWC;
    TasmotaGlobal.light_driver = XLGT_08;
    AddLog(LOG_LEVEL_DEBUG, PSTR("LGT: BP5758D Found"));
  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xlgt08(uint32_t function)
{
  bool result = false;

  switch (function) {
    case FUNC_SET_CHANNELS:
      result = Bp5758dSetChannels();
      break;
    case FUNC_MODULE_INIT:
      Bp5758dModuleSelected();
      break;
  }
  return result;
}

#endif  // USE_BP5758D
#endif  // USE_LIGHT
