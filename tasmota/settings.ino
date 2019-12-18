/*
  settings.ino - user settings for Tasmota

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
#define DOMOTICZ_UPDATE_TIMER       0          // [DomoticzUpdateTimer] Send relay status (0 = disable, 1 - 3600 seconds) (Optional)
#endif

#ifndef EMULATION
#define EMULATION                   EMUL_NONE  // [Emulation] Select Belkin WeMo (single relay/light) or Hue Bridge emulation (multi relay/light) (EMUL_NONE, EMUL_WEMO or EMUL_HUE)
#endif

#ifndef MTX_ADDRESS1                           // Add Display Support for up to eigth Matrices
#define MTX_ADDRESS1                0
#endif
#ifndef MTX_ADDRESS2
#define MTX_ADDRESS2                0
#endif
#ifndef MTX_ADDRESS3
#define MTX_ADDRESS3                0
#endif
#ifndef MTX_ADDRESS4
#define MTX_ADDRESS4                0
#endif
#ifndef MTX_ADDRESS5
#define MTX_ADDRESS5                0
#endif
#ifndef MTX_ADDRESS6
#define MTX_ADDRESS6                0
#endif
#ifndef MTX_ADDRESS7
#define MTX_ADDRESS7                0
#endif
#ifndef MTX_ADDRESS8
#define MTX_ADDRESS8                0
#endif

#ifndef HOME_ASSISTANT_DISCOVERY_ENABLE
#define HOME_ASSISTANT_DISCOVERY_ENABLE 0
#endif

#ifndef LATITUDE
#define LATITUDE                    48.858360  // [Latitude] Your location to be used with sunrise and sunset
#endif
#ifndef LONGITUDE
#define LONGITUDE                   2.294442   // [Longitude] Your location to be used with sunrise and sunset
#endif

#ifndef WORKING_PERIOD
#define WORKING_PERIOD              5          // Working period of the SDS Sensor, Takes a reading every X Minutes
#endif

#ifndef COLOR_TEXT
#define COLOR_TEXT                  "#000"     // Global text color - Black
#endif
#ifndef COLOR_BACKGROUND
#define COLOR_BACKGROUND            "#fff"     // Global background color - White
#endif
#ifndef COLOR_FORM
#define COLOR_FORM                  "#f2f2f2"  // Form background color - Greyish
#endif
#ifndef COLOR_INPUT_TEXT
#define COLOR_INPUT_TEXT            "#000"     // Input text color - Black
#endif
#ifndef COLOR_INPUT
#define COLOR_INPUT                 "#fff"     // Input background color - White
#endif
#ifndef COLOR_CONSOLE_TEXT
#define COLOR_CONSOLE_TEXT          "#000"     // Console text color - Black
#endif
#ifndef COLOR_CONSOLE
#define COLOR_CONSOLE               "#fff"     // Console background color - White
#endif
#ifndef COLOR_TEXT_WARNING
#define COLOR_TEXT_WARNING          "#f00"     // Warning text color - Red
#endif
#ifndef COLOR_TEXT_SUCCESS
#define COLOR_TEXT_SUCCESS          "#008000"  // Success text color - Green
#endif
#ifndef COLOR_BUTTON_TEXT
#define COLOR_BUTTON_TEXT           "#fff"     // Button text color - White
#endif
#ifndef COLOR_BUTTON
#define COLOR_BUTTON                "#1fa3ec"  // Button color - Blueish
#endif
#ifndef COLOR_BUTTON_HOVER
#define COLOR_BUTTON_HOVER          "#0e70a4"  // Button color when hovered over - Darker blueish
#endif
#ifndef COLOR_BUTTON_RESET
#define COLOR_BUTTON_RESET          "#d43535"  // Restart/Reset/Delete button color - Redish
#endif
#ifndef COLOR_BUTTON_RESET_HOVER
#define COLOR_BUTTON_RESET_HOVER    "#931f1f"  // Restart/Reset/Delete button color when hovered over - Darker redish
#endif
#ifndef COLOR_BUTTON_SAVE
#define COLOR_BUTTON_SAVE           "#47c266"  // Save button color - Greenish
#endif
#ifndef COLOR_BUTTON_SAVE_HOVER
#define COLOR_BUTTON_SAVE_HOVER     "#5aaf6f"  // Save button color when hovered over - Darker greenish
#endif
#ifndef COLOR_TIMER_TAB_TEXT
#define COLOR_TIMER_TAB_TEXT        "#fff"     // Config timer tab text color - White
#endif
#ifndef COLOR_TIMER_TAB_BACKGROUND
#define COLOR_TIMER_TAB_BACKGROUND  "#999"     // Config timer tab background color - Light grey
#endif
#ifndef COLOR_TITLE_TEXT
#define COLOR_TITLE_TEXT			      COLOR_TEXT // Title text color defaults to global text color either dark or light
#endif
#ifndef IR_RCV_MIN_UNKNOWN_SIZE
#define IR_RCV_MIN_UNKNOWN_SIZE     6          // Set the smallest sized "UNKNOWN" message packets we actually care about (default 6, max 255)
#endif
#ifndef ENERGY_OVERTEMP
#define ENERGY_OVERTEMP             90         // Overtemp in Celsius
#endif
#ifndef DEFAULT_DIMMER_MAX
#define DEFAULT_DIMMER_MAX          100
#endif
#ifndef DEFAULT_DIMMER_MIN
#define DEFAULT_DIMMER_MIN          0
#endif
#ifndef DEFAULT_LIGHT_DIMMER
#define DEFAULT_LIGHT_DIMMER        10
#endif
#ifndef DEFAULT_LIGHT_COMPONENT
#define DEFAULT_LIGHT_COMPONENT     255
#endif
#ifndef CORS_ENABLED_ALL
#define CORS_ENABLED_ALL            "*"
#endif


enum WebColors {
  COL_TEXT, COL_BACKGROUND, COL_FORM,
  COL_INPUT_TEXT, COL_INPUT, COL_CONSOLE_TEXT, COL_CONSOLE,
  COL_TEXT_WARNING, COL_TEXT_SUCCESS,
  COL_BUTTON_TEXT, COL_BUTTON, COL_BUTTON_HOVER, COL_BUTTON_RESET, COL_BUTTON_RESET_HOVER, COL_BUTTON_SAVE, COL_BUTTON_SAVE_HOVER,
  COL_TIMER_TAB_TEXT, COL_TIMER_TAB_BACKGROUND, COL_TITLE,
  COL_LAST };

const char kWebColors[] PROGMEM =
  COLOR_TEXT "|" COLOR_BACKGROUND "|" COLOR_FORM "|"
  COLOR_INPUT_TEXT "|" COLOR_INPUT "|" COLOR_CONSOLE_TEXT "|" COLOR_CONSOLE "|"
  COLOR_TEXT_WARNING "|" COLOR_TEXT_SUCCESS "|"
  COLOR_BUTTON_TEXT "|" COLOR_BUTTON "|" COLOR_BUTTON_HOVER "|" COLOR_BUTTON_RESET "|" COLOR_BUTTON_RESET_HOVER "|" COLOR_BUTTON_SAVE "|" COLOR_BUTTON_SAVE_HOVER "|"
  COLOR_TIMER_TAB_TEXT "|" COLOR_TIMER_TAB_BACKGROUND "|" COLOR_TITLE_TEXT;

enum TasmotaSerialConfig {
  TS_SERIAL_5N1, TS_SERIAL_6N1, TS_SERIAL_7N1, TS_SERIAL_8N1,
  TS_SERIAL_5N2, TS_SERIAL_6N2, TS_SERIAL_7N2, TS_SERIAL_8N2,
  TS_SERIAL_5E1, TS_SERIAL_6E1, TS_SERIAL_7E1, TS_SERIAL_8E1,
  TS_SERIAL_5E2, TS_SERIAL_6E2, TS_SERIAL_7E2, TS_SERIAL_8E2,
  TS_SERIAL_5O1, TS_SERIAL_6O1, TS_SERIAL_7O1, TS_SERIAL_8O1,
  TS_SERIAL_5O2, TS_SERIAL_6O2, TS_SERIAL_7O2, TS_SERIAL_8O2 };

const uint8_t kTasmotaSerialConfig[] PROGMEM = {
  SERIAL_5N1, SERIAL_6N1, SERIAL_7N1, SERIAL_8N1,
  SERIAL_5N2, SERIAL_6N2, SERIAL_7N2, SERIAL_8N2,
  SERIAL_5E1, SERIAL_6E1, SERIAL_7E1, SERIAL_8E1,
  SERIAL_5E2, SERIAL_6E2, SERIAL_7E2, SERIAL_8E2,
  SERIAL_5O1, SERIAL_6O1, SERIAL_7O1, SERIAL_8O1,
  SERIAL_5O2, SERIAL_6O2, SERIAL_7O2, SERIAL_8O2
};

/*********************************************************************************************\
 * RTC memory
\*********************************************************************************************/

