/*
  support.ino - support for Sonoff-Tasmota

  Copyright (C) 2018  Theo Arends

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

IPAddress syslog_host_addr;  // Syslog host IP address
unsigned long syslog_host_refresh = 0;

/*********************************************************************************************\
 * Watchdog extension (https://github.com/esp8266/Arduino/issues/1532)
\*********************************************************************************************/

Ticker tickerOSWatch;

#define OSWATCH_RESET_TIME 120

static unsigned long oswatch_last_loop_time;
byte oswatch_blocked_loop = 0;

#ifndef USE_WS2812_DMA  // Collides with Neopixelbus but solves exception
//void OsWatchTicker() ICACHE_RAM_ATTR;
#endif  // USE_WS2812_DMA

void OsWatchTicker()
{
  unsigned long t = millis();
  unsigned long last_run = abs(t - oswatch_last_loop_time);

#ifdef DEBUG_THEO
  snprintf_P(log_data, sizeof(log_data), PSTR(D_LOG_APPLICATION D_OSWATCH " FreeRam %d, rssi %d, last_run %d"), ESP.getFreeHeap(), WifiGetRssiAsQuality(WiFi.RSSI()), last_run);
  AddLog(LOG_LEVEL_DEBUG);
#endif  // DEBUG_THEO
  if (last_run >= (OSWATCH_RESET_TIME * 1000)) {
//    AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_APPLICATION D_OSWATCH " " D_BLOCKED_LOOP ". " D_RESTARTING));  // Save iram space
    RtcSettings.oswatch_blocked_loop = 1;
    RtcSettingsSave();
//    ESP.restart();  // normal reboot
    ESP.reset();  // hard reset
  }
}

void OsWatchInit()
{
  oswatch_blocked_loop = RtcSettings.oswatch_blocked_loop;
  RtcSettings.oswatch_blocked_loop = 0;
  oswatch_last_loop_time = millis();
  tickerOSWatch.attach_ms(((OSWATCH_RESET_TIME / 3) * 1000), OsWatchTicker);
}

void OsWatchLoop()
{
  oswatch_last_loop_time = millis();
//  while(1) delay(1000);  // this will trigger the os watch
}

String GetResetReason()
{
  char buff[32];
  if (oswatch_blocked_loop) {
    strncpy_P(buff, PSTR(D_JSON_BLOCKED_LOOP), sizeof(buff));
    return String(buff);
  } else {
    return ESP.getResetReason();
  }
}

#ifdef DEBUG_THEO
void ExceptionTest(byte type)
{
/*
Exception (28):
epc1=0x4000bf64 epc2=0x00000000 epc3=0x00000000 excvaddr=0x00000007 depc=0x00000000

ctx: cont
sp: 3fff1f30 end: 3fff2840 offset: 01a0

>>>stack>>>
3fff20d0:  202c3573 756f7247 2c302070 646e4920
3fff20e0:  40236a6e 7954202c 45206570 00454358
3fff20f0:  00000010 00000007 00000000 3fff2180
3fff2100:  3fff2190 40107bfc 3fff3e4c 3fff22c0
3fff2110:  40261934 000000f0 3fff22c0 401004d8
3fff2120:  40238fcf 00000050 3fff2100 4021fc10
3fff2130:  3fff32bc 4021680c 3ffeade1 4021ff7d
3fff2140:  3fff2190 3fff2180 0000000c 7fffffff
3fff2150:  00000019 00000000 00000000 3fff21c0
3fff2160:  3fff23f3 3ffe8e08 00000000 4021ffb4
3fff2170:  3fff2190 3fff2180 0000000c 40201118
3fff2180:  3fff21c0 0000003c 3ffef840 00000007
3fff2190:  00000000 00000000 00000000 40201128
3fff21a0:  3fff23f3 000000f1 3fff23ec 4020fafb
3fff21b0:  3fff23f3 3fff21c0 3fff21d0 3fff23f6
3fff21c0:  00000000 3fff23fb 4022321b 00000000

Exception 28: LoadProhibited: A load referenced a page mapped with an attribute that does not permit loads
Decoding 14 results
0x40236a6e: ets_vsnprintf at ?? line ?
0x40107bfc: vsnprintf at C:\Data2\Arduino\arduino-1.8.1-esp-2.3.0\portable\packages\esp8266\hardware\esp8266\2.3.0\cores\esp8266/libc_replacements.c line 387
0x40261934: bignum_exptmod at ?? line ?
0x401004d8: malloc at C:\Data2\Arduino\arduino-1.8.1-esp-2.3.0\portable\packages\esp8266\hardware\esp8266\2.3.0\cores\esp8266\umm_malloc/umm_malloc.c line 1664
0x40238fcf: wifi_station_get_connect_status at ?? line ?
0x4021fc10: operator new[](unsigned int) at C:\Data2\Arduino\arduino-1.8.1-esp-2.3.0\portable\packages\esp8266\hardware\esp8266\2.3.0\cores\esp8266/abi.cpp line 57
0x4021680c: ESP8266WiFiSTAClass::status() at C:\Data2\Arduino\arduino-1.8.1-esp-2.3.0\portable\packages\esp8266\hardware\esp8266\2.3.0\libraries\ESP8266WiFi\src/ESP8266WiFiSTA.cpp line 569
0x4021ff7d: vsnprintf_P(char*, unsigned int, char const*, __va_list_tag) at C:\Data2\Arduino\arduino-1.8.1-esp-2.3.0\portable\packages\esp8266\hardware\esp8266\2.3.0\cores\esp8266/pgmspace.cpp line 146
0x4021ffb4: snprintf_P(char*, unsigned int, char const*, ...) at C:\Data2\Arduino\arduino-1.8.1-esp-2.3.0\portable\packages\esp8266\hardware\esp8266\2.3.0\cores\esp8266/pgmspace.cpp line 146
0x40201118: atol at C:\Data2\Arduino\arduino-1.8.1-esp-2.3.0\portable\packages\esp8266\hardware\esp8266\2.3.0\cores\esp8266/core_esp8266_noniso.c line 45
0x40201128: atoi at C:\Data2\Arduino\arduino-1.8.1-esp-2.3.0\portable\packages\esp8266\hardware\esp8266\2.3.0\cores\esp8266/core_esp8266_noniso.c line 45
0x4020fafb: MqttDataCallback(char*, unsigned char*, unsigned int) at R:\Arduino\Work-ESP8266\Theo\sonoff\sonoff-4\sonoff/sonoff.ino line 679 (discriminator 1)
0x4022321b: pp_attach at ?? line ?

00:00:08 MQTT: tele/sonoff/INFO3 = {"Started":"Fatal exception:28 flag:2 (EXCEPTION) epc1:0x4000bf64 epc2:0x00000000 epc3:0x00000000 excvaddr:0x00000007 depc:0x00000000"}
*/
  if (1 == type) {
    char svalue[10];
    snprintf_P(svalue, sizeof(svalue), PSTR("%s"), 7);  // Exception 28 as number in string (7 in excvaddr)
  }
/*
14:50:52 osWatch: FreeRam 25896, rssi 68, last_run 0
14:51:02 osWatch: FreeRam 25896, rssi 58, last_run 0
14:51:03 CMND: exception 2
14:51:12 osWatch: FreeRam 25360, rssi 60, last_run 8771
14:51:22 osWatch: FreeRam 25360, rssi 62, last_run 18771
14:51:32 osWatch: FreeRam 25360, rssi 62, last_run 28771
14:51:42 osWatch: FreeRam 25360, rssi 62, last_run 38771
14:51:42 osWatch: Warning, loop blocked. Restart now
*/
  if (2 == type) {
    while(1) delay(1000);  // this will trigger the os watch
  }
}
#endif  // DEBUG_THEO

