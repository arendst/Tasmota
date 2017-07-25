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

/*********************************************************************************************\
 * RTC memory
\*********************************************************************************************/

#define RTC_MEM_VALID 0xA55A

uint32_t _rtcHash = 0;

uint32_t getRtcHash()
{
  uint32_t hash = 0;
  uint8_t *bytes = (uint8_t*)&rtcMem;

  for (uint16_t i = 0; i < sizeof(RTCMEM); i++) {
    hash += bytes[i]*(i+1);
  }
  return hash;
}

void RTC_Save()
{
  if (getRtcHash() != _rtcHash) {
    rtcMem.valid = RTC_MEM_VALID;
    ESP.rtcUserMemoryWrite(100, (uint32_t*)&rtcMem, sizeof(RTCMEM));
    _rtcHash = getRtcHash();
#ifdef DEBUG_THEO
    addLog_P(LOG_LEVEL_DEBUG, PSTR("Dump: Save"));
    RTC_Dump();
#endif  // DEBUG_THEO
  }
}

void RTC_Load()
{
  ESP.rtcUserMemoryRead(100, (uint32_t*)&rtcMem, sizeof(RTCMEM));
#ifdef DEBUG_THEO
  addLog_P(LOG_LEVEL_DEBUG, PSTR("Dump: Load"));
  RTC_Dump();
#endif  // DEBUG_THEO
  if (rtcMem.valid != RTC_MEM_VALID) {
    memset(&rtcMem, 0x00, sizeof(RTCMEM));
    rtcMem.valid = RTC_MEM_VALID;
    rtcMem.power = sysCfg.power;
    rtcMem.hlw_kWhtoday = sysCfg.hlw_kWhtoday;
    rtcMem.hlw_kWhtotal = sysCfg.hlw_kWhtotal;
    for (byte i = 0; i < 4; i++) {
      rtcMem.pCounter[i] = sysCfg.pCounter[i];
    }
    RTC_Save();
  }
  _rtcHash = getRtcHash();
}

boolean RTC_Valid()
{
  return (RTC_MEM_VALID == rtcMem.valid);
}

#ifdef DEBUG_THEO
void RTC_Dump()
{
  #define CFG_COLS 16
  
  char log[LOGSZ];
  uint16_t idx;
  uint16_t maxrow;
  uint16_t row;
  uint16_t col;

  uint8_t *buffer = (uint8_t *) &rtcMem;
  maxrow = ((sizeof(RTCMEM)+CFG_COLS)/CFG_COLS);

  for (row = 0; row < maxrow; row++) {
    idx = row * CFG_COLS;
    snprintf_P(log, sizeof(log), PSTR("%04X:"), idx);
    for (col = 0; col < CFG_COLS; col++) {
      if (!(col%4)) {
        snprintf_P(log, sizeof(log), PSTR("%s "), log);
      }
      snprintf_P(log, sizeof(log), PSTR("%s %02X"), log, buffer[idx + col]);
    }
    snprintf_P(log, sizeof(log), PSTR("%s |"), log);
    for (col = 0; col < CFG_COLS; col++) {
//      if (!(col%4)) {
//        snprintf_P(log, sizeof(log), PSTR("%s "), log);
//      }
      snprintf_P(log, sizeof(log), PSTR("%s%c"), log, ((buffer[idx + col] > 0x20) && (buffer[idx + col] < 0x7F)) ? (char)buffer[idx + col] : ' ');
    }
    snprintf_P(log, sizeof(log), PSTR("%s|"), log);
    addLog(LOG_LEVEL_INFO, log);
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
#define CFG_LOCATION_3      SPIFFS_END - 4

// Version 4.2 config = eeprom area
#define CFG_LOCATION        SPIFFS_END  // No need for SPIFFS as it uses EEPROM area
// Version 5.2 allow for more flash space
#define CFG_ROTATES         8           // Number of flash sectors used (handles uploads)

uint32_t _cfgHash = 0;
uint32_t _cfgLocation = CFG_LOCATION;

/********************************************************************************************/
/*
 * Based on cores/esp8266/Updater.cpp
 */
void setFlashMode(byte option, byte mode)
{
  char log[LOGSZ];
  uint8_t *_buffer;
  uint32_t address;

// option 0 - Use absolute address 0
// option 1 - Use OTA/Upgrade relative address

  if (option) {
    eboot_command ebcmd;
    eboot_command_read(&ebcmd);
    address = ebcmd.args[0];
  } else {
    address = 0;
  }
  _buffer = new uint8_t[FLASH_SECTOR_SIZE];
  if (SPI_FLASH_RESULT_OK == spi_flash_read(address, (uint32_t*)_buffer, FLASH_SECTOR_SIZE)) {
    if (_buffer[2] != mode) {
      _buffer[2] = mode &3;
      noInterrupts();
      if (SPI_FLASH_RESULT_OK == spi_flash_erase_sector(address / FLASH_SECTOR_SIZE)) {
        spi_flash_write(address, (uint32_t*)_buffer, FLASH_SECTOR_SIZE);
      }
      interrupts();
      snprintf_P(log, sizeof(log), PSTR("FLSH: Set Flash Mode to %d"), (option) ? mode : ESP.getFlashChipMode());
      addLog(LOG_LEVEL_DEBUG, log);
    }
  }
  delete[] _buffer;
}

uint32_t getHash()
{
  uint32_t hash = 0;
  uint8_t *bytes = (uint8_t*)&sysCfg;

  for (uint16_t i = 0; i < sizeof(SYSCFG); i++) {
    hash += bytes[i]*(i+1);
  }
  return hash;
}

/*********************************************************************************************\
 * Config Save - Save parameters to Flash ONLY if any parameter has changed
\*********************************************************************************************/

uint32_t CFG_Address()
{
  return _cfgLocation * SPI_FLASH_SEC_SIZE;
}

void CFG_Save(byte rotate)
{
/* Save configuration in eeprom or one of 7 slots below
 *  
 * rotate 0 = Save in next flash slot
 * rotate 1 = Save only in eeprom flash slot until SetOption12 0 or restart
 * rotate 2 = Save in eeprom flash slot, erase next flash slots and continue depending on stop_flash_rotate
 * stop_flash_rotate 0 = Allow flash slot rotation (SetOption12 0)
 * stop_flash_rotate 1 = Allow only eeprom flash slot use (SetOption12 1)
 */
  char log[LOGSZ];

#ifndef BE_MINIMAL
  if ((getHash() != _cfgHash) || rotate) {
    if (1 == rotate) {   // Use eeprom flash slot only and disable flash rotate from now on (upgrade)
      stop_flash_rotate = 1;
    }
    if (2 == rotate) {   // Use eeprom flash slot and erase next flash slots if stop_flash_rotate is off (default)
      _cfgLocation = CFG_LOCATION +1;
    }
    if (stop_flash_rotate) {
      _cfgLocation = CFG_LOCATION;
    } else {
      _cfgLocation--;
      if (_cfgLocation <= (CFG_LOCATION - CFG_ROTATES)) {
        _cfgLocation = CFG_LOCATION;
      }
    }
    sysCfg.saveFlag++;
    noInterrupts();
    spi_flash_erase_sector(_cfgLocation);
    spi_flash_write(_cfgLocation * SPI_FLASH_SEC_SIZE, (uint32*)&sysCfg, sizeof(SYSCFG));
    interrupts();
    if (!stop_flash_rotate && rotate) {
      for (byte i = 1; i < CFG_ROTATES; i++) {
        noInterrupts();
        spi_flash_erase_sector(_cfgLocation -i);  // Delete previous configurations by resetting to 0xFF
        interrupts();
        delay(1);
      }
    }
    snprintf_P(log, sizeof(log), PSTR("Cnfg: Save (%d bytes) to flash at %X and count %d"), sizeof(SYSCFG), _cfgLocation, sysCfg.saveFlag);
    addLog(LOG_LEVEL_DEBUG, log);
    _cfgHash = getHash();
  }
#endif  // BE_MINIMAL
  RTC_Save();
}

void CFG_Load()
{
/* Load configuration from eeprom or one of 7 slots below if first load does not stop_flash_rotate
 */
  char log[LOGSZ];

  struct SYSCFGH {
    unsigned long cfg_holder;
    unsigned long saveFlag;
  } _sysCfgH;

  _cfgLocation = CFG_LOCATION +1;
  for (byte i = 0; i < CFG_ROTATES; i++) {
    _cfgLocation--;
    noInterrupts();
    spi_flash_read(_cfgLocation * SPI_FLASH_SEC_SIZE, (uint32*)&sysCfg, sizeof(SYSCFG));
    spi_flash_read((_cfgLocation -1) * SPI_FLASH_SEC_SIZE, (uint32*)&_sysCfgH, sizeof(SYSCFGH));
    interrupts();

//  snprintf_P(log, sizeof(log), PSTR("Cnfg: Check at %X with count %d and holder %X"), _cfgLocation -1, _sysCfgH.saveFlag, _sysCfgH.cfg_holder);
//  addLog(LOG_LEVEL_DEBUG, log);

    if (((sysCfg.version > 0x05000200) && sysCfg.flag.stop_flash_rotate) || (sysCfg.cfg_holder != _sysCfgH.cfg_holder) || (sysCfg.saveFlag > _sysCfgH.saveFlag)) {
      break;
    }
    delay(1);
  }
  snprintf_P(log, sizeof(log), PSTR("Cnfg: Load from flash at %X and count %d"), _cfgLocation, sysCfg.saveFlag);
  addLog(LOG_LEVEL_DEBUG, log);
/*  
  if (sysCfg.cfg_holder != CFG_HOLDER) {
    CFG_Default();
  }
*/
  if (sysCfg.cfg_holder != CFG_HOLDER) {
/*
    // Auto upgrade
    if ((sysCfg.version < 0x04020000) || (sysCfg.version > VERSION)) {
      noInterrupts();
      spi_flash_read((CFG_LOCATION_3) * SPI_FLASH_SEC_SIZE, (uint32*)&sysCfg, sizeof(SYSCFG));
      spi_flash_read((CFG_LOCATION_3 + 1) * SPI_FLASH_SEC_SIZE, (uint32*)&_sysCfgH, sizeof(SYSCFGH));
      if (sysCfg.saveFlag < _sysCfgH.saveFlag)
        spi_flash_read((CFG_LOCATION_3 + 1) * SPI_FLASH_SEC_SIZE, (uint32*)&sysCfg, sizeof(SYSCFG));
      interrupts();
      if (sysCfg.cfg_holder != CFG_HOLDER) {
        CFG_Default();
      } else {
        sysCfg.saveFlag = 0;
      }
    } else {
      CFG_Default();
    }
*/
    // Auto upgrade
    noInterrupts();
    spi_flash_read((CFG_LOCATION_3) * SPI_FLASH_SEC_SIZE, (uint32*)&sysCfg, sizeof(SYSCFG));
    spi_flash_read((CFG_LOCATION_3 + 1) * SPI_FLASH_SEC_SIZE, (uint32*)&_sysCfgH, sizeof(SYSCFGH));
    if (sysCfg.saveFlag < _sysCfgH.saveFlag)
      spi_flash_read((CFG_LOCATION_3 + 1) * SPI_FLASH_SEC_SIZE, (uint32*)&sysCfg, sizeof(SYSCFG));
    interrupts();
    if ((sysCfg.cfg_holder != CFG_HOLDER) || (sysCfg.version >= 0x04020000)) {
      CFG_Default();
    }
  }
  
  _cfgHash = getHash();

  RTC_Load();
}

void CFG_Erase()
{
  char log[LOGSZ];
  SpiFlashOpResult result;

  uint32_t _sectorStart = (ESP.getSketchSize() / SPI_FLASH_SEC_SIZE) + 1;
  uint32_t _sectorEnd = ESP.getFlashChipRealSize() / SPI_FLASH_SEC_SIZE;
  boolean _serialoutput = (LOG_LEVEL_DEBUG_MORE <= seriallog_level);

  snprintf_P(log, sizeof(log), PSTR("Cnfg: Erase %d flash sectors"), _sectorEnd - _sectorStart);
  addLog(LOG_LEVEL_DEBUG, log);

  for (uint32_t _sector = _sectorStart; _sector < _sectorEnd; _sector++) {
    noInterrupts();
    result = spi_flash_erase_sector(_sector);
    interrupts();
    if (_serialoutput) {
      Serial.print(F("Flash: Erased sector "));
      Serial.print(_sector);
      if (SPI_FLASH_RESULT_OK == result) {
        Serial.println(F(" OK"));
      } else {
        Serial.println(F(" Error"));
      }
      delay(10);
    }
  }
}

void CFG_Dump(uint16_t srow, uint16_t mrow)
{
  #define CFG_COLS 16
  
  char log[LOGSZ];
  uint16_t idx;
  uint16_t maxrow;
  uint16_t row;
  uint16_t col;

  uint8_t *buffer = (uint8_t *) &sysCfg;
  row = 0;
  maxrow = ((sizeof(SYSCFG)+CFG_COLS)/CFG_COLS);
  if ((srow > 0) && (srow < maxrow)) {
    row = srow;
  }
  if (0 == mrow) {  // Default only four lines
    mrow = 4;
  }
  if ((mrow > 0) && (mrow < (maxrow - row))) {
    maxrow = row + mrow;
  }

  for (row = srow; row < maxrow; row++) {
    idx = row * CFG_COLS;
    snprintf_P(log, sizeof(log), PSTR("%04X:"), idx);
    for (col = 0; col < CFG_COLS; col++) {
      if (!(col%4)) {
        snprintf_P(log, sizeof(log), PSTR("%s "), log);
      }
      snprintf_P(log, sizeof(log), PSTR("%s %02X"), log, buffer[idx + col]);
    }
    snprintf_P(log, sizeof(log), PSTR("%s |"), log);
    for (col = 0; col < CFG_COLS; col++) {
//      if (!(col%4)) {
//        snprintf_P(log, sizeof(log), PSTR("%s "), log);
//      }
      snprintf_P(log, sizeof(log), PSTR("%s%c"), log, ((buffer[idx + col] > 0x20) && (buffer[idx + col] < 0x7F)) ? (char)buffer[idx + col] : ' ');
    }
    snprintf_P(log, sizeof(log), PSTR("%s|"), log);
    addLog(LOG_LEVEL_INFO, log);
  }
}

/********************************************************************************************/

void CFG_Default()
{
  addLog_P(LOG_LEVEL_NONE, PSTR("Cnfg: Use defaults"));
  CFG_DefaultSet1();
  CFG_DefaultSet2();
  CFG_Save(2);
}

void CFG_DefaultSet1()
{
  memset(&sysCfg, 0x00, sizeof(SYSCFG));

  sysCfg.cfg_holder = CFG_HOLDER;
//  sysCfg.saveFlag = 0;
  sysCfg.version = VERSION;
//  sysCfg.bootcount = 0;
}
  
void CFG_DefaultSet2()
{
  memset((char*)&sysCfg +16, 0x00, sizeof(SYSCFG) -16);
  
  sysCfg.flag.savestate = SAVE_STATE;
  sysCfg.savedata = SAVE_DATA;
  sysCfg.timezone = APP_TIMEZONE;
  strlcpy(sysCfg.otaUrl, OTA_URL, sizeof(sysCfg.otaUrl));

  sysCfg.seriallog_level = SERIAL_LOG_LEVEL;
//  sysCfg.sta_active = 0;
  strlcpy(sysCfg.sta_ssid[0], STA_SSID1, sizeof(sysCfg.sta_ssid[0]));
  strlcpy(sysCfg.sta_pwd[0], STA_PASS1, sizeof(sysCfg.sta_pwd[0]));
  strlcpy(sysCfg.sta_ssid[1], STA_SSID2, sizeof(sysCfg.sta_ssid[1]));
  strlcpy(sysCfg.sta_pwd[1], STA_PASS2, sizeof(sysCfg.sta_pwd[1]));
  strlcpy(sysCfg.hostname, WIFI_HOSTNAME, sizeof(sysCfg.hostname));
  sysCfg.sta_config = WIFI_CONFIG_TOOL;
  strlcpy(sysCfg.syslog_host, SYS_LOG_HOST, sizeof(sysCfg.syslog_host));
  sysCfg.syslog_port = SYS_LOG_PORT;
  sysCfg.syslog_level = SYS_LOG_LEVEL;
  sysCfg.webserver = WEB_SERVER;
  sysCfg.weblog_level = WEB_LOG_LEVEL;

  strlcpy(sysCfg.mqtt_fingerprint, MQTT_FINGERPRINT, sizeof(sysCfg.mqtt_fingerprint));
  strlcpy(sysCfg.mqtt_host, MQTT_HOST, sizeof(sysCfg.mqtt_host));
  sysCfg.mqtt_port = MQTT_PORT;
  strlcpy(sysCfg.mqtt_client, MQTT_CLIENT_ID, sizeof(sysCfg.mqtt_client));
  strlcpy(sysCfg.mqtt_user, MQTT_USER, sizeof(sysCfg.mqtt_user));
  strlcpy(sysCfg.mqtt_pwd, MQTT_PASS, sizeof(sysCfg.mqtt_pwd));
  strlcpy(sysCfg.mqtt_topic, MQTT_TOPIC, sizeof(sysCfg.mqtt_topic));
  strlcpy(sysCfg.button_topic, "0", sizeof(sysCfg.button_topic));
  strlcpy(sysCfg.mqtt_grptopic, MQTT_GRPTOPIC, sizeof(sysCfg.mqtt_grptopic));
  sysCfg.flag.mqtt_button_retain = MQTT_BUTTON_RETAIN;
  sysCfg.flag.mqtt_power_retain = MQTT_POWER_RETAIN;
//  sysCfg.flag.value_units = 0;
//  sysCfg.flag.button_restrict = 0;
  sysCfg.tele_period = TELE_PERIOD;

  sysCfg.power = APP_POWER;
  sysCfg.poweronstate = APP_POWERON_STATE;
  sysCfg.ledstate = APP_LEDSTATE;
  sysCfg.blinktime = APP_BLINKTIME;
  sysCfg.blinkcount = APP_BLINKCOUNT;
  sysCfg.sleep = APP_SLEEP;

  sysCfg.domoticz_update_timer = DOMOTICZ_UPDATE_TIMER;
  for (byte i = 0; i < 4; i++) {
    sysCfg.switchmode[i] = SWITCH_MODE;
//    sysCfg.domoticz_relay_idx[i] = 0;
//    sysCfg.domoticz_key_idx[i] = 0;
//    sysCfg.domoticz_switch_idx[i] = 0;
  }

  sysCfg.hlw_pcal = HLW_PREF_PULSE;
  sysCfg.hlw_ucal = HLW_UREF_PULSE;
  sysCfg.hlw_ical = HLW_IREF_PULSE;
//  sysCfg.hlw_kWhtoday = 0;
//  sysCfg.hlw_kWhyesterday = 0;
//  sysCfg.hlw_kWhdoy = 0;
//  sysCfg.hlw_pmin = 0;
//  sysCfg.hlw_pmax = 0;
//  sysCfg.hlw_umin = 0;
//  sysCfg.hlw_umax = 0;
//  sysCfg.hlw_imin = 0;
//  sysCfg.hlw_imax = 0;
//  sysCfg.hlw_mpl = 0;                              // MaxPowerLimit
  sysCfg.hlw_mplh = MAX_POWER_HOLD;
  sysCfg.hlw_mplw = MAX_POWER_WINDOW;
//  sysCfg.hlw_mspl = 0;                             // MaxSafePowerLimit
  sysCfg.hlw_msplh = SAFE_POWER_HOLD;
  sysCfg.hlw_msplw = SAFE_POWER_WINDOW;
//  sysCfg.hlw_mkwh = 0;                             // MaxEnergy
//  sysCfg.hlw_mkwhs = 0;                            // MaxEnergyStart

  CFG_DefaultSet_3_2_4();

  strlcpy(sysCfg.friendlyname[0], FRIENDLY_NAME, sizeof(sysCfg.friendlyname[0]));
  strlcpy(sysCfg.friendlyname[1], FRIENDLY_NAME"2", sizeof(sysCfg.friendlyname[1]));
  strlcpy(sysCfg.friendlyname[2], FRIENDLY_NAME"3", sizeof(sysCfg.friendlyname[2]));
  strlcpy(sysCfg.friendlyname[3], FRIENDLY_NAME"4", sizeof(sysCfg.friendlyname[3]));

  CFG_DefaultSet_3_9_3();

  strlcpy(sysCfg.switch_topic, "0", sizeof(sysCfg.switch_topic));
  sysCfg.flag.mqtt_switch_retain = MQTT_SWITCH_RETAIN;
  sysCfg.flag.mqtt_enabled = MQTT_USE;

  sysCfg.flag.emulation = EMULATION;

  strlcpy(sysCfg.web_password, WEB_PASSWORD, sizeof(sysCfg.web_password));

  CFG_DefaultSet_4_0_4();
  sysCfg.pulsetime[0] = APP_PULSETIME;

  // 4.0.7
//  for (byte i = 0; i < 5; i++) sysCfg.pwmvalue[i] = 0;

  // 4.0.9
  CFG_DefaultSet_4_0_9();

  // 4.1.1 + 5.1.6
  CFG_DefaultSet_4_1_1();

  // 5.0.2
  CFG_DefaultSet_5_0_2();

  // 5.0.4
//  sysCfg.hlw_kWhtotal = 0;
  rtcMem.hlw_kWhtotal = 0;

  // 5.0.5
  strlcpy(sysCfg.mqtt_fulltopic, MQTT_FULLTOPIC, sizeof(sysCfg.mqtt_fulltopic));

  // 5.0.6
  sysCfg.mqtt_retry = MQTT_RETRY_SECS;

  // 5.1.7
  sysCfg.param[P_HOLD_TIME] = KEY_HOLD_TIME;  // Default 4 seconds hold time

  // 5.2.0
  sysCfg.param[P_MAX_POWER_RETRY] = MAX_POWER_RETRY;

}

/********************************************************************************************/

void CFG_DefaultSet_3_2_4()
{
  sysCfg.ws_pixels = WS2812_LEDS;
  sysCfg.ws_red = 255;
  sysCfg.ws_green = 0;
  sysCfg.ws_blue = 0;
  sysCfg.ws_ledtable = 0;
  sysCfg.ws_dimmer = 8;
  sysCfg.ws_fade = 0;
  sysCfg.ws_speed = 1;
  sysCfg.ws_scheme = 0;
  sysCfg.ws_width = 1;
  sysCfg.ws_wakeup = 0;
}

void CFG_DefaultSet_3_9_3()
{
  for (byte i = 0; i < 4; i++) {
    sysCfg.domoticz_switch_idx[i] = 0;
  }
  for (byte i = 0; i < 12; i++) {
    sysCfg.domoticz_sensor_idx[i] = 0;
  }

  sysCfg.module = MODULE;
  for (byte i = 0; i < MAX_GPIO_PIN; i++){
    sysCfg.my_module.gp.io[i] = 0;
  }

  sysCfg.led_pixels = 0;
  for (byte i = 0; i < 5; i++) {
    sysCfg.led_color[i] = 255;
  }
  sysCfg.led_table = 0;
  for (byte i = 0; i < 3; i++){
    sysCfg.led_dimmer[i] = 10;
  }
  sysCfg.led_fade = 0;
  sysCfg.led_speed = 0;
  sysCfg.led_scheme = 0;
  sysCfg.led_width = 0;
  sysCfg.led_wakeup = 0;
}

void CFG_DefaultSet_4_0_4()
{
  strlcpy(sysCfg.ntp_server[0], NTP_SERVER1, sizeof(sysCfg.ntp_server[0]));
  strlcpy(sysCfg.ntp_server[1], NTP_SERVER2, sizeof(sysCfg.ntp_server[1]));
  strlcpy(sysCfg.ntp_server[2], NTP_SERVER3, sizeof(sysCfg.ntp_server[2]));
  for (byte j =0; j < 3; j++) {
    for (byte i = 0; i < strlen(sysCfg.ntp_server[j]); i++) {
      if (sysCfg.ntp_server[j][i] == ',') {
        sysCfg.ntp_server[j][i] = '.';
      }
    }
  }
  sysCfg.pulsetime[0] = APP_PULSETIME;
  for (byte i = 1; i < MAX_PULSETIMERS; i++) {
    sysCfg.pulsetime[i] = 0;
  }
}

void CFG_DefaultSet_4_0_9()
{
  strlcpy(sysCfg.mqtt_prefix[0], SUB_PREFIX, sizeof(sysCfg.mqtt_prefix[0]));
  strlcpy(sysCfg.mqtt_prefix[1], PUB_PREFIX, sizeof(sysCfg.mqtt_prefix[1]));
  strlcpy(sysCfg.mqtt_prefix[2], PUB_PREFIX2, sizeof(sysCfg.mqtt_prefix[2]));
  parseIP(&sysCfg.ip_address[0], WIFI_IP_ADDRESS);
  parseIP(&sysCfg.ip_address[1], WIFI_GATEWAY);
  parseIP(&sysCfg.ip_address[2], WIFI_SUBNETMASK);
  parseIP(&sysCfg.ip_address[3], WIFI_DNS);
}

void CFG_DefaultSet_4_1_1()
{
  strlcpy(sysCfg.state_text[0], MQTT_STATUS_OFF, sizeof(sysCfg.state_text[0]));
  strlcpy(sysCfg.state_text[1], MQTT_STATUS_ON, sizeof(sysCfg.state_text[1]));
  strlcpy(sysCfg.state_text[2], MQTT_CMND_TOGGLE, sizeof(sysCfg.state_text[2]));
  strlcpy(sysCfg.state_text[3], MQTT_CMND_HOLD, sizeof(sysCfg.state_text[3]));  // v5.1.6
}

void CFG_DefaultSet_5_0_2()
{
  sysCfg.flag.temperature_conversion = TEMP_CONVERSION;
  sysCfg.flag.temperature_resolution = TEMP_RESOLUTION;
  sysCfg.flag.humidity_resolution = HUMIDITY_RESOLUTION;
  sysCfg.flag.pressure_resolution = PRESSURE_RESOLUTION;
  sysCfg.flag.energy_resolution = ENERGY_RESOLUTION;
}

/********************************************************************************************/

void CFG_Delta()
{
  if (sysCfg.version != VERSION) {      // Fix version dependent changes
    if (sysCfg.version < 0x03010200) {  // 3.1.2 - Add parameter
      sysCfg.poweronstate = APP_POWERON_STATE;
    }
    if (sysCfg.version < 0x03010600) {  // 3.1.6 - Add parameter
      sysCfg.blinktime = APP_BLINKTIME;
      sysCfg.blinkcount = APP_BLINKCOUNT;
    }
    if (sysCfg.version < 0x03020400) {  // 3.2.4 - Add parameter
      CFG_DefaultSet_3_2_4();
    }
    if (sysCfg.version < 0x03020500) {  // 3.2.5 - Add parameter
      getClient(sysCfg.friendlyname[0], sysCfg.mqtt_client, sizeof(sysCfg.friendlyname[0]));
      strlcpy(sysCfg.friendlyname[1], FRIENDLY_NAME"2", sizeof(sysCfg.friendlyname[1]));
      strlcpy(sysCfg.friendlyname[2], FRIENDLY_NAME"3", sizeof(sysCfg.friendlyname[2]));
      strlcpy(sysCfg.friendlyname[3], FRIENDLY_NAME"4", sizeof(sysCfg.friendlyname[3]));
    }      
    if (sysCfg.version < 0x03020800) {  // 3.2.8 - Add parameter
      strlcpy(sysCfg.switch_topic, sysCfg.button_topic, sizeof(sysCfg.switch_topic));
      sysCfg.ex_mqtt_switch_retain = MQTT_SWITCH_RETAIN;
      sysCfg.ex_mqtt_enabled = MQTT_USE;
    }
    if (sysCfg.version < 0x03020C00) {  // 3.2.12 - Add parameter
      sysCfg.sleep = APP_SLEEP;
    }
    if (sysCfg.version < 0x03090300) {  // 3.9.2d - Add parameter
      CFG_DefaultSet_3_9_3();
    }
    if (sysCfg.version < 0x03090700) {  // 3.9.7 - Add parameter
      sysCfg.ex_emulation = EMULATION;
    }
    if (sysCfg.version < 0x03091400) {
      strlcpy(sysCfg.web_password, WEB_PASSWORD, sizeof(sysCfg.web_password));
    }
    if (sysCfg.version < 0x03091500) {
      for (byte i = 0; i < 4; i++) sysCfg.switchmode[i] = SWITCH_MODE;
    }
    if (sysCfg.version < 0x04000200) {
      sysCfg.ex_button_restrict = 0;
    }
    if (sysCfg.version < 0x04000400) {
      CFG_DefaultSet_4_0_4();
    }
    if (sysCfg.version < 0x04000500) {
      memmove(sysCfg.my_module.gp.io, sysCfg.my_module.gp.io +1, MAX_GPIO_PIN -1);  // move myio 1 byte to front
      sysCfg.my_module.gp.io[MAX_GPIO_PIN -1] = 0;  // Clear ADC0
    }
    if (sysCfg.version < 0x04000700) {
      for (byte i = 0; i < 5; i++) {
        sysCfg.pwmvalue[i] = 0;
      }
    }
    if (sysCfg.version < 0x04000804) {
      CFG_DefaultSet_4_0_9();
    }
    if (sysCfg.version < 0x04010100) {
      CFG_DefaultSet_4_1_1();
    }
    if (sysCfg.version < 0x05000105) {
      sysCfg.flag = { 0 };
      sysCfg.flag.savestate = SAVE_STATE;
      sysCfg.flag.button_restrict = sysCfg.ex_button_restrict;
      sysCfg.flag.value_units = sysCfg.ex_value_units;
      sysCfg.flag.mqtt_enabled = sysCfg.ex_mqtt_enabled;
//      sysCfg.flag.mqtt_response = 0;
      sysCfg.flag.mqtt_power_retain = sysCfg.ex_mqtt_power_retain;
      sysCfg.flag.mqtt_button_retain = sysCfg.ex_mqtt_button_retain;
      sysCfg.flag.mqtt_switch_retain = sysCfg.ex_mqtt_switch_retain;
      sysCfg.flag.emulation = sysCfg.ex_emulation;

      CFG_DefaultSet_5_0_2();

      sysCfg.savedata = SAVE_DATA;
    }
    if (sysCfg.version < 0x05000400) {
      sysCfg.hlw_kWhtotal = 0;
      rtcMem.hlw_kWhtotal = 0;
    }
    if (sysCfg.version < 0x05000500) {
      strlcpy(sysCfg.mqtt_fulltopic, MQTT_FULLTOPIC, sizeof(sysCfg.mqtt_fulltopic));
    }
    if (sysCfg.version < 0x05000600) {
      sysCfg.mqtt_retry = MQTT_RETRY_SECS;
    }
    if (sysCfg.version < 0x05010100) {
      sysCfg.pCounterType = 0;
      sysCfg.pCounterDebounce = 0;
      for (byte i = 0; i < MAX_COUNTERS; i++) {
        sysCfg.pCounter[i] = 0;
        rtcMem.pCounter[i] = 0;
      }
    }
    if (sysCfg.version < 0x05010600) {
      if (sysCfg.version > 0x04010100) {
        memcpy(sysCfg.state_text, sysCfg.ex_state_text, 33);
      }
      strlcpy(sysCfg.state_text[3], MQTT_CMND_HOLD, sizeof(sysCfg.state_text[3]));
    }
    if (sysCfg.version < 0x05010700) {
      sysCfg.param[P_HOLD_TIME] = KEY_HOLD_TIME;  // Default 4 seconds hold time
    }
    if (sysCfg.version < 0x05020000) {
      sysCfg.param[P_MAX_POWER_RETRY] = MAX_POWER_RETRY;
    }
    
    sysCfg.version = VERSION;
    CFG_Save(1);
  }
}


