/*
  settings.ino - user settings for Sonoff-Tasmota

  Copyright (C) 2017  Theo Arends

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
#define DOMOTICZ_UPDATE_TIMER  0               // [DomoticzUpdateTimer] Send relay status (0 = disable, 1 - 3600 seconds) (Optional)
#endif

/*********************************************************************************************\
 * RTC memory
\*********************************************************************************************/

#define RTC_MEM_VALID 0xA55A

uint32_t rtc_settings_hash = 0;

uint32_t GetRtcSettingsHash()
{
  uint32_t hash = 0;
  uint8_t *bytes = (uint8_t*)&RtcSettings;

  for (uint16_t i = 0; i < sizeof(RTCMEM); i++) {
    hash += bytes[i]*(i+1);
  }
  return hash;
}

void RtcSettingsSave()
{
  if (GetRtcSettingsHash() != rtc_settings_hash) {
    RtcSettings.valid = RTC_MEM_VALID;
    ESP.rtcUserMemoryWrite(100, (uint32_t*)&RtcSettings, sizeof(RTCMEM));
    rtc_settings_hash = GetRtcSettingsHash();
#ifdef DEBUG_THEO
    AddLog_P(LOG_LEVEL_DEBUG, PSTR("Dump: Save"));
    RtcSettingsDump();
#endif  // DEBUG_THEO
  }
}

void RtcSettingsLoad()
{
  ESP.rtcUserMemoryRead(100, (uint32_t*)&RtcSettings, sizeof(RTCMEM));
#ifdef DEBUG_THEO
  AddLog_P(LOG_LEVEL_DEBUG, PSTR("Dump: Load"));
  RtcSettingsDump();
#endif  // DEBUG_THEO
  if (RtcSettings.valid != RTC_MEM_VALID) {
    memset(&RtcSettings, 0, sizeof(RTCMEM));
    RtcSettings.valid = RTC_MEM_VALID;
    RtcSettings.hlw_kWhtoday = Settings.hlw_kWhtoday;
    RtcSettings.hlw_kWhtotal = Settings.hlw_kWhtotal;
    for (byte i = 0; i < MAX_COUNTERS; i++) {
      RtcSettings.pulse_counter[i] = Settings.pulse_counter[i];
    }
    RtcSettings.power = Settings.power;
    RtcSettingsSave();
  }
  rtc_settings_hash = GetRtcSettingsHash();
}

boolean RtcSettingsValid()
{
  return (RTC_MEM_VALID == RtcSettings.valid);
}

#ifdef DEBUG_THEO
void RtcSettingsDump()
{
  #define CFG_COLS 16

  uint16_t idx;
  uint16_t maxrow;
  uint16_t row;
  uint16_t col;

  uint8_t *buffer = (uint8_t *) &RtcSettings;
  maxrow = ((sizeof(RTCMEM)+CFG_COLS)/CFG_COLS);

  for (row = 0; row < maxrow; row++) {
    idx = row * CFG_COLS;
    snprintf_P(log_data, sizeof(log_data), PSTR("%04X:"), idx);
    for (col = 0; col < CFG_COLS; col++) {
      if (!(col%4)) {
        snprintf_P(log_data, sizeof(log_data), PSTR("%s "), log_data);
      }
      snprintf_P(log_data, sizeof(log_data), PSTR("%s %02X"), log_data, buffer[idx + col]);
    }
    snprintf_P(log_data, sizeof(log_data), PSTR("%s |"), log_data);
    for (col = 0; col < CFG_COLS; col++) {
//      if (!(col%4)) {
//        snprintf_P(log_data, sizeof(log_data), PSTR("%s "), log_data);
//      }
      snprintf_P(log_data, sizeof(log_data), PSTR("%s%c"), log_data, ((buffer[idx + col] > 0x20) && (buffer[idx + col] < 0x7F)) ? (char)buffer[idx + col] : ' ');
    }
    snprintf_P(log_data, sizeof(log_data), PSTR("%s|"), log_data);
    AddLog(LOG_LEVEL_INFO);
  }
}
#endif  // DEBUG_THEO

/*********************************************************************************************\
 * Config - Flash
\*********************************************************************************************/