/*********************************************************************************************\
 * Miscellaneous
\*********************************************************************************************/

#ifdef ARDUINO_ESP8266_RELEASE_2_3_0
// Functions not available in 2.3.0

// http://clc-wiki.net/wiki/C_standard_library:string.h:memchr
void* memchr(const void* ptr, int value, size_t num)
{
  unsigned char *p = (unsigned char*)ptr;
  while (num--) {
    if (*p != (unsigned char)value) {
      p++;
    } else {
      return p;
    }
  }
  return 0;
}

// http://clc-wiki.net/wiki/C_standard_library:string.h:strspn
// Get span until any character in string
size_t strcspn(const char *str1, const char *str2)
{
  size_t ret = 0;
  while (*str1) {
    if (strchr(str2, *str1)) {  // Slow
      return ret;
    } else {
      str1++;
      ret++;
    }
  }
  return ret;
}
#endif  // ARDUINO_ESP8266_RELEASE_2_3_0

// Get span until single character in string
size_t strchrspn(const char *str1, int character)
{
  size_t ret = 0;
  char *start = (char*)str1;
  char *end = strchr(str1, character);
  if (end) ret = end - start;
  return ret;
}

char* dtostrfd(double number, unsigned char prec, char *s)
{
  return dtostrf(number, 1, prec, s);
}

boolean ParseIp(uint32_t* addr, const char* str)
{
  uint8_t *part = (uint8_t*)addr;
  byte i;

  *addr = 0;
  for (i = 0; i < 4; i++) {
    part[i] = strtoul(str, NULL, 10);        // Convert byte
    str = strchr(str, '.');
    if (str == NULL || *str == '\0') {
      break;  // No more separators, exit
    }
    str++;                                   // Point to next character after separator
  }
  return (3 == i);
}

void MakeValidMqtt(byte option, char* str)
{
// option 0 = replace by underscore
// option 1 = delete character
  uint16_t i = 0;
  while (str[i] > 0) {
//        if ((str[i] == '/') || (str[i] == '+') || (str[i] == '#') || (str[i] == ' ')) {
    if ((str[i] == '+') || (str[i] == '#') || (str[i] == ' ')) {
      if (option) {
        uint16_t j = i;
        while (str[j] > 0) {
          str[j] = str[j +1];
          j++;
        }
        i--;
      } else {
        str[i] = '_';
      }
    }
    i++;
  }
}

// Function to parse & check if version_str is newer than our currently installed version.
bool NewerVersion(char* version_str)
{
  uint32_t version = 0;
  uint8_t i = 0;
  char *str_ptr;
  char* version_dup = strdup(version_str);  // Duplicate the version_str as strtok_r will modify it.

  if (!version_dup) {
    return false;  // Bail if we can't duplicate. Assume bad.
  }
  // Loop through the version string, splitting on '.' seperators.
  for (char *str = strtok_r(version_dup, ".", &str_ptr); str && i < sizeof(VERSION); str = strtok_r(NULL, ".", &str_ptr), i++) {
    int field = atoi(str);
    // The fields in a version string can only range from 0-255.
    if ((field < 0) || (field > 255)) {
      free(version_dup);
      return false;
    }
    // Shuffle the accumulated bytes across, and add the new byte.
    version = (version << 8) + field;
    // Check alpha delimiter after 1.2.3 only
    if ((2 == i) && isalpha(str[strlen(str)-1])) {
      field = str[strlen(str)-1] & 0x1f;
      version = (version << 8) + field;
      i++;
    }
  }
  free(version_dup);  // We no longer need this.
  // A version string should have 2-4 fields. e.g. 1.2, 1.2.3, or 1.2.3a (= 1.2.3.1).
  // If not, then don't consider it a valid version string.
  if ((i < 2) || (i > sizeof(VERSION))) {
    return false;
  }
  // Keep shifting the parsed version until we hit the maximum number of tokens.
  // VERSION stores the major number of the version in the most significant byte of the uint32_t.
  while (i < sizeof(VERSION)) {
    version <<= 8;
    i++;
  }
  // Now we should have a fully constructed version number in uint32_t form.
  return (version > VERSION);
}

char* GetPowerDevice(char* dest, uint8_t idx, size_t size, uint8_t option)
{
  char sidx[8];

  strncpy_P(dest, S_RSLT_POWER, size);
  if ((devices_present + option) > 1) {
    snprintf_P(sidx, sizeof(sidx), PSTR("%d"), idx);
    strncat(dest, sidx, size);
  }
  return dest;
}

char* GetPowerDevice(char* dest, uint8_t idx, size_t size)
{
  return GetPowerDevice(dest, idx, size, 0);
}

float ConvertTemp(float c)
{
  float result = c;

  if (!isnan(c) && Settings.flag.temperature_conversion) {
    result = c * 1.8 + 32;  // Fahrenheit
  }
  return result;
}

char TempUnit()
{
  return (Settings.flag.temperature_conversion) ? 'F' : 'C';
}

double FastPrecisePow(double a, double b)
{
  // https://martin.ankerl.com/2012/01/25/optimized-approximative-pow-in-c-and-cpp/
  // calculate approximation with fraction of the exponent
  int e = (int)b;
  union {
    double d;
    int x[2];
  } u = { a };
  u.x[1] = (int)((b - e) * (u.x[1] - 1072632447) + 1072632447);
  u.x[0] = 0;
  // exponentiation by squaring with the exponent's integer part
  // double r = u.d makes everything much slower, not sure why
  double r = 1.0;
  while (e) {
    if (e & 1) {
      r *= a;
    }
    a *= a;
    e >>= 1;
  }
  return r * u.d;
}

