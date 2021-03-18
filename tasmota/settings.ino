/*
  settings.ino - user settings for Tasmota

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

/*********************************************************************************************\
 * RTC memory
\*********************************************************************************************/

const uint16_t RTC_MEM_VALID = 0xA55A;

uint32_t rtc_settings_crc = 0;

uint32_t GetRtcSettingsCrc(void) {
  uint32_t crc = 0;
  uint8_t *bytes = (uint8_t*)&RtcSettings;

  for (uint32_t i = 0; i < sizeof(RtcSettings); i++) {
    crc += bytes[i]*(i+1);
  }
  return crc;
}

void RtcSettingsSave(void) {
  RtcSettings.baudrate = Settings.baudrate * 300;
  if (GetRtcSettingsCrc() != rtc_settings_crc) {
    RtcSettings.valid = RTC_MEM_VALID;
#ifdef ESP8266
    ESP.rtcUserMemoryWrite(100, (uint32_t*)&RtcSettings, sizeof(RtcSettings));
#endif  // ESP8266
#ifdef ESP32
    RtcDataSettings = RtcSettings;
#endif  // ESP32
    rtc_settings_crc = GetRtcSettingsCrc();
  }
}

bool RtcSettingsLoad(uint32_t update) {
#ifdef ESP8266
  ESP.rtcUserMemoryRead(100, (uint32_t*)&RtcSettings, sizeof(RtcSettings));  // 0x290
#endif  // ESP8266
#ifdef ESP32
  RtcSettings = RtcDataSettings;
#endif  // ESP32

  bool read_valid = (RTC_MEM_VALID == RtcSettings.valid);
  if (update) {
    if (!read_valid) {
      memset(&RtcSettings, 0, sizeof(RtcSettings));
      RtcSettings.valid = RTC_MEM_VALID;
      RtcSettings.energy_kWhtoday = Settings.energy_kWhtoday;
      RtcSettings.energy_kWhtotal = Settings.energy_kWhtotal;
      RtcSettings.energy_usage = Settings.energy_usage;
      for (uint32_t i = 0; i < MAX_COUNTERS; i++) {
        RtcSettings.pulse_counter[i] = Settings.pulse_counter[i];
      }
      RtcSettings.power = Settings.power;
  //    RtcSettings.baudrate = Settings.baudrate * 300;
      RtcSettings.baudrate = APP_BAUDRATE;
      RtcSettingsSave();
    }
    rtc_settings_crc = GetRtcSettingsCrc();
  }
  return read_valid;
}

bool RtcSettingsValid(void) {
  return (RTC_MEM_VALID == RtcSettings.valid);
}

/********************************************************************************************/

uint32_t rtc_reboot_crc = 0;

uint32_t GetRtcRebootCrc(void) {
  uint32_t crc = 0;
  uint8_t *bytes = (uint8_t*)&RtcReboot;

  for (uint32_t i = 0; i < sizeof(RtcReboot); i++) {
    crc += bytes[i]*(i+1);
  }
  return crc;
}

void RtcRebootSave(void) {
  if (GetRtcRebootCrc() != rtc_reboot_crc) {
    RtcReboot.valid = RTC_MEM_VALID;
#ifdef ESP8266
    ESP.rtcUserMemoryWrite(100 - sizeof(RtcReboot), (uint32_t*)&RtcReboot, sizeof(RtcReboot));
#endif  // ESP8266
#ifdef ESP32
    RtcDataReboot = RtcReboot;
#endif  // ESP32
    rtc_reboot_crc = GetRtcRebootCrc();
  }
}

void RtcRebootReset(void) {
  RtcReboot.fast_reboot_count = 0;
  RtcRebootSave();
}

void RtcRebootLoad(void) {
#ifdef ESP8266
  ESP.rtcUserMemoryRead(100 - sizeof(RtcReboot), (uint32_t*)&RtcReboot, sizeof(RtcReboot));  // 0x280
#endif  // ESP8266
#ifdef ESP32
  RtcReboot = RtcDataReboot;
#endif  // ESP32
  if (RtcReboot.valid != RTC_MEM_VALID) {
    memset(&RtcReboot, 0, sizeof(RtcReboot));
    RtcReboot.valid = RTC_MEM_VALID;
//    RtcReboot.fast_reboot_count = 0;  // Explicit by memset
    RtcRebootSave();
  }
  rtc_reboot_crc = GetRtcRebootCrc();
}

bool RtcRebootValid(void) {
  return (RTC_MEM_VALID == RtcReboot.valid);
}

/*********************************************************************************************\
 * ESP8266 Tasmota Flash usage offset from 0x40200000
 *
 * Tasmota 1M  Tasmota 2M  Tasmota 4M - Flash usage
 * 0x00000000                         - 4k Unzipped binary bootloader
 * 0x00000FFF
 *
 * 0x00001000                         - Unzipped binary code start
 *    ::::
 * 0x000xxxxx                         - Unzipped binary code end
 * 0x000x1000                         - First page used by Core OTA
 *    ::::
 * 0x000F2FFF  0x000F5FFF  0x000F5FFF
 ******************************************************************************
 *                                      Next 32k is overwritten by OTA
 * 0x000F3000  0x000F6000  0x000F6000 - 4k Tasmota Quick Power Cycle counter (SETTINGS_LOCATION - CFG_ROTATES) - First four bytes only
 * 0x000F3FFF  0x000F6FFF  0x000F6FFF
 * 0x000F4000  0x000F7000  0x000F7000 - 4k First Tasmota rotating settings page
 *    ::::
 * 0x000FA000  0x000FD000  0x000FD000 - 4k Last Tasmota rotating settings page = Last page used by Core OTA (SETTINGS_LOCATION)
 * 0x000FAFFF  0x000FDFFF  0x000FDFFF
 ******************************************************************************
 *             0x000FE000  0x000FE000 - 3k9 Not used
 *             0x000FEFF0  0x000FEFF0 - 4k1  Empty
 *             0x000FFFFF  0x000FFFFF
 *
 * 0x000FB000  0x00100000  0x00100000 - 0k, 980k or 2980k Core FS start (LittleFS)
 * 0x000FB000  0x001FA000  0x003FA000 - 0k, 980k or 2980k Core FS end (LittleFS)
 *             0x001FAFFF  0x003FAFFF
 *
 * 0x000FB000  0x001FB000  0x003FB000 - 4k Core EEPROM = Tasmota settings page during OTA and when no flash rotation is active (EEPROM_LOCATION)
 * 0x000FBFFF  0x001FBFFF  0x003FBFFF
 *
 * 0x000FC000  0x001FC000  0x003FC000 - 4k SDK - Uses first 128 bytes for phy init data mirrored by Core in RAM. See core_esp8266_phy.cpp phy_init_data[128] = Core user_rf_cal_sector
 * 0x000FD000  0x001FD000  0x003FD000 - 4k SDK - Uses scattered bytes from 0x340 (iTead use as settings storage from 0x000FD000)
 * 0x000FE000  0x001FE000  0x003FE000 - 4k SDK - Uses scattered bytes from 0x340 (iTead use as mirrored settings storage from 0x000FE000)
 * 0x000FF000  0x001FF000  0x0031F000 - 4k SDK - Uses at least first 32 bytes of this page - Tasmota Zigbee persistence from 0x000FF800 to 0x000FFFFF
 * 0x000FFFFF  0x001FFFFF  0x003FFFFF
\*********************************************************************************************/

extern "C" {
#include "spi_flash.h"
}
#include "eboot_command.h"

#ifdef ESP8266