extern "C" {
#include "spi_flash.h"
}
#include "eboot_command.h"

extern "C" uint32_t _SPIFFS_end;

#define SPIFFS_END          ((uint32_t)&_SPIFFS_end - 0x40200000) / SPI_FLASH_SEC_SIZE

// Version 3.x config
#define SETTINGS_LOCATION_3 SPIFFS_END - 4

// Version 4.2 config = eeprom area
#define SETTINGS_LOCATION   SPIFFS_END  // No need for SPIFFS as it uses EEPROM area
// Version 5.2 allow for more flash space
#define CFG_ROTATES         8           // Number of flash sectors used (handles uploads)

uint32_t settings_hash = 0;
uint32_t settings_location = SETTINGS_LOCATION;

/********************************************************************************************/
/*
 * Based on cores/esp8266/Updater.cpp
 */
void SetFlashModeDout()
{
  uint8_t *_buffer;
  uint32_t address;

  eboot_command ebcmd;
  eboot_command_read(&ebcmd);
  address = ebcmd.args[0];
  _buffer = new uint8_t[FLASH_SECTOR_SIZE];
  if (SPI_FLASH_RESULT_OK == spi_flash_read(address, (uint32_t*)_buffer, FLASH_SECTOR_SIZE)) {
    if (_buffer[2] != 3) {  // DOUT
      _buffer[2] = 3;
      noInterrupts();
      if (SPI_FLASH_RESULT_OK == spi_flash_erase_sector(address / FLASH_SECTOR_SIZE)) {
        spi_flash_write(address, (uint32_t*)_buffer, FLASH_SECTOR_SIZE);
      }
      interrupts();
    }
  }
  delete[] _buffer;
}

uint32_t GetSettingsHash()
{
  uint32_t hash = 0;
  uint8_t *bytes = (uint8_t*)&Settings;

  for (uint16_t i = 0; i < sizeof(SYSCFG); i++) {
    hash += bytes[i]*(i+1);
  }
  return hash;
}

void SettingsSaveAll()
{
  if (Settings.flag.save_state) {
    Settings.power = power;
  } else {
    Settings.power = 0;
  }
  if (hlw_flg) {
    HlwSaveState();
  }
  CounterSaveState();
  SettingsSave(0);
}

/*********************************************************************************************\
 * Config Save - Save parameters to Flash ONLY if any parameter has changed
\*********************************************************************************************/

uint32_t GetSettingsAddress()
{
  return settings_location * SPI_FLASH_SEC_SIZE;
}

void SettingsSave(byte rotate)
{
/* Save configuration in eeprom or one of 7 slots below
 *
 * rotate 0 = Save in next flash slot
 * rotate 1 = Save only in eeprom flash slot until SetOption12 0 or restart
 * rotate 2 = Save in eeprom flash slot, erase next flash slots and continue depending on stop_flash_rotate
 * stop_flash_rotate 0 = Allow flash slot rotation (SetOption12 0)
 * stop_flash_rotate 1 = Allow only eeprom flash slot use (SetOption12 1)
 */
#ifndef BE_MINIMAL
  if ((GetSettingsHash() != settings_hash) || rotate) {
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
    noInterrupts();
    spi_flash_erase_sector(settings_location);
    spi_flash_write(settings_location * SPI_FLASH_SEC_SIZE, (uint32*)&Settings, sizeof(SYSCFG));
    interrupts();
    if (!stop_flash_rotate && rotate) {
      for (byte i = 1; i < CFG_ROTATES; i++) {
        noInterrupts();
        spi_flash_erase_sector(settings_location -i);  // Delete previous configurations by resetting to 0xFF
        interrupts();
        delay(1);
      }
    }
    snprintf_P(log_data, sizeof(log_data), PSTR(D_LOG_CONFIG D_SAVED_TO_FLASH_AT " %X, " D_COUNT " %d, " D_BYTES " %d"),
       settings_location, Settings.save_flag, sizeof(SYSCFG));
    AddLog(LOG_LEVEL_DEBUG);
    settings_hash = GetSettingsHash();
  }
#endif  // BE_MINIMAL
  RtcSettingsSave();
}

