/*
  settings.ino - user settings for Sonoff-Tasmota

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

#ifndef DOMOTICZ_UPDATE_TIMER
#define DOMOTICZ_UPDATE_TIMER  0            // [DomoticzUpdateTimer] Send relay status (0 = disable, 1 - 3600 seconds) (Optional)
#endif

#ifndef EMULATION
#define EMULATION              EMUL_NONE    // [Emulation] Select Belkin WeMo (single relay/light) or Hue Bridge emulation (multi relay/light) (EMUL_NONE, EMUL_WEMO or EMUL_HUE)
#endif

#ifndef MTX_ADDRESS1                        // Add Display Support for up to eigth Matrices
#define MTX_ADDRESS1           0
#endif
#ifndef MTX_ADDRESS2
#define MTX_ADDRESS2           0
#endif
#ifndef MTX_ADDRESS3
#define MTX_ADDRESS3           0
#endif
#ifndef MTX_ADDRESS4
#define MTX_ADDRESS4           0
#endif
#ifndef MTX_ADDRESS5
#define MTX_ADDRESS5           0
#endif
#ifndef MTX_ADDRESS6
#define MTX_ADDRESS6           0
#endif
#ifndef MTX_ADDRESS7
#define MTX_ADDRESS7           0
#endif
#ifndef MTX_ADDRESS8
#define MTX_ADDRESS8           0
#endif

#ifndef HOME_ASSISTANT_DISCOVERY_ENABLE
#define HOME_ASSISTANT_DISCOVERY_ENABLE 0
#endif

#ifndef LATITUDE
#define LATITUDE               48.858360         // [Latitude] Your location to be used with sunrise and sunset
#endif
#ifndef LONGITUDE
#define LONGITUDE              2.294442          // [Longitude] Your location to be used with sunrise and sunset
#endif

/*********************************************************************************************\
 * RTC memory
\*********************************************************************************************/

#define RTC_MEM_VALID 0xA55A

uint32_t rtc_settings_crc = 0;

uint32_t GetRtcSettingsCrc(void)
{
  uint32_t crc = 0;
  uint8_t *bytes = (uint8_t*)&RtcSettings;

  for (uint16_t i = 0; i < sizeof(RTCMEM); i++) {
    crc += bytes[i]*(i+1);
  }
  return crc;
}

void RtcSettingsSave(void)
{
  if (GetRtcSettingsCrc() != rtc_settings_crc) {
    RtcSettings.valid = RTC_MEM_VALID;
    ESP.rtcUserMemoryWrite(100, (uint32_t*)&RtcSettings, sizeof(RTCMEM));
    rtc_settings_crc = GetRtcSettingsCrc();
  }
}

void RtcSettingsLoad(void)
{
  ESP.rtcUserMemoryRead(100, (uint32_t*)&RtcSettings, sizeof(RTCMEM));  // 0x290
  if (RtcSettings.valid != RTC_MEM_VALID) {
    memset(&RtcSettings, 0, sizeof(RTCMEM));
    RtcSettings.valid = RTC_MEM_VALID;
    RtcSettings.energy_kWhtoday = Settings.energy_kWhtoday;
    RtcSettings.energy_kWhtotal = Settings.energy_kWhtotal;
    for (uint8_t i = 0; i < MAX_COUNTERS; i++) {
      RtcSettings.pulse_counter[i] = Settings.pulse_counter[i];
    }
    RtcSettings.power = Settings.power;
    RtcSettingsSave();
  }
  rtc_settings_crc = GetRtcSettingsCrc();
}

bool RtcSettingsValid(void)
{
  return (RTC_MEM_VALID == RtcSettings.valid);
}

/********************************************************************************************/

uint32_t rtc_reboot_crc = 0;

uint32_t GetRtcRebootCrc(void)
{
  uint32_t crc = 0;
  uint8_t *bytes = (uint8_t*)&RtcReboot;

  for (uint16_t i = 0; i < sizeof(RTCRBT); i++) {
    crc += bytes[i]*(i+1);
  }
  return crc;
}

void RtcRebootSave(void)
{
  if (GetRtcRebootCrc() != rtc_reboot_crc) {
    RtcReboot.valid = RTC_MEM_VALID;
    ESP.rtcUserMemoryWrite(100 - sizeof(RTCRBT), (uint32_t*)&RtcReboot, sizeof(RTCRBT));
    rtc_reboot_crc = GetRtcRebootCrc();
  }
}

void RtcRebootLoad(void)
{
  ESP.rtcUserMemoryRead(100 - sizeof(RTCRBT), (uint32_t*)&RtcReboot, sizeof(RTCRBT));  // 0x280
  if (RtcReboot.valid != RTC_MEM_VALID) {
    memset(&RtcReboot, 0, sizeof(RTCRBT));
    RtcReboot.valid = RTC_MEM_VALID;
//    RtcReboot.fast_reboot_count = 0;  // Explicit by memset
    RtcRebootSave();
  }
  rtc_reboot_crc = GetRtcRebootCrc();
}

bool RtcRebootValid(void)
{
  return (RTC_MEM_VALID == RtcReboot.valid);
}

/*********************************************************************************************\
 * Config - Flash
\*********************************************************************************************/

extern "C" {
#include "spi_flash.h"
}
#include "eboot_command.h"

extern "C" uint32_t _SPIFFS_end;

// From libraries/EEPROM/EEPROM.cpp EEPROMClass
#define SPIFFS_END          ((uint32_t)&_SPIFFS_end - 0x40200000) / SPI_FLASH_SEC_SIZE

