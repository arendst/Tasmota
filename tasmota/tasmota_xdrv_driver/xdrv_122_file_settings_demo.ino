/*
  xdrv_122_file_settings_demo.ino - Demo for Tasmota

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

// Enable this define to use this demo
//#define USE_DRV_FILE_DEMO

#ifdef USE_DRV_FILE_DEMO
/*********************************************************************************************\
 * Settings load and save demo using Tasmota file system
 *
 * To test this file:
 * - Have hardware with at least 2M flash
 * - Enable a board with at least 256k filesystem in platform_override.ini
\*********************************************************************************************/
#warning **** USE_DRV_FILE_DEMO is enabled ****

#define XDRV_122               122

#define DRV_DEMO_MAX_DRV_TEXT  16

const uint16_t DRV_DEMO_VERSION = 0x0101;       // Latest driver version (See settings deltas below)

// Demo command line commands
const char kDrvDemoCommands[] PROGMEM = "Drv|"  // Prefix
  "Text";

void (* const DrvDemoCommand[])(void) PROGMEM = {
  &CmndDrvText };

// Global structure containing driver saved variables
struct {
  uint32_t  crc32;    // To detect file changes
  uint16_t  version;  // To detect driver function changes
  uint16_t  spare;
  char      drv_text[DRV_DEMO_MAX_DRV_TEXT][10];
} DrvDemoSettings;

// Global structure containing driver non-saved variables
struct {
  uint32_t any_value;
} DrvDemoGlobal;

void CmndDrvText(void) {
  if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= DRV_DEMO_MAX_DRV_TEXT)) {
    if (!XdrvMailbox.usridx) {
      // Command DrvText
      for (uint32_t i = 0; i < DRV_DEMO_MAX_DRV_TEXT; i++) {
        AddLog(LOG_LEVEL_DEBUG, PSTR("DRV: DrvText%02d %s"), i, DrvDemoSettings.drv_text[i]);
      }
      ResponseCmndDone();
    } else {
      // Command DrvText<index> <text>
      uint32_t index = XdrvMailbox.index -1;
      if (XdrvMailbox.data_len > 0) {
        snprintf_P(DrvDemoSettings.drv_text[index], sizeof(DrvDemoSettings.drv_text[index]), XdrvMailbox.data);
      }
      ResponseCmndIdxChar(DrvDemoSettings.drv_text[index]);
    }
  }
}

/*********************************************************************************************\
 * Driver Settings load and save
\*********************************************************************************************/

void DrvDemoSettingsLoad(bool erase) {
  // Called from FUNC_PRE_INIT (erase = 0) once at restart
  // Called from FUNC_RESET_SETTINGS (erase = 1) after command reset 4, 5, or 6

  // *** Start init default values in case file is not found ***
  AddLog(LOG_LEVEL_INFO, PSTR("DRV: " D_USE_DEFAULTS));

  memset(&DrvDemoSettings, 0x00, sizeof(DrvDemoSettings));
  DrvDemoSettings.version = DRV_DEMO_VERSION;
  // Init any other parameter in struct DrvDemoSettings
  snprintf_P(DrvDemoSettings.drv_text[0], sizeof(DrvDemoSettings.drv_text[0]), PSTR("Azalea"));

  // *** End Init default values ***

#ifndef USE_UFILESYS
  AddLog(LOG_LEVEL_INFO, PSTR("CFG: Demo use defaults as file system not enabled"));
#else
  // Try to load file /.drvset122
  char filename[20];
  // Use for sensors:
//  snprintf_P(filename, sizeof(filename), PSTR(TASM_FILE_SENSOR), XSNS_122);
  // Use for drivers:
  snprintf_P(filename, sizeof(filename), PSTR(TASM_FILE_DRIVER), XDRV_122);
  if (erase) {
    TfsDeleteFile(filename);  // Use defaults
  }
  else if (TfsLoadFile(filename, (uint8_t*)&DrvDemoSettings, sizeof(DrvDemoSettings))) {
    if (DrvDemoSettings.version != DRV_DEMO_VERSION) {      // Fix version dependent changes

      // *** Start fix possible setting deltas ***
      if (DrvDemoSettings.version < 0x01010100) {
        AddLog(LOG_LEVEL_INFO, PSTR("CFG: Update oldest version restore"));

      }
      if (DrvDemoSettings.version < 0x01010101) {
        AddLog(LOG_LEVEL_INFO, PSTR("CFG: Update old version restore"));

      }

      // *** End setting deltas ***

      // Set current version and save settings
      DrvDemoSettings.version = DRV_DEMO_VERSION;
      DrvDemoSettingsSave();
    }
    AddLog(LOG_LEVEL_INFO, PSTR("CFG: Demo loaded from file"));
  }
  else {
    // File system not ready: No flash space reserved for file system
    AddLog(LOG_LEVEL_DEBUG, PSTR("CFG: Demo use defaults as file system not ready or file not found"));
  }
#endif  // USE_UFILESYS
}

void DrvDemoSettingsSave(void) {
  // Called from FUNC_SAVE_SETTINGS every SaveData second and at restart
#ifdef USE_UFILESYS
  uint32_t crc32 = GetCfgCrc32((uint8_t*)&DrvDemoSettings +4, sizeof(DrvDemoSettings) -4);  // Skip crc32
  if (crc32 != DrvDemoSettings.crc32) {
    // Try to save file /.drvset122
    DrvDemoSettings.crc32 = crc32;

    char filename[20];
    // Use for sensors:
//    snprintf_P(filename, sizeof(filename), PSTR(TASM_FILE_SENSOR), XSNS_122);
    // Use for drivers:
    snprintf_P(filename, sizeof(filename), PSTR(TASM_FILE_DRIVER), XDRV_122);
    if (TfsSaveFile(filename, (const uint8_t*)&DrvDemoSettings, sizeof(DrvDemoSettings))) {
      AddLog(LOG_LEVEL_DEBUG, PSTR("CFG: Demo saved to file"));
    } else {
      // File system not ready: No flash space reserved for file system
      AddLog(LOG_LEVEL_DEBUG, PSTR("CFG: ERROR Demo file system not ready or unable to save file"));
    }
  }
#endif  // USE_UFILESYS
}

bool DrvDemoSettingsRestore(void) {
  XdrvMailbox.data = (char*)&DrvDemoSettings;
  XdrvMailbox.index = sizeof(DrvDemoSettings);
  return true;
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xdrv122(uint32_t function) {
  bool result = false;

  switch (function) {
    case FUNC_RESET_SETTINGS:
      DrvDemoSettingsLoad(1);
      break;
    case FUNC_RESTORE_SETTINGS:
      result = DrvDemoSettingsRestore();
      break;
    case FUNC_SAVE_SETTINGS:
      DrvDemoSettingsSave();
      break;
    case FUNC_COMMAND:
      result = DecodeCommand(kDrvDemoCommands, DrvDemoCommand);
      break;
    case FUNC_PRE_INIT:
      DrvDemoSettingsLoad(0);
      break;
    case FUNC_SAVE_BEFORE_RESTART:
      // !!! DO NOT USE AS IT'S FUNCTION IS BETTER HANDLED BY FUNC_SAVE_SETTINGS !!!
      break;
    case FUNC_ACTIVE:
      result = true;
      break;
  }
  return result;
}

#endif  // USE_DRV_FILE_DEMO