void SettingsLoad()
{
/* Load configuration from eeprom or one of 7 slots below if first load does not stop_flash_rotate
 */
  struct SYSCFGH {
    unsigned long cfg_holder;
    unsigned long save_flag;
  } _SettingsH;

  settings_location = SETTINGS_LOCATION +1;
  for (byte i = 0; i < CFG_ROTATES; i++) {
    settings_location--;
    noInterrupts();
    spi_flash_read(settings_location * SPI_FLASH_SEC_SIZE, (uint32*)&Settings, sizeof(SYSCFG));
    spi_flash_read((settings_location -1) * SPI_FLASH_SEC_SIZE, (uint32*)&_SettingsH, sizeof(SYSCFGH));
    interrupts();

//  snprintf_P(log_data, sizeof(log_data), PSTR("Cnfg: Check at %X with count %d and holder %X"), settings_location -1, _SettingsH.save_flag, _SettingsH.cfg_holder);
//  AddLog(LOG_LEVEL_DEBUG);

    if (((Settings.version > 0x05000200) && Settings.flag.stop_flash_rotate) || (Settings.cfg_holder != _SettingsH.cfg_holder) || (Settings.save_flag > _SettingsH.save_flag)) {
      break;
    }
    delay(1);
  }
  snprintf_P(log_data, sizeof(log_data), PSTR(D_LOG_CONFIG D_LOADED_FROM_FLASH_AT " %X, " D_COUNT " %d"),
    settings_location, Settings.save_flag);
  AddLog(LOG_LEVEL_DEBUG);
  if (Settings.cfg_holder != CFG_HOLDER) {
    // Auto upgrade
    noInterrupts();
    spi_flash_read((SETTINGS_LOCATION_3) * SPI_FLASH_SEC_SIZE, (uint32*)&Settings, sizeof(SYSCFG));
    spi_flash_read((SETTINGS_LOCATION_3 + 1) * SPI_FLASH_SEC_SIZE, (uint32*)&_SettingsH, sizeof(SYSCFGH));
    if (Settings.save_flag < _SettingsH.save_flag)
      spi_flash_read((SETTINGS_LOCATION_3 + 1) * SPI_FLASH_SEC_SIZE, (uint32*)&Settings, sizeof(SYSCFG));
    interrupts();
    if ((Settings.cfg_holder != CFG_HOLDER) || (Settings.version >= 0x04020000)) {
      SettingsDefault();
    }
  }

  settings_hash = GetSettingsHash();

  RtcSettingsLoad();
}

void SettingsErase()
{
  SpiFlashOpResult result;

  uint32_t _sectorStart = (ESP.getSketchSize() / SPI_FLASH_SEC_SIZE) + 1;
  uint32_t _sectorEnd = ESP.getFlashChipRealSize() / SPI_FLASH_SEC_SIZE;
  boolean _serialoutput = (LOG_LEVEL_DEBUG_MORE <= seriallog_level);

  snprintf_P(log_data, sizeof(log_data), PSTR(D_LOG_APPLICATION D_ERASE " %d " D_UNIT_SECTORS), _sectorEnd - _sectorStart);
  AddLog(LOG_LEVEL_DEBUG);

  for (uint32_t _sector = _sectorStart; _sector < _sectorEnd; _sector++) {
    noInterrupts();
    result = spi_flash_erase_sector(_sector);
    interrupts();
    if (_serialoutput) {
      Serial.print(F(D_LOG_APPLICATION D_ERASED_SECTOR " "));
      Serial.print(_sector);
      if (SPI_FLASH_RESULT_OK == result) {
        Serial.println(F(" " D_OK));
      } else {
        Serial.println(F(" " D_ERROR));
      }
      delay(10);
    }
    OsWatchLoop();
  }
}