// Version 4.2 config = eeprom area
#define SETTINGS_LOCATION   SPIFFS_END  // No need for SPIFFS as it uses EEPROM area
// Version 5.2 allow for more flash space
#define CFG_ROTATES         8           // Number of flash sectors used (handles uploads)

/*********************************************************************************************\
 * EEPROM support based on EEPROM library and tuned for Tasmota
\*********************************************************************************************/

uint32_t eeprom_sector = SPIFFS_END;
uint8_t* eeprom_data = 0;
size_t eeprom_size = 0;
bool eeprom_dirty = false;

void EepromBegin(size_t size)
{
  if (size <= 0) { return; }
  if (size > SPI_FLASH_SEC_SIZE - sizeof(Settings) -4) { size = SPI_FLASH_SEC_SIZE - sizeof(Settings) -4; }
  size = (size + 3) & (~3);

  // In case begin() is called a 2nd+ time, don't reallocate if size is the same
  if (eeprom_data && size != eeprom_size) {
    delete[] eeprom_data;
    eeprom_data = new uint8_t[size];
  } else if (!eeprom_data) {
    eeprom_data = new uint8_t[size];
  }
  eeprom_size = size;

  size_t flash_offset = SPI_FLASH_SEC_SIZE - eeprom_size;
  uint8_t* flash_buffer;
  flash_buffer = new uint8_t[SPI_FLASH_SEC_SIZE];
  noInterrupts();
  spi_flash_read(eeprom_sector * SPI_FLASH_SEC_SIZE, reinterpret_cast<uint32_t*>(flash_buffer), SPI_FLASH_SEC_SIZE);
  interrupts();
  memcpy(eeprom_data, flash_buffer + flash_offset, eeprom_size);
  delete[] flash_buffer;

  eeprom_dirty = false;  // make sure dirty is cleared in case begin() is called 2nd+ time
}

size_t EepromLength(void)
{
  return eeprom_size;
}

uint8_t EepromRead(int const address)
{
  if (address < 0 || (size_t)address >= eeprom_size) { return 0; }
  if (!eeprom_data) { return 0; }

  return eeprom_data[address];
}

// Prototype needed for Arduino IDE - https://forum.arduino.cc/index.php?topic=406509.0
template<typename T> T EepromGet(int const address, T &t);
template<typename T> T EepromGet(int const address, T &t)
{
  if (address < 0 || address + sizeof(T) > eeprom_size) { return t; }
  if (!eeprom_data) { return 0; }

  memcpy((uint8_t*) &t, eeprom_data + address, sizeof(T));
  return t;
}

void EepromWrite(int const address, uint8_t const value)
{
  if (address < 0 || (size_t)address >= eeprom_size) { return; }
  if (!eeprom_data) { return; }

  // Optimise eeprom_dirty. Only flagged if data written is different.
  uint8_t* pData = &eeprom_data[address];
  if (*pData != value) {
    *pData = value;
    eeprom_dirty = true;
  }
}

// Prototype needed for Arduino IDE - https://forum.arduino.cc/index.php?topic=406509.0
template<typename T> void EepromPut(int const address, const T &t);
template<typename T> void EepromPut(int const address, const T &t)
{
  if (address < 0 || address + sizeof(T) > eeprom_size) { return; }
  if (!eeprom_data) { return; }

  // Optimise eeprom_dirty. Only flagged if data written is different.
  if (memcmp(eeprom_data + address, (const uint8_t*)&t, sizeof(T)) != 0) {
    eeprom_dirty = true;
    memcpy(eeprom_data + address, (const uint8_t*)&t, sizeof(T));
  }
}

bool EepromCommit(void)
{
  bool ret = false;
  if (!eeprom_size) { return false; }
  if (!eeprom_dirty) { return true; }
  if (!eeprom_data) { return false; }

  size_t flash_offset = SPI_FLASH_SEC_SIZE - eeprom_size;
  uint8_t* flash_buffer;
  flash_buffer = new uint8_t[SPI_FLASH_SEC_SIZE];
  noInterrupts();
  spi_flash_read(eeprom_sector * SPI_FLASH_SEC_SIZE, reinterpret_cast<uint32_t*>(flash_buffer), SPI_FLASH_SEC_SIZE);
  memcpy(flash_buffer + flash_offset, eeprom_data, eeprom_size);
  if (spi_flash_erase_sector(eeprom_sector) == SPI_FLASH_RESULT_OK) {
    if (spi_flash_write(eeprom_sector * SPI_FLASH_SEC_SIZE, reinterpret_cast<uint32_t*>(flash_buffer), SPI_FLASH_SEC_SIZE) == SPI_FLASH_RESULT_OK) {
      eeprom_dirty = false;
      ret = true;
    }
  }
  interrupts();
  delete[] flash_buffer;

  return ret;
}

uint8_t * EepromGetDataPtr()
{
  eeprom_dirty = true;
  return &eeprom_data[0];
}

void EepromEnd(void)
{
  if (!eeprom_size) { return; }

  EepromCommit();
  if (eeprom_data) {
    delete[] eeprom_data;
  }
  eeprom_data = 0;
  eeprom_size = 0;
  eeprom_dirty = false;
}

/********************************************************************************************/

uint16_t settings_crc = 0;
uint32_t settings_location = SETTINGS_LOCATION;
uint8_t *settings_buffer = NULL;

/********************************************************************************************/
/*
 * Based on cores/esp8266/Updater.cpp
 */
void SetFlashModeDout(void)
{
  uint8_t *_buffer;
  uint32_t address;

  eboot_command ebcmd;
  eboot_command_read(&ebcmd);
  address = ebcmd.args[0];
  _buffer = new uint8_t[FLASH_SECTOR_SIZE];

  if (ESP.flashRead(address, (uint32_t*)_buffer, FLASH_SECTOR_SIZE)) {
    if (_buffer[2] != 3) {  // DOUT
      _buffer[2] = 3;
      if (ESP.flashEraseSector(address / FLASH_SECTOR_SIZE)) ESP.flashWrite(address, (uint32_t*)_buffer, FLASH_SECTOR_SIZE);
    }
  }
  delete[] _buffer;
}

