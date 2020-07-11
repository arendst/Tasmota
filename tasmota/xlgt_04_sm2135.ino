/*
  xlgt_04_sm2135.ino - sm2135 five channel led support for Tasmota

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
#ifdef USE_SM2135
/*********************************************************************************************\
 * SM2135 RGBCW Led bulbs like some Action LSC SmartLed
 *
 * {"NAME":"LSC RGBCW LED","GPIO":[0,0,0,0,0,0,0,0,181,0,180,0,0],"FLAG":0,"BASE":18}
\*********************************************************************************************/

#define XLGT_04             4

#define SM2135_ADDR_MC      0xC0  // Max current register
#define SM2135_ADDR_CH      0xC1  // RGB or CW channel select register
#define SM2135_ADDR_R       0xC2  // Red color
#define SM2135_ADDR_G       0xC3  // Green color
#define SM2135_ADDR_B       0xC4  // Blue color
#define SM2135_ADDR_C       0xC5  // Cold
#define SM2135_ADDR_W       0xC6  // Warm

#define SM2135_RGB          0x00  // RGB channel
#define SM2135_CW           0x80  // CW channel (Chip default)

#define SM2135_10MA         0x00
#define SM2135_15MA         0x01
#define SM2135_20MA         0x02  // RGB max current (Chip default)
#define SM2135_25MA         0x03
#define SM2135_30MA         0x04  // CW max current (Chip default)
#define SM2135_35MA         0x05
#define SM2135_40MA         0x06
#define SM2135_45MA         0x07  // Max value for RGB
#define SM2135_50MA         0x08
#define SM2135_55MA         0x09
#define SM2135_60MA         0x0A

//                              RGB current         CW current
const uint8_t SM2135_CURRENT = (SM2135_20MA << 4) | SM2135_15MA;  // See https://github.com/arendst/Tasmota/issues/6495#issuecomment-549121683

struct SM2135 {
  uint8_t clk = 0;
  uint8_t data = 0;
} Sm2135;

uint8_t Sm2135Write(uint8_t data)
{
  for (uint32_t i = 0; i < 8; i++) {
    digitalWrite(Sm2135.clk, LOW);
    digitalWrite(Sm2135.data, (data & 0x80));
    digitalWrite(Sm2135.clk, HIGH);
    data = data << 1;
  }
  digitalWrite(Sm2135.clk, LOW);
  digitalWrite(Sm2135.data, HIGH);
  pinMode(Sm2135.data, INPUT);
  digitalWrite(Sm2135.clk, HIGH);
  uint8_t ack = digitalRead(Sm2135.data);
  pinMode(Sm2135.data, OUTPUT);
  return ack;
}

void Sm2135Send(uint8_t *buffer, uint8_t size)
{
  digitalWrite(Sm2135.data, LOW);
  for (uint32_t i = 0; i < size; i++) {
    Sm2135Write(buffer[i]);
  }
  digitalWrite(Sm2135.clk, LOW);
  digitalWrite(Sm2135.clk, HIGH);
  digitalWrite(Sm2135.data, HIGH);
}

/********************************************************************************************/

bool Sm2135SetChannels(void)
{
  uint8_t *cur_col = (uint8_t*)XdrvMailbox.data;
  uint8_t data[6];

  if ((0 == cur_col[0]) && (0 == cur_col[1]) && (0 == cur_col[2])) {
    // No color so must be Cold/Warm
/*
    if ((cur_col[3] + cur_col[4]) >= (1 * 256)) {
      // Scale down to 255 total to fix max power usage of 9W (=40mA)

//      cur_col[3] >>= 1;  // Divide by 2
//      cur_col[4] >>= 1;  // Divide by 2
    }
*/
    data[0] = SM2135_ADDR_MC;
    data[1] = SM2135_CURRENT;
    data[2] = SM2135_CW;
    Sm2135Send(data, 3);
    delay(1);
    data[0] = SM2135_ADDR_C;
    data[1] = cur_col[4];  // Warm
    data[2] = cur_col[3];  // Cold
    Sm2135Send(data, 3);
  } else {
    // Color
/*
    if ((cur_col[0] + cur_col[1] + cur_col[2]) >= (3 * 256)) {
      // Scale down to 765 total to fix max power usage of 9W
      // Currently not needed with setting 3 x 15mA = 45mA = 11W = 765
    }
*/
    data[0] = SM2135_ADDR_MC;
    data[1] = SM2135_CURRENT;
    data[2] = SM2135_RGB;
    data[3] = cur_col[1];  // Green
    data[4] = cur_col[0];  // Red
    data[5] = cur_col[2];  // Blue
    Sm2135Send(data, 6);
  }

  return true;
}

void Sm2135ModuleSelected(void)
{
  if (PinUsed(GPIO_SM2135_CLK) && PinUsed(GPIO_SM2135_DAT)) {
    Sm2135.clk = Pin(GPIO_SM2135_CLK);
    Sm2135.data = Pin(GPIO_SM2135_DAT);

    pinMode(Sm2135.data, OUTPUT);
    digitalWrite(Sm2135.data, HIGH);
    pinMode(Sm2135.clk, OUTPUT);
    digitalWrite(Sm2135.clk, HIGH);

    light_type = LT_RGBWC;
    light_flg = XLGT_04;
    AddLog_P2(LOG_LEVEL_DEBUG, PSTR("DBG: SM2135 Found"));
  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xlgt04(uint8_t function)
{
  bool result = false;

  switch (function) {
    case FUNC_SET_CHANNELS:
      result = Sm2135SetChannels();
      break;
    case FUNC_MODULE_INIT:
      Sm2135ModuleSelected();
      break;
  }
  return result;
}

#endif  // USE_SM2135
#endif  // USE_LIGHT