extern "C" uint32_t _FS_start;      // 1M = 0x402fb000, 2M = 0x40300000, 4M = 0x40300000
const uint32_t FLASH_FS_START = (((uint32_t)&_FS_start - 0x40200000) / SPI_FLASH_SEC_SIZE);
uint32_t SETTINGS_LOCATION = FLASH_FS_START -1;                                                 // 0xFA, 0x0FF or 0x0FF

// From libraries/EEPROM/EEPROM.cpp EEPROMClass
extern "C" uint32_t _EEPROM_start;  // 1M = 0x402FB000, 2M = 0x403FB000, 4M = 0x405FB000
const uint32_t EEPROM_LOCATION = ((uint32_t)&_EEPROM_start - 0x40200000) / SPI_FLASH_SEC_SIZE;  // 0xFB, 0x1FB or 0x3FB

#endif  // ESP8266

#ifdef ESP32

// dummy defines
#define EEPROM_LOCATION (SPI_FLASH_SEC_SIZE * 200)
uint32_t SETTINGS_LOCATION = EEPROM_LOCATION;

#endif  // ESP32

const uint8_t CFG_ROTATES = 7;      // Number of flash sectors used (handles uploads)

uint32_t settings_location = EEPROM_LOCATION;
uint32_t settings_crc32 = 0;
uint8_t *settings_buffer = nullptr;

void SettingsInit(void) {
  if (SETTINGS_LOCATION > 0xFA) {
    SETTINGS_LOCATION = 0xFD;       // Skip empty partition part and keep in first 1M
  }
}

/********************************************************************************************/
/*
 * Based on cores/esp8266/Updater.cpp
 */
void SetFlashModeDout(void) {
#ifdef ESP8266
  uint8_t *_buffer;
  uint32_t address;

  eboot_command ebcmd;
  eboot_command_read(&ebcmd);
  address = ebcmd.args[0];
  _buffer = new uint8_t[FLASH_SECTOR_SIZE];

  if (ESP.flashRead(address, (uint32_t*)_buffer, FLASH_SECTOR_SIZE)) {
    if (_buffer[2] != 3) {  // DOUT
      _buffer[2] = 3;
      if (ESP.flashEraseSector(address / FLASH_SECTOR_SIZE)) {
        ESP.flashWrite(address, (uint32_t*)_buffer, FLASH_SECTOR_SIZE);
      }
    }
  }
  delete[] _buffer;
#endif  // ESP8266
}

void SettingsBufferFree(void) {
  if (settings_buffer != nullptr) {
    free(settings_buffer);
    settings_buffer = nullptr;
  }
}

bool SettingsBufferAlloc(void) {
  SettingsBufferFree();
  if (!(settings_buffer = (uint8_t *)malloc(sizeof(Settings)))) {
    AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_APPLICATION D_UPLOAD_ERR_2));  // Not enough (memory) space
    return false;
  }
  return true;
}

uint16_t GetCfgCrc16(uint8_t *bytes, uint32_t size) {
  uint16_t crc = 0;

  for (uint32_t i = 0; i < size; i++) {
    if ((i < 14) || (i > 15)) { crc += bytes[i]*(i+1); }  // Skip crc
  }
  return crc;
}

uint16_t GetSettingsCrc(void) {
  // Fix miscalculation if previous Settings was 3584 and current Settings is 4096 between 0x06060007 and 0x0606000A
  uint32_t size = ((Settings.version < 0x06060007) || (Settings.version > 0x0606000A)) ? 3584 : sizeof(Settings);
  return GetCfgCrc16((uint8_t*)&Settings, size);
}

uint32_t GetCfgCrc32(uint8_t *bytes, uint32_t size) {
  // https://create.stephan-brumme.com/crc32/#bitwise
  uint32_t crc = 0;

  while (size--) {
    crc ^= *bytes++;
    for (uint32_t j = 0; j < 8; j++) {
      crc = (crc >> 1) ^ (-int(crc & 1) & 0xEDB88320);
    }
  }
  return ~crc;
}

uint32_t GetSettingsCrc32(void) {
  return GetCfgCrc32((uint8_t*)&Settings, sizeof(Settings) -4);  // Skip crc32
}

void SettingsSaveAll(void) {
  if (Settings.flag.save_state) {
    Settings.power = TasmotaGlobal.power;
  } else {
    Settings.power = 0;
  }
  XsnsCall(FUNC_SAVE_BEFORE_RESTART);
  XdrvCall(FUNC_SAVE_BEFORE_RESTART);
  SettingsSave(0);
}

/*********************************************************************************************\
 * Quick power cycle monitoring
\*********************************************************************************************/

void UpdateQuickPowerCycle(bool update) {
#ifndef FIRMWARE_MINIMAL
  if (Settings.flag3.fast_power_cycle_disable) { return; }  // SetOption65 - Disable fast power cycle detection for device reset

  const uint32_t QPC_COUNT = 7;  // Number of Power Cycles before Settings erase
  const uint32_t QPC_SIGNATURE = 0xFFA55AFF;

#ifdef ESP8266
  const uint32_t qpc_sector = SETTINGS_LOCATION - CFG_ROTATES;
  const uint32_t qpc_location = qpc_sector * SPI_FLASH_SEC_SIZE;

  uint32_t qpc_buffer[QPC_COUNT +1];
  ESP.flashRead(qpc_location, (uint32*)&qpc_buffer, sizeof(qpc_buffer));
  if (update && (QPC_SIGNATURE == qpc_buffer[0])) {
    uint32_t counter = 1;
    while ((0 == qpc_buffer[counter]) && (counter <= QPC_COUNT)) { counter++; }
    if (QPC_COUNT == counter) {  // 7 power cycles in a row
      SettingsErase(3);          // Quickly reset all settings including QuickPowerCycle flag
      EspRestart();              // And restart
    } else {
      qpc_buffer[0] = 0;
      ESP.flashWrite(qpc_location + (counter * 4), (uint32*)&qpc_buffer, 4);
      AddLog(LOG_LEVEL_INFO, PSTR("QPC: Count %d"), counter);
    }
  }
  else if ((qpc_buffer[0] != QPC_SIGNATURE) || (0 == qpc_buffer[1])) {
    qpc_buffer[0] = QPC_SIGNATURE;
    // Assume flash is default all ones and setting a bit to zero does not need an erase
    if (ESP.flashEraseSector(qpc_sector)) {
      ESP.flashWrite(qpc_location, (uint32*)&qpc_buffer, 4);
      AddLog(LOG_LEVEL_INFO, PSTR("QPC: Reset"));
    }
  }
#endif  // ESP8266
#ifdef ESP32
  uint32_t pc_register;
  QPCRead(&pc_register, sizeof(pc_register));
  if (update && ((pc_register & 0xFFFFFFF0) == 0xFFA55AF0)) {
    uint32_t counter = pc_register & 0xF;  // Allow up to 15 cycles
    if (0xF == counter) { counter = 0; }
    counter++;
    if (QPC_COUNT == counter) {  // 7 power cycles in a row
      SettingsErase(3);          // Quickly reset all settings including QuickPowerCycle flag
      EspRestart();              // And restart
    } else {
      pc_register = 0xFFA55AF0 | counter;
      QPCWrite(&pc_register, sizeof(pc_register));
      AddLog(LOG_LEVEL_INFO, PSTR("QPC: Count %d"), counter);
    }
  }
  else if (pc_register != QPC_SIGNATURE) {
    pc_register = QPC_SIGNATURE;
    QPCWrite(&pc_register, sizeof(pc_register));
    AddLog(LOG_LEVEL_INFO, PSTR("QPC: Reset"));
  }
#endif  // ESP32

#endif  // FIRMWARE_MINIMAL
}

/*********************************************************************************************\
 * Config Settings.text char array support
\*********************************************************************************************/