void SettingsBufferFree(void)
{
  if (settings_buffer != NULL) {
    free(settings_buffer);
    settings_buffer = NULL;
  }
}

bool SettingsBufferAlloc(void)
{
  SettingsBufferFree();
  if (!(settings_buffer = (uint8_t *)malloc(sizeof(Settings)))) {
    AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_APPLICATION D_UPLOAD_ERR_2));  // Not enough (memory) space
    return false;
  }
  return true;
}

uint16_t GetSettingsCrc(void)
{
  uint16_t crc = 0;
  uint8_t *bytes = (uint8_t*)&Settings;

  for (uint16_t i = 0; i < sizeof(SYSCFG); i++) {
    if ((i < 14) || (i > 15)) { crc += bytes[i]*(i+1); }  // Skip crc
  }
  return crc;
}

void SettingsSaveAll(void)
{
  if (Settings.flag.save_state) {
    Settings.power = power;
  } else {
    Settings.power = 0;
  }
  XsnsCall(FUNC_SAVE_BEFORE_RESTART);
  EepromCommit();
  SettingsSave(0);
}

/*********************************************************************************************\
 * Config Save - Save parameters to Flash ONLY if any parameter has changed
\*********************************************************************************************/

uint32_t GetSettingsAddress(void)
{
  return settings_location * SPI_FLASH_SEC_SIZE;
}

void SettingsSave(uint8_t rotate)
{
/* Save configuration in eeprom or one of 7 slots below
 *
 * rotate 0 = Save in next flash slot
 * rotate 1 = Save only in eeprom flash slot until SetOption12 0 or restart
 * rotate 2 = Save in eeprom flash slot, erase next flash slots and continue depending on stop_flash_rotate
 * stop_flash_rotate 0 = Allow flash slot rotation (SetOption12 0)
 * stop_flash_rotate 1 = Allow only eeprom flash slot use (SetOption12 1)
 */
#ifndef FIRMWARE_MINIMAL
  if ((GetSettingsCrc() != settings_crc) || rotate) {
    if (1 == rotate) {   // Use eeprom flash slot only and disable flash rotate from now on (upgrade)
      stop_flash_rotate = 1;
    }
    if (2 == rotate) {   // Use eeprom flash slot and erase next flash slots if stop_flash_rotate is off (default)
      settings_location = SETTINGS_LOCATION +1;
    }
    if (stop_flash_rotate) {
      settings_location = SETTINGS_LOCATION;
    } else {
      settings_location--;
      if (settings_location <= (SETTINGS_LOCATION - CFG_ROTATES)) {
        settings_location = SETTINGS_LOCATION;
      }
    }
    Settings.save_flag++;
    Settings.cfg_size = sizeof(SYSCFG);
    Settings.cfg_crc = GetSettingsCrc();

    if (SPIFFS_END == settings_location) {
      uint8_t* flash_buffer;
      flash_buffer = new uint8_t[SPI_FLASH_SEC_SIZE];
      if (eeprom_data && eeprom_size) {
        size_t flash_offset = SPI_FLASH_SEC_SIZE - eeprom_size;
        memcpy(flash_buffer + flash_offset, eeprom_data, eeprom_size);  // Write dirty EEPROM data
      } else {
        ESP.flashRead(settings_location * SPI_FLASH_SEC_SIZE, (uint32*)flash_buffer, SPI_FLASH_SEC_SIZE);   // Read EEPROM area
      }
      memcpy(flash_buffer, &Settings, sizeof(Settings));
      ESP.flashEraseSector(settings_location);
      ESP.flashWrite(settings_location * SPI_FLASH_SEC_SIZE, (uint32*)flash_buffer, SPI_FLASH_SEC_SIZE);
      delete[] flash_buffer;
    } else {
      ESP.flashEraseSector(settings_location);
      ESP.flashWrite(settings_location * SPI_FLASH_SEC_SIZE, (uint32*)&Settings, sizeof(SYSCFG));
    }

    if (!stop_flash_rotate && rotate) {
      for (uint8_t i = 1; i < CFG_ROTATES; i++) {
        ESP.flashEraseSector(settings_location -i);  // Delete previous configurations by resetting to 0xFF
        delay(1);
      }
    }

    AddLog_P2(LOG_LEVEL_DEBUG, PSTR(D_LOG_CONFIG D_SAVED_TO_FLASH_AT " %X, " D_COUNT " %d, " D_BYTES " %d"), settings_location, Settings.save_flag, sizeof(SYSCFG));

    settings_crc = Settings.cfg_crc;
  }
#endif  // FIRMWARE_MINIMAL
  RtcSettingsSave();
}