void SettingsDump(char* parms)
{
  #define CFG_COLS 16

  uint16_t idx;
  uint16_t maxrow;
  uint16_t row;
  uint16_t col;
  char *p;

  uint8_t *buffer = (uint8_t *) &Settings;
  maxrow = ((sizeof(SYSCFG)+CFG_COLS)/CFG_COLS);

  uint16_t srow = strtol(parms, &p, 16) / CFG_COLS;
  uint16_t mrow = strtol(p, &p, 10);

//  snprintf_P(log_data, sizeof(log_data), PSTR("Cnfg: Parms %s, Start row %d, rows %d"), parms, srow, mrow);
//  AddLog(LOG_LEVEL_DEBUG);

  if (0 == mrow) {  // Default only 8 lines
    mrow = 8;
  }
  if (srow > maxrow) {
    srow = maxrow - mrow;
  }
  if (mrow < (maxrow - srow)) {
    maxrow = srow + mrow;
  }

  for (row = srow; row < maxrow; row++) {
    idx = row * CFG_COLS;
    snprintf_P(log_data, sizeof(log_data), PSTR("%04X:"), idx);
    for (col = 0; col < CFG_COLS; col++) {
      if (!(col%4)) {
        snprintf_P(log_data, sizeof(log_data), PSTR("%s "), log_data);
      }
      snprintf_P(log_data, sizeof(log_data), PSTR("%s %02X"), log_data, buffer[idx + col]);
    }
    snprintf_P(log_data, sizeof(log_data), PSTR("%s |"), log_data);
    for (col = 0; col < CFG_COLS; col++) {
//      if (!(col%4)) {
//        snprintf_P(log_data, sizeof(log_data), PSTR("%s "), log_data);
//      }
      snprintf_P(log_data, sizeof(log_data), PSTR("%s%c"), log_data, ((buffer[idx + col] > 0x20) && (buffer[idx + col] < 0x7F)) ? (char)buffer[idx + col] : ' ');
    }
    snprintf_P(log_data, sizeof(log_data), PSTR("%s|"), log_data);
    AddLog(LOG_LEVEL_INFO);
    delay(1);
  }
}

/********************************************************************************************/

void SettingsDefault()
{
  AddLog_P(LOG_LEVEL_NONE, PSTR(D_LOG_CONFIG D_USE_DEFAULTS));
  SettingsDefaultSet1();
  SettingsDefaultSet2();
  SettingsSave(2);
}

void SettingsDefaultSet1()
{
  memset(&Settings, 0x00, sizeof(SYSCFG));

  Settings.cfg_holder = CFG_HOLDER;
//  Settings.save_flag = 0;
  Settings.version = VERSION;
//  Settings.bootcount = 0;
}

