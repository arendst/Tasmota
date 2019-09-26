/*
  xdrv_26_sm2135.ino - sm2135 I2C five channel led support for Sonoff-Tasmota

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
 * SM2135 I2C RGBCW Led bulbs like Action LSC SmartLed
\*********************************************************************************************/

#define XDRV_26             26

#define SM2135_ADDR         0x40  // 0x40 .. 0x46
#define SM2135_CURRENT      0x24  // Defaults: 20mA for RGB, 30mA for CW
#define SM2135_RGB          0x00
#define SM2135_CW           0x80

struct SM2135 {
  bool found = true;
} Sm2135;

bool Sm2135SetChannels(void)
{
  char *buffer = XdrvMailbox.data;

  // EXPERIMENTAL: Figure out if selecting RGB or CW blanks the opposite

  if (('\0' == buffer[3]) && ('\0' == buffer[4])) {
    Wire.beginTransmission(SM2135_ADDR);
    Wire.write(SM2135_CURRENT);           // Set current
    Wire.write(SM2135_RGB);               // Select RGB - Shutdown CW?
    Wire.write(buffer[0]);                // Red
    Wire.write(buffer[1]);                // Green
    Wire.write(buffer[2]);                // Blue
    Wire.endTransmission();
  } else {
    Wire.beginTransmission(SM2135_ADDR);
    Wire.write(SM2135_CURRENT);           // Set current
    Wire.write(SM2135_CW);                // Select CW - Shutdown RGB?
    Wire.endTransmission();

    Wire.beginTransmission(SM2135_ADDR +5);
    Wire.write(buffer[4]);                // Cold
    Wire.write(buffer[3]);                // Warm
    Wire.endTransmission();
  }

  return true;
}

bool Sm2135ModuleSelected(void)
{
  if (I2cDevice(SM2135_ADDR)) {

    // Make sure it is the SM2135 chip as it's address is also used by HTU21, INA219, INA226
    // EXPERIMENTAL: Need further testing

    light_type = LT_RGBWC;

    AddLog_P2(LOG_LEVEL_DEBUG, S_LOG_I2C_FOUND_AT, "SM2135", SM2135_ADDR);
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

  if (i2c_flg && Sm2135.found) {
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