char* GetTextIndexed(char* destination, size_t destination_size, uint16_t index, const char* haystack)
{
  // Returns empty string if not found
  // Returns text of found
  char* write = destination;
  const char* read = haystack;

  index++;
  while (index--) {
    size_t size = destination_size -1;
    write = destination;
    char ch = '.';
    while ((ch != '\0') && (ch != '|')) {
      ch = pgm_read_byte(read++);
      if (size && (ch != '|'))  {
        *write++ = ch;
        size--;
      }
    }
    if (0 == ch) {
      if (index) {
        write = destination;
      }
      break;
    }
  }
  *write = '\0';
  return destination;
}

int GetCommandCode(char* destination, size_t destination_size, const char* needle, const char* haystack)
{
  // Returns -1 of not found
  // Returns index and command if found
  int result = -1;
  const char* read = haystack;
  char* write = destination;

  while (true) {
    result++;
    size_t size = destination_size -1;
    write = destination;
    char ch = '.';
    while ((ch != '\0') && (ch != '|')) {
      ch = pgm_read_byte(read++);
      if (size && (ch != '|'))  {
        *write++ = ch;
        size--;
      }
    }
    *write = '\0';
    if (!strcasecmp(needle, destination)) {
      break;
    }
    if (0 == ch) {
      result = -1;
      break;
    }
  }
  return result;
}

void SetSerialBaudrate(int baudrate)
{
  if (Serial.baudRate() != baudrate) {
    if (seriallog_level) {
      snprintf_P(log_data, sizeof(log_data), PSTR(D_LOG_APPLICATION D_SET_BAUDRATE_TO " %d"), baudrate);
      AddLog(LOG_LEVEL_INFO);
    }
    delay(100);
    Serial.flush();
    Serial.begin(baudrate, serial_config);
    delay(10);
    Serial.println();
  }
}

/*********************************************************************************************\
 * Wifi
\*********************************************************************************************/

#define WIFI_CONFIG_SEC   180  // seconds before restart
#define WIFI_CHECK_SEC    20   // seconds
#define WIFI_RETRY_SEC    30   // seconds

uint8_t wifi_counter;
uint8_t wifi_retry;
uint8_t wifi_status;
uint8_t wps_result;
uint8_t wifi_config_type = 0;
uint8_t wifi_config_counter = 0;

int WifiGetRssiAsQuality(int rssi)
{
  int quality = 0;

  if (rssi <= -100) {
    quality = 0;
  } else if (rssi >= -50) {
    quality = 100;
  } else {
    quality = 2 * (rssi + 100);
  }
  return quality;
}

boolean WifiConfigCounter()
{
  if (wifi_config_counter) {
    wifi_config_counter = WIFI_CONFIG_SEC;
  }
  return (wifi_config_counter);
}

extern "C" {
#include "user_interface.h"
}

void WifiWpsStatusCallback(wps_cb_status status);

void WifiWpsStatusCallback(wps_cb_status status)
{
/* from user_interface.h:
  enum wps_cb_status {
    WPS_CB_ST_SUCCESS = 0,
    WPS_CB_ST_FAILED,
    WPS_CB_ST_TIMEOUT,
    WPS_CB_ST_WEP,      // WPS failed because that WEP is not supported
    WPS_CB_ST_SCAN_ERR, // can not find the target WPS AP
  };
*/
  wps_result = status;
  if (WPS_CB_ST_SUCCESS == wps_result) {
    wifi_wps_disable();
  } else {
    snprintf_P(log_data, sizeof(log_data), PSTR(D_LOG_WIFI D_WPS_FAILED_WITH_STATUS " %d"), wps_result);
    AddLog(LOG_LEVEL_DEBUG);
    wifi_config_counter = 2;
  }
}

boolean WifiWpsConfigDone(void)
{
  return (!wps_result);
}

boolean WifiWpsConfigBegin(void)
{
  wps_result = 99;
  if (!wifi_wps_disable()) {
    return false;
  }
  if (!wifi_wps_enable(WPS_TYPE_PBC)) {
    return false;  // so far only WPS_TYPE_PBC is supported (SDK 2.0.0)
  }
  if (!wifi_set_wps_cb((wps_st_cb_t) &WifiWpsStatusCallback)) {
    return false;
  }
  if (!wifi_wps_start()) {
    return false;
  }
  return true;
}

void WifiConfig(uint8_t type)
{
  if (!wifi_config_type) {
    if (type >= WIFI_RETRY) {  // WIFI_RETRY and WIFI_WAIT
      return;
    }
#ifdef USE_EMULATION
    UdpDisconnect();
#endif  // USE_EMULATION
    WiFi.disconnect();        // Solve possible Wifi hangs
    wifi_config_type = type;
    wifi_config_counter = WIFI_CONFIG_SEC;   // Allow up to WIFI_CONFIG_SECS seconds for phone to provide ssid/pswd
    wifi_counter = wifi_config_counter +5;
    blinks = 1999;
    if (WIFI_RESTART == wifi_config_type) {
      restart_flag = 2;
    }
    else if (WIFI_SMARTCONFIG == wifi_config_type) {
      AddLog_P(LOG_LEVEL_INFO, S_LOG_WIFI, PSTR(D_WCFG_1_SMARTCONFIG D_ACTIVE_FOR_3_MINUTES));
      WiFi.beginSmartConfig();
    }
    else if (WIFI_WPSCONFIG == wifi_config_type) {
      if (WifiWpsConfigBegin()) {
        AddLog_P(LOG_LEVEL_INFO, S_LOG_WIFI, PSTR(D_WCFG_3_WPSCONFIG D_ACTIVE_FOR_3_MINUTES));
      } else {
        AddLog_P(LOG_LEVEL_INFO, S_LOG_WIFI, PSTR(D_WCFG_3_WPSCONFIG D_FAILED_TO_START));
        wifi_config_counter = 3;
      }
    }
#ifdef USE_WEBSERVER
    else if (WIFI_MANAGER == wifi_config_type) {
      AddLog_P(LOG_LEVEL_INFO, S_LOG_WIFI, PSTR(D_WCFG_2_WIFIMANAGER D_ACTIVE_FOR_3_MINUTES));
      WifiManagerBegin();
    }
#endif  // USE_WEBSERVER
  }
}