uint32_t GetSettingsTextLen(void) {
  char* position = Settings.text_pool;
  for (uint32_t size = 0; size < SET_MAX; size++) {
    while (*position++ != '\0') { }
  }
  return position - Settings.text_pool;
}

bool settings_text_mutex = false;
uint32_t settings_text_busy_count = 0;

bool SettingsUpdateFinished(void) {
  uint32_t wait_loop = 10;
  while (settings_text_mutex && wait_loop) {  // Wait for any update to finish
    yield();
    delayMicroseconds(1);
    wait_loop--;
  }
  return (wait_loop > 0);  // true if finished
}

bool SettingsUpdateText(uint32_t index, const char* replace_me) {
  if (index >= SET_MAX) {
    return false;  // Setting not supported - internal error
  }

  // Make a copy first in case we use source from Settings.text
  uint32_t replace_len = strlen_P(replace_me);
  char replace[replace_len +1];
  memcpy_P(replace, replace_me, sizeof(replace));
  uint32_t index_save = index;

  uint32_t start_pos = 0;
  uint32_t end_pos = 0;
  char* position = Settings.text_pool;
  for (uint32_t size = 0; size < SET_MAX; size++) {
    while (*position++ != '\0') { }
    if (1 == index) {
      start_pos = position - Settings.text_pool;
    }
    else if (0 == index) {
      end_pos = position - Settings.text_pool -1;
    }
    index--;
  }
  uint32_t char_len = position - Settings.text_pool;

  uint32_t current_len = end_pos - start_pos;
  int diff = replace_len - current_len;

//  AddLog(LOG_LEVEL_DEBUG, PSTR("TST: start %d, end %d, len %d, current %d, replace %d, diff %d"),
//    start_pos, end_pos, char_len, current_len, replace_len, diff);

  int too_long = (char_len + diff) - settings_text_size;
  if (too_long > 0) {
    AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_CONFIG "Text overflow by %d char(s)"), too_long);
    return false;  // Replace text too long
  }

  if (settings_text_mutex && !SettingsUpdateFinished()) {
    settings_text_busy_count++;
  } else {
    settings_text_mutex = true;

    if (diff != 0) {
      // Shift Settings.text up or down
      memmove_P(Settings.text_pool + start_pos + replace_len, Settings.text_pool + end_pos, char_len - end_pos);
    }
    // Replace text
    memmove_P(Settings.text_pool + start_pos, replace, replace_len);
    // Fill for future use
    memset(Settings.text_pool + char_len + diff, 0x00, settings_text_size - char_len - diff);

    settings_text_mutex = false;
  }

#ifdef DEBUG_FUNC_SETTINGSUPDATETEXT
  AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_CONFIG "CR %d/%d, Busy %d, Id %02d = \"%s\""), GetSettingsTextLen(), settings_text_size, settings_text_busy_count, index_save, replace);
#else
  AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_CONFIG "CR %d/%d, Busy %d"), GetSettingsTextLen(), settings_text_size, settings_text_busy_count);
#endif

  return true;
}

char* SettingsText(uint32_t index) {
  char* position = Settings.text_pool;

  if (index >= SET_MAX) {
    position += settings_text_size -1;  // Setting not supported - internal error - return empty string
  } else {
    SettingsUpdateFinished();
    for (;index > 0; index--) {
      while (*position++ != '\0') { }
    }
  }
  return position;
}

/*********************************************************************************************\
 * Config Save - Save parameters to Flash ONLY if any parameter has changed
\*********************************************************************************************/

void UpdateBackwardCompatibility(void) {
  // Perform updates for backward compatibility
  strlcpy(Settings.user_template_name, SettingsText(SET_TEMPLATE_NAME), sizeof(Settings.user_template_name));
}

uint32_t GetSettingsAddress(void) {
  return settings_location * SPI_FLASH_SEC_SIZE;
}

void SettingsSave(uint8_t rotate) {
/* Save configuration in eeprom or one of 7 slots below
 *
 * rotate 0 = Save in next flash slot
 * rotate 1 = Save only in eeprom flash slot until SetOption12 0 or restart
 * rotate 2 = Save in eeprom flash slot, erase next flash slots and continue depending on stop_flash_rotate
 * stop_flash_rotate 0 = Allow flash slot rotation (SetOption12 0)
 * stop_flash_rotate 1 = Allow only eeprom flash slot use (SetOption12 1)
 */
#ifndef FIRMWARE_MINIMAL
  XsnsCall(FUNC_SAVE_SETTINGS);
  XdrvCall(FUNC_SAVE_SETTINGS);
  UpdateBackwardCompatibility();
  if ((GetSettingsCrc32() != settings_crc32) || rotate) {
    if (1 == rotate) {                                 // Use eeprom flash slot only and disable flash rotate from now on (upgrade)
      TasmotaGlobal.stop_flash_rotate = 1;
    }

    if (TasmotaGlobal.stop_flash_rotate || (2 == rotate)) {  // Use eeprom flash slot and erase next flash slots if stop_flash_rotate is off (default)
      settings_location = EEPROM_LOCATION;
    } else {                                           // Rotate flash slots
      if (settings_location == EEPROM_LOCATION) {
        settings_location = SETTINGS_LOCATION;
      } else {
        settings_location--;
      }
      if (settings_location <= (SETTINGS_LOCATION - CFG_ROTATES)) {
        settings_location = EEPROM_LOCATION;
      }
    }

    Settings.save_flag++;
    if (UtcTime() > START_VALID_TIME) {
      Settings.cfg_timestamp = UtcTime();
    } else {
      Settings.cfg_timestamp++;
    }
    Settings.cfg_size = sizeof(Settings);
    Settings.cfg_crc = GetSettingsCrc();               // Keep for backward compatibility in case of fall-back just after upgrade
    Settings.cfg_crc32 = GetSettingsCrc32();

#ifdef ESP8266
#ifdef USE_UFILESYS
    TfsSaveFile(TASM_FILE_SETTINGS, (const uint8_t*)&Settings, sizeof(Settings));
#endif  // USE_UFILESYS
    if (ESP.flashEraseSector(settings_location)) {
      ESP.flashWrite(settings_location * SPI_FLASH_SEC_SIZE, (uint32*)&Settings, sizeof(Settings));
    }

    if (!TasmotaGlobal.stop_flash_rotate && rotate) {  // SetOption12 - (Settings) Switch between dynamic (0) or fixed (1) slot flash save location
      for (uint32_t i = 0; i < CFG_ROTATES; i++) {
        ESP.flashEraseSector(SETTINGS_LOCATION -i);    // Delete previous configurations by resetting to 0xFF
        delay(1);
      }
    }
    AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_CONFIG D_SAVED_TO_FLASH_AT " %X, " D_COUNT " %d, " D_BYTES " %d"), settings_location, Settings.save_flag, sizeof(Settings));
#endif  // ESP8266
#ifdef ESP32
    SettingsWrite(&Settings, sizeof(Settings));
    AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_CONFIG "Saved, " D_COUNT " %d, " D_BYTES " %d"), Settings.save_flag, sizeof(Settings));
#endif  // ESP32

    settings_crc32 = Settings.cfg_crc32;
  }
#endif  // FIRMWARE_MINIMAL
  RtcSettingsSave();
}