void SettingsDefaultSet2()
{
  memset((char*)&Settings +16, 0x00, sizeof(SYSCFG) -16);

  Settings.flag.save_state = SAVE_STATE;
  //Settings.flag.button_restrict = 0;
  //Settings.flag.value_units = 0;
  Settings.flag.mqtt_enabled = MQTT_USE;
  //Settings.flag.mqtt_response = 0;
  Settings.flag.mqtt_power_retain = MQTT_POWER_RETAIN;
  Settings.flag.mqtt_button_retain = MQTT_BUTTON_RETAIN;
  Settings.flag.mqtt_switch_retain = MQTT_SWITCH_RETAIN;

  Settings.flag2.emulation = EMULATION;

  Settings.save_data = SAVE_DATA;
  Settings.timezone = APP_TIMEZONE;
  strlcpy(Settings.ota_url, OTA_URL, sizeof(Settings.ota_url));

  Settings.seriallog_level = SERIAL_LOG_LEVEL;
//  Settings.sta_active = 0;
  strlcpy(Settings.sta_ssid[0], STA_SSID1, sizeof(Settings.sta_ssid[0]));
  strlcpy(Settings.sta_pwd[0], STA_PASS1, sizeof(Settings.sta_pwd[0]));
  strlcpy(Settings.sta_ssid[1], STA_SSID2, sizeof(Settings.sta_ssid[1]));
  strlcpy(Settings.sta_pwd[1], STA_PASS2, sizeof(Settings.sta_pwd[1]));
  strlcpy(Settings.hostname, WIFI_HOSTNAME, sizeof(Settings.hostname));
  Settings.sta_config = WIFI_CONFIG_TOOL;
  strlcpy(Settings.syslog_host, SYS_LOG_HOST, sizeof(Settings.syslog_host));
  Settings.syslog_port = SYS_LOG_PORT;
  Settings.syslog_level = SYS_LOG_LEVEL;
  Settings.webserver = WEB_SERVER;
  Settings.weblog_level = WEB_LOG_LEVEL;

  strlcpy(Settings.mqtt_fingerprint, MQTT_FINGERPRINT, sizeof(Settings.mqtt_fingerprint));
  strlcpy(Settings.mqtt_host, MQTT_HOST, sizeof(Settings.mqtt_host));
  Settings.mqtt_port = MQTT_PORT;
  strlcpy(Settings.mqtt_client, MQTT_CLIENT_ID, sizeof(Settings.mqtt_client));
  strlcpy(Settings.mqtt_user, MQTT_USER, sizeof(Settings.mqtt_user));
  strlcpy(Settings.mqtt_pwd, MQTT_PASS, sizeof(Settings.mqtt_pwd));
  strlcpy(Settings.mqtt_topic, MQTT_TOPIC, sizeof(Settings.mqtt_topic));
  strlcpy(Settings.button_topic, "0", sizeof(Settings.button_topic));
  strlcpy(Settings.mqtt_grptopic, MQTT_GRPTOPIC, sizeof(Settings.mqtt_grptopic));
  Settings.tele_period = TELE_PERIOD;

  Settings.power = APP_POWER;
  Settings.poweronstate = APP_POWERON_STATE;
  Settings.ledstate = APP_LEDSTATE;
  Settings.blinktime = APP_BLINKTIME;
  Settings.blinkcount = APP_BLINKCOUNT;
  Settings.sleep = APP_SLEEP;

  Settings.domoticz_update_timer = DOMOTICZ_UPDATE_TIMER;
  for (byte i = 0; i < MAX_SWITCHES; i++) {
    Settings.switchmode[i] = SWITCH_MODE;
//    Settings.domoticz_relay_idx[i] = 0;
//    Settings.domoticz_key_idx[i] = 0;
//    Settings.domoticz_switch_idx[i] = 0;
  }

  Settings.hlw_power_calibration = HLW_PREF_PULSE;
  Settings.hlw_voltage_calibration = HLW_UREF_PULSE;
  Settings.hlw_current_calibration = HLW_IREF_PULSE;
//  Settings.hlw_kWhtoday = 0;
//  Settings.hlw_kWhyesterday = 0;
//  Settings.hlw_kWhdoy = 0;
//  Settings.hlw_pmin = 0;
//  Settings.hlw_pmax = 0;
//  Settings.hlw_umin = 0;
//  Settings.hlw_umax = 0;
//  Settings.hlw_imin = 0;
//  Settings.hlw_imax = 0;
//  Settings.hlw_mpl = 0;                              // MaxPowerLimit
  Settings.hlw_mplh = MAX_POWER_HOLD;
  Settings.hlw_mplw = MAX_POWER_WINDOW;
//  Settings.hlw_mspl = 0;                             // MaxSafePowerLimit
  Settings.hlw_msplh = SAFE_POWER_HOLD;
  Settings.hlw_msplw = SAFE_POWER_WINDOW;
//  Settings.hlw_mkwh = 0;                             // MaxEnergy
//  Settings.hlw_mkwhs = 0;                            // MaxEnergyStart

  SettingsDefaultSet_3_2_4();

  strlcpy(Settings.friendlyname[0], FRIENDLY_NAME, sizeof(Settings.friendlyname[0]));
  strlcpy(Settings.friendlyname[1], FRIENDLY_NAME"2", sizeof(Settings.friendlyname[1]));
  strlcpy(Settings.friendlyname[2], FRIENDLY_NAME"3", sizeof(Settings.friendlyname[2]));
  strlcpy(Settings.friendlyname[3], FRIENDLY_NAME"4", sizeof(Settings.friendlyname[3]));

  SettingsDefaultSet_3_9_3();

  strlcpy(Settings.switch_topic, "0", sizeof(Settings.switch_topic));

  strlcpy(Settings.web_password, WEB_PASSWORD, sizeof(Settings.web_password));

  SettingsDefaultSet_4_0_4();
  Settings.pulse_timer[0] = APP_PULSETIME;

  // 4.0.7
//  for (byte i = 0; i < MAX_PWMS; i++) Settings.pwm_value[i] = 0;

  // 4.0.9
  SettingsDefaultSet_4_0_9();

  // 4.1.1 + 5.1.6
  SettingsDefaultSet_4_1_1();

  // 5.0.2
  SettingsDefaultSet_5_0_2();

  // 5.0.4
//  Settings.hlw_kWhtotal = 0;
  RtcSettings.hlw_kWhtotal = 0;

  // 5.0.5
  strlcpy(Settings.mqtt_fulltopic, MQTT_FULLTOPIC, sizeof(Settings.mqtt_fulltopic));

  // 5.0.6
  Settings.mqtt_retry = MQTT_RETRY_SECS;

  // 5.1.7
  Settings.param[P_HOLD_TIME] = KEY_HOLD_TIME;  // Default 4 seconds hold time

  // 5.2.0
  Settings.param[P_MAX_POWER_RETRY] = MAX_POWER_RETRY;

  // 5.4.1
  memcpy_P(Settings.rf_code[0], kDefaultRfCode, 9);

  // 5.8.0
  Settings.light_pixels = WS2812_LEDS;

  // 5.8.1
//  Settings.altitude = 0;
  Settings.pwm_frequency = PWM_FREQ;
  Settings.pwm_range = PWM_RANGE;
  SettingsDefaultSet_5_8_1();

  // 5.9.2
  Settings.flag2.current_resolution = 3;
}