void WifiBegin(uint8_t flag)
{
  const char kWifiPhyMode[] = " BGN";

#ifdef USE_EMULATION
  UdpDisconnect();
#endif  // USE_EMULATION

#ifdef ARDUINO_ESP8266_RELEASE_2_3_0  // (!strncmp_P(ESP.getSdkVersion(),PSTR("1.5.3"),5))
  AddLog_P(LOG_LEVEL_DEBUG, S_LOG_WIFI, PSTR(D_PATCH_ISSUE_2186));
  WiFi.mode(WIFI_OFF);    // See https://github.com/esp8266/Arduino/issues/2186
#endif

  WiFi.disconnect();
  WiFi.mode(WIFI_STA);      // Disable AP mode
  if (Settings.sleep) {
    WiFi.setSleepMode(WIFI_LIGHT_SLEEP);  // Allow light sleep during idle times
  }
//  if (WiFi.getPhyMode() != WIFI_PHY_MODE_11N) {
//    WiFi.setPhyMode(WIFI_PHY_MODE_11N);
//  }
  if (!WiFi.getAutoConnect()) {
    WiFi.setAutoConnect(true);
  }
//  WiFi.setAutoReconnect(true);
  switch (flag) {
  case 0:  // AP1
  case 1:  // AP2
    Settings.sta_active = flag;
    break;
  case 2:  // Toggle
    Settings.sta_active ^= 1;
  }        // 3: Current AP
  if (0 == strlen(Settings.sta_ssid[1])) {
    Settings.sta_active = 0;
  }
  if (Settings.ip_address[0]) {
    WiFi.config(Settings.ip_address[0], Settings.ip_address[1], Settings.ip_address[2], Settings.ip_address[3]);  // Set static IP
  }
  WiFi.hostname(my_hostname);
  WiFi.begin(Settings.sta_ssid[Settings.sta_active], Settings.sta_pwd[Settings.sta_active]);
  snprintf_P(log_data, sizeof(log_data), PSTR(D_LOG_WIFI D_CONNECTING_TO_AP "%d %s " D_IN_MODE " 11%c " D_AS " %s..."),
    Settings.sta_active +1, Settings.sta_ssid[Settings.sta_active], kWifiPhyMode[WiFi.getPhyMode() & 0x3], my_hostname);
  AddLog(LOG_LEVEL_INFO);
}

void WifiCheckIp()
{
  if ((WL_CONNECTED == WiFi.status()) && (static_cast<uint32_t>(WiFi.localIP()) != 0)) {
    wifi_counter = WIFI_CHECK_SEC;
    wifi_retry = WIFI_RETRY_SEC;
    AddLog_P((wifi_status != WL_CONNECTED) ? LOG_LEVEL_INFO : LOG_LEVEL_DEBUG_MORE, S_LOG_WIFI, PSTR(D_CONNECTED));
    if (wifi_status != WL_CONNECTED) {
//      AddLog_P(LOG_LEVEL_INFO, PSTR("Wifi: Set IP addresses"));
      Settings.ip_address[1] = (uint32_t)WiFi.gatewayIP();
      Settings.ip_address[2] = (uint32_t)WiFi.subnetMask();
      Settings.ip_address[3] = (uint32_t)WiFi.dnsIP();
    }
    wifi_status = WL_CONNECTED;
  } else {
    wifi_status = WiFi.status();
    switch (wifi_status) {
      case WL_CONNECTED:
        AddLog_P(LOG_LEVEL_INFO, S_LOG_WIFI, PSTR(D_CONNECT_FAILED_NO_IP_ADDRESS));
        wifi_status = 0;
        wifi_retry = WIFI_RETRY_SEC;
        break;
      case WL_NO_SSID_AVAIL:
        AddLog_P(LOG_LEVEL_INFO, S_LOG_WIFI, PSTR(D_CONNECT_FAILED_AP_NOT_REACHED));
        if (WIFI_WAIT == Settings.sta_config) {
          wifi_retry = WIFI_RETRY_SEC;
        } else {
          if (wifi_retry > (WIFI_RETRY_SEC / 2)) {
            wifi_retry = WIFI_RETRY_SEC / 2;
          }
          else if (wifi_retry) {
            wifi_retry = 0;
          }
        }
        break;
      case WL_CONNECT_FAILED:
        AddLog_P(LOG_LEVEL_INFO, S_LOG_WIFI, PSTR(D_CONNECT_FAILED_WRONG_PASSWORD));
        if (wifi_retry > (WIFI_RETRY_SEC / 2)) {
          wifi_retry = WIFI_RETRY_SEC / 2;
        }
        else if (wifi_retry) {
          wifi_retry = 0;
        }
        break;
      default:  // WL_IDLE_STATUS and WL_DISCONNECTED
        if (!wifi_retry || ((WIFI_RETRY_SEC / 2) == wifi_retry)) {
          AddLog_P(LOG_LEVEL_INFO, S_LOG_WIFI, PSTR(D_CONNECT_FAILED_AP_TIMEOUT));
        } else {
          AddLog_P(LOG_LEVEL_DEBUG, S_LOG_WIFI, PSTR(D_ATTEMPTING_CONNECTION));
        }
    }
    if (wifi_retry) {
      if (WIFI_RETRY_SEC == wifi_retry) {
        WifiBegin(3);  // Select default SSID
      }
      if ((Settings.sta_config != WIFI_WAIT) && ((WIFI_RETRY_SEC / 2) == wifi_retry)) {
        WifiBegin(2);  // Select alternate SSID
      }
      wifi_counter = 1;
      wifi_retry--;
    } else {
      WifiConfig(Settings.sta_config);
      wifi_counter = 1;
      wifi_retry = WIFI_RETRY_SEC;
    }
  }
}

void WifiCheck(uint8_t param)
{
  wifi_counter--;
  switch (param) {
  case WIFI_SMARTCONFIG:
  case WIFI_MANAGER:
  case WIFI_WPSCONFIG:
    WifiConfig(param);
    break;
  default:
    if (wifi_config_counter) {
      wifi_config_counter--;
      wifi_counter = wifi_config_counter +5;
      if (wifi_config_counter) {
        if ((WIFI_SMARTCONFIG == wifi_config_type) && WiFi.smartConfigDone()) {
          wifi_config_counter = 0;
        }
        if ((WIFI_WPSCONFIG == wifi_config_type) && WifiWpsConfigDone()) {
          wifi_config_counter = 0;
        }
        if (!wifi_config_counter) {
          if (strlen(WiFi.SSID().c_str())) {
            strlcpy(Settings.sta_ssid[0], WiFi.SSID().c_str(), sizeof(Settings.sta_ssid[0]));
          }
          if (strlen(WiFi.psk().c_str())) {
            strlcpy(Settings.sta_pwd[0], WiFi.psk().c_str(), sizeof(Settings.sta_pwd[0]));
          }
          Settings.sta_active = 0;
          snprintf_P(log_data, sizeof(log_data), PSTR(D_LOG_WIFI D_WCFG_1_SMARTCONFIG D_CMND_SSID "1 %s, " D_CMND_PASSWORD "1 %s"), Settings.sta_ssid[0], Settings.sta_pwd[0]);
          AddLog(LOG_LEVEL_INFO);
        }
      }
      if (!wifi_config_counter) {
        if (WIFI_SMARTCONFIG == wifi_config_type) {
          WiFi.stopSmartConfig();
        }
        restart_flag = 2;
      }
    } else {
      if (wifi_counter <= 0) {
        AddLog_P(LOG_LEVEL_DEBUG_MORE, S_LOG_WIFI, PSTR(D_CHECKING_CONNECTION));
        wifi_counter = WIFI_CHECK_SEC;
        WifiCheckIp();
      }
      if ((WL_CONNECTED == WiFi.status()) && (static_cast<uint32_t>(WiFi.localIP()) != 0) && !wifi_config_type) {
#ifdef BE_MINIMAL
        if (1 == RtcSettings.ota_loader) {
          RtcSettings.ota_loader = 0;
          ota_state_flag = 3;
        }
#endif  // BE_MINIMAL
#ifdef USE_DISCOVERY
        if (!mdns_begun) {
          mdns_begun = MDNS.begin(my_hostname);
          snprintf_P(log_data, sizeof(log_data), PSTR(D_LOG_MDNS "%s"), (mdns_begun) ? D_INITIALIZED : D_FAILED);
          AddLog(LOG_LEVEL_INFO);
        }
#endif  // USE_DISCOVERY
#ifdef USE_WEBSERVER
        if (Settings.webserver) {
          StartWebserver(Settings.webserver, WiFi.localIP());
#ifdef USE_DISCOVERY
#ifdef WEBSERVER_ADVERTISE
          MDNS.addService("http", "tcp", 80);
#endif  // WEBSERVER_ADVERTISE
#endif  // USE_DISCOVERY
        } else {
          StopWebserver();
        }
#ifdef USE_EMULATION
        if (Settings.flag2.emulation) {
          UdpConnect();
        }
#endif  // USE_EMULATION
#endif  // USE_WEBSERVER
      } else {
#ifdef USE_EMULATION
        UdpDisconnect();
#endif  // USE_EMULATION
        mdns_begun = false;
      }
    }
  }
}