void SettingsLoad(void) {
#ifdef ESP8266
  // Load configuration from optional file and flash (eeprom and 7 additonal slots) if first valid load does not stop_flash_rotate
  // Activated with version 8.4.0.2 - Fails to read any config before version 6.6.0.11
  settings_location = 0;
  uint32_t save_flag = 0;
  uint32_t max_slots = CFG_ROTATES +1;
  uint32_t flash_location;
  uint32_t slot = 1;
#ifdef USE_UFILESYS
  if (TfsLoadFile(TASM_FILE_SETTINGS, (uint8_t*)&Settings, sizeof(Settings))) {
    flash_location = 1;
    slot = 0;
  }
#endif  // USE_UFILESYS
  while (slot <= max_slots) {                                  // Read all config pages in search of valid and latest
    if (slot > 0) {
      flash_location = (1 == slot) ? EEPROM_LOCATION : (2 == slot) ? SETTINGS_LOCATION : flash_location -1;
      ESP.flashRead(flash_location * SPI_FLASH_SEC_SIZE, (uint32*)&Settings, sizeof(Settings));
    }
    if ((Settings.cfg_crc32 != 0xFFFFFFFF) && (Settings.cfg_crc32 != 0x00000000) && (Settings.cfg_crc32 == GetSettingsCrc32())) {
      if (Settings.save_flag > save_flag) {                    // Find latest page based on incrementing save_flag
        save_flag = Settings.save_flag;
        settings_location = flash_location;
        if (Settings.flag.stop_flash_rotate && (1 == slot)) {  // Stop if only eeprom area should be used and it is valid
          break;
        }
      }
    }
    slot++;
    delay(1);
  }
  if (settings_location > 0) {
#ifdef USE_UFILESYS
    if (1 == settings_location) {
      TfsLoadFile(TASM_FILE_SETTINGS, (uint8_t*)&Settings, sizeof(Settings));
      AddLog(LOG_LEVEL_NONE, PSTR(D_LOG_CONFIG "Loaded from File, " D_COUNT " %lu"), Settings.save_flag);
    } else
#endif  // USE_UFILESYS
    {
      ESP.flashRead(settings_location * SPI_FLASH_SEC_SIZE, (uint32*)&Settings, sizeof(Settings));
      AddLog(LOG_LEVEL_NONE, PSTR(D_LOG_CONFIG D_LOADED_FROM_FLASH_AT " %X, " D_COUNT " %lu"), settings_location, Settings.save_flag);
    }
  }
#endif  // ESP8266
#ifdef ESP32
  uint32_t source = SettingsRead(&Settings, sizeof(Settings));
  if (source) { settings_location = 1; }
  AddLog(LOG_LEVEL_NONE, PSTR(D_LOG_CONFIG "Loaded from %s, " D_COUNT " %lu"), (source)?"File":"Nvm", Settings.save_flag);
#endif  // ESP32

#ifndef FIRMWARE_MINIMAL
  if ((0 == settings_location) || (Settings.cfg_holder != (uint16_t)CFG_HOLDER)) {  // Init defaults if cfg_holder differs from user settings in my_user_config.h
#ifdef USE_UFILESYS
    if (TfsLoadFile(TASM_FILE_SETTINGS_LKG, (uint8_t*)&Settings, sizeof(Settings)) && (Settings.cfg_crc32 == GetSettingsCrc32())) {
      settings_location = 1;
      AddLog(LOG_LEVEL_NONE, PSTR(D_LOG_CONFIG "Loaded from LKG File, " D_COUNT " %lu"), Settings.save_flag);
    } else
#endif  // USE_UFILESYS
    {
      SettingsDefault();
    }
  }
  settings_crc32 = GetSettingsCrc32();
#endif  // FIRMWARE_MINIMAL

  RtcSettingsLoad(1);
}

// Used in TLS - returns the timestamp of the last Flash settings write
uint32_t CfgTime(void) {
  return Settings.cfg_timestamp;
}

#ifdef ESP8266
void SettingsErase(uint8_t type) {
  /*
    For Arduino core and SDK:
    Erase only works from flash start address to SDK recognized flash end address (flashchip->chip_size = ESP.getFlashChipSize).
    Addresses above SDK recognized size (up to ESP.getFlashChipRealSize) are not accessable.
    For Esptool:
    The only way to erase whole flash is esptool which uses direct SPI writes to flash.

    The default erase function is EspTool (EsptoolErase)

    0 = Erase from program end until end of flash as seen by SDK including optional filesystem
    1 = Erase 16k SDK parameter area near end of flash as seen by SDK (0x0XFCxxx - 0x0XFFFFF) solving possible wifi errors
    2 = Erase from program end until end of flash as seen by SDK excluding optional filesystem
    3 = Erase Tasmota and SDK parameter area (0x0F3xxx - 0x0FFFFF)
    4 = Erase SDK parameter area used for wifi calibration (0x0FCxxx - 0x0FCFFF)
  */

#ifndef FIRMWARE_MINIMAL
                           // Reset 2 = Erase all flash from program end to end of physical flash
  uint32_t _sectorStart = (ESP.getSketchSize() / SPI_FLASH_SEC_SIZE) + 1;
  uint32_t _sectorEnd = ESP.getFlashChipRealSize() / SPI_FLASH_SEC_SIZE;  // Flash size as reported by hardware
  if (1 == type) {         // Reset 3 = SDK parameter area
    // source Esp.cpp and core_esp8266_phy.cpp
    _sectorStart = (ESP.getFlashChipSize() / SPI_FLASH_SEC_SIZE) - 4;
  }
  else if (2 == type) {    // Reset 5, 6 = Erase all flash from program end to end of physical flash but skip filesystem
/*
#ifdef USE_UFILESYS
    TfsDeleteFile(TASM_FILE_SETTINGS);  // Not needed as it is recreated by set defaults before restart
#endif
*/
    EsptoolErase(_sectorStart, FLASH_FS_START);
    _sectorStart = EEPROM_LOCATION;
    _sectorEnd = ESP.getFlashChipSize() / SPI_FLASH_SEC_SIZE;  // Flash size as seen by SDK
  }
  else if (3 == type) {    // QPC Reached = QPC and Tasmota and SDK parameter area (0x0F3xxx - 0x0FFFFF)
#ifdef USE_UFILESYS
    TfsDeleteFile(TASM_FILE_SETTINGS);
#endif
    EsptoolErase(SETTINGS_LOCATION - CFG_ROTATES, SETTINGS_LOCATION +1);
    _sectorStart = EEPROM_LOCATION;
    _sectorEnd = ESP.getFlashChipSize() / SPI_FLASH_SEC_SIZE;  // Flash size as seen by SDK
  }
  else if (4 == type) {    // WIFI_FORCE_RF_CAL_ERASE = SDK wifi calibration
    _sectorStart = EEPROM_LOCATION +1;                         // SDK phy area and Core calibration sector (0x0XFC000)
    _sectorEnd = _sectorStart +1;                              // SDK end of phy area and Core calibration sector (0x0XFCFFF)
  }

  EsptoolErase(_sectorStart, _sectorEnd);                      // Esptool - erases flash completely
#endif  // FIRMWARE_MINIMAL
}
#endif  // ESP8266

void SettingsSdkErase(void) {
  WiFi.disconnect(false);  // Delete SDK wifi config
  SettingsErase(1);
  delay(1000);
}

/********************************************************************************************/

void SettingsDefault(void) {
  AddLog(LOG_LEVEL_NONE, PSTR(D_LOG_CONFIG D_USE_DEFAULTS));
  SettingsDefaultSet1();
  SettingsDefaultSet2();
  SettingsSave(2);
}

void SettingsDefaultSet1(void) {
  memset(&Settings, 0x00, sizeof(Settings));

  Settings.cfg_holder = (uint16_t)CFG_HOLDER;
  Settings.cfg_size = sizeof(Settings);
//  Settings.save_flag = 0;
  Settings.version = VERSION;
//  Settings.bootcount = 0;
//  Settings.cfg_crc = 0;
}

// default Fingerprints in PROGMEM
const uint8_t default_fingerprint1[] PROGMEM = { MQTT_FINGERPRINT1 };
const uint8_t default_fingerprint2[] PROGMEM = { MQTT_FINGERPRINT2 };

