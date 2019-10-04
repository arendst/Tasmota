/*
  xdrv_26_sm2135.ino - sm2135 five channel led support for Sonoff-Tasmota

  Copyright (C) 2019  Theo Arends

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
 * SM2135 RGBCW Led bulbs like Action LSC SmartLed
 *
 * {"NAME":"LSC RGBCW LED","GPIO":[0,0,0,0,0,0,0,0,181,0,180,0,0],"FLAG":0,"BASE":18}
\*********************************************************************************************/

#define XDRV_26             26

#define SM2135_ADDR         0xC0

//#define SM2135_CURRENT      0x12  // 3 x 15mA for RGB, 2 x 20mA for CW - Get really hot
//#define SM2135_CURRENT      0x11  // 3 x 15mA for RGB, 2 x 15mA for CW
#define SM2135_CURRENT      0x20  // 3 x 20mA for RGB, 2 x 10mA for CW

#define SM2135_RGB          0x00
#define SM2135_CW           0x80

struct SM2135 {
  uint8_t clk = 0;
  uint8_t data = 0;
  bool found = true;
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

bool Sm2135SetChannels(void)
{
  char *buffer = XdrvMailbox.data;
  uint8_t data[8];

  if (('\0' == buffer[0]) && ('\0' == buffer[1]) && ('\0' == buffer[2])) {
    // No color so must be Cold/Warm
/*
    if ((buffer[3] + buffer[4]) >= (1 * 256)) {
      // Scale down to 255 total to fix max power usage of 9W (=40mA)

//      buffer[3] >>= 1;  // Divide by 2
//      buffer[4] >>= 1;  // Divide by 2
    }
*/
    data[0] = SM2135_ADDR;
    data[1] = SM2135_CURRENT;
    data[2] = SM2135_CW;
    Sm2135Send(data, 3);
    delay(1);
    data[0] = SM2135_ADDR +5;
    data[1] = buffer[4];  // Warm
    data[2] = buffer[3];  // Cold
    Sm2135Send(data, 3);
  } else {
    // Color
/*
    if ((buffer[0] + buffer[1] + buffer[2]) >= (3 * 256)) {
      // Scale down to 765 total to fix max power usage of 9W
      // Currently not needed with setting 3 x 15mA = 45mA = 11W = 765
    }
*/
    data[0] = SM2135_ADDR;
    data[1] = SM2135_CURRENT;
    data[2] = SM2135_RGB;
    data[3] = buffer[1];  // Green
    data[4] = buffer[0];  // Red
    data[5] = buffer[2];  // Blue
    Sm2135Send(data, 6);
  }

  return true;
}

bool Sm2135ModuleSelected(void)
{
  if ((pin[GPIO_SM2135_CLK] < 99) && (pin[GPIO_SM2135_DAT] < 99)) {
    Sm2135.clk = pin[GPIO_SM2135_CLK];
    Sm2135.data = pin[GPIO_SM2135_DAT];

    pinMode(Sm2135.data, OUTPUT);
    digitalWrite(Sm2135.data, HIGH);
    pinMode(Sm2135.clk, OUTPUT);
    digitalWrite(Sm2135.clk, HIGH);

    light_type = LT_RGBWC;
    AddLog_P2(LOG_LEVEL_DEBUG, PSTR("DBG: SM2135 Found"));
  } else {
    Sm2135.found = false;
  }
  return Sm2135.found;
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xdrv26(uint8_t function)
{
  bool result = false;

  if (Sm2135.found) {
    switch (function) {
      case FUNC_SET_CHANNELS:
        result = Sm2135SetChannels();
        break;
      case FUNC_MODULE_INIT:
        result = Sm2135ModuleSelected();
        break;
    }
  }
  return result;
}

#endif  // USE_SM2135
#endif  // USE_LIGHT
