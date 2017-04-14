/*
Copyright (c) 2017 Theo Arends.  All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

- Redistributions of source code must retain the above copyright notice,
  this list of conditions and the following disclaimer.
- Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation
  and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.
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

  for (uint16_t i = 0; i < sizeof(RTCMEM); i++) hash += bytes[i]*(i+1);
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
    RTC_Save();
  }
  _rtcHash = getRtcHash();
}

boolean RTC_Valid()
{
  return (rtcMem.valid == RTC_MEM_VALID);
}

#ifdef DEBUG_THEO
void RTC_Dump()
{
  #define CFG_COLS 16
  
  char log[LOGSZ];
  uint16_t idx, maxrow, row, col;

  uint8_t *buffer = (uint8_t *) &rtcMem;
  maxrow = ((sizeof(RTCMEM)+CFG_COLS)/CFG_COLS);

  for (row = 0; row < maxrow; row++) {
    idx = row * CFG_COLS;
    snprintf_P(log, sizeof(log), PSTR("%04X:"), idx);
    for (col = 0; col < CFG_COLS; col++) {
      if (!(col%4)) snprintf_P(log, sizeof(log), PSTR("%s "), log);
      snprintf_P(log, sizeof(log), PSTR("%s %02X"), log, buffer[idx + col]);
    }
    snprintf_P(log, sizeof(log), PSTR("%s |"), log);
    for (col = 0; col < CFG_COLS; col++) {
//      if (!(col%4)) snprintf_P(log, sizeof(log), PSTR("%s "), log);
      snprintf_P(log, sizeof(log), PSTR("%s%c"), log, ((buffer[idx + col] > 0x20) && (buffer[idx + col] < 0x7F)) ? (char)buffer[idx + col] : ' ');
    }
    snprintf_P(log, sizeof(log), PSTR("%s|"), log);
    addLog(LOG_LEVEL_INFO, log);
  }
}
#endif  // DEBUG_THEO

/*********************************************************************************************\
 * Config - Flash or Spiffs
\*********************************************************************************************/

extern "C" {
#include "spi_flash.h"
}
#include "eboot_command.h"

#define SPIFFS_START        ((uint32_t)&_SPIFFS_start - 0x40200000) / SPI_FLASH_SEC_SIZE
#define SPIFFS_END          ((uint32_t)&_SPIFFS_end - 0x40200000) / SPI_FLASH_SEC_SIZE

// Version 3.x config
#define SPIFFS_CONFIG       "/cfg.ini"
#define CFG_LOCATION        SPIFFS_END - 4

uint32_t _cfgHash = 0;
int spiffsflag = 0;

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
  if (spi_flash_read(address, (uint32_t*)_buffer, FLASH_SECTOR_SIZE) == SPI_FLASH_RESULT_OK) {
    if (_buffer[2] != mode) {
      _buffer[2] = mode &3;
      noInterrupts();
      if (spi_flash_erase_sector(address / FLASH_SECTOR_SIZE) == SPI_FLASH_RESULT_OK) {
        spi_flash_write(address, (uint32_t*)_buffer, FLASH_SECTOR_SIZE);
      }
      interrupts();
      snprintf_P(log, sizeof(log), PSTR("FLSH: Set Flash Mode to %d"), (option) ? mode : ESP.getFlashChipMode());
      addLog(LOG_LEVEL_DEBUG, log);
    }
  }
  delete[] _buffer;
}

void setModuleFlashMode(byte option)
{
  uint8_t mode = 0;  // QIO - ESP8266
  if ((sysCfg.module == SONOFF_TOUCH) || (sysCfg.module == SONOFF_4CH)) mode = 3;  // DOUT - ESP8285
  setFlashMode(option, mode);
}

boolean spiffsPresent()
{
  return (SPIFFS_END - SPIFFS_START);
}

uint32_t getHash()
{
  uint32_t hash = 0;
  uint8_t *bytes = (uint8_t*)&sysCfg;

  for (uint16_t i = 0; i < sizeof(SYSCFG); i++) hash += bytes[i]*(i+1);
  return hash;
}

/*********************************************************************************************\
 * Config Save - Save parameters to Flash or Spiffs ONLY if any parameter has changed
\*********************************************************************************************/