void SettingsDefaultSet2(void) {
  memset((char*)&Settings +16, 0x00, sizeof(Settings) -16);

  // this little trick allows GCC to optimize the assignment by grouping values and doing only ORs
  SysBitfield   flag = { 0 };
  SysBitfield2  flag2 = { 0 };
  SysBitfield3  flag3 = { 0 };
  SysBitfield4  flag4 = { 0 };
  SysBitfield5  flag5 = { 0 };

#ifdef ESP8266
  Settings.gpio16_converted = 0xF5A0;
//  Settings.config_version = 0;  // ESP8266 (Has been 0 for long time)
#endif  // ESP8266
#ifdef ESP32
  Settings.config_version = 1;  // ESP32
#endif  // ESP32

  flag.stop_flash_rotate |= APP_FLASH_CYCLE;
  flag.global_state |= APP_ENABLE_LEDLINK;
  flag3.sleep_normal |= APP_NORMAL_SLEEP;
  flag3.no_power_feedback |= APP_NO_RELAY_SCAN;
  flag3.fast_power_cycle_disable |= APP_DISABLE_POWERCYCLE;
  flag3.bootcount_update |= DEEPSLEEP_BOOTCOUNT;
  flag3.mqtt_buttons |= MQTT_BUTTONS;
  Settings.save_data = SAVE_DATA;
  Settings.param[P_BACKLOG_DELAY] = MIN_BACKLOG_DELAY;
  Settings.param[P_BOOT_LOOP_OFFSET] = BOOT_LOOP_OFFSET;  // SetOption36
  Settings.param[P_RGB_REMAP] = RGB_REMAP_RGBW;
  Settings.sleep = APP_SLEEP;
  if (Settings.sleep < 50) {
    Settings.sleep = 50;                // Default to 50 for sleep, for now
  }

  // Module
  flag.interlock |= APP_INTERLOCK_MODE;
  Settings.interlock[0] = APP_INTERLOCK_GROUP_1;
  Settings.interlock[1] = APP_INTERLOCK_GROUP_2;
  Settings.interlock[2] = APP_INTERLOCK_GROUP_3;
  Settings.interlock[3] = APP_INTERLOCK_GROUP_4;
  Settings.module = MODULE;
  Settings.fallback_module = FALLBACK_MODULE;
  ModuleDefault(WEMOS);
//  for (uint32_t i = 0; i < nitems(Settings.my_gp.io); i++) { Settings.my_gp.io[i] = GPIO_NONE; }
  SettingsUpdateText(SET_FRIENDLYNAME1, PSTR(FRIENDLY_NAME));
  SettingsUpdateText(SET_FRIENDLYNAME2, PSTR(FRIENDLY_NAME"2"));
  SettingsUpdateText(SET_FRIENDLYNAME3, PSTR(FRIENDLY_NAME"3"));
  SettingsUpdateText(SET_FRIENDLYNAME4, PSTR(FRIENDLY_NAME"4"));
  SettingsUpdateText(SET_DEVICENAME, SettingsText(SET_FRIENDLYNAME1));
  SettingsUpdateText(SET_OTAURL, PSTR(OTA_URL));

  // Power
  flag.save_state |= SAVE_STATE;
  Settings.power = APP_POWER;
  Settings.poweronstate = APP_POWERON_STATE;
  Settings.blinktime = APP_BLINKTIME;
  Settings.blinkcount = APP_BLINKCOUNT;
  Settings.ledstate = APP_LEDSTATE;
  Settings.ledmask = APP_LEDMASK;
//  Settings.ledpwm_off = 0;
  Settings.ledpwm_on = 255;
//  Settings.ledpwm_mask = 0;
  Settings.pulse_timer[0] = APP_PULSETIME;
//  for (uint32_t i = 1; i < MAX_PULSETIMERS; i++) { Settings.pulse_timer[i] = 0; }

  // Serial
  Settings.serial_config = TS_SERIAL_8N1;
  Settings.baudrate = APP_BAUDRATE / 300;
  Settings.sbaudrate = SOFT_BAUDRATE / 300;
  Settings.serial_delimiter = 0xff;
  Settings.seriallog_level = SERIAL_LOG_LEVEL;

  // Ethernet
  flag4.network_ethernet |= 1;
#ifdef ESP32
  Settings.eth_type = ETH_TYPE;
  Settings.eth_clk_mode = ETH_CLKMODE;
  Settings.eth_address = ETH_ADDRESS;
#endif  // ESP32

  // Wifi
  flag4.network_wifi |= 1;
  flag3.use_wifi_scan |= WIFI_SCAN_AT_RESTART;
  flag3.use_wifi_rescan |= WIFI_SCAN_REGULARLY;
  Settings.wifi_output_power = 170;
  Settings.param[P_ARP_GRATUITOUS] = WIFI_ARP_INTERVAL;
  ParseIPv4(&Settings.ipv4_address[0], PSTR(WIFI_IP_ADDRESS));
  ParseIPv4(&Settings.ipv4_address[1], PSTR(WIFI_GATEWAY));
  ParseIPv4(&Settings.ipv4_address[2], PSTR(WIFI_SUBNETMASK));
  ParseIPv4(&Settings.ipv4_address[3], PSTR(WIFI_DNS));
  Settings.sta_config = WIFI_CONFIG_TOOL;
//  Settings.sta_active = 0;
  SettingsUpdateText(SET_STASSID1, PSTR(STA_SSID1));
  SettingsUpdateText(SET_STASSID2, PSTR(STA_SSID2));
  SettingsUpdateText(SET_STAPWD1, PSTR(STA_PASS1));
  SettingsUpdateText(SET_STAPWD2, PSTR(STA_PASS2));
  SettingsUpdateText(SET_HOSTNAME, WIFI_HOSTNAME);

  // Syslog
  SettingsUpdateText(SET_SYSLOG_HOST, PSTR(SYS_LOG_HOST));
  Settings.syslog_port = SYS_LOG_PORT;
  Settings.syslog_level = SYS_LOG_LEVEL;

  // Webserver
  flag2.emulation |= EMULATION;
  flag4.alexa_gen_1 |= EMULATION_HUE_1ST_GEN;
  flag3.gui_hostname_ip |= GUI_SHOW_HOSTNAME;
  flag3.mdns_enabled |= MDNS_ENABLED;
  Settings.webserver = WEB_SERVER;
  Settings.weblog_level = WEB_LOG_LEVEL;
  SettingsUpdateText(SET_WEBPWD, PSTR(WEB_PASSWORD));
  SettingsUpdateText(SET_CORS, PSTR(CORS_DOMAIN));

  // Button
  flag.button_restrict |= KEY_DISABLE_MULTIPRESS;
  flag.button_swap |= KEY_SWAP_DOUBLE_PRESS;
  flag.button_single |= KEY_ONLY_SINGLE_PRESS;
  Settings.param[P_HOLD_TIME] = KEY_HOLD_TIME;  // Default 4 seconds hold time

  // Switch
  for (uint32_t i = 0; i < MAX_SWITCHES_SET; i++) { Settings.switchmode[i] = SWITCH_MODE; }

  // MQTT
  flag.mqtt_enabled |= MQTT_USE;
  flag.mqtt_response |= MQTT_RESULT_COMMAND;
  flag.mqtt_offline |= MQTT_LWT_MESSAGE;
  flag.mqtt_power_retain |= MQTT_POWER_RETAIN;
  flag.mqtt_button_retain |= MQTT_BUTTON_RETAIN;
  flag.mqtt_switch_retain |= MQTT_SWITCH_RETAIN;
  flag.mqtt_sensor_retain |= MQTT_SENSOR_RETAIN;
  flag5.mqtt_info_retain |= MQTT_INFO_RETAIN;
  flag5.mqtt_state_retain |= MQTT_STATE_RETAIN;
  flag5.mqtt_switches |= MQTT_SWITCHES;
//  flag.mqtt_serial |= 0;
  flag.device_index_enable |= MQTT_POWER_FORMAT;
  flag3.time_append_timezone |= MQTT_APPEND_TIMEZONE;
  flag3.button_switch_force_local |= MQTT_BUTTON_SWITCH_FORCE_LOCAL;
  flag3.no_hold_retain |= MQTT_NO_HOLD_RETAIN;
  flag3.use_underscore |= MQTT_INDEX_SEPARATOR;
  flag3.grouptopic_mode |= MQTT_GROUPTOPIC_FORMAT;
  SettingsUpdateText(SET_MQTT_HOST, MQTT_HOST);
  Settings.mqtt_port = MQTT_PORT;
  SettingsUpdateText(SET_MQTT_CLIENT, PSTR(MQTT_CLIENT_ID));
  SettingsUpdateText(SET_MQTT_USER, PSTR(MQTT_USER));
  SettingsUpdateText(SET_MQTT_PWD, PSTR(MQTT_PASS));
  SettingsUpdateText(SET_MQTT_TOPIC, PSTR(MQTT_TOPIC));
  SettingsUpdateText(SET_MQTT_BUTTON_TOPIC, PSTR(MQTT_BUTTON_TOPIC));
  SettingsUpdateText(SET_MQTT_SWITCH_TOPIC, PSTR(MQTT_SWITCH_TOPIC));
  SettingsUpdateText(SET_MQTT_GRP_TOPIC, PSTR(MQTT_GRPTOPIC));
  SettingsUpdateText(SET_MQTT_FULLTOPIC, PSTR(MQTT_FULLTOPIC));
  Settings.mqtt_retry = MQTT_RETRY_SECS;
  SettingsUpdateText(SET_MQTTPREFIX1, PSTR(SUB_PREFIX));
  SettingsUpdateText(SET_MQTTPREFIX2, PSTR(PUB_PREFIX));
  SettingsUpdateText(SET_MQTTPREFIX3, PSTR(PUB_PREFIX2));
  SettingsUpdateText(SET_STATE_TXT1, PSTR(MQTT_STATUS_OFF));
  SettingsUpdateText(SET_STATE_TXT2, PSTR(MQTT_STATUS_ON));
  SettingsUpdateText(SET_STATE_TXT3, PSTR(MQTT_CMND_TOGGLE));
  SettingsUpdateText(SET_STATE_TXT4, PSTR(MQTT_CMND_HOLD));
  memcpy_P(Settings.mqtt_fingerprint[0], default_fingerprint1, sizeof(default_fingerprint1));
  memcpy_P(Settings.mqtt_fingerprint[1], default_fingerprint2, sizeof(default_fingerprint2));
  Settings.tele_period = TELE_PERIOD;
  Settings.mqttlog_level = MQTT_LOG_LEVEL;
  Settings.mqtt_keepalive = MQTT_KEEPALIVE;
  Settings.mqtt_socket_timeout = MQTT_SOCKET_TIMEOUT;

  // Energy
  flag.no_power_on_check |= ENERGY_VOLTAGE_ALWAYS;
  flag2.current_resolution |= 3;
//  flag2.voltage_resolution |= 0;
//  flag2.wattage_resolution |= 0;
  flag2.energy_resolution |= ENERGY_RESOLUTION;
  flag3.dds2382_model |= ENERGY_DDS2382_MODE;
  flag3.hardware_energy_total |= ENERGY_HARDWARE_TOTALS;
  Settings.param[P_MAX_POWER_RETRY] = MAX_POWER_RETRY;
//  Settings.energy_power_delta[0] = 0;
//  Settings.energy_power_delta[1] = 0;
//  Settings.energy_power_delta[2] = 0;
  Settings.energy_power_calibration = HLW_PREF_PULSE;
  Settings.energy_voltage_calibration = HLW_UREF_PULSE;
  Settings.energy_current_calibration = HLW_IREF_PULSE;
//  Settings.energy_kWhtoday = 0;
//  Settings.energy_kWhyesterday = 0;
//  Settings.energy_kWhdoy = 0;
//  Settings.energy_min_power = 0;
//  Settings.energy_max_power = 0;
//  Settings.energy_min_voltage = 0;
//  Settings.energy_max_voltage = 0;
//  Settings.energy_min_current = 0;
//  Settings.energy_max_current = 0;
//  Settings.energy_max_power_limit = 0;                            // MaxPowerLimit
  Settings.energy_max_power_limit_hold = MAX_POWER_HOLD;
  Settings.energy_max_power_limit_window = MAX_POWER_WINDOW;
//  Settings.energy_max_power_safe_limit = 0;                       // MaxSafePowerLimit
  Settings.energy_max_power_safe_limit_hold = SAFE_POWER_HOLD;
  Settings.energy_max_power_safe_limit_window = SAFE_POWER_WINDOW;
//  Settings.energy_max_energy = 0;                                 // MaxEnergy
//  Settings.energy_max_energy_start = 0;                           // MaxEnergyStart
//  Settings.energy_kWhtotal = 0;
  RtcSettings.energy_kWhtotal = 0;
//  memset((char*)&Settings.energy_usage, 0x00, sizeof(Settings.energy_usage));
  memset((char*)&RtcSettings.energy_usage, 0x00, sizeof(RtcSettings.energy_usage));
  Settings.param[P_OVER_TEMP] = ENERGY_OVERTEMP;

  // IRRemote
  flag.ir_receive_decimal |= IR_DATA_RADIX;
  flag3.receive_raw |= IR_ADD_RAW_DATA;
  Settings.param[P_IR_UNKNOW_THRESHOLD] = IR_RCV_MIN_UNKNOWN_SIZE;

  // RF Bridge
  flag.rf_receive_decimal |= RF_DATA_RADIX;
//  for (uint32_t i = 0; i < 17; i++) { Settings.rf_code[i][0] = 0; }
  memcpy_P(Settings.rf_code[0], kDefaultRfCode, 9);

  // Domoticz
  Settings.domoticz_update_timer = DOMOTICZ_UPDATE_TIMER;
//  for (uint32_t i = 0; i < MAX_DOMOTICZ_IDX; i++) {
//    Settings.domoticz_relay_idx[i] = 0;
//    Settings.domoticz_key_idx[i] = 0;
//    Settings.domoticz_switch_idx[i] = 0;
//  }
//  for (uint32_t i = 0; i < MAX_DOMOTICZ_SNS_IDX; i++) {
//    Settings.domoticz_sensor_idx[i] = 0;
//  }

  // Sensor
  flag.temperature_conversion |= TEMP_CONVERSION;
  flag.pressure_conversion |= PRESSURE_CONVERSION;
  flag2.pressure_resolution |= PRESSURE_RESOLUTION;
  flag2.humidity_resolution |= HUMIDITY_RESOLUTION;
  flag2.temperature_resolution |= TEMP_RESOLUTION;
  flag3.ds18x20_internal_pullup |= DS18X20_PULL_UP;
  flag3.counter_reset_on_tele |= COUNTER_RESET;
//  Settings.altitude = 0;

  // Rules
//  Settings.rule_enabled = 0;
//  Settings.rule_once = 0;
//  for (uint32_t i = 1; i < MAX_RULE_SETS; i++) { Settings.rules[i][0] = '\0'; }
  flag2.calc_resolution |= CALC_RESOLUTION;

  // Timer
  flag3.timers_enable |= TIMERS_ENABLED;

  // Home Assistant
  flag.hass_light |= HASS_AS_LIGHT;
  flag.hass_discovery |= HOME_ASSISTANT_DISCOVERY_ENABLE;
  flag3.hass_tele_on_power |= TELE_ON_POWER;

  // Knx
  flag.knx_enabled |= KNX_ENABLED;
  flag.knx_enable_enhancement |= KNX_ENHANCED;

  // Light
  flag.pwm_control |= LIGHT_MODE;
  flag.ws_clock_reverse |= LIGHT_CLOCK_DIRECTION;
  flag.light_signal |= LIGHT_PAIRS_CO2;
  flag.not_power_linked |= LIGHT_POWER_CONTROL;
  flag.decimal_text |= LIGHT_COLOR_RADIX;
  flag3.pwm_multi_channels |= LIGHT_CHANNEL_MODE;
  flag3.slider_dimmer_stay_on |= LIGHT_SLIDER_POWER;
  flag4.alexa_ct_range |= LIGHT_ALEXA_CT_RANGE;
  flag4.pwm_ct_mode |= LIGHT_PWM_CT_MODE;
  flag4.white_blend_mode |= LIGHT_WHITE_BLEND_MODE;
  flag4.virtual_ct |= LIGHT_VIRTUAL_CT;
  flag4.virtual_ct_cw |= LIGHT_VIRTUAL_CT_CW;

  Settings.pwm_frequency = PWM_FREQ;
  Settings.pwm_range = PWM_RANGE;
  for (uint32_t i = 0; i < MAX_PWMS; i++) {
    Settings.light_color[i] = DEFAULT_LIGHT_COMPONENT;
//    Settings.pwm_value[i] = 0;
  }
  Settings.light_correction = 1;
  Settings.light_dimmer = DEFAULT_LIGHT_DIMMER;
//  Settings.light_fade = 0;
  Settings.light_speed = 1;
//  Settings.light_scheme = 0;
  Settings.light_width = 1;
//  Settings.light_wakeup = 0;
  Settings.light_pixels = WS2812_LEDS;
//  Settings.light_rotation = 0;
  Settings.ws_width[WS_SECOND] = 1;
  Settings.ws_color[WS_SECOND][WS_RED] = 255;
//  Settings.ws_color[WS_SECOND][WS_GREEN] = 0;
  Settings.ws_color[WS_SECOND][WS_BLUE] = 255;
  Settings.ws_width[WS_MINUTE] = 3;
//  Settings.ws_color[WS_MINUTE][WS_RED] = 0;
  Settings.ws_color[WS_MINUTE][WS_GREEN] = 255;
//  Settings.ws_color[WS_MINUTE][WS_BLUE] = 0;
  Settings.ws_width[WS_HOUR] = 5;
  Settings.ws_color[WS_HOUR][WS_RED] = 255;
//  Settings.ws_color[WS_HOUR][WS_GREEN] = 0;
//  Settings.ws_color[WS_HOUR][WS_BLUE] = 0;

  Settings.dimmer_hw_max = DEFAULT_DIMMER_MAX;
  Settings.dimmer_hw_min = DEFAULT_DIMMER_MIN;

  Settings.dimmer_step = DEFAULT_DIMMER_STEP;

  // Device Groups
  *(uint32_t *)&Settings.device_group_tie = 0x04030201;

  // Display
//  Settings.display_model = 0;
  Settings.display_mode = 1;
  Settings.display_refresh = 2;
  Settings.display_rows = 2;
  Settings.display_cols[0] = 16;
  Settings.display_cols[1] = 8;
  Settings.display_dimmer = 1;
  Settings.display_size = 1;
  Settings.display_font = 1;
//  Settings.display_rotate = 0;
  Settings.display_address[0] = MTX_ADDRESS1;
  Settings.display_address[1] = MTX_ADDRESS2;
  Settings.display_address[2] = MTX_ADDRESS3;
  Settings.display_address[3] = MTX_ADDRESS4;
  Settings.display_address[4] = MTX_ADDRESS5;
  Settings.display_address[5] = MTX_ADDRESS6;
  Settings.display_address[6] = MTX_ADDRESS7;
  Settings.display_address[7] = MTX_ADDRESS8;

  // Time
  if (((APP_TIMEZONE > -14) && (APP_TIMEZONE < 15)) || (99 == APP_TIMEZONE)) {
    Settings.timezone = APP_TIMEZONE;
    Settings.timezone_minutes = 0;
  } else {
    Settings.timezone = APP_TIMEZONE / 60;
    Settings.timezone_minutes = abs(APP_TIMEZONE % 60);
  }
  SettingsUpdateText(SET_NTPSERVER1, PSTR(NTP_SERVER1));
  SettingsUpdateText(SET_NTPSERVER2, PSTR(NTP_SERVER2));
  SettingsUpdateText(SET_NTPSERVER3, PSTR(NTP_SERVER3));
  for (uint32_t i = 0; i < MAX_NTP_SERVERS; i++) {
    SettingsUpdateText(SET_NTPSERVER1 +i, ReplaceCommaWithDot(SettingsText(SET_NTPSERVER1 +i)));
  }
  Settings.latitude = (int)((double)LATITUDE * 1000000);
  Settings.longitude = (int)((double)LONGITUDE * 1000000);
  SettingsResetStd();
  SettingsResetDst();

  Settings.button_debounce = KEY_DEBOUNCE_TIME;
  Settings.switch_debounce = SWITCH_DEBOUNCE_TIME;

  for (uint32_t j = 0; j < 5; j++) {
    Settings.rgbwwTable[j] = 255;
  }

  Settings.novasds_startingoffset = STARTING_OFFSET;

  SettingsDefaultWebColor();

  memset(&Settings.monitors, 0xFF, 20);  // Enable all possible monitors, displays and sensors
  SettingsEnableAllI2cDrivers();

  // Tuya
  flag3.tuya_apply_o20 |= TUYA_SETOPTION_20;
  flag3.tuya_serial_mqtt_publish |= MQTT_TUYA_RECEIVED;

  flag3.buzzer_enable |= BUZZER_ENABLE;
  flag3.shutter_mode |= SHUTTER_SUPPORT;
  flag3.pcf8574_ports_inverted |= PCF8574_INVERT_PORTS;
  flag4.zigbee_use_names |= ZIGBEE_FRIENDLY_NAMES;
  flag4.zigbee_distinct_topics |= ZIGBEE_DISTINCT_TOPICS;
  flag4.remove_zbreceived |= ZIGBEE_RMV_ZBRECEIVED;
  flag4.zb_index_ep |= ZIGBEE_INDEX_EP;
  flag4.mqtt_tls |= MQTT_TLS_ENABLED;
  flag4.mqtt_no_retain |= MQTT_NO_RETAIN;

#ifdef USER_TEMPLATE
  String user_template = USER_TEMPLATE;
  JsonTemplate((char*)user_template.c_str());
#endif

  Settings.flag = flag;
  Settings.flag2 = flag2;
  Settings.flag3 = flag3;
  Settings.flag4 = flag4;
  Settings.flag5 = flag5;
}