void SettingsLoad(void)
{
  // Load configuration from eeprom or one of 7 slots below if first valid load does not stop_flash_rotate
  struct SYSCFGH {
    uint16_t cfg_holder;                     // 000
    uint16_t cfg_size;                       // 002
    unsigned long save_flag;                 // 004
  } _SettingsH;
  unsigned long save_flag = 0;

  settings_location = 0;
  uint32_t flash_location = SETTINGS_LOCATION +1;
  for (uint8_t i = 0; i < CFG_ROTATES; i++) {
    flash_location--;
    ESP.flashRead(flash_location * SPI_FLASH_SEC_SIZE, (uint32*)&Settings, sizeof(SYSCFG));

    bool valid = false;
    if (Settings.version > 0x06000000) {
      valid = (Settings.cfg_crc == GetSettingsCrc());
    } else {
      ESP.flashRead((flash_location -1) * SPI_FLASH_SEC_SIZE, (uint32*)&_SettingsH, sizeof(SYSCFGH));
      valid = (Settings.cfg_holder == _SettingsH.cfg_holder);
    }
    if (valid) {
      if (Settings.save_flag > save_flag) {
        save_flag = Settings.save_flag;
        settings_location = flash_location;
        if (Settings.flag.stop_flash_rotate && (0 == i)) {  // Stop only if eeprom area should be used and it is valid
          break;
        }
      }
    }

    delay(1);
  }
  if (settings_location > 0) {
    ESP.flashRead(settings_location * SPI_FLASH_SEC_SIZE, (uint32*)&Settings, sizeof(SYSCFG));
    AddLog_P2(LOG_LEVEL_DEBUG, PSTR(D_LOG_CONFIG D_LOADED_FROM_FLASH_AT " %X, " D_COUNT " %d"), settings_location, Settings.save_flag);
  }

#ifndef FIRMWARE_MINIMAL
  if (!settings_location || (Settings.cfg_holder != (uint16_t)CFG_HOLDER)) {  // Init defaults if cfg_holder differs from user settings in my_user_config.h
    SettingsDefault();
  }
  settings_crc = GetSettingsCrc();
#endif  // FIRMWARE_MINIMAL

  RtcSettingsLoad();
}

void SettingsErase(uint8_t type)
{
  /*
    0 = Erase from program end until end of physical flash
    1 = Erase SDK parameter area at end of linker memory model (0x0FDxxx - 0x0FFFFF) solving possible wifi errors
  */

#ifndef FIRMWARE_MINIMAL
  bool result;

  uint32_t _sectorStart = (ESP.getSketchSize() / SPI_FLASH_SEC_SIZE) + 1;
  uint32_t _sectorEnd = ESP.getFlashChipRealSize() / SPI_FLASH_SEC_SIZE;
  if (1 == type) {
    _sectorStart = SETTINGS_LOCATION +2;  // SDK parameter area above EEPROM area (0x0FDxxx - 0x0FFFFF)
    _sectorEnd = SETTINGS_LOCATION +5;
  }

  bool _serialoutput = (LOG_LEVEL_DEBUG_MORE <= seriallog_level);

  AddLog_P2(LOG_LEVEL_DEBUG, PSTR(D_LOG_APPLICATION D_ERASE " %d " D_UNIT_SECTORS), _sectorEnd - _sectorStart);

  for (uint32_t _sector = _sectorStart; _sector < _sectorEnd; _sector++) {
    result = ESP.flashEraseSector(_sector);
    if (_serialoutput) {
      Serial.print(F(D_LOG_APPLICATION D_ERASED_SECTOR " "));
      Serial.print(_sector);
      if (result) {
        Serial.println(F(" " D_OK));
      } else {
        Serial.println(F(" " D_ERROR));
      }
      delay(10);
    }
    OsWatchLoop();
  }
#endif  // FIRMWARE_MINIMAL
}

// Copied from 2.4.0 as 2.3.0 is incomplete
bool SettingsEraseConfig(void) {
  const size_t cfgSize = 0x4000;
  size_t cfgAddr = ESP.getFlashChipSize() - cfgSize;

  for (size_t offset = 0; offset < cfgSize; offset += SPI_FLASH_SEC_SIZE) {
    if (!ESP.flashEraseSector((cfgAddr + offset) / SPI_FLASH_SEC_SIZE)) {
      return false;
    }
  }
  return true;
}

void SettingsSdkErase(void)
{
  WiFi.disconnect(true);    // Delete SDK wifi config
  SettingsErase(1);
  SettingsEraseConfig();
  delay(1000);
}

/********************************************************************************************/

void SettingsDefault(void)
{
  AddLog_P(LOG_LEVEL_NONE, PSTR(D_LOG_CONFIG D_USE_DEFAULTS));
  SettingsDefaultSet1();
  SettingsDefaultSet2();
  SettingsSave(2);
}

void SettingsDefaultSet1(void)
{
  memset(&Settings, 0x00, sizeof(SYSCFG));

  Settings.cfg_holder = (uint16_t)CFG_HOLDER;
  Settings.cfg_size = sizeof(SYSCFG);
//  Settings.save_flag = 0;
  Settings.version = VERSION;
//  Settings.bootcount = 0;
//  Settings.cfg_crc = 0;
}