int WifiState()
{
  int state;

  if ((WL_CONNECTED == WiFi.status()) && (static_cast<uint32_t>(WiFi.localIP()) != 0)) {
    state = WIFI_RESTART;
  }
  if (wifi_config_type) {
    state = wifi_config_type;
  }
  return state;
}

void WifiConnect()
{
  WiFi.persistent(false);   // Solve possible wifi init errors
  wifi_status = 0;
  wifi_retry = WIFI_RETRY_SEC;
  wifi_counter = 1;
}

#ifdef USE_DISCOVERY
/*********************************************************************************************\
 * mDNS
\*********************************************************************************************/

#ifdef MQTT_HOST_DISCOVERY
boolean MdnsDiscoverMqttServer()
{
  if (!mdns_begun) {
    return false;
  }

  int n = MDNS.queryService("mqtt", "tcp");  // Search for mqtt service

  snprintf_P(log_data, sizeof(log_data), PSTR(D_LOG_MDNS D_QUERY_DONE " %d"), n);
  AddLog(LOG_LEVEL_INFO);

  if (n > 0) {
    // Note: current strategy is to get the first MQTT service (even when many are found)
    snprintf_P(Settings.mqtt_host, sizeof(Settings.mqtt_host), MDNS.IP(0).toString().c_str());
    Settings.mqtt_port = MDNS.port(0);

    snprintf_P(log_data, sizeof(log_data), PSTR(D_LOG_MDNS D_MQTT_SERVICE_FOUND " %s, " D_IP_ADDRESS " %s, " D_PORT " %d"),
      MDNS.hostname(0).c_str(), Settings.mqtt_host, Settings.mqtt_port);
    AddLog(LOG_LEVEL_INFO);
  }

  return n > 0;
}
#endif  // MQTT_HOST_DISCOVERY
#endif  // USE_DISCOVERY

/*********************************************************************************************\
 * Basic I2C routines
\*********************************************************************************************/

#ifdef USE_I2C
#define I2C_RETRY_COUNTER 3

uint32_t i2c_buffer = 0;

bool I2cValidRead(uint8_t addr, uint8_t reg, uint8_t size)
{
  byte x = I2C_RETRY_COUNTER;

  i2c_buffer = 0;
  do {
    Wire.beginTransmission(addr);                       // start transmission to device
    Wire.write(reg);                                    // sends register address to read from
    if (0 == Wire.endTransmission(false)) {             // Try to become I2C Master, send data and collect bytes, keep master status for next request...
      Wire.requestFrom((int)addr, (int)size);           // send data n-bytes read
      if (Wire.available() == size) {
        for (byte i = 0; i < size; i++) {
          i2c_buffer = i2c_buffer << 8 | Wire.read();   // receive DATA
        }
      }
    }
    x--;
  } while (Wire.endTransmission(true) != 0 && x != 0);  // end transmission
  return (x);
}

bool I2cValidRead8(uint8_t *data, uint8_t addr, uint8_t reg)
{
  bool status = I2cValidRead(addr, reg, 1);
  *data = (uint8_t)i2c_buffer;
  return status;
}

bool I2cValidRead16(uint16_t *data, uint8_t addr, uint8_t reg)
{
  bool status = I2cValidRead(addr, reg, 2);
  *data = (uint16_t)i2c_buffer;
  return status;
}

bool I2cValidReadS16(int16_t *data, uint8_t addr, uint8_t reg)
{
  bool status = I2cValidRead(addr, reg, 2);
  *data = (int16_t)i2c_buffer;
  return status;
}

bool I2cValidRead16LE(uint16_t *data, uint8_t addr, uint8_t reg)
{
  uint16_t ldata;
  bool status = I2cValidRead16(&ldata, addr, reg);
  *data = (ldata >> 8) | (ldata << 8);
  return status;
}

bool I2cValidReadS16_LE(int16_t *data, uint8_t addr, uint8_t reg)
{
  uint16_t ldata;
  bool status = I2cValidRead16LE(&ldata, addr, reg);
  *data = (int16_t)ldata;
  return status;
}

bool I2cValidRead24(int32_t *data, uint8_t addr, uint8_t reg)
{
  bool status = I2cValidRead(addr, reg, 3);
  *data = i2c_buffer;
  return status;
}

uint8_t I2cRead8(uint8_t addr, uint8_t reg)
{
  I2cValidRead(addr, reg, 1);
  return (uint8_t)i2c_buffer;
}

uint16_t I2cRead16(uint8_t addr, uint8_t reg)
{
  I2cValidRead(addr, reg, 2);
  return (uint16_t)i2c_buffer;
}

int16_t I2cReadS16(uint8_t addr, uint8_t reg)
{
  I2cValidRead(addr, reg, 2);
  return (int16_t)i2c_buffer;
}

uint16_t I2cRead16LE(uint8_t addr, uint8_t reg)
{
  I2cValidRead(addr, reg, 2);
  uint16_t temp = (uint16_t)i2c_buffer;
  return (temp >> 8) | (temp << 8);
}

int16_t I2cReadS16_LE(uint8_t addr, uint8_t reg)
{
  return (int16_t)I2cRead16LE(addr, reg);
}

int32_t I2cRead24(uint8_t addr, uint8_t reg)
{
  I2cValidRead(addr, reg, 3);
  return i2c_buffer;
}