/********************************************************************************************/

void SettingsDefaultSet_3_2_4()
{
  Settings.ws_pixels = WS2812_LEDS;
  Settings.ws_red = 255;
  Settings.ws_green = 0;
  Settings.ws_blue = 0;
  Settings.ws_ledtable = 0;
  Settings.ws_dimmer = 8;
  Settings.ws_fade = 0;
  Settings.ws_speed = 1;
  Settings.ws_scheme = 0;
  Settings.ex_ws_width = 1;
  Settings.ws_wakeup = 0;
}

void SettingsDefaultSet_3_9_3()
{
  for (byte i = 0; i < MAX_DOMOTICZ_IDX; i++) {
    Settings.domoticz_switch_idx[i] = 0;
  }
  for (byte i = 0; i < 12; i++) {
    Settings.domoticz_sensor_idx[i] = 0;
  }

  Settings.module = MODULE;
  for (byte i = 0; i < MAX_GPIO_PIN; i++){
    Settings.my_gp.io[i] = 0;
  }

  Settings.light_pixels = WS2812_LEDS;
  for (byte i = 0; i < MAX_PWMS; i++) {
    Settings.light_color[i] = 255;
  }
  Settings.light_correction = 0;
  Settings.light_dimmer = 10;
  Settings.light_fade = 0;
  Settings.light_speed = 1;
  Settings.light_scheme = 0;
  Settings.light_width = 1;
  Settings.light_wakeup = 0;
}

void SettingsDefaultSet_4_0_4()
{
  strlcpy(Settings.ntp_server[0], NTP_SERVER1, sizeof(Settings.ntp_server[0]));
  strlcpy(Settings.ntp_server[1], NTP_SERVER2, sizeof(Settings.ntp_server[1]));
  strlcpy(Settings.ntp_server[2], NTP_SERVER3, sizeof(Settings.ntp_server[2]));
  for (byte j = 0; j < 3; j++) {
    for (byte i = 0; i < strlen(Settings.ntp_server[j]); i++) {
      if (Settings.ntp_server[j][i] == ',') {
        Settings.ntp_server[j][i] = '.';
      }
    }
  }
  Settings.pulse_timer[0] = APP_PULSETIME;
  for (byte i = 1; i < MAX_PULSETIMERS; i++) {
    Settings.pulse_timer[i] = 0;
  }
}

void SettingsDefaultSet_4_0_9()
{
  strlcpy(Settings.mqtt_prefix[0], SUB_PREFIX, sizeof(Settings.mqtt_prefix[0]));
  strlcpy(Settings.mqtt_prefix[1], PUB_PREFIX, sizeof(Settings.mqtt_prefix[1]));
  strlcpy(Settings.mqtt_prefix[2], PUB_PREFIX2, sizeof(Settings.mqtt_prefix[2]));
  ParseIp(&Settings.ip_address[0], WIFI_IP_ADDRESS);
  ParseIp(&Settings.ip_address[1], WIFI_GATEWAY);
  ParseIp(&Settings.ip_address[2], WIFI_SUBNETMASK);
  ParseIp(&Settings.ip_address[3], WIFI_DNS);
}