void SettingsDefaultSet2(void)
{
  memset((char*)&Settings +16, 0x00, sizeof(SYSCFG) -16);

//  Settings.flag.value_units = 0;
//  Settings.flag.stop_flash_rotate = 0;
  Settings.save_data = SAVE_DATA;
  Settings.param[P_BOOT_LOOP_OFFSET] = BOOT_LOOP_OFFSET;
  Settings.param[P_RGB_REMAP] = RGB_REMAP_RGBW;
  Settings.sleep = APP_SLEEP;
  if (Settings.sleep < 50) {
    Settings.sleep = 50;                // Default to 50 for sleep, for now
  }

  // Module
//  Settings.flag.interlock = 0;
  Settings.interlock[0] = 0xFF;         // Legacy support using all relays in one interlock group
  Settings.module = MODULE;
  ModuleDefault(WEMOS);
//  for (uint8_t i = 0; i < sizeof(Settings.my_gp); i++) { Settings.my_gp.io[i] = GPIO_NONE; }
  strlcpy(Settings.friendlyname[0], FRIENDLY_NAME, sizeof(Settings.friendlyname[0]));
  strlcpy(Settings.friendlyname[1], FRIENDLY_NAME"2", sizeof(Settings.friendlyname[1]));
  strlcpy(Settings.friendlyname[2], FRIENDLY_NAME"3", sizeof(Settings.friendlyname[2]));
  strlcpy(Settings.friendlyname[3], FRIENDLY_NAME"4", sizeof(Settings.friendlyname[3]));
  strlcpy(Settings.ota_url, OTA_URL, sizeof(Settings.ota_url));

  // Power
  Settings.flag.save_state = SAVE_STATE;
  Settings.power = APP_POWER;
  Settings.poweronstate = APP_POWERON_STATE;
  Settings.blinktime = APP_BLINKTIME;
  Settings.blinkcount = APP_BLINKCOUNT;
  Settings.ledstate = APP_LEDSTATE;
  Settings.pulse_timer[0] = APP_PULSETIME;
//  for (uint8_t i = 1; i < MAX_PULSETIMERS; i++) { Settings.pulse_timer[i] = 0; }

  // Serial
  Settings.baudrate = APP_BAUDRATE / 1200;
  Settings.sbaudrate = SOFT_BAUDRATE / 1200;
  Settings.serial_delimiter = 0xff;
  Settings.seriallog_level = SERIAL_LOG_LEVEL;

  // Wifi
  ParseIp(&Settings.ip_address[0], WIFI_IP_ADDRESS);
  ParseIp(&Settings.ip_address[1], WIFI_GATEWAY);
  ParseIp(&Settings.ip_address[2], WIFI_SUBNETMASK);
  ParseIp(&Settings.ip_address[3], WIFI_DNS);
  Settings.sta_config = WIFI_CONFIG_TOOL;
//  Settings.sta_active = 0;
  strlcpy(Settings.sta_ssid[0], STA_SSID1, sizeof(Settings.sta_ssid[0]));
  strlcpy(Settings.sta_pwd[0], STA_PASS1, sizeof(Settings.sta_pwd[0]));
  strlcpy(Settings.sta_ssid[1], STA_SSID2, sizeof(Settings.sta_ssid[1]));
  strlcpy(Settings.sta_pwd[1], STA_PASS2, sizeof(Settings.sta_pwd[1]));
  strlcpy(Settings.hostname, WIFI_HOSTNAME, sizeof(Settings.hostname));

  // Syslog
  strlcpy(Settings.syslog_host, SYS_LOG_HOST, sizeof(Settings.syslog_host));
  Settings.syslog_port = SYS_LOG_PORT;
  Settings.syslog_level = SYS_LOG_LEVEL;

  // Webserver
  Settings.flag2.emulation = EMULATION;
  Settings.webserver = WEB_SERVER;
  Settings.weblog_level = WEB_LOG_LEVEL;
  strlcpy(Settings.web_password, WEB_PASSWORD, sizeof(Settings.web_password));
  Settings.flag3.mdns_enabled = MDNS_ENABLED;

  // Button
//  Settings.flag.button_restrict = 0;
//  Settings.flag.button_swap = 0;
//  Settings.flag.button_single = 0;
  Settings.param[P_HOLD_TIME] = KEY_HOLD_TIME;  // Default 4 seconds hold time

  // Switch
  for (uint8_t i = 0; i < MAX_SWITCHES; i++) { Settings.switchmode[i] = SWITCH_MODE; }

  // MQTT
  Settings.flag.mqtt_enabled = MQTT_USE;
//  Settings.flag.mqtt_response = 0;
  Settings.flag.mqtt_power_retain = MQTT_POWER_RETAIN;
  Settings.flag.mqtt_button_retain = MQTT_BUTTON_RETAIN;
  Settings.flag.mqtt_switch_retain = MQTT_SWITCH_RETAIN;
  Settings.flag3.button_switch_force_local = MQTT_BUTTON_SWITCH_FORCE_LOCAL;
  Settings.flag3.hass_tele_on_power = TELE_ON_POWER;
//  Settings.flag.mqtt_sensor_retain = 0;
//  Settings.flag.mqtt_offline = 0;
//  Settings.flag.mqtt_serial = 0;
//  Settings.flag.device_index_enable = 0;
  strlcpy(Settings.mqtt_host, MQTT_HOST, sizeof(Settings.mqtt_host));
  Settings.mqtt_port = MQTT_PORT;
  strlcpy(Settings.mqtt_client, MQTT_CLIENT_ID, sizeof(Settings.mqtt_client));
  strlcpy(Settings.mqtt_user, MQTT_USER, sizeof(Settings.mqtt_user));
  strlcpy(Settings.mqtt_pwd, MQTT_PASS, sizeof(Settings.mqtt_pwd));
  strlcpy(Settings.mqtt_topic, MQTT_TOPIC, sizeof(Settings.mqtt_topic));
  strlcpy(Settings.button_topic, MQTT_BUTTON_TOPIC, sizeof(Settings.button_topic));
  strlcpy(Settings.switch_topic, MQTT_SWITCH_TOPIC, sizeof(Settings.switch_topic));
  strlcpy(Settings.mqtt_grptopic, MQTT_GRPTOPIC, sizeof(Settings.mqtt_grptopic));
  strlcpy(Settings.mqtt_fulltopic, MQTT_FULLTOPIC, sizeof(Settings.mqtt_fulltopic));
  Settings.mqtt_retry = MQTT_RETRY_SECS;
  strlcpy(Settings.mqtt_prefix[0], SUB_PREFIX, sizeof(Settings.mqtt_prefix[0]));
  strlcpy(Settings.mqtt_prefix[1], PUB_PREFIX, sizeof(Settings.mqtt_prefix[1]));
  strlcpy(Settings.mqtt_prefix[2], PUB_PREFIX2, sizeof(Settings.mqtt_prefix[2]));
  strlcpy(Settings.state_text[0], MQTT_STATUS_OFF, sizeof(Settings.state_text[0]));
  strlcpy(Settings.state_text[1], MQTT_STATUS_ON, sizeof(Settings.state_text[1]));
  strlcpy(Settings.state_text[2], MQTT_CMND_TOGGLE, sizeof(Settings.state_text[2]));
  strlcpy(Settings.state_text[3], MQTT_CMND_HOLD, sizeof(Settings.state_text[3]));
  char fingerprint[60];
  strlcpy(fingerprint, MQTT_FINGERPRINT1, sizeof(fingerprint));
  char *p = fingerprint;
  for (uint8_t i = 0; i < 20; i++) {
    Settings.mqtt_fingerprint[0][i] = strtol(p, &p, 16);
  }
  strlcpy(fingerprint, MQTT_FINGERPRINT2, sizeof(fingerprint));
  p = fingerprint;
  for (uint8_t i = 0; i < 20; i++) {
    Settings.mqtt_fingerprint[1][i] = strtol(p, &p, 16);
  }
  Settings.tele_period = TELE_PERIOD;

  // Energy
  Settings.flag2.current_resolution = 3;
//  Settings.flag2.voltage_resolution = 0;
//  Settings.flag2.wattage_resolution = 0;
  Settings.flag2.energy_resolution = ENERGY_RESOLUTION;
  Settings.param[P_MAX_POWER_RETRY] = MAX_POWER_RETRY;
  Settings.energy_power_delta = DEFAULT_POWER_DELTA;
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

  // RF Bridge
//  for (uint8_t i = 0; i < 17; i++) { Settings.rf_code[i][0] = 0; }
  memcpy_P(Settings.rf_code[0], kDefaultRfCode, 9);

  // Domoticz
  Settings.domoticz_update_timer = DOMOTICZ_UPDATE_TIMER;
//  for (uint8_t i = 0; i < MAX_DOMOTICZ_IDX; i++) {
//    Settings.domoticz_relay_idx[i] = 0;
//    Settings.domoticz_key_idx[i] = 0;
//    Settings.domoticz_switch_idx[i] = 0;
//  }
//  for (uint8_t i = 0; i < MAX_DOMOTICZ_SNS_IDX; i++) {
//    Settings.domoticz_sensor_idx[i] = 0;
//  }

  // Sensor
  Settings.flag.temperature_conversion = TEMP_CONVERSION;
  Settings.flag.pressure_conversion = PRESSURE_CONVERSION;
  Settings.flag2.pressure_resolution = PRESSURE_RESOLUTION;
  Settings.flag2.humidity_resolution = HUMIDITY_RESOLUTION;
  Settings.flag2.temperature_resolution = TEMP_RESOLUTION;
//  Settings.altitude = 0;

  // Rules
//  Settings.rule_enabled = 0;
//  Settings.rule_once = 0;
//  for (uint8_t i = 1; i < MAX_RULE_SETS; i++) { Settings.rules[i][0] = '\0'; }
  Settings.flag2.calc_resolution = CALC_RESOLUTION;

  // Home Assistant
  Settings.flag.hass_discovery = HOME_ASSISTANT_DISCOVERY_ENABLE;

  // Knx
//  Settings.flag.knx_enabled = 0;
//  Settings.flag.knx_enable_enhancement = 0;

  // Light
  Settings.flag.pwm_control = 1;
  //Settings.flag.ws_clock_reverse = 0;
  //Settings.flag.light_signal = 0;
  //Settings.flag.not_power_linked = 0;
  //Settings.flag.decimal_text = 0;
  Settings.pwm_frequency = PWM_FREQ;
  Settings.pwm_range = PWM_RANGE;
  for (uint8_t i = 0; i < MAX_PWMS; i++) {
    Settings.light_color[i] = 255;
//    Settings.pwm_value[i] = 0;
  }
//  Settings.light_correction = 0;
  Settings.light_dimmer = 10;
//  Settings.light_fade = 0;
  Settings.light_speed = 1;
//  Settings.light_scheme = 0;
  Settings.light_width = 1;
//  Settings.light_wakeup = 0;
  Settings.light_pixels = WS2812_LEDS;
//  Settings.light_rotation = 0;
  SettingsDefaultSet_5_8_1();    // Clock color

  // Display
  SettingsDefaultSet_5_10_1();   // Display settings

  // Time
  if (((APP_TIMEZONE > -14) && (APP_TIMEZONE < 15)) || (99 == APP_TIMEZONE)) {
    Settings.timezone = APP_TIMEZONE;
    Settings.timezone_minutes = 0;
  } else {
    Settings.timezone = APP_TIMEZONE / 60;
    Settings.timezone_minutes = abs(APP_TIMEZONE % 60);
  }
  strlcpy(Settings.ntp_server[0], NTP_SERVER1, sizeof(Settings.ntp_server[0]));
  strlcpy(Settings.ntp_server[1], NTP_SERVER2, sizeof(Settings.ntp_server[1]));
  strlcpy(Settings.ntp_server[2], NTP_SERVER3, sizeof(Settings.ntp_server[2]));
  for (uint8_t j = 0; j < 3; j++) {
    for (uint8_t i = 0; i < strlen(Settings.ntp_server[j]); i++) {
      if (Settings.ntp_server[j][i] == ',') {
        Settings.ntp_server[j][i] = '.';
      }
    }
  }
  Settings.latitude = (int)((double)LATITUDE * 1000000);
  Settings.longitude = (int)((double)LONGITUDE * 1000000);
  SettingsDefaultSet_5_13_1c();  // Time STD/DST settings

  Settings.button_debounce = KEY_DEBOUNCE_TIME;
  Settings.switch_debounce = SWITCH_DEBOUNCE_TIME;

  for (uint8_t j = 0; j < 5; j++) {
    Settings.rgbwwTable[j] = 255;
  }

  memset(&Settings.drivers, 0xFF, 32);  // Enable all possible monitors, displays, drivers and sensors
}