bool I2cWrite(uint8_t addr, uint8_t reg, uint32_t val, uint8_t size)
{
  byte x = I2C_RETRY_COUNTER;

  do {
    Wire.beginTransmission((uint8_t)addr);              // start transmission to device
    Wire.write(reg);                                    // sends register address to write to
    uint8_t bytes = size;
    while (bytes--) {
      Wire.write((val >> (8 * bytes)) & 0xFF);          // write data
    }
    x--;
  } while (Wire.endTransmission(true) != 0 && x != 0);  // end transmission
  return (x);
}

bool I2cWrite8(uint8_t addr, uint8_t reg, uint16_t val)
{
   return I2cWrite(addr, reg, val, 1);
}

bool I2cWrite16(uint8_t addr, uint8_t reg, uint16_t val)
{
   return I2cWrite(addr, reg, val, 2);
}

void I2cScan(char *devs, unsigned int devs_len)
{
  byte error;
  byte address;
  byte any = 0;
  char tstr[10];

  snprintf_P(devs, devs_len, PSTR("{\"" D_CMND_I2CSCAN "\":\"" D_JSON_I2CSCAN_DEVICES_FOUND_AT));
  for (address = 1; address <= 127; address++) {
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    if (0 == error) {
      snprintf_P(tstr, sizeof(tstr), PSTR(" 0x%2x"), address);
      strncat(devs, tstr, devs_len);
      any = 1;
    }
    else if (4 == error) {
      snprintf_P(devs, devs_len, PSTR("{\"" D_CMND_I2CSCAN "\":\"" D_JSON_I2CSCAN_UNKNOWN_ERROR_AT " 0x%2x\"}"), address);
    }
  }
  if (any) {
    strncat(devs, "\"}", devs_len);
  } else {
    snprintf_P(devs, devs_len, PSTR("{\"" D_CMND_I2CSCAN "\":\"" D_JSON_I2CSCAN_NO_DEVICES_FOUND "\"}"));
  }
}

boolean I2cDevice(byte addr)
{
  for (byte address = 1; address <= 127; address++) {
    Wire.beginTransmission(address);
    if (!Wire.endTransmission() && (address == addr)) {
      return true;
    }
  }
  return false;
}
#endif  // USE_I2C

/*********************************************************************************************\
 * Real Time Clock
 *
 * Sources: Time by Michael Margolis and Paul Stoffregen (https://github.com/PaulStoffregen/Time)
 *          Timezone by Jack Christensen (https://github.com/JChristensen/Timezone)
\*********************************************************************************************/

extern "C" {
#include "sntp.h"
}

#define SECS_PER_MIN  ((uint32_t)(60UL))
#define SECS_PER_HOUR ((uint32_t)(3600UL))
#define SECS_PER_DAY  ((uint32_t)(SECS_PER_HOUR * 24UL))
#define LEAP_YEAR(Y)  (((1970+Y)>0) && !((1970+Y)%4) && (((1970+Y)%100) || !((1970+Y)%400)))

Ticker TickerRtc;

static const uint8_t kDaysInMonth[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 }; // API starts months from 1, this array starts from 0
static const char kMonthNamesEnglish[] = "JanFebMarAprMayJunJulAugSepOctNovDec";

uint32_t utc_time = 0;
uint32_t local_time = 0;
uint32_t daylight_saving_time = 0;
uint32_t standard_time = 0;
uint32_t ntp_time = 0;
uint32_t midnight = 1451602800;
uint8_t  midnight_now = 0;
uint8_t  ntp_sync_minute = 0;

String GetBuildDateAndTime()
{
  // "2017-03-07T11:08:02" - ISO8601:2004
  char bdt[21];
  char *p;
  char mdate[] = __DATE__;  // "Mar  7 2017"
  char *smonth = mdate;
  int day = 0;
  int year = 0;

  // sscanf(mdate, "%s %d %d", bdt, &day, &year);  // Not implemented in 2.3.0 and probably too much code
  byte i = 0;
  for (char *str = strtok_r(mdate, " ", &p); str && i < 3; str = strtok_r(NULL, " ", &p)) {
    switch (i++) {
    case 0:  // Month
      smonth = str;
      break;
    case 1:  // Day
      day = atoi(str);
      break;
    case 2:  // Year
      year = atoi(str);
    }
  }
  int month = (strstr(kMonthNamesEnglish, smonth) -kMonthNamesEnglish) /3 +1;
  snprintf_P(bdt, sizeof(bdt), PSTR("%d" D_YEAR_MONTH_SEPARATOR "%02d" D_MONTH_DAY_SEPARATOR "%02d" D_DATE_TIME_SEPARATOR "%s"), year, month, day, __TIME__);
  return String(bdt);
}

String GetDateAndTime()
{
  // "2017-03-07T11:08:02" - ISO8601:2004
  char dt[21];

  snprintf_P(dt, sizeof(dt), PSTR("%04d" D_YEAR_MONTH_SEPARATOR "%02d" D_MONTH_DAY_SEPARATOR "%02d" D_DATE_TIME_SEPARATOR "%02d" D_HOUR_MINUTE_SEPARATOR "%02d" D_MINUTE_SECOND_SEPARATOR "%02d"),
    RtcTime.year, RtcTime.month, RtcTime.day_of_month, RtcTime.hour, RtcTime.minute, RtcTime.second);
  return String(dt);
}

String GetUtcDateAndTime()
{
  // "2017-03-07T11:08:02" - ISO8601:2004
  char dt[21];

  TIME_T tmpTime;
  BreakTime(utc_time, tmpTime);
  tmpTime.year += 1970;

  snprintf_P(dt, sizeof(dt), PSTR("%04d" D_YEAR_MONTH_SEPARATOR "%02d" D_MONTH_DAY_SEPARATOR "%02d" D_DATE_TIME_SEPARATOR "%02d" D_HOUR_MINUTE_SEPARATOR "%02d" D_MINUTE_SECOND_SEPARATOR "%02d"),
    tmpTime.year, tmpTime.month, tmpTime.day_of_month, tmpTime.hour, tmpTime.minute, tmpTime.second);
  return String(dt);
}

String GetUptime()
{
  char dt[16];

  TIME_T ut;
  BreakTime(uptime, ut);

  // "P128DT14H35M44S" - ISO8601:2004 - https://en.wikipedia.org/wiki/ISO_8601 Durations
//  snprintf_P(dt, sizeof(dt), PSTR("P%dDT%02dH%02dM%02dS"), ut.days, ut.hour, ut.minute, ut.second);

  // "128 14:35:44" - OpenVMS
  // "128T14:35:44" - Tasmota
  snprintf_P(dt, sizeof(dt), PSTR("%d" D_DATE_TIME_SEPARATOR "%02d" D_HOUR_MINUTE_SEPARATOR "%02d" D_MINUTE_SECOND_SEPARATOR "%02d"), ut.days, ut.hour, ut.minute, ut.second);
  return String(dt);
}