void CFG_Save()
{
  char log[LOGSZ];

#ifndef BE_MINIMAL
  if ((getHash() != _cfgHash) && (spiffsPresent())) {
    if (!spiffsflag) {
      noInterrupts();
      if (sysCfg.saveFlag == 0) {  // Handle default and rollover
        spi_flash_erase_sector(CFG_LOCATION + (sysCfg.saveFlag &1));
        spi_flash_write((CFG_LOCATION + (sysCfg.saveFlag &1)) * SPI_FLASH_SEC_SIZE, (uint32*)&sysCfg, sizeof(SYSCFG));
      }
      sysCfg.saveFlag++;
      spi_flash_erase_sector(CFG_LOCATION + (sysCfg.saveFlag &1));
      spi_flash_write((CFG_LOCATION + (sysCfg.saveFlag &1)) * SPI_FLASH_SEC_SIZE, (uint32*)&sysCfg, sizeof(SYSCFG));
      interrupts();
      snprintf_P(log, sizeof(log), PSTR("Config: Saved configuration (%d bytes) to flash at %X and count %d"), sizeof(SYSCFG), CFG_LOCATION + (sysCfg.saveFlag &1), sysCfg.saveFlag);
      addLog(LOG_LEVEL_DEBUG, log);
    }
    _cfgHash = getHash();
  }
#endif  // BE_MINIMAL
  RTC_Save();
}

void CFG_Load()
{
  char log[LOGSZ];

  if (spiffsPresent()) {
    if (!spiffsflag) {
      struct SYSCFGH {
        unsigned long cfg_holder;
        unsigned long saveFlag;
      } _sysCfgH;

      noInterrupts();
      spi_flash_read((CFG_LOCATION) * SPI_FLASH_SEC_SIZE, (uint32*)&sysCfg, sizeof(SYSCFG));
      spi_flash_read((CFG_LOCATION + 1) * SPI_FLASH_SEC_SIZE, (uint32*)&_sysCfgH, sizeof(SYSCFGH));
      if (sysCfg.saveFlag < _sysCfgH.saveFlag)
        spi_flash_read((CFG_LOCATION + 1) * SPI_FLASH_SEC_SIZE, (uint32*)&sysCfg, sizeof(SYSCFG));
      interrupts();
      snprintf_P(log, sizeof(log), PSTR("Config: Loaded configuration from flash at %X and count %d"), CFG_LOCATION + (sysCfg.saveFlag &1), sysCfg.saveFlag);
      addLog(LOG_LEVEL_DEBUG, log);
    }
  }
  if (sysCfg.cfg_holder != CFG_HOLDER) CFG_Default();
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

  snprintf_P(log, sizeof(log), PSTR("Config: Erasing %d flash sectors"), _sectorEnd - _sectorStart);
  addLog(LOG_LEVEL_DEBUG, log);

  for (uint32_t _sector = _sectorStart; _sector < _sectorEnd; _sector++) {
    noInterrupts();
    result = spi_flash_erase_sector(_sector);
    interrupts();
    if (_serialoutput) {
      Serial.print(F("Flash: Erased sector "));
      Serial.print(_sector);
      if (result == SPI_FLASH_RESULT_OK) {
        Serial.println(F(" OK"));
      } else {
        Serial.println(F(" Error"));
      }
      delay(10);
    }
  }
}

void CFG_Dump()
{
  #define CFG_COLS 16
  
  char log[LOGSZ];
  uint16_t idx, maxrow, row, col;

  uint8_t *buffer = (uint8_t *) &sysCfg;
  maxrow = ((sizeof(SYSCFG)+CFG_COLS)/CFG_COLS);

  for (row = 0; row < maxrow; row++) {
    idx = row * CFG_COLS;
    snprintf_P(log, sizeof(log), PSTR("%04X:"), idx);
    for (col = 0; col < CFG_COLS; col++) {
      if (!(col%4)) snprintf_P(log, sizeof(log), PSTR("%s "), log);
      snprintf_P(log, sizeof(log), PSTR("%s %02X"), log, buffer[idx + col]);
    }
    snprintf_P(log, sizeof(log), PSTR("%s |"), log);
    for (col = 0; col < CFG_COLS; col++) {
//      if (!(col%4)) snprintf_P(log, sizeof(log), PSTR("%s "), log);
      snprintf_P(log, sizeof(log), PSTR("%s%c"), log, ((buffer[idx + col] > 0x20) && (buffer[idx + col] < 0x7F)) ? (char)buffer[idx + col] : ' ');
    }
    snprintf_P(log, sizeof(log), PSTR("%s|"), log);
    addLog(LOG_LEVEL_INFO, log);
  }
}