/********************************************************************************************/

void SettingsDefaultSet_5_8_1(void)
{
//  Settings.flag.ws_clock_reverse = 0;
  Settings.ws_width[WS_SECOND] = 1;
  Settings.ws_color[WS_SECOND][WS_RED] = 255;
  Settings.ws_color[WS_SECOND][WS_GREEN] = 0;
  Settings.ws_color[WS_SECOND][WS_BLUE] = 255;
  Settings.ws_width[WS_MINUTE] = 3;
  Settings.ws_color[WS_MINUTE][WS_RED] = 0;
  Settings.ws_color[WS_MINUTE][WS_GREEN] = 255;
  Settings.ws_color[WS_MINUTE][WS_BLUE] = 0;
  Settings.ws_width[WS_HOUR] = 5;
  Settings.ws_color[WS_HOUR][WS_RED] = 255;
  Settings.ws_color[WS_HOUR][WS_GREEN] = 0;
  Settings.ws_color[WS_HOUR][WS_BLUE] = 0;
}

void SettingsDefaultSet_5_10_1(void)
{
  Settings.display_model = 0;
  Settings.display_mode = 1;
  Settings.display_refresh = 2;
  Settings.display_rows = 2;
  Settings.display_cols[0] = 16;
  Settings.display_cols[1] = 8;
  Settings.display_dimmer = 1;
  Settings.display_size = 1;
  Settings.display_font = 1;
  Settings.display_rotate = 0;
  Settings.display_address[0] = MTX_ADDRESS1;
  Settings.display_address[1] = MTX_ADDRESS2;
  Settings.display_address[2] = MTX_ADDRESS3;
  Settings.display_address[3] = MTX_ADDRESS4;
  Settings.display_address[4] = MTX_ADDRESS5;
  Settings.display_address[5] = MTX_ADDRESS6;
  Settings.display_address[6] = MTX_ADDRESS7;
  Settings.display_address[7] = MTX_ADDRESS8;
}