const uint16_t RTC_MEM_VALID = 0xA55A;

uint32_t rtc_settings_crc = 0;

uint32_t GetRtcSettingsCrc(void)
{
  uint32_t crc = 0;
  uint8_t *bytes = (uint8_t*)&RtcSettings;

  for (uint32_t i = 0; i < sizeof(RTCMEM); i++) {
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
    RtcSettings.energy_usage = Settings.energy_usage;
    for (uint32_t i = 0; i < MAX_COUNTERS; i++) {
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

  for (uint32_t i = 0; i < sizeof(RTCRBT); i++) {
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

void RtcRebootReset(void)
{
  RtcReboot.fast_reboot_count = 0;
  RtcRebootSave();
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

#if defined(ARDUINO_ESP8266_RELEASE_2_3_0) || defined(ARDUINO_ESP8266_RELEASE_2_4_0) || defined(ARDUINO_ESP8266_RELEASE_2_4_1) || defined(ARDUINO_ESP8266_RELEASE_2_4_2) || defined(ARDUINO_ESP8266_RELEASE_2_5_0) || defined(ARDUINO_ESP8266_RELEASE_2_5_1) || defined(ARDUINO_ESP8266_RELEASE_2_5_2)

extern "C" uint32_t _SPIFFS_end;
// From libraries/EEPROM/EEPROM.cpp EEPROMClass
const uint32_t SPIFFS_END = ((uint32_t)&_SPIFFS_end - 0x40200000) / SPI_FLASH_SEC_SIZE;

#else  // Core > 2.5.2 and STAGE

#if AUTOFLASHSIZE

#include "flash_hal.h"

// From libraries/EEPROM/EEPROM.cpp EEPROMClass
const uint32_t SPIFFS_END = (FS_end - 0x40200000) / SPI_FLASH_SEC_SIZE;

#else

extern "C" uint32_t _FS_end;
// From libraries/EEPROM/EEPROM.cpp EEPROMClass
const uint32_t SPIFFS_END = ((uint32_t)&_FS_end - 0x40200000) / SPI_FLASH_SEC_SIZE;

#endif  // AUTOFLASHSIZE

#endif  // All cores < pre-2.6.0

// Version 4.2 config = eeprom area
const uint32_t SETTINGS_LOCATION = SPIFFS_END;  // No need for SPIFFS as it uses EEPROM area
// Version 5.2 allow for more flash space
const uint8_t CFG_ROTATES = 8;          // Number of flash sectors used (handles uploads)

uint32_t settings_location = SETTINGS_LOCATION;
uint32_t settings_crc32 = 0;
uint8_t *settings_buffer = nullptr;

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
      if (ESP.flashEraseSector(address / FLASH_SECTOR_SIZE)) {
        ESP.flashWrite(address, (uint32_t*)_buffer, FLASH_SECTOR_SIZE);
      }
    }
  }
  delete[] _buffer;
}

void SettingsBufferFree(void)
{
  if (settings_buffer != nullptr) {
    free(settings_buffer);
    settings_buffer = nullptr;
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

uint16_t GetCfgCrc16(uint8_t *bytes, uint32_t size)
{
  uint16_t crc = 0;

  for (uint32_t i = 0; i < size; i++) {
    if ((i < 14) || (i > 15)) { crc += bytes[i]*(i+1); }  // Skip crc
  }
  return crc;
}

uint16_t GetSettingsCrc(void)
{
  // Fix miscalculation if previous Settings was 3584 and current Settings is 4096 between 0x06060007 and 0x0606000A
  uint32_t size = ((Settings.version < 0x06060007) || (Settings.version > 0x0606000A)) ? 3584 : sizeof(SYSCFG);
  return GetCfgCrc16((uint8_t*)&Settings, size);
}

uint32_t GetCfgCrc32(uint8_t *bytes, uint32_t size)
{
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

uint32_t GetSettingsCrc32(void)
{
  return GetCfgCrc32((uint8_t*)&Settings, sizeof(SYSCFG) -4);  // Skip crc32
}

void SettingsSaveAll(void)
{
  if (Settings.flag.save_state) {
    Settings.power = power;
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

void UpdateQuickPowerCycle(bool update)
{
  if (Settings.flag3.fast_power_cycle_disable) { return; }

  uint32_t pc_register;
  uint32_t pc_location = SETTINGS_LOCATION - CFG_ROTATES;

  ESP.flashRead(pc_location * SPI_FLASH_SEC_SIZE, (uint32*)&pc_register, sizeof(pc_register));
  if (update && ((pc_register & 0xFFFFFFF0) == 0xFFA55AB0)) {
    uint32_t counter = ((pc_register & 0xF) << 1) & 0xF;
    if (0 == counter) {  // 4 power cycles in a row
      SettingsErase(3);  // Quickly reset all settings including QuickPowerCycle flag
      EspRestart();      // And restart
    } else {
      pc_register = 0xFFA55AB0 | counter;
      ESP.flashWrite(pc_location * SPI_FLASH_SEC_SIZE, (uint32*)&pc_register, sizeof(pc_register));
      AddLog_P2(LOG_LEVEL_DEBUG, PSTR("QPC: Flag %02X"), counter);
    }
  }
  else if (pc_register != 0xFFA55ABF) {
    pc_register = 0xFFA55ABF;
    // Assume flash is default all ones and setting a bit to zero does not need an erase
    if (ESP.flashEraseSector(pc_location)) {
      ESP.flashWrite(pc_location * SPI_FLASH_SEC_SIZE, (uint32*)&pc_register, sizeof(pc_register));
    }
    AddLog_P2(LOG_LEVEL_DEBUG, PSTR("QPC: Reset"));
  }
}

/*********************************************************************************************\
 * Config Settings.text char array support
\*********************************************************************************************/

char aws_mqtt_host[66];
char aws_mqtt_user[1] { 0 };

const uint32_t settings_text_size = 457;  // Settings.flag4 (1E0) - Settings.ota_url (017)

uint32_t GetSettingsTextLen(void)
{
  char* position = Settings.ota_url;
  for (uint32_t size = 0; size < SET_MAX; size++) {
    while (*position++ != '\0') { }
  }
  return position - Settings.ota_url;
}

bool SettingsUpdateText(uint32_t index, const char* replace_me)
{
  if (index >= SET_MAX) {
    return false;  // Setting not supported - internal error
  }

  // Make a copy first in case we use source from Settings.text
  uint32_t replace_len = strlen(replace_me);
  char replace[replace_len +1];
  memcpy(replace, replace_me, sizeof(replace));

  if (Settings.version < 0x08000000) {
    uint32_t idx = 0;
    switch (index) {
      case SET_OTAURL:            strlcpy(Settings.ota_url, replace, sizeof(Settings.ota_url)); break;
      case SET_MQTTPREFIX3:       idx++;
      case SET_MQTTPREFIX2:       idx++;
      case SET_MQTTPREFIX1:       strlcpy(Settings.mqtt_prefix[idx], replace, sizeof(Settings.mqtt_prefix[idx])); break;
      case SET_STASSID2:          idx++;
      case SET_STASSID1:          strlcpy(Settings.sta_ssid[idx], replace, sizeof(Settings.sta_ssid[idx])); break;
      case SET_STAPWD2:           idx++;
      case SET_STAPWD1:           strlcpy(Settings.sta_pwd[idx], replace, sizeof(Settings.sta_pwd[idx])); break;
      case SET_HOSTNAME:          strlcpy(Settings.hostname, replace, sizeof(Settings.hostname)); break;
      case SET_SYSLOG_HOST:       strlcpy(Settings.syslog_host, replace, sizeof(Settings.syslog_host)); break;
      case SET_WEBPWD:            strlcpy(Settings.web_password, replace, sizeof(Settings.web_password)); break;
#if defined(USE_MQTT_TLS) && defined(USE_MQTT_AWS_IOT)
      case SET_MQTT_HOST:
        if (strlen(replace) <= sizeof(Settings.mqtt_host)) {
          strlcpy(Settings.mqtt_host, replace, sizeof(Settings.mqtt_host));
          Settings.mqtt_user[0] = 0;
        } else {
          // need to split in mqtt_user first then mqtt_host
          strlcpy(Settings.mqtt_user, replace, sizeof(Settings.mqtt_user));
          strlcpy(Settings.mqtt_host, &replace[sizeof(Settings.mqtt_user)-1], sizeof(Settings.mqtt_host));
        }
        break;
      case SET_MQTT_USER:         break;
#else
      case SET_MQTT_HOST:         strlcpy(Settings.mqtt_host, replace, sizeof(Settings.mqtt_host)); break;
      case SET_MQTT_USER:         strlcpy(Settings.mqtt_user, replace, sizeof(Settings.mqtt_user)); break;
#endif
      case SET_MQTT_CLIENT:       strlcpy(Settings.mqtt_client, replace, sizeof(Settings.mqtt_client)); break;
      case SET_MQTT_PWD:          strlcpy(Settings.mqtt_pwd, replace, sizeof(Settings.mqtt_pwd)); break;
      case SET_MQTT_FULLTOPIC:    strlcpy(Settings.mqtt_fulltopic, replace, sizeof(Settings.mqtt_fulltopic)); break;
      case SET_MQTT_TOPIC:        strlcpy(Settings.mqtt_topic, replace, sizeof(Settings.mqtt_topic)); break;
      case SET_MQTT_BUTTON_TOPIC: strlcpy(Settings.button_topic, replace, sizeof(Settings.button_topic)); break;
      case SET_MQTT_SWITCH_TOPIC: strlcpy(Settings.switch_topic, replace, sizeof(Settings.switch_topic)); break;
      case SET_MQTT_GRP_TOPIC:    strlcpy(Settings.mqtt_grptopic, replace, sizeof(Settings.mqtt_grptopic)); break;
      case SET_STATE_TXT4:        idx++;
      case SET_STATE_TXT3:        idx++;
      case SET_STATE_TXT2:        idx++;
      case SET_STATE_TXT1:        strlcpy(Settings.state_text[idx], replace, sizeof(Settings.state_text[idx])); break;
      case SET_NTPSERVER3:        idx++;
      case SET_NTPSERVER2:        idx++;
      case SET_NTPSERVER1:        strlcpy(Settings.ntp_server[idx], replace, sizeof(Settings.ntp_server[idx])); break;
      case SET_MEM5:              idx++;
      case SET_MEM4:              idx++;
      case SET_MEM3:              idx++;
      case SET_MEM2:              idx++;
      case SET_MEM1:              strlcpy(Settings.mems[idx], replace, sizeof(Settings.mems[idx])); break;
      case SET_CORS:              strlcpy(Settings.cors_domain, replace, sizeof(Settings.cors_domain)); break;
      case SET_FRIENDLYNAME4:     idx++;
      case SET_FRIENDLYNAME3:     idx++;
      case SET_FRIENDLYNAME2:     idx++;
      case SET_FRIENDLYNAME1:     strlcpy(Settings.friendlyname[idx], replace, sizeof(Settings.friendlyname[idx])); break;
    }
  } else {
    uint32_t start_pos = 0;
    uint32_t end_pos = 0;
    char* position = Settings.ota_url;
    for (uint32_t size = 0; size < SET_MAX; size++) {
      while (*position++ != '\0') { }
      if (1 == index) {
        start_pos = position - Settings.ota_url;
      }
      else if (0 == index) {
        end_pos = position - Settings.ota_url -1;
      }
      index--;
    }
    uint32_t char_len = position - Settings.ota_url;

    uint32_t current_len = end_pos - start_pos;
    int diff = replace_len - current_len;

  //  AddLog_P2(LOG_LEVEL_DEBUG, PSTR("TST: start %d, end %d, len %d, current %d, replace %d, diff %d"),
  //    start_pos, end_pos, char_len, current_len, replace_len, diff);

    int too_long = (char_len + diff) - settings_text_size;
    if (too_long > 0) {
  //    AddLog_P2(LOG_LEVEL_INFO, PSTR("CFG: Text too long by %d char(s)"), too_long);
      return false;  // Replace text too long
    }

    if (diff != 0) {
      // Shift Settings.text up or down
      memmove_P(Settings.ota_url + start_pos + replace_len, Settings.ota_url + end_pos, char_len - end_pos);
    }
    // Replace text
    memmove_P(Settings.ota_url + start_pos, replace, replace_len);
    // Fill for future use
    memset(Settings.ota_url + char_len + diff, 0x00, settings_text_size - char_len - diff);
  }

  return true;
}

char* SettingsText(uint32_t index)
{
  if (index >= SET_MAX) {
    return nullptr;  // Setting not supported - internal error
  }

  char* position = Settings.ota_url;

  if (Settings.version < 0x08000000) {
    uint32_t idx = 0;
    switch (index) {
      case SET_MQTTPREFIX3:       idx++;
      case SET_MQTTPREFIX2:       idx++;
      case SET_MQTTPREFIX1:       position = Settings.mqtt_prefix[idx]; break;
      case SET_STASSID2:          idx++;
      case SET_STASSID1:          position = Settings.sta_ssid[idx]; break;
      case SET_STAPWD2:           idx++;
      case SET_STAPWD1:           position = Settings.sta_pwd[idx]; break;
      case SET_HOSTNAME:          position = Settings.hostname; break;
      case SET_SYSLOG_HOST:       position = Settings.syslog_host; break;
      case SET_WEBPWD:            position = Settings.web_password; break;
#if defined(USE_MQTT_TLS) && defined(USE_MQTT_AWS_IOT)
      case SET_MQTT_HOST:
        strncpy(aws_mqtt_host, Settings.mqtt_user, strlen(Settings.mqtt_user));
        strncpy(&aws_mqtt_host[strlen(Settings.mqtt_user)], Settings.mqtt_host, strlen(Settings.mqtt_host));
        position = aws_mqtt_host; break;
      case SET_MQTT_USER:         position = aws_mqtt_user; break;
#else
      case SET_MQTT_HOST:         position = Settings.mqtt_host; break;
      case SET_MQTT_USER:         position = Settings.mqtt_user; break;
#endif
      case SET_MQTT_CLIENT:       position = Settings.mqtt_client; break;
      case SET_MQTT_PWD:          position = Settings.mqtt_pwd; break;
      case SET_MQTT_FULLTOPIC:    position = Settings.mqtt_fulltopic; break;
      case SET_MQTT_TOPIC:        position = Settings.mqtt_topic; break;
      case SET_MQTT_BUTTON_TOPIC: position = Settings.button_topic; break;
      case SET_MQTT_SWITCH_TOPIC: position = Settings.switch_topic; break;
      case SET_MQTT_GRP_TOPIC:    position = Settings.mqtt_grptopic; break;
      case SET_STATE_TXT4:        idx++;
      case SET_STATE_TXT3:        idx++;
      case SET_STATE_TXT2:        idx++;
      case SET_STATE_TXT1:        position = Settings.state_text[idx]; break;
      case SET_NTPSERVER3:        idx++;
      case SET_NTPSERVER2:        idx++;
      case SET_NTPSERVER1:        position = Settings.ntp_server[idx]; break;
      case SET_MEM5:              idx++;
      case SET_MEM4:              idx++;
      case SET_MEM3:              idx++;
      case SET_MEM2:              idx++;
      case SET_MEM1:              position = Settings.mems[idx]; break;
      case SET_CORS:              position = Settings.cors_domain; break;
      case SET_FRIENDLYNAME4:     idx++;
      case SET_FRIENDLYNAME3:     idx++;
      case SET_FRIENDLYNAME2:     idx++;
      case SET_FRIENDLYNAME1:     position = Settings.friendlyname[idx]; break;
    }

  } else {
    for (;index > 0; index--) {
      while (*position++ != '\0') { }
    }
  }

  return position;
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
  if ((GetSettingsCrc32() != settings_crc32) || rotate) {
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
    if (UtcTime() > START_VALID_TIME) {
      Settings.cfg_timestamp = UtcTime();
    } else {
      Settings.cfg_timestamp++;
    }
    Settings.cfg_size = sizeof(SYSCFG);
    Settings.cfg_crc = GetSettingsCrc();  // Keep for backward compatibility in case of fall-back just after upgrade
    Settings.cfg_crc32 = GetSettingsCrc32();

    if (ESP.flashEraseSector(settings_location)) {
      ESP.flashWrite(settings_location * SPI_FLASH_SEC_SIZE, (uint32*)&Settings, sizeof(SYSCFG));
    }

    if (!stop_flash_rotate && rotate) {
      for (uint32_t i = 1; i < CFG_ROTATES; i++) {
        ESP.flashEraseSector(settings_location -i);  // Delete previous configurations by resetting to 0xFF
        delay(1);
      }
    }

    AddLog_P2(LOG_LEVEL_DEBUG, PSTR(D_LOG_CONFIG D_SAVED_TO_FLASH_AT " %X, " D_COUNT " %d, " D_BYTES " %d"), settings_location, Settings.save_flag, sizeof(SYSCFG));

    settings_crc32 = Settings.cfg_crc32;
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
  uint16_t cfg_holder = 0;
  for (uint32_t i = 0; i < CFG_ROTATES; i++) {
    flash_location--;
    ESP.flashRead(flash_location * SPI_FLASH_SEC_SIZE, (uint32*)&Settings, sizeof(SYSCFG));

    bool valid = false;
    if (Settings.version > 0x06000000) {
      bool almost_valid = (Settings.cfg_crc32 == GetSettingsCrc32());
      if (Settings.version < 0x0606000B) {
        almost_valid = (Settings.cfg_crc == GetSettingsCrc());
      }
      // Sometimes CRC on pages below FB, overwritten by OTA, is fine but Settings are still invalid. So check cfg_holder too
      if (almost_valid && (0 == cfg_holder)) { cfg_holder = Settings.cfg_holder; }  // At FB always active cfg_holder
      valid = (cfg_holder == Settings.cfg_holder);
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
    AddLog_P2(LOG_LEVEL_NONE, PSTR(D_LOG_CONFIG D_LOADED_FROM_FLASH_AT " %X, " D_COUNT " %lu"), settings_location, Settings.save_flag);
  }

#ifndef FIRMWARE_MINIMAL
  if (!settings_location || (Settings.cfg_holder != (uint16_t)CFG_HOLDER)) {  // Init defaults if cfg_holder differs from user settings in my_user_config.h
    SettingsDefault();
  }
  settings_crc32 = GetSettingsCrc32();
#endif  // FIRMWARE_MINIMAL

  RtcSettingsLoad();
}

void EspErase(uint32_t start_sector, uint32_t end_sector)
{
  bool serial_output = (LOG_LEVEL_DEBUG_MORE <= seriallog_level);
  for (uint32_t sector = start_sector; sector < end_sector; sector++) {

    bool result = ESP.flashEraseSector(sector);  // Arduino core - erases flash as seen by SDK
//    bool result = !SPIEraseSector(sector);       // SDK - erases flash as seen by SDK
//    bool result = EsptoolEraseSector(sector);    // Esptool - erases flash completely (slow)

    if (serial_output) {
      Serial.printf_P(PSTR(D_LOG_APPLICATION D_ERASED_SECTOR " %d %s\n"), sector, (result) ? D_OK : D_ERROR);
      delay(10);
    } else {
      yield();
    }
    OsWatchLoop();
  }
}

void SettingsErase(uint8_t type)
{
  /*
    For Arduino core and SDK:
    Erase only works from flash start address to SDK recognized flash end address (flashchip->chip_size = ESP.getFlashChipSize).
    Addresses above SDK recognized size (up to ESP.getFlashChipRealSize) are not accessable.
    For Esptool:
    The only way to erase whole flash is esptool which uses direct SPI writes to flash.

    The default erase function is EspTool (EsptoolErase)

    0 = Erase from program end until end of flash as seen by SDK
    1 = Erase 16k SDK parameter area near end of flash as seen by SDK (0x0xFCxxx - 0x0xFFFFF) solving possible wifi errors
    2 = Erase Tasmota parameter area (0x0xF3xxx - 0x0xFBFFF)
    3 = Erase Tasmota and SDK parameter area (0x0F3xxx - 0x0FFFFF)
  */

#ifndef FIRMWARE_MINIMAL
  uint32_t _sectorStart = (ESP.getSketchSize() / SPI_FLASH_SEC_SIZE) + 1;
  uint32_t _sectorEnd = ESP.getFlashChipRealSize() / SPI_FLASH_SEC_SIZE;  // Flash size as reported by hardware
  if (1 == type) {
    // source Esp.cpp and core_esp8266_phy.cpp
    _sectorStart = (ESP.getFlashChipSize() / SPI_FLASH_SEC_SIZE) - 4;     // SDK parameter area
  }
  else if (2 == type) {
    _sectorStart = SETTINGS_LOCATION - CFG_ROTATES;                       // Tasmota parameter area (0x0F3xxx - 0x0FBFFF)
    _sectorEnd = SETTINGS_LOCATION +1;
  }
  else if (3 == type) {
    _sectorStart = SETTINGS_LOCATION - CFG_ROTATES;                       // Tasmota and SDK parameter area (0x0F3xxx - 0x0FFFFF)
    _sectorEnd = ESP.getFlashChipSize() / SPI_FLASH_SEC_SIZE;             // Flash size as seen by SDK
  }

  AddLog_P2(LOG_LEVEL_DEBUG, PSTR(D_LOG_APPLICATION D_ERASE " %d " D_UNIT_SECTORS), _sectorEnd - _sectorStart);

//  EspErase(_sectorStart, _sectorEnd);                                     // Arduino core and SDK - erases flash as seen by SDK
  EsptoolErase(_sectorStart, _sectorEnd);                                 // Esptool - erases flash completely
#endif  // FIRMWARE_MINIMAL
}

void SettingsSdkErase(void)
{
  WiFi.disconnect(true);    // Delete SDK wifi config
  SettingsErase(1);
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
  Settings.param[P_BACKLOG_DELAY] = MIN_BACKLOG_DELAY;
  Settings.param[P_BOOT_LOOP_OFFSET] = BOOT_LOOP_OFFSET;  // SetOption36
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
//  for (uint32_t i = 0; i < sizeof(Settings.my_gp); i++) { Settings.my_gp.io[i] = GPIO_NONE; }
  SettingsUpdateText(SET_FRIENDLYNAME1, FRIENDLY_NAME);
  SettingsUpdateText(SET_FRIENDLYNAME2, FRIENDLY_NAME"2");
  SettingsUpdateText(SET_FRIENDLYNAME3, FRIENDLY_NAME"3");
  SettingsUpdateText(SET_FRIENDLYNAME4, FRIENDLY_NAME"4");
  SettingsUpdateText(SET_OTAURL, OTA_URL);

  // Power
  Settings.flag.save_state = SAVE_STATE;
  Settings.power = APP_POWER;
  Settings.poweronstate = APP_POWERON_STATE;
  Settings.blinktime = APP_BLINKTIME;
  Settings.blinkcount = APP_BLINKCOUNT;
  Settings.ledstate = APP_LEDSTATE;
  Settings.ledmask = APP_LEDMASK;
  Settings.pulse_timer[0] = APP_PULSETIME;
//  for (uint32_t i = 1; i < MAX_PULSETIMERS; i++) { Settings.pulse_timer[i] = 0; }

  // Serial
  Settings.serial_config = TS_SERIAL_8N1;
  Settings.baudrate = APP_BAUDRATE / 300;
  Settings.sbaudrate = SOFT_BAUDRATE / 300;
  Settings.serial_delimiter = 0xff;
  Settings.seriallog_level = SERIAL_LOG_LEVEL;

  // Wifi
  Settings.wifi_output_power = 170;
  ParseIp(&Settings.ip_address[0], WIFI_IP_ADDRESS);
  ParseIp(&Settings.ip_address[1], WIFI_GATEWAY);
  ParseIp(&Settings.ip_address[2], WIFI_SUBNETMASK);
  ParseIp(&Settings.ip_address[3], WIFI_DNS);
  Settings.sta_config = WIFI_CONFIG_TOOL;
//  Settings.sta_active = 0;
  SettingsUpdateText(SET_STASSID1, STA_SSID1);
  SettingsUpdateText(SET_STASSID2, STA_SSID2);
  SettingsUpdateText(SET_STAPWD1, STA_PASS1);
  SettingsUpdateText(SET_STAPWD2, STA_PASS2);
  SettingsUpdateText(SET_HOSTNAME, WIFI_HOSTNAME);

  // Syslog
  SettingsUpdateText(SET_SYSLOG_HOST, SYS_LOG_HOST);
  Settings.syslog_port = SYS_LOG_PORT;
  Settings.syslog_level = SYS_LOG_LEVEL;

  // Webserver
  Settings.flag2.emulation = EMULATION;
  Settings.webserver = WEB_SERVER;
  Settings.weblog_level = WEB_LOG_LEVEL;
  SettingsUpdateText(SET_WEBPWD, WEB_PASSWORD);
  Settings.flag3.mdns_enabled = MDNS_ENABLED;
  SettingsUpdateText(SET_CORS, CORS_DOMAIN);

  // Button
//  Settings.flag.button_restrict = 0;
//  Settings.flag.button_swap = 0;
//  Settings.flag.button_single = 0;
  Settings.param[P_HOLD_TIME] = KEY_HOLD_TIME;  // Default 4 seconds hold time

  // Switch
  for (uint32_t i = 0; i < MAX_SWITCHES; i++) { Settings.switchmode[i] = SWITCH_MODE; }

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
  SettingsUpdateText(SET_MQTT_HOST, MQTT_HOST);
  Settings.mqtt_port = MQTT_PORT;
  SettingsUpdateText(SET_MQTT_CLIENT, MQTT_CLIENT_ID);
  SettingsUpdateText(SET_MQTT_USER, MQTT_USER);
  SettingsUpdateText(SET_MQTT_PWD, MQTT_PASS);
  SettingsUpdateText(SET_MQTT_TOPIC, MQTT_TOPIC);
  SettingsUpdateText(SET_MQTT_BUTTON_TOPIC, MQTT_BUTTON_TOPIC);
  SettingsUpdateText(SET_MQTT_SWITCH_TOPIC, MQTT_SWITCH_TOPIC);
  SettingsUpdateText(SET_MQTT_GRP_TOPIC, MQTT_GRPTOPIC);
  SettingsUpdateText(SET_MQTT_FULLTOPIC, MQTT_FULLTOPIC);
  Settings.mqtt_retry = MQTT_RETRY_SECS;
  SettingsUpdateText(SET_MQTTPREFIX1, SUB_PREFIX);
  SettingsUpdateText(SET_MQTTPREFIX2, PUB_PREFIX);
  SettingsUpdateText(SET_MQTTPREFIX3, PUB_PREFIX2);
  SettingsUpdateText(SET_STATE_TXT1, MQTT_STATUS_OFF);
  SettingsUpdateText(SET_STATE_TXT2, MQTT_STATUS_ON);
  SettingsUpdateText(SET_STATE_TXT3, MQTT_CMND_TOGGLE);
  SettingsUpdateText(SET_STATE_TXT4, MQTT_CMND_HOLD);
  char fingerprint[60];
  strlcpy(fingerprint, MQTT_FINGERPRINT1, sizeof(fingerprint));
  char *p = fingerprint;
  for (uint32_t i = 0; i < 20; i++) {
    Settings.mqtt_fingerprint[0][i] = strtol(p, &p, 16);
  }
  strlcpy(fingerprint, MQTT_FINGERPRINT2, sizeof(fingerprint));
  p = fingerprint;
  for (uint32_t i = 0; i < 20; i++) {
    Settings.mqtt_fingerprint[1][i] = strtol(p, &p, 16);
  }
  Settings.tele_period = TELE_PERIOD;
  Settings.mqttlog_level = MQTT_LOG_LEVEL;

  // Energy
  Settings.flag2.current_resolution = 3;
//  Settings.flag2.voltage_resolution = 0;
//  Settings.flag2.wattage_resolution = 0;
  Settings.flag2.energy_resolution = ENERGY_RESOLUTION;
  Settings.param[P_MAX_POWER_RETRY] = MAX_POWER_RETRY;
//  Settings.energy_power_delta = 0;
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
  Settings.param[P_IR_UNKNOW_THRESHOLD] = IR_RCV_MIN_UNKNOWN_SIZE;

  // RF Bridge
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
  Settings.flag.temperature_conversion = TEMP_CONVERSION;
  Settings.flag.pressure_conversion = PRESSURE_CONVERSION;
  Settings.flag2.pressure_resolution = PRESSURE_RESOLUTION;
  Settings.flag2.humidity_resolution = HUMIDITY_RESOLUTION;
  Settings.flag2.temperature_resolution = TEMP_RESOLUTION;
//  Settings.altitude = 0;

  // Rules
//  Settings.rule_enabled = 0;
//  Settings.rule_once = 0;
//  for (uint32_t i = 1; i < MAX_RULE_SETS; i++) { Settings.rules[i][0] = '\0'; }
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
  SettingsUpdateText(SET_NTPSERVER1, NTP_SERVER1);
  SettingsUpdateText(SET_NTPSERVER2, NTP_SERVER2);
  SettingsUpdateText(SET_NTPSERVER3, NTP_SERVER3);
  for (uint32_t i = 0; i < 3; i++) {
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
}

/********************************************************************************************/

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

void SettingsDefaultWebColor(void)
{
  char scolor[10];
  for (uint32_t i = 0; i < COL_LAST; i++) {
    WebHexCode(i, GetTextIndexed(scolor, sizeof(scolor), i, kWebColors));
  }
}

void SettingsEnableAllI2cDrivers(void)
{
  Settings.i2c_drivers[0] = 0xFFFFFFFF;
  Settings.i2c_drivers[1] = 0xFFFFFFFF;
  Settings.i2c_drivers[2] = 0xFFFFFFFF;
}

/********************************************************************************************/

void SettingsDelta(void)
{
  if (Settings.version != VERSION) {      // Fix version dependent changes
    if (Settings.version < 0x06000000) {
      Settings.cfg_size = sizeof(SYSCFG);
      Settings.cfg_crc = GetSettingsCrc();
    }
    if (Settings.version < 0x06000002) {
      for (uint32_t i = 0; i < MAX_SWITCHES; i++) {
        if (i < 4) {
          Settings.switchmode[i] = Settings.interlock[i];
        } else {
          Settings.switchmode[i] = SWITCH_MODE;
        }
      }
      for (uint32_t i = 0; i < sizeof(Settings.my_gp); i++) {
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
      for (uint32_t j = 0; j < 5; j++) {
        Settings.rgbwwTable[j] = 255;
      }
    }
    if (Settings.version < 0x06030002) {
      Settings.timezone_minutes = 0;
    }
    if (Settings.version < 0x06030004) {
      memset(&Settings.monitors, 0xFF, 20);  // Enable all possible monitors, displays and sensors
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
      for (uint32_t i = 1; i < MAX_INTERLOCKS; i++) { Settings.interlock[i] = 0; }
    }
    if (Settings.version < 0x0604010D) {
      Settings.param[P_BOOT_LOOP_OFFSET] = BOOT_LOOP_OFFSET;  // SetOption36
    }
    if (Settings.version < 0x06040110) {
      ModuleDefault(WEMOS);
    }
    if (Settings.version < 0x06040113) {
      Settings.param[P_RGB_REMAP] = RGB_REMAP_RGBW;
    }
    if (Settings.version < 0x06050003) {
      Settings.novasds_startingoffset = STARTING_OFFSET;
    }
    if (Settings.version < 0x06050006) {
      SettingsDefaultWebColor();
    }
    if (Settings.version < 0x06050007) {
      Settings.ledmask = APP_LEDMASK;
    }
    if (Settings.version < 0x0605000A) {
      Settings.my_adc0 = ADC0_NONE;
    }
    if (Settings.version < 0x0605000D) {
      Settings.param[P_IR_UNKNOW_THRESHOLD] = IR_RCV_MIN_UNKNOWN_SIZE;
    }
    if (Settings.version < 0x06060001) {
      Settings.param[P_OVER_TEMP] = ENERGY_OVERTEMP;
    }
    if (Settings.version < 0x06060007) {
      memset((char*)&Settings +0xE00, 0x00, sizeof(SYSCFG) -0xE00);
    }
    if (Settings.version < 0x06060008) {
      // Move current tuya dimmer range to the new param.
      if (Settings.flag3.tuya_serial_mqtt_publish) { // ex Settings.flag3.ex_tuya_dimmer_range_255 SetOption
        Settings.param[P_ex_DIMMER_MAX] = 100;
      } else {
        Settings.param[P_ex_DIMMER_MAX] = 255;
      }
    }
    if (Settings.version < 0x06060009) {
      Settings.baudrate = APP_BAUDRATE / 300;
      Settings.sbaudrate = SOFT_BAUDRATE / 300;
    }
    if (Settings.version < 0x0606000A) {
      uint8_t tuyaindex = 0;
      if (Settings.param[P_BACKLOG_DELAY] > 0) {             // ex SetOption34
        Settings.tuya_fnid_map[tuyaindex].fnid = 21;         // TUYA_MCU_FUNC_DIMMER - Move Tuya Dimmer Id to Map
        Settings.tuya_fnid_map[tuyaindex].dpid = Settings.param[P_BACKLOG_DELAY];
        tuyaindex++;
      } else if (Settings.flag3.fast_power_cycle_disable == 1) {  // ex SetOption65
        Settings.tuya_fnid_map[tuyaindex].fnid = 11;         // TUYA_MCU_FUNC_REL1 - Create FnID for Switches
        Settings.tuya_fnid_map[tuyaindex].dpid = 1;
        tuyaindex++;
      }
      if (Settings.param[P_ex_TUYA_RELAYS] > 0) {
        for (uint8_t i = 0 ; i < Settings.param[P_ex_TUYA_RELAYS]; i++) {  // ex SetOption41
          Settings.tuya_fnid_map[tuyaindex].fnid = 12 + i;   // TUYA_MCU_FUNC_REL2 -  Create FnID for Switches
          Settings.tuya_fnid_map[tuyaindex].dpid = i + 2;
          tuyaindex++;
        }
      }
      if (Settings.param[P_ex_TUYA_POWER_ID] > 0) {          // ex SetOption46
        Settings.tuya_fnid_map[tuyaindex].fnid = 31;         // TUYA_MCU_FUNC_POWER -  Move Tuya Power Id to Map
        Settings.tuya_fnid_map[tuyaindex].dpid = Settings.param[P_ex_TUYA_POWER_ID];
        tuyaindex++;
      }
      if (Settings.param[P_ex_TUYA_VOLTAGE_ID] > 0) {        // ex SetOption44
        Settings.tuya_fnid_map[tuyaindex].fnid = 33;         // TUYA_MCU_FUNC_VOLTAGE - Move Tuya Voltage Id to Map
        Settings.tuya_fnid_map[tuyaindex].dpid = Settings.param[P_ex_TUYA_VOLTAGE_ID];
        tuyaindex++;
      }
      if (Settings.param[P_ex_TUYA_CURRENT_ID] > 0) {        // ex SetOption45
        Settings.tuya_fnid_map[tuyaindex].fnid = 32;         // TUYA_MCU_FUNC_CURRENT - Move Tuya Current Id to Map
        Settings.tuya_fnid_map[tuyaindex].dpid = Settings.param[P_ex_TUYA_CURRENT_ID];
      }
    }
    if (Settings.version < 0x0606000C) {
      memset((char*)&Settings +0x1D6, 0x00, 16);
    }
    if (Settings.version < 0x0606000F) {
      Settings.shutter_accuracy = 0;
      Settings.mqttlog_level = MQTT_LOG_LEVEL;
    }
    if (Settings.version < 0x06060011) {
      Settings.param[P_BACKLOG_DELAY] = MIN_BACKLOG_DELAY;
    }
    if (Settings.version < 0x06060012) {
      Settings.dimmer_hw_min = DEFAULT_DIMMER_MIN;
      Settings.dimmer_hw_max = DEFAULT_DIMMER_MAX;
      if (TUYA_DIMMER == Settings.module) {
        if (Settings.flag3.ex_tuya_dimmer_min_limit) {
          Settings.dimmer_hw_min = 25;
        } else {
          Settings.dimmer_hw_min = 1;
        }
        Settings.dimmer_hw_max = Settings.param[P_ex_DIMMER_MAX];
      }
      else if (PS_16_DZ == Settings.module) {
        Settings.dimmer_hw_min = 10;
        Settings.dimmer_hw_max = Settings.param[P_ex_DIMMER_MAX];
      }
    }
    if (Settings.version < 0x06060014) {
      // Clear unused parameters for future use
/*
      Settings.flag3.tuya_serial_mqtt_publish = 0;  // ex Settings.flag3.ex_tuya_dimmer_range_255
      Settings.flag3.ex_tuya_dimmer_min_limit = 0;
      Settings.param[P_ex_TUYA_RELAYS] = 0;
      Settings.param[P_ex_DIMMER_MAX] = 0;
      Settings.param[P_ex_TUYA_VOLTAGE_ID] = 0;
      Settings.param[P_ex_TUYA_CURRENT_ID] = 0;
      Settings.param[P_ex_TUYA_POWER_ID] = 0;
      Settings.ex_baudrate = 0;
      Settings.ex_sbaudrate = 0;
*/
      Settings.flag3.fast_power_cycle_disable = 0;
      Settings.energy_power_delta = Settings.ex_energy_power_delta;
      Settings.ex_energy_power_delta = 0;
    }
    if (Settings.version < 0x06060015) {
      if ((EX_WIFI_SMARTCONFIG == Settings.sta_config) || (EX_WIFI_WPSCONFIG == Settings.sta_config)) {
        Settings.sta_config = WIFI_MANAGER;
      }
    }

    if (Settings.version < 0x07000002) {
      Settings.web_color2[0][0] = Settings.web_color[0][0];
      Settings.web_color2[0][1] = Settings.web_color[0][1];
      Settings.web_color2[0][2] = Settings.web_color[0][2];
    }
    if (Settings.version < 0x07000003) {
      SettingsEnableAllI2cDrivers();
    }
    if (Settings.version < 0x07000004) {
      Settings.wifi_output_power = 170;
    }
    if (Settings.version < 0x07010202) {
      Settings.serial_config = TS_SERIAL_8N1;
    }
    if (Settings.version < 0x07010204) {
      if (Settings.flag3.ex_cors_enabled == 1) {
        strlcpy(Settings.cors_domain, CORS_ENABLED_ALL, sizeof(Settings.cors_domain));
      } else {
        Settings.cors_domain[0] = 0;
      }
    }
    if (Settings.version < 0x07010205) {
      Settings.seriallog_level = Settings.ex_seriallog_level;  // 09E -> 452
      Settings.sta_config = Settings.ex_sta_config;            // 09F -> EC7
      Settings.sta_active = Settings.ex_sta_active;            // 0A0 -> EC8
      memcpy((char*)&Settings.rule_stop, (char*)&Settings.ex_rule_stop, 47);  // 1A7 -> EC9
    }
    if (Settings.version < 0x07010206) {
      Settings.flag4 = Settings.ex_flag4;                      // 1E0 -> EF8
      Settings.mqtt_port = Settings.ex_mqtt_port;              // 20A -> EFC
      memcpy((char*)&Settings.serial_config, (char*)&Settings.ex_serial_config, 5);  // 1E4 -> EFE
    }

    if ((VERSION < 0x08000000) && (Settings.version > VERSION)) {
      char temp[strlen(SettingsText(SET_OTAURL)) +1];        strncpy(temp, SettingsText(SET_OTAURL), sizeof(temp));
      char temp21[strlen(SettingsText(SET_MQTTPREFIX1)) +1]; strncpy(temp21, SettingsText(SET_MQTTPREFIX1), sizeof(temp21));
      char temp22[strlen(SettingsText(SET_MQTTPREFIX2)) +1]; strncpy(temp22, SettingsText(SET_MQTTPREFIX2), sizeof(temp22));
      char temp23[strlen(SettingsText(SET_MQTTPREFIX3)) +1]; strncpy(temp23, SettingsText(SET_MQTTPREFIX3), sizeof(temp23));
      char temp31[strlen(SettingsText(SET_STASSID1)) +1];    strncpy(temp31, SettingsText(SET_STASSID1), sizeof(temp31));
      char temp32[strlen(SettingsText(SET_STASSID2)) +1];    strncpy(temp32, SettingsText(SET_STASSID2), sizeof(temp32));
      char temp41[strlen(SettingsText(SET_STAPWD1)) +1];     strncpy(temp41, SettingsText(SET_STAPWD1), sizeof(temp41));
      char temp42[strlen(SettingsText(SET_STAPWD2)) +1];     strncpy(temp42, SettingsText(SET_STAPWD2), sizeof(temp42));
      char temp5[strlen(SettingsText(SET_HOSTNAME)) +1];     strncpy(temp5, SettingsText(SET_HOSTNAME), sizeof(temp5));
      char temp6[strlen(SettingsText(SET_SYSLOG_HOST)) +1];  strncpy(temp6, SettingsText(SET_SYSLOG_HOST), sizeof(temp5));

      uint32_t version = Settings.version;
      Settings.version = VERSION;
      SettingsUpdateText(SET_OTAURL, temp);
      SettingsUpdateText(SET_MQTTPREFIX1, temp21);
      SettingsUpdateText(SET_MQTTPREFIX2, temp22);
      SettingsUpdateText(SET_MQTTPREFIX3, temp23);
      SettingsUpdateText(SET_STASSID1, temp31);
      SettingsUpdateText(SET_STASSID2, temp32);
      SettingsUpdateText(SET_STAPWD1, temp41);
      SettingsUpdateText(SET_STAPWD2, temp42);
      SettingsUpdateText(SET_HOSTNAME, temp5);
      SettingsUpdateText(SET_SYSLOG_HOST, temp6);
      Settings.version = version;
    }

    Settings.version = VERSION;
    SettingsSave(1);
  }
}