/********************************************************************************************/

void SettingsResetStd(void) {
  Settings.tflag[0].hemis = TIME_STD_HEMISPHERE;
  Settings.tflag[0].week = TIME_STD_WEEK;
  Settings.tflag[0].dow = TIME_STD_DAY;
  Settings.tflag[0].month = TIME_STD_MONTH;
  Settings.tflag[0].hour = TIME_STD_HOUR;
  Settings.toffset[0] = TIME_STD_OFFSET;
}

void SettingsResetDst(void) {
  Settings.tflag[1].hemis = TIME_DST_HEMISPHERE;
  Settings.tflag[1].week = TIME_DST_WEEK;
  Settings.tflag[1].dow = TIME_DST_DAY;
  Settings.tflag[1].month = TIME_DST_MONTH;
  Settings.tflag[1].hour = TIME_DST_HOUR;
  Settings.toffset[1] = TIME_DST_OFFSET;
}

void SettingsDefaultWebColor(void) {
  char scolor[10];
  for (uint32_t i = 0; i < COL_LAST; i++) {
    WebHexCode(i, GetTextIndexed(scolor, sizeof(scolor), i, kWebColors));
  }
}

void SettingsEnableAllI2cDrivers(void) {
  Settings.i2c_drivers[0] = I2CDRIVERS_0_31;
  Settings.i2c_drivers[1] = I2CDRIVERS_32_63;
  Settings.i2c_drivers[2] = I2CDRIVERS_64_95;
}