/********************************************************************************************/

void CFG_DefaultSet1()
{
  memset(&sysCfg, 0x00, sizeof(SYSCFG));

  sysCfg.cfg_holder = CFG_HOLDER;
  sysCfg.saveFlag = 0;
  sysCfg.version = VERSION;
  sysCfg.bootcount = 0;
}
  
void CFG_DefaultSet2()
{
  sysCfg.savedata = SAVE_DATA;
  sysCfg.savestate = SAVE_STATE;
  sysCfg.timezone = APP_TIMEZONE;
  strlcpy(sysCfg.otaUrl, OTA_URL, sizeof(sysCfg.otaUrl));

  sysCfg.seriallog_level = SERIAL_LOG_LEVEL;
  sysCfg.sta_active = 0;
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
  sysCfg.mqtt_button_retain = MQTT_BUTTON_RETAIN;
  sysCfg.mqtt_power_retain = MQTT_POWER_RETAIN;
  sysCfg.value_units = 0;
  sysCfg.button_restrict = 0;
  sysCfg.tele_period = TELE_PERIOD;

  sysCfg.power = APP_POWER;
  sysCfg.poweronstate = APP_POWERON_STATE;
  sysCfg.ledstate = APP_LEDSTATE;
  sysCfg.blinktime = APP_BLINKTIME;
  sysCfg.blinkcount = APP_BLINKCOUNT;
  sysCfg.sleep = APP_SLEEP;

  strlcpy(sysCfg.domoticz_in_topic, DOMOTICZ_IN_TOPIC, sizeof(sysCfg.domoticz_in_topic));
  strlcpy(sysCfg.domoticz_out_topic, DOMOTICZ_OUT_TOPIC, sizeof(sysCfg.domoticz_out_topic));
  sysCfg.domoticz_update_timer = DOMOTICZ_UPDATE_TIMER;
  for (byte i = 0; i < 4; i++) {
    sysCfg.switchmode[i] = SWITCH_MODE;
    sysCfg.domoticz_relay_idx[i] = 0;
    sysCfg.domoticz_key_idx[i] = 0;
    sysCfg.domoticz_switch_idx[i] = 0;
  }

  sysCfg.hlw_pcal = HLW_PREF_PULSE;
  sysCfg.hlw_ucal = HLW_UREF_PULSE;
  sysCfg.hlw_ical = HLW_IREF_PULSE;
  sysCfg.hlw_kWhtoday = 0;
  sysCfg.hlw_kWhyesterday = 0;
  sysCfg.hlw_kWhdoy = 0;
  sysCfg.hlw_pmin = 0;
  sysCfg.hlw_pmax = 0;
  sysCfg.hlw_umin = 0;
  sysCfg.hlw_umax = 0;
  sysCfg.hlw_imin = 0;
  sysCfg.hlw_imax = 0;
  sysCfg.hlw_mpl = 0;                              // MaxPowerLimit
  sysCfg.hlw_mplh = MAX_POWER_HOLD;
  sysCfg.hlw_mplw = MAX_POWER_WINDOW;
  sysCfg.hlw_mspl = 0;                             // MaxSafePowerLimit
  sysCfg.hlw_msplh = SAFE_POWER_HOLD;
  sysCfg.hlw_msplw = SAFE_POWER_WINDOW;
  sysCfg.hlw_mkwh = 0;                             // MaxEnergy
  sysCfg.hlw_mkwhs = 0;                            // MaxEnergyStart

  CFG_DefaultSet_3_2_4();

  strlcpy(sysCfg.friendlyname[0], FRIENDLY_NAME, sizeof(sysCfg.friendlyname[0]));
  strlcpy(sysCfg.friendlyname[1], FRIENDLY_NAME"2", sizeof(sysCfg.friendlyname[1]));
  strlcpy(sysCfg.friendlyname[2], FRIENDLY_NAME"3", sizeof(sysCfg.friendlyname[2]));
  strlcpy(sysCfg.friendlyname[3], FRIENDLY_NAME"4", sizeof(sysCfg.friendlyname[3]));

  CFG_DefaultSet_3_9_3();

  strlcpy(sysCfg.switch_topic, "0", sizeof(sysCfg.switch_topic));
  sysCfg.mqtt_switch_retain = MQTT_SWITCH_RETAIN;
  sysCfg.mqtt_enabled = MQTT_USE;

  sysCfg.emulation = EMULATION;

  strlcpy(sysCfg.web_password, WEB_PASSWORD, sizeof(sysCfg.web_password));

  CFG_DefaultSet_4_0_4();
  sysCfg.pulsetime[0] = APP_PULSETIME;

  // 4.0.7
  for (byte i = 0; i < 5; i++) sysCfg.pwmvalue[i] = 0;

  // 4.0.9
  CFG_DefaultSet_4_0_9();

  // 4.1.1
  CFG_DefaultSet_4_1_1();

}

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
  for (byte i = 0; i < 4; i++) sysCfg.domoticz_switch_idx[i] = 0;
  for (byte i = 0; i < 12; i++) sysCfg.domoticz_sensor_idx[i] = 0;

  sysCfg.module = MODULE;
  for (byte i = 0; i < MAX_GPIO_PIN; i++) sysCfg.my_module.gp.io[i] = 0;

  sysCfg.led_pixels = 0;
  for (byte i = 0; i < 5; i++) sysCfg.led_color[i] = 255;
  sysCfg.led_table = 0;
  for (byte i = 0; i < 3; i++) sysCfg.led_dimmer[i] = 10;
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
  for (byte j =0; j < 3; j++)
    for (byte i = 0; i < strlen(sysCfg.ntp_server[j]); i++)
      if (sysCfg.ntp_server[j][i] == ',') sysCfg.ntp_server[j][i] = '.';
  sysCfg.pulsetime[0] = sysCfg.ex_pulsetime;
  for (byte i = 1; i < MAX_PULSETIMERS; i++) sysCfg.pulsetime[i] = 0;
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
  sysCfg.mqtt_response = 0;
  strlcpy(sysCfg.state_text[0], MQTT_STATUS_OFF, sizeof(sysCfg.state_text[0]));
  strlcpy(sysCfg.state_text[1], MQTT_STATUS_ON, sizeof(sysCfg.state_text[1]));
  strlcpy(sysCfg.state_text[2], MQTT_CMND_TOGGLE, sizeof(sysCfg.state_text[2]));
}

