/*
  xdrv_98_file_settings_demo.ino - Demo for Tasmota

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
 * - Enable a build.ldscript with at least 256k filesystem in platform_override.ini
 * - Enable define USE_UFILESYS in user_config_override.h
\*********************************************************************************************/
#warning **** USE_DRV_FILE_DEMO is enabled ****

#define XDRV_98             98

#define DRV98_MAX_DRV_TEXT  16

const uint32_t DRV98_VERSION = 0x01010101;  // Latest driver version (See settings deltas below)

// Demo command line commands
const char kDrvDemoCommands[] PROGMEM = "Drv|"  // Prefix
  "Text";

void (* const DrvDemoCommand[])(void) PROGMEM = {
  &CmndDrvText };

// Global structure containing driver saved variables
struct {
  uint32_t  crc32;    // To detect file changes
  uint32_t  version;  // To detect driver function changes
  char      drv_text[DRV98_MAX_DRV_TEXT -1][10];
} Drv98Settings;

// Global structure containing driver non-saved variables
struct {
  uint32_t any_value;
} Drv98Global;

void CmndDrvText(void) {
  if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= DRV98_MAX_DRV_TEXT)) {
    if (!XdrvMailbox.usridx) {
      // Command DrvText
      for (uint32_t i = 0; i < DRV98_MAX_DRV_TEXT; i++) {
        AddLog(LOG_LEVEL_DEBUG, PSTR("DRV: DrvText%02d %s"), i, Drv98Settings.drv_text[i]);
      }
      ResponseCmndDone();
    } else {
      // Command DrvText<index> <text>
      uint32_t index = XdrvMailbox.index -1;
      if (XdrvMailbox.data_len > 0) {
        snprintf_P(Drv98Settings.drv_text[index], sizeof(Drv98Settings.drv_text[index]), XdrvMailbox.data);
      }
      ResponseCmndIdxChar(Drv98Settings.drv_text[index]);
    }
  }
}

/*********************************************************************************************\
 * Driver Settings load and save
\*********************************************************************************************/

uint32_t DrvDemoSettingsCrc32(void) {
  // Use Tasmota CRC calculation function
  return GetCfgCrc32((uint8_t*)&Drv98Settings +4, sizeof(Drv98Settings) -4);  // Skip crc32
}

void DrvDemoSettingsDefault(void) {
  // Init default values in case file is not found

  AddLog(LOG_LEVEL_INFO, PSTR("DRV: " D_USE_DEFAULTS));

  memset(&Drv98Settings, 0x00, sizeof(Drv98Settings));
  Drv98Settings.version = DRV98_VERSION;
  // Init any other parameter in struct Drv98Settings
  snprintf_P(Drv98Settings.drv_text[0], sizeof(Drv98Settings.drv_text[0]), PSTR("Azalea"));
}

void DrvDemoSettingsDelta(void) {
  // Fix possible setting deltas

  if (Drv98Settings.version != DRV98_VERSION) {      // Fix version dependent changes

    if (Settings.version < 0x01010100) {
      AddLog(LOG_LEVEL_INFO, PSTR("DRV: Update oldest version restore"));

    }
    if (Settings.version < 0x01010101) {
      AddLog(LOG_LEVEL_INFO, PSTR("DRV: Update old version restore"));

    }

    // Set current version and save settings
    Drv98Settings.version = DRV98_VERSION;
    DrvDemoSettingsSave();
  }
}

void DrvDemoSettingsLoad(void) {
  // Called from FUNC_PRE_INIT once at restart

  // Init default values in case file is not found
  DrvDemoSettingsDefault();

  // Try to load file /.drvset098
  char filename[20];
  // Use for sensors:
//  snprintf_P(filename, sizeof(filename), PSTR(TASM_FILE_SENSOR), XSNS_98);
  // Use for drivers:
  snprintf_P(filename, sizeof(filename), PSTR(TASM_FILE_DRIVER), XDRV_98);

  AddLog(LOG_LEVEL_INFO, PSTR("DRV: About to load settings from file %s"), filename);

#ifdef USE_UFILESYS
  if (TfsLoadFile(filename, (uint8_t*)&Drv98Settings, sizeof(Drv98Settings))) {
    // Fix possible setting deltas
    DrvDemoSettingsDelta();
  } else {
    // File system not ready: No flash space reserved for file system
    AddLog(LOG_LEVEL_INFO, PSTR("DRV: ERROR File system not ready or file not found"));
  }
#else
  AddLog(LOG_LEVEL_INFO, PSTR("DRV: ERROR File system not enabled"));
#endif  // USE_UFILESYS

  Drv98Settings.crc32 = DrvDemoSettingsCrc32();
}

void DrvDemoSettingsSave(void) {
  // Called from FUNC_SAVE_SETTINGS every SaveData second and at restart

  if (DrvDemoSettingsCrc32() != Drv98Settings.crc32) {
    // Try to save file /.drvset098
    Drv98Settings.crc32 = DrvDemoSettingsCrc32();

    char filename[20];
    // Use for sensors:
//    snprintf_P(filename, sizeof(filename), PSTR(TASM_FILE_SENSOR), XSNS_98);
    // Use for drivers:
    snprintf_P(filename, sizeof(filename), PSTR(TASM_FILE_DRIVER), XDRV_98);

    AddLog(LOG_LEVEL_INFO, PSTR("DRV: About to save settings to file %s"), filename);

#ifdef USE_UFILESYS
    if (!TfsSaveFile(filename, (const uint8_t*)&Drv98Settings, sizeof(Drv98Settings))) {
      // File system not ready: No flash space reserved for file system
      AddLog(LOG_LEVEL_INFO, PSTR("DRV: ERROR File system not ready or unable to save file"));
    }
#else
    AddLog(LOG_LEVEL_INFO, PSTR("DRV: ERROR File system not enabled"));
#endif  // USE_UFILESYS
  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xdrv98(uint8_t function) {
  bool result = false;

  switch (function) {
    case FUNC_SAVE_SETTINGS:
      DrvDemoSettingsSave();
      break;
    case FUNC_COMMAND:
      result = DecodeCommand(kDrvDemoCommands, DrvDemoCommand);
      break;
    case FUNC_PRE_INIT:
      DrvDemoSettingsLoad();
      break;
    case FUNC_SAVE_BEFORE_RESTART:
      // !!! DO NOT USE AS IT'S FUNCTION IS BETTER HANDLED BY FUNC_SAVE_SETTINGS !!!
      break;
  }
  return result;
}

#endif  // USE_DRV_FILE_DEMO