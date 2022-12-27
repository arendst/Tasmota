/*
  xlgt_11_pca9632.ino - pca9632 four channel led support for Tasmota

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
#ifdef USE_PCA9632
/*********************************************************************************************\
 * PCA9632 - Controlling RGBW over I2C
 * Copyright (C) 2022 Pascal Heinrich
 *
\*********************************************************************************************/

#define XLGT_11             11

#define D_LOG_PCA9632       "PCA9632: "

struct PCA9632 {
  int8_t pin_clk = 0;
  int8_t pin_dat = 0;
  bool enabled = false;
} Pca9632;

void PCA9632_L_SendBit(uint8_t v)
{

}

void PCA9632_L_SendByte(uint8_t v)
{

}

void PCA9632_L_Update(uint8_t duty_r, uint8_t duty_g, uint8_t duty_b)
{

}

void PCA9632_L_Init(void)
{

}

/********************************************************************************************/

bool PCA9632_L_SetChannels(void)
{
  return true;
}

bool PCA9632_L_ModuleSelected(void)
{
  if (PinUsed(GPIO_PCA9632_CLK) && PinUsed(GPIO_PCA9632_DAT)) {
    Pca9632.pin_clk = Pin(GPIO_PCA9632_CLK);
    Pca9632.pin_dat = Pin(GPIO_PCA9632_DAT);

    PCA9632_Init();

    TasmotaGlobal.light_type += LST_RGBW;    // Add RGBW to be controlled by PCA9632
    TasmotaGlobal.light_driver = XLGT_11;
    AddLog(LOG_LEVEL_DEBUG, PSTR("DBG: PCA9632 Found"));

    return true;
  }

  return false;
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xlgt11(uint32_t function)
{
  bool result = false;

  switch (function) {
    case FUNC_SET_CHANNELS:
      result = PCA9632_L_SetChannels();
      break;
    case FUNC_MODULE_INIT:
      result = PCA9632_L_ModuleSelected();
      break;
  }
  return result;
}

#endif  // USE_PCA9632
#endif  // USE_LIGHT