void CFG_Default()
{
  addLog_P(LOG_LEVEL_NONE, PSTR("Config: Use default configuration"));
  CFG_DefaultSet1();
  CFG_DefaultSet2();
  CFG_Save();
}

/********************************************************************************************/

void CFG_Delta()
{
  if (sysCfg.version != VERSION) {      // Fix version dependent changes
    if (sysCfg.version < 0x03000600) {  // 3.0.6 - Add parameter
      sysCfg.ex_pulsetime = APP_PULSETIME;
    }
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
      sysCfg.mqtt_switch_retain = MQTT_SWITCH_RETAIN;
      sysCfg.mqtt_enabled = MQTT_USE;
    }
    if (sysCfg.version < 0x03020C00) {  // 3.2.12 - Add parameter
      sysCfg.sleep = APP_SLEEP;
    }
    if (sysCfg.version < 0x03090300) {  // 3.9.2d - Add parameter
      CFG_DefaultSet_3_9_3();
    }
    if (sysCfg.version < 0x03090700) {  // 3.9.7 - Add parameter
      sysCfg.emulation = EMULATION;
    }
    if (sysCfg.version < 0x03091400) {
      strlcpy(sysCfg.web_password, WEB_PASSWORD, sizeof(sysCfg.web_password));
    }
    if (sysCfg.version < 0x03091500) {
      for (byte i = 0; i < 4; i++) sysCfg.switchmode[i] = sysCfg.ex_switchmode;
    }
    if (sysCfg.version < 0x04000200) {
      sysCfg.button_restrict = 0;
    }
    if (sysCfg.version < 0x04000400) {
      CFG_DefaultSet_4_0_4();
    }
    if (sysCfg.version < 0x04000500) {
      memmove(sysCfg.my_module.gp.io, sysCfg.my_module.gp.io +1, MAX_GPIO_PIN -1);  // move myio 1 byte to front
      sysCfg.my_module.gp.io[MAX_GPIO_PIN -1] = 0;  // Clear ADC0
    }
    if (sysCfg.version < 0x04000700) {
      for (byte i = 0; i < 5; i++) sysCfg.pwmvalue[i] = 0;
    }
    if (sysCfg.version < 0x04000804) {
      CFG_DefaultSet_4_0_9();
    }
    if (sysCfg.version < 0x04010100) {
      CFG_DefaultSet_4_1_1();
    }
    sysCfg.version = VERSION;
  }
}