void BreakTime(uint32_t time_input, TIME_T &tm)
{
// break the given time_input into time components
// this is a more compact version of the C library localtime function
// note that year is offset from 1970 !!!

  uint8_t year;
  uint8_t month;
  uint8_t month_length;
  uint32_t time;
  unsigned long days;

  time = time_input;
  tm.second = time % 60;
  time /= 60;                // now it is minutes
  tm.minute = time % 60;
  time /= 60;                // now it is hours
  tm.hour = time % 24;
  time /= 24;                // now it is days
  tm.days = time;
  tm.day_of_week = ((time + 4) % 7) + 1;  // Sunday is day 1

  year = 0;
  days = 0;
  while((unsigned)(days += (LEAP_YEAR(year) ? 366 : 365)) <= time) {
    year++;
  }
  tm.year = year;            // year is offset from 1970

  days -= LEAP_YEAR(year) ? 366 : 365;
  time -= days;              // now it is days in this year, starting at 0
  tm.day_of_year = time;

  days = 0;
  month = 0;
  month_length = 0;
  for (month = 0; month < 12; month++) {
    if (1 == month) { // february
      if (LEAP_YEAR(year)) {
        month_length = 29;
      } else {
        month_length = 28;
      }
    } else {
      month_length = kDaysInMonth[month];
    }

    if (time >= month_length) {
      time -= month_length;
    } else {
      break;
    }
  }
  strlcpy(tm.name_of_month, kMonthNames + (month *3), 4);
  tm.month = month + 1;      // jan is month 1
  tm.day_of_month = time + 1;         // day of month
  tm.valid = (time_input > 1451602800);  // 2016-01-01
}

uint32_t MakeTime(TIME_T &tm)
{
// assemble time elements into time_t
// note year argument is offset from 1970

  int i;
  uint32_t seconds;

  // seconds from 1970 till 1 jan 00:00:00 of the given year
  seconds = tm.year * (SECS_PER_DAY * 365);
  for (i = 0; i < tm.year; i++) {
    if (LEAP_YEAR(i)) {
      seconds +=  SECS_PER_DAY;   // add extra days for leap years
    }
  }

  // add days for this year, months start from 1
  for (i = 1; i < tm.month; i++) {
    if ((2 == i) && LEAP_YEAR(tm.year)) {
      seconds += SECS_PER_DAY * 29;
    } else {
      seconds += SECS_PER_DAY * kDaysInMonth[i-1];  // monthDay array starts from 0
    }
  }
  seconds+= (tm.day_of_month - 1) * SECS_PER_DAY;
  seconds+= tm.hour * SECS_PER_HOUR;
  seconds+= tm.minute * SECS_PER_MIN;
  seconds+= tm.second;
  return seconds;
}

uint32_t RuleToTime(TimeChangeRule r, int yr)
{
  TIME_T tm;
  uint32_t t;
  uint8_t m;
  uint8_t w;                // temp copies of r.month and r.week

  m = r.month;
  w = r.week;
  if (0 == w) {             // Last week = 0
    if (++m > 12) {         // for "Last", go to the next month
      m = 1;
      yr++;
    }
    w = 1;                  // and treat as first week of next month, subtract 7 days later
  }

  tm.hour = r.hour;
  tm.minute = 0;
  tm.second = 0;
  tm.day_of_month = 1;
  tm.month = m;
  tm.year = yr - 1970;
  t = MakeTime(tm);         // First day of the month, or first day of next month for "Last" rules
  BreakTime(t, tm);
  t += (7 * (w - 1) + (r.dow - tm.day_of_week + 7) % 7) * SECS_PER_DAY;
  if (0 == r.week) {
    t -= 7 * SECS_PER_DAY;  // back up a week if this is a "Last" rule
  }
  return t;
}

String GetTime(int type)
{
  char stime[25];   // Skip newline

  uint32_t time = utc_time;
  if (1 == type) time = local_time;
  if (2 == type) time = daylight_saving_time;
  if (3 == type) time = standard_time;
  snprintf_P(stime, sizeof(stime), sntp_get_real_time(time));
  return String(stime);
}

uint32_t LocalTime()
{
  return local_time;
}

uint32_t Midnight()
{
  return midnight;
}

boolean MidnightNow()
{
  boolean mnflg = midnight_now;
  if (mnflg) midnight_now = 0;
  return mnflg;
}

void RtcSecond()
{
  uint32_t stdoffset;
  uint32_t dstoffset;
  TIME_T tmpTime;

  if ((ntp_sync_minute > 59) && (3 == RtcTime.minute)) ntp_sync_minute = 1;                // If sync prepare for a new cycle
  uint8_t offset = (uptime < 30) ? RtcTime.second : (((ESP.getChipId() & 0xF) * 3) + 3) ;  // First try ASAP to sync. If fails try once every 60 seconds based on chip id
  if ((WL_CONNECTED == WiFi.status()) && (offset == RtcTime.second) && ((RtcTime.year < 2016) || (ntp_sync_minute == RtcTime.minute))) {
    ntp_time = sntp_get_current_timestamp();
    if (ntp_time) {
      utc_time = ntp_time;
      ntp_sync_minute = 60;  // Sync so block further requests
      BreakTime(utc_time, tmpTime);
      RtcTime.year = tmpTime.year + 1970;
      daylight_saving_time = RuleToTime(DaylightSavingTime, RtcTime.year);
      standard_time = RuleToTime(StandardTime, RtcTime.year);
      snprintf_P(log_data, sizeof(log_data), PSTR(D_LOG_APPLICATION "(" D_UTC_TIME ") %s, (" D_DST_TIME ") %s, (" D_STD_TIME ") %s"),
        GetTime(0).c_str(), GetTime(2).c_str(), GetTime(3).c_str());
      AddLog(LOG_LEVEL_DEBUG);
    } else {
      ntp_sync_minute++;  // Try again in next minute
    }
  }
  utc_time++;
  local_time = utc_time;
  if (local_time > 1451602800) {  // 2016-01-01
    if (99 == Settings.timezone) {
      if (DaylightSavingTime.hemis) {
        dstoffset = StandardTime.offset * SECS_PER_MIN;  // Southern hemisphere
        stdoffset = DaylightSavingTime.offset * SECS_PER_MIN;
      } else {
        dstoffset = DaylightSavingTime.offset * SECS_PER_MIN;  // Northern hemisphere
        stdoffset = StandardTime.offset * SECS_PER_MIN;
      }
      if ((utc_time >= (daylight_saving_time - stdoffset)) && (utc_time < (standard_time - dstoffset))) {
        local_time += dstoffset;  // Daylight Saving Time
      } else {
        local_time += stdoffset;  // Standard Time
      }
    } else {
      local_time += Settings.timezone * SECS_PER_HOUR;
    }
  }
  BreakTime(local_time, RtcTime);
  if (!RtcTime.hour && !RtcTime.minute && !RtcTime.second && RtcTime.valid) {
    midnight = local_time;
    midnight_now = 1;
  }
  RtcTime.year += 1970;
}