void SettingsDefaultSet_4_1_1()
{
  strlcpy(Settings.state_text[0], MQTT_STATUS_OFF, sizeof(Settings.state_text[0]));
  strlcpy(Settings.state_text[1], MQTT_STATUS_ON, sizeof(Settings.state_text[1]));
  strlcpy(Settings.state_text[2], MQTT_CMND_TOGGLE, sizeof(Settings.state_text[2]));
  strlcpy(Settings.state_text[3], MQTT_CMND_HOLD, sizeof(Settings.state_text[3]));  // v5.1.6
}

void SettingsDefaultSet_5_0_2()
{
  Settings.flag.temperature_conversion = TEMP_CONVERSION;
  Settings.flag2.temperature_resolution = TEMP_RESOLUTION;
  Settings.flag2.humidity_resolution = HUMIDITY_RESOLUTION;
  Settings.flag2.pressure_resolution = PRESSURE_RESOLUTION;
  Settings.flag2.energy_resolution = ENERGY_RESOLUTION;
}

void SettingsDefaultSet_5_8_1()
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

/********************************************************************************************/

void SettingsDelta()
{
  if (Settings.version != VERSION) {      // Fix version dependent changes
    if (Settings.version < 0x03010200) {  // 3.1.2 - Add parameter
      Settings.poweronstate = APP_POWERON_STATE;
    }
    if (Settings.version < 0x03010600) {  // 3.1.6 - Add parameter
      Settings.blinktime = APP_BLINKTIME;
      Settings.blinkcount = APP_BLINKCOUNT;
    }
    if (Settings.version < 0x03020400) {  // 3.2.4 - Add parameter
      SettingsDefaultSet_3_2_4();
    }
    if (Settings.version < 0x03020500) {  // 3.2.5 - Add parameter
      GetMqttClient(Settings.friendlyname[0], Settings.mqtt_client, sizeof(Settings.friendlyname[0]));
      strlcpy(Settings.friendlyname[1], FRIENDLY_NAME"2", sizeof(Settings.friendlyname[1]));
      strlcpy(Settings.friendlyname[2], FRIENDLY_NAME"3", sizeof(Settings.friendlyname[2]));
      strlcpy(Settings.friendlyname[3], FRIENDLY_NAME"4", sizeof(Settings.friendlyname[3]));
    }
    if (Settings.version < 0x03020800) {  // 3.2.8 - Add parameter
      strlcpy(Settings.switch_topic, Settings.button_topic, sizeof(Settings.switch_topic));
    }
    if (Settings.version < 0x03020C00) {  // 3.2.12 - Add parameter
      Settings.sleep = APP_SLEEP;
    }
    if (Settings.version < 0x03090300) {  // 3.9.2d - Add parameter
      SettingsDefaultSet_3_9_3();
    }
    if (Settings.version < 0x03091400) {
      strlcpy(Settings.web_password, WEB_PASSWORD, sizeof(Settings.web_password));
    }
    if (Settings.version < 0x03091500) {
      for (byte i = 0; i < MAX_SWITCHES; i++) {
        Settings.switchmode[i] = SWITCH_MODE;
      }
    }
    if (Settings.version < 0x04000400) {
      SettingsDefaultSet_4_0_4();
    }
    if (Settings.version < 0x04000500) {
      memmove(Settings.my_gp.io, Settings.my_gp.io +1, MAX_GPIO_PIN -1);  // move myio 1 byte to front
      Settings.my_gp.io[MAX_GPIO_PIN -1] = 0;  // Clear ADC0
    }
    if (Settings.version < 0x04000700) {
      for (byte i = 0; i < MAX_PWMS; i++) {
        Settings.pwm_value[i] = 0;
      }
    }
    if (Settings.version < 0x04000804) {
      SettingsDefaultSet_4_0_9();
    }
    if (Settings.version < 0x04010100) {
      SettingsDefaultSet_4_1_1();
    }
    if (Settings.version < 0x05000105) {
      Settings.flag = { 0 };
      Settings.flag.save_state = SAVE_STATE;
//      Settings.flag.button_restrict = 0;
//      Settings.flag.value_units = 0;
      Settings.flag.mqtt_enabled = MQTT_USE;
//      Settings.flag.mqtt_response = 0;
//      Settings.flag.mqtt_power_retain = 0;
//      Settings.flag.mqtt_button_retain = 0;
      Settings.flag.mqtt_switch_retain = MQTT_SWITCH_RETAIN;
      Settings.flag2.emulation = EMULATION;

      SettingsDefaultSet_5_0_2();

      Settings.save_data = SAVE_DATA;
    }
    if (Settings.version < 0x05000400) {
      Settings.hlw_kWhtotal = 0;
      RtcSettings.hlw_kWhtotal = 0;
    }
    if (Settings.version < 0x05000500) {
      strlcpy(Settings.mqtt_fulltopic, MQTT_FULLTOPIC, sizeof(Settings.mqtt_fulltopic));
    }
    if (Settings.version < 0x05000600) {
      Settings.mqtt_retry = MQTT_RETRY_SECS;
    }
    if (Settings.version < 0x05010100) {
      Settings.pulse_counter_type = 0;
      Settings.pulse_counter_debounce = 0;
      for (byte i = 0; i < MAX_COUNTERS; i++) {
        Settings.pulse_counter[i] = 0;
        RtcSettings.pulse_counter[i] = 0;
      }
    }
    if (Settings.version < 0x05010600) {
      SettingsDefaultSet_4_1_1();
    }
    if (Settings.version < 0x05010700) {
      Settings.param[P_HOLD_TIME] = KEY_HOLD_TIME;  // Default 4 seconds hold time
    }
    if (Settings.version < 0x05020000) {
      Settings.param[P_MAX_POWER_RETRY] = MAX_POWER_RETRY;
    }
    if (Settings.version < 0x05050000) {
      for (byte i = 0; i < 17; i++) {
        Settings.rf_code[i][0] = 0;
      }
      memcpy_P(Settings.rf_code[0], kDefaultRfCode, 9);
    }
    if (Settings.version < 0x05080000) {
      uint8_t cfg_wsflg = 0;
      for (byte i = 0; i < MAX_GPIO_PIN; i++) {
        if (GPIO_WS2812 == Settings.my_gp.io[i]) {
          cfg_wsflg = 1;
        }
      }
      if (!Settings.light_pixels && cfg_wsflg) {
        Settings.light_pixels = Settings.ws_pixels;
        Settings.light_color[0] = Settings.ws_red;
        Settings.light_color[1] = Settings.ws_green;
        Settings.light_color[2] = Settings.ws_blue;
        Settings.light_dimmer = Settings.ws_dimmer;
        Settings.light_correction = Settings.ws_ledtable;
        Settings.light_fade = Settings.ws_fade;
        Settings.light_speed = Settings.ws_speed;
        Settings.light_scheme = Settings.ws_scheme;
        Settings.light_width = Settings.ex_ws_width;
        Settings.light_wakeup = Settings.ws_wakeup;
      } else {
        Settings.light_pixels = WS2812_LEDS;
        Settings.light_width = 1;
      }
    }
    if (Settings.version < 0x0508000A) {
      Settings.power = Settings.ex_power;
      Settings.altitude = 0;
    }
    if (Settings.version < 0x0508000B) {
      for (byte i = 0; i < MAX_GPIO_PIN; i++) {  // Move GPIO_LEDs
        if ((Settings.my_gp.io[i] >= 25) && (Settings.my_gp.io[i] <= 32)) {  // Was GPIO_LED1
          Settings.my_gp.io[i] += 23;  // Move GPIO_LED1
        }
      }
      for (byte i = 0; i < MAX_PWMS; i++) {      // Move pwm_value and reset additional pulse_timerrs
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
      Settings.flag2.voltage_resolution = Settings.flag.voltage_resolution;
      Settings.flag2.current_resolution = 3;
      Settings.ina219_mode = 0;
    }

    Settings.version = VERSION;
    SettingsSave(1);
  }
}