void SettingsResetStd(void)
{
  Settings.tflag[0].hemis = TIME_STD_HEMISPHERE;
  Settings.tflag[0].week = TIME_STD_WEEK;
  Settings.tflag[0].dow = TIME_STD_DAY;
  Settings.tflag[0].month = TIME_STD_MONTH;
  Settings.tflag[0].hour = TIME_STD_HOUR;
  Settings.toffset[0] = TIME_STD_OFFSET;
}

void SettingsResetDst(void)
{
  Settings.tflag[1].hemis = TIME_DST_HEMISPHERE;
  Settings.tflag[1].week = TIME_DST_WEEK;
  Settings.tflag[1].dow = TIME_DST_DAY;
  Settings.tflag[1].month = TIME_DST_MONTH;
  Settings.tflag[1].hour = TIME_DST_HOUR;
  Settings.toffset[1] = TIME_DST_OFFSET;
}

void SettingsDefaultSet_5_13_1c(void)
{
  SettingsResetStd();
  SettingsResetDst();
}

/********************************************************************************************/

void SettingsDelta(void)
{
  if (Settings.version != VERSION) {      // Fix version dependent changes

    if (Settings.version < 0x05050000) {
      for (uint8_t i = 0; i < 17; i++) { Settings.rf_code[i][0] = 0; }
      memcpy_P(Settings.rf_code[0], kDefaultRfCode, 9);
    }
    if (Settings.version < 0x05080000) {
      Settings.light_pixels = WS2812_LEDS;
      Settings.light_width = 1;
      Settings.light_color[0] = 255;
      Settings.light_color[1] = 0;
      Settings.light_color[2] = 0;
      Settings.light_dimmer = 10;
      Settings.light_correction = 0;
      Settings.light_fade = 0;
      Settings.light_speed = 1;
      Settings.light_scheme = 0;
      Settings.light_width = 1;
      Settings.light_wakeup = 0;
    }
    if (Settings.version < 0x0508000A) {
      Settings.power = 0;
      Settings.altitude = 0;
    }
    if (Settings.version < 0x0508000B) {
      for (uint8_t i = 0; i < sizeof(Settings.my_gp); i++) {  // Move GPIO_LEDs
        if ((Settings.my_gp.io[i] >= 25) && (Settings.my_gp.io[i] <= 32)) {  // Was GPIO_LED1
          Settings.my_gp.io[i] += 23;  // Move GPIO_LED1
        }
      }
      for (uint8_t i = 0; i < MAX_PWMS; i++) {      // Move pwm_value and reset additional pulse_timerrs
        Settings.pwm_value[i] = Settings.pulse_timer[4 +i];
        Settings.pulse_timer[4 +i] = 0;
      }
    }
    if (Settings.version < 0x0508000D) {
      Settings.pwm_frequency = PWM_FREQ;
      Settings.pwm_range = PWM_RANGE;
    }
    if (Settings.version < 0x0508000E) {
      SettingsDefaultSet_5_8_1();
    }
    if (Settings.version < 0x05090102) {
      Settings.flag2.data = Settings.flag.data;
      Settings.flag2.data &= 0xFFE80000;
      Settings.flag2.voltage_resolution = Settings.flag.not_power_linked;
      Settings.flag2.current_resolution = 3;
      Settings.ina219_mode = 0;
    }
    if (Settings.version < 0x050A0009) {
      SettingsDefaultSet_5_10_1();
    }
    if (Settings.version < 0x050B0107) {
      Settings.flag.not_power_linked = 0;
    }
    if (Settings.version < 0x050C0005) {
      Settings.light_rotation = 0;
      Settings.energy_power_delta = DEFAULT_POWER_DELTA;
      char fingerprint[60];
      memcpy(fingerprint, Settings.mqtt_fingerprint, sizeof(fingerprint));
      char *p = fingerprint;
      for (uint8_t i = 0; i < 20; i++) {
        Settings.mqtt_fingerprint[0][i] = strtol(p, &p, 16);
        Settings.mqtt_fingerprint[1][i] = Settings.mqtt_fingerprint[0][i];
      }
    }
    if (Settings.version < 0x050C0007) {
      Settings.baudrate = APP_BAUDRATE / 1200;
    }
    if (Settings.version < 0x050C0008) {
      Settings.sbaudrate = SOFT_BAUDRATE / 1200;
      Settings.serial_delimiter = 0xff;
    }
    if (Settings.version < 0x050C000A) {
      Settings.latitude = (int)((double)LATITUDE * 1000000);
      Settings.longitude = (int)((double)LONGITUDE * 1000000);
    }
    if (Settings.version < 0x050C000B) {
      Settings.rules[0][0] = '\0';
    }
    if (Settings.version < 0x050C000D) {
      memmove(Settings.rules, Settings.rules -256, sizeof(Settings.rules));  // move rules up by 256 bytes
      memset(&Settings.timer, 0x00, sizeof(Timer) * MAX_TIMERS);  // Reset timers as layout has changed from v5.12.0i
      Settings.knx_GA_registered = 0;
      Settings.knx_CB_registered = 0;
      memset(&Settings.knx_physsical_addr, 0x00, 0x800 - 0x6b8);  // Reset until 0x800 for future use
    }
    if (Settings.version < 0x050C000F) {
      Settings.energy_kWhtoday /= 1000;
      Settings.energy_kWhyesterday /= 1000;
      RtcSettings.energy_kWhtoday /= 1000;
    }
    if (Settings.version < 0x050D0103) {
      SettingsDefaultSet_5_13_1c();
    }
    if (Settings.version < 0x050E0002) {
      for (uint8_t i = 1; i < MAX_RULE_SETS; i++) { Settings.rules[i][0] = '\0'; }
      Settings.rule_enabled = Settings.flag.mqtt_serial_raw;   // Was rules_enabled until 5.14.0b
      Settings.rule_once = Settings.flag.pressure_conversion;  // Was rules_once until 5.14.0b
    }
    if (Settings.version < 0x06000000) {
      Settings.cfg_size = sizeof(SYSCFG);
      Settings.cfg_crc = GetSettingsCrc();
    }
    if (Settings.version < 0x06000002) {
      for (uint8_t i = 0; i < MAX_SWITCHES; i++) {
        if (i < 4) {
          Settings.switchmode[i] = Settings.interlock[i];
        } else {
          Settings.switchmode[i] = SWITCH_MODE;
        }
      }
      for (uint8_t i = 0; i < sizeof(Settings.my_gp); i++) {
        if (Settings.my_gp.io[i] >= GPIO_SWT5) {  // Move up from GPIO_SWT5 to GPIO_KEY1
          Settings.my_gp.io[i] += 4;
        }
      }
    }
    if (Settings.version < 0x06000003) {
      Settings.flag.mqtt_serial_raw = 0;      // Was rules_enabled until 5.14.0b
      Settings.flag.pressure_conversion = 0;  // Was rules_once until 5.14.0b
      Settings.flag3.data = 0;
    }
    if (Settings.version < 0x06010103) {
      Settings.flag3.timers_enable = 1;
    }
    if (Settings.version < 0x0601010C) {
      Settings.button_debounce = KEY_DEBOUNCE_TIME;
      Settings.switch_debounce = SWITCH_DEBOUNCE_TIME;
    }
    if (Settings.version < 0x0602010A) {
      for (uint8_t j = 0; j < 5; j++) {
        Settings.rgbwwTable[j] = 255;
      }
    }
    if (Settings.version < 0x06030002) {
      Settings.timezone_minutes = 0;
    }
    if (Settings.version < 0x06030004) {
      memset(&Settings.drivers, 0xFF, 32);  // Enable all possible monitors, displays, drivers and sensors
    }
    if (Settings.version < 0x0603000E) {
      Settings.flag2.calc_resolution = CALC_RESOLUTION;
    }
    if (Settings.version < 0x0603000F) {
      if (Settings.sleep < 50) {
        Settings.sleep = 50;                // Default to 50 for sleep, for now
      }
    }
    if (Settings.version < 0x06040105) {
      Settings.flag3.mdns_enabled = MDNS_ENABLED;
      Settings.param[P_MDNS_DELAYED_START] = 0;
    }
    if (Settings.version < 0x0604010B) {
      Settings.interlock[0] = 0xFF;         // Legacy support using all relays in one interlock group
      for (uint8_t i = 1; i < MAX_INTERLOCKS; i++) { Settings.interlock[i] = 0; }
    }
    if (Settings.version < 0x0604010D) {
      Settings.param[P_BOOT_LOOP_OFFSET] = BOOT_LOOP_OFFSET;
    }
    if (Settings.version < 0x06040110) {
      ModuleDefault(WEMOS);
    }
    if (Settings.version < 0x06040113) {
      Settings.param[P_RGB_REMAP] = RGB_REMAP_RGBW;
    }

    Settings.version = VERSION;
    SettingsSave(1);
  }
}