void RtcInit()
{
  sntp_setservername(0, Settings.ntp_server[0]);
  sntp_setservername(1, Settings.ntp_server[1]);
  sntp_setservername(2, Settings.ntp_server[2]);
  sntp_stop();
  sntp_set_timezone(0);      // UTC time
  sntp_init();
  utc_time = 0;
  BreakTime(utc_time, RtcTime);
  TickerRtc.attach(1, RtcSecond);
}

#ifndef USE_ADC_VCC
/*********************************************************************************************\
 * ADC support
\*********************************************************************************************/

void AdcShow(boolean json)
{
  uint16_t analog = 0;
  for (byte i = 0; i < 32; i++) {
    analog += analogRead(A0);
    delay(1);
  }
  analog >>= 5;

  if (json) {
    snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s,\"" D_JSON_ANALOG_INPUT "0\":%d"), mqtt_data, analog);
#ifdef USE_WEBSERVER
  } else {
    snprintf_P(mqtt_data, sizeof(mqtt_data), HTTP_SNS_ANALOG, mqtt_data, "", 0, analog);
#endif  // USE_WEBSERVER
  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

#define XSNS_02

boolean Xsns02(byte function)
{
  boolean result = false;

  if (pin[GPIO_ADC0] < 99) {
    switch (function) {
      case FUNC_JSON_APPEND:
        AdcShow(1);
        break;
#ifdef USE_WEBSERVER
      case FUNC_WEB_APPEND:
        AdcShow(0);
        break;
#endif  // USE_WEBSERVER
    }
  }
  return result;
}
#endif  // USE_ADC_VCC

/*********************************************************************************************\
 * Syslog
 *
 * Example:
 *   snprintf_P(log_data, sizeof(log_data), PSTR(D_LOG_LOG "Any value %d"), value);
 *   AddLog(LOG_LEVEL_DEBUG);
 *
\*********************************************************************************************/

#ifdef USE_WEBSERVER
void GetLog(byte idx, char** entry_pp, size_t* len_p)
{
  char* entry_p = NULL;
  size_t len = 0;

  if (idx) {
    char* it = web_log;
    do {
      byte cur_idx = *it;
      it++;
      size_t tmp = strchrspn(it, '\1');
      tmp++;                             // Skip terminating '\1'
      if (cur_idx == idx) {              // Found the requested entry
        len = tmp;
        entry_p = it;
        break;
      }
      it += tmp;
    } while (it < web_log + WEB_LOG_SIZE && *it != '\0');
  }
  *entry_pp = entry_p;
  *len_p = len;
}
#endif  // USE_WEBSERVER

void Syslog()
{
  // Destroys log_data
  char syslog_preamble[64];  // Hostname + Id

  if ((static_cast<uint32_t>(syslog_host_addr) == 0) || ((millis() - syslog_host_refresh) > 60000)) {
    WiFi.hostByName(Settings.syslog_host, syslog_host_addr);
    syslog_host_refresh = millis();
  }
  if (PortUdp.beginPacket(syslog_host_addr, Settings.syslog_port)) {
    snprintf_P(syslog_preamble, sizeof(syslog_preamble), PSTR("%s ESP-"), my_hostname);
    memmove(log_data + strlen(syslog_preamble), log_data, sizeof(log_data) - strlen(syslog_preamble));
    log_data[sizeof(log_data) -1] = '\0';
    memcpy(log_data, syslog_preamble, strlen(syslog_preamble));
    PortUdp.write(log_data);
    PortUdp.endPacket();
  } else {
    syslog_level = 0;
    syslog_timer = SYSLOG_TIMER;
    snprintf_P(log_data, sizeof(log_data), PSTR(D_LOG_APPLICATION D_SYSLOG_HOST_NOT_FOUND ". " D_RETRY_IN " %d " D_UNIT_SECOND), SYSLOG_TIMER);
    AddLog(LOG_LEVEL_INFO);
  }
}

void AddLog(byte loglevel)
{
  char mxtime[10];  // "13:45:21 "

  snprintf_P(mxtime, sizeof(mxtime), PSTR("%02d" D_HOUR_MINUTE_SEPARATOR "%02d" D_MINUTE_SECOND_SEPARATOR "%02d "), RtcTime.hour, RtcTime.minute, RtcTime.second);

  if (loglevel <= seriallog_level) {
    Serial.printf("%s%s\n", mxtime, log_data);
  }
#ifdef USE_WEBSERVER
  if (Settings.webserver && (loglevel <= Settings.weblog_level)) {
    // Delimited, zero-terminated buffer of log lines.
    // Each entry has this format: [index][log data]['\1']
    if (!web_log_index) web_log_index++;   // Index 0 is not allowed as it is the end of char string
    while (web_log_index == web_log[0] ||  // If log already holds the next index, remove it
           strlen(web_log) + strlen(log_data) + 13 > WEB_LOG_SIZE)  // 13 = web_log_index + mxtime + '\1' + '\0'
    {
      char* it = web_log;
      it++;                                // Skip web_log_index
      it += strchrspn(it, '\1');           // Skip log line
      it++;                                // Skip delimiting "\1"
      memmove(web_log, it, WEB_LOG_SIZE -(it-web_log));  // Move buffer forward to remove oldest log line
    }
    snprintf_P(web_log, sizeof(web_log), PSTR("%s%c%s%s\1"), web_log, web_log_index++, mxtime, log_data);
    if (!web_log_index) web_log_index++;   // Index 0 is not allowed as it is the end of char string
  }
#endif  // USE_WEBSERVER
  if ((WL_CONNECTED == WiFi.status()) && (loglevel <= syslog_level)) {
    Syslog();
  }
}

void AddLog_P(byte loglevel, const char *formatP)
{
  snprintf_P(log_data, sizeof(log_data), formatP);
  AddLog(loglevel);
}

void AddLog_P(byte loglevel, const char *formatP, const char *formatP2)
{
  char message[100];

  snprintf_P(log_data, sizeof(log_data), formatP);
  snprintf_P(message, sizeof(message), formatP2);
  strncat(log_data, message, sizeof(log_data));
  AddLog(loglevel);
}

void AddLogSerial(byte loglevel)
{
  snprintf_P(log_data, sizeof(log_data), PSTR(D_LOG_SERIAL D_RECEIVED));
  for (byte i = 0; i < serial_in_byte_counter; i++) {
    snprintf_P(log_data, sizeof(log_data), PSTR("%s %02X"), log_data, serial_in_buffer[i]);
  }
  AddLog(loglevel);
}

/*********************************************************************************************\
 *
\*********************************************************************************************/