/********************************************************************************************/

void SettingsDelta(void) {
  if (Settings.version != VERSION) {      // Fix version dependent changes

#ifdef ESP8266
#ifndef UPGRADE_V8_MIN
    // Although no direct upgrade is supported try to make a viable environment
    if (Settings.version < 0x08000000) {
      // Save SSIDs and Passwords
      char temp31[strlen(Settings.ex_sta_ssid[0]) +1];
      strncpy(temp31, Settings.ex_sta_ssid[0], sizeof(temp31));
      char temp32[strlen(Settings.ex_sta_ssid[1]) +1];
      strncpy(temp32, Settings.ex_sta_ssid[1], sizeof(temp32));
      char temp41[strlen(Settings.ex_sta_pwd[0]) +1];
      strncpy(temp41, Settings.ex_sta_pwd[0], sizeof(temp41));
      char temp42[strlen(Settings.ex_sta_pwd[1]) +1];
      strncpy(temp42, Settings.ex_sta_pwd[1], sizeof(temp42));

      char temp7[strlen(Settings.ex_mqtt_host) +1];
      strncpy(temp7, Settings.ex_mqtt_host, sizeof(temp7));
      char temp9[strlen(Settings.ex_mqtt_user) +1];
      strncpy(temp9, Settings.ex_mqtt_user, sizeof(temp9));
      char temp10[strlen(Settings.ex_mqtt_pwd) +1];
      strncpy(temp10, Settings.ex_mqtt_pwd, sizeof(temp10));
      char temp11[strlen(Settings.ex_mqtt_topic) +1];
      strncpy(temp11, Settings.ex_mqtt_topic, sizeof(temp11));

      SettingsDefault();

      // Restore current SSIDs and Passwords
      SettingsUpdateText(SET_STASSID1, temp31);
      SettingsUpdateText(SET_STASSID2, temp32);
      SettingsUpdateText(SET_STAPWD1, temp41);
      SettingsUpdateText(SET_STAPWD2, temp42);

#if defined(USE_MQTT_TLS) && defined(USE_MQTT_AWS_IOT)
      if (!strlen(Settings.ex_mqtt_user)) {
        SettingsUpdateText(SET_MQTT_HOST, temp7);
        SettingsUpdateText(SET_MQTT_USER, temp9);
      } else {
        char aws_mqtt_host[66];
        snprintf_P(aws_mqtt_host, sizeof(aws_mqtt_host), PSTR("%s%s"), temp9, temp7);
        SettingsUpdateText(SET_MQTT_HOST, aws_mqtt_host);
        SettingsUpdateText(SET_MQTT_USER, "");
      }
#else  // No USE_MQTT_TLS and USE_MQTT_AWS_IOT
      SettingsUpdateText(SET_MQTT_HOST, temp7);
      SettingsUpdateText(SET_MQTT_USER, temp9);
#endif  // USE_MQTT_TLS and USE_MQTT_AWS_IOT
      SettingsUpdateText(SET_MQTT_PWD, temp10);
      SettingsUpdateText(SET_MQTT_TOPIC, temp11);
    }
#endif  // UPGRADE_V8_MIN

    if (Settings.version < 0x08020003) {
      SettingsUpdateText(SET_TEMPLATE_NAME, Settings.user_template_name);
      Settings.zb_channel = 0;      // set channel to zero to force reinit of zigbee parameters
    }
#endif  // ESP8266

    if (Settings.version < 0x08020004) {
      Settings.flag3.mqtt_buttons = 0;  // SetOption73 (0) - Decouple button from relay and send just mqtt topic
#ifdef ESP8266
      Settings.config_version = 0;  // ESP8266 (Has been 0 for long time)
#endif  // ESP8266
#ifdef ESP32
      Settings.config_version = 1;  // ESP32
#endif  // ESP32
    }
    if (Settings.version < 0x08020006) {
#ifdef ESP32
      Settings.module = WEMOS;
      ModuleDefault(WEMOS);
#endif  // ESP32
      // make sure the empty rules have two consecutive NULLs, to be compatible with compressed rules
      if (Settings.rules[0][0] == 0) { Settings.rules[0][1] = 0; }
      if (Settings.rules[1][0] == 0) { Settings.rules[1][1] = 0; }
      if (Settings.rules[2][0] == 0) { Settings.rules[2][1] = 0; }
    }
    if (Settings.version < 0x08030002) {
      SettingsUpdateText(SET_DEVICENAME, SettingsText(SET_FRIENDLYNAME1));
      Settings.ledpwm_off = 0;
      Settings.ledpwm_on = 255;
      Settings.ledpwm_mask = 0;
    }
    if (Settings.version < 0x08030104) {
      Settings.flag4.network_wifi = 1;
      Settings.flag4.network_ethernet = 1;
    }
#ifdef ESP32
    if (Settings.version < 0x08030105) {
      Settings.eth_type = ETH_TYPE;
      Settings.eth_clk_mode = ETH_CLKMODE;
      Settings.eth_address = ETH_ADDRESS;
    }
#endif  // ESP32
    if (Settings.version < 0x08030106) {
      Settings.fallback_module = FALLBACK_MODULE;
    }
    if (Settings.version < 0x08040003) {
      Settings.energy_power_delta[0] = Settings.hass_new_discovery; // replaced ex2_energy_power_delta on 8.5.0.1
      Settings.energy_power_delta[1] = 0;
      Settings.energy_power_delta[2] = 0;
    }
#ifdef ESP8266
    if (Settings.version < 0x09000002) {
      char parameters[32];
      snprintf_P(parameters, sizeof(parameters), PSTR("%d,%d,%d,%d,%d"),
        Settings.ex_adc_param_type, Settings.ex_adc_param1, Settings.ex_adc_param2, Settings.ex_adc_param3, Settings.ex_adc_param4);
      SettingsUpdateText(SET_ADC_PARAM1, parameters);
    }
#endif  // ESP8266
    if (Settings.version < 0x09010000) {
      Settings.dimmer_step = DEFAULT_DIMMER_STEP;
    }
    if (Settings.version < 0x09020003) {
      Settings.flag3.use_wifi_rescan = true;  // As a result of #10395
    }
    if (Settings.version < 0x09020006) {
      for (uint32_t i = 0; i < MAX_SWITCHES_SET; i++) {
        Settings.switchmode[i] = (i < 8) ? Settings.ex_switchmode[i] : SWITCH_MODE;
      }
      for (uint32_t i = 0; i < MAX_INTERLOCKS_SET; i++) {
        Settings.interlock[i] = (i < 4) ? Settings.ex_interlock[i] : 0;
      }
    }
    if (Settings.version < 0x09020007) {
      *(uint32_t *)&Settings.device_group_tie = 0x04030201;
    }
    if (Settings.version < 0x09030102) {
      Settings.mqtt_keepalive = MQTT_KEEPALIVE;
      Settings.mqtt_socket_timeout = MQTT_SOCKET_TIMEOUT;
    }

    Settings.version = VERSION;
    SettingsSave(1);
  }

}
