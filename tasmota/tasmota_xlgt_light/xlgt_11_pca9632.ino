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

/********************************************************************************************/

bool PCA9632_SetChannels(void) {
  uint8_t *cur_col = (uint8_t*)XdrvMailbox.data;

  DEBUG_TRACE_LOG(PSTR("LGT: PCA9632 %d - %d - %d - %d"), cur_col[0], cur_col[1], cur_col[2], cur_col[3]);

  PCA9632_Enable(PCA9632_SetPWM_Bulk(cur_col, 4));

  return true;
}

bool PCA9632_ModuleSelected(void) {
  DEBUG_TRACE_LOG(PSTR("LGT: PCA9632 ModuleSelected"));

  if (PCA9632_Detect()) {
    PCA9632_Init();

    TasmotaGlobal.light_type += LST_RGBW;    // Add RGBW to be controlled by PCA9632
    TasmotaGlobal.light_driver = XLGT_11;

    AddLog(LOG_LEVEL_INFO, PSTR("LGT: PCA9632 Found"));

    return true;
  }

  return false;
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xlgt11(uint32_t function) {
  bool result = false;

  switch (function) {
    case FUNC_SET_CHANNELS:
      result = PCA9632_SetChannels();
      break;
    case FUNC_MODULE_INIT:
      result = PCA9632_ModuleSelected();
      break;
  }
  return result;
}

#endif  // USE_PCA9632
#endif  // USE_LIGHT